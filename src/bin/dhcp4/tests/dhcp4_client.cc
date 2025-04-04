// Copyright (C) 2014-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>
#include <dhcp/dhcp4.h>
#include <dhcp/option.h>
#include <dhcp/option_int_array.h>
#include <dhcp/option_vendor.h>
#include <dhcp/testutils/iface_mgr_test_config.h>
#include <dhcpsrv/lease.h>
#include <dhcp4/tests/dhcp4_client.h>
#include <util/multi_threading_mgr.h>
#include <util/range_utilities.h>
#include <boost/pointer_cast.hpp>
#include <cstdlib>

using namespace isc::asiolink;
using namespace isc::util;

namespace isc {
namespace dhcp {
namespace test {

Dhcp4Client::Configuration::Configuration()
    : routers_(), dns_servers_(), log_servers_(), quotes_servers_(),
      serverid_("0.0.0.0"), siaddr_(IOAddress::IPV4_ZERO_ADDRESS()) {
    reset();
}

void
Dhcp4Client::Configuration::reset() {
    routers_.clear();
    dns_servers_.clear();
    log_servers_.clear();
    quotes_servers_.clear();
    serverid_ = IOAddress("0.0.0.0");
    siaddr_ = IOAddress::IPV4_ZERO_ADDRESS();
    sname_.clear();
    boot_file_name_.clear();
    lease_ = Lease4();
}

Dhcp4Client::Dhcp4Client(const Dhcp4Client::State& state) :
    config_(),
    ciaddr_(),
    curr_transid_(0),
    dest_addr_("255.255.255.255"),
    hwaddr_(generateHWAddr()),
    clientid_(),
    iface_name_("eth0"),
    iface_index_(ETH0_INDEX),
    relay_addr_("192.0.2.2"),
    requested_options_(),
    server_facing_relay_addr_("10.0.0.2"),
    srv_(boost::shared_ptr<NakedDhcpv4Srv>(new NakedDhcpv4Srv(0))),
    state_(state),
    use_relay_(false),
    circuit_id_() {
    std::string memfile = "type=memfile universe=4 persist=false";
    isc::dhcp::LeaseMgrFactory::create(memfile);
}

Dhcp4Client::Dhcp4Client(boost::shared_ptr<NakedDhcpv4Srv> srv,
                         const Dhcp4Client::State& state) :
    config_(),
    ciaddr_(),
    curr_transid_(0),
    dest_addr_("255.255.255.255"),
    fqdn_(),
    hwaddr_(generateHWAddr()),
    clientid_(),
    iface_name_("eth0"),
    iface_index_(ETH0_INDEX),
    relay_addr_("192.0.2.2"),
    requested_options_(),
    server_facing_relay_addr_("10.0.0.2"),
    srv_(srv),
    state_(state),
    use_relay_(false),
    circuit_id_() {
}

void
Dhcp4Client::addRequestedAddress(const IOAddress& addr) {
    if (context_.query_) {
        Option4AddrLstPtr opt(new Option4AddrLst(DHO_DHCP_REQUESTED_ADDRESS,
                                                 addr));
        context_.query_->addOption(opt);
    }
}

void
Dhcp4Client::appendClientId() {
    if (!context_.query_) {
        isc_throw(Dhcp4ClientError, "pointer to the query must not be NULL"
                  " when adding Client Identifier option");
    }

    if (clientid_) {
        OptionPtr opt(new Option(Option::V4, DHO_DHCP_CLIENT_IDENTIFIER,
                                 clientid_->getClientId()));
        context_.query_->addOption(opt);
    }
}

void
Dhcp4Client::appendServerId() {
    OptionPtr opt(new Option4AddrLst(DHO_DHCP_SERVER_IDENTIFIER,
                                     config_.serverid_));
    context_.query_->addOption(opt);
}

void
Dhcp4Client::appendName() {
    if (!context_.query_) {
        isc_throw(Dhcp4ClientError, "pointer to the query must not be NULL"
                  " when adding FQDN or Hostname option");
    }

    if (fqdn_) {
        context_.query_->addOption(fqdn_);

    } else if (hostname_) {
        context_.query_->addOption(hostname_);
    }
}

void
Dhcp4Client::appendPRL() {
    if (!context_.query_) {
        isc_throw(Dhcp4ClientError, "pointer to the query must not be NULL"
                  " when adding option codes to the PRL option");

    } else if (!requested_options_.empty()) {
        // Include Parameter Request List if at least one option code
        // has been specified to be requested.
        OptionUint8ArrayPtr prl(new OptionUint8Array(Option::V4,
                                  DHO_DHCP_PARAMETER_REQUEST_LIST));
        for (auto const& opt : requested_options_) {
            prl->addValue(opt);
        }
        context_.query_->addOption(prl);
    }
}

void
Dhcp4Client::applyConfiguration() {
    Pkt4Ptr resp = context_.response_;
    if (!resp) {
        return;
    }

    // Let's keep the old lease in case this is a response to Inform.
    Lease4 old_lease = config_.lease_;
    config_.reset();

    // Routers
    Option4AddrLstPtr opt_routers = boost::dynamic_pointer_cast<
        Option4AddrLst>(resp->getOption(DHO_ROUTERS));
    if (opt_routers) {
        config_.routers_ = opt_routers->getAddresses();
    }
    // DNS Servers
    Option4AddrLstPtr opt_dns_servers = boost::dynamic_pointer_cast<
        Option4AddrLst>(resp->getOption(DHO_DOMAIN_NAME_SERVERS));
    if (opt_dns_servers) {
        config_.dns_servers_ = opt_dns_servers->getAddresses();
    }
    // Log Servers
    Option4AddrLstPtr opt_log_servers = boost::dynamic_pointer_cast<
        Option4AddrLst>(resp->getOption(DHO_LOG_SERVERS));
    if (opt_log_servers) {
        config_.log_servers_ = opt_log_servers->getAddresses();
    }
    // Quotes Servers
    Option4AddrLstPtr opt_quotes_servers = boost::dynamic_pointer_cast<
        Option4AddrLst>(resp->getOption(DHO_COOKIE_SERVERS));
    if (opt_quotes_servers) {
        config_.quotes_servers_ = opt_quotes_servers->getAddresses();
    }
    // Vendor Specific options
    OptionVendorPtr opt_vendor = boost::dynamic_pointer_cast<
        OptionVendor>(resp->getOption(DHO_VIVSO_SUBOPTIONS));
    if (opt_vendor) {
        config_.vendor_suboptions_ = opt_vendor->getOptions();
    }
    // siaddr
    config_.siaddr_ = resp->getSiaddr();
    // sname
    OptionBuffer buf = resp->getSname();
    // sname is a fixed length field holding null terminated string. Use
    // of c_str() guarantees that only a useful portion (ending with null
    // character) is assigned.
    config_.sname_.assign(std::string(buf.begin(), buf.end()).c_str());
    // (boot)file
    buf = resp->getFile();
    config_.boot_file_name_.assign(std::string(buf.begin(), buf.end()).c_str());
    // Server Identifier
    OptionCustomPtr opt_serverid = boost::dynamic_pointer_cast<
        OptionCustom>(resp->getOption(DHO_DHCP_SERVER_IDENTIFIER));
    if (opt_serverid) {
        config_.serverid_ = opt_serverid->readAddress();
    }

    // If the message sent was Inform, we don't want to throw
    // away the old lease info, just the bits about options.
    if (context_.query_->getType() == DHCPINFORM) {
        config_.lease_ = old_lease;
    } else {
        /// @todo Set the valid lifetime, t1, t2 etc.
        config_.lease_ = Lease4(IOAddress(context_.response_->getYiaddr()),
                                context_.response_->getHWAddr(),
                                0, 0, 0, time(NULL), 0, false, false,
                                "");
    }
}

void
Dhcp4Client::createLease(const IOAddress& addr, const uint32_t valid_lft) {
    Lease4 lease(addr, hwaddr_, 0, 0, valid_lft,
                 time(NULL), 0, false, false, "");
    config_.lease_ = lease;
}

Pkt4Ptr
Dhcp4Client::createMsg(const uint8_t msg_type) {
    Pkt4Ptr msg(new Pkt4(msg_type, curr_transid_++));
    msg->setHWAddr(hwaddr_);
    return (msg);
}

void
Dhcp4Client::appendExtraOptions() {
    // If there are any custom options specified, add them all to the message.
    if (!extra_options_.empty()) {
        for (auto const& opt : extra_options_) {
            // Call base class function so that unittests can add multiple
            // options with the same code.
            context_.query_->Pkt::addOption(opt.second);
        }
    }
}

void
Dhcp4Client::appendClasses() {
    for (auto const& cclass : classes_) {
        context_.query_->addClass(cclass);
    }
}

void
Dhcp4Client::doDiscover(const boost::shared_ptr<IOAddress>& requested_addr) {
    context_.query_ = createMsg(DHCPDISCOVER);
    // Request options if any.
    appendPRL();
    // Include FQDN or Hostname.
    appendName();
    // Include Client Identifier
    appendClientId();
    if (requested_addr) {
        addRequestedAddress(*requested_addr);
    }
    // Override the default ciaddr if specified by a test.
    if (!ciaddr_.unspecified()) {
        context_.query_->setCiaddr(ciaddr_);
    }
    appendExtraOptions();
    appendClasses();

    // Send the message to the server.
    sendMsg(context_.query_);
    // Expect response.
    context_.response_ = receiveOneMsg();
}

void
Dhcp4Client::doDORA(const boost::shared_ptr<IOAddress>& requested_addr) {
    doDiscover(requested_addr);
    if (context_.response_ && (context_.response_->getType() == DHCPOFFER)) {
        doRequest();
    }
}

void
Dhcp4Client::doInform(const bool set_ciaddr) {
    context_.query_ = createMsg(DHCPINFORM);
    // Request options if any.
    appendPRL();
    // Any other options to be sent by a client.
    appendExtraOptions();
    // Add classes.
    appendClasses();
    // The client sending a DHCPINFORM message has an IP address obtained
    // by some other means, e.g. static configuration. The lease which we
    // are using here is most likely set by the createLease method.
    if (set_ciaddr) {
        context_.query_->setCiaddr(config_.lease_.addr_);
    }
    context_.query_->setLocalAddr(config_.lease_.addr_);
    // Send the message to the server.
    sendMsg(context_.query_);
    // Expect response. If there is no response, return.
    context_.response_ = receiveOneMsg();
    if (!context_.response_) {
        return;
    }
    // If DHCPACK has been returned by the server, use the returned
    // configuration.
    if (context_.response_->getType() == DHCPACK) {
        applyConfiguration();
    }
}

void
Dhcp4Client::doRelease() {
    // There is no response for Release message.
    context_.response_.reset();

    if (config_.lease_.addr_.isV4Zero()) {
        isc_throw(Dhcp4ClientError, "failed to send the release"
                  " message because client doesn't have a lease");
    }
    context_.query_ = createMsg(DHCPRELEASE);
    // Set ciaddr to the address which we want to release.
    context_.query_->setCiaddr(config_.lease_.addr_);
    // Include client identifier.
    appendClientId();

    // Remove configuration.
    config_.reset();

    // Send the message to the server.
    sendMsg(context_.query_);
}

void
Dhcp4Client::doDecline() {
    if (config_.lease_.addr_.isV4Zero()) {
        isc_throw(Dhcp4ClientError, "failed to send the decline"
                  " message because client doesn't have a lease");
    }

    context_.query_ = createMsg(DHCPDECLINE);

    // Set ciaddr to 0.
    context_.query_->setCiaddr(IOAddress("0.0.0.0"));

    // Include Requested IP Address Option
    addRequestedAddress(config_.lease_.addr_);

    // Include client identifier.
    appendClientId();

    // Include server identifier.
    appendServerId();

    // Remove configuration.
    config_.reset();

    // Send the message to the server.
    sendMsg(context_.query_);
}

void
Dhcp4Client::doRequest() {
    context_.query_ = createMsg(DHCPREQUEST);

    // Override the default ciaddr if specified by a test.
    if (!ciaddr_.unspecified()) {
        context_.query_->setCiaddr(ciaddr_);
    } else if ((state_ == SELECTING) || (state_ == INIT_REBOOT)) {
        context_.query_->setCiaddr(IOAddress("0.0.0.0"));
    } else {
        context_.query_->setCiaddr(IOAddress(config_.lease_.addr_));
    }

    // Requested IP address.
    if (state_ == SELECTING) {
        if (context_.response_ &&
            (context_.response_->getType() == DHCPOFFER) &&
            (context_.response_->getYiaddr() != IOAddress("0.0.0.0"))) {
            addRequestedAddress(context_.response_->getYiaddr());
        } else {
            isc_throw(Dhcp4ClientError, "error sending the DHCPREQUEST because"
                      " the received DHCPOFFER message was invalid");
        }
    } else if (state_ == INIT_REBOOT) {
        addRequestedAddress(config_.lease_.addr_);
    }

    // Server identifier.
    if (state_ == SELECTING) {
        if (context_.response_) {
            OptionPtr opt_serverid =
                context_.response_->getOption(DHO_DHCP_SERVER_IDENTIFIER);
            if (!opt_serverid) {
                isc_throw(Dhcp4ClientError, "missing server identifier in the"
                          " server's response");
            }
            context_.query_->addOption(opt_serverid);
        }
    }

    // Request options if any.
    appendPRL();
    // Include FQDN or Hostname.
    appendName();
    // Include Client Identifier
    appendClientId();
    // Any other options to be sent by a client.
    appendExtraOptions();
    // Add classes.
    appendClasses();
    // Send the message to the server.
    sendMsg(context_.query_);
    // Expect response.
    context_.response_ = receiveOneMsg();
    // If the server has responded, store the configuration received.
    if (context_.response_) {
        applyConfiguration();
    }
}

void
Dhcp4Client::receiveResponse() {
    context_.response_ = receiveOneMsg();
    // If the server has responded, store the configuration received.
    if (context_.response_) {
        applyConfiguration();
    }
}

void
Dhcp4Client::includeClientId(const std::string& clientid) {
    if (clientid.empty()) {
        clientid_.reset();

    } else {
        clientid_ = ClientId::fromText(clientid);
    }
}

void
Dhcp4Client::includeFQDN(const uint8_t flags, const std::string& fqdn_name,
                         Option4ClientFqdn::DomainNameType fqdn_type) {
    fqdn_.reset(new Option4ClientFqdn(flags, Option4ClientFqdn::RCODE_CLIENT(),
                                      fqdn_name, fqdn_type));
}

void
Dhcp4Client::includeHostname(const std::string& name) {
    if (name.empty()) {
       hostname_.reset();
    } else {
        hostname_.reset(new OptionString(Option::V4, DHO_HOST_NAME, name));
    }
}

HWAddrPtr
Dhcp4Client::generateHWAddr(const uint8_t htype) const {
    if (htype != HTYPE_ETHER) {
        isc_throw(isc::NotImplemented,
                  "The hardware address type " << static_cast<int>(htype)
                  << " is currently not supported");
    }
    std::vector<uint8_t> hwaddr(HWAddr::ETHERNET_HWADDR_LEN);
    // Generate ethernet hardware address by assigning random byte values.
    isc::util::fillRandom(hwaddr.begin(), hwaddr.end());
    return (HWAddrPtr(new HWAddr(hwaddr, htype)));
}

void
Dhcp4Client::modifyHWAddr() {
    if (!hwaddr_) {
        hwaddr_ = generateHWAddr();
        return;
    }
    // Modify the HW address by adding 1 to its last byte.
    ++hwaddr_->hwaddr_[hwaddr_->hwaddr_.size() - 1];
}

void
Dhcp4Client::requestOption(const uint8_t option) {
    if (option != 0) {
        requested_options_.insert(option);
    }
}

void
Dhcp4Client::requestOptions(const uint8_t option1, const uint8_t option2,
                            const uint8_t option3) {
    requested_options_.clear();
    requestOption(option1);
    requestOption(option2);
    requestOption(option3);
}

Pkt4Ptr
Dhcp4Client::receiveOneMsg() {
    Pkt4Ptr msg = srv_->receiveOneMsg();
    if (!msg) {
        return (Pkt4Ptr());
    }

    // Copy the original message to simulate reception over the wire.
    msg->pack();
    Pkt4Ptr msg_copy(new Pkt4(static_cast<const uint8_t*>
                              (msg->getBuffer().getData()),
                              msg->getBuffer().getLength()));
    msg_copy->setRemoteAddr(msg->getLocalAddr());
    msg_copy->setLocalAddr(msg->getRemoteAddr());
    msg_copy->setRemotePort(msg->getLocalPort());
    msg_copy->setLocalPort(msg->getRemotePort());
    msg_copy->setIface(msg->getIface());
    msg_copy->setIndex(msg->getIndex());

    msg_copy->unpack();

    return (msg_copy);
}

void
Dhcp4Client::sendMsg(const Pkt4Ptr& msg) {
    srv_->shutdown_ = false;
    if (use_relay_) {
        try {
            msg->setHops(1);
            msg->setGiaddr(relay_addr_);
            msg->setLocalAddr(server_facing_relay_addr_);
            // Insert RAI
            OptionPtr rai(new Option(Option::V4, DHO_DHCP_AGENT_OPTIONS));
            // Insert circuit id, if specified.
            if (!circuit_id_.empty()) {
                rai->addOption(OptionPtr(new Option(Option::V4, RAI_OPTION_AGENT_CIRCUIT_ID,
                                                    OptionBuffer(circuit_id_.begin(),
                                                                 circuit_id_.end()))));
            }
            msg->addOption(rai);
        } catch (...) {
            // If relay options have already been added in the unittest, ignore
            // exception on add.
        }
    }
    // Repack the message to simulate wire-data parsing.
    msg->pack();
    Pkt4Ptr msg_copy(new Pkt4(static_cast<const uint8_t*>
                              (msg->getBuffer().getData()),
                              msg->getBuffer().getLength()));
    msg_copy->setRemoteAddr(msg->getLocalAddr());
    msg_copy->setLocalAddr(dest_addr_);
    msg_copy->setIface(iface_name_);
    msg_copy->setIndex(iface_index_);
    // Copy classes
    const ClientClasses& classes = msg->getClasses();
    for (auto const& cclass : classes) {
        msg_copy->addClass(cclass);
    }
    srv_->fakeReceive(msg_copy);

    try {
        // Invoke runOne instead of run, because we want to avoid triggering
        // IO service.
        srv_->runOne();
    } catch (...) {
        // Suppress errors, as the DHCPv4 server does.
    }

    // Make sure the server processed all packets in MT.
    isc::util::MultiThreadingMgr::instance().getThreadPool().wait(3);
}

void
Dhcp4Client::setHWAddress(const std::string& hwaddr_str) {
    if (hwaddr_str.empty()) {
        hwaddr_.reset();
    } else {
        hwaddr_.reset(new HWAddr(HWAddr::fromText(hwaddr_str)));
    }
}

void
Dhcp4Client::addExtraOption(const OptionPtr& opt) {
    extra_options_.insert(std::make_pair(opt->getType(), opt));
}

void
Dhcp4Client::addClass(const ClientClass& client_class) {
    if (!classes_.contains(client_class)) {
        classes_.insert(client_class);
    }
}

} // end of namespace isc::dhcp::test
} // end of namespace isc::dhcp
} // end of namespace isc
