if not TESTS_OPT.enabled()
    subdir_done()
endif

kea_database_tests = executable(
    'kea-database-tests',
    'audit_entry_unittest.cc',
    'backend_selector_unittest.cc',
    'database_connection_unittest.cc',
    'database_log_unittest.cc',
    'dbaccess_parser_unittest.cc',
    'run_unittests.cc',
    'server_selector_unittest.cc',
    'server_unittest.cc',
    dependencies: [GTEST_DEP],
    include_directories: [include_directories('.')] + INCLUDES,
    link_with: [kea_testutils_lib, kea_util_unittests_lib] + LIBS_BUILT_SO_FAR,
)
test('kea-database-tests', kea_database_tests, protocol: 'gtest')
