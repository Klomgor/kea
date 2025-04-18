SUBDIRS = . tests

AM_CPPFLAGS = -I$(top_srcdir)/src/lib -I$(top_builddir)/src/lib
AM_CPPFLAGS += -I$(top_srcdir)/src/bin -I$(top_builddir)/src/bin
AM_CPPFLAGS += -I$(top_srcdir)/src -I$(top_builddir)/src
AM_CPPFLAGS += $(BOOST_INCLUDES) $(CRYPTO_CFLAGS) $(CRYPTO_INCLUDES)

AM_CXXFLAGS = $(KEA_CXXFLAGS)

if USE_STATIC_LINK
AM_LDFLAGS = -static
endif

CLEANFILES  = *.gcno *.gcda

EXTRA_DIST = dhcp6.dox dhcp6_hooks.dox dhcp4o6.dox
EXTRA_DIST += dhcp6_parser.yy

# convenience archive

noinst_LTLIBRARIES = libdhcp6.la

libdhcp6_la_SOURCES  =
libdhcp6_la_SOURCES += ctrl_dhcp6_srv.cc ctrl_dhcp6_srv.h
libdhcp6_la_SOURCES += json_config_parser.cc json_config_parser.h
libdhcp6_la_SOURCES += dhcp6_log.cc dhcp6_log.h
libdhcp6_la_SOURCES += dhcp6_srv.cc dhcp6_srv.h
libdhcp6_la_SOURCES += dhcp6to4_ipc.cc dhcp6to4_ipc.h
libdhcp6_la_SOURCES += client_handler.cc client_handler.h
libdhcp6_la_SOURCES += dhcp6_lexer.ll location.hh
libdhcp6_la_SOURCES += dhcp6_parser.cc dhcp6_parser.h
libdhcp6_la_SOURCES += parser_context.cc parser_context.h parser_context_decl.h
libdhcp6_la_SOURCES += dhcp6_messages.h dhcp6_messages.cc
EXTRA_DIST += dhcp6_messages.mes

sbin_PROGRAMS = kea-dhcp6

kea_dhcp6_SOURCES  = main.cc

kea_dhcp6_LDADD  = libdhcp6.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/dhcpsrv/libkea-dhcpsrv.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/process/libkea-process.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/eval/libkea-eval.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/dhcp_ddns/libkea-dhcp_ddns.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/stats/libkea-stats.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/config/libkea-cfgclient.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/http/libkea-http.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/dhcp/libkea-dhcp++.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/hooks/libkea-hooks.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/database/libkea-database.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/cc/libkea-cc.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/asiolink/libkea-asiolink.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/dns/libkea-dns++.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/cryptolink/libkea-cryptolink.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/log/libkea-log.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/util/libkea-util.la
kea_dhcp6_LDADD += $(top_builddir)/src/lib/exceptions/libkea-exceptions.la
kea_dhcp6_LDADD += $(LOG4CPLUS_LIBS) $(CRYPTO_LIBS) $(BOOST_LIBS)

kea_dhcp6_LDFLAGS = $(AM_LDFLAGS) $(CRYPTO_LDFLAGS)

kea_dhcp6dir = $(pkgdatadir)

# If we want to get rid of all generated messages files, we need to use
# make maintainer-clean. The proper way to introduce custom commands for
# that operation is to define maintainer-clean-local target. However,
# make maintainer-clean also removes Makefile, so running configure script
# is required.  To make it easy to rebuild messages without going through
# reconfigure, a new target messages-clean has been added.
maintainer-clean-local:
	rm -f dhcp6_messages.h dhcp6_messages.cc

# To regenerate messages files, one can do:
#
# make messages-clean
# make messages
#
# This is needed only when a .mes file is modified.
messages-clean: maintainer-clean-local

if GENERATE_MESSAGES

# Define rule to build logging source files from message file
messages: dhcp6_messages.h dhcp6_messages.cc
	@echo Message files regenerated

dhcp6_messages.h dhcp6_messages.cc: dhcp6_messages.mes
	(cd $(top_srcdir); \
	$(abs_top_builddir)/src/lib/log/compiler/kea-msg-compiler src/bin/dhcp6/dhcp6_messages.mes)

else

messages dhcp6_messages.h dhcp6_messages.cc:
	@echo Messages generation disabled. Configure with --enable-generate-messages to enable it.

endif

if GENERATE_PARSER

# Generate parser first.
all-recursive: dhcp6_lexer.cc location.hh dhcp6_parser.cc dhcp6_parser.h

parser: dhcp6_lexer.cc location.hh dhcp6_parser.cc dhcp6_parser.h
	@echo "Flex/bison files regenerated"

# --- Flex/Bison stuff below --------------------------------------------------
# When debugging grammar issues, it's useful to add -v to bison parameters.
# bison will generate parser.output file that explains the whole grammar.
# It can be used to manually follow what's going on in the parser.
# This is especially useful if yydebug_ is set to 1 as that variable
# will cause parser to print out its internal state.
# Call flex with -s to check that the default rule can be suppressed
# Call bison with -W to get warnings like unmarked empty rules
# Note C++11 deprecated register still used by flex < 2.6.0
location.hh dhcp6_parser.cc dhcp6_parser.h: dhcp6_parser.yy
	$(YACC) -Wno-yacc --defines=dhcp6_parser.h --report=all \
	--report-file=dhcp6_parser.report -o dhcp6_parser.cc dhcp6_parser.yy

dhcp6_lexer.cc: dhcp6_lexer.ll
	$(LEX) --prefix parser6_ -o dhcp6_lexer.cc dhcp6_lexer.ll

else

parser location.hh dhcp6_parser.cc dhcp6_parser.h dhcp6_lexer.cc:
	@echo Parser generation disabled. Configure with --enable-generate-parser to enable it.

endif
