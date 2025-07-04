// Copyright (C) 2018-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <gtest/gtest.h>

#include <testutils/gtest_utils.h>
#include <testutils/io_utils.h>
#include <testutils/user_context_utils.h>
#include <yang/adaptor_config.h>

#include <vector>

using namespace std;
using namespace isc;
using namespace isc::data;
using namespace isc::test;
using namespace isc::yang;

namespace {

/// @brief Fixture class that helps testing AdaptorConfig
class AdaptorConfigTest : public ::testing::Test {
public:

/// @brief Load an example JSON config
///
/// @param fname name of the file (expected to be a valid JSON config)
/// @param v6 - false=v4, true=v6
/// @param result - JSON converted by a AdaptorConfig::preprocess[4/6]
void testFile(const string& fname, bool v6, ElementPtr& result) {
    ElementPtr json;
    ElementPtr reference_json;

    string decommented = decommentJSONfile(fname);

    EXPECT_NO_THROW_LOG(json = Element::fromJSONFile(decommented, true));
    reference_json = moveComments(json);

    // remove the temporary file
    EXPECT_NO_THROW_LOG(::remove(decommented.c_str()));

    string before = json->str();
    if (v6) {
        ASSERT_NO_THROW_LOG(AdaptorConfig::preProcess6(json));
    } else {
        ASSERT_NO_THROW_LOG(AdaptorConfig::preProcess4(json));
    }
    string after = json->str();

    EXPECT_FALSE(before.empty());
    EXPECT_FALSE(after.empty());

    result = json;
}

};  // AdaptorConfigTest

TEST_F(AdaptorConfigTest, loadExamples4) {
    vector<string> configs = {
        "advanced.json",
        "all-keys-netconf.json",
        "all-options.json",
        "backends.json",
        "classify.json",
        "classify2.json",
        "comments.json",
        "config-backend.json",
        "dhcpv4-over-dhcpv6.json",
        "global-reservations.json",
        "ha-load-balancing-server1-mt-with-tls.json",
        "ha-load-balancing-server2-mt.json",
        "hooks.json",
        "hooks-radius.json",
        "leases-expiration.json",
        "multiple-options.json",
        "mysql-reservations.json",
        "pgsql-reservations.json",
        "reservations.json",
        "several-subnets.json",
        "shared-network.json",
        "single-subnet.json",
        "vendor-specific.json",
        "vivso.json",
        "with-ddns.json",
    };

    ElementPtr x;

    for (unsigned i = 0; i < configs.size(); i++) {
        x.reset();
        testFile(string(CFG_EXAMPLES) + "/kea4/" + configs[i], false, x);
        ASSERT_TRUE(x);
    }
}

TEST_F(AdaptorConfigTest, loadExamples6) {
    vector<string> configs = {
        "advanced.json",
        "all-keys-netconf.json",
        "all-options.json",
        "backends.json",
        "classify.json",
        "classify2.json",
        "comments.json",
        "config-backend.json",
        "dhcpv4-over-dhcpv6.json",
        "duid.json",
        "global-reservations.json",
        "ha-hot-standby-server1-with-tls.json",
        "ha-hot-standby-server2.json",
        "hooks.json",
        "iPXE.json",
        "leases-expiration.json",
        "multiple-options.json",
        "mysql-reservations.json",
        "pgsql-reservations.json",
        "reservations.json",
        "several-subnets.json",
        "shared-network.json",
        "simple.json",
        "softwire46.json",
        "stateless.json",
        "tee-times.json",
        "with-ddns.json",
    };
    ElementPtr x;

    for (unsigned i = 0; i<configs.size(); i++) {
        x.reset();
        testFile(string(CFG_EXAMPLES) + "/kea6/" + configs[i], true, x);
        ASSERT_TRUE(x);
    }
}

/// In general, the AdatorConfig class doesn't need very thorough
/// direct tests, as there will be tests that will process whole
/// configuration that will in turn use AdaptorConfig calls.
/// Nevertheless, here are some ideas for tests. We hope to have
/// them implemented one day.
///
/// @todo: Check subnet-id assigned in subnets using any config
/// @todo: Check shared-networks assign id using kea4/shared-network.json
/// @todo: Check option classes using kea4/classify2.json
/// @todo: Check option data using kea6/with-ddns.json
/// @todo: Check option defs using kea6/dhcpv4-over-dhcpv6.json

}  // anonymous namespace
