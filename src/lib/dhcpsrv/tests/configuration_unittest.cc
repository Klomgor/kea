// Copyright (C) 2014 Internet Systems Consortium, Inc. ("ISC")
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
// OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#include <config.h>

#include <dhcpsrv/cfgmgr.h>
#include <dhcpsrv/configuration.h>
#include <dhcpsrv/subnet.h>
#include <gtest/gtest.h>

using namespace isc::asiolink;
using namespace isc::dhcp;

// Those are the tests for Configuration storage. Right now they are minimal,
// but the number is expected to grow significantly once we migrate more
// parameters from CfgMgr storage to Configuration storage.

namespace {

/// @brief Number of IPv4 and IPv6 subnets to be created for a test.
const int TEST_SUBNETS_NUM = 3;

/// @brief Test fixture class for testing configuration data storage.
class ConfigurationTest : public ::testing::Test {
public:
    /// @brief Constructor.
    ///
    /// Creates IPv4 and IPv6 subnets for unit test. The number of subnets
    /// is @c TEST_SUBNETS_NUM for IPv4 and IPv6 each.
    ConfigurationTest() {
        // Remove any subnets dangling from previous unit tests.
        clearSubnets();
        // Create IPv4 subnets.
        for (int i = 0; i < TEST_SUBNETS_NUM; ++i) {
            // Default triplet carried undefined value.
            Triplet<uint32_t> def_triplet;
            // Create a collection of subnets: 192.0.X.0/24 where X is
            // 0, 1, 2 etc.
            Subnet4Ptr subnet(new Subnet4(IOAddress(0xC0000000 | (i << 2)),
                                          24, def_triplet, def_triplet,
                                          4000));
            test_subnets4_.push_back(subnet);
        }
        // Create IPv6 subnets.
        for (int i = 0; i < TEST_SUBNETS_NUM; ++i) {
            // This is a base prefix. All other prefixes will be created by
            // modifying this one.
            IOAddress prefix("2001:db8:1::0");
            std::vector<uint8_t> prefix_bytes = prefix.toBytes();
            // Modify 5th byte of the prefix, so 2001:db8:1::0 becomes
            // 2001:db8:2::0 etc.
            ++prefix_bytes[5];
            prefix = IOAddress::fromBytes(prefix.getFamily(), &prefix_bytes[0]);
            Subnet6Ptr subnet(new Subnet6(prefix, 64, 1000, 2000, 3000, 4000));
            test_subnets6_.push_back(subnet);
        }
    }

    /// @brief Destructor.
    ///
    /// Removes any dangling configuration.
    virtual ~ConfigurationTest() {
        clearSubnets();
    }

    /// @brief Convenience function which adds IPv4 subnet to the configuration.
    ///
    /// @param index Index of the subnet in the @c test_subnets4_ collection
    /// which should be added to the configuration. The configuration is stored
    /// in the @ conf_ member. This value must be lower than
    /// @c TEST_SUBNETS_NUM.
    ///
    /// @todo Until the subnets configuration is migrated from the @c CfgMgr to
    /// the @c Configuration object, this function adds the subnet to the
    /// @c CfgMgr. Once, the subnet configuration is held in the
    /// @c Configuration this function must be modified to store the subnets in
    /// the @c conf_ object.
    void addSubnet4(const unsigned int index);

    /// @brief Convenience function which adds IPv6 subnet to the configuration.
    ///
    /// @param index Index of the subnet in the @c test_subnets6_ collection
    /// which should be added to the configuration. The configuration is stored
    /// in the @ conf_ member. This value must be lower than
    /// @c TEST_SUBNETS_NUM.
    ///
    /// @todo Until the subnets configuration is migrated from the @c CfgMgr to
    /// the @c Configuration object, this function adds the subnet to the
    /// @c CfgMgr. Once, the subnet configuration is held in the
    /// @c Configuration this function must be modified to store the subnets in
    /// @c conf_ object.
    void addSubnet6(const unsigned int index);

    /// @brief Removes all subnets from the configuration.
    ///
    /// @todo Modify this function once the subnet configuration is migrated
    /// from @c CfgMgr to @c Configuration.
    void clearSubnets();

    /// @brief Stores configuration.
    Configuration conf_;
    /// @brief A collection of IPv4 subnets used by unit tests.
    Subnet4Collection test_subnets4_;
    /// @brief A collection of IPv6 subnets used by unit tests.
    Subnet6Collection test_subnets6_;

};

void
ConfigurationTest::addSubnet4(const unsigned int index) {
    if (index >= TEST_SUBNETS_NUM) {
        FAIL() << "Subnet index " << index << "out of range (0.."
               << TEST_SUBNETS_NUM << "): " << "unable to add IPv4 subnet";
    }
    CfgMgr::instance().addSubnet4(test_subnets4_[index]);
}

void
ConfigurationTest::addSubnet6(const unsigned int index) {
    if (index >= TEST_SUBNETS_NUM) {
        FAIL() << "Subnet index " << index << "out of range (0.."
               << TEST_SUBNETS_NUM << "): " << "unable to add IPv6 subnet";
    }
    CfgMgr::instance().addSubnet6(test_subnets6_[index]);
}

void
ConfigurationTest::clearSubnets() {
    CfgMgr::instance().deleteSubnets4();
    CfgMgr::instance().deleteSubnets6();
}

// Check that by default there are no logging entries
TEST_F(ConfigurationTest, basic) {
    EXPECT_TRUE(conf_.logging_info_.empty());
}

// Check that Configuration can store logging information.
TEST_F(ConfigurationTest, loggingInfo) {
    LoggingInfo log1;
    log1.name_ = "foo";
    log1.severity_ = isc::log::WARN;
    log1.debuglevel_ = 77;

    LoggingDestination dest;
    dest.output_ = "some-logfile.txt";
    dest.maxver_ = 5;
    dest.maxsize_ = 2097152;

    log1.destinations_.push_back(dest);

    conf_.logging_info_.push_back(log1);

    EXPECT_EQ("foo", conf_.logging_info_[0].name_);
    EXPECT_EQ(isc::log::WARN, conf_.logging_info_[0].severity_);
    EXPECT_EQ(77, conf_.logging_info_[0].debuglevel_);

    EXPECT_EQ("some-logfile.txt", conf_.logging_info_[0].destinations_[0].output_);
    EXPECT_EQ(5, conf_.logging_info_[0].destinations_[0].maxver_);
    EXPECT_EQ(2097152, conf_.logging_info_[0].destinations_[0].maxsize_);
}

// Check that the configuration summary including information about added
// subnets is returned.
TEST_F(ConfigurationTest, summarySubnets) {
    EXPECT_EQ("no config details available",
              conf_.getConfigSummary(Configuration::CFGSEL_NONE));

    // Initially, there are no subnets added but it should be explicitly
    // reported when we query for information about the subnets.
    EXPECT_EQ("no IPv4 subnets!; no IPv6 subnets!",
              conf_.getConfigSummary(Configuration::CFGSEL_ALL));

    // If we just want information about IPv4 subnets, there should be no
    // mention of IPv6 subnets, even though there are none added.
    EXPECT_EQ("no IPv4 subnets!",
              conf_.getConfigSummary(Configuration::CFGSEL_ALL4));

    // If we just want information about IPv6 subnets, there should be no
    // mention of IPv4 subnets, even though there are none added.
    EXPECT_EQ("no IPv6 subnets!",
              conf_.getConfigSummary(Configuration::CFGSEL_ALL6));

    // Add IPv4 subnet and make sure it is reported.
    addSubnet4(0);
    EXPECT_EQ("added IPv4 subnets: 1",
              conf_.getConfigSummary(Configuration::CFGSEL_ALL4));
    EXPECT_EQ("added IPv4 subnets: 1; no IPv6 subnets!",
              conf_.getConfigSummary(Configuration::CFGSEL_ALL));

    // Add IPv6 subnet and make sure it is reported.
    addSubnet6(0);
    EXPECT_EQ("added IPv6 subnets: 1",
              conf_.getConfigSummary(Configuration::CFGSEL_ALL6));
    EXPECT_EQ("added IPv4 subnets: 1; added IPv6 subnets: 1",
              conf_.getConfigSummary(Configuration::CFGSEL_ALL));

    // Add one more subnet and make sure the bumped value is only
    // for IPv4, but not for IPv6.
    addSubnet4(1);
    EXPECT_EQ("added IPv4 subnets: 2; added IPv6 subnets: 1",
              conf_.getConfigSummary(Configuration::CFGSEL_ALL));
    EXPECT_EQ("added IPv4 subnets: 2",
              conf_.getConfigSummary(Configuration::CFGSEL_ALL4));

    addSubnet6(1);
    EXPECT_EQ("added IPv4 subnets: 2; added IPv6 subnets: 2",
              conf_.getConfigSummary(Configuration::CFGSEL_ALL));

    // Remove all subnets and make sure that there are no reported subnets
    // back again.
    clearSubnets();
    EXPECT_EQ("no IPv4 subnets!; no IPv6 subnets!",
              conf_.getConfigSummary(Configuration::CFGSEL_ALL));
}

} // end of anonymous namespace
