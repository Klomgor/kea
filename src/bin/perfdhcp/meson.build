perfdhcp_lib = static_library(
    'perfdhcp',
    'avalanche_scen.cc',
    'basic_scen.cc',
    'command_options.cc',
    'perf_pkt4.cc',
    'perf_pkt6.cc',
    'perf_socket.cc',
    'pkt_transform.cc',
    'rate_control.cc',
    'receiver.cc',
    'stats_mgr.cc',
    'test_control.cc',
    include_directories: [include_directories('.')] + INCLUDES,
)
executable(
    'perfdhcp',
    'main.cc',
    include_directories: [include_directories('.')] + INCLUDES,
    install: true,
    install_dir: SBINDIR,
    install_rpath: INSTALL_RPATH,
    build_rpath: BUILD_RPATH,
    link_with: [perfdhcp_lib] + LIBS_BUILT_SO_FAR,
)
subdir('tests')
