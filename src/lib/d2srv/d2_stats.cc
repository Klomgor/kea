// Copyright (C) 2021 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

/// Defines the logger used by the top-level component of kea-dhcp-ddns.

#include <config.h>

#include <d2srv/d2_stats.h>

using namespace std;

namespace isc {
namespace d2 {

set<string>
D2Stats::ncr = {
    "ncr-received",
    "ncr-invalid",
    "ncr-error"
};

set<string>
D2Stats::update = {
    "update-sent",
    "update-signed",
    "update-unsigned",
    "update-success",
    "update-timeout",
    "update-error"
};

set<string>
D2Stats::key = {
    "update-sent",
    "update-success",
    "update-timeout",
    "update-error"
};

} // namespace d2
} // namespace isc