if not gtest.found()
    subdir_done()
endif

kea_database_testutils_lib = static_library(
    'kea-database-testutils',
    'schema.cc',
    cpp_args: [f'-DTEST_CA_DIR="@TEST_CA_DIR@"'],
    include_directories: [include_directories('.')] + INCLUDES,
)
