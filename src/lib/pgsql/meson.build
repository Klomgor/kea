if not POSTGRESQL_DEP.found()
    subdir_done()
endif

kea_pgsql_lib = shared_library(
    'kea-pgsql',
    'pgsql_connection.cc',
    'pgsql_exchange.cc',
    cpp_args: [f'-DKEA_ADMIN="@KEA_ADMIN_INSTALLED@"'],
    dependencies: [POSTGRESQL_DEP],
    include_directories: [include_directories('.')] + INCLUDES,
    install: true,
    install_dir: LIBDIR,
    install_rpath: INSTALL_RPATH,
    build_rpath: BUILD_RPATH,
    link_with: LIBS_BUILT_SO_FAR,
    version: '99.0.0',
)
LIBS_BUILT_SO_FAR = [kea_pgsql_lib] + LIBS_BUILT_SO_FAR
subdir('testutils')
subdir('tests')
kea_pgsql_headers = ['pgsql_connection.h', 'pgsql_exchange.h']
install_headers(kea_pgsql_headers, preserve_path: true, subdir: 'kea/pgsql')
