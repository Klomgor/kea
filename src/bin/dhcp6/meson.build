dhcp6_lib = static_library(
    'dhcp6',
    'client_handler.cc',
    'ctrl_dhcp6_srv.cc',
    'dhcp6to4_ipc.cc',
    'dhcp6_lexer.cc',
    'dhcp6_log.cc',
    'dhcp6_messages.cc',
    'dhcp6_parser.cc',
    'dhcp6_srv.cc',
    'json_config_parser.cc',
    'main.cc',
    'parser_context.cc',
    dependencies: [crypto],
    include_directories: [include_directories('.')] + INCLUDES,
)
executable(
    'kea-dhcp6',
    'main.cc',
    dependencies: [crypto],
    include_directories: [include_directories('.')] + INCLUDES,
    install: true,
    install_dir: 'sbin',
    link_with: [dhcp6_lib] + LIBS_BUILT_SO_FAR,
)
subdir('tests')
