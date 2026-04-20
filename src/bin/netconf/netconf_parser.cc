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
#define yylex   netconf_lex



#include "netconf_parser.h"


// Unqualified %code blocks.
#line 36 "netconf_parser.yy"

#include <netconf/parser_context.h>

// Avoid warnings with the error counter.
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#line 57 "netconf_parser.cc"


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
#if NETCONF_DEBUG

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

#else // !NETCONF_DEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !NETCONF_DEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 14 "netconf_parser.yy"
namespace isc { namespace netconf {
#line 150 "netconf_parser.cc"

  /// Build a parser object.
  NetconfParser::NetconfParser (isc::netconf::ParserContext& ctx_yyarg)
#if NETCONF_DEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      ctx (ctx_yyarg)
  {}

  NetconfParser::~NetconfParser ()
  {}

  NetconfParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  NetconfParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  NetconfParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  NetconfParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  NetconfParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  NetconfParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  NetconfParser::symbol_kind_type
  NetconfParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  NetconfParser::stack_symbol_type::stack_symbol_type ()
  {}

  NetconfParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_value: // value
      case symbol_kind::S_map_value: // map_value
      case symbol_kind::S_socket_type_value: // socket_type_value
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
        value.YY_MOVE_OR_COPY< string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  NetconfParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_value: // value
      case symbol_kind::S_map_value: // map_value
      case symbol_kind::S_socket_type_value: // socket_type_value
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
        value.move< string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  NetconfParser::stack_symbol_type&
  NetconfParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_value: // value
      case symbol_kind::S_map_value: // map_value
      case symbol_kind::S_socket_type_value: // socket_type_value
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
        value.copy< string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  NetconfParser::stack_symbol_type&
  NetconfParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_value: // value
      case symbol_kind::S_map_value: // map_value
      case symbol_kind::S_socket_type_value: // socket_type_value
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
        value.move< string > (that.value);
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
  NetconfParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if NETCONF_DEBUG
  template <typename Base>
  void
  NetconfParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
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
#line 114 "netconf_parser.yy"
                 { yyoutput << yysym.value.template as < string > (); }
#line 389 "netconf_parser.cc"
        break;

      case symbol_kind::S_INTEGER: // "integer"
#line 114 "netconf_parser.yy"
                 { yyoutput << yysym.value.template as < int64_t > (); }
#line 395 "netconf_parser.cc"
        break;

      case symbol_kind::S_FLOAT: // "floating point"
#line 114 "netconf_parser.yy"
                 { yyoutput << yysym.value.template as < double > (); }
#line 401 "netconf_parser.cc"
        break;

      case symbol_kind::S_BOOLEAN: // "boolean"
#line 114 "netconf_parser.yy"
                 { yyoutput << yysym.value.template as < bool > (); }
#line 407 "netconf_parser.cc"
        break;

      case symbol_kind::S_value: // value
#line 114 "netconf_parser.yy"
                 { yyoutput << yysym.value.template as < ElementPtr > (); }
#line 413 "netconf_parser.cc"
        break;

      case symbol_kind::S_map_value: // map_value
#line 114 "netconf_parser.yy"
                 { yyoutput << yysym.value.template as < ElementPtr > (); }
#line 419 "netconf_parser.cc"
        break;

      case symbol_kind::S_socket_type_value: // socket_type_value
#line 114 "netconf_parser.yy"
                 { yyoutput << yysym.value.template as < ElementPtr > (); }
#line 425 "netconf_parser.cc"
        break;

      default:
        break;
    }
        yyo << ')';
      }
  }
#endif

  void
  NetconfParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  NetconfParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  NetconfParser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if NETCONF_DEBUG
  std::ostream&
  NetconfParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  NetconfParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  NetconfParser::debug_level_type
  NetconfParser::debug_level () const
  {
    return yydebug_;
  }

  void
  NetconfParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // NETCONF_DEBUG

  NetconfParser::state_type
  NetconfParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  NetconfParser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  NetconfParser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  NetconfParser::operator() ()
  {
    return parse ();
  }

  int
  NetconfParser::parse ()
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
      case symbol_kind::S_socket_type_value: // socket_type_value
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
        yylhs.value.emplace< string > ();
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
#line 125 "netconf_parser.yy"
                       { ctx.ctx_ = ctx.NO_KEYWORDS; }
#line 700 "netconf_parser.cc"
    break;

  case 4: // $@2: %empty
#line 126 "netconf_parser.yy"
                         { ctx.ctx_ = ctx.CONFIG; }
#line 706 "netconf_parser.cc"
    break;

  case 6: // $@3: %empty
#line 127 "netconf_parser.yy"
                         { ctx.ctx_ = ctx.NETCONF; }
#line 712 "netconf_parser.cc"
    break;

  case 8: // $@4: %empty
#line 135 "netconf_parser.yy"
                            {
    // Parse the Netconf map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 722 "netconf_parser.cc"
    break;

  case 9: // sub_netconf: "{" $@4 global_params "}"
#line 139 "netconf_parser.yy"
                               {
    // parsing completed
}
#line 730 "netconf_parser.cc"
    break;

  case 10: // json: value
#line 146 "netconf_parser.yy"
            {
    // Push back the JSON value on the stack
    ctx.stack_.push_back(yystack_[0].value.as < ElementPtr > ());
}
#line 739 "netconf_parser.cc"
    break;

  case 11: // value: "integer"
#line 152 "netconf_parser.yy"
               { yylhs.value.as < ElementPtr > () = ElementPtr(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location))); }
#line 745 "netconf_parser.cc"
    break;

  case 12: // value: "floating point"
#line 153 "netconf_parser.yy"
             { yylhs.value.as < ElementPtr > () = ElementPtr(new DoubleElement(yystack_[0].value.as < double > (), ctx.loc2pos(yystack_[0].location))); }
#line 751 "netconf_parser.cc"
    break;

  case 13: // value: "boolean"
#line 154 "netconf_parser.yy"
               { yylhs.value.as < ElementPtr > () = ElementPtr(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location))); }
#line 757 "netconf_parser.cc"
    break;

  case 14: // value: "constant string"
#line 155 "netconf_parser.yy"
              { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement(yystack_[0].value.as < string > (), ctx.loc2pos(yystack_[0].location))); }
#line 763 "netconf_parser.cc"
    break;

  case 15: // value: "null"
#line 156 "netconf_parser.yy"
                 { yylhs.value.as < ElementPtr > () = ElementPtr(new NullElement(ctx.loc2pos(yystack_[0].location))); }
#line 769 "netconf_parser.cc"
    break;

  case 16: // value: map
#line 157 "netconf_parser.yy"
           { yylhs.value.as < ElementPtr > () = ctx.stack_.back(); ctx.stack_.pop_back(); }
#line 775 "netconf_parser.cc"
    break;

  case 17: // value: list_generic
#line 158 "netconf_parser.yy"
                    { yylhs.value.as < ElementPtr > () = ctx.stack_.back(); ctx.stack_.pop_back(); }
#line 781 "netconf_parser.cc"
    break;

  case 18: // $@5: %empty
#line 162 "netconf_parser.yy"
                    {
    // This code is executed when we're about to start parsing
    // the content of the map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 792 "netconf_parser.cc"
    break;

  case 19: // map: "{" $@5 map_content "}"
#line 167 "netconf_parser.yy"
                             {
    // map parsing completed. If we ever want to do any wrap up
    // (maybe some sanity checking), this would be the best place
    // for it.
}
#line 802 "netconf_parser.cc"
    break;

  case 20: // map_value: map
#line 173 "netconf_parser.yy"
               { yylhs.value.as < ElementPtr > () = ctx.stack_.back(); ctx.stack_.pop_back(); }
#line 808 "netconf_parser.cc"
    break;

  case 23: // not_empty_map: "constant string" ":" value
#line 187 "netconf_parser.yy"
                                  {
                  // map containing a single entry
                  ctx.unique(yystack_[2].value.as < string > (), ctx.loc2pos(yystack_[2].location));
                  ctx.stack_.back()->set(yystack_[2].value.as < string > (), yystack_[0].value.as < ElementPtr > ());
                  }
#line 818 "netconf_parser.cc"
    break;

  case 24: // not_empty_map: not_empty_map "," "constant string" ":" value
#line 192 "netconf_parser.yy"
                                                      {
                  // map consisting of a shorter map followed by
                  // comma and string:value
                  ctx.unique(yystack_[2].value.as < string > (), ctx.loc2pos(yystack_[2].location));
                  ctx.stack_.back()->set(yystack_[2].value.as < string > (), yystack_[0].value.as < ElementPtr > ());
                  }
#line 829 "netconf_parser.cc"
    break;

  case 25: // not_empty_map: not_empty_map ","
#line 198 "netconf_parser.yy"
                                   {
                 ctx.warnAboutExtraCommas(yystack_[0].location);
                 }
#line 837 "netconf_parser.cc"
    break;

  case 26: // $@6: %empty
#line 203 "netconf_parser.yy"
                              {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(l);
}
#line 846 "netconf_parser.cc"
    break;

  case 27: // list_generic: "[" $@6 list_content "]"
#line 206 "netconf_parser.yy"
                               {
}
#line 853 "netconf_parser.cc"
    break;

  case 30: // not_empty_list: value
#line 213 "netconf_parser.yy"
                      {
                  // List consisting of a single element.
                  ctx.stack_.back()->add(yystack_[0].value.as < ElementPtr > ());
                  }
#line 862 "netconf_parser.cc"
    break;

  case 31: // not_empty_list: not_empty_list "," value
#line 217 "netconf_parser.yy"
                                           {
                  // List ending with , and a value.
                  ctx.stack_.back()->add(yystack_[0].value.as < ElementPtr > ());
                  }
#line 871 "netconf_parser.cc"
    break;

  case 32: // not_empty_list: not_empty_list ","
#line 221 "netconf_parser.yy"
                                     {
                  ctx.warnAboutExtraCommas(yystack_[0].location);
                  }
#line 879 "netconf_parser.cc"
    break;

  case 33: // unknown_map_entry: "constant string" ":"
#line 233 "netconf_parser.yy"
                                {
    const string& where = ctx.contextName();
    const string& keyword = yystack_[1].value.as < string > ();
    error(yystack_[1].location,
          "got unexpected keyword \"" + keyword + "\" in " + where + " map.");
}
#line 890 "netconf_parser.cc"
    break;

  case 34: // $@7: %empty
#line 241 "netconf_parser.yy"
                                   {
    // This code is executed when we're about to start parsing
    // the content of the map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 901 "netconf_parser.cc"
    break;

  case 35: // netconf_syntax_map: "{" $@7 global_object "}"
#line 246 "netconf_parser.yy"
                               {
    // map parsing completed. If we ever want to do any wrap up
    // (maybe some sanity checking), this would be the best place
    // for it.
}
#line 911 "netconf_parser.cc"
    break;

  case 36: // $@8: %empty
#line 253 "netconf_parser.yy"
                       {
    // Let's create a MapElement that will represent it, add it to the
    // top level map (that's already on the stack) and put the new map
    // on the stack as well, so child elements will be able to add
    // themselves to it.

    // Prevent against duplicate.
    ctx.unique("Netconf", ctx.loc2pos(yystack_[0].location));
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("Netconf", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.NETCONF);
}
#line 929 "netconf_parser.cc"
    break;

  case 37: // global_object: "Netconf" $@8 ":" "{" global_params "}"
#line 265 "netconf_parser.yy"
                                                    {
    // Ok, we're done with parsing Netconf. Let's take the map
    // off the stack.
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 940 "netconf_parser.cc"
    break;

  case 39: // global_object_comma: global_object ","
#line 274 "netconf_parser.yy"
                                         {
    ctx.warnAboutExtraCommas(yystack_[0].location);
}
#line 948 "netconf_parser.cc"
    break;

  case 44: // not_empty_global_params: not_empty_global_params ","
#line 285 "netconf_parser.yy"
                                                       {
                           ctx.warnAboutExtraCommas(yystack_[0].location);
                           }
#line 956 "netconf_parser.cc"
    break;

  case 54: // boot_update: "boot-update" ":" "boolean"
#line 303 "netconf_parser.yy"
                                       {
    ctx.unique("boot-update", ctx.loc2pos(yystack_[2].location));
    ElementPtr flag(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("boot-update", flag);
}
#line 966 "netconf_parser.cc"
    break;

  case 55: // subscribe_changes: "subscribe-changes" ":" "boolean"
#line 309 "netconf_parser.yy"
                                                   {
    ctx.unique("subscribe-changes", ctx.loc2pos(yystack_[2].location));
    ElementPtr flag(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("subscribe-changes", flag);
}
#line 976 "netconf_parser.cc"
    break;

  case 56: // validate_changes: "validate-changes" ":" "boolean"
#line 315 "netconf_parser.yy"
                                                 {
    ctx.unique("validate-changes", ctx.loc2pos(yystack_[2].location));
    ElementPtr flag(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("validate-changes", flag);
}
#line 986 "netconf_parser.cc"
    break;

  case 57: // $@9: %empty
#line 321 "netconf_parser.yy"
                           {
    ctx.enter(ctx.NO_KEYWORDS);
}
#line 994 "netconf_parser.cc"
    break;

  case 58: // user_context: "user-context" $@9 ":" map_value
#line 323 "netconf_parser.yy"
                  {
    ElementPtr parent = ctx.stack_.back();
    ElementPtr user_context = yystack_[0].value.as < ElementPtr > ();
    ConstElementPtr old = parent->get("user-context");

    // Handle already existing user context
    if (old) {
        // Check if it was a comment or a duplicate
        if ((old->size() != 1) || !old->contains("comment")) {
            stringstream msg;
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
#line 1021 "netconf_parser.cc"
    break;

  case 59: // $@10: %empty
#line 346 "netconf_parser.yy"
                 {
    ctx.enter(ctx.NO_KEYWORDS);
}
#line 1029 "netconf_parser.cc"
    break;

  case 60: // comment: "comment" $@10 ":" "constant string"
#line 348 "netconf_parser.yy"
               {
    ElementPtr parent = ctx.stack_.back();
    ElementPtr user_context(new MapElement(ctx.loc2pos(yystack_[3].location)));
    ElementPtr comment(new StringElement(yystack_[0].value.as < string > (), ctx.loc2pos(yystack_[0].location)));
    user_context->set("comment", comment);

    // Handle already existing user context
    ConstElementPtr old = parent->get("user-context");
    if (old) {
        // Check for duplicate comment
        if (old->contains("comment")) {
            stringstream msg;
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
#line 1058 "netconf_parser.cc"
    break;

  case 61: // $@11: %empty
#line 374 "netconf_parser.yy"
                                 {
    ctx.unique("hooks-libraries", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hooks-libraries", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.HOOKS_LIBRARIES);
}
#line 1070 "netconf_parser.cc"
    break;

  case 62: // hooks_libraries: "hooks-libraries" $@11 ":" "[" hooks_libraries_list "]"
#line 380 "netconf_parser.yy"
                                                             {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1079 "netconf_parser.cc"
    break;

  case 67: // not_empty_hooks_libraries_list: not_empty_hooks_libraries_list ","
#line 391 "netconf_parser.yy"
                                           {
        ctx.warnAboutExtraCommas(yystack_[0].location);
        }
#line 1087 "netconf_parser.cc"
    break;

  case 68: // $@12: %empty
#line 396 "netconf_parser.yy"
                              {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 1097 "netconf_parser.cc"
    break;

  case 69: // hooks_library: "{" $@12 hooks_params "}"
#line 400 "netconf_parser.yy"
                              {
    ctx.stack_.pop_back();
}
#line 1105 "netconf_parser.cc"
    break;

  case 72: // hooks_params: hooks_params ","
#line 406 "netconf_parser.yy"
                                 {
              ctx.warnAboutExtraCommas(yystack_[0].location);
              }
#line 1113 "netconf_parser.cc"
    break;

  case 76: // $@13: %empty
#line 416 "netconf_parser.yy"
                 {
    ctx.unique("library", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORDS);
}
#line 1122 "netconf_parser.cc"
    break;

  case 77: // library: "library" $@13 ":" "constant string"
#line 419 "netconf_parser.yy"
               {
    ElementPtr lib(new StringElement(yystack_[0].value.as < string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("library", lib);
    ctx.leave();
}
#line 1132 "netconf_parser.cc"
    break;

  case 78: // $@14: %empty
#line 425 "netconf_parser.yy"
                       {
    ctx.unique("parameters", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORDS);
}
#line 1141 "netconf_parser.cc"
    break;

  case 79: // parameters: "parameters" $@14 ":" map_value
#line 428 "netconf_parser.yy"
                  {
    ctx.stack_.back()->set("parameters", yystack_[0].value.as < ElementPtr > ());
    ctx.leave();
}
#line 1150 "netconf_parser.cc"
    break;

  case 80: // $@15: %empty
#line 436 "netconf_parser.yy"
                                                      {
    ctx.unique("managed-servers", ctx.loc2pos(yystack_[2].location));
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[2].location)));
    ctx.stack_.back()->set("managed-servers", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.MANAGED_SERVERS);
}
#line 1162 "netconf_parser.cc"
    break;

  case 81: // managed_servers: "managed-servers" ":" "{" $@15 servers_entries "}"
#line 442 "netconf_parser.yy"
                                 {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1171 "netconf_parser.cc"
    break;

  case 86: // not_empty_servers_entries: not_empty_servers_entries ","
#line 453 "netconf_parser.yy"
                                                           {
                             ctx.warnAboutExtraCommas(yystack_[0].location);
                             }
#line 1179 "netconf_parser.cc"
    break;

  case 91: // $@16: %empty
#line 468 "netconf_parser.yy"
                           {
    ctx.unique("dhcp4", ctx.loc2pos(yystack_[0].location));
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("dhcp4", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.SERVER);
}
#line 1191 "netconf_parser.cc"
    break;

  case 92: // dhcp4_server: "dhcp4" $@16 ":" "{" managed_server_params "}"
#line 474 "netconf_parser.yy"
                                                            {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1200 "netconf_parser.cc"
    break;

  case 93: // $@17: %empty
#line 480 "netconf_parser.yy"
                           {
    ctx.unique("dhcp6", ctx.loc2pos(yystack_[0].location));
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("dhcp6", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.SERVER);
}
#line 1212 "netconf_parser.cc"
    break;

  case 94: // dhcp6_server: "dhcp6" $@17 ":" "{" managed_server_params "}"
#line 486 "netconf_parser.yy"
                                                            {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1221 "netconf_parser.cc"
    break;

  case 95: // $@18: %empty
#line 492 "netconf_parser.yy"
                     {
    ctx.unique("d2", ctx.loc2pos(yystack_[0].location));
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("d2", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.SERVER);
}
#line 1233 "netconf_parser.cc"
    break;

  case 96: // d2_server: "d2" $@18 ":" "{" managed_server_params "}"
#line 498 "netconf_parser.yy"
                                                            {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1242 "netconf_parser.cc"
    break;

  case 99: // managed_server_params: managed_server_params ","
#line 506 "netconf_parser.yy"
                                                   {
                         ctx.warnAboutExtraCommas(yystack_[0].location);
                         }
#line 1250 "netconf_parser.cc"
    break;

  case 108: // $@19: %empty
#line 523 "netconf_parser.yy"
             {
    ctx.unique("model", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORDS);
}
#line 1259 "netconf_parser.cc"
    break;

  case 109: // model: "model" $@19 ":" "constant string"
#line 526 "netconf_parser.yy"
               {
    ElementPtr model(new StringElement(yystack_[0].value.as < string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("model", model);
    ctx.leave();
}
#line 1269 "netconf_parser.cc"
    break;

  case 110: // $@20: %empty
#line 533 "netconf_parser.yy"
                               {
    ctx.unique("control-socket", ctx.loc2pos(yystack_[0].location));
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("control-socket", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.CONTROL_SOCKET);
}
#line 1281 "netconf_parser.cc"
    break;

  case 111: // control_socket: "control-socket" $@20 ":" "{" control_socket_params "}"
#line 539 "netconf_parser.yy"
                                                            {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1290 "netconf_parser.cc"
    break;

  case 114: // control_socket_params: control_socket_params ","
#line 547 "netconf_parser.yy"
                                                   {
                         ctx.warnAboutExtraCommas(yystack_[0].location);
                         }
#line 1298 "netconf_parser.cc"
    break;

  case 121: // $@21: %empty
#line 560 "netconf_parser.yy"
                         {
    ctx.unique("socket-type", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.SOCKET_TYPE);
}
#line 1307 "netconf_parser.cc"
    break;

  case 122: // socket_type: "socket-type" $@21 ":" socket_type_value
#line 563 "netconf_parser.yy"
                          {
    ctx.stack_.back()->set("socket-type", yystack_[0].value.as < ElementPtr > ());
    ctx.leave();
}
#line 1316 "netconf_parser.cc"
    break;

  case 123: // socket_type_value: "unix"
#line 569 "netconf_parser.yy"
                         { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("unix", ctx.loc2pos(yystack_[0].location))); }
#line 1322 "netconf_parser.cc"
    break;

  case 124: // socket_type_value: "http"
#line 570 "netconf_parser.yy"
                         { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("http", ctx.loc2pos(yystack_[0].location))); }
#line 1328 "netconf_parser.cc"
    break;

  case 125: // socket_type_value: "stdout"
#line 571 "netconf_parser.yy"
                           { yylhs.value.as < ElementPtr > () = ElementPtr(new StringElement("stdout", ctx.loc2pos(yystack_[0].location))); }
#line 1334 "netconf_parser.cc"
    break;

  case 126: // $@22: %empty
#line 574 "netconf_parser.yy"
                         {
    ctx.unique("socket-name", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORDS);
}
#line 1343 "netconf_parser.cc"
    break;

  case 127: // socket_name: "socket-name" $@22 ":" "constant string"
#line 577 "netconf_parser.yy"
               {
    ElementPtr name(new StringElement(yystack_[0].value.as < string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("socket-name", name);
    ctx.leave();
}
#line 1353 "netconf_parser.cc"
    break;

  case 128: // $@23: %empty
#line 584 "netconf_parser.yy"
                       {
    ctx.unique("socket-url", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORDS);
}
#line 1362 "netconf_parser.cc"
    break;

  case 129: // socket_url: "socket-url" $@23 ":" "constant string"
#line 587 "netconf_parser.yy"
               {
    ElementPtr url(new StringElement(yystack_[0].value.as < string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("socket-url", url);
    ctx.leave();
}
#line 1372 "netconf_parser.cc"
    break;

  case 130: // $@24: %empty
#line 597 "netconf_parser.yy"
                 {
    ctx.unique("loggers", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("loggers", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.LOGGERS);
}
#line 1384 "netconf_parser.cc"
    break;

  case 131: // loggers: "loggers" $@24 ":" "[" loggers_entries "]"
#line 603 "netconf_parser.yy"
                                                         {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1393 "netconf_parser.cc"
    break;

  case 134: // loggers_entries: loggers_entries ","
#line 612 "netconf_parser.yy"
                                       {
                   ctx.warnAboutExtraCommas(yystack_[0].location);
                   }
#line 1401 "netconf_parser.cc"
    break;

  case 135: // $@25: %empty
#line 618 "netconf_parser.yy"
                             {
    ElementPtr l(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(l);
    ctx.stack_.push_back(l);
}
#line 1411 "netconf_parser.cc"
    break;

  case 136: // logger_entry: "{" $@25 logger_params "}"
#line 622 "netconf_parser.yy"
                               {
    ctx.stack_.pop_back();
}
#line 1419 "netconf_parser.cc"
    break;

  case 139: // logger_params: logger_params ","
#line 628 "netconf_parser.yy"
                                   {
                 ctx.warnAboutExtraCommas(yystack_[0].location);
                 }
#line 1427 "netconf_parser.cc"
    break;

  case 147: // $@26: %empty
#line 642 "netconf_parser.yy"
           {
    ctx.unique("name", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORDS);
}
#line 1436 "netconf_parser.cc"
    break;

  case 148: // name: "name" $@26 ":" "constant string"
#line 645 "netconf_parser.yy"
               {
    ElementPtr name(new StringElement(yystack_[0].value.as < string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("name", name);
    ctx.leave();
}
#line 1446 "netconf_parser.cc"
    break;

  case 149: // debuglevel: "debuglevel" ":" "integer"
#line 651 "netconf_parser.yy"
                                     {
    ctx.unique("debuglevel", ctx.loc2pos(yystack_[2].location));
    ElementPtr dl(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("debuglevel", dl);
}
#line 1456 "netconf_parser.cc"
    break;

  case 150: // $@27: %empty
#line 657 "netconf_parser.yy"
                   {
    ctx.unique("severity", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORDS);
}
#line 1465 "netconf_parser.cc"
    break;

  case 151: // severity: "severity" $@27 ":" "constant string"
#line 660 "netconf_parser.yy"
               {
    ElementPtr sev(new StringElement(yystack_[0].value.as < string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("severity", sev);
    ctx.leave();
}
#line 1475 "netconf_parser.cc"
    break;

  case 152: // $@28: %empty
#line 666 "netconf_parser.yy"
                                    {
    ctx.unique("output-options", ctx.loc2pos(yystack_[0].location));
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("output-options", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.OUTPUT_OPTIONS);
}
#line 1487 "netconf_parser.cc"
    break;

  case 153: // output_options_list: "output-options" $@28 ":" "[" output_options_list_content "]"
#line 672 "netconf_parser.yy"
                                                                    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1496 "netconf_parser.cc"
    break;

  case 156: // output_options_list_content: output_options_list_content ","
#line 679 "netconf_parser.yy"
                                                               {
                               ctx.warnAboutExtraCommas(yystack_[0].location);
                               }
#line 1504 "netconf_parser.cc"
    break;

  case 157: // $@29: %empty
#line 684 "netconf_parser.yy"
                             {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 1514 "netconf_parser.cc"
    break;

  case 158: // output_entry: "{" $@29 output_params_list "}"
#line 688 "netconf_parser.yy"
                                    {
    ctx.stack_.pop_back();
}
#line 1522 "netconf_parser.cc"
    break;

  case 161: // output_params_list: output_params_list ","
#line 694 "netconf_parser.yy"
                                        {
                 ctx.warnAboutExtraCommas(yystack_[0].location);
                 }
#line 1530 "netconf_parser.cc"
    break;

  case 167: // $@30: %empty
#line 706 "netconf_parser.yy"
               {
    ctx.unique("output", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORDS);
}
#line 1539 "netconf_parser.cc"
    break;

  case 168: // output: "output" $@30 ":" "constant string"
#line 709 "netconf_parser.yy"
               {
    ElementPtr sev(new StringElement(yystack_[0].value.as < string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("output", sev);
    ctx.leave();
}
#line 1549 "netconf_parser.cc"
    break;

  case 169: // flush: "flush" ":" "boolean"
#line 715 "netconf_parser.yy"
                           {
    ctx.unique("flush", ctx.loc2pos(yystack_[2].location));
    ElementPtr flush(new BoolElement(yystack_[0].value.as < bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("flush", flush);
}
#line 1559 "netconf_parser.cc"
    break;

  case 170: // maxsize: "maxsize" ":" "integer"
#line 721 "netconf_parser.yy"
                               {
    ctx.unique("maxsize", ctx.loc2pos(yystack_[2].location));
    ElementPtr maxsize(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("maxsize", maxsize);
}
#line 1569 "netconf_parser.cc"
    break;

  case 171: // maxver: "maxver" ":" "integer"
#line 727 "netconf_parser.yy"
                             {
    ctx.unique("maxver", ctx.loc2pos(yystack_[2].location));
    ElementPtr maxver(new IntElement(yystack_[0].value.as < int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("maxver", maxver);
}
#line 1579 "netconf_parser.cc"
    break;

  case 172: // $@31: %empty
#line 733 "netconf_parser.yy"
                 {
    ctx.unique("pattern", ctx.loc2pos(yystack_[0].location));
    ctx.enter(ctx.NO_KEYWORDS);
}
#line 1588 "netconf_parser.cc"
    break;

  case 173: // pattern: "pattern" $@31 ":" "constant string"
#line 736 "netconf_parser.yy"
               {
    ElementPtr sev(new StringElement(yystack_[0].value.as < string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("pattern", sev);
    ctx.leave();
}
#line 1598 "netconf_parser.cc"
    break;


#line 1602 "netconf_parser.cc"

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
  NetconfParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  NetconfParser::yytnamerr_ (const char *yystr)
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
  NetconfParser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // NetconfParser::context.
  NetconfParser::context::context (const NetconfParser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  NetconfParser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
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
  NetconfParser::yy_syntax_error_arguments_ (const context& yyctx,
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
  NetconfParser::yysyntax_error_ (const context& yyctx) const
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


  const signed char NetconfParser::yypact_ninf_ = -102;

  const signed char NetconfParser::yytable_ninf_ = -1;

  const short
  NetconfParser::yypact_[] =
  {
      73,  -102,  -102,  -102,    25,    15,     9,    22,  -102,  -102,
    -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,
    -102,  -102,  -102,  -102,    15,    19,    57,    -1,  -102,    65,
      79,    75,    82,    88,  -102,    33,  -102,  -102,  -102,    94,
     100,   102,   106,  -102,  -102,   115,  -102,    85,   121,  -102,
    -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,    15,
      15,  -102,    81,   122,  -102,  -102,   123,   124,    83,    84,
      87,   125,   129,   132,  -102,  -102,    -1,  -102,  -102,   133,
     131,   135,    99,  -102,  -102,  -102,  -102,   139,   140,  -102,
      15,    -1,  -102,  -102,  -102,     0,   141,   142,  -102,   138,
    -102,  -102,  -102,  -102,   143,   126,  -102,  -102,  -102,  -102,
    -102,   144,   149,  -102,  -102,    20,  -102,  -102,   150,   151,
     152,  -102,     0,   -23,  -102,   141,    76,   142,  -102,   146,
     153,   154,  -102,  -102,  -102,  -102,    34,  -102,  -102,  -102,
    -102,  -102,  -102,   155,  -102,  -102,  -102,  -102,    44,  -102,
    -102,  -102,  -102,  -102,  -102,    63,    63,    63,   158,   159,
      16,  -102,   160,   161,   112,   162,    76,  -102,  -102,  -102,
    -102,  -102,  -102,  -102,  -102,  -102,    45,  -102,  -102,  -102,
      77,    78,   103,   135,  -102,   114,   163,  -102,   127,  -102,
     165,   166,    63,  -102,  -102,  -102,  -102,  -102,  -102,   167,
    -102,   128,   168,  -102,  -102,    32,  -102,  -102,    91,    -6,
     167,  -102,  -102,  -102,  -102,  -102,  -102,  -102,    86,  -102,
    -102,  -102,  -102,  -102,   169,   172,   173,  -102,    89,  -102,
    -102,  -102,  -102,  -102,  -102,  -102,   174,   175,   176,    91,
    -102,   180,   120,   145,   147,   181,    -6,  -102,    98,   156,
     157,  -102,   164,  -102,  -102,  -102,   171,  -102,  -102,  -102,
    -102,  -102,  -102,  -102,  -102,  -102
  };

  const unsigned char
  NetconfParser::yydefact_[] =
  {
       0,     2,     4,     6,     0,     0,     0,     0,     1,    26,
      18,    15,    14,    11,    12,    13,     3,    10,    16,    17,
      34,     5,     8,     7,    28,    21,     0,    40,    30,     0,
      29,     0,     0,    22,    36,     0,    38,    57,    59,     0,
       0,     0,     0,    61,   130,     0,    53,     0,    41,    42,
      45,    46,    47,    51,    52,    49,    48,    50,    27,    32,
       0,    19,    25,     0,    39,    35,     0,     0,     0,     0,
       0,     0,     0,     0,    33,     9,    44,    31,    23,     0,
       0,     0,     0,    54,    55,    56,    80,     0,     0,    43,
       0,    40,    20,    58,    60,    82,    63,     0,    24,     0,
      91,    93,    95,    90,     0,    83,    84,    87,    88,    89,
      68,     0,    64,    65,   135,     0,   132,    37,     0,     0,
       0,    81,    86,     0,    62,    67,     0,   134,   131,     0,
       0,     0,    85,    76,    78,    73,     0,    70,    74,    75,
      66,   147,   152,     0,   150,   146,   144,   145,     0,   137,
     140,   142,   143,   141,   133,     0,     0,     0,     0,     0,
      72,    69,     0,     0,     0,     0,   139,   136,   108,   110,
     107,   101,   102,   103,   105,   106,     0,    97,   100,   104,
       0,     0,     0,     0,    71,     0,     0,   149,     0,   138,
       0,     0,    99,    92,    94,    96,    77,    79,   148,     0,
     151,     0,     0,    98,   157,     0,   154,   109,     0,     0,
     156,   153,   121,   126,   128,   120,   118,   119,     0,   112,
     115,   116,   117,   167,     0,     0,     0,   172,     0,   159,
     162,   163,   164,   165,   166,   155,     0,     0,     0,   114,
     111,     0,     0,     0,     0,     0,   161,   158,     0,     0,
       0,   113,     0,   169,   170,   171,     0,   160,   123,   124,
     125,   122,   127,   129,   168,   173
  };

  const signed char
  NetconfParser::yypgoto_[] =
  {
    -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,   -20,   -78,
    -102,     3,  -102,  -102,  -102,  -102,  -102,  -102,   -27,  -102,
    -102,  -102,  -102,  -102,    96,  -102,   113,   -22,   -19,   -18,
     -26,  -102,   -25,  -102,  -102,  -102,  -102,  -102,    66,  -102,
    -102,    28,  -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,
      71,  -102,  -102,  -102,  -102,  -102,  -102,  -101,     2,  -102,
    -102,  -102,  -102,  -102,   -44,  -102,  -102,  -102,  -102,  -102,
    -102,  -102,  -102,  -102,  -102,    69,  -102,  -102,    31,  -102,
    -102,  -102,  -102,  -102,  -102,  -102,  -102,   -12,  -102,  -102,
     -47,  -102,  -102,  -102,  -102,  -102,  -102,  -102
  };

  const short
  NetconfParser::yydefgoto_[] =
  {
       0,     4,     5,     6,     7,    23,    27,    16,    17,    18,
      25,    93,    32,    33,    19,    24,    29,    30,   170,    21,
      26,    35,    63,    36,    47,    48,    49,   171,   172,   173,
     174,    66,   175,    67,    55,    72,   111,   112,   113,   123,
     136,   137,   138,   158,   139,   159,    56,    95,   104,   105,
     106,   107,   118,   108,   119,   109,   120,   176,   177,   178,
     190,   179,   191,   218,   219,   220,   236,   261,   221,   237,
     222,   238,    57,    73,   115,   116,   126,   148,   149,   150,
     162,   151,   152,   165,   153,   163,   205,   206,   209,   228,
     229,   230,   241,   231,   232,   233,   234,   245
  };

  const short
  NetconfParser::yytable_[] =
  {
      46,    53,    54,    92,    28,    50,   133,   134,    51,    52,
      37,    38,    39,    40,    41,    42,    20,   100,   101,   102,
       9,    45,    10,   127,    11,     8,   128,    43,   223,    22,
      44,   224,   225,   226,   227,   210,    64,   160,   211,    77,
      78,    65,   161,    45,    45,   133,   134,   166,   192,    46,
      53,    54,   167,   193,    50,   180,   181,    51,    52,    12,
      13,    14,    15,    31,    46,    53,    54,    34,   103,    50,
      98,    58,    51,    52,    37,    38,    39,    40,    41,    60,
     192,   192,    59,   168,   169,   194,   195,    37,    38,   239,
      61,    62,   246,    75,   240,   103,   135,   247,    68,   145,
     146,   147,    37,    38,    69,    92,    70,    45,   141,   142,
      71,   143,   144,   212,     1,     2,     3,   213,   214,    74,
      45,   258,   259,   260,    76,    79,    80,    81,    82,   122,
      83,    84,    86,    87,    85,    45,    88,    90,    91,   145,
     146,   147,    10,    94,    96,    97,   117,   196,   110,   114,
     124,   121,   125,   155,   129,   130,   131,   187,   198,   164,
     156,   157,   182,   183,   185,   186,   188,   253,   199,   201,
     202,   200,   207,   242,   204,   208,   243,   244,   248,   249,
     250,   215,   216,   217,   252,   256,   197,    99,   184,    89,
     254,   140,   255,   132,   203,   251,   154,   189,   235,   257,
     262,   263,     0,     0,     0,     0,     0,     0,   264,     0,
       0,     0,   215,   216,   217,   265
  };

  const short
  NetconfParser::yycheck_[] =
  {
      27,    27,    27,    81,    24,    27,    29,    30,    27,    27,
      11,    12,    13,    14,    15,    16,     7,    17,    18,    19,
       5,    44,     7,     3,     9,     0,     6,    28,    34,     7,
      31,    37,    38,    39,    40,     3,     3,     3,     6,    59,
      60,     8,     8,    44,    44,    29,    30,     3,     3,    76,
      76,    76,     8,     8,    76,   156,   157,    76,    76,    44,
      45,    46,    47,    44,    91,    91,    91,    10,    95,    91,
      90,     6,    91,    91,    11,    12,    13,    14,    15,     4,
       3,     3,     3,    20,    21,     8,     8,    11,    12,     3,
       8,     3,     3,     8,     8,   122,   123,     8,     4,   126,
     126,   126,    11,    12,     4,   183,     4,    44,    32,    33,
       4,    35,    36,    22,    41,    42,    43,    26,    27,     4,
      44,    23,    24,    25,     3,    44,     4,     4,     4,     3,
      47,    47,     7,     4,    47,    44,     4,     4,     7,   166,
     166,   166,     7,    44,     5,     5,     8,    44,     7,     7,
       6,     8,     3,     7,     4,     4,     4,    45,    44,     4,
       7,     7,     4,     4,     4,     4,     4,    47,     5,     4,
       4,    44,    44,     4,     7,     7,     4,     4,     4,     4,
       4,   208,   208,   208,     4,     4,   183,    91,   160,    76,
      45,   125,    45,   122,   192,   239,   127,   166,   210,   246,
      44,    44,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,   239,   239,   239,    44
  };

  const unsigned char
  NetconfParser::yystos_[] =
  {
       0,    41,    42,    43,    49,    50,    51,    52,     0,     5,
       7,     9,    44,    45,    46,    47,    55,    56,    57,    62,
       7,    67,     7,    53,    63,    58,    68,    54,    56,    64,
      65,    44,    60,    61,    10,    69,    71,    11,    12,    13,
      14,    15,    16,    28,    31,    44,    66,    72,    73,    74,
      75,    76,    77,    78,    80,    82,    94,   120,     6,     3,
       4,     8,     3,    70,     3,     8,    79,    81,     4,     4,
       4,     4,    83,   121,     4,     8,     3,    56,    56,    44,
       4,     4,     4,    47,    47,    47,     7,     4,     4,    74,
       4,     7,    57,    59,    44,    95,     5,     5,    56,    72,
      17,    18,    19,    66,    96,    97,    98,    99,   101,   103,
       7,    84,    85,    86,     7,   122,   123,     8,   100,   102,
     104,     8,     3,    87,     6,     3,   124,     3,     6,     4,
       4,     4,    98,    29,    30,    66,    88,    89,    90,    92,
      86,    32,    33,    35,    36,    66,    78,    80,   125,   126,
     127,   129,   130,   132,   123,     7,     7,     7,    91,    93,
       3,     8,   128,   133,     4,   131,     3,     8,    20,    21,
      66,    75,    76,    77,    78,    80,   105,   106,   107,   109,
     105,   105,     4,     4,    89,     4,     4,    45,     4,   126,
     108,   110,     3,     8,     8,     8,    44,    59,    44,     5,
      44,     4,     4,   106,     7,   134,   135,    44,     7,   136,
       3,     6,    22,    26,    27,    66,    78,    80,   111,   112,
     113,   116,   118,    34,    37,    38,    39,    40,   137,   138,
     139,   141,   142,   143,   144,   135,   114,   117,   119,     3,
       8,   140,     4,     4,     4,   145,     3,     8,     4,     4,
       4,   112,     4,    47,    45,    45,     4,   138,    23,    24,
      25,   115,    44,    44,    44,    44
  };

  const unsigned char
  NetconfParser::yyr1_[] =
  {
       0,    48,    50,    49,    51,    49,    52,    49,    54,    53,
      55,    56,    56,    56,    56,    56,    56,    56,    58,    57,
      59,    60,    60,    61,    61,    61,    63,    62,    64,    64,
      65,    65,    65,    66,    68,    67,    70,    69,    69,    71,
      72,    72,    73,    73,    73,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    75,    76,    77,    79,    78,    81,
      80,    83,    82,    84,    84,    85,    85,    85,    87,    86,
      88,    88,    88,    88,    89,    89,    91,    90,    93,    92,
      95,    94,    96,    96,    97,    97,    97,    98,    98,    98,
      98,   100,    99,   102,   101,   104,   103,   105,   105,   105,
     106,   106,   106,   106,   106,   106,   106,   106,   108,   107,
     110,   109,   111,   111,   111,   112,   112,   112,   112,   112,
     112,   114,   113,   115,   115,   115,   117,   116,   119,   118,
     121,   120,   122,   122,   122,   124,   123,   125,   125,   125,
     126,   126,   126,   126,   126,   126,   126,   128,   127,   129,
     131,   130,   133,   132,   134,   134,   134,   136,   135,   137,
     137,   137,   138,   138,   138,   138,   138,   140,   139,   141,
     142,   143,   145,   144
  };

  const signed char
  NetconfParser::yyr2_[] =
  {
       0,     2,     0,     3,     0,     3,     0,     3,     0,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     4,
       1,     0,     1,     3,     5,     2,     0,     4,     0,     1,
       1,     3,     2,     2,     0,     4,     0,     6,     1,     2,
       0,     1,     1,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     0,     4,     0,
       4,     0,     6,     0,     1,     1,     3,     2,     0,     4,
       1,     3,     2,     1,     1,     1,     0,     4,     0,     4,
       0,     6,     0,     1,     1,     3,     2,     1,     1,     1,
       1,     0,     6,     0,     6,     0,     6,     1,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     4,
       0,     6,     1,     3,     2,     1,     1,     1,     1,     1,
       1,     0,     4,     1,     1,     1,     0,     4,     0,     4,
       0,     6,     1,     3,     2,     0,     4,     1,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     0,     4,     3,
       0,     4,     0,     6,     1,     3,     2,     0,     4,     1,
       3,     2,     1,     1,     1,     1,     1,     0,     4,     3,
       3,     3,     0,     4
  };


#if NETCONF_DEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const NetconfParser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "\",\"", "\":\"",
  "\"[\"", "\"]\"", "\"{\"", "\"}\"", "\"null\"", "\"Netconf\"",
  "\"user-context\"", "\"comment\"", "\"boot-update\"",
  "\"subscribe-changes\"", "\"validate-changes\"", "\"managed-servers\"",
  "\"dhcp4\"", "\"dhcp6\"", "\"d2\"", "\"model\"", "\"control-socket\"",
  "\"socket-type\"", "\"unix\"", "\"http\"", "\"stdout\"",
  "\"socket-name\"", "\"socket-url\"", "\"hooks-libraries\"",
  "\"library\"", "\"parameters\"", "\"loggers\"", "\"name\"",
  "\"output-options\"", "\"output\"", "\"debuglevel\"", "\"severity\"",
  "\"flush\"", "\"maxsize\"", "\"maxver\"", "\"pattern\"", "START_JSON",
  "START_NETCONF", "START_SUB_NETCONF", "\"constant string\"",
  "\"integer\"", "\"floating point\"", "\"boolean\"", "$accept", "start",
  "$@1", "$@2", "$@3", "sub_netconf", "$@4", "json", "value", "map", "$@5",
  "map_value", "map_content", "not_empty_map", "list_generic", "$@6",
  "list_content", "not_empty_list", "unknown_map_entry",
  "netconf_syntax_map", "$@7", "global_object", "$@8",
  "global_object_comma", "global_params", "not_empty_global_params",
  "global_param", "boot_update", "subscribe_changes", "validate_changes",
  "user_context", "$@9", "comment", "$@10", "hooks_libraries", "$@11",
  "hooks_libraries_list", "not_empty_hooks_libraries_list",
  "hooks_library", "$@12", "hooks_params", "hooks_param", "library",
  "$@13", "parameters", "$@14", "managed_servers", "$@15",
  "servers_entries", "not_empty_servers_entries", "server_entry",
  "dhcp4_server", "$@16", "dhcp6_server", "$@17", "d2_server", "$@18",
  "managed_server_params", "managed_server_param", "model", "$@19",
  "control_socket", "$@20", "control_socket_params",
  "control_socket_param", "socket_type", "$@21", "socket_type_value",
  "socket_name", "$@22", "socket_url", "$@23", "loggers", "$@24",
  "loggers_entries", "logger_entry", "$@25", "logger_params",
  "logger_param", "name", "$@26", "debuglevel", "severity", "$@27",
  "output_options_list", "$@28", "output_options_list_content",
  "output_entry", "$@29", "output_params_list", "output_params", "output",
  "$@30", "flush", "maxsize", "maxver", "pattern", "$@31", YY_NULLPTR
  };
#endif


#if NETCONF_DEBUG
  const short
  NetconfParser::yyrline_[] =
  {
       0,   125,   125,   125,   126,   126,   127,   127,   135,   135,
     146,   152,   153,   154,   155,   156,   157,   158,   162,   162,
     173,   178,   179,   187,   192,   198,   203,   203,   209,   210,
     213,   217,   221,   233,   241,   241,   253,   253,   271,   274,
     279,   280,   283,   284,   285,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   303,   309,   315,   321,   321,   346,
     346,   374,   374,   385,   386,   389,   390,   391,   396,   396,
     404,   405,   406,   409,   412,   413,   416,   416,   425,   425,
     436,   436,   447,   448,   451,   452,   453,   461,   462,   463,
     464,   468,   468,   480,   480,   492,   492,   504,   505,   506,
     512,   513,   514,   515,   516,   517,   518,   519,   523,   523,
     533,   533,   545,   546,   547,   552,   553,   554,   555,   556,
     557,   560,   560,   569,   570,   571,   574,   574,   584,   584,
     597,   597,   610,   611,   612,   618,   618,   626,   627,   628,
     633,   634,   635,   636,   637,   638,   639,   642,   642,   651,
     657,   657,   666,   666,   677,   678,   679,   684,   684,   692,
     693,   694,   699,   700,   701,   702,   703,   706,   706,   715,
     721,   727,   733,   733
  };

  void
  NetconfParser::yy_stack_print_ () const
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
  NetconfParser::yy_reduce_print_ (int yyrule) const
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
#endif // NETCONF_DEBUG


#line 14 "netconf_parser.yy"
} } // isc::netconf
#line 2282 "netconf_parser.cc"

#line 742 "netconf_parser.yy"


void
isc::netconf::NetconfParser::error(const location_type& loc,
                                   const string& what) {
    ctx.error(loc, what);
}
