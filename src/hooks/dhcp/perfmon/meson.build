dhcp_perfmon_lib = shared_library(
    'dhcp_perfmon',
    'alarm.cc',
    'alarm_store.cc',
    'monitored_duration.cc',
    'monitored_duration_store.cc',
    'perfmon_callouts.cc',
    'perfmon_config.cc',
    'perfmon_log.cc',
    'perfmon_messages.cc',
    'perfmon_mgr.cc',
    'version.cc',
    dependencies: [crypto],
    include_directories: [include_directories('.')] + INCLUDES,
    install: true,
    install_dir: 'lib/kea/hooks',
    link_with: LIBS_BUILT_SO_FAR,
    name_suffix: 'so',
)
dhcp_perfmon_archive = static_library(
    'dhcp_perfmon',
    objects: dhcp_perfmon_lib.extract_all_objects(recursive: false),
)
subdir('libloadtests')
subdir('tests')
