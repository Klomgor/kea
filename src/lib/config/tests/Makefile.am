SUBDIRS = .

AM_CPPFLAGS = -I$(top_builddir)/src/lib -I$(top_srcdir)/src/lib
AM_CPPFLAGS += $(BOOST_INCLUDES) $(CRYPTO_CFLAGS) $(CRYPTO_INCLUDES)
AM_CPPFLAGS += -DTEST_DATA_BUILDDIR=\"$(abs_top_builddir)/src/lib/config/tests\"
AM_CPPFLAGS += -DTEST_CA_DIR=\"$(srcdir)/../../asiolink/testutils/ca\"

AM_CXXFLAGS = $(KEA_CXXFLAGS)

if USE_STATIC_LINK
AM_LDFLAGS = -static
endif

CLEANFILES = *.gcno *.gcda

TESTS_ENVIRONMENT = $(LIBTOOL) --mode=execute $(VALGRIND_COMMAND)

TESTS =
if HAVE_GTEST
TESTS += run_unittests
run_unittests_SOURCES = client_connection_unittests.cc
run_unittests_SOURCES += run_unittests.cc
run_unittests_SOURCES += command_mgr_unittests.cc
run_unittests_SOURCES += unix_command_config_unittests.cc
run_unittests_SOURCES += unix_command_mgr_unittests.cc
run_unittests_SOURCES += cmd_http_listener_unittests.cc
run_unittests_SOURCES += cmd_response_creator_unittests.cc
run_unittests_SOURCES += cmd_response_creator_factory_unittests.cc
run_unittests_SOURCES += http_command_config_unittests.cc
run_unittests_SOURCES += http_command_mgr_unittests.cc
run_unittests_SOURCES += http_command_response_creator_factory_unittests.cc
run_unittests_SOURCES += http_command_response_creator_unittests.cc

run_unittests_CPPFLAGS = $(AM_CPPFLAGS) $(GTEST_INCLUDES)
run_unittests_LDFLAGS = $(AM_LDFLAGS) $(CRYPTO_LDFLAGS) $(GTEST_LDFLAGS)

run_unittests_LDADD  = $(top_builddir)/src/lib/config/libkea-cfgclient.la
run_unittests_LDADD += $(top_builddir)/src/lib/http/libkea-http.la
run_unittests_LDADD += $(top_builddir)/src/lib/dhcp/libkea-dhcp++.la
run_unittests_LDADD += $(top_builddir)/src/lib/hooks/libkea-hooks.la
run_unittests_LDADD += $(top_builddir)/src/lib/testutils/libkea-testutils.la
run_unittests_LDADD += $(top_builddir)/src/lib/cc/libkea-cc.la
run_unittests_LDADD += $(top_builddir)/src/lib/asiolink/testutils/libasiolinktest.la
run_unittests_LDADD += $(top_builddir)/src/lib/asiolink/libkea-asiolink.la
run_unittests_LDADD += $(top_builddir)/src/lib/dns/libkea-dns++.la
run_unittests_LDADD += $(top_builddir)/src/lib/cryptolink/libkea-cryptolink.la
run_unittests_LDADD += $(top_builddir)/src/lib/log/libkea-log.la
run_unittests_LDADD += $(top_builddir)/src/lib/util/unittests/libutil_unittests.la
run_unittests_LDADD += $(top_builddir)/src/lib/util/libkea-util.la
run_unittests_LDADD += $(top_builddir)/src/lib/exceptions/libkea-exceptions.la
run_unittests_LDADD += $(LOG4CPLUS_LIBS) $(CRYPTO_LIBS)
run_unittests_LDADD += $(BOOST_LIBS) $(GTEST_LDADD)

endif

noinst_PROGRAMS = $(TESTS)
