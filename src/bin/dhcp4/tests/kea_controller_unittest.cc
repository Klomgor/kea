// Copyright (C) 2014-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <asiolink/interval_timer.h>
#include <asiolink/io_address.h>
#include <asiolink/io_service.h>
#include <cc/command_interpreter.h>
#include <dhcp/dhcp4.h>
#include <dhcp/hwaddr.h>
#include <dhcp/iface_mgr.h>
#include <dhcp4/ctrl_dhcp4_srv.h>
#include <dhcp4/parser_context.h>
#include <dhcp4/tests/dhcp4_test_utils.h>
#include <dhcpsrv/cb_ctl_dhcp4.h>
#include <dhcpsrv/cfgmgr.h>
#include <dhcpsrv/lease.h>
#include <dhcpsrv/lease_mgr_factory.h>
#include <process/config_base.h>

#ifdef HAVE_MYSQL
#include <mysql/testutils/mysql_schema.h>
#include <hooks/dhcp/mysql/mysql_lease_mgr.h>
#endif

#include <log/logger_support.h>
#include <util/stopwatch.h>

#include <boost/pointer_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include <gtest/gtest.h>

#include <functional>
#include <fstream>
#include <iostream>
#include <signal.h>
#include <sstream>

#include <arpa/inet.h>
#include <unistd.h>

using namespace std;
using namespace isc;
using namespace isc::asiolink;
using namespace isc::config;
using namespace isc::data;
using namespace isc::db;

#ifdef HAVE_MYSQL
using namespace isc::db::test;
#endif

using namespace isc::dhcp;
using namespace isc::dhcp::test;
using namespace isc::hooks;

namespace {

/// @brief Test implementation of the @c CBControlDHCPv4.
///
/// This implementation is installed on the test server instance. It
/// overrides the implementation of the @c databaseConfigFetch function
/// to verify arguments passed to this function and throw an exception
/// when desired in the negative test scenarios. It doesn't do the
/// actual configuration fetch as this is tested elswhere and would
/// require setting up a database configuration backend.
class TestCBControlDHCPv4 : public CBControlDHCPv4 {
public:

    /// @brief Constructor.
    TestCBControlDHCPv4()
        : CBControlDHCPv4(), db_total_config_fetch_calls_(0),
          db_current_config_fetch_calls_(0), db_staging_config_fetch_calls_(0),
          enable_check_fetch_mode_(false), enable_throw_(false) {
    }

    /// @brief Stub implementation of the "fetch" function.
    ///
    /// If this is not the first invocation of this function, it
    /// verifies that the @c fetch_mode has been correctly set to
    /// @c FetchMode::FETCH_UPDATE.
    ///
    /// It also throws an exception when desired by a test, to
    /// verify that the server gracefully handles such exception.
    ///
    /// @param config either the staging or the current configuration.
    /// @param fetch_mode value indicating if the method is called upon the
    /// server start up or it is called to fetch configuration updates.
    ///
    /// @throw Unexpected when configured to do so.
    virtual void databaseConfigFetch(const process::ConfigPtr& config,
                                     const FetchMode& fetch_mode) {
        ++db_total_config_fetch_calls_;

        if (config == CfgMgr::instance().getCurrentCfg()) {
            ++db_current_config_fetch_calls_;
        } else if (config == CfgMgr::instance().getStagingCfg()) {
            ++db_staging_config_fetch_calls_;
        }

        if (enable_check_fetch_mode_) {
            if ((db_total_config_fetch_calls_ <= 1) &&
                (fetch_mode == FetchMode::FETCH_UPDATE)) {
                ADD_FAILURE() << "databaseConfigFetch was called with the value "
                    "of fetch_mode=FetchMode::FETCH_UPDATE upon the server configuration";

            } else if ((db_total_config_fetch_calls_ > 1) &&
                       (fetch_mode == FetchMode::FETCH_ALL)) {
                ADD_FAILURE() << "databaseConfigFetch was called with the value "
                    "of fetch_mode=FetchMode::FETCH_ALL during fetching the updates";
            }
        }

        if (enable_throw_) {
            isc_throw(Unexpected, "testing if exceptions are correctly handled");
        }
    }

    /// @brief Returns number of invocations of the @c databaseConfigFetch
    /// (total).
    size_t getDatabaseTotalConfigFetchCalls() const {
        return (db_total_config_fetch_calls_);
    }

    /// @brief Returns number of invocations of the @c databaseConfigFetch
    /// (current configuration).
    size_t getDatabaseCurrentConfigFetchCalls() const {
        return (db_current_config_fetch_calls_);
    }

    /// @brief Returns number of invocations of the @c databaseConfigFetch
    /// (staging configuration).
    size_t getDatabaseStagingConfigFetchCalls() const {
        return (db_staging_config_fetch_calls_);
    }

    /// @brief Enables checking of the @c fetch_mode value.
    void enableCheckFetchMode() {
        enable_check_fetch_mode_ = true;
    }

    /// @brief Enables the object to throw from @c databaseConfigFetch.
    void enableThrow() {
        enable_throw_ = true;
    }

private:

    /// @brief Counter holding number of invocations of the
    /// @c databaseConfigFetch (total).
    size_t db_total_config_fetch_calls_;

    /// @brief Counter holding number of invocations of the
    /// @c databaseConfigFetch (current configuration).
    size_t db_current_config_fetch_calls_;

    /// @brief Counter holding number of invocations of the
    /// @c databaseConfigFetch (staging configuration).
    size_t db_staging_config_fetch_calls_;

    /// @brief Boolean flag indicated if the value of the @c fetch_mode
    /// should be verified.
    bool enable_check_fetch_mode_;

    /// @brief Boolean flag indicating if the @c databaseConfigFetch should
    /// throw.
    bool enable_throw_;
};

/// @brief Shared pointer to the @c TestCBControlDHCPv4.
typedef boost::shared_ptr<TestCBControlDHCPv4> TestCBControlDHCPv4Ptr;

/// @brief "Naked" DHCPv4 server.
///
/// Exposes internal fields and installs stub implementation of the
/// @c CBControlDHCPv4 object.
class NakedControlledDhcpv4Srv : public ControlledDhcpv4Srv {
public:

    /// @brief Constructor.
    NakedControlledDhcpv4Srv()
        : ControlledDhcpv4Srv(0) {
        // We're replacing the @c CBControlDHCPv4 instance with our
        // stub implementation used in tests.
        cb_control_.reset(new TestCBControlDHCPv4());
    }


    /// @brief Convenience method that enables or disables DHCP service.
    ///
    /// @param enable true to enable service, false to disable it.
    void enableService(bool enable) {
        if (enable) {
            network_state_->enableService(NetworkState::USER_COMMAND);
        } else {
            network_state_->disableService(NetworkState::USER_COMMAND);
        }
    }
};

/// @brief test class for Kea configuration backend.
///
/// This class is used for testing Kea configuration backend.
/// It is very simple and currently focuses on reading
/// config file from disk. It is expected to be expanded in the
/// near future.
class JSONFileBackendTest : public isc::dhcp::test::BaseServerTest {
public:
    JSONFileBackendTest() {
    }

    ~JSONFileBackendTest() {
        LeaseMgrFactory::destroy();
        isc::log::setDefaultLoggingOutput();
        static_cast<void>(remove(TEST_FILE));
        static_cast<void>(remove(TEST_INCLUDE));
    };

    /// @brief writes specified content to a well known file
    ///
    /// Writes specified content to TEST_FILE. Tests will
    /// attempt to read that file.
    ///
    /// @param file_name name of file to be written
    /// @param content content to be written to file
    void writeFile(const std::string& file_name, const std::string& content) {
        static_cast<void>(remove(file_name.c_str()));

        ofstream out(file_name.c_str(), ios::trunc);
        EXPECT_TRUE(out.is_open());
        out << content;
        out.close();
    }

    /// @brief Runs timers for specified time.
    ///
    /// @param io_service Pointer to the IO service to be ran.
    /// @param timeout_ms Amount of time after which the method returns.
    /// @param cond Pointer to the function which if returns true it
    /// stops the IO service and causes the function to return.
    void runTimersWithTimeout(const IOServicePtr& io_service, const long timeout_ms,
                              std::function<bool()> cond = std::function<bool()>()) {
        IntervalTimer timer(io_service);
        std::atomic<bool> stopped(false);
        timer.setup([&io_service, &stopped]() {
            stopped = true;
            io_service->stop();
        }, timeout_ms, IntervalTimer::ONE_SHOT);

        // Run as long as the timeout hasn't occurred and the interrupting
        // condition is not specified or not met.
        while (!stopped && (!cond || !cond())) {
            io_service->runOne();
        }
        io_service->stop();
        io_service->restart();
    }

    /// @brief This test verifies that the timer used to fetch the configuration
    /// updates from the database works as expected.
    void testConfigBackendTimer(const int config_wait_fetch_time,
                                const bool throw_during_fetch = false,
                                const bool call_command = false) {
        std::ostringstream config;
        config <<
            "{ \"Dhcp4\": {"
            "\"interfaces-config\": {"
            "    \"interfaces\": [ ]"
            "},"
            "\"lease-database\": {"
            "     \"type\": \"memfile\","
            "     \"persist\": false"
            "},"
            "\"config-control\": {"
            "     \"config-fetch-wait-time\": " << config_wait_fetch_time <<
            "},"
            "\"rebind-timer\": 2000, "
            "\"renew-timer\": 1000, \n"
            "\"subnet4\": [ ],"
            "\"valid-lifetime\": 4000 }"
            "}";
        writeFile(TEST_FILE, config.str());

        // Create an instance of the server and initialize it.
        boost::scoped_ptr<NakedControlledDhcpv4Srv> srv;
        ASSERT_NO_THROW(srv.reset(new NakedControlledDhcpv4Srv()));
        ASSERT_NO_THROW(srv->init(TEST_FILE));

        // Get the CBControlDHCPv4 object belonging to this server.
        auto cb_control = boost::dynamic_pointer_cast<TestCBControlDHCPv4>(srv->getCBControl());

        // Verify that the parameter passed to the databaseConfigFetch has an
        // expected value.
        cb_control->enableCheckFetchMode();

        // Instruct our stub implementation of the CBControlDHCPv4 to throw as a
        // result of fetch if desired.
        if (throw_during_fetch) {
            cb_control->enableThrow();
        }

        // So far there should be exactly one attempt to fetch the configuration
        // from the backend. That's the attempt made upon startup on
        // the staging configuration.
        // All other fetches will be on the current configuration:
        //  - the timer makes a closure with the staging one but it is
        //    committed so becomes the current one.
        //  - the command is called outside configuration so it must
        //    be the current configuration. The test explicitly checks this.
        EXPECT_EQ(1, cb_control->getDatabaseTotalConfigFetchCalls());
        EXPECT_EQ(0, cb_control->getDatabaseCurrentConfigFetchCalls());
        EXPECT_EQ(1, cb_control->getDatabaseStagingConfigFetchCalls());

        if (call_command) {
            // The case where there is no backend is tested in the
            // controlled server tests so we have only to verify
            // that the command calls the database config fetch.

            // Count the startup.
            EXPECT_EQ(cb_control->getDatabaseTotalConfigFetchCalls(), 1);
            EXPECT_EQ(cb_control->getDatabaseCurrentConfigFetchCalls(), 0);
            EXPECT_EQ(cb_control->getDatabaseStagingConfigFetchCalls(), 1);

            ConstElementPtr list_cmds = createCommand("config-backend-pull");
            ConstElementPtr result = CommandMgr::instance().processCommand(list_cmds);
            EXPECT_EQ(cb_control->getDatabaseTotalConfigFetchCalls(), 2);
            std::string expected;

            if (throw_during_fetch) {
                expected = "{ \"result\": 1, \"text\": ";
                expected += "\"On demand configuration update failed: ";
                expected += "testing if exceptions are correctly handled\" }";
            } else {
                expected = "{ \"result\": 0, \"text\": ";
                expected += "\"On demand configuration update successful.\" }";
            }
            EXPECT_EQ(expected, result->str());

            // No good way to check the rescheduling...
            ASSERT_NO_THROW(runTimersWithTimeout(srv->getIOService(), 20));

            if (config_wait_fetch_time > 0) {
                EXPECT_GE(cb_control->getDatabaseTotalConfigFetchCalls(), 5);
                EXPECT_GE(cb_control->getDatabaseCurrentConfigFetchCalls(), 4);
                EXPECT_EQ(cb_control->getDatabaseStagingConfigFetchCalls(), 1);
            } else {
                EXPECT_EQ(cb_control->getDatabaseTotalConfigFetchCalls(), 2);
                EXPECT_EQ(cb_control->getDatabaseCurrentConfigFetchCalls(), 1);
                EXPECT_EQ(cb_control->getDatabaseStagingConfigFetchCalls(), 1);
            }

        } else if ((config_wait_fetch_time > 0) && (!throw_during_fetch)) {
            // If we're configured to run the timer, we expect that it was
            // invoked at least 3 times. This is sufficient to verify that
            // the timer was scheduled and that the timer continued to run
            // even when an exception occurred during fetch (that's why it
            // is 3 not 2).
            ASSERT_NO_THROW(runTimersWithTimeout(srv->getIOService(), 500,
                [cb_control]() {
                    // Interrupt the timers poll if we have recorded at
                    // least 3 attempts to fetch the updates.
                    return (cb_control->getDatabaseTotalConfigFetchCalls() >= 3);
                }));
            EXPECT_GE(cb_control->getDatabaseTotalConfigFetchCalls(), 3);
            EXPECT_GE(cb_control->getDatabaseCurrentConfigFetchCalls(), 2);
            EXPECT_EQ(cb_control->getDatabaseStagingConfigFetchCalls(), 1);

        } else {
            ASSERT_NO_THROW(runTimersWithTimeout(srv->getIOService(), 500));

            if (throw_during_fetch) {
                // If we're simulating the failure condition the number
                // of consecutive failures should not exceed 10. Therefore
                // the number of recorded fetches should be 12. One at
                // startup, 10 failures and one that causes the timer
                // to stop.
                EXPECT_EQ(12, cb_control->getDatabaseTotalConfigFetchCalls());
                EXPECT_EQ(11, cb_control->getDatabaseCurrentConfigFetchCalls());
                EXPECT_EQ(1, cb_control->getDatabaseStagingConfigFetchCalls());

            } else {
                // If the server is not configured to schedule the timer,
                // we should still have one fetch attempt recorded.
                EXPECT_EQ(1, cb_control->getDatabaseTotalConfigFetchCalls());
                EXPECT_EQ(0, cb_control->getDatabaseCurrentConfigFetchCalls());
                EXPECT_EQ(1, cb_control->getDatabaseStagingConfigFetchCalls());
            }
        }
    }

    /// Name of a config file used during tests
    static const char* TEST_FILE;
    static const char* TEST_INCLUDE;
};

const char* JSONFileBackendTest::TEST_FILE  = "test-config.json";
const char* JSONFileBackendTest::TEST_INCLUDE = "test-include.json";

// This test checks if configuration can be read from a JSON file.
TEST_F(JSONFileBackendTest, jsonFile) {

    // Prepare configuration file.
    string config = "{ \"Dhcp4\": {"
        "\"interfaces-config\": {"
        "    \"interfaces\": [ \"*\" ]"
        "},"
        "\"rebind-timer\": 2000, "
        "\"renew-timer\": 1000, "
        "\"subnet4\": [ { "
        "    \"pools\": [ { \"pool\": \"192.0.2.1 - 192.0.2.100\" } ],"
        "    \"id\": 1, "
        "    \"subnet\": \"192.0.2.0/24\" "
        " },"
        " {"
        "    \"pools\": [ { \"pool\": \"192.0.3.101 - 192.0.3.150\" } ],"
        "    \"subnet\": \"192.0.3.0/24\", "
        "    \"id\": 2 "
        " },"
        " {"
        "    \"pools\": [ { \"pool\": \"192.0.4.101 - 192.0.4.150\" } ],"
        "    \"id\": 3, "
        "    \"subnet\": \"192.0.4.0/24\" "
        " } ],"
        "\"valid-lifetime\": 4000 }"
        "}";

    writeFile(TEST_FILE, config);

    // Now initialize the server
    boost::scoped_ptr<ControlledDhcpv4Srv> srv;
    ASSERT_NO_THROW(
        srv.reset(new ControlledDhcpv4Srv(0))
    );

    // And configure it using the config file.
    EXPECT_NO_THROW(srv->init(TEST_FILE));

    // Now check if the configuration has been applied correctly.
    const Subnet4Collection* subnets =
        CfgMgr::instance().getCurrentCfg()->getCfgSubnets4()->getAll();
    ASSERT_TRUE(subnets);
    ASSERT_EQ(3, subnets->size()); // We expect 3 subnets.

    // Check subnet 1.
    auto subnet = subnets->begin();
    ASSERT_TRUE(subnet != subnets->end());
    EXPECT_EQ("192.0.2.0", (*subnet)->get().first.toText());
    EXPECT_EQ(24, (*subnet)->get().second);

    // Check pools in the first subnet.
    const PoolCollection& pools1 = (*subnet)->getPools(Lease::TYPE_V4);
    ASSERT_EQ(1, pools1.size());
    EXPECT_EQ("192.0.2.1", pools1.at(0)->getFirstAddress().toText());
    EXPECT_EQ("192.0.2.100", pools1.at(0)->getLastAddress().toText());
    EXPECT_EQ(Lease::TYPE_V4, pools1.at(0)->getType());

    // Check subnet 2.
    ++subnet;
    ASSERT_TRUE(subnet != subnets->end());
    EXPECT_EQ("192.0.3.0", (*subnet)->get().first.toText());
    EXPECT_EQ(24, (*subnet)->get().second);

    // Check pools in the second subnet.
    const PoolCollection& pools2 = (*subnet)->getPools(Lease::TYPE_V4);
    ASSERT_EQ(1, pools2.size());
    EXPECT_EQ("192.0.3.101", pools2.at(0)->getFirstAddress().toText());
    EXPECT_EQ("192.0.3.150", pools2.at(0)->getLastAddress().toText());
    EXPECT_EQ(Lease::TYPE_V4, pools2.at(0)->getType());

    // And finally check subnet 3.
    ++subnet;
    ASSERT_TRUE(subnet != subnets->end());
    EXPECT_EQ("192.0.4.0", (*subnet)->get().first.toText());
    EXPECT_EQ(24, (*subnet)->get().second);

    // ... and its only pool.
    const PoolCollection& pools3 = (*subnet)->getPools(Lease::TYPE_V4);
    EXPECT_EQ("192.0.4.101", pools3.at(0)->getFirstAddress().toText());
    EXPECT_EQ("192.0.4.150", pools3.at(0)->getLastAddress().toText());
    EXPECT_EQ(Lease::TYPE_V4, pools3.at(0)->getType());
}

// This test checks if configuration can be read from a JSON file
// using hash (#) line comments
TEST_F(JSONFileBackendTest, hashComments) {

    string config_hash_comments = "# This is a comment. It should be \n"
        "#ignored. Real config starts in line below\n"
        "{ \"Dhcp4\": {"
        "\"interfaces-config\": {"
        "    \"interfaces\": [ \"*\" ]"
        "},"
        "\"rebind-timer\": 2000, "
        "\"renew-timer\": 1000, \n"
        "# comments in the middle should be ignored, too\n"
        "\"subnet4\": [ { "
        "    \"pools\": [ { \"pool\": \"192.0.2.0/24\" } ],"
        "    \"id\": 1, "
        "    \"subnet\": \"192.0.2.0/22\" "
        " } ],"
        "\"valid-lifetime\": 4000 }"
        "}";

    writeFile(TEST_FILE, config_hash_comments);

    // Now initialize the server
    boost::scoped_ptr<ControlledDhcpv4Srv> srv;
    ASSERT_NO_THROW(
        srv.reset(new ControlledDhcpv4Srv(0))
    );

    // And configure it using config with comments.
    EXPECT_NO_THROW(srv->init(TEST_FILE));

    // Now check if the configuration has been applied correctly.
    const Subnet4Collection* subnets =
        CfgMgr::instance().getCurrentCfg()->getCfgSubnets4()->getAll();
    ASSERT_TRUE(subnets);
    ASSERT_EQ(1, subnets->size());

    // Check subnet 1.
    auto subnet = subnets->begin();
    ASSERT_TRUE(subnet != subnets->end());
    EXPECT_EQ("192.0.2.0", (*subnet)->get().first.toText());
    EXPECT_EQ(22, (*subnet)->get().second);

    // Check pools in the first subnet.
    const PoolCollection& pools1 = (*subnet)->getPools(Lease::TYPE_V4);
    ASSERT_EQ(1, pools1.size());
    EXPECT_EQ("192.0.2.0", pools1.at(0)->getFirstAddress().toText());
    EXPECT_EQ("192.0.2.255", pools1.at(0)->getLastAddress().toText());
    EXPECT_EQ(Lease::TYPE_V4, pools1.at(0)->getType());
}

// This test checks if configuration can be read from a JSON file
// using C++ line (//) comments.
TEST_F(JSONFileBackendTest, cppLineComments) {

    string config_cpp_line_comments = "// This is a comment. It should be \n"
        "//ignored. Real config starts in line below\n"
        "{ \"Dhcp4\": {"
        "\"interfaces-config\": {"
        "    \"interfaces\": [ \"*\" ]"
        "},"
        "\"rebind-timer\": 2000, "
        "\"renew-timer\": 1000, \n"
        "// comments in the middle should be ignored, too\n"
        "\"subnet4\": [ { "
        "    \"pools\": [ { \"pool\": \"192.0.2.0/24\" } ],"
        "    \"id\": 1, "
        "    \"subnet\": \"192.0.2.0/22\" "
        " } ],"
        "\"valid-lifetime\": 4000 }"
        "}";

    writeFile(TEST_FILE, config_cpp_line_comments);

    // Now initialize the server
    boost::scoped_ptr<ControlledDhcpv4Srv> srv;
    ASSERT_NO_THROW(
        srv.reset(new ControlledDhcpv4Srv(0))
    );

    // And configure it using config with comments.
    EXPECT_NO_THROW(srv->init(TEST_FILE));

    // Now check if the configuration has been applied correctly.
    const Subnet4Collection* subnets =
        CfgMgr::instance().getCurrentCfg()->getCfgSubnets4()->getAll();
    ASSERT_TRUE(subnets);
    ASSERT_EQ(1, subnets->size());

    // Check subnet 1.
    auto subnet = subnets->begin();
    ASSERT_TRUE(subnet != subnets->end());
    EXPECT_EQ("192.0.2.0", (*subnet)->get().first.toText());
    EXPECT_EQ(22, (*subnet)->get().second);

    // Check pools in the first subnet.
    const PoolCollection& pools1 = (*subnet)->getPools(Lease::TYPE_V4);
    ASSERT_EQ(1, pools1.size());
    EXPECT_EQ("192.0.2.0", pools1.at(0)->getFirstAddress().toText());
    EXPECT_EQ("192.0.2.255", pools1.at(0)->getLastAddress().toText());
    EXPECT_EQ(Lease::TYPE_V4, pools1.at(0)->getType());
}

// This test checks if configuration can be read from a JSON file
// using C block (/* */) comments
TEST_F(JSONFileBackendTest, cBlockComments) {

    string config_c_block_comments = "/* This is a comment. It should be \n"
        "ignored. Real config starts in line below*/\n"
        "{ \"Dhcp4\": {"
        "\"interfaces-config\": {"
        "    \"interfaces\": [ \"*\" ]"
        "},"
        "\"rebind-timer\": 2000, "
        "\"renew-timer\": 1000, \n"
        "/* comments in the middle should be ignored, too*/\n"
        "\"subnet4\": [ { "
        "    \"pools\": [ { \"pool\": \"192.0.2.0/24\" } ],"
        "    \"id\": 1, "
        "    \"subnet\": \"192.0.2.0/22\" "
        " } ],"
        "\"valid-lifetime\": 4000 }"
        "}";

    writeFile(TEST_FILE, config_c_block_comments);

    // Now initialize the server
    boost::scoped_ptr<ControlledDhcpv4Srv> srv;
    ASSERT_NO_THROW(
        srv.reset(new ControlledDhcpv4Srv(0))
    );

    // And configure it using config with comments.
    EXPECT_NO_THROW(srv->init(TEST_FILE));

    // Now check if the configuration has been applied correctly.
    const Subnet4Collection* subnets =
        CfgMgr::instance().getCurrentCfg()->getCfgSubnets4()->getAll();
    ASSERT_TRUE(subnets);
    ASSERT_EQ(1, subnets->size());

    // Check subnet 1.
    auto subnet = subnets->begin();
    ASSERT_TRUE(subnet != subnets->end());
    EXPECT_EQ("192.0.2.0", (*subnet)->get().first.toText());
    EXPECT_EQ(22, (*subnet)->get().second);

    // Check pools in the first subnet.
    const PoolCollection& pools1 = (*subnet)->getPools(Lease::TYPE_V4);
    ASSERT_EQ(1, pools1.size());
    EXPECT_EQ("192.0.2.0", pools1.at(0)->getFirstAddress().toText());
    EXPECT_EQ("192.0.2.255", pools1.at(0)->getLastAddress().toText());
    EXPECT_EQ(Lease::TYPE_V4, pools1.at(0)->getType());
}

// This test checks if configuration can be read from a JSON file
// using an include file.
TEST_F(JSONFileBackendTest, include) {

    string config_hash_comments = "{ \"Dhcp4\": {"
        "\"interfaces-config\": {"
        "    \"interfaces\": [ \"*\" ]"
        "},"
        "\"rebind-timer\": 2000, "
        "\"renew-timer\": 1000, \n"
        "<?include \"" + string(TEST_INCLUDE) + "\"?>,"
        "\"valid-lifetime\": 4000 }"
        "}";
    string include = "\n"
        "\"subnet4\": [ { "
        "    \"pools\": [ { \"pool\": \"192.0.2.0/24\" } ],"
        "    \"id\": 1, "
        "    \"subnet\": \"192.0.2.0/22\" "
        " } ]\n";

    writeFile(TEST_FILE, config_hash_comments);
    writeFile(TEST_INCLUDE, include);

    // Now initialize the server
    boost::scoped_ptr<ControlledDhcpv4Srv> srv;
    ASSERT_NO_THROW(
        srv.reset(new ControlledDhcpv4Srv(0))
    );

    // And configure it using config with comments.
    EXPECT_NO_THROW(srv->init(TEST_FILE));

    // Now check if the configuration has been applied correctly.
    const Subnet4Collection* subnets =
        CfgMgr::instance().getCurrentCfg()->getCfgSubnets4()->getAll();
    ASSERT_TRUE(subnets);
    ASSERT_EQ(1, subnets->size());

    // Check subnet 1.
    auto subnet = subnets->begin();
    ASSERT_TRUE(subnet != subnets->end());
    EXPECT_EQ("192.0.2.0", (*subnet)->get().first.toText());
    EXPECT_EQ(22, (*subnet)->get().second);

    // Check pools in the first subnet.
    const PoolCollection& pools1 = (*subnet)->getPools(Lease::TYPE_V4);
    ASSERT_EQ(1, pools1.size());
    EXPECT_EQ("192.0.2.0", pools1.at(0)->getFirstAddress().toText());
    EXPECT_EQ("192.0.2.255", pools1.at(0)->getLastAddress().toText());
    EXPECT_EQ(Lease::TYPE_V4, pools1.at(0)->getType());
}

// This test checks if recursive include of a file is detected
TEST_F(JSONFileBackendTest, recursiveInclude) {

    string config_recursive_include = "{ \"Dhcp4\": {"
        "\"interfaces-config\": {"
        "  \"interfaces\": [ <?include \"" + string(TEST_INCLUDE) + "\"?> ]"
        "},"
        "\"rebind-timer\": 2000, "
        "\"renew-timer\": 1000, \n"
        "\"subnet4\": [ { "
        "    \"pools\": [ { \"pool\": \"192.0.2.0/24\" } ],"
        "    \"id\": 1, "
        "    \"subnet\": \"192.0.2.0/22\" "
        " } ],"
        "\"valid-lifetime\": 4000 }"
        "}";
    string include = "\"eth\", <?include \"" + string(TEST_INCLUDE) + "\"?>";
    string msg = "configuration error using file '" + string(TEST_FILE) +
        "': Too many nested include.";

    writeFile(TEST_FILE, config_recursive_include);
    writeFile(TEST_INCLUDE, include);

    // Now initialize the server
    boost::scoped_ptr<ControlledDhcpv4Srv> srv;
    ASSERT_NO_THROW(
        srv.reset(new ControlledDhcpv4Srv(0))
    );

    // And configure it using config with comments.
    try {
        srv->init(TEST_FILE);
        FAIL() << "Expected Dhcp4ParseError but nothing was raised";
    }
    catch (const Exception& ex) {
        EXPECT_EQ(msg, ex.what());
    }
}

// This test checks if configuration detects failure when trying:
// - empty file
// - empty filename
// - no Dhcp4 element
// - Config file that contains Dhcp4 but has a content error
TEST_F(JSONFileBackendTest, configBroken) {

    // Empty config is not allowed, because Dhcp4 element is missing
    string config_empty = "";

    // This config does not have mandatory Dhcp4 element
    string config_v6 = "{ \"Dhcp6\": { \"interfaces\": [ \"*\" ],"
        "\"preferred-lifetime\": 3000,"
        "\"rebind-timer\": 2000, "
        "\"renew-timer\": 1000, "
        "\"subnet4\": [ { "
        "    \"pool\": [ \"2001:db8::/80\" ],"
        "    \"id\": 1, "
        "    \"subnet\": \"2001:db8::/64\" "
        " } ]}";

    // This has Dhcp4 element, but it's utter nonsense
    string config_nonsense = "{ \"Dhcp4\": { \"reviews\": \"are so much fun\" } }";

    // Now initialize the server
    boost::scoped_ptr<ControlledDhcpv4Srv> srv;
    ASSERT_NO_THROW(
        srv.reset(new ControlledDhcpv4Srv(0))
    );

    // Try to configure without filename. Should fail.
    EXPECT_THROW(srv->init(""), BadValue);

    // Try to configure it using empty file. Should fail.
    writeFile(TEST_FILE, config_empty);
    EXPECT_THROW(srv->init(TEST_FILE), BadValue);

    // Now try to load a config that does not have Dhcp4 component.
    writeFile(TEST_FILE, config_v6);
    EXPECT_THROW(srv->init(TEST_FILE), BadValue);

    // Now try to load a config with Dhcp4 full of nonsense.
    writeFile(TEST_FILE, config_nonsense);
    EXPECT_THROW(srv->init(TEST_FILE), BadValue);
}

/// This unit-test reads all files enumerated in configs-test.txt file, loads
/// each of them and verify that they can be loaded.
///
/// @todo: Unfortunately, we have this test disabled, because all loaded
/// configs use memfile, which attempts to create lease file in
/// /usr/local/var/lib/kea/kea-leases4.csv. We have couple options here:
/// a) disable persistence in example configs - a very bad thing to do
///    as users will forget to reenable it and then will be surprised when their
///    leases disappear
/// b) change configs to store lease file in /tmp. It's almost as bad as the
///    previous one. Users will then be displeased when all their leases are
///    wiped. (most systems wipe /tmp during boot)
/// c) read each config and rewrite it on the fly, so persistence is disabled.
///    This is probably the way to go, but this is a work for a dedicated ticket.
///
/// Hence I'm leaving the test in, but it is disabled.
TEST_F(JSONFileBackendTest, DISABLED_loadAllConfigs) {

    // Create server first
    boost::scoped_ptr<ControlledDhcpv4Srv> srv;
    ASSERT_NO_THROW(
        srv.reset(new ControlledDhcpv4Srv(0))
    );

    const char* configs_list = "configs-list.txt";
    fstream configs(configs_list, ios::in);
    ASSERT_TRUE(configs.is_open());
    std::string config_name;
    while (std::getline(configs, config_name)) {

        // Ignore empty and commented lines
        if (config_name.empty() || config_name[0] == '#') {
            continue;
        }

        // Unit-tests usually do not print out anything, but in this case I
        // think printing out tests configs is warranted.
        std::cout << "Loading config file " << config_name << std::endl;

        try {
            srv->init(config_name);
        } catch (const std::exception& ex) {
            ADD_FAILURE() << "Exception thrown" << ex.what() << endl;
        }
    }
}

// This test verifies that the DHCP server installs the timers for reclaiming
// and flushing expired leases.
TEST_F(JSONFileBackendTest, timers) {
    // This is a basic configuration which enables timers for reclaiming
    // expired leases and flushing them after 500 seconds since they expire.
    // Both timers run at 1 second intervals.
    string config =
        "{ \"Dhcp4\": {"
        "\"interfaces-config\": {"
        "    \"interfaces\": [ ]"
        "},"
        "\"lease-database\": {"
        "     \"type\": \"memfile\","
        "     \"persist\": false"
        "},"
        "\"expired-leases-processing\": {"
        "     \"reclaim-timer-wait-time\": 1,"
        "     \"hold-reclaimed-time\": 500,"
        "     \"flush-reclaimed-timer-wait-time\": 1"
        "},"
        "\"rebind-timer\": 2000, "
        "\"renew-timer\": 1000, \n"
        "\"subnet4\": [ ],"
        "\"valid-lifetime\": 4000 }"
        "}";
    writeFile(TEST_FILE, config);

    // Create an instance of the server and initialize it.
    boost::scoped_ptr<ControlledDhcpv4Srv> srv;
    ASSERT_NO_THROW(srv.reset(new ControlledDhcpv4Srv(0)));
    ASSERT_NO_THROW(srv->init(TEST_FILE));

    // Create an expired lease. The lease is expired by 40 seconds ago
    // (valid lifetime = 60, cltt = now - 100). The lease will be reclaimed
    // but shouldn't be flushed in the database because the reclaimed are
    // held in the database 500 seconds after reclamation, according to the
    // current configuration.
    HWAddrPtr hwaddr_expired(new HWAddr(HWAddr::fromText("00:01:02:03:04:05")));
    Lease4Ptr lease_expired(new Lease4(IOAddress("10.0.0.1"), hwaddr_expired,
                                       ClientIdPtr(), 60,
                                       time(NULL) - 100, SubnetID(1)));

    // Create expired-reclaimed lease. The lease has expired 1000 - 60 seconds
    // ago. It should be removed from the lease database when the "flush" timer
    // goes off.
    HWAddrPtr hwaddr_reclaimed(new HWAddr(HWAddr::fromText("01:02:03:04:05:06")));
    Lease4Ptr lease_reclaimed(new Lease4(IOAddress("10.0.0.2"), hwaddr_reclaimed,
                                         ClientIdPtr(), 60,
                                         time(NULL) - 1000, SubnetID(1)));
    lease_reclaimed->state_ = Lease4::STATE_EXPIRED_RECLAIMED;

    // Add leases to the database.
    LeaseMgr& lease_mgr = LeaseMgrFactory::instance();
    ASSERT_NO_THROW(lease_mgr.addLease(lease_expired));
    ASSERT_NO_THROW(lease_mgr.addLease(lease_reclaimed));

    // Make sure they have been added.
    ASSERT_TRUE(lease_mgr.getLease4(IOAddress("10.0.0.1")));
    ASSERT_TRUE(lease_mgr.getLease4(IOAddress("10.0.0.2")));

    // Poll the timers for a while to make sure that each of them is executed
    // at least once.
    ASSERT_NO_THROW(runTimersWithTimeout(srv->getIOService(), 5000));

    // Verify that the leases in the database have been processed as expected.

    // First lease should be reclaimed, but not removed.
    ASSERT_NO_THROW(lease_expired = lease_mgr.getLease4(IOAddress("10.0.0.1")));
    ASSERT_TRUE(lease_expired);
    EXPECT_TRUE(lease_expired->stateExpiredReclaimed());

    // Second lease should have been removed.
    ASSERT_NO_THROW(
        lease_reclaimed = lease_mgr.getLease4(IOAddress("10.0.0.2"));
    );
    EXPECT_FALSE(lease_reclaimed);
}

// This test verifies that the server uses default (Memfile) lease database
// backend when no backend is explicitly specified in the configuration.
TEST_F(JSONFileBackendTest, defaultLeaseDbBackend) {
    // This is basic server configuration which excludes lease database
    // backend specification. The default Memfile backend should be
    // initialized in this case.
    string config =
        "{ \"Dhcp4\": {"
        "\"interfaces-config\": {"
        "    \"interfaces\": [ ]"
        "},"
        "\"rebind-timer\": 2000, "
        "\"renew-timer\": 1000, \n"
        "\"subnet4\": [ ],"
        "\"valid-lifetime\": 4000 }"
        "}";
    writeFile(TEST_FILE, config);

    // Create an instance of the server and initialize it.
    boost::scoped_ptr<ControlledDhcpv4Srv> srv;
    ASSERT_NO_THROW(srv.reset(new ControlledDhcpv4Srv(0)));
    ASSERT_NO_THROW(srv->init(TEST_FILE));

    // The backend should have been created.
    EXPECT_NO_THROW(static_cast<void>(LeaseMgrFactory::instance()));
}

// This test verifies that the timer triggering configuration updates
// is invoked according to the configured value of the
// config-fetch-wait-time.
TEST_F(JSONFileBackendTest, configBackendTimer) {
    testConfigBackendTimer(1);
}

// This test verifies that the timer for triggering configuration updates
// is not invoked when the value of the config-fetch-wait-time is set
// to 0.
TEST_F(JSONFileBackendTest, configBackendTimerDisabled) {
    testConfigBackendTimer(0);
}

// This test verifies that the server will gracefully handle exceptions
// thrown from the CBControlDHCPv4::databaseConfigFetch, i.e. will
// reschedule the timer.
TEST_F(JSONFileBackendTest, configBackendTimerWithThrow) {
    // The true value instructs the test to throw during the fetch.
    testConfigBackendTimer(1, true);
}

// This test verifies that the server will be updated by the
// config-backend-pull command.
TEST_F(JSONFileBackendTest, configBackendPullCommand) {
    testConfigBackendTimer(0, false, true);
}

// This test verifies that the server will be updated by the
// config-backend-pull command even when updates fail.
TEST_F(JSONFileBackendTest, configBackendPullCommandWithThrow) {
    testConfigBackendTimer(0, true, true);
}

// This test verifies that the server will be updated by the
// config-backend-pull command and the timer rescheduled.
TEST_F(JSONFileBackendTest, configBackendPullCommandWithTimer) {
    testConfigBackendTimer(1, false, true);
}

// Starting tests which require MySQL backend availability. Those tests
// will not be executed if Kea has been compiled without MySQL support.
#ifdef HAVE_MYSQL

/// @brief Test fixture class for the tests utilizing MySQL database
/// backend.
class JSONFileBackendMySQLTest : public JSONFileBackendTest {
public:

    /// @brief Constructor.
    ///
    /// Recreates MySQL schema for a test.
    JSONFileBackendMySQLTest() : JSONFileBackendTest() {
        // Ensure we have the proper schema with no transient data.
        createMySQLSchema();
    }

    /// @brief Destructor.
    ///
    /// Destroys MySQL schema.
    virtual ~JSONFileBackendMySQLTest() {
        // If data wipe enabled, delete transient data otherwise destroy the schema.
        destroyMySQLSchema();
    }

    /// @brief Creates server configuration with specified backend type.
    ///
    /// @param backend Backend type or empty string to indicate that the
    /// backend configuration should not be placed in the resulting
    /// JSON configuration.
    ///
    /// @return Server configuration.
    std::string createConfiguration(const std::string& backend) const;

    /// @brief Test reconfiguration with a backend change.
    ///
    /// If any of the parameters is an empty string it indicates that the
    /// created configuration should exclude backend configuration.
    ///
    /// @param backend_first Type of a backend to be used initially.
    /// @param backend_second Type of a backend to be used after
    /// reconfiguration.
    void testBackendReconfiguration(const std::string& backend_first,
                                    const std::string& backend_second);

    /// @brief Initializer.
    MySqlLeaseMgrInit init_;
};

std::string
JSONFileBackendMySQLTest::createConfiguration(const std::string& backend) const {
    // This is basic server configuration which excludes lease database
    // backend specification. The default Memfile backend should be
    // initialized in this case.
    std::ostringstream config;
    config <<
        "{ \"Dhcp4\": {"
        "\"interfaces-config\": {"
        "    \"interfaces\": [ ]"
        "},";

    // For non-empty lease backend type we have to add a backend configuration
    // section.
    if (!backend.empty()) {
        config <<
        "\"lease-database\": {"
        "     \"type\": \"" << backend << "\"";

        // SQL backends require database credentials.
        if (backend != "memfile") {
            config <<
                ","
                "     \"name\": \"keatest\","
                "     \"user\": \"keatest\","
                "     \"password\": \"keatest\"";
        }
        config << "},";
    }

    // Append the rest of the configuration.
    config <<
        "\"rebind-timer\": 2000, "
        "\"renew-timer\": 1000, \n"
        "\"subnet4\": [ ],"
        "\"valid-lifetime\": 4000 }"
        "}";

    return (config.str());
}

void
JSONFileBackendMySQLTest::
testBackendReconfiguration(const std::string& backend_first,
                           const std::string& backend_second) {
    writeFile(TEST_FILE, createConfiguration(backend_first));

    // Create an instance of the server and initialize it.
    boost::scoped_ptr<NakedControlledDhcpv4Srv> srv;
    ASSERT_NO_THROW(srv.reset(new NakedControlledDhcpv4Srv()));
    srv->setConfigFile(TEST_FILE);
    ASSERT_NO_THROW(srv->init(TEST_FILE));

    // The backend should have been created and its type should be
    // correct.
    ASSERT_NO_THROW(static_cast<void>(LeaseMgrFactory::instance()));
    EXPECT_EQ(backend_first.empty() ? "memfile" : backend_first,
              LeaseMgrFactory::instance().getType());

    // New configuration modifies the lease database backend type.
    writeFile(TEST_FILE, createConfiguration(backend_second));

    // Explicitly calling signal handler for SIGHUP to trigger server
    // reconfiguration.
    raise(SIGHUP);

    // Polling once to be sure that the signal handle has been called.
    srv->getIOService()->poll();

    // The backend should have been created and its type should be
    // correct.
    ASSERT_NO_THROW(static_cast<void>(LeaseMgrFactory::instance()));
    EXPECT_EQ(backend_second.empty() ? "memfile" : backend_second,
              LeaseMgrFactory::instance().getType());
}

// This test verifies that backend specification can be added on
// server reconfiguration.
TEST_F(JSONFileBackendMySQLTest, reconfigureBackendUndefinedToMySQL) {
    testBackendReconfiguration("", "mysql");
}

// This test verifies that when backend specification is removed the
// default backend is used.
TEST_F(JSONFileBackendMySQLTest, reconfigureBackendMySQLToUndefined) {
    testBackendReconfiguration("mysql", "");
}

// This test verifies that backend type can be changed from Memfile
// to MySQL.
TEST_F(JSONFileBackendMySQLTest, reconfigureBackendMemfileToMySQL) {
    testBackendReconfiguration("memfile", "mysql");
}

#endif

// This test verifies that the DHCP server only reclaims or flushes leases
// when DHCP6 service is enabled.
TEST_F(JSONFileBackendTest, reclaimOnlyWhenServiceEnabled) {
    // This is a basic configuration which enables timers for reclaiming
    // expired leases and flushing them after 500 seconds since they expire.
    // Both timers run at 1 second intervals.
    string config =
        "{ \"Dhcp4\": {"
        "\"interfaces-config\": {"
        "    \"interfaces\": [ ]"
        "},"
        "\"lease-database\": {"
        "     \"type\": \"memfile\","
        "     \"persist\": false"
        "},"
        "\"expired-leases-processing\": {"
        "     \"reclaim-timer-wait-time\": 1,"
        "     \"hold-reclaimed-time\": 500,"
        "     \"flush-reclaimed-timer-wait-time\": 1"
        "},"
        "\"rebind-timer\": 2000, "
        "\"renew-timer\": 1000, \n"
        "\"subnet4\": [ ],"
        "\"valid-lifetime\": 4000 }"
        "}";
    writeFile(TEST_FILE, config);

    // Create an instance of the server and initialize it.
    boost::scoped_ptr<NakedControlledDhcpv4Srv> srv;
    ASSERT_NO_THROW(srv.reset(new NakedControlledDhcpv4Srv()));
    ASSERT_NO_THROW(srv->init(TEST_FILE));

    // Create an expired lease. The lease is expired by 40 seconds ago
    // (valid lifetime = 60, cltt = now - 100). The lease will be reclaimed
    // but shouldn't be flushed in the database because the reclaimed are
    // held in the database 500 seconds after reclamation, according to the
    // current configuration.
    HWAddrPtr hwaddr_expired(new HWAddr(HWAddr::fromText("00:01:02:03:04:05")));
    Lease4Ptr lease_expired(new Lease4(IOAddress("10.0.0.1"), hwaddr_expired,
                                       ClientIdPtr(), 60,
                                       time(NULL) - 100, SubnetID(1)));

    // Create expired-reclaimed lease. The lease has expired 1000 - 60 seconds
    // ago. It should be removed from the lease database when the "flush" timer
    // goes off.
    HWAddrPtr hwaddr_reclaimed(new HWAddr(HWAddr::fromText("01:02:03:04:05:06")));
    Lease4Ptr lease_reclaimed(new Lease4(IOAddress("10.0.0.2"), hwaddr_reclaimed,
                                         ClientIdPtr(), 60,
                                         time(NULL) - 1000, SubnetID(1)));
    lease_reclaimed->state_ = Lease4::STATE_EXPIRED_RECLAIMED;

    // Add leases to the database.
    LeaseMgr& lease_mgr = LeaseMgrFactory::instance();
    ASSERT_NO_THROW(lease_mgr.addLease(lease_expired));
    ASSERT_NO_THROW(lease_mgr.addLease(lease_reclaimed));

    // Make sure they have been added.
    ASSERT_TRUE(lease_mgr.getLease4(IOAddress("10.0.0.1")));
    ASSERT_TRUE(lease_mgr.getLease4(IOAddress("10.0.0.2")));

    // Disable service.
    srv->enableService(false);

    // Poll the timers for a while to make sure that each of them is executed
    // at least once.
    ASSERT_NO_THROW(runTimersWithTimeout(srv->getIOService(), 5000));

    // Verify that the leases in the database have not been processed.
    ASSERT_NO_THROW(
        lease_expired = lease_mgr.getLease4(IOAddress("10.0.0.1"))
    );
    ASSERT_TRUE(lease_expired);
    ASSERT_EQ(Lease::STATE_DEFAULT, lease_expired->state_);

    // Second lease should not have been removed.
    ASSERT_NO_THROW(
        lease_reclaimed = lease_mgr.getLease4(IOAddress("10.0.0.2"))
    );
    ASSERT_TRUE(lease_reclaimed);
    ASSERT_EQ(Lease::STATE_EXPIRED_RECLAIMED, lease_reclaimed->state_);

    // Enable service.
    srv->enableService(true);

    // Poll the timers for a while to make sure that each of them is executed
    // at least once.
    ASSERT_NO_THROW(runTimersWithTimeout(srv->getIOService(), 5000));

    // Verify that the leases in the database have been processed as expected.

    // First lease should be reclaimed, but not removed.
    ASSERT_NO_THROW(lease_expired = lease_mgr.getLease4(IOAddress("10.0.0.1")));
    ASSERT_TRUE(lease_expired);
    EXPECT_TRUE(lease_expired->stateExpiredReclaimed());

    // Second lease should have been removed.
    ASSERT_NO_THROW(
        lease_reclaimed = lease_mgr.getLease4(IOAddress("10.0.0.2"));
    );
    EXPECT_FALSE(lease_reclaimed);
}


} // End of anonymous namespace
