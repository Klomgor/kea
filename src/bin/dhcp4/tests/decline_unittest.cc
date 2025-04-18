// Copyright (C) 2014-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.


#include <config.h>
#include <asiolink/io_address.h>
#include <cc/data.h>
#include <dhcp/dhcp4.h>
#include <dhcp/testutils/iface_mgr_test_config.h>
#include <dhcpsrv/cfgmgr.h>
#include <dhcpsrv/subnet_id.h>

#ifdef HAVE_MYSQL
#include <mysql/testutils/mysql_schema.h>
#include <hooks/dhcp/mysql/mysql_lease_mgr.h>
#endif

#ifdef HAVE_PGSQL
#include <pgsql/testutils/pgsql_schema.h>
#include <hooks/dhcp/pgsql/pgsql_lease_mgr.h>
#endif

#include <dhcp4/tests/dhcp4_test_utils.h>
#include <dhcp4/tests/dhcp4_client.h>
#include <stats/stats_mgr.h>
#include <boost/shared_ptr.hpp>
#include <sstream>

using namespace isc;
using namespace isc::asiolink;
using namespace isc::data;
using namespace isc::db;
using namespace isc::dhcp;
using namespace isc::dhcp::test;
using namespace isc::stats;

namespace {
/// @brief Set of JSON configurations used throughout the Decline tests.
///
/// - Configuration 0:
///   - Used for testing Decline message processing
///   - 1 subnet: 10.0.0.0/24
///   - 1 pool: 10.0.0.10-10.0.0.100
///   - Router option present: 10.0.0.200 and 10.0.0.201
const char* DECLINE_CONFIGS[] = {
// Configuration 0
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1,"
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "    \"option-data\": [ {"
        "        \"name\": \"routers\","
        "        \"data\": \"10.0.0.200,10.0.0.201\""
        "    } ]"
        " } ]"
    "}",
// Configuration 1 - only use when mysql is enabled
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"lease-database\": {"
            "\"type\": \"mysql\","
            "\"name\": \"keatest\","
            "\"user\": \"keatest\","
            "\"password\": \"keatest\""
        "},"
        "\"valid-lifetime\": 600,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1,"
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "    \"option-data\": [ {"
        "        \"name\": \"routers\","
        "        \"data\": \"10.0.0.200,10.0.0.201\""
        "    } ]"
        " } ]"
    "}",
// Configuration 2 - only use when pgsql is enabled
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"lease-database\": {"
            "\"type\": \"postgresql\","
            "\"name\": \"keatest\","
            "\"user\": \"keatest\","
            "\"password\": \"keatest\""
        "},"
        "\"valid-lifetime\": 600,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1,"
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "    \"option-data\": [ {"
        "        \"name\": \"routers\","
        "        \"data\": \"10.0.0.200,10.0.0.201\""
        "    } ]"
        " } ]"
    "}",
};

}

namespace isc {
namespace dhcp {
namespace test {

void
Dhcpv4SrvTest::acquireLease(Dhcp4Client& client) {
    // Perform 4-way exchange with the server but to not request any
    // specific address in the DHCPDISCOVER message.
    ASSERT_NO_THROW(client.doDORA());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Response must not be relayed.
    EXPECT_FALSE(resp->isRelayed());
    // Make sure that the server id is present.
    EXPECT_EQ("10.0.0.1", client.config_.serverid_.toText());
    // Make sure that the client has got the lease with the requested address.
    ASSERT_NE(client.config_.lease_.addr_.toText(), "0.0.0.0");
    Lease4Ptr lease = LeaseMgrFactory::instance().getLease4(client.config_.lease_.addr_);
    ASSERT_TRUE(lease);
}

void
Dhcpv4SrvTest::acquireAndDecline(Dhcp4Client& client,
                                 const std::string& hw_address_1,
                                 const std::string& client_id_1,
                                 const std::string& hw_address_2,
                                 const std::string& client_id_2,
                                 ExpectedResult expected_result,
                                 uint8_t config_index) {

    // Set this global statistic explicitly to zero.
    isc::stats::StatsMgr::instance().setValue("declined-addresses",
                                              static_cast<int64_t>(0));

    // Ok, do the normal lease acquisition.
    CfgMgr::instance().clear();

    // Configure DHCP server.
    configure(DECLINE_CONFIGS[config_index], *client.getServer());
    // Explicitly set the client id.
    client.includeClientId(client_id_1);
    // Explicitly set the HW address.
    client.setHWAddress(hw_address_1);
    // Perform 4-way exchange to obtain a new lease.
    acquireLease(client);

    // Let's get the subnet-id and generate statistics name out of it.
    const Subnet4Collection* subnets =
        CfgMgr::instance().getCurrentCfg()->getCfgSubnets4()->getAll();
    ASSERT_EQ(1, subnets->size());
    std::stringstream name;
    name << "subnet[" << (*subnets->begin())->getID() << "].declined-addresses";

    // Set the subnet specific statistic explicitly to zero.
    isc::stats::StatsMgr::instance().setValue(name.str(), static_cast<int64_t>(0));

    // Check the declined-addresses (subnet) statistic before the Decline operation.
    ObservationPtr declined_cnt = StatsMgr::instance().getObservation(name.str());
    ASSERT_TRUE(declined_cnt);
    uint64_t before = declined_cnt->getInteger().first;

    // Check the global declined-addresses statistic before the Decline.
    ObservationPtr declined_global = StatsMgr::instance()
        .getObservation("declined-addresses");
    ASSERT_TRUE(declined_global);
    uint64_t before_global = declined_cnt->getInteger().first;

    // Remember the acquired address.
    IOAddress declined_address = client.config_.lease_.addr_;

    // Explicitly set the client id for DHCPDECLINE.
    client.includeClientId(client_id_2);
    // Explicitly set the HW address for DHCPDECLINE.
    client.setHWAddress(hw_address_2);

    // Send the decline and make sure that the lease is removed from the
    // server.
    ASSERT_NO_THROW(client.doDecline());
    Lease4Ptr lease = LeaseMgrFactory::instance().getLease4(declined_address);

    declined_cnt = StatsMgr::instance().getObservation(name.str());
    ASSERT_TRUE(declined_cnt);
    uint64_t after = declined_cnt->getInteger().first;

    declined_global = StatsMgr::instance().getObservation("declined-addresses");
    ASSERT_TRUE(declined_global);
    uint64_t after_global = declined_global->getInteger().first;

    ASSERT_TRUE(lease);
    // We check if the decline process was successful by checking if the
    // lease is in the database and what is its state.
    if (expected_result == SHOULD_PASS) {
        ASSERT_EQ(Lease::STATE_DECLINED, lease->state_);

        ASSERT_TRUE(lease->hwaddr_);
        ASSERT_TRUE(lease->hwaddr_->hwaddr_.empty());
        ASSERT_FALSE(lease->client_id_);
        ASSERT_TRUE(lease->hostname_.empty());
        ASSERT_FALSE(lease->fqdn_fwd_);
        ASSERT_FALSE(lease->fqdn_rev_);

        // The decline succeeded, so the declined-addresses statistic should
        // be increased by one
        ASSERT_EQ(after, before + 1);
        ASSERT_EQ(after_global, before_global + 1);
    } else {
        // the decline was supposed, to be rejected.
        ASSERT_EQ(Lease::STATE_DEFAULT, lease->state_);

        ASSERT_TRUE(lease->hwaddr_);
        ASSERT_FALSE(lease->hwaddr_->hwaddr_.empty());
        ASSERT_TRUE(lease->client_id_);
        ASSERT_FALSE(lease->fqdn_fwd_);
        ASSERT_FALSE(lease->fqdn_rev_);

        // The decline failed, so the declined-addresses should be the same
        // as before
        ASSERT_EQ(before, after);
        ASSERT_EQ(before_global, after_global);
    }
}

} // end of isc::dhcp::test namespace
} // end of isc::dhcp namespace
} // end of isc namespace

namespace {

/// @brief Test fixture class for testing DHCPDECLINE message handling.
///
/// @todo This class is very similar to ReleaseTest. Maybe we could
/// merge those two classes one day and use derived classes?
class DeclineTest : public Dhcpv4SrvTest {
public:

    /// @brief Constructor.
    ///
    /// Sets up fake interfaces.
    DeclineTest()
        : Dhcpv4SrvTest(),
          iface_mgr_test_config_(true) {
    }

    /// @brief Interface Manager's fake configuration control.
    IfaceMgrTestConfig iface_mgr_test_config_;

};

// This test checks that the client can acquire and decline the lease.
TEST_F(DeclineTest, declineNoIdentifierChangeMemfile) {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    acquireAndDecline(client, "01:02:03:04:05:06", "12:14",
                      "01:02:03:04:05:06", "12:14",
                      SHOULD_PASS);
}

#ifdef HAVE_MYSQL
// This test checks that the client can acquire and decline the lease.
TEST_F(DeclineTest, declineNoIdentifierChangeMySQL) {
    MySqlLeaseMgrInit init;
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    acquireAndDecline(client, "01:02:03:04:05:06", "12:14",
                      "01:02:03:04:05:06", "12:14",
                      SHOULD_PASS, 1);
}
#endif

#ifdef HAVE_PGSQL
// This test checks that the client can acquire and decline the lease.
TEST_F(DeclineTest, declineNoIdentifierChangePgSQL) {
    PgSqlLeaseMgrInit init;
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    acquireAndDecline(client, "01:02:03:04:05:06", "12:14",
                      "01:02:03:04:05:06", "12:14",
                      SHOULD_PASS, 2);
}
#endif

// This test verifies the decline correctness in the following case:
// - Client acquires new lease using HW address only
// - Client sends the DHCPDECLINE with valid HW address and without
//   client identifier.
// - The server successfully declines the lease.
TEST_F(DeclineTest, declineHWAddressOnly) {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    acquireAndDecline(client, "01:02:03:04:05:06", "",
                      "01:02:03:04:05:06", "",
                      SHOULD_PASS);
}

// This test verifies the decline correctness in the following case:
// - Client acquires new lease using the client identifier and HW address
// - Client sends the DHCPDECLINE with valid HW address but with no
//   client identifier.
// - The server successfully declines the lease.
TEST_F(DeclineTest, declineNoClientId) {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    acquireAndDecline(client, "01:02:03:04:05:06", "12:14",
                      "01:02:03:04:05:06", "",
                      SHOULD_PASS);
}

// This test verifies the decline correctness in the following case:
// - Client acquires new lease using HW address
// - Client sends the DHCPDECLINE with valid HW address and some
//   client identifier.
// - The server identifies the lease using HW address and declines
//   this lease.
TEST_F(DeclineTest, declineNoClientId2) {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    acquireAndDecline(client, "01:02:03:04:05:06", "",
                      "01:02:03:04:05:06", "12:14",
                      SHOULD_PASS);
}

// This test checks the server's behavior in the following case:
// - Client acquires new lease using the client identifier and HW address
// - Client sends the DHCPDECLINE with the valid HW address but with invalid
//   client identifier.
// - The server should not remove the lease.
TEST_F(DeclineTest, declineNonMatchingClientId) {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    acquireAndDecline(client, "01:02:03:04:05:06", "12:14",
                      "01:02:03:04:05:06", "12:15:16",
                      SHOULD_FAIL);
}

// This test checks the server's behavior in the following case:
// - Client acquires new lease using client identifier and HW address
// - Client sends the DHCPDECLINE with the same client identifier but
//   different HW address.
// - The server uses client identifier to find the client's lease and
//   declines it.
TEST_F(DeclineTest, declineNonMatchingHWAddress) {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    acquireAndDecline(client, "01:02:03:04:05:06", "12:14",
                      "06:06:06:06:06:06", "12:14",
                      SHOULD_PASS);
}

// This test checks the server's behavior in the following case:
// - Client acquires new lease (address A).
// - Client sends DHCPDECLINE with the requested IP address set to a different
//   address B than it has acquired from the server.
// - Server determines that the client is trying to decline a
//   wrong address and will refuse to decline.
TEST_F(DeclineTest, declineNonMatchingIPAddress) {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DECLINE_CONFIGS[0], *client.getServer());
    // Perform 4-way exchange to obtain a new lease.
    acquireLease(client);

    // Remember the acquired address.
    IOAddress leased_address = client.config_.lease_.addr_;

    // Modify the client's address to force it to decline a different address
    // than it has obtained from the server.
    client.config_.lease_.addr_ = IOAddress(leased_address.toUint32() + 1);

    // Send DHCPDECLINE and make sure it was unsuccessful, i.e. the lease
    // remains in the database.
    ASSERT_NO_THROW(client.doDecline());
    Lease4Ptr lease = LeaseMgrFactory::instance().getLease4(leased_address);
    ASSERT_TRUE(lease);
    EXPECT_EQ(Lease::STATE_DEFAULT, lease->state_);
}

// Test that the released lease cannot be declined.
TEST_F(DeclineTest, declineAfterRelease) {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DECLINE_CONFIGS[0], *client.getServer(), true, true, true, false, LEASE_AFFINITY_ENABLED);
    // Perform 4-way exchange to obtain a new lease.
    acquireLease(client);

    // Remember the acquired address.
    IOAddress leased_address = client.config_.lease_.addr_;

    // Release the acquired lease.
    client.doRelease();

    // Try to decline the released address.
    client.config_.lease_.addr_ = leased_address;
    ASSERT_NO_THROW(client.doDecline());

    // The address should not be declined. It should still be in the
    // released state.
    Lease4Ptr lease = LeaseMgrFactory::instance().getLease4(leased_address);
    ASSERT_TRUE(lease);
    EXPECT_EQ(Lease::STATE_RELEASED, lease->state_);
}

// Test that the expired lease cannot be declined.
TEST_F(DeclineTest, declineAfterExpire) {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DECLINE_CONFIGS[0], *client.getServer());
    // Perform 4-way exchange to obtain a new lease.
    acquireLease(client);

    // Age the lease (expire it).
    auto lease = LeaseMgrFactory::instance().getLease4(client.config_.lease_.addr_);
    ASSERT_TRUE(lease);
    lease->cltt_ -= 7200;
    LeaseMgrFactory::instance().updateLease4(lease);

    // Try to decline the expired lease.
    ASSERT_NO_THROW(client.doDecline());

    // The address should not be declined. It should still be in the
    // default state.
    lease = LeaseMgrFactory::instance().getLease4(lease->addr_);
    ASSERT_TRUE(lease);
    EXPECT_EQ(Lease::STATE_DEFAULT, lease->state_);
}

// Test that expired-reclaimed lease cannot be declined.
TEST_F(DeclineTest, declineAfterReclamation) {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DECLINE_CONFIGS[0], *client.getServer());
    // Perform 4-way exchange to obtain a new lease.
    acquireLease(client);

    // Reclaim the lease.
    auto lease = LeaseMgrFactory::instance().getLease4(client.config_.lease_.addr_);
    ASSERT_TRUE(lease);
    lease->state_ = Lease::STATE_EXPIRED_RECLAIMED;
    LeaseMgrFactory::instance().updateLease4(lease);

    // Try to decline the reclaimed lease.
    ASSERT_NO_THROW(client.doDecline());

    // The address should not be declined. It should still be in the
    // reclaimed state.
    lease = LeaseMgrFactory::instance().getLease4(lease->addr_);
    ASSERT_TRUE(lease);
    EXPECT_EQ(Lease::STATE_EXPIRED_RECLAIMED, lease->state_);
}

} // end of anonymous namespace
