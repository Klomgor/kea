if not TESTS_OPT.enabled()
    subdir_done()
endif

current_build_dir = meson.current_build_dir()
configure_file(
    input: 'process_spawn_app.sh.in',
    output: 'process_spawn_app.sh',
    copy: true,
)
kea_asiolink_tests = executable(
    'kea-asiolink-tests',
    'addr_utilities_unittest.cc',
    'dummy_io_callback_unittest.cc',
    'hash_address_unittest.cc',
    'interval_timer_unittest.cc',
    'io_address_unittest.cc',
    'io_endpoint_unittest.cc',
    'io_service_mgr_unittest.cc',
    'io_service_signal_unittests.cc',
    'io_service_thread_pool_unittests.cc',
    'io_service_unittest.cc',
    'io_socket_unittest.cc',
    'process_spawn_unittest.cc',
    'run_unittests.cc',
    'tcp_acceptor_unittest.cc',
    'tcp_endpoint_unittest.cc',
    'tcp_socket_unittest.cc',
    'tls_acceptor_unittest.cc',
    'tls_socket_unittest.cc',
    'tls_unittest.cc',
    'udp_endpoint_unittest.cc',
    'udp_socket_unittest.cc',
    'unix_domain_socket_unittest.cc',
    cpp_args: [
        f'-DTEST_SCRIPT_SH="@current_build_dir@/process_spawn_app.sh"',
        f'-DINVALID_TEST_SCRIPT_SH="@TOP_SOURCE_DIR@/README"',
        f'-DTEST_CA_DIR="@TEST_CA_DIR@"',
    ],
    dependencies: [GTEST_DEP, CRYPTO_DEP],
    include_directories: [include_directories('.')] + INCLUDES,
    link_with: [kea_asiolink_testutils_lib, kea_util_unittests_lib] + LIBS_BUILT_SO_FAR,
)
test(
    'kea-asiolink-tests',
    kea_asiolink_tests,
    protocol: 'gtest',
    is_parallel: false,
    priority: -1,
)
