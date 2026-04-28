// Copyright (C) 2025-2026 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PKT_FILTER_IFACE_TEST_UTILS_H
#define PKT_FILTER_IFACE_TEST_UTILS_H

#include <asiolink/io_address.h>
#include <dhcp/iface_mgr.h>
#include <dhcp/pkt_filter.h>
#include <gtest/gtest.h>

#include <mutex>
#include <unordered_map>

namespace isc {
namespace dhcp {
namespace test {

class PktFilterIfaceSocketTest {
public:
    /// @brief Constructor.
    ///
    /// @param ready_on_send Flag which indicates if socket should be marked as
    /// readReady when calling @ref send.
    /// @param clear_on_read Flag which indicates is socket should be unmarked as
    /// readReady when calling @ref receive.
    PktFilterIfaceSocketTest(bool ready_on_send = true, bool clear_on_read = true);

    /// @brief Destructor.
    virtual ~PktFilterIfaceSocketTest();

    /// @brief Simulate opening of the socket.
    ///
    /// This function simulates opening a primary socket. In reality, it doesn't
    /// open a socket but uses a pipe which can control if a read event is ready
    /// or not.
    ///
    /// @param iface An interface descriptor.
    /// @param addr Address on the interface to be used to send packets.
    /// @param port Port number to bind socket to.
    /// @param receive_bcast A flag which indicates if socket should be
    /// configured to receive broadcast packets (if true).
    /// @param send_bcast A flag which indicates if the socket should be
    /// configured to send broadcast packets (if true).
    ///
    /// @return A SocketInfo structure with the socket descriptor set. The
    /// fallback socket descriptor is set to a negative value.
    virtual SocketInfo openSocketCommon(Iface& iface,
                                        const isc::asiolink::IOAddress& addr,
                                        const uint16_t port);

    /// @brief Simulate reception of the DHCPv4/DHCPv6 message.
    ///
    /// @param sock_info A descriptor of the primary and fallback sockets.
    ///
    /// @return the same packet used by @ref send (if any).
    virtual PktPtr receiveCommon(const SocketInfo& sock_info);

    /// @brief Simulates sending a DHCPv4/DHCPv6 message.
    ///
    /// @param iface An interface to be used to send DHCPv4/DHCPv6 message.
    /// @param sockfd socket descriptor.
    /// @param pkt A DHCPv4/DHCPv6 to be sent.
    ///
    /// @return 0.
    virtual int sendCommon(const Iface& iface, uint16_t sockfd, const PktPtr& pkt);

    /// @brief Flag which indicates if socket should be marked as
    /// readReady when calling @ref send.
    bool ready_on_send_;

    /// @brief Flag which indicates is socket should be unmarked as
    /// readReady when calling @ref receive.
    bool clear_on_read_;

    /// @brief Mutex to protect the internal state.
    std::mutex mutex_;

    /// @brief The set of opened file descriptors.
    std::unordered_map<int, int> socket_fds_;

    std::unordered_map<int, PktPtr> pkts_;
};

class PktFilter4IfaceSocketTest : public PktFilterIfaceSocketTest, public PktFilter {
public:

    /// @brief Constructor.
    ///
    /// @param ready_on_send Flag which indicates if socket should be marked as
    /// readReady when calling @ref send.
    /// @param clear_on_read Flag which indicates is socket should be unmarked as
    /// readReady when calling @ref receive.
    PktFilter4IfaceSocketTest(bool ready_on_send = true, bool clear_on_read = true);

    /// @brief Destructor.
    ~PktFilter4IfaceSocketTest() = default;

    /// @brief Checks if the direct DHCPv4 response is supported.
    ///
    /// This function checks if the direct response capability is supported,
    /// i.e. if the server can respond to the client which doesn't have an
    /// address yet. For this dummy class, the true is always returned.
    ///
    /// @return always true.
    virtual bool isDirectResponseSupported() const;

    /// @brief Check if the socket received time is supported.
    ///
    /// If true, then packets received through this filter will include
    /// a SOCKET_RECEIVED event in its event stack.
    ///
    /// @return always true.
    virtual bool isSocketReceivedTimeSupported() const;

    /// @brief Simulate opening of the socket.
    ///
    /// This function simulates opening a primary socket. In reality, it doesn't
    /// open a socket but uses a pipe which can control if a read event is ready
    /// or not.
    ///
    /// @param iface An interface descriptor.
    /// @param addr Address on the interface to be used to send packets.
    /// @param port Port number to bind socket to.
    /// @param receive_bcast A flag which indicates if socket should be
    /// configured to receive broadcast packets (if true).
    /// @param send_bcast A flag which indicates if the socket should be
    /// configured to send broadcast packets (if true).
    ///
    /// @return A SocketInfo structure with the socket descriptor set. The
    /// fallback socket descriptor is set to a negative value.
    virtual SocketInfo openSocket(Iface& iface,
                                  const isc::asiolink::IOAddress& addr,
                                  const uint16_t port,
                                  const bool receive_bcast,
                                  const bool send_bcast);

    /// @brief Simulate reception of the DHCPv4 message.
    ///
    /// @param iface An interface to be used to receive DHCPv4 message.
    /// @param sock_info A descriptor of the primary and fallback sockets.
    ///
    /// @return the same packet used by @ref send (if any).
    virtual Pkt4Ptr receive(Iface& iface, const SocketInfo& sock_info);

    /// @brief Simulates sending a DHCPv4 message.
    ///
    /// @param iface An interface to be used to send DHCPv4 message.
    /// @param sockfd socket descriptor.
    /// @param pkt A DHCPv4 to be sent.
    ///
    /// @return 0.
    virtual int send(const Iface& iface, uint16_t sockfd, const Pkt4Ptr& pkt);
};

class PktFilter6IfaceSocketTest : public PktFilterIfaceSocketTest, public PktFilter6 {
public:

    /// @brief Constructor.
    ///
    /// @param ready_on_send Flag which indicates if socket should be marked as
    /// readReady when calling @ref send.
    /// @param clear_on_read Flag which indicates is socket should be unmarked as
    /// readReady when calling @ref receive.
    PktFilter6IfaceSocketTest(bool ready_on_send = true, bool clear_on_read = true);

    /// @brief Destructor.
    ~PktFilter6IfaceSocketTest() = default;

    /// @brief Simulate opening of the socket.
    ///
    /// This function simulates opening a primary socket. In reality, it doesn't
    /// open a socket but uses a pipe which can control if a read event is ready
    /// or not.
    ///
    /// @param iface Interface descriptor.
    /// @param addr Address on the interface to be used to send packets.
    /// @param port Port number.
    /// @param join_multicast A boolean parameter which indicates whether
    /// socket should join All_DHCP_Relay_Agents_and_servers multicast
    /// group.
    ///
    /// @return A SocketInfo structure with the socket descriptor set. The
    /// fallback socket descriptor is set to a negative value.
    virtual SocketInfo openSocket(Iface& iface,
                                  const isc::asiolink::IOAddress& addr,
                                  const uint16_t port,
                                  const bool join_multicast);

    /// @brief Simulate reception of the DHCPv6 message.
    ///
    /// @param iface An interface to be used to receive DHCPv6 message.
    /// @param sock_info A descriptor of the primary and fallback sockets.
    ///
    /// @return the same packet used by @ref send (if any).
    virtual Pkt6Ptr receive(const SocketInfo& sock_info);

    /// @brief Simulates sending a DHCPv6 message.
    ///
    /// @param iface An interface to be used to send DHCPv6 message.
    /// @param sockfd socket descriptor.
    /// @param pkt A DHCPv6 to be sent.
    ///
    /// @return 0.
    virtual int send(const Iface& iface, uint16_t sockfd, const Pkt6Ptr& pkt);
};

}  // end of isc::dhcp::test namespace
}  // end of isc::dhcp namespace
}  // end of isc namespace

#endif // PKT_FILTER_IFACE_TEST_UTILS_H
