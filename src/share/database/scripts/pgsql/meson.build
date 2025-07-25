install_dir = DATADIR / 'kea/scripts/pgsql'
configure_file(
    input: 'dhcpdb_create.pgsql',
    output: 'dhcpdb_create.pgsql',
    copy: true,
    install: true,
    install_dir: install_dir,
)
configure_file(
    input: 'dhcpdb_drop.pgsql',
    output: 'dhcpdb_drop.pgsql',
    copy: true,
    install: true,
    install_dir: install_dir,
)

pgsql_scripts_conf_data = configuration_data()
pgsql_scripts_conf_data.set('prefix', PREFIX)
if TESTS_OPT.enabled()
    pgsql_scripts_conf_data.set('abs_top_builddir', TOP_BUILD_DIR)
else
    pgsql_scripts_conf_data.set('abs_top_builddir', 'XXX-tests-builddir-XXX')
endif
pgsql_scripts_conf_data.set('datarootdir', '${prefix}/' + DATADIR)
pgsql_scripts_conf_data.set('PACKAGE_NAME', 'kea')
configure_file(
    input: 'wipe_data.sh.in',
    output: 'wipe_data.sh',
    configuration: pgsql_scripts_conf_data,
    install: true,
    install_dir: install_dir,
)
upgrade_scripts = [
    'upgrade_001.0_to_002.0.sh',
    'upgrade_002.0_to_003.0.sh',
    'upgrade_003.0_to_003.1.sh',
    'upgrade_003.1_to_003.2.sh',
    'upgrade_003.2_to_003.3.sh',
    'upgrade_003.3_to_004.0.sh',
    'upgrade_004.0_to_005.0.sh',
    'upgrade_005.0_to_005.1.sh',
    'upgrade_005.1_to_006.0.sh',
    'upgrade_006.0_to_006.1.sh',
    'upgrade_006.1_to_006.2.sh',
    'upgrade_006.2_to_007.0.sh',
    'upgrade_007_to_008.sh',
    'upgrade_008_to_009.sh',
    'upgrade_009_to_010.sh',
    'upgrade_010_to_011.sh',
    'upgrade_011_to_012.sh',
    'upgrade_012_to_013.sh',
    'upgrade_013_to_014.sh',
    'upgrade_014_to_015.sh',
    'upgrade_015_to_016.sh',
    'upgrade_016_to_017.sh',
    'upgrade_017_to_018.sh',
    'upgrade_018_to_019.sh',
    'upgrade_019_to_020.sh',
    'upgrade_020_to_021.sh',
    'upgrade_021_to_022.sh',
    'upgrade_022_to_023.sh',
    'upgrade_023_to_024.sh',
    'upgrade_024_to_025.sh',
    'upgrade_025_to_026.sh',
    'upgrade_026_to_027.sh',
    'upgrade_027_to_028.sh',
    'upgrade_028_to_029.sh',
    'upgrade_029_to_030.sh',
]
list = run_command(
    GRABBER,
    TOP_SOURCE_DIR / 'src/share/database/scripts/pgsql',
    'upgrade_*_to_*.sh.in',
    check: true,
)
grabbed_upgrade_scripts = list.stdout().strip().split('\n')
assert(
    upgrade_scripts.length() == grabbed_upgrade_scripts.length(),
    'incorrect list of upgrade scripts',
)
foreach file : upgrade_scripts
    configure_file(
        input: file + '.in',
        output: file,
        configuration: pgsql_scripts_conf_data,
        install: true,
        install_dir: install_dir,
    )
endforeach
