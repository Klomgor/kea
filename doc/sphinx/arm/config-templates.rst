.. _config-templates:

Configuration Templates
=======================

The following sections include configuration templates for
certain deployment types. The example configuration files are also available in the Kea sources,
in the ``doc/examples`` directory.

.. include:: template-power-user-home.md

Some tweaking of these templates may be required to match specific system needs: at a
minimum, the lines highlighted in yellow must be adjusted to match the actual deployment.

Server1's DHCPv4 configuration file:

.. literalinclude:: template-power-user-home-dhcp4-1.conf
   :language: javascript
   :emphasize-lines: 25, 32, 36, 39, 84, 89, 129, 141, 156, 161, 163-167, 175-189, 199-208
   :linenos:

Server2's DHCPv4 configuration file:

.. literalinclude:: template-power-user-home-dhcp4-2.conf
   :language: javascript
   :emphasize-lines: 25, 32, 36, 39, 84, 89, 129, 141, 156, 161, 163-167, 175-189, 199-208
   :linenos:

.. include:: template-ha-mt-tls.md

Some tweaking of these templates may be required to match specific system needs: at a
minimum, the lines highlighted in yellow must be adjusted to match the actual deployment.

Server1's DHCPv4 configuration file:

.. literalinclude:: template-ha-mt-tls-dhcp4-1.conf
   :language: javascript
   :emphasize-lines: 25, 31, 32, 36, 45, 49, 52, 55, 58, 59, 62-77, 120, 125, 161-175, 184, 186, 188, 190, 192, 227, 231, 234-238
   :linenos:

Server2's DHCPv4 configuration file:

.. literalinclude:: template-ha-mt-tls-dhcp4-2.conf
   :language: javascript
   :emphasize-lines: 25, 31, 32, 36, 45, 49, 52, 55, 58, 59, 62-77, 120, 125, 161-175, 184, 186, 188, 190, 192, 227, 231, 234-238
   :linenos:
