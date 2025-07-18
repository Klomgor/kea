// Copyright (C) 2012-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <asiolink/io_address.h>
#include <cc/command_interpreter.h>
#include <config/command_mgr.h>
#include <config/timeouts.h>
#include <config/unix_command_mgr.h>
#include <dhcp/libdhcp++.h>
#include <dhcp/testutils/iface_mgr_test_config.h>
#include <dhcpsrv/cfgmgr.h>
#include <dhcpsrv/host_mgr.h>
#include <dhcpsrv/lease.h>
#include <dhcpsrv/lease_mgr_factory.h>
#include <dhcp6/ctrl_dhcp6_srv.h>
#include <dhcp6/tests/dhcp6_test_utils.h>
#include <hooks/hooks_manager.h>
#include <log/logger_support.h>
#include <stats/stats_mgr.h>
#include <util/multi_threading_mgr.h>
#include <util/chrono_time_utils.h>
#include <testutils/io_utils.h>
#include <testutils/unix_control_client.h>
#include <testutils/sandbox.h>

#include "marker_file.h"

#include <boost/scoped_ptr.hpp>
#include <gtest/gtest.h>

#include <fstream>
#include <iomanip>
#include <sstream>
#include <thread>

#include <sys/select.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <cstdlib>
#include <unistd.h>

using namespace std;
using namespace isc;
using namespace isc::asiolink;
using namespace isc::config;
using namespace isc::data;
using namespace isc::dhcp;
using namespace isc::dhcp::test;
using namespace isc::hooks;
using namespace isc::stats;
using namespace isc::test;
using namespace isc::util;
namespace ph = std::placeholders;

namespace {

/// @brief Test timeout (ms).
const long TEST_TIMEOUT = 10000;

/// @brief Simple RAII class which stops IO service upon destruction
/// of the object.
class IOServiceWork {
public:

    /// @brief Constructor.
    ///
    /// @param io_service Pointer to the IO service to be stopped.
    explicit IOServiceWork(const IOServicePtr& io_service)
        : io_service_(io_service) {
    }

    /// @brief Destructor.
    ///
    /// Stops IO service.
    ~IOServiceWork() {
        io_service_->stop();
    }

private:

    /// @brief Pointer to the IO service to be stopped upon destruction.
    IOServicePtr io_service_;

};

class NakedControlledDhcpv6Srv: public ControlledDhcpv6Srv {
    // "Naked" DHCPv6 server, exposes internal fields
public:
    NakedControlledDhcpv6Srv() : ControlledDhcpv6Srv(DHCP6_SERVER_PORT + 10000) {
        CfgMgr::instance().setFamily(AF_INET6);
    }

    /// Expose internal methods for the sake of testing
    using Dhcpv6Srv::receivePacket;
    using Dhcpv6Srv::network_state_;
};

/// @brief Default control connection timeout.
const size_t DEFAULT_CONNECTION_TIMEOUT = 10000;

class CtrlDhcpv6SrvTest : public BaseServerTest {
public:
    CtrlDhcpv6SrvTest()
        : BaseServerTest() {
        reset();
    }

    virtual ~CtrlDhcpv6SrvTest() {
        LeaseMgrFactory::destroy();
        StatsMgr::instance().removeAll();
        CommandMgr::instance().deregisterAll();
        UnixCommandMgr::instance().setConnectionTimeout(DEFAULT_CONNECTION_TIMEOUT);

        reset();
    }

    /// @brief Reset hooks data
    ///
    /// Resets the data for the hooks-related portion of the test by ensuring
    /// that no libraries are loaded and that any marker files are deleted.
    virtual void reset() {
        // Unload any previously-loaded libraries.
        EXPECT_TRUE(HooksManager::unloadLibraries());

        // Get rid of any marker files.
        static_cast<void>(remove(LOAD_MARKER_FILE));
        static_cast<void>(remove(UNLOAD_MARKER_FILE));
        IfaceMgr::instance().deleteAllExternalSockets();
        CfgMgr::instance().clear();
    }

};

class CtrlChannelDhcpv6SrvTest : public CtrlDhcpv6SrvTest {
public:
    isc::test::Sandbox sandbox;

    /// @brief Path to the UNIX socket being used to communicate with the server
    std::string socket_path_;

    /// @brief List of interfaces (defaults to "*").
    std::string interfaces_;

    /// @brief Pointer to the tested server object
    boost::shared_ptr<NakedControlledDhcpv6Srv> server_;

    /// @brief Asynchronous timer service to detect timeouts.
    IntervalTimerPtr test_timer_;

    /// @brief Default constructor
    ///
    /// Sets socket path to its default value.
    CtrlChannelDhcpv6SrvTest() : interfaces_("\"*\"") {
        reset();
        IfaceMgr::instance().setTestMode(false);
        IfaceMgr::instance().setDetectCallback(std::bind(&IfaceMgr::checkDetectIfaces,
                                               IfaceMgr::instancePtr().get(), ph::_1));
        setSocketTestPath();
        socket_path_ = UnixCommandConfig::getSocketPath() + "/kea6.sock";
    }

    /// @brief Destructor
    ~CtrlChannelDhcpv6SrvTest() {
        if (test_timer_) {
            test_timer_->cancel();
            getIOService()->stopAndPoll();
        }
        LeaseMgrFactory::destroy();
        StatsMgr::instance().removeAll();

        CommandMgr::instance().deregisterAll();
        UnixCommandMgr::instance().setConnectionTimeout(TIMEOUT_DHCP_SERVER_RECEIVE_COMMAND);

        server_.reset();
        reset();
        IfaceMgr::instance().setTestMode(false);
        IfaceMgr::instance().setDetectCallback(std::bind(&IfaceMgr::checkDetectIfaces,
                                               IfaceMgr::instancePtr().get(), ph::_1));
        IfaceMgr::instance().clearIfaces();
        IfaceMgr::instance().closeSockets();
        IfaceMgr::instance().detectIfaces();
    }

    /// @brief Returns pointer to the server's IO service.
    ///
    /// @return Pointer to the server's IO service or null pointer if the server
    /// hasn't been created.
    IOServicePtr getIOService() {
        return (server_ ? server_->getIOService() : IOServicePtr());
    }

    /// @brief Callback function invoke upon test timeout.
    ///
    /// It stops the IO service and reports test timeout.
    ///
    /// @param fail_on_timeout Specifies if test failure should be reported.
    void timeoutHandler(const bool fail_on_timeout) {
        if (fail_on_timeout) {
            ADD_FAILURE() << "Timeout occurred while running the test!";
        }
        getIOService()->stop();
    }

    void createUnixChannelServer() {
        static_cast<void>(::remove(socket_path_.c_str()));

        // Just a simple config. The important part here is the socket
        // location information.
        std::string header =
            "{"
            "    \"interfaces-config\": {"
            "        \"interfaces\": [";

        std::string body = "]"
            "    },"
            "    \"expired-leases-processing\": {"
            "         \"reclaim-timer-wait-time\": 60,"
            "         \"hold-reclaimed-time\": 500,"
            "         \"flush-reclaimed-timer-wait-time\": 60"
            "    },"
            "    \"rebind-timer\": 2000, "
            "    \"renew-timer\": 1000, "
            "    \"subnet6\": [ ],"
            "    \"valid-lifetime\": 4000,"
            "    \"control-socket\": {"
            "        \"socket-type\": \"unix\","
            "        \"socket-name\": \"";

        std::string footer =
            "\"    },"
            "    \"lease-database\": {"
            "       \"type\": \"memfile\", \"persist\": false },"
            "    \"loggers\": [ {"
            "       \"name\": \"kea-dhcp6\","
            "       \"severity\": \"INFO\","
            "       \"debuglevel\": 0"
            "       } ]"
            "}";

        // Fill in the socket-name value with socket_path_  to
        // make the actual configuration text.
        std::string config_txt = header + interfaces_ + body + socket_path_  + footer;
        ASSERT_NO_THROW(server_.reset(new NakedControlledDhcpv6Srv()));

        ConstElementPtr config;
        ASSERT_NO_THROW(config = parseDHCP6(config_txt));

        // Parse the logger configuration explicitly into the staging config.
        // Note this does not alter the current loggers, they remain in
        // effect until we apply the logging config below.  If no logging
        // is supplied logging will revert to default logging.
        server_->configureLogger(config, CfgMgr::instance().getStagingCfg());

        // Let's apply the new logging. We do it early, so we'll be able to print
        // out what exactly is wrong with the new config in case of problems.
        CfgMgr::instance().getStagingCfg()->applyLoggingCfg();

        ConstElementPtr answer = server_->processConfig(config);

        // Commit the configuration so any subsequent reconfigurations
        // will only close the command channel if its configuration has
        // changed.
        CfgMgr::instance().commit();

        ASSERT_TRUE(answer);

        int status = 0;
        ConstElementPtr txt = isc::config::parseAnswer(status, answer);
        // This should succeed. If not, print the error message.
        ASSERT_EQ(0, status) << txt->str();

        // Now check that the socket was indeed open.
        ASSERT_GT(isc::config::UnixCommandMgr::instance().getControlSocketFD(), -1);

        test_timer_.reset(new IntervalTimer(getIOService()));
        test_timer_->setup(std::bind(&CtrlChannelDhcpv6SrvTest::timeoutHandler, this, true),
                           TEST_TIMEOUT, IntervalTimer::ONE_SHOT);
    }

    /// @brief Reset
    void reset() override {
        CtrlDhcpv6SrvTest::reset();

        // Remove unix socket file
        static_cast<void>(::remove(socket_path_.c_str()));
    }

    /// @brief Conducts a command/response exchange via UnixCommandSocket
    ///
    /// This method connects to the given server over the given socket path.
    /// If successful, it then sends the given command and retrieves the
    /// server's response.  Note that it polls the server's I/O service
    /// where needed to cause the server to process IO events on
    /// the control channel sockets.
    ///
    /// @param command the command text to execute in JSON form
    /// @param response variable into which the received response should be
    /// placed.
    void sendUnixCommand(const std::string& command, std::string& response) {
        response = "";
        boost::scoped_ptr<UnixControlClient> client;
        client.reset(new UnixControlClient());
        ASSERT_TRUE(client);

        // Connect to the server. This is expected to trigger server's acceptor
        // handler when IOService::poll() is run.
        ASSERT_TRUE(client->connectToServer(socket_path_));
        ASSERT_NO_THROW(getIOService()->poll());

        // Send the command. This will trigger server's handler which receives
        // data over the unix domain socket. The server will start sending
        // response to the client.
        ASSERT_TRUE(client->sendCommand(command));
        ASSERT_NO_THROW(getIOService()->pollOne());

        // Read the response generated by the server. Note that getResponse
        // only fails if there an IO error or no response data was present.
        // It is not based on the response content.
        ASSERT_TRUE(client->getResponse(response));

        // Now disconnect and process the close event
        client->disconnectFromServer();

        ASSERT_NO_THROW(getIOService()->poll());
    }

    /// @brief Checks response for list-commands
    ///
    /// This method checks if the list-commands response is generally sane
    /// and whether specified command is mentioned in the response.
    ///
    /// @param rsp response sent back by the server
    /// @param command command expected to be on the list.
    void checkListCommands(const ConstElementPtr& rsp, const std::string& command) {
        ConstElementPtr params;
        int status_code = -1;
        EXPECT_NO_THROW(params = parseAnswer(status_code, rsp));
        EXPECT_EQ(CONTROL_RESULT_SUCCESS, status_code);
        ASSERT_TRUE(params);
        ASSERT_EQ(Element::list, params->getType());

        int cnt = 0;
        for (size_t i = 0; i < params->size(); ++i) {
            string tmp = params->get(i)->stringValue();
            if (tmp == command) {
                // Command found, but that's not enough. Need to continue working
                // through the list to see if there are no duplicates.
                cnt++;
            }
        }

        // Exactly one command on the list is expected.
        EXPECT_EQ(1, cnt) << "Command " << command << " not found";
    }

    /// @brief Check if the answer for config-write command is correct.
    ///
    /// @param response_txt response in text form (as read from
    /// the control socket)
    /// @param exp_status expected status (0 success, 1 failure)
    /// @param exp_txt for success cases this defines the expected filename,
    /// for failure cases this defines the expected error message.
    void checkConfigWrite(const std::string& response_txt, int exp_status,
                          const std::string& exp_txt = "") {

        ConstElementPtr rsp;
        EXPECT_NO_THROW(rsp = Element::fromJSON(response_txt));
        ASSERT_TRUE(rsp);

        int status;
        ConstElementPtr params = parseAnswer(status, rsp);
        EXPECT_EQ(exp_status, status);

        if (exp_status == CONTROL_RESULT_SUCCESS) {
            // Let's check couple things...

            // The parameters must include filename
            ASSERT_TRUE(params);
            ASSERT_TRUE(params->get("filename"));
            ASSERT_EQ(Element::string, params->get("filename")->getType());
            EXPECT_EQ(exp_txt, params->get("filename")->stringValue());

            // The parameters must include size. And the size
            // must indicate some content.
            ASSERT_TRUE(params->get("size"));
            ASSERT_EQ(Element::integer, params->get("size")->getType());
            int64_t size = params->get("size")->intValue();
            EXPECT_LE(1, size);

            // Now check if the file is really there and suitable for
            // opening.
            ifstream f(exp_txt, ios::binary | ios::ate);
            ASSERT_TRUE(f.good());

            // Now check that it is the correct size as reported.
            EXPECT_EQ(size, static_cast<int64_t>(f.tellg()));

            // Finally, check that it's really a JSON.
            ElementPtr from_file = Element::fromJSONFile(exp_txt);
            ASSERT_TRUE(from_file);
        } else if (exp_status == CONTROL_RESULT_ERROR) {

            // Let's check if the reason for failure was given.
            ConstElementPtr text = rsp->get("text");
            ASSERT_TRUE(text);
            ASSERT_EQ(Element::string, text->getType());
            EXPECT_EQ(exp_txt, text->stringValue());
        } else {
            ADD_FAILURE() << "Invalid expected status: " << exp_status;
        }
    }

    /// @brief Handler for long command.
    ///
    /// It checks whether the received command is equal to the one specified
    /// as an argument.
    ///
    /// @param expected_command String representing an expected command.
    /// @param command_name Command name received by the handler.
    /// @param arguments Command arguments received by the handler.
    ///
    /// @returns Success answer.
    static ConstElementPtr
    longCommandHandler(const std::string& expected_command,
                       const std::string& command_name,
                       const ConstElementPtr& arguments) {
        // The handler is called with a command name and the structure holding
        // command arguments. We have to rebuild the command from those
        // two arguments so as it can be compared against expected_command.
        ElementPtr entire_command = Element::createMap();
        entire_command->set("command", Element::create(command_name));
        entire_command->set("arguments", (arguments));

        // The rebuilt command will have a different order of parameters so
        // let's parse expected_command back to JSON to guarantee that
        // both structures are built using the same order.
        EXPECT_EQ(Element::fromJSON(expected_command)->str(),
                  entire_command->str());
        return (createAnswer(CONTROL_RESULT_SUCCESS, "long command received ok"));
    }

    /// @brief Command handler which generates long response
    ///
    /// This handler generates a large response (over 400kB). It includes
    /// a list of randomly generated strings to make sure that the test
    /// can catch out of order delivery.
    static ConstElementPtr longResponseHandler(const std::string&,
                                               const ConstElementPtr&) {
        ElementPtr arguments = Element::createList();
        for (unsigned i = 0; i < 80000; ++i) {
            std::ostringstream s;
            s << std::setw(5) << i;
            arguments->add(Element::create(s.str()));
        }
        return (createAnswer(CONTROL_RESULT_SUCCESS, arguments));
    }
};

TEST_F(CtrlDhcpv6SrvTest, commands) {

    boost::scoped_ptr<ControlledDhcpv6Srv> srv;
    ASSERT_NO_THROW(
        srv.reset(new ControlledDhcpv6Srv(DHCP6_SERVER_PORT + 10000))
    );

    // Use empty parameters list
    ElementPtr params(new isc::data::MapElement());
    int rcode = -1;

    // Case 1: send bogus command
    ConstElementPtr result = CommandMgr::instance().processCommand(createCommand("blah", params));
    ConstElementPtr comment = parseAnswer(rcode, result);
    EXPECT_EQ(2, rcode); // expect failure (no such command as blah)

    // Case 2: send shutdown command without any parameters
    result = CommandMgr::instance().processCommand(createCommand("shutdown", params));
    comment = parseAnswer(rcode, result);
    EXPECT_EQ(0, rcode); // expect success
    // Exit value should default to 0.
    EXPECT_EQ(0, srv->getExitValue());

    // Case 3: send shutdown command with exit-value parameter.
    ConstElementPtr x(new isc::data::IntElement(77));
    params->set("exit-value", x);

    result = CommandMgr::instance().processCommand(createCommand("shutdown", params));
    comment = parseAnswer(rcode, result);
    EXPECT_EQ(0, rcode); // expect success

    // Exit value should match.
    EXPECT_EQ(77, srv->getExitValue());
}

// This test checks which commands are registered by the DHCPv6 server.
TEST_F(CtrlDhcpv6SrvTest, commandsRegistration) {

    ConstElementPtr list_cmds = createCommand("list-commands");
    ConstElementPtr answer;

    // By default the list should be empty (except the standard list-commands
    // supported by the CommandMgr itself)
    EXPECT_NO_THROW(answer = CommandMgr::instance().processCommand(list_cmds));
    ASSERT_TRUE(answer);
    ASSERT_TRUE(answer->get("arguments"));
    EXPECT_EQ("[ \"list-commands\" ]", answer->get("arguments")->str());

    // Created server should register several additional commands.
    boost::scoped_ptr<ControlledDhcpv6Srv> srv;
    ASSERT_NO_THROW(
        srv.reset(new ControlledDhcpv6Srv(0));
    );

    EXPECT_NO_THROW(answer = CommandMgr::instance().processCommand(list_cmds));
    ASSERT_TRUE(answer);

    ASSERT_TRUE(answer->get("arguments"));
    std::string command_list = answer->get("arguments")->str();

    EXPECT_TRUE(command_list.find("\"list-commands\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"build-report\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"config-backend-pull\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"config-get\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"config-hash-get\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"config-set\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"config-write\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"leases-reclaim\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"subnet6-select-test\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"server-tag-get\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"shutdown\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"statistic-get\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"statistic-get-all\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"statistic-remove\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"statistic-remove-all\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"statistic-reset\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"statistic-reset-all\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"statistic-sample-age-set\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"statistic-sample-age-set-all\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"statistic-sample-count-set\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"statistic-sample-count-set-all\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"status-get\"") != string::npos);
    EXPECT_TRUE(command_list.find("\"version-get\"") != string::npos);

    // Ok, and now delete the server. It should deregister its commands.
    srv.reset();

    // The list should be (almost) empty again.
    EXPECT_NO_THROW(answer = CommandMgr::instance().processCommand(list_cmds));
    ASSERT_TRUE(answer);
    ASSERT_TRUE(answer->get("arguments"));
    EXPECT_EQ("[ \"list-commands\" ]", answer->get("arguments")->str());
}

// Tests that the server properly responds to invalid commands sent
// via ControlChannel
TEST_F(CtrlChannelDhcpv6SrvTest, controlChannelNegative) {
    createUnixChannelServer();
    std::string response;

    sendUnixCommand("{ \"command\": \"bogus\" }", response);
    EXPECT_EQ("{ \"result\": 2,"
              " \"text\": \"'bogus' command not supported.\" }", response);

    sendUnixCommand("utter nonsense", response);
    EXPECT_EQ("{ \"result\": 1, "
              "\"text\": \"invalid first character u\" }",
              response);
}

// Tests that the server properly responds to shutdown command sent
// via ControlChannel
TEST_F(CtrlChannelDhcpv6SrvTest, controlChannelShutdown) {
    createUnixChannelServer();
    std::string response;

    sendUnixCommand("{ \"command\": \"shutdown\" }", response);
    EXPECT_EQ("{ \"result\": 0, \"text\": \"Shutting down.\" }", response);
}

// Tests that the server properly responds to statistics commands.  Note this
// is really only intended to verify that the appropriate Statistics handler
// is called based on the command.  It is not intended to be an exhaustive
// test of Dhcpv6 statistics.
TEST_F(CtrlChannelDhcpv6SrvTest, controlChannelStats) {
    createUnixChannelServer();
    std::string response;

    // Check statistic-get
    sendUnixCommand("{ \"command\" : \"statistic-get\", "
                    "  \"arguments\": {"
                    "  \"name\":\"bogus\" }}", response);
    EXPECT_EQ("{ \"arguments\": {  }, \"result\": 0 }", response);

    // Check statistic-get-all
    sendUnixCommand("{ \"command\" : \"statistic-get-all\", "
                    "  \"arguments\": {}}", response);

    std::set<std::string> initial_stats = {
        "pkt6-received",
        "pkt6-solicit-received",
        "pkt6-advertise-received",
        "pkt6-request-received",
        "pkt6-reply-received",
        "pkt6-renew-received",
        "pkt6-rebind-received",
        "pkt6-decline-received",
        "pkt6-release-received",
        "pkt6-infrequest-received",
        "pkt6-dhcpv4-query-received",
        "pkt6-dhcpv4-response-received",
        "pkt6-addr-reg-inform-received",
        "pkt6-addr-reg-reply-received",
        "pkt6-unknown-received",
        "pkt6-sent",
        "pkt6-advertise-sent",
        "pkt6-reply-sent",
        "pkt6-dhcpv4-response-sent",
        "pkt6-addr-reg-reply-sent",
        "pkt6-parse-failed",
        "pkt6-receive-drop",
        "v6-allocation-fail",
        "v6-allocation-fail-shared-network",
        "v6-allocation-fail-subnet",
        "v6-allocation-fail-no-pools",
        "v6-allocation-fail-classes",
        "v6-ia-na-lease-reuses",
        "v6-ia-pd-lease-reuses",
    };

    std::ostringstream s;
    s << "{ \"arguments\": { ";
    bool first = true;
    for (auto const& st : initial_stats) {
        if (!first) {
            s << ", ";
        } else {
            first = false;
        }
        s << "\"" << st << "\": [ [ 0, \"";
        s << isc::util::clockToText(StatsMgr::instance().getObservation(st)->getInteger().second);
        s << "\" ] ]";
    }
    s << " }, \"result\": 0 }";

    auto stats_get_all = s.str();

    EXPECT_EQ(stats_get_all, response);

    // Check statistic-reset
    sendUnixCommand("{ \"command\" : \"statistic-reset\", "
                    "  \"arguments\": {"
                    "  \"name\":\"bogus\" }}", response);
    EXPECT_EQ("{ \"result\": 1, \"text\": \"No 'bogus' statistic found\" }",
              response);

    // Check statistic-reset-all
    sendUnixCommand("{ \"command\" : \"statistic-reset-all\", "
                    "  \"arguments\": {}}", response);
    EXPECT_EQ("{ \"result\": 0, \"text\": "
              "\"All statistics reset to neutral values.\" }", response);

    // Check statistic-remove
    sendUnixCommand("{ \"command\" : \"statistic-remove\", "
                    "  \"arguments\": {"
                    "  \"name\":\"bogus\" }}", response);
    EXPECT_EQ("{ \"result\": 1, \"text\": \"No 'bogus' statistic found\" }",
              response);

    // Check statistic-remove-all (deprecated)

    // Check statistic-sample-age-set
    sendUnixCommand("{ \"command\" : \"statistic-sample-age-set\", "
                    "  \"arguments\": {"
                    "  \"name\":\"bogus\", \"duration\": 1245 }}", response);
    EXPECT_EQ("{ \"result\": 1, \"text\": \"No 'bogus' statistic found\" }",
              response);

    // Check statistic-sample-age-set-all
    sendUnixCommand("{ \"command\" : \"statistic-sample-age-set-all\", "
                    "  \"arguments\": {"
                    "  \"duration\": 1245 }}", response);
    EXPECT_EQ("{ \"result\": 0, \"text\": \"All statistics duration limit are set.\" }",
              response);

    // Check statistic-sample-count-set
    sendUnixCommand("{ \"command\" : \"statistic-sample-count-set\", "
                    "  \"arguments\": {"
                    "  \"name\":\"bogus\", \"max-samples\": 100 }}", response);
    EXPECT_EQ("{ \"result\": 1, \"text\": \"No 'bogus' statistic found\" }",
              response);

    // Check statistic-sample-count-set-all
    sendUnixCommand("{ \"command\" : \"statistic-sample-count-set-all\", "
                    "  \"arguments\": {"
                    "  \"max-samples\": 100 }}", response);
    EXPECT_EQ("{ \"result\": 0, \"text\": \"All statistics count limit are set.\" }",
              response);
}

// Check that the "config-set" command will replace current configuration
TEST_F(CtrlChannelDhcpv6SrvTest, configSet) {
    setLogTestPath("/dev");
    createUnixChannelServer();

    // Define strings to permutate the config arguments
    // (Note the line feeds makes errors easy to find)
    string set_config_txt = "{ \"command\": \"config-set\" \n";
    string args_txt = " \"arguments\": { \n";
    string dhcp6_cfg_txt =
        "    \"Dhcp6\": { \n"
        "        \"interfaces-config\": { \n"
        "            \"interfaces\": [\"*\"] \n"
        "        },   \n"
        "        \"preferred-lifetime\": 3000, \n"
        "        \"valid-lifetime\": 4000, \n"
        "        \"renew-timer\": 1000, \n"
        "        \"rebind-timer\": 2000, \n"
        "        \"lease-database\": { \n"
        "           \"type\": \"memfile\", \n"
        "           \"persist\":false, \n"
        "           \"lfc-interval\": 0  \n"
        "        }, \n"
        "        \"expired-leases-processing\": { \n"
        "            \"reclaim-timer-wait-time\": 0, \n"
        "            \"hold-reclaimed-time\": 0, \n"
        "            \"flush-reclaimed-timer-wait-time\": 0 \n"
        "        },"
        "        \"subnet6\": [ \n";
    string subnet1 =
        "               {\"subnet\": \"3002::/64\", \"id\": 1, \n"
        "                \"pools\": [{ \"pool\": \"3002::100-3002::200\" }]}\n";
    string subnet2 =
        "               {\"subnet\": \"3003::/64\", \"id\": 2, \n"
        "                \"pools\": [{ \"pool\": \"3003::100-3003::200\" }]}\n";
    string bad_subnet =
        "               {\"comment\": \"3005::/64\", \"id\": 10, \n"
        "                \"pools\": [{ \"pool\": \"3005::100-3005::200\" }]}\n";
    string subnet_footer =
        "          ] \n";
    string option_def =
        "    ,\"option-def\": [\n"
        "    {\n"
        "        \"name\": \"foo\",\n"
        "        \"code\": 163,\n"
        "        \"type\": \"uint32\",\n"
        "        \"array\": false,\n"
        "        \"record-types\": \"\",\n"
        "        \"space\": \"dhcp6\",\n"
        "        \"encapsulate\": \"\"\n"
        "    }\n"
        "]\n";
    string option_data =
        "    ,\"option-data\": [\n"
        "    {\n"
        "        \"name\": \"foo\",\n"
        "        \"code\": 163,\n"
        "        \"space\": \"dhcp6\",\n"
        "        \"csv-format\": true,\n"
        "        \"data\": \"12345\"\n"
        "    }\n"
        "]\n";
    string control_socket_header =
        "       ,\"control-socket\": { \n"
        "       \"socket-type\": \"unix\", \n"
        "       \"socket-name\": \"";
    string control_socket_footer =
        "\"   \n} \n";
    string logger_txt =
        "       ,\"loggers\": [ { \n"
        "            \"name\": \"kea\", \n"
        "            \"severity\": \"FATAL\", \n"
        "            \"output-options\": [{ \n"
        "                \"output\": \"/dev/null\", \n"
        "                \"maxsize\": 0"
        "            }] \n"
        "        }] \n";

    std::ostringstream os;

    // Create a valid config with all the parts should parse
    os << set_config_txt << ","
        << args_txt
        << dhcp6_cfg_txt
        << subnet1
        << subnet_footer
        << option_def
        << option_data
        << control_socket_header
        << socket_path_
        << control_socket_footer
        << logger_txt
        << "}\n"                      // close dhcp6
        << "}}";

    // Send the config-set command
    std::string response;
    sendUnixCommand(os.str(), response);

    // Verify the configuration was successful. The config contains random
    // socket name (/tmp/kea-<value-changing-each-time>/kea6.sock), so the
    // hash will be different each time. As such, we can do simplified checks:
    // - verify the "result": 0 is there
    // - verify the "text": "Configuration successful." is there
    EXPECT_NE(response.find("\"result\": 0"), std::string::npos);
    EXPECT_NE(response.find("\"text\": \"Configuration successful.\""), std::string::npos);

    // Check that the config was indeed applied.
    const Subnet6Collection* subnets =
        CfgMgr::instance().getCurrentCfg()->getCfgSubnets6()->getAll();
    EXPECT_EQ(1, subnets->size());

    OptionDefinitionPtr def =
        LibDHCP::getRuntimeOptionDef(DHCP6_OPTION_SPACE, 163);
    ASSERT_TRUE(def);

    // Create a config with malformed subnet that should fail to parse.
    os.str("");
    os << set_config_txt << ","
        << args_txt
        << dhcp6_cfg_txt
        << bad_subnet
        << subnet_footer
        << control_socket_header
        << socket_path_
        << control_socket_footer
        << "}\n"                      // close dhcp6
        << "}}";

    // Send the config-set command
    sendUnixCommand(os.str(), response);

    // Should fail with a syntax error
    EXPECT_EQ("{ \"result\": 1, "
              "\"text\": \"subnet configuration failed: mandatory 'subnet' "
              "parameter is missing for a subnet being configured (<wire>:20:17)\" }",
              response);

    // Check that the config was not lost
    subnets = CfgMgr::instance().getCurrentCfg()->getCfgSubnets6()->getAll();
    EXPECT_EQ(1, subnets->size());

    def = LibDHCP::getRuntimeOptionDef(DHCP6_OPTION_SPACE, 163);
    ASSERT_TRUE(def);

    // Create a valid config with two subnets and no command channel.
    // It should succeed, client should still receive the response
    os.str("");
    os << set_config_txt << ","
        << args_txt
        << dhcp6_cfg_txt
        << subnet1
        << ",\n"
        << subnet2
        << subnet_footer
        << "}\n"                      // close dhcp6
        << "}}";

    // Verify the control channel socket exists.
    ASSERT_TRUE(fileExists(socket_path_));

    // Send the config-set command.
    sendUnixCommand(os.str(), response);

    // Verify the control channel socket no longer exists.
    EXPECT_FALSE(fileExists(socket_path_));

    // With no command channel, should still receive the response.
    // The config contains random socket name
    // (/tmp/kea-<value-changing-each-time>/kea6.sock), so the hash will
    // be different each time. As such, we can do simplified checks:
    // - verify the "result": 0 is there
    // - verify the "text": "Configuration successful." is there
    EXPECT_NE(response.find("\"result\": 0"), std::string::npos);
    EXPECT_NE(response.find("\"text\": \"Configuration successful.\""), std::string::npos);

    // Check that the config was not lost
    subnets = CfgMgr::instance().getCurrentCfg()->getCfgSubnets6()->getAll();
    EXPECT_EQ(2, subnets->size());

    // Clean up after the test.
    CfgMgr::instance().clear();
}

// Tests if the server returns its configuration using config-get.
// Note there are separate tests that verify if toElement() called by the
// config-get handler are actually converting the configuration correctly.
TEST_F(CtrlChannelDhcpv6SrvTest, configGet) {
    createUnixChannelServer();
    std::string response;

    sendUnixCommand("{ \"command\": \"config-get\" }", response);
    ConstElementPtr rsp;

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    int status;
    ConstElementPtr cfg = parseAnswer(status, rsp);
    EXPECT_EQ(CONTROL_RESULT_SUCCESS, status);

    // Ok, now roughly check if the response seems legit.
    ASSERT_TRUE(cfg);
    ASSERT_EQ(Element::map, cfg->getType());
    EXPECT_TRUE(cfg->get("Dhcp6"));
    EXPECT_TRUE(cfg->get("Dhcp6")->get("loggers"));
}

// Tests if the server returns the hash of its configuration using
// config-hash-get.
TEST_F(CtrlChannelDhcpv6SrvTest, configHashGet) {
    createUnixChannelServer();
    std::string response;

    sendUnixCommand("{ \"command\": \"config-hash-get\" }", response);
    ConstElementPtr rsp;

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    int status;
    ConstElementPtr args = parseAnswer(status, rsp);
    EXPECT_EQ(CONTROL_RESULT_SUCCESS, status);
    // The parseAnswer is trying to be smart with ignoring hash.
    // But this time we really want to see the hash, so we'll retrieve
    // the arguments manually.
    args = rsp->get(CONTROL_ARGUMENTS);

    // Ok, now roughly check if the response seems legit.
    ASSERT_TRUE(args);
    ASSERT_EQ(Element::map, args->getType());
    ConstElementPtr hash = args->get("hash");
    ASSERT_TRUE(hash);
    ASSERT_EQ(Element::string, hash->getType());
    // SHA-256 -> 64 hex digits.
    EXPECT_EQ(64, hash->stringValue().size());
}

// Verify that the "config-test" command will do what we expect.
TEST_F(CtrlChannelDhcpv6SrvTest, configTest) {
    setLogTestPath("/dev");
    createUnixChannelServer();

    // Define strings to permutate the config arguments
    // (Note the line feeds makes errors easy to find)
    string set_config_txt = "{ \"command\": \"config-set\" \n";
    string config_test_txt = "{ \"command\": \"config-test\" \n";
    string args_txt = " \"arguments\": { \n";
    string dhcp6_cfg_txt =
        "    \"Dhcp6\": { \n"
        "        \"interfaces-config\": { \n"
        "            \"interfaces\": [\"*\"] \n"
        "        },   \n"
        "        \"preferred-lifetime\": 3000, \n"
        "        \"valid-lifetime\": 4000, \n"
        "        \"renew-timer\": 1000, \n"
        "        \"rebind-timer\": 2000, \n"
        "        \"lease-database\": { \n"
        "           \"type\": \"memfile\", \n"
        "           \"persist\":false, \n"
        "           \"lfc-interval\": 0  \n"
        "        }, \n"
        "        \"expired-leases-processing\": { \n"
        "            \"reclaim-timer-wait-time\": 0, \n"
        "            \"hold-reclaimed-time\": 0, \n"
        "            \"flush-reclaimed-timer-wait-time\": 0 \n"
        "        },"
        "        \"subnet6\": [ \n";
    string subnet1 =
        "               {\"subnet\": \"3002::/64\", \"id\": 1, \n"
        "                \"pools\": [{ \"pool\": \"3002::100-3002::200\" }]}\n";
    string subnet2 =
        "               {\"subnet\": \"3003::/64\", \"id\": 2, \n"
        "                \"pools\": [{ \"pool\": \"3003::100-3003::200\" }]}\n";
    string bad_subnet =
        "               {\"comment\": \"3005::/64\", \"id\": 10, \n"
        "                \"pools\": [{ \"pool\": \"3005::100-3005::200\" }]}\n";
    string subnet_footer =
        "          ] \n";
    string control_socket_header =
        "       ,\"control-socket\": { \n"
        "       \"socket-type\": \"unix\", \n"
        "       \"socket-name\": \"";
    string control_socket_footer =
        "\"   \n} \n";
    string logger_txt =
        "       ,\"loggers\": [ { \n"
        "            \"name\": \"kea\", \n"
        "            \"severity\": \"FATAL\", \n"
        "            \"output-options\": [{ \n"
        "                \"output\": \"/dev/null\", \n"
        "                \"maxsize\": 0"
        "            }] \n"
        "        }] \n";

    std::ostringstream os;

    // Create a valid config with all the parts should parse
    os << set_config_txt << ","
        << args_txt
        << dhcp6_cfg_txt
        << subnet1
        << subnet_footer
        << control_socket_header
        << socket_path_
        << control_socket_footer
        << logger_txt
        << "}\n"                      // close dhcp6
        << "}}";

    // Send the config-set command
    std::string response;
    sendUnixCommand(os.str(), response);

    // Verify the configuration was successful. The config contains random
    // socket name (/tmp/kea-<value-changing-each-time>/kea6.sock), so the
    // hash will be different each time. As such, we can do simplified checks:
    // - verify the "result": 0 is there
    // - verify the "text": "Configuration successful." is there
    EXPECT_NE(response.find("\"result\": 0"), std::string::npos);
    EXPECT_NE(response.find("\"text\": \"Configuration successful.\""), std::string::npos);

    // Check that the config was indeed applied.
    const Subnet6Collection* subnets =
        CfgMgr::instance().getCurrentCfg()->getCfgSubnets6()->getAll();
    EXPECT_EQ(1, subnets->size());

    // Create a config with malformed subnet that should fail to parse.
    os.str("");
    os << config_test_txt << ","
        << args_txt
        << dhcp6_cfg_txt
        << bad_subnet
        << subnet_footer
        << control_socket_header
        << socket_path_
        << control_socket_footer
        << "}\n"                      // close dhcp6
        << "}}";

    // Send the config-test command
    sendUnixCommand(os.str(), response);

    // Should fail with a syntax error
    EXPECT_EQ("{ \"result\": 1, "
              "\"text\": \"subnet configuration failed: mandatory 'subnet' parameter "
              "is missing for a subnet being configured (<wire>:20:17)\" }",
              response);

    // Check that the config was not lost
    subnets = CfgMgr::instance().getCurrentCfg()->getCfgSubnets6()->getAll();
    EXPECT_EQ(1, subnets->size());

    // Create a valid config with two subnets and no command channel.
    os.str("");
    os << config_test_txt << ","
        << args_txt
        << dhcp6_cfg_txt
        << subnet1
        << ",\n"
        << subnet2
        << subnet_footer
        << "}\n"                      // close dhcp6
        << "}}";

    // Verify the control channel socket exists.
    ASSERT_TRUE(fileExists(socket_path_));

    // Send the config-test command.
    sendUnixCommand(os.str(), response);

    // Verify the control channel socket still exists.
    EXPECT_TRUE(fileExists(socket_path_));

    // Verify the configuration was successful.
    EXPECT_EQ("{ \"result\": 0, \"text\": \"Configuration seems sane. "
              "Control-socket, hook-libraries, and D2 configuration were "
              "sanity checked, but not applied.\" }",
              response);

    // Check that the config was not applied.
    subnets = CfgMgr::instance().getCurrentCfg()->getCfgSubnets6()->getAll();
    EXPECT_EQ(1, subnets->size());

    // Clean up after the test.
    CfgMgr::instance().clear();
}

// Verify that the "subnet6-select-test" command will do what we expect.
TEST_F(CtrlChannelDhcpv6SrvTest, subnetSelectTest) {
    createUnixChannelServer();

    string command_txt = "{ \"command\": \"subnet6-select-test\", \"arguments\": { \"classes\": [ \"foo\" ] } }";

    // Send the subnet6-select-test command
    std::string response;
    sendUnixCommand(command_txt, response);

    EXPECT_EQ("{ \"result\": 3, \"text\": \"no subnet selected\" }",
              response);
}

// This test verifies that the DHCP server handles version-get commands
TEST_F(CtrlChannelDhcpv6SrvTest, getVersion) {
    createUnixChannelServer();

    std::string response;

    // Send the version-get command
    sendUnixCommand("{ \"command\": \"version-get\" }", response);
    EXPECT_TRUE(response.find("\"result\": 0") != string::npos);
    EXPECT_TRUE(response.find("log4cplus") != string::npos);
    EXPECT_FALSE(response.find("Hooks directory: ") != string::npos);

    // Send the build-report command
    sendUnixCommand("{ \"command\": \"build-report\" }", response);
    EXPECT_TRUE(response.find("\"result\": 0") != string::npos);
    EXPECT_TRUE(response.find("Hooks directory: ") != string::npos);
}

// This test verifies that the DHCP server handles server-tag-get command
TEST_F(CtrlChannelDhcpv6SrvTest, serverTagGet) {
    createUnixChannelServer();

    std::string response;
    std::string expected;

    // Send the server-tag-get command
    sendUnixCommand("{ \"command\": \"server-tag-get\" }", response);
    expected = "{ \"arguments\": { \"server-tag\": \"\" }, \"result\": 0 }";
    EXPECT_EQ(expected, response);

    // Set a value to the server tag
    CfgMgr::instance().getCurrentCfg()->setServerTag("foobar");

    // Retry...
    sendUnixCommand("{ \"command\": \"server-tag-get\" }", response);
    expected = "{ \"arguments\": { \"server-tag\": \"foobar\" }, \"result\": 0 }";
    EXPECT_EQ(expected, response);
}

// This test verifies that the DHCP server handles status-get commands
TEST_F(CtrlChannelDhcpv6SrvTest, statusGet) {
    createUnixChannelServer();

    // start_ is initialized by init.
    ASSERT_THROW(server_->init("/no/such/file"), BadValue);

    std::string response_txt;

    // Send the status-get command.
    sendUnixCommand("{ \"command\": \"status-get\" }", response_txt);
    ConstElementPtr response;
    ASSERT_NO_THROW(response = Element::fromJSON(response_txt));
    ASSERT_TRUE(response);
    ASSERT_EQ(Element::map, response->getType());
    EXPECT_EQ(2, response->size());
    ConstElementPtr result = response->get("result");
    ASSERT_TRUE(result);
    ASSERT_EQ(Element::integer, result->getType());
    EXPECT_EQ(0, result->intValue());
    ConstElementPtr arguments = response->get("arguments");
    ASSERT_EQ(Element::map, arguments->getType());

    // The returned pid should be the pid of our process.
    auto found_pid = arguments->get("pid");
    ASSERT_TRUE(found_pid);
    EXPECT_EQ(static_cast<int64_t>(getpid()), found_pid->intValue());

    // It is hard to check the actual uptime (and reload) as it is based
    // on current time. Let's just make sure it is within a reasonable
    // range.
    auto found_uptime = arguments->get("uptime");
    ASSERT_TRUE(found_uptime);
    EXPECT_LE(found_uptime->intValue(), 5);
    EXPECT_GE(found_uptime->intValue(), 0);

    auto found_reload = arguments->get("reload");
    ASSERT_TRUE(found_reload);
    EXPECT_LE(found_reload->intValue(), 5);
    EXPECT_GE(found_reload->intValue(), 0);

    auto found_multi_threading = arguments->get("multi-threading-enabled");
    ASSERT_TRUE(found_multi_threading);
    EXPECT_TRUE(found_multi_threading->boolValue());

    auto found_thread_count = arguments->get("thread-pool-size");
    ASSERT_TRUE(found_thread_count);
    // The default value varies between systems.
    // Let's just make sure it's a positive value.
    EXPECT_LE(0, found_thread_count->intValue());

    auto found_queue_size = arguments->get("packet-queue-size");
    ASSERT_TRUE(found_queue_size);
    EXPECT_EQ(64, found_queue_size->intValue());

    auto found_queue_stats = arguments->get("packet-queue-statistics");
    ASSERT_TRUE(found_queue_stats);
    EXPECT_FALSE(found_queue_stats->str().empty());

    auto found_extended_info_tables = arguments->get("extended-info-tables");
    ASSERT_TRUE(found_extended_info_tables);
    EXPECT_FALSE(found_extended_info_tables->boolValue());

    MultiThreadingMgr::instance().setMode(true);
    MultiThreadingMgr::instance().setThreadPoolSize(4);
    MultiThreadingMgr::instance().setPacketQueueSize(64);
    sendUnixCommand("{ \"command\": \"status-get\" }", response_txt);
    ASSERT_NO_THROW(response = Element::fromJSON(response_txt));
    ASSERT_TRUE(response);
    ASSERT_EQ(Element::map, response->getType());
    EXPECT_EQ(2, response->size());
    result = response->get("result");
    ASSERT_TRUE(result);
    ASSERT_EQ(Element::integer, result->getType());
    EXPECT_EQ(0, result->intValue());
    arguments = response->get("arguments");
    ASSERT_EQ(Element::map, arguments->getType());

    // The returned pid should be the pid of our process.
    found_pid = arguments->get("pid");
    ASSERT_TRUE(found_pid);
    EXPECT_EQ(static_cast<int64_t>(getpid()), found_pid->intValue());

    // It is hard to check the actual uptime (and reload) as it is based
    // on current time. Let's just make sure it is within a reasonable
    // range.
    found_uptime = arguments->get("uptime");
    ASSERT_TRUE(found_uptime);
    EXPECT_LE(found_uptime->intValue(), 5);
    EXPECT_GE(found_uptime->intValue(), 0);

    found_reload = arguments->get("reload");
    ASSERT_TRUE(found_reload);
    EXPECT_LE(found_reload->intValue(), 5);
    EXPECT_GE(found_reload->intValue(), 0);

    found_multi_threading = arguments->get("multi-threading-enabled");
    ASSERT_TRUE(found_multi_threading);
    EXPECT_TRUE(found_multi_threading->boolValue());

    found_thread_count = arguments->get("thread-pool-size");
    ASSERT_TRUE(found_thread_count);
    EXPECT_EQ(found_thread_count->intValue(), 4);

    found_queue_size = arguments->get("packet-queue-size");
    ASSERT_TRUE(found_queue_size);
    EXPECT_EQ(found_queue_size->intValue(), 64);

    found_queue_stats = arguments->get("packet-queue-statistics");
    ASSERT_TRUE(found_queue_stats);
    ASSERT_EQ(Element::list, found_queue_stats->getType());
    EXPECT_EQ(3, found_queue_stats->size());
}

// Check that status is returned even if LeaseMgr and HostMgr are not created.
TEST_F(CtrlChannelDhcpv6SrvTest, noManagers) {
    // Send the status-get command.
    createUnixChannelServer();
    LeaseMgrFactory::destroy();
    HostMgr::create();
    string response_text;
    sendUnixCommand(R"({ "command": "status-get" })", response_text);
    ConstElementPtr response;
    ASSERT_NO_THROW(response = Element::fromJSON(response_text));
    ASSERT_TRUE(response);
    ASSERT_EQ(Element::map, response->getType());
    ConstElementPtr result(response->get("result"));
    ASSERT_TRUE(result);
    ASSERT_EQ(Element::integer, result->getType());
    EXPECT_EQ(0, result->intValue());
    ConstElementPtr arguments(response->get("arguments"));
    ASSERT_TRUE(arguments);
    ASSERT_EQ(Element::map, arguments->getType());
}

// Checks that socket status exists in status-get responses.
TEST_F(CtrlChannelDhcpv6SrvTest, statusGetSockets) {
    // Create dummy interfaces to test socket status.
    isc::dhcp::test::IfaceMgrTestConfig test_config(true);

    // Send the status-get command.
    createUnixChannelServer();
    string response_text;
    sendUnixCommand(R"({ "command": "status-get" })", response_text);
    ConstElementPtr response;
    ASSERT_NO_THROW(response = Element::fromJSON(response_text));
    ASSERT_TRUE(response);
    ASSERT_EQ(Element::map, response->getType());
    ConstElementPtr result(response->get("result"));
    ASSERT_TRUE(result);
    ASSERT_EQ(Element::integer, result->getType());
    EXPECT_EQ(0, result->intValue());
    ConstElementPtr arguments(response->get("arguments"));
    ASSERT_TRUE(arguments);
    ASSERT_EQ(Element::map, arguments->getType());

    ConstElementPtr sockets(arguments->get("sockets"));
    ASSERT_TRUE(sockets);
    ASSERT_EQ(Element::map, sockets->getType());

    ConstElementPtr status(sockets->get("status"));
    ASSERT_TRUE(status);
    ASSERT_EQ(Element::string, status->getType());
    EXPECT_EQ("ready", status->stringValue());

    ConstElementPtr errors(sockets->get("errors"));
    ASSERT_FALSE(errors);
}

// Checks that socket status includes errors in status-get responses.
TEST_F(CtrlChannelDhcpv6SrvTest, statusGetSocketsErrors) {
    // Create dummy interfaces to test socket status and add a custom down interface.
    isc::dhcp::test::IfaceMgrTestConfig test_config(true);
    test_config.addIface("down_interface", 4);
    test_config.setIfaceFlags("down_interface", FlagLoopback(false), FlagUp(false),
                              FlagRunning(true), FlagInactive4(false),
                              FlagInactive6(false));

    // Send the status-get command.
    createUnixChannelServer();
    string response_text;
    sendUnixCommand(R"({ "command": "status-get" })", response_text);
    ConstElementPtr response;
    ASSERT_NO_THROW(response = Element::fromJSON(response_text));
    ASSERT_TRUE(response);
    ASSERT_EQ(Element::map, response->getType());
    ConstElementPtr result(response->get("result"));
    ASSERT_TRUE(result);
    ASSERT_EQ(Element::integer, result->getType());
    EXPECT_EQ(0, result->intValue());
    ConstElementPtr arguments(response->get("arguments"));
    ASSERT_TRUE(arguments);
    ASSERT_EQ(Element::map, arguments->getType());

    ConstElementPtr sockets(arguments->get("sockets"));
    ASSERT_TRUE(sockets);
    ASSERT_EQ(Element::map, sockets->getType());

    ConstElementPtr status(sockets->get("status"));
    ASSERT_TRUE(status);
    ASSERT_EQ(Element::string, status->getType());
    EXPECT_EQ("failed", status->stringValue());

    ConstElementPtr errors(sockets->get("errors"));
    ASSERT_TRUE(errors);
    ASSERT_EQ(Element::list, errors->getType());
    ASSERT_EQ(1, errors->size());

    ConstElementPtr error(errors->get(0));
    ASSERT_TRUE(error);
    ASSERT_EQ(Element::string, error->getType());
    ASSERT_EQ("the interface down_interface is down", error->stringValue());
}

// This test verifies that the DHCP server handles config-backend-pull command
TEST_F(CtrlChannelDhcpv6SrvTest, configBackendPull) {
    createUnixChannelServer();

    std::string response;
    std::string expected;

    // Send the config-backend-pull command. Note there is no configured backend.
    sendUnixCommand("{ \"command\": \"config-backend-pull\" }", response);
    expected = "{ \"result\": 3, \"text\": \"No config backend.\" }";
    EXPECT_EQ(expected, response);
}

// This test verifies that the DHCP server immediately reclaims expired
// leases on leases-reclaim command
TEST_F(CtrlChannelDhcpv6SrvTest, controlLeasesReclaim) {
    createUnixChannelServer();

    // Create expired leases. Leases are expired by 40 seconds ago
    // (valid lifetime = 60, cltt = now - 100).
    DuidPtr duid0(new DUID(DUID::fromText("00:01:02:03:04:05:06").getDuid()));
    Lease6Ptr lease0(new Lease6(Lease::TYPE_NA, IOAddress("3000::1"),
                                duid0, 1, 50, 60, SubnetID(1)));
    lease0->cltt_ = time(NULL) - 100;
    DuidPtr duid1(new DUID(DUID::fromText("01:02:03:04:05:06:07").getDuid()));
    Lease6Ptr lease1(new Lease6(Lease::TYPE_NA, IOAddress("3000::2"),
                                duid1, 1, 50, 60, SubnetID(1)));
    lease1->cltt_ = time(NULL) - 100;

    // Add leases to the database.
    LeaseMgr& lease_mgr = LeaseMgrFactory::instance();
    ASSERT_NO_THROW(lease_mgr.addLease(lease0));
    ASSERT_NO_THROW(lease_mgr.addLease(lease1));

    // Make sure they have been added.
    ASSERT_TRUE(lease_mgr.getLease6(Lease::TYPE_NA, IOAddress("3000::1")));
    ASSERT_TRUE(lease_mgr.getLease6(Lease::TYPE_NA, IOAddress("3000::2")));

    // No arguments
    std::string response;
    sendUnixCommand("{ \"command\": \"leases-reclaim\" }", response);
    EXPECT_EQ("{ \"result\": 1, \"text\": "
              "\"Missing mandatory 'remove' parameter.\" }", response);

    // Bad argument name
    sendUnixCommand("{ \"command\": \"leases-reclaim\", "
                    "\"arguments\": { \"reclaim\": true } }", response);
    EXPECT_EQ("{ \"result\": 1, \"text\": "
              "\"Missing mandatory 'remove' parameter.\" }", response);

    // Bad remove argument type
    sendUnixCommand("{ \"command\": \"leases-reclaim\", "
                    "\"arguments\": { \"remove\": \"bogus\" } }", response);
    EXPECT_EQ("{ \"result\": 1, \"text\": "
              "\"'remove' parameter expected to be a boolean.\" }", response);

    // Send the command
    sendUnixCommand("{ \"command\": \"leases-reclaim\", "
                    "\"arguments\": { \"remove\": false } }", response);
    EXPECT_EQ("{ \"result\": 0, \"text\": "
              "\"Reclamation of expired leases is complete.\" }", response);

    // Leases should be reclaimed, but not removed
    ASSERT_NO_THROW(
        lease0 = lease_mgr.getLease6(Lease::TYPE_NA, IOAddress("3000::1"))
    );
    ASSERT_NO_THROW(
        lease1 = lease_mgr.getLease6(Lease::TYPE_NA, IOAddress("3000::2"))
    );
    ASSERT_TRUE(lease0);
    ASSERT_TRUE(lease1);
    EXPECT_TRUE(lease0->stateExpiredReclaimed());
    EXPECT_TRUE(lease1->stateExpiredReclaimed());
}

// This test verifies that the DHCP server immediately reclaims expired
// leases on leases-reclaim command with remove = true
TEST_F(CtrlChannelDhcpv6SrvTest, controlLeasesReclaimRemove) {
    createUnixChannelServer();

    // Create expired leases. Leases are expired by 40 seconds ago
    // (valid lifetime = 60, cltt = now - 100).
    DuidPtr duid0(new DUID(DUID::fromText("00:01:02:03:04:05:06").getDuid()));
    Lease6Ptr lease0(new Lease6(Lease::TYPE_NA, IOAddress("3000::1"),
                                duid0, 1, 50, 60, SubnetID(1)));
    lease0->cltt_ = time(NULL) - 100;
    DuidPtr duid1(new DUID(DUID::fromText("01:02:03:04:05:06:07").getDuid()));
    Lease6Ptr lease1(new Lease6(Lease::TYPE_NA, IOAddress("3000::2"),
                                duid1, 1, 50, 60, SubnetID(1)));
    lease1->cltt_ = time(NULL) - 100;

    // Add leases to the database.
    LeaseMgr& lease_mgr = LeaseMgrFactory::instance();
    ASSERT_NO_THROW(lease_mgr.addLease(lease0));
    ASSERT_NO_THROW(lease_mgr.addLease(lease1));

    // Make sure they have been added.
    ASSERT_TRUE(lease_mgr.getLease6(Lease::TYPE_NA, IOAddress("3000::1")));
    ASSERT_TRUE(lease_mgr.getLease6(Lease::TYPE_NA, IOAddress("3000::2")));

    // Send the command
    std::string response;
    sendUnixCommand("{ \"command\": \"leases-reclaim\", "
                    "\"arguments\": { \"remove\": true } }", response);
    EXPECT_EQ("{ \"result\": 0, \"text\": "
              "\"Reclamation of expired leases is complete.\" }", response);

    // Leases should have been removed.
    ASSERT_NO_THROW(
        lease0 = lease_mgr.getLease6(Lease::TYPE_NA, IOAddress("3000::1"))
    );
    ASSERT_NO_THROW(
        lease1 = lease_mgr.getLease6(Lease::TYPE_NA, IOAddress("3000::2"))
    );
    ASSERT_FALSE(lease0);
    ASSERT_FALSE(lease1);
}

// Tests that the server properly responds to list-commands command sent
// via ControlChannel
TEST_F(CtrlChannelDhcpv6SrvTest, listCommands) {
    createUnixChannelServer();
    std::string response;

    sendUnixCommand("{ \"command\": \"list-commands\" }", response);

    ConstElementPtr rsp;
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));

    // We expect the server to report at least the following commands:
    checkListCommands(rsp, "build-report");
    checkListCommands(rsp, "config-backend-pull");
    checkListCommands(rsp, "config-get");
    checkListCommands(rsp, "config-hash-get");
    checkListCommands(rsp, "config-reload");
    checkListCommands(rsp, "config-set");
    checkListCommands(rsp, "config-test");
    checkListCommands(rsp, "config-write");
    checkListCommands(rsp, "list-commands");
    checkListCommands(rsp, "leases-reclaim");
    checkListCommands(rsp, "version-get");
    checkListCommands(rsp, "server-tag-get");
    checkListCommands(rsp, "shutdown");
    checkListCommands(rsp, "statistic-get");
    checkListCommands(rsp, "statistic-get-all");
    checkListCommands(rsp, "statistic-remove");
    checkListCommands(rsp, "statistic-remove-all");
    checkListCommands(rsp, "statistic-reset");
    checkListCommands(rsp, "statistic-reset-all");
    checkListCommands(rsp, "statistic-sample-age-set");
    checkListCommands(rsp, "statistic-sample-age-set-all");
    checkListCommands(rsp, "statistic-sample-count-set");
    checkListCommands(rsp, "statistic-sample-count-set-all");
}

// Tests if config-write can be called without any parameters.
TEST_F(CtrlChannelDhcpv6SrvTest, configWriteNoFilename) {
    createUnixChannelServer();
    std::string response;

    // This is normally set by the command line -c parameter.
    server_->setConfigFile("test1.json");

    // If the filename is not explicitly specified, the name used
    // in -c command line switch is used.
    sendUnixCommand("{ \"command\": \"config-write\" }", response);

    checkConfigWrite(response, CONTROL_RESULT_SUCCESS, "test1.json");
    ::remove("test1.json");
}

// Tests if config-write can be called with a valid filename as parameter.
TEST_F(CtrlChannelDhcpv6SrvTest, configWriteFilename) {
    createUnixChannelServer();
    std::string response;

    // This is normally set by the command line -c parameter.
    server_->setConfigFile("test1.json");

    sendUnixCommand("{ \"command\": \"config-write\", "
                    "\"arguments\": { \"filename\": \"test2.json\" } }", response);

    checkConfigWrite(response, CONTROL_RESULT_SUCCESS, "test2.json");
    ::remove("test2.json");
}

// Tests if config-write can be called with a valid full path as parameter.
TEST_F(CtrlChannelDhcpv6SrvTest, configWriteFullPath) {
    createUnixChannelServer();
    std::string response;

    // This is normally set by the command line -c parameter.
    server_->setConfigFile("/tmp/test1.json");

    sendUnixCommand("{ \"command\": \"config-write\", "
                    "\"arguments\": { \"filename\": \"/tmp/test2.json\" } }", response);

    checkConfigWrite(response, CONTROL_RESULT_SUCCESS, "/tmp/test2.json");
    ::remove("/tmp/test2.json");
}

// Tests if config-write raises an error with invalid path as parameter.
TEST_F(CtrlChannelDhcpv6SrvTest, configWriteBadPath) {
    createUnixChannelServer();
    std::string response;

    // This is normally set by the command line -c parameter.
    server_->setConfigFile("test1.json");

    sendUnixCommand("{ \"command\": \"config-write\", "
                    "\"arguments\": { \"filename\": \"/tmp/test2.json\" } }", response);

    string expected = "not allowed to write config into /tmp/test2.json: ";
    expected += "file /tmp/test2.json must be in the same directory ";
    expected += "as the config file (test1.json)";
    checkConfigWrite(response, CONTROL_RESULT_ERROR, expected);
    ::remove("/tmp/test2.json");
}

// Tests if config-write raises an error with invalid full path as parameter.
TEST_F(CtrlChannelDhcpv6SrvTest, configWriteBadFullPath) {
    createUnixChannelServer();
    std::string response;

    // This is normally set by the command line -c parameter.
    server_->setConfigFile("/tmp/kea1/test.json");

    sendUnixCommand("{ \"command\": \"config-write\", "
                    "\"arguments\": { \"filename\": \"/tmp/kea2/test.json\" } }", response);

    string expected = "not allowed to write config into /tmp/kea2/test.json: ";
    expected += "file /tmp/kea2/test.json must be in the same directory ";
    expected += "as the config file (/tmp/kea1/test.json)";
    checkConfigWrite(response, CONTROL_RESULT_ERROR, expected);
    ::remove("/tmp/kea2/test.json");
}

// Tests if config-reload attempts to reload a file and reports that the
// file is missing.
TEST_F(CtrlChannelDhcpv6SrvTest, configReloadMissingFile) {
    createUnixChannelServer();
    std::string response;

    // This is normally set to whatever value is passed to -c when the server is
    // started, but we're not starting it that way, so need to set it by hand.
    server_->setConfigFile("test6.json");

    // Tell the server to reload its configuration. It should attempt to load
    // test6.json (and fail, because the file is not there).
    sendUnixCommand("{ \"command\": \"config-reload\" }", response);

    // Verify the reload was rejected.
    EXPECT_EQ("{ \"result\": 1, \"text\": \"Config reload failed: "
              "configuration error using file 'test6.json': Unable to open file "
              "test6.json\" }",
              response);
}

// Tests if config-reload attempts to reload a file and reports that the
// file is not a valid JSON.
TEST_F(CtrlChannelDhcpv6SrvTest, configReloadBrokenFile) {
    createUnixChannelServer();
    std::string response;

    // This is normally set to whatever value is passed to -c when the server is
    // started, but we're not starting it that way, so need to set it by hand.
    server_->setConfigFile("test7.json");

    // Although Kea is smart, its AI routines are not smart enough to handle
    // this one... at least not yet.
    ofstream f("test7.json", ios::trunc);
    f << "gimme some addrs, bro!";
    f.close();

    // Now tell Kea to reload its config.
    sendUnixCommand("{ \"command\": \"config-reload\" }", response);

    // Verify the reload will fail.
    EXPECT_EQ("{ \"result\": 1, \"text\": \"Config reload failed: "
              "configuration error using file 'test7.json': "
              "test7.json:1.1: Invalid character: g\" }",
              response);

    ::remove("test7.json");
}

// Tests if config-reload attempts to reload a file and reports that the
// file is loaded correctly.
TEST_F(CtrlChannelDhcpv6SrvTest, configReloadValid) {
    createUnixChannelServer();
    std::string response;

    // This is normally set to whatever value is passed to -c when the server is
    // started, but we're not starting it that way, so need to set it by hand.
    server_->setConfigFile("test8.json");

    // Ok, enough fooling around. Let's create a valid config.
    const std::string cfg_txt =
        "{ \"Dhcp6\": {"
        "    \"interfaces-config\": {"
        "        \"interfaces\": [ \"*\" ]"
        "    },"
        "    \"subnet6\": ["
        "        { \"subnet\": \"2001:db8:1::/64\", \"id\": 1 },"
        "        { \"subnet\": \"2001:db8:2::/64\", \"id\": 2 }"
        "     ],"
        "    \"lease-database\": {"
        "       \"type\": \"memfile\", \"persist\": false }"
        "} }";
    ofstream f("test8.json", ios::trunc);
    f << cfg_txt;
    f.close();

    // This command should reload test8.json config.
    sendUnixCommand("{ \"command\": \"config-reload\" }", response);

    // Verify the configuration was successful. The config contains random
    // socket name (/tmp/kea-<value-changing-each-time>/kea6.sock), so the
    // hash will be different each time. As such, we can do simplified checks:
    // - verify the "result": 0 is there
    // - verify the "text": "Configuration successful." is there
    EXPECT_NE(response.find("\"result\": 0"), std::string::npos);
    EXPECT_NE(response.find("\"text\": \"Configuration successful.\""), std::string::npos);

    // Check that the config was indeed applied.
    const Subnet6Collection* subnets =
        CfgMgr::instance().getCurrentCfg()->getCfgSubnets6()->getAll();
    EXPECT_EQ(2, subnets->size());

    ::remove("test8.json");
}

// Tests if config-reload attempts to reload a file and reports that the
// file is loaded correctly.
TEST_F(CtrlChannelDhcpv6SrvTest, configReloadDetectInterfaces) {
    interfaces_ = "\"eth0\"";
    IfacePtr eth0 = IfaceMgrTestConfig::createIface("eth0", ETH0_INDEX,
                                                    "11:22:33:44:55:66");
    auto detectIfaces = [&](bool update_only) {
        if (!update_only) {
            eth0->addAddress(IOAddress("10.0.0.1"));
            eth0->addAddress(IOAddress("fe80::3a60:77ff:fed5:cdef"));
            eth0->addAddress(IOAddress("2001:db8:1::1"));
            IfaceMgr::instance().addInterface(eth0);
        }
        return (false);
    };
    IfaceMgr::instance().setDetectCallback(detectIfaces);
    IfaceMgr::instance().clearIfaces();
    IfaceMgr::instance().closeSockets();
    IfaceMgr::instance().detectIfaces();
    createUnixChannelServer();
    std::string response;

    // This is normally set to whatever value is passed to -c when the server is
    // started, but we're not starting it that way, so need to set it by hand.
    server_->setConfigFile("test8.json");

    // Ok, enough fooling around. Let's create a valid config.
    const std::string cfg_txt =
        "{ \"Dhcp6\": {"
        "    \"interfaces-config\": {"
        "        \"interfaces\": [ \"eth1\" ]"
        "    },"
        "    \"subnet6\": ["
        "        { \"subnet\": \"2001:db8:1::/64\", \"id\": 1 },"
        "        { \"subnet\": \"2001:db8:2::/64\", \"id\": 2 }"
        "     ],"
        "    \"lease-database\": {"
        "       \"type\": \"memfile\", \"persist\": false }"
        "} }";
    ofstream f("test8.json", ios::trunc);
    f << cfg_txt;
    f.close();

    IfacePtr eth1 = IfaceMgrTestConfig::createIface("eth1", ETH1_INDEX,
                                                    "AA:BB:CC:DD:EE:FF");
    auto detectUpdateIfaces = [&](bool update_only) {
        if (!update_only) {
            eth1->addAddress(IOAddress("192.0.2.3"));
            eth1->addAddress(IOAddress("fe80::3a60:77ff:fed5:abcd"));
            eth1->addAddress(IOAddress("3001:db8:100::1"));
            IfaceMgr::instance().addInterface(eth1);
        }
        return (false);
    };
    IfaceMgr::instance().setDetectCallback(detectUpdateIfaces);

    // This command should reload test8.json config.
    sendUnixCommand("{ \"command\": \"config-reload\" }", response);

    // Verify the configuration was successful. The config contains random
    // socket name (/tmp/kea-<value-changing-each-time>/kea6.sock), so the
    // hash will be different each time. As such, we can do simplified checks:
    // - verify the "result": 0 is there
    // - verify the "text": "Configuration successful." is there
    EXPECT_NE(response.find("\"result\": 0"), std::string::npos);
    EXPECT_NE(response.find("\"text\": \"Configuration successful.\""), std::string::npos);

    // Check that the config was indeed applied.
    const Subnet6Collection* subnets =
        CfgMgr::instance().getCurrentCfg()->getCfgSubnets6()->getAll();
    EXPECT_EQ(2, subnets->size());

    ::remove("test8.json");
}

// This test verifies that disable DHCP service command performs sanity check on
// parameters.
TEST_F(CtrlChannelDhcpv6SrvTest, dhcpDisableBadParam) {
    createUnixChannelServer();
    std::string response;

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-disable\","
                    "    \"arguments\": {"
                    "        \"max-period\": -3"
                    "    }"
                    "}", response);
    ConstElementPtr rsp;

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    EXPECT_EQ("{ \"result\": 1, \"text\": \"'max-period' must be positive "
              "integer\" }", response);

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-disable\","
                    "    \"arguments\": {"
                    "        \"origin-id\": \"\""
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    EXPECT_EQ("{ \"result\": 1, \"text\": \"'origin-id' argument must be a number\" }", response);

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-disable\","
                    "    \"arguments\": {"
                    "        \"origin-id\": \"foo\""
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    EXPECT_EQ("{ \"result\": 1, \"text\": \"'origin-id' argument must be a number\" }", response);

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-disable\","
                    "    \"arguments\": {"
                    "        \"origin\": \"\""
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    EXPECT_EQ("{ \"result\": 1, \"text\": \"invalid value used for 'origin' "
              "parameter: (empty string)\" }", response);

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-disable\","
                    "    \"arguments\": {"
                    "        \"origin\": \"test\""
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    EXPECT_EQ("{ \"result\": 1, \"text\": \"invalid value used for 'origin' "
              "parameter: test\" }", response);
}

// This test verifies if it is possible to disable DHCP service via command.
TEST_F(CtrlChannelDhcpv6SrvTest, dhcpDisable) {
    createUnixChannelServer();
    std::string response;

    sendUnixCommand("{ \"command\": \"dhcp-disable\" }", response);
    ConstElementPtr rsp;

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    int status;
    ConstElementPtr cfg = parseAnswer(status, rsp);
    EXPECT_EQ(CONTROL_RESULT_SUCCESS, status);

    EXPECT_FALSE(server_->network_state_->isServiceEnabled());

    server_->network_state_->enableService(NetworkState::USER_COMMAND);

    EXPECT_TRUE(server_->network_state_->isServiceEnabled());

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-disable\","
                    "    \"arguments\": {"
                    "        \"origin\": \"user\""
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    cfg = parseAnswer(status, rsp);
    EXPECT_EQ(CONTROL_RESULT_SUCCESS, status);

    EXPECT_FALSE(server_->network_state_->isServiceEnabled());

    server_->network_state_->enableService(NetworkState::USER_COMMAND);

    EXPECT_TRUE(server_->network_state_->isServiceEnabled());

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-disable\","
                    "    \"arguments\": {"
                    "        \"origin\": \"ha-partner\""
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    cfg = parseAnswer(status, rsp);
    EXPECT_EQ(CONTROL_RESULT_SUCCESS, status);

    EXPECT_FALSE(server_->network_state_->isServiceEnabled());

    server_->network_state_->enableService(NetworkState::HA_REMOTE_COMMAND);

    EXPECT_TRUE(server_->network_state_->isServiceEnabled());

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-disable\","
                    "    \"arguments\": {"
                    "        \"origin\": 2001"
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    cfg = parseAnswer(status, rsp);
    EXPECT_EQ(CONTROL_RESULT_SUCCESS, status);

    EXPECT_FALSE(server_->network_state_->isServiceEnabled());

    server_->network_state_->enableService(NetworkState::HA_REMOTE_COMMAND+1);

    EXPECT_TRUE(server_->network_state_->isServiceEnabled());
}

// This test verifies if it is possible to disable DHCP service using
// the origin-id.
TEST_F(CtrlChannelDhcpv6SrvTest, dhcpDisableOriginId) {
    createUnixChannelServer();
    std::string response;

    EXPECT_TRUE(server_->network_state_->isServiceEnabled());

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-disable\","
                    "    \"arguments\": {"
                    "        \"origin-id\": 2002,"
                    "        \"origin\": \"user\""
                    "    }"
                    "}", response);

    ConstElementPtr rsp;

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    int status;
    ConstElementPtr cfg = parseAnswer(status, rsp);
    EXPECT_EQ(CONTROL_RESULT_SUCCESS, status);

    EXPECT_FALSE(server_->network_state_->isServiceEnabled());

    server_->network_state_->enableService(NetworkState::HA_REMOTE_COMMAND+2);

    EXPECT_TRUE(server_->network_state_->isServiceEnabled());
}

// This test verifies that it is possible to disable DHCP service for a short
// period of time, after which the service is automatically enabled.
TEST_F(CtrlChannelDhcpv6SrvTest, dhcpDisableTemporarily) {
    createUnixChannelServer();
    std::string response;

    // Send a command to disable DHCP service for 3 seconds.
    sendUnixCommand("{"
                    "    \"command\": \"dhcp-disable\","
                    "    \"arguments\": {"
                    "        \"max-period\": 3"
                    "    }"
                    "}", response);
    ConstElementPtr rsp;

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    int status;
    ConstElementPtr cfg = parseAnswer(status, rsp);
    EXPECT_EQ(CONTROL_RESULT_SUCCESS, status);

    // The service should be disabled.
    EXPECT_FALSE(server_->network_state_->isServiceEnabled());
    // And the timer should be scheduled which counts the time to automatic
    // enabling of the service.
    EXPECT_TRUE(server_->network_state_->isDelayedEnableService());
}

// This test verifies that enable DHCP service command performs sanity check on
// parameters.
TEST_F(CtrlChannelDhcpv6SrvTest, dhcpEnableBadParam) {
    createUnixChannelServer();
    std::string response;
    ConstElementPtr rsp;

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-enable\","
                    "    \"arguments\": {"
                    "        \"origin-id\": \"\""
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    EXPECT_EQ("{ \"result\": 1, \"text\": \"'origin-id' argument must be a number\" }", response);

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-enable\","
                    "    \"arguments\": {"
                    "        \"origin-id\": \"foo\""
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    EXPECT_EQ("{ \"result\": 1, \"text\": \"'origin-id' argument must be a number\" }", response);

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-enable\","
                    "    \"arguments\": {"
                    "        \"origin\": \"\""
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    EXPECT_EQ("{ \"result\": 1, \"text\": \"invalid value used for 'origin' "
              "parameter: (empty string)\" }", response);

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-enable\","
                    "    \"arguments\": {"
                    "        \"origin\": \"test\""
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    EXPECT_EQ("{ \"result\": 1, \"text\": \"invalid value used for 'origin' "
              "parameter: test\" }", response);
}

// This test verifies if it is possible to enable DHCP service via command.
TEST_F(CtrlChannelDhcpv6SrvTest, dhcpEnable) {
    createUnixChannelServer();
    std::string response;

    sendUnixCommand("{ \"command\": \"dhcp-enable\" }", response);
    ConstElementPtr rsp;

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    int status;
    ConstElementPtr cfg = parseAnswer(status, rsp);
    EXPECT_EQ(CONTROL_RESULT_SUCCESS, status);

    EXPECT_TRUE(server_->network_state_->isServiceEnabled());

    server_->network_state_->disableService(NetworkState::USER_COMMAND);

    EXPECT_FALSE(server_->network_state_->isServiceEnabled());

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-enable\","
                    "    \"arguments\": {"
                    "        \"origin\": \"user\""
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    cfg = parseAnswer(status, rsp);
    EXPECT_EQ(CONTROL_RESULT_SUCCESS, status);

    EXPECT_TRUE(server_->network_state_->isServiceEnabled());

    server_->network_state_->disableService(NetworkState::HA_REMOTE_COMMAND);

    EXPECT_FALSE(server_->network_state_->isServiceEnabled());

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-enable\","
                    "    \"arguments\": {"
                    "        \"origin\": \"ha-partner\""
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    cfg = parseAnswer(status, rsp);
    EXPECT_EQ(CONTROL_RESULT_SUCCESS, status);

    EXPECT_TRUE(server_->network_state_->isServiceEnabled());

    server_->network_state_->disableService(NetworkState::HA_REMOTE_COMMAND+2);

    EXPECT_FALSE(server_->network_state_->isServiceEnabled());

    sendUnixCommand("{"
                    "    \"command\": \"dhcp-enable\","
                    "    \"arguments\": {"
                    "        \"origin\": 2002"
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    cfg = parseAnswer(status, rsp);
    EXPECT_EQ(CONTROL_RESULT_SUCCESS, status);

    EXPECT_TRUE(server_->network_state_->isServiceEnabled());
}

// This test verifies if it is possible to enable DHCP service using
// the origin-id.
TEST_F(CtrlChannelDhcpv6SrvTest, dhcpEnableOriginId) {
    createUnixChannelServer();
    std::string response;

    ConstElementPtr rsp;

    int status;

    // Disable the service using two distinct origins.
    server_->network_state_->disableService(NetworkState::HA_REMOTE_COMMAND+1);
    server_->network_state_->disableService(NetworkState::USER_COMMAND);

    EXPECT_FALSE(server_->network_state_->isServiceEnabled());

    // Enable the service for the 'origin-id' of 2001. The 'origin' should
    // be ignored.
    sendUnixCommand("{"
                    "    \"command\": \"dhcp-enable\","
                    "    \"arguments\": {"
                    "        \"origin-id\": 2001,"
                    "        \"origin\": \"user\""
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    ConstElementPtr cfg = parseAnswer(status, rsp);
    EXPECT_EQ(CONTROL_RESULT_SUCCESS, status);

    // The service should still be disabled.
    EXPECT_FALSE(server_->network_state_->isServiceEnabled());

    // Enable the service for the user command.
    sendUnixCommand("{"
                    "    \"command\": \"dhcp-enable\","
                    "    \"arguments\": {"
                    "        \"origin-id\": 1"
                    "    }"
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    cfg = parseAnswer(status, rsp);
    EXPECT_EQ(CONTROL_RESULT_SUCCESS, status);

    // The service should be enabled.
    EXPECT_TRUE(server_->network_state_->isServiceEnabled());
}

// This test verifies that subnet6-select-test command performs sanity check parameters.
TEST_F(CtrlChannelDhcpv6SrvTest, subnet6SelectTestBadParam) {
    createUnixChannelServer();
    std::string response;
    ConstElementPtr rsp;

    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\""
                    "}", response);

    // The response should be a valid JSON.
    EXPECT_NO_THROW(rsp = Element::fromJSON(response));
    ASSERT_TRUE(rsp);

    EXPECT_EQ("{ \"result\": 1, \"text\": \"empty arguments\" }",
              response);

    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": [ ]"
                    "    }"
                    "}", response);
    EXPECT_EQ("{ \"result\": 1, \"text\": \"arguments must be a map\" }",
              response);

    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"foo\": \"bar\""
                    "    }"
                    "}", response);
    EXPECT_EQ("{ \"result\": 1, \"text\": \"unknown entry 'foo'\" }",
              response);

    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"interface\": 1"
                    "    }"
                    "}", response);
    string expected = "{ \"result\": 1, \"text\": \"";
    expected += "'interface' entry must be a string";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"interface-id\": 1"
                    "    }"
                    "}", response);
    expected = "{ \"result\": 1, \"text\": \"";
    expected += "'interface-id' entry must be a string";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"interface-id\": \"\""
                    "    }"
                    "}", response);
    expected = "{ \"result\": 1, \"text\": \"";
    expected += "'interface-id' must be not empty";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"interface-id\": \"foo\""
                    "    }"
                    "}", response);
    expected = "{ \"result\": 1, \"text\": \"";
    expected += "value of 'interface-id' was not recognized";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"remote\": 1"
                    "    }"
                    "}", response);
    expected = "{ \"result\": 1, \"text\": \"";
    expected += "'remote' entry must be a string";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"remote\": \"192.2.1.2\""
                    "    }"
                    "}", response);
    expected = "{ \"result\": 1, \"text\": \"";
    expected += "bad 'remote' entry: not IPv6";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"remote\": \"foobar\""
                    "    }"
                    "}", response);
    expected = "{ \"result\": 1, \"text\": \"";
    expected += "bad 'remote' entry: Failed to convert string to address ";
    expected += "'foobar': Invalid argument";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"link\": 1"
                    "    }"
                    "}", response);
    expected = "{ \"result\": 1, \"text\": \"";
    expected += "'link' entry must be a string";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"link\": \"192.2.1.2\""
                    "    }"
                    "}", response);
    expected = "{ \"result\": 1, \"text\": \"";
    expected += "bad 'link' entry: not IPv6";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"link\": \"foobar\""
                    "    }"
                    "}", response);
    expected = "{ \"result\": 1, \"text\": \"";
    expected += "bad 'link' entry: Failed to convert string to address ";
    expected += "'foobar': Invalid argument";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"classes\": \"foo\""
                    "    }"
                    "}", response);
    expected = "{ \"result\": 1, \"text\": \"";
    expected += "'classes' entry must be a list";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"classes\": [ 1 ]"
                    "    }"
                    "}", response);
    expected = "{ \"result\": 1, \"text\": \"";
    expected += "'classes' entry must be a list of strings";
    expected += "\" }";
    EXPECT_EQ(expected, response);
}

// This test verifies if subnet6-select-test command returns proper subnet for a given
// remote/source address.
TEST_F(CtrlChannelDhcpv6SrvTest, subnet6SelectTestAddr) {
    createUnixChannelServer();
    std::string response;

    auto subnet = Subnet6::create(IOAddress("2001:db8:1::"),
                                  64, 1, 2, 3, 4, SubnetID(1));
    SharedNetwork6Ptr network(new SharedNetwork6("foo"));
    CfgMgr::instance().clear();
    CfgMgr::instance().getStagingCfg()->getCfgSubnets6()->add(subnet);
    CfgMgr::instance().getStagingCfg()->getCfgSharedNetworks6()->add(network);
    CfgMgr::instance().commit();

    // Address not in range: nothing can be selected.
    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"remote\": \"fe80::abcd\""
                    "    }"
                    "}", response);
    EXPECT_EQ("{ \"result\": 3, \"text\": \"no subnet selected\" }",
              response);

    // Address in range: the subnet is selected.
    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"remote\": \"2001:db8:1::1\""
                    "    }"
                    "}", response);
    string expected = "{ \"result\": 0, \"text\": \"";
    expected += "selected subnet '2001:db8:1::/64' id 1";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    // Add the subnet to the shared network.
    subnet->setSharedNetwork(network);
    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"remote\": \"2001:db8:1::1\""
                    "    }"
                    "}", response);
    expected = "{ \"result\": 0, \"text\": \"";
    expected += "selected shared network 'foo' ";
    expected += "starting with subnet '2001:db8:1::/64' id 1";
    expected += "\" }";
    EXPECT_EQ(expected, response);
}

// This test verifies if subnet6-select-test command returns proper subnet for a given
// incoming interface.
TEST_F(CtrlChannelDhcpv6SrvTest, subnet6SelectTestIface) {
    createUnixChannelServer();
    std::string response;

    auto subnet = Subnet6::create(IOAddress("2001:db8:1::"),
                                  64, 1, 2, 3, 4, SubnetID(1));
    subnet->setIface("eth0");
    SharedNetwork6Ptr network(new SharedNetwork6("foo"));
    CfgMgr::instance().clear();
    CfgMgr::instance().getStagingCfg()->getCfgSubnets6()->add(subnet);
    CfgMgr::instance().getStagingCfg()->getCfgSharedNetworks6()->add(network);
    CfgMgr::instance().commit();

    // Different interface: nothing can be selected.
    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"interface\": \"bar\""
                    "    }"
                    "}", response);
    EXPECT_EQ("{ \"result\": 3, \"text\": \"no subnet selected\" }",
              response);

    // Same interface: the subnet is selected.
    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"interface\": \"eth0\""
                    "    }"
                    "}", response);
    string expected = "{ \"result\": 0, \"text\": \"";
    expected += "selected subnet '2001:db8:1::/64' id 1";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    // Add the subnet to the shared network.
    subnet->setSharedNetwork(network);
    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"interface\": \"eth0\""
                    "    }"
                    "}", response);
    expected = "{ \"result\": 0, \"text\": \"";
    expected += "selected shared network 'foo' ";
    expected += "starting with subnet '2001:db8:1::/64' id 1";
    expected += "\" }";
    EXPECT_EQ(expected, response);
}

// This test verifies if subnet6-select-test command returns proper subnet for a given
// relay link address.
TEST_F(CtrlChannelDhcpv6SrvTest, subnet6SelectTestRelayLinkaddr) {
    createUnixChannelServer();
    std::string response;

    auto subnet = Subnet6::create(IOAddress("2001:db8:1::"),
                                  64, 1, 2, 3, 4, SubnetID(1));
    SharedNetwork6Ptr network(new SharedNetwork6("foo"));
    CfgMgr::instance().clear();
    CfgMgr::instance().getStagingCfg()->getCfgSubnets6()->add(subnet);
    CfgMgr::instance().getStagingCfg()->getCfgSharedNetworks6()->add(network);
    CfgMgr::instance().commit();

    // Address not in range: nothing can be selected.
    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"link\": \"2001:db8:2::2\""
                    "    }"
                    "}", response);
    EXPECT_EQ("{ \"result\": 3, \"text\": \"no subnet selected\" }",
              response);

    // Address in range: the subnet is selected.
    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"link\": \"2001:db8:1::1\""
                    "    }"
                    "}", response);
    string expected = "{ \"result\": 0, \"text\": \"";
    expected += "selected subnet '2001:db8:1::/64' id 1";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    // Add the subnet to the shared network.
    subnet->setSharedNetwork(network);
    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"link\": \"2001:db8:1::1\""
                    "    }"
                    "}", response);
    expected = "{ \"result\": 0, \"text\": \"";
    expected += "selected shared network 'foo' ";
    expected += "starting with subnet '2001:db8:1::/64' id 1";
    expected += "\" }";
    EXPECT_EQ(expected, response);
}

// This test verifies if subnet6-select-test command returns proper subnet for a given
// relay interface id.
TEST_F(CtrlChannelDhcpv6SrvTest, subnet6SelectTestRelayInterfaceId) {
    createUnixChannelServer();
    std::string response;

    auto subnet = Subnet6::create(IOAddress("2001:db8:1::"),
                                  64, 1, 2, 3, 4, SubnetID(1));
    string iface_id("relay");
    vector<uint8_t> bin(iface_id.cbegin(), iface_id.cend());
    OptionPtr id(new Option(Option::V6, D6O_INTERFACE_ID, bin));
    subnet->setInterfaceId(id);
    SharedNetwork6Ptr network(new SharedNetwork6("foo"));
    CfgMgr::instance().clear();
    CfgMgr::instance().getStagingCfg()->getCfgSubnets6()->add(subnet);
    CfgMgr::instance().getStagingCfg()->getCfgSharedNetworks6()->add(network);
    CfgMgr::instance().commit();

    // Note that below a relay link address is required: it says the client
    // is behind a relay.
    // Different interface id: nothing can be selected.
    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"link\": \"2001:db8:2::2\","
                    "        \"interface-id\": \"'foobar'\""
                    "    }"
                    "}", response);
    EXPECT_EQ("{ \"result\": 3, \"text\": \"no subnet selected\" }",
              response);

    // Same interface id: the subnet is selected.
    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"link\": \"2001:db8:2::2\","
                    "        \"interface-id\": \"'relay'\""
                    "    }"
                    "}", response);
    string expected = "{ \"result\": 0, \"text\": \"";
    expected += "selected subnet '2001:db8:1::/64' id 1";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    // Add the subnet to the shared network.
    subnet->setSharedNetwork(network);
    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"link\": \"2001:db8:2::2\","
                    "        \"interface-id\": \"'relay'\""
                    "    }"
                    "}", response);
    expected = "{ \"result\": 0, \"text\": \"";
    expected += "selected shared network 'foo' ";
    expected += "starting with subnet '2001:db8:1::/64' id 1";
    expected += "\" }";
    EXPECT_EQ(expected, response);
}

// This test verifies if subnet6-select-test command returns proper guarded subnet.
TEST_F(CtrlChannelDhcpv6SrvTest, subnet6SelectTestClass) {
    createUnixChannelServer();
    std::string response;

    auto subnet = Subnet6::create(IOAddress("2001:db8:1::"),
                                  64, 1, 2, 3, 4, SubnetID(1));
    subnet->allowClientClass("foobar");
    SharedNetwork6Ptr network(new SharedNetwork6("foo"));
    CfgMgr::instance().clear();
    CfgMgr::instance().getStagingCfg()->getCfgSubnets6()->add(subnet);
    CfgMgr::instance().getStagingCfg()->getCfgSharedNetworks6()->add(network);
    CfgMgr::instance().commit();

    // Address in range but not in guard: nothing can be selected.
    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"remote\": \"2001:db8:1::1\""
                    "    }"
                    "}", response);
    EXPECT_EQ("{ \"result\": 3, \"text\": \"no subnet selected\" }",
              response);

    // Address in range and in guard: the subnet is selected.
    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"remote\": \"2001:db8:1::1\","
                    "        \"classes\": [ \"foobar\" ]"
                    "    }"
                    "}", response);
    string expected = "{ \"result\": 0, \"text\": \"";
    expected += "selected subnet '2001:db8:1::/64' id 1";
    expected += "\" }";
    EXPECT_EQ(expected, response);

    // Add the subnet to the shared network.
    subnet->setSharedNetwork(network);
    sendUnixCommand("{"
                    "    \"command\": \"subnet6-select-test\","
                    "    \"arguments\": {"
                    "        \"remote\": \"2001:db8:1::1\","
                    "        \"classes\": [ \"foobar\" ]"
                    "    }"
                    "}", response);
    expected = "{ \"result\": 0, \"text\": \"";
    expected += "selected shared network 'foo' ";
    expected += "starting with subnet '2001:db8:1::/64' id 1";
    expected += "\" }";
    EXPECT_EQ(expected, response);
}

/// Verify that concurrent connections over the control channel can be
///  established.
/// @todo Future Kea 1.3 tickets will modify the behavior of the CommandMgr
/// such that the server will be able to send response in multiple chunks.
/// This test will need to be extended. For now, the receive and write
/// operations are atomic and there is no conflict between concurrent
/// connections.
TEST_F(CtrlChannelDhcpv6SrvTest, concurrentConnections) {
    createUnixChannelServer();

    boost::scoped_ptr<UnixControlClient> client1(new UnixControlClient());
    ASSERT_TRUE(client1);

    boost::scoped_ptr<UnixControlClient> client2(new UnixControlClient());
    ASSERT_TRUE(client2);

    // Client 1 connects.
    ASSERT_TRUE(client1->connectToServer(socket_path_));
    ASSERT_NO_THROW(getIOService()->poll());

    // Client 2 connects.
    ASSERT_TRUE(client2->connectToServer(socket_path_));
    ASSERT_NO_THROW(getIOService()->poll());

    // Send the command while another client is connected.
    ASSERT_TRUE(client2->sendCommand("{ \"command\": \"list-commands\" }"));
    ASSERT_NO_THROW(getIOService()->poll());

    std::string response;
    // The server should respond ok.
    ASSERT_TRUE(client2->getResponse(response));
    EXPECT_TRUE(response.find("\"result\": 0") != std::string::npos);

    // Disconnect the servers.
    client1->disconnectFromServer();
    client2->disconnectFromServer();
    ASSERT_NO_THROW(getIOService()->poll());
}

// This test verifies that the server can receive and process a large command.
TEST_F(CtrlChannelDhcpv6SrvTest, longCommand) {

    std::ostringstream command;

    // This is the desired size of the command sent to the server (1MB). The
    // actual size sent will be slightly greater than that.
    const ssize_t command_size = 1024 * 1000;

    while (command.tellp() < command_size) {

        // We're sending command 'foo' with arguments being a list of
        // strings. If this is the first transmission, send command name
        // and open the arguments list. Also insert the first argument
        // so as all subsequent arguments can be prefixed with a comma.
        if (command.tellp() == 0) {
            command << "{ \"command\": \"foo\", \"arguments\": [ \"begin\"";

        } else {
            // Generate a random number and insert it into the stream as
            // 10 digits long string.
            std::ostringstream arg;
            arg << setw(10) << std::rand();
            // Append the argument in the command.
            command << ", \"" << arg.str() << "\"\n";

            // If we have hit the limit of the command size, close braces to
            // get appropriate JSON.
            if (command.tellp() > command_size) {
                command << "] }";
            }
        }
    }

    ASSERT_NO_THROW(
        CommandMgr::instance().registerCommand("foo",
             std::bind(&CtrlChannelDhcpv6SrvTest::longCommandHandler,
                       command.str(), ph::_1, ph::_2));
    );

    createUnixChannelServer();

    std::string response;
    std::thread th([this, &response, &command]() {

        // IO service will be stopped automatically when this object goes
        // out of scope and is destroyed. This is useful because we use
        // asserts which may break the thread in various exit points.
        IOServiceWork work(getIOService());

        // Create client which we will use to send command to the server.
        boost::scoped_ptr<UnixControlClient> client(new UnixControlClient());
        ASSERT_TRUE(client);

        // Connect to the server. This will trigger acceptor handler on the
        // server side and create a new connection.
        ASSERT_TRUE(client->connectToServer(socket_path_));

        // Initially the remaining_string holds the entire command and we
        // will be erasing the portions that we have sent.
        std::string remaining_data = command.str();
        while (!remaining_data.empty()) {
            // Send the command in chunks of 1024 bytes.
            const size_t l = remaining_data.size() < 1024 ? remaining_data.size() : 1024;
            ASSERT_TRUE(client->sendCommand(remaining_data.substr(0, l)));
            remaining_data.erase(0, l);
        }

        // Set timeout to 5 seconds to allow the time for the server to send
        // a response.
        const unsigned int timeout = 5;
        ASSERT_TRUE(client->getResponse(response, timeout));

        // We're done. Close the connection to the server.
        client->disconnectFromServer();
    });

    // Run the server until the command has been processed and response
    // received.
    getIOService()->run();

    // Wait for the thread to complete.
    th.join();

    EXPECT_EQ("{ \"result\": 0, \"text\": \"long command received ok\" }",
              response);
}

// This test verifies that the server can send long response to the client.
TEST_F(CtrlChannelDhcpv6SrvTest, longResponse) {
    // We need to generate large response. The simplest way is to create
    // a command and a handler which will generate some static response
    // of a desired size.
    ASSERT_NO_THROW(
        CommandMgr::instance().registerCommand("foo",
             std::bind(&CtrlChannelDhcpv6SrvTest::longResponseHandler, ph::_1, ph::_2));
    );

    createUnixChannelServer();

    // The UnixControlClient doesn't have any means to check that the entire
    // response has been received. What we want to do is to generate a
    // reference response using our command handler and then compare
    // what we have received over the unix domain socket with this reference
    // response to figure out when to stop receiving.
    std::string reference_response = longResponseHandler("foo", ConstElementPtr())->str();

    // In this stream we're going to collect out partial responses.
    std::ostringstream response;

    // The client is synchronous so it is useful to run it in a thread.
    std::thread th([this, &response, reference_response]() {

        // IO service will be stopped automatically when this object goes
        // out of scope and is destroyed. This is useful because we use
        // asserts which may break the thread in various exit points.
        IOServiceWork work(getIOService());

        // Remember the response size so as we know when we should stop
        // receiving.
        const ssize_t long_response_size = reference_response.size();

        // Create the client and connect it to the server.
        boost::scoped_ptr<UnixControlClient> client(new UnixControlClient());
        ASSERT_TRUE(client);
        ASSERT_TRUE(client->connectToServer(socket_path_));

        // Send the stub command.
        std::string command = "{ \"command\": \"foo\", \"arguments\": { }  }";
        ASSERT_TRUE(client->sendCommand(command));

        // Keep receiving response data until we have received the full answer.
        while (response.tellp() < long_response_size) {
            std::string partial;
            const unsigned int timeout = 5;
            ASSERT_TRUE(client->getResponse(partial, timeout));
            response << partial;
        }

        // We have received the entire response, so close the connection and
        // stop the IO service.
        client->disconnectFromServer();
    });

    // Run the server until the entire response has been received.
    getIOService()->run();

    // Wait for the thread to complete.
    th.join();

    // Make sure we have received correct response.
    EXPECT_EQ(reference_response, response.str());
}

// This test verifies that the server signals timeout if the transmission
// takes too long, having received a partial command.
TEST_F(CtrlChannelDhcpv6SrvTest, connectionTimeoutPartialCommand) {
    createUnixChannelServer();

    // Set connection timeout to 2s to prevent long waiting time for the
    // timeout during this test.
    const unsigned short timeout = 2000;
    UnixCommandMgr::instance().setConnectionTimeout(timeout);

    // Server's response will be assigned to this variable.
    std::string response;

    // It is useful to create a thread and run the server and the client
    // at the same time and independently.
    std::thread th([this, &response]() {

        // IO service will be stopped automatically when this object goes
        // out of scope and is destroyed. This is useful because we use
        // asserts which may break the thread in various exit points.
        IOServiceWork work(getIOService());

        // Create the client and connect it to the server.
        boost::scoped_ptr<UnixControlClient> client(new UnixControlClient());
        ASSERT_TRUE(client);
        ASSERT_TRUE(client->connectToServer(socket_path_));

        // Send partial command. The server will be waiting for the remaining
        // part to be sent and will eventually signal a timeout.
        std::string command = "{ \"command\": \"foo\" ";
        ASSERT_TRUE(client->sendCommand(command));

        // Let's wait up to 15s for the server's response. The response
        // should arrive sooner assuming that the timeout mechanism for
        // the server is working properly.
        const unsigned int response_timeout = 15;
        ASSERT_TRUE(client->getResponse(response, response_timeout));

        // Explicitly close the client's connection.
        client->disconnectFromServer();
    });

    // Run the server until stopped.
    getIOService()->run();

    // Wait for the thread to return.
    th.join();

    // Check that the server has signalled a timeout.
    EXPECT_EQ("{ \"result\": 1, \"text\": "
              "\"Connection over control channel timed out, "
              "discarded partial command of 19 bytes\" }", response);
}

// This test verifies that the server signals timeout if the transmission
// takes too long, having received no data from the client.
TEST_F(CtrlChannelDhcpv6SrvTest, connectionTimeoutNoData) {
    createUnixChannelServer();

    // Set connection timeout to 2s to prevent long waiting time for the
    // timeout during this test.
    const unsigned short timeout = 2000;
    UnixCommandMgr::instance().setConnectionTimeout(timeout);

    // Server's response will be assigned to this variable.
    std::string response;

    // It is useful to create a thread and run the server and the client
    // at the same time and independently.
    std::thread th([this, &response]() {

        // IO service will be stopped automatically when this object goes
        // out of scope and is destroyed. This is useful because we use
        // asserts which may break the thread in various exit points.
        IOServiceWork work(getIOService());

        // Create the client and connect it to the server.
        boost::scoped_ptr<UnixControlClient> client(new UnixControlClient());
        ASSERT_TRUE(client);
        ASSERT_TRUE(client->connectToServer(socket_path_));

        // Having sent nothing let's just wait and see if Server times us out.
        // Let's wait up to 15s for the server's response. The response
        // should arrive sooner assuming that the timeout mechanism for
        // the server is working properly.
        const unsigned int response_timeout = 15;
        ASSERT_TRUE(client->getResponse(response, response_timeout));

        // Explicitly close the client's connection.
        client->disconnectFromServer();
    });

    // Run the server until stopped.
    getIOService()->run();

    // Wait for the thread to return.
    th.join();

    // Check that the server has signalled a timeout.
    EXPECT_EQ("{ \"result\": 1, \"text\": "
              "\"Connection over control channel timed out\" }", response);
}

} // End of anonymous namespace
