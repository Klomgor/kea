// Copyright (C) 2014-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>
#include <asiolink/io_address.h>
#include <cc/data.h>
#include <dhcp/dhcp4.h>
#include <dhcp/testutils/iface_mgr_test_config.h>
#include <dhcpsrv/cfgmgr.h>
#include <dhcpsrv/host.h>
#include <dhcpsrv/host_mgr.h>
#include <dhcpsrv/subnet_id.h>
#include <testutils/gtest_utils.h>

#ifdef HAVE_MYSQL
#include <mysql/testutils/mysql_schema.h>
#include <hooks/dhcp/mysql/mysql_lease_mgr.h>
#endif

#ifdef HAVE_PGSQL
#include <pgsql/testutils/pgsql_schema.h>
#include <hooks/dhcp/pgsql/pgsql_lease_mgr.h>
#endif

#include <dhcp4/tests/dhcp4_test_utils.h>
#include <dhcp4/tests/dhcp4_client.h>
#include <boost/shared_ptr.hpp>
#include <stats/stats_mgr.h>

#include <set>
#include <vector>

using namespace isc;
using namespace isc::asiolink;
using namespace isc::data;
using namespace isc::db;
using namespace isc::dhcp;
using namespace isc::dhcp::test;
using namespace isc::stats;

namespace {

/// @brief Set of JSON configurations used throughout the DORA tests.
///
/// - Configuration 0:
///   - Used for testing direct traffic
///   - 1 subnet: 10.0.0.0/24
///   - 1 pool: 10.0.0.10-10.0.0.100
///   - Router option present: 10.0.0.200 and 10.0.0.201
///   - Domain Name Server option present: 10.0.0.202, 10.0.0.203.
///   - Log Servers option present: 192.0.2.200 and 192.0.2.201
///   - Quotes Servers option present: 192.0.2.202, 192.0.2.203.
///
/// - Configuration 1:
///   - Use for testing relayed messages
///   - 1 subnet: 192.0.2.0/24
///   - Router option present: 192.0.2.200 and 192.0.2.201
///   - Domain Name Server option present: 192.0.2.202, 192.0.2.203.
///   - Log Servers option present: 192.0.2.200 and 192.0.2.201
///   - Quotes Servers option present: 192.0.2.202, 192.0.2.203.
///
/// - Configuration 2:
///   - Use for testing simple scenarios with host reservations
///   - 1 subnet: 10.0.0.0/24
///   - One reservation for the client using MAC address:
///     aa:bb:cc:dd:ee:ff, reserved address 10.0.0.7
///
/// - Configuration 3:
///   - Use for testing match-client-id flag
///   - 1 subnet: 10.0.0.0/24
///   - 1 pool: 10.0.0.10-10.0.0.100
///   - match-client-id flag is set to false, thus the server
///     uses HW address for lease lookup, rather than client id
///
/// - Configuration 4:
///   - Used for testing host reservations where circuit-id takes precedence
///     over hw-address, and the hw-address takes precedence over duid.
///   - 1 subnet: 10.0.0.0/24
///   - 3 reservations for this subnet:
///     - IP address 10.0.0.7 for HW address aa:bb:cc:dd:ee:ff
///     - IP address 10.0.0.8 for DUID 01:02:03:04:05
///     - IP address 10.0.0.9 for circuit-id 'charter950'
///     - IP address 10.0.0.1 for client-id
///
/// - Configuration 5:
///   - The same as configuration 4, but using the following order of
///     host-reservation-identifiers: duid, circuit-id, hw-address,
///     client-id.
///
/// - Configuration 6:
///   - This configuration provides reservations for next-server,
///     server-hostname and boot-file-name value.
///   - 1 subnet: 10.0.0.0/24
///   - 1 reservation for this subnet:
///     - Client's HW address: aa:bb:cc:dd:ee:ff
///     - next-server = 10.0.0.7
///     - server name = "some-name.example.org"
///     - boot-file-name = "bootfile.efi"
///
/// - Configuration 7:
///   - Used for testing custom value of dhcp-server-identifier option.
///   - 3 subnets: 10.0.0.0/24, 192.0.2.0/26 and 192.0.2.64/26
///   - Custom server identifier specified for 2 subnets subnet.
///   - Custom server identifier specified at global level.
///
/// - Configuration 8:
///   - Simple configuration with a single subnet and single pool
///   - Using MySQL lease database backend to store leases
///
/// - Configuration 9:
///   - Simple configuration with a single subnet and single pool
///   - Using PostgreSQL lease database backend to store leases
///
/// - Configuration 10:
///   - Simple configuration with a single subnet
///   - One in-pool reservation for a circuit-id of 'charter950'
///
/// - Configuration 11:
///   - Simple configuration with a single subnet
///   - One in-pool reservation for MAC address aa:bb:cc:dd:ee:ff
///   - The reservations-in-subnet flag is set to true
///
/// - Configuration 12:
///   - Simple configuration with a single subnet as in #12
///   - The reservations-in-subnet flag is set to false for testing that the
///     reservations are ignored
///
/// - Configuration 13:
///   - Simple configuration with a single subnet
///   - Two host reservations, one out of the pool, another one in pool
///   - The reservations-in-subnet and reservations-out-of-pool flags are set to
///     true to test that only out of pool reservations are honored.
///
/// - Configuration 14:
///   - Use for testing authoritative flag
///   - 1 subnet: 10.0.0.0/24
///   - 1 pool: 10.0.0.10-10.0.0.100
///   - authoritative flag is set to true, thus the server responds
///     with DHCPNAK to requests from unknown clients.
///
/// - Configuration 15:
///   - Use for testing authoritative flag
///   - 1 subnet: 10.0.0.0/24
///   - 1 pool: 10.0.0.10-10.0.0.100
///   - authoritative flag is set to false, thus the server does not
///     respond to requests from unknown clients.
///
/// - Configuration 16:
///   - Use for testing storing extended info
///   - Two subnets with one with storing extended info enabled and
///     one with disabled.
///
/// - Configuration 17:
///   - Selects random allocator.
///   - One subnet with three distinct pools.
///   - Random allocator enabled globally.
///
/// - Configuration 18:
///   - Two subnets, one with an address pool, one without.
///   - cache-max-age set to 600
///   - cache-threshold set to .50.
///
/// - Configuration 19:
///   - No subnets.
///   - Global authoritative flag is false.
///
/// - Configuration 20:
///   - No subnets.
///   - Global authoritative flag is true.
///
/// - Configuration 21:
///   - Use for testing stash agent options (flag true).
///   - 1 subnet with storing extended info enabled
///
/// - Configuration 22:
///   - Use for testing stash agent options (flag true).
///   - 1 subnet with storing extended info enabled
///   - 1 reservation using the circuit-id
///
/// - Configuration 23:
///   - Use for testing stash agent options (flag false).
///   - 1 subnet with storing extended info enabled
///   - 1 reservation using the circuit-id
const char* DORA_CONFIGS[] = {
    // Configuration 0
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1,"
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "    \"option-data\": [ {"
        "        \"name\": \"routers\","
        "        \"data\": \"10.0.0.200,10.0.0.201\""
        "    },"
        "    {"
        "        \"name\": \"domain-name-servers\","
        "        \"data\": \"10.0.0.202,10.0.0.203\""
        "    },"
        "    {"
        "        \"name\": \"log-servers\","
        "        \"data\": \"10.0.0.200,10.0.0.201\""
        "    },"
        "    {"
        "        \"name\": \"cookie-servers\","
        "        \"data\": \"10.0.0.202,10.0.0.203\""
        "    } ]"
        " } ]"
    "}",

    // Configuration 1
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"

        "\"valid-lifetime\": 600,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"192.0.2.0/24\", "
        "    \"id\": 1,"
        "    \"option-data\": [ {"
        "        \"name\": \"routers\","
        "        \"data\": \"192.0.2.200,192.0.2.201\""
        "    },"
        "    {"
        "        \"name\": \"domain-name-servers\","
        "        \"data\": \"192.0.2.202,192.0.2.203\""
        "    },"
        "    {"
        "        \"name\": \"log-servers\","
        "        \"data\": \"10.0.0.200,10.0.0.201\""
        "    },"
        "    {"
        "        \"name\": \"cookie-servers\","
        "        \"data\": \"10.0.0.202,10.0.0.203\""
        "    } ]"
        " } ]"
    "}",

    // Configuration 2
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1,"
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "    \"reservations\": [ "
        "       {"
        "         \"hw-address\": \"aa:bb:cc:dd:ee:ff\","
        "         \"ip-address\": \"10.0.0.7\""
        "       },"
        "       {"
        "         \"duid\": \"01:02:03:04:05\","
        "         \"ip-address\": \"10.0.0.8\""
        "       },"
        "       {"
        "         \"circuit-id\": \"'charter950'\","
        "         \"ip-address\": \"10.0.0.9\""
        "       },"
        "       {"
        "         \"client-id\": \"01:11:22:33:44:55:66\","
        "         \"ip-address\": \"10.0.0.1\""
        "       }"
        "    ]"
        "} ]"
    "}",

    // Configuration 3
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"match-client-id\": false,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1,"
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "    \"option-data\": [ {"
        "        \"name\": \"routers\","
        "        \"data\": \"10.0.0.200,10.0.0.201\""
        "    } ]"
        " } ]"
    "}",

    // Configuration 4
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"host-reservation-identifiers\": [ \"circuit-id\", \"hw-address\","
        "                                    \"duid\", \"client-id\" ],"
        "\"valid-lifetime\": 600,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1,"
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "    \"reservations\": [ "
        "       {"
        "         \"hw-address\": \"aa:bb:cc:dd:ee:ff\","
        "         \"ip-address\": \"10.0.0.7\""
        "       },"
        "       {"
        "         \"duid\": \"01:02:03:04:05\","
        "         \"ip-address\": \"10.0.0.8\""
        "       },"
        "       {"
        "         \"circuit-id\": \"'charter950'\","
        "         \"ip-address\": \"10.0.0.9\""
        "       },"
        "       {"
        "         \"client-id\": \"01:11:22:33:44:55:66\","
        "         \"ip-address\": \"10.0.0.1\""
        "       }"
        "    ]"
        "} ]"
    "}",

    // Configuration 5
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"host-reservation-identifiers\": [ \"duid\", \"client-id\","
        "                                    \"circuit-id\", \"hw-address\" ],"
        "\"valid-lifetime\": 600,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1,"
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "    \"reservations\": [ "
        "       {"
        "         \"hw-address\": \"aa:bb:cc:dd:ee:ff\","
        "         \"ip-address\": \"10.0.0.7\""
        "       },"
        "       {"
        "         \"duid\": \"01:02:03:04:05\","
        "         \"ip-address\": \"10.0.0.8\""
        "       },"
        "       {"
        "         \"circuit-id\": \"'charter950'\","
        "         \"ip-address\": \"10.0.0.9\""
        "       },"
        "       {"
        "         \"client-id\": \"01:11:22:33:44:55:66\","
        "         \"ip-address\": \"10.0.0.1\""
        "       }"
        "    ]"
        "} ]"
    "}",

    // Configuration 6
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"next-server\": \"10.0.0.1\","
        "\"server-hostname\": \"nohost\","
        "\"boot-file-name\": \"nofile\","
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1,"
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "    \"reservations\": [ "
        "       {"
        "         \"hw-address\": \"aa:bb:cc:dd:ee:ff\","
        "         \"next-server\": \"10.0.0.7\","
        "         \"server-hostname\": \"some-name.example.org\","
        "         \"boot-file-name\": \"bootfile.efi\""
        "       }"
        "    ]"
        "} ]"
    "}",

    // Configuration 7
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"option-data\": ["
        "    {"
        "        \"name\": \"dhcp-server-identifier\","
        "        \"data\": \"3.4.5.6\""
        "    }"
        "],"
        "\"subnet4\": ["
        "    {"
        "        \"subnet\": \"10.0.0.0/24\", "
        "        \"id\": 1, "
        "        \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "        \"interface\": \"eth0\","
        "        \"option-data\": ["
        "            {"
        "                \"name\": \"dhcp-server-identifier\","
        "                \"data\": \"1.2.3.4\""
        "            }"
        "        ]"
        "    },"
        "    {"
        "        \"subnet\": \"192.0.2.0/26\", "
        "        \"id\": 2, "
        "        \"pools\": [ { \"pool\": \"192.0.2.10-192.0.2.63\" } ],"
        "        \"interface\": \"eth1\","
        "        \"option-data\": ["
        "            {"
        "                \"name\": \"dhcp-server-identifier\","
        "                \"data\": \"2.3.4.5\""
        "            }"
        "        ]"
        "    },"
        "    {"
        "        \"subnet\": \"192.0.2.64/26\", "
        "        \"id\": 3, "
        "        \"pools\": [ { \"pool\": \"192.0.2.65-192.0.2.100\" } ],"
        "        \"relay\": {"
        "            \"ip-addresses\": [ \"10.2.3.4\" ]"
        "        }"
        "    }"
        "]"
    "}",

    // Configuration 8
    "{ \"interfaces-config\": {"
        "   \"interfaces\": [ \"*\" ]"
        "},"
        "\"lease-database\": {"
        "   \"type\": \"mysql\","
        "   \"name\": \"keatest\","
        "   \"user\": \"keatest\","
        "   \"password\": \"keatest\""
        "},"
        "\"valid-lifetime\": 600,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1,"
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ]"
        " } ]"
    "}",

    // Configuration 9
    "{ \"interfaces-config\": {"
        "   \"interfaces\": [ \"*\" ]"
        "},"
        "\"lease-database\": {"
        "   \"type\": \"postgresql\","
        "   \"name\": \"keatest\","
        "   \"user\": \"keatest\","
        "   \"password\": \"keatest\""
        "},"
        "\"valid-lifetime\": 600,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1,"
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ]"
        " } ]"
    "}",

    // Configuration 10
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"host-reservation-identifiers\": [ \"circuit-id\" ],"
        "\"valid-lifetime\": 600,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1, "
        "    \"pools\": [ { \"pool\": \"10.0.0.5-10.0.0.100\" } ],"
        "    \"reservations\": [ "
        "       {"
        "         \"circuit-id\": \"'charter950'\","
        "         \"ip-address\": \"10.0.0.9\""
        "       }"
        "    ]"
        "} ]"
    "}",

    // Configuration 11
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\","
        "    \"id\": 1, "
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "    \"reservations-global\": false,"
        "    \"reservations-in-subnet\": true,"
        "    \"reservations-out-of-pool\": false,"
        "    \"reservations\": [ "
        "       {"
        "         \"hw-address\": \"aa:bb:cc:dd:ee:ff\","
        "         \"ip-address\": \"10.0.0.65\""
        "       }"
        "    ]"
        "} ]"
    "}",

    // Configuration 12
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\","
        "    \"id\": 1, "
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "    \"reservations-global\": false,"
        "    \"reservations-in-subnet\": false,"
        "    \"reservations\": [ "
        "       {"
        "         \"hw-address\": \"aa:bb:cc:dd:ee:ff\","
        "         \"ip-address\": \"10.0.0.65\""
        "       }"
        "    ]"
        "} ]"
    "}",

    // Configuration 13
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\","
        "    \"id\": 1, "
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "    \"reservations-global\": false,"
        "    \"reservations-in-subnet\": true,"
        "    \"reservations-out-of-pool\": true,"
        "    \"reservations\": [ "
        "       {"
        "         \"hw-address\": \"aa:bb:cc:dd:ee:ff\","
        "         \"ip-address\": \"10.0.0.200\""
        "       },"
        "       {"
        "         \"hw-address\": \"11:22:33:44:55:66\","
        "         \"ip-address\": \"10.0.0.65\""
        "       }"
        "    ]"
        "} ]"
    "}",

    // Configuration 14
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"authoritative\": true,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1,"
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "    \"option-data\": [ {"
        "        \"name\": \"routers\","
        "        \"data\": \"10.0.0.200,10.0.0.201\""
        "    } ]"
        " } ]"
    "}",

    // Configuration 15
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"authoritative\": false,"
        "\"subnet4\": [ { "
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1,"
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "    \"option-data\": [ {"
        "        \"name\": \"routers\","
        "        \"data\": \"10.0.0.200,10.0.0.201\""
        "    } ]"
        " } ]"
    "}",

    // Configuration 16
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"subnet4\": ["
        "    {"
        "        \"subnet\": \"10.0.0.0/24\", "
        "        \"id\": 1, "
        "        \"store-extended-info\": true,"
        "        \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ],"
        "        \"interface\": \"eth0\""
        "    },"
        "    {"
        "        \"subnet\": \"192.0.2.0/26\", "
        "        \"id\": 2, "
        "        \"store-extended-info\": false,"
        "        \"pools\": [ { \"pool\": \"192.0.2.10-192.0.2.63\" } ],"
        "        \"interface\": \"eth1\""
        "    }"
        "]"
    "}",

    // Configuration 17
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"allocator\": \"random\","
        "\"subnet4\": ["
        "    {"
        "        \"subnet\": \"10.0.0.0/24\", "
        "        \"id\": 1, "
        "        \"pools\": ["
        "            {"
        "                \"pool\": \"10.0.0.10-10.0.0.20\""
        "            },"
        "            {"
        "                \"pool\": \"10.0.0.30-10.0.0.40\""
        "            },"
        "            {"
        "                \"pool\": \"10.0.0.50-10.0.0.60\""
        "            }"
        "         ],"
        "        \"interface\": \"eth0\""
        "    }"
        "]"
    "}",

    // Configuration 18
    R"({
        "cache-max-age": 600,
        "cache-threshold": .50,
        "interfaces-config": {
            "interfaces": [ "*" ]
        },
        "dhcp-ddns": {
            "enable-updates": true
        },
        "ddns-send-updates": true,
        "ddns-update-on-renew": true,
        "subnet4": [
            {
                "id": 1,
                "pools": [
                    {
                        "pool": "10.0.0.10 - 10.0.0.20"
                    },
                ],
                "subnet": "10.0.0.0/24"
            },
            {
                "id": 2,
                "subnet": "192.168.0.0/24"
            }
        ],
        "valid-lifetime": 600
    })",

    // Configuration 19
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"authoritative\": false,"
        "\"subnet4\": []"
    "}",

    // Configuration 20
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"authoritative\": true,"
        "\"subnet4\": []"
    "}",

    // Configuration 21
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"stash-agent-options\": true,"
        "\"subnet4\": [ {"
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1, "
        "    \"store-extended-info\": true,"
        "    \"pools\": [ { \"pool\": \"10.0.0.10-10.0.0.100\" } ]"
        "} ]"
    "}",

    // Configuration 22
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"stash-agent-options\": true,"
        "\"host-reservation-identifiers\": [ \"circuit-id\" ],"
        "\"subnet4\": [ {"
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1, "
        "    \"store-extended-info\": true,"
        "    \"reservations\": [ {"
        "        \"circuit-id\": \"'charter950'\","
        "        \"ip-address\": \"10.0.0.9\" } ]"
        "} ]"
    "}",

    // Configuration 23
    "{ \"interfaces-config\": {"
        "      \"interfaces\": [ \"*\" ]"
        "},"
        "\"valid-lifetime\": 600,"
        "\"stash-agent-options\": false,"
        "\"host-reservation-identifiers\": [ \"circuit-id\" ],"
        "\"subnet4\": [ {"
        "    \"subnet\": \"10.0.0.0/24\", "
        "    \"id\": 1, "
        "    \"store-extended-info\": true,"
        "    \"reservations\": [ {"
        "        \"circuit-id\": \"'charter950'\","
        "        \"ip-address\": \"10.0.0.9\" } ]"
        "} ]"
    "}",
};

/// @brief Test fixture class for testing 4-way (DORA) exchanges.
///
/// @todo Currently there is a limit number of test cases covered here.
/// In the future it is planned that the tests from the
/// dhcp4_srv_unittest.cc will be migrated here and will use the
/// @c Dhcp4Client class.
class DORATest : public Dhcpv4SrvTest {
public:

    /// @brief Constructor.
    ///
    /// Sets up fake interfaces.
    DORATest()
        : Dhcpv4SrvTest(),
          iface_mgr_test_config_(true) {
        // Let's wipe all existing statistics.
        isc::stats::StatsMgr::instance().removeAll();
    }

    /// @brief Destructor.
    ///
    /// Cleans up statistics after the test.
    ~DORATest() {
        // Let's wipe all existing statistics.
        isc::stats::StatsMgr::instance().removeAll();
    }

    /// @brief Test that server returns the same lease for the client which is
    /// sometimes using client identifier, sometimes not.
    ///
    /// This test checks the server's behavior in the following scenario:
    /// - Client identifies itself to the server using HW address, and may use
    ///   client identifier.
    /// - Client performs the 4-way exchange and obtains a lease from the server.
    /// - If the client identifier was in use when the client has acquired the lease,
    ///   the client uses null client identifier in the next exchange with the server.
    /// - If the client identifier was not in use when the client has acquired the
    ///   lease, the client uses client identifier in the next exchange with the
    ///   server.
    /// - When the client contacts the server for the second time using the
    ///   DHCPDISCOVER the server determines (using HW address) that the client
    ///   already has a lease and returns this lease to the client.
    /// - The client renews the existing lease.
    ///
    /// @param clientid_a Client identifier when the client initially allocates
    /// the lease. An empty value means "no client identifier".
    /// @param clientid_b Client identifier when the client sends the DHCPDISCOVER
    /// and then DHCPREQUEST to renew lease.
    void oneAllocationOverlapTest(const std::string& clientid_a,
                                  const std::string& clientid_b);

    /// @brief Test that the client using the same hardware address but
    /// multiple client identifiers will obtain multiple leases.
    ///
    /// This reflects the scenario of the OS installation over the network
    /// when BIOS, installer and the host request DHCPv4 lease assignment
    /// using the same MAC address/interface but generating different
    /// client identifiers.
    ///
    /// @param config_index Index of the configuration within the
    /// @c DORA_CONFIGS array.
    void testMultiStageBoot(const unsigned int config_index);

    /// @brief This test verifies that the client in the SELECTING state can get
    /// an address when it doesn't request any specific address in the
    /// DHCPDISCOVER message.
    void selectingDoNotRequestAddress();

    /// @brief This test verifies that multiple clients may use the DHCPv4
    /// server and obtain unique leases.
    void selectingMultipleClients();

    /// @brief This test verifies that the client in a SELECTING state can
    /// request a specific address and that this address will be assigned when
    /// available. It also tests that if the client requests an address which is
    /// in use the client will get a different address.
    void selectingRequestAddress();

    /// @brief This test verifies that the client will get the address that it
    /// has been allocated when the client requests a different address.
    void selectingRequestNonMatchingAddress();

    /// @brief Test that the client in the INIT-REBOOT state can request the IP
    /// address it has and the address is returned. Also, check that if the
    /// client requests invalid address the server sends a DHCPNAK.
    void initRebootRequest();

    /// @brief Test that the client in the INIT-REBOOT state can request the IP
    /// address it has and the address is returned. Also, check that if the
    /// client is unknown the server sends a DHCPNAK.
    void authoritative();

    /// @brief Test that the client in the INIT-REBOOT state can request the IP
    /// address it has and the address is returned. Also, check that if  the
    /// client is unknown the request is dropped.
    void notAuthoritative();

    /// @brief Test that the authoritative server sends a DHCPNAK to the client
    /// in the INIT-REBOOT state when subnet selection fails.
    void authoritativeSubnetSelectionFail();

    /// @brief Test that the server does not respond to the client in the
    /// INIT-REBOOT state when subnet selection fails.
    void notAuthoritativeSubnetSelectionFail();

    /// @brief Check that the ciaddr returned by the server is correct for
    /// DHCPOFFER and DHCPNAK according to RFC2131, section 4.3.1.
    void ciaddr();

    /// @brief This test checks the server behavior in the following situation:
    /// - Client A identifies itself to the server using client identifier
    ///   and the hardware address and requests allocation of the new lease.
    /// - Server allocates the lease to the client.
    /// - Client B has the same hardware address but is using a different
    ///   client identifier then Client A.
    /// - Client B sends DHCPDISCOVER.
    /// - Server should determine that the client B is not client A, because
    ///   it is using a different client identifier, even though they use the
    ///   same HW address. As a consequence, the server should offer a
    ///    different address to the client B.
    /// - The client B performs the 4-way exchange again and the server
    ///   allocates a new address to the client, which should be different
    ///   than the address used by the client A.
    /// - Client B is in the renewing state and it successfully renews its
    ///   address.
    /// - Client A also renews its address successfully.
    void twoAllocationsOverlap();

    /// @brief This is a simple test for the host reservation. It creates a
    /// reservation for an address for a single client, identified by the HW
    /// address. The test verifies that the client using this HW address will
    /// obtain a lease for the reserved address. It also checks that the client
    /// using a different HW address will obtain an address from the dynamic
    /// pool.
    void reservation();

    /// @brief  This test checks that it is possible to make a reservation by
    /// DUID carried in the Client Identifier option.
    void reservationByDUID();

    /// @brief This test checks that it is possible to make a reservation by
    /// circuit-id inserted by the relay agent.
    void reservationByCircuitId();

    /// @brief This test checks that it is possible to make a reservation by
    ///  client-id.
    void reservationByClientId();

    /// @brief  This test verifies that order in which host identifiers are used
    /// to retrieve host reservations can be controlled.
    void hostIdentifiersOrder();

    /// @brief This test checks that setting the match-client-id value to false
    /// causes the server to ignore changing client identifier when the client
    /// is using consistent HW address.
    void ignoreChangingClientId();

    /// @brief This test checks that the match-client-id parameter doesn't have
    /// effect on the lease lookup using the HW address.
    void changingHWAddress();

    /// @brief This test verifies that the server assigns reserved values for
    /// the siaddr, sname and file fields carried within DHCPv4 message.
    void messageFieldsReservations();

    /// @brief This test checks the following scenario:
    /// 1. Client A performs 4-way exchange and obtains a lease from the dynamic
    /// pool.
    /// 2. Reservation is created for the client A using an address out of the
    ///    dynamic pool.
    /// 3. Client A renews the lease.
    /// 4. Server responds with DHCPNAK to indicate that the client should stop
    ///    using an address for which it has a lease. Server doesn't want to
    ///    renew an address for which the client doesn't have a reservation,
    ///    while it has a reservation for a different address.
    /// 5. Client A receives a DHCPNAK and returns to the DHCP server discovery.
    /// 6. Client A performs a 4-way exchange with a server and the server
    ///    allocates a reserved address to the Client A.
    /// 7. Client A renews the allocated address and the server returns a
    ///    DHCPACK.
    /// 8. Reservation for the Client A is removed.
    /// 9. Client A renews the (previously reserved) lease and the server
    ///    returns DHCPNAK because the address in use is neither reserved nor
    ///    belongs to the dynamic pool.
    /// 10. Client A returns to the DHCP server discovery.
    /// 11. Client A uses 4-way exchange to obtain a lease from the dynamic
    ///     pool.
    /// 12. The new address that the Client A is using is reserved for Client B.
    ///     Client A still holds this address.
    /// 13. Client B uses 4-way exchange to obtain a new lease.
    /// 14. The server determines that the Client B has a reservation for the
    ///     address which is in use by Client A and offers an address different
    ///     than reserved.
    /// 15. Client B requests the allocation of the offered address and the
    ///     server allocates this address.
    /// 16. Client A renews the lease.
    /// 17. The server determines that the address that Client A is using is
    ///     reserved for Client B. The server returns DHCPNAK to the Client A.
    /// 18. Client B uses 4-way exchange to obtain the reserved lease but the
    ///     lease for the Client A hasn't been removed yet. Client B is assigned
    ///     the same address it has been using.
    /// 19. Client A uses 4-way exchange to allocate a new lease.
    /// 20. The server allocates a new lease from the dynamic pool but it avoids
    ///     allocating the address reserved for the Client B.
    /// 21. Client B uses 4-way exchange to obtain a new lease.
    /// 22. The server finally allocates a reserved address to the Client B.
    /// @param offer_lifetime value to use as the global value of offer-lifetime
    /// throughout the test.
    void reservationsWithConflicts(int offer_lifetime = 0);

    /// @brief This test verifies that the allocation engine ignores
    /// reservations when reservations flags are set to "disabled".
    void reservationModeDisabled();

    /// @brief This test verifies that allocation engine assigns a reserved
    /// address to the client which doesn't own this reservation. We want to
    /// avoid such cases in the real deployments, but this is just a test that
    /// the allocation engine skips checking if the reservation exists when it
    /// allocates an address. In the real deployment the reservation simply
    /// wouldn't exist.
    void reservationIgnoredInDisabledMode();

    /// @brief This test verifies that in pool reservations are ignored when the
    /// reservations-out-of-pool flag is set to true.
    void reservationModeOutOfPool();

    /// @brief This test verifies that the in-pool reservation can be assigned
    /// to a client not owning this reservation when the
    /// reservations-out-of-pool flag is set to true.
    void reservationIgnoredInOutOfPoolMode();

    /// @brief This test verifies that after a client completes its DORA
    /// exchange, appropriate statistics are updated.
    void statisticsDORA();

    /// @brief This test verifies that after a client completes an exchange that
    /// result in NAK, appropriate statistics are updated.
    void statisticsNAK();

    /// @brief This test verifies that custom server identifier can be specified
    /// for a subnet.
    void customServerIdentifier();

    /// @brief This test verifies that reserved lease is not assigned to a
    /// client which identifier doesn't match the identifier in the reservation.
    void changingCircuitId();

    /// @brief  Verifies that extended info is stored on the lease when
    /// store-extended-info is enabled.
    void storeExtendedInfoEnabled();

    /// @brief Verifies that extended info is not stored on the lease when
    ///  store-extended-info is disabled.
    void storeExtendedInfoDisabled();

    /// @brief This test verifies that random allocator is used according
    /// to the configuration and it allocates random addresses.
    void randomAllocation();

    /// @brief Checks that features related to lease caching (such as lease reuse statistics) work.
    void leaseCaching();

    /// @brief Checks that stash-agent-options enables direct renewal
    /// of addresses from pool allocation for a relayed client.
    ///
    /// @note: no negative counterpart as the stash-agent-options is not
    /// required for this scenario.
    void stashAgentOptions();

    /// @brief Checks that stash-agent-options enables direct renewal
    /// of addresses from host reservation using the relay-agent-info option.
    void stashAgentOptionsReservation();

    /// @brief Checks that stash-agent-options is required for direct renewal
    /// of addresses from host reservation using the relay-agent-info option.
    void noStashAgentOptionsReservation();

    /// @brief Checks the value of a statistic.
    ///
    /// @param name name of statistic to check
    /// @param expected_size expected number of statistic samples
    /// @param expected_value expected value of the latest statistic sample
    void checkStat(string const& name,
                   size_t const expected_size,
                   int64_t const expected_value);

    /// @brief Interface Manager's fake configuration control.
    IfaceMgrTestConfig iface_mgr_test_config_;
};

void
DORATest::selectingDoNotRequestAddress() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DORA_CONFIGS[0], *client.getServer());

    // Perform 4-way exchange with the server but to not request any
    // specific address in the DHCPDISCOVER message.
    ASSERT_NO_THROW(client.doDORA());

    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Response must not be relayed.
    EXPECT_FALSE(resp->isRelayed());
    // Make sure that the server id is present.
    EXPECT_EQ("10.0.0.1", client.config_.serverid_.toText());
    // Make sure that the client has got the lease with the requested address.
    ASSERT_NE(client.config_.lease_.addr_.toText(), "0.0.0.0");
}

TEST_F(DORATest, selectingDoNotRequestAddress) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    selectingDoNotRequestAddress();
}

TEST_F(DORATest, selectingDoNotRequestAddressMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    selectingDoNotRequestAddress();
}

void
DORATest::selectingMultipleClients() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DORA_CONFIGS[0], *client.getServer());

    // Get the first lease.
    ASSERT_NO_THROW(client.doDORA());

    // Make sure that the server responded.
    Pkt4Ptr resp = client.getContext().response_;
    ASSERT_TRUE(resp);
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Store the lease.
    Lease4 lease1 = client.config_.lease_;

    // Get the lease for a different client.
    client.modifyHWAddr();
    ASSERT_NO_THROW(client.doDORA());
    // Make sure that the server responded.
    resp = client.getContext().response_;
    ASSERT_TRUE(resp);
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Store the lease.
    Lease4 lease2 = client.config_.lease_;

    // Get the lease for a different client.
    client.modifyHWAddr();
    ASSERT_NO_THROW(client.doDORA());
    // Make sure that the server responded.
    resp = client.getContext().response_;
    ASSERT_TRUE(resp);
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Store the lease.
    Lease4 lease3 = client.config_.lease_;

    // Make sure that unique addresses have been assigned.
    EXPECT_NE(lease1.addr_, lease2.addr_);
    EXPECT_NE(lease2.addr_, lease3.addr_);
    EXPECT_NE(lease1.addr_, lease3.addr_);
}

TEST_F(DORATest, selectingMultipleClients) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    selectingMultipleClients();
}

TEST_F(DORATest, selectingMultipleClientsMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    selectingMultipleClients();
}

void
DORATest::selectingRequestAddress() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DORA_CONFIGS[0], *client.getServer());

    // Perform 4-way exchange with the server.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("10.0.0.50"))));

    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Response must not be relayed.
    EXPECT_FALSE(resp->isRelayed());
    // Make sure that the server id is present.
    EXPECT_EQ("10.0.0.1", client.config_.serverid_.toText());
    // Make sure that the client has got the lease with the requested address.
    ASSERT_EQ("10.0.0.50", client.config_.lease_.addr_.toText());

    // Simulate different client requesting the same address.
    client.modifyHWAddr();
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("10.0.0.50"))));
    resp = client.getContext().response_;
    // Make sure that the server responded.
    ASSERT_TRUE(resp);
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Response must not be relayed.
    EXPECT_FALSE(resp->isRelayed());
    // Make sure that the server id is present.
    EXPECT_EQ("10.0.0.1", client.config_.serverid_.toText());
    // Make sure that the client has got some address.
    EXPECT_NE(client.config_.lease_.addr_.toText(), "0.0.0.0");
    // Make sure that the client has got a different address than requested
    // as the requested one is already in use.
    EXPECT_NE(client.config_.lease_.addr_.toText(), "10.0.0.50");
}

TEST_F(DORATest, selectingRequestAddress) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    selectingRequestAddress();
}

TEST_F(DORATest, selectingRequestAddressMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    selectingRequestAddress();
}

void
DORATest::selectingRequestNonMatchingAddress() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DORA_CONFIGS[0], *client.getServer());

    // Perform 4-way exchange with the server.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("10.0.0.50"))));

    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Response must not be relayed.
    EXPECT_FALSE(resp->isRelayed());
    // Make sure that the server id is present.
    EXPECT_EQ("10.0.0.1", client.config_.serverid_.toText());
    // Make sure that the client has got the lease with the requested address.
    ASSERT_EQ("10.0.0.50", client.config_.lease_.addr_.toText());

    // Let's request a different address. The server should respond with
    // the one that the client already has allocated.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("10.0.0.80"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Response must not be relayed.
    EXPECT_FALSE(resp->isRelayed());
    // Make sure that the server id is present.
    EXPECT_EQ("10.0.0.1", client.config_.serverid_.toText());
    // Make sure that the client has got the lease with the address that
    // the client has recorded in the lease database.
    EXPECT_EQ("10.0.0.50", client.config_.lease_.addr_.toText());
}

TEST_F(DORATest, selectingRequestNonMatchingAddress) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    selectingRequestNonMatchingAddress();
}

TEST_F(DORATest, selectingRequestNonMatchingAddressMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    selectingRequestNonMatchingAddress();
}

void
DORATest::initRebootRequest() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DORA_CONFIGS[0], *client.getServer());
    client.includeClientId("11:22");
    // Obtain a lease from the server using the 4-way exchange.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("10.0.0.50"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Response must not be relayed.
    EXPECT_FALSE(resp->isRelayed());
    // Make sure that the server id is present.
    EXPECT_EQ("10.0.0.1", client.config_.serverid_.toText());
    // Make sure that the client has got the lease with the requested address.
    ASSERT_EQ("10.0.0.50", client.config_.lease_.addr_.toText());

    // Client has a lease in the database. Let's transition the client
    // to the INIT_REBOOT state so as the client can request the cached
    // lease using the DHCPREQUEST message.
    client.setState(Dhcp4Client::INIT_REBOOT);
    ASSERT_NO_THROW(client.doRequest());

    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Response must not be relayed.
    EXPECT_FALSE(resp->isRelayed());
    // Make sure that the server id is present.
    EXPECT_EQ("10.0.0.1", client.config_.serverid_.toText());
    // Make sure that the client has got the lease with the requested address.
    ASSERT_EQ("10.0.0.50", client.config_.lease_.addr_.toText());

    // Try to request a different address than the client has. The server
    // should respond with DHCPNAK.
    client.config_.lease_.addr_ = IOAddress("10.0.0.30");
    ASSERT_NO_THROW(client.doRequest());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    EXPECT_EQ(DHCPNAK, static_cast<int>(resp->getType()));

    // Change client identifier. The server should treat the request
    // as a request from unknown client and ignore it.
    client.includeClientId("12:34");
    ASSERT_NO_THROW(client.doRequest());
    ASSERT_FALSE(client.getContext().response_);

    // Now let's fix the IP address. The client identifier is still
    // invalid so the message should be dropped.
    client.config_.lease_.addr_ = IOAddress("10.0.0.50");
    ASSERT_NO_THROW(client.doRequest());
    ASSERT_FALSE(client.getContext().response_);

    // Restore original client identifier.
    client.includeClientId("11:22");

    // Try to request from a different HW address. This should be successful
    // because the client identifier matches.
    client.modifyHWAddr();
    ASSERT_NO_THROW(client.doRequest());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease with the requested address.
    ASSERT_EQ("10.0.0.50", client.config_.lease_.addr_.toText());
}

TEST_F(DORATest, initRebootRequest) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    initRebootRequest();
}

TEST_F(DORATest, initRebootRequestMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    initRebootRequest();
}

void
DORATest::authoritative() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DORA_CONFIGS[14], *client.getServer());
    client.includeClientId("11:22");

    // Try to renew an address that is outside the pool.
    client.setState(Dhcp4Client::RENEWING);
    client.ciaddr_ = IOAddress("10.0.0.9");
    ASSERT_NO_THROW_LOG(client.doRequest());
    // Even though we're authoritative server should not respond
    // since it does not know this address.
    ASSERT_FALSE(client.getContext().response_);

    // Obtain a lease from the server using the 4-way exchange.
    client.ciaddr_ = IOAddress("0.0.0.0");
    client.setState(Dhcp4Client::SELECTING);
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("10.0.0.50"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Response must not be relayed.
    EXPECT_FALSE(resp->isRelayed());
    // Make sure that the server id is present.
    EXPECT_EQ("10.0.0.1", client.config_.serverid_.toText());
    // Make sure that the client has got the lease with the requested address.
    ASSERT_EQ("10.0.0.50", client.config_.lease_.addr_.toText());

    // Client has a lease in the database. Let's transition the client
    // to the INIT_REBOOT state so as the client can request the cached
    // lease using the DHCPREQUEST message.
    client.setState(Dhcp4Client::INIT_REBOOT);
    ASSERT_NO_THROW(client.doRequest());

    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Response must not be relayed.
    EXPECT_FALSE(resp->isRelayed());
    // Make sure that the server id is present.
    EXPECT_EQ("10.0.0.1", client.config_.serverid_.toText());
    // Make sure that the client has got the lease with the requested address.
    ASSERT_EQ("10.0.0.50", client.config_.lease_.addr_.toText());

    // Try to request a different address than the client has. The server
    // should respond with DHCPNAK.
    client.config_.lease_.addr_ = IOAddress("10.0.0.30");
    ASSERT_NO_THROW(client.doRequest());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    EXPECT_EQ(DHCPNAK, static_cast<int>(resp->getType()));

    // Try to request another different address from an unknown subnet.
    // The server should respond with DHCPNAK.
    client.config_.lease_.addr_ = IOAddress("10.1.0.30");
    ASSERT_NO_THROW(client.doRequest());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    EXPECT_EQ(DHCPNAK, static_cast<int>(resp->getType()));

    // Change client identifier. The server should treat the request
    // as a request from unknown client and respond with DHCPNAK.
    client.includeClientId("12:34");
    client.config_.lease_.addr_ = IOAddress("10.1.0.30");
    ASSERT_NO_THROW(client.doRequest());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    EXPECT_EQ(DHCPNAK, static_cast<int>(resp->getType()));

    // Now let's fix the IP address. The client identifier is still
    // invalid so the server still responds with DHCPNAK.

    ASSERT_NO_THROW(client.doRequest());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    EXPECT_EQ(DHCPNAK, static_cast<int>(resp->getType()));

    // Restore original client identifier.
    client.includeClientId("11:22");
    client.config_.lease_.addr_ = IOAddress("10.0.0.50");

    // Try to request from a different HW address. This should be successful
    // because the client identifier matches.
    client.modifyHWAddr();
    ASSERT_NO_THROW(client.doRequest());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease with the requested address.
    ASSERT_EQ("10.0.0.50", client.config_.lease_.addr_.toText());
}

TEST_F(DORATest, authoritative) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    authoritative();
}

TEST_F(DORATest, authoritativeMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    authoritative();
}

void
DORATest::notAuthoritative() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DORA_CONFIGS[15], *client.getServer());
    client.includeClientId("11:22");

    // Try to renew an address that is outside the pool.
    client.setState(Dhcp4Client::RENEWING);
    client.ciaddr_ = IOAddress("10.0.0.9");
    ASSERT_NO_THROW_LOG(client.doRequest());
    // We are not authoritative sure that the server does
    // not respond at all.
    ASSERT_FALSE(client.getContext().response_);

    // Obtain a lease from the server using the 4-way exchange.
    client.ciaddr_ = IOAddress("0.0.0.0");
    client.setState(Dhcp4Client::SELECTING);
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("10.0.0.50"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Response must not be relayed.
    EXPECT_FALSE(resp->isRelayed());
    // Make sure that the server id is present.
    EXPECT_EQ("10.0.0.1", client.config_.serverid_.toText());
    // Make sure that the client has got the lease with the requested address.
    ASSERT_EQ("10.0.0.50", client.config_.lease_.addr_.toText());

    // Client has a lease in the database. Let's transition the client
    // to the INIT_REBOOT state so as the client can request the cached
    // lease using the DHCPREQUEST message.
    client.setState(Dhcp4Client::INIT_REBOOT);
    ASSERT_NO_THROW(client.doRequest());

    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Response must not be relayed.
    EXPECT_FALSE(resp->isRelayed());
    // Make sure that the server id is present.
    EXPECT_EQ("10.0.0.1", client.config_.serverid_.toText());
    // Make sure that the client has got the lease with the requested address.
    ASSERT_EQ("10.0.0.50", client.config_.lease_.addr_.toText());

    // Try to request a different address than the client has. The server
    // should respond with DHCPNAK.
    client.config_.lease_.addr_ = IOAddress("10.0.0.30");
    ASSERT_NO_THROW(client.doRequest());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    EXPECT_EQ(DHCPNAK, static_cast<int>(resp->getType()));

    // Try to request another different address from an unknown subnet.
    // The server should respond with DHCPNAK.
    client.config_.lease_.addr_ = IOAddress("10.1.0.30");
    ASSERT_NO_THROW(client.doRequest());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    ASSERT_EQ(DHCPNAK, static_cast<int>(resp->getType()));

    // Change client identifier. The server should treat the request
    // as a request from unknown client and not respond (no DHCPNAK).
    // Changed behavior vs authoritative!
    client.includeClientId("12:34");
    client.config_.lease_.addr_ = IOAddress("10.1.0.30");
    ASSERT_NO_THROW(client.doRequest());
    // Make sure that the server did not respond.
    EXPECT_FALSE(client.getContext().response_);

    // Now let's fix the IP address. The client identifier is still
    // invalid so the message should be dropped (no DHCPNAK).
    // Changed behavior vs authoritative!
    client.config_.lease_.addr_ = IOAddress("10.0.0.50");
    ASSERT_NO_THROW(client.doRequest());
    // Make sure that the server did not respond.
    EXPECT_FALSE(client.getContext().response_);

    // Restore original client identifier.
    client.includeClientId("11:22");
    client.config_.lease_.addr_ = IOAddress("10.0.0.50");

    // Try to request from a different HW address. This should be successful
    // because the client identifier matches.
    client.modifyHWAddr();
    ASSERT_NO_THROW(client.doRequest());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease with the requested address.
    ASSERT_EQ("10.0.0.50", client.config_.lease_.addr_.toText());
}

TEST_F(DORATest, notAuthoritative) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    notAuthoritative();
}

TEST_F(DORATest, notAuthoritativeMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    notAuthoritative();
}

void
DORATest::authoritativeSubnetSelectionFail() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DORA_CONFIGS[15], *client.getServer());
    client.includeClientId("11:22");
    client.useRelay(true, IOAddress("10.0.0.1"), IOAddress("10.0.0.2"));

    // Current configuration contains a matching subnet from which
    // the client should get a lease.
    client.doDORA();

    // Let's now reconfigure the server to remove the subnet. The
    // global authoritative flag is true.
    configure(DORA_CONFIGS[20], *client.getServer());

    // Simulate that the client is in the INIT-REBOOT state. The
    // client will request the previously assigned address and
    // remove the server-id.
    client.setState(Dhcp4Client::INIT_REBOOT);
    ASSERT_NO_THROW_LOG(client.doRequest());

    // The server should respond because it is authoritative.
    auto resp = client.getContext().response_;
    ASSERT_TRUE(resp);
    EXPECT_EQ(DHCPNAK, static_cast<int>(resp->getType()));
}

TEST_F(DORATest, authoritativeSubnetSelectionFail) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    authoritativeSubnetSelectionFail();
}

TEST_F(DORATest, authoritativeSubnetSelectionFailMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    authoritativeSubnetSelectionFail();
}

void
DORATest::notAuthoritativeSubnetSelectionFail() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DORA_CONFIGS[15], *client.getServer());
    client.includeClientId("11:22");
    client.useRelay(true, IOAddress("10.0.0.1"), IOAddress("10.0.0.2"));

    // Current configuration contains a matching subnet from which
    // the client should get a lease.
    client.doDORA();

    // Let's now reconfigure the server to remove the subnet. The
    // global authoritative flag is true.
    configure(DORA_CONFIGS[19], *client.getServer());

    // Simulate that the client is in the INIT-REBOOT state. The
    // client will request the previously assigned address and
    // remove the server-id.
    client.setState(Dhcp4Client::INIT_REBOOT);
    ASSERT_NO_THROW_LOG(client.doRequest());

    // We are not authoritative so the server does not respond
    // at all.
    EXPECT_FALSE(client.getContext().response_);
}

TEST_F(DORATest, notAuthoritativeSubnetSelectionFail) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    notAuthoritativeSubnetSelectionFail();
}

TEST_F(DORATest, notAuthoritativeSubnetSelectionFailMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    notAuthoritativeSubnetSelectionFail();
}

void
DORATest::ciaddr() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DORA_CONFIGS[0], *client.getServer());
    // Force ciaddr of Discover message to be non-zero.
    client.ciaddr_ = IOAddress("10.0.0.50");
    // Obtain a lease from the server using the 4-way exchange.
    ASSERT_NO_THROW(client.doDiscover(boost::shared_ptr<
                                      IOAddress>(new IOAddress("10.0.0.50"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPOFFER.
    ASSERT_EQ(DHCPOFFER, static_cast<int>(resp->getType()));
    // Make sure ciaddr is not set for DHCPOFFER.
    EXPECT_EQ("0.0.0.0", resp->getCiaddr().toText());

    // Obtain a lease from the server using the 4-way exchange.
    ASSERT_NO_THROW(client.doRequest());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));

    // Let's transition the client to Renewing state.
    client.setState(Dhcp4Client::RENEWING);

    // Set the unicast destination address to indicate that it is a renewal.
    client.setDestAddress(IOAddress("10.0.0.1"));
    ASSERT_NO_THROW(client.doRequest());
    // The client is sending invalid ciaddr so the server should send a NAK.
    resp = client.getContext().response_;
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // For DHCPACK the ciaddr may be 0 or may be set to the ciaddr value
    // from the client's message. Kea sets it to the latter.
    EXPECT_EQ("10.0.0.50", resp->getCiaddr().toText());

    // Replace the address held by the client. The client will request
    // the assignment of this address but the server has a different
    // address for this client.
    client.ciaddr_ = IOAddress("192.168.0.30");
    ASSERT_NO_THROW(client.doRequest());
    // The client is sending invalid ciaddr so the server should send a NAK.
    resp = client.getContext().response_;
    ASSERT_EQ(DHCPNAK, static_cast<int>(resp->getType()));
    // For DHCPNAK the ciaddr is always 0 (should not be copied) from the
    // client's message.
    EXPECT_EQ("0.0.0.0", resp->getCiaddr().toText());
}

TEST_F(DORATest, ciaddr) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    ciaddr();
}

TEST_F(DORATest, ciaddrMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    ciaddr();
}

void
DORATest::oneAllocationOverlapTest(const std::string& clientid_a,
                                   const std::string& clientid_b) {
    // Allocate a lease by client using the 4-way exchange.
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    client.includeClientId(clientid_a);
    client.setHWAddress("01:02:03:04:05:06");
    configure(DORA_CONFIGS[0], *client.getServer());
    ASSERT_NO_THROW(client.doDORA());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    Lease4Ptr lease_a = LeaseMgrFactory::instance().getLease4(client.config_.lease_.addr_);
    ASSERT_TRUE(lease_a);
    // Remember the allocated address.
    IOAddress yiaddr = lease_a->addr_;

    // Change client identifier. If parameters clientid_a and clientid_b
    // are specified correctly, this removes the client identifier from
    // client's requests if the lease has been acquired with the client
    // identifier, or adds the client identifier otherwise.
    client.includeClientId(clientid_b);

    // Check if the server will offer the same address.
    ASSERT_NO_THROW(client.doDiscover());
    resp = client.getContext().response_;
    ASSERT_TRUE(resp);
    EXPECT_EQ(yiaddr, resp->getYiaddr());

    // Client should also be able to renew its address.
    client.setState(Dhcp4Client::RENEWING);
    ASSERT_NO_THROW(client.doRequest());
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    ASSERT_EQ(yiaddr, client.config_.lease_.addr_);
}

void
DORATest::twoAllocationsOverlap() {
    // Allocate a lease by client A using the 4-way exchange.
    Dhcp4Client client_a(srv_, Dhcp4Client::SELECTING);
    client_a.includeClientId("12:34");
    client_a.setHWAddress("01:02:03:04:05:06");
    configure(DORA_CONFIGS[0], *client_a.getServer());
    ASSERT_NO_THROW(client_a.doDORA());
    // Make sure that the server responded.
    ASSERT_TRUE(client_a.getContext().response_);
    Pkt4Ptr resp_a = client_a.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp_a->getType()));

    // Make sure that the lease has been recorded by the server.
    Lease4Ptr lease_a = LeaseMgrFactory::instance().getLease4(client_a.config_.lease_.addr_);
    ASSERT_TRUE(lease_a);

    // Create client B.
    Dhcp4Client client_b(srv_, Dhcp4Client::SELECTING);
    client_b.setHWAddress("01:02:03:04:05:06");
    client_b.includeClientId("45:67");
    // Send DHCPDISCOVER and expect the response.
    ASSERT_NO_THROW(client_b.doDiscover());
    Pkt4Ptr resp_b = client_b.getContext().response_;
    // Make sure that the server has responded with DHCPOFFER.
    ASSERT_EQ(DHCPOFFER, static_cast<int>(resp_b->getType()));
    // The offered address should be different than the address which
    // was obtained by the client A.
    ASSERT_NE(resp_b->getYiaddr(), client_a.config_.lease_.addr_);

    // Make sure that the client A lease hasn't been modified.
    lease_a = LeaseMgrFactory::instance().getLease4(client_a.config_.lease_.addr_);
    ASSERT_TRUE(lease_a);

    // Now that we know that the server will avoid assigning the same
    // address that the client A has, use the 4-way exchange to actually
    // allocate some address.
    ASSERT_NO_THROW(client_b.doDORA());
    // Make sure that the server responded.
    ASSERT_TRUE(client_b.getContext().response_);
    resp_b = client_b.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp_b->getType()));
    // Again, make sure the assigned addresses are different.
    ASSERT_NE(client_b.config_.lease_.addr_, client_a.config_.lease_.addr_);

    // Make sure that the client A still has a lease.
    lease_a = LeaseMgrFactory::instance().getLease4(client_a.config_.lease_.addr_);
    ASSERT_TRUE(lease_a);

    // Make sure that the client B has a lease.
    Lease4Ptr lease_b = LeaseMgrFactory::instance().getLease4(client_b.config_.lease_.addr_);
    ASSERT_TRUE(lease_b);

    // Client B should be able to renew its address.
    client_b.setState(Dhcp4Client::RENEWING);
    ASSERT_NO_THROW(client_b.doRequest());
    ASSERT_TRUE(client_b.getContext().response_);
    resp_b = client_b.getContext().response_;
    ASSERT_EQ(DHCPACK, static_cast<int>(resp_b->getType()));
    ASSERT_NE(client_b.config_.lease_.addr_, client_a.config_.lease_.addr_);

    // Client A should also be able to renew its address.
    client_a.setState(Dhcp4Client::RENEWING);
    ASSERT_NO_THROW(client_a.doRequest());
    ASSERT_TRUE(client_a.getContext().response_);
    resp_b = client_a.getContext().response_;
    ASSERT_EQ(DHCPACK, static_cast<int>(resp_b->getType()));
    ASSERT_NE(client_a.config_.lease_.addr_, client_b.config_.lease_.addr_);
}

TEST_F(DORATest, twoAllocationsOverlap) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    twoAllocationsOverlap();
}

TEST_F(DORATest, twoAllocationsOverlapMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    twoAllocationsOverlap();
}

// This test checks the server behavior in the following situation:
// - Client A identifies itself to the server using the hardware address
//   and client identifier.
// - Client A performs the 4-way exchange and obtains a lease from the server.
// - Client B uses the same HW address as the client A, but it doesn't use
//   the client identifier.
// - Client B sends the DHCPDISCOVER to the server.
//   The server determines that there is a lease for the client A using the
//   same HW address as the client B. Server discards the client's message and
//   doesn't offer the lease for the client B to prevent allocation of the
//   lease without a unique identifier.
// - The client sends the DHCPREQUEST and the server sends the DHCPNAK for the
//   same reason.
// - Client A renews its address successfully.
TEST_F(DORATest, oneAllocationOverlap1) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    oneAllocationOverlapTest("12:34", "");
}

// This test checks the server behavior in the following situation:
// - Client A identifies itself to the server using the hardware address
//   and client identifier.
// - Client A performs the 4-way exchange and obtains a lease from the server.
// - Client B uses the same HW address as the client A, but it doesn't use
//   the client identifier.
// - Client B sends the DHCPDISCOVER to the server.
//   The server determines that there is a lease for the client A using the
//   same HW address as the client B. Server discards the client's message and
//   doesn't offer the lease for the client B to prevent allocation of the
//   lease without a unique identifier.
// - The client sends the DHCPREQUEST and the server sends the DHCPNAK for the
//   same reason.
// - Client A renews its address successfully.
TEST_F(DORATest, oneAllocationOverlap1MultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    oneAllocationOverlapTest("12:34", "");
}

// This test is similar to oneAllocationOverlap2 but this time the client A
// uses no client identifier, and the client B uses the HW address and the
// client identifier. The server behaves as previously.
TEST_F(DORATest, oneAllocationOverlap2) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    oneAllocationOverlapTest("", "12:34");
}

// This test is similar to oneAllocationOverlap2 but this time the client A
// uses no client identifier, and the client B uses the HW address and the
// client identifier. The server behaves as previously.
TEST_F(DORATest, oneAllocationOverlap2MultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    oneAllocationOverlapTest("", "12:34");
}

void
DORATest::reservation() {
    // Client A is a one which will have a reservation.
    Dhcp4Client clientA(srv_, Dhcp4Client::SELECTING);
    // Set explicit HW address so as it matches the reservation in the
    // configuration used below.
    clientA.setHWAddress("aa:bb:cc:dd:ee:ff");
    // Configure DHCP server.
    configure(DORA_CONFIGS[2], *clientA.getServer());
    // Client A performs 4-way exchange and should obtain a reserved
    // address.
    ASSERT_NO_THROW(clientA.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(clientA.getContext().response_);
    Pkt4Ptr resp = clientA.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease for the reserved address.
    ASSERT_EQ("10.0.0.7", clientA.config_.lease_.addr_.toText());

    // Client B uses the same server as Client A.
    Dhcp4Client clientB(srv_, Dhcp4Client::SELECTING);
    // Client B has no reservation so it should get the lease from
    // the dynamic pool.
    ASSERT_NO_THROW(clientB.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(clientB.getContext().response_);
    resp = clientB.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Obtain the subnet to which the returned address belongs.
    ConstSubnet4Ptr subnet = CfgMgr::instance().getCurrentCfg()->
        getCfgSubnets4()->selectSubnet(clientB.config_.lease_.addr_);
    ASSERT_TRUE(subnet);
    // Make sure that the address has been allocated from the dynamic pool.
    ASSERT_TRUE(subnet->inPool(Lease::TYPE_V4, clientB.config_.lease_.addr_));
}

TEST_F(DORATest, reservation) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    reservation();
}

TEST_F(DORATest, reservationMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    reservation();
}

void
DORATest:: reservationByDUID() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Use relay agent.
    client.useRelay(true, IOAddress("10.0.0.1"), IOAddress("10.0.0.2"));
    // Modify HW address so as the server doesn't assign reserved
    // address by HW address.
    client.modifyHWAddr();
    // Specify DUID for which address 10.0.0.8 is reserved.
    // The value specified as client id includes:
    // - FF is a client identifier type for DUID,
    // - 45454545 - represents 4 bytes for IAID
    // - 01:02:03:04:05 - is an actual DUID for which there is a
    //   reservation.
    client.includeClientId("FF:45:45:45:45:01:02:03:04:05");

    // Configure DHCP server.
    configure(DORA_CONFIGS[2], *client.getServer());
    // Client A performs 4-way exchange and should obtain a reserved
    // address.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease for the reserved address.
    ASSERT_EQ("10.0.0.8", client.config_.lease_.addr_.toText());
}

TEST_F(DORATest, reservationByDUID) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    reservationByDUID();
}

TEST_F(DORATest, reservationByDUIDMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    reservationByDUID();
}

void
DORATest::reservationByCircuitId() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Use relay agent so as the circuit-id can be inserted.
    client.useRelay(true, IOAddress("10.0.0.1"), IOAddress("10.0.0.2"));
    // Specify circuit-id.
    client.setCircuitId("charter950");

    // Configure DHCP server.
    configure(DORA_CONFIGS[2], *client.getServer());
    // Client A performs 4-way exchange and should obtain a reserved
    // address.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease for the reserved address.
    ASSERT_EQ("10.0.0.9", client.config_.lease_.addr_.toText());
}

TEST_F(DORATest, reservationByCircuitId) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    reservationByCircuitId();
}

TEST_F(DORATest, reservationByCircuitIdMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    reservationByCircuitId();
}

void
DORATest::reservationByClientId() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Use relay agent to make sure that the desired subnet is
    // selected for our client.
    client.useRelay(true, IOAddress("10.0.0.20"), IOAddress("10.0.0.21"));
    // Specify client identifier.
    client.includeClientId("01:11:22:33:44:55:66");

    // Configure DHCP server.
    configure(DORA_CONFIGS[2], *client.getServer());
    // Client A performs 4-way exchange and should obtain a reserved
    // address.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease for the reserved address.
    ASSERT_EQ("10.0.0.1", client.config_.lease_.addr_.toText());
}

TEST_F(DORATest, reservationByClientId) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    reservationByClientId();
}

TEST_F(DORATest, reservationByClientIdMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    reservationByClientId();
}

void
DORATest::hostIdentifiersOrder() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    client.setHWAddress("aa:bb:cc:dd:ee:ff");
    // Use relay agent so as the circuit-id can be inserted.
    client.useRelay(true, IOAddress("10.0.0.1"), IOAddress("10.0.0.2"));
    // Specify DUID for which address 10.0.0.8 is reserved.
    // The value specified as client id includes:
    // - FF is a client identifier type for DUID,
    // - 45454545 - represents 4 bytes for IAID
    // - 01:02:03:04:05 - is an actual DUID for which there is a
    //   reservation.
    client.includeClientId("FF:45:45:45:45:01:02:03:04:05");
    // Specify circuit-id.
    client.setCircuitId("charter950");

    // Configure DHCP server.
    configure(DORA_CONFIGS[2], *client.getServer());
    // Perform 4-way exchange to obtain reserved address.
    // The client has in fact two reserved addresses, but the one assigned
    // should be by hw-address.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease for the reserved address.
    ASSERT_EQ("10.0.0.7", client.config_.lease_.addr_.toText());

    // Reconfigure the server to change the preference order of the
    // host identifiers. The 'circuit-id' should now take precedence over
    // the hw-address, duid and client-id.
    configure(DORA_CONFIGS[4], *client.getServer());
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease for the reserved address.
    ASSERT_EQ("10.0.0.9", client.config_.lease_.addr_.toText());

    // Reconfigure the server to change the preference order of the
    // host identifiers. The 'duid' should now take precedence over
    // the client-id, hw-address and circuit-id
    configure(DORA_CONFIGS[5], *client.getServer());
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease for the reserved address.
    ASSERT_EQ("10.0.0.8", client.config_.lease_.addr_.toText());

    // Replace the client identifier with the one for which address
    // 10.0.0.1 is reserved. Because the DUID is a special type of
    // client identifier, this change effectively removes the association
    // of the client with the DUID for which address 10.0.0.8 is reserved.
    // The next identifier type to be used by the server (after DUID) is
    // client-id and thus the server should assign address 10.0.0.1.
    client.includeClientId("01:11:22:33:44:55:66");
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease for the reserved address.
    ASSERT_EQ("10.0.0.1", client.config_.lease_.addr_.toText());
}

TEST_F(DORATest, hostIdentifiersOrder) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    hostIdentifiersOrder();
}

TEST_F(DORATest, hostIdentifiersOrderMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    hostIdentifiersOrder();
}

void
DORATest::ignoreChangingClientId() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DORA_CONFIGS[3], *client.getServer());
    client.includeClientId("12:12");
    // Obtain the lease using 4-way exchange.
    ASSERT_NO_THROW(client.doDORA());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    EXPECT_FALSE(client.config_.lease_.client_id_);

    // Remember address which the client has obtained.
    IOAddress leased_address = client.config_.lease_.addr_;

    // Modify client id. Because we have set the configuration flag which
    // forces the server to lookup leases using the HW address, the
    // client id modification should not matter and the client should
    // obtain the same lease.
    client.includeClientId("14:14");
    // Obtain the lease using 4-way exchange.
    ASSERT_NO_THROW(client.doDORA());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the server assigned the same address, even though the
    // client id has changed.
    EXPECT_EQ(leased_address, client.config_.lease_.addr_);
    // Check that the client id is not present in the lease.
    EXPECT_FALSE(client.config_.lease_.client_id_);
}

TEST_F(DORATest, ignoreChangingClientId) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    ignoreChangingClientId();
}

TEST_F(DORATest, ignoreChangingClientIdMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    ignoreChangingClientId();
}

void
DORATest::changingHWAddress() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DORA_CONFIGS[3], *client.getServer());
    client.includeClientId("12:12");
    client.setHWAddress("00:01:02:03:04:05");
    // Obtain the lease using 4-way exchange.
    ASSERT_NO_THROW(client.doDORA());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Check that the client id is not present in the lease.
    EXPECT_FALSE(client.config_.lease_.client_id_);

    // Remember address which the client has obtained.
    IOAddress leased_address = client.config_.lease_.addr_;

    // Modify HW address but leave client id in place. The value of the
    // match-client-id set to false must not have any effect on the
    // case when the HW address is changing. In such case the server will
    // allocate the new address for the client.
    client.setHWAddress("01:01:01:01:01:01");
    // Obtain a lease.
    ASSERT_NO_THROW(client.doDORA());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Client must assign different address because the client id is
    // ignored and the HW address was changed.
    EXPECT_NE(client.config_.lease_.addr_, leased_address);
    // Check that the client id is not present in the lease.
    EXPECT_FALSE(client.config_.lease_.client_id_);
}

TEST_F(DORATest, changingHWAddress) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    changingHWAddress();
}

TEST_F(DORATest, changingHWAddressMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    changingHWAddress();
}

void
DORATest::messageFieldsReservations() {
    // Client has a reservation.
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Set explicit HW address so as it matches the reservation in the
    // configuration used below.
    client.setHWAddress("aa:bb:cc:dd:ee:ff");
    // Configure DHCP server.
    configure(DORA_CONFIGS[6], *client.getServer());
    // Client performs 4-way exchange and should obtain a reserved
    // address and fixed fields.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));

    // Check that the reserved values have been assigned.
    EXPECT_EQ("10.0.0.7", client.config_.siaddr_.toText());
    EXPECT_EQ("some-name.example.org", client.config_.sname_);
    EXPECT_EQ("bootfile.efi", client.config_.boot_file_name_);
}

TEST_F(DORATest, messageFieldsReservations) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    messageFieldsReservations();
}

TEST_F(DORATest, messageFieldsReservationsMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    messageFieldsReservations();
}

void
DORATest::reservationsWithConflicts(int offer_lifetime /* = 0 */) {
    ElementPtr offer_lft(Element::create(offer_lifetime));
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DORA_CONFIGS[0], *client.getServer());
    CfgMgr::instance().getCurrentCfg()->addConfiguredGlobal("offer-lifetime", offer_lft);

    // Client A performs 4-way exchange and obtains a lease from the
    // dynamic pool.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("10.0.0.50"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease with the requested address.
    ASSERT_EQ("10.0.0.50", client.config_.lease_.addr_.toText());

    configure(DORA_CONFIGS[0], false);
    // Reservation is created for the client A using an address out of the
    // dynamic pool.
    HostPtr host(new Host(&client.getHWAddress()->hwaddr_[0],
                          client.getHWAddress()->hwaddr_.size(),
                          Host::IDENT_HWADDR, SubnetID(1),
                          SUBNET_ID_UNUSED, IOAddress("10.0.0.9")));
    CfgMgr::instance().getStagingCfg()->getCfgHosts()->add(host);
    CfgMgr::instance().getStagingCfg()->addConfiguredGlobal("offer-lifetime", offer_lft);
    CfgMgr::instance().commit();

    // Let's transition the client to Renewing state.
    client.setState(Dhcp4Client::RENEWING);

    // Set the unicast destination address to indicate that it is a renewal.
    client.setDestAddress(IOAddress("10.0.0.1"));
    ASSERT_NO_THROW(client.doRequest());

    // Client should get the DHCPNAK from the server because the client has
    // a reservation for a different address that it is trying to renew.
    resp = client.getContext().response_;
    ASSERT_EQ(DHCPNAK, static_cast<int>(resp->getType()));

    // A conforming client would go back to the server discovery.
    client.setState(Dhcp4Client::SELECTING);
    // Obtain a lease from the server using the 4-way exchange.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK with a reserved
    // address
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    ASSERT_EQ("10.0.0.9", client.config_.lease_.addr_.toText());

    // Client A renews the allocated address.
    client.setState(Dhcp4Client::RENEWING);
    // Set the unicast destination address to indicate that it is a renewal.
    client.setDestAddress(IOAddress("10.0.0.1"));
    ASSERT_NO_THROW(client.doRequest());
    // Make sure the server responded and renewed the client's address.
    resp = client.getContext().response_;
    ASSERT_EQ("10.0.0.9", client.config_.lease_.addr_.toText());

    // By reconfiguring the server, we remove the existing reservations.
    configure(DORA_CONFIGS[0]);
    CfgMgr::instance().getCurrentCfg()->addConfiguredGlobal("offer-lifetime", offer_lft);

    // Try to renew the existing lease again.
    ASSERT_NO_THROW(client.doRequest());

    // The reservation has been removed. Since address that the client is
    // using doesn't belong to a dynamic pool and the server is not
    // authoritative it should not send a DHCPNAK.
    resp = client.getContext().response_;
    ASSERT_FALSE(resp);

    // A conforming client would go back to the server discovery.
    client.setState(Dhcp4Client::SELECTING);
    // Obtain a lease from the server using the 4-way exchange.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Obtain the subnet to which the returned address belongs.
    ConstSubnet4Ptr subnet = CfgMgr::instance().getCurrentCfg()->
        getCfgSubnets4()->selectSubnet(client.config_.lease_.addr_);
    ASSERT_TRUE(subnet);
    // Make sure that the address has been allocated from the dynamic pool.
    ASSERT_TRUE(subnet->inPool(Lease::TYPE_V4, client.config_.lease_.addr_));

    // Remember the address allocated in the dynamic pool.
    IOAddress in_pool_addr = client.config_.lease_.addr_;

    // Create Client B.
    Dhcp4Client clientB(srv_);
    clientB.modifyHWAddr();

    // Create reservation for the Client B, for the address that the
    // Client A is using.
    configure(DORA_CONFIGS[0], false);
    host.reset(new Host(&clientB.getHWAddress()->hwaddr_[0],
                        clientB.getHWAddress()->hwaddr_.size(),
                        Host::IDENT_HWADDR, SubnetID(1),
                        SUBNET_ID_UNUSED, in_pool_addr));
    CfgMgr::instance().getStagingCfg()->getCfgHosts()->add(host);
    CfgMgr::instance().getStagingCfg()->addConfiguredGlobal("offer-lifetime", offer_lft);
    CfgMgr::instance().commit();

    // Client B performs a DHCPDISCOVER.
    clientB.setState(Dhcp4Client::SELECTING);
    // The server determines that the address reserved for Client B is
    // in use by Client A so it offers a different address.
    ASSERT_NO_THROW(clientB.doDORA(boost::shared_ptr<
                                   IOAddress>(new IOAddress("0.0.0.0"))));
    ASSERT_TRUE(clientB.getContext().response_);
    ASSERT_EQ(DHCPACK, static_cast<int>(clientB.getContext().response_->getType()));
    IOAddress client_b_addr = clientB.config_.lease_.addr_;
    ASSERT_NE(client_b_addr, in_pool_addr);
    // Ensure stats are being recorded for HR conflicts
    ObservationPtr subnet_conflicts = StatsMgr::instance().getObservation(
        "subnet[1].v4-reservation-conflicts");
    ASSERT_TRUE(subnet_conflicts);
    ASSERT_EQ(1, subnet_conflicts->getInteger().first);
    subnet_conflicts = StatsMgr::instance().getObservation("v4-reservation-conflicts");
    ASSERT_TRUE(subnet_conflicts);
    ASSERT_EQ(1, subnet_conflicts->getInteger().first);

    // Client A renews the lease.
    client.setState(Dhcp4Client::RENEWING);
    // Set the unicast destination address to indicate that it is a renewal.
    client.setDestAddress(IOAddress(in_pool_addr));
    ASSERT_NO_THROW(client.doRequest());
    // Client A should get a DHCPNAK because it is using an address reserved
    // for Client B.
    resp = client.getContext().response_;
    ASSERT_EQ(DHCPNAK, static_cast<int>(resp->getType()));

    // Client B performs 4-way exchange but still gets an address from the
    // dynamic pool, because Client A hasn't obtained a new lease, so it is
    // still using an address reserved for Client B.
    clientB.setState(Dhcp4Client::SELECTING);
    // Obtain a lease from the server using the 4-way exchange.
    ASSERT_NO_THROW(clientB.doDORA(boost::shared_ptr<
                                   IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(clientB.getContext().response_);
    ASSERT_EQ(DHCPACK, static_cast<int>(clientB.getContext().response_->getType()));
    ASSERT_NE(clientB.config_.lease_.addr_, in_pool_addr);
    ASSERT_EQ(client_b_addr, clientB.config_.lease_.addr_);
    // Ensure stats are being recorded for HR conflicts
    subnet_conflicts = StatsMgr::instance().getObservation(
        "subnet[1].v4-reservation-conflicts");
    ASSERT_TRUE(subnet_conflicts);
    ASSERT_EQ(2, subnet_conflicts->getInteger().first);
    subnet_conflicts = StatsMgr::instance().getObservation("v4-reservation-conflicts");
    ASSERT_TRUE(subnet_conflicts);
    ASSERT_EQ(2, subnet_conflicts->getInteger().first);

    // Client B renews its lease.
    clientB.setState(Dhcp4Client::RENEWING);
    clientB.setDestAddress(IOAddress("10.0.0.1"));
    ASSERT_NO_THROW(clientB.doRequest());
    // The server should renew the client's B lease because the address
    // reserved for client B is still in use by the client A.
    ASSERT_TRUE(clientB.getContext().response_);
    EXPECT_EQ(DHCPACK, static_cast<int>(clientB.getContext().response_->getType()));
    ASSERT_NE(clientB.config_.lease_.addr_, in_pool_addr);
    ASSERT_EQ(client_b_addr, clientB.config_.lease_.addr_);

    // Client A performs 4-way exchange.
    client.setState(Dhcp4Client::SELECTING);
    // Revert to the broadcast address for the selecting client.
    client.setDestAddress(IOAddress::IPV4_BCAST_ADDRESS());
    // Obtain a lease from the server using the 4-way exchange.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // The server should have assigned a different address than the one
    // reserved for the Client B.
    ASSERT_NE(client.config_.lease_.addr_, in_pool_addr);
    subnet = CfgMgr::instance().getCurrentCfg()->getCfgSubnets4()->
        selectSubnet(client.config_.lease_.addr_);
    ASSERT_TRUE(subnet);
    ASSERT_TRUE(subnet->inPool(Lease::TYPE_V4, client.config_.lease_.addr_));

    // Client B renews again.
    ASSERT_NO_THROW(clientB.doRequest());
    // The client B should now receive the DHCPNAK from the server because
    // the reserved address is now available and the client should
    // revert to the DHCPDISCOVER to obtain it.
    ASSERT_TRUE(clientB.getContext().response_);
    EXPECT_EQ(DHCPNAK, static_cast<int>(clientB.getContext().response_->getType()));

    // Client B performs 4-way exchange and obtains a lease for the
    // reserved address.
    clientB.setState(Dhcp4Client::SELECTING);
    ASSERT_NO_THROW(clientB.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(clientB.getContext().response_);
    resp = clientB.getContext().response_;
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    ASSERT_EQ(in_pool_addr, clientB.config_.lease_.addr_);
}

TEST_F(DORATest, reservationsWithConflicts) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    reservationsWithConflicts();
}

TEST_F(DORATest, reservationsWithConflictsMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    reservationsWithConflicts();
}

TEST_F(DORATest, reservationsWithConflictsOfferLft) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    reservationsWithConflicts(120);
}

TEST_F(DORATest, reservationsWithConflictsOfferLftMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    reservationsWithConflicts(120);
}

void
DORATest::reservationModeDisabled() {
    // Client has a reservation.
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Set explicit HW address so as it matches the reservation in the
    // configuration used below.
    client.setHWAddress("aa:bb:cc:dd:ee:ff");
    // Configure DHCP server. In this configuration the reservations flags are
    // set to false. Thus, the server should ignore the reservation for
    // this client.
    configure(DORA_CONFIGS[12], *client.getServer());
    // Client requests the 10.0.0.50 address and the server should assign it
    // as it ignores the reservation in the current mode.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("10.0.0.50"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));

    // Check that the requested IP address was assigned.
    ASSERT_EQ("10.0.0.50", client.config_.lease_.addr_.toText());

    // Reconfigure the server to respect the host reservations.
    configure(DORA_CONFIGS[11], *client.getServer());

    // The client requests the previously allocated address again, but the
    // server should allocate the reserved address this time.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("10.0.0.50"))));
    // Check that the reserved IP address has been assigned.
    ASSERT_EQ("10.0.0.65", client.config_.lease_.addr_.toText());
}

TEST_F(DORATest, reservationModeDisabled) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    reservationModeDisabled();
}

TEST_F(DORATest, reservationModeDisabledMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    reservationModeDisabled();
}

void
DORATest::reservationIgnoredInDisabledMode() {
    // Client has a reservation.
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Set MAC address which doesn't match the reservation configured.
    client.setHWAddress("11:22:33:44:55:66");
    // Configure DHCP server. In this configuration the reservations flags are
    // disabled. Any client should be able to hijack the reserved address.
    configure(DORA_CONFIGS[13], *client.getServer());
    // Client requests the 10.0.0.65 address reserved for another client.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                  IOAddress>(new IOAddress("10.0.0.65"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));

    // Check that the address was hijacked.
    ASSERT_EQ("10.0.0.65", client.config_.lease_.addr_.toText());
}

TEST_F(DORATest, reservationIgnoredInDisabledMode) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    reservationIgnoredInDisabledMode();
}

TEST_F(DORATest, reservationIgnoredInDisabledModeMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    reservationIgnoredInDisabledMode();
}

void
DORATest::reservationModeOutOfPool() {
    // Create the first client for which we have a reservation out of the
    // dynamic pool.
    Dhcp4Client clientA(srv_, Dhcp4Client::SELECTING);
    clientA.setHWAddress("aa:bb:cc:dd:ee:ff");
    // Configure the server to respect out of the pool reservations.
    configure(DORA_CONFIGS[13], *clientA.getServer());
    // The client for which we have a reservation is doing 4-way exchange
    // and requests a different address than reserved. The server should
    // allocate the reserved address to this client.
    ASSERT_NO_THROW(clientA.doDORA(boost::shared_ptr<
                                   IOAddress>(new IOAddress("10.0.0.40"))));
    // Make sure that the server responded.
    ASSERT_TRUE(clientA.getContext().response_);
    Pkt4Ptr resp = clientA.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Check that the server allocated the reserved address.
    ASSERT_EQ("10.0.0.200", clientA.config_.lease_.addr_.toText());

    // Create another client which has a reservation within the pool.
    // The server should ignore this reservation in the current mode.
    Dhcp4Client clientB(srv_, Dhcp4Client::SELECTING);
    clientB.setHWAddress("11:22:33:44:55:66");
    // This client is requesting a different address than reserved. The
    // server should allocate this address to the client.
    ASSERT_NO_THROW(clientB.doDORA(boost::shared_ptr<
                                   IOAddress>(new IOAddress("10.0.0.40"))));
    // Make sure that the server responded.
    ASSERT_TRUE(clientB.getContext().response_);
    resp = clientB.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Check that the requested address was assigned.
    ASSERT_EQ("10.0.0.40", clientB.config_.lease_.addr_.toText());
}

TEST_F(DORATest, reservationModeOutOfPool) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    reservationModeOutOfPool();
}

TEST_F(DORATest, reservationModeOutOfPoolMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    reservationModeOutOfPool();
}

void
DORATest::reservationIgnoredInOutOfPoolMode() {
    // Create the first client for which we have a reservation out of the
    // dynamic pool.
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    client.setHWAddress("12:34:56:78:9A:BC");
    // Configure the server to respect out of the pool reservations only.
    configure(DORA_CONFIGS[14], *client.getServer());
    // The client which doesn't have a reservation is trying to hijack
    // the reserved address and it should succeed.
    ASSERT_NO_THROW(client.doDORA(boost::shared_ptr<
                                   IOAddress>(new IOAddress("10.0.0.65"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Check that the server allocated the requested address.
    ASSERT_EQ("10.0.0.65", client.config_.lease_.addr_.toText());
}

TEST_F(DORATest, reservationIgnoredInOutOfPoolMode) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    reservationIgnoredInOutOfPoolMode();
}

TEST_F(DORATest, reservationIgnoredInOutOfPoolModeMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    reservationIgnoredInOutOfPoolMode();
}

void
DORATest::statisticsDORA() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DORA_CONFIGS[0], *client.getServer());

    // Perform 4-way exchange with the server but to not request any
    // specific address in the DHCPDISCOVER message.
    ASSERT_NO_THROW(client.doDORA());

    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));

    // Ok, let's check the statistics.
    using namespace isc::stats;
    StatsMgr& mgr = StatsMgr::instance();
    ObservationPtr pkt4_received = mgr.getObservation("pkt4-received");
    ObservationPtr pkt4_discover_received = mgr.getObservation("pkt4-discover-received");
    ObservationPtr pkt4_offer_sent = mgr.getObservation("pkt4-offer-sent");
    ObservationPtr pkt4_request_received = mgr.getObservation("pkt4-request-received");
    ObservationPtr pkt4_ack_sent = mgr.getObservation("pkt4-ack-sent");
    ObservationPtr pkt4_sent = mgr.getObservation("pkt4-sent");

    // All expected statistics must be present.
    ASSERT_TRUE(pkt4_received);
    ASSERT_TRUE(pkt4_discover_received);
    ASSERT_TRUE(pkt4_offer_sent);
    ASSERT_TRUE(pkt4_request_received);
    ASSERT_TRUE(pkt4_ack_sent);
    ASSERT_TRUE(pkt4_sent);

    // They also must have expected values.
    EXPECT_EQ(2, pkt4_received->getInteger().first);
    EXPECT_EQ(1, pkt4_discover_received->getInteger().first);
    EXPECT_EQ(1, pkt4_offer_sent->getInteger().first);
    EXPECT_EQ(1, pkt4_request_received->getInteger().first);
    EXPECT_EQ(1, pkt4_ack_sent->getInteger().first);
    EXPECT_EQ(2, pkt4_sent->getInteger().first);

    // Let the client send request 3 times, which should make the server
    // to send 3 acks.
    client.setState(Dhcp4Client::RENEWING);
    ASSERT_NO_THROW(client.doRequest());
    ASSERT_NO_THROW(client.doRequest());
    ASSERT_NO_THROW(client.doRequest());

    // Let's see if the stats are properly updated.
    EXPECT_EQ(5, pkt4_received->getInteger().first);
    EXPECT_EQ(1, pkt4_discover_received->getInteger().first);
    EXPECT_EQ(1, pkt4_offer_sent->getInteger().first);
    EXPECT_EQ(4, pkt4_request_received->getInteger().first);
    EXPECT_EQ(4, pkt4_ack_sent->getInteger().first);
    EXPECT_EQ(5, pkt4_sent->getInteger().first);
}

TEST_F(DORATest, statisticsDORA) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    statisticsDORA();
}

TEST_F(DORATest, statisticsDORAMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    statisticsDORA();
}

void
DORATest::statisticsNAK() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    configure(DORA_CONFIGS[0], *client.getServer());
    // Obtain a lease from the server using the 4-way exchange.

    // Get a lease.
    client.doDORA();

    // Wipe all stats.
    isc::stats::StatsMgr::instance().removeAll();

    client.setState(Dhcp4Client::INIT_REBOOT);
    client.config_.lease_.addr_ = IOAddress("10.0.0.30");
    ASSERT_NO_THROW(client.doRequest());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    EXPECT_EQ(DHCPNAK, static_cast<int>(resp->getType()));

    using namespace isc::stats;
    StatsMgr& mgr = StatsMgr::instance();
    ObservationPtr pkt4_received = mgr.getObservation("pkt4-received");
    ObservationPtr pkt4_request_received = mgr.getObservation("pkt4-request-received");
    ObservationPtr pkt4_ack_sent = mgr.getObservation("pkt4-ack-sent");
    ObservationPtr pkt4_nak_sent = mgr.getObservation("pkt4-nak-sent");
    ObservationPtr pkt4_sent = mgr.getObservation("pkt4-sent");

    // All expected statistics must be present.
    ASSERT_TRUE(pkt4_received);
    ASSERT_TRUE(pkt4_request_received);
    ASSERT_FALSE(pkt4_ack_sent); // No acks were sent, no such statistic expected.
    ASSERT_TRUE(pkt4_nak_sent);
    ASSERT_TRUE(pkt4_sent);

    // They also must have expected values.
    EXPECT_EQ(1, pkt4_received->getInteger().first);
    EXPECT_EQ(1, pkt4_request_received->getInteger().first);
    EXPECT_EQ(1, pkt4_nak_sent->getInteger().first);
    EXPECT_EQ(1, pkt4_sent->getInteger().first);
}

TEST_F(DORATest, statisticsNAK) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    statisticsNAK();
}

TEST_F(DORATest, statisticsNAKMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    statisticsNAK();
}

void
DORATest::testMultiStageBoot(const unsigned int config_index) {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    ASSERT_NO_THROW(configure(DORA_CONFIGS[config_index],
                              *client.getServer()));

    // Stage 1: get the first lease for our client. In PXE boot, it would be
    // a stage when the BIOS requests a lease.

    // Include client id apart from the MAC address.
    client.includeClientId("10:21:32:AB:CD:EF");

    ASSERT_NO_THROW(client.doDORA());

    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));

    // Make sure that the client has got the lease which belongs
    // to a pool.
    IOAddress leased_address1 = client.config_.lease_.addr_;
    ConstSubnet4Ptr subnet = CfgMgr::instance().getCurrentCfg()->
        getCfgSubnets4()->selectSubnet(leased_address1);
    ASSERT_TRUE(subnet);
    // Make sure that the address has been allocated from the dynamic pool.
    ASSERT_TRUE(subnet->inPool(Lease::TYPE_V4, leased_address1));

    // Stage 2: the client with a given MAC address has a lease in the
    // lease database. The installer comes up and uses the same MAC address
    // but generates a different client id. The server should treat the
    // client with modified client identifier as a different client and
    // create a new lease for it.

    // Modify client identifier.
    client.includeClientId("11:54:45:AB:AA:FE");

    ASSERT_NO_THROW(client.doDORA());

    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));

    // Make sure that the client has got the lease which belongs
    // to a pool.
    IOAddress leased_address2 = client.config_.lease_.addr_;
    // Make sure that the address has been allocated from the dynamic pool.
    ASSERT_TRUE(subnet->inPool(Lease::TYPE_V4, leased_address2));

    // The client should have got a new lease.
    ASSERT_NE(leased_address1, leased_address2);

    // Modify client identifier again.
    client.includeClientId("22:34:AC:BE:44:54");

    ASSERT_NO_THROW(client.doDORA());

    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));

    // Make sure that the client has got the lease which belongs
    // to a pool.
    IOAddress leased_address3 = client.config_.lease_.addr_;
    // Make sure that the address has been allocated from the dynamic pool.
    ASSERT_TRUE(subnet->inPool(Lease::TYPE_V4, leased_address3));

    // The client should have got a new lease.
    ASSERT_NE(leased_address1, leased_address3);
    ASSERT_NE(leased_address2, leased_address3);
}

// Test that the client using the same hardware address but multiple
// client identifiers will obtain multiple leases.
TEST_F(DORATest, multiStageBoot) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    // DORA_CONFIGS[0] to be used for server configuration.
    testMultiStageBoot(0);
}

// Test that the client using the same hardware address but multiple
// client identifiers will obtain multiple leases.
TEST_F(DORATest, multiStageBootMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    // DORA_CONFIGS[0] to be used for server configuration.
    testMultiStageBoot(0);
}

void
DORATest::customServerIdentifier() {
    Dhcp4Client client1(srv_, Dhcp4Client::SELECTING);
    // Configure DHCP server.
    ASSERT_NO_THROW(configure(DORA_CONFIGS[7], *client1.getServer()));

    ASSERT_NO_THROW(client1.doDORA());
    // Make sure that the server responded.
    ASSERT_TRUE(client1.getContext().response_);
    Pkt4Ptr resp = client1.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // The explicitly configured server identifier should take precedence
    // over generated server identifier.
    EXPECT_EQ("1.2.3.4", client1.config_.serverid_.toText());

    // Repeat the test for different subnet.
    Dhcp4Client client2(srv_, Dhcp4Client::SELECTING);
    client2.setIfaceName("eth1");
    client2.setIfaceIndex(ETH1_INDEX);

    ASSERT_NO_THROW(client2.doDORA());
    ASSERT_TRUE(client2.getContext().response_);
    resp = client2.getContext().response_;
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    EXPECT_EQ("2.3.4.5", client2.config_.serverid_.toText());

    // Create relayed client which will be assigned a lease from the third
    // subnet. This subnet inherits server identifier value from the global
    // scope.
    Dhcp4Client client3(srv_, Dhcp4Client::SELECTING);
    client3.useRelay(true, IOAddress("10.2.3.4"));

    ASSERT_NO_THROW(client3.doDORA());
    ASSERT_TRUE(client3.getContext().response_);
    resp = client3.getContext().response_;
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    EXPECT_EQ("3.4.5.6", client3.config_.serverid_.toText());
}

TEST_F(DORATest, customServerIdentifier) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    customServerIdentifier();
}

TEST_F(DORATest, customServerIdentifierMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    customServerIdentifier();
}

void
DORATest::changingCircuitId() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    client.setHWAddress("aa:bb:cc:dd:ee:ff");
    // Use relay agent so as the circuit-id can be inserted.
    client.useRelay(true, IOAddress("10.0.0.1"), IOAddress("10.0.0.2"));

    // Configure DHCP server.
    configure(DORA_CONFIGS[10], *client.getServer());

    // Send DHCPDISCOVER.
    boost::shared_ptr<IOAddress> requested_address(new IOAddress("10.0.0.9"));
    ASSERT_NO_THROW(client.doDiscover(requested_address));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPOFFER
    ASSERT_EQ(DHCPOFFER, static_cast<int>(resp->getType()));
    // Make sure that the client has been offered a different address
    // given that circuit-id is not used.
    EXPECT_NE("10.0.0.9", resp->getYiaddr().toText());

    // Specify circuit-id matching the one in the configuration.
    client.setCircuitId("charter950");

    // Send DHCPDISCOVER.
    ASSERT_NO_THROW(client.doDiscover());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPOFFER
    ASSERT_EQ(DHCPOFFER, static_cast<int>(resp->getType()));
    // Make sure that the client has been offered reserved address given that
    // matching circuit-id has been specified.
    EXPECT_EQ("10.0.0.9", resp->getYiaddr().toText());

    // Let's now change the circuit-id.
    client.setCircuitId("gdansk");

    // The client requests offered address but should be refused this address
    // given that the circuit-id is not matching.
    ASSERT_NO_THROW(client.doRequest());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // The client should be refused this address.
    EXPECT_EQ(DHCPNAK, static_cast<int>(resp->getType()));

    // In this case, the client falls back to the 4-way exchange and should be
    // allocated an address from the dynamic pool.
    ASSERT_NO_THROW(client.doDORA());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // The client should be allocated some address.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    EXPECT_NE("10.0.0.9", client.config_.lease_.addr_.toText());
}

TEST_F(DORATest, changingCircuitId) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    changingCircuitId();
}

TEST_F(DORATest, changingCircuitIdMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    changingCircuitId();
}

void
DORATest::storeExtendedInfoEnabled() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Use relay agent to make sure that the desired subnet is
    // selected for our client.
    client.useRelay(true, IOAddress("10.0.0.20"), IOAddress("10.0.0.21"));
    // Specify client identifier.
    client.includeClientId("01:11:22:33:44:55:66");
    // Set the circuit id to make relay-agent-info more interesting.
    client.setCircuitId("charter950");

    // Configure DHCP server.
    configure(DORA_CONFIGS[16], *client.getServer());
    // Client A performs 4-way exchange and should obtain a reserved
    // address.
    ASSERT_NO_THROW_LOG(client.doDORA(boost::shared_ptr<
                                      IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease for the reserved address.
    ASSERT_EQ("10.0.0.10", client.config_.lease_.addr_.toText());

    // The stored lease should have extended info.
    Lease4Ptr lease = LeaseMgrFactory::instance().getLease4(client.config_.lease_.addr_);
    ASSERT_TRUE(lease);
    ASSERT_TRUE(lease->getContext());

    // Make user the user-context content is correct.
    std::string expected_context = "{ \"ISC\": { \"relay-agent-info\": {"
                                   " \"sub-options\":"
                                   " \"0x010A63686172746572393530\" } } }";
    stringstream ss;
    ss << *(lease->getContext());
    ASSERT_EQ(expected_context, ss.str());
}

TEST_F(DORATest, storeExtendedInfoEnabled) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    storeExtendedInfoEnabled();
}

TEST_F(DORATest, storeExtendedInfoEnabledMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    storeExtendedInfoEnabled();
}

void
DORATest::storeExtendedInfoDisabled() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Use relay agent to make sure that the desired subnet is
    // selected for our client.
    client.useRelay(true, IOAddress("192.0.2.20"), IOAddress("192.0.2.21"));
    // Specify client identifier.
    client.includeClientId("01:11:22:33:44:55:66");
    // Set the circuit id to make relay-agent-info more interesting.
    client.setCircuitId("charter950");

    // Configure DHCP server.
    configure(DORA_CONFIGS[16], *client.getServer());
    // Client A performs 4-way exchange and should obtain a reserved
    // address.
    ASSERT_NO_THROW_LOG(client.doDORA(boost::shared_ptr<
                                      IOAddress>(new IOAddress("0.0.0.0"))));
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease for the reserved address.
    ASSERT_EQ("192.0.2.10", client.config_.lease_.addr_.toText());

    // The stored lease should not have extended info.
    Lease4Ptr lease = LeaseMgrFactory::instance().getLease4(client.config_.lease_.addr_);
    ASSERT_TRUE(lease);
    ASSERT_FALSE(lease->getContext());
}

TEST_F(DORATest, storeExtendedInfoDisabled) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    storeExtendedInfoDisabled();
}

TEST_F(DORATest, storeExtendedInfoDisabledMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    storeExtendedInfoDisabled();
}

void
DORATest::randomAllocation() {
    // Create the base client and server configuration.
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    configure(DORA_CONFIGS[17], *client.getServer());

    // Record what addresses have been allocated and in what order.
    std::set<std::string> allocated_set;
    std::vector<IOAddress> allocated_list;

    // Simulate allocations from different clients.
    for (auto i = 0; i < 30; ++i) {
        // Create a client from the base client.
        Dhcp4Client next_client(srv_, Dhcp4Client::SELECTING);
        // Run 4-way exchange.
        ASSERT_NO_THROW(next_client.doDORA());
        // Make sure that the server responded.
        ASSERT_TRUE(next_client.getContext().response_);
        auto resp = next_client.getContext().response_;
        // Make sure that the server has responded with DHCPACK.
        ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
        // Remember allocated address uniqueness and order.
        allocated_set.insert(next_client.config_.lease_.addr_.toText());
        allocated_list.push_back(next_client.config_.lease_.addr_);
    }
    // Make sure that we have 30 distinct allocations.
    ASSERT_EQ(30, allocated_set.size());
    ASSERT_EQ(30, allocated_list.size());

    // Make sure that the addresses are not allocated iteratively.
    size_t consecutives = 0;
    for (size_t i = 1; i < allocated_list.size(); ++i) {
        // Record the cases when the previously allocated address is
        // lower by 1 (iterative allocation). Some cases like this are
        // possible even with the random allocation but they should be
        // very rare.
        if (allocated_list[i].toUint32() == allocated_list[i-1].toUint32()+1) {
            ++consecutives;
        }
    }
    // Make sure we don't have too many allocations when previously
    // allocated address is the current address minus one.
    EXPECT_LT(consecutives, 10);
}

TEST_F(DORATest, randomAllocation) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    randomAllocation();
}

TEST_F(DORATest, randomAllocationMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    randomAllocation();
}

void
DORATest::leaseCaching() {
    StatsMgr::instance().setValue("v4-lease-reuses", int64_t(0));
    // Configure a DHCP client.
    Dhcp4Client client(srv_);
    client.includeClientId("11:22");
    // Include the Client FQDN option.
    ASSERT_NO_THROW(client.includeFQDN((Option4ClientFqdn::FLAG_S
                                       | Option4ClientFqdn::FLAG_E),
                                       "first.example.com.",
                                       Option4ClientFqdn::FULL));
    // Configure a DHCP server.
    configure(DORA_CONFIGS[18], *client.getServer());

    ASSERT_NO_THROW(client.getServer()->startD2());
    auto& d2_mgr = CfgMgr::instance().getD2ClientMgr();
    ASSERT_EQ(0, d2_mgr.getQueueSize());

    // Obtain a lease from the server using the 4-way exchange.
    ASSERT_NO_THROW(client.doDORA());

    // Make sure that the server responded.
    Pkt4Ptr response(client.getContext().response_);
    ASSERT_TRUE(response);

    // Make sure that the server has responded with DHCPACK.
    EXPECT_EQ(DHCPACK, response->getType());

    // Make sure that the server ID is present.
    EXPECT_EQ("10.0.0.1", client.config_.serverid_.toText());

    // Make sure that the client has got the first lease in the pool.
    ASSERT_EQ("10.0.0.10", client.config_.lease_.addr_.toText());

    // There should only be the default global statistic point.
    checkStat("v4-lease-reuses", 1, 0);

    // There should only be the default subnet statistic point.
    checkStat("subnet[1].v4-lease-reuses", 1, 0);

    // There should only be the default statistic point in the other subnet.
    checkStat("subnet[2].v4-lease-reuses", 1, 0);

    // There should be a single NCR.
    ASSERT_EQ(1, d2_mgr.getQueueSize());
    // Clear the NCR queue.
    ASSERT_NO_THROW(d2_mgr.runReadyIO());
    ASSERT_EQ(0, d2_mgr.getQueueSize());

    // Assume the client enters init-reboot and does a request.
    client.setState(Dhcp4Client::INIT_REBOOT);
    ASSERT_NO_THROW(client.doRequest());

    // Make sure that the server responded.
    response = client.getContext().response_;
    ASSERT_TRUE(response);

    // Make sure that the server has responded with DHCPACK.
    EXPECT_EQ(DHCPACK, response->getType());

    // Make sure that the server ID is present.
    EXPECT_EQ("10.0.0.1", client.config_.serverid_.toText());

    // Make sure that the client has got the same lease.
    ASSERT_EQ("10.0.0.10", client.config_.lease_.addr_.toText());

    // There should be one global lease reuse.
    checkStat("v4-lease-reuses", 2, 1);

    // There should be one subnet lease reuse.
    checkStat("subnet[1].v4-lease-reuses", 2, 1);

    // Statistics for the other subnet should not be affected.
    checkStat("subnet[2].v4-lease-reuses", 1, 0);

    // There should be no NCRs queued.
    ASSERT_EQ(0, d2_mgr.getQueueSize());

    // Let's say the client suddenly decides to do a full DORA.
    ASSERT_NO_THROW(client.doDORA());

    // Make sure that the server responded.
    response = client.getContext().response_;
    ASSERT_TRUE(response);

    // Make sure that the server has responded with DHCPACK.
    EXPECT_EQ(DHCPACK, response->getType());

    // Make sure that the server ID is present.
    EXPECT_EQ("10.0.0.1", client.config_.serverid_.toText());

    // Make sure that the client has got the same lease.
    ASSERT_EQ("10.0.0.10", client.config_.lease_.addr_.toText());

    // There should be three global lease reuses (REQUEST + DISCOVER + REQUEST).
    checkStat("v4-lease-reuses", 4, 3);

    // There should be three subnet lease reuses (REQUEST + DISCOVER + REQUEST).
    checkStat("subnet[1].v4-lease-reuses", 4, 3);

    // Statistics for the other subnet should not be affected.
    checkStat("subnet[2].v4-lease-reuses", 1, 0);

    // There should be no NCRs queued.
    ASSERT_EQ(0, d2_mgr.getQueueSize());

    // Try to request a different address than the client has. The server
    // should respond with DHCPNAK.
    client.config_.lease_.addr_ = IOAddress("10.0.0.30");
    ASSERT_NO_THROW(client.doRequest());

    // Make sure that the server responded.
    response = client.getContext().response_;
    ASSERT_TRUE(response);
    EXPECT_EQ(DHCPNAK, response->getType());

    // Change client identifier. The server should treat the request
    // as a request from unknown client and ignore it.
    client.includeClientId("12:34");
    ASSERT_NO_THROW(client.doRequest());
    ASSERT_FALSE(client.getContext().response_);

    // Global statistics should remain unchanged.
    checkStat("v4-lease-reuses", 4, 3);

    // Subnet statistics should remain unchanged.
    checkStat("subnet[1].v4-lease-reuses", 4, 3);

    // Statistics for the other subnet should certainly not be affected.
    checkStat("subnet[2].v4-lease-reuses", 1, 0);

    // There should be no NCRs queued.
    ASSERT_EQ(0, d2_mgr.getQueueSize());
}

TEST_F(DORATest, leaseCaching) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    leaseCaching();
}

TEST_F(DORATest, leaseCachingMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    leaseCaching();
}

void
DORATest::stashAgentOptions() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Use relay agent to make sure that the desired subnet is
    // selected for our client.
    client.useRelay(true, IOAddress("10.0.0.20"), IOAddress("10.0.0.21"));
    // Specify client identifier.
    client.includeClientId("01:11:22:33:44:55:66");
    // Set the circuit id.
    client.setCircuitId("charter950");

    // Configure DHCP server.
    configure(DORA_CONFIGS[21], *client.getServer());
    // Perform 4-way exchange and should obtain an address from pool.
    ASSERT_NO_THROW_LOG(client.doDORA());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease for the first address
    // of the pool.
    ASSERT_EQ("10.0.0.10", client.config_.lease_.addr_.toText());

    // Renew.
    client.setState(Dhcp4Client::RENEWING);
    // Set the unicast destination address to indicate that it is a renewal.
    client.setDestAddress(IOAddress("10.0.0.1"));
    // Disable relay.
    client.useRelay(false);
    ASSERT_NO_THROW_LOG(client.doRequest());
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    ASSERT_EQ("10.0.0.10", client.config_.lease_.addr_.toText());
}

TEST_F(DORATest, stashAgentOptions) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    stashAgentOptions();
}

TEST_F(DORATest, stashAgentOptionsMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    stashAgentOptions();
}

void
DORATest::stashAgentOptionsReservation() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Use relay agent to make sure that the desired subnet is
    // selected for our client.
    client.useRelay(true, IOAddress("10.0.0.20"), IOAddress("10.0.0.21"));
    // Specify client identifier.
    client.includeClientId("01:11:22:33:44:55:66");
    // Set the circuit id.
    client.setCircuitId("charter950");

    // Configure DHCP server.
    configure(DORA_CONFIGS[22], *client.getServer());
    // Perform 4-way exchange and should obtain the reserved address.
    ASSERT_NO_THROW_LOG(client.doDORA());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease for the reserved address.
    ASSERT_EQ("10.0.0.9", client.config_.lease_.addr_.toText());

    // Renew.
    client.setState(Dhcp4Client::RENEWING);
    // Set the unicast destination address to indicate that it is a renewal.
    client.setDestAddress(IOAddress("10.0.0.1"));
    // Disable relay.
    client.useRelay(false);
    ASSERT_NO_THROW_LOG(client.doRequest());
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    ASSERT_EQ("10.0.0.9", client.config_.lease_.addr_.toText());
}

TEST_F(DORATest, stashAgentOptionsReservation) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    stashAgentOptionsReservation();
}

TEST_F(DORATest, stashAgentOptionsReservationMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    stashAgentOptionsReservation();
}

void
DORATest::noStashAgentOptionsReservation() {
    Dhcp4Client client(srv_, Dhcp4Client::SELECTING);
    // Use relay agent to make sure that the desired subnet is
    // selected for our client.
    client.useRelay(true, IOAddress("10.0.0.20"), IOAddress("10.0.0.21"));
    // Specify client identifier.
    client.includeClientId("01:11:22:33:44:55:66");
    // Set the circuit id.
    client.setCircuitId("charter950");

    // Configure DHCP server.
    configure(DORA_CONFIGS[23], *client.getServer());
    // Perform 4-way exchange and should obtain the reserved address.
    ASSERT_NO_THROW_LOG(client.doDORA());
    // Make sure that the server responded.
    ASSERT_TRUE(client.getContext().response_);
    Pkt4Ptr resp = client.getContext().response_;
    // Make sure that the server has responded with DHCPACK.
    ASSERT_EQ(DHCPACK, static_cast<int>(resp->getType()));
    // Make sure that the client has got the lease for the reserved address.
    ASSERT_EQ("10.0.0.9", client.config_.lease_.addr_.toText());

    // Renew.
    client.setState(Dhcp4Client::RENEWING);
    // Set the unicast destination address to indicate that it is a renewal.
    client.setDestAddress(IOAddress("10.0.0.1"));
    // Disable relay.
    client.useRelay(false);
    ASSERT_NO_THROW_LOG(client.doRequest());
    ASSERT_TRUE(client.getContext().response_);
    resp = client.getContext().response_;
    // The client is not recognized so a NAK is returned.
    ASSERT_EQ(DHCPNAK, static_cast<int>(resp->getType()));
}

TEST_F(DORATest, noStashAgentOptionsReservation) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    noStashAgentOptionsReservation();
}

TEST_F(DORATest, noStashAgentOptionsReservationMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    noStashAgentOptionsReservation();
}

/// @brief Checks the value of a statistic.
///
/// @param name name of statistic to check
/// @param expected_size expected number of statistic samples
/// @param expected_value expected value of the latest statistic sample
void DORATest::checkStat(string const& name,
                         size_t const expected_size,
                         int64_t const expected_value) {
    ObservationPtr const stats(StatsMgr::instance().getObservation(name));
    ASSERT_TRUE(stats) << "no such stat: " << name;
    EXPECT_EQ(expected_size, stats->getSize())
        << name << " stat has wrong size: found " << stats->getSize() << ", expected "
        << expected_size;
    EXPECT_EQ(expected_value, stats->getInteger().first)
        << name << " stat has wrong value: found " << stats->getInteger().first << ", expected "
        << expected_value;
}

// Starting tests which require MySQL backend availability. Those tests
// will not be executed if Kea has been compiled without MySQL support.
#ifdef HAVE_MYSQL

/// @brief Test fixture class for the test utilizing MySQL database backend.
class DORAMySQLTest : public DORATest {
public:
    /// @brief Constructor.
    ///
    /// Recreates MySQL schema for a test.
    DORAMySQLTest() : DORATest() {
        // Ensure we have the proper schema with no transient data.
        db::test::createMySQLSchema();
    }

    /// @brief Destructor.
    ///
    /// Destroys MySQL schema.
    virtual ~DORAMySQLTest() {
        // If data wipe enabled, delete transient data otherwise destroy the schema.
        db::test::destroyMySQLSchema();
    }

    /// @brief Initializer.
    MySqlLeaseMgrInit init_;
};

// Test that the client using the same hardware address but multiple
// client identifiers will obtain multiple leases (MySQL lease database).
TEST_F(DORAMySQLTest, multiStageBoot) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    // DORA_CONFIGS[8] to be used for server configuration.
    testMultiStageBoot(8);
}

// Test that the client using the same hardware address but multiple
// client identifiers will obtain multiple leases (MySQL lease database).
TEST_F(DORAMySQLTest, multiStageBootMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    // DORA_CONFIGS[8] to be used for server configuration.
    testMultiStageBoot(8);
}

#endif

// Starting tests which require MySQL backend availability. Those tests
// will not be executed if Kea has been compiled without PostgreSQL support.
#ifdef HAVE_PGSQL

/// @brief Test fixture class for the test utilizing PostgreSQL database backend.
class DORAPgSQLTest : public DORATest {
public:
    /// @brief Constructor.
    ///
    /// Recreates PgSQL schema for a test.
    DORAPgSQLTest() : DORATest() {
        // Ensure we have the proper schema with no transient data.
        db::test::createPgSQLSchema();
    }

    /// @brief Destructor.
    ///
    /// Destroys PgSQL schema.
    virtual ~DORAPgSQLTest() {
        // If data wipe enabled, delete transient data otherwise destroy the schema
        db::test::destroyPgSQLSchema();
    }

    /// @brief Initializer.
    PgSqlLeaseMgrInit init_;
};

// Test that the client using the same hardware address but multiple
// client identifiers will obtain multiple leases (PostgreSQL lease database).
TEST_F(DORAPgSQLTest, multiStageBoot) {
    Dhcpv4SrvMTTestGuard guard(*this, false);
    // DORA_CONFIGS[9] to be used for server configuration.
    testMultiStageBoot(9);
}

// Test that the client using the same hardware address but multiple
// client identifiers will obtain multiple leases (PostgreSQL lease database).
TEST_F(DORAPgSQLTest, multiStageBootMultiThreading) {
    Dhcpv4SrvMTTestGuard guard(*this, true);
    // DORA_CONFIGS[9] to be used for server configuration.
    testMultiStageBoot(9);
}

#endif

} // end of anonymous namespace
