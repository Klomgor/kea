/* Copyright (C) 2016-2025 Internet Systems Consortium, Inc. ("ISC")

   This Source Code Form is subject to the terms of the Mozilla Public
   License, v. 2.0. If a copy of the MPL was not distributed with this
   file, You can obtain one at http://mozilla.org/MPL/2.0/. */

%{ /* -*- C++ -*- */

/* Generated files do not make clang static analyser so happy */
#ifndef __clang_analyzer__

#include <cctype>
#include <cerrno>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <dhcp4/parser_context.h>
#include <asiolink/io_address.h>
#include <boost/lexical_cast.hpp>
#include <exceptions/exceptions.h>

/* Please avoid C++ style comments (// ... eol) as they break flex 2.6.2 */

/* Work around an incompatibility in flex (at least versions
   2.5.31 through 2.5.33): it generates code that does
   not conform to C89.  See Debian bug 333231
   <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>. */
# undef yywrap
# define yywrap() 1

namespace {

bool start_token_flag = false;

isc::dhcp::Parser4Context::ParserType start_token_value;
unsigned int comment_start_line = 0;

using namespace isc::dhcp;

};

/* To avoid the call to exit... oops! */
#define YY_FATAL_ERROR(msg) isc::dhcp::Parser4Context::fatal(msg)
%}

/* noyywrap disables automatic rewinding for the next file to parse. Since we
   always parse only a single string, there's no need to do any wraps. And
   using yywrap requires linking with -lfl, which provides the default yywrap
   implementation that always returns 1 anyway. */
%option noyywrap

/* nounput simplifies the lexer, by removing support for putting a character
   back into the input stream. We never use such capability anyway. */
%option nounput

/* batch means that we'll never use the generated lexer interactively. */
%option batch

/* avoid to get static global variables to remain with C++. */
/* in last resort %option reentrant */

/* Enables debug mode. To see the debug messages, one needs to also set
   yy_flex_debug to 1, then the debug messages will be printed on stderr. */
%option debug

/* I have no idea what this option does, except it was specified in the bison
   examples and Postgres folks added it to remove gcc 4.3 warnings. Let's
   be on the safe side and keep it. */
%option noinput

%x COMMENT
%x DIR_ENTER DIR_INCLUDE DIR_EXIT

/* These are not token expressions yet, just convenience expressions that
   can be used during actual token definitions. Note some can match
   incorrect inputs (e.g., IP addresses) which must be checked. */
int   \-?[0-9]+
blank [ \t\r]

UnicodeEscapeSequence           u[0-9A-Fa-f]{4}
JSONEscapeCharacter             ["\\/bfnrt]
JSONEscapeSequence              {JSONEscapeCharacter}|{UnicodeEscapeSequence}
JSONStandardCharacter           [^\x00-\x1f"\\]
JSONStringCharacter             {JSONStandardCharacter}|\\{JSONEscapeSequence}
JSONString                      \"{JSONStringCharacter}*\"

/* for errors */

BadUnicodeEscapeSequence        u[0-9A-Fa-f]{0,3}[^0-9A-Fa-f"]
BadJSONEscapeSequence           [^"\\/bfnrtu]|{BadUnicodeEscapeSequence}
ControlCharacter                [\x00-\x1f]
ControlCharacterFill            [^"\\]|\\["\\/bfnrtu]

%{
/* This code run each time a pattern is matched. It updates the location
   by moving it ahead by yyleng bytes. yyleng specifies the length of the
   currently matched token. */
#define YY_USER_ACTION  driver.loc_.columns(yyleng);
%}

%%

%{
    /* This part of the code is copied over to the verbatim to the top
       of the generated yylex function. Explanation:
       http://www.gnu.org/software/bison/manual/html_node/Multiple-start_002dsymbols.html */

    /* Code run each time yylex is called. */
    driver.loc_.step();

    if (start_token_flag) {
        start_token_flag = false;
        switch (start_token_value) {
        case Parser4Context::PARSER_JSON:
        default:
            return isc::dhcp::Dhcp4Parser::make_TOPLEVEL_JSON(driver.loc_);
        case Parser4Context::PARSER_DHCP4:
            return isc::dhcp::Dhcp4Parser::make_TOPLEVEL_DHCP4(driver.loc_);
        case Parser4Context::SUBPARSER_DHCP4:
            return isc::dhcp::Dhcp4Parser::make_SUB_DHCP4(driver.loc_);
        case Parser4Context::PARSER_INTERFACES:
            return isc::dhcp::Dhcp4Parser::make_SUB_INTERFACES4(driver.loc_);
        case Parser4Context::PARSER_SUBNET4:
            return isc::dhcp::Dhcp4Parser::make_SUB_SUBNET4(driver.loc_);
        case Parser4Context::PARSER_POOL4:
            return isc::dhcp::Dhcp4Parser::make_SUB_POOL4(driver.loc_);
        case Parser4Context::PARSER_HOST_RESERVATION:
            return isc::dhcp::Dhcp4Parser::make_SUB_RESERVATION(driver.loc_);
        case Parser4Context::PARSER_OPTION_DEFS:
            return isc::dhcp::Dhcp4Parser::make_SUB_OPTION_DEFS(driver.loc_);
        case Parser4Context::PARSER_OPTION_DEF:
            return isc::dhcp::Dhcp4Parser::make_SUB_OPTION_DEF(driver.loc_);
        case Parser4Context::PARSER_OPTION_DATA:
            return isc::dhcp::Dhcp4Parser::make_SUB_OPTION_DATA(driver.loc_);
        case Parser4Context::PARSER_HOOKS_LIBRARY:
            return isc::dhcp::Dhcp4Parser::make_SUB_HOOKS_LIBRARY(driver.loc_);
        case Parser4Context::PARSER_DHCP_DDNS:
            return isc::dhcp::Dhcp4Parser::make_SUB_DHCP_DDNS(driver.loc_);
        case Parser4Context::PARSER_CONFIG_CONTROL:
            return isc::dhcp::Dhcp4Parser::make_SUB_CONFIG_CONTROL(driver.loc_);
        }
    }
%}

#.* ;

"//"(.*) ;

"/*" {
  BEGIN(COMMENT);
  comment_start_line = driver.loc_.end.line;;
}

<COMMENT>"*/" BEGIN(INITIAL);
<COMMENT>. ;
<COMMENT><<EOF>> {
    isc_throw(Dhcp4ParseError, "Comment not closed. (/* in line " << comment_start_line);
}

"<?" BEGIN(DIR_ENTER);
<DIR_ENTER>"include" BEGIN(DIR_INCLUDE);
<DIR_INCLUDE>\"([^\"\n])+\" {
    /* Include directive. */

    /* Extract the filename. */
    std::string tmp(yytext+1);
    tmp.resize(tmp.size() - 1);

    driver.includeFile(tmp);
}
<DIR_ENTER,DIR_INCLUDE,DIR_EXIT><<EOF>> {
    isc_throw(Dhcp4ParseError, "Directive not closed.");
}
<DIR_EXIT>"?>" BEGIN(INITIAL);

<*>{blank}+   {
    /* Ok, we found a with space. Let's ignore it and update loc variable. */
    driver.loc_.step();
}

<*>[\n]+      {
    /* Newline found. Let's update the location and continue. */
    driver.loc_.lines(yyleng);
    driver.loc_.step();
}

\"Dhcp4\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CONFIG:
        return isc::dhcp::Dhcp4Parser::make_DHCP4(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("Dhcp4", driver.loc_);
    }
}

\"interfaces-config\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_INTERFACES_CONFIG(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("interfaces-config", driver.loc_);
    }
}

\"sanity-checks\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_SANITY_CHECKS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("sanity-checks", driver.loc_);
    }
}

\"lease-checks\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SANITY_CHECKS:
        return isc::dhcp::Dhcp4Parser::make_LEASE_CHECKS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("lease-checks", driver.loc_);
    }
}

\"extended-info-checks\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SANITY_CHECKS:
        return isc::dhcp::Dhcp4Parser::make_EXTENDED_INFO_CHECKS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("extended-info-checks", driver.loc_);
    }
}

\"dhcp-socket-type\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::INTERFACES_CONFIG:
        return isc::dhcp::Dhcp4Parser::make_DHCP_SOCKET_TYPE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("dhcp-socket-type", driver.loc_);
    }
}

\"raw\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_SOCKET_TYPE:
        return isc::dhcp::Dhcp4Parser::make_RAW(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("raw", driver.loc_);
    }
}

\"udp\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_SOCKET_TYPE:
    case isc::dhcp::Parser4Context::NCR_PROTOCOL:
        return isc::dhcp::Dhcp4Parser::make_UDP(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("udp", driver.loc_);
    }
}

\"outbound-interface\" {
    switch(driver.ctx_) {
    case Parser4Context::INTERFACES_CONFIG:
        return isc::dhcp::Dhcp4Parser::make_OUTBOUND_INTERFACE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("outbound-interface", driver.loc_);
    }
}

\"same-as-inbound\" {
    switch(driver.ctx_) {
    case Parser4Context::OUTBOUND_INTERFACE:
        return Dhcp4Parser::make_SAME_AS_INBOUND(driver.loc_);
    default:
        return Dhcp4Parser::make_STRING("same-as-inbound", driver.loc_);
    }
}

\"use-routing\" {
    switch(driver.ctx_) {
    case Parser4Context::OUTBOUND_INTERFACE:
        return Dhcp4Parser::make_USE_ROUTING(driver.loc_);
    default:
        return Dhcp4Parser::make_STRING("use-routing", driver.loc_);
    }
}

\"interfaces\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::INTERFACES_CONFIG:
        return isc::dhcp::Dhcp4Parser::make_INTERFACES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("interfaces", driver.loc_);
    }
}

\"re-detect\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::INTERFACES_CONFIG:
        return isc::dhcp::Dhcp4Parser::make_RE_DETECT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("re-detect", driver.loc_);
    }
}

\"service-sockets-require-all\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::INTERFACES_CONFIG:
        return isc::dhcp::Dhcp4Parser::make_SERVICE_SOCKETS_REQUIRE_ALL(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("service-sockets-require-all", driver.loc_);
    }
}

\"service-sockets-retry-wait-time\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::INTERFACES_CONFIG:
        return isc::dhcp::Dhcp4Parser::make_SERVICE_SOCKETS_RETRY_WAIT_TIME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("service-sockets-retry-wait-time", driver.loc_);
    }
}

\"service-sockets-max-retries\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::INTERFACES_CONFIG:
        return isc::dhcp::Dhcp4Parser::make_SERVICE_SOCKETS_MAX_RETRIES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("service-sockets-max-retries", driver.loc_);
    }
}

\"lease-database\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_LEASE_DATABASE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("lease-database", driver.loc_);
    }
}

\"hosts-database\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_HOSTS_DATABASE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("hosts-database", driver.loc_);
    }
}

\"hosts-databases\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_HOSTS_DATABASES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("hosts-databases", driver.loc_);
    }
}

\"config-control\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_CONFIG_CONTROL(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("config-control", driver.loc_);
    }
}

\"config-databases\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CONFIG_CONTROL:
        return isc::dhcp::Dhcp4Parser::make_CONFIG_DATABASES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("config-databases", driver.loc_);
    }
}

\"config-fetch-wait-time\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CONFIG_CONTROL:
        return isc::dhcp::Dhcp4Parser::make_CONFIG_FETCH_WAIT_TIME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("config-fetch-wait-time", driver.loc_);
    }
}

\"readonly\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_READONLY(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("readonly", driver.loc_);
    }
}

\"type\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::OPTION_DEF:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
    case isc::dhcp::Parser4Context::AUTHENTICATION:
        return isc::dhcp::Dhcp4Parser::make_TYPE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("type", driver.loc_);
    }
}

\"user\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
    case isc::dhcp::Parser4Context::CLIENTS:
        return isc::dhcp::Dhcp4Parser::make_USER(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("user", driver.loc_);
    }
}

\"password\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
    case isc::dhcp::Parser4Context::CLIENTS:
        return isc::dhcp::Dhcp4Parser::make_PASSWORD(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("password", driver.loc_);
    }
}

\"host\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_HOST(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("host", driver.loc_);
    }
}

\"port\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_PORT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("port", driver.loc_);
    }
}

\"persist\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_PERSIST(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("persist", driver.loc_);
    }
}

\"lfc-interval\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_LFC_INTERVAL(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("lfc-interval", driver.loc_);
    }
}

\"connect-timeout\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_CONNECT_TIMEOUT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("connect-timeout", driver.loc_);
    }
}

\"read-timeout\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_READ_TIMEOUT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("read-timeout", driver.loc_);
    }
}

\"write-timeout\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_WRITE_TIMEOUT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("write-timeout", driver.loc_);
    }
}

\"tcp-user-timeout\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_TCP_USER_TIMEOUT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("tcp-user-timeout", driver.loc_);
    }
}

\"reconnect-wait-time\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_RECONNECT_WAIT_TIME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("reconnect-wait-time", driver.loc_);
    }
}

\"on-fail\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_ON_FAIL(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("on-fail", driver.loc_);
    }
}

\"stop-retry-exit\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DATABASE_ON_FAIL:
        return isc::dhcp::Dhcp4Parser::make_STOP_RETRY_EXIT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("stop-retry-exit", driver.loc_);
    }
}

\"serve-retry-exit\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DATABASE_ON_FAIL:
        return isc::dhcp::Dhcp4Parser::make_SERVE_RETRY_EXIT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("serve-retry-exit", driver.loc_);
    }
}

\"serve-retry-continue\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DATABASE_ON_FAIL:
        return isc::dhcp::Dhcp4Parser::make_SERVE_RETRY_CONTINUE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("serve-retry-continue", driver.loc_);
    }
}

\"retry-on-startup\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_RETRY_ON_STARTUP(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("retry-on-startup", driver.loc_);
    }
}

\"max-reconnect-tries\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_MAX_RECONNECT_TRIES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("max-reconnect-tries", driver.loc_);
    }
}

\"max-row-errors\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_MAX_ROW_ERRORS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("max-row-errors", driver.loc_);
    }
}

\"trust-anchor\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
    case isc::dhcp::Parser4Context::CONTROL_SOCKET:
        return isc::dhcp::Dhcp4Parser::make_TRUST_ANCHOR(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("trust-anchor", driver.loc_);
    }
}

\"cert-file\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
    case isc::dhcp::Parser4Context::CONTROL_SOCKET:
        return isc::dhcp::Dhcp4Parser::make_CERT_FILE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("cert-file", driver.loc_);
    }
}

\"key-file\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
    case isc::dhcp::Parser4Context::CONTROL_SOCKET:
        return isc::dhcp::Dhcp4Parser::make_KEY_FILE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("key-file", driver.loc_);
    }
}

\"ssl-mode\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_SSL_MODE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ssl-mode", driver.loc_);
    }
}

\"disable\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SSL_MODE:
        return isc::dhcp::Dhcp4Parser::make_DISABLE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("disable", driver.loc_);
    }
}

\"prefer\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SSL_MODE:
        return isc::dhcp::Dhcp4Parser::make_PREFER(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("prefer", driver.loc_);
    }
}

\"require\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SSL_MODE:
        return isc::dhcp::Dhcp4Parser::make_REQUIRE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("require", driver.loc_);
    }
}

\"verify-ca\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SSL_MODE:
        return isc::dhcp::Dhcp4Parser::make_VERIFY_CA(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("verify-ca", driver.loc_);
    }
}

\"verify-full\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SSL_MODE:
        return isc::dhcp::Dhcp4Parser::make_VERIFY_FULL(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("verify-full", driver.loc_);
    }
}

\"cipher-list\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
        return isc::dhcp::Dhcp4Parser::make_CIPHER_LIST(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("cipher-list", driver.loc_);
    }
}

\"valid-lifetime\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
        return isc::dhcp::Dhcp4Parser::make_VALID_LIFETIME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("valid-lifetime", driver.loc_);
    }
}

\"min-valid-lifetime\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
        return isc::dhcp::Dhcp4Parser::make_MIN_VALID_LIFETIME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("min-valid-lifetime", driver.loc_);
    }
}

\"max-valid-lifetime\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
        return isc::dhcp::Dhcp4Parser::make_MAX_VALID_LIFETIME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("max-valid-lifetime", driver.loc_);
    }
}

\"renew-timer\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_RENEW_TIMER(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("renew-timer", driver.loc_);
    }
}

\"rebind-timer\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_REBIND_TIMER(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("rebind-timer", driver.loc_);
    }
}

\"decline-probation-period\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_DECLINE_PROBATION_PERIOD(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("decline-probation-period", driver.loc_);
    }
}

\"server-tag\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_SERVER_TAG(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("server-tag", driver.loc_);
    }
}

\"parked-packet-limit\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_PARKED_PACKET_LIMIT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("parked-packet-limit", driver.loc_);
    }
}

\"allocator\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_ALLOCATOR(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("allocator", driver.loc_);
    }
}

\"statistic-default-sample-count\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_STATISTIC_DEFAULT_SAMPLE_COUNT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("statistic-default-sample-count", driver.loc_);
    }
}

\"statistic-default-sample-age\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_STATISTIC_DEFAULT_SAMPLE_AGE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("statistic-default-sample-age", driver.loc_);
    }
}

\"ddns-send-updates\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_DDNS_SEND_UPDATES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ddns-send-updates", driver.loc_);
    }
}

\"ddns-override-no-update\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_DDNS_OVERRIDE_NO_UPDATE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ddns-override-no-update", driver.loc_);
    }
}

\"ddns-override-client-update\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_DDNS_OVERRIDE_CLIENT_UPDATE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ddns-override-client-update", driver.loc_);
    }
}

\"ddns-replace-client-name\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_DDNS_REPLACE_CLIENT_NAME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ddns-replace-client-name", driver.loc_);
    }
}

\"ddns-generated-prefix\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_DDNS_GENERATED_PREFIX(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ddns-generated-prefix", driver.loc_);
    }
}

\"ddns-qualifying-suffix\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_DDNS_QUALIFYING_SUFFIX(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ddns-qualifying-suffix", driver.loc_);
    }
}

\"ddns-update-on-renew\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_DDNS_UPDATE_ON_RENEW(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ddns-update-on-renew", driver.loc_);
    }
}

\"ddns-use-conflict-resolution\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_DDNS_USE_CONFLICT_RESOLUTION(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ddns-use-conflict-resolution", driver.loc_);
    }
}

\"ddns-conflict-resolution-mode\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_DDNS_CONFLICT_RESOLUTION_MODE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ddns-conflict-resolution-mode", driver.loc_);
    }
}

(?i:\"check-with-dhcid\") {
    if (driver.ctx_ == isc::dhcp::Parser4Context::DDNS_CONFLICT_RESOLUTION_MODE) {
        return isc::dhcp::Dhcp4Parser::make_CHECK_WITH_DHCID(driver.loc_);
    }
    std::string tmp(yytext+1);
    tmp.resize(tmp.size() - 1);
    return isc::dhcp::Dhcp4Parser::make_STRING(tmp, driver.loc_);
}

(?i:\"no-check-with-dhcid\") {
    if (driver.ctx_ == isc::dhcp::Parser4Context::DDNS_CONFLICT_RESOLUTION_MODE) {
        return isc::dhcp::Dhcp4Parser::make_NO_CHECK_WITH_DHCID(driver.loc_);
    }
    std::string tmp(yytext+1);
    tmp.resize(tmp.size() - 1);
    return isc::dhcp::Dhcp4Parser::make_STRING(tmp, driver.loc_);
}

(?i:\"check-exists-with-dhcid\") {
    if (driver.ctx_ == isc::dhcp::Parser4Context::DDNS_CONFLICT_RESOLUTION_MODE) {
        return isc::dhcp::Dhcp4Parser::make_CHECK_EXISTS_WITH_DHCID(driver.loc_);
    }
    std::string tmp(yytext+1);
    tmp.resize(tmp.size() - 1);
    return isc::dhcp::Dhcp4Parser::make_STRING(tmp, driver.loc_);
}

(?i:\"no-check-without-dhcid\") {
    if (driver.ctx_ == isc::dhcp::Parser4Context::DDNS_CONFLICT_RESOLUTION_MODE) {
        return isc::dhcp::Dhcp4Parser::make_NO_CHECK_WITHOUT_DHCID(driver.loc_);
    }
    std::string tmp(yytext+1);
    tmp.resize(tmp.size() - 1);
    return isc::dhcp::Dhcp4Parser::make_STRING(tmp, driver.loc_);
}

\"ddns-ttl-percent\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_DDNS_TTL_PERCENT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ddns-ttl-percent", driver.loc_);
    }
}

\"ddns-ttl\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_DDNS_TTL(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ddns-ttl", driver.loc_);
    }
}

\"ddns-ttl-min\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_DDNS_TTL_MIN(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ddns-ttl-min", driver.loc_);
    }
}

\"ddns-ttl-max\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_DDNS_TTL_MAX(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ddns-ttl-max", driver.loc_);
    }
}

\"subnet4\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_SUBNET4(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("subnet4", driver.loc_);
    }
}

\"store-extended-info\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_STORE_EXTENDED_INFO(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("store-extended-info", driver.loc_);
    }
}

\"shared-networks\" {
    switch (driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_SHARED_NETWORKS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("shared-networks", driver.loc_);
    }
}

\"option-def\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
        return isc::dhcp::Dhcp4Parser::make_OPTION_DEF(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("option-def", driver.loc_);
    }
}

\"option-data\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::POOLS:
    case isc::dhcp::Parser4Context::RESERVATIONS:
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
        return isc::dhcp::Dhcp4Parser::make_OPTION_DATA(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("option-data", driver.loc_);
    }
}

\"name\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LEASE_DATABASE:
    case isc::dhcp::Parser4Context::HOSTS_DATABASE:
    case isc::dhcp::Parser4Context::CONFIG_DATABASE:
    case isc::dhcp::Parser4Context::OPTION_DEF:
    case isc::dhcp::Parser4Context::OPTION_DATA:
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::HTTP_HEADERS:
    case isc::dhcp::Parser4Context::LOGGERS:
        return isc::dhcp::Dhcp4Parser::make_NAME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("name", driver.loc_);
    }
}

\"data\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::OPTION_DATA:
        return isc::dhcp::Dhcp4Parser::make_DATA(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("data", driver.loc_);
    }
}

\"always-send\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::OPTION_DATA:
        return isc::dhcp::Dhcp4Parser::make_ALWAYS_SEND(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("always-send", driver.loc_);
    }
}

\"never-send\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::OPTION_DATA:
        return isc::dhcp::Dhcp4Parser::make_NEVER_SEND(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("never-send", driver.loc_);
    }
}

\"pools\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SUBNET4:
        return isc::dhcp::Dhcp4Parser::make_POOLS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("pools", driver.loc_);
    }
}

\"pool\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_POOL(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("pool", driver.loc_);
    }
}

\"pool-id\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_POOL_ID(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("pool-id", driver.loc_);
    }
}

\"user-context\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::INTERFACES_CONFIG:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::POOLS:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::OPTION_DEF:
    case isc::dhcp::Parser4Context::OPTION_DATA:
    case isc::dhcp::Parser4Context::RESERVATIONS:
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
    case isc::dhcp::Parser4Context::CONTROL_SOCKET:
    case isc::dhcp::Parser4Context::AUTHENTICATION:
    case isc::dhcp::Parser4Context::CLIENTS:
    case isc::dhcp::Parser4Context::HTTP_HEADERS:
    case isc::dhcp::Parser4Context::DHCP_QUEUE_CONTROL:
    case isc::dhcp::Parser4Context::DHCP_MULTI_THREADING:
    case isc::dhcp::Parser4Context::LOGGERS:
    case isc::dhcp::Parser4Context::DHCP_DDNS:
        return isc::dhcp::Dhcp4Parser::make_USER_CONTEXT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("user-context", driver.loc_);
    }
}

\"comment\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::INTERFACES_CONFIG:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::POOLS:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::OPTION_DEF:
    case isc::dhcp::Parser4Context::OPTION_DATA:
    case isc::dhcp::Parser4Context::RESERVATIONS:
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
    case isc::dhcp::Parser4Context::CONTROL_SOCKET:
    case isc::dhcp::Parser4Context::AUTHENTICATION:
    case isc::dhcp::Parser4Context::CLIENTS:
    case isc::dhcp::Parser4Context::HTTP_HEADERS:
    case isc::dhcp::Parser4Context::DHCP_QUEUE_CONTROL:
    case isc::dhcp::Parser4Context::DHCP_MULTI_THREADING:
    case isc::dhcp::Parser4Context::LOGGERS:
    case isc::dhcp::Parser4Context::DHCP_DDNS:
        return isc::dhcp::Dhcp4Parser::make_COMMENT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("comment", driver.loc_);
    }
}

\"subnet\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SUBNET4:
        return isc::dhcp::Dhcp4Parser::make_SUBNET(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("subnet", driver.loc_);
    }
}

\"interface\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_INTERFACE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("interface", driver.loc_);
    }
}

\"id\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SUBNET4:
        return isc::dhcp::Dhcp4Parser::make_ID(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("id", driver.loc_);
    }
}

\"reservations-global\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_RESERVATIONS_GLOBAL(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("reservations-global", driver.loc_);
    }
}

\"reservations-in-subnet\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_RESERVATIONS_IN_SUBNET(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("reservations-in-subnet", driver.loc_);
    }
}

\"reservations-out-of-pool\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_RESERVATIONS_OUT_OF_POOL(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("reservations-out-of-pool", driver.loc_);
    }
}

\"code\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::OPTION_DEF:
    case isc::dhcp::Parser4Context::OPTION_DATA:
        return isc::dhcp::Dhcp4Parser::make_CODE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("code", driver.loc_);
    }
}

\"host-reservation-identifiers\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_HOST_RESERVATION_IDENTIFIERS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("host-reservation-identifiers", driver.loc_);
    }
}

\"calculate-tee-times\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_CALCULATE_TEE_TIMES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("calculate-tee-times", driver.loc_);
    }
}

\"t1-percent\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_T1_PERCENT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("t1-percent", driver.loc_);
    }
}

\"t2-percent\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_T2_PERCENT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("t2-percent", driver.loc_);
    }
}

\"cache-threshold\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_CACHE_THRESHOLD(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("cache-threshold", driver.loc_);
    }
}

\"cache-max-age\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_CACHE_MAX_AGE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("cache-max-age", driver.loc_);
    }
}

\"loggers\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_LOGGERS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("loggers", driver.loc_);
    }
}

\"output_options\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LOGGERS:
        return isc::dhcp::Dhcp4Parser::make_OUTPUT_OPTIONS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("output_options", driver.loc_);
    }
}

\"output-options\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LOGGERS:
        return isc::dhcp::Dhcp4Parser::make_OUTPUT_OPTIONS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("output-options", driver.loc_);
    }
}

\"output\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::OUTPUT_OPTIONS:
        return isc::dhcp::Dhcp4Parser::make_OUTPUT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("output", driver.loc_);
    }
}

\"debuglevel\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LOGGERS:
        return isc::dhcp::Dhcp4Parser::make_DEBUGLEVEL(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("debuglevel", driver.loc_);
    }
}

\"flush\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::OUTPUT_OPTIONS:
        return isc::dhcp::Dhcp4Parser::make_FLUSH(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("flush", driver.loc_);
    }
}

\"maxsize\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::OUTPUT_OPTIONS:
        return isc::dhcp::Dhcp4Parser::make_MAXSIZE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("maxsize", driver.loc_);
    }
}

\"maxver\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::OUTPUT_OPTIONS:
        return isc::dhcp::Dhcp4Parser::make_MAXVER(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("maxver", driver.loc_);
    }
}

\"pattern\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::OUTPUT_OPTIONS:
        return isc::dhcp::Dhcp4Parser::make_PATTERN(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("pattern", driver.loc_);
    }
}

\"severity\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::LOGGERS:
        return isc::dhcp::Dhcp4Parser::make_SEVERITY(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("severity", driver.loc_);
    }
}

\"client-classes\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::RESERVATIONS:
    case isc::dhcp::Parser4Context::OPTION_DATA:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::POOLS:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_CLIENT_CLASSES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("client-classes", driver.loc_);
    }
}

\"require-client-classes\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::POOLS:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_REQUIRE_CLIENT_CLASSES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("require-client-classes", driver.loc_);
    }
}

\"evaluate-additional-classes\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::POOLS:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_EVALUATE_ADDITIONAL_CLASSES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("evaluate-additional-classes", driver.loc_);
    }
}

\"client-class\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::POOLS:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
        return isc::dhcp::Dhcp4Parser::make_CLIENT_CLASS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("client-class", driver.loc_);
    }
}

\"test\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
        return isc::dhcp::Dhcp4Parser::make_TEST(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("test", driver.loc_);
    }
}

\"template-test\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
        return isc::dhcp::Dhcp4Parser::make_TEMPLATE_TEST(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("template-test", driver.loc_);
    }
}

\"only-if-required\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
        return isc::dhcp::Dhcp4Parser::make_ONLY_IF_REQUIRED(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("only-if-required", driver.loc_);
    }
}

\"only-in-additional-list\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
        return isc::dhcp::Dhcp4Parser::make_ONLY_IN_ADDITIONAL_LIST(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("only-in-additional-list", driver.loc_);
    }
}

\"reservations\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
        return isc::dhcp::Dhcp4Parser::make_RESERVATIONS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("reservations", driver.loc_);
    }
}

\"duid\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::HOST_RESERVATION_IDENTIFIERS:
    case isc::dhcp::Parser4Context::RESERVATIONS:
        return isc::dhcp::Dhcp4Parser::make_DUID(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("duid", driver.loc_);
    }
}

\"hw-address\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::HOST_RESERVATION_IDENTIFIERS:
    case isc::dhcp::Parser4Context::RESERVATIONS:
        return isc::dhcp::Dhcp4Parser::make_HW_ADDRESS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("hw-address", driver.loc_);
    }
}

\"client-id\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::HOST_RESERVATION_IDENTIFIERS:
    case isc::dhcp::Parser4Context::RESERVATIONS:
        return isc::dhcp::Dhcp4Parser::make_CLIENT_ID(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("client-id", driver.loc_);
    }
}

\"circuit-id\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::HOST_RESERVATION_IDENTIFIERS:
    case isc::dhcp::Parser4Context::RESERVATIONS:
        return isc::dhcp::Dhcp4Parser::make_CIRCUIT_ID(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("circuit-id", driver.loc_);
    }
}

\"flex-id\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::HOST_RESERVATION_IDENTIFIERS:
    case isc::dhcp::Parser4Context::RESERVATIONS:
        return isc::dhcp::Dhcp4Parser::make_FLEX_ID(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("flex-id", driver.loc_);
    }
}

\"hostname\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::RESERVATIONS:
        return isc::dhcp::Dhcp4Parser::make_HOSTNAME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("hostname", driver.loc_);
    }
}

\"space\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::OPTION_DEF:
    case isc::dhcp::Parser4Context::OPTION_DATA:
        return isc::dhcp::Dhcp4Parser::make_SPACE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("space", driver.loc_);
    }
}

\"csv-format\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::OPTION_DATA:
        return isc::dhcp::Dhcp4Parser::make_CSV_FORMAT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("csv-format", driver.loc_);
    }
}

\"record-types\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::OPTION_DEF:
        return isc::dhcp::Dhcp4Parser::make_RECORD_TYPES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("record-types", driver.loc_);
    }
}

\"encapsulate\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::OPTION_DEF:
        return isc::dhcp::Dhcp4Parser::make_ENCAPSULATE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("encapsulate", driver.loc_);
    }
}

\"array\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::OPTION_DEF:
        return isc::dhcp::Dhcp4Parser::make_ARRAY(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("array", driver.loc_);
    }
}

\"relay\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_RELAY(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("relay", driver.loc_);
    }
}

\"ip-address\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::RESERVATIONS:
    return isc::dhcp::Dhcp4Parser::make_IP_ADDRESS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ip-address", driver.loc_);
    }
}

\"ip-addresses\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::RELAY:
    return isc::dhcp::Dhcp4Parser::make_IP_ADDRESSES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ip-addresses", driver.loc_);
    }
}

\"hooks-libraries\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_HOOKS_LIBRARIES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("hooks-libraries", driver.loc_);
    }
}

\"parameters\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::HOOKS_LIBRARIES:
        return isc::dhcp::Dhcp4Parser::make_PARAMETERS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("parameters", driver.loc_);
    }
}

\"library\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::HOOKS_LIBRARIES:
        return isc::dhcp::Dhcp4Parser::make_LIBRARY(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("library", driver.loc_);
    }
}

\"expired-leases-processing\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_EXPIRED_LEASES_PROCESSING(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("expired-leases-processing", driver.loc_);
    }
}

\"reclaim-timer-wait-time\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::EXPIRED_LEASES_PROCESSING:
        return isc::dhcp::Dhcp4Parser::make_RECLAIM_TIMER_WAIT_TIME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("reclaim-timer-wait-time", driver.loc_);
    }
}

\"flush-reclaimed-timer-wait-time\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::EXPIRED_LEASES_PROCESSING:
        return isc::dhcp::Dhcp4Parser::make_FLUSH_RECLAIMED_TIMER_WAIT_TIME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("flush-reclaimed-timer-wait-time", driver.loc_);
    }
}

\"hold-reclaimed-time\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::EXPIRED_LEASES_PROCESSING:
        return isc::dhcp::Dhcp4Parser::make_HOLD_RECLAIMED_TIME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("hold-reclaimed-time", driver.loc_);
    }
}

\"max-reclaim-leases\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::EXPIRED_LEASES_PROCESSING:
        return isc::dhcp::Dhcp4Parser::make_MAX_RECLAIM_LEASES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("max-reclaim-leases", driver.loc_);
    }
}

\"max-reclaim-time\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::EXPIRED_LEASES_PROCESSING:
        return isc::dhcp::Dhcp4Parser::make_MAX_RECLAIM_TIME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("max-reclaim-time", driver.loc_);
    }
}

\"unwarned-reclaim-cycles\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::EXPIRED_LEASES_PROCESSING:
        return isc::dhcp::Dhcp4Parser::make_UNWARNED_RECLAIM_CYCLES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("unwarned-reclaim-cycles", driver.loc_);
    }
}

\"dhcp4o6-port\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_DHCP4O6_PORT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("dhcp4o6-port", driver.loc_);
    }
}

\"multi-threading\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_DHCP_MULTI_THREADING(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("multi-threading", driver.loc_);
    }
}

\"enable-multi-threading\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_MULTI_THREADING:
        return isc::dhcp::Dhcp4Parser::make_ENABLE_MULTI_THREADING(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("enable-multi-threading", driver.loc_);
    }
}

\"thread-pool-size\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_MULTI_THREADING:
        return isc::dhcp::Dhcp4Parser::make_THREAD_POOL_SIZE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("thread-pool-size", driver.loc_);
    }
}

\"packet-queue-size\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_MULTI_THREADING:
        return isc::dhcp::Dhcp4Parser::make_PACKET_QUEUE_SIZE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("packet-queue-size", driver.loc_);
    }
}

\"control-socket\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_CONTROL_SOCKET(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("control-socket", driver.loc_);
    }
}

\"control-sockets\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_CONTROL_SOCKETS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("control-sockets", driver.loc_);
    }
}

\"socket-type\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CONTROL_SOCKET:
        return isc::dhcp::Dhcp4Parser::make_SOCKET_TYPE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("socket-type", driver.loc_);
    }
}

\"unix\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CONTROL_SOCKET_TYPE:
        return isc::dhcp::Dhcp4Parser::make_UNIX(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("unix", driver.loc_);
    }
}

\"http\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CONTROL_SOCKET_TYPE:
        return isc::dhcp::Dhcp4Parser::make_HTTP(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("http", driver.loc_);
    }
}

\"https\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CONTROL_SOCKET_TYPE:
        return isc::dhcp::Dhcp4Parser::make_HTTPS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("https", driver.loc_);
    }
}

\"socket-name\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CONTROL_SOCKET:
        return isc::dhcp::Dhcp4Parser::make_SOCKET_NAME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("socket-name", driver.loc_);
    }
}

\"socket-address\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CONTROL_SOCKET:
        return isc::dhcp::Dhcp4Parser::make_SOCKET_ADDRESS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("socket-address", driver.loc_);
    }
}

\"socket-port\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CONTROL_SOCKET:
        return isc::dhcp::Dhcp4Parser::make_SOCKET_PORT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("socket-port", driver.loc_);
    }
}

\"authentication\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CONTROL_SOCKET:
        return isc::dhcp::Dhcp4Parser::make_AUTHENTICATION(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("authentication", driver.loc_);
    }
}

\"basic\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::AUTH_TYPE:
        return isc::dhcp::Dhcp4Parser::make_BASIC(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("basic", driver.loc_);
    }
}

\"realm\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::AUTHENTICATION:
        return isc::dhcp::Dhcp4Parser::make_REALM(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("realm", driver.loc_);
    }
}

\"directory\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::AUTHENTICATION:
        return isc::dhcp::Dhcp4Parser::make_DIRECTORY(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("directory", driver.loc_);
    }
}

\"clients\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::AUTHENTICATION:
        return isc::dhcp::Dhcp4Parser::make_CLIENTS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("clients", driver.loc_);
    }
}

\"user-file\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CLIENTS:
        return isc::dhcp::Dhcp4Parser::make_USER_FILE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("user-file", driver.loc_);
    }
}

\"password-file\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CLIENTS:
        return isc::dhcp::Dhcp4Parser::make_PASSWORD_FILE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("password-file", driver.loc_);
    }
}

\"cert-required\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CONTROL_SOCKET:
        return isc::dhcp::Dhcp4Parser::make_CERT_REQUIRED(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("cert-required", driver.loc_);
    }
}

\"http-headers\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::CONTROL_SOCKET:
        return isc::dhcp::Dhcp4Parser::make_HTTP_HEADERS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("http-headers", driver.loc_);
    }
}

\"value\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::HTTP_HEADERS:
        return isc::dhcp::Dhcp4Parser::make_VALUE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("value", driver.loc_);
    }
}

\"dhcp-queue-control\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_DHCP_QUEUE_CONTROL(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("dhcp-queue-control", driver.loc_);
    }
}

\"enable-queue\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_QUEUE_CONTROL:
        return isc::dhcp::Dhcp4Parser::make_ENABLE_QUEUE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("enable-queue", driver.loc_);
    }
}

\"queue-type\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_QUEUE_CONTROL:
        return isc::dhcp::Dhcp4Parser::make_QUEUE_TYPE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("queue-type", driver.loc_);
    }
}

\"capacity\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_QUEUE_CONTROL:
        return isc::dhcp::Dhcp4Parser::make_CAPACITY(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("capacity", driver.loc_);
    }
}

\"dhcp-ddns\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_DHCP_DDNS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("dhcp-ddns", driver.loc_);
    }
}

\"enable-updates\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_DDNS:
        return isc::dhcp::Dhcp4Parser::make_ENABLE_UPDATES(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("enable-updates", driver.loc_);
    }
}

\"server-ip\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_DDNS:
        return isc::dhcp::Dhcp4Parser::make_SERVER_IP(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("server-ip", driver.loc_);
    }
}

\"server-port\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_DDNS:
        return isc::dhcp::Dhcp4Parser::make_SERVER_PORT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("server-port", driver.loc_);
    }
}

\"sender-ip\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_DDNS:
        return isc::dhcp::Dhcp4Parser::make_SENDER_IP(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("sender-ip", driver.loc_);
    }
}

\"sender-port\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_DDNS:
        return isc::dhcp::Dhcp4Parser::make_SENDER_PORT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("sender-port", driver.loc_);
    }
}

\"max-queue-size\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_DDNS:
        return isc::dhcp::Dhcp4Parser::make_MAX_QUEUE_SIZE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("max-queue-size", driver.loc_);
    }
}

\"ncr-protocol\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_DDNS:
        return isc::dhcp::Dhcp4Parser::make_NCR_PROTOCOL(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ncr-protocol", driver.loc_);
    }
}

\"ncr-format\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP_DDNS:
        return isc::dhcp::Dhcp4Parser::make_NCR_FORMAT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ncr-format", driver.loc_);
    }
}

\"hostname-char-set\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_HOSTNAME_CHAR_SET(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("hostname-char-set", driver.loc_);
    }
}

\"hostname-char-replacement\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::POOLS:
        return isc::dhcp::Dhcp4Parser::make_HOSTNAME_CHAR_REPLACEMENT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("hostname-char-replacement", driver.loc_);
    }
}

(?i:\"UDP\") {
    /* dhcp-ddns value keywords are case insensitive */
    if (driver.ctx_ == isc::dhcp::Parser4Context::NCR_PROTOCOL) {
         return isc::dhcp::Dhcp4Parser::make_UDP(driver.loc_);
    }
    std::string tmp(yytext+1);
    tmp.resize(tmp.size() - 1);
    return isc::dhcp::Dhcp4Parser::make_STRING(tmp, driver.loc_);
}

(?i:\"TCP\") {
    /* dhcp-ddns value keywords are case insensitive */
    if (driver.ctx_ == isc::dhcp::Parser4Context::NCR_PROTOCOL) {
        return isc::dhcp::Dhcp4Parser::make_TCP(driver.loc_);
    }
    std::string tmp(yytext+1);
    tmp.resize(tmp.size() - 1);
    return isc::dhcp::Dhcp4Parser::make_STRING(tmp, driver.loc_);
}

(?i:\"JSON\") {
    /* dhcp-ddns value keywords are case insensitive */
    if (driver.ctx_ == isc::dhcp::Parser4Context::NCR_FORMAT) {
        return isc::dhcp::Dhcp4Parser::make_JSON(driver.loc_);
    }
    std::string tmp(yytext+1);
    tmp.resize(tmp.size() - 1);
    return isc::dhcp::Dhcp4Parser::make_STRING(tmp, driver.loc_);
}

(?i:\"when-present\") {
    /* dhcp-ddns value keywords are case insensitive */
    if (driver.ctx_ == isc::dhcp::Parser4Context::REPLACE_CLIENT_NAME) {
        return isc::dhcp::Dhcp4Parser::make_WHEN_PRESENT(driver.loc_);
    }
    std::string tmp(yytext+1);
    tmp.resize(tmp.size() - 1);
    return isc::dhcp::Dhcp4Parser::make_STRING(tmp, driver.loc_);
}

(?i:\"true\") {
    /* dhcp-ddns value keywords are case insensitive */
    if (driver.ctx_ == isc::dhcp::Parser4Context::REPLACE_CLIENT_NAME) {
        return isc::dhcp::Dhcp4Parser::make_WHEN_PRESENT(driver.loc_);
    }
    std::string tmp(yytext+1);
    tmp.resize(tmp.size() - 1);
    return isc::dhcp::Dhcp4Parser::make_STRING(tmp, driver.loc_);
}

(?i:\"never\") {
    /* dhcp-ddns value keywords are case insensitive */
    if (driver.ctx_ == isc::dhcp::Parser4Context::REPLACE_CLIENT_NAME) {
        return isc::dhcp::Dhcp4Parser::make_NEVER(driver.loc_);
    }
    std::string tmp(yytext+1);
    tmp.resize(tmp.size() - 1);
    return isc::dhcp::Dhcp4Parser::make_STRING(tmp, driver.loc_);
}

(?i:\"false\") {
    /* dhcp-ddns value keywords are case insensitive */
    if (driver.ctx_ == isc::dhcp::Parser4Context::REPLACE_CLIENT_NAME) {
        return isc::dhcp::Dhcp4Parser::make_NEVER(driver.loc_);
    }
    std::string tmp(yytext+1);
    tmp.resize(tmp.size() - 1);
    return isc::dhcp::Dhcp4Parser::make_STRING(tmp, driver.loc_);
}

(?i:\"always\") {
    /* dhcp-ddns value keywords are case insensitive */
    if (driver.ctx_ == isc::dhcp::Parser4Context::REPLACE_CLIENT_NAME) {
        return isc::dhcp::Dhcp4Parser::make_ALWAYS(driver.loc_);
    }
    std::string tmp(yytext+1);
    tmp.resize(tmp.size() - 1);
    return isc::dhcp::Dhcp4Parser::make_STRING(tmp, driver.loc_);
}

(?i:\"when-not-present\") {
    /* dhcp-ddns value keywords are case insensitive */
    if (driver.ctx_ == isc::dhcp::Parser4Context::REPLACE_CLIENT_NAME) {
        return isc::dhcp::Dhcp4Parser::make_WHEN_NOT_PRESENT(driver.loc_);
    }
    std::string tmp(yytext+1);
    tmp.resize(tmp.size() - 1);
    return isc::dhcp::Dhcp4Parser::make_STRING(tmp, driver.loc_);
}

\"4o6-interface\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SUBNET4:
        return isc::dhcp::Dhcp4Parser::make_SUBNET_4O6_INTERFACE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("4o6-interface", driver.loc_);
    }
}

\"4o6-interface-id\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SUBNET4:
        return isc::dhcp::Dhcp4Parser::make_SUBNET_4O6_INTERFACE_ID(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("4o6-interface-id", driver.loc_);
    }
}

\"4o6-subnet\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::SUBNET4:
        return isc::dhcp::Dhcp4Parser::make_SUBNET_4O6_SUBNET(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("4o6-subnet", driver.loc_);
    }
}

\"echo-client-id\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_ECHO_CLIENT_ID(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("echo-client-id", driver.loc_);
    }
}

\"match-client-id\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_MATCH_CLIENT_ID(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("match-client-id", driver.loc_);
    }
}

\"authoritative\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
        return isc::dhcp::Dhcp4Parser::make_AUTHORITATIVE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("authoritative", driver.loc_);
    }
}

\"next-server\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::RESERVATIONS:
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
        return isc::dhcp::Dhcp4Parser::make_NEXT_SERVER(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("next-server", driver.loc_);
    }
}

\"server-hostname\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::RESERVATIONS:
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
        return isc::dhcp::Dhcp4Parser::make_SERVER_HOSTNAME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("server-hostname", driver.loc_);
    }
}

\"boot-file-name\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::RESERVATIONS:
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
        return isc::dhcp::Dhcp4Parser::make_BOOT_FILE_NAME(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("boot-file-name", driver.loc_);
    }
}

\"early-global-reservations-lookup\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_EARLY_GLOBAL_RESERVATIONS_LOOKUP(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("early-global-reservations-lookup", driver.loc_);
    }
}

\"ip-reservations-unique\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_IP_RESERVATIONS_UNIQUE(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ip-reservations-unique", driver.loc_);
    }
}

\"reservations-lookup-first\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_RESERVATIONS_LOOKUP_FIRST(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("reservations-lookup-first", driver.loc_);
    }
}

\"compatibility\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_COMPATIBILITY(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("compatibility", driver.loc_);
    }
}

\"lenient-option-parsing\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::COMPATIBILITY:
        return isc::dhcp::Dhcp4Parser::make_LENIENT_OPTION_PARSING(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("lenient-option-parsing", driver.loc_);
    }
}

\"ignore-dhcp-server-identifier\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::COMPATIBILITY:
        return isc::dhcp::Dhcp4Parser::make_IGNORE_DHCP_SERVER_ID(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ignore-dhcp-server-identifier", driver.loc_);
    }
}

\"ignore-rai-link-selection\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::COMPATIBILITY:
        return isc::dhcp::Dhcp4Parser::make_IGNORE_RAI_LINK_SEL(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("ignore-rai-link-selection", driver.loc_);
    }
}

\"exclude-first-last-24\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::COMPATIBILITY:
        return isc::dhcp::Dhcp4Parser::make_EXCLUDE_FIRST_LAST_24(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("exclude-first-last-24", driver.loc_);
    }
}

\"offer-lifetime\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
    case isc::dhcp::Parser4Context::SUBNET4:
    case isc::dhcp::Parser4Context::SHARED_NETWORK:
    case isc::dhcp::Parser4Context::CLIENT_CLASSES:
        return isc::dhcp::Dhcp4Parser::make_OFFER_LFT(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("offer-lifetime", driver.loc_);
    }
}

\"stash-agent-options\" {
    switch(driver.ctx_) {
    case isc::dhcp::Parser4Context::DHCP4:
        return isc::dhcp::Dhcp4Parser::make_STASH_AGENT_OPTIONS(driver.loc_);
    default:
        return isc::dhcp::Dhcp4Parser::make_STRING("stash-agent-options", driver.loc_);
    }
}

{JSONString} {
    /* A string has been matched. It contains the actual string and single quotes.
       We need to get those quotes out of the way and just use its content, e.g.
       for 'foo' we should get foo */
    std::string raw(yytext+1);
    size_t len = raw.size() - 1;
    raw.resize(len);
    std::string decoded;
    decoded.reserve(len);
    for (size_t pos = 0; pos < len; ++pos) {
        int b = 0;
        char c = raw[pos];
        switch (c) {
        case '"':
            /* impossible condition */
            driver.error(driver.loc_, "Bad quote in \"" + raw + "\"");
            break;
        case '\\':
            ++pos;
            if (pos >= len) {
                /* impossible condition */
                driver.error(driver.loc_, "Overflow escape in \"" + raw + "\"");
            }
            c = raw[pos];
            switch (c) {
            case '"':
            case '\\':
            case '/':
                decoded.push_back(c);
                break;
            case 'b':
                decoded.push_back('\b');
                break;
            case 'f':
                decoded.push_back('\f');
                break;
            case 'n':
                decoded.push_back('\n');
                break;
            case 'r':
                decoded.push_back('\r');
                break;
            case 't':
                decoded.push_back('\t');
                break;
            case 'u':
                /* support only \u0000 to \u00ff */
                ++pos;
                if (pos + 4 > len) {
                    /* impossible condition */
                    driver.error(driver.loc_,
                                 "Overflow unicode escape in \"" + raw + "\"");
                }
                if ((raw[pos] != '0') || (raw[pos + 1] != '0')) {
                    driver.error(driver.loc_,
                    "Unsupported unicode escape in \"" + raw + "\"",
                    pos + 1);
                }
                pos += 2;
                c = raw[pos];
                if ((c >= '0') && (c <= '9')) {
                    b = (c - '0') << 4;
                } else if ((c >= 'A') && (c <= 'F')) {
                    b = (c - 'A' + 10) << 4;
                } else if ((c >= 'a') && (c <= 'f')) {
                    b = (c - 'a' + 10) << 4;
                } else {
                    /* impossible condition */
                    driver.error(driver.loc_, "Not hexadecimal in unicode escape in \"" + raw + "\"");
                }
                pos++;
                c = raw[pos];
                if ((c >= '0') && (c <= '9')) {
                    b |= c - '0';
                } else if ((c >= 'A') && (c <= 'F')) {
                    b |= c - 'A' + 10;
                } else if ((c >= 'a') && (c <= 'f')) {
                    b |= c - 'a' + 10;
                } else {
                    /* impossible condition */
                    driver.error(driver.loc_, "Not hexadecimal in unicode escape in \"" + raw + "\"");
                }
                decoded.push_back(static_cast<char>(b & 0xff));
                break;
            default:
                /* impossible condition */
                driver.error(driver.loc_, "Bad escape in \"" + raw + "\"");
            }
            break;
        default:
            if ((c >= 0) && (c < 0x20)) {
                /* impossible condition */
                driver.error(driver.loc_, "Invalid control in \"" + raw + "\"");
            }
            decoded.push_back(c);
        }
    }

    return isc::dhcp::Dhcp4Parser::make_STRING(decoded, driver.loc_);
}

\"{JSONStringCharacter}*{ControlCharacter}{ControlCharacterFill}*\" {
    /* Bad string with a forbidden control character inside */
    std::string raw(yytext+1);
    size_t len = raw.size() - 1;
    size_t pos = 0;
    for (; pos < len; ++pos) {
        char c = raw[pos];
        if ((c >= 0) && (c < 0x20)) {
            break;
        }
    }
    driver.error(driver.loc_,
                 "Invalid control in " + std::string(yytext),
                 pos + 1);
}

\"{JSONStringCharacter}*\\{BadJSONEscapeSequence}[^"]*\" {
    /* Bad string with a bad escape inside */
    std::string raw(yytext+1);
    size_t len = raw.size() - 1;
    size_t pos = 0;
    bool found = false;
    for (; pos < len; ++pos) {
        if (found) {
            break;
        }
        char c = raw[pos];
        if (c == '\\') {
            ++pos;
            c = raw[pos];
            switch (c) {
            case '"':
            case '\\':
            case '/':
            case 'b':
            case 'f':
            case 'n':
            case 'r':
            case 't':
                break;
            case 'u':
                if ((pos + 4 > len) ||
                    !std::isxdigit(raw[pos + 1]) ||
                    !std::isxdigit(raw[pos + 2]) ||
                    !std::isxdigit(raw[pos + 3]) ||
                    !std::isxdigit(raw[pos + 4])) {
                    found = true;
                }
                break;
            default:
                found = true;
                break;
            }
        }
    }
    /* The rule stops on the first " including on \" so add ... in this case */
    std::string trailer = "";
    if (raw[len - 1] == '\\') {
        trailer = "...";
    }
    driver.error(driver.loc_,
                 "Bad escape in " + std::string(yytext) + trailer,
                 pos);
}

\"{JSONStringCharacter}*\\\" {
    /* Bad string with an open escape at the end */
    std::string raw(yytext+1);
    driver.error(driver.loc_,
                 "Overflow escape in " + std::string(yytext),
                 raw.size() + 1);
}

\"{JSONStringCharacter}*\\u[0-9A-Fa-f]{0,3}\" {
    /* Bad string with an open unicode escape at the end */
    std::string raw(yytext+1);
    size_t pos = raw.size() - 1;
    for (; pos > 0; --pos) {
        char c = raw[pos];
        if (c == 'u') {
            break;
        }
    }
    driver.error(driver.loc_,
                 "Overflow unicode escape in " + std::string(yytext),
                 pos + 1);
}

"["    { return isc::dhcp::Dhcp4Parser::make_LSQUARE_BRACKET(driver.loc_); }
"]"    { return isc::dhcp::Dhcp4Parser::make_RSQUARE_BRACKET(driver.loc_); }
"{"    { return isc::dhcp::Dhcp4Parser::make_LCURLY_BRACKET(driver.loc_); }
"}"    { return isc::dhcp::Dhcp4Parser::make_RCURLY_BRACKET(driver.loc_); }
","    { return isc::dhcp::Dhcp4Parser::make_COMMA(driver.loc_); }
":"    { return isc::dhcp::Dhcp4Parser::make_COLON(driver.loc_); }

{int} {
    /* An integer was found. */
    std::string tmp(yytext);
    int64_t integer = 0;
    try {
        /* In substring we want to use negative values (e.g. -1).
           In enterprise-id we need to use values up to 0xffffffff.
           To cover both of those use cases, we need at least
           int64_t. */
        integer = boost::lexical_cast<int64_t>(tmp);
    } catch (const boost::bad_lexical_cast &) {
        driver.error(driver.loc_, "Failed to convert " + tmp + " to an integer.");
    }

    /* The parser needs the string form as double conversion is no lossless */
    return isc::dhcp::Dhcp4Parser::make_INTEGER(integer, driver.loc_);
}

[-+]?[0-9]*\.?[0-9]*([eE][-+]?[0-9]+)? {
    /* A floating point was found. */
    std::string tmp(yytext);
    double fp = 0.0;
    try {
        fp = boost::lexical_cast<double>(tmp);
    } catch (const boost::bad_lexical_cast &) {
        driver.error(driver.loc_, "Failed to convert " + tmp + " to a floating point.");
    }

    return isc::dhcp::Dhcp4Parser::make_FLOAT(fp, driver.loc_);
}

true|false {
    string tmp(yytext);
    return isc::dhcp::Dhcp4Parser::make_BOOLEAN(tmp == "true", driver.loc_);
}

null {
   return isc::dhcp::Dhcp4Parser::make_NULL_TYPE(driver.loc_);
}

(?i:true) driver.error (driver.loc_, "JSON true reserved keyword is lower case only");

(?i:false) driver.error (driver.loc_, "JSON false reserved keyword is lower case only");

(?i:null) driver.error (driver.loc_, "JSON null reserved keyword is lower case only");

<*>.   driver.error (driver.loc_, "Invalid character: " + std::string(yytext));

<<EOF>> {
    if (driver.states_.empty()) {
        return isc::dhcp::Dhcp4Parser::make_END(driver.loc_);
    }
    driver.loc_ = driver.locs_.back();
    driver.locs_.pop_back();
    driver.file_ = driver.files_.back();
    driver.files_.pop_back();
    if (driver.sfile_) {
        fclose(driver.sfile_);
        driver.sfile_ = 0;
    }
    if (!driver.sfiles_.empty()) {
        driver.sfile_ = driver.sfiles_.back();
        driver.sfiles_.pop_back();
    }
    parser4__delete_buffer(YY_CURRENT_BUFFER);
    parser4__switch_to_buffer(driver.states_.back());
    driver.states_.pop_back();

    BEGIN(DIR_EXIT);
}

%%

using namespace isc::dhcp;

void
Parser4Context::scanStringBegin(const std::string& str, ParserType parser_type)
{
    start_token_flag = true;
    start_token_value = parser_type;

    file_ = "<string>";
    sfile_ = 0;
    loc_.initialize(&file_);
    yy_flex_debug = trace_scanning_;
    YY_BUFFER_STATE buffer;
    buffer = parser4__scan_bytes(str.c_str(), str.size());
    if (!buffer) {
        fatal("cannot scan string");
        /* fatal() throws an exception so this can't be reached */
    }
}

void
Parser4Context::scanFileBegin(FILE * f,
                              const std::string& filename,
                              ParserType parser_type)
{
    start_token_flag = true;
    start_token_value = parser_type;

    file_ = filename;
    sfile_ = f;
    loc_.initialize(&file_);
    yy_flex_debug = trace_scanning_;
    YY_BUFFER_STATE buffer;

    /* See dhcp4_lexer.cc header for available definitions */
    buffer = parser4__create_buffer(f, 65536 /*buffer size*/);
    if (!buffer) {
        fatal("cannot scan file " + filename);
    }
    parser4__switch_to_buffer(buffer);
}

void
Parser4Context::scanEnd() {
    if (sfile_)
        fclose(sfile_);
    sfile_ = 0;
    static_cast<void>(parser4_lex_destroy());
    /* Close files */
    while (!sfiles_.empty()) {
        FILE* f = sfiles_.back();
        if (f) {
            fclose(f);
        }
        sfiles_.pop_back();
    }
    /* Delete states */
    while (!states_.empty()) {
        parser4__delete_buffer(states_.back());
        states_.pop_back();
    }
}

void
Parser4Context::includeFile(const std::string& filename) {
    if (states_.size() > 10) {
        fatal("Too many nested include.");
    }

    FILE* f = fopen(filename.c_str(), "r");
    if (!f) {
        fatal("Can't open include file " + filename);
    }
    if (sfile_) {
        sfiles_.push_back(sfile_);
    }
    sfile_ = f;
    states_.push_back(YY_CURRENT_BUFFER);
    YY_BUFFER_STATE buffer;
    buffer = parser4__create_buffer(f, 65536 /*buffer size*/);
    if (!buffer) {
        fatal( "Can't scan include file " + filename);
    }
    parser4__switch_to_buffer(buffer);
    files_.push_back(file_);
    file_ = filename;
    locs_.push_back(loc_);
    loc_.initialize(&file_);

    BEGIN(INITIAL);
}

namespace {
/** To avoid unused function error */
class Dummy {
    /* cppcheck-suppress unusedPrivateFunction */
    void dummy() { yy_fatal_error("Fix me: how to disable its definition?"); }
};
}
#endif /* !__clang_analyzer__ */
