if not TESTS_OPT.enabled()
    subdir_done()
endif

dhcp_host_cmds_libloadtests = executable(
    'dhcp-host-cmds-libload-tests',
    'load_unload_unittests.cc',
    'run_unittests.cc',
    cpp_args: [
        f'-DLIBDHCP_HOST_CMDS_SO="@TOP_BUILD_DIR@/src/hooks/dhcp/host_cmds/libdhcp_host_cmds.so"',
    ],
    dependencies: [GTEST_DEP, CRYPTO_DEP],
    include_directories: [include_directories('.')] + INCLUDES,
    link_with: LIBS_BUILT_SO_FAR,
)
test(
    'dhcp-host-cmds-libloadtests',
    dhcp_host_cmds_libloadtests,
    depends: [dhcp_host_cmds_lib],
    protocol: 'gtest',
)
