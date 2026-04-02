// Copyright (C) 2026 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <asiolink/io_address.h>
#include <dhcpsrv/iterative_allocator.h>
#include <sflqtest_lease_mgr.h>

using namespace isc::asiolink;
using namespace isc::db;
using namespace isc::dhcp;
using namespace std;

namespace isc {
namespace dhcp {
namespace test {

SflqPool::SflqPool(asiolink::IOAddress start_address,
                   asiolink::IOAddress end_address,
                   SubnetID subnet_id,
                   Lease::Type lease_type /* = Lease::TYPE_V4 */,
                   uint8_t delegated_len /* = 1 */)
    : start_address_(start_address), end_address_(end_address),
      subnet_id_(subnet_id), lease_type_(lease_type),
      delegated_len_(delegated_len) {
    repopulateFreeLeases();
}

IOAddress
SflqPool::zeroAddress() {
    if (lease_type_ == Lease::TYPE_V4) {
        return (IOAddress::IPV4_ZERO_ADDRESS());
    }

    return (IOAddress::IPV6_ZERO_ADDRESS());
}

void
SflqPool::repopulateFreeLeases() {
    // Populate list of free leases with all addresses in
    // the pool.  For purposes of testing the SharedFlqAllocator
    // class we don't care about actual leases.
    IOAddress next_address = start_address_;
    free_addresses_.clear();
    while (next_address <= end_address_) {
        free_addresses_.push_back(next_address);
        next_address = IterativeAllocator::increaseAddress(next_address,
                                                           (lease_type_ == Lease::TYPE_PD),
                                                           delegated_len_);
    }
}

IOAddress
SflqPool::popFreeAddress() {
    if (free_addresses_.empty()) {
        return (zeroAddress());
    }

    IOAddress free_address = free_addresses_.front();
    free_addresses_.pop_front();
    return (free_address);
}

TrackingLeaseMgrPtr
SflqTestLeaseMgr::factory(const DatabaseConnection::ParameterMap& params) {
    return (TrackingLeaseMgrPtr(new SflqTestLeaseMgr(params)));
}

SflqTestLeaseMgr::SflqTestLeaseMgr(const DatabaseConnection::ParameterMap& params)
    : ConcreteLeaseMgr(params) {
}

SflqTestLeaseMgr::~SflqTestLeaseMgr() {
}

bool
SflqTestLeaseMgr::sflqCreateFlqPool4(IOAddress start_address, IOAddress end_address,
                                     SubnetID subnet_id, bool recreate) {
    auto sflq_pool = findPool(start_address, end_address);
    if (sflq_pool && recreate) {
        sflq_pool->repopulateFreeLeases();
    }

    // Create the pool and add it to the list of pools.
    sflq_pool.reset(new SflqPool(start_address, end_address, subnet_id));
    sflq_pools_.push_back(sflq_pool);
    return(true);
}

IOAddress
SflqTestLeaseMgr::sflqPickFreeLease4(IOAddress start_address, IOAddress end_address) {
    auto sflq_pool = findPool(start_address, end_address);
    if (!sflq_pool) {
        return (IOAddress::IPV4_ZERO_ADDRESS());
    }

    return (sflq_pool->popFreeAddress());
}

bool
SflqTestLeaseMgr::sflqCreateFlqPool6(IOAddress start_address, IOAddress end_address,
                                     Lease::Type lease_type, uint8_t delegated_len,
                                     SubnetID subnet_id, bool recreate) {
    auto sflq_pool = findPool(start_address, end_address);
    if (sflq_pool && recreate) {
        sflq_pool->repopulateFreeLeases();
    }

    // Create the pool and add it to the list of pools.
    sflq_pool.reset(new SflqPool(start_address, end_address, subnet_id,
                                 lease_type, delegated_len));
    sflq_pools_.push_back(sflq_pool);
    return(true);
}

IOAddress
SflqTestLeaseMgr::sflqPickFreeLease6(IOAddress start_address, IOAddress end_address) {
    auto sflq_pool = findPool(start_address, end_address);
    if (!sflq_pool) {
        return (IOAddress::IPV6_ZERO_ADDRESS());
    }

    return (sflq_pool->popFreeAddress());
}

SflqPoolPtr
SflqTestLeaseMgr::findPool(IOAddress start_address, IOAddress end_address) {
    for (auto pool : sflq_pools_) {
        if (pool->start_address_ == start_address &&
            pool->end_address_ == end_address) {
            return (pool);
        }
    }

    return (SflqPoolPtr());
}

void
SflqTestLeaseMgr::repopulateFlqPools() {
    for (auto pool : sflq_pools_) {
        pool->repopulateFreeLeases();
    }
}

std::string
SflqTestLeaseMgr::getType() const {
    return (std::string("sflqtest"));
}

} // end of namespace isc::dhcp::test
} // end of namespace isc::dhcp
} // end of namespace isc
