if not TESTS_OPT.enabled()
    subdir_done()
endif

run_script_test_conf_data = configuration_data()
run_script_test_conf_data.set('abs_top_builddir', TOP_BUILD_DIR)
configure_file(
    input: 'run_script_test.sh.in',
    output: 'run_script_test.sh',
    configuration: run_script_test_conf_data,
)

current_build_dir = meson.current_build_dir()
dhcp_run_script_lib_tests = executable(
    'dhcp-run-script-lib-tests',
    'run_script_unittests.cc',
    'run_unittests.cc',
    cpp_args: [
        f'-DTEST_DATA_BUILDDIR="@current_build_dir@"',
        f'-DRUN_SCRIPT_LIB_SO="@TOP_BUILD_DIR@/src/hooks/dhcp/run_script/libdhcp_run_script.so"',
        f'-DTEST_LOG_FILE="@current_build_dir@/test.log"',
        f'-DRUN_SCRIPT_TEST_SH="@current_build_dir@/run_script_test.sh"',
    ],
    dependencies: [GTEST_DEP, CRYPTO_DEP],
    include_directories: [include_directories('.'), include_directories('..')] + INCLUDES,
    link_with: [dhcp_run_script_archive] + LIBS_BUILT_SO_FAR,
)
test('dhcp-run-script-lib-tests', dhcp_run_script_lib_tests, protocol: 'gtest')
