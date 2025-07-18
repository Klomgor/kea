// Copyright (C) 2014-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>
#include <dhcp/iface_mgr.h>
#include <dhcp/pkt4.h>
#include <dhcp/classify.h>
#include <dhcp/testutils/iface_mgr_test_config.h>
#include <dhcpsrv/cfgmgr.h>
#include <dhcpsrv/cfg_subnets4.h>
#include <dhcpsrv/lease_mgr_factory.h>
#include <dhcpsrv/subnet.h>
#include <dhcp4/json_config_parser.h>
#include <dhcp4/tests/dhcp4_client.h>
#include <dhcp4/tests/dhcp4_test_utils.h>
#include <gtest/gtest.h>
#include <string>

using namespace isc;
using namespace isc::asiolink;
using namespace isc::data;
using namespace isc::dhcp;
using namespace isc::dhcp::test;

namespace {

/// @brief Test fixture class for testing message processing from directly
/// connected clients.
///
/// This class provides mechanisms for testing processing of DHCPv4 messages
/// from directly connected clients.
class DirectClientTest : public Dhcpv4SrvTest {
public:
    /// @brief Constructor.
    ///
    /// Initializes DHCPv4 server object used by various tests.
    DirectClientTest();

    /// @brief Configures the server with one subnet.
    ///
    /// This creates new configuration for the DHCPv4 with one subnet having
    /// a specified prefix.
    ///
    /// The subnet parameters (such as options, timers etc.) are arbitrarily
    /// selected. The subnet and pool mask is always /24. The real configuration
    /// would exclude .0 (network address) and .255 (broadcast address), but we
    /// ignore that fact for the sake of test simplicity.
    ///
    /// @param prefix Prefix for a subnet.
    void configureSubnet(const std::string& prefix);

    /// @brief Configures the server with two subnets.
    ///
    /// This function configures DHCPv4 server with two different subnets.
    /// The subnet parameters (such as options, timers etc.) are arbitrarily
    /// selected. The subnet and pool mask is /24. The real configuration
    /// would exclude .0 (network address) and .255 (broadcast address), but we
    /// ignore that fact for the sake of test simplicity.
    ///
    /// @param prefix1 Prefix of the first subnet to be configured.
    /// @param prefix2 Prefix of the second subnet to be configured.
    void configureTwoSubnets(const std::string& prefix1,
                             const std::string& prefix2);

    /// @brief Creates simple message from a client.
    ///
    /// This function creates a DHCPv4 message having a specified type
    /// (e.g. Discover, Request) and sets some properties of this
    /// message: client identifier, address and interface. The copy of
    /// this message is then created by parsing wire data of the original
    /// message. This simulates the case when the message is received and
    /// parsed by the server.
    ///
    /// @param msg_type Type of the message to be created.
    /// @param iface Name of the interface on which the message has been
    /// "received" by the server.
    /// @param ifindex Index of the interface on which the message has been
    /// "received" by the server.
    ///
    /// @return Generated message.
    Pkt4Ptr createClientMessage(const uint16_t msg_type,
                                const std::string& iface,
                                const unsigned int ifindex);

    /// @brief Creates simple message from a client.
    ///
    /// This function configures a client's message by adding client identifier,
    /// setting interface and addresses. The copy of this message is then
    /// created by parsing wire data of the original message. This simulates the
    /// case when the message is received and parsed by the server.
    ///
    /// @param msg Caller supplied message to be configured. This object must
    /// not be NULL.
    /// @param iface Name of the interface on which the message has been
    /// "received" by the server.
    /// @param ifindex Index of the interface on which the message has been
    /// "received" by the server.
    ///
    /// @return Configured and parsed message.
    Pkt4Ptr createClientMessage(const Pkt4Ptr &msg,
                                const std::string& iface,
                                const unsigned int ifindex);

    /// @brief This test checks that the message from directly connected client
    /// is processed and that client is offered IPv4 address from the subnet
    /// which is suitable for the local interface on which the client's message
    /// is received. This test uses two subnets, with two active interfaces
    /// which IP addresses belong to these subnets. The address offered to the
    /// client which message has been sent over eth0 should belong to a
    /// different subnet than the address offered for the client sending its
    /// message via eth1.
    void twoSubnets();

    /// @brief This test checks that server selects a subnet when receives a
    /// message through an interface for which the subnet has been configured.
    /// This interface has IPv4 address assigned which belongs to this subnet.
    /// This test also verifies that when the message is received through the
    /// interface for which there is no suitable subnet, the message is
    /// discarded.
    void oneSubnet();

    /// @brief This test verifies that the server uses ciaddr to select a subnet
    /// for a client which renews its lease.
    void renew();

    /// This test verifies that when a client in the Rebinding state broadcasts
    /// a Request message through an interface for which a subnet is configured,
    /// the server responds to this Request. It also verifies that when such a
    /// Request is sent through the interface for which there is no subnet
    /// configured the client's message is discarded.
    void rebind();

    /// @brief classes the client belongs to
    ///
    /// This is empty in most cases, but it is needed as a parameter for all
    /// getSubnet4() calls.
    ClientClasses classify_;
};

DirectClientTest::DirectClientTest() : Dhcpv4SrvTest() {
}

void
DirectClientTest::configureSubnet(const std::string& prefix) {
    std::ostringstream config;
    config << "{ \"interfaces-config\": {"
        "    \"interfaces\": [ \"*\" ]"
        "},"
        "\"rebind-timer\": 2000, "
        "\"renew-timer\": 1000, "
        "\"option-data\": [ ],"
        "\"subnet4\": [ { "
        "    \"id\": 1, "
        "    \"pools\": [ { \"pool\": \"" << prefix << "/24\" } ],"
        "    \"subnet\": \"" << prefix << "/24\", "
        "    \"rebind-timer\": 2000, "
        "    \"renew-timer\": 1000, "
        "    \"valid-lifetime\": 4000"
        "} ],"
        "\"valid-lifetime\": 4000 }";

    configure(config.str());
}

void
DirectClientTest::configureTwoSubnets(const std::string& prefix1,
                                      const std::string& prefix2) {
    std::ostringstream config;
    config << "{ \"interfaces-config\": {"
        "    \"interfaces\": [ \"*\" ]"
        "},"
        "\"rebind-timer\": 2000, "
        "\"renew-timer\": 1000, "
        "\"option-data\": [ ],"
        "\"subnet4\": [ { "
        "    \"id\": 1, "
        "    \"pools\": [ { \"pool\": \"" << prefix1 << "/24\" } ],"
        "    \"subnet\": \"" << prefix1 << "/24\", "
        "    \"rebind-timer\": 2000, "
        "    \"renew-timer\": 1000, "
        "    \"valid-lifetime\": 4000"
        " },"
        "{ "
        "    \"id\": 2, "
        "    \"pools\": [ { \"pool\": \"" << prefix2 << "/24\" } ],"
        "    \"subnet\": \"" << prefix2 << "/24\", "
        "    \"rebind-timer\": 2000, "
        "    \"renew-timer\": 1000, "
        "    \"valid-lifetime\": 4000"
        "} ],"
        "\"valid-lifetime\": 4000 }";

    configure(config.str());
}

Pkt4Ptr
DirectClientTest::createClientMessage(const uint16_t msg_type,
                                      const std::string& iface,
                                      const unsigned int ifindex) {
    // Create a source packet.
    Pkt4Ptr msg = Pkt4Ptr(new Pkt4(msg_type, 1234));
    return (createClientMessage(msg, iface, ifindex));

}

Pkt4Ptr
DirectClientTest::createClientMessage(const Pkt4Ptr& msg,
                                      const std::string& iface,
                                      const unsigned int ifindex) {
    msg->setRemoteAddr(IOAddress("255.255.255.255"));
    msg->addOption(generateClientId());
    msg->setIface(iface);
    msg->setIndex(ifindex);

    // Create copy of this packet by parsing its wire data. Make sure that the
    // local and remote address are set like it was a message sent from the
    // directly connected client.
    Pkt4Ptr received;
    static_cast<void>(createPacketFromBuffer(msg, received));
    received->setIface(iface);
    received->setIndex(ifindex);
    received->setLocalAddr(IOAddress("255.255.255.255"));
    received->setRemoteAddr(IOAddress("0.0.0.0"));

    return (received);
}

void
DirectClientTest::twoSubnets() {
    // Configure IfaceMgr with fake interfaces lo, eth0 and eth1.
    IfaceMgrTestConfig iface_config(true);
    // After creating interfaces we have to open sockets as it is required
    // by the message processing code.
    ASSERT_NO_THROW(IfaceMgr::instance().openSockets4());
    // Add two subnets: address on eth0 belongs to the second subnet,
    // address on eth1 belongs to the first subnet.
    ASSERT_NO_FATAL_FAILURE(configureTwoSubnets("192.0.2.0", "10.0.0.0"));
    // Create Discover and simulate reception of this message through eth0.
    Pkt4Ptr dis = createClientMessage(DHCPDISCOVER, "eth0", ETH0_INDEX);
    srv_->fakeReceive(dis);
    // Create Request and simulate reception of this message through eth1.
    Pkt4Ptr req = createClientMessage(DHCPREQUEST, "eth1", ETH1_INDEX);
    srv_->fakeReceive(req);

    // Process clients' messages.
    srv_->run();

    // Check that the server did send responses.
    ASSERT_EQ(2, srv_->fake_sent_.size());

    // In multi-threading responses can be received out of order.
    Pkt4Ptr offer;
    Pkt4Ptr ack;

    while (srv_->fake_sent_.size()) {
        // Make sure that we received a response.
        Pkt4Ptr response = srv_->fake_sent_.front();
        ASSERT_TRUE(response);
        srv_->fake_sent_.pop_front();

        if (response->getType() == DHCPOFFER) {
            offer = response;
        } else if (response->getType() == DHCPACK) {
            ack = response;
        }
    }

    // Client should get an Offer (not a NAK).
    ASSERT_TRUE(offer);

    // Client should get an Ack (not a NAK).
    ASSERT_TRUE(ack);

    // Check that the offered address belongs to the suitable subnet.
    ConstSubnet4Ptr subnet = CfgMgr::instance().getCurrentCfg()->
        getCfgSubnets4()->selectSubnet(offer->getYiaddr());
    ASSERT_TRUE(subnet);
    EXPECT_EQ("10.0.0.0", subnet->get().first.toText());


    // Check that the offered address belongs to the suitable subnet.
    subnet = CfgMgr::instance().getCurrentCfg()->
        getCfgSubnets4()->selectSubnet(ack->getYiaddr());
    ASSERT_TRUE(subnet);
    EXPECT_EQ("192.0.2.0", subnet->get().first.toText());
}

TEST_F(DirectClientTest, twoSubnets) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    twoSubnets();
}

TEST_F(DirectClientTest, twoSubnetsMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    twoSubnets();
}

void
DirectClientTest::oneSubnet() {
    // Configure IfaceMgr with fake interfaces lo, eth0 and eth1.
    IfaceMgrTestConfig iface_config(true);
    // After creating interfaces we have to open sockets as it is required
    // by the message processing code.
    ASSERT_NO_THROW(IfaceMgr::instance().openSockets4());
    // Add a subnet which will be selected when a message from directly
    // connected client is received through interface eth0.
    ASSERT_NO_FATAL_FAILURE(configureSubnet("10.0.0.0"));
    // Create Discover and simulate reception of this message through eth0.
    Pkt4Ptr dis = createClientMessage(DHCPDISCOVER, "eth0", ETH0_INDEX);
    srv_->fakeReceive(dis);
    // Create Request and simulate reception of this message through eth1.
    Pkt4Ptr req = createClientMessage(DHCPDISCOVER, "eth1", ETH1_INDEX);
    srv_->fakeReceive(req);

    // Process clients' messages.
    srv_->run();

    // Check that the server sent one response for the message received
    // through eth0. The other client's message should be discarded.
    ASSERT_EQ(1, srv_->fake_sent_.size());

    // Check the response. The first Discover was sent via eth0 for which
    // the subnet has been configured.
    Pkt4Ptr response = srv_->fake_sent_.front();
    ASSERT_TRUE(response);
    srv_->fake_sent_.pop_front();

    // Since Discover has been received through the interface for which
    // the subnet has been configured, the server should respond with
    // an Offer message.
    ASSERT_EQ(DHCPOFFER, response->getType());
    // Check that the offered address belongs to the suitable subnet.
    ConstSubnet4Ptr subnet = CfgMgr::instance().getCurrentCfg()->
        getCfgSubnets4()->selectSubnet(response->getYiaddr());
    ASSERT_TRUE(subnet);
    EXPECT_EQ("10.0.0.0", subnet->get().first.toText());
}

TEST_F(DirectClientTest, oneSubnet) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    oneSubnet();
}

TEST_F(DirectClientTest, oneSubnetMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    oneSubnet();
}

void
DirectClientTest::renew() {
    // Configure IfaceMgr with fake interfaces lo, eth0 and eth1.
    IfaceMgrTestConfig iface_config(true);
    // After creating interfaces we have to open sockets as it is required
    // by the message processing code.
    ASSERT_NO_THROW(IfaceMgr::instance().openSockets4());
    // Add a subnet.
    ASSERT_NO_FATAL_FAILURE(configureSubnet("10.0.0.0"));

    // Create the DHCPv4 client.
    Dhcp4Client client(srv_);
    client.useRelay(false);

    // Obtain the lease using the 4-way exchange.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<IOAddress>(new IOAddress("10.0.0.10"))));
    ASSERT_EQ("10.0.0.10", client.config_.lease_.addr_.toText());

    // Put the client into the renewing state.
    client.setState(Dhcp4Client::RENEWING);

    // Renew, and make sure we have obtained the same address.
    ASSERT_NO_THROW(client.doRequest());
    ASSERT_TRUE(client.getContext().response_);
    EXPECT_EQ(DHCPACK, static_cast<int>(client.getContext().response_->getType()));
    EXPECT_EQ("10.0.0.10", client.config_.lease_.addr_.toText());
}

TEST_F(DirectClientTest, renew) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    renew();
}

TEST_F(DirectClientTest, renewMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    renew();
}

void
DirectClientTest::rebind() {
    // Configure IfaceMgr with fake interfaces lo, eth0 and eth1.
    IfaceMgrTestConfig iface_config(true);
    // After creating interfaces we have to open sockets as it is required
    // by the message processing code.
    ASSERT_NO_THROW(IfaceMgr::instance().openSockets4());
    // Add a subnet.
    ASSERT_NO_FATAL_FAILURE(configureSubnet("10.0.0.0"));

    // Create the DHCPv4 client.
    Dhcp4Client client(srv_);
    client.useRelay(false);

    // Obtain the lease using the 4-way exchange.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<IOAddress>(new IOAddress("10.0.0.10"))));
    ASSERT_EQ("10.0.0.10", client.config_.lease_.addr_.toText());

    // Put the client into the rebinding state.
    client.setState(Dhcp4Client::REBINDING);

    // Broadcast Request through an interface for which there is no subnet
    // configured. This message should be discarded by the server.
    client.setIfaceName("eth1");
    client.setIfaceIndex(ETH1_INDEX);
    ASSERT_NO_THROW(client.doRequest());
    EXPECT_FALSE(client.getContext().response_);

    // Send Rebind over the correct interface, and make sure we have obtained
    // the same address.
    client.setIfaceName("eth0");
    client.setIfaceIndex(ETH0_INDEX);
    ASSERT_NO_THROW(client.doRequest());
    ASSERT_TRUE(client.getContext().response_);
    EXPECT_EQ(DHCPACK, static_cast<int>(client.getContext().response_->getType()));
    EXPECT_EQ("10.0.0.10", client.config_.lease_.addr_.toText());
}

TEST_F(DirectClientTest, rebind) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    rebind();
}

TEST_F(DirectClientTest, rebindMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    rebind();
}

}
