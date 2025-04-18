dhcp6_lib = static_library(
    'dhcp6',
    'client_handler.cc',
    'ctrl_dhcp6_srv.cc',
    'dhcp6to4_ipc.cc',
    'dhcp6_lexer.cc',
    'dhcp6_log.cc',
    'dhcp6_messages.cc',
    'dhcp6_parser.cc',
    'dhcp6_srv.cc',
    'json_config_parser.cc',
    'main.cc',
    'parser_context.cc',
    dependencies: [CRYPTO_DEP],
    include_directories: [include_directories('.')] + INCLUDES,
)
kea_dhcp6 = executable(
    'kea-dhcp6',
    'main.cc',
    dependencies: [CRYPTO_DEP],
    include_directories: [include_directories('.')] + INCLUDES,
    install: true,
    install_dir: SBINDIR,
    install_rpath: INSTALL_RPATH,
    build_rpath: BUILD_RPATH,
    link_with: [dhcp6_lib] + LIBS_BUILT_SO_FAR,
)
subdir('tests')

current_source_dir = meson.current_source_dir()
if KEA_MSG_COMPILER.found()
    target_gen_messages = run_target(
        'src-bin-dhcp6-dhcp6_messages',
        command: [
            CD_AND_RUN,
            TOP_SOURCE_DIR,
            KEA_MSG_COMPILER,
            'src/bin/dhcp6/dhcp6_messages.mes',
        ],
    )
    TARGETS_GEN_MESSAGES += [target_gen_messages]
endif

if FLEX.found() and BISON.found()
    target_parser = run_target(
        'src-bin-dhcp6-parser',
        command: [
            CD_AND_RUN,
            current_source_dir,
            BISON,
            '-Wno-yacc',
            '--defines=dhcp6_parser.h',
            '--report=all',
            '--report-file=dhcp6_parser.report',
            '-o',
            'dhcp6_parser.cc',
            'dhcp6_parser.yy',
        ],
    )
    target_lexer = run_target(
        'src-bin-dhcp6-lexer',
        command: [
            CD_AND_RUN,
            current_source_dir,
            FLEX,
            '--prefix',
            'parser6_',
            '-o',
            'dhcp6_lexer.cc',
            'dhcp6_lexer.ll',
        ],
    )
    TARGETS_GEN_PARSER += [target_parser, target_lexer]
endif
