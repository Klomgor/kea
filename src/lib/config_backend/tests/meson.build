if not TESTS_OPT.enabled()
    subdir_done()
endif

kea_config_backend_tests = executable(
    'kea-config_backend-tests',
    'config_backend_mgr_unittest.cc',
    'run_unittests.cc',
    dependencies: [GTEST_DEP],
    include_directories: [include_directories('.')] + INCLUDES,
    link_with: [kea_testutils_lib, kea_util_unittests_lib] + LIBS_BUILT_SO_FAR,
)
test('kea-config-backend-tests', kea_config_backend_tests, protocol: 'gtest')
