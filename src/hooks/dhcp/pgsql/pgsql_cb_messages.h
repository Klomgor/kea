// File created from src/hooks/dhcp/pgsql/pgsql_cb_messages.mes

#ifndef PGSQL_CB_MESSAGES_H
#define PGSQL_CB_MESSAGES_H

#include <log/message_types.h>

namespace isc {
namespace cb {

extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_BY_POOL_OPTION4;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_BY_POOL_OPTION6;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_BY_PREFIX_OPTION6;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_BY_SUBNET_ID_OPTION4;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_BY_SUBNET_ID_OPTION6;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_CLIENT_CLASS4;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_CLIENT_CLASS6;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_GLOBAL_PARAMETER4;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_GLOBAL_PARAMETER6;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_OPTION4;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_OPTION6;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_OPTION_DEF4;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_OPTION_DEF6;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_SERVER4;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_SERVER6;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_SHARED_NETWORK4;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_SHARED_NETWORK6;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_SHARED_NETWORK_OPTION4;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_SHARED_NETWORK_OPTION6;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_SUBNET4;
extern const isc::log::MessageID PGSQL_CB_CREATE_UPDATE_SUBNET6;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_CLIENT_CLASSES4;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_CLIENT_CLASSES4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_CLIENT_CLASSES6;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_CLIENT_CLASSES6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_GLOBAL_PARAMETERS4;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_GLOBAL_PARAMETERS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_GLOBAL_PARAMETERS6;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_GLOBAL_PARAMETERS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_OPTION_DEFS4;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_OPTION_DEFS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_OPTION_DEFS6;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_OPTION_DEFS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_SERVERS4;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_SERVERS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_SERVERS6;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_SERVERS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_SHARED_NETWORKS4;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_SHARED_NETWORKS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_SHARED_NETWORKS6;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_SHARED_NETWORKS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_SUBNETS4;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_SUBNETS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_SUBNETS6;
extern const isc::log::MessageID PGSQL_CB_DELETE_ALL_SUBNETS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_POOL_OPTION4;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_POOL_OPTION4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_POOL_OPTION6;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_POOL_OPTION6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_POOL_PREFIX_OPTION6;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_POOL_PREFIX_OPTION6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_PREFIX_SUBNET4;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_PREFIX_SUBNET4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_PREFIX_SUBNET6;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_PREFIX_SUBNET6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_SUBNET_ID_OPTION4;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_SUBNET_ID_OPTION4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_SUBNET_ID_OPTION6;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_SUBNET_ID_OPTION6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_SUBNET_ID_SUBNET4;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_SUBNET_ID_SUBNET4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_SUBNET_ID_SUBNET6;
extern const isc::log::MessageID PGSQL_CB_DELETE_BY_SUBNET_ID_SUBNET6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_CLIENT_CLASS4;
extern const isc::log::MessageID PGSQL_CB_DELETE_CLIENT_CLASS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_CLIENT_CLASS6;
extern const isc::log::MessageID PGSQL_CB_DELETE_CLIENT_CLASS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_GLOBAL_PARAMETER4;
extern const isc::log::MessageID PGSQL_CB_DELETE_GLOBAL_PARAMETER4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_GLOBAL_PARAMETER6;
extern const isc::log::MessageID PGSQL_CB_DELETE_GLOBAL_PARAMETER6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_OPTION4;
extern const isc::log::MessageID PGSQL_CB_DELETE_OPTION4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_OPTION6;
extern const isc::log::MessageID PGSQL_CB_DELETE_OPTION6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_OPTION_DEF4;
extern const isc::log::MessageID PGSQL_CB_DELETE_OPTION_DEF4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_OPTION_DEF6;
extern const isc::log::MessageID PGSQL_CB_DELETE_OPTION_DEF6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_SERVER4;
extern const isc::log::MessageID PGSQL_CB_DELETE_SERVER4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_SERVER6;
extern const isc::log::MessageID PGSQL_CB_DELETE_SERVER6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_SHARED_NETWORK4;
extern const isc::log::MessageID PGSQL_CB_DELETE_SHARED_NETWORK4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_SHARED_NETWORK6;
extern const isc::log::MessageID PGSQL_CB_DELETE_SHARED_NETWORK6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_SHARED_NETWORK_OPTION4;
extern const isc::log::MessageID PGSQL_CB_DELETE_SHARED_NETWORK_OPTION4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_SHARED_NETWORK_OPTION6;
extern const isc::log::MessageID PGSQL_CB_DELETE_SHARED_NETWORK_OPTION6_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_SHARED_NETWORK_SUBNETS4;
extern const isc::log::MessageID PGSQL_CB_DELETE_SHARED_NETWORK_SUBNETS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_DELETE_SHARED_NETWORK_SUBNETS6;
extern const isc::log::MessageID PGSQL_CB_DELETE_SHARED_NETWORK_SUBNETS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_CLIENT_CLASSES4;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_CLIENT_CLASSES4_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_CLIENT_CLASSES6;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_CLIENT_CLASSES6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_GLOBAL_PARAMETERS4;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_GLOBAL_PARAMETERS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_GLOBAL_PARAMETERS6;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_GLOBAL_PARAMETERS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_OPTIONS4;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_OPTIONS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_OPTIONS6;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_OPTIONS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_OPTION_DEFS4;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_OPTION_DEFS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_OPTION_DEFS6;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_OPTION_DEFS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_SERVERS4;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_SERVERS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_SERVERS6;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_SERVERS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_SHARED_NETWORKS4;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_SHARED_NETWORKS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_SHARED_NETWORKS6;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_SHARED_NETWORKS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_SUBNETS4;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_SUBNETS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_SUBNETS6;
extern const isc::log::MessageID PGSQL_CB_GET_ALL_SUBNETS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_CLIENT_CLASS4;
extern const isc::log::MessageID PGSQL_CB_GET_CLIENT_CLASS6;
extern const isc::log::MessageID PGSQL_CB_GET_GLOBAL_PARAMETER4;
extern const isc::log::MessageID PGSQL_CB_GET_GLOBAL_PARAMETER6;
extern const isc::log::MessageID PGSQL_CB_GET_HOST4;
extern const isc::log::MessageID PGSQL_CB_GET_HOST6;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_CLIENT_CLASSES4;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_CLIENT_CLASSES4_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_CLIENT_CLASSES6;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_CLIENT_CLASSES6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_GLOBAL_PARAMETERS4;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_GLOBAL_PARAMETERS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_GLOBAL_PARAMETERS6;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_GLOBAL_PARAMETERS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_OPTIONS4;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_OPTIONS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_OPTIONS6;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_OPTIONS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_OPTION_DEFS4;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_OPTION_DEFS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_OPTION_DEFS6;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_OPTION_DEFS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_SHARED_NETWORKS4;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_SHARED_NETWORKS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_SHARED_NETWORKS6;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_SHARED_NETWORKS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_SUBNETS4;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_SUBNETS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_SUBNETS6;
extern const isc::log::MessageID PGSQL_CB_GET_MODIFIED_SUBNETS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_OPTION4;
extern const isc::log::MessageID PGSQL_CB_GET_OPTION6;
extern const isc::log::MessageID PGSQL_CB_GET_OPTION_DEF4;
extern const isc::log::MessageID PGSQL_CB_GET_OPTION_DEF6;
extern const isc::log::MessageID PGSQL_CB_GET_PORT4;
extern const isc::log::MessageID PGSQL_CB_GET_PORT6;
extern const isc::log::MessageID PGSQL_CB_GET_RECENT_AUDIT_ENTRIES4;
extern const isc::log::MessageID PGSQL_CB_GET_RECENT_AUDIT_ENTRIES4_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_RECENT_AUDIT_ENTRIES6;
extern const isc::log::MessageID PGSQL_CB_GET_RECENT_AUDIT_ENTRIES6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_SERVER4;
extern const isc::log::MessageID PGSQL_CB_GET_SERVER6;
extern const isc::log::MessageID PGSQL_CB_GET_SHARED_NETWORK4;
extern const isc::log::MessageID PGSQL_CB_GET_SHARED_NETWORK6;
extern const isc::log::MessageID PGSQL_CB_GET_SHARED_NETWORK_SUBNETS4;
extern const isc::log::MessageID PGSQL_CB_GET_SHARED_NETWORK_SUBNETS4_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_SHARED_NETWORK_SUBNETS6;
extern const isc::log::MessageID PGSQL_CB_GET_SHARED_NETWORK_SUBNETS6_RESULT;
extern const isc::log::MessageID PGSQL_CB_GET_SUBNET4_BY_PREFIX;
extern const isc::log::MessageID PGSQL_CB_GET_SUBNET4_BY_SUBNET_ID;
extern const isc::log::MessageID PGSQL_CB_GET_SUBNET6_BY_PREFIX;
extern const isc::log::MessageID PGSQL_CB_GET_SUBNET6_BY_SUBNET_ID;
extern const isc::log::MessageID PGSQL_CB_GET_TYPE4;
extern const isc::log::MessageID PGSQL_CB_GET_TYPE6;
extern const isc::log::MessageID PGSQL_CB_RECONNECT_ATTEMPT_FAILED4;
extern const isc::log::MessageID PGSQL_CB_RECONNECT_ATTEMPT_FAILED6;
extern const isc::log::MessageID PGSQL_CB_RECONNECT_ATTEMPT_SCHEDULE4;
extern const isc::log::MessageID PGSQL_CB_RECONNECT_ATTEMPT_SCHEDULE6;
extern const isc::log::MessageID PGSQL_CB_RECONNECT_FAILED4;
extern const isc::log::MessageID PGSQL_CB_RECONNECT_FAILED6;
extern const isc::log::MessageID PGSQL_CB_REGISTER_BACKEND_TYPE4;
extern const isc::log::MessageID PGSQL_CB_REGISTER_BACKEND_TYPE6;
extern const isc::log::MessageID PGSQL_CB_UNREGISTER_BACKEND_TYPE4;
extern const isc::log::MessageID PGSQL_CB_UNREGISTER_BACKEND_TYPE6;

} // namespace cb
} // namespace isc

#endif // PGSQL_CB_MESSAGES_H
