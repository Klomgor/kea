if not PYTHON.found()
    subdir_done()
endif

kea_shell_conf_data = configuration_data()
kea_shell_conf_data.set('PYTHON', PYTHON.full_path())
kea_shell_conf_data.set('PACKAGE_VERSION', PROJECT_VERSION)
kea_shell_conf_data.set('EXTENDED_VERSION', PROJECT_VERSION + ' (tarball)')

r = run_command(
    PYTHON,
    '-c',
    'import sysconfig; print(sysconfig.get_paths()[\'purelib\'])',
    check: true,
)
python_site_package = r.stdout().strip()
pkgpythondir = f'@python_site_package@/kea'
kea_shell_conf_data.set('PKGPYTHONDIR', pkgpythondir)
configure_file(
    input: 'kea-shell.in',
    output: 'kea-shell',
    configuration: kea_shell_conf_data,
    install: true,
    install_dir: get_option('sbindir'),
)
# Install into the system seti package location or not?
configure_file(
    input: 'kea_conn.py',
    output: 'kea_conn.py',
    copy: true,
    install: false,
    install_dir: pkgpythondir,
)
configure_file(
    input: 'kea_connector3.py',
    output: 'kea_connector3.py',
    copy: true,
    install: false,
    install_dir: pkgpythondir,
)
subdir('tests')
