if not POSTGRESQL_DEP.found()
    subdir_done()
endif

dhcp_pgsql_lib = shared_library(
    'dhcp_pgsql',
    'pgsql_callouts.cc',
    'pgsql_cb_dhcp4.cc',
    'pgsql_cb_dhcp6.cc',
    'pgsql_cb_impl.cc',
    'pgsql_cb_log.cc',
    'pgsql_cb_messages.cc',
    'pgsql_fb_log.cc',
    'pgsql_fb_messages.cc',
    'pgsql_hb_log.cc',
    'pgsql_hb_messages.cc',
    'pgsql_host_data_source.cc',
    'pgsql_lb_log.cc',
    'pgsql_lb_messages.cc',
    'pgsql_lease_mgr.cc',
    'pgsql_legal_log.cc',
    'version.cc',
    dependencies: [CRYPTO_DEP, POSTGRESQL_DEP],
    include_directories: [include_directories('.')] + INCLUDES,
    install: true,
    install_dir: HOOKS_PATH,
    install_rpath: INSTALL_RPATH,
    build_rpath: BUILD_RPATH,
    link_with: LIBS_BUILT_SO_FAR,
    name_suffix: 'so',
)
dhcp_pgsql_archive = static_library(
    'dhcp_pgsql',
    objects: dhcp_pgsql_lib.extract_all_objects(recursive: false),
)
subdir('libloadtests')
subdir('tests')

if KEA_MSG_COMPILER.found()
    target_gen_messages = run_target(
        'src-hooks-dhcp-pgsql-pgsql_cb_messages',
        command: [
            CD_AND_RUN,
            TOP_SOURCE_DIR,
            KEA_MSG_COMPILER,
            'src/hooks/dhcp/pgsql/pgsql_cb_messages.mes',
        ],
    )
    TARGETS_GEN_MESSAGES += [target_gen_messages]
    target_gen_messages = run_target(
        'src-hooks-dhcp-pgsql-pgsql_fb_messages',
        command: [
            CD_AND_RUN,
            TOP_SOURCE_DIR,
            KEA_MSG_COMPILER,
            'src/hooks/dhcp/pgsql/pgsql_fb_messages.mes',
        ],
    )
    TARGETS_GEN_MESSAGES += [target_gen_messages]
    target_gen_messages = run_target(
        'src-hooks-dhcp-pgsql-pgsql_hb_messages',
        command: [
            CD_AND_RUN,
            TOP_SOURCE_DIR,
            KEA_MSG_COMPILER,
            'src/hooks/dhcp/pgsql/pgsql_hb_messages.mes',
        ],
    )
    TARGETS_GEN_MESSAGES += [target_gen_messages]
    target_gen_messages = run_target(
        'src-hooks-dhcp-pgsql-pgsql_lb_messages',
        command: [
            CD_AND_RUN,
            TOP_SOURCE_DIR,
            KEA_MSG_COMPILER,
            'src/hooks/dhcp/pgsql/pgsql_lb_messages.mes',
        ],
    )
    TARGETS_GEN_MESSAGES += [target_gen_messages]
endif
