// Copyright (C) 2018-2021 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TEST_CONFIG_BACKEND_DHCP4
#define TEST_CONFIG_BACKEND_DHCP4

#include <config.h>

#include <database/database_connection.h>
#include <database/server.h>
#include <database/server_collection.h>
#include <dhcpsrv/client_class_def.h>
#include <dhcpsrv/config_backend_dhcp4_mgr.h>
#include <dhcpsrv/testutils/test_config_backend.h>

#include <boost/shared_ptr.hpp>

#include <map>
#include <string>
#include <vector>

namespace isc {
namespace dhcp {
namespace test {

/// @brief Test config backend that implements all of the DHCPv4 API calls
///
/// This backend should be used for unit testing the DHCPv4 server and the
/// commands which manipulate the configuration information stored in the
/// database.
///
/// Server selectors supported by this test configuration backend are a
/// superset of the server selectors allowed by the API. Therefore, if
/// additional server selectors are allowed by the API in the future
/// this backend should not require any additional changes to support them.
///
/// This backend stores server configuration information in memory.
class TestConfigBackendDHCPv4 : public TestConfigBackend<ConfigBackendDHCPv4> {
public:
    /// @brief Constructor
    ///
    /// @param params Database connection parameters.
    TestConfigBackendDHCPv4(const db::DatabaseConnection::ParameterMap& params)
        : TestConfigBackend(params) {
    }

    /// @brief virtual Destructor.
    virtual ~TestConfigBackendDHCPv4(){};

    /// @brief Registers the backend type with the given backend manager
    ///
    /// @param mgr configuration manager to register with
    /// @brief db_type back end type - Note you will need to
    /// use the same value here as you do when creating backend instances.
    static bool registerBackendType(ConfigBackendDHCPv4Mgr& mgr,
                                    const std::string& db_type);

    /// @brief Unregisters the backend from the given backend manager
    ///
    /// @param mgr configuration manager to unregister from
    /// @brief db_type back end type - Note you will need to
    /// use the same value here as you do when registering the backend type
    static void unregisterBackendType(ConfigBackendDHCPv4Mgr& mgr,
                                      const std::string& db_type);

    /// @brief Retrieves a single subnet by subnet_prefix.
    ///
    /// @param server_selector Server selector.
    /// @param subnet_prefix Prefix of the subnet to be retrieved.
    /// @return Pointer to the retrieved subnet or NULL if not found.
    virtual Subnet4Ptr
    getSubnet4(const db::ServerSelector& server_selector,
               const std::string& subnet_prefix) const;

    /// @brief Retrieves a single subnet by subnet identifier.
    ///
    /// @param server_selector Server selector.
    /// @param subnet_id Identifier of a subnet to be retrieved.
    /// @return Pointer to the retrieved subnet or NULL if not found.
    virtual Subnet4Ptr
    getSubnet4(const db::ServerSelector& server_selector, const SubnetID& subnet_id) const;

    /// @brief Retrieves all subnets.
    ///
    /// @param server_selector Server selector.
    /// @return Collection of subnets or empty collection if no subnet found.
    virtual Subnet4Collection
    getAllSubnets4(const db::ServerSelector& server_selector) const;

    /// @brief Retrieves subnets modified after specified time.
    ///
    /// @param server_selector Server selector.
    /// @param modification_time Lower bound subnet modification time.
    /// @return Collection of subnets or empty collection if no subnet found.
    virtual Subnet4Collection
    getModifiedSubnets4(const db::ServerSelector& server_selector,
                        const boost::posix_time::ptime& modification_time) const;

    /// @brief Retrieves all subnets belonging to a specified shared network.
    ///
    /// @param server_selector Server selector.
    /// @param shared_network_name Name of the shared network for which the
    /// subnets should be retrieved.
    /// @return Collection of subnets or empty collection if no subnet found.
    virtual Subnet4Collection
    getSharedNetworkSubnets4(const db::ServerSelector& server_selector,
                             const std::string& shared_network_name) const;

    /// @brief Retrieves shared network by name.
    ///
    /// @param server_selector Server selector.
    /// @param name Name of the shared network to be retrieved.
    /// @return Pointer to the shared network or NULL if not found.
    virtual SharedNetwork4Ptr
    getSharedNetwork4(const db::ServerSelector& server_selector,
                      const std::string& name) const;

    /// @brief Retrieves all shared networks.
    ///
    /// @param server_selector Server selector.
    /// @return Collection of shared network or empty collection if
    /// no shared network found.
    virtual SharedNetwork4Collection
    getAllSharedNetworks4(const db::ServerSelector& server_selector) const;

    /// @brief Retrieves shared networks modified after specified time.
    ///
    /// @param server_selector Server selector.
    /// @param modification_time Lower bound shared network modification time.
    /// @return Collection of shared network or empty collection if
    /// no shared network found.
    virtual SharedNetwork4Collection
    getModifiedSharedNetworks4(const db::ServerSelector& server_selector,
                               const boost::posix_time::ptime& modification_time) const;

    /// @brief Retrieves single option definition by code and space.
    ///
    /// @param server_selector Server selector.
    /// @param code Code of the option to be retrieved.
    /// @param space Option space of the option to be retrieved.
    /// @return Pointer to the option definition or NULL if not found.
    virtual OptionDefinitionPtr
    getOptionDef4(const db::ServerSelector& server_selector, const uint16_t code,
                  const std::string& space) const;

    /// @brief Retrieves all option definitions.
    ///
    /// @param server_selector Server selector.
    /// @return Collection of option definitions or empty collection if
    /// no option definition found.
    virtual OptionDefContainer
    getAllOptionDefs4(const db::ServerSelector& server_selector) const;

    /// @brief Retrieves option definitions modified after specified time.
    ///
    /// @param server_selector Server selector.
    /// @param modification_time Lower bound option definition modification
    /// time.
    /// @return Collection of option definitions or empty collection if
    /// no option definition found.
    virtual OptionDefContainer
    getModifiedOptionDefs4(const db::ServerSelector& server_selector,
                           const boost::posix_time::ptime& modification_time) const;

    /// @brief Retrieves single option by code and space.
    ///
    /// @param server_selector Server selector.
    /// @param code Code of the option to be deleted.
    /// @param space Option space of the option to be deleted.
    /// @param client_classes Optional client classes list of the option to be deleted.
    /// Defaults to an empty pointer.
    /// @return Pointer to the retrieved option descriptor or null if
    /// no option was found.
    virtual OptionDescriptorPtr
    getOption4(const db::ServerSelector& server_selector, const uint16_t code,
               const std::string& space,
               const ClientClassesPtr client_classes = ClientClassesPtr()) const;

    /// @brief Retrieves all global options.
    ///
    /// @param server_selector Server selector.
    /// @return Collection of global options or empty collection if no
    /// option found.
    virtual OptionContainer
    getAllOptions4(const db::ServerSelector& server_selector) const;

    /// @brief Retrieves option modified after specified time.
    ///
    /// @param selector Server selector.
    /// @param modification_time Lower bound option modification time.
    /// @return Collection of global options or empty collection if no
    /// option found.
    virtual OptionContainer
    getModifiedOptions4(const db::ServerSelector& server_selector,
                        const boost::posix_time::ptime& modification_time) const;

    /// @brief Retrieves global parameter value.
    ///
    /// @param server_selector Server selector.
    /// @param name Name of the global parameter to be retrieved.
    /// @return Value of the global parameter or null if parameter doesn't
    /// exist.
    virtual data::StampedValuePtr
    getGlobalParameter4(const db::ServerSelector& server_selector,
                        const std::string& name) const;

    /// @brief Retrieves all global parameters.
    ///
    /// @param backend_selector Backend selector.
    /// @return Collection of global parameters.
    virtual data::StampedValueCollection
    getAllGlobalParameters4(const db::ServerSelector& server_selector) const;

    /// @brief Retrieves global parameters modified after specified time.
    ///
    /// @param selector Server selector.
    /// @return Collection of modified global parameters.
    virtual data::StampedValueCollection
    getModifiedGlobalParameters4(const db::ServerSelector& server_selector,
                                 const boost::posix_time::ptime& modification_time) const;

    /// @brief Retrieves a client class by name.
    ///
    /// @param server_selector Server selector.
    /// @param name Client class name.
    /// @return Pointer to the retrieved client class.
    virtual ClientClassDefPtr
    getClientClass4(const db::ServerSelector& selector, const std::string& name) const;

    /// @brief Retrieves all client classes.
    ///
    /// @param selector Server selector.
    /// @return Collection of client classes.
    virtual ClientClassDictionary
    getAllClientClasses4(const db::ServerSelector& selector) const;

    /// @brief Retrieves client classes modified after specified time.
    ///
    /// @param selector Server selector.
    /// @param modification_time Modification time.
    /// @return Collection of client classes.
    virtual ClientClassDictionary
    getModifiedClientClasses4(const db::ServerSelector& selector,
                              const boost::posix_time::ptime& modification_time) const;

    /// @brief Retrieves the most recent audit entries.
    ///
    /// @param server_selector Server selector.
    /// @param modification_time Timestamp being a lower limit for the returned
    /// result set, i.e. entries later than specified time are returned.
    /// @param modification_id Identifier being a lower limit for the returned
    /// result set, used when two (or more) entries have the same
    /// modification_time.
    /// @return Collection of audit entries.
    virtual db::AuditEntryCollection
    getRecentAuditEntries(const db::ServerSelector& server_selector,
                          const boost::posix_time::ptime& modification_time,
                          const uint64_t& modification_id) const;

    /// @brief Retrieves all servers.
    ///
    /// @return Collection of servers from the backend.
    virtual db::ServerCollection
    getAllServers4() const;

    /// @brief Retrieves a server.
    ///
    /// @param server_tag Tag of the server to be retrieved.
    /// @return Pointer to the server instance or null pointer if no server
    /// with the particular tag was found.
    virtual db::ServerPtr
    getServer4(const data::ServerTag& server_tag) const;

    /// @brief Creates or updates a subnet.
    ///
    /// @param server_selector Server selector.
    /// @param subnet Subnet to be added or updated.
    virtual void
    createUpdateSubnet4(const db::ServerSelector& server_selector,
                        const Subnet4Ptr& subnet);

    /// @brief Creates or updates a shared network.
    ///
    /// @param server_selector Server selector.
    /// @param shared_network Shared network to be added or updated.
    virtual void
    createUpdateSharedNetwork4(const db::ServerSelector& server_selector,
                               const SharedNetwork4Ptr& shared_network);

    /// @brief Creates or updates an option definition.
    ///
    /// @param server_selector Server selector.
    /// @param option_def Option definition to be added or updated.
    virtual void
    createUpdateOptionDef4(const db::ServerSelector& server_selector,
                           const OptionDefinitionPtr& option_def);

    /// @brief Creates or updates global option.
    ///
    /// @param server_selector Server selector.
    /// @param option Option to be added or updated.
    virtual void
    createUpdateOption4(const db::ServerSelector& server_selector,
                        const OptionDescriptorPtr& option);

    /// @brief Creates or updates shared network level option.
    ///
    /// @param selector Server selector.
    /// @param shared_network_name Name of a shared network to which option
    /// belongs.
    /// @param option Option to be added or updated.
    virtual void
    createUpdateOption4(const db::ServerSelector& server_selector,
                        const std::string& shared_network_name,
                        const OptionDescriptorPtr& option);

    /// @brief Creates or updates subnet level option.
    ///
    /// @param server_selector Server selector.
    /// @param subnet_id Identifier of a subnet to which option belongs.
    /// @param option Option to be added or updated.
    virtual void
    createUpdateOption4(const db::ServerSelector& server_selector,
                        const SubnetID& subnet_id,
                        const OptionDescriptorPtr& option);

    /// @brief Creates or updates pool level option.
    ///
    /// @param server_selector Server selector.
    /// @param pool_start_address Lower bound address of the pool to which
    /// the option belongs.
    /// @param pool_end_address Upper bound address of the pool to which the
    /// option belongs.
    /// @param option Option to be added or updated.
    virtual void
    createUpdateOption4(const db::ServerSelector& server_selector,
                        const asiolink::IOAddress& pool_start_address,
                        const asiolink::IOAddress& pool_end_address,
                        const OptionDescriptorPtr& option);

    /// @brief Creates or updates global parameter.
    ///
    /// @param server_selector Server selector.
    /// @param value Value of the global parameter.
    virtual void
    createUpdateGlobalParameter4(const db::ServerSelector& server_selector,
                                 const data::StampedValuePtr& value);

    /// @brief Creates or updates DHCPv4 client class.
    ///
    /// @param server_selector Server selector.
    /// @param client_class Client class to be added or updated.
    /// @param follow_class_name name of the class after which the
    /// new or updated class should be positioned. An empty value
    /// causes the class to be appended at the end of the class
    /// hierarchy.
    virtual void
    createUpdateClientClass4(const db::ServerSelector& server_selector,
                             const ClientClassDefPtr& client_class,
                             const std::string& follow_class_name);

    /// @brief Creates or updates a server.
    ///
    /// @param server Instance of the server to be stored.
    virtual void
    createUpdateServer4(const db::ServerPtr& server);

    /// @brief Deletes subnet by prefix.
    ///
    /// @param server_selector Server selector.
    /// @param subnet_prefix Prefix of the subnet to be deleted.
    /// @return Number of deleted subnets.
    virtual uint64_t
    deleteSubnet4(const db::ServerSelector& server_selector,
                  const std::string& subnet_prefix);

    /// @brief Deletes subnet by identifier.
    ///
    /// @param server_selector Server selector.
    /// @param subnet_id Identifier of the subnet to be deleted.
    /// @return Number of deleted subnets.
    virtual uint64_t
    deleteSubnet4(const db::ServerSelector& server_selector, const SubnetID& subnet_id);

    /// @brief Deletes all subnets.
    ///
    /// @param server_selector Server selector.
    /// @return Number of deleted subnets.
    virtual uint64_t
    deleteAllSubnets4(const db::ServerSelector& server_selector);

    /// @brief Deletes all subnets belonging to a specified shared network.
    ///
    /// @param server_selector Server selector.
    /// @param shared_network_name Name of the shared network for which the
    /// subnets should be deleted.
    /// @return Number of deleted subnets.
    virtual uint64_t
    deleteSharedNetworkSubnets4(const db::ServerSelector& server_selector,
                                const std::string& shared_network_name);

    /// @brief Deletes shared network by name.
    ///
    /// @param server_selector Server selector.
    /// @param name Name of the shared network to be deleted.
    /// @return Number of deleted shared networks..
    virtual uint64_t
    deleteSharedNetwork4(const db::ServerSelector& server_selector,
                         const std::string& name);

    /// @brief Deletes all shared networks.
    ///
    /// @param server_selector Server selector.
    /// @return Number of deleted shared networks.
    virtual uint64_t
    deleteAllSharedNetworks4(const db::ServerSelector& server_selector);

    /// @brief Deletes option definition.
    ///
    /// @param server_selector Server selector.
    /// @param code Code of the option to be deleted.
    /// @param space Option space of the option to be deleted.
    /// @return Number of deleted option definitions.
    virtual uint64_t
    deleteOptionDef4(const db::ServerSelector& server_selector, const uint16_t code,
                     const std::string& space);

    /// @brief Deletes all option definitions.
    ///
    /// @param server_selector Server selector.
    /// @return Number of deleted option definitions.
    virtual uint64_t
    deleteAllOptionDefs4(const db::ServerSelector& server_selector);

    /// @brief Deletes global option.
    ///
    /// @param server_selector Server selector.
    /// @param code Code of the option to be deleted.
    /// @param space Option space of the option to be deleted.
    /// @param client_classes Optional client classes list of the option to be deleted.
    /// Defaults to an empty pointer.
    /// @return Number of deleted options.
    virtual uint64_t
    deleteOption4(const db::ServerSelector& server_selector, const uint16_t code,
                  const std::string& space,
                  ClientClassesPtr client_classes = ClientClassesPtr());

    /// @brief Deletes shared network level option.
    ///
    /// @param selector Server selector.
    /// @param shared_network_name Name of the shared network which option
    /// belongs to.
    /// @param code Code of the option to be deleted.
    /// @param space Option space of the option to be deleted.
    /// @param client_classes Optional client classes list of the option to be deleted.
    /// Defaults to an empty pointer.
    virtual uint64_t
    deleteOption4(const db::ServerSelector& server_selector,
                  const std::string& shared_network_name,
                  const uint16_t code,
                  const std::string& space,
                  ClientClassesPtr client_classes = ClientClassesPtr());

    /// @brief Deletes subnet level option.
    ///
    /// @param server_selector Server selector.
    /// @param subnet_id Identifier of the subnet to which deleted option
    /// belongs.
    /// @param code Code of the deleted option.
    /// @param space Option space of the deleted option.
    /// @param client_classes Optional client classes list of the option to be deleted.
    /// Defaults to an empty pointer.
    /// @return Number of deleted options.
    virtual uint64_t
    deleteOption4(const db::ServerSelector& server_selector, const SubnetID& subnet_id,
                  const uint16_t code, const std::string& space,
                  ClientClassesPtr client_classes = ClientClassesPtr());

    /// @brief Deletes pool level option.
    ///
    /// @param server_selector Server selector.
    /// @param pool_start_address Lower bound address of the pool to which
    /// deleted option belongs.
    /// @param pool_end_address Upper bound address of the pool to which the
    /// deleted option belongs.
    /// @param code Code of the deleted option.
    /// @param space Option space of the deleted option.
    /// @param client_classes Optional client classes list of the option to be deleted.
    /// Defaults to an empty pointer.
    /// @return Number of deleted options.
    virtual uint64_t
    deleteOption4(const db::ServerSelector& server_selector,
                  const asiolink::IOAddress& pool_start_address,
                  const asiolink::IOAddress& pool_end_address,
                  const uint16_t code,
                  const std::string& space,
                  ClientClassesPtr client_classes = ClientClassesPtr());

    /// @brief Deletes global parameter.
    ///
    /// @param server_selector Server selector.
    /// @param name Name of the global parameter to be deleted.
    /// @return Number of deleted global parameters.
    virtual uint64_t
    deleteGlobalParameter4(const db::ServerSelector& server_selector,
                           const std::string& name);

    /// @brief Deletes all global parameters.
    ///
    /// @param server_selector Server selector.
    /// @return Number of deleted global parameters.
    virtual uint64_t
    deleteAllGlobalParameters4(const db::ServerSelector& server_selector);

    /// @brief Deletes DHCPv4 client class.
    ///
    /// @param server_selector Server selector.
    /// @param name Name of the class to be deleted.
    /// @return Number of deleted client classes.
    virtual uint64_t
    deleteClientClass4(const db::ServerSelector& server_selector,
                       const std::string& name);

    /// @brief Deletes all client classes.
    ///
    /// @param server_selector Server selector.
    /// @return Number of deleted client classes.
    virtual uint64_t
    deleteAllClientClasses4(const db::ServerSelector& server_selector);

    /// @brief Deletes a server from the backend.
    ///
    /// @param server_tag Tag of the server to be deleted.
    /// @return Number of deleted servers.
    virtual uint64_t
    deleteServer4(const data::ServerTag& server_tag);

    /// @brief Deletes all servers from the backend except the logical
    /// server 'all'.
    ///
    /// @return Number of deleted servers.
    virtual uint64_t
    deleteAllServers4();

/// @{
/// @brief Containers used to house the "database" entries
    Subnet4Collection subnets_;
    SharedNetwork4Collection shared_networks_;
    OptionDefContainer option_defs_;
    OptionContainer options_;
    data::StampedValueCollection globals_;
    std::vector<ClientClassDefPtr> classes_;
    db::ServerCollection servers_;
/// @}
};

/// @brief Shared pointer to the @c TestConfigBackend.
typedef boost::shared_ptr<TestConfigBackendDHCPv4> TestConfigBackendDHCPv4Ptr;

} // namespace test
} // namespace dhcp
} // namespace isc

#endif // TEST_CONFIG_BACKEND_DHCP4
