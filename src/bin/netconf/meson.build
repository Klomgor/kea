if not NETCONF_DEPS_FOUND
    subdir_done()
endif

netconf_lib = static_library(
    'netconf',
    'control_socket.cc',
    'http_control_socket.cc',
    'netconf.cc',
    'netconf_cfg_mgr.cc',
    'netconf_config.cc',
    'netconf_controller.cc',
    'netconf_lexer.cc',
    'netconf_log.cc',
    'netconf_messages.cc',
    'netconf_parser.cc',
    'netconf_process.cc',
    'parser_context.cc',
    'simple_parser.cc',
    'stdout_control_socket.cc',
    'unix_control_socket.cc',
    dependencies: NETCONF_DEPS_ARRAY + [crypto],
    include_directories: [include_directories('.')] + INCLUDES,
    link_with: [kea_yang_testutils_lib, kea_process_testutils_lib] + LIBS_BUILT_SO_FAR,
    override_options: ['cpp_std=c++20'],
)

executable(
    'kea-netconf',
    'main.cc',
    dependencies: NETCONF_DEPS_ARRAY + [crypto],
    include_directories: [include_directories('.')] + INCLUDES,
    install: true,
    install_dir: 'sbin',
    link_with: [netconf_lib] + LIBS_BUILT_SO_FAR,
    override_options: ['cpp_std=c++20'],
)

subdir('tests')
