lfc_lib = static_library(
    'lfc',
    'lfc_controller.cc',
    'lfc_log.cc',
    'lfc_messages.cc',
    include_directories: [include_directories('.')] + INCLUDES,
)
kea_lfc = executable(
    'kea-lfc',
    'main.cc',
    include_directories: [include_directories('.')] + INCLUDES,
    install: true,
    install_dir: SBINDIR,
    install_rpath: INSTALL_RPATH,
    build_rpath: BUILD_RPATH,
    link_with: [lfc_lib] + LIBS_BUILT_SO_FAR,
)
subdir('tests')

if KEA_MSG_COMPILER.found()
    target_gen_messages = run_target(
        'src-bin-lfc-lfc_messages',
        command: [
            CD_AND_RUN,
            TOP_SOURCE_DIR,
            KEA_MSG_COMPILER,
            'src/bin/lfc/lfc_messages.mes',
        ],
    )
    TARGETS_GEN_MESSAGES += [target_gen_messages]
endif
