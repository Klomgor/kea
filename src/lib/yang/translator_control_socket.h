// Copyright (C) 2018-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef ISC_TRANSLATOR_CONTROL_SOCKET_H
#define ISC_TRANSLATOR_CONTROL_SOCKET_H 1

#include <yang/translator.h>

namespace isc {
namespace yang {

/// Control socket translation between YANG and JSON
///
/// JSON syntax for all Kea servers with command channel is:
/// @code
/// "control-sockets": [ {
///     "socket-type": "<socket type>",
///     "socket-name": "<socket name>",
///     "user-context": { <json map> },
///     "comment": "<comment>"
/// } ]
/// @endcode
///
/// YANG syntax is:
/// @code
/// +--rw control-socket!
/// +--rw control-sockets*
/// +--rw socket-name     string
/// +--rw socket-type     enumeration
/// +--rw user-context?   user-context
/// @endcode
///
/// An example in JSON and YANG formats:
/// @code
/// {
///     "socket-name": "kea.sock",
///     "socket-type": "unix",
///     "user-context": { "foo": 1 }
/// }
/// @endcode
/// @code
///  /kea-ctrl-agent:config (container)
///  /kea-ctrl-agent:config/control-sockets (container)
///  /kea-ctrl-agent:config/control-sockets/
///     control-sockets[server-type='dhcp4'] (list instance)
///  /kea-ctrl-agent:config/control-sockets/control-sockets[server-type='dhcp4']/
///     server-type = dhcp4
///  /kea-ctrl-agent:config/control-sockets/control-sockets[server-type='dhcp4']/
///     control-socket (container)
///  /kea-ctrl-agent:config/control-sockets/control-sockets[server-type='dhcp4']/
///     control-socket/socket-name = kea.sock
///  /kea-ctrl-agent:config/control-sockets/control-sockets[server-type='dhcp4']/
///     control-socket/socket-type = unix
///  /kea-ctrl-agent:config/control-sockets/control-sockets[server-type='dhcp4']/
///     control-socket/user-context = { \"foo\": 1 }
/// @endcode

/// @brief A translator class for converting a control socket between
/// YANG and JSON.
///
/// Supports the following models:
/// - kea-dhcp4-server
/// - kea-dhcp6-server
/// - kea-dhcp-ddns
/// - kea-ctrl-agent
class TranslatorControlSocket : virtual public Translator {
public:
    /// @brief Constructor.
    ///
    /// @param session Sysrepo session.
    /// @param model Model name.
    TranslatorControlSocket(sysrepo::Session session,
                            const std::string& model);

    /// @brief Destructor.
    virtual ~TranslatorControlSocket() = default;

    /// @brief Translate a control socket from YANG to JSON.
    ///
    /// @param data_node the YANG node representing the control sockets
    ///
    /// @return the JSON representation of the control socket
    ///
    /// @throw NetconfError when sysrepo raises an error.
    isc::data::ElementPtr getControlSockets(libyang::DataNode const& data_node);

    /// @brief Translate a control socket from YANG to JSON.
    ///
    /// @param data_node the YANG node representing the control socket
    ///
    /// @return the JSON representation of the control socket
    ///
    /// @throw NetconfError when sysrepo raises an error.
    isc::data::ElementPtr getControlSocket(libyang::DataNode const& data_node);

    /// @brief Translate a control socket from YANG to JSON.
    ///
    /// @note This is a computationally expensive operation that makes a lookup in the sysrepo
    /// datastore by calling Session::getData(). It should be used sparingly in production code,
    /// mainly to get an initial data node to work with. It may be used at will in unit tests.
    /// Use getControlSocket(libyang::DataNode) as a scalable alternative.
    ///
    /// @param xpath The xpath of the control socket.
    ///
    /// @return JSON representation of the control socket
    ///
    /// @throw NetconfError when sysrepo raises an error.
    isc::data::ElementPtr getControlSocketFromAbsoluteXpath(std::string const& xpath);

    /// @brief Translate and set control sockets from JSON to YANG.
    ///
    /// @param xpath The xpath of the control socket.
    /// @param elem The JSON element.
    void setControlSockets(const std::string& xpath,
                           isc::data::ConstElementPtr elem);

    /// @brief Translate and set control socket from JSON to YANG.
    ///
    /// @param xpath The xpath of the control socket.
    /// @param elem The JSON element.
    void setControlSocket(const std::string& xpath,
                          isc::data::ConstElementPtr elem);

protected:
    /// @brief getControlSocket JSON for kea models.
    ///
    /// @param data_node the YANG node representing the control socket
    /// @return JSON representation of the control socket.
    /// @throw NetconfError when sysrepo raises an error.
    isc::data::ElementPtr getControlSocketsKea(libyang::DataNode const& data_node);

    /// @brief getControlSocket JSON for kea models.
    ///
    /// @param data_node the YANG node representing the control socket
    /// @return JSON representation of the control socket.
    /// @throw NetconfError when sysrepo raises an error.
    isc::data::ElementPtr getControlSocketKea(libyang::DataNode const& data_node);

    /// @brief getControlSocketsAuthenticationClients JSON for kea models.
    ///
    /// @param data_node the YANG node representing the control socket
    /// @return JSON representation of the control socket.
    /// @throw NetconfError when sysrepo raises an error.
    isc::data::ElementPtr getControlSocketAuthenticationClients(libyang::DataNode const& data_node);

    /// @brief getControlSocketsAuthenticationClients JSON for kea models.
    ///
    /// @param data_node the YANG node representing the control socket
    /// @return JSON representation of the control socket.
    /// @throw NetconfError when sysrepo raises an error.
    isc::data::ElementPtr getControlSocketAuthenticationClient(libyang::DataNode const& data_node);

    /// @brief getControlSocketsAuthenticationClients JSON for kea models.
    ///
    /// @param data_node the YANG node representing the control socket
    /// @return JSON representation of the control socket.
    /// @throw NetconfError when sysrepo raises an error.
    isc::data::ElementPtr getControlSocketHttpHeaders(libyang::DataNode const& data_node);

    /// @brief getControlSocketsAuthenticationClients JSON for kea models.
    ///
    /// @param data_node the YANG node representing the control socket
    /// @return JSON representation of the control socket.
    /// @throw NetconfError when sysrepo raises an error.
    isc::data::ElementPtr getControlSocketHttpHeader(libyang::DataNode const& data_node);

    /// @brief setControlSocket for kea models.
    ///
    /// Null elem argument removes the container.
    /// Required parameters passed in elem are: socket-name, socket-type.
    /// Optional parameters are: user-context.
    ///
    /// @param xpath The xpath of the control socket.
    /// @param elem The JSON element.
    /// @throw BadValue on control socket without socket type or name.
    void setControlSocketsKea(const std::string& xpath,
                              isc::data::ConstElementPtr elem);

    /// @brief setControlSocket for kea models.
    ///
    /// Null elem argument removes the container.
    /// Required parameters passed in elem are: socket-name, socket-type.
    /// Optional parameters are: user-context.
    ///
    /// @param xpath The xpath of the control socket.
    /// @param elem The JSON element.
    /// @param has_mandatory_key Whether this specific database instance has a mandatory key.
    /// @throw BadValue on control socket without socket type or name.
    void setControlSocketKea(const std::string& xpath, isc::data::ConstElementPtr elem, bool has_mandatory_key);

    /// @brief setControlSocketAuthenticationClients for kea models.
    ///
    /// Null elem argument removes the container.
    /// Required parameters passed in elem are: socket-name, socket-type.
    /// Optional parameters are: user-context.
    ///
    /// @param xpath The xpath of the control socket.
    /// @param elem The JSON element.
    /// @throw BadValue on control socket without socket type or name.
    void setControlSocketAuthenticationClients(const std::string& xpath,
                                               isc::data::ConstElementPtr elem);

    /// @brief setControlSocketAuthenticationClient for kea models.
    ///
    /// Null elem argument removes the container.
    /// Required parameters passed in elem are: socket-name, socket-type.
    /// Optional parameters are: user-context.
    ///
    /// @param xpath The xpath of the control socket.
    /// @param elem The JSON element.
    /// @throw BadValue on control socket without socket type or name.
    void
    setControlSocketAuthenticationClient(const std::string& xpath, isc::data::ConstElementPtr elem);

    /// @brief setControlSocketHttpHeaders for kea models.
    ///
    /// Null elem argument removes the container.
    /// Required parameters passed in elem are: socket-name, socket-type.
    /// Optional parameters are: user-context.
    ///
    /// @param xpath The xpath of the control socket.
    /// @param elem The JSON element.
    /// @throw BadValue on control socket without socket type or name.
    void setControlSocketHttpHeaders(const std::string& xpath, isc::data::ConstElementPtr elem);

    /// @brief setControlSocketHttpHeader for kea models.
    ///
    /// Null elem argument removes the container.
    /// Required parameters passed in elem are: socket-name, socket-type.
    /// Optional parameters are: user-context.
    ///
    /// @param xpath The xpath of the control socket.
    /// @param elem The JSON element.
    /// @throw BadValue on control socket without socket type or name.
    void setControlSocketHttpHeader(const std::string& xpath, isc::data::ConstElementPtr elem);
};  // TranslatorControlSocket

}  // namespace yang
}  // namespace isc

#endif  // ISC_TRANSLATOR_CONTROL_SOCKET_H
