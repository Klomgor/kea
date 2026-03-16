.. _kea-ctrl-agent:

*********************
The Kea Control Agent
*********************

Kea version 3.1,7 removed the obsolete cControl Agent. Please use
the Kea server control sockets.

.. _migration:

Migration
=========

The HTTP/HTTPS control sockets of kea servers are:

- in :ref:`dhcp4-http-ctrl-channel` section for the DHCPv4 server

- in :ref:`dhcp6-http-ctrl-channel` section for the DHCPv6 server

- in :ref:`d2-http-ctrl-channel` section for the DHCP-DDNS server

An entry must be created in ``socket-controls` list with:

- ``http`` or ``https`` (the second is better when TLS parameters
  are present) for the ``socket-type``

- ``http-host`` becomes ``socket-address``

- ``http-port`` becomes ``socket-port``

All other parameters, ``http-headers``, TLS parameters, basic HTTP
authentication setup, ..., can go directly in the control socket entry
of the Kea server configuration with the same syntax and meaning.

