if not TESTS_OPT.enabled()
    subdir_done()
endif

kea_dhcp_testutils_lib = static_library(
    'kea-dhcp-testutils',
    'iface_mgr_test_config.cc',
    'pkt_captures4.cc',
    'pkt_captures6.cc',
    'pkt_filter6_test_stub.cc',
    'pkt_filter_test_stub.cc',
    dependencies: [GTEST_DEP],
    include_directories: [include_directories('.')] + INCLUDES,
)
