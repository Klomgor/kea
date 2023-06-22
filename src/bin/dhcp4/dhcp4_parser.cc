// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.


// Take the name prefix into account.
#define yylex   parser4_lex



#include "dhcp4_parser.h"


// Unqualified %code blocks.
#line 34 "dhcp4_parser.yy"

#include <dhcp4/parser_context.h>

// Avoid warnings with the error counter.
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#line 57 "dhcp4_parser.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if PARSER4_DEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !PARSER4_DEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !PARSER4_DEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 14 "dhcp4_parser.yy"
namespace isc { namespace dhcp {
#line 150 "dhcp4_parser.cc"

  /// Build a parser object.
  Dhcp4Parser::Dhcp4Parser (isc::dhcp::Parser4Context& ctx_yyarg)
#if PARSER4_DEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      ctx (ctx_yyarg)
  {}

  Dhcp4Parser::~Dhcp4Parser ()
  {}

  Dhcp4Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  Dhcp4Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Dhcp4Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Dhcp4Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Dhcp4Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Dhcp4Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Dhcp4Parser::symbol_kind_type
  Dhcp4Parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  Dhcp4Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Dhcp4Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_value: // value
      case symbol_kind::S_map_value: // map_value
      case symbol_kind::S_ddns_replace_client_name_value: // ddns_replace_client_name_value
      case symbol_kind::S_socket_type: // socket_type
      case symbol_kind::S_outbound_interface_value: // outbound_interface_value
      case symbol_kind::S_db_type: // db_type
      case symbol_kind::S_on_fail_mode: // on_fail_mode
      case symbol_kind::S_hr_mode: // hr_mode
      case symbol_kind::S_ncr_protocol_value: // ncr_protocol_value
        value.YY_MOVE_OR_COPY< ElementPtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BOOLEAN: // "boolean"
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // "floating point"
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // "integer"
        value.YY_MOVE_OR_COPY< int64_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // "constant string"
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Dhcp4Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_value: // value
      case symbol_kind::S_map_value: // map_value
      case symbol_kind::S_ddns_replace_client_name_value: // ddns_replace_client_name_value
      case symbol_kind::S_socket_type: // socket_type
      case symbol_kind::S_outbound_interface_value: // outbound_interface_value
      case symbol_kind::S_db_type: // db_type
      case symbol_kind::S_on_fail_mode: // on_fail_mode
      case symbol_kind::S_hr_mode: // hr_mode
      case symbol_kind::S_ncr_protocol_value: // ncr_protocol_value
        value.move< ElementPtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BOOLEAN: // "boolean"
        value.move< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // "floating point"
        value.move< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // "integer"
        value.move< int64_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // "constant string"
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  Dhcp4Parser::stack_symbol_type&
  Dhcp4Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_value: // value
      case symbol_kind::S_map_value: // map_value
      case symbol_kind::S_ddns_replace_client_name_value: // ddns_replace_client_name_value
      case symbol_kind::S_socket_type: // socket_type
      case symbol_kind::S_outbound_interface_value: // outbound_interface_value
      case symbol_kind::S_db_type: // db_type
      case symbol_kind::S_on_fail_mode: // on_fail_mode
      case symbol_kind::S_hr_mode: // hr_mode
      case symbol_kind::S_ncr_protocol_value: // ncr_protocol_value
        value.copy< ElementPtr > (that.value);
        break;

      case symbol_kind::S_BOOLEAN: // "boolean"
        value.copy< bool > (that.value);
        break;

      case symbol_kind::S_FLOAT: // "floating point"
        value.copy< double > (that.value);
        break;

      case symbol_kind::S_INTEGER: // "integer"
        value.copy< int64_t > (that.value);
        break;

      case symbol_kind::S_STRING: // "constant string"
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  Dhcp4Parser::stack_symbol_type&
  Dhcp4Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_value: // value
      case symbol_kind::S_map_value: // map_value
      case symbol_kind::S_ddns_replace_client_name_value: // ddns_replace_client_name_value
      case symbol_kind::S_socket_type: // socket_type
      case symbol_kind::S_outbound_interface_value: // outbound_interface_value
      case symbol_kind::S_db_type: // db_type
      case symbol_kind::S_on_fail_mode: // on_fail_mode
      case symbol_kind::S_hr_mode: // hr_mode
      case symbol_kind::S_ncr_protocol_value: // ncr_protocol_value
        value.move< ElementPtr > (that.value);
        break;

      case symbol_kind::S_BOOLEAN: // "boolean"
        value.move< bool > (that.value);
        break;

      case symbol_kind::S_FLOAT: // "floating point"
        value.move< double > (that.value);
        break;

      case symbol_kind::S_INTEGER: // "integer"
        value.move< int64_t > (that.value);
        break;

      case symbol_kind::S_STRING: // "constant string"
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Dhcp4Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if PARSER4_DEBUG
  template <typename Base>
  void
  Dhcp4Parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        switch (yykind)
    {
      case symbol_kind::S_STRING: // "constant string"
#line 304 "dhcp4_parser.yy"
                 { yyoutput << yysym.value.template as < std::string > (); }
#line 413 "dhcp4_parser.cc"
        break;

      case symbol_kind::S_INTEGER: // "integer"
#line 304 "dhcp4_parser.yy"
                 { yyoutput << yysym.value.template as < int64_t > (); }
#line 419 "dhcp4_parser.cc"
        break;

      case symbol_kind::S_FLOAT: // "floating point"
#line 304 "dhcp4_parser.yy"
                 { yyoutput << yysym.value.template as < double > (); }
#line 425 "dhcp4_parser.cc"
        break;

      case symbol_kind::S_BOOLEAN: // "boolean"
#line 304 "dhcp4_parser.yy"
                 { yyoutput << yysym.value.template as < bool > (); }
#line 431 "dhcp4_parser.cc"
        break;

      case symbol_kind::S_value: // value
#line 304 "dhcp4_parser.yy"
                 { yyoutput << yysym.value.template as < ElementPtr > (); }
#line 437 "dhcp4_parser.cc"
        break;

      case symbol_kind::S_map_value: // map_value
#line 304 "dhcp4_parser.yy"
                 { yyoutput << yysym.value.template as < ElementPtr > (); }
#line 443 "dhcp4_parser.cc"
        break;

      case symbol_kind::S_ddns_replace_client_name_value: // ddns_replace_client_name_value
#line 304 "dhcp4_parser.yy"
                 { yyoutput << yysym.value.template as < ElementPtr > (); }
#line 449 "dhcp4_parser.cc"
        break;

      case symbol_kind::S_socket_type: // socket_type
#line 304 "dhcp4_parser.yy"
                 { yyoutput << yysym.value.template as < ElementPtr > (); }
#line 455 "dhcp4_parser.cc"
        break;

      case symbol_kind::S_outbound_interface_value: // outbound_interface_value
#line 304 "dhcp4_parser.yy"
                 { yyoutput << yysym.value.template as < ElementPtr > (); }
#line 461 "dhcp4_parser.cc"
        break;

      case symbol_kind::S_db_type: // db_type
#line 304 "dhcp4_parser.yy"
                 { yyoutput << yysym.value.template as < ElementPtr > (); }
#line 467 "dhcp4_parser.cc"
        break;

      case symbol_kind::S_on_fail_mode: // on_fail_mode
#line 304 "dhcp4_parser.yy"
                 { yyoutput << yysym.value.template as < ElementPtr > (); }
#line 473 "dhcp4_parser.cc"
        break;

      case symbol_kind::S_hr_mode: // hr_mode
#line 304 "dhcp4_parser.yy"
                 { yyoutput << yysym.value.template as < ElementPtr > (); }
#line 479 "dhcp4_parser.cc"
        break;

      case symbol_kind::S_ncr_protocol_value: // ncr_protocol_value
#line 304 "dhcp4_parser.yy"
                 { yyoutput << yysym.value.template as < ElementPtr > (); }
#line 485 "dhcp4_parser.cc"
        break;

      default:
        break;
    }
        yyo << ')';
      }
  }
#endif

  void
  Dhcp4Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Dhcp4Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Dhcp4Parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if PARSER4_DEBUG
  std::ostream&
  Dhcp4Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Dhcp4Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Dhcp4Parser::debug_level_type
  Dhcp4Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Dhcp4Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // PARSER4_DEBUG

  Dhcp4Parser::state_type
  Dhcp4Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  Dhcp4Parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Dhcp4Parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Dhcp4Parser::operator() ()
  {
    return parse ();
  }

  int
  Dhcp4Parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (ctx));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_value: // value
      case symbol_kind::S_map_value: // map_value
      case symbol_kind::S_ddns_replace_client_name_value: // ddns_replace_client_name_value
      case symbol_kind::S_socket_type: // socket_type
      case symbol_kind::S_outbound_interface_value: // outbound_interface_value
      case symbol_kind::S_db_type: // db_type
      case symbol_kind::S_on_fail_mode: // on_fail_mode
      case symbol_kind::S_hr_mode: // hr_mode
      case symbol_kind::S_ncr_protocol_value: // ncr_protocol_value
        yylhs.value.emplace< ElementPtr > ();
        break;

      case symbol_kind::S_BOOLEAN: // "boolean"
        yylhs.value.emplace< bool > ();
        break;

      case symbol_kind::S_FLOAT: // "floating point"
        yylhs.value.emplace< double > ();
        break;

      case symbol_kind::S_INTEGER: // "integer"
        yylhs.value.emplace< int64_t > ();
        break;

      case symbol_kind::S_STRING: // "constant string"
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // $@1: %empty
#line 313 "dhcp4_parser.yy"
                     { ctx.ctx_ = ctx.NO_KEYWORD; }
#line 766 "dhcp4_parser.cc"
    break;

  case 4: // $@2: %empty
#line 314 "dhcp4_parser.yy"
                      { ctx.ctx_ = ctx.CONFIG; }
#line 772 "dhcp4_parser.cc"
    break;

  case 6: // $@3: %empty
#line 315 "dhcp4_parser.yy"
                 { ctx.ctx_ = ctx.DHCP4; }
#line 778 "dhcp4_parser.cc"
    break;

  case 8: // $@4: %empty
#line 316 "dhcp4_parser.yy"
                       { ctx.ctx_ = ctx.INTERFACES_CONFIG; }
#line 784 "dhcp4_parser.cc"
    break;

  case 10: // $@5: %empty
#line 317 "dhcp4_parser.yy"
                   { ctx.ctx_ = ctx.SUBNET4; }
#line 790 "dhcp4_parser.cc"
    break;

  case 12: // $@6: %empty
#line 318 "dhcp4_parser.yy"
                 { ctx.ctx_ = ctx.POOLS; }
#line 796 "dhcp4_parser.cc"
    break;

  case 14: // $@7: %empty
#line 319 "dhcp4_parser.yy"
                       { ctx.ctx_ = ctx.RESERVATIONS; }
#line 802 "dhcp4_parser.cc"
    break;

  case 16: // $@8: %empty
#line 320 "dhcp4_parser.yy"
                       { ctx.ctx_ = ctx.DHCP4; }
#line 808 "dhcp4_parser.cc"
    break;

  case 18: // $@9: %empty
#line 321 "dhcp4_parser.yy"
                      { ctx.ctx_ = ctx.OPTION_DEF; }
#line 814 "dhcp4_parser.cc"
    break;

  case 20: // $@10: %empty
#line 322 "dhcp4_parser.yy"
                       { ctx.ctx_ = ctx.OPTION_DATA; }
#line 820 "dhcp4_parser.cc"
    break;

  case 22: // $@11: %empty
#line 323 "dhcp4_parser.yy"
                         { ctx.ctx_ = ctx.HOOKS_LIBRARIES; }
#line 826 "dhcp4_parser.cc"
    break;

  case 24: // $@12: %empty
#line 324 "dhcp4_parser.yy"
                     { ctx.ctx_ = ctx.DHCP_DDNS; }
#line 832 "dhcp4_parser.cc"
    break;

  case 26: // $@13: %empty
#line 325 "dhcp4_parser.yy"
                          { ctx.ctx_ = ctx.CONFIG_CONTROL; }
#line 838 "dhcp4_parser.cc"
    break;

  case 28: // value: "integer"
#line 333 "dhcp4_parser.yy"
               { yylhs.value.as < ElementPtr > () = ElementPtr(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location))); }
#line 844 "dhcp4_parser.cc"
    break;

  case 29: // value: "floating point"
#line 334 "dhcp4_parser.yy"
             { yylhs.value.as < ElementPtr > () = ElementPtr(new DoubleElement(yystack_[0].value.as < double > (), ctx.loc2pos(yystack_[0].location))); }
#line 850 "dhcp4_parser.cc"
    break;

  case 30: // value: "boolean"
#line 335 "dhcp4_parser.yy"
               { yylhs.value.as < ElementPtr > () = ElementPtr(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location))); }
#line 856 "dhcp4_parser.cc"
    break;

  case 31: // value: "constant string"
#line 336 "dhcp4_parser.yy"
              { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location))); }
#line 862 "dhcp4_parser.cc"
    break;

  case 32: // value: "null"
#line 337 "dhcp4_parser.yy"
                 { yylhs.value.as < ElementPtr > () = ElementPtr(new NullElement(ctx.loc2pos(yystack_[0].location))); }
#line 868 "dhcp4_parser.cc"
    break;

  case 33: // value: map2
#line 338 "dhcp4_parser.yy"
            { yylhs.value.as < ElementPtr > () = ctx.stack_.back(); ctx.stack_.pop_back(); }
#line 874 "dhcp4_parser.cc"
    break;

  case 34: // value: list_generic
#line 339 "dhcp4_parser.yy"
                    { yylhs.value.as < ElementPtr > () = ctx.stack_.back(); ctx.stack_.pop_back(); }
#line 880 "dhcp4_parser.cc"
    break;

  case 35: // sub_json: value
#line 342 "dhcp4_parser.yy"
                {
    // Push back the JSON value on the stack
    ctx.stack_.push_back(yystack_[0].value.as < ElementPtr > ());
}
#line 889 "dhcp4_parser.cc"
    break;

  case 36: // $@14: %empty
#line 347 "dhcp4_parser.yy"
                     {
    // This code is executed when we're about to start parsing
    // the content of the map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 900 "dhcp4_parser.cc"
    break;

  case 37: // map2: "{" $@14 map_content "}"
#line 352 "dhcp4_parser.yy"
                             {
    // map parsing completed. If we ever want to do any wrap up
    // (maybe some sanity checking), this would be the best place
    // for it.
}
#line 910 "dhcp4_parser.cc"
    break;

  case 38: // map_value: map2
#line 358 "dhcp4_parser.yy"
                { yylhs.value.as < ElementPtr > () = ctx.stack_.back(); ctx.stack_.pop_back(); }
#line 916 "dhcp4_parser.cc"
    break;

  case 41: // not_empty_map: "constant string" ":" value
#line 365 "dhcp4_parser.yy"
                                  {
                  // map containing a single entry
                  ctx.unique(yystack_[2].value.as < std::string > (), ctx.loc2pos(yystack_[2].location));
                  ctx.stack_.back()->set(yystack_[2].value.as < std::string > (), yystack_[0].value.as < ElementPtr > ());
                  }
#line 926 "dhcp4_parser.cc"
    break;

  case 42: // not_empty_map: not_empty_map "," "constant string" ":" value
#line 370 "dhcp4_parser.yy"
                                                      {
                  // map consisting of a shorter map followed by
                  // comma and string:value
                  ctx.unique(yystack_[2].value.as < std::string > (), ctx.loc2pos(yystack_[2].location));
                  ctx.stack_.back()->set(yystack_[2].value.as < std::string > (), yystack_[0].value.as < ElementPtr > ());
                  }
#line 937 "dhcp4_parser.cc"
    break;

  case 43: // not_empty_map: not_empty_map ","
#line 376 "dhcp4_parser.yy"
                                   {
                 ctx.warnAboutExtraCommas(yystack_[0].location);
                 }
#line 945 "dhcp4_parser.cc"
    break;

  case 44: // $@15: %empty
#line 381 "dhcp4_parser.yy"
                              {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(l);
}
#line 954 "dhcp4_parser.cc"
    break;

  case 45: // list_generic: "[" $@15 list_content "]"
#line 384 "dhcp4_parser.yy"
                               {
    // list parsing complete. Put any sanity checking here
}
#line 962 "dhcp4_parser.cc"
    break;

  case 48: // not_empty_list: value
#line 392 "dhcp4_parser.yy"
                      {
                  // List consisting of a single element.
                  ctx.stack_.back()->add(yystack_[0].value.as < ElementPtr > ());
                  }
#line 971 "dhcp4_parser.cc"
    break;

  case 49: // not_empty_list: not_empty_list "," value
#line 396 "dhcp4_parser.yy"
                                           {
                  // List ending with , and a value.
                  ctx.stack_.back()->add(yystack_[0].value.as < ElementPtr > ());
                  }
#line 980 "dhcp4_parser.cc"
    break;

  case 50: // not_empty_list: not_empty_list ","
#line 400 "dhcp4_parser.yy"
                                     {
                  ctx.warnAboutExtraCommas(yystack_[0].location);
                  }
#line 988 "dhcp4_parser.cc"
    break;

  case 51: // $@16: %empty
#line 406 "dhcp4_parser.yy"
                              {
    // List parsing about to start
}
#line 996 "dhcp4_parser.cc"
    break;

  case 52: // list_strings: "[" $@16 list_strings_content "]"
#line 408 "dhcp4_parser.yy"
                                       {
    // list parsing complete. Put any sanity checking here
    //ctx.stack_.pop_back();
}
#line 1005 "dhcp4_parser.cc"
    break;

  case 55: // not_empty_list_strings: "constant string"
#line 417 "dhcp4_parser.yy"
                               {
                          ElementPtr s(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
                          ctx.stack_.back()->add(s);
                          }
#line 1014 "dhcp4_parser.cc"
    break;

  case 56: // not_empty_list_strings: not_empty_list_strings "," "constant string"
#line 421 "dhcp4_parser.yy"
                                                            {
                          ElementPtr s(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
                          ctx.stack_.back()->add(s);
                          }
#line 1023 "dhcp4_parser.cc"
    break;

  case 57: // not_empty_list_strings: not_empty_list_strings ","
#line 425 "dhcp4_parser.yy"
                                                     {
                          ctx.warnAboutExtraCommas(yystack_[0].location);
                          }
#line 1031 "dhcp4_parser.cc"
    break;

  case 58: // unknown_map_entry: "constant string" ":"
#line 435 "dhcp4_parser.yy"
                                {
    const std::string& where = ctx.contextName();
    const std::string& keyword = yystack_[1].value.as < std::string > ();
    error(yystack_[1].location,
          "got unexpected keyword \"" + keyword + "\" in " + where + " map.");
}
#line 1042 "dhcp4_parser.cc"
    break;

  case 59: // $@17: %empty
#line 444 "dhcp4_parser.yy"
                           {
    // This code is executed when we're about to start parsing
    // the content of the map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 1053 "dhcp4_parser.cc"
    break;

  case 60: // syntax_map: "{" $@17 global_object "}"
#line 449 "dhcp4_parser.yy"
                               {
    // map parsing completed. If we ever want to do any wrap up
    // (maybe some sanity checking), this would be the best place
    // for it.

    // Dhcp4 is required
    ctx.require("Dhcp4", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
}
#line 1066 "dhcp4_parser.cc"
    break;

  case 61: // $@18: %empty
#line 459 "dhcp4_parser.yy"
                     {
    // This code is executed when we're about to start parsing
    // the content of the map
    // Prevent against duplicate.
    ctx.unique("Dhcp4", ctx.loc2pos(yystack_[0].location));
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("Dhcp4", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.DHCP4);
}
#line 1081 "dhcp4_parser.cc"
    break;

  case 62: // global_object: "Dhcp4" $@18 ":" "{" global_params "}"
#line 468 "dhcp4_parser.yy"
                                                    {
    // No global parameter is required
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1091 "dhcp4_parser.cc"
    break;

  case 64: // global_object_comma: global_object ","
#line 476 "dhcp4_parser.yy"
                                         {
    ctx.warnAboutExtraCommas(yystack_[0].location);
}
#line 1099 "dhcp4_parser.cc"
    break;

  case 65: // $@19: %empty
#line 482 "dhcp4_parser.yy"
                          {
    // Parse the Dhcp4 map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 1109 "dhcp4_parser.cc"
    break;

  case 66: // sub_dhcp4: "{" $@19 global_params "}"
#line 486 "dhcp4_parser.yy"
                               {
    // No global parameter is required
    // parsing completed
}
#line 1118 "dhcp4_parser.cc"
    break;

  case 69: // global_params: global_params ","
#line 493 "dhcp4_parser.yy"
                                   {
                 ctx.warnAboutExtraCommas(yystack_[0].location);
                 }
#line 1126 "dhcp4_parser.cc"
    break;

  case 137: // valid_lifetime: "valid-lifetime" ":" "integer"
#line 569 "dhcp4_parser.yy"
                                             {
    ctx.unique("valid-lifetime", ctx.loc2pos(yystack_[2].location));
    ElementPtr prf(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("valid-lifetime", prf);
}
#line 1136 "dhcp4_parser.cc"
    break;

  case 138: // min_valid_lifetime: "min-valid-lifetime" ":" "integer"
#line 575 "dhcp4_parser.yy"
                                                     {
    ctx.unique("min-valid-lifetime", ctx.loc2pos(yystack_[2].location));
    ElementPtr prf(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("min-valid-lifetime", prf);
}
#line 1146 "dhcp4_parser.cc"
    break;

  case 139: // max_valid_lifetime: "max-valid-lifetime" ":" "integer"
#line 581 "dhcp4_parser.yy"
                                                     {
    ctx.unique("max-valid-lifetime", ctx.loc2pos(yystack_[2].location));
    ElementPtr prf(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("max-valid-lifetime", prf);
}
#line 1156 "dhcp4_parser.cc"
    break;

  case 140: // renew_timer: "renew-timer" ":" "integer"
#line 587 "dhcp4_parser.yy"
                                       {
    ctx.unique("renew-timer", ctx.loc2pos(yystack_[2].location));
    ElementPtr prf(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("renew-timer", prf);
}
#line 1166 "dhcp4_parser.cc"
    break;

  case 141: // rebind_timer: "rebind-timer" ":" "integer"
#line 593 "dhcp4_parser.yy"
                                         {
    ctx.unique("rebind-timer", ctx.loc2pos(yystack_[2].location));
    ElementPtr prf(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("rebind-timer", prf);
}
#line 1176 "dhcp4_parser.cc"
    break;

  case 142: // calculate_tee_times: "calculate-tee-times" ":" "boolean"
#line 599 "dhcp4_parser.yy"
                                                       {
    ctx.unique("calculate-tee-times", ctx.loc2pos(yystack_[2].location));
    ElementPtr ctt(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("calculate-tee-times", ctt);
}
#line 1186 "dhcp4_parser.cc"
    break;

  case 143: // t1_percent: "t1-percent" ":" "floating point"
#line 605 "dhcp4_parser.yy"
                                   {
    ctx.unique("t1-percent", ctx.loc2pos(yystack_[2].location));
    ElementPtr t1(new DoubleElement(yystack_[0].value.as < double > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("t1-percent", t1);
}
#line 1196 "dhcp4_parser.cc"
    break;

  case 144: // t2_percent: "t2-percent" ":" "floating point"
#line 611 "dhcp4_parser.yy"
                                   {
    ctx.unique("t2-percent", ctx.loc2pos(yystack_[2].location));
    ElementPtr t2(new DoubleElement(yystack_[0].value.as < double > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("t2-percent", t2);
}
#line 1206 "dhcp4_parser.cc"
    break;

  case 145: // cache_threshold: "cache-threshold" ":" "floating point"
#line 617 "dhcp4_parser.yy"
                                             {
    ctx.unique("cache-threshold", ctx.loc2pos(yystack_[2].location));
    ElementPtr ct(new DoubleElement(yystack_[0].value.as < double > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("cache-threshold", ct);
}
#line 1216 "dhcp4_parser.cc"
    break;

  case 146: // cache_max_age: "cache-max-age" ":" "integer"
#line 623 "dhcp4_parser.yy"
                                           {
    ctx.unique("cache-max-age", ctx.loc2pos(yystack_[2].location));
    ElementPtr cm(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("cache-max-age", cm);
}
#line 1226 "dhcp4_parser.cc"
    break;

  case 147: // decline_probation_period: "decline-probation-period" ":" "integer"
#line 629 "dhcp4_parser.yy"
                                                                 {
    ctx.unique("decline-probation-period", ctx.loc2pos(yystack_[2].location));
    ElementPtr dpp(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("decline-probation-period", dpp);
}
#line 1236 "dhcp4_parser.cc"
    break;

  case 148: // $@20: %empty
#line 635 "dhcp4_parser.yy"
                       {
    ctx.unique("server-tag", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1245 "dhcp4_parser.cc"
    break;

  case 149: // server_tag: "server-tag" $@20 ":" "constant string"
#line 638 "dhcp4_parser.yy"
               {
    ElementPtr stag(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("server-tag", stag);
    ctx.leave();
}
#line 1255 "dhcp4_parser.cc"
    break;

  case 150: // parked_packet_limit: "parked-packet-limit" ":" "integer"
#line 644 "dhcp4_parser.yy"
                                                       {
    ctx.unique("parked-packet-limit", ctx.loc2pos(yystack_[2].location));
    ElementPtr ppl(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("parked-packet-limit", ppl);
}
#line 1265 "dhcp4_parser.cc"
    break;

  case 151: // $@21: %empty
#line 650 "dhcp4_parser.yy"
                     {
    ctx.unique("allocator", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1274 "dhcp4_parser.cc"
    break;

  case 152: // allocator: "allocator" $@21 ":" "constant string"
#line 653 "dhcp4_parser.yy"
               {
    ElementPtr al(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("allocator", al);
    ctx.leave();
}
#line 1284 "dhcp4_parser.cc"
    break;

  case 153: // echo_client_id: "echo-client-id" ":" "boolean"
#line 659 "dhcp4_parser.yy"
                                             {
    ctx.unique("echo-client-id", ctx.loc2pos(yystack_[2].location));
    ElementPtr echo(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("echo-client-id", echo);
}
#line 1294 "dhcp4_parser.cc"
    break;

  case 154: // match_client_id: "match-client-id" ":" "boolean"
#line 665 "dhcp4_parser.yy"
                                               {
    ctx.unique("match-client-id", ctx.loc2pos(yystack_[2].location));
    ElementPtr match(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("match-client-id", match);
}
#line 1304 "dhcp4_parser.cc"
    break;

  case 155: // authoritative: "authoritative" ":" "boolean"
#line 671 "dhcp4_parser.yy"
                                           {
    ctx.unique("authoritative", ctx.loc2pos(yystack_[2].location));
    ElementPtr prf(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("authoritative", prf);
}
#line 1314 "dhcp4_parser.cc"
    break;

  case 156: // ddns_send_updates: "ddns-send-updates" ":" "boolean"
#line 677 "dhcp4_parser.yy"
                                                   {
    ctx.unique("ddns-send-updates", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ddns-send-updates", b);
}
#line 1324 "dhcp4_parser.cc"
    break;

  case 157: // ddns_override_no_update: "ddns-override-no-update" ":" "boolean"
#line 683 "dhcp4_parser.yy"
                                                               {
    ctx.unique("ddns-override-no-update", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ddns-override-no-update", b);
}
#line 1334 "dhcp4_parser.cc"
    break;

  case 158: // ddns_override_client_update: "ddns-override-client-update" ":" "boolean"
#line 689 "dhcp4_parser.yy"
                                                                       {
    ctx.unique("ddns-override-client-update", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ddns-override-client-update", b);
}
#line 1344 "dhcp4_parser.cc"
    break;

  case 159: // $@22: %empty
#line 695 "dhcp4_parser.yy"
                                                   {
    ctx.enter(ctx.REPLACE_CLIENT_NAME);
    ctx.unique("ddns-replace-client-name", ctx.loc2pos(yystack_[0].location));
}
#line 1353 "dhcp4_parser.cc"
    break;

  case 160: // ddns_replace_client_name: "ddns-replace-client-name" $@22 ":" ddns_replace_client_name_value
#line 698 "dhcp4_parser.yy"
                                       {
    ctx.stack_.back()->set("ddns-replace-client-name", yystack_[0].value.as < ElementPtr > ());
    ctx.leave();
}
#line 1362 "dhcp4_parser.cc"
    break;

  case 161: // ddns_replace_client_name_value: "when-present"
#line 704 "dhcp4_parser.yy"
                 {
      yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("when-present", ctx.loc2pos(yystack_[0].location)));
      }
#line 1370 "dhcp4_parser.cc"
    break;

  case 162: // ddns_replace_client_name_value: "never"
#line 707 "dhcp4_parser.yy"
          {
      yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("never", ctx.loc2pos(yystack_[0].location)));
      }
#line 1378 "dhcp4_parser.cc"
    break;

  case 163: // ddns_replace_client_name_value: "always"
#line 710 "dhcp4_parser.yy"
           {
      yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("always", ctx.loc2pos(yystack_[0].location)));
      }
#line 1386 "dhcp4_parser.cc"
    break;

  case 164: // ddns_replace_client_name_value: "when-not-present"
#line 713 "dhcp4_parser.yy"
                     {
      yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("when-not-present", ctx.loc2pos(yystack_[0].location)));
      }
#line 1394 "dhcp4_parser.cc"
    break;

  case 165: // ddns_replace_client_name_value: "boolean"
#line 716 "dhcp4_parser.yy"
            {
      error(yystack_[0].location, "boolean values for the replace-client-name are "
                "no longer supported");
      }
#line 1403 "dhcp4_parser.cc"
    break;

  case 166: // $@23: %empty
#line 722 "dhcp4_parser.yy"
                                             {
    ctx.unique("ddns-generated-prefix", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1412 "dhcp4_parser.cc"
    break;

  case 167: // ddns_generated_prefix: "ddns-generated-prefix" $@23 ":" "constant string"
#line 725 "dhcp4_parser.yy"
               {
    ElementPtr s(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ddns-generated-prefix", s);
    ctx.leave();
}
#line 1422 "dhcp4_parser.cc"
    break;

  case 168: // $@24: %empty
#line 731 "dhcp4_parser.yy"
                                               {
    ctx.unique("ddns-qualifying-suffix", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1431 "dhcp4_parser.cc"
    break;

  case 169: // ddns_qualifying_suffix: "ddns-qualifying-suffix" $@24 ":" "constant string"
#line 734 "dhcp4_parser.yy"
               {
    ElementPtr s(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ddns-qualifying-suffix", s);
    ctx.leave();
}
#line 1441 "dhcp4_parser.cc"
    break;

  case 170: // ddns_update_on_renew: "ddns-update-on-renew" ":" "boolean"
#line 740 "dhcp4_parser.yy"
                                                         {
    ctx.unique("ddns-update-on-renew", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ddns-update-on-renew", b);
}
#line 1451 "dhcp4_parser.cc"
    break;

  case 171: // ddns_use_conflict_resolution: "ddns-use-conflict-resolution" ":" "boolean"
#line 746 "dhcp4_parser.yy"
                                                                         {
    ctx.unique("ddns-use-conflict-resolution", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ddns-use-conflict-resolution", b);
}
#line 1461 "dhcp4_parser.cc"
    break;

  case 172: // ddns_ttl_percent: "ddns-ttl-percent" ":" "floating point"
#line 752 "dhcp4_parser.yy"
                                               {
    ctx.unique("ddns-ttl-percent", ctx.loc2pos(yystack_[2].location));
    ElementPtr ttl(new DoubleElement(yystack_[0].value.as < double > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ddns-ttl-percent", ttl);
}
#line 1471 "dhcp4_parser.cc"
    break;

  case 173: // $@25: %empty
#line 758 "dhcp4_parser.yy"
                                     {
    ctx.unique("hostname-char-set", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1480 "dhcp4_parser.cc"
    break;

  case 174: // hostname_char_set: "hostname-char-set" $@25 ":" "constant string"
#line 761 "dhcp4_parser.yy"
               {
    ElementPtr s(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hostname-char-set", s);
    ctx.leave();
}
#line 1490 "dhcp4_parser.cc"
    break;

  case 175: // $@26: %empty
#line 767 "dhcp4_parser.yy"
                                                     {
    ctx.unique("hostname-char-replacement", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1499 "dhcp4_parser.cc"
    break;

  case 176: // hostname_char_replacement: "hostname-char-replacement" $@26 ":" "constant string"
#line 770 "dhcp4_parser.yy"
               {
    ElementPtr s(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hostname-char-replacement", s);
    ctx.leave();
}
#line 1509 "dhcp4_parser.cc"
    break;

  case 177: // store_extended_info: "store-extended-info" ":" "boolean"
#line 776 "dhcp4_parser.yy"
                                                       {
    ctx.unique("store-extended-info", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("store-extended-info", b);
}
#line 1519 "dhcp4_parser.cc"
    break;

  case 178: // statistic_default_sample_count: "statistic-default-sample-count" ":" "integer"
#line 782 "dhcp4_parser.yy"
                                                                             {
    ctx.unique("statistic-default-sample-count", ctx.loc2pos(yystack_[2].location));
    ElementPtr count(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("statistic-default-sample-count", count);
}
#line 1529 "dhcp4_parser.cc"
    break;

  case 179: // statistic_default_sample_age: "statistic-default-sample-age" ":" "integer"
#line 788 "dhcp4_parser.yy"
                                                                         {
    ctx.unique("statistic-default-sample-age", ctx.loc2pos(yystack_[2].location));
    ElementPtr age(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("statistic-default-sample-age", age);
}
#line 1539 "dhcp4_parser.cc"
    break;

  case 180: // early_global_reservations_lookup: "early-global-reservations-lookup" ":" "boolean"
#line 794 "dhcp4_parser.yy"
                                                                                 {
    ctx.unique("early-global-reservations-lookup", ctx.loc2pos(yystack_[2].location));
    ElementPtr early(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("early-global-reservations-lookup", early);
}
#line 1549 "dhcp4_parser.cc"
    break;

  case 181: // ip_reservations_unique: "ip-reservations-unique" ":" "boolean"
#line 800 "dhcp4_parser.yy"
                                                             {
    ctx.unique("ip-reservations-unique", ctx.loc2pos(yystack_[2].location));
    ElementPtr unique(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ip-reservations-unique", unique);
}
#line 1559 "dhcp4_parser.cc"
    break;

  case 182: // reservations_lookup_first: "reservations-lookup-first" ":" "boolean"
#line 806 "dhcp4_parser.yy"
                                                                   {
    ctx.unique("reservations-lookup-first", ctx.loc2pos(yystack_[2].location));
    ElementPtr first(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("reservations-lookup-first", first);
}
#line 1569 "dhcp4_parser.cc"
    break;

  case 183: // offer_lifetime: "offer-lifetime" ":" "integer"
#line 812 "dhcp4_parser.yy"
                                        {
    ctx.unique("offer-lifetime", ctx.loc2pos(yystack_[2].location));
    ElementPtr offer_lifetime(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("offer-lifetime", offer_lifetime);
}
#line 1579 "dhcp4_parser.cc"
    break;

  case 184: // $@27: %empty
#line 818 "dhcp4_parser.yy"
                                     {
    ctx.unique("interfaces-config", ctx.loc2pos(yystack_[0].location));
    ElementPtr i(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("interfaces-config", i);
    ctx.stack_.push_back(i);
    ctx.enter(ctx.INTERFACES_CONFIG);
}
#line 1591 "dhcp4_parser.cc"
    break;

  case 185: // interfaces_config: "interfaces-config" $@27 ":" "{" interfaces_config_params "}"
#line 824 "dhcp4_parser.yy"
                                                               {
    // No interfaces config param is required
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1601 "dhcp4_parser.cc"
    break;

  case 188: // interfaces_config_params: interfaces_config_params ","
#line 832 "dhcp4_parser.yy"
                                                         {
                            ctx.warnAboutExtraCommas(yystack_[0].location);
                            }
#line 1609 "dhcp4_parser.cc"
    break;

  case 199: // $@28: %empty
#line 849 "dhcp4_parser.yy"
                                {
    // Parse the interfaces-config map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 1619 "dhcp4_parser.cc"
    break;

  case 200: // sub_interfaces4: "{" $@28 interfaces_config_params "}"
#line 853 "dhcp4_parser.yy"
                                          {
    // No interfaces config param is required
    // parsing completed
}
#line 1628 "dhcp4_parser.cc"
    break;

  case 201: // $@29: %empty
#line 858 "dhcp4_parser.yy"
                            {
    ctx.unique("interfaces", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("interfaces", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1640 "dhcp4_parser.cc"
    break;

  case 202: // interfaces_list: "interfaces" $@29 ":" list_strings
#line 864 "dhcp4_parser.yy"
                     {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1649 "dhcp4_parser.cc"
    break;

  case 203: // $@30: %empty
#line 869 "dhcp4_parser.yy"
                                   {
    ctx.unique("dhcp-socket-type", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.DHCP_SOCKET_TYPE);
}
#line 1658 "dhcp4_parser.cc"
    break;

  case 204: // dhcp_socket_type: "dhcp-socket-type" $@30 ":" socket_type
#line 872 "dhcp4_parser.yy"
                    {
    ctx.stack_.back()->set("dhcp-socket-type", yystack_[0].value.as < ElementPtr > ());
    ctx.leave();
}
#line 1667 "dhcp4_parser.cc"
    break;

  case 205: // socket_type: "raw"
#line 877 "dhcp4_parser.yy"
                 { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("raw", ctx.loc2pos(yystack_[0].location))); }
#line 1673 "dhcp4_parser.cc"
    break;

  case 206: // socket_type: "udp"
#line 878 "dhcp4_parser.yy"
                 { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("udp", ctx.loc2pos(yystack_[0].location))); }
#line 1679 "dhcp4_parser.cc"
    break;

  case 207: // $@31: %empty
#line 881 "dhcp4_parser.yy"
                                       {
    ctx.unique("outbound-interface", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.OUTBOUND_INTERFACE);
}
#line 1688 "dhcp4_parser.cc"
    break;

  case 208: // outbound_interface: "outbound-interface" $@31 ":" outbound_interface_value
#line 884 "dhcp4_parser.yy"
                                 {
    ctx.stack_.back()->set("outbound-interface", yystack_[0].value.as < ElementPtr > ());
    ctx.leave();
}
#line 1697 "dhcp4_parser.cc"
    break;

  case 209: // outbound_interface_value: "same-as-inbound"
#line 889 "dhcp4_parser.yy"
                                          {
    yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("same-as-inbound", ctx.loc2pos(yystack_[0].location)));
}
#line 1705 "dhcp4_parser.cc"
    break;

  case 210: // outbound_interface_value: "use-routing"
#line 891 "dhcp4_parser.yy"
                {
    yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("use-routing", ctx.loc2pos(yystack_[0].location)));
    }
#line 1713 "dhcp4_parser.cc"
    break;

  case 211: // re_detect: "re-detect" ":" "boolean"
#line 895 "dhcp4_parser.yy"
                                   {
    ctx.unique("re-detect", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("re-detect", b);
}
#line 1723 "dhcp4_parser.cc"
    break;

  case 212: // service_sockets_require_all: "service-sockets-require-all" ":" "boolean"
#line 901 "dhcp4_parser.yy"
                                                                       {
    ctx.unique("service-sockets-require-all", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("service-sockets-require-all", b);
}
#line 1733 "dhcp4_parser.cc"
    break;

  case 213: // service_sockets_retry_wait_time: "service-sockets-retry-wait-time" ":" "integer"
#line 907 "dhcp4_parser.yy"
                                                                               {
    ctx.unique("service-sockets-retry-wait-time", ctx.loc2pos(yystack_[2].location));
    ElementPtr n(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("service-sockets-retry-wait-time", n);
}
#line 1743 "dhcp4_parser.cc"
    break;

  case 214: // service_sockets_max_retries: "service-sockets-max-retries" ":" "integer"
#line 913 "dhcp4_parser.yy"
                                                                       {
    ctx.unique("service-sockets-max-retries", ctx.loc2pos(yystack_[2].location));
    ElementPtr n(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("service-sockets-max-retries", n);
}
#line 1753 "dhcp4_parser.cc"
    break;

  case 215: // $@32: %empty
#line 919 "dhcp4_parser.yy"
                               {
    ctx.unique("lease-database", ctx.loc2pos(yystack_[0].location));
    ElementPtr i(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("lease-database", i);
    ctx.stack_.push_back(i);
    ctx.enter(ctx.LEASE_DATABASE);
}
#line 1765 "dhcp4_parser.cc"
    break;

  case 216: // lease_database: "lease-database" $@32 ":" "{" database_map_params "}"
#line 925 "dhcp4_parser.yy"
                                                          {
    // The type parameter is required
    ctx.require("type", ctx.loc2pos(yystack_[2].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1776 "dhcp4_parser.cc"
    break;

  case 217: // $@33: %empty
#line 932 "dhcp4_parser.yy"
                             {
    ctx.unique("sanity-checks", ctx.loc2pos(yystack_[0].location));
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("sanity-checks", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.SANITY_CHECKS);
}
#line 1788 "dhcp4_parser.cc"
    break;

  case 218: // sanity_checks: "sanity-checks" $@33 ":" "{" sanity_checks_params "}"
#line 938 "dhcp4_parser.yy"
                                                           {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1797 "dhcp4_parser.cc"
    break;

  case 221: // sanity_checks_params: sanity_checks_params ","
#line 945 "dhcp4_parser.yy"
                                                 {
                        ctx.warnAboutExtraCommas(yystack_[0].location);
                        }
#line 1805 "dhcp4_parser.cc"
    break;

  case 224: // $@34: %empty
#line 954 "dhcp4_parser.yy"
                           {
    ctx.unique("lease-checks", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1814 "dhcp4_parser.cc"
    break;

  case 225: // lease_checks: "lease-checks" $@34 ":" "constant string"
#line 957 "dhcp4_parser.yy"
               {

    if ( (string(yystack_[0].value.as < std::string > ()) == "none") ||
         (string(yystack_[0].value.as < std::string > ()) == "warn") ||
         (string(yystack_[0].value.as < std::string > ()) == "fix") ||
         (string(yystack_[0].value.as < std::string > ()) == "fix-del") ||
         (string(yystack_[0].value.as < std::string > ()) == "del")) {
        ElementPtr user(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
        ctx.stack_.back()->set("lease-checks", user);
        ctx.leave();
    } else {
        error(yystack_[0].location, "Unsupported 'lease-checks value: " + string(yystack_[0].value.as < std::string > ()) +
              ", supported values are: none, warn, fix, fix-del, del");
    }
}
#line 1834 "dhcp4_parser.cc"
    break;

  case 226: // $@35: %empty
#line 973 "dhcp4_parser.yy"
                                           {
    ctx.unique("extended-info-checks", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1843 "dhcp4_parser.cc"
    break;

  case 227: // extended_info_checks: "extended-info-checks" $@35 ":" "constant string"
#line 976 "dhcp4_parser.yy"
               {

    if ( (string(yystack_[0].value.as < std::string > ()) == "none") ||
         (string(yystack_[0].value.as < std::string > ()) == "fix") ||
         (string(yystack_[0].value.as < std::string > ()) == "strict") ||
         (string(yystack_[0].value.as < std::string > ()) == "pedantic")) {
        ElementPtr user(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
        ctx.stack_.back()->set("extended-info-checks", user);
        ctx.leave();
    } else {
        error(yystack_[0].location, "Unsupported 'extended-info-checks value: " + string(yystack_[0].value.as < std::string > ()) +
              ", supported values are: none, fix, strict, pedantic");
    }
}
#line 1862 "dhcp4_parser.cc"
    break;

  case 228: // $@36: %empty
#line 991 "dhcp4_parser.yy"
                               {
    ctx.unique("hosts-database", ctx.loc2pos(yystack_[0].location));
    ElementPtr i(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hosts-database", i);
    ctx.stack_.push_back(i);
    ctx.enter(ctx.HOSTS_DATABASE);
}
#line 1874 "dhcp4_parser.cc"
    break;

  case 229: // hosts_database: "hosts-database" $@36 ":" "{" database_map_params "}"
#line 997 "dhcp4_parser.yy"
                                                          {
    // The type parameter is required
    ctx.require("type", ctx.loc2pos(yystack_[2].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1885 "dhcp4_parser.cc"
    break;

  case 230: // $@37: %empty
#line 1004 "dhcp4_parser.yy"
                                 {
    ctx.unique("hosts-databases", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hosts-databases", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.HOSTS_DATABASE);
}
#line 1897 "dhcp4_parser.cc"
    break;

  case 231: // hosts_databases: "hosts-databases" $@37 ":" "[" database_list "]"
#line 1010 "dhcp4_parser.yy"
                                                      {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1906 "dhcp4_parser.cc"
    break;

  case 236: // not_empty_database_list: not_empty_database_list ","
#line 1021 "dhcp4_parser.yy"
                                                       {
                           ctx.warnAboutExtraCommas(yystack_[0].location);
                           }
#line 1914 "dhcp4_parser.cc"
    break;

  case 237: // $@38: %empty
#line 1026 "dhcp4_parser.yy"
                         {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 1924 "dhcp4_parser.cc"
    break;

  case 238: // database: "{" $@38 database_map_params "}"
#line 1030 "dhcp4_parser.yy"
                                     {
    // The type parameter is required
    ctx.require("type", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
}
#line 1934 "dhcp4_parser.cc"
    break;

  case 241: // database_map_params: database_map_params ","
#line 1038 "dhcp4_parser.yy"
                                               {
                       ctx.warnAboutExtraCommas(yystack_[0].location);
                       }
#line 1942 "dhcp4_parser.cc"
    break;

  case 264: // $@39: %empty
#line 1067 "dhcp4_parser.yy"
                    {
    ctx.unique("type", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.DATABASE_TYPE);
}
#line 1951 "dhcp4_parser.cc"
    break;

  case 265: // database_type: "type" $@39 ":" db_type
#line 1070 "dhcp4_parser.yy"
                {
    ctx.stack_.back()->set("type", yystack_[0].value.as < ElementPtr > ());
    ctx.leave();
}
#line 1960 "dhcp4_parser.cc"
    break;

  case 266: // db_type: "memfile"
#line 1075 "dhcp4_parser.yy"
                 { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("memfile", ctx.loc2pos(yystack_[0].location))); }
#line 1966 "dhcp4_parser.cc"
    break;

  case 267: // db_type: "mysql"
#line 1076 "dhcp4_parser.yy"
               { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("mysql", ctx.loc2pos(yystack_[0].location))); }
#line 1972 "dhcp4_parser.cc"
    break;

  case 268: // db_type: "postgresql"
#line 1077 "dhcp4_parser.yy"
                    { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("postgresql", ctx.loc2pos(yystack_[0].location))); }
#line 1978 "dhcp4_parser.cc"
    break;

  case 269: // $@40: %empty
#line 1080 "dhcp4_parser.yy"
           {
    ctx.unique("user", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1987 "dhcp4_parser.cc"
    break;

  case 270: // user: "user" $@40 ":" "constant string"
#line 1083 "dhcp4_parser.yy"
               {
    ElementPtr user(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("user", user);
    ctx.leave();
}
#line 1997 "dhcp4_parser.cc"
    break;

  case 271: // $@41: %empty
#line 1089 "dhcp4_parser.yy"
                   {
    ctx.unique("password", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2006 "dhcp4_parser.cc"
    break;

  case 272: // password: "password" $@41 ":" "constant string"
#line 1092 "dhcp4_parser.yy"
               {
    ElementPtr pwd(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("password", pwd);
    ctx.leave();
}
#line 2016 "dhcp4_parser.cc"
    break;

  case 273: // $@42: %empty
#line 1098 "dhcp4_parser.yy"
           {
    ctx.unique("host", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2025 "dhcp4_parser.cc"
    break;

  case 274: // host: "host" $@42 ":" "constant string"
#line 1101 "dhcp4_parser.yy"
               {
    ElementPtr h(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("host", h);
    ctx.leave();
}
#line 2035 "dhcp4_parser.cc"
    break;

  case 275: // port: "port" ":" "integer"
#line 1107 "dhcp4_parser.yy"
                         {
    ctx.unique("port", ctx.loc2pos(yystack_[2].location));
    ElementPtr p(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("port", p);
}
#line 2045 "dhcp4_parser.cc"
    break;

  case 276: // $@43: %empty
#line 1113 "dhcp4_parser.yy"
           {
    ctx.unique("name", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2054 "dhcp4_parser.cc"
    break;

  case 277: // name: "name" $@43 ":" "constant string"
#line 1116 "dhcp4_parser.yy"
               {
    ElementPtr name(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("name", name);
    ctx.leave();
}
#line 2064 "dhcp4_parser.cc"
    break;

  case 278: // persist: "persist" ":" "boolean"
#line 1122 "dhcp4_parser.yy"
                               {
    ctx.unique("persist", ctx.loc2pos(yystack_[2].location));
    ElementPtr n(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("persist", n);
}
#line 2074 "dhcp4_parser.cc"
    break;

  case 279: // lfc_interval: "lfc-interval" ":" "integer"
#line 1128 "dhcp4_parser.yy"
                                         {
    ctx.unique("lfc-interval", ctx.loc2pos(yystack_[2].location));
    ElementPtr n(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("lfc-interval", n);
}
#line 2084 "dhcp4_parser.cc"
    break;

  case 280: // readonly: "readonly" ":" "boolean"
#line 1134 "dhcp4_parser.yy"
                                 {
    ctx.unique("readonly", ctx.loc2pos(yystack_[2].location));
    ElementPtr n(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("readonly", n);
}
#line 2094 "dhcp4_parser.cc"
    break;

  case 281: // connect_timeout: "connect-timeout" ":" "integer"
#line 1140 "dhcp4_parser.yy"
                                               {
    ctx.unique("connect-timeout", ctx.loc2pos(yystack_[2].location));
    ElementPtr n(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("connect-timeout", n);
}
#line 2104 "dhcp4_parser.cc"
    break;

  case 282: // read_timeout: "read-timeout" ":" "integer"
#line 1146 "dhcp4_parser.yy"
                                         {
    ctx.unique("read-timeout", ctx.loc2pos(yystack_[2].location));
    ElementPtr n(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("read-timeout", n);
}
#line 2114 "dhcp4_parser.cc"
    break;

  case 283: // write_timeout: "write-timeout" ":" "integer"
#line 1152 "dhcp4_parser.yy"
                                           {
    ctx.unique("write-timeout", ctx.loc2pos(yystack_[2].location));
    ElementPtr n(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("write-timeout", n);
}
#line 2124 "dhcp4_parser.cc"
    break;

  case 284: // tcp_user_timeout: "tcp-user-timeout" ":" "integer"
#line 1158 "dhcp4_parser.yy"
                                                 {
    ctx.unique("tcp-user-timeout", ctx.loc2pos(yystack_[2].location));
    ElementPtr n(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("tcp-user-timeout", n);
}
#line 2134 "dhcp4_parser.cc"
    break;

  case 285: // max_reconnect_tries: "max-reconnect-tries" ":" "integer"
#line 1164 "dhcp4_parser.yy"
                                                       {
    ctx.unique("max-reconnect-tries", ctx.loc2pos(yystack_[2].location));
    ElementPtr n(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("max-reconnect-tries", n);
}
#line 2144 "dhcp4_parser.cc"
    break;

  case 286: // reconnect_wait_time: "reconnect-wait-time" ":" "integer"
#line 1170 "dhcp4_parser.yy"
                                                       {
    ctx.unique("reconnect-wait-time", ctx.loc2pos(yystack_[2].location));
    ElementPtr n(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("reconnect-wait-time", n);
}
#line 2154 "dhcp4_parser.cc"
    break;

  case 287: // $@44: %empty
#line 1176 "dhcp4_parser.yy"
                 {
    ctx.unique("on-fail", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.DATABASE_ON_FAIL);
}
#line 2163 "dhcp4_parser.cc"
    break;

  case 288: // on_fail: "on-fail" $@44 ":" on_fail_mode
#line 1179 "dhcp4_parser.yy"
                     {
    ctx.stack_.back()->set("on-fail", yystack_[0].value.as < ElementPtr > ());
    ctx.leave();
}
#line 2172 "dhcp4_parser.cc"
    break;

  case 289: // on_fail_mode: "stop-retry-exit"
#line 1184 "dhcp4_parser.yy"
                              { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("stop-retry-exit", ctx.loc2pos(yystack_[0].location))); }
#line 2178 "dhcp4_parser.cc"
    break;

  case 290: // on_fail_mode: "serve-retry-exit"
#line 1185 "dhcp4_parser.yy"
                               { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("serve-retry-exit", ctx.loc2pos(yystack_[0].location))); }
#line 2184 "dhcp4_parser.cc"
    break;

  case 291: // on_fail_mode: "serve-retry-continue"
#line 1186 "dhcp4_parser.yy"
                                   { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("serve-retry-continue", ctx.loc2pos(yystack_[0].location))); }
#line 2190 "dhcp4_parser.cc"
    break;

  case 292: // max_row_errors: "max-row-errors" ":" "integer"
#line 1189 "dhcp4_parser.yy"
                                             {
    ctx.unique("max-row-errors", ctx.loc2pos(yystack_[2].location));
    ElementPtr n(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("max-row-errors", n);
}
#line 2200 "dhcp4_parser.cc"
    break;

  case 293: // $@45: %empty
#line 1195 "dhcp4_parser.yy"
                           {
    ctx.unique("trust-anchor", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2209 "dhcp4_parser.cc"
    break;

  case 294: // trust_anchor: "trust-anchor" $@45 ":" "constant string"
#line 1198 "dhcp4_parser.yy"
               {
    ElementPtr ca(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("trust-anchor", ca);
    ctx.leave();
}
#line 2219 "dhcp4_parser.cc"
    break;

  case 295: // $@46: %empty
#line 1204 "dhcp4_parser.yy"
                     {
    ctx.unique("cert-file", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2228 "dhcp4_parser.cc"
    break;

  case 296: // cert_file: "cert-file" $@46 ":" "constant string"
#line 1207 "dhcp4_parser.yy"
               {
    ElementPtr cert(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("cert-file", cert);
    ctx.leave();
}
#line 2238 "dhcp4_parser.cc"
    break;

  case 297: // $@47: %empty
#line 1213 "dhcp4_parser.yy"
                   {
    ctx.unique("key-file", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2247 "dhcp4_parser.cc"
    break;

  case 298: // key_file: "key-file" $@47 ":" "constant string"
#line 1216 "dhcp4_parser.yy"
               {
    ElementPtr key(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("key-file", key);
    ctx.leave();
}
#line 2257 "dhcp4_parser.cc"
    break;

  case 299: // $@48: %empty
#line 1222 "dhcp4_parser.yy"
                         {
    ctx.unique("cipher-list", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2266 "dhcp4_parser.cc"
    break;

  case 300: // cipher_list: "cipher-list" $@48 ":" "constant string"
#line 1225 "dhcp4_parser.yy"
               {
    ElementPtr cl(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("cipher-list", cl);
    ctx.leave();
}
#line 2276 "dhcp4_parser.cc"
    break;

  case 301: // $@49: %empty
#line 1231 "dhcp4_parser.yy"
                                                           {
    ctx.unique("host-reservation-identifiers", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("host-reservation-identifiers", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.HOST_RESERVATION_IDENTIFIERS);
}
#line 2288 "dhcp4_parser.cc"
    break;

  case 302: // host_reservation_identifiers: "host-reservation-identifiers" $@49 ":" "[" host_reservation_identifiers_list "]"
#line 1237 "dhcp4_parser.yy"
                                                                          {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2297 "dhcp4_parser.cc"
    break;

  case 305: // host_reservation_identifiers_list: host_reservation_identifiers_list ","
#line 1244 "dhcp4_parser.yy"
                                              {
        ctx.warnAboutExtraCommas(yystack_[0].location);
        }
#line 2305 "dhcp4_parser.cc"
    break;

  case 311: // duid_id: "duid"
#line 1256 "dhcp4_parser.yy"
              {
    ElementPtr duid(new StringElement("duid", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(duid);
}
#line 2314 "dhcp4_parser.cc"
    break;

  case 312: // hw_address_id: "hw-address"
#line 1261 "dhcp4_parser.yy"
                          {
    ElementPtr hwaddr(new StringElement("hw-address", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(hwaddr);
}
#line 2323 "dhcp4_parser.cc"
    break;

  case 313: // circuit_id: "circuit-id"
#line 1266 "dhcp4_parser.yy"
                       {
    ElementPtr circuit(new StringElement("circuit-id", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(circuit);
}
#line 2332 "dhcp4_parser.cc"
    break;

  case 314: // client_id: "client-id"
#line 1271 "dhcp4_parser.yy"
                     {
    ElementPtr client(new StringElement("client-id", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(client);
}
#line 2341 "dhcp4_parser.cc"
    break;

  case 315: // flex_id: "flex-id"
#line 1276 "dhcp4_parser.yy"
                 {
    ElementPtr flex_id(new StringElement("flex-id", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(flex_id);
}
#line 2350 "dhcp4_parser.cc"
    break;

  case 316: // $@50: %empty
#line 1283 "dhcp4_parser.yy"
                                           {
    ctx.unique("multi-threading", ctx.loc2pos(yystack_[0].location));
    ElementPtr mt(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("multi-threading", mt);
    ctx.stack_.push_back(mt);
    ctx.enter(ctx.DHCP_MULTI_THREADING);
}
#line 2362 "dhcp4_parser.cc"
    break;

  case 317: // dhcp_multi_threading: "multi-threading" $@50 ":" "{" multi_threading_params "}"
#line 1289 "dhcp4_parser.yy"
                                                             {
    // The enable parameter is required.
    ctx.require("enable-multi-threading", ctx.loc2pos(yystack_[2].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2373 "dhcp4_parser.cc"
    break;

  case 320: // multi_threading_params: multi_threading_params ","
#line 1298 "dhcp4_parser.yy"
                                                     {
                          ctx.warnAboutExtraCommas(yystack_[0].location);
                          }
#line 2381 "dhcp4_parser.cc"
    break;

  case 327: // enable_multi_threading: "enable-multi-threading" ":" "boolean"
#line 1311 "dhcp4_parser.yy"
                                                             {
    ctx.unique("enable-multi-threading", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("enable-multi-threading", b);
}
#line 2391 "dhcp4_parser.cc"
    break;

  case 328: // thread_pool_size: "thread-pool-size" ":" "integer"
#line 1317 "dhcp4_parser.yy"
                                                 {
    ctx.unique("thread-pool-size", ctx.loc2pos(yystack_[2].location));
    ElementPtr prf(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("thread-pool-size", prf);
}
#line 2401 "dhcp4_parser.cc"
    break;

  case 329: // packet_queue_size: "packet-queue-size" ":" "integer"
#line 1323 "dhcp4_parser.yy"
                                                   {
    ctx.unique("packet-queue-size", ctx.loc2pos(yystack_[2].location));
    ElementPtr prf(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("packet-queue-size", prf);
}
#line 2411 "dhcp4_parser.cc"
    break;

  case 330: // $@51: %empty
#line 1329 "dhcp4_parser.yy"
                                 {
    ctx.unique("hooks-libraries", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hooks-libraries", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.HOOKS_LIBRARIES);
}
#line 2423 "dhcp4_parser.cc"
    break;

  case 331: // hooks_libraries: "hooks-libraries" $@51 ":" "[" hooks_libraries_list "]"
#line 1335 "dhcp4_parser.yy"
                                                             {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2432 "dhcp4_parser.cc"
    break;

  case 336: // not_empty_hooks_libraries_list: not_empty_hooks_libraries_list ","
#line 1346 "dhcp4_parser.yy"
                                           {
        ctx.warnAboutExtraCommas(yystack_[0].location);
        }
#line 2440 "dhcp4_parser.cc"
    break;

  case 337: // $@52: %empty
#line 1351 "dhcp4_parser.yy"
                              {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 2450 "dhcp4_parser.cc"
    break;

  case 338: // hooks_library: "{" $@52 hooks_params "}"
#line 1355 "dhcp4_parser.yy"
                              {
    // The library hooks parameter is required
    ctx.require("library", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
}
#line 2460 "dhcp4_parser.cc"
    break;

  case 339: // $@53: %empty
#line 1361 "dhcp4_parser.yy"
                                  {
    // Parse the hooks-libraries list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 2470 "dhcp4_parser.cc"
    break;

  case 340: // sub_hooks_library: "{" $@53 hooks_params "}"
#line 1365 "dhcp4_parser.yy"
                              {
    // The library hooks parameter is required
    ctx.require("library", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    // parsing completed
}
#line 2480 "dhcp4_parser.cc"
    break;

  case 343: // hooks_params: hooks_params ","
#line 1373 "dhcp4_parser.yy"
                                 {
                ctx.warnAboutExtraCommas(yystack_[0].location);
                }
#line 2488 "dhcp4_parser.cc"
    break;

  case 347: // $@54: %empty
#line 1383 "dhcp4_parser.yy"
                 {
    ctx.unique("library", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2497 "dhcp4_parser.cc"
    break;

  case 348: // library: "library" $@54 ":" "constant string"
#line 1386 "dhcp4_parser.yy"
               {
    ElementPtr lib(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("library", lib);
    ctx.leave();
}
#line 2507 "dhcp4_parser.cc"
    break;

  case 349: // $@55: %empty
#line 1392 "dhcp4_parser.yy"
                       {
    ctx.unique("parameters", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2516 "dhcp4_parser.cc"
    break;

  case 350: // parameters: "parameters" $@55 ":" map_value
#line 1395 "dhcp4_parser.yy"
                  {
    ctx.stack_.back()->set("parameters", yystack_[0].value.as < ElementPtr > ());
    ctx.leave();
}
#line 2525 "dhcp4_parser.cc"
    break;

  case 351: // $@56: %empty
#line 1401 "dhcp4_parser.yy"
                                                     {
    ctx.unique("expired-leases-processing", ctx.loc2pos(yystack_[0].location));
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("expired-leases-processing", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.EXPIRED_LEASES_PROCESSING);
}
#line 2537 "dhcp4_parser.cc"
    break;

  case 352: // expired_leases_processing: "expired-leases-processing" $@56 ":" "{" expired_leases_params "}"
#line 1407 "dhcp4_parser.yy"
                                                            {
    // No expired lease parameter is required
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2547 "dhcp4_parser.cc"
    break;

  case 355: // expired_leases_params: expired_leases_params ","
#line 1415 "dhcp4_parser.yy"
                                                   {
                         ctx.warnAboutExtraCommas(yystack_[0].location);
                         }
#line 2555 "dhcp4_parser.cc"
    break;

  case 362: // reclaim_timer_wait_time: "reclaim-timer-wait-time" ":" "integer"
#line 1428 "dhcp4_parser.yy"
                                                               {
    ctx.unique("reclaim-timer-wait-time", ctx.loc2pos(yystack_[2].location));
    ElementPtr value(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("reclaim-timer-wait-time", value);
}
#line 2565 "dhcp4_parser.cc"
    break;

  case 363: // flush_reclaimed_timer_wait_time: "flush-reclaimed-timer-wait-time" ":" "integer"
#line 1434 "dhcp4_parser.yy"
                                                                               {
    ctx.unique("flush-reclaimed-timer-wait-time", ctx.loc2pos(yystack_[2].location));
    ElementPtr value(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("flush-reclaimed-timer-wait-time", value);
}
#line 2575 "dhcp4_parser.cc"
    break;

  case 364: // hold_reclaimed_time: "hold-reclaimed-time" ":" "integer"
#line 1440 "dhcp4_parser.yy"
                                                       {
    ctx.unique("hold-reclaimed-time", ctx.loc2pos(yystack_[2].location));
    ElementPtr value(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hold-reclaimed-time", value);
}
#line 2585 "dhcp4_parser.cc"
    break;

  case 365: // max_reclaim_leases: "max-reclaim-leases" ":" "integer"
#line 1446 "dhcp4_parser.yy"
                                                     {
    ctx.unique("max-reclaim-leases", ctx.loc2pos(yystack_[2].location));
    ElementPtr value(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("max-reclaim-leases", value);
}
#line 2595 "dhcp4_parser.cc"
    break;

  case 366: // max_reclaim_time: "max-reclaim-time" ":" "integer"
#line 1452 "dhcp4_parser.yy"
                                                 {
    ctx.unique("max-reclaim-time", ctx.loc2pos(yystack_[2].location));
    ElementPtr value(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("max-reclaim-time", value);
}
#line 2605 "dhcp4_parser.cc"
    break;

  case 367: // unwarned_reclaim_cycles: "unwarned-reclaim-cycles" ":" "integer"
#line 1458 "dhcp4_parser.yy"
                                                               {
    ctx.unique("unwarned-reclaim-cycles", ctx.loc2pos(yystack_[2].location));
    ElementPtr value(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("unwarned-reclaim-cycles", value);
}
#line 2615 "dhcp4_parser.cc"
    break;

  case 368: // $@57: %empty
#line 1467 "dhcp4_parser.yy"
                      {
    ctx.unique("subnet4", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("subnet4", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.SUBNET4);
}
#line 2627 "dhcp4_parser.cc"
    break;

  case 369: // subnet4_list: "subnet4" $@57 ":" "[" subnet4_list_content "]"
#line 1473 "dhcp4_parser.yy"
                                                             {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2636 "dhcp4_parser.cc"
    break;

  case 374: // not_empty_subnet4_list: not_empty_subnet4_list ","
#line 1487 "dhcp4_parser.yy"
                                                     {
                          ctx.warnAboutExtraCommas(yystack_[0].location);
                          }
#line 2644 "dhcp4_parser.cc"
    break;

  case 375: // $@58: %empty
#line 1496 "dhcp4_parser.yy"
                        {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 2654 "dhcp4_parser.cc"
    break;

  case 376: // subnet4: "{" $@58 subnet4_params "}"
#line 1500 "dhcp4_parser.yy"
                                {
    // Once we reached this place, the subnet parsing is now complete.
    // If we want to, we can implement default values here.
    // In particular we can do things like this:
    // if (!ctx.stack_.back()->get("interface")) {
    //     ctx.stack_.back()->set("interface", StringElement("loopback"));
    // }
    //
    // We can also stack up one level (Dhcp4) and copy over whatever
    // global parameters we want to:
    // if (!ctx.stack_.back()->get("renew-timer")) {
    //     ElementPtr renew = ctx_stack_[...].get("renew-timer");
    //     if (renew) {
    //         ctx.stack_.back()->set("renew-timer", renew);
    //     }
    // }

    // The subnet subnet4 parameter is required
    ctx.require("subnet", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
}
#line 2680 "dhcp4_parser.cc"
    break;

  case 377: // $@59: %empty
#line 1522 "dhcp4_parser.yy"
                            {
    // Parse the subnet4 list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 2690 "dhcp4_parser.cc"
    break;

  case 378: // sub_subnet4: "{" $@59 subnet4_params "}"
#line 1526 "dhcp4_parser.yy"
                                {
    // The subnet subnet4 parameter is required
    ctx.require("subnet", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    // parsing completed
}
#line 2700 "dhcp4_parser.cc"
    break;

  case 381: // subnet4_params: subnet4_params ","
#line 1535 "dhcp4_parser.yy"
                                     {
                  ctx.warnAboutExtraCommas(yystack_[0].location);
                  }
#line 2708 "dhcp4_parser.cc"
    break;

  case 430: // $@60: %empty
#line 1591 "dhcp4_parser.yy"
               {
    ctx.unique("subnet", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2717 "dhcp4_parser.cc"
    break;

  case 431: // subnet: "subnet" $@60 ":" "constant string"
#line 1594 "dhcp4_parser.yy"
               {
    ElementPtr subnet(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("subnet", subnet);
    ctx.leave();
}
#line 2727 "dhcp4_parser.cc"
    break;

  case 432: // $@61: %empty
#line 1600 "dhcp4_parser.yy"
                                           {
    ctx.unique("4o6-interface", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2736 "dhcp4_parser.cc"
    break;

  case 433: // subnet_4o6_interface: "4o6-interface" $@61 ":" "constant string"
#line 1603 "dhcp4_parser.yy"
               {
    ElementPtr iface(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("4o6-interface", iface);
    ctx.leave();
}
#line 2746 "dhcp4_parser.cc"
    break;

  case 434: // $@62: %empty
#line 1609 "dhcp4_parser.yy"
                                                 {
    ctx.unique("4o6-interface-id", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2755 "dhcp4_parser.cc"
    break;

  case 435: // subnet_4o6_interface_id: "4o6-interface-id" $@62 ":" "constant string"
#line 1612 "dhcp4_parser.yy"
               {
    ElementPtr iface(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("4o6-interface-id", iface);
    ctx.leave();
}
#line 2765 "dhcp4_parser.cc"
    break;

  case 436: // $@63: %empty
#line 1618 "dhcp4_parser.yy"
                                     {
    ctx.unique("4o6-subnet", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2774 "dhcp4_parser.cc"
    break;

  case 437: // subnet_4o6_subnet: "4o6-subnet" $@63 ":" "constant string"
#line 1621 "dhcp4_parser.yy"
               {
    ElementPtr iface(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("4o6-subnet", iface);
    ctx.leave();
}
#line 2784 "dhcp4_parser.cc"
    break;

  case 438: // $@64: %empty
#line 1627 "dhcp4_parser.yy"
                     {
    ctx.unique("interface", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2793 "dhcp4_parser.cc"
    break;

  case 439: // interface: "interface" $@64 ":" "constant string"
#line 1630 "dhcp4_parser.yy"
               {
    ElementPtr iface(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("interface", iface);
    ctx.leave();
}
#line 2803 "dhcp4_parser.cc"
    break;

  case 440: // $@65: %empty
#line 1636 "dhcp4_parser.yy"
                           {
    ctx.unique("client-class", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2812 "dhcp4_parser.cc"
    break;

  case 441: // client_class: "client-class" $@65 ":" "constant string"
#line 1639 "dhcp4_parser.yy"
               {
    ElementPtr cls(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("client-class", cls);
    ctx.leave();
}
#line 2822 "dhcp4_parser.cc"
    break;

  case 442: // $@66: %empty
#line 1645 "dhcp4_parser.yy"
                                               {
    ctx.unique("require-client-classes", ctx.loc2pos(yystack_[0].location));
    ElementPtr c(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("require-client-classes", c);
    ctx.stack_.push_back(c);
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2834 "dhcp4_parser.cc"
    break;

  case 443: // require_client_classes: "require-client-classes" $@66 ":" list_strings
#line 1651 "dhcp4_parser.yy"
                     {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2843 "dhcp4_parser.cc"
    break;

  case 444: // reservations_global: "reservations-global" ":" "boolean"
#line 1656 "dhcp4_parser.yy"
                                                       {
    ctx.unique("reservations-global", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("reservations-global", b);
}
#line 2853 "dhcp4_parser.cc"
    break;

  case 445: // reservations_in_subnet: "reservations-in-subnet" ":" "boolean"
#line 1662 "dhcp4_parser.yy"
                                                             {
    ctx.unique("reservations-in-subnet", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("reservations-in-subnet", b);
}
#line 2863 "dhcp4_parser.cc"
    break;

  case 446: // reservations_out_of_pool: "reservations-out-of-pool" ":" "boolean"
#line 1668 "dhcp4_parser.yy"
                                                                 {
    ctx.unique("reservations-out-of-pool", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("reservations-out-of-pool", b);
}
#line 2873 "dhcp4_parser.cc"
    break;

  case 447: // $@67: %empty
#line 1674 "dhcp4_parser.yy"
                                   {
    ctx.unique("reservation-mode", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.RESERVATION_MODE);
}
#line 2882 "dhcp4_parser.cc"
    break;

  case 448: // reservation_mode: "reservation-mode" $@67 ":" hr_mode
#line 1677 "dhcp4_parser.yy"
                {
    ctx.stack_.back()->set("reservation-mode", yystack_[0].value.as < ElementPtr > ());
    ctx.leave();
}
#line 2891 "dhcp4_parser.cc"
    break;

  case 449: // hr_mode: "disabled"
#line 1682 "dhcp4_parser.yy"
                  { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("disabled", ctx.loc2pos(yystack_[0].location))); }
#line 2897 "dhcp4_parser.cc"
    break;

  case 450: // hr_mode: "out-of-pool"
#line 1683 "dhcp4_parser.yy"
                     { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("out-of-pool", ctx.loc2pos(yystack_[0].location))); }
#line 2903 "dhcp4_parser.cc"
    break;

  case 451: // hr_mode: "global"
#line 1684 "dhcp4_parser.yy"
                { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("global", ctx.loc2pos(yystack_[0].location))); }
#line 2909 "dhcp4_parser.cc"
    break;

  case 452: // hr_mode: "all"
#line 1685 "dhcp4_parser.yy"
             { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("all", ctx.loc2pos(yystack_[0].location))); }
#line 2915 "dhcp4_parser.cc"
    break;

  case 453: // id: "id" ":" "integer"
#line 1688 "dhcp4_parser.yy"
                     {
    ctx.unique("id", ctx.loc2pos(yystack_[2].location));
    ElementPtr id(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("id", id);
}
#line 2925 "dhcp4_parser.cc"
    break;

  case 454: // $@68: %empty
#line 1696 "dhcp4_parser.yy"
                                 {
    ctx.unique("shared-networks", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("shared-networks", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.SHARED_NETWORK);
}
#line 2937 "dhcp4_parser.cc"
    break;

  case 455: // shared_networks: "shared-networks" $@68 ":" "[" shared_networks_content "]"
#line 1702 "dhcp4_parser.yy"
                                                                {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2946 "dhcp4_parser.cc"
    break;

  case 460: // shared_networks_list: shared_networks_list ","
#line 1715 "dhcp4_parser.yy"
                                                 {
                        ctx.warnAboutExtraCommas(yystack_[0].location);
                        }
#line 2954 "dhcp4_parser.cc"
    break;

  case 461: // $@69: %empty
#line 1720 "dhcp4_parser.yy"
                               {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 2964 "dhcp4_parser.cc"
    break;

  case 462: // shared_network: "{" $@69 shared_network_params "}"
#line 1724 "dhcp4_parser.yy"
                                       {
    ctx.stack_.pop_back();
}
#line 2972 "dhcp4_parser.cc"
    break;

  case 465: // shared_network_params: shared_network_params ","
#line 1730 "dhcp4_parser.yy"
                                                   {
                         ctx.warnAboutExtraCommas(yystack_[0].location);
                         }
#line 2980 "dhcp4_parser.cc"
    break;

  case 509: // $@70: %empty
#line 1784 "dhcp4_parser.yy"
                            {
    ctx.unique("option-def", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("option-def", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.OPTION_DEF);
}
#line 2992 "dhcp4_parser.cc"
    break;

  case 510: // option_def_list: "option-def" $@70 ":" "[" option_def_list_content "]"
#line 1790 "dhcp4_parser.yy"
                                                                {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 3001 "dhcp4_parser.cc"
    break;

  case 511: // $@71: %empty
#line 1798 "dhcp4_parser.yy"
                                    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 3010 "dhcp4_parser.cc"
    break;

  case 512: // sub_option_def_list: "{" $@71 option_def_list "}"
#line 1801 "dhcp4_parser.yy"
                                 {
    // parsing completed
}
#line 3018 "dhcp4_parser.cc"
    break;

  case 517: // not_empty_option_def_list: not_empty_option_def_list ","
#line 1813 "dhcp4_parser.yy"
                                                           {
                             ctx.warnAboutExtraCommas(yystack_[0].location);
                             }
#line 3026 "dhcp4_parser.cc"
    break;

  case 518: // $@72: %empty
#line 1820 "dhcp4_parser.yy"
                                 {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 3036 "dhcp4_parser.cc"
    break;

  case 519: // option_def_entry: "{" $@72 option_def_params "}"
#line 1824 "dhcp4_parser.yy"
                                   {
    // The name, code and type option def parameters are required.
    ctx.require("name", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.require("code", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.require("type", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
}
#line 3048 "dhcp4_parser.cc"
    break;

  case 520: // $@73: %empty
#line 1835 "dhcp4_parser.yy"
                               {
    // Parse the option-def list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 3058 "dhcp4_parser.cc"
    break;

  case 521: // sub_option_def: "{" $@73 option_def_params "}"
#line 1839 "dhcp4_parser.yy"
                                   {
    // The name, code and type option def parameters are required.
    ctx.require("name", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.require("code", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.require("type", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    // parsing completed
}
#line 3070 "dhcp4_parser.cc"
    break;

  case 526: // not_empty_option_def_params: not_empty_option_def_params ","
#line 1855 "dhcp4_parser.yy"
                                                               {
                               ctx.warnAboutExtraCommas(yystack_[0].location);
                               }
#line 3078 "dhcp4_parser.cc"
    break;

  case 538: // code: "code" ":" "integer"
#line 1874 "dhcp4_parser.yy"
                         {
    ctx.unique("code", ctx.loc2pos(yystack_[2].location));
    ElementPtr code(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("code", code);
}
#line 3088 "dhcp4_parser.cc"
    break;

  case 540: // $@74: %empty
#line 1882 "dhcp4_parser.yy"
                      {
    ctx.unique("type", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3097 "dhcp4_parser.cc"
    break;

  case 541: // option_def_type: "type" $@74 ":" "constant string"
#line 1885 "dhcp4_parser.yy"
               {
    ElementPtr prf(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("type", prf);
    ctx.leave();
}
#line 3107 "dhcp4_parser.cc"
    break;

  case 542: // $@75: %empty
#line 1891 "dhcp4_parser.yy"
                                      {
    ctx.unique("record-types", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3116 "dhcp4_parser.cc"
    break;

  case 543: // option_def_record_types: "record-types" $@75 ":" "constant string"
#line 1894 "dhcp4_parser.yy"
               {
    ElementPtr rtypes(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("record-types", rtypes);
    ctx.leave();
}
#line 3126 "dhcp4_parser.cc"
    break;

  case 544: // $@76: %empty
#line 1900 "dhcp4_parser.yy"
             {
    ctx.unique("space", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3135 "dhcp4_parser.cc"
    break;

  case 545: // space: "space" $@76 ":" "constant string"
#line 1903 "dhcp4_parser.yy"
               {
    ElementPtr space(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("space", space);
    ctx.leave();
}
#line 3145 "dhcp4_parser.cc"
    break;

  case 547: // $@77: %empty
#line 1911 "dhcp4_parser.yy"
                                    {
    ctx.unique("encapsulate", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3154 "dhcp4_parser.cc"
    break;

  case 548: // option_def_encapsulate: "encapsulate" $@77 ":" "constant string"
#line 1914 "dhcp4_parser.yy"
               {
    ElementPtr encap(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("encapsulate", encap);
    ctx.leave();
}
#line 3164 "dhcp4_parser.cc"
    break;

  case 549: // option_def_array: "array" ":" "boolean"
#line 1920 "dhcp4_parser.yy"
                                      {
    ctx.unique("array", ctx.loc2pos(yystack_[2].location));
    ElementPtr array(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("array", array);
}
#line 3174 "dhcp4_parser.cc"
    break;

  case 550: // $@78: %empty
#line 1930 "dhcp4_parser.yy"
                              {
    ctx.unique("option-data", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("option-data", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.OPTION_DATA);
}
#line 3186 "dhcp4_parser.cc"
    break;

  case 551: // option_data_list: "option-data" $@78 ":" "[" option_data_list_content "]"
#line 1936 "dhcp4_parser.yy"
                                                                 {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 3195 "dhcp4_parser.cc"
    break;

  case 556: // not_empty_option_data_list: not_empty_option_data_list ","
#line 1951 "dhcp4_parser.yy"
                                                             {
                              ctx.warnAboutExtraCommas(yystack_[0].location);
                              }
#line 3203 "dhcp4_parser.cc"
    break;

  case 557: // $@79: %empty
#line 1958 "dhcp4_parser.yy"
                                  {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 3213 "dhcp4_parser.cc"
    break;

  case 558: // option_data_entry: "{" $@79 option_data_params "}"
#line 1962 "dhcp4_parser.yy"
                                    {
    /// @todo: the code or name parameters are required.
    ctx.stack_.pop_back();
}
#line 3222 "dhcp4_parser.cc"
    break;

  case 559: // $@80: %empty
#line 1970 "dhcp4_parser.yy"
                                {
    // Parse the option-data list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 3232 "dhcp4_parser.cc"
    break;

  case 560: // sub_option_data: "{" $@80 option_data_params "}"
#line 1974 "dhcp4_parser.yy"
                                    {
    /// @todo: the code or name parameters are required.
    // parsing completed
}
#line 3241 "dhcp4_parser.cc"
    break;

  case 565: // not_empty_option_data_params: not_empty_option_data_params ","
#line 1990 "dhcp4_parser.yy"
                                         {
        ctx.warnAboutExtraCommas(yystack_[0].location);
        }
#line 3249 "dhcp4_parser.cc"
    break;

  case 577: // $@81: %empty
#line 2011 "dhcp4_parser.yy"
                       {
    ctx.unique("data", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3258 "dhcp4_parser.cc"
    break;

  case 578: // option_data_data: "data" $@81 ":" "constant string"
#line 2014 "dhcp4_parser.yy"
               {
    ElementPtr data(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("data", data);
    ctx.leave();
}
#line 3268 "dhcp4_parser.cc"
    break;

  case 581: // option_data_csv_format: "csv-format" ":" "boolean"
#line 2024 "dhcp4_parser.yy"
                                                 {
    ctx.unique("csv-format", ctx.loc2pos(yystack_[2].location));
    ElementPtr csv(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("csv-format", csv);
}
#line 3278 "dhcp4_parser.cc"
    break;

  case 582: // option_data_always_send: "always-send" ":" "boolean"
#line 2030 "dhcp4_parser.yy"
                                                   {
    ctx.unique("always-send", ctx.loc2pos(yystack_[2].location));
    ElementPtr persist(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("always-send", persist);
}
#line 3288 "dhcp4_parser.cc"
    break;

  case 583: // option_data_never_send: "never-send" ":" "boolean"
#line 2036 "dhcp4_parser.yy"
                                                 {
    ctx.unique("never-send", ctx.loc2pos(yystack_[2].location));
    ElementPtr cancel(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("never-send", cancel);
}
#line 3298 "dhcp4_parser.cc"
    break;

  case 584: // $@82: %empty
#line 2045 "dhcp4_parser.yy"
                  {
    ctx.unique("pools", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("pools", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.POOLS);
}
#line 3310 "dhcp4_parser.cc"
    break;

  case 585: // pools_list: "pools" $@82 ":" "[" pools_list_content "]"
#line 2051 "dhcp4_parser.yy"
                                                           {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 3319 "dhcp4_parser.cc"
    break;

  case 590: // not_empty_pools_list: not_empty_pools_list ","
#line 2064 "dhcp4_parser.yy"
                                                 {
                        ctx.warnAboutExtraCommas(yystack_[0].location);
                        }
#line 3327 "dhcp4_parser.cc"
    break;

  case 591: // $@83: %empty
#line 2069 "dhcp4_parser.yy"
                                {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 3337 "dhcp4_parser.cc"
    break;

  case 592: // pool_list_entry: "{" $@83 pool_params "}"
#line 2073 "dhcp4_parser.yy"
                             {
    // The pool parameter is required.
    ctx.require("pool", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
}
#line 3347 "dhcp4_parser.cc"
    break;

  case 593: // $@84: %empty
#line 2079 "dhcp4_parser.yy"
                          {
    // Parse the pool list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 3357 "dhcp4_parser.cc"
    break;

  case 594: // sub_pool4: "{" $@84 pool_params "}"
#line 2083 "dhcp4_parser.yy"
                             {
    // The pool parameter is required.
    ctx.require("pool", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    // parsing completed
}
#line 3367 "dhcp4_parser.cc"
    break;

  case 597: // pool_params: pool_params ","
#line 2091 "dhcp4_parser.yy"
                               {
               ctx.warnAboutExtraCommas(yystack_[0].location);
               }
#line 3375 "dhcp4_parser.cc"
    break;

  case 606: // $@85: %empty
#line 2106 "dhcp4_parser.yy"
                 {
    ctx.unique("pool", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3384 "dhcp4_parser.cc"
    break;

  case 607: // pool_entry: "pool" $@85 ":" "constant string"
#line 2109 "dhcp4_parser.yy"
               {
    ElementPtr pool(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("pool", pool);
    ctx.leave();
}
#line 3394 "dhcp4_parser.cc"
    break;

  case 608: // pool_id: "pool-id" ":" "integer"
#line 2115 "dhcp4_parser.yy"
                               {
    ctx.unique("pool-id", ctx.loc2pos(yystack_[2].location));
    ElementPtr id(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("pool-id", id);
}
#line 3404 "dhcp4_parser.cc"
    break;

  case 609: // $@86: %empty
#line 2121 "dhcp4_parser.yy"
                           {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3412 "dhcp4_parser.cc"
    break;

  case 610: // user_context: "user-context" $@86 ":" map_value
#line 2123 "dhcp4_parser.yy"
                  {
    ElementPtr parent = ctx.stack_.back();
    ElementPtr user_context = yystack_[0].value.as < ElementPtr > ();
    ConstElementPtr old = parent->get("user-context");

    // Handle already existing user context
    if (old) {
        // Check if it was a comment or a duplicate
        if ((old->size() != 1) || !old->contains("comment")) {
            std::stringstream msg;
            msg << "duplicate user-context entries (previous at "
                << old->getPosition().str() << ")";
            error(yystack_[3].location, msg.str());
        }
        // Merge the comment
        user_context->set("comment", old->get("comment"));
    }

    // Set the user context
    parent->set("user-context", user_context);
    ctx.leave();
}
#line 3439 "dhcp4_parser.cc"
    break;

  case 611: // $@87: %empty
#line 2146 "dhcp4_parser.yy"
                 {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3447 "dhcp4_parser.cc"
    break;

  case 612: // comment: "comment" $@87 ":" "constant string"
#line 2148 "dhcp4_parser.yy"
               {
    ElementPtr parent = ctx.stack_.back();
    ElementPtr user_context(new MapElement(ctx.loc2pos(yystack_[3].location)));
    ElementPtr comment(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    user_context->set("comment", comment);

    // Handle already existing user context
    ConstElementPtr old = parent->get("user-context");
    if (old) {
        // Check for duplicate comment
        if (old->contains("comment")) {
            std::stringstream msg;
            msg << "duplicate user-context/comment entries (previous at "
                << old->getPosition().str() << ")";
            error(yystack_[3].location, msg.str());
        }
        // Merge the user context in the comment
        merge(user_context, old);
    }

    // Set the user context
    parent->set("user-context", user_context);
    ctx.leave();
}
#line 3476 "dhcp4_parser.cc"
    break;

  case 613: // $@88: %empty
#line 2176 "dhcp4_parser.yy"
                           {
    ctx.unique("reservations", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("reservations", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.RESERVATIONS);
}
#line 3488 "dhcp4_parser.cc"
    break;

  case 614: // reservations: "reservations" $@88 ":" "[" reservations_list "]"
#line 2182 "dhcp4_parser.yy"
                                                          {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 3497 "dhcp4_parser.cc"
    break;

  case 619: // not_empty_reservations_list: not_empty_reservations_list ","
#line 2193 "dhcp4_parser.yy"
                                                               {
                               ctx.warnAboutExtraCommas(yystack_[0].location);
                               }
#line 3505 "dhcp4_parser.cc"
    break;

  case 620: // $@89: %empty
#line 2198 "dhcp4_parser.yy"
                            {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 3515 "dhcp4_parser.cc"
    break;

  case 621: // reservation: "{" $@89 reservation_params "}"
#line 2202 "dhcp4_parser.yy"
                                    {
    /// @todo: an identifier parameter is required.
    ctx.stack_.pop_back();
}
#line 3524 "dhcp4_parser.cc"
    break;

  case 622: // $@90: %empty
#line 2207 "dhcp4_parser.yy"
                                {
    // Parse the reservations list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 3534 "dhcp4_parser.cc"
    break;

  case 623: // sub_reservation: "{" $@90 reservation_params "}"
#line 2211 "dhcp4_parser.yy"
                                    {
    /// @todo: an identifier parameter is required.
    // parsing completed
}
#line 3543 "dhcp4_parser.cc"
    break;

  case 628: // not_empty_reservation_params: not_empty_reservation_params ","
#line 2222 "dhcp4_parser.yy"
                                         {
        ctx.warnAboutExtraCommas(yystack_[0].location);
        }
#line 3551 "dhcp4_parser.cc"
    break;

  case 644: // $@91: %empty
#line 2245 "dhcp4_parser.yy"
                         {
    ctx.unique("next-server", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3560 "dhcp4_parser.cc"
    break;

  case 645: // next_server: "next-server" $@91 ":" "constant string"
#line 2248 "dhcp4_parser.yy"
               {
    ElementPtr next_server(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("next-server", next_server);
    ctx.leave();
}
#line 3570 "dhcp4_parser.cc"
    break;

  case 646: // $@92: %empty
#line 2254 "dhcp4_parser.yy"
                                 {
    ctx.unique("server-hostname", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3579 "dhcp4_parser.cc"
    break;

  case 647: // server_hostname: "server-hostname" $@92 ":" "constant string"
#line 2257 "dhcp4_parser.yy"
               {
    ElementPtr srv(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("server-hostname", srv);
    ctx.leave();
}
#line 3589 "dhcp4_parser.cc"
    break;

  case 648: // $@93: %empty
#line 2263 "dhcp4_parser.yy"
                               {
    ctx.unique("boot-file-name", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3598 "dhcp4_parser.cc"
    break;

  case 649: // boot_file_name: "boot-file-name" $@93 ":" "constant string"
#line 2266 "dhcp4_parser.yy"
               {
    ElementPtr bootfile(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("boot-file-name", bootfile);
    ctx.leave();
}
#line 3608 "dhcp4_parser.cc"
    break;

  case 650: // $@94: %empty
#line 2272 "dhcp4_parser.yy"
                       {
    ctx.unique("ip-address", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3617 "dhcp4_parser.cc"
    break;

  case 651: // ip_address: "ip-address" $@94 ":" "constant string"
#line 2275 "dhcp4_parser.yy"
               {
    ElementPtr addr(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ip-address", addr);
    ctx.leave();
}
#line 3627 "dhcp4_parser.cc"
    break;

  case 652: // $@95: %empty
#line 2281 "dhcp4_parser.yy"
                           {
    ctx.unique("ip-addresses", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ip-addresses", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3639 "dhcp4_parser.cc"
    break;

  case 653: // ip_addresses: "ip-addresses" $@95 ":" list_strings
#line 2287 "dhcp4_parser.yy"
                     {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 3648 "dhcp4_parser.cc"
    break;

  case 654: // $@96: %empty
#line 2292 "dhcp4_parser.yy"
           {
    ctx.unique("duid", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3657 "dhcp4_parser.cc"
    break;

  case 655: // duid: "duid" $@96 ":" "constant string"
#line 2295 "dhcp4_parser.yy"
               {
    ElementPtr d(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("duid", d);
    ctx.leave();
}
#line 3667 "dhcp4_parser.cc"
    break;

  case 656: // $@97: %empty
#line 2301 "dhcp4_parser.yy"
                       {
    ctx.unique("hw-address", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3676 "dhcp4_parser.cc"
    break;

  case 657: // hw_address: "hw-address" $@97 ":" "constant string"
#line 2304 "dhcp4_parser.yy"
               {
    ElementPtr hw(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hw-address", hw);
    ctx.leave();
}
#line 3686 "dhcp4_parser.cc"
    break;

  case 658: // $@98: %empty
#line 2310 "dhcp4_parser.yy"
                           {
    ctx.unique("client-id", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3695 "dhcp4_parser.cc"
    break;

  case 659: // client_id_value: "client-id" $@98 ":" "constant string"
#line 2313 "dhcp4_parser.yy"
               {
    ElementPtr hw(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("client-id", hw);
    ctx.leave();
}
#line 3705 "dhcp4_parser.cc"
    break;

  case 660: // $@99: %empty
#line 2319 "dhcp4_parser.yy"
                             {
    ctx.unique("circuit-id", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3714 "dhcp4_parser.cc"
    break;

  case 661: // circuit_id_value: "circuit-id" $@99 ":" "constant string"
#line 2322 "dhcp4_parser.yy"
               {
    ElementPtr hw(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("circuit-id", hw);
    ctx.leave();
}
#line 3724 "dhcp4_parser.cc"
    break;

  case 662: // $@100: %empty
#line 2328 "dhcp4_parser.yy"
                       {
    ctx.unique("flex-id", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3733 "dhcp4_parser.cc"
    break;

  case 663: // flex_id_value: "flex-id" $@100 ":" "constant string"
#line 2331 "dhcp4_parser.yy"
               {
    ElementPtr hw(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("flex-id", hw);
    ctx.leave();
}
#line 3743 "dhcp4_parser.cc"
    break;

  case 664: // $@101: %empty
#line 2337 "dhcp4_parser.yy"
                   {
    ctx.unique("hostname", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3752 "dhcp4_parser.cc"
    break;

  case 665: // hostname: "hostname" $@101 ":" "constant string"
#line 2340 "dhcp4_parser.yy"
               {
    ElementPtr host(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hostname", host);
    ctx.leave();
}
#line 3762 "dhcp4_parser.cc"
    break;

  case 666: // $@102: %empty
#line 2346 "dhcp4_parser.yy"
                                           {
    ctx.unique("client-classes", ctx.loc2pos(yystack_[0].location));
    ElementPtr c(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("client-classes", c);
    ctx.stack_.push_back(c);
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3774 "dhcp4_parser.cc"
    break;

  case 667: // reservation_client_classes: "client-classes" $@102 ":" list_strings
#line 2352 "dhcp4_parser.yy"
                     {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 3783 "dhcp4_parser.cc"
    break;

  case 668: // $@103: %empty
#line 2360 "dhcp4_parser.yy"
             {
    ctx.unique("relay", ctx.loc2pos(yystack_[0].location));
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("relay", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.RELAY);
}
#line 3795 "dhcp4_parser.cc"
    break;

  case 669: // relay: "relay" $@103 ":" "{" relay_map "}"
#line 2366 "dhcp4_parser.yy"
                                                {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 3804 "dhcp4_parser.cc"
    break;

  case 672: // $@104: %empty
#line 2378 "dhcp4_parser.yy"
                               {
    ctx.unique("client-classes", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("client-classes", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.CLIENT_CLASSES);
}
#line 3816 "dhcp4_parser.cc"
    break;

  case 673: // client_classes: "client-classes" $@104 ":" "[" client_classes_list "]"
#line 2384 "dhcp4_parser.yy"
                                                            {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 3825 "dhcp4_parser.cc"
    break;

  case 676: // client_classes_list: client_classes_list ","
#line 2391 "dhcp4_parser.yy"
                                               {
                       ctx.warnAboutExtraCommas(yystack_[0].location);
                       }
#line 3833 "dhcp4_parser.cc"
    break;

  case 677: // $@105: %empty
#line 2396 "dhcp4_parser.yy"
                                   {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 3843 "dhcp4_parser.cc"
    break;

  case 678: // client_class_entry: "{" $@105 client_class_params "}"
#line 2400 "dhcp4_parser.yy"
                                     {
    // The name client class parameter is required.
    ctx.require("name", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
}
#line 3853 "dhcp4_parser.cc"
    break;

  case 683: // not_empty_client_class_params: not_empty_client_class_params ","
#line 2412 "dhcp4_parser.yy"
                                          {
        ctx.warnAboutExtraCommas(yystack_[0].location);
        }
#line 3861 "dhcp4_parser.cc"
    break;

  case 701: // $@106: %empty
#line 2437 "dhcp4_parser.yy"
                        {
    ctx.unique("test", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3870 "dhcp4_parser.cc"
    break;

  case 702: // client_class_test: "test" $@106 ":" "constant string"
#line 2440 "dhcp4_parser.yy"
               {
    ElementPtr test(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("test", test);
    ctx.leave();
}
#line 3880 "dhcp4_parser.cc"
    break;

  case 703: // $@107: %empty
#line 2446 "dhcp4_parser.yy"
                                          {
    ctx.unique("template-test", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3889 "dhcp4_parser.cc"
    break;

  case 704: // client_class_template_test: "template-test" $@107 ":" "constant string"
#line 2449 "dhcp4_parser.yy"
               {
    ElementPtr template_test(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("template-test", template_test);
    ctx.leave();
}
#line 3899 "dhcp4_parser.cc"
    break;

  case 705: // only_if_required: "only-if-required" ":" "boolean"
#line 2455 "dhcp4_parser.yy"
                                                 {
    ctx.unique("only-if-required", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("only-if-required", b);
}
#line 3909 "dhcp4_parser.cc"
    break;

  case 706: // dhcp4o6_port: "dhcp4o6-port" ":" "integer"
#line 2463 "dhcp4_parser.yy"
                                         {
    ctx.unique("dhcp4o6-port", ctx.loc2pos(yystack_[2].location));
    ElementPtr time(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("dhcp4o6-port", time);
}
#line 3919 "dhcp4_parser.cc"
    break;

  case 707: // $@108: %empty
#line 2471 "dhcp4_parser.yy"
                               {
    ctx.unique("control-socket", ctx.loc2pos(yystack_[0].location));
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("control-socket", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.CONTROL_SOCKET);
}
#line 3931 "dhcp4_parser.cc"
    break;

  case 708: // control_socket: "control-socket" $@108 ":" "{" control_socket_params "}"
#line 2477 "dhcp4_parser.yy"
                                                            {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 3940 "dhcp4_parser.cc"
    break;

  case 711: // control_socket_params: control_socket_params ","
#line 2484 "dhcp4_parser.yy"
                                                   {
                          ctx.warnAboutExtraCommas(yystack_[0].location);
                          }
#line 3948 "dhcp4_parser.cc"
    break;

  case 717: // $@109: %empty
#line 2496 "dhcp4_parser.yy"
                                 {
    ctx.unique("socket-type", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3957 "dhcp4_parser.cc"
    break;

  case 718: // control_socket_type: "socket-type" $@109 ":" "constant string"
#line 2499 "dhcp4_parser.yy"
               {
    ElementPtr stype(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("socket-type", stype);
    ctx.leave();
}
#line 3967 "dhcp4_parser.cc"
    break;

  case 719: // $@110: %empty
#line 2505 "dhcp4_parser.yy"
                                 {
    ctx.unique("socket-name", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3976 "dhcp4_parser.cc"
    break;

  case 720: // control_socket_name: "socket-name" $@110 ":" "constant string"
#line 2508 "dhcp4_parser.yy"
               {
    ElementPtr name(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("socket-name", name);
    ctx.leave();
}
#line 3986 "dhcp4_parser.cc"
    break;

  case 721: // $@111: %empty
#line 2517 "dhcp4_parser.yy"
                                       {
    ctx.unique("dhcp-queue-control", ctx.loc2pos(yystack_[0].location));
    ElementPtr qc(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("dhcp-queue-control", qc);
    ctx.stack_.push_back(qc);
    ctx.enter(ctx.DHCP_QUEUE_CONTROL);
}
#line 3998 "dhcp4_parser.cc"
    break;

  case 722: // dhcp_queue_control: "dhcp-queue-control" $@111 ":" "{" queue_control_params "}"
#line 2523 "dhcp4_parser.yy"
                                                           {
    // The enable queue parameter is required.
    ctx.require("enable-queue", ctx.loc2pos(yystack_[2].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 4009 "dhcp4_parser.cc"
    break;

  case 725: // queue_control_params: queue_control_params ","
#line 2532 "dhcp4_parser.yy"
                                                 {
                        ctx.warnAboutExtraCommas(yystack_[0].location);
                        }
#line 4017 "dhcp4_parser.cc"
    break;

  case 732: // enable_queue: "enable-queue" ":" "boolean"
#line 2545 "dhcp4_parser.yy"
                                         {
    ctx.unique("enable-queue", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("enable-queue", b);
}
#line 4027 "dhcp4_parser.cc"
    break;

  case 733: // $@112: %empty
#line 2551 "dhcp4_parser.yy"
                       {
    ctx.unique("queue-type", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 4036 "dhcp4_parser.cc"
    break;

  case 734: // queue_type: "queue-type" $@112 ":" "constant string"
#line 2554 "dhcp4_parser.yy"
               {
    ElementPtr qt(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("queue-type", qt);
    ctx.leave();
}
#line 4046 "dhcp4_parser.cc"
    break;

  case 735: // capacity: "capacity" ":" "integer"
#line 2560 "dhcp4_parser.yy"
                                 {
    ctx.unique("capacity", ctx.loc2pos(yystack_[2].location));
    ElementPtr c(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("capacity", c);
}
#line 4056 "dhcp4_parser.cc"
    break;

  case 736: // $@113: %empty
#line 2566 "dhcp4_parser.yy"
                            {
    ctx.unique(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 4065 "dhcp4_parser.cc"
    break;

  case 737: // arbitrary_map_entry: "constant string" $@113 ":" value
#line 2569 "dhcp4_parser.yy"
              {
    ctx.stack_.back()->set(yystack_[3].value.as < std::string > (), yystack_[0].value.as < ElementPtr > ());
    ctx.leave();
}
#line 4074 "dhcp4_parser.cc"
    break;

  case 738: // $@114: %empty
#line 2576 "dhcp4_parser.yy"
                     {
    ctx.unique("dhcp-ddns", ctx.loc2pos(yystack_[0].location));
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("dhcp-ddns", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.DHCP_DDNS);
}
#line 4086 "dhcp4_parser.cc"
    break;

  case 739: // dhcp_ddns: "dhcp-ddns" $@114 ":" "{" dhcp_ddns_params "}"
#line 2582 "dhcp4_parser.yy"
                                                       {
    // The enable updates DHCP DDNS parameter is required.
    ctx.require("enable-updates", ctx.loc2pos(yystack_[2].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 4097 "dhcp4_parser.cc"
    break;

  case 740: // $@115: %empty
#line 2589 "dhcp4_parser.yy"
                              {
    // Parse the dhcp-ddns map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 4107 "dhcp4_parser.cc"
    break;

  case 741: // sub_dhcp_ddns: "{" $@115 dhcp_ddns_params "}"
#line 2593 "dhcp4_parser.yy"
                                  {
    // The enable updates DHCP DDNS parameter is required.
    ctx.require("enable-updates", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    // parsing completed
}
#line 4117 "dhcp4_parser.cc"
    break;

  case 744: // dhcp_ddns_params: dhcp_ddns_params ","
#line 2601 "dhcp4_parser.yy"
                                         {
                    ctx.warnAboutExtraCommas(yystack_[0].location);
                    }
#line 4125 "dhcp4_parser.cc"
    break;

  case 763: // enable_updates: "enable-updates" ":" "boolean"
#line 2626 "dhcp4_parser.yy"
                                             {
    ctx.unique("enable-updates", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("enable-updates", b);
}
#line 4135 "dhcp4_parser.cc"
    break;

  case 764: // $@116: %empty
#line 2632 "dhcp4_parser.yy"
                     {
    ctx.unique("server-ip", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 4144 "dhcp4_parser.cc"
    break;

  case 765: // server_ip: "server-ip" $@116 ":" "constant string"
#line 2635 "dhcp4_parser.yy"
               {
    ElementPtr s(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("server-ip", s);
    ctx.leave();
}
#line 4154 "dhcp4_parser.cc"
    break;

  case 766: // server_port: "server-port" ":" "integer"
#line 2641 "dhcp4_parser.yy"
                                       {
    ctx.unique("server-port", ctx.loc2pos(yystack_[2].location));
    ElementPtr i(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("server-port", i);
}
#line 4164 "dhcp4_parser.cc"
    break;

  case 767: // $@117: %empty
#line 2647 "dhcp4_parser.yy"
                     {
    ctx.unique("sender-ip", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 4173 "dhcp4_parser.cc"
    break;

  case 768: // sender_ip: "sender-ip" $@117 ":" "constant string"
#line 2650 "dhcp4_parser.yy"
               {
    ElementPtr s(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("sender-ip", s);
    ctx.leave();
}
#line 4183 "dhcp4_parser.cc"
    break;

  case 769: // sender_port: "sender-port" ":" "integer"
#line 2656 "dhcp4_parser.yy"
                                       {
    ctx.unique("sender-port", ctx.loc2pos(yystack_[2].location));
    ElementPtr i(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("sender-port", i);
}
#line 4193 "dhcp4_parser.cc"
    break;

  case 770: // max_queue_size: "max-queue-size" ":" "integer"
#line 2662 "dhcp4_parser.yy"
                                             {
    ctx.unique("max-queue-size", ctx.loc2pos(yystack_[2].location));
    ElementPtr i(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("max-queue-size", i);
}
#line 4203 "dhcp4_parser.cc"
    break;

  case 771: // $@118: %empty
#line 2668 "dhcp4_parser.yy"
                           {
    ctx.unique("ncr-protocol", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NCR_PROTOCOL);
}
#line 4212 "dhcp4_parser.cc"
    break;

  case 772: // ncr_protocol: "ncr-protocol" $@118 ":" ncr_protocol_value
#line 2671 "dhcp4_parser.yy"
                           {
    ctx.stack_.back()->set("ncr-protocol", yystack_[0].value.as < ElementPtr > ());
    ctx.leave();
}
#line 4221 "dhcp4_parser.cc"
    break;

  case 773: // ncr_protocol_value: "udp"
#line 2677 "dhcp4_parser.yy"
        { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("UDP", ctx.loc2pos(yystack_[0].location))); }
#line 4227 "dhcp4_parser.cc"
    break;

  case 774: // ncr_protocol_value: "tcp"
#line 2678 "dhcp4_parser.yy"
        { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("TCP", ctx.loc2pos(yystack_[0].location))); }
#line 4233 "dhcp4_parser.cc"
    break;

  case 775: // $@119: %empty
#line 2681 "dhcp4_parser.yy"
                       {
    ctx.unique("ncr-format", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NCR_FORMAT);
}
#line 4242 "dhcp4_parser.cc"
    break;

  case 776: // ncr_format: "ncr-format" $@119 ":" "JSON"
#line 2684 "dhcp4_parser.yy"
             {
    ElementPtr json(new StringElement("JSON", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ncr-format", json);
    ctx.leave();
}
#line 4252 "dhcp4_parser.cc"
    break;

  case 777: // $@120: %empty
#line 2691 "dhcp4_parser.yy"
                                         {
    ctx.unique("qualifying-suffix", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 4261 "dhcp4_parser.cc"
    break;

  case 778: // dep_qualifying_suffix: "qualifying-suffix" $@120 ":" "constant string"
#line 2694 "dhcp4_parser.yy"
               {
    ElementPtr s(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("qualifying-suffix", s);
    ctx.leave();
}
#line 4271 "dhcp4_parser.cc"
    break;

  case 779: // dep_override_no_update: "override-no-update" ":" "boolean"
#line 2701 "dhcp4_parser.yy"
                                                         {
    ctx.unique("override-no-update", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("override-no-update", b);
}
#line 4281 "dhcp4_parser.cc"
    break;

  case 780: // dep_override_client_update: "override-client-update" ":" "boolean"
#line 2708 "dhcp4_parser.yy"
                                                                 {
    ctx.unique("override-client-update", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("override-client-update", b);
}
#line 4291 "dhcp4_parser.cc"
    break;

  case 781: // $@121: %empty
#line 2715 "dhcp4_parser.yy"
                                             {
    ctx.unique("replace-client-name", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.REPLACE_CLIENT_NAME);
}
#line 4300 "dhcp4_parser.cc"
    break;

  case 782: // dep_replace_client_name: "replace-client-name" $@121 ":" ddns_replace_client_name_value
#line 2718 "dhcp4_parser.yy"
                                       {
    ctx.stack_.back()->set("replace-client-name", yystack_[0].value.as < ElementPtr > ());
    ctx.leave();
}
#line 4309 "dhcp4_parser.cc"
    break;

  case 783: // $@122: %empty
#line 2724 "dhcp4_parser.yy"
                                       {
    ctx.unique("generated-prefix", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 4318 "dhcp4_parser.cc"
    break;

  case 784: // dep_generated_prefix: "generated-prefix" $@122 ":" "constant string"
#line 2727 "dhcp4_parser.yy"
               {
    ElementPtr s(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("generated-prefix", s);
    ctx.leave();
}
#line 4328 "dhcp4_parser.cc"
    break;

  case 785: // $@123: %empty
#line 2734 "dhcp4_parser.yy"
                                         {
    ctx.unique("hostname-char-set", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 4337 "dhcp4_parser.cc"
    break;

  case 786: // dep_hostname_char_set: "hostname-char-set" $@123 ":" "constant string"
#line 2737 "dhcp4_parser.yy"
               {
    ElementPtr s(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hostname-char-set", s);
    ctx.leave();
}
#line 4347 "dhcp4_parser.cc"
    break;

  case 787: // $@124: %empty
#line 2744 "dhcp4_parser.yy"
                                                         {
    ctx.unique("hostname-char-replacement", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 4356 "dhcp4_parser.cc"
    break;

  case 788: // dep_hostname_char_replacement: "hostname-char-replacement" $@124 ":" "constant string"
#line 2747 "dhcp4_parser.yy"
               {
    ElementPtr s(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hostname-char-replacement", s);
    ctx.leave();
}
#line 4366 "dhcp4_parser.cc"
    break;

  case 789: // $@125: %empty
#line 2756 "dhcp4_parser.yy"
                               {
    ctx.unique("config-control", ctx.loc2pos(yystack_[0].location));
    ElementPtr i(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("config-control", i);
    ctx.stack_.push_back(i);
    ctx.enter(ctx.CONFIG_CONTROL);
}
#line 4378 "dhcp4_parser.cc"
    break;

  case 790: // config_control: "config-control" $@125 ":" "{" config_control_params "}"
#line 2762 "dhcp4_parser.yy"
                                                            {
    // No config control params are required
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 4388 "dhcp4_parser.cc"
    break;

  case 791: // $@126: %empty
#line 2768 "dhcp4_parser.yy"
                                   {
    // Parse the config-control map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 4398 "dhcp4_parser.cc"
    break;

  case 792: // sub_config_control: "{" $@126 config_control_params "}"
#line 2772 "dhcp4_parser.yy"
                                       {
    // No config_control params are required
    // parsing completed
}
#line 4407 "dhcp4_parser.cc"
    break;

  case 795: // config_control_params: config_control_params ","
#line 2780 "dhcp4_parser.yy"
                                                   {
                         ctx.warnAboutExtraCommas(yystack_[0].location);
                         }
#line 4415 "dhcp4_parser.cc"
    break;

  case 798: // $@127: %empty
#line 2790 "dhcp4_parser.yy"
                                   {
    ctx.unique("config-databases", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("config-databases", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.CONFIG_DATABASE);
}
#line 4427 "dhcp4_parser.cc"
    break;

  case 799: // config_databases: "config-databases" $@127 ":" "[" database_list "]"
#line 2796 "dhcp4_parser.yy"
                                                      {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 4436 "dhcp4_parser.cc"
    break;

  case 800: // config_fetch_wait_time: "config-fetch-wait-time" ":" "integer"
#line 2801 "dhcp4_parser.yy"
                                                             {
    ctx.unique("config-fetch-wait-time", ctx.loc2pos(yystack_[2].location));
    ElementPtr value(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("config-fetch-wait-time", value);
}
#line 4446 "dhcp4_parser.cc"
    break;

  case 801: // $@128: %empty
#line 2809 "dhcp4_parser.yy"
                 {
    ctx.unique("loggers", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("loggers", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.LOGGERS);
}
#line 4458 "dhcp4_parser.cc"
    break;

  case 802: // loggers: "loggers" $@128 ":" "[" loggers_entries "]"
#line 2815 "dhcp4_parser.yy"
                                                         {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 4467 "dhcp4_parser.cc"
    break;

  case 805: // loggers_entries: loggers_entries ","
#line 2824 "dhcp4_parser.yy"
                                       {
                   ctx.warnAboutExtraCommas(yystack_[0].location);
                   }
#line 4475 "dhcp4_parser.cc"
    break;

  case 806: // $@129: %empty
#line 2830 "dhcp4_parser.yy"
                             {
    ElementPtr l(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(l);
    ctx.stack_.push_back(l);
}
#line 4485 "dhcp4_parser.cc"
    break;

  case 807: // logger_entry: "{" $@129 logger_params "}"
#line 2834 "dhcp4_parser.yy"
                               {
    ctx.stack_.pop_back();
}
#line 4493 "dhcp4_parser.cc"
    break;

  case 810: // logger_params: logger_params ","
#line 2840 "dhcp4_parser.yy"
                                   {
                 ctx.warnAboutExtraCommas(yystack_[0].location);
                 }
#line 4501 "dhcp4_parser.cc"
    break;

  case 818: // debuglevel: "debuglevel" ":" "integer"
#line 2854 "dhcp4_parser.yy"
                                     {
    ctx.unique("debuglevel", ctx.loc2pos(yystack_[2].location));
    ElementPtr dl(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("debuglevel", dl);
}
#line 4511 "dhcp4_parser.cc"
    break;

  case 819: // $@130: %empty
#line 2860 "dhcp4_parser.yy"
                   {
    ctx.unique("severity", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 4520 "dhcp4_parser.cc"
    break;

  case 820: // severity: "severity" $@130 ":" "constant string"
#line 2863 "dhcp4_parser.yy"
               {
    ElementPtr sev(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("severity", sev);
    ctx.leave();
}
#line 4530 "dhcp4_parser.cc"
    break;

  case 821: // $@131: %empty
#line 2869 "dhcp4_parser.yy"
                                    {
    ctx.unique("output_options", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("output_options", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.OUTPUT_OPTIONS);
}
#line 4542 "dhcp4_parser.cc"
    break;

  case 822: // output_options_list: "output_options" $@131 ":" "[" output_options_list_content "]"
#line 2875 "dhcp4_parser.yy"
                                                                    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 4551 "dhcp4_parser.cc"
    break;

  case 825: // output_options_list_content: output_options_list_content ","
#line 2882 "dhcp4_parser.yy"
                                                               {
                               ctx.warnAboutExtraCommas(yystack_[0].location);
                               }
#line 4559 "dhcp4_parser.cc"
    break;

  case 826: // $@132: %empty
#line 2887 "dhcp4_parser.yy"
                             {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 4569 "dhcp4_parser.cc"
    break;

  case 827: // output_entry: "{" $@132 output_params_list "}"
#line 2891 "dhcp4_parser.yy"
                                    {
    ctx.stack_.pop_back();
}
#line 4577 "dhcp4_parser.cc"
    break;

  case 830: // output_params_list: output_params_list ","
#line 2897 "dhcp4_parser.yy"
                                             {
                      ctx.warnAboutExtraCommas(yystack_[0].location);
                      }
#line 4585 "dhcp4_parser.cc"
    break;

  case 836: // $@133: %empty
#line 2909 "dhcp4_parser.yy"
               {
    ctx.unique("output", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 4594 "dhcp4_parser.cc"
    break;

  case 837: // output: "output" $@133 ":" "constant string"
#line 2912 "dhcp4_parser.yy"
               {
    ElementPtr sev(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("output", sev);
    ctx.leave();
}
#line 4604 "dhcp4_parser.cc"
    break;

  case 838: // flush: "flush" ":" "boolean"
#line 2918 "dhcp4_parser.yy"
                           {
    ctx.unique("flush", ctx.loc2pos(yystack_[2].location));
    ElementPtr flush(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("flush", flush);
}
#line 4614 "dhcp4_parser.cc"
    break;

  case 839: // maxsize: "maxsize" ":" "integer"
#line 2924 "dhcp4_parser.yy"
                               {
    ctx.unique("maxsize", ctx.loc2pos(yystack_[2].location));
    ElementPtr maxsize(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("maxsize", maxsize);
}
#line 4624 "dhcp4_parser.cc"
    break;

  case 840: // maxver: "maxver" ":" "integer"
#line 2930 "dhcp4_parser.yy"
                             {
    ctx.unique("maxver", ctx.loc2pos(yystack_[2].location));
    ElementPtr maxver(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("maxver", maxver);
}
#line 4634 "dhcp4_parser.cc"
    break;

  case 841: // $@134: %empty
#line 2936 "dhcp4_parser.yy"
                 {
    ctx.unique("pattern", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORD);
}
#line 4643 "dhcp4_parser.cc"
    break;

  case 842: // pattern: "pattern" $@134 ":" "constant string"
#line 2939 "dhcp4_parser.yy"
               {
    ElementPtr sev(new StringElement(yystack_[0].value.as < std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("pattern", sev);
    ctx.leave();
}
#line 4653 "dhcp4_parser.cc"
    break;

  case 843: // $@135: %empty
#line 2945 "dhcp4_parser.yy"
                             {
    ctx.unique("compatibility", ctx.loc2pos(yystack_[0].location));
    ElementPtr i(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("compatibility", i);
    ctx.stack_.push_back(i);
    ctx.enter(ctx.COMPATIBILITY);
}
#line 4665 "dhcp4_parser.cc"
    break;

  case 844: // compatibility: "compatibility" $@135 ":" "{" compatibility_params "}"
#line 2951 "dhcp4_parser.yy"
                                                           {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 4674 "dhcp4_parser.cc"
    break;

  case 847: // compatibility_params: compatibility_params ","
#line 2958 "dhcp4_parser.yy"
                                                 {
                        ctx.warnAboutExtraCommas(yystack_[0].location);
                        }
#line 4682 "dhcp4_parser.cc"
    break;

  case 853: // lenient_option_parsing: "lenient-option-parsing" ":" "boolean"
#line 2970 "dhcp4_parser.yy"
                                                             {
    ctx.unique("lenient-option-parsing", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("lenient-option-parsing", b);
}
#line 4692 "dhcp4_parser.cc"
    break;

  case 854: // ignore_dhcp_server_identifier: "ignore-dhcp-server-identifier" ":" "boolean"
#line 2976 "dhcp4_parser.yy"
                                                                   {
    ctx.unique("ignore-dhcp-server-identifier", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ignore-dhcp-server-identifier", b);
}
#line 4702 "dhcp4_parser.cc"
    break;

  case 855: // ignore_rai_link_selection: "ignore-rai-link-selection" ":" "boolean"
#line 2982 "dhcp4_parser.yy"
                                                             {
    ctx.unique("ignore-rai-link-selection", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ignore-rai-link-selection", b);
}
#line 4712 "dhcp4_parser.cc"
    break;

  case 856: // exclude_first_last_24: "exclude-first-last-24" ":" "boolean"
#line 2988 "dhcp4_parser.yy"
                                                           {
    ctx.unique("exclude-first-last-24", ctx.loc2pos(yystack_[2].location));
    ElementPtr b(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("exclude-first-last-24", b);
}
#line 4722 "dhcp4_parser.cc"
    break;


#line 4726 "dhcp4_parser.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Dhcp4Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Dhcp4Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  Dhcp4Parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // Dhcp4Parser::context.
  Dhcp4Parser::context::context (const Dhcp4Parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  Dhcp4Parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  Dhcp4Parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  Dhcp4Parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short Dhcp4Parser::yypact_ninf_ = -1012;

  const signed char Dhcp4Parser::yytable_ninf_ = -1;

  const short
  Dhcp4Parser::yypact_[] =
  {
     634, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012,    35,    31,    44,    64,    92,   137,
     190,   228,   238,   254,   264,   276,   277,   280, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012,    31,  -182,   180,   145,    88,   333,
     163,   533,   -48,    90,   -20,   -89,   644,    43, -1012,   285,
      59,   147,   298,   318, -1012,    23, -1012, -1012, -1012, -1012,
     337,   355,   357, -1012, -1012, -1012,   365, -1012, -1012, -1012,
     366,   375,   376,   383,   384,   385,   386,   387,   388,   389,
     390, -1012,   391,   392,   393,   404,   405, -1012, -1012, -1012,
     406,   407,   418,   422, -1012, -1012, -1012,   424, -1012, -1012,
   -1012, -1012, -1012,   425,   426,   427, -1012, -1012, -1012, -1012,
   -1012,   428, -1012, -1012, -1012, -1012, -1012, -1012,   429,   430,
     431, -1012, -1012,   432, -1012,    49, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012,   433,   434,   438,   446,
   -1012,    62, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,   447, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,    65,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012,   448, -1012, -1012, -1012, -1012,    85, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012,   315,   334, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
     449, -1012, -1012,   452, -1012, -1012, -1012,   455, -1012, -1012,
     458,   460, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012,   464,   465,   466, -1012, -1012,
   -1012, -1012,   467,   470, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012,    86, -1012, -1012,
   -1012,   473, -1012, -1012,   474, -1012,   476,   477, -1012, -1012,
     478,   480, -1012, -1012, -1012, -1012, -1012, -1012, -1012,    89,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012,   481,    93, -1012,
   -1012, -1012, -1012,    31,    31, -1012,   272,   484, -1012, -1012,
     485,   486,   489,   279,   281,   289,   491,   496,   499,   295,
     509,   515,   517,   307,   308,   309,   310,   311,   314,   317,
     320,   321,   313,   323,   531,   324,   329,   332,   335,   340,
     532,   543,   547,   341,   342,   338,   344,   548,   559,   560,
     352,   566,   567,   571,   573,   578,   367,   369,   370,   586,
     588,   589,   591,   596,   397,   599,   601,   602,   603,   604,
     610,   371,   398,   399,   611,   614, -1012,   145, -1012,   615,
     616,   617,   408,   435,   409,   436,    88, -1012,   618,   619,
     638,   646,   647,   650,   439,   655,   656,   657,   333, -1012,
     658,   457,   163, -1012,   660,   667,   670,   671,   673,   674,
     675,   683, -1012,   533, -1012,   684,   685,   475,   686,   688,
     691,   479, -1012,    90,   694,   483,   487,   488, -1012,   -20,
     695,   698,    30, -1012,   490,   699,   700,   493,   703,   494,
     510,   723,   724,   511,   523,   738,   739,   740,   743,   644,
   -1012,   745,   540,    43, -1012, -1012, -1012,   754,   752,   753,
     756,   758, -1012, -1012, -1012,   552,   553,   554, -1012,   763,
     767,   770, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012,   562, -1012, -1012, -1012, -1012, -1012,   -16,
     563,   564, -1012, -1012, -1012, -1012,   776,   777,   778, -1012,
     569,   780,   779,   572,    87, -1012, -1012, -1012,   783,   785,
     786,   787,   788, -1012,   789,   790,   791,   792,   579,   587,
   -1012, -1012, -1012,   796,   797, -1012,   798,    98,   141, -1012,
   -1012, -1012, -1012, -1012,   590,   592,   606,   801,   607,   608,
   -1012,   798,   609,   818, -1012,   635, -1012, -1012,   798,   636,
     637,   639,   640,   641,   642,   643, -1012,   645,   648, -1012,
     649,   652,   653, -1012, -1012,   654, -1012, -1012, -1012, -1012,
     659,   779, -1012, -1012,   661,   662, -1012,   663, -1012, -1012,
      19,   687, -1012, -1012,   -16,   664,   665,   666, -1012,   821,
   -1012, -1012,    31,   145,    43,    88,   236, -1012, -1012, -1012,
     585,   585,   822, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012,   823,   824,   825, -1012,   826, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012,   217,   842,   846,   854,   198,
     171,   -24,    47,   644, -1012, -1012,   855,   -31, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,   863,
   -1012, -1012, -1012, -1012,   159, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012,   822, -1012,   115,   138,   144, -1012, -1012,
     152, -1012, -1012, -1012, -1012, -1012, -1012, -1012,   867,   868,
     869,   871,   878,   879,   880,   881,   882,   883, -1012,   884,
   -1012, -1012, -1012, -1012, -1012,   233, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,   234, -1012,
     885,   886, -1012, -1012,   887,   889, -1012, -1012,   888,   892,
   -1012, -1012,   890,   894, -1012, -1012,   893,   895, -1012, -1012,
   -1012, -1012, -1012, -1012,    38, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012,   102, -1012, -1012,   896,   897, -1012, -1012,   898,
     900, -1012,   901,   902,   903,   904,   905,   906,   250, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012,   907,   908,   909, -1012,
     251, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012,   267, -1012, -1012, -1012,   910, -1012,   911, -1012,
   -1012, -1012,   282, -1012, -1012, -1012, -1012, -1012,   292, -1012,
     103, -1012,   912,   913,   914,   915, -1012,   299, -1012, -1012,
   -1012, -1012, -1012,   705, -1012,   916,   918, -1012, -1012, -1012,
   -1012,   917,   920, -1012, -1012, -1012,   919,   923,   236, -1012,
     924,   925,   926,   927,   577,   672,   708,   714,   717,   718,
     719,   720,   721,   722,   935,   725,   936,   938,   939,   940,
     585, -1012, -1012,   585, -1012,   822,   333, -1012,   823,    90,
   -1012,   824,   -20, -1012,   825,   651, -1012,   826,   217, -1012,
     245,   842, -1012,   533, -1012,   846,   -89, -1012,   854,   729,
     730,   731,   732,   734,   735,   198, -1012,   736,   737,   741,
     171, -1012,   952,   957,   -24, -1012,   746,   961,   759,   968,
      47, -1012, -1012,   -61,   855, -1012,   764,   773,   774,   795,
     -31, -1012, -1012,   988,   994,   163, -1012,   863,  1013, -1012,
   -1012,   806,   807, -1012,   294,   809,   812,   816, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,   326, -1012,
     819,   820,   832,   833, -1012,   306, -1012,   312, -1012,  1029,
   -1012,  1047, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,   319,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012,  1053, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012,  1052,  1099, -1012, -1012,
   -1012, -1012, -1012, -1012,  1097, -1012,   350, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,   899,
     921, -1012, -1012,   922, -1012,    31, -1012, -1012,  1103, -1012,
   -1012, -1012, -1012, -1012,   354, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012,   928,   373, -1012,   798,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012,   651, -1012,  1104,  1106,   929, -1012,   245, -1012, -1012,
   -1012, -1012, -1012, -1012,  1107,   932,  1108,   -61, -1012, -1012,
   -1012, -1012, -1012,   930,   934, -1012, -1012,  1110, -1012,   937,
   -1012, -1012, -1012,  1109, -1012, -1012,   183, -1012,   123,  1109,
   -1012, -1012,  1113,  1114,  1115, -1012,   374, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012,  1116,   933,   941,   942,  1117,   123,
   -1012,   944, -1012, -1012, -1012,   945, -1012, -1012, -1012
  };

  const short
  Dhcp4Parser::yydefact_[] =
  {
       0,     2,     4,     6,     8,    10,    12,    14,    16,    18,
      20,    22,    24,    26,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     1,    44,
      36,    32,    31,    28,    29,    30,    35,     3,    33,    34,
      59,     5,    65,     7,   199,     9,   377,    11,   593,    13,
     622,    15,   511,    17,   520,    19,   559,    21,   339,    23,
     740,    25,   791,    27,    46,    39,     0,     0,     0,     0,
       0,   624,     0,   522,   561,     0,     0,     0,    48,     0,
      47,     0,     0,    40,    61,     0,    63,   789,   184,   217,
       0,     0,     0,   644,   646,   648,     0,   215,   228,   230,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   148,     0,     0,     0,     0,     0,   159,   166,   168,
       0,     0,     0,     0,   368,   509,   550,     0,   151,   454,
     609,   611,   447,     0,     0,     0,   301,   672,   613,   330,
     351,     0,   316,   707,   721,   738,   173,   175,     0,     0,
       0,   801,   843,     0,   136,     0,    67,    70,    71,    72,
      73,    74,   108,   109,   110,   111,   112,    75,   103,   133,
     134,    92,    93,    94,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   114,   115,   125,   126,   127,   129,   130,
     131,   135,    78,    79,   100,    80,    81,    82,   128,    86,
      87,    76,   105,   106,   107,   104,    77,    84,    85,    98,
      99,   101,    95,    96,    97,    83,    88,    89,    90,    91,
     102,   113,   132,   201,   203,   207,     0,     0,     0,     0,
     198,     0,   186,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   432,   434,   436,   584,   430,   438,     0,   442,
     440,   668,   429,   382,   383,   384,   385,   386,   410,   411,
     412,   413,   414,   427,   400,   401,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   424,   425,   426,   428,     0,
     379,   389,   405,   406,   407,   390,   392,   393,   396,   397,
     398,   395,   391,   387,   388,   408,   409,   394,   402,   403,
     404,   399,   606,     0,   605,   601,   602,   600,     0,   595,
     598,   599,   603,   604,   666,   654,   656,   660,   658,   664,
     662,   650,   643,   637,   641,   642,     0,   625,   626,   638,
     639,   640,   634,   629,   635,   631,   632,   633,   636,   630,
       0,   540,   276,     0,   544,   542,   547,     0,   536,   537,
       0,   523,   524,   527,   539,   528,   529,   530,   546,   531,
     532,   533,   534,   535,   577,     0,     0,     0,   575,   576,
     579,   580,     0,   562,   563,   566,   567,   568,   569,   570,
     571,   572,   573,   574,   347,   349,   344,     0,   341,   345,
     346,     0,   777,   764,     0,   767,     0,     0,   771,   775,
       0,     0,   781,   783,   785,   787,   762,   760,   761,     0,
     742,   745,   746,   747,   748,   749,   750,   751,   752,   757,
     753,   754,   755,   756,   758,   759,   798,     0,     0,   793,
     796,   797,    45,    50,     0,    37,    43,     0,    64,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    69,    66,     0,
       0,     0,     0,     0,     0,     0,   188,   200,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,   378,
       0,     0,   597,   594,     0,     0,     0,     0,     0,     0,
       0,     0,   623,   628,   512,     0,     0,     0,     0,     0,
       0,     0,   521,   526,     0,     0,     0,     0,   560,   565,
       0,     0,   343,   340,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   744,
     741,     0,     0,   795,   792,    49,    41,     0,     0,     0,
       0,     0,   153,   154,   155,     0,     0,     0,   183,     0,
       0,     0,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,     0,   178,   179,   156,   157,   158,     0,
       0,     0,   170,   171,   172,   177,     0,     0,     0,   150,
       0,     0,     0,     0,     0,   444,   445,   446,     0,     0,
       0,     0,     0,   706,     0,     0,     0,     0,     0,     0,
     180,   181,   182,     0,     0,    68,     0,     0,     0,   211,
     212,   213,   214,   187,     0,     0,     0,     0,     0,     0,
     453,     0,     0,     0,   380,     0,   608,   596,     0,     0,
       0,     0,     0,     0,     0,     0,   627,     0,     0,   538,
       0,     0,     0,   549,   525,     0,   581,   582,   583,   564,
       0,     0,   342,   763,     0,     0,   766,     0,   769,   770,
       0,     0,   779,   780,     0,     0,     0,     0,   743,     0,
     800,   794,     0,     0,     0,     0,     0,   645,   647,   649,
       0,     0,   232,   149,   161,   162,   163,   164,   165,   160,
     167,   169,   370,   513,   552,   152,   456,    38,   610,   612,
     449,   450,   451,   452,   448,     0,     0,   615,   332,     0,
       0,     0,     0,     0,   174,   176,     0,     0,    51,   202,
     205,   206,   204,   209,   210,   208,   433,   435,   437,   586,
     431,   439,   443,   441,     0,   607,   667,   655,   657,   661,
     659,   665,   663,   651,   541,   277,   545,   543,   548,   578,
     348,   350,   778,   765,   768,   773,   774,   772,   776,   782,
     784,   786,   788,   232,    42,     0,     0,     0,   224,   226,
       0,   219,   222,   223,   264,   269,   271,   273,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   287,     0,
     293,   295,   297,   299,   263,     0,   239,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,     0,   237,
       0,   233,   234,   375,     0,   371,   372,   518,     0,   514,
     515,   557,     0,   553,   554,   461,     0,   457,   458,   311,
     312,   313,   314,   315,     0,   303,   306,   307,   308,   309,
     310,   677,     0,   674,   620,     0,   616,   617,   337,     0,
     333,   334,     0,     0,     0,     0,     0,     0,     0,   353,
     356,   357,   358,   359,   360,   361,     0,     0,     0,   326,
       0,   318,   321,   322,   323,   324,   325,   717,   719,   716,
     714,   715,     0,   709,   712,   713,     0,   733,     0,   736,
     729,   730,     0,   723,   726,   727,   728,   731,     0,   806,
       0,   803,     0,     0,     0,     0,   852,     0,   845,   848,
     849,   850,   851,    53,   591,     0,   587,   588,   652,   670,
     671,     0,     0,    62,   790,   185,     0,     0,   221,   218,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     241,   216,   229,     0,   231,   236,     0,   369,   374,   522,
     510,   517,   561,   551,   556,     0,   455,   460,   305,   302,
     679,   676,   673,   624,   614,   619,     0,   331,   336,     0,
       0,     0,     0,     0,     0,   355,   352,     0,     0,     0,
     320,   317,     0,     0,   711,   708,     0,     0,     0,     0,
     725,   722,   739,     0,   805,   802,     0,     0,     0,     0,
     847,   844,    55,     0,    54,     0,   585,   590,     0,   669,
     799,     0,     0,   220,     0,     0,     0,     0,   275,   278,
     279,   280,   281,   282,   283,   284,   285,   286,     0,   292,
       0,     0,     0,     0,   240,     0,   235,     0,   373,     0,
     516,     0,   555,   508,   484,   485,   486,   469,   470,   489,
     490,   491,   492,   493,   506,   472,   473,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   507,
     466,   467,   468,   482,   483,   479,   480,   481,   478,     0,
     463,   471,   487,   488,   474,   475,   476,   477,   459,   304,
     701,   703,     0,   695,   696,   697,   698,   699,   700,   688,
     689,   693,   694,   690,   691,   692,     0,   680,   681,   684,
     685,   686,   687,   675,     0,   618,     0,   335,   362,   363,
     364,   365,   366,   367,   354,   327,   328,   329,   319,     0,
       0,   710,   732,     0,   735,     0,   724,   821,     0,   819,
     817,   811,   815,   816,     0,   808,   813,   814,   812,   804,
     853,   854,   855,   856,   846,    52,    57,     0,   589,     0,
     225,   227,   266,   267,   268,   265,   270,   272,   274,   289,
     290,   291,   288,   294,   296,   298,   300,   238,   376,   519,
     558,   465,   462,     0,     0,     0,   678,   683,   621,   338,
     718,   720,   734,   737,     0,     0,     0,   810,   807,    56,
     592,   653,   464,     0,     0,   705,   682,     0,   818,     0,
     809,   702,   704,     0,   820,   826,     0,   823,     0,   825,
     822,   836,     0,     0,     0,   841,     0,   828,   831,   832,
     833,   834,   835,   824,     0,     0,     0,     0,     0,   830,
     827,     0,   838,   839,   840,     0,   829,   837,   842
  };

  const short
  Dhcp4Parser::yypgoto_[] =
  {
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012,   -59, -1012,  -565, -1012,   200,
   -1012, -1012, -1012, -1012, -1012, -1012,  -655, -1012, -1012, -1012,
     -67, -1012, -1012, -1012, -1012, -1012, -1012, -1012,   400,   620,
     -30,   -26,    26,   -54,   -37,   -27,     0,    14,    16,    45,
   -1012, -1012, -1012, -1012,    48, -1012, -1012,    50,    51,    52,
      53,    55,    56, -1012,   410,    58, -1012,    63, -1012,    66,
      68,    69,    70, -1012,    71, -1012,    73, -1012, -1012, -1012,
   -1012, -1012,    33, -1012, -1012,   401,   597, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012,   134, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012,   316, -1012,   110, -1012,  -719,
     118, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012,   -45, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012,   104, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012,    80, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012,    95, -1012, -1012, -1012,    99,   576,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012,    94, -1012, -1012,
   -1012, -1012, -1012, -1012, -1011, -1012, -1012, -1012,   113, -1012,
   -1012, -1012,   124,   613, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1008, -1012,    78, -1012,    79, -1012,    74,    76,
      81,    84, -1012, -1012, -1012, -1012, -1012, -1012, -1012,   107,
   -1012, -1012,  -109,   -62, -1012, -1012, -1012, -1012, -1012,   129,
   -1012, -1012, -1012,   125, -1012,   612, -1012,   -63, -1012, -1012,
   -1012, -1012, -1012,   -44, -1012, -1012, -1012, -1012, -1012,   -23,
   -1012, -1012, -1012,   131, -1012, -1012, -1012,   139, -1012,   605,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012,    91, -1012, -1012, -1012,    96,   624, -1012, -1012,
   -1012,   -49, -1012,   -10, -1012,   -56, -1012, -1012, -1012,   127,
   -1012, -1012, -1012,   130, -1012,   623,     3, -1012,    13, -1012,
      29, -1012,   394, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1007, -1012, -1012, -1012, -1012, -1012,   136, -1012, -1012, -1012,
     -88, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012,   116, -1012, -1012, -1012, -1012, -1012, -1012, -1012,   112,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
     411,   594, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012,   451,   593, -1012, -1012, -1012, -1012, -1012, -1012,   117,
   -1012, -1012,   -90, -1012, -1012, -1012, -1012, -1012, -1012,  -110,
   -1012, -1012,  -129, -1012, -1012, -1012, -1012, -1012, -1012, -1012,
   -1012, -1012, -1012,   119, -1012, -1012, -1012, -1012
  };

  const short
  Dhcp4Parser::yydefgoto_[] =
  {
       0,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    36,    37,    38,    65,   748,
      82,    83,    39,    64,    79,    80,   769,   973,  1073,  1074,
     844,    41,    66,    85,   437,    86,    43,    67,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   464,   169,   170,   481,   171,   172,   173,   174,
     175,   176,   177,   470,   739,   178,   471,   179,   472,   180,
     181,   182,   183,   499,   184,   500,   185,   186,   187,   188,
     189,   190,   191,   192,   441,   231,   232,    45,    68,   233,
     509,   234,   510,   772,   235,   511,   775,   236,   237,   238,
     239,   193,   450,   194,   442,   820,   821,   822,   986,   823,
     987,   195,   451,   196,   452,   870,   871,   872,  1013,   845,
     846,   847,   990,  1235,   848,   991,   849,   992,   850,   993,
     851,   852,   546,   853,   854,   855,   856,   857,   858,   859,
     860,   861,   862,  1004,  1242,   863,   864,  1006,   865,  1007,
     866,  1008,   867,  1009,   197,   489,   894,   895,   896,   897,
     898,   899,   900,   198,   495,   930,   931,   932,   933,   934,
     199,   492,   909,   910,   911,  1036,    59,    75,   387,   388,
     389,   560,   390,   561,   200,   493,   918,   919,   920,   921,
     922,   923,   924,   925,   201,   477,   874,   875,   876,  1016,
      47,    69,   279,   280,   281,   522,   282,   518,   283,   519,
     284,   520,   285,   523,   286,   526,   287,   525,   202,   203,
     204,   205,   485,   754,   292,   206,   482,   886,   887,   888,
    1025,  1149,  1150,   207,   478,    53,    72,   878,   879,   880,
    1019,    55,    73,   350,   351,   352,   353,   354,   355,   356,
     545,   357,   549,   358,   548,   359,   360,   550,   361,   208,
     479,   882,   883,   884,  1022,    57,    74,   372,   373,   374,
     375,   376,   554,   377,   378,   379,   380,   381,   294,   521,
     975,   976,   977,  1075,    49,    70,   308,   309,   310,   530,
     311,   209,   483,   210,   484,   211,   491,   905,   906,   907,
    1033,    51,    71,   326,   327,   328,   212,   446,   213,   447,
     214,   448,   332,   541,   980,  1078,   333,   535,   334,   536,
     335,   538,   336,   537,   337,   540,   338,   539,   339,   534,
     301,   527,   981,   215,   490,   902,   903,  1030,  1176,  1177,
    1178,  1179,  1180,  1253,  1181,  1254,  1182,   216,   217,   496,
     942,   943,   944,  1052,   945,  1053,   218,   497,   952,   953,
     954,   955,  1057,   956,   957,  1059,   219,   498,    61,    76,
     409,   410,   411,   412,   566,   413,   414,   568,   415,   416,
     417,   571,   807,   418,   572,   419,   565,   420,   421,   422,
     575,   423,   576,   424,   577,   425,   578,   220,   440,    63,
      77,   428,   429,   430,   581,   431,   221,   504,   960,   961,
    1063,  1214,  1215,  1216,  1217,  1266,  1218,  1264,  1286,  1287,
    1288,  1296,  1297,  1298,  1304,  1299,  1300,  1301,  1302,  1308,
     222,   505,   967,   968,   969,   970,   971,   972
  };

  const short
  Dhcp4Parser::yytable_[] =
  {
     154,   230,   252,   304,   322,    78,   348,   368,   386,   406,
     340,   370,   868,   297,  1141,   256,   782,  1142,  1157,   240,
     295,   312,   324,   786,   362,   382,   438,   407,   349,   369,
     371,   439,   257,    81,   342,    28,    29,   805,    30,   253,
      31,  1028,   258,   254,  1029,   125,   293,   307,   323,   130,
     131,    40,   507,   384,   385,   426,   427,   508,   241,   296,
     313,   325,   433,   363,   383,   516,   408,   747,   528,   259,
     517,    42,   298,   529,   329,   342,   364,   343,   344,   365,
     366,   367,   299,   260,   330,   261,   130,   131,   532,   562,
     130,   131,   579,   533,   563,   255,   583,   580,   300,    44,
     331,   584,   278,   223,   224,  1031,  1064,   225,  1032,  1065,
     226,   227,   228,   229,   262,   770,   771,   263,   507,   264,
     265,   266,   267,   983,   268,   269,   153,   270,  1207,   341,
    1208,  1209,   271,   937,   938,   272,   747,   273,   274,   275,
     276,   583,   277,   288,    46,   289,   984,   516,   305,   306,
     290,   434,   985,   291,   153,   988,    87,   130,   131,    88,
     989,   773,   774,   734,   735,   736,   737,   962,   963,   964,
     965,    89,   384,   385,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   153,   342,  1289,   343,   344,  1290,
      84,   153,   345,   346,   347,   153,   806,    48,   130,   131,
     130,   131,   738,   750,   751,   752,   753,   946,   947,   948,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    50,  1010,  1010,   125,   126,
    1141,  1011,  1012,  1142,  1157,    52,    32,    33,    34,    35,
     127,   128,   129,  1045,  1050,   130,   131,   126,  1046,  1051,
     132,    54,   949,   818,   819,   133,   134,   135,   136,   137,
    1054,    56,   302,   130,   131,  1055,   138,    93,    94,    95,
      96,   130,   131,    58,    60,  1060,   139,    62,   249,   140,
    1061,   432,   250,   303,  1105,   579,   141,   142,   321,   978,
    1062,   143,  1070,   153,   144,   153,   435,  1071,   145,  1010,
     100,   101,   102,  1291,  1247,   528,  1292,  1293,  1294,  1295,
    1248,   436,  1251,   542,   926,   927,   928,  1252,   146,   147,
     148,   149,   150,   151,  1232,  1233,  1234,   543,   125,   126,
     342,   443,   152,   912,   913,   914,   915,   916,   917,   889,
     890,   891,   892,   562,   893,   130,   131,  1267,  1259,   444,
     153,   445,  1268,    91,    92,    93,    94,    95,    96,   449,
     453,  1160,  1161,  1162,   585,   586,   532,  1309,   153,   454,
     455,  1270,  1310,  1239,  1240,  1241,   153,   456,   457,   458,
     459,   460,   461,   462,   463,   465,   466,   467,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   468,   469,
     473,   474,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   475,   242,   243,   244,   476,   126,   480,   486,
     487,   488,   494,   501,   502,   503,   506,   512,   513,   128,
     154,   245,   514,   130,   131,   246,   247,   248,   132,   230,
     515,   524,   531,   133,   134,   135,   547,   544,   249,   551,
     153,   252,   250,   553,   138,   304,   552,   240,   555,   556,
     557,   251,   297,   559,   256,   558,   322,   564,   567,   295,
     569,   570,   573,   312,   574,   582,   348,   587,   588,   589,
     590,   257,   368,   591,   324,   595,   370,   592,   253,   593,
     596,   258,   254,   597,   362,   293,   241,   594,   349,   307,
     382,   598,   406,   599,   369,   371,   146,   147,   296,   600,
     323,   601,   313,   602,   603,   604,   605,   606,   259,   611,
     407,   298,   607,   325,   608,   613,   619,   609,   610,   612,
     614,   299,   260,   363,   261,   615,   329,   620,   153,   383,
     616,   621,   626,   617,   255,   624,   330,   300,   618,   622,
     623,   278,   625,   627,   628,    93,    94,    95,   629,   408,
     630,   631,   331,   262,  1271,   632,   263,   633,   264,   265,
     266,   267,   634,   268,   269,   635,   270,   636,   637,   650,
     638,   271,   639,   640,   272,   641,   273,   274,   275,   276,
     642,   277,   288,   644,   289,   645,   646,   647,   648,   290,
     305,   306,   291,   643,   649,   653,   651,   652,   654,   656,
     657,   658,   664,   665,   824,   661,   659,   126,   825,   826,
     827,   828,   829,   830,   831,   832,   833,   834,   835,   836,
     837,   838,   666,   130,   131,   839,   840,   841,   842,   843,
     667,   668,   662,   660,   669,   670,   154,   314,   230,   671,
     672,   673,   675,   814,   678,   315,   316,   317,   318,   319,
     320,   679,   321,   676,   680,   681,   240,   682,   683,   684,
     342,    91,    92,    93,    94,    95,    96,   685,   687,   688,
     690,   689,   691,   929,   939,   692,   406,   693,   695,   700,
     966,   696,   701,   704,   705,   697,   698,   707,   703,   706,
     708,   935,   940,   950,   407,   241,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   709,   710,   711,   712,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   713,   714,   715,   716,   126,   342,   717,   153,   719,
     936,   941,   951,   408,   130,   131,   720,   128,   722,   723,
     724,   130,   131,   725,   247,   726,   132,   727,   728,   729,
     730,   133,   134,   135,   731,   732,   249,   733,   740,   741,
     250,   742,   743,   744,   745,   746,    30,   749,   755,   251,
     756,   757,   758,  1088,   764,   759,   760,   761,   762,   763,
     153,   766,   765,   768,   767,   776,   779,   777,   391,   392,
     393,   394,   395,   396,   397,   398,   399,   400,   401,   402,
     403,   778,   780,   781,   783,   784,   813,   404,   405,   869,
     873,   877,   881,   885,   146,   147,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,   901,
     785,   787,   788,   904,   789,   790,   791,   792,   793,   153,
     794,   908,   959,   795,   796,   808,   153,   797,   798,   799,
     974,   994,   995,   996,   800,   997,   802,   803,   804,   810,
     811,   812,   998,   999,  1000,  1001,  1002,  1003,  1005,  1015,
    1089,  1014,  1018,  1017,  1020,  1021,  1023,  1024,  1027,  1026,
    1035,   801,  1034,  1038,  1037,  1039,  1040,  1041,  1042,  1043,
    1044,  1047,  1048,  1049,  1056,  1058,  1066,  1067,  1068,  1069,
    1072,  1077,  1076,  1081,  1090,  1079,  1080,  1082,  1084,  1085,
    1086,  1087,  1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,
    1100,  1099,  1101,  1102,  1103,  1188,  1189,  1190,  1191,   252,
    1192,  1193,   348,  1196,  1195,   368,  1199,  1197,  1113,   370,
     297,  1200,   256,  1163,  1202,  1203,   322,   295,  1169,   386,
     362,  1117,  1205,   382,   349,  1204,  1152,   369,   371,   257,
    1140,  1171,  1220,   929,   324,  1168,   253,   939,  1118,   258,
     254,  1221,  1222,   293,  1225,  1114,  1210,  1226,  1119,  1115,
    1164,   935,  1151,   966,  1165,   940,   296,  1170,   304,   363,
     323,   950,   383,  1223,  1212,  1153,   259,  1229,  1211,   298,
    1172,  1230,  1231,   325,  1236,  1120,   312,  1237,  1154,   299,
     260,  1238,   261,  1173,  1243,  1244,   329,  1249,  1155,  1121,
     936,  1122,   255,  1174,   941,   300,   330,  1245,  1246,   278,
     951,  1116,   307,  1213,  1156,  1250,  1166,  1255,  1139,  1175,
    1256,   262,   331,  1167,   263,   313,   264,   265,   266,   267,
    1123,   268,   269,  1124,   270,  1125,  1126,  1127,  1128,   271,
    1129,  1130,   272,  1131,   273,   274,   275,   276,  1132,   277,
     288,  1133,   289,  1134,  1135,  1136,  1137,   290,  1138,  1145,
     291,  1146,  1257,  1143,  1144,  1258,  1147,  1265,  1273,  1148,
    1274,  1277,  1279,   663,  1260,  1283,  1285,  1305,  1306,  1307,
    1311,  1315,  1083,   815,   809,  1106,   817,   655,  1104,   982,
    1198,  1108,  1159,  1187,  1158,  1186,  1261,  1262,   702,  1194,
    1107,   674,  1272,  1269,  1109,  1281,  1263,  1275,  1278,  1282,
    1110,  1312,  1284,   305,   306,  1112,   677,  1313,  1314,  1317,
    1318,  1111,  1185,  1184,   699,   694,   686,  1183,  1228,  1276,
    1201,  1227,  1206,   718,   958,   816,   721,  1280,   979,  1303,
    1316,  1219,     0,     0,  1113,     0,     0,     0,     0,  1224,
    1163,     0,     0,     0,     0,  1169,     0,  1117,     0,     0,
    1210,     0,  1152,     0,     0,     0,  1140,     0,  1171,     0,
       0,     0,  1168,     0,  1118,     0,     0,     0,  1212,     0,
       0,  1114,  1211,     0,  1119,  1115,     0,  1164,  1151,     0,
       0,  1165,     0,     0,  1170,     0,     0,     0,     0,     0,
       0,  1153,     0,     0,     0,     0,     0,  1172,     0,     0,
       0,  1120,     0,     0,  1154,     0,     0,  1213,     0,     0,
    1173,     0,     0,     0,  1155,  1121,     0,  1122,     0,     0,
    1174,     0,     0,     0,     0,     0,     0,  1116,     0,     0,
    1156,     0,     0,  1166,  1139,     0,  1175,     0,     0,     0,
    1167,     0,     0,     0,     0,     0,  1123,     0,     0,  1124,
       0,  1125,  1126,  1127,  1128,     0,  1129,  1130,     0,  1131,
       0,     0,     0,     0,  1132,     0,     0,  1133,     0,  1134,
    1135,  1136,  1137,     0,  1138,  1145,     0,  1146,     0,  1143,
    1144,     0,  1147,     0,     0,  1148
  };

  const short
  Dhcp4Parser::yycheck_[] =
  {
      67,    68,    69,    70,    71,    64,    73,    74,    75,    76,
      72,    74,   731,    69,  1025,    69,   671,  1025,  1025,    68,
      69,    70,    71,   678,    73,    74,     3,    76,    73,    74,
      74,     8,    69,   215,    95,     0,     5,    18,     7,    69,
       9,     3,    69,    69,     6,    93,    69,    70,    71,   110,
     111,     7,     3,   142,   143,    12,    13,     8,    68,    69,
      70,    71,     3,    73,    74,     3,    76,   632,     3,    69,
       8,     7,    69,     8,    71,    95,    96,    97,    98,    99,
     100,   101,    69,    69,    71,    69,   110,   111,     3,     3,
     110,   111,     3,     8,     8,    69,     3,     8,    69,     7,
      71,     8,    69,    15,    16,     3,     3,    19,     6,     6,
      22,    23,    24,    25,    69,    17,    18,    69,     3,    69,
      69,    69,    69,     8,    69,    69,   215,    69,   189,    39,
     191,   192,    69,   157,   158,    69,   701,    69,    69,    69,
      69,     3,    69,    69,     7,    69,     8,     3,    70,    70,
      69,     4,     8,    69,   215,     3,    11,   110,   111,    14,
       8,    20,    21,   179,   180,   181,   182,   198,   199,   200,
     201,    26,   142,   143,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,   215,    95,     3,    97,    98,     6,
      10,   215,   102,   103,   104,   215,   177,     7,   110,   111,
     110,   111,   218,   116,   117,   118,   119,   160,   161,   162,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,     7,     3,     3,    93,    94,
    1251,     8,     8,  1251,  1251,     7,   215,   216,   217,   218,
     105,   106,   107,     3,     3,   110,   111,    94,     8,     8,
     115,     7,   215,    27,    28,   120,   121,   122,   123,   124,
       3,     7,   109,   110,   111,     8,   131,    32,    33,    34,
      35,   110,   111,     7,     7,     3,   141,     7,   125,   144,
       8,     6,   129,   130,  1013,     3,   151,   152,   139,   140,
       8,   156,     3,   215,   159,   215,     8,     8,   163,     3,
      65,    66,    67,   190,     8,     3,   193,   194,   195,   196,
       8,     3,     3,     8,   153,   154,   155,     8,   183,   184,
     185,   186,   187,   188,    40,    41,    42,     3,    93,    94,
      95,     4,   197,   145,   146,   147,   148,   149,   150,   132,
     133,   134,   135,     3,   137,   110,   111,     3,     8,     4,
     215,     4,     8,    30,    31,    32,    33,    34,    35,     4,
       4,   126,   127,   128,   433,   434,     3,     3,   215,     4,
       4,     8,     8,    57,    58,    59,   215,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,     4,     4,
       4,     4,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,     4,    90,    91,    92,     4,    94,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,   106,
     507,   108,     4,   110,   111,   112,   113,   114,   115,   516,
       4,     4,     4,   120,   121,   122,     4,     8,   125,     4,
     215,   528,   129,     3,   131,   532,     8,   516,     4,     4,
       4,   138,   528,     3,   528,     8,   543,     4,     4,   528,
       4,     4,     4,   532,     4,     4,   553,   215,     4,     4,
       4,   528,   559,     4,   543,     4,   559,   218,   528,   218,
       4,   528,   528,     4,   553,   528,   516,   218,   553,   532,
     559,   216,   579,     4,   559,   559,   183,   184,   528,     4,
     543,     4,   532,   216,   216,   216,   216,   216,   528,   216,
     579,   528,   218,   543,   217,     4,     4,   217,   217,   216,
     216,   528,   528,   553,   528,   216,   543,     4,   215,   559,
     218,     4,     4,   218,   528,   217,   543,   528,   218,   218,
     218,   528,   218,     4,     4,    32,    33,    34,   216,   579,
       4,     4,   543,   528,  1229,     4,   528,     4,   528,   528,
     528,   528,     4,   528,   528,   218,   528,   218,   218,   218,
       4,   528,     4,     4,   528,     4,   528,   528,   528,   528,
       4,   528,   528,     4,   528,     4,     4,     4,     4,   528,
     532,   532,   528,   216,     4,     4,   218,   218,     4,     4,
       4,     4,     4,     4,    39,   216,   218,    94,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,     4,   110,   111,    60,    61,    62,    63,    64,
       4,     4,   216,   218,     4,   216,   723,   124,   725,     4,
       4,     4,     4,   722,     4,   132,   133,   134,   135,   136,
     137,     4,   139,   216,     4,     4,   725,     4,     4,     4,
      95,    30,    31,    32,    33,    34,    35,     4,     4,     4,
       4,   216,     4,   760,   761,     4,   763,   218,     4,     4,
     767,   218,     4,     4,     4,   218,   218,     4,   218,   216,
     216,   760,   761,   762,   763,   725,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,   216,     4,     4,   218,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,   218,     4,     4,     4,    94,    95,     4,   215,     4,
     760,   761,   762,   763,   110,   111,   216,   106,     4,     7,
       7,   110,   111,     7,   113,     7,   115,   215,   215,   215,
       7,   120,   121,   122,     7,     5,   125,   215,   215,   215,
     129,     5,     5,     5,   215,     5,     7,   215,     5,   138,
       5,     5,     5,   216,   215,     7,     7,     7,     7,     7,
     215,     5,   215,     5,     7,   215,     5,   215,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   215,   215,   215,   215,     7,     5,   183,   184,     7,
       7,     7,     7,     7,   183,   184,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,     7,
     215,   215,   215,     7,   215,   215,   215,   215,   215,   215,
     215,     7,     7,   215,   215,   178,   215,   215,   215,   215,
       7,     4,     4,     4,   215,     4,   215,   215,   215,   215,
     215,   215,     4,     4,     4,     4,     4,     4,     4,     3,
     218,     6,     3,     6,     6,     3,     6,     3,     3,     6,
       3,   701,     6,     3,     6,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
     215,     3,     6,     4,   216,     8,     6,     4,     4,     4,
       4,     4,   218,   216,   216,   216,   216,   216,   216,     4,
       4,   216,     4,     4,     4,   216,   216,   216,   216,  1016,
     216,   216,  1019,   216,   218,  1022,     4,   216,  1025,  1022,
    1016,     4,  1016,  1030,   218,     4,  1033,  1016,  1030,  1036,
    1019,  1025,     4,  1022,  1019,   216,  1025,  1022,  1022,  1016,
    1025,  1030,   218,  1050,  1033,  1030,  1016,  1054,  1025,  1016,
    1016,   218,   218,  1016,     6,  1025,  1063,     3,  1025,  1025,
    1030,  1050,  1025,  1070,  1030,  1054,  1016,  1030,  1075,  1019,
    1033,  1060,  1022,   218,  1063,  1025,  1016,     4,  1063,  1016,
    1030,   215,   215,  1033,   215,  1025,  1075,   215,  1025,  1016,
    1016,   215,  1016,  1030,   215,   215,  1033,     8,  1025,  1025,
    1050,  1025,  1016,  1030,  1054,  1016,  1033,   215,   215,  1016,
    1060,  1025,  1075,  1063,  1025,     8,  1030,     4,  1025,  1030,
       8,  1016,  1033,  1030,  1016,  1075,  1016,  1016,  1016,  1016,
    1025,  1016,  1016,  1025,  1016,  1025,  1025,  1025,  1025,  1016,
    1025,  1025,  1016,  1025,  1016,  1016,  1016,  1016,  1025,  1016,
    1016,  1025,  1016,  1025,  1025,  1025,  1025,  1016,  1025,  1025,
    1016,  1025,     3,  1025,  1025,     8,  1025,     4,     4,  1025,
       4,     4,     4,   516,   215,     5,     7,     4,     4,     4,
       4,     4,   988,   723,   714,  1015,   725,   507,  1010,   813,
    1050,  1018,  1028,  1038,  1027,  1036,   215,   215,   562,  1045,
    1016,   528,  1251,   215,  1019,   215,  1205,   218,   216,   215,
    1021,   218,   215,  1075,  1075,  1024,   532,   216,   216,   215,
     215,  1022,  1035,  1033,   559,   553,   543,  1031,  1077,  1257,
    1054,  1075,  1060,   579,   763,   724,   583,  1267,   784,  1289,
    1309,  1064,    -1,    -1,  1251,    -1,    -1,    -1,    -1,  1070,
    1257,    -1,    -1,    -1,    -1,  1257,    -1,  1251,    -1,    -1,
    1267,    -1,  1251,    -1,    -1,    -1,  1251,    -1,  1257,    -1,
      -1,    -1,  1257,    -1,  1251,    -1,    -1,    -1,  1267,    -1,
      -1,  1251,  1267,    -1,  1251,  1251,    -1,  1257,  1251,    -1,
      -1,  1257,    -1,    -1,  1257,    -1,    -1,    -1,    -1,    -1,
      -1,  1251,    -1,    -1,    -1,    -1,    -1,  1257,    -1,    -1,
      -1,  1251,    -1,    -1,  1251,    -1,    -1,  1267,    -1,    -1,
    1257,    -1,    -1,    -1,  1251,  1251,    -1,  1251,    -1,    -1,
    1257,    -1,    -1,    -1,    -1,    -1,    -1,  1251,    -1,    -1,
    1251,    -1,    -1,  1257,  1251,    -1,  1257,    -1,    -1,    -1,
    1257,    -1,    -1,    -1,    -1,    -1,  1251,    -1,    -1,  1251,
      -1,  1251,  1251,  1251,  1251,    -1,  1251,  1251,    -1,  1251,
      -1,    -1,    -1,    -1,  1251,    -1,    -1,  1251,    -1,  1251,
    1251,  1251,  1251,    -1,  1251,  1251,    -1,  1251,    -1,  1251,
    1251,    -1,  1251,    -1,    -1,  1251
  };

  const short
  Dhcp4Parser::yystos_[] =
  {
       0,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,     0,     5,
       7,     9,   215,   216,   217,   218,   234,   235,   236,   241,
       7,   250,     7,   255,     7,   306,     7,   419,     7,   503,
       7,   520,     7,   454,     7,   460,     7,   484,     7,   395,
       7,   587,     7,   618,   242,   237,   251,   256,   307,   420,
     504,   521,   455,   461,   485,   396,   588,   619,   234,   243,
     244,   215,   239,   240,    10,   252,   254,    11,    14,    26,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    93,    94,   105,   106,   107,
     110,   111,   115,   120,   121,   122,   123,   124,   131,   141,
     144,   151,   152,   156,   159,   163,   183,   184,   185,   186,
     187,   188,   197,   215,   249,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   272,
     273,   275,   276,   277,   278,   279,   280,   281,   284,   286,
     288,   289,   290,   291,   293,   295,   296,   297,   298,   299,
     300,   301,   302,   320,   322,   330,   332,   373,   382,   389,
     403,   413,   437,   438,   439,   440,   444,   452,   478,   510,
     512,   514,   525,   527,   529,   552,   566,   567,   575,   585,
     616,   625,   649,    15,    16,    19,    22,    23,    24,    25,
     249,   304,   305,   308,   310,   313,   316,   317,   318,   319,
     510,   512,    90,    91,    92,   108,   112,   113,   114,   125,
     129,   138,   249,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   273,   276,   277,   278,   279,   280,   281,
     284,   286,   288,   289,   290,   291,   293,   295,   301,   421,
     422,   423,   425,   427,   429,   431,   433,   435,   437,   438,
     439,   440,   443,   478,   497,   510,   512,   514,   525,   527,
     529,   549,   109,   130,   249,   433,   435,   478,   505,   506,
     507,   509,   510,   512,   124,   132,   133,   134,   135,   136,
     137,   139,   249,   478,   510,   512,   522,   523,   524,   525,
     527,   529,   531,   535,   537,   539,   541,   543,   545,   547,
     452,    39,    95,    97,    98,   102,   103,   104,   249,   350,
     462,   463,   464,   465,   466,   467,   468,   470,   472,   474,
     475,   477,   510,   512,    96,    99,   100,   101,   249,   350,
     466,   472,   486,   487,   488,   489,   490,   492,   493,   494,
     495,   496,   510,   512,   142,   143,   249,   397,   398,   399,
     401,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   183,   184,   249,   510,   512,   589,
     590,   591,   592,   594,   595,   597,   598,   599,   602,   604,
     606,   607,   608,   610,   612,   614,    12,    13,   620,   621,
     622,   624,     6,     3,     4,     8,     3,   253,     3,     8,
     617,   303,   323,     4,     4,     4,   526,   528,   530,     4,
     321,   331,   333,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,   271,     4,     4,     4,     4,     4,
     282,   285,   287,     4,     4,     4,     4,   414,   453,   479,
       4,   274,   445,   511,   513,   441,     4,     4,     4,   374,
     553,   515,   390,   404,     4,   383,   568,   576,   586,   292,
     294,     4,     4,     4,   626,   650,     4,     3,     8,   309,
     311,   314,     4,     4,     4,     4,     3,     8,   426,   428,
     430,   498,   424,   432,     4,   436,   434,   550,     3,     8,
     508,     4,     3,     8,   548,   536,   538,   542,   540,   546,
     544,   532,     8,     3,     8,   469,   351,     4,   473,   471,
     476,     4,     8,     3,   491,     4,     4,     4,     8,     3,
     400,   402,     3,     8,     4,   605,   593,     4,   596,     4,
       4,   600,   603,     4,     4,   609,   611,   613,   615,     3,
       8,   623,     4,     3,     8,   234,   234,   215,     4,     4,
       4,     4,   218,   218,   218,     4,     4,     4,   216,     4,
       4,     4,   216,   216,   216,   216,   216,   218,   217,   217,
     217,   216,   216,     4,   216,   216,   218,   218,   218,     4,
       4,     4,   218,   218,   217,   218,     4,     4,     4,   216,
       4,     4,     4,     4,     4,   218,   218,   218,     4,     4,
       4,     4,     4,   216,     4,     4,     4,     4,     4,     4,
     218,   218,   218,     4,     4,   258,     4,     4,     4,   218,
     218,   216,   216,   305,     4,     4,     4,     4,     4,     4,
     216,     4,     4,     4,   422,     4,   216,   506,     4,     4,
       4,     4,     4,     4,     4,     4,   524,     4,     4,   216,
       4,     4,     4,   218,   464,     4,   218,   218,   218,   488,
       4,     4,   398,   218,     4,     4,   216,     4,   216,   216,
       4,     4,   218,   218,     4,     4,     4,     4,   590,     4,
     216,   621,     4,     7,     7,     7,     7,   215,   215,   215,
       7,     7,     5,   215,   179,   180,   181,   182,   218,   283,
     215,   215,     5,     5,     5,   215,     5,   236,   238,   215,
     116,   117,   118,   119,   442,     5,     5,     5,     5,     7,
       7,     7,     7,     7,   215,   215,     5,     7,     5,   245,
      17,    18,   312,    20,    21,   315,   215,   215,   215,     5,
     215,   215,   245,   215,     7,   215,   245,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   238,   215,   215,   215,    18,   177,   601,   178,   283,
     215,   215,   215,     5,   234,   257,   620,   304,    27,    28,
     324,   325,   326,   328,    39,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    60,
      61,    62,    63,    64,   249,   338,   339,   340,   343,   345,
     347,   349,   350,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   364,   365,   367,   369,   371,   338,     7,
     334,   335,   336,     7,   415,   416,   417,     7,   456,   457,
     458,     7,   480,   481,   482,     7,   446,   447,   448,   132,
     133,   134,   135,   137,   375,   376,   377,   378,   379,   380,
     381,     7,   554,   555,     7,   516,   517,   518,     7,   391,
     392,   393,   145,   146,   147,   148,   149,   150,   405,   406,
     407,   408,   409,   410,   411,   412,   153,   154,   155,   249,
     384,   385,   386,   387,   388,   510,   512,   157,   158,   249,
     510,   512,   569,   570,   571,   573,   160,   161,   162,   215,
     510,   512,   577,   578,   579,   580,   582,   583,   589,     7,
     627,   628,   198,   199,   200,   201,   249,   651,   652,   653,
     654,   655,   656,   246,     7,   499,   500,   501,   140,   531,
     533,   551,   334,     8,     8,     8,   327,   329,     3,     8,
     341,   344,   346,   348,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,   362,     4,   366,   368,   370,   372,
       3,     8,     8,   337,     6,     3,   418,     6,     3,   459,
       6,     3,   483,     6,     3,   449,     6,     3,     3,     6,
     556,     3,     6,   519,     6,     3,   394,     6,     3,     4,
       4,     4,     4,     4,     4,     3,     8,     4,     4,     4,
       3,     8,   572,   574,     3,     8,     4,   581,     4,   584,
       3,     8,     8,   629,     3,     6,     4,     4,     4,     4,
       3,     8,   215,   247,   248,   502,     6,     3,   534,     8,
       6,     4,     4,   325,     4,     4,     4,     4,   216,   218,
     216,   218,   216,   216,   216,   216,   216,   216,     4,   216,
       4,     4,     4,     4,   339,   338,   336,   421,   417,   462,
     458,   486,   482,   249,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   273,   276,   277,   278,   279,   280,
     281,   284,   286,   288,   289,   290,   291,   293,   295,   301,
     350,   413,   431,   433,   435,   437,   438,   439,   440,   450,
     451,   478,   510,   512,   525,   527,   529,   549,   448,   376,
     126,   127,   128,   249,   259,   260,   261,   301,   350,   452,
     478,   510,   512,   525,   527,   529,   557,   558,   559,   560,
     561,   563,   565,   555,   522,   518,   397,   393,   216,   216,
     216,   216,   216,   216,   406,   218,   216,   216,   385,     4,
       4,   570,   218,     4,   216,     4,   578,   189,   191,   192,
     249,   350,   510,   512,   630,   631,   632,   633,   635,   628,
     218,   218,   218,   218,   652,     6,     3,   505,   501,     4,
     215,   215,    40,    41,    42,   342,   215,   215,   215,    57,
      58,    59,   363,   215,   215,   215,   215,     8,     8,     8,
       8,     3,     8,   562,   564,     4,     8,     3,     8,     8,
     215,   215,   215,   234,   636,     4,   634,     3,     8,   215,
       8,   245,   451,     4,     4,   218,   559,     4,   216,     4,
     631,   215,   215,     5,   215,     7,   637,   638,   639,     3,
       6,   190,   193,   194,   195,   196,   640,   641,   642,   644,
     645,   646,   647,   638,   643,     4,     4,     4,   648,     3,
       8,     4,   218,   216,   216,     4,   641,   215,   215
  };

  const short
  Dhcp4Parser::yyr1_[] =
  {
       0,   219,   221,   220,   222,   220,   223,   220,   224,   220,
     225,   220,   226,   220,   227,   220,   228,   220,   229,   220,
     230,   220,   231,   220,   232,   220,   233,   220,   234,   234,
     234,   234,   234,   234,   234,   235,   237,   236,   238,   239,
     239,   240,   240,   240,   242,   241,   243,   243,   244,   244,
     244,   246,   245,   247,   247,   248,   248,   248,   249,   251,
     250,   253,   252,   252,   254,   256,   255,   257,   257,   257,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   271,   270,
     272,   274,   273,   275,   276,   277,   278,   279,   280,   282,
     281,   283,   283,   283,   283,   283,   285,   284,   287,   286,
     288,   289,   290,   292,   291,   294,   293,   295,   296,   297,
     298,   299,   300,   301,   303,   302,   304,   304,   304,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   307,
     306,   309,   308,   311,   310,   312,   312,   314,   313,   315,
     315,   316,   317,   318,   319,   321,   320,   323,   322,   324,
     324,   324,   325,   325,   327,   326,   329,   328,   331,   330,
     333,   332,   334,   334,   335,   335,   335,   337,   336,   338,
     338,   338,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,   339,   339,   341,   340,   342,   342,   342,   344,
     343,   346,   345,   348,   347,   349,   351,   350,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   362,   361,   363,
     363,   363,   364,   366,   365,   368,   367,   370,   369,   372,
     371,   374,   373,   375,   375,   375,   376,   376,   376,   376,
     376,   377,   378,   379,   380,   381,   383,   382,   384,   384,
     384,   385,   385,   385,   385,   385,   385,   386,   387,   388,
     390,   389,   391,   391,   392,   392,   392,   394,   393,   396,
     395,   397,   397,   397,   397,   398,   398,   400,   399,   402,
     401,   404,   403,   405,   405,   405,   406,   406,   406,   406,
     406,   406,   407,   408,   409,   410,   411,   412,   414,   413,
     415,   415,   416,   416,   416,   418,   417,   420,   419,   421,
     421,   421,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     424,   423,   426,   425,   428,   427,   430,   429,   432,   431,
     434,   433,   436,   435,   437,   438,   439,   441,   440,   442,
     442,   442,   442,   443,   445,   444,   446,   446,   447,   447,
     447,   449,   448,   450,   450,   450,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   453,
     452,   455,   454,   456,   456,   457,   457,   457,   459,   458,
     461,   460,   462,   462,   463,   463,   463,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   464,   465,   466,   467,
     469,   468,   471,   470,   473,   472,   474,   476,   475,   477,
     479,   478,   480,   480,   481,   481,   481,   483,   482,   485,
     484,   486,   486,   487,   487,   487,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   489,   491,   490,   492,
     493,   494,   495,   496,   498,   497,   499,   499,   500,   500,
     500,   502,   501,   504,   503,   505,   505,   505,   506,   506,
     506,   506,   506,   506,   506,   506,   508,   507,   509,   511,
     510,   513,   512,   515,   514,   516,   516,   517,   517,   517,
     519,   518,   521,   520,   522,   522,   523,   523,   523,   524,
     524,   524,   524,   524,   524,   524,   524,   524,   524,   524,
     524,   524,   524,   524,   526,   525,   528,   527,   530,   529,
     532,   531,   534,   533,   536,   535,   538,   537,   540,   539,
     542,   541,   544,   543,   546,   545,   548,   547,   550,   549,
     551,   551,   553,   552,   554,   554,   554,   556,   555,   557,
     557,   558,   558,   558,   559,   559,   559,   559,   559,   559,
     559,   559,   559,   559,   559,   559,   559,   559,   559,   559,
     560,   562,   561,   564,   563,   565,   566,   568,   567,   569,
     569,   569,   570,   570,   570,   570,   570,   572,   571,   574,
     573,   576,   575,   577,   577,   577,   578,   578,   578,   578,
     578,   578,   579,   581,   580,   582,   584,   583,   586,   585,
     588,   587,   589,   589,   589,   590,   590,   590,   590,   590,
     590,   590,   590,   590,   590,   590,   590,   590,   590,   590,
     590,   590,   590,   591,   593,   592,   594,   596,   595,   597,
     598,   600,   599,   601,   601,   603,   602,   605,   604,   606,
     607,   609,   608,   611,   610,   613,   612,   615,   614,   617,
     616,   619,   618,   620,   620,   620,   621,   621,   623,   622,
     624,   626,   625,   627,   627,   627,   629,   628,   630,   630,
     630,   631,   631,   631,   631,   631,   631,   631,   632,   634,
     633,   636,   635,   637,   637,   637,   639,   638,   640,   640,
     640,   641,   641,   641,   641,   641,   643,   642,   644,   645,
     646,   648,   647,   650,   649,   651,   651,   651,   652,   652,
     652,   652,   652,   653,   654,   655,   656
  };

  const signed char
  Dhcp4Parser::yyr2_[] =
  {
       0,     2,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     4,     1,     0,
       1,     3,     5,     2,     0,     4,     0,     1,     1,     3,
       2,     0,     4,     0,     1,     1,     3,     2,     2,     0,
       4,     0,     6,     1,     2,     0,     4,     1,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     0,     4,
       3,     0,     4,     3,     3,     3,     3,     3,     3,     0,
       4,     1,     1,     1,     1,     1,     0,     4,     0,     4,
       3,     3,     3,     0,     4,     0,     4,     3,     3,     3,
       3,     3,     3,     3,     0,     6,     1,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       4,     0,     4,     0,     4,     1,     1,     0,     4,     1,
       1,     3,     3,     3,     3,     0,     6,     0,     6,     1,
       3,     2,     1,     1,     0,     4,     0,     4,     0,     6,
       0,     6,     0,     1,     1,     3,     2,     0,     4,     1,
       3,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     4,     1,     1,     1,     0,
       4,     0,     4,     0,     4,     3,     0,     4,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     0,     4,     1,
       1,     1,     3,     0,     4,     0,     4,     0,     4,     0,
       4,     0,     6,     1,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     6,     1,     3,
       2,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       0,     6,     0,     1,     1,     3,     2,     0,     4,     0,
       4,     1,     3,     2,     1,     1,     1,     0,     4,     0,
       4,     0,     6,     1,     3,     2,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     0,     6,
       0,     1,     1,     3,     2,     0,     4,     0,     4,     1,
       3,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     3,     3,     3,     0,     4,     1,
       1,     1,     1,     3,     0,     6,     0,     1,     1,     3,
       2,     0,     4,     1,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       6,     0,     4,     0,     1,     1,     3,     2,     0,     4,
       0,     4,     0,     1,     1,     3,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       0,     4,     0,     4,     0,     4,     1,     0,     4,     3,
       0,     6,     0,     1,     1,     3,     2,     0,     4,     0,
       4,     0,     1,     1,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     4,     1,
       1,     3,     3,     3,     0,     6,     0,     1,     1,     3,
       2,     0,     4,     0,     4,     1,     3,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     4,     3,     0,
       4,     0,     4,     0,     6,     0,     1,     1,     3,     2,
       0,     4,     0,     4,     0,     1,     1,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     6,
       1,     1,     0,     6,     1,     3,     2,     0,     4,     0,
       1,     1,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     4,     0,     4,     3,     3,     0,     6,     1,
       3,     2,     1,     1,     1,     1,     1,     0,     4,     0,
       4,     0,     6,     1,     3,     2,     1,     1,     1,     1,
       1,     1,     3,     0,     4,     3,     0,     4,     0,     6,
       0,     4,     1,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     4,     3,     0,     4,     3,
       3,     0,     4,     1,     1,     0,     4,     0,     4,     3,
       3,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       6,     0,     4,     1,     3,     2,     1,     1,     0,     6,
       3,     0,     6,     1,     3,     2,     0,     4,     1,     3,
       2,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       4,     0,     6,     1,     3,     2,     0,     4,     1,     3,
       2,     1,     1,     1,     1,     1,     0,     4,     3,     3,
       3,     0,     4,     0,     6,     1,     3,     2,     1,     1,
       1,     1,     1,     3,     3,     3,     3
  };


#if PARSER4_DEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const Dhcp4Parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "\",\"", "\":\"",
  "\"[\"", "\"]\"", "\"{\"", "\"}\"", "\"null\"", "\"Dhcp4\"",
  "\"config-control\"", "\"config-databases\"",
  "\"config-fetch-wait-time\"", "\"interfaces-config\"", "\"interfaces\"",
  "\"dhcp-socket-type\"", "\"raw\"", "\"udp\"", "\"outbound-interface\"",
  "\"same-as-inbound\"", "\"use-routing\"", "\"re-detect\"",
  "\"service-sockets-require-all\"", "\"service-sockets-retry-wait-time\"",
  "\"service-sockets-max-retries\"", "\"sanity-checks\"",
  "\"lease-checks\"", "\"extended-info-checks\"", "\"echo-client-id\"",
  "\"match-client-id\"", "\"authoritative\"", "\"next-server\"",
  "\"server-hostname\"", "\"boot-file-name\"", "\"offer-lifetime\"",
  "\"lease-database\"", "\"hosts-database\"", "\"hosts-databases\"",
  "\"type\"", "\"memfile\"", "\"mysql\"", "\"postgresql\"", "\"user\"",
  "\"password\"", "\"host\"", "\"port\"", "\"persist\"",
  "\"lfc-interval\"", "\"readonly\"", "\"connect-timeout\"",
  "\"read-timeout\"", "\"write-timeout\"", "\"tcp-user-timeout\"",
  "\"max-reconnect-tries\"", "\"reconnect-wait-time\"", "\"on-fail\"",
  "\"stop-retry-exit\"", "\"serve-retry-exit\"",
  "\"serve-retry-continue\"", "\"max-row-errors\"", "\"trust-anchor\"",
  "\"cert-file\"", "\"key-file\"", "\"cipher-list\"", "\"valid-lifetime\"",
  "\"min-valid-lifetime\"", "\"max-valid-lifetime\"", "\"renew-timer\"",
  "\"rebind-timer\"", "\"calculate-tee-times\"", "\"t1-percent\"",
  "\"t2-percent\"", "\"cache-threshold\"", "\"cache-max-age\"",
  "\"decline-probation-period\"", "\"server-tag\"",
  "\"statistic-default-sample-count\"", "\"statistic-default-sample-age\"",
  "\"ddns-send-updates\"", "\"ddns-override-no-update\"",
  "\"ddns-override-client-update\"", "\"ddns-replace-client-name\"",
  "\"ddns-generated-prefix\"", "\"ddns-qualifying-suffix\"",
  "\"ddns-update-on-renew\"", "\"ddns-use-conflict-resolution\"",
  "\"ddns-ttl-percent\"", "\"store-extended-info\"", "\"subnet4\"",
  "\"4o6-interface\"", "\"4o6-interface-id\"", "\"4o6-subnet\"",
  "\"option-def\"", "\"option-data\"", "\"name\"", "\"data\"", "\"code\"",
  "\"space\"", "\"csv-format\"", "\"always-send\"", "\"never-send\"",
  "\"record-types\"", "\"encapsulate\"", "\"array\"",
  "\"parked-packet-limit\"", "\"allocator\"", "\"shared-networks\"",
  "\"pools\"", "\"pool\"", "\"user-context\"", "\"comment\"", "\"subnet\"",
  "\"interface\"", "\"id\"", "\"reservation-mode\"", "\"disabled\"",
  "\"out-of-pool\"", "\"global\"", "\"all\"", "\"reservations-global\"",
  "\"reservations-in-subnet\"", "\"reservations-out-of-pool\"",
  "\"host-reservation-identifiers\"", "\"client-classes\"",
  "\"require-client-classes\"", "\"test\"", "\"template-test\"",
  "\"only-if-required\"", "\"client-class\"", "\"pool-id\"",
  "\"reservations\"", "\"duid\"", "\"hw-address\"", "\"circuit-id\"",
  "\"client-id\"", "\"hostname\"", "\"flex-id\"", "\"relay\"",
  "\"ip-address\"", "\"ip-addresses\"", "\"hooks-libraries\"",
  "\"library\"", "\"parameters\"", "\"expired-leases-processing\"",
  "\"reclaim-timer-wait-time\"", "\"flush-reclaimed-timer-wait-time\"",
  "\"hold-reclaimed-time\"", "\"max-reclaim-leases\"",
  "\"max-reclaim-time\"", "\"unwarned-reclaim-cycles\"",
  "\"dhcp4o6-port\"", "\"multi-threading\"", "\"enable-multi-threading\"",
  "\"thread-pool-size\"", "\"packet-queue-size\"", "\"control-socket\"",
  "\"socket-type\"", "\"socket-name\"", "\"dhcp-queue-control\"",
  "\"enable-queue\"", "\"queue-type\"", "\"capacity\"", "\"dhcp-ddns\"",
  "\"enable-updates\"", "\"qualifying-suffix\"", "\"server-ip\"",
  "\"server-port\"", "\"sender-ip\"", "\"sender-port\"",
  "\"max-queue-size\"", "\"ncr-protocol\"", "\"ncr-format\"",
  "\"override-no-update\"", "\"override-client-update\"",
  "\"replace-client-name\"", "\"generated-prefix\"", "\"tcp\"", "\"JSON\"",
  "\"when-present\"", "\"never\"", "\"always\"", "\"when-not-present\"",
  "\"hostname-char-set\"", "\"hostname-char-replacement\"",
  "\"early-global-reservations-lookup\"", "\"ip-reservations-unique\"",
  "\"reservations-lookup-first\"", "\"loggers\"", "\"output_options\"",
  "\"output\"", "\"debuglevel\"", "\"severity\"", "\"flush\"",
  "\"maxsize\"", "\"maxver\"", "\"pattern\"", "\"compatibility\"",
  "\"lenient-option-parsing\"", "\"ignore-dhcp-server-identifier\"",
  "\"ignore-rai-link-selection\"", "\"exclude-first-last-24\"",
  "TOPLEVEL_JSON", "TOPLEVEL_DHCP4", "SUB_DHCP4", "SUB_INTERFACES4",
  "SUB_SUBNET4", "SUB_POOL4", "SUB_RESERVATION", "SUB_OPTION_DEFS",
  "SUB_OPTION_DEF", "SUB_OPTION_DATA", "SUB_HOOKS_LIBRARY",
  "SUB_DHCP_DDNS", "SUB_CONFIG_CONTROL", "\"constant string\"",
  "\"integer\"", "\"floating point\"", "\"boolean\"", "$accept", "start",
  "$@1", "$@2", "$@3", "$@4", "$@5", "$@6", "$@7", "$@8", "$@9", "$@10",
  "$@11", "$@12", "$@13", "value", "sub_json", "map2", "$@14", "map_value",
  "map_content", "not_empty_map", "list_generic", "$@15", "list_content",
  "not_empty_list", "list_strings", "$@16", "list_strings_content",
  "not_empty_list_strings", "unknown_map_entry", "syntax_map", "$@17",
  "global_object", "$@18", "global_object_comma", "sub_dhcp4", "$@19",
  "global_params", "global_param", "valid_lifetime", "min_valid_lifetime",
  "max_valid_lifetime", "renew_timer", "rebind_timer",
  "calculate_tee_times", "t1_percent", "t2_percent", "cache_threshold",
  "cache_max_age", "decline_probation_period", "server_tag", "$@20",
  "parked_packet_limit", "allocator", "$@21", "echo_client_id",
  "match_client_id", "authoritative", "ddns_send_updates",
  "ddns_override_no_update", "ddns_override_client_update",
  "ddns_replace_client_name", "$@22", "ddns_replace_client_name_value",
  "ddns_generated_prefix", "$@23", "ddns_qualifying_suffix", "$@24",
  "ddns_update_on_renew", "ddns_use_conflict_resolution",
  "ddns_ttl_percent", "hostname_char_set", "$@25",
  "hostname_char_replacement", "$@26", "store_extended_info",
  "statistic_default_sample_count", "statistic_default_sample_age",
  "early_global_reservations_lookup", "ip_reservations_unique",
  "reservations_lookup_first", "offer_lifetime", "interfaces_config",
  "$@27", "interfaces_config_params", "interfaces_config_param",
  "sub_interfaces4", "$@28", "interfaces_list", "$@29", "dhcp_socket_type",
  "$@30", "socket_type", "outbound_interface", "$@31",
  "outbound_interface_value", "re_detect", "service_sockets_require_all",
  "service_sockets_retry_wait_time", "service_sockets_max_retries",
  "lease_database", "$@32", "sanity_checks", "$@33",
  "sanity_checks_params", "sanity_checks_param", "lease_checks", "$@34",
  "extended_info_checks", "$@35", "hosts_database", "$@36",
  "hosts_databases", "$@37", "database_list", "not_empty_database_list",
  "database", "$@38", "database_map_params", "database_map_param",
  "database_type", "$@39", "db_type", "user", "$@40", "password", "$@41",
  "host", "$@42", "port", "name", "$@43", "persist", "lfc_interval",
  "readonly", "connect_timeout", "read_timeout", "write_timeout",
  "tcp_user_timeout", "max_reconnect_tries", "reconnect_wait_time",
  "on_fail", "$@44", "on_fail_mode", "max_row_errors", "trust_anchor",
  "$@45", "cert_file", "$@46", "key_file", "$@47", "cipher_list", "$@48",
  "host_reservation_identifiers", "$@49",
  "host_reservation_identifiers_list", "host_reservation_identifier",
  "duid_id", "hw_address_id", "circuit_id", "client_id", "flex_id",
  "dhcp_multi_threading", "$@50", "multi_threading_params",
  "multi_threading_param", "enable_multi_threading", "thread_pool_size",
  "packet_queue_size", "hooks_libraries", "$@51", "hooks_libraries_list",
  "not_empty_hooks_libraries_list", "hooks_library", "$@52",
  "sub_hooks_library", "$@53", "hooks_params", "hooks_param", "library",
  "$@54", "parameters", "$@55", "expired_leases_processing", "$@56",
  "expired_leases_params", "expired_leases_param",
  "reclaim_timer_wait_time", "flush_reclaimed_timer_wait_time",
  "hold_reclaimed_time", "max_reclaim_leases", "max_reclaim_time",
  "unwarned_reclaim_cycles", "subnet4_list", "$@57",
  "subnet4_list_content", "not_empty_subnet4_list", "subnet4", "$@58",
  "sub_subnet4", "$@59", "subnet4_params", "subnet4_param", "subnet",
  "$@60", "subnet_4o6_interface", "$@61", "subnet_4o6_interface_id",
  "$@62", "subnet_4o6_subnet", "$@63", "interface", "$@64", "client_class",
  "$@65", "require_client_classes", "$@66", "reservations_global",
  "reservations_in_subnet", "reservations_out_of_pool", "reservation_mode",
  "$@67", "hr_mode", "id", "shared_networks", "$@68",
  "shared_networks_content", "shared_networks_list", "shared_network",
  "$@69", "shared_network_params", "shared_network_param",
  "option_def_list", "$@70", "sub_option_def_list", "$@71",
  "option_def_list_content", "not_empty_option_def_list",
  "option_def_entry", "$@72", "sub_option_def", "$@73",
  "option_def_params", "not_empty_option_def_params", "option_def_param",
  "option_def_name", "code", "option_def_code", "option_def_type", "$@74",
  "option_def_record_types", "$@75", "space", "$@76", "option_def_space",
  "option_def_encapsulate", "$@77", "option_def_array", "option_data_list",
  "$@78", "option_data_list_content", "not_empty_option_data_list",
  "option_data_entry", "$@79", "sub_option_data", "$@80",
  "option_data_params", "not_empty_option_data_params",
  "option_data_param", "option_data_name", "option_data_data", "$@81",
  "option_data_code", "option_data_space", "option_data_csv_format",
  "option_data_always_send", "option_data_never_send", "pools_list",
  "$@82", "pools_list_content", "not_empty_pools_list", "pool_list_entry",
  "$@83", "sub_pool4", "$@84", "pool_params", "pool_param", "pool_entry",
  "$@85", "pool_id", "user_context", "$@86", "comment", "$@87",
  "reservations", "$@88", "reservations_list",
  "not_empty_reservations_list", "reservation", "$@89", "sub_reservation",
  "$@90", "reservation_params", "not_empty_reservation_params",
  "reservation_param", "next_server", "$@91", "server_hostname", "$@92",
  "boot_file_name", "$@93", "ip_address", "$@94", "ip_addresses", "$@95",
  "duid", "$@96", "hw_address", "$@97", "client_id_value", "$@98",
  "circuit_id_value", "$@99", "flex_id_value", "$@100", "hostname",
  "$@101", "reservation_client_classes", "$@102", "relay", "$@103",
  "relay_map", "client_classes", "$@104", "client_classes_list",
  "client_class_entry", "$@105", "client_class_params",
  "not_empty_client_class_params", "client_class_param",
  "client_class_name", "client_class_test", "$@106",
  "client_class_template_test", "$@107", "only_if_required",
  "dhcp4o6_port", "control_socket", "$@108", "control_socket_params",
  "control_socket_param", "control_socket_type", "$@109",
  "control_socket_name", "$@110", "dhcp_queue_control", "$@111",
  "queue_control_params", "queue_control_param", "enable_queue",
  "queue_type", "$@112", "capacity", "arbitrary_map_entry", "$@113",
  "dhcp_ddns", "$@114", "sub_dhcp_ddns", "$@115", "dhcp_ddns_params",
  "dhcp_ddns_param", "enable_updates", "server_ip", "$@116", "server_port",
  "sender_ip", "$@117", "sender_port", "max_queue_size", "ncr_protocol",
  "$@118", "ncr_protocol_value", "ncr_format", "$@119",
  "dep_qualifying_suffix", "$@120", "dep_override_no_update",
  "dep_override_client_update", "dep_replace_client_name", "$@121",
  "dep_generated_prefix", "$@122", "dep_hostname_char_set", "$@123",
  "dep_hostname_char_replacement", "$@124", "config_control", "$@125",
  "sub_config_control", "$@126", "config_control_params",
  "config_control_param", "config_databases", "$@127",
  "config_fetch_wait_time", "loggers", "$@128", "loggers_entries",
  "logger_entry", "$@129", "logger_params", "logger_param", "debuglevel",
  "severity", "$@130", "output_options_list", "$@131",
  "output_options_list_content", "output_entry", "$@132",
  "output_params_list", "output_params", "output", "$@133", "flush",
  "maxsize", "maxver", "pattern", "$@134", "compatibility", "$@135",
  "compatibility_params", "compatibility_param", "lenient_option_parsing",
  "ignore_dhcp_server_identifier", "ignore_rai_link_selection",
  "exclude_first_last_24", YY_NULLPTR
  };
#endif


#if PARSER4_DEBUG
  const short
  Dhcp4Parser::yyrline_[] =
  {
       0,   313,   313,   313,   314,   314,   315,   315,   316,   316,
     317,   317,   318,   318,   319,   319,   320,   320,   321,   321,
     322,   322,   323,   323,   324,   324,   325,   325,   333,   334,
     335,   336,   337,   338,   339,   342,   347,   347,   358,   361,
     362,   365,   370,   376,   381,   381,   388,   389,   392,   396,
     400,   406,   406,   413,   414,   417,   421,   425,   435,   444,
     444,   459,   459,   473,   476,   482,   482,   491,   492,   493,
     500,   501,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   517,   518,   519,
     520,   521,   522,   523,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
     540,   541,   542,   543,   544,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   561,   562,   563,   564,   565,   566,   569,   575,   581,
     587,   593,   599,   605,   611,   617,   623,   629,   635,   635,
     644,   650,   650,   659,   665,   671,   677,   683,   689,   695,
     695,   704,   707,   710,   713,   716,   722,   722,   731,   731,
     740,   746,   752,   758,   758,   767,   767,   776,   782,   788,
     794,   800,   806,   812,   818,   818,   830,   831,   832,   837,
     838,   839,   840,   841,   842,   843,   844,   845,   846,   849,
     849,   858,   858,   869,   869,   877,   878,   881,   881,   889,
     891,   895,   901,   907,   913,   919,   919,   932,   932,   943,
     944,   945,   950,   951,   954,   954,   973,   973,   991,   991,
    1004,  1004,  1015,  1016,  1019,  1020,  1021,  1026,  1026,  1036,
    1037,  1038,  1043,  1044,  1045,  1046,  1047,  1048,  1049,  1050,
    1051,  1052,  1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,
    1061,  1062,  1063,  1064,  1067,  1067,  1075,  1076,  1077,  1080,
    1080,  1089,  1089,  1098,  1098,  1107,  1113,  1113,  1122,  1128,
    1134,  1140,  1146,  1152,  1158,  1164,  1170,  1176,  1176,  1184,
    1185,  1186,  1189,  1195,  1195,  1204,  1204,  1213,  1213,  1222,
    1222,  1231,  1231,  1242,  1243,  1244,  1249,  1250,  1251,  1252,
    1253,  1256,  1261,  1266,  1271,  1276,  1283,  1283,  1296,  1297,
    1298,  1303,  1304,  1305,  1306,  1307,  1308,  1311,  1317,  1323,
    1329,  1329,  1340,  1341,  1344,  1345,  1346,  1351,  1351,  1361,
    1361,  1371,  1372,  1373,  1376,  1379,  1380,  1383,  1383,  1392,
    1392,  1401,  1401,  1413,  1414,  1415,  1420,  1421,  1422,  1423,
    1424,  1425,  1428,  1434,  1440,  1446,  1452,  1458,  1467,  1467,
    1481,  1482,  1485,  1486,  1487,  1496,  1496,  1522,  1522,  1533,
    1534,  1535,  1541,  1542,  1543,  1544,  1545,  1546,  1547,  1548,
    1549,  1550,  1551,  1552,  1553,  1554,  1555,  1556,  1557,  1558,
    1559,  1560,  1561,  1562,  1563,  1564,  1565,  1566,  1567,  1568,
    1569,  1570,  1571,  1572,  1573,  1574,  1575,  1576,  1577,  1578,
    1579,  1580,  1581,  1582,  1583,  1584,  1585,  1586,  1587,  1588,
    1591,  1591,  1600,  1600,  1609,  1609,  1618,  1618,  1627,  1627,
    1636,  1636,  1645,  1645,  1656,  1662,  1668,  1674,  1674,  1682,
    1683,  1684,  1685,  1688,  1696,  1696,  1708,  1709,  1713,  1714,
    1715,  1720,  1720,  1728,  1729,  1730,  1735,  1736,  1737,  1738,
    1739,  1740,  1741,  1742,  1743,  1744,  1745,  1746,  1747,  1748,
    1749,  1750,  1751,  1752,  1753,  1754,  1755,  1756,  1757,  1758,
    1759,  1760,  1761,  1762,  1763,  1764,  1765,  1766,  1767,  1768,
    1769,  1770,  1771,  1772,  1773,  1774,  1775,  1776,  1777,  1784,
    1784,  1798,  1798,  1807,  1808,  1811,  1812,  1813,  1820,  1820,
    1835,  1835,  1849,  1850,  1853,  1854,  1855,  1860,  1861,  1862,
    1863,  1864,  1865,  1866,  1867,  1868,  1869,  1872,  1874,  1880,
    1882,  1882,  1891,  1891,  1900,  1900,  1909,  1911,  1911,  1920,
    1930,  1930,  1943,  1944,  1949,  1950,  1951,  1958,  1958,  1970,
    1970,  1982,  1983,  1988,  1989,  1990,  1997,  1998,  1999,  2000,
    2001,  2002,  2003,  2004,  2005,  2006,  2009,  2011,  2011,  2020,
    2022,  2024,  2030,  2036,  2045,  2045,  2058,  2059,  2062,  2063,
    2064,  2069,  2069,  2079,  2079,  2089,  2090,  2091,  2096,  2097,
    2098,  2099,  2100,  2101,  2102,  2103,  2106,  2106,  2115,  2121,
    2121,  2146,  2146,  2176,  2176,  2187,  2188,  2191,  2192,  2193,
    2198,  2198,  2207,  2207,  2216,  2217,  2220,  2221,  2222,  2228,
    2229,  2230,  2231,  2232,  2233,  2234,  2235,  2236,  2237,  2238,
    2239,  2240,  2241,  2242,  2245,  2245,  2254,  2254,  2263,  2263,
    2272,  2272,  2281,  2281,  2292,  2292,  2301,  2301,  2310,  2310,
    2319,  2319,  2328,  2328,  2337,  2337,  2346,  2346,  2360,  2360,
    2371,  2372,  2378,  2378,  2389,  2390,  2391,  2396,  2396,  2406,
    2407,  2410,  2411,  2412,  2417,  2418,  2419,  2420,  2421,  2422,
    2423,  2424,  2425,  2426,  2427,  2428,  2429,  2430,  2431,  2432,
    2435,  2437,  2437,  2446,  2446,  2455,  2463,  2471,  2471,  2482,
    2483,  2484,  2489,  2490,  2491,  2492,  2493,  2496,  2496,  2505,
    2505,  2517,  2517,  2530,  2531,  2532,  2537,  2538,  2539,  2540,
    2541,  2542,  2545,  2551,  2551,  2560,  2566,  2566,  2576,  2576,
    2589,  2589,  2599,  2600,  2601,  2606,  2607,  2608,  2609,  2610,
    2611,  2612,  2613,  2614,  2615,  2616,  2617,  2618,  2619,  2620,
    2621,  2622,  2623,  2626,  2632,  2632,  2641,  2647,  2647,  2656,
    2662,  2668,  2668,  2677,  2678,  2681,  2681,  2691,  2691,  2701,
    2708,  2715,  2715,  2724,  2724,  2734,  2734,  2744,  2744,  2756,
    2756,  2768,  2768,  2778,  2779,  2780,  2786,  2787,  2790,  2790,
    2801,  2809,  2809,  2822,  2823,  2824,  2830,  2830,  2838,  2839,
    2840,  2845,  2846,  2847,  2848,  2849,  2850,  2851,  2854,  2860,
    2860,  2869,  2869,  2880,  2881,  2882,  2887,  2887,  2895,  2896,
    2897,  2902,  2903,  2904,  2905,  2906,  2909,  2909,  2918,  2924,
    2930,  2936,  2936,  2945,  2945,  2956,  2957,  2958,  2963,  2964,
    2965,  2966,  2967,  2970,  2976,  2982,  2988
  };

  void
  Dhcp4Parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  Dhcp4Parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // PARSER4_DEBUG


#line 14 "dhcp4_parser.yy"
} } // isc::dhcp
#line 6367 "dhcp4_parser.cc"

#line 2994 "dhcp4_parser.yy"


void
isc::dhcp::Dhcp4Parser::error(const location_type& loc,
                              const std::string& what)
{
    ctx.error(loc, what);
}
