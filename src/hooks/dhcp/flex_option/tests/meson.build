if not TESTS_OPT.enabled()
    subdir_done()
endif

dhcp_flex_option_lib_tests = executable(
    'dhcp-flex-option-lib-tests',
    'flex_option_unittests.cc',
    'run_unittests.cc',
    'sub_option_unittests.cc',
    cpp_args: [
        f'-DFLEX_OPTION_LIB_SO="@TOP_BUILD_DIR@/src/hooks/dhcp/flex_option/libdhcp_flex_option.so"',
    ],
    dependencies: [GTEST_DEP, CRYPTO_DEP],
    include_directories: [include_directories('.'), include_directories('..')] + INCLUDES,
    link_with: [dhcp_flex_option_archive] + LIBS_BUILT_SO_FAR,
)
test('dhcp-flex-option-lib-tests', dhcp_flex_option_lib_tests, protocol: 'gtest')
