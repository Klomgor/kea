// Copyright (C) 2026 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>
#include <dhcpsrv/sflq_allocation_state.h>
#include <util/multi_threading_mgr.h>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace isc::util;

namespace isc {
namespace dhcp {

SubnetSflqAllocationState::SubnetSflqAllocationState()
    : SubnetAllocationState() {
}

void
SubnetSflqAllocationState::setLastAllocatedTime(boost::posix_time::ptime last_time
                                                /* = not_a_date_time */) {
    MultiThreadingLock lock(*mutex_);
    if (last_time == boost::posix_time::not_a_date_time) {
        setCurrentAllocatedTimeInternal();
    } else {
        last_allocated_time_ = last_time;
    }
}

}
}
