if not gtest.found()
    subdir_done()
endif

kea_d2srv_testutils_lib = static_library(
    'kea-d2srv-testutils',
    'nc_test_utils.cc',
    'stats_test_utils.cc',
    dependencies: [crypto, gtest],
    include_directories: [include_directories('.')] + INCLUDES,
)
