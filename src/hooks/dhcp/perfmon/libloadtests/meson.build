if not TESTS_OPT.enabled()
    subdir_done()
endif

dhcp_perfmon_libload_tests = executable(
    'dhcp-perfmon-libload-tests',
    'load_unload_unittests.cc',
    'run_unittests.cc',
    cpp_args: [
        f'-DLIBDHCP_PERFMON_SO="@TOP_BUILD_DIR@/src/hooks/dhcp/perfmon/libdhcp_perfmon.so"',
    ],
    dependencies: [GTEST_DEP, CRYPTO_DEP],
    include_directories: [include_directories('.'), include_directories('..')] + INCLUDES,
    link_with: LIBS_BUILT_SO_FAR,
)
test(
    'dhcp-perfmon-libload-tests',
    dhcp_perfmon_libload_tests,
    depends: [dhcp_perfmon_lib],
    protocol: 'gtest',
)
