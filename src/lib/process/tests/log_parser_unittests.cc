// Copyright (C) 2014-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <cc/data.h>
#include <process/log_parser.h>
#include <process/process_messages.h>
#include <process/daemon.h>
#include <exceptions/exceptions.h>
#include <log/logger_support.h>
#include <process/d_log.h>
#include <util/filesystem.h>
#include <testutils/gtest_utils.h>
#include <testutils/io_utils.h>
#include <testutils/log_utils.h>

#include <gtest/gtest.h>

using namespace isc;
using namespace isc::process;
using namespace isc::data;
using namespace isc::util;

namespace {

/// @brief Logging Test Fixture Class
///
/// Trivial class that ensures that the logging is reset to its defaults after
/// each test.  Strictly speaking this only resets the testing root logger (which
/// has the name "kea") but as the only other logger mentioned here ("wombat")
/// is not used elsewhere, that is sufficient.
class LoggingTest : public dhcp::test::LogContentTest {
   public:
        /// @brief Constructor
        LoggingTest() {
            resetLogPath();
        }

        /// @brief Destructor
        ///
        /// Reset root logger back to defaults.
        ~LoggingTest() {
            isc::log::initLogger();
            wipeFiles();
            resetLogPath();
        }

    /// @brief Generates a log file name suffixed with a rotation number
    /// @param rotation number to the append to the end of the file
    std::string logName(int rotation) {
        std::ostringstream os;
        os << TEST_LOG_NAME << "." << rotation;
        return (os.str());
    }

    /// @brief Removes the base log file name and 1 rotation
    void wipeFiles()  {
        static_cast<void>(remove(TEST_LOG_NAME));
        for (int i = 1; i < TEST_MAX_VERS + 1; ++i) {
            static_cast<void>(remove(logName(i).c_str()));
        }

        // Remove the lock file
        std::ostringstream os;
        os << TEST_LOG_NAME << ".lock";
        static_cast<void>(remove(os.str().c_str()));
    }

    /// @brief Sets the Hooks path from which hooks can be loaded.
    /// @param explicit_path path to use as the hooks path.
    void setLogTestPath(const std::string explicit_path = "") {
        LogConfigParser::getLogPath(true,
                                     (!explicit_path.empty() ?
                                      explicit_path : TEST_DATA_BUILDDIR));
    }

    /// @brief Resets the log path to default.
    void resetLogPath() {
        LogConfigParser::getLogPath(true);
        file::PathChecker::enableEnforcement(true);
    }

    /// @brief Name of the log file
    static const char* TEST_LOG_NAME;

    /// @brief Maximum log size
    static const int TEST_MAX_SIZE;

    /// @brief Maximum rotated log versions
    static const int TEST_MAX_VERS;

};

const char* LoggingTest::TEST_LOG_NAME = "kea.test.log";
const int LoggingTest::TEST_MAX_SIZE = 204800;  // Smallest without disabling rotation
const int LoggingTest::TEST_MAX_VERS = 2;       // More than the default of 1

// Checks that the constructor is able to process specified storage properly.
TEST_F(LoggingTest, constructor) {

    ConfigPtr null_ptr;
    EXPECT_THROW(LogConfigParser parser(null_ptr), BadValue);

    ConfigPtr nonnull(new ConfigBase());

    EXPECT_NO_THROW(LogConfigParser parser(nonnull));
}

// Checks if the LogConfigParser class is able to transform JSON structures
// into Configuration usable by log4cplus. This test checks for output
// configured to stdout on debug level.
TEST_F(LoggingTest, parsingConsoleOutput) {

    const char* config_txt =
    "{ \"loggers\": ["
    "    {"
    "        \"name\": \"kea\","
    "        \"output-options\": ["
    "            {"
    "                \"output\": \"stdout\","
    "                \"flush\": true"
    "            }"
    "        ],"
    "        \"debuglevel\": 99,"
    "        \"severity\": \"DEBUG\""
    "    }"
    "]}";

    ConfigPtr storage(new ConfigBase());

    LogConfigParser parser(storage);

    // We need to parse properly formed JSON and then extract
    // "loggers" element from it. For some reason fromJSON is
    // throwing at opening square bracket
    ConstElementPtr config = Element::fromJSON(config_txt);
    config = config->get("loggers");

    EXPECT_NO_THROW(parser.parseConfiguration(config));

    ASSERT_EQ(1, storage->getLoggingInfo().size());

    EXPECT_EQ("kea", storage->getLoggingInfo()[0].name_);
    EXPECT_EQ(99, storage->getLoggingInfo()[0].debuglevel_);
    EXPECT_EQ(isc::log::DEBUG, storage->getLoggingInfo()[0].severity_);

    ASSERT_EQ(1, storage->getLoggingInfo()[0].destinations_.size());
    EXPECT_EQ("stdout" , storage->getLoggingInfo()[0].destinations_[0].output_);
    EXPECT_TRUE(storage->getLoggingInfo()[0].destinations_[0].flush_);
}

// This test checks that deprecated parameter output_options is accepted.
TEST_F(LoggingTest, parsingDeprecatedOutputOptions) {

    const char* config_txt =
    "{ \"loggers\": ["
    "    {"
    "        \"name\": \"kea\","
    "        \"output_options\": ["
    "            {"
    "                \"output\": \"stdout\","
    "                \"flush\": true"
    "            }"
    "        ],"
    "        \"debuglevel\": 99,"
    "        \"severity\": \"DEBUG\""
    "    }"
    "]}";

    ConfigPtr storage(new ConfigBase());

    LogConfigParser parser(storage);

    // We need to parse properly formed JSON and then extract
    // "loggers" element from it. For some reason fromJSON is
    // throwing at opening square bracket
    ConstElementPtr config = Element::fromJSON(config_txt);
    config = config->get("loggers");

    ASSERT_EQ(1, config->size());
    ASSERT_TRUE(config->get(0)->get("output_options"));
    ASSERT_FALSE(config->get(0)->get("output-options"));

    EXPECT_NO_THROW(parser.parseConfiguration(config));

    ASSERT_EQ(1, storage->getLoggingInfo().size());

    EXPECT_EQ("kea", storage->getLoggingInfo()[0].name_);
    EXPECT_EQ(99, storage->getLoggingInfo()[0].debuglevel_);
    EXPECT_EQ(isc::log::DEBUG, storage->getLoggingInfo()[0].severity_);

    ASSERT_EQ(1, storage->getLoggingInfo()[0].destinations_.size());
    EXPECT_EQ("stdout" , storage->getLoggingInfo()[0].destinations_[0].output_);
    EXPECT_TRUE(storage->getLoggingInfo()[0].destinations_[0].flush_);

    ASSERT_TRUE(config->get(0)->get("output-options"));
    ASSERT_FALSE(config->get(0)->get("output_options"));
}

// This test checks that specifying both output-options and deprecated parameter
// output_options is not accepted.
TEST_F(LoggingTest, parsingErrorOutputOptions) {

    const char* config_txt =
    "{ \"loggers\": ["
    "    {"
    "        \"name\": \"kea\","
    "        \"output_options\": ["
    "            {"
    "                \"output\": \"stdout\","
    "                \"flush\": true"
    "            }"
    "        ],"
    "        \"output-options\": ["
    "            {"
    "                \"output\": \"stdout\","
    "                \"flush\": true"
    "            }"
    "        ],"
    "        \"debuglevel\": 99,"
    "        \"severity\": \"DEBUG\""
    "    }"
    "]}";

    ConfigPtr storage(new ConfigBase());

    LogConfigParser parser(storage);

    // We need to parse properly formed JSON and then extract
    // "loggers" element from it. For some reason fromJSON is
    // throwing at opening square bracket
    ConstElementPtr config = Element::fromJSON(config_txt);
    config = config->get("loggers");

    ASSERT_EQ(1, config->size());
    ASSERT_TRUE(config->get(0)->get("output_options"));
    ASSERT_TRUE(config->get(0)->get("output-options"));

    ASSERT_THROW_MSG(parser.parseConfiguration(config), BadValue, "Only one of "
                     "'output-options' and 'output_options' may be specified.");

    ASSERT_EQ(0, storage->getLoggingInfo().size());
}

// Check that LogConfigParser can parse configuration that
// lacks a severity entry.
TEST_F(LoggingTest, parsingNoSeverity) {

    const char* config_txt =
    "{ \"loggers\": ["
    "    {"
    "        \"name\": \"kea\","
    "        \"output-options\": ["
    "            {"
    "                \"output\": \"stdout\","
    "                \"flush\": true"
    "            }"
    "        ],"
    "        \"debuglevel\": 99"
    "    }"
    "]}";

    ConfigPtr storage(new ConfigBase());

    LogConfigParser parser(storage);

    // We need to parse properly formed JSON and then extract
    // "loggers" element from it. For some reason fromJSON is
    // throwing at opening square bracket
    ConstElementPtr config = Element::fromJSON(config_txt);
    config = config->get("loggers");

    // No exception should be thrown.
    EXPECT_NO_THROW_LOG(parser.parseConfiguration(config));

    // Entries should be the ones set.
    ASSERT_EQ(1, storage->getLoggingInfo().size());
    LoggingInfo const& logging_info(storage->getLoggingInfo()[0]);
    EXPECT_EQ("kea", logging_info.name_);
    EXPECT_EQ(99, logging_info.debuglevel_);
    ASSERT_EQ(1, logging_info.destinations_.size());
    EXPECT_EQ("stdout" , logging_info.destinations_[0].output_);
    EXPECT_TRUE(logging_info.destinations_[0].flush_);

    // Severity should default to DEFAULT.
    EXPECT_EQ(isc::log::DEFAULT, logging_info.severity_);

    // Pattern should default to empty string.
    EXPECT_TRUE(logging_info.destinations_[0].pattern_.empty());
}

// Checks if the LogConfigParser class is able to transform JSON structures
// into Configuration usable by log4cplus. This test checks for output
// configured to a file on INFO level.
TEST_F(LoggingTest, parsingFile) {

    const char* config_txt =
    "{ \"loggers\": ["
    "    {"
    "        \"name\": \"kea\","
    "        \"output-options\": ["
    "            {"
    "                \"output\": \"logfile.txt\""
    "            }"
    "        ],"
    "        \"severity\": \"INFO\""
    "    }"
    "]}";

    ConfigPtr storage(new ConfigBase());

    LogConfigParser parser(storage);

    // We need to parse properly formed JSON and then extract
    // "loggers" element from it. For some reason fromJSON is
    // throwing at opening square bracket
    ConstElementPtr config = Element::fromJSON(config_txt);
    config = config->get("loggers");

    EXPECT_NO_THROW(parser.parseConfiguration(config));

    ASSERT_EQ(1, storage->getLoggingInfo().size());

    EXPECT_EQ("kea", storage->getLoggingInfo()[0].name_);
    EXPECT_EQ(0, storage->getLoggingInfo()[0].debuglevel_);
    EXPECT_EQ(isc::log::INFO, storage->getLoggingInfo()[0].severity_);

    ASSERT_EQ(1, storage->getLoggingInfo()[0].destinations_.size());
    EXPECT_EQ(LogConfigParser::validatePath("logfile.txt"),
              storage->getLoggingInfo()[0].destinations_[0].output_);
    // Default for immediate flush is true
    EXPECT_TRUE(storage->getLoggingInfo()[0].destinations_[0].flush_);

    // Pattern should default to empty string.
    EXPECT_TRUE(storage->getLoggingInfo()[0].destinations_[0].pattern_.empty());
}

// Checks if the LogConfigParser class is able to transform data structures
// into Configuration usable by log4cplus. This test checks that more than
// one logger can be configured.
TEST_F(LoggingTest, multipleLoggers) {

    const char* config_txt =
    "{ \"loggers\": ["
    "    {"
    "        \"name\": \"kea\","
    "        \"output-options\": ["
    "            {"
    "                \"output\": \"logfile.txt\","
    "                \"flush\": true"
    "            }"
    "        ],"
    "        \"severity\": \"INFO\""
    "    },"
    "    {"
    "        \"name\": \"wombat\","
    "        \"output-options\": ["
    "            {"
    "                \"output\": \"logfile2.txt\","
    "                \"flush\": false"
    "            }"
    "        ],"
    "        \"severity\": \"DEBUG\","
    "        \"debuglevel\": 99"
    "    }"
    "]}";

    ConfigPtr storage(new ConfigBase());

    LogConfigParser parser(storage);

    // We need to parse properly formed JSON and then extract
    // "loggers" element from it. For some reason fromJSON is
    // throwing at opening square bracket
    ConstElementPtr config = Element::fromJSON(config_txt);
    config = config->get("loggers");

    EXPECT_NO_THROW(parser.parseConfiguration(config));

    ASSERT_EQ(2, storage->getLoggingInfo().size());

    EXPECT_EQ("kea", storage->getLoggingInfo()[0].name_);
    EXPECT_EQ(0, storage->getLoggingInfo()[0].debuglevel_);
    EXPECT_EQ(isc::log::INFO, storage->getLoggingInfo()[0].severity_);
    ASSERT_EQ(1, storage->getLoggingInfo()[0].destinations_.size());
    EXPECT_EQ(LogConfigParser::validatePath("logfile.txt"),
              storage->getLoggingInfo()[0].destinations_[0].output_);
    EXPECT_TRUE(storage->getLoggingInfo()[0].destinations_[0].flush_);
    EXPECT_EQ("wombat", storage->getLoggingInfo()[1].name_);
    EXPECT_EQ(99, storage->getLoggingInfo()[1].debuglevel_);
    EXPECT_EQ(isc::log::DEBUG, storage->getLoggingInfo()[1].severity_);
    ASSERT_EQ(1, storage->getLoggingInfo()[1].destinations_.size());
    EXPECT_EQ(LogConfigParser::validatePath("logfile2.txt"),
              storage->getLoggingInfo()[1].destinations_[0].output_);
    EXPECT_FALSE(storage->getLoggingInfo()[1].destinations_[0].flush_);
}

// Checks if the LogConfigParser class is able to transform data structures
// into Configuration usable by log4cplus. This test checks that more than
// one logging destination can be configured.
TEST_F(LoggingTest, multipleLoggingDestinations) {
    setLogTestPath();
    const char* config_txt =
    "{ \"loggers\": ["
    "    {"
    "        \"name\": \"kea\","
    "        \"output-options\": ["
    "            {"
    "                \"output\": \"logfile.txt\""
    "            },"
    "            {"
    "                \"output\": \"stdout\""
    "            }"
    "        ],"
    "        \"severity\": \"INFO\""
    "    }"
    "]}";

    ConfigPtr storage(new ConfigBase());

    LogConfigParser parser(storage);

    // We need to parse properly formed JSON and then extract
    // "loggers" element from it. For some reason fromJSON is
    // throwing at opening square bracket
    ConstElementPtr config = Element::fromJSON(config_txt);
    config = config->get("loggers");

    EXPECT_NO_THROW(parser.parseConfiguration(config));

    ASSERT_EQ(1, storage->getLoggingInfo().size());

    EXPECT_EQ("kea", storage->getLoggingInfo()[0].name_);
    EXPECT_EQ(0, storage->getLoggingInfo()[0].debuglevel_);
    EXPECT_EQ(isc::log::INFO, storage->getLoggingInfo()[0].severity_);
    ASSERT_EQ(2, storage->getLoggingInfo()[0].destinations_.size());
    EXPECT_EQ(LogConfigParser::validatePath("logfile.txt"),
              storage->getLoggingInfo()[0].destinations_[0].output_);
    EXPECT_TRUE(storage->getLoggingInfo()[0].destinations_[0].flush_);
    EXPECT_EQ("stdout" , storage->getLoggingInfo()[0].destinations_[1].output_);
    EXPECT_TRUE(storage->getLoggingInfo()[0].destinations_[1].flush_);
}

// Verifies that log rotation occurs when configured.  We do not
// worry about contents of the log files, only that rotation occurs.
// Such details are tested in lib/log.  This test verifies that
// we can correctly configure logging such that rotation occurs as
// expected.
TEST_F(LoggingTest, logRotate) {
    setLogTestPath();
    wipeFiles();

    std::ostringstream os;
    os <<
        "{ \"loggers\": ["
        "    {"
        "        \"name\": \"kea\","
        "        \"output-options\": ["
        "            {"
        "                \"output\": \""
        << TEST_LOG_NAME << "\","  <<
        "                \"flush\": true,"
        "                \"maxsize\":"
        << TEST_MAX_SIZE << "," <<
        "                \"maxver\":"
        << TEST_MAX_VERS <<
        "            }"
        "        ],"
        "        \"debuglevel\": 99,"
        "        \"severity\": \"DEBUG\""
        "    }"
        "]}";

    // Create our server config container.
    ConfigPtr server_cfg(new ConfigBase());

    // LogConfigParser expects a list of loggers, so parse
    // the JSON text and extract the "loggers" element from it
    ConstElementPtr config = Element::fromJSON(os.str());
    config = config->get("loggers");

    // Parse the config and then apply it.
    LogConfigParser parser(server_cfg);
    ASSERT_NO_THROW(parser.parseConfiguration(config));
    ASSERT_NO_THROW(server_cfg->applyLoggingCfg());

    EXPECT_EQ(TEST_MAX_SIZE, server_cfg->getLoggingInfo()[0].destinations_[0].maxsize_);
    EXPECT_EQ(TEST_MAX_VERS, server_cfg->getLoggingInfo()[0].destinations_[0].maxver_);

    // Make sure we have the initial log file.
    ASSERT_TRUE(isc::test::fileExists(LogConfigParser::validatePath(TEST_LOG_NAME)));

    // Now generate a log we know will be large enough to force a rotation.
    // We borrow a one argument log message for the test.
    std::string big_arg(TEST_MAX_SIZE, 'x');
    isc::log::Logger logger("kea");

    for (int i = 1; i < TEST_MAX_VERS + 1; i++) {
        // Output the big log and make sure we get the expected rotation file.
        LOG_INFO(logger, DCTL_CONFIG_COMPLETE).arg(big_arg);
        EXPECT_TRUE(isc::test::fileExists(LogConfigParser::validatePath(logName(i).c_str())));
    }

    // Clean up.
    wipeFiles();
}

// Verifies that a valid output option,'pattern' parses correctly.
TEST_F(LoggingTest, validPattern) {

    // Note the backslash must be doubled in the pattern definition.
    const char* config_txt =
    "{ \"loggers\": ["
    "    {"
    "        \"name\": \"kea\","
    "        \"output-options\": ["
    "            {"
    "                \"output\": \"stdout\","
    "                \"pattern\": \"mylog %m\\n\""
    "            }"
    "        ],"
    "        \"severity\": \"INFO\""
    "    }"
    "]}";

    ConfigPtr storage(new ConfigBase());

    LogConfigParser parser(storage);

    // We need to parse properly formed JSON and then extract
    // "loggers" element from it. For some reason fromJSON is
    // throwing at opening square bracket
    ConstElementPtr config = Element::fromJSON(config_txt);
    config = config->get("loggers");

    EXPECT_NO_THROW(parser.parseConfiguration(config));

    ASSERT_EQ(1, storage->getLoggingInfo().size());

    EXPECT_EQ("kea", storage->getLoggingInfo()[0].name_);
    EXPECT_EQ(isc::log::INFO, storage->getLoggingInfo()[0].severity_);

    ASSERT_EQ(1, storage->getLoggingInfo()[0].destinations_.size());
    EXPECT_EQ("stdout" , storage->getLoggingInfo()[0].destinations_[0].output_);
    EXPECT_EQ(storage->getLoggingInfo()[0].destinations_[0].pattern_,
              std::string("mylog %m\n"));
}

// Verifies that output option,'pattern', may be an empty string
TEST_F(LoggingTest, emptyPattern) {
    const char* config_txt =
    "{ \"loggers\": ["
    "    {"
    "        \"name\": \"kea\","
    "        \"output-options\": ["
    "            {"
    "                \"output\": \"stdout\","
    "                \"pattern\": \"\""
    "            }"
    "        ],"
    "        \"severity\": \"INFO\""
    "    }"
    "]}";

    ConfigPtr storage(new ConfigBase());

    LogConfigParser parser(storage);

    // We need to parse properly formed JSON and then extract
    // "loggers" element from it. For some reason fromJSON is
    // throwing at opening square bracket
    ConstElementPtr config = Element::fromJSON(config_txt);
    config = config->get("loggers");

    EXPECT_NO_THROW(parser.parseConfiguration(config));

    ASSERT_EQ(1, storage->getLoggingInfo().size());

    EXPECT_EQ("kea", storage->getLoggingInfo()[0].name_);
    EXPECT_EQ(isc::log::INFO, storage->getLoggingInfo()[0].severity_);

    ASSERT_EQ(1, storage->getLoggingInfo()[0].destinations_.size());
    EXPECT_EQ("stdout" , storage->getLoggingInfo()[0].destinations_[0].output_);
    EXPECT_TRUE(storage->getLoggingInfo()[0].destinations_[0].pattern_.empty());
}

void testMaxSize(uint64_t maxsize_candidate, uint64_t expected_maxsize) {
    std::string const logger(R"(
    {
      "loggers": [
        {

          "debuglevel": 99,
          "name": "kea",
          "output-options": [
            {
              "output": "kea.test.log",
              "flush": true,
              "maxsize": )" + std::to_string(maxsize_candidate) + R"(,
              "maxver": 2
            }
          ],
          "severity": "DEBUG"
        }
      ]
    }
    )");

    // Create our server config container.
    ConfigPtr server_cfg(boost::make_shared<ConfigBase>());

    // LogConfigParser expects a list of loggers, so parse
    // the JSON text and extract the "loggers" element from it
    ConstElementPtr config(Element::fromJSON(logger));
    config = config->get("loggers");

    // Parse the config and then apply it.
    LogConfigParser parser(server_cfg);
    ASSERT_NO_THROW(parser.parseConfiguration(config));
    ASSERT_NO_THROW(server_cfg->applyLoggingCfg());

    EXPECT_EQ(server_cfg->getLoggingInfo()[0].destinations_[0].maxsize_,
              expected_maxsize);
}

// Test that maxsize can be configured with high values.
TEST_F(LoggingTest, maxsize) {
    setLogTestPath();
    testMaxSize(TEST_MAX_SIZE, TEST_MAX_SIZE);
    testMaxSize(std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max());
    testMaxSize(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max());
    testMaxSize(1000LL * std::numeric_limits<int32_t>::max(), 1000LL * std::numeric_limits<int32_t>::max());
    testMaxSize(1000000LL * std::numeric_limits<int32_t>::max(), 1000000LL * std::numeric_limits<int32_t>::max());
}

// Verifies syslog destination
TEST_F(LoggingTest, syslogDestination) {
    // Note the backslash must be doubled in the pattern definition.
    const char* config_txt =
    "{ \"loggers\": ["
    "    {"
    "        \"name\": \"kea\","
    "        \"output-options\": ["
    "            {"
    "                \"output\": \"syslog\""
    "            }"
    "        ],"
    "        \"severity\": \"INFO\""
    "    }"
    "]}";

    ConfigPtr storage(new ConfigBase());

    LogConfigParser parser(storage);

    // We need to parse properly formed JSON and then extract
    // "loggers" element from it. For some reason fromJSON is
    // throwing at opening square bracket
    ConstElementPtr config = Element::fromJSON(config_txt);
    config = config->get("loggers");

    EXPECT_NO_THROW(parser.parseConfiguration(config));

    ASSERT_EQ(1, storage->getLoggingInfo().size());

    EXPECT_EQ("kea", storage->getLoggingInfo()[0].name_);
    EXPECT_EQ(isc::log::INFO, storage->getLoggingInfo()[0].severity_);

    ASSERT_EQ(1, storage->getLoggingInfo()[0].destinations_.size());
    // The destination output should NOT include the validated path.
    EXPECT_EQ("syslog" , storage->getLoggingInfo()[0].destinations_[0].output_);
}

// Verifies syslog destination
TEST_F(LoggingTest, syslogPlusFacility) {
    // Note the backslash must be doubled in the pattern definition.
    const char* config_txt =
    "{ \"loggers\": ["
    "    {"
    "        \"name\": \"kea\","
    "        \"output-options\": ["
    "            {"
    "                \"output\": \"syslog:daemon\""
    "            }"
    "        ],"
    "        \"severity\": \"INFO\""
    "    }"
    "]}";

    ConfigPtr storage(new ConfigBase());

    LogConfigParser parser(storage);

    // We need to parse properly formed JSON and then extract
    // "loggers" element from it. For some reason fromJSON is
    // throwing at opening square bracket
    ConstElementPtr config = Element::fromJSON(config_txt);
    config = config->get("loggers");

    EXPECT_NO_THROW(parser.parseConfiguration(config));

    ASSERT_EQ(1, storage->getLoggingInfo().size());

    EXPECT_EQ("kea", storage->getLoggingInfo()[0].name_);
    EXPECT_EQ(isc::log::INFO, storage->getLoggingInfo()[0].severity_);

    ASSERT_EQ(1, storage->getLoggingInfo()[0].destinations_.size());
    // The destination output should NOT include the validated path.
    EXPECT_EQ("syslog:daemon" , storage->getLoggingInfo()[0].destinations_[0].output_);
}

// Verifies that an invalid output path results in an
// error security enforcement is enabled.
TEST_F(LoggingTest, enforceSecurityTrue) {
    const char* config_txt =
    "{ \"loggers\": ["
    "    {"
    "        \"name\": \"kea\","
    "        \"output-options\": ["
    "            {"
    "                \"output\": \"/tmp/myfile.log\""
    "            }"
    "        ],"
    "        \"severity\": \"INFO\""
    "    }"
    "]}";

    ConfigPtr storage(new ConfigBase());

    LogConfigParser parser(storage);

    // We need to parse properly formed JSON and then extract
    // "loggers" element from it. For some reason fromJSON is
    // throwing at opening square bracket
    ConstElementPtr config = Element::fromJSON(config_txt);
    config = config->get("loggers");

    std::ostringstream oss;
    oss << "invalid path in `output`: invalid path specified: '/tmp', supported path is '"
        << LogConfigParser::getLogPath()
        << "' (<string>:1:82)";

    ASSERT_THROW_MSG(parser.parseConfiguration(config), BadValue,
                     oss.str());
}

// Verifies that an invalid output path results in a
// warning when security enforcement is disabled.
TEST_F(LoggingTest, enforceSecurityFalse) {
    file::PathChecker::enableEnforcement(false);
    const char* config_txt =
    "{ \"loggers\": ["
    "    {"
    "        \"name\": \"kea\","
    "        \"output-options\": ["
    "            {"
    "                \"output\": \"/tmp/myfile.log\""
    "            }"
    "        ],"
    "        \"severity\": \"INFO\""
    "    }"
    "]}";

    ConfigPtr storage(new ConfigBase());

    LogConfigParser parser(storage);

    // We need to parse properly formed JSON and then extract
    // "loggers" element from it. For some reason fromJSON is
    // throwing at opening square bracket
    ConstElementPtr config = Element::fromJSON(config_txt);
    config = config->get("loggers");

    ASSERT_NO_THROW_LOG(parser.parseConfiguration(config));

    ASSERT_EQ(1, storage->getLoggingInfo().size());

    EXPECT_EQ("kea", storage->getLoggingInfo()[0].name_);
    EXPECT_EQ(isc::log::INFO, storage->getLoggingInfo()[0].severity_);

    ASSERT_EQ(1, storage->getLoggingInfo()[0].destinations_.size());
    EXPECT_EQ("/tmp/myfile.log" , storage->getLoggingInfo()[0].destinations_[0].output_);

    std::ostringstream oss;
    oss << "DCTL_LOG_PATH_SECURITY_WARNING Log output path specified is NOT SECURE:"
        << " invalid path specified: '/tmp', supported path is '"
        << LogConfigParser::getLogPath() <<  "'";

    EXPECT_EQ(1, countFile(oss.str()));
}

/// @todo Add tests for malformed logging configuration

/// @todo There is no easy way to test applyConfiguration() and defaultLogging().
/// To test them, it would require instrumenting log4cplus to actually fake
/// the logging set up. Alternatively, we could develop set of test suites
/// that check each logging destination separately (e.g. configure log file, then
/// check if the file is indeed created or configure stdout destination, then
/// swap console file descriptors and check that messages are really logged.

}  // namespace
