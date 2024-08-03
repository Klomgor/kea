// Copyright (C) 2017-2024 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>
#include <asiolink/asio_wrapper.h>
#include <asiolink/interval_timer.h>
#include <asiolink/tls_acceptor.h>
#include <cc/data.h>
#include <http/testutils/test_http_client.h>
#include <http/client.h>
#include <http/http_types.h>
#include <http/listener.h>
#include <http/listener_impl.h>
#include <http/post_request_json.h>
#include <http/response_creator.h>
#include <http/response_creator_factory.h>
#include <http/response_json.h>
#include <http/tests/response_test.h>
#include <http/url.h>
#include <util/multi_threading_mgr.h>

#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/pointer_cast.hpp>
#include <gtest/gtest.h>

#include <functional>
#include <list>
#include <sstream>
#include <string>

// Keep order of these headers.
#include <http/tests/http_tests.h>
#include <http/tests/http_response_creator_test.h>

using namespace boost::asio::ip;
using namespace isc::asiolink;
using namespace isc::data;
using namespace isc::http;
using namespace isc::http::test;
using namespace isc::util;
namespace ph = std::placeholders;

namespace {

/// @brief Test fixture class for @ref HttpListener.
class HttpListenerTest : public ::testing::Test {
public:

    /// @brief Constructor.
    ///
    /// Starts test timer which detects timeouts.
    HttpListenerTest()
        : io_service_(new IOService()), factory_(new TestHttpResponseCreatorFactory()),
          test_timer_(io_service_), run_io_service_timer_(io_service_) {
        test_timer_.setup(std::bind(&HttpListenerTest::timeoutHandler, this, true),
                          TEST_TIMEOUT, IntervalTimer::ONE_SHOT);
    }

    /// @brief Destructor.
    ///
    /// Removes active HTTP clients.
    virtual ~HttpListenerTest() {
        test_timer_.cancel();
        io_service_->stopAndPoll();
    }

    /// @brief Callback function invoke upon test timeout.
    ///
    /// It stops the IO service and reports test timeout.
    ///
    /// @param fail_on_timeout Specifies if test failure should be reported.
    void timeoutHandler(const bool fail_on_timeout) {
        if (fail_on_timeout) {
            ADD_FAILURE() << "Timeout occurred while running the test!";
        }
        io_service_->stop();
    }

    /// @brief Runs IO service with optional timeout.
    ///
    /// @param timeout Optional value specifying for how long the io service
    /// should be ran.
    void runIOService(long timeout = 0) {
        io_service_->stop();
        io_service_->restart();

        if (timeout > 0) {
            run_io_service_timer_.setup(std::bind(&HttpListenerTest::timeoutHandler,
                                                  this, false),
                                        timeout, IntervalTimer::ONE_SHOT);
        }
        io_service_->run();
        io_service_->stopAndPoll(false);
    }

    /// @brief IO service used in the tests.
    IOServicePtr io_service_;

    /// @brief Pointer to the response creator factory.
    HttpResponseCreatorFactoryPtr factory_;

    /// @brief Asynchronous timer service to detect timeouts.
    IntervalTimer test_timer_;

    /// @brief Asynchronous timer for running IO service for a specified amount
    /// of time.
    IntervalTimer run_io_service_timer_;
};

/// @brief Test fixture class for testing HTTP client.
class HttpClientTest : public HttpListenerTest {
public:

    /// @brief Constructor.
    HttpClientTest()
        : HttpListenerTest(),
          listener_(io_service_, IOAddress(SERVER_ADDRESS), SERVER_PORT,
                    TlsContextPtr(), factory_,
                    HttpListener::RequestTimeout(REQUEST_TIMEOUT),
                    HttpListener::IdleTimeout(IDLE_TIMEOUT)),
          listener2_(io_service_, IOAddress(IPV6_SERVER_ADDRESS), SERVER_PORT + 1,
                     TlsContextPtr(), factory_,
                     HttpListener::RequestTimeout(REQUEST_TIMEOUT),
                     HttpListener::IdleTimeout(IDLE_TIMEOUT)),
          listener3_(io_service_, IOAddress(SERVER_ADDRESS), SERVER_PORT + 2,
                     TlsContextPtr(), factory_,
                     HttpListener::RequestTimeout(REQUEST_TIMEOUT),
                     HttpListener::IdleTimeout(SHORT_IDLE_TIMEOUT)) {
        MultiThreadingMgr::instance().setMode(false);
    }

    /// @brief Destructor.
    ~HttpClientTest() {
        listener_.stop();
        listener2_.stop();
        listener3_.stop();
        io_service_->stopAndPoll();
        MultiThreadingMgr::instance().setMode(false);
    }

    /// @brief Creates HTTP request with JSON body.
    ///
    /// It includes a JSON parameter with a specified value.
    ///
    /// @param parameter_name JSON parameter to be included.
    /// @param value JSON parameter value.
    /// @param version HTTP version to be used. Default is HTTP/1.1.
    template<typename ValueType>
    PostHttpRequestJsonPtr createRequest(const std::string& parameter_name,
                                         const ValueType& value,
                                         const HttpVersion& version = HttpVersion(1, 1)) {
        // Create POST request with JSON body.
        PostHttpRequestJsonPtr request(new PostHttpRequestJson(HttpRequest::Method::HTTP_POST,
                                                               "/", version));
        // Body is a map with a specified parameter included.
        ElementPtr body = Element::createMap();
        body->set(parameter_name, Element::create(value));
        request->setBodyAsJson(body);
        try {
            request->finalize();

        } catch (const std::exception& ex) {
            ADD_FAILURE() << "failed to create request: " << ex.what();
        }

        return (request);
    }

    /// @brief Test that two consecutive requests can be sent over the same
    /// connection (if persistent, if not persistent two connections will
    /// be used).
    ///
    /// @param version HTTP version to be used.
    void testConsecutiveRequests(const HttpVersion& version) {
        // Start the server.
        ASSERT_NO_THROW(listener_.start());

        // Create a client and specify the URL on which the server can be reached.
        HttpClient client(io_service_, false);
        Url url("http://127.0.0.1:18123");

        // Initiate request to the server.
        PostHttpRequestJsonPtr request1 = createRequest("sequence", 1, version);
        HttpResponseJsonPtr response1(new HttpResponseJson());
        unsigned resp_num = 0;
        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request1, response1,
            [this, &resp_num](const boost::system::error_code& ec,
                              const HttpResponsePtr&,
                              const std::string&) {
            if (++resp_num > 1) {
                io_service_->stop();
            }
            EXPECT_FALSE(ec);
        }));

        // Initiate another request to the destination.
        PostHttpRequestJsonPtr request2 = createRequest("sequence", 2, version);
        HttpResponseJsonPtr response2(new HttpResponseJson());
        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request2, response2,
            [this, &resp_num](const boost::system::error_code& ec,
                              const HttpResponsePtr&,
                              const std::string&) {
            if (++resp_num > 1) {
                io_service_->stop();
            }
            EXPECT_FALSE(ec);
        }));

        // Actually trigger the requests. The requests should be handlded by the
        // server one after another. While the first request is being processed
        // the server should queue another one.
        ASSERT_NO_THROW(runIOService());

        // Make sure that the received responses are different. We check that by
        // comparing value of the sequence parameters.
        ASSERT_TRUE(response1);
        ConstElementPtr sequence1 = response1->getJsonElement("sequence");
        ASSERT_TRUE(sequence1);

        ASSERT_TRUE(response2);
        ConstElementPtr sequence2 = response2->getJsonElement("sequence");
        ASSERT_TRUE(sequence2);

        EXPECT_NE(sequence1->intValue(), sequence2->intValue());
    }

    /// @brief Test that the client can communicate with two different
    /// destinations simultaneously.
    void testMultipleDestinations() {
        // Start two servers running on different ports.
        ASSERT_NO_THROW(listener_.start());
        ASSERT_NO_THROW(listener2_.start());

        // Create the client. It will be communicating with the two servers.
        HttpClient client(io_service_, false);

        // Specify the URLs on which the servers are available.
        Url url1("http://127.0.0.1:18123");
        Url url2("http://[::1]:18124");

        // Create a request to the first server.
        PostHttpRequestJsonPtr request1 = createRequest("sequence", 1);
        HttpResponseJsonPtr response1(new HttpResponseJson());
        unsigned resp_num = 0;
        ASSERT_NO_THROW(client.asyncSendRequest(url1, TlsContextPtr(),
                                                request1, response1,
            [this, &resp_num](const boost::system::error_code& ec,
                              const HttpResponsePtr&,
                              const std::string&) {
            if (++resp_num > 1) {
                io_service_->stop();
            }
            EXPECT_FALSE(ec);
        }));

        // Create a request to the second server.
        PostHttpRequestJsonPtr request2 = createRequest("sequence", 2);
        HttpResponseJsonPtr response2(new HttpResponseJson());
        ASSERT_NO_THROW(client.asyncSendRequest(url2, TlsContextPtr(),
                                                request2, response2,
            [this, &resp_num](const boost::system::error_code& ec,
                              const HttpResponsePtr&,
                              const std::string&) {
            if (++resp_num > 1) {
                io_service_->stop();
            }
            EXPECT_FALSE(ec);
        }));

        // Actually trigger the requests.
        ASSERT_NO_THROW(runIOService());

        // Make sure we have received two different responses.
        ASSERT_TRUE(response1);
        ConstElementPtr sequence1 = response1->getJsonElement("sequence");
        ASSERT_TRUE(sequence1);

        ASSERT_TRUE(response2);
        ConstElementPtr sequence2 = response2->getJsonElement("sequence");
        ASSERT_TRUE(sequence2);

        EXPECT_NE(sequence1->intValue(), sequence2->intValue());
    }

    /// @brief Test that the client can communicate with the same destination
    /// address and port but with different TLS contexts so
    void testMultipleTlsContexts() {
        // Start only one server.
        ASSERT_NO_THROW(listener_.start());

        // Create the client.
        HttpClient client(io_service_, false);

        // Specify the URL on which the server is available.
        Url url("http://127.0.0.1:18123");

        // Create a request to the first server.
        PostHttpRequestJsonPtr request1 = createRequest("sequence", 1);
        HttpResponseJsonPtr response1(new HttpResponseJson());
        unsigned resp_num = 0;
        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request1, response1,
            [this, &resp_num](const boost::system::error_code& ec,
                              const HttpResponsePtr&,
                              const std::string&) {
            if (++resp_num > 1) {
                io_service_->stop();
            }
            if (ec) {
                ADD_FAILURE() << "asyncSendRequest failed: " << ec.message();
            }
        }));

        // Create a request with the second TLS context.
        PostHttpRequestJsonPtr request2 = createRequest("sequence", 2);
        HttpResponseJsonPtr response2(new HttpResponseJson());
        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request2, response2,
            [this, &resp_num](const boost::system::error_code& ec,
                              const HttpResponsePtr&,
                              const std::string&) {
            if (++resp_num > 1) {
                io_service_->stop();
            }
            if (ec) {
                ADD_FAILURE() << "asyncSendRequest failed: " << ec.message();
            }
        }));

        // Actually trigger the requests.
        ASSERT_NO_THROW(runIOService());

        // Make sure we have received two different responses.
        ASSERT_TRUE(response1);
        ConstElementPtr sequence1 = response1->getJsonElement("sequence");
        ASSERT_TRUE(sequence1);

        ASSERT_TRUE(response2);
        ConstElementPtr sequence2 = response2->getJsonElement("sequence");
        ASSERT_TRUE(sequence2);

        EXPECT_NE(sequence1->intValue(), sequence2->intValue());
    }

    /// @brief Test that idle connection can be resumed for second request.
    void testIdleConnection() {
        // Start the server that has short idle timeout. It closes the idle
        // connection after 200ms.
        ASSERT_NO_THROW(listener3_.start());

        // Create the client that will communicate with this server.
        HttpClient client(io_service_, false);

        // Specify the URL of this server.
        Url url("http://127.0.0.1:18125");

        // Create the first request.
        PostHttpRequestJsonPtr request1 = createRequest("sequence", 1);
        HttpResponseJsonPtr response1(new HttpResponseJson());
        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request1, response1,
            [this](const boost::system::error_code& ec, const HttpResponsePtr&,
                   const std::string&) {
            io_service_->stop();
            EXPECT_FALSE(ec);
        }));

        // Run the IO service until the response is received.
        ASSERT_NO_THROW(runIOService());

        // Make sure the response has been received.
        ASSERT_TRUE(response1);
        ConstElementPtr sequence1 = response1->getJsonElement("sequence");
        ASSERT_TRUE(sequence1);

        // Delay the generation of the second request by 2x server idle timeout.
        // This should be enough to cause the server to close the connection.
        ASSERT_NO_THROW(runIOService(SHORT_IDLE_TIMEOUT * 2));

        // Create another request.
        PostHttpRequestJsonPtr request2 = createRequest("sequence", 2);
        HttpResponseJsonPtr response2(new HttpResponseJson());
        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request2, response2,
            [this](const boost::system::error_code& ec, const HttpResponsePtr&,
                   const std::string&) {
            io_service_->stop();
            EXPECT_FALSE(ec);
        }));

        // Actually trigger the second request.
        ASSERT_NO_THROW(runIOService());

        // Make sire that the server has responded.
        ASSERT_TRUE(response2);
        ConstElementPtr sequence2 = response2->getJsonElement("sequence");
        ASSERT_TRUE(sequence2);

        // Make sure that two different responses have been received.
        EXPECT_NE(sequence1->intValue(), sequence2->intValue());
    }

    /// @brief This test verifies that the client returns IO error code when the
    /// server is unreachable.
    void testUnreachable () {
        // Create the client.
        HttpClient client(io_service_, false);

        // Specify the URL of the server. This server is down.
        Url url("http://127.0.0.1:18123");

        // Create the request.
        PostHttpRequestJsonPtr request = createRequest("sequence", 1);
        HttpResponseJsonPtr response(new HttpResponseJson());
        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request, response,
            [this](const boost::system::error_code& ec,
                   const HttpResponsePtr&,
                   const std::string&) {
            io_service_->stop();
            // The server should have returned an IO error.
            EXPECT_TRUE(ec);
        }));

        // Actually trigger the request.
        ASSERT_NO_THROW(runIOService());
    }

    /// @brief Test that an error is returned by the client if the server
    /// response is malformed.
    void testMalformedResponse () {
        // Start the server.
        ASSERT_NO_THROW(listener_.start());

        // Create the client.
        HttpClient client(io_service_, false);

        // Specify the URL of the server.
        Url url("http://127.0.0.1:18123");

        // The response is going to be malformed in such a way that it holds
        // an invalid content type. We affect the content type by creating
        // a request that holds a JSON parameter requesting a specific
        // content type.
        PostHttpRequestJsonPtr request = createRequest("requested-content-type",
                                                       "text/html");
        HttpResponseJsonPtr response(new HttpResponseJson());
        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request, response,
            [this](const boost::system::error_code& ec,
                   const HttpResponsePtr& response,
                   const std::string& parsing_error) {
            io_service_->stop();
            // There should be no IO error (answer from the server is received).
            EXPECT_FALSE(ec);
            // The response object is NULL because it couldn't be finalized.
            EXPECT_FALSE(response);
            // The message parsing error should be returned.
            EXPECT_FALSE(parsing_error.empty());
        }));

        // Actually trigger the request.
        ASSERT_NO_THROW(runIOService());
    }

    /// @brief Test that client times out when it doesn't receive the entire
    /// response from the server within a desired time.
    void testClientRequestTimeout() {
        // Start the server.
        ASSERT_NO_THROW(listener_.start());

        // Create the client.
        HttpClient client(io_service_, false);

        // Specify the URL of the server.
        Url url("http://127.0.0.1:18123");

        unsigned cb_num = 0;

        // Create the request which asks the server to generate a partial
        // (although well formed) response. The client will be waiting for the
        // rest of the response to be provided and will eventually time out.
        PostHttpRequestJsonPtr request1 = createRequest("partial-response", true);
        HttpResponseJsonPtr response1(new HttpResponseJson());
        // This value will be set to true if the connection close callback is
        // invoked upon time out.
        auto connection_closed = false;
        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request1, response1,
            [this, &cb_num](const boost::system::error_code& ec,
                            const HttpResponsePtr& response,
                            const std::string&) {
                if (++cb_num > 1) {
                    io_service_->stop();
                }
                // In this particular case we know exactly the type of the
                // IO error returned, because the client explicitly sets this
                // error code.
                EXPECT_TRUE(ec.value() == boost::asio::error::timed_out);
                // There should be no response returned.
                EXPECT_FALSE(response);
            },
            HttpClient::RequestTimeout(100),
            HttpClient::ConnectHandler(),
            HttpClient::HandshakeHandler(),
            [&connection_closed](const int) {
                // This callback is called when the connection gets closed
                // by the client.
                connection_closed = true;
            })
        );

        // Create another request after the timeout. It should be handled ok.
        PostHttpRequestJsonPtr request2 = createRequest("sequence", 1);
        HttpResponseJsonPtr response2(new HttpResponseJson());
        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request2, response2,
            [this, &cb_num](const boost::system::error_code& /*ec*/,
                            const HttpResponsePtr&,
                            const std::string&) {
                if (++cb_num > 1) {
                    io_service_->stop();
                }
            }));

        // Actually trigger the requests.
        ASSERT_NO_THROW(runIOService());
        // Make sure that the client has closed the connection upon timeout.
        EXPECT_TRUE(connection_closed);
    }

    /// @brief Test that client times out when connection takes too long.
    void testClientConnectTimeout() {
        // Start the server.
        ASSERT_NO_THROW(listener_.start());

        // Create the client.
        HttpClient client(io_service_, false);

        // Specify the URL of the server.
        Url url("http://127.0.0.1:18123");

        unsigned cb_num = 0;

        PostHttpRequestJsonPtr request = createRequest("sequence", 1);
        HttpResponseJsonPtr response(new HttpResponseJson());
        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request, response,
            [this, &cb_num](const boost::system::error_code& ec,
                            const HttpResponsePtr& response,
                            const std::string&) {
            if (++cb_num > 1) {
                io_service_->stop();
            }
            // In this particular case we know exactly the type of the
            // IO error returned, because the client explicitly sets this
            // error code.
            EXPECT_TRUE(ec.value() == boost::asio::error::timed_out);
            // There should be no response returned.
            EXPECT_FALSE(response);

        }, HttpClient::RequestTimeout(100),

           // This callback is invoked upon an attempt to connect to the
           // server. The false value indicates to the HttpClient to not
           // try to send a request to the server. This simulates the
           // case of connect() taking very long and should eventually
           // cause the transaction to time out.
           [](const boost::system::error_code& /*ec*/, int) {
               return (false);
        }));

        // Create another request after the timeout. It should be handled ok.
        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request, response,
            [this, &cb_num](const boost::system::error_code& /*ec*/,
                            const HttpResponsePtr&,
                            const std::string&) {
                if (++cb_num > 1) {
                    io_service_->stop();
                }
            }));

        // Actually trigger the requests.
        ASSERT_NO_THROW(runIOService());
    }

    /// @brief Tests the behavior of the HTTP client when the premature
    /// timeout occurs.
    ///
    /// The premature timeout may occur when the system clock is moved
    /// during the transaction. This test simulates this behavior by
    /// starting new transaction and waiting for the timeout to occur
    /// before the IO service is ran. The timeout handler is invoked
    /// first and it resets the transaction state. This test verifies
    /// that the started transaction tears down gracefully after the
    /// transaction state is reset.
    ///
    /// There are two variants of this test. The first variant schedules
    /// one transaction before running the IO service. The second variant
    /// schedules two transactions prior to running the IO service. The
    /// second transaction is queued, so it is expected that it doesn't
    /// time out and it runs successfully.
    ///
    /// @param queue_two_requests Boolean value indicating if a single
    /// transaction should be queued (false), or two (true).
    void testClientRequestLateStart(const bool queue_two_requests) {
        // Start the server.
        ASSERT_NO_THROW(listener_.start());

        // Create the client.
        HttpClient client(io_service_, false);

        // Specify the URL of the server.
        Url url("http://127.0.0.1:18123");

        // Specify the TLS context of the server.
        TlsContextPtr tls_context;

        // Generate first request.
        PostHttpRequestJsonPtr request1 = createRequest("sequence", 1);
        HttpResponseJsonPtr response1(new HttpResponseJson());

        // Use very short timeout to make sure that it occurs before we actually
        // run the transaction.
        ASSERT_NO_THROW(client.asyncSendRequest(url, tls_context,
                                                request1, response1,
            [](const boost::system::error_code& ec,
               const HttpResponsePtr& response,
               const std::string&) {

            // In this particular case we know exactly the type of the
            // IO error returned, because the client explicitly sets this
            // error code.
            EXPECT_TRUE(ec.value() == boost::asio::error::timed_out);
            // There should be no response returned.
            EXPECT_FALSE(response);
        }, HttpClient::RequestTimeout(1)));

        if (queue_two_requests) {
            PostHttpRequestJsonPtr request2 = createRequest("sequence", 2);
            HttpResponseJsonPtr response2(new HttpResponseJson());
            ASSERT_NO_THROW(client.asyncSendRequest(url, tls_context,
                                                    request2, response2,
                [](const boost::system::error_code& ec,
                   const HttpResponsePtr& response,
                   const std::string&) {

                // This second request should be successful.
                EXPECT_TRUE(ec.value() == 0);
                EXPECT_TRUE(response);
            }));
        }

        // This waits for 3ms to make sure that the timeout occurs before we
        // run the transaction. This leads to an unusual situation that the
        // transaction state is reset as a result of the timeout but the
        // transaction is alive. We want to make sure that the client can
        // gracefully deal with this situation.
        usleep(3000);

        // Run the transaction and hope it will gracefully tear down.
        ASSERT_NO_THROW(runIOService(100));

        // Now try to send another request to make sure that the client
        // is healthy.
        PostHttpRequestJsonPtr request3 = createRequest("sequence", 3);
        HttpResponseJsonPtr response3(new HttpResponseJson());
        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request3, response3,
                         [this](const boost::system::error_code& ec,
                                const HttpResponsePtr&,
                                const std::string&) {
            io_service_->stop();

            // Everything should be ok.
            EXPECT_TRUE(ec.value() == 0);
        }));

        // Actually trigger the requests.
        ASSERT_NO_THROW(runIOService());
    }

    /// @brief Tests that underlying TCP socket can be registered and
    /// unregistered via connection and close callbacks.
    ///
    /// It conducts to consecutive requests over the same client.
    ///
    /// @param version HTTP version to be used.
    void testConnectCloseCallbacks(const HttpVersion& version) {
        // Start the server.
        ASSERT_NO_THROW(listener_.start());

        // Create a client and specify the URL on which the server can be reached.
        HttpClient client(io_service_, false);
        Url url("http://127.0.0.1:18123");

        // Initiate request to the server.
        PostHttpRequestJsonPtr request1 = createRequest("sequence", 1, version);
        HttpResponseJsonPtr response1(new HttpResponseJson());
        unsigned resp_num = 0;
        ExternalMonitor monitor;

        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request1, response1,
            [this, &resp_num](const boost::system::error_code& ec,
                              const HttpResponsePtr&,
                              const std::string&) {
            if (++resp_num > 1) {
                io_service_->stop();
            }

            EXPECT_FALSE(ec);
        },
            HttpClient::RequestTimeout(10000),
            std::bind(&ExternalMonitor::connectHandler, &monitor, ph::_1, ph::_2),
            std::bind(&ExternalMonitor::handshakeHandler, &monitor, ph::_1, ph::_2),
            std::bind(&ExternalMonitor::closeHandler, &monitor, ph::_1)
        ));

        // Initiate another request to the destination.
        PostHttpRequestJsonPtr request2 = createRequest("sequence", 2, version);
        HttpResponseJsonPtr response2(new HttpResponseJson());
        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request2, response2,
            [this, &resp_num](const boost::system::error_code& ec,
                              const HttpResponsePtr&,
                              const std::string&) {
            if (++resp_num > 1) {
                io_service_->stop();
            }
            EXPECT_FALSE(ec);
        },
            HttpClient::RequestTimeout(10000),
            std::bind(&ExternalMonitor::connectHandler, &monitor, ph::_1, ph::_2),
            std::bind(&ExternalMonitor::handshakeHandler, &monitor, ph::_1, ph::_2),
            std::bind(&ExternalMonitor::closeHandler, &monitor, ph::_1)
        ));

        // Actually trigger the requests. The requests should be handlded by the
        // server one after another. While the first request is being processed
        // the server should queue another one.
        ASSERT_NO_THROW(runIOService());

        // We should have had 2 connect invocations, no closes
        // and a valid registered fd
        EXPECT_EQ(2, monitor.connect_cnt_);
        EXPECT_EQ(0, monitor.close_cnt_);
        EXPECT_GT(monitor.registered_fd_, -1);

        // Make sure that the received responses are different. We check that by
        // comparing value of the sequence parameters.
        ASSERT_TRUE(response1);
        ConstElementPtr sequence1 = response1->getJsonElement("sequence");
        ASSERT_TRUE(sequence1);

        ASSERT_TRUE(response2);
        ConstElementPtr sequence2 = response2->getJsonElement("sequence");
        ASSERT_TRUE(sequence2);
        EXPECT_NE(sequence1->intValue(), sequence2->intValue());

        // Stopping the client the close the connection.
        client.stop();

        // We should have had 2 connect invocations, 1 closes
        // and an invalid registered fd
        EXPECT_EQ(2, monitor.connect_cnt_);
        EXPECT_EQ(1, monitor.close_cnt_);
        EXPECT_EQ(-1, monitor.registered_fd_);
    }

    /// @brief Tests detection and handling out-of-band socket events
    ///
    /// It initiates a transaction and verifies that a mid-transaction call
    /// to HttpClient::closeIfOutOfBand() has no affect on the connection.
    /// After successful completion of the transaction, a second call to
    /// HttpClient::closeIfOutOfBand() is made. This should result in the
    /// connection being closed.
    /// This step is repeated to verify that after an OOB closure, transactions
    /// to the same destination can be processed.
    ///
    /// Lastly, we verify that HttpClient::stop() closes the connection correctly.
    ///
    /// @param version HTTP version to be used.
    void testCloseIfOutOfBand(const HttpVersion& version) {
        // Start the server.
        ASSERT_NO_THROW(listener_.start());

        // Create a client and specify the URL on which the server can be reached.
        HttpClient client(io_service_, false);
        Url url("http://127.0.0.1:18123");

        // Initiate request to the server.
        PostHttpRequestJsonPtr request1 = createRequest("sequence", 1, version);
        HttpResponseJsonPtr response1(new HttpResponseJson());
        unsigned resp_num = 0;
        ExternalMonitor monitor;

        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request1, response1,
            [this, &client, &resp_num, &monitor](const boost::system::error_code& ec,
                              const HttpResponsePtr&,
                              const std::string&) {
            if (++resp_num == 1) {
                io_service_->stop();
            }

            EXPECT_EQ(1, monitor.connect_cnt_);      // We should have 1 connect.
            EXPECT_EQ(0, monitor.close_cnt_);        // We should have 0 closes
            ASSERT_GT(monitor.registered_fd_, -1);   // We should have a valid fd.
            int orig_fd = monitor.registered_fd_;

            // Test our socket for OOBness.
            client.closeIfOutOfBand(monitor.registered_fd_);

            // Since we're in a transaction, we should have no closes and
            // the same valid fd.
            EXPECT_EQ(0, monitor.close_cnt_);
            ASSERT_EQ(monitor.registered_fd_, orig_fd);

            EXPECT_FALSE(ec);
        },
            HttpClient::RequestTimeout(10000),
            std::bind(&ExternalMonitor::connectHandler, &monitor, ph::_1, ph::_2),
            std::bind(&ExternalMonitor::handshakeHandler, &monitor, ph::_1, ph::_2),
            std::bind(&ExternalMonitor::closeHandler, &monitor, ph::_1)
        ));

        // Actually trigger the requests. The requests should be handlded by the
        // server one after another. While the first request is being processed
        // the server should queue another one.
        ASSERT_NO_THROW(runIOService());

        // Make sure that we received a response.
        ASSERT_TRUE(response1);
        ConstElementPtr sequence1 = response1->getJsonElement("sequence");
        ASSERT_TRUE(sequence1);
        EXPECT_EQ(1, sequence1->intValue());

        // We should have had 1 connect invocations, no closes
        // and a valid registered fd
        EXPECT_EQ(1, monitor.connect_cnt_);
        EXPECT_EQ(0, monitor.close_cnt_);
        EXPECT_GT(monitor.registered_fd_, -1);

        // Test our socket for OOBness.
        client.closeIfOutOfBand(monitor.registered_fd_);

        // Since we're in a transaction, we should have no closes and
        // the same valid fd.
        EXPECT_EQ(1, monitor.close_cnt_);
        EXPECT_EQ(-1, monitor.registered_fd_);

        // Now let's do another request to the destination to verify that
        // we'll reopen the connection without issue.
        PostHttpRequestJsonPtr request2 = createRequest("sequence", 2, version);
        HttpResponseJsonPtr response2(new HttpResponseJson());
        resp_num = 0;
        ASSERT_NO_THROW(client.asyncSendRequest(url, TlsContextPtr(),
                                                request2, response2,
            [this, &client, &resp_num, &monitor](const boost::system::error_code& ec,
                              const HttpResponsePtr&,
                              const std::string&) {
            if (++resp_num == 1) {
                io_service_->stop();
            }

            EXPECT_EQ(2, monitor.connect_cnt_);      // We should have 1 connect.
            EXPECT_EQ(1, monitor.close_cnt_);        // We should have 0 closes
            ASSERT_GT(monitor.registered_fd_, -1);   // We should have a valid fd.
            int orig_fd = monitor.registered_fd_;

            // Test our socket for OOBness.
            client.closeIfOutOfBand(monitor.registered_fd_);

            // Since we're in a transaction, we should have no closes and
            // the same valid fd.
            EXPECT_EQ(1, monitor.close_cnt_);
            ASSERT_EQ(monitor.registered_fd_, orig_fd);

            EXPECT_FALSE(ec);
        },
            HttpClient::RequestTimeout(10000),
            std::bind(&ExternalMonitor::connectHandler, &monitor, ph::_1, ph::_2),
            std::bind(&ExternalMonitor::handshakeHandler, &monitor, ph::_1, ph::_2),
            std::bind(&ExternalMonitor::closeHandler, &monitor, ph::_1)
        ));

        // Actually trigger the requests. The requests should be handlded by the
        // server one after another. While the first request is being processed
        // the server should queue another one.
        ASSERT_NO_THROW(runIOService());

        // Make sure that we received the second response.
        ASSERT_TRUE(response2);
        ConstElementPtr sequence2 = response2->getJsonElement("sequence");
        ASSERT_TRUE(sequence2);
        EXPECT_EQ(2, sequence2->intValue());

        // Stopping the client the close the connection.
        client.stop();

        // We should have had 2 connect invocations, 2 closes
        // and an invalid registered fd
        EXPECT_EQ(2, monitor.connect_cnt_);
        EXPECT_EQ(2, monitor.close_cnt_);
        EXPECT_EQ(-1, monitor.registered_fd_);
    }

    /// @brief Simulates external registery of Connection TCP sockets
    ///
    /// Provides methods compatible with Connection callbacks for connect
    /// and close operations.
    class ExternalMonitor {
    public:
        /// @brief Constructor
        ExternalMonitor()
            : registered_fd_(-1), connect_cnt_(0), close_cnt_(0) {
        }

        /// @brief Connect callback handler
        /// @param ec Error status of the ASIO connect
        /// @param tcp_native_fd socket descriptor to register
        bool connectHandler(const boost::system::error_code& ec, int tcp_native_fd) {
            ++connect_cnt_;
            if ((!ec || (ec.value() == boost::asio::error::in_progress))
                && (tcp_native_fd >= 0)) {
                registered_fd_ = tcp_native_fd;
                return (true);
            } else if ((ec.value() == boost::asio::error::already_connected)
                       && (registered_fd_ != tcp_native_fd)) {
                return (false);
            }

            // ec indicates an error, return true, so that error can be handled
            // by Connection logic.
            return (true);
        }

        /// @brief Handshake callback handler
        /// @param ec Error status of the ASIO connect
        bool handshakeHandler(const boost::system::error_code&, int) {
            ADD_FAILURE() << "handshake callback handler is called";
            // ec indicates an error, return true, so that error can be handled
            // by Connection logic.
            return (true);
        }

        /// @brief Close callback handler
        ///
        /// @param tcp_native_fd socket descriptor to register
        void closeHandler(int tcp_native_fd) {
            ++close_cnt_;
            EXPECT_EQ(tcp_native_fd, registered_fd_) << "closeHandler fd mismatch";
            if (tcp_native_fd >= 0) {
                registered_fd_ = -1;
            }
        }

        /// @brief Keeps track of socket currently "registered" for external monitoring.
        int registered_fd_;

        /// @brief Tracks how many times the connect callback is invoked.
        int connect_cnt_;

        /// @brief Tracks how many times the close callback is invoked.
        int close_cnt_;
    };

    /// @brief Instance of the listener used in the tests.
    HttpListener listener_;

    /// @brief Instance of the second listener used in the tests.
    HttpListener listener2_;

    /// @brief Instance of the third listener used in the tests (with short idle
    /// timeout).
    HttpListener listener3_;

};

// Test that two consecutive requests can be sent over the same (persistent)
// connection.
TEST_F(HttpClientTest, consecutiveRequests) {
    ASSERT_NO_FATAL_FAILURE(testConsecutiveRequests(HttpVersion(1, 1)));
}

// Test that two consecutive requests can be sent over the same (persistent)
// connection.
TEST_F(HttpClientTest, consecutiveRequestsMultiThreading) {
    MultiThreadingMgr::instance().setMode(true);
    ASSERT_NO_FATAL_FAILURE(testConsecutiveRequests(HttpVersion(1, 1)));
}

// Test that two consecutive requests can be sent over non-persistent connection.
// This is achieved by sending HTTP/1.0 requests, which are non-persistent by
// default. The client should close the connection right after receiving a response
// from the server.
TEST_F(HttpClientTest, closeBetweenRequests) {
    ASSERT_NO_FATAL_FAILURE(testConsecutiveRequests(HttpVersion(1, 0)));
}

// Test that two consecutive requests can be sent over non-persistent connection.
// This is achieved by sending HTTP/1.0 requests, which are non-persistent by
// default. The client should close the connection right after receiving a response
// from the server.
TEST_F(HttpClientTest, closeBetweenRequestsMultiThreading) {
    MultiThreadingMgr::instance().setMode(true);
    ASSERT_NO_FATAL_FAILURE(testConsecutiveRequests(HttpVersion(1, 0)));
}

// Test that the client can communicate with two different destinations
// simultaneously.
TEST_F(HttpClientTest, multipleDestinations) {
    ASSERT_NO_FATAL_FAILURE(testMultipleDestinations());
}

// Test that the client can communicate with two different destinations
// simultaneously.
TEST_F(HttpClientTest, multipleDestinationsMultiThreading) {
    MultiThreadingMgr::instance().setMode(true);
    ASSERT_NO_FATAL_FAILURE(testMultipleDestinations());
}

// Test that the client can use two different TLS contexts to the same
// destination address and port simultaneously.
TEST_F(HttpClientTest, multipleTlsContexts) {
    ASSERT_NO_FATAL_FAILURE(testMultipleTlsContexts());
}

// Test that the client can use two different TLS contexts to the same
// destination address and port simultaneously.
TEST_F(HttpClientTest, multipleTlsContextsMultiThreading) {
    MultiThreadingMgr::instance().setMode(true);
    ASSERT_NO_FATAL_FAILURE(testMultipleTlsContexts());
}

// Test that idle connection can be resumed for second request.
TEST_F(HttpClientTest, idleConnection) {
    ASSERT_NO_FATAL_FAILURE(testIdleConnection());
}

// Test that idle connection can be resumed for second request.
TEST_F(HttpClientTest, idleConnectionMultiThreading) {
    MultiThreadingMgr::instance().setMode(true);
    ASSERT_NO_FATAL_FAILURE(testIdleConnection());
}

// This test verifies that the client returns IO error code when the
// server is unreachable.
TEST_F(HttpClientTest, unreachable) {
    ASSERT_NO_FATAL_FAILURE(testUnreachable());
}

// This test verifies that the client returns IO error code when the
// server is unreachable.
TEST_F(HttpClientTest, unreachableMultiThreading) {
    MultiThreadingMgr::instance().setMode(true);
    ASSERT_NO_FATAL_FAILURE(testUnreachable());
}

// Test that an error is returned by the client if the server response is
// malformed.
TEST_F(HttpClientTest, malformedResponse) {
    ASSERT_NO_FATAL_FAILURE(testMalformedResponse());
}

// Test that an error is returned by the client if the server response is
// malformed.
TEST_F(HttpClientTest, malformedResponseMultiThreading) {
    MultiThreadingMgr::instance().setMode(true);
    ASSERT_NO_FATAL_FAILURE(testMalformedResponse());
}

// Test that client times out when it doesn't receive the entire response
// from the server within a desired time.
TEST_F(HttpClientTest, clientRequestTimeout) {
    ASSERT_NO_FATAL_FAILURE(testClientRequestTimeout());
}

// Test that client times out when it doesn't receive the entire response
// from the server within a desired time.
TEST_F(HttpClientTest, clientRequestTimeoutMultiThreading) {
    MultiThreadingMgr::instance().setMode(true);
    ASSERT_NO_FATAL_FAILURE(testClientRequestTimeout());
}

// This test verifies the behavior of the HTTP client when the premature
// (and unexpected) timeout occurs. The premature timeout may be caused
// by the system clock move.
TEST_F(HttpClientTest, clientRequestLateStartNoQueue) {
    testClientRequestLateStart(false);
}

// This test verifies the behavior of the HTTP client when the premature
// (and unexpected) timeout occurs. The premature timeout may be caused
// by the system clock move.
TEST_F(HttpClientTest, clientRequestLateStartNoQueueMultiThreading) {
    MultiThreadingMgr::instance().setMode(true);
    testClientRequestLateStart(false);
}

// This test verifies the behavior of the HTTP client when the premature
// timeout occurs and there are requests queued after the request which
// times out.
TEST_F(HttpClientTest, clientRequestLateStartQueue) {
    testClientRequestLateStart(true);
}

// This test verifies the behavior of the HTTP client when the premature
// timeout occurs and there are requests queued after the request which
// times out.
TEST_F(HttpClientTest, clientRequestLateStartQueueMultiThreading) {
    MultiThreadingMgr::instance().setMode(true);
    testClientRequestLateStart(true);
}

// Test that client times out when connection takes too long.
TEST_F(HttpClientTest, clientConnectTimeout) {
    ASSERT_NO_FATAL_FAILURE(testClientConnectTimeout());
}

// Test that client times out when connection takes too long.
TEST_F(HttpClientTest, clientConnectTimeoutMultiThreading) {
    MultiThreadingMgr::instance().setMode(true);
    ASSERT_NO_FATAL_FAILURE(testClientConnectTimeout());
}

/// Tests that connect and close callbacks work correctly.
TEST_F(HttpClientTest, connectCloseCallbacks) {
    ASSERT_NO_FATAL_FAILURE(testConnectCloseCallbacks(HttpVersion(1, 1)));
}

/// Tests that connect and close callbacks work correctly.
TEST_F(HttpClientTest, connectCloseCallbacksMultiThreading) {
    MultiThreadingMgr::instance().setMode(true);
    ASSERT_NO_FATAL_FAILURE(testConnectCloseCallbacks(HttpVersion(1, 1)));
}

/// Tests that HttpClient::closeIfOutOfBand works correctly.
TEST_F(HttpClientTest, closeIfOutOfBand) {
    ASSERT_NO_FATAL_FAILURE(testCloseIfOutOfBand(HttpVersion(1, 1)));
}

/// Tests that HttpClient::closeIfOutOfBand works correctly.
TEST_F(HttpClientTest, closeIfOutOfBandMultiThreading) {
    MultiThreadingMgr::instance().setMode(true);
    ASSERT_NO_FATAL_FAILURE(testCloseIfOutOfBand(HttpVersion(1, 1)));
}

}
