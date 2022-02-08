// Copyright (C) 2020-2021 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <d2/simple_add.h>
#include <d2srv/d2_cfg_mgr.h>
#include <d2srv/d2_log.h>

#include <util/buffer.h>
#include <dns/rdataclass.h>

#include <functional>

namespace isc {
namespace d2 {

// SimpleAddTransaction states
const int SimpleAddTransaction::REPLACING_FWD_ADDRS_ST;
const int SimpleAddTransaction::REPLACING_REV_PTRS_ST;

// SimpleAddTransaction events
const int SimpleAddTransaction::FQDN_IN_USE_EVT;
const int SimpleAddTransaction::FQDN_NOT_IN_USE_EVT;

SimpleAddTransaction::
SimpleAddTransaction(asiolink::IOServicePtr& io_service,
                   dhcp_ddns::NameChangeRequestPtr& ncr,
                   DdnsDomainPtr& forward_domain,
                   DdnsDomainPtr& reverse_domain,
                   D2CfgMgrPtr& cfg_mgr)
    : NameChangeTransaction(io_service, ncr, forward_domain, reverse_domain,
                            cfg_mgr) {
    if (ncr->getChangeType() != isc::dhcp_ddns::CHG_ADD) {
        isc_throw (SimpleAddTransactionError,
                   "SimpleAddTransaction, request type must be CHG_ADD");
    }
}

SimpleAddTransaction::~SimpleAddTransaction(){
}

void
SimpleAddTransaction::defineEvents() {
    // Call superclass impl first.
    NameChangeTransaction::defineEvents();

    // Define SimpleAddTransaction events.
    defineEvent(FQDN_IN_USE_EVT, "FQDN_IN_USE_EVT");
    defineEvent(FQDN_NOT_IN_USE_EVT, "FQDN_NOT_IN_USE_EVT");
}

void
SimpleAddTransaction::verifyEvents() {
    // Call superclass implementation first to verify its events. These are
    // events common to all transactions, and they must be defined.
    // SELECT_SERVER_EVT
    // SERVER_SELECTED_EVT
    // SERVER_IO_ERROR_EVT
    // NO_MORE_SERVERS_EVT
    // IO_COMPLETED_EVT
    // UPDATE_OK_EVT
    // UPDATE_FAILED_EVT
    NameChangeTransaction::verifyEvents();

    // Verify SimpleAddTransaction events by attempting to fetch them.
    getEvent(FQDN_IN_USE_EVT);
    getEvent(FQDN_NOT_IN_USE_EVT);
}

void
SimpleAddTransaction::defineStates() {
    // Call superclass impl first.
    NameChangeTransaction::defineStates();

    // Define SimpleAddTransaction states.
    defineState(READY_ST, "READY_ST",
             std::bind(&SimpleAddTransaction::readyHandler, this));

    defineState(SELECTING_FWD_SERVER_ST, "SELECTING_FWD_SERVER_ST",
             std::bind(&SimpleAddTransaction::selectingFwdServerHandler, this));

    defineState(SELECTING_REV_SERVER_ST, "SELECTING_REV_SERVER_ST",
             std::bind(&SimpleAddTransaction::selectingRevServerHandler, this));

    defineState(REPLACING_FWD_ADDRS_ST, "REPLACING_FWD_ADDRS_ST",
             std::bind(&SimpleAddTransaction::replacingFwdAddrsHandler, this));

    defineState(REPLACING_REV_PTRS_ST, "REPLACING_REV_PTRS_ST",
             std::bind(&SimpleAddTransaction::replacingRevPtrsHandler, this));

    defineState(PROCESS_TRANS_OK_ST, "PROCESS_TRANS_OK_ST",
             std::bind(&SimpleAddTransaction::processAddOkHandler, this));

    defineState(PROCESS_TRANS_FAILED_ST, "PROCESS_TRANS_FAILED_ST",
             std::bind(&SimpleAddTransaction::processAddFailedHandler, this));
}

void
SimpleAddTransaction::verifyStates() {
    // Call superclass implementation first to verify its states. These are
    // states common to all transactions, and they must be defined.
    // READY_ST
    // SELECTING_FWD_SERVER_ST
    // SELECTING_REV_SERVER_ST
    // PROCESS_TRANS_OK_ST
    // PROCESS_TRANS_FAILED_ST
    NameChangeTransaction::verifyStates();

    // Verify SimpleAddTransaction states by attempting to fetch them.
    getStateInternal(REPLACING_FWD_ADDRS_ST);
    getStateInternal(REPLACING_REV_PTRS_ST);
}

void
SimpleAddTransaction::readyHandler() {
    switch(getNextEvent()) {
    case START_EVT:
        if (getForwardDomain()) {
            // Request includes a forward change, do that first.
            transition(SELECTING_FWD_SERVER_ST, SELECT_SERVER_EVT);
        } else {
            // Reverse change only, transition accordingly.
            transition(SELECTING_REV_SERVER_ST, SELECT_SERVER_EVT);
        }

        break;
    default:
        // Event is invalid.
        isc_throw(SimpleAddTransactionError,
                  "Wrong event for context: " << getContextStr());
    }
}

void
SimpleAddTransaction::selectingFwdServerHandler() {
    switch(getNextEvent()) {
    case SELECT_SERVER_EVT:
        // First time through for this transaction, so initialize server
        // selection.
        initServerSelection(getForwardDomain());
        break;
    case SERVER_IO_ERROR_EVT:
        // We failed to communicate with current server. Attempt to select
        // another one below.
        break;
    default:
        // Event is invalid.
        isc_throw(SimpleAddTransactionError,
                  "Wrong event for context: " << getContextStr());
    }

    // Select the next server from the list of forward servers.
    if (selectNextServer()) {
        // We have a server to try.
        transition(REPLACING_FWD_ADDRS_ST, SERVER_SELECTED_EVT);
    } else {
        // Server list is exhausted, so fail the transaction.
        transition(PROCESS_TRANS_FAILED_ST, NO_MORE_SERVERS_EVT);
    }
}

void
SimpleAddTransaction::replacingFwdAddrsHandler() {
    if (doOnEntry()) {
        // Clear the update attempts count on initial transition.
        clearUpdateAttempts();
    }

    switch(getNextEvent()) {
    case SERVER_SELECTED_EVT:
        try {
            clearDnsUpdateRequest();
            buildReplaceFwdAddressRequest();
        } catch (const std::exception& ex) {
            // While unlikely, the build might fail if we have invalid
            // data.  Should that be the case, we need to fail the
            // transaction.
            LOG_ERROR(d2_to_dns_logger, DHCP_DDNS_FORWARD_ADD_BUILD_FAILURE)
                .arg(getRequestId())
                .arg(getNcr()->toText())
                .arg(ex.what());
            transition(PROCESS_TRANS_FAILED_ST, UPDATE_FAILED_EVT);
            break;
        }

        // Call sendUpdate() to initiate the async send. Note it also sets
        // next event to NOP_EVT.
        sendUpdate("Forward Add");
        break;

    case IO_COMPLETED_EVT: {
        switch (getDnsUpdateStatus()) {
        case DNSClient::SUCCESS: {
            // We successfully received a response packet from the server.
            const dns::Rcode& rcode = getDnsUpdateResponse()->getRcode();
            if (rcode == dns::Rcode::NOERROR()) {
                // We were able to add it. Mark it as done.
                setForwardChangeCompleted(true);

                // If request calls for reverse update then do that next,
                // otherwise we can process ok.
                if (getReverseDomain()) {
                    transition(SELECTING_REV_SERVER_ST, SELECT_SERVER_EVT);
                } else {
                    transition(PROCESS_TRANS_OK_ST, UPDATE_OK_EVT);
                }
            } else {
                // Any other value means cease. Really shouldn't happen.
                LOG_ERROR(d2_to_dns_logger, DHCP_DDNS_FORWARD_ADD_REJECTED)
                          .arg(getRequestId())
                          .arg(getCurrentServer()->toText())
                          .arg(getNcr()->getFqdn())
                          .arg(rcode.getCode());
                transition(PROCESS_TRANS_FAILED_ST, UPDATE_FAILED_EVT);
            }

            break;
        }

        case DNSClient::TIMEOUT:
        case DNSClient::OTHER:
            // We couldn't send to the current server, log it and set up
            // to select the next server for a retry.
            // @note For now we treat OTHER as an IO error like TIMEOUT. It
            // is not entirely clear if this is accurate.
            LOG_ERROR(d2_to_dns_logger, DHCP_DDNS_FORWARD_ADD_IO_ERROR)
                      .arg(getRequestId())
                      .arg(getNcr()->getFqdn())
                      .arg(getCurrentServer()->toText());

            retryTransition(SELECTING_FWD_SERVER_ST);
            break;

        case DNSClient::INVALID_RESPONSE:
            // A response was received but was corrupt. Retry it like an IO
            // error.
            LOG_ERROR(d2_to_dns_logger, DHCP_DDNS_FORWARD_ADD_RESP_CORRUPT)
                      .arg(getRequestId())
                      .arg(getCurrentServer()->toText())
                      .arg(getNcr()->getFqdn());

            retryTransition(SELECTING_FWD_SERVER_ST);
            break;

        default:
            // Any other value and we will fail this transaction, something
            // bigger is wrong.
            LOG_ERROR(d2_to_dns_logger, DHCP_DDNS_FORWARD_ADD_BAD_DNSCLIENT_STATUS)
                      .arg(getRequestId())
                      .arg(getDnsUpdateStatus())
                      .arg(getNcr()->getFqdn())
                      .arg(getCurrentServer()->toText());

            transition(PROCESS_TRANS_FAILED_ST, UPDATE_FAILED_EVT);
            break;
        } // end switch on dns_status

        break;
    } // end case IO_COMPLETE_EVT

    default:
        // Event is invalid.
        isc_throw(SimpleAddTransactionError,
                  "Wrong event for context: " << getContextStr());
    }
}

void
SimpleAddTransaction::selectingRevServerHandler() {
    switch(getNextEvent()) {
    case SELECT_SERVER_EVT:
        // First time through for this transaction, so initialize server
        // selection.
        initServerSelection(getReverseDomain());
        break;
    case SERVER_IO_ERROR_EVT:
        // We failed to communicate with current server. Attempt to select
        // another one below.
        break;
    default:
        // Event is invalid.
        isc_throw(SimpleAddTransactionError,
                  "Wrong event for context: " << getContextStr());
    }

    // Select the next server from the list of forward servers.
    if (selectNextServer()) {
        // We have a server to try.
        transition(REPLACING_REV_PTRS_ST, SERVER_SELECTED_EVT);
    } else {
        // Server list is exhausted, so fail the transaction.
        transition(PROCESS_TRANS_FAILED_ST, NO_MORE_SERVERS_EVT);
    }
}


void
SimpleAddTransaction::replacingRevPtrsHandler() {
    if (doOnEntry()) {
        // Clear the update attempts count on initial transition.
        clearUpdateAttempts();
    }

    switch(getNextEvent()) {
    case SERVER_SELECTED_EVT:
        try {
            clearDnsUpdateRequest();
            buildReplaceRevPtrsRequest();
        } catch (const std::exception& ex) {
            // While unlikely, the build might fail if we have invalid
            // data.  Should that be the case, we need to fail the
            // transaction.
            LOG_ERROR(d2_to_dns_logger, DHCP_DDNS_REVERSE_REPLACE_BUILD_FAILURE)
                .arg(getRequestId())
                .arg(getNcr()->toText())
                .arg(ex.what());
            transition(PROCESS_TRANS_FAILED_ST, UPDATE_FAILED_EVT);
            break;
        }

        // Call sendUpdate() to initiate the async send. Note it also sets
        // next event to NOP_EVT.
        sendUpdate("Reverse Replace");
        break;

    case IO_COMPLETED_EVT: {
        switch (getDnsUpdateStatus()) {
        case DNSClient::SUCCESS: {
            // We successfully received a response packet from the server.
            const dns::Rcode& rcode = getDnsUpdateResponse()->getRcode();
            if (rcode == dns::Rcode::NOERROR()) {
                // We were able to update the reverse mapping. Mark it as done.
                setReverseChangeCompleted(true);
                transition(PROCESS_TRANS_OK_ST, UPDATE_OK_EVT);
            } else {
                // Per RFC4703 any other value means cease.
                // If we get not authorized should try the next server in
                // the list? @todo  This needs some discussion perhaps.
                LOG_ERROR(d2_to_dns_logger, DHCP_DDNS_REVERSE_REPLACE_REJECTED)
                          .arg(getRequestId())
                          .arg(getCurrentServer()->toText())
                          .arg(getNcr()->getFqdn())
                          .arg(rcode.getCode());
                transition(PROCESS_TRANS_FAILED_ST, UPDATE_FAILED_EVT);
            }

            break;
        }

        case DNSClient::TIMEOUT:
        case DNSClient::OTHER:
            // We couldn't send to the current server, log it and set up
            // to select the next server for a retry.
            // @note For now we treat OTHER as an IO error like TIMEOUT. It
            // is not entirely clear if this is accurate.
            LOG_ERROR(d2_to_dns_logger, DHCP_DDNS_REVERSE_REPLACE_IO_ERROR)
                      .arg(getRequestId())
                      .arg(getNcr()->getFqdn())
                      .arg(getCurrentServer()->toText());

            // If we are out of retries on this server, we go back and start
            // all over on a new server.
            retryTransition(SELECTING_REV_SERVER_ST);
            break;

        case DNSClient::INVALID_RESPONSE:
            // A response was received but was corrupt. Retry it like an IO
            // error.
            LOG_ERROR(d2_to_dns_logger, DHCP_DDNS_REVERSE_REPLACE_RESP_CORRUPT)
                      .arg(getRequestId())
                      .arg(getCurrentServer()->toText())
                      .arg(getNcr()->getFqdn());

            // If we are out of retries on this server, we go back and start
            // all over on a new server.
            retryTransition(SELECTING_REV_SERVER_ST);
            break;

        default:
            // Any other value and we will fail this transaction, something
            // bigger is wrong.
            LOG_ERROR(d2_to_dns_logger,
                      DHCP_DDNS_REVERSE_REPLACE_BAD_DNSCLIENT_STATUS)
                      .arg(getRequestId())
                      .arg(getDnsUpdateStatus())
                      .arg(getNcr()->getFqdn())
                      .arg(getCurrentServer()->toText());

            transition(PROCESS_TRANS_FAILED_ST, UPDATE_FAILED_EVT);
            break;
        } // end switch on dns_status

        break;
    } // end case IO_COMPLETE_EVT

    default:
        // Event is invalid.
        isc_throw(SimpleAddTransactionError,
                  "Wrong event for context: " << getContextStr());
    }
}

void
SimpleAddTransaction::processAddOkHandler() {
    switch(getNextEvent()) {
    case UPDATE_OK_EVT:
        LOG_INFO(d2_to_dns_logger, DHCP_DDNS_ADD_SUCCEEDED)
                 .arg(getRequestId())
                 .arg(getNcr()->toText());
        setNcrStatus(dhcp_ddns::ST_COMPLETED);
        endModel();
        break;
    default:
        // Event is invalid.
        isc_throw(SimpleAddTransactionError,
                  "Wrong event for context: " << getContextStr());
    }
}

void
SimpleAddTransaction::processAddFailedHandler() {
    switch(getNextEvent()) {
    case UPDATE_FAILED_EVT:
    case NO_MORE_SERVERS_EVT:
        setNcrStatus(dhcp_ddns::ST_FAILED);
        LOG_ERROR(d2_to_dns_logger, DHCP_DDNS_ADD_FAILED)
                  .arg(getRequestId())
                  .arg(transactionOutcomeString());
        endModel();
        break;
    default:
        // Event is invalid.
        isc_throw(SimpleAddTransactionError,
                  "Wrong event for context: " << getContextStr());
    }
}

void
SimpleAddTransaction::buildReplaceFwdAddressRequest() {
    // Construct an empty request.
    D2UpdateMessagePtr request = prepNewRequest(getForwardDomain());

    // Construct dns::Name from NCR fqdn.
    dns::Name fqdn(dns::Name(getNcr()->getFqdn()));

    // There are no prerequisites.

    // Build the Update Section.  First we delete any pre-existing
    // FQDN/IP and DHCID RRs.  Then we add new ones.

    // Create the FQDN/IP 'delete' RR and add it to update section.
    dns::RRsetPtr update(new dns::RRset(fqdn, dns::RRClass::ANY(),
                         getAddressRRType(), dns::RRTTL(0)));

    request->addRRset(D2UpdateMessage::SECTION_UPDATE, update);

    // Create the DHCID 'delete' RR and add it to the update section.
    update.reset(new dns::RRset(fqdn, dns::RRClass::ANY(),
                                dns::RRType::DHCID(), dns::RRTTL(0)));
    request->addRRset(D2UpdateMessage::SECTION_UPDATE, update);

    // Now make the new RRs.
    // Create the TTL based on lease length.
    dns::RRTTL lease_ttl(getNcr()->getLeaseLength());

    // Create the FQDN/IP 'add' RR and add it to the to update section.
    // Based on RFC 2136, section 2.5.1
    update.reset(new dns::RRset(fqdn, dns::RRClass::IN(),
                                getAddressRRType(), lease_ttl));

    addLeaseAddressRdata(update);
    request->addRRset(D2UpdateMessage::SECTION_UPDATE, update);

    // Now create the FQDN/DHCID 'add' RR and add it to update section.
    // Based on RFC 2136, section 2.5.1
    update.reset(new dns::RRset(fqdn, dns::RRClass::IN(),
                                dns::RRType::DHCID(), lease_ttl));

    // We add the DHCID for auditing purposes and in the event
    // conflict resolution is later enabled.
    addDhcidRdata(update);
    request->addRRset(D2UpdateMessage::SECTION_UPDATE, update);

    // Set the transaction's update request to the new request.
    setDnsUpdateRequest(request);
}

void
SimpleAddTransaction::buildReplaceRevPtrsRequest() {
    // Construct an empty request.
    D2UpdateMessagePtr request = prepNewRequest(getReverseDomain());

    // Create the reverse IP address "FQDN".
    std::string rev_addr = D2CfgMgr::reverseIpAddress(getNcr()->getIpAddress());
    dns::Name rev_ip(rev_addr);

    // Create the TTL based on lease length.
    dns::RRTTL lease_ttl(getNcr()->getLeaseLength());

    // There are no prerequisites.

    // Create the FQDN/IP PTR 'delete' RR for this IP and add it to
    // the update section.
    dns::RRsetPtr update(new dns::RRset(rev_ip, dns::RRClass::ANY(),
                         dns::RRType::PTR(), dns::RRTTL(0)));
    request->addRRset(D2UpdateMessage::SECTION_UPDATE, update);

    // Create the DHCID 'delete' RR and add it to the update section.
    update.reset(new dns::RRset(rev_ip, dns::RRClass::ANY(),
                                dns::RRType::DHCID(), dns::RRTTL(0)));
    request->addRRset(D2UpdateMessage::SECTION_UPDATE, update);

    // Create the FQDN/IP PTR 'add' RR, add the FQDN as the PTR Rdata
    // then add it to update section.
    update.reset(new dns::RRset(rev_ip, dns::RRClass::IN(),
                                dns::RRType::PTR(), lease_ttl));
    addPtrRdata(update);
    request->addRRset(D2UpdateMessage::SECTION_UPDATE, update);

    // Create the FQDN/IP PTR 'add' RR, add the DHCID Rdata
    // then add it to update section.
    update.reset(new dns::RRset(rev_ip, dns::RRClass::IN(),
                                dns::RRType::DHCID(), lease_ttl));
    addDhcidRdata(update);
    request->addRRset(D2UpdateMessage::SECTION_UPDATE, update);

    // Set the transaction's update request to the new request.
    setDnsUpdateRequest(request);
}

} // namespace isc::d2
} // namespace isc
