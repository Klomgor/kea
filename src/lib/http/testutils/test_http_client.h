// Copyright (C) 2017-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TEST_HTTP_CLIENT_H
#define TEST_HTTP_CLIENT_H

#include <cc/data.h>
#include <http/client.h>
#include <http/http_types.h>
#include <http/response_parser.h>

#include <boost/asio/read.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <gtest/gtest.h>

using namespace boost::asio::ip;
using namespace isc::asiolink;
using namespace isc::http;

/// @brief Sending chuck size.
const size_t TEST_HTTP_CHUCK_SIZE = 8 * 1024;

/// @brief Common base for test HTTP/HTTPS clients.
class BaseTestHttpClient : public boost::noncopyable {
public:

    /// @brief Destructor.
    virtual ~BaseTestHttpClient() = default;

    /// @brief Send HTTP request specified in textual format.
    ///
    /// @param request HTTP request in the textual format.
    virtual void startRequest(const std::string& request) = 0;

    /// @brief Send HTTP request.
    ///
    /// @param request HTTP request in the textual format.
    virtual void sendRequest(const std::string& request) = 0;

    /// @brief Send part of the HTTP request.
    ///
    /// @param request part of the HTTP request to be sent.
    virtual void sendPartialRequest(std::string request) = 0;

    /// @brief Receive response from the server.
    virtual void receivePartialResponse() = 0;

    /// @brief Checks if the TCP connection is still open.
    ///
    /// Tests the TCP connection by trying to read from the socket.
    /// Unfortunately expected failure depends on a race between the read
    /// and the other side close so to check if the connection is closed
    /// please use @c isConnectionClosed instead.
    ///
    /// @return true if the TCP connection is open.
    virtual bool isConnectionAlive() = 0;

    /// @brief Checks if the TCP connection is already closed.
    ///
    /// Tests the TCP connection by trying to read from the socket.
    /// The read can block so this must be used to check if a connection
    /// is alive so to check if the connection is alive please always
    /// use @c isConnectionAlive.
    ///
    /// @return true if the TCP connection is closed.
    virtual bool isConnectionClosed() = 0;

    /// @brief Close connection.
    virtual void close() = 0;

    /// @brief Returns the HTTP response string.
    ///
    /// @return string containing the response.
    virtual std::string getResponse() const = 0;

    /// @brief Returns true if the receive completed without error.
    ///
    /// @return True if the receive completed successfully, false
    /// otherwise.
    virtual bool receiveDone() const = 0;
};

/// @brief Entity which can connect to the HTTP server endpoint.
class TestHttpClient : public BaseTestHttpClient,
                       public boost::enable_shared_from_this<TestHttpClient> {
public:

    /// @brief Constructor.
    ///
    /// This constructor creates new socket instance. It doesn't connect. Call
    /// connect() to connect to the server.
    ///
    /// @param io_service IO service to be stopped on error or completion.
    /// @param server_address string containing the IP address of the server.
    /// @param port port number of the server.
    explicit TestHttpClient(const IOServicePtr& io_service,
                            const std::string& server_address = "127.0.0.1",
                            uint16_t port = 18123)
        : io_service_(io_service), socket_(io_service_->getInternalIOService()),
          buf_(), response_(), server_address_(server_address),
          server_port_(port), receive_done_(false) {
    }

    /// @brief Destructor.
    ///
    /// Closes the underlying socket if it is open.
    virtual ~TestHttpClient() {
        close();
    }

    /// @brief Send HTTP request specified in textual format.
    ///
    /// @param request HTTP request in the textual format.
    virtual void startRequest(const std::string& request) {
        tcp::endpoint endpoint(make_address(server_address_), server_port_);
        auto ref = shared_from_this();
        socket_.async_connect(endpoint,
                              [this, ref, request](const boost::system::error_code& ec) {
            receive_done_ = false;
            if (ec) {
                // One would expect that async_connect wouldn't return
                // EINPROGRESS error code, but simply wait for the connection
                // to get established before the handler is invoked. It turns out,
                // however, that on some OSes the connect handler may receive this
                // error code which doesn't necessarily indicate a problem.
                // Making an attempt to write and read from this socket will
                // typically succeed. So, we ignore this error.
                if (ec.value() != boost::asio::error::in_progress) {
                    ADD_FAILURE() << "error occurred while connecting: "
                                  << ec.message();
                    io_service_->stop();
                    return;
                }
            }
            sendRequest(request);
        });
    }

    /// @brief Send HTTP request.
    ///
    /// @param request HTTP request in the textual format.
    virtual void sendRequest(const std::string& request) {
        sendPartialRequest(request);
    }

    /// @brief Send part of the HTTP request.
    ///
    /// @param request part of the HTTP request to be sent.
    virtual void sendPartialRequest(std::string request) {
        size_t chuck_size = std::min(TEST_HTTP_CHUCK_SIZE, request.size());
        auto ref = shared_from_this();
        socket_.async_send(boost::asio::buffer(request.data(), chuck_size),
                           [this, ref, request](const boost::system::error_code& ec,
                                                std::size_t bytes_transferred) mutable {
            if (ec) {
                if (ec.value() == boost::asio::error::operation_aborted) {
                    return;

                } else if ((ec.value() == boost::asio::error::try_again) ||
                           (ec.value() == boost::asio::error::would_block)) {
                    // If we should try again make sure there is no garbage in the
                    // bytes_transferred.
                    bytes_transferred = 0;

                } else {
                    ADD_FAILURE() << "error occurred while connecting: "
                                  << ec.message();
                    io_service_->stop();
                    return;
                }
            }

            // Remove the part of the request which has been sent.
            request.erase(0, bytes_transferred);

            // Continue sending request data if there are still some data to be
            // sent.
            if (!request.empty()) {
                sendPartialRequest(request);

            } else {
                // Request has been sent. Start receiving response.
                response_.clear();
                receivePartialResponse();
            }
       });
    }

    /// @brief Receive response from the server.
    virtual void receivePartialResponse() {
        auto ref = shared_from_this();
        socket_.async_read_some(boost::asio::buffer(ref->buf_.data(), ref->buf_.size()),
                                [this, ref](const boost::system::error_code& ec,
                                            std::size_t bytes_transferred) {
            if (ec) {
                // IO service stopped so simply return.
                if (ec.value() == boost::asio::error::operation_aborted) {
                    return;

                } else if ((ec.value() == boost::asio::error::try_again) ||
                           (ec.value() == boost::asio::error::would_block)) {
                    // If we should try again, make sure that there is no garbage
                    // in the bytes_transferred.
                    bytes_transferred = 0;

                } else {
                    // Error occurred, bail...
                    ADD_FAILURE() << "error occurred while receiving HTTP"
                        " response from the server: " << ec.message();
                    io_service_->stop();
                }
            }

            if (bytes_transferred > 0) {
                response_.insert(response_.end(), buf_.data(),
                                 buf_.data() + bytes_transferred);
            }

            // Two consecutive new lines end the part of the response we're
            // expecting.
            bool need_data(true);
            if (response_.find("\r\n\r\n", 0) != std::string::npos) {
                // Try to parse the response.
                try {
                    HttpResponse hr;
                    HttpResponseParser parser(hr);
                    parser.initModel();
                    parser.postBuffer(&response_[0], response_.size());
                    parser.poll();
                    if (!parser.needData()) {
                        need_data = false;
                        if (parser.httpParseOk()) {
                            receive_done_ = true;
                        }
                    }
                } catch (const std::exception& ex) {
                    need_data = false;
                    ADD_FAILURE() << "error parsing response: " << ex.what();
                }
            }
            if (!need_data) {
                io_service_->stop();
            } else {
                receivePartialResponse();
            }
        });
    }

    /// @brief Checks if the TCP connection is still open.
    ///
    /// Tests the TCP connection by trying to read from the socket.
    /// Unfortunately expected failure depends on a race between the read
    /// and the other side close so to check if the connection is closed
    /// please use @c isConnectionClosed instead.
    ///
    /// @return true if the TCP connection is open.
    virtual bool isConnectionAlive() {
        // Remember the current non blocking setting.
        const bool non_blocking_orig = socket_.non_blocking();
        // Set the socket to non blocking mode. We're going to test if the socket
        // returns would_block status on the attempt to read from it.
        socket_.non_blocking(true);

        // We need to provide a buffer for a call to read.
        char data[2];
        boost::system::error_code ec;
        boost::asio::read(socket_, boost::asio::buffer(data, sizeof(data)), ec);

        // Revert the original non_blocking flag on the socket.
        socket_.non_blocking(non_blocking_orig);

        // If the connection is alive we'd typically get would_block status code.
        // If there are any data that haven't been read we may also get success
        // status. We're guessing that try_again may also be returned by some
        // implementations in some situations. Any other error code indicates a
        // problem with the connection so we assume that the connection has been
        // closed.
        return (!ec || (ec.value() == boost::asio::error::try_again) ||
                (ec.value() == boost::asio::error::would_block));
    }

    /// @brief Checks if the TCP connection is already closed.
    ///
    /// Tests the TCP connection by trying to read from the socket.
    /// The read can block so this must be used to check if a connection
    /// is alive so to check if the connection is alive please always
    /// use @c isConnectionAlive.
    ///
    /// @return true if the TCP connection is closed.
    virtual bool isConnectionClosed() {
        // Remember the current non blocking setting.
        const bool non_blocking_orig = socket_.non_blocking();
        // Set the socket to blocking mode. We're going to test if the socket
        // returns eof status on the attempt to read from it.
        socket_.non_blocking(false);

        // We need to provide a buffer for a call to read.
        char data[2];
        boost::system::error_code ec;
        boost::asio::read(socket_, boost::asio::buffer(data, sizeof(data)), ec);

        // Revert the original non_blocking flag on the socket.
        socket_.non_blocking(non_blocking_orig);

        // If the connection is closed we'd typically get eof status code.
        return (ec.value() == boost::asio::error::eof);
    }

    /// @brief Close connection.
    virtual void close() {
        socket_.close();
    }

    /// @brief Returns the HTTP response string.
    ///
    /// @return string containing the response.
    virtual std::string getResponse() const {
        return (response_);
    }

    /// @brief Returns true if the receive completed without error.
    ///
    /// @return True if the receive completed successfully, false
    /// otherwise.
    virtual bool receiveDone() const {
        return (receive_done_);
    }

private:

    /// @brief Holds pointer to the IO service.
    isc::asiolink::IOServicePtr io_service_;

    /// @brief A socket used for the connection.
    boost::asio::ip::tcp::socket socket_;

    /// @brief Buffer into which response is written.
    std::array<char, 8192> buf_;

    /// @brief Response in the textual format.
    std::string response_;

    /// @brief IP address of the server.
    std::string server_address_;

    /// @brief IP port of the server.
    uint16_t server_port_;

    /// @brief Set to true when the receive has completed successfully.
    bool receive_done_;
};

/// @brief Pointer to the TestHttpClient.
typedef boost::shared_ptr<TestHttpClient> TestHttpClientPtr;

/// @brief Entity which can connect to the HTTPS server endpoint.
class TestHttpsClient : public BaseTestHttpClient,
                        public boost::enable_shared_from_this<TestHttpsClient> {
public:

    /// @brief Constructor.
    ///
    /// This constructor creates new socket instance. It doesn't connect. Call
    /// connect() to connect to the server.
    ///
    /// @param io_service IO service to be stopped on error.
    /// @param tls_context TLS context.
    /// @param server_address string containing the IP address of the server.
    /// @param port port number of the server.
    TestHttpsClient(const IOServicePtr& io_service, TlsContextPtr tls_context,
                    const std::string& server_address = "127.0.0.1",
                    uint16_t port = 18123)
        : io_service_(io_service), stream_(io_service_->getInternalIOService(),
          tls_context->getContext()), buf_(), response_(),
          server_address_(server_address), server_port_(port),
          receive_done_(false) {
    }

    /// @brief Destructor.
    ///
    /// Closes the underlying socket if it is open.
    virtual ~TestHttpsClient() {
        close();
    }

    /// @brief Send HTTP request specified in textual format.
    ///
    /// @param request HTTP request in the textual format.
    virtual void startRequest(const std::string& request) {
        tcp::endpoint endpoint(make_address(server_address_), server_port_);
        auto ref = shared_from_this();
        stream_.lowest_layer().async_connect(endpoint,
                                             [this, ref, request](const boost::system::error_code& ec) {
            receive_done_ = false;
            if (ec) {
                // One would expect that async_connect wouldn't return
                // EINPROGRESS error code, but simply wait for the connection
                // to get established before the handler is invoked. It turns out,
                // however, that on some OSes the connect handler may receive this
                // error code which doesn't necessarily indicate a problem.
                // Making an attempt to write and read from this socket will
                // typically succeed. So, we ignore this error.
                if (ec.value() != boost::asio::error::in_progress) {
                    ADD_FAILURE() << "error occurred while connecting: "
                                  << ec.message();
                    io_service_->stop();
                    return;
                }
            }
            stream_.async_handshake(roleToImpl(TlsRole::CLIENT),
            [this, request](const boost::system::error_code& ec) {
                if (ec) {
                    ADD_FAILURE() << "error occurred during handshake: "
                                  << ec.message();
                    io_service_->stop();
                    return;
                }
                sendRequest(request);
            });
        });
    }

    /// @brief Send HTTP request.
    ///
    /// @param request HTTP request in the textual format.
    virtual void sendRequest(const std::string& request) {
        sendPartialRequest(request);
    }

    /// @brief Send part of the HTTP request.
    ///
    /// @param request part of the HTTP request to be sent.
    virtual void sendPartialRequest(std::string request) {
        size_t chuck_size = std::min(TEST_HTTP_CHUCK_SIZE, request.size());
        auto ref = shared_from_this();
        boost::asio::async_write(stream_,
                                 boost::asio::buffer(request.data(), chuck_size),
                                 [this, ref, request](const boost::system::error_code& ec,
                                                      std::size_t bytes_transferred) mutable {
            if (ec) {
                if (ec.value() == boost::asio::error::operation_aborted) {
                    return;

                } else if ((ec.value() == boost::asio::error::try_again) ||
                           (ec.value() == boost::asio::error::would_block)) {
                    // If we should try again make sure there is no garbage in the
                    // bytes_transferred.
                    bytes_transferred = 0;

                } else {
                    ADD_FAILURE() << "error occurred while connecting: "
                                  << ec.message();
                    io_service_->stop();
                    return;
                }
            }

            // Remove the part of the request which has been sent.
            request.erase(0, bytes_transferred);

            // Continue sending request data if there are still some data to be
            // sent.
            if (!request.empty()) {
                sendPartialRequest(request);

            } else {
                // Request has been sent. Start receiving response.
                response_.clear();
                receivePartialResponse();
            }
       });
    }

    /// @brief Receive response from the server.
    virtual void receivePartialResponse() {
        auto ref = shared_from_this();
        stream_.async_read_some(boost::asio::buffer(ref->buf_.data(), ref->buf_.size()),
                                [this, ref](const boost::system::error_code& ec,
                                            std::size_t bytes_transferred) {
            if (ec) {
                // IO service stopped so simply return.
                if (ec.value() == boost::asio::error::operation_aborted) {
                    return;

                } else if ((ec.value() == boost::asio::error::try_again) ||
                           (ec.value() == boost::asio::error::would_block)) {
                    // If we should try again, make sure that there is no garbage
                    // in the bytes_transferred.
                    bytes_transferred = 0;

                } else {
                    // Error occurred, bail...
                    ADD_FAILURE() << "error occurred while receiving HTTP"
                        " response from the server: " << ec.message();
                    io_service_->stop();
                }
            }

            if (bytes_transferred > 0) {
                response_.insert(response_.end(), buf_.data(),
                                 buf_.data() + bytes_transferred);
            }

            // Two consecutive new lines end the part of the response we're
            // expecting.
            bool need_data(true);
            if (response_.find("\r\n\r\n", 0) != std::string::npos) {
                // Try to parse the response.
                try {
                    HttpResponse hr;
                    HttpResponseParser parser(hr);
                    parser.initModel();
                    parser.postBuffer(&response_[0], response_.size());
                    parser.poll();
                    if (!parser.needData()) {
                        need_data = false;
                        if (parser.httpParseOk()) {
                            receive_done_ = true;
                        }
                    }
                } catch (const std::exception& ex) {
                    need_data = false;
                    ADD_FAILURE() << "error parsing response: " << ex.what();
                }
            }
            if (!need_data) {
                io_service_->stop();
            } else {
                receivePartialResponse();
            }
        });
    }

    /// @brief Checks if the TCP connection is still open.
    ///
    /// Tests the TCP connection by trying to read from the socket.
    /// Unfortunately expected failure depends on a race between the read
    /// and the other side close so to check if the connection is closed
    /// please use @c isConnectionClosed instead.
    ///
    /// @return true if the TCP connection is open.
    virtual bool isConnectionAlive() {
        // Remember the current non blocking setting.
        const bool non_blocking_orig = stream_.lowest_layer().non_blocking();
        // Set the socket to non blocking mode. We're going to test if the socket
        // returns would_block status on the attempt to read from it.
        stream_.lowest_layer().non_blocking(true);

        // We need to provide a buffer for a call to read.
        char data[2];
        boost::system::error_code ec;
        boost::asio::read(stream_, boost::asio::buffer(data, sizeof(data)), ec);

        // Revert the original non_blocking flag on the socket.
        stream_.lowest_layer().non_blocking(non_blocking_orig);

        // If the connection is alive we'd typically get would_block status code.
        // If there are any data that haven't been read we may also get success
        // status. We're guessing that try_again may also be returned by some
        // implementations in some situations. Any other error code indicates a
        // problem with the connection so we assume that the connection has been
        // closed.
        return (!ec || (ec.value() == boost::asio::error::try_again) ||
                (ec.value() == boost::asio::error::would_block));
    }

    /// @brief Checks if the TCP connection is already closed.
    ///
    /// Tests the TCP connection by trying to read from the socket.
    /// The read can block so this must be used to check if a connection
    /// is alive so to check if the connection is alive please always
    /// use @c isConnectionAlive.
    ///
    /// @return true if the TCP connection is closed.
    virtual bool isConnectionClosed() {
        // Remember the current non blocking setting.
        const bool non_blocking_orig = stream_.lowest_layer().non_blocking();
        // Set the socket to blocking mode. We're going to test if the socket
        // returns eof status on the attempt to read from it.
        stream_.lowest_layer().non_blocking(false);

        // We need to provide a buffer for a call to read.
        char data[2];
        boost::system::error_code ec;
        boost::asio::read(stream_, boost::asio::buffer(data, sizeof(data)), ec);

        // Revert the original non_blocking flag on the socket.
        stream_.lowest_layer().non_blocking(non_blocking_orig);

        // If the connection is closed we'd typically get eof or
        // stream_truncated status code.
        return ((ec.value() == boost::asio::error::eof) ||
                (ec.value() == STREAM_TRUNCATED));
    }

    /// @brief Close connection.
    virtual void close() {
        stream_.lowest_layer().close();
    }

    /// @brief Returns the HTTP response string.
    ///
    /// @return string containing the response.
    virtual std::string getResponse() const {
        return (response_);
    }

    /// @brief Returns true if the receive completed without error.
    ///
    /// @return True if the receive completed successfully, false
    /// otherwise.
    virtual bool receiveDone() const {
        return (receive_done_);
    }

private:

    /// @brief Holds pointer to the IO service.
    isc::asiolink::IOServicePtr io_service_;

    /// @brief A socket used for the connection.
    TlsStreamImpl stream_;

    /// @brief Buffer into which response is written.
    std::array<char, 8192> buf_;

    /// @brief Response in the textual format.
    std::string response_;

    /// @brief IP address of the server.
    std::string server_address_;

    /// @brief IP port of the server.
    uint16_t server_port_;

    /// @brief Set to true when the receive has completed successfully.
    bool receive_done_;
};

/// @brief Pointer to the TestHttpsClient.
typedef boost::shared_ptr<TestHttpsClient> TestHttpsClientPtr;

#endif
