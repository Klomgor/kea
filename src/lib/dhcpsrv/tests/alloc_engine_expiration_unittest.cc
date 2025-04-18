// Copyright (C) 2015-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>
#include <dhcp/duid.h>
#include <dhcp/option_data_types.h>
#include <dhcp_ddns/ncr_msg.h>
#include <dhcpsrv/testutils/alloc_engine_utils.h>
#include <dhcpsrv/testutils/test_utils.h>
#include <hooks/hooks_manager.h>
#include <stats/stats_mgr.h>
#include <gtest/gtest.h>
#include <boost/static_assert.hpp>
#include <functional>
#include <iomanip>
#include <sstream>
#include <time.h>
#include <unistd.h>
#include <vector>

using namespace std;
using namespace isc;
using namespace isc::asiolink;
using namespace isc::data;
using namespace isc::dhcp;
using namespace isc::dhcp::test;
using namespace isc::dhcp_ddns;
using namespace isc::hooks;
using namespace isc::stats;
namespace ph = std::placeholders;

namespace {

/// @brief Number of leases to be initialized for each test.
///
/// This value is expected by some of the tests to be multiple
/// of 10.
const unsigned int TEST_LEASES_NUM = 100;

/// @brief Structure wrapping a lower limit within the collection
/// of leases.
///
/// We're using this structure rather than a size_t value directly
/// to make API of the test fixture class more readable, i.e. the
/// struct name indicates the purpose of the value being held.
struct LowerBound {
    /// @brief Constructor.
    ///
    /// @param lower_bound Integer value wrapped by the structure.
    explicit LowerBound(const size_t lower_bound)
        : lower_bound_(lower_bound) { };

    /// @brief Operator returning the size_t value wrapped.
    operator size_t() const {
        return (lower_bound_);
    }

    /// @brief Value wrapped in the structure.
    size_t lower_bound_;
};

/// @brief Structure wrapping an upper limit within the collection
/// of leases.
///
/// We're using this structure rather than a size_t value directly
/// to make API of the test fixture class more readable, i.e. the
/// struct name indicates the purpose of the value being held.
struct UpperBound {
    /// @brief Constructor.
    ///
    /// @param lower_bound Integer value wrapped by the structure.
    explicit UpperBound(const size_t upper_bound)
        : upper_bound_(upper_bound) { };

    /// @brief Operator returning the size_t value wrapped.
    operator size_t() const {
        return (upper_bound_);
    }

    /// @brief Value wrapped in the structure.
    size_t upper_bound_;
};

/// @brief List holding addresses for executed callouts.
std::list<IOAddress> callouts_;

/// @brief Callout argument name for expired lease.
std::string callout_argument_name("lease4");

/// @brief Base test fixture class for the lease reclamation routines in the
/// @c AllocEngine.
///
/// This class implements infrastructure for testing leases reclamation
/// routines. The lease reclamation routine has the following
/// characteristic:
/// - it processes multiple leases,
/// - leases are processed in certain order,
/// - number of processed leases may be limited by the parameters,
/// - maximum duration of the lease reclamation routine may be limited,
/// - reclaimed leases may be marked as reclaimed or deleted,
/// - DNS records for some of the leases must be removed when the lease
///   is reclaimed and DNS updates are enabled,
/// - hooks must be invoked (if installed) for each reclaimed lease
/// - statistics must be updated to increase the number of reclaimed
///   leases and decrease the number of allocated leases
///
/// The typical test requires many leases to be initialized and stored
/// in the lease database for the test. The test fixture class creates
/// these leases upon construction. It is possible to modify these
/// leases to test various properties of the lease reclamation routine
/// as listed above. For example: some of the leases may be marked
/// as expired or hostname may be cleared for some of the leases to
/// check that DNS updates are not generated for them.
///
/// The tests are built around the
/// @c ExpirationAllocEngineTest::testLeases methods. These methods
/// verify that the certain operations have been performed by the
/// lease reclamation routine on selected leases. The leases for which
/// certain conditions should be met are selected using the "index
/// algorithms". Various index algorithms are implemented in the
/// test fixture class as static functions and the algorithm is
/// selected by passing function pointer to the @c testLeases method.
///
/// Examples of index algorithms are:
/// - evenLeaseIndex(index) - picks even index numbers,
/// - oddLeaseIndex(index) - picks odd index numbers,
/// - allLeasesIndexes(index) - picks all index number.
///
/// For example, the test may use the @c evenLeaseIndex algorithm
/// to mark leases with even indexes as expired and then test whether
/// leases with even indexes have been successfully reclaimed.
///
/// The "lease algorithm" verifies if the given lease fulfills the
/// specific conditions after reclamation. These are the examples of
/// the lease algorithms:
/// - leaseExists - lease still exists in the database,
/// - leaseDoesntExist - lease removed from the database,
/// - leaseReclaimed - lease exists but has reclaimed status,
/// - leaseNotReclaimed - lease exists and is not in the reclaimed status,
/// - leaseDeclined - lease exists and is in declined state,
/// - dnsUpdateGeneratedForLease - DNS updates generated for lease,
/// - dnsUpdateNotGeneratedForLease - DNS updates not generated for lease
///
/// The combination of index algorithm and lease algorithm allows for
/// verifying that the whole sets of leases in the lease database fulfill
/// certain conditions. For example, it is possible to verify that
/// after lease reclamation leases with even indexes have state set to
/// "expired-reclaimed".
///
/// See @c ExpirationAllocEngineTest::testLeases for further details.
///
/// @todo These tests should be extended to cover the following cases:
/// - declined leases - declined leases expire and should be removed
///   from the lease database by the lease reclamation routine. See
///   ticket #3976.
template<typename LeasePtrType>
class ExpirationAllocEngineTest : public ::testing::Test {
public:

    /// @brief Type definition for the lease algorithm.
    typedef std::function<bool (const LeasePtrType)> LeaseAlgorithmFun;
    /// @brief type definition for the lease index algorithm.
    typedef std::function<bool (const size_t)> IndexAlgorithmFun;

    /// @brief Constructor.
    ///
    /// Clears configuration, creates new lease manager and allocation engine
    /// instances.
    ExpirationAllocEngineTest(const std::string& lease_mgr_params) {
        // Clear configuration.
        CfgMgr::instance().clear();
        D2ClientConfigPtr cfg(new D2ClientConfig());
        CfgMgr::instance().setD2ClientConfig(cfg);

        // Remove all statistics.
        StatsMgr::instance().resetAll();

        // Set the 'reclaimed-leases' statistics to '0'. This statistics
        // is used by some tests to verify that the leases reclamation
        // routine has been called.
        StatsMgr::instance().setValue("reclaimed-leases",
                                      static_cast<int64_t>(0));

        // Create lease manager.
        LeaseMgrFactory::create(lease_mgr_params);

        // Create allocation engine instance.
        engine_.reset(new AllocEngine(100));
    }

    /// @brief Destructor
    ///
    /// Stops D2 client (if running), clears configuration and removes
    /// an instance of the lease manager.
    virtual ~ExpirationAllocEngineTest() {
        // Stop D2 client if running and remove all queued name change
        // requests.
        D2ClientMgr& mgr = CfgMgr::instance().getD2ClientMgr();
        if (mgr.amSending()) {
            mgr.stopSender();
            mgr.clearQueue();
            mgr.stop();
        }

        // Clear configuration.
        CfgMgr::instance().clear();
        D2ClientConfigPtr cfg(new D2ClientConfig());
        CfgMgr::instance().setD2ClientConfig(cfg);

        // Remove all statistics.
        StatsMgr::instance().resetAll();

        // Kill lease manager.
        LeaseMgrFactory::destroy();

        // Remove callouts executed.
        callouts_.clear();

        // Unload libraries.
        bool status = HooksManager::unloadLibraries();
        if (!status) {
            cerr << "(fixture dtor) unloadLibraries failed" << endl;
        }
    }

    /// @brief Starts D2 client.
    void enableDDNS() const {
        // Start DDNS and assign no-op error handler.
        D2ClientMgr& mgr = CfgMgr::instance().getD2ClientMgr();
        D2ClientConfigPtr cfg(new D2ClientConfig());
        cfg->enableUpdates(true);
        mgr.setD2ClientConfig(cfg);
        mgr.startSender(std::bind(&ExpirationAllocEngineTest::d2ErrorHandler, ph::_1, ph::_2));
    }

    /// @brief No-op error handler for the D2 client.
    static void d2ErrorHandler(const dhcp_ddns::NameChangeSender::Result,
                               dhcp_ddns::NameChangeRequestPtr&) {
    }

    /// @brief Marks a lease as expired.
    ///
    /// @param lease_index Lease index. Must be between 0 and
    /// @c TEST_LEASES_NUM.
    /// @param secs Offset of the expiration time since now. For example
    /// a value of 2 would set the lease expiration time to 2 seconds ago.
    void expire(const uint16_t lease_index, const time_t secs) {
        ASSERT_GT(leases_.size(), lease_index);
        // We set the expiration time indirectly by modifying the cltt value.
        leases_[lease_index]->cltt_ = time(0) - secs -
            leases_[lease_index]->valid_lft_;
        ASSERT_NO_THROW(updateLease(lease_index));
    }

    /// @brief Changes the owner of a lease.
    ///
    /// This method changes the owner of the lease. It must be implemented in
    /// the derived classes to update the unique identifier(s) in the lease to
    /// point to a different client.
    ///
    /// @param lease_index Lease index. Must be between 0 and
    /// @c TEST_LEASES_NUM.
    virtual void transferOwnership(const uint16_t lease_index) = 0;

    /// @brief Marks lease as expired-reclaimed.
    ///
    /// @param lease_index Lease index. Must be between 0 and
    /// @c TEST_LEASES_NUM.
    /// @param secs Offset of the expiration time since now. For example
    /// a value of 2 would set the lease expiration time to 2 seconds ago.
    void reclaim(const uint16_t lease_index, const time_t secs) {
        ASSERT_GT(leases_.size(), lease_index);
        leases_[lease_index]->cltt_ = time(0) - secs -
            leases_[lease_index]->valid_lft_;
        leases_[lease_index]->state_ = Lease::STATE_EXPIRED_RECLAIMED;
        ASSERT_NO_THROW(updateLease(lease_index));
    }

    /// @brief Declines specified lease
    ///
    /// Sets specified lease to declined state and sets its probation-period.
    /// @param lease_index Index of the lease.
    /// @param probation_time value of probation period to be set (in seconds)
    void decline(const uint16_t lease_index, const time_t probation_time) {
        ASSERT_GT(leases_.size(), lease_index);
        leases_[lease_index]->decline(probation_time);
        ASSERT_NO_THROW(updateLease(lease_index));
    }

    /// @brief Mark lease as registered.
    ///
    /// @param lease_index Lease index. Must be between 0 and
    /// @c TEST_LEASES_NUM.
    void registered(const uint16_t lease_index) {
        ASSERT_GT(leases_.size(), lease_index);
        ASSERT_EQ(Lease::TYPE_NA, leases_[lease_index]->type_);
        leases_[lease_index]->state_ = Lease::STATE_REGISTERED;
        ASSERT_NO_THROW(updateLease(lease_index));

        // Update the stats.
        StatsMgr& stats_mgr = StatsMgr::instance();
        auto subnet_id = leases_[lease_index]->subnet_id_;
        stats_mgr.addValue(stats_mgr.generateName("subnet", subnet_id,
                                                  "assigned-nas"),
                           int64_t(-1));
        stats_mgr.addValue(stats_mgr.generateName("subnet", subnet_id,
                                                  "registered-nas"),
                           int64_t(1));
    }

    /// @brief Updates lease in the lease database.
    ///
    /// @param lease_index Index of the lease.
    virtual void updateLease(const unsigned int lease_index) = 0;

    /// @brief Retrieves lease from the database.
    ///
    /// @param lease_index Index of the lease.
    virtual LeasePtrType getLease(const unsigned int lease_index) const = 0;

    /// @brief Sets subnet id for a lease.
    ///
    /// It also updates statistics of assigned leases in the stats manager.
    ///
    /// @param lease_index Lease index.
    /// @param id New subnet id.
    virtual void setSubnetId(const uint16_t lease_index, const SubnetID& id) = 0;

    /// @brief Wrapper method running lease reclamation routine.
    ///
    /// @param max_leases Maximum number of leases to be reclaimed.
    /// @param timeout Maximum amount of time that the reclamation routine
    /// may be processing expired leases, expressed in seconds.
    /// @param remove_lease A boolean value indicating if the lease should
    /// be removed when it is reclaimed (if true) or it should be left in the
    /// database in the "expired-reclaimed" state (if false).
    virtual void reclaimExpiredLeases(const size_t max_leases,
                                      const uint16_t timeout,
                                      const bool remove_lease) = 0;

    /// @brief Wrapper method for removing expired-reclaimed leases.
    ///
    /// @param secs The minimum amount of time, expressed in seconds,
    /// for the lease to be left in the "expired-reclaimed" state
    /// before it can be removed.
    virtual void deleteExpiredReclaimedLeases(const uint32_t secs) = 0;

    /// @brief Test selected leases using the specified algorithms.
    ///
    /// This function picks leases from the range of 0 thru
    /// @c TEST_LEASES_NUM and selects the ones to be verified using the
    /// specified index algorithm. Selected leases are tested using
    /// the specified lease algorithm.
    ///
    /// @param lease_algorithm Pointer to the lease algorithm function.
    /// @param index_algorithm Pointer to the index algorithm function.
    bool testLeases(const LeaseAlgorithmFun& lease_algorithm,
                    const IndexAlgorithmFun& index_algorithm) const {
        // No limits are specified, so test all leases in the range of
        // 0 .. TEST_LEASES_NUM.
        return (testLeases(lease_algorithm, index_algorithm, LowerBound(0),
                           UpperBound(TEST_LEASES_NUM)));
    }


    /// @brief Test selected leases using the specified algorithms.
    ///
    /// This function picks leases from the range of @c lower_bound
    /// thru @c upper_bound and selects the ones to be verified using the
    /// specified index algorithm. Selected leases are tested using the
    /// specified lease algorithm.
    ///
    /// @param lease_algorithm Pointer to the lease algorithm function.
    /// @param index_algorithm Pointer to the index algorithm function.
    /// @param lower_bound First index in the range.
    /// @param upper_bound Last + 1 index in the range.
    bool testLeases(const LeaseAlgorithmFun& lease_algorithm,
                    const IndexAlgorithmFun& index_algorithm,
                    const LowerBound& lower_bound,
                    const UpperBound& upper_bound) const {
        // Select leases between the lower_bound and upper_bound.
        for (size_t i = lower_bound; i < upper_bound; ++i) {
            // Get the lease from the lease database.
            LeasePtrType lease = getLease(i);
            // index_algorithm(i) checks if the lease should be checked.
            // If so, check if the lease_algorithm indicates that the
            // lease fulfills a given condition, e.g. is present in the
            // database. If not, return false.
            if (index_algorithm(i) && !lease_algorithm(lease)) {
                return (false);
            }
        }
        // All leases checked, so return true.
        return (true);
    }

    /// @brief Index algorithm selecting even indexes.
    ///
    /// @param index Lease index.
    /// @return true if index is an even number.
    static bool evenLeaseIndex(const size_t index) {
        return (index % 2 == 0);
    }

    /// @brief Index algorithm selecting odd indexes.
    ///
    /// @param index Lease index.
    /// @return true if index is an odd number.
    static bool oddLeaseIndex(const size_t index) {
        return (!evenLeaseIndex(index));
    }

    /// @brief Index algorithm selecting all indexes.
    ///
    /// @param index Lease index.
    /// @return true if the index is in the range of [0 .. TEST_LEASES_NUM).
    static bool allLeaseIndexes(const size_t index) {
        return (index < TEST_LEASES_NUM);
    }

    /// @brief Lease algorithm checking if lease exists.
    ///
    /// @param lease Pointer to lease.
    /// @return true if lease pointer is non-null.
    static bool leaseExists(const LeasePtrType& lease) {
        return (static_cast<bool>(lease));
    }

    /// @brief Lease algorithm checking if lease doesn't exist.
    ///
    /// @param lease Pointer to lease.
    /// @return true if lease pointer is null.
    static bool leaseDoesntExist(const LeasePtrType& lease) {
        return (static_cast<bool>(!lease));
    }

    /// @brief Lease algorithm checking if lease state is expired-reclaimed.
    ///
    /// This algorithm also checks that the FQDN information has been removed
    /// from the lease.
    ///
    /// @param lease Pointer to lease.
    /// @return true if lease state is "expired-reclaimed" and the FQDN
    /// information has been removed from the lease.
    static bool leaseReclaimed(const LeasePtrType& lease) {
        return (lease && lease->stateExpiredReclaimed() &&
                lease->hostname_.empty() && !lease->fqdn_fwd_ &&
                !lease->fqdn_rev_ && !lease->getContext());
    }

    /// @brief Lease algorithm checking if lease state is Declined.
    ///
    /// @param lease Pointer to lease.
    /// @return true if lease state is "declined".
    static bool leaseDeclined(const LeasePtrType& lease) {
        return (lease && lease->stateDeclined());
    }

    /// @brief Lease algorithm checking if lease state is not
    /// expired-reclaimed.
    ///
    /// @param lease Pointer to lease.
    /// @return true if lease state is not "expired-reclaimed".
    static bool leaseNotReclaimed(const LeasePtrType& lease) {
        return (lease && !lease->stateExpiredReclaimed());
    }

    /// @brief Lease algorithm checking if removal name change request
    /// has been generated for lease.
    ///
    /// @param lease Pointer to lease.
    /// @return true if NCR has been generated for the lease.
    static bool dnsUpdateGeneratedForLease(const LeasePtrType& lease) {
        try {
            return (static_cast<bool>(getNCRForLease(lease)));

        } catch (...) {
            // If error occurred, treat it as no match.
            return (false);
        }
    }

    /// @brief Lease algorithm checking if removal name change request
    /// hasn't been generated for lease.
    ///
    /// @param lease Pointer to lease.
    /// @return true if NCR has not been generated for the lease.
    static bool dnsUpdateNotGeneratedForLease(const LeasePtrType& lease) {
        try {
            // Iterate over the generated name change requests and try
            // to find the match with our lease (using IP address). If
            D2ClientMgr& mgr = CfgMgr::instance().getD2ClientMgr();
            for (size_t i = 0; i < mgr.getQueueSize(); ++i) {
                const NameChangeRequestPtr& ncr = mgr.peekAt(i);
                // If match found, we treat it as if the test fails
                // because we expected no NCR.
                if (ncr->getIpAddress() == lease->addr_.toText()) {
                    return (false);
                }
            }
        } catch (...) {
            return (false);
        }

        // No match found, so we're good.
        return (true);
    }

    /// @brief Lease algorithm checking if callout has been executed for
    /// the expired lease.
    ///
    /// @param lease Pointer to lease.
    /// @return true if callout has been executed for the lease.
    static bool leaseCalloutExecuted(const LeasePtrType& lease) {
        return (std::find(callouts_.begin(), callouts_.end(), lease->addr_) !=
                callouts_.end());
    }

    /// @brief Lease algorithm checking if callout hasn't been executed for
    /// the expired lease.
    ///
    /// @param lease Pointer to lease.
    /// @return true if callout hasn't been executed for the lease.
    static bool leaseCalloutNotExecuted(const LeasePtrType& lease) {
        return (!leaseCalloutExecuted(lease));
    }

    /// @brief Implements "lease{4,6}_expire" callout.
    ///
    /// @param callout_handle Callout handle.
    /// @return Zero.
    static int leaseExpireCallout(CalloutHandle& callout_handle) {
        LeasePtrType lease;
        callout_handle.getArgument(callout_argument_name, lease);
        bool remove_lease = true;
        callout_handle.getArgument("remove_lease", remove_lease);

        // Check if the remove_lease is set to false and assume that the callout
        // has been successfully executed if it is. This is mainly to test
        // that the lease reclamation routine sets this value at all.
        if (!remove_lease) {
            callouts_.push_back(lease->addr_);
        }

        return (0);
    }

    /// @brief Implements "lease{4,6}_expire callout returning skip flag.
    ///
    /// @param callout_handle Callout handle.
    /// @return Zero.
    static int leaseExpireWithSkipCallout(CalloutHandle& callout_handle) {
        leaseExpireCallout(callout_handle);
        callout_handle.setStatus(CalloutHandle::NEXT_STEP_SKIP);

        return (0);
    }

    /// @brief Implements "lease{4,6}_expire callout, which lasts at least
    /// 40ms.
    ///
    /// This callout is useful to test scenarios where the reclamation of the
    /// lease needs to take a known amount of time. If the callout is installed
    /// it will take at least 40ms for each lease. It is then possible to calculate
    /// the approximate time that the reclamation of all leases would take and
    /// test that the timeouts for the leases' reclamation work as expected.
    ///
    /// The value of 40ms is relatively high, but it has been selected to
    /// mitigate the problems with usleep on some virtual machines. On those
    /// machines the wakeup from usleep may take significant amount of time,
    /// i.e. usually around 10ms. Thus, the sleep time should be considerably
    /// higher than this delay.
    ///
    /// @param callout_handle Callout handle.
    /// @return Zero.
    static int leaseExpireWithDelayCallout(CalloutHandle& callout_handle) {
        leaseExpireCallout(callout_handle);
        // Delay the return from the callout by 40ms.
        usleep(40000);

        return (0);
    }

    /// @brief Returns removal name change request from the D2 client queue.
    ///
    /// @param lease Pointer to the lease to be matched with NCR.
    ///
    /// @return null pointer if no match found.
    static NameChangeRequestPtr getNCRForLease(const LeasePtrType& lease) {
        // Iterate over the generated name change requests and try
        // to find the match with our lease (using IP address). If
        D2ClientMgr& mgr = CfgMgr::instance().getD2ClientMgr();
        for (size_t i = 0; i < mgr.getQueueSize(); ++i) {
            const NameChangeRequestPtr& ncr = mgr.peekAt(i);
            // If match found, return true.
            if ((ncr->getIpAddress() == lease->addr_.toText()) &&
                (ncr->getChangeType() == CHG_REMOVE)) {
                return (ncr);
            }
        }
        return (NameChangeRequestPtr());
    }

    /// @brief Returns index of the lease from the address.
    ///
    /// This method assumes that leases are ordered from the smallest to
    /// the highest address, e.g. 10.0.0.0, 10.0.0.1, 10.0.0.2 etc. The
    /// last two bytes can be used to extract index.
    ///
    /// @param address Address.
    ///
    /// @return index
    static uint16_t getLeaseIndexFromAddress(const IOAddress& address) {
        std::vector<uint8_t> bytes = address.toBytes();
        std::vector<uint8_t>::reverse_iterator bytes_it = bytes.rbegin();
        uint16_t index = static_cast<uint16_t>(*bytes_it) |
            (static_cast<uint16_t>(*(bytes_it + 1)) << 8);
        return (index);
    }

    /// @brief Generates hostname for lease index.
    ///
    /// Generates hostname in the form of "hostXXXX.example.org", where
    /// XXXX is a lease index.
    ///
    /// @param index Lease index.
    ///
    /// @return Generated hostname.
    static std::string generateHostnameForLeaseIndex(const uint16_t index) {
        std::ostringstream hostname_s;
        hostname_s << "host" << std::setw(4) << std::setfill('0')
                   << index << ".example.org";
        return (hostname_s.str());
    }

    /// @brief Test that leases can be reclaimed without being removed.
    void testReclaimExpiredLeasesUpdateState() {
        for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
            // Mark leases with even indexes as expired.
            if (evenLeaseIndex(i)) {
                // The higher the index, the more expired the lease.
                expire(i, 10 + i);
            }
        }

        // Run leases reclamation routine on all leases. This should result
        // in setting "expired-reclaimed" state for all leases with even
        // indexes.
        ASSERT_NO_THROW(reclaimExpiredLeases(0, 0, false));

        // Leases with even indexes should be marked as reclaimed.
        EXPECT_TRUE(testLeases(&leaseReclaimed, &evenLeaseIndex));
        // Leases with odd indexes shouldn't be marked as reclaimed.
        EXPECT_TRUE(testLeases(&leaseNotReclaimed, &oddLeaseIndex));
    }

    /// @brief Test that the leases may be reclaimed by being deleted.
    void testReclaimExpiredLeasesDelete() {
        for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
            // Mark leases with even indexes as expired.
            if (evenLeaseIndex(i)) {
                // The higher the index, the more expired the lease.
                expire(i, 10 + i);
            }
        }

        // Run leases reclamation routine on all leases. This should result
        // in removal of all leases with even indexes.
        ASSERT_NO_THROW(reclaimExpiredLeases(0, 0, true));

        // Leases with odd indexes should be retained and their state
        // shouldn't be "expired-reclaimed".
        EXPECT_TRUE(testLeases(&leaseNotReclaimed, &oddLeaseIndex));
        // Leases with even indexes should have been removed.
        EXPECT_TRUE(testLeases(&leaseDoesntExist, &evenLeaseIndex));
    }

    /// @brief Test that it is possible to specify the limit for the number
    /// of reclaimed leases.
    void testReclaimExpiredLeasesLimit() {
        for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
            // Mark all leases as expired. The higher the index the less
            // expired the lease.
            expire(i, 1000 - i);
        }

        // We will be performing lease reclamation on lease groups of 10.
        // Hence, it is convenient if the number of test leases is a
        // multiple of 10.
        const size_t reclamation_group_size = 10;
        BOOST_STATIC_ASSERT(TEST_LEASES_NUM % reclamation_group_size == 0);

        // Leases will be reclaimed in groups of 10.
        for (unsigned int i = reclamation_group_size; i < TEST_LEASES_NUM;
             i += reclamation_group_size) {

            // Reclaim 10 most expired leases out of TEST_LEASES_NUM. Since
            // leases are ordered from the most expired to the least expired
            // this should reclaim leases between 0 and 9, then 10 and 19 etc.
            ASSERT_NO_THROW(reclaimExpiredLeases(reclamation_group_size,
                                                 0, false));

            // Check that leases having all indexes between 0 and 9, 19, 29 etc.
            // have been reclaimed.
            EXPECT_TRUE(testLeases(&leaseReclaimed, &allLeaseIndexes,
                                   LowerBound(0), UpperBound(i)))
                << "check failed for i = " << i;

            // Check that all remaining leases haven't been reclaimed.
            EXPECT_TRUE(testLeases(&leaseNotReclaimed, &allLeaseIndexes,
                                   LowerBound(i), UpperBound(TEST_LEASES_NUM)))
                << "check failed for i = " << i;
        }
    }

    /// @brief Test that DNS updates are generated for the leases for which
    /// the DNS records exist.
    void testReclaimExpiredLeasesWithDDNS() {
        // DNS must be started for the D2 client to accept NCRs.
        ASSERT_NO_THROW(enableDDNS());

        for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
            // Expire all leases with even indexes.
            if (evenLeaseIndex(i)) {
                // The higher the index, the more expired the lease.
                expire(i, 10 + i);
            }
        }

        // Reclaim all expired leases.
        ASSERT_NO_THROW(reclaimExpiredLeases(0, 0, false));

        // Leases with odd indexes shouldn't be reclaimed.
        EXPECT_TRUE(testLeases(&leaseNotReclaimed, &oddLeaseIndex));
        // Leases with even indexes should be reclaimed.
        EXPECT_TRUE(testLeases(&leaseReclaimed, &evenLeaseIndex));
        // DNS updates (removal NCRs) should be generated for leases with even
        // indexes.
        EXPECT_TRUE(testLeases(&dnsUpdateGeneratedForLease, &evenLeaseIndex));
        // DNS updates (removal NCRs) shouldn't be generated for leases with
        // odd indexes.
        EXPECT_TRUE(testLeases(&dnsUpdateNotGeneratedForLease, &oddLeaseIndex));
    }

    /// @brief Test that DNS updates are only generated for the reclaimed
    /// leases (not for all leases with hostname stored).
    void testReclaimExpiredLeasesWithDDNSAndLimit() {
        // DNS must be started for the D2 client to accept NCRs.
        ASSERT_NO_THROW(enableDDNS());

        for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
            // Expire only leases with even indexes.
            if (evenLeaseIndex(i)) {
                // The higher the index, the more expired the lease.
                expire(i, 10 + i);
            }
        }

        const size_t reclamation_group_size = 10;
        BOOST_STATIC_ASSERT(TEST_LEASES_NUM % reclamation_group_size == 0);

        // Leases will be reclaimed in groups of 10
        for (unsigned int i = 10; i < TEST_LEASES_NUM;  i += reclamation_group_size) {
            // Reclaim 10 most expired leases. Note that the leases with the
            // higher index are more expired. For example, if the
            // TEST_LEASES_NUM is equal to 100, the most expired lease will
            // be 98, then 96, 94 etc.
            ASSERT_NO_THROW(reclaimExpiredLeases(reclamation_group_size, 0,
                                                 false));

            // After the first iteration the lower bound is 80, because there
            // will be 10 the most expired leases in this group: 80, 82, 84,
            // 86, 88, 90, 92, 94, 96, 98. For subsequent iterations
            // accordingly.
            int reclaimed_lower_bound = TEST_LEASES_NUM - 2 * i;
            // At some point the lower bound will hit the negative value, which
            // must be corrected to 0.
            if (reclaimed_lower_bound < 0) {
                reclaimed_lower_bound = 0;
            }

            // Leases between the lower bound calculated above and the upper
            // bound of all leases, and having even indexes should have been
            // reclaimed.
            EXPECT_TRUE(testLeases(&leaseReclaimed, &evenLeaseIndex,
                                   LowerBound(reclaimed_lower_bound),
                                   UpperBound(TEST_LEASES_NUM)))
                << "check failed for i = " << i;

            // For the same leases we should have generated DNS updates
            // (removal NCRs).
            EXPECT_TRUE(testLeases(&dnsUpdateGeneratedForLease, &evenLeaseIndex,
                                   LowerBound(reclaimed_lower_bound),
                                   UpperBound(TEST_LEASES_NUM)))
                << "check failed for i = " << i;

            // Leases with odd indexes (falling between the reclaimed ones)
            // shouldn't have been reclaimed, because they are not expired.
            EXPECT_TRUE(testLeases(&leaseNotReclaimed, &oddLeaseIndex,
                                   LowerBound(reclaimed_lower_bound),
                                   UpperBound(TEST_LEASES_NUM)))
                << "check failed for i = " << i;

            EXPECT_TRUE(testLeases(&dnsUpdateNotGeneratedForLease,
                                   &oddLeaseIndex,
                                   LowerBound(reclaimed_lower_bound),
                                   UpperBound(TEST_LEASES_NUM)))
                << "check failed for i = " << i;


            // At early stages of iterations, there should be continuous
            // group of leases (expired and not expired) which haven't been
            // reclaimed.
            if (reclaimed_lower_bound > 0) {
                EXPECT_TRUE(testLeases(&leaseNotReclaimed, &allLeaseIndexes,
                                       LowerBound(0),
                                       UpperBound(reclaimed_lower_bound)))
                    << "check failed for i = " << i;

                EXPECT_TRUE(testLeases(&dnsUpdateNotGeneratedForLease,
                                       &oddLeaseIndex,
                                       LowerBound(0),
                                       UpperBound(reclaimed_lower_bound)));
            }
        }
    }

    /// @brief This test verifies that reclamation routine continues if the
    /// DNS update has failed for some leases.
    void testReclaimExpiredLeasesInvalidHostname() {
        // DNS must be started for the D2 client to accept NCRs.
        ASSERT_NO_THROW(enableDDNS());

        for (size_t i = 0; i < TEST_LEASES_NUM; ++i) {
            // Generate invalid hostname for every other lease.
            if (evenLeaseIndex(i)) {
                // Hostname with two consecutive dots is invalid and may result
                // in exception if the reclamation routine doesn't protect
                // against such exceptions.
                std::ostringstream hostname_s;
                hostname_s << "invalid-host" << i << "..example.com";
                leases_[i]->hostname_ = hostname_s.str();
                ASSERT_NO_THROW(updateLease(i));
            }
            // Every lease is expired.
            expire(i, 10 + i);
        }

        // Although we know that some hostnames are broken we don't want the
        // reclamation process to break when it finds a broken record.
        // It should rather continue to process other leases.
        ASSERT_NO_THROW(reclaimExpiredLeases(0, 0, false));

        // All leases should have been reclaimed. Broken DNS entry doesn't
        // warrant that we don't reclaim the lease.
        EXPECT_TRUE(testLeases(&leaseReclaimed, &allLeaseIndexes));
        // The routine should not generate DNS updates for the leases with
        // broken hostname.
        EXPECT_TRUE(testLeases(&dnsUpdateNotGeneratedForLease,
                               &evenLeaseIndex));
        // But it should generate DNS updates for the leases with the correct
        // hostname.
        EXPECT_TRUE(testLeases(&dnsUpdateGeneratedForLease, &oddLeaseIndex));
    }

    /// @brief This test verifies that callouts are executed for each expired
    /// lease when installed.
    void testReclaimExpiredLeasesHooks() {
        for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
            if (evenLeaseIndex(i)) {
                expire(i, 1000 - i);
            }
        }

        HookLibsCollection libraries; // no libraries at this time
        HooksManager::loadLibraries(libraries);

        // Install a callout: lease4_expire or lease6_expire.
        std::ostringstream callout_name;
        callout_name << callout_argument_name << "_expire";
        EXPECT_NO_THROW(HooksManager::preCalloutsLibraryHandle().registerCallout(
                        callout_name.str(), leaseExpireCallout));

        ASSERT_NO_THROW(reclaimExpiredLeases(0, 0, false));

        // Callouts should be executed for leases with even indexes and these
        // leases should be reclaimed.
        EXPECT_TRUE(testLeases(&leaseCalloutExecuted, &evenLeaseIndex));
        EXPECT_TRUE(testLeases(&leaseReclaimed, &evenLeaseIndex));
        // Callouts should not be executed for leases with odd indexes and these
        // leases should not be reclaimed.
        EXPECT_TRUE(testLeases(&leaseCalloutNotExecuted, &oddLeaseIndex));
        EXPECT_TRUE(testLeases(&leaseNotReclaimed, &oddLeaseIndex));
    }

    /// @brief This test verifies that callouts are executed for each expired
    /// lease and that the lease is not reclaimed when skip flag is set.
    void testReclaimExpiredLeasesHooksWithSkip() {
        for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
            if (evenLeaseIndex(i)) {
                expire(i, 1000 - i);
            }
        }

        HookLibsCollection libraries; // no libraries at this time
        HooksManager::loadLibraries(libraries);

        // Install a callout: lease4_expire or lease6_expire.
        std::ostringstream callout_name;
        callout_name << callout_argument_name << "_expire";
        EXPECT_NO_THROW(HooksManager::preCalloutsLibraryHandle().registerCallout(
                        callout_name.str(), leaseExpireWithSkipCallout));

        ASSERT_NO_THROW(reclaimExpiredLeases(0, 0, false));

        // Callouts should have been executed for leases with even indexes.
        EXPECT_TRUE(testLeases(&leaseCalloutExecuted, &evenLeaseIndex));
        // Callouts should not be executed for leases with odd indexes.
        EXPECT_TRUE(testLeases(&leaseCalloutNotExecuted, &oddLeaseIndex));
        // Leases shouldn't be reclaimed because the callout sets the
        // skip flag for each of them.
        EXPECT_TRUE(testLeases(&leaseNotReclaimed, &allLeaseIndexes));
    }

    /// @brief This test verifies that it is possible to set the timeout for
    /// the execution of the lease reclamation routine.
    void testReclaimExpiredLeasesTimeout(const uint16_t timeout) {
        // Leases are segregated from the most expired to the least expired.
        for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
            expire(i, 2000 - i);
        }

        HookLibsCollection libraries;
        HooksManager::loadLibraries(libraries);

        // Install a callout: lease4_expire or lease6_expire. Each callout
        // takes at least 40ms to run (it uses usleep).
        std::ostringstream callout_name;
        callout_name << callout_argument_name << "_expire";
        EXPECT_NO_THROW(HooksManager::preCalloutsLibraryHandle().registerCallout(
                        callout_name.str(), leaseExpireWithDelayCallout));

        // Reclaim leases with timeout.
        ASSERT_NO_THROW(reclaimExpiredLeases(0, timeout, false));

        // We reclaimed at most (timeout / 40ms) leases.
        const uint16_t theoretical_reclaimed = static_cast<uint16_t>(timeout / 40);

        // The actual number of leases reclaimed is likely to be lower than
        // the theoretical number. For low theoretical number the adjusted
        // number is always 1. For higher number, it will be 10 less than the
        // theoretical number.
        const uint16_t adjusted_reclaimed = (theoretical_reclaimed > 10 ?
                                             theoretical_reclaimed - 10 : 1);

        EXPECT_TRUE(testLeases(&leaseCalloutExecuted, &allLeaseIndexes,
                               LowerBound(0), UpperBound(adjusted_reclaimed)));
        EXPECT_TRUE(testLeases(&leaseReclaimed, &allLeaseIndexes,
                               LowerBound(0), UpperBound(adjusted_reclaimed)));
        EXPECT_TRUE(testLeases(&leaseCalloutNotExecuted, &allLeaseIndexes,
                               LowerBound(theoretical_reclaimed + 1),
                               UpperBound(TEST_LEASES_NUM)));
        EXPECT_TRUE(testLeases(&leaseNotReclaimed, &allLeaseIndexes,
                               LowerBound(theoretical_reclaimed + 1),
                               UpperBound(TEST_LEASES_NUM)));
    }

    /// @brief This test verifies that expired-reclaimed leases are removed
    /// from the lease database.
    void testDeleteExpiredReclaimedLeases() {
        for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
            // Mark leases with even indexes as expired.
            if (evenLeaseIndex(i)) {
                // The higher the index, the more expired the lease.
                reclaim(i, 10 + i);
            }
        }

        // Run leases reclamation routine on all leases. This should result
        // in removal of all leases with even indexes.
        ASSERT_NO_THROW(deleteExpiredReclaimedLeases(10));

        // Leases with odd indexes shouldn't be removed from the database.
        EXPECT_TRUE(testLeases(&leaseExists, &oddLeaseIndex));
        // Leases with even indexes should have been removed.
        EXPECT_TRUE(testLeases(&leaseDoesntExist, &evenLeaseIndex));
    }

    /// @brief Test that declined expired leases can be removed.
    ///
    /// This method allows controlling remove_leases parameter when calling
    /// @ref AllocEngine::reclaimExpiredLeases4 or
    /// @ref AllocEngine::reclaimExpiredLeases6. This should not matter, as
    /// the address affinity doesn't make sense for declined leases (they don't
    /// have any useful information in them anymore), so AllocEngine should
    /// remove them all the time.
    ///
    /// @param remove see description above
    void testReclaimDeclined(bool remove) {
        for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {

            // Mark leases with even indexes as expired.
            if (evenLeaseIndex(i)) {

                // Mark lease as declined with 100 seconds of probation-period
                // (i.e. lease is supposed to be off limits for 100 seconds)
                decline(i, 100);

                // The higher the index, the more expired the lease.
                expire(i, 10 + i);
            }
        }

        // Run leases reclamation routine on all leases. This should result
        // in removing all leases with status = declined, i.e. all
        // even leases should be gone.
        ASSERT_NO_THROW(reclaimExpiredLeases(0, 0, remove));

        // Leases with even indexes should not exist in the DB
        EXPECT_TRUE(testLeases(&leaseDoesntExist, &evenLeaseIndex));
    }

    /// @brief Test that appropriate statistics are updated when
    ///        declined expired leases are processed by AllocEngine.
    ///
    /// This method works for both v4 and v6. Just make sure the correct
    /// statistic name is passed. This is the name of the assigned addresses,
    /// that is expected to be decreased once the reclamation procedure
    /// is complete.
    ///
    /// @param stat_name name of the statistic for assigned addresses statistic
    ///        ("assigned-addresses" for both v4 and "assigned-nas" for v6)
    void testReclaimDeclinedStats(const std::string& stat_name) {

        // Leases by default all belong to subnet_id_ = 1. Let's count the
        // number of declined leases.
        int subnet1_cnt = 0;
        int subnet2_cnt = 0;

        // Let's move all leases to declined,expired state.
        for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {

            // Move the lease to declined state
            decline(i, 100);

            // And expire it, so it will be reclaimed
            expire(i, 10 + 1);

            // Move every other lease to subnet-id = 2.
            if (evenLeaseIndex(i)) {
                subnet1_cnt++;
            } else {
                subnet2_cnt++;
                setSubnetId(i, 2);
            }
        }

        StatsMgr& stats_mgr = StatsMgr::instance();

        // Let's set the global statistic. Values are arbitrary and can
        // be used to easily detect whether a given stat was decreased or
        // increased. They are sufficiently high compared to number of leases
        // to avoid any chances of going into negative.
        stats_mgr.setValue("declined-addresses", static_cast<int64_t>(1000));

        // Let's set global the counter for reclaimed declined addresses.
        stats_mgr.setValue("reclaimed-declined-addresses",
                           static_cast<int64_t>(2000));

        // And those subnet specific as well
        stats_mgr.setValue(stats_mgr.generateName("subnet", 1,
                           stat_name), int64_t(1000));
        stats_mgr.setValue(stats_mgr.generateName("subnet", 2,
                           stat_name), int64_t(2000));

        stats_mgr.setValue(stats_mgr.generateName("subnet", 1,
                           "reclaimed-declined-addresses"), int64_t(10000));
        stats_mgr.setValue(stats_mgr.generateName("subnet", 2,
                           "reclaimed-declined-addresses"), int64_t(20000));

        stats_mgr.setValue(stats_mgr.generateName("subnet", 1,
                           "declined-addresses"), int64_t(100));
        stats_mgr.setValue(stats_mgr.generateName("subnet", 2,
                           "declined-addresses"), int64_t(200));

        // Run leases reclamation routine on all leases. This should result
        // in removal of all leases with even indexes.
        ASSERT_NO_THROW(reclaimExpiredLeases(0, 0, true));

        // Declined-addresses should be decreased from its initial value (1000)
        // for both recovered addresses from subnet1 and subnet2.
        testStatistics("declined-addresses", 1000 - subnet1_cnt - subnet2_cnt);

        // The code should bump up global counter for reclaimed declined
        // addresses.
        testStatistics("reclaimed-declined-addresses", 2000 + subnet1_cnt + subnet2_cnt);

        // subnet[X].assigned-addresses should go down. Between the time
        // of DHCPDECLINE(v4)/DECLINE(v6) reception and declined expired lease
        // reclamation, we count this address as assigned-addresses. We decrease
        // assigned-addresses(v4)/assigned-nas(v6) when we reclaim the lease,
        // not when the packet is received. For explanation, see Duplicate
        // Addresses (DHCPDECLINE support) (v4) or Duplicate Addresses (DECLINE
        // support) sections in the User's Guide or a comment in
        // Dhcpv4Srv::declineLease or Dhcpv6Srv::declineLease.
        testStatistics(stat_name, 1000 - subnet1_cnt, 1);
        testStatistics(stat_name, 2000 - subnet2_cnt, 2);

        testStatistics("declined-addresses", 100 - subnet1_cnt, 1);
        testStatistics("declined-addresses", 200 - subnet2_cnt, 2);

        // subnet[X].reclaimed-declined-addresses should go up in each subnet
        testStatistics("reclaimed-declined-addresses", 10000 + subnet1_cnt, 1);
        testStatistics("reclaimed-declined-addresses", 20000 + subnet1_cnt, 2);
    }

    /// @brief Collection of leases created at construction time.
    std::vector<LeasePtrType> leases_;

    /// @brief Allocation engine instance used for tests.
    AllocEnginePtr engine_;
};



/// @brief Specialization of the @c ExpirationAllocEngineTest class to test
/// reclamation of the IPv6 leases.
class ExpirationAllocEngine6Test : public ExpirationAllocEngineTest<Lease6Ptr> {
public:

    /// @brief Class constructor.
    ///
    /// This constructor initializes @c TEST_LEASES_NUM leases and
    /// stores them in the lease manager.
    ExpirationAllocEngine6Test();

    /// @brief Virtual destructor.
    ///
    /// Clears up static fields that may be modified by hooks.
    virtual ~ExpirationAllocEngine6Test() {
        callout_lease_.reset();
        callout_name_ = string("");
    }

    /// @brief Creates collection of leases for a test.
    ///
    /// It is called internally at the construction time.
    void createLeases();

    /// @brief Updates lease in the lease database.
    ///
    /// @param lease_index Index of the lease.
    virtual void updateLease(const unsigned int lease_index) {
        LeaseMgrFactory::instance().updateLease6(leases_[lease_index]);
    }

    /// @brief Changes the owner of a lease.
    ///
    /// This method changes the owner of the lease by modifying the DUID.
    ///
    /// @param lease_index Lease index. Must be between 0 and
    /// @c TEST_LEASES_NUM.
    virtual void transferOwnership(const uint16_t lease_index);

    /// @brief Sets subnet id for a lease.
    ///
    /// It also updates statistics of assigned leases in the stats manager.
    ///
    /// @param lease_index Lease index.
    /// @param id New subnet id.
    virtual void setSubnetId(const uint16_t lease_index, const SubnetID& id);

    /// @brief Sets type of a lease.
    ///
    /// It also updates statistics of assigned leases in the stats manager.
    ///
    /// @param lease_index Lease index.
    /// @param lease_type Lease type.
    void setLeaseType(const uint16_t lease_index, const Lease6::Type& lease_type);

    /// @brief Retrieves lease from the database.
    ///
    /// @param lease_index Index of the lease.
    virtual Lease6Ptr getLease(const unsigned int lease_index) const {
        return (LeaseMgrFactory::instance().getLease6(leases_[lease_index]->type_,
                                                      leases_[lease_index]->addr_));
    }

    /// @brief Wrapper method running lease reclamation routine.
    ///
    /// @param max_leases Maximum number of leases to be reclaimed.
    /// @param timeout Maximum amount of time that the reclamation routine
    /// may be processing expired leases, expressed in seconds.
    /// @param remove_lease A boolean value indicating if the lease should
    /// be removed when it is reclaimed (if true) or it should be left in the
    /// database in the "expired-reclaimed" state (if false).
    virtual void reclaimExpiredLeases(const size_t max_leases,
                                      const uint16_t timeout,
                                      const bool remove_lease) {
        engine_->reclaimExpiredLeases6(max_leases, timeout, remove_lease);
    }

    /// @brief Wrapper method for removing expired-reclaimed leases.
    ///
    /// @param secs The minimum amount of time, expressed in seconds,
    /// for the lease to be left in the "expired-reclaimed" state
    /// before it can be removed.
    virtual void deleteExpiredReclaimedLeases(const uint32_t secs) {
        engine_->deleteExpiredReclaimedLeases6(secs);
    }

    /// @brief Test that statistics is updated when leases are reclaimed.
    void testReclaimExpiredLeasesStats();

    /// @brief Test that expired leases are reclaimed before they are allocated.
    ///
    /// @param msg_type DHCPv6 message type.
    /// @param use_reclaimed Boolean parameter indicating if the leases
    /// stored in the lease database should be marked as 'expired-reclaimed'
    /// or 'expired'. This allows to test whether the allocation engine can
    /// determine that the lease has been reclaimed already and not reclaim
    /// it the second time.
    void testReclaimReusedLeases(const uint16_t msg_type, const bool use_reclaimed);

    /// @brief Callout for lease6_recover
    ///
    /// This callout stores passed parameter into static fields.
    ///
    /// @param callout_handle will be provided by hooks framework
    /// @return always 0
    static int lease6RecoverCallout(CalloutHandle& callout_handle) {
        callout_name_ = "lease6_recover";

        callout_handle.getArgument("lease6", callout_lease_);

        return (0);
    }

    /// @brief Callout for lease6_recover that sets status to SKIP
    ///
    /// This callout stores passed parameter into static fields.
    ///
    /// @param callout_handle will be provided by hooks framework
    /// @return always 0
    static int lease6RecoverSkipCallout(CalloutHandle& callout_handle) {
        // Set the next step status to SKIP
        callout_handle.setStatus(CalloutHandle::NEXT_STEP_SKIP);

        return (lease6RecoverCallout(callout_handle));
    }

    /// @brief Test install a hook callout, recovers declined leases
    ///
    /// This test: declines, then expires half of the leases, then
    /// installs a callout on lease6_recover hook, then reclaims
    /// expired leases and checks that:
    /// - the callout was indeed called
    /// - the parameter (lease6) was indeed passed as expected
    /// - checks that the leases are removed (skip=false) or
    /// - checks that the leases are still there (skip=true)
    /// @param skip should the callout set the next step status to skip?
    void
    testReclaimDeclinedHook(bool skip);

    /// @brief Test that the registered leases may be reclaimed.
    void testReclaimExpiredLeasesRegistered() {
        for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
            // Mark all leases as registered.
            registered(i);
            // Mark leases with even indexes as expired.
            if (evenLeaseIndex(i)) {
                // The higher the index, the more expired the lease.
                expire(i, 10 + i);
            }
        }

        // Run leases reclamation routine on all leases. This should result
        // in removal of all leases with even indexes even if remove_lease
        // flag is false.
        ASSERT_NO_THROW(reclaimExpiredLeases(0, 0, false));

        // Leases with odd indexes should be retained and their state
        // shouldn't be "expired-reclaimed".
        EXPECT_TRUE(testLeases(&leaseNotReclaimed, &oddLeaseIndex));
        // Leases with even indexes should have been removed.
        EXPECT_TRUE(testLeases(&leaseDoesntExist, &evenLeaseIndex));
    }

    /// @brief Test that the registered leases may be reclaimed and
    /// DNS updates are generated.
    void testReclaimExpiredLeasesRegisteredWithDDNS() {
        // DNS must be started for the D2 client to accept NCRs.
        ASSERT_NO_THROW(enableDDNS());

        for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
            // Mark all leases as registered.
            registered(i);
            // Mark leases with even indexes as expired.
            if (evenLeaseIndex(i)) {
                // The higher the index, the more expired the lease.
                expire(i, 10 + i);
            }
        }

        // Reclaim all expired leases.
        ASSERT_NO_THROW(reclaimExpiredLeases(0, 0, false));

        // Leases with odd indexes should be retained and their state
        // shouldn't be "expired-reclaimed".
        EXPECT_TRUE(testLeases(&leaseNotReclaimed, &oddLeaseIndex));
        // Leases with even indexes should have been removed.
        EXPECT_TRUE(testLeases(&leaseDoesntExist, &evenLeaseIndex));

        // Can't use testLeases on a deleted lease so do it the hard way.
        for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
            bool ncr = dnsUpdateGeneratedForLease(leases_[i]);
            EXPECT_EQ(ncr, evenLeaseIndex(i));
        }
    }

    /// @brief Test that the registered leases may be reclaimed and
    /// statistics is updated.
    void testReclaimExpiredLeasesRegisteredStats();

    /// The following parameters will be written by a callout
    static std::string callout_name_; ///< Stores callout name
    static Lease6Ptr callout_lease_;  ///< Stores callout parameter
};

std::string ExpirationAllocEngine6Test::callout_name_;
Lease6Ptr ExpirationAllocEngine6Test::callout_lease_;

ExpirationAllocEngine6Test::ExpirationAllocEngine6Test()
    : ExpirationAllocEngineTest<Lease6Ptr>("type=memfile universe=6 persist=false") {
    createLeases();
    callout_argument_name = "lease6";

    // Let's clear any garbage previous test may have left in static fields.
    callout_name_ = string("");
    callout_lease_.reset();
}

void
ExpirationAllocEngine6Test::createLeases() {
    // Create TEST_LEASES_NUM leases.
    for (uint16_t i = 0; i < TEST_LEASES_NUM; ++i) {
        // DUID
        std::ostringstream duid_s;
        duid_s << "01020304050607" << std::setw(4) << std::setfill('0') << i;
        DuidPtr duid(new DUID(DUID::fromText(duid_s.str()).getDuid()));

        // Address.
        std::ostringstream address_s;
        address_s << "2001:db8:1::" << std::setw(4) << std::setfill('0') << i;
        IOAddress address(address_s.str());

        // Create lease.
        Lease6Ptr lease(new Lease6(Lease::TYPE_NA, address, duid, 1, 50, 60,
                                   SubnetID(1), true, true,
                                   generateHostnameForLeaseIndex(i)));
        ElementPtr user_context = Element::createMap();
        user_context->set("index", Element::create(static_cast<int>(i)));
        lease->setContext(user_context);
        leases_.push_back(lease);
        // Copy the lease before adding it to the lease manager. We want to
        // make sure that modifications to the leases held in the leases_
        // container doesn't affect the leases in the lease manager.
        Lease6Ptr tmp(new Lease6(*lease));
        LeaseMgrFactory::instance().addLease(tmp);

        // Note in the statistics that this lease has been added.
        StatsMgr& stats_mgr = StatsMgr::instance();
        std::string stat_name =
            lease->type_ == Lease::TYPE_NA ? "assigned-nas" : "assigned-pds";
        stats_mgr.addValue(stats_mgr.generateName("subnet", lease->subnet_id_, stat_name),
                           int64_t(1));
    }
}

void
ExpirationAllocEngine6Test::transferOwnership(const uint16_t lease_index) {
    ASSERT_GT(leases_.size(), lease_index);
    std::vector<uint8_t> bytes = leases_[lease_index]->duid_->getDuid();
    if (bytes.size() > 1) {
        if (++bytes[0] == 0) {
            ++bytes[1];
        }
    }

    leases_[lease_index]->duid_.reset(new DUID(bytes));
}

void
ExpirationAllocEngine6Test::setSubnetId(const uint16_t lease_index, const SubnetID& id) {
    ASSERT_GT(leases_.size(), lease_index);
    if (leases_[lease_index]->subnet_id_ != id) {
        StatsMgr& stats_mgr = StatsMgr::instance();
        std::string stats_name = (leases_[lease_index]->type_ == Lease::TYPE_NA ?
                                  "assigned-nas" : "assigned-pds");
        stats_mgr.addValue(stats_mgr.generateName("subnet", id, stats_name),
                           int64_t(1));
        stats_mgr.addValue(stats_mgr.generateName("subnet",
                                                  leases_[lease_index]->subnet_id_,
                                                  stats_name),
                           int64_t(-1));
        leases_[lease_index]->subnet_id_ = id;
        ASSERT_NO_THROW(updateLease(lease_index));
    }
}

void
ExpirationAllocEngine6Test::setLeaseType(const uint16_t lease_index,
                                         const Lease6::Type& lease_type) {
    ASSERT_GT(leases_.size(), lease_index);
    if (leases_[lease_index]->type_ != lease_type) {
        StatsMgr& stats_mgr = StatsMgr::instance();
        std::string stats_name = (lease_type == Lease::TYPE_NA ?
                                  "assigned-nas" : "assigned-pds");
        stats_mgr.addValue(stats_mgr.generateName("subnet",
                                                  leases_[lease_index]->subnet_id_,
                                                  stats_name),
                           int64_t(1));
        stats_name = (leases_[lease_index]->type_ == Lease::TYPE_NA ?
                      "assigned-nas" : "assigned-pds");
        stats_mgr.addValue(stats_mgr.generateName("subnet",
                                                  leases_[lease_index]->subnet_id_,
                                                  stats_name),
                           int64_t(-1));
        leases_[lease_index]->type_ = lease_type;
        ASSERT_NO_THROW(updateLease(lease_index));
    }
}


void
ExpirationAllocEngine6Test::testReclaimExpiredLeasesStats() {
    // This test requires that the number of leases is an even number.
    BOOST_STATIC_ASSERT(TEST_LEASES_NUM % 2 == 0);

    for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
        // Mark all leases as expired. The higher the index the less
        // expired the lease.
        expire(i, 1000 - i);
        // Modify subnet ids and lease types for some leases.
        if (evenLeaseIndex(i)) {
            setSubnetId(i, SubnetID(2));
            setLeaseType(i, Lease::TYPE_PD);
        }
    }

    // Leases will be reclaimed in groups of 8.
    const size_t reclamation_group_size = 8;
    for (unsigned int i = reclamation_group_size; i < TEST_LEASES_NUM;
         i += reclamation_group_size) {

        // Reclaim 8 most expired leases out of TEST_LEASES_NUM.
        ASSERT_NO_THROW(reclaimExpiredLeases(reclamation_group_size,
                                             0, false));

        // Number of reclaimed leases should increase as we loop.
        EXPECT_TRUE(testStatistics("reclaimed-leases", i));
        // Make sure that the number of reclaimed leases is also distributed
        // across two subnets.
        EXPECT_TRUE(testStatistics("reclaimed-leases", i / 2, 1));
        EXPECT_TRUE(testStatistics("reclaimed-leases", i / 2, 2));
        // Number of assigned leases should decrease as we reclaim them.
        EXPECT_TRUE(testStatistics("assigned-nas",
                                   (TEST_LEASES_NUM - i) / 2, 1));
        EXPECT_TRUE(testStatistics("assigned-pds",
                                   (TEST_LEASES_NUM - i) / 2, 2));
    }
}

void
ExpirationAllocEngine6Test::testReclaimReusedLeases(const uint16_t msg_type,
                                                    const bool use_reclaimed) {
    BOOST_STATIC_ASSERT(TEST_LEASES_NUM < 1000);

    for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
        // Depending on the parameter, mark leases 'expired-reclaimed' or
        // simply 'expired'.
        if (use_reclaimed) {
            reclaim(i, 1000 - i);

        } else {
            // Mark all leases as expired.
            expire(i, 1000 - i);
        }

        // For the Renew case, we don't change the ownership of leases. We
        // will let the lease owners renew them. For other cases, we modify
        // the DUIDs to simulate reuse of expired leases.
        if (msg_type != DHCPV6_RENEW) {
            transferOwnership(i);
        }
    }

    // Create subnet and the pool. This is required by the allocation process.
    Subnet6Ptr subnet(new Subnet6(IOAddress("2001:db8:1::"), 64, 10, 20, 50, 60,
                                  SubnetID(1)));
    ASSERT_NO_THROW(subnet->addPool(Pool6Ptr(new Pool6(Lease::TYPE_NA,
                                                       IOAddress("2001:db8:1::"),
                                                       IOAddress("2001:db8:1::FFFF")))));

    for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
        // Build the context.
        AllocEngine::ClientContext6 ctx(subnet, leases_[i]->duid_,
                                        false, false,
                                        leases_[i]->hostname_,
                                        msg_type == DHCPV6_SOLICIT,
                                        Pkt6Ptr(new Pkt6(msg_type, 0x1234)));
        ctx.currentIA().iaid_ = 1;
        ctx.currentIA().addHint(leases_[i]->addr_);

        // Depending on the message type, we will call a different function.
        if (msg_type == DHCPV6_RENEW) {
            ASSERT_NO_THROW(engine_->renewLeases6(ctx));

        } else {
            ASSERT_NO_THROW(engine_->allocateLeases6(ctx));
        }
    }

    // The Solicit should not trigger leases reclamation. The Renew and
    // Request must trigger leases reclamation unless the lease is
    // initially reclaimed.
    if (use_reclaimed || (msg_type == DHCPV6_SOLICIT)) {
        EXPECT_TRUE(testStatistics("reclaimed-leases", 0));
    } else {
        EXPECT_TRUE(testStatistics("reclaimed-leases", TEST_LEASES_NUM));
        EXPECT_TRUE(testStatistics("assigned-nas", TEST_LEASES_NUM, subnet->getID()));
        // Leases should have been updated in the lease database and their
        // state should not be 'expired-reclaimed' anymore.
        EXPECT_TRUE(testLeases(&leaseNotReclaimed, &allLeaseIndexes));
    }

}

void
ExpirationAllocEngine6Test::testReclaimDeclinedHook(bool skip) {
    for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {

        // Mark leases with even indexes as expired.
        if (evenLeaseIndex(i)) {

            // Mark lease as declined with 100 seconds of probation-period
            // (i.e. lease is supposed to be off limits for 100 seconds)
            decline(i, 100);

            // The higher the index, the more expired the lease.
            expire(i, 10 + i);
        }
    }

    EXPECT_NO_THROW(HooksManager::preCalloutsLibraryHandle().registerCallout(
                        "lease6_recover",
                        skip ? lease6RecoverSkipCallout : lease6RecoverCallout));

    // Run leases reclamation routine on all leases.
    ASSERT_NO_THROW(reclaimExpiredLeases(0, 0, true));

    // Make sure that the callout really was called. It was supposed to modify
    // the callout_name_ and store the lease in callout_lease_
    EXPECT_EQ("lease6_recover", callout_name_);
    EXPECT_TRUE(callout_lease_);

    // Leases with even indexes should not exist in the DB
    if (skip) {
        // Skip status should have prevented removing the lease.
        EXPECT_TRUE(testLeases(&leaseExists, &evenLeaseIndex));
    } else {
        // The hook hasn't modified next step status. The lease should be gone.
        EXPECT_TRUE(testLeases(&leaseDoesntExist, &evenLeaseIndex));
    }
};

void
ExpirationAllocEngine6Test::testReclaimExpiredLeasesRegisteredStats() {
    // This test requires that the number of leases is an even number.
    BOOST_STATIC_ASSERT(TEST_LEASES_NUM % 2 == 0);

    for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
        // Mark all leases as expired. The higher the index the less
        // expired the lease.
        expire(i, 1000 - i);
        // Modify subnet ids some leases.
        if (evenLeaseIndex(i)) {
            setSubnetId(i, SubnetID(2));
        }
        // Mark all leases as registered.
        registered(i);
    }

    // Leases will be reclaimed in groups of 8.
    const size_t reclamation_group_size = 8;
    for (unsigned int i = reclamation_group_size; i < TEST_LEASES_NUM;
         i += reclamation_group_size) {

        // Reclaim 8 most expired leases out of TEST_LEASES_NUM.
        ASSERT_NO_THROW(reclaimExpiredLeases(reclamation_group_size,
                                             0, false));

        // Number of reclaimed leases should increase as we loop.
        EXPECT_TRUE(testStatistics("reclaimed-leases", i));
        // Make sure that the number of reclaimed leases is also distributed
        // across two subnets.
        EXPECT_TRUE(testStatistics("reclaimed-leases", i / 2, 1));
        EXPECT_TRUE(testStatistics("reclaimed-leases", i / 2, 2));
        // Number of registerer leases should decrease as we reclaim them.
        EXPECT_TRUE(testStatistics("registered-nas",
                                   (TEST_LEASES_NUM - i) / 2, 1));
        EXPECT_TRUE(testStatistics("registered-nas",
                                   (TEST_LEASES_NUM - i) / 2, 2));
    }
}

// This test verifies that the leases can be reclaimed without being removed
// from the database. In such case, the leases' state is set to
// "expired-reclaimed".
TEST_F(ExpirationAllocEngine6Test, reclaimExpiredLeases6UpdateState) {
    testReclaimExpiredLeasesUpdateState();
}

// This test verifies that the reclaimed leases are deleted when requested.
TEST_F(ExpirationAllocEngine6Test, reclaimExpiredLeasesDelete) {
    testReclaimExpiredLeasesDelete();
}

// This test verifies that it is possible to specify the limit for the
// number of reclaimed leases.
TEST_F(ExpirationAllocEngine6Test, reclaimExpiredLeasesLimit) {
    testReclaimExpiredLeasesLimit();
}

// This test verifies that DNS updates are generated for the leases
// for which the DNS records exist.
TEST_F(ExpirationAllocEngine6Test, reclaimExpiredLeasesWithDDNS) {
    testReclaimExpiredLeasesWithDDNS();
}

// This test verifies that it is DNS updates are generated only for the
// reclaimed expired leases. In this case we limit the number of leases
// reclaimed during a single call to reclamation routine.
TEST_F(ExpirationAllocEngine6Test, reclaimExpiredLeasesWithDDNSAndLimit) {
    testReclaimExpiredLeasesWithDDNSAndLimit();
}

// This test verifies that if some leases have invalid hostnames, the
// lease reclamation routine continues with reclamation of leases anyway.
TEST_F(ExpirationAllocEngine6Test, reclaimExpiredLeasesInvalidHostname) {
    testReclaimExpiredLeasesInvalidHostname();
}

// This test verifies that statistics is correctly updated when the leases
// are reclaimed.
TEST_F(ExpirationAllocEngine6Test, reclaimExpiredLeasesStats) {
    testReclaimExpiredLeasesStats();
}

// This test verifies that callouts are executed for each expired lease.
TEST_F(ExpirationAllocEngine6Test, reclaimExpiredLeasesHooks) {
    testReclaimExpiredLeasesHooks();
}

// This test verifies that callouts are executed for each expired lease
// and that the lease is not reclaimed when the skip flag is set.
TEST_F(ExpirationAllocEngine6Test, reclaimExpiredLeasesHooksWithSkip) {
    testReclaimExpiredLeasesHooksWithSkip();
}

// This test verifies that it is possible to set the timeout for the
// execution of the lease reclamation routine.
TEST_F(ExpirationAllocEngine6Test, reclaimExpiredLeasesTimeout) {
    // This test needs at least 40 leases to make sense.
    BOOST_STATIC_ASSERT(TEST_LEASES_NUM >= 40);
    // Run with timeout of 1.2s.
    testReclaimExpiredLeasesTimeout(1200);
}

// This test verifies that at least one lease is reclaimed if the timeout
// for the lease reclamation routine is shorter than the time needed for
// the reclamation of a single lease. This prevents the situation when
// very short timeout (perhaps misconfigured) effectively precludes leases
// reclamation.
TEST_F(ExpirationAllocEngine6Test, reclaimExpiredLeasesShortTimeout) {
    // We will most likely reclaim just one lease, so 5 is more than enough.
    BOOST_STATIC_ASSERT(TEST_LEASES_NUM >= 5);
    // Reclaim leases with the 1ms timeout.
    testReclaimExpiredLeasesTimeout(1);
}

// This test verifies that expired-reclaimed leases are removed from the
// lease database.
TEST_F(ExpirationAllocEngine6Test, deleteExpiredReclaimedLeases) {
    BOOST_STATIC_ASSERT(TEST_LEASES_NUM >= 10);
    testDeleteExpiredReclaimedLeases();
}

/// This test verifies that @ref AllocEngine::reclaimExpiredLeases6 properly
/// handles declined leases that have expired in case when it is told to
/// remove leases.}
TEST_F(ExpirationAllocEngine6Test, reclaimDeclined1) {
    testReclaimDeclined(true);
}

/// This test verifies that @ref AllocEngine::reclaimExpiredLeases6 properly
/// handles declined leases that have expired in case when it is told to
/// not remove leases. This flag should not matter and declined expired
/// leases should always be removed.
TEST_F(ExpirationAllocEngine6Test, reclaimDeclined2) {
    testReclaimDeclined(false);
}

/// This test verifies that statistics are modified correctly after
/// reclaim expired leases is called.
TEST_F(ExpirationAllocEngine6Test, reclaimDeclinedStats) {
    testReclaimDeclinedStats("assigned-nas");
}

// This test verifies that expired leases are reclaimed before they are
// allocated to another client sending a Request message.
TEST_F(ExpirationAllocEngine6Test, reclaimReusedLeases) {
    testReclaimReusedLeases(DHCPV6_REQUEST, false);
}

// This test verifies that allocation engine detects that the expired
// lease has been reclaimed already when it reuses this lease.
TEST_F(ExpirationAllocEngine6Test, reclaimReusedLeasesAlreadyReclaimed) {
    testReclaimReusedLeases(DHCPV6_REQUEST, true);
}

// This test verifies that expired leases are reclaimed before they
// are renewed.
TEST_F(ExpirationAllocEngine6Test, reclaimRenewedLeases) {
    testReclaimReusedLeases(DHCPV6_RENEW, false);
}

// This test verifies that allocation engine detects that the expired
// lease has been reclaimed already when it renews the lease.
TEST_F(ExpirationAllocEngine6Test, reclaimRenewedLeasesAlreadyReclaimed) {
    testReclaimReusedLeases(DHCPV6_RENEW, true);
}

// This test verifies that the expired leases are not reclaimed when the
// Solicit message is being processed.
TEST_F(ExpirationAllocEngine6Test, reclaimReusedLeasesSolicit) {
    testReclaimReusedLeases(DHCPV6_SOLICIT, false);
}

// This test verifies that the 'expired-reclaimed' leases are not reclaimed
// again when the Solicit message is being processed.
TEST_F(ExpirationAllocEngine6Test, reclaimReusedLeasesSolicitAlreadyReclaimed) {
    testReclaimReusedLeases(DHCPV6_SOLICIT, true);
}

// This test verifies if the hooks installed on lease6_recover are called
// when the lease expires.
TEST_F(ExpirationAllocEngine6Test, reclaimDeclinedHook1) {
    testReclaimDeclinedHook(false); // false = don't use skip callout
}

// This test verifies if the hooks installed on lease6_recover are called
// when the lease expires and that the next step status set to SKIP
// causes the recovery to not be conducted.
TEST_F(ExpirationAllocEngine6Test, reclaimDeclinedHook2) {
    testReclaimDeclinedHook(true); // true = use skip callout
}

// This test verifies expired registered leases are deleted by the
// reclamation routine.
TEST_F(ExpirationAllocEngine6Test, reclaimExpiredLeasesRegistered) {
    testReclaimExpiredLeasesRegistered();
}

// This test verifies expired registered leases are deleted by the
// reclamation routine and DNS updates generated.
TEST_F(ExpirationAllocEngine6Test, reclaimExpiredLeasesRegisteredWithDDNS) {
    testReclaimExpiredLeasesRegisteredWithDDNS();
}

// This test verifies expired registered leases are deleted by the
// reclamation routine and statistics updated.
TEST_F(ExpirationAllocEngine6Test, reclaimExpiredLeasesRegisteredStats) {
    testReclaimExpiredLeasesRegisteredStats();
}

// *******************************************************
//
// DHCPv4 lease reclamation routine tests start here!
//
// *******************************************************

/// @brief Specialization of the @c ExpirationAllocEngineTest class to test
/// reclamation of the IPv4 leases.
class ExpirationAllocEngine4Test : public ExpirationAllocEngineTest<Lease4Ptr> {
public:

    /// @brief Class constructor.
    ///
    /// This constructor initializes @c TEST_LEASES_NUM leases and
    /// stores them in the lease manager.
    ExpirationAllocEngine4Test();

    /// @brief Virtual destructor.
    ///
    /// Clears up static fields that may be modified by hooks.
    virtual ~ExpirationAllocEngine4Test() {
        callout_lease_.reset();
        callout_name_ = string("");
    }

    /// @brief Creates collection of leases for a test.
    ///
    /// It is called internally at the construction time.
    void createLeases();

    /// @brief Generates unique client identifier from lease index.
    ///
    /// @param index lease index.
    void setUniqueClientId(const uint16_t index);

    /// @brief Updates lease in the lease database.
    ///
    /// @param lease_index Index of the lease.
    virtual void updateLease(const unsigned int lease_index) {
        LeaseMgrFactory::instance().updateLease4(leases_[lease_index]);
    }

    /// @brief Changes the owner of a lease.
    ///
    /// This method changes the owner of the lease by updating the client
    /// identifier (if present) or HW address.
    ///
    /// @param lease_index Lease index. Must be between 0 and
    /// @c TEST_LEASES_NUM.
    virtual void transferOwnership(const uint16_t lease_index);

    /// @brief Retrieves lease from the database.
    ///
    /// @param lease_index Index of the lease.
    virtual Lease4Ptr getLease(const unsigned int lease_index) const {
        return (LeaseMgrFactory::instance().getLease4(leases_[lease_index]->addr_));
    }

    /// @brief Sets subnet id for a lease.
    ///
    /// It also updates statistics of assigned leases in the stats manager.
    ///
    /// @param lease_index Lease index.
    /// @param id New subnet id.
    virtual void setSubnetId(const uint16_t lease_index, const SubnetID& id);

    /// @brief Wrapper method running lease reclamation routine.
    ///
    /// @param max_leases Maximum number of leases to be reclaimed.
    /// @param timeout Maximum amount of time that the reclamation routine
    /// may be processing expired leases, expressed in seconds.
    /// @param remove_lease A boolean value indicating if the lease should
    /// be removed when it is reclaimed (if true) or it should be left in the
    /// database in the "expired-reclaimed" state (if false).
    virtual void reclaimExpiredLeases(const size_t max_leases,
                                      const uint16_t timeout,
                                      const bool remove_lease) {
        engine_->reclaimExpiredLeases4(max_leases, timeout, remove_lease);
    }

    /// @brief Wrapper method for removing expired-reclaimed leases.
    ///
    /// @param secs The minimum amount of time, expressed in seconds,
    /// for the lease to be left in the "expired-reclaimed" state
    /// before it can be removed.
    virtual void deleteExpiredReclaimedLeases(const uint32_t secs) {
        engine_->deleteExpiredReclaimedLeases4(secs);
    }

    /// @brief Lease algorithm checking if NCR has been generated from client
    /// identifier.
    ///
    /// @param lease Pointer to the lease for which the NCR needs to be checked.
    static bool dnsUpdateGeneratedFromClientId(const Lease4Ptr& lease);

    /// @brief Lease algorithm checking if NCR has been generated from
    /// HW address.
    static bool dnsUpdateGeneratedFromHWAddress(const Lease4Ptr& lease);

    /// @brief Test that DNS updates are properly generated when the
    /// reclaimed leases contain client identifier.
    void testReclaimExpiredLeasesWithDDNSAndClientId();

    /// @brief Test that statistics is updated when leases are reclaimed..
    void testReclaimExpiredLeasesStats();

    /// @brief Test that the lease is reclaimed before it is renewed or
    /// reused.
    ///
    /// @param msg_type DHCPv4 message type, i.e. DHCPDISCOVER or DHCPREQUEST.
    /// @param client_renews A boolean value which indicates if the test should
    /// simulate renewals of leases (if true) or reusing expired leases which
    /// belong to different clients (if false).
    /// @param use_reclaimed Boolean parameter indicating if the leases being
    /// reused should initially be reclaimed.
    void testReclaimReusedLeases(const uint8_t msg_type, const bool client_renews,
                                 const bool use_reclaimed);

    /// @brief Callout for lease4_recover
    ///
    /// This callout stores passed parameter into static fields.
    ///
    /// @param callout_handle will be provided by hooks framework
    /// @return always 0
    static int lease4RecoverCallout(CalloutHandle& callout_handle) {
        callout_name_ = "lease4_recover";

        callout_handle.getArgument("lease4", callout_lease_);

        return (0);
    }

    /// @brief Callout for lease4_recover that sets status to SKIP
    ///
    /// This callout stores passed parameter into static fields.
    ///
    /// @param callout_handle will be provided by hooks framework
    /// @return always 0
    static int lease4RecoverSkipCallout(CalloutHandle& callout_handle) {
        // Set the next step status to SKIP
        callout_handle.setStatus(CalloutHandle::NEXT_STEP_SKIP);

        return (lease4RecoverCallout(callout_handle));
    }

    /// @brief Test install a hook callout, recovers declined leases
    ///
    /// This test: declines, then expires half of the leases, then
    /// installs a callout on lease4_recover hook, then reclaims
    /// expired leases and checks that:
    /// - the callout was indeed called
    /// - the parameter (lease4) was indeed passed as expected
    /// - checks that the leases are removed (skip=false) or
    /// - checks that the leases are still there (skip=true)
    /// @param skip should the callout set the next step status to skip?
    void
    testReclaimDeclinedHook(bool skip);

    /// The following parameters will be written by a callout
    static std::string callout_name_; ///< Stores callout name
    static Lease4Ptr callout_lease_;  ///< Stores callout parameter
};

std::string ExpirationAllocEngine4Test::callout_name_;
Lease4Ptr ExpirationAllocEngine4Test::callout_lease_;

ExpirationAllocEngine4Test::ExpirationAllocEngine4Test()
    : ExpirationAllocEngineTest<Lease4Ptr>("type=memfile universe=4 persist=false") {
    createLeases();
    callout_argument_name = "lease4";

    // Let's clear any garbage previous test may have left in static fields.
    callout_name_ = string("");
    callout_lease_.reset();
}

void
ExpirationAllocEngine4Test::createLeases() {
    // Create TEST_LEASES_NUM leases.
    for (uint16_t i = 0; i < TEST_LEASES_NUM; ++i) {
        // HW address
        std::ostringstream hwaddr_s;
        hwaddr_s << "01:02:03:04:" << std::setw(2) << std::setfill('0')
                 << (i >> 8) << ":" << std::setw(2) << std::setfill('0')
                 << (i & 0x00FF);
        HWAddrPtr hwaddr(new HWAddr(HWAddr::fromText(hwaddr_s.str(),
                                                     HTYPE_ETHER)));

        // Address.
        std::ostringstream address_s;
        address_s << "10.0." << (i >> 8) << "." << (i & 0x00FF);
        IOAddress address(address_s.str());

        // Create lease.
        Lease4Ptr lease(new Lease4(address, hwaddr, ClientIdPtr(), 60,
                                   time(0), SubnetID(1), true, true,
                                   generateHostnameForLeaseIndex(i)));
        ElementPtr user_context = Element::createMap();
        user_context->set("index", Element::create(static_cast<int>(i)));
        lease->setContext(user_context);
        leases_.push_back(lease);
        // Copy the lease before adding it to the lease manager. We want to
        // make sure that modifications to the leases held in the leases_
        // container doesn't affect the leases in the lease manager.
        Lease4Ptr tmp(new Lease4(*lease));
        LeaseMgrFactory::instance().addLease(tmp);

        // Note in the statistics that this lease has been added.
        StatsMgr& stats_mgr = StatsMgr::instance();
        std::string stat_name = "assigned-addresses";
        stats_mgr.addValue(stats_mgr.generateName("subnet", lease->subnet_id_, stat_name),
                           int64_t(1));
    }
}

void
ExpirationAllocEngine4Test::setUniqueClientId(const uint16_t index) {
    std::ostringstream clientid_s;
    clientid_s << "AA:BB:" << std::setw(2) << std::setfill('0')
        << (index >> 8) << ":" << std::setw(2) << std::setfill('0')
        << (index & 0x00FF);
    ClientIdPtr client_id(ClientId::fromText(clientid_s.str()));
    leases_[index]->client_id_ = client_id;
    LeaseMgrFactory::instance().updateLease4(leases_[index]);
}

void
ExpirationAllocEngine4Test::setSubnetId(const uint16_t lease_index, const SubnetID& id) {
    ASSERT_GT(leases_.size(), lease_index);
    if (leases_[lease_index]->subnet_id_ != id) {
        StatsMgr& stats_mgr = StatsMgr::instance();
        stats_mgr.addValue(stats_mgr.generateName("subnet", id, "assigned-addresses"),
                           int64_t(1));
        stats_mgr.addValue(stats_mgr.generateName("subnet",
                                                  leases_[lease_index]->subnet_id_,
                                                  "assigned-addresses"),
                           int64_t(-1));
        leases_[lease_index]->subnet_id_ = id;
        ASSERT_NO_THROW(updateLease(lease_index));
    }
}

void
ExpirationAllocEngine4Test::transferOwnership(const uint16_t lease_index) {
    ASSERT_GT(leases_.size(), lease_index);
    std::vector<uint8_t> bytes;
    if (leases_[lease_index]->client_id_) {
        bytes = leases_[lease_index]->client_id_->getClientId();
    } else {
        bytes = leases_[lease_index]->hwaddr_->hwaddr_;
    }

    if (!bytes.empty()) {
        if (++bytes[0] == 0) {
            ++bytes[1];
        }
    }

    if (leases_[lease_index]->client_id_) {
        leases_[lease_index]->client_id_.reset(new ClientId(bytes));
    } else {
        leases_[lease_index]->hwaddr_.reset(new HWAddr(bytes, HTYPE_ETHER));
    }
}


bool
ExpirationAllocEngine4Test::dnsUpdateGeneratedFromClientId(const Lease4Ptr& lease) {
    try {
        NameChangeRequestPtr ncr = getNCRForLease(lease);
        if (ncr) {
            if (lease->client_id_) {
                // Generate hostname for this lease. Note that the lease
                // in the database doesn't have the hostname because it
                // has been removed by the lease reclamation routine.
                std::string hostname = generateHostnameForLeaseIndex(
                      getLeaseIndexFromAddress(lease->addr_));

                // Get DHCID from NCR.
                const D2Dhcid& dhcid = ncr->getDhcid();
                // Generate reference DHCID to compare with the one from
                // the NCR.
                std::vector<uint8_t> fqdn_wire;
                OptionDataTypeUtil::writeFqdn(hostname, fqdn_wire, true);
                D2Dhcid clientid_dhcid(lease->client_id_->getClientId(),
                                       fqdn_wire);
                // Return true if they match.
                return (dhcid == clientid_dhcid);
            }
        }

    } catch (...) {
        // If error occurred, treat it as no match.
        return (false);
    }

    // All leases checked - no match.
    return (false);
}

bool
ExpirationAllocEngine4Test::dnsUpdateGeneratedFromHWAddress(const Lease4Ptr& lease) {
    try {
        NameChangeRequestPtr ncr = getNCRForLease(lease);
        if (ncr) {
            if (lease->hwaddr_) {
                // Generate hostname for this lease. Note that the lease
                // in the database doesn't have the hostname because it
                // has been removed by the lease reclamation routine.
                std::string hostname = generateHostnameForLeaseIndex(
                      getLeaseIndexFromAddress(lease->addr_));

                // Get DHCID from NCR.
                const D2Dhcid& dhcid = ncr->getDhcid();
                // Generate reference DHCID to compare with the one from
                // the NCR.
                std::vector<uint8_t> fqdn_wire;
                OptionDataTypeUtil::writeFqdn(hostname, fqdn_wire, true);
                D2Dhcid hwaddr_dhcid(lease->hwaddr_, fqdn_wire);
                // Return true if they match.
                return (dhcid == hwaddr_dhcid);
            }
        }

    } catch (...) {
        // If error occurred, treat it as no match.
        return (false);
    }

    // All leases checked - no match.
    return (false);
}

void
ExpirationAllocEngine4Test::testReclaimExpiredLeasesWithDDNSAndClientId() {
    // DNS must be started for the D2 client to accept NCRs.
    ASSERT_NO_THROW(enableDDNS());

    for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
        // Set client identifiers for leases with even indexes only.
        if (evenLeaseIndex(i)) {
            setUniqueClientId(i);
        }
        // Expire all leases. The higher the index, the more expired the lease.
        expire(i, 10 + i);
    }

    // Reclaim all expired leases.
    ASSERT_NO_THROW(reclaimExpiredLeases(0, 0, false));

    // Leases with even indexes should be reclaimed.
    EXPECT_TRUE(testLeases(&leaseReclaimed, &evenLeaseIndex));
    // DNS updates (removal NCRs) should be generated for all leases.
    EXPECT_TRUE(testLeases(&dnsUpdateGeneratedForLease, &allLeaseIndexes));
    // Leases with even indexes include client identifiers so the DHCID should
    // be generated from the client identifiers.
    EXPECT_TRUE(testLeases(&dnsUpdateGeneratedFromClientId, &evenLeaseIndex));
    // Leases with odd indexes do not include client identifiers so their
    // DHCID should be generated from the HW address.
    EXPECT_TRUE(testLeases(&dnsUpdateGeneratedFromHWAddress, &oddLeaseIndex));
}

void
ExpirationAllocEngine4Test::testReclaimExpiredLeasesStats() {
    // This test requires that the number of leases is an even number.
    BOOST_STATIC_ASSERT(TEST_LEASES_NUM % 2 == 0);

    for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
        // Mark all leases as expired. The higher the index the less
        // expired the lease.
        expire(i, 1000 - i);
        // Modify subnet ids of some leases.
        if (evenLeaseIndex(i)) {
            setSubnetId(i, 2);
        }
    }

    // Leases will be reclaimed in groups of 8.
    const size_t reclamation_group_size = 8;
    for (unsigned int i = reclamation_group_size; i < TEST_LEASES_NUM;
         i += reclamation_group_size) {

        // Reclaim 8 most expired leases out of TEST_LEASES_NUM.
        ASSERT_NO_THROW(reclaimExpiredLeases(reclamation_group_size,
                                             0, false));

        // Number of reclaimed leases should increase as we loop.
        EXPECT_TRUE(testStatistics("reclaimed-leases", i));
        // Make sure that the number of reclaimed leases is also distributed
        // across two subnets.
        EXPECT_TRUE(testStatistics("reclaimed-leases", i / 2, 1));
        EXPECT_TRUE(testStatistics("reclaimed-leases", i / 2, 2));
        // Number of assigned leases should decrease as we reclaim them.
        EXPECT_TRUE(testStatistics("assigned-addresses",
                                   (TEST_LEASES_NUM - i) / 2, 1));
        EXPECT_TRUE(testStatistics("assigned-addresses",
                                   (TEST_LEASES_NUM - i) / 2, 2));
    }
}

void
ExpirationAllocEngine4Test::testReclaimReusedLeases(const uint8_t msg_type,
                                                    const bool client_renews,
                                                    const bool use_reclaimed) {
    // Let's restrict the number of leases.
    BOOST_STATIC_ASSERT(TEST_LEASES_NUM < 1000);

    for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
        // Depending on the parameter, mark leases 'expired-reclaimed' or
        // simply 'expired'.
        if (use_reclaimed) {
            reclaim(i, 1000 - i);

        } else {
            // Mark all leases as expired.
            expire(i, 1000 - i);
        }

        // Check if we're simulating renewals or reusing leases. If this is
        // about reusing leases, we should be using different MAC addresses
        // or client identifiers for the leases than those stored presently
        // in the database.
        if (!client_renews) {
            // This function modifies the MAC address or the client identifier
            // of the test lease to make sure it doesn't match the one we
            // have in the database.
            transferOwnership(i);
        }
    }

    // The call to AllocEngine::allocateLease4 requires the subnet selection.
    // The pool must be present within a subnet for the allocation engine to
    // hand out address from.
    Subnet4Ptr subnet(new Subnet4(IOAddress("10.0.0.0"), 16, 10, 20, 60, SubnetID(1)));
    ASSERT_NO_THROW(subnet->addPool(Pool4Ptr(new Pool4(IOAddress("10.0.0.0"),
                                                       IOAddress("10.0.255.255")))));

    // Re-allocate leases (reuse or renew).
    for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {
        // Build the context.
        AllocEngine::ClientContext4 ctx(subnet, leases_[i]->client_id_,
                                        leases_[i]->hwaddr_,
                                        leases_[i]->addr_, false, false,
                                        leases_[i]->hostname_,
                                        msg_type == DHCPDISCOVER);
        // Query is needed for logging purposes.
        ctx.query_.reset(new Pkt4(msg_type, 0x1234));

        // Re-allocate a lease. Note that the iterative will pick addresses
        // starting from the beginning of the pool. This matches exactly
        // the set of addresses we have allocated and stored in the database.
        // Since all leases are marked expired the allocation engine will
        // reuse them or renew them as appropriate.
        ASSERT_NO_THROW(engine_->allocateLease4(ctx));
    }

    // If DHCPDISCOVER is being processed, the leases should not be reclaimed.
    // Also, the leases should not be reclaimed if they are already in the
    // 'expired-reclaimed' state.
    if (use_reclaimed || (msg_type == DHCPDISCOVER)) {
        EXPECT_TRUE(testStatistics("reclaimed-leases", 0));

    } else if (msg_type == DHCPREQUEST) {
        // Re-allocation of expired leases should result in reclamations.
        EXPECT_TRUE(testStatistics("reclaimed-leases", TEST_LEASES_NUM));
        EXPECT_TRUE(testStatistics("assigned-addresses", TEST_LEASES_NUM, subnet->getID()));
        // Leases should have been updated in the lease database and their
        // state should not be 'expired-reclaimed' anymore.
        EXPECT_TRUE(testLeases(&leaseNotReclaimed, &allLeaseIndexes));
    }
}

void
ExpirationAllocEngine4Test::testReclaimDeclinedHook(bool skip) {
    for (unsigned int i = 0; i < TEST_LEASES_NUM; ++i) {

        // Mark leases with even indexes as expired.
        if (evenLeaseIndex(i)) {

            // Mark lease as declined with 100 seconds of probation-period
            // (i.e. lease is supposed to be off limits for 100 seconds)
            decline(i, 100);

            // The higher the index, the more expired the lease.
            expire(i, 10 + i);
        }
    }

    EXPECT_NO_THROW(HooksManager::preCalloutsLibraryHandle().registerCallout(
                        "lease4_recover",
                        skip ? lease4RecoverSkipCallout : lease4RecoverCallout));

    // Run leases reclamation routine on all leases.
    ASSERT_NO_THROW(reclaimExpiredLeases(0, 0, true));

    // Make sure that the callout really was called. It was supposed to modify
    // the callout_name_ and store the lease in callout_lease_
    EXPECT_EQ("lease4_recover", callout_name_);
    EXPECT_TRUE(callout_lease_);

    // Leases with even indexes should not exist in the DB
    if (skip) {
        // Skip status should have prevented removing the lease.
        EXPECT_TRUE(testLeases(&leaseExists, &evenLeaseIndex));
    } else {
        // The hook hasn't modified next step status. The lease should be gone.
        EXPECT_TRUE(testLeases(&leaseDoesntExist, &evenLeaseIndex));
    }
};

// This test verifies that the leases can be reclaimed without being removed
// from the database. In such case, the leases' state is set to
// "expired-reclaimed".
TEST_F(ExpirationAllocEngine4Test, reclaimExpiredLeasesUpdateState) {
    testReclaimExpiredLeasesUpdateState();
}

// This test verifies that the reclaimed leases are deleted when requested.
TEST_F(ExpirationAllocEngine4Test, reclaimExpiredLeasesDelete) {
    testReclaimExpiredLeasesDelete();
}

// This test verifies that it is possible to specify the limit for the
// number of reclaimed leases.
TEST_F(ExpirationAllocEngine4Test, reclaimExpiredLeasesLimit) {
    testReclaimExpiredLeasesLimit();
}

// This test verifies that DNS updates are generated for the leases
// for which the DNS records exist.
TEST_F(ExpirationAllocEngine4Test, reclaimExpiredLeasesWithDDNS) {
    testReclaimExpiredLeasesWithDDNS();
}

// This test verifies that it is DNS updates are generated only for the
// reclaimed expired leases. In this case we limit the number of leases
// reclaimed during a single call to reclamation routine.
TEST_F(ExpirationAllocEngine4Test, reclaimExpiredLeasesWithDDNSAndLimit) {
    testReclaimExpiredLeasesWithDDNSAndLimit();
}

// This test verifies that if some leases have invalid hostnames, the
// lease reclamation routine continues with reclamation of leases anyway.
TEST_F(ExpirationAllocEngine4Test, reclaimExpiredLeasesInvalidHostname) {
    testReclaimExpiredLeasesInvalidHostname();
}

// This test verifies that DNS updates are properly generated when the
// client id is used as a primary identifier in the lease.
TEST_F(ExpirationAllocEngine4Test, reclaimExpiredLeasesWithDDNSAndClientId) {
    testReclaimExpiredLeasesWithDDNSAndClientId();
}

// This test verifies that statistics is correctly updated when the leases
// are reclaimed.
TEST_F(ExpirationAllocEngine4Test, reclaimExpiredLeasesStats) {
    testReclaimExpiredLeasesStats();
}

// This test verifies that callouts are executed for each expired lease.
TEST_F(ExpirationAllocEngine4Test, reclaimExpiredLeasesHooks) {
    testReclaimExpiredLeasesHooks();
}

// This test verifies that callouts are executed for each expired lease
// and that the lease is not reclaimed when the skip flag is set.
TEST_F(ExpirationAllocEngine4Test, reclaimExpiredLeasesHooksWithSkip) {
    testReclaimExpiredLeasesHooksWithSkip();
}

// This test verifies that it is possible to set the timeout for the
// execution of the lease reclamation routine.
TEST_F(ExpirationAllocEngine4Test, reclaimExpiredLeasesTimeout) {
    // This test needs at least 40 leases to make sense.
    BOOST_STATIC_ASSERT(TEST_LEASES_NUM >= 40);
    // Run with timeout of 1.2s.
    testReclaimExpiredLeasesTimeout(1200);
}

// This test verifies that at least one lease is reclaimed if the timeout
// for the lease reclamation routine is shorter than the time needed for
// the reclamation of a single lease. This prevents the situation when
// very short timeout (perhaps misconfigured) effectively precludes leases
// reclamation.
TEST_F(ExpirationAllocEngine4Test, reclaimExpiredLeasesShortTimeout) {
    // We will most likely reclaim just one lease, so 5 is more than enough.
    BOOST_STATIC_ASSERT(TEST_LEASES_NUM >= 5);
    // Reclaim leases with the 1ms timeout.
    testReclaimExpiredLeasesTimeout(1);
}

// This test verifies that expired-reclaimed leases are removed from the
// lease database.
TEST_F(ExpirationAllocEngine4Test, deleteExpiredReclaimedLeases) {
    BOOST_STATIC_ASSERT(TEST_LEASES_NUM >= 10);
    testDeleteExpiredReclaimedLeases();
}

/// This test verifies that @ref AllocEngine::reclaimExpiredLeases4 properly
/// handles declined leases that have expired in case when it is told to
/// remove leases.
TEST_F(ExpirationAllocEngine4Test, reclaimDeclined1) {
    testReclaimDeclined(true);
}

/// This test verifies that @ref AllocEngine::reclaimExpiredLeases4 properly
/// handles declined leases that have expired in case when it is told to
/// not remove leases. This flag should not matter and declined expired
/// leases should always be removed.
TEST_F(ExpirationAllocEngine4Test, reclaimDeclined2) {
    testReclaimDeclined(false);
}

/// This test verifies that statistics are modified correctly after
/// reclaim expired leases is called.
TEST_F(ExpirationAllocEngine4Test, reclaimDeclinedStats) {
    testReclaimDeclinedStats("assigned-addresses");
}

// This test verifies that the lease is reclaimed before it is reused.
TEST_F(ExpirationAllocEngine4Test, reclaimReusedLeases) {
    // First false value indicates that the leases will be reused.
    // Second false value indicates that the lease will not be
    // initially reclaimed.
    testReclaimReusedLeases(DHCPREQUEST, false, false);
}

// This test verifies that the lease is not reclaimed when it is
// reused and  if its state indicates that it has been already reclaimed.
TEST_F(ExpirationAllocEngine4Test, reclaimReusedLeasesAlreadyReclaimed) {
    // false value indicates that the leases will be reused
    // true value indicates that the lease will be initially reclaimed.
    testReclaimReusedLeases(DHCPREQUEST, false, true);
}

// This test verifies that the expired lease is reclaimed before it
// is renewed.
TEST_F(ExpirationAllocEngine4Test, reclaimRenewedLeases) {
    // true value indicates that the leases will be renewed.
    // false value indicates that the lease will not be initially
    // reclaimed.
    testReclaimReusedLeases(DHCPREQUEST, true, false);
}

// This test verifies that the lease is not reclaimed upon renewal
// if its state indicates that it has been already reclaimed.
TEST_F(ExpirationAllocEngine4Test, reclaimRenewedLeasesAlreadyReclaimed) {
    // First true value indicates that the leases will be renewed.
    // Second true value indicates that the lease will be initially
    // reclaimed.
    testReclaimReusedLeases(DHCPREQUEST, true, true);
}

// This test verifies that the reused lease is not reclaimed when the
// processed message is a DHCPDISCOVER.
TEST_F(ExpirationAllocEngine4Test, reclaimReusedLeasesDiscover) {
    testReclaimReusedLeases(DHCPDISCOVER, false, false);
}

// This test verifies that the lease being in the 'expired-reclaimed'
// state is not reclaimed again when processing the DHCPDISCOVER
// message.
TEST_F(ExpirationAllocEngine4Test, reclaimRenewedLeasesDiscoverAlreadyReclaimed) {
    testReclaimReusedLeases(DHCPDISCOVER, false, true);
}

// This test verifies if the hooks installed on lease4_recover are called
// when the lease expires.
TEST_F(ExpirationAllocEngine4Test, reclaimDeclinedHook1) {
    testReclaimDeclinedHook(false); // false = don't use skip callout
}

// This test verifies if the hooks installed on lease4_recover are called
// when the lease expires and that the next step status set to SKIP
// causes the recovery to not be conducted.
TEST_F(ExpirationAllocEngine4Test, reclaimDeclinedHook2) {
    testReclaimDeclinedHook(true); // true = use skip callout
}

} // end of anonymous namespace
