// Copyright (C) 2018-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef ISC_YANG_REVISIONS_H
#define ISC_YANG_REVISIONS_H 1

#include <string>
#include <unordered_map>

namespace isc {
namespace yang {

// Table of module name / revision.
static const std::unordered_map<std::string, std::string> YANG_REVISIONS = {
#ifdef KEATEST_MODULE
    { "keatest-module", "2022-11-30" },
#endif  // KEATEST_MODULE
    { "ietf-dhcpv6-types", "2018-09-04" },
    { "ietf-dhcpv6-options", "2018-09-04" },
    { "ietf-dhcpv6-server", "2018-09-04" },
    { "kea-types", "2025-06-25" },
    { "kea-dhcp-types", "2025-06-25" },
    { "kea-dhcp4-server", "2025-06-25" },
    { "kea-dhcp6-server", "2025-06-25" },
    { "kea-ctrl-agent", "2025-06-25" },
    { "kea-dhcp-ddns", "2025-06-25" }
};  // YANG_REVISIONS

}  // namespace yang
}  // namespace isc

#endif  // ISC_YANG_REVISIONS_H
