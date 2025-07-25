.. _hooks-libraries:

**************
Hook Libraries
**************

.. _hooks-libraries-introduction:

Introduction
============

Kea is both flexible and customizable, via the use of "hooks." This feature
lets Kea load one or more
dynamically linked libraries (known as "hook libraries") and call functions
in them at various points in its processing ("hook points").
Those functions perform whatever custom processing is required.

The hooks concept allows the core Kea code to remain reasonably small
by moving features that only some, but not all, users find useful to
external libraries. Those with no need for certain functions can simply choose not
to load those libraries.

Hook libraries are loaded by individual Kea processes, not by Kea as a
whole. This means, among other things, that it is possible to associate one set
of libraries with the DHCPv4 server and a different set with the DHCPv6
server.

It is also possible for a process to load
multiple libraries. When processing reaches a hook point, Kea calls the
hook library functions attached to it. If multiple libraries have
attached a function to a given hook point, Kea calls all of them, in the
order in which the libraries are specified in the configuration file.
The order may be important; consult the documentation of the libraries
for specifics.

When a Kea process unloads a library, it expects the ``dlclose`` function
to remove all library symbols, as well as the library code, from address space.
Although most OSes implement the ``dlclose`` function, this behavior is not
required by the POSIX standard and not all systems support it; for example, the musl
library, used by default by Alpine Linux, implements the ``dlclose`` function
as a no operation. On such systems a library actually remains loaded for the
lifetime of the process, which means that it must be restarted
to update libraries with newer versions; it is not sufficient to simply
reconfigure or reload the Kea process.

The next sections describe how to install and configure hook libraries. Users who are
interested in writing their own hook library can find information
in the `Hooks Developer's Guide section of the Kea Developer's
Guide <https://reports.kea.isc.org/dev_guide/df/d46/hooksdgDevelopersGuide.html>`__.

Note that some libraries are available under different licenses.

Please also note that some libraries may require additional dependencies and/or
compilation switches to be enabled.


Installing Hook Packages
========================

.. note::

    For more details about installing the Kea Subscriber Hooks, please read
    `this Knowledgebase article <https://kb.isc.org/docs/aa-01587>`__.

Most hook packages are now included in the base Kea sources. There is no
need to do anything special to compile or install them, as they are covered
by the usual building and installation procedures. Please
refer to :ref:`installation` for a general overview of the installation process.

ISC provides several additional subscriber-only hooks in the form of packages, which
follow a similar installation procedure but with several additional steps.
For our users' convenience, the subscriber hooks' installation procedure is described in this section.

1. Download the package; detailed instructions are provided in the KB article
above. The package will be a file with a name similar to
``kea-subscriber-|release|.tar.xz``.

2. Administrators who have the sources for the corresponding version of the
open-source Kea package on their system from the initial Kea installation
should skip this step. Otherwise, extract the Kea source from the original
tarball that was downloaded. For example, with a download of Kea |release|,
there should be a tarball called ``kea-|release|.tar.xz`` on the system.
Unpack this tarball:

.. parsed-literal::

    $ tar -Jxvf kea-|release|.tar.xz

This will unpack the tarball into the ``kea-|release|`` subdirectory of
the current working directory.

3. Unpack the Kea subscriber hooks tarball into the same directory where the
original Kea source is located. Once Kea |release| has been unpacked into a ``kea-|release|``
subdirectory and the Kea subscriber tarball is in the current directory, the following
steps will unpack the subscriber tarball into the correct location:

.. parsed-literal::

    $ cd kea-|release|
    $ tar -Jxvf ../kea-subscriber-|release|.tar.xz

Note that unpacking the Kea subscriber package puts the files into a
directory named ``premium``. Regardless of the name of the package, the
directory is always called ``premium``.

4. Set up the build. Make sure ``meson`` and ``ninja`` are installed on your
   system. The first section of the output should look something like this:

.. code-block:: console

     $ meson setup build

.. parsed-literal::

    Package:
      Name:               kea
      Version:            |release|
      Extended version:   |release| (tarball)
      Version type:       development
      OS Family:          Linux

      Prefix:             /opt/kea
      Hooks directory:    /opt/kea/lib/kea/hooks
      Premium hooks:      yes

6. Compile Kea.

.. code-block:: console

    $ meson compile -C build

7. Install Kea sources along with the hooks:

::

   $ sudo make install

The installation location of the hook libraries depends on whether the
``--prefix`` parameter was specified at the ``meson setup`` step. If not,
the default location is ``/usr/local/lib/kea/hooks``. The proper installation
of the libraries can be verified with this command:

.. code-block:: console

   $ ls -l /usr/local/lib/kea/hooks/*.so
   /usr/local/lib/kea/hooks/libddns_gss_tsig.so
   /usr/local/lib/kea/hooks/libdhcp_bootp.so
   /usr/local/lib/kea/hooks/libdhcp_class_cmds.so
   /usr/local/lib/kea/hooks/libdhcp_ddns_tuning.so
   /usr/local/lib/kea/hooks/libdhcp_flex_id.so
   /usr/local/lib/kea/hooks/libdhcp_flex_option.so
   /usr/local/lib/kea/hooks/libdhcp_ha.so
   /usr/local/lib/kea/hooks/libdhcp_host_cache.so
   /usr/local/lib/kea/hooks/libdhcp_host_cmds.so
   /usr/local/lib/kea/hooks/libdhcp_lease_cmds.so
   /usr/local/lib/kea/hooks/libdhcp_lease_query.so
   /usr/local/lib/kea/hooks/libdhcp_legal_log.so
   /usr/local/lib/kea/hooks/libdhcp_limits.so
   /usr/local/lib/kea/hooks/libdhcp_mysql.so
   /usr/local/lib/kea/hooks/libdhcp_perfmon.so
   /usr/local/lib/kea/hooks/libdhcp_pgsql.so
   /usr/local/lib/kea/hooks/libdhcp_ping_check.so
   /usr/local/lib/kea/hooks/libdhcp_radius.so
   /usr/local/lib/kea/hooks/libdhcp_run_script.so
   /usr/local/lib/kea/hooks/libdhcp_stat_cmds.so
   /usr/local/lib/kea/hooks/libdhcp_subnet_cmds.so

with the following subscriber libraries:

.. code-block:: console

   /usr/local/lib/kea/hooks/libca_rbac.so -> libdhcp_rbac.so
   /usr/local/lib/kea/hooks/libdhcp_cb_cmds.so
   /usr/local/lib/kea/hooks/libdhcp_rbac.so

The exact list returned depends on the packages installed. If the
directory was specified via ``--prefix``, the hook libraries will be located in
``{prefix directory}/lib/kea/hooks``.

Configuring Hook Libraries
==========================

The hook libraries for a given process are configured using the
``hooks-libraries`` keyword in the configuration for that process. (Note
that the word "hooks" is plural.) The value of the keyword is an array
of map structures, with each structure corresponding to a hook library. For
example, to set up two hook libraries for the DHCPv4 server, the
configuration would be:

.. code-block:: json

    {
        "Dhcp6": {
            "hooks-libraries": [
                {
                    "library": "/opt/first_custom_hooks_example.so"
                },
                {
                    "library": "/opt/local/second_custom_hooks_example.so",
                    "parameters": {
                        "mail": "spam@example.com",
                        "floor": 13,
                        "debug": false,
                        "users": [
                            "alice",
                            "bob",
                            "charlie"
                        ],
                        "languages": {
                            "french": "bonjour",
                            "klingon": "yl'el"
                        }
                    }
                }
            ]
        }
    }

.. note::

    Libraries are reloaded even if their lists have not changed,
    because the parameters specified for the library (or the files those
    parameters point to) may have changed.

As of Kea 2.7.9, hook libraries may only be loaded from the default installation
directory determined during compilation and shown in the config report as
"Hooks directory".  This value may be overridden at startup by setting the
environment variable ``KEA_HOOKS_PATH`` to the desired path.  If a path other
than this value is used in a ``library`` element Kea will emit an error and refuse
to load the library. For ease of use ``library`` elements may simply omit path
components, specifying the file name only as shown below:

.. code-block:: json

    {
        "Dhcp6": {
            "hooks-libraries": [
                {
                    "library": "first_custom_hooks_example.so"
                },
                {
                    "library": "second_custom_hooks_example.so"
                }
            ]
        }
    }

This snippet (on Debian 12) is equivalent to:

.. code-block:: json

    {
        "Dhcp6": {
            "hooks-libraries": [
                {
                    "library": "/usr/lib/x86_64-linux-gnu/kea/hooks/first_custom_hooks_example.so"
                },
                {
                    "library": "/usr/lib/x86_64-linux-gnu/kea/hooks/second_custom_hooks_example.so"
                }
            ]
        }
    }

.. note::
    The support for path components in ``library`` elements is deprecated as of 2.7.8, and will
    eventually be removed.

Libraries may have additional parameters that are not mandatory, in the
sense that there may be libraries that do not require them. However, for any
given library there is often a requirement to specify a certain
set of parameters. Please consult the documentation for each individual library for
details. In the example above, the first library (``/opt/first_custom_hooks_example.so``)
has no parameters. The second library (``/opt/local/second_custom_hooks_example.so``)
has five parameters: specifying mail (string parameter), floor (integer parameter),
debug (boolean parameter), lists (list of strings), and maps (containing strings).
Nested parameters can be used if the library supports it. This topic is explained in detail
in the `Hooks Developer's Guide section of the Kea Developer's Guide
<https://reports.kea.isc.org/dev_guide/df/d46/hooksdgDevelopersGuide.html>`__.

Some hooks use user context to set the parameters. See :ref:`user-context-hooks`.

Notes:

-  The full path to each library should be given.

-  As noted above, the order in which the hooks are called may be important;
   consult the documentation for each library for specifics.

-  An empty list has the same effect as omitting the ``hooks-libraries``
   configuration element altogether.

   .. note::

      There is one case where this is not true: if Kea is running with a
      configuration that contains a ``hooks-libraries`` item, and that
      item is removed and the configuration reloaded, the removal is
      ignored and the libraries remain loaded. As a workaround,
      instead of removing the ``hooks-libraries`` item, change it to an
      empty list.

At the moment, only the :iscman:`kea-dhcp4` and :iscman:`kea-dhcp6` processes support
hook libraries.

.. _order-of-configuration-hooks:

Order of Configuration:
~~~~~~~~~~~~~~~~~~~~~~~

It is important to recognize that the order in which hook libraries are
configured determines the order in which their callouts will be executed,
in cases where more than one hook library implements the same callout. For
example, to use the Flexible Identifier (also called Flex ID) hook library to formulate the client
IDs in conjunction with the High Availability (HA) hook library for load-balanced HA, it is essential
that the Flex ID library be specified first in the server's ``hooks-libraries``
section.  This ensures that the client ID is formulated by the Flex ID library
before the HA library uses it for load-balancing. Similarly, it is best to
specify the Forensic Logging library last, to ensure that any other installed hooks have already made
their contributions to the packet processing before that one is loaded.

.. _user-context-hooks:

User Contexts in Hooks
~~~~~~~~~~~~~~~~~~~~~~

Hook libraries can have their own configuration parameters, which is
convenient if the parameter applies to the whole library. However,
sometimes it is useful to extend certain configuration entities
with additional configuration data. This is where the concept
of user contexts comes in. A system administrator can define an arbitrary set of
data and attach it to Kea structures, as long as the data is specified
as a JSON map. In particular, it is possible to define fields that are
integers, strings, boolean, lists, or maps. It is possible to define
nested structures of arbitrary complexity. Kea does not use that data on
its own; it simply stores it and makes it available for the hook libraries.

Another use case for user contexts may be storing comments and other
information that will be retained by Kea. Regular comments are discarded
when the configuration is loaded, but user contexts are retained. This is
useful if administrators want their comments to survive :isccmd:`config-set` or :isccmd:`config-get`
operations, for example.

If user context is supported in a given context, the parser translates
"comment" entries into user context with a "comment" entry.

User context can store configuration for multiple hooks and comments at once.

Some hooks use user context for a configuration that can be easily edited
without the need to restart the server.

The DDNS Tuning hook uses user context to configure per-subnet behavior. Here's an example:

::

    "subnet4": [{
        "id": 1,
        "subnet": "192.0.2.0/24",
        "pools": [{
            "pool": "192.0.2.10 - 192.0.2.20"
        } ],
        "user-context": {
            "ddns-tuning": {
                "hostname-expr": "'guest-'+int8totext(substring(pkt4.yiaddr, 0,1))+'-' \
                                          +int8totext(substring(pkt4.yiaddr, 1,2))+'-' \
                                          +int8totext(substring(pkt4.yiaddr, 2,3))+'-' \
                                          +int8totext(substring(pkt4.yiaddr, 3,4))"
            },
            "last-modified": "2017-09-04 13:32",
            "phones": [ "x1234", "x2345" ],
            "devices-registered": 42,
            "billing": false
        }
    }]


The Limits hook uses user-context in classes and subnets to set parameters. For example:

.. code-block:: json

    {
      "Dhcp6": {
        "client-classes": [
          {
            "name": "gold",
            "user-context": {
              "limits": {
                "address-limit": 2,
                "prefix-limit": 1,
                "rate-limit": "1000 packets per second"
              }
            }
          }
        ],
        "hooks-libraries": [
          {
            "library": "/usr/local/lib/libdhcp_limits.so"
          }
        ],
        "subnet6": [
          {
            "id": 1,
            "pools": [
              {
                "pool": "2001:db8::/64"
              }
            ],
            "subnet": "2001:db8::/64",
            "user-context": {
              "limits": {
                "address-limit": 4,
                "prefix-limit": 2,
                "rate-limit": "10 packets per minute"
              }
            }
          }
        ]
      }
    }

.. _parked-packet-limit:

Parked-Packet Limit
~~~~~~~~~~~~~~~~~~~

Kea servers contain a mechanism by which the response to a client packet may
be held, pending completion of hook library work. We refer to this as "parking
the packet." When work is ready to continue, the server unparks the response
and continues processing.

There is a global parameter, ``parked-packet-limit``, that may be used to limit
the number of responses that may be parked at any given time. This acts as a
form of congestion handling and protects the server from being swamped when the
volume of client queries is outpacing the server's ability to respond. Once the
limit is reached, the server emits a log and drops any new responses until
parking spaces are available.

In general, smaller values for the parking lot limit are likely to cause more
drops but with shorter response times; larger values are likely to result in
fewer drops but with longer response times. Currently, the default value for
``parked-packet-limit`` is 256.

.. warning::

   Using too small a value may result in an unnecessarily high drop rate, while
   using too large a value may lead to response times that are simply too long
   to be useful. A value of 0, while allowed, disables the limit altogether, but
   this is highly discouraged as it may lead to Kea servers becoming
   unresponsive to clients. Choosing the best value is very site-specific; we
   recommend users initially leave it at the default value of 256 and observe
   how the system behaves over time with varying load conditions.

Here is an example of the global parameter used with :ischooklib:`libdhcp_ha.so`.
It lowers the number of concurrently parked packets to 128.

.. code-block:: json

    {
      "Dhcp6": {
        "parked-packet-limit": 128
        "hooks-libraries": [
          {
            "library": "/usr/lib/kea/hooks/libdhcp_lease_cmds.so"
          },
          {
            "library": "/usr/lib/kea/hooks/libdhcp_ha.so",
            "parameters": {
              "high-availability": [
                {
                  "mode": "hot-standby",
                  "peers": [
                    {
                      "name": "server1",
                      "role": "primary",
                      "url": "http://127.0.0.1:8080/"
                    },
                    {
                      "name": "server2",
                      "role": "standby",
                      "url": "http://127.0.0.1:8088/"
                    }
                  ],
                  "this-server-name": "server1"
                }
              ]
            }
          }
        ]
      }
    }

Available Hook Libraries
========================

As described above, the hook functionality provides a way to customize
a Kea server without modifying the core code. ISC has chosen to take
advantage of this feature to provide functions that may only be useful
to a subset of Kea users. To this end, ISC has created some hook
libraries, discussed in the following sections.

.. note::

   As of Kea 3.0.0, most of these libraries are available at no cost with the open source base code;
   a few are only available to organizations
   that contribute to Kea's development through paid ISC support contracts. Paid support
   includes professional engineering assistance, advance security notifications, input
   into ISC's roadmap planning, and many other benefits, while helping
   keep Kea sustainable in the long term. ISC encourages companies and organizations
   to consider purchasing a paid support contract; further information can be
   obtained by completing the form at https://www.isc.org/contact.

The following table provides a list of hook libraries currently available
from ISC. It is important to pay attention to which libraries may be
loaded by which Kea processes. It is a common mistake to configure the
:iscman:`kea-ctrl-agent` process to load libraries that should, in fact, be
loaded by the :iscman:`kea-dhcp4` or :iscman:`kea-dhcp6` processes. If a library
from ISC does not work as expected, please make sure that it has been
loaded by the correct process per the table below.

.. tabularcolumns:: |p{0.1\linewidth}|p{0.1\linewidth}|p{0.8\linewidth}|

.. table:: List of available hook libraries
   :class: longtable
   :widths: 10 10 80

   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | Name                                                      | Availability | Description                                                  |
   +===========================================================+==============+==============================================================+
   | :ref:`BOOTP <hooks-bootp>`                                | Kea open     | This hook library adds BOOTP support, as defined in          |
   |                                                           | source       | RFC 1497. It recognizes received BOOTP requests:             |
   |                                                           |              | they are translated into DHCPREQUEST packets, put into the   |
   |                                                           |              | BOOTP client class, and receive infinite lifetime leases.    |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`Class Commands <hooks-class-cmds>`                  | Kea open     | This hook library allows configured DHCP client classes to   |
   |                                                           | source       | be added, updated, deleted, and fetched without              |
   |                                                           |              | needing to restart the DHCP server.                          |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`Configuration Backend Commands <hooks-cb-cmds>`     | ISC support  | This hook                                                    |
   |                                                           | customers    | library implements a collection of commands to manage        |
   |                                                           |              | Kea configuration information in a                           |
   |                                                           |              | database. This library may only be used in conjunction with  |
   |                                                           |              | one of the supported Configuration Backend implementations.  |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`DDNS Tuning <hooks-ddns-tuning>`                    | Kea open     | This hook library adds custom behaviors related to Dynamic   |
   |                                                           | source       | DNS updates on a per-client basis. Its primary feature is to |
   |                                                           |              | allow the host name used for DNS to be                       |
   |                                                           |              | calculated using an expression.                              |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`Flexible Identifier <hooks-flex-id>`                | Kea open     | Kea software provides a way to handle host reservations that |
   |                                                           | source       | include addresses, prefixes, options, client classes and     |
   |                                                           |              | other features. The reservation can be based on hardware     |
   |                                                           |              | address, DUID, circuit-id, or client-id in DHCPv4 and on     |
   |                                                           |              | hardware address or DUID in DHCPv6. However, there are       |
   |                                                           |              | sometimes scenarios where the reservation is more complex,   |
   |                                                           |              | e.g. uses other options than mentioned above, uses parts of  |
   |                                                           |              | specific options, or perhaps uses a combination of several   |
   |                                                           |              | options and fields to uniquely identify a client. Those      |
   |                                                           |              | scenarios are addressed by the Flexible Identifier hook      |
   |                                                           |              | application. It allows defining an expression, similar to    |
   |                                                           |              | the one used in client classification,                       |
   |                                                           |              | e.g. ``substring(relay6[0].option[37],0,6)``. Each incoming  |
   |                                                           |              | packet is evaluated against that expression and its value is |
   |                                                           |              | then searched in the reservations database.                  |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`Flexible Option <hooks-flex-option>`                | Kea open     | This library provides hooks that compute option values       |
   |                                                           | source       | instead of static configured values. An expression is        |
   |                                                           |              | evaluated on the query packet. Defined add, supersede, and   |
   |                                                           |              | remove actions are applied on the response packet before     |
   |                                                           |              | it is sent using the evaluation result.                      |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`Forensic Logging <hooks-legal-log>`                 | Kea open     | This library provides hooks that record a detailed log of    |
   |                                                           | source       | lease assignments and renewals in a set of log files. In     |
   |                                                           |              | many legal jurisdictions, companies - especially ISPs - must |
   |                                                           |              | record information about the addresses they have leased to   |
   |                                                           |              | DHCP clients. This library is designed to help with that     |
   |                                                           |              | requirement. If the information that it records is           |
   |                                                           |              | sufficient, it may be used directly. If a jurisdiction       |
   |                                                           |              | requires a different set of information to be saved, it can  |
   |                                                           |              | be used as a template or example to create                   |
   |                                                           |              | custom logging hooks. In Kea 1.9.8, additional parameters    |
   |                                                           |              | were added to give users more flexibility regarding          |
   |                                                           |              | what information should be logged.                           |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`GSS-TSIG <hooks-gss-tsig>`                          | Kea open     | This hook library adds support to the Kea D2 server          |
   |                                                           | source       | (kea-dhcp-ddns) for using GSS-TSIG to sign DNS updates.      |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`High Availability <hooks-high-availability>`        | Kea open     | The risk of DHCP service unavailability can be minimized     |
   |                                                           | source       | by setting up a pair of DHCP servers in a network. Two       |
   |                                                           |              | modes of operation are supported. The first one is called    |
   |                                                           |              | load-balancing, and is sometimes referred to as              |
   |                                                           |              | "active-active." Each server can handle selected groups of   |
   |                                                           |              | clients in this network, or all clients if it detects that   |
   |                                                           |              | its partner has become unavailable. It is also possible to   |
   |                                                           |              | designate one server to serve all DHCP clients, and leave    |
   |                                                           |              | another server as standby. This mode is called "hot standby" |
   |                                                           |              | and is sometimes referred to as "active-passive." This       |
   |                                                           |              | server activates its DHCP function only when it detects that |
   |                                                           |              | its partner is not available. Such cooperation between the   |
   |                                                           |              | DHCP servers requires that these servers constantly          |
   |                                                           |              | communicate with each other to send updates about allocated  |
   |                                                           |              | leases, and to periodically test whether their partners are  |
   |                                                           |              | still operational. The hook library also provides an ability |
   |                                                           |              | to send lease updates to external backup servers, making it  |
   |                                                           |              | much easier to have a replacement that is up to date.        |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`Host Cache <hooks-host-cache>`                      | Kea open     | Some database backends, such as RADIUS,                      |
   |                                                           | source       | may take a long time to respond. Since                       |
   |                                                           |              | Kea in general is synchronous, backend performance           |
   |                                                           |              | directly affects DHCP performance. To minimize               |
   |                                                           |              | performance impact, this library                             |
   |                                                           |              | provides a way to cache responses from other hosts. This     |
   |                                                           |              | includes negative caching, i.e. the ability to remember that |
   |                                                           |              | there is no client information in the database.              |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`Host Commands <hooks-host-cmds>`                    | Kea open     | Kea provides a way to store host reservations in a           |
   |                                                           | source       | database. In many larger deployments it is useful to be able |
   |                                                           |              | to manage that information while the server is running. This |
   |                                                           |              | library provides management commands for adding, querying,   |
   |                                                           |              | and deleting host reservations in a safe way without         |
   |                                                           |              | restarting the server. In particular, it validates the       |
   |                                                           |              | parameters, so an attempt to insert incorrect data, e.g. add |
   |                                                           |              | a host with conflicting identifier in the same subnet, is    |
   |                                                           |              | rejected. Those commands are exposed via the command channel |
   |                                                           |              | (JSON over UNIX sockets) and the Control Agent (JSON over    |
   |                                                           |              | RESTful interface).                                          |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`Lease Commands <hooks-lease-cmds>`                  | Kea open     | This hook library offers a number of commands used to        |
   |                                                           | source       | manage leases. Kea can store lease information in various    |
   |                                                           |              | backends: memfile, MySQL, PostgreSQL. This library provides  |
   |                                                           |              | a unified interface to manipulate leases in an unified, safe |
   |                                                           |              | way. In particular, it allows manipulation of memfile leases |
   |                                                           |              | while Kea is running, sanity check changes, lease existence  |
   |                                                           |              | checks, and removal of all leases belonging to a specific    |
   |                                                           |              | subnet. It can also catch obscure errors, like the addition  |
   |                                                           |              | of a lease with subnet-id that does not exist in the         |
   |                                                           |              | configuration, or configuration of a lease to use an address |
   |                                                           |              | that is outside of the subnet to which it is supposed to     |
   |                                                           |              | belong. This library allows easy management of user contexts |
   |                                                           |              | associated with leases.                                      |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`Leasequery <hooks-lease-query>`                     | Kea open     | This library adds support for DHCPv4 Leasequery (RFC 4388),  |
   |                                                           | source       | DHCPv4 Bulk Leasequery (RFC6926); DHCPv6 Leasequery          |
   |                                                           |              | (RFC 5007), and DHCPv6 Bulk Leasequery (RFC5460).            |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`Limits <hooks-limits>`                              | Kea open     | With this hook library, :iscman:`kea-dhcp4` and              |
   |                                                           | source       | :iscman:`kea-dhcp6` servers can apply a limit to the rate at |
   |                                                           |              | which packets receive a response. The limit can be applied   |
   |                                                           |              | per-client class or per-subnet.                              |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`MySQL Configuration Backend <hooks-mysql>`          | Kea open     | This hook library is an implementation of the Kea Lease,     |
   |                                                           | source       | Host and Configuration Backend for MySQL. It uses a          |
   |                                                           |              | MySQL database as a repository for the Kea leases, host      |
   |                                                           |              | reservations and configuration information. Kea servers use  |
   |                                                           |              | this library to fetch their configurations if Configuration  |
   |                                                           |              | Backend is used.                                             |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`PerfMon <hooks-perfmon>`                            | Kea open     | With this hook library, :iscman:`kea-dhcp4` and              |
   |                                                           | source       | :iscman:`kea-dhcp6` servers can track and report performance |
   |                                                           |              | data.                                                        |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`Ping Check <hooks-ping-check>`                      | Kea open     | With this hook library, the :iscman:`kea-dhcp4` server can   |
   |                                                           | source       | perform ping checks of candidate lease addresses before      |
   |                                                           |              | offering them to clients.                                    |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`PostgreSQL Database Backend <hooks-pgsql>`          | Kea open     | This hook library is an implementation of the Kea Lease,     |
   |                                                           | source       | Host and Configuration Backend for PostgreSQL. It uses a     |
   |                                                           |              | PostgreSQL database as a repository for the Kea leases, host |
   |                                                           |              | reservations and configuration information. Kea servers use  |
   |                                                           |              | this library to fetch their configurations if Configuration  |
   |                                                           |              | Backend is used.                                             |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`RADIUS <hooks-radius>`                              | Kea open     | The RADIUS hook library allows Kea to interact with          |
   |                                                           | source       | RADIUS servers using access and accounting mechanisms. The   |
   |                                                           |              | access mechanism may be used for access control, assigning   |
   |                                                           |              | specific IPv4 or IPv6 addresses reserved by RADIUS,          |
   |                                                           |              | dynamically assigning addresses from designated pools chosen |
   |                                                           |              | by RADIUS, or rejecting the client's messages altogether.    |
   |                                                           |              | The accounting mechanism allows a RADIUS server to keep      |
   |                                                           |              | track of device activity over time.                          |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`RBAC <hooks-rbac>`                                  | ISC support  | This hook library adds support to the Kea Control Agent      |
   |                                                           | customers    | (kea-ctrl-agent) and Kea servers (kea-dhcp4, kea-dhcp6 and   |
   |                                                           |              | kea-dhcp-ddns) for Role-Based Access Control filtering of    |
   |                                                           |              | commands.                                                    |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`Run Script <hooks-run-script>`                      | Kea open     | This hook library adds support to run external               |
   |                                                           | source       | scripts for specific packet-processing hook points. There    |
   |                                                           |              | are several exported environment variables available for     |
   |                                                           |              | the script.                                                  |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`Statistics Commands <hooks-stat-cmds>`              | Kea open     | This library provides additional                             |
   |                                                           | source       | commands for retrieving accurate DHCP lease statistics, for  |
   |                                                           |              | Kea DHCP servers that share the same lease database. This    |
   |                                                           |              | setup is common in deployments where DHCP service redundancy |
   |                                                           |              | is required and a shared lease database is used to avoid     |
   |                                                           |              | lease-data replication between the DHCP servers.             |
   |                                                           |              | This hook library returns lease statistics                   |
   |                                                           |              | for each subnet.                                             |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`Subnet Commands <hooks-subnet-cmds>`                | Kea open     | In deployments in which subnet configuration needs to be     |
   |                                                           | source       | frequently updated, it is a hard requirement that such       |
   |                                                           |              | updates be performed without the need for a full DHCP server |
   |                                                           |              | reconfiguration or restart. This hook library allows for     |
   |                                                           |              | incremental changes to the subnet configuration such as      |
   |                                                           |              | adding or removing a subnet. It also allows for              |
   |                                                           |              | listing all available subnets and fetching detailed          |
   |                                                           |              | information about a selected subnet. The commands exposed by |
   |                                                           |              | this library do not affect other subnets or configuration    |
   |                                                           |              | parameters currently used by the server.                     |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+
   | :ref:`User Check <hooks-user-chk>`                        | Kea open     | Reads known users list from a file. Unknown users will be    |
   |                                                           | source       | assigned a lease from the last subnet defined in the         |
   |                                                           |              | configuration file, e.g. to redirect them to a captive       |
   |                                                           |              | portal. This demonstrates how an external source of          |
   |                                                           |              | information can be used to influence the Kea allocation      |
   |                                                           |              | engine. This hook is part of the Kea source code and is      |
   |                                                           |              | available in the ``src/hooks/dhcp/user_chk`` directory.      |
   +-----------------------------------------------------------+--------------+--------------------------------------------------------------+

ISC hopes to see more hook libraries become available as time
progresses, developed both internally and externally. Since this list
may evolve dynamically, it is maintained on a wiki page, available
at https://gitlab.isc.org/isc-projects/kea/wikis/Hooks-available.
Developers or others who are aware of any hook libraries not listed there
are asked to please send a note to the kea-users or kea-dev mailing lists for
updating. (Information on all of ISC's public mailing lists can be found
at https://www.isc.org/mailinglists/.)

The libraries developed by ISC are described in detail in the following
sections.

.. include:: hooks-bootp.rst
.. include:: hooks-cb-cmds.rst
.. include:: hooks-class-cmds.rst
.. include:: hooks-ddns-tuning.rst
.. include:: hooks-flex-id.rst
.. include:: hooks-flex-option.rst
.. include:: hooks-gss-tsig.rst
.. include:: hooks-ha.rst
.. include:: hooks-host-cache.rst
.. include:: hooks-host-cmds.rst
.. include:: hooks-lease-cmds.rst
.. include:: hooks-lease-query.rst
.. include:: hooks-legal-log.rst
.. include:: hooks-limits.rst
.. include:: hooks-mysql.rst
.. include:: hooks-perfmon.rst
.. include:: hooks-ping-check.rst
.. include:: hooks-pgsql.rst
.. include:: hooks-radius.rst
.. include:: hooks-rbac.rst
.. include:: hooks-run-script.rst
.. include:: hooks-stat-cmds.rst
.. include:: hooks-subnet-cmds.rst
.. include:: hooks-user-chk.rst
