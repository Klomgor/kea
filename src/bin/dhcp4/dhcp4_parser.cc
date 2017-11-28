// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

// Take the name prefix into account.
#define yylex   parser4_lex

// First part of user declarations.

#line 39 "dhcp4_parser.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "dhcp4_parser.h"

// User implementation prologue.

#line 53 "dhcp4_parser.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 34 "dhcp4_parser.yy" // lalr1.cc:413

#include <dhcp4/parser_context.h>

#line 59 "dhcp4_parser.cc" // lalr1.cc:413


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
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

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
      *yycdebug_ << std::endl;                  \
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
      yystack_print_ ();                \
  } while (false)

#else // !PARSER4_DEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !PARSER4_DEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 14 "dhcp4_parser.yy" // lalr1.cc:479
namespace isc { namespace dhcp {
#line 145 "dhcp4_parser.cc" // lalr1.cc:479

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
        std::string yyr = "";
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
              // Fall through.
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


  /// Build a parser object.
  Dhcp4Parser::Dhcp4Parser (isc::dhcp::Parser4Context& ctx_yyarg)
    :
#if PARSER4_DEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      ctx (ctx_yyarg)
  {}

  Dhcp4Parser::~Dhcp4Parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  Dhcp4Parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  Dhcp4Parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  Dhcp4Parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  Dhcp4Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  Dhcp4Parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  Dhcp4Parser::symbol_number_type
  Dhcp4Parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  Dhcp4Parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  Dhcp4Parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 165: // value
      case 169: // map_value
      case 207: // socket_type
      case 210: // outbound_interface_value
      case 220: // db_type
      case 297: // hr_mode
      case 442: // ncr_protocol_value
      case 450: // replace_client_name_value
        value.move< ElementPtr > (that.value);
        break;

      case 150: // "boolean"
        value.move< bool > (that.value);
        break;

      case 149: // "floating point"
        value.move< double > (that.value);
        break;

      case 148: // "integer"
        value.move< int64_t > (that.value);
        break;

      case 147: // "constant string"
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  Dhcp4Parser::stack_symbol_type&
  Dhcp4Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 165: // value
      case 169: // map_value
      case 207: // socket_type
      case 210: // outbound_interface_value
      case 220: // db_type
      case 297: // hr_mode
      case 442: // ncr_protocol_value
      case 450: // replace_client_name_value
        value.copy< ElementPtr > (that.value);
        break;

      case 150: // "boolean"
        value.copy< bool > (that.value);
        break;

      case 149: // "floating point"
        value.copy< double > (that.value);
        break;

      case 148: // "integer"
        value.copy< int64_t > (that.value);
        break;

      case 147: // "constant string"
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  Dhcp4Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if PARSER4_DEBUG
  template <typename Base>
  void
  Dhcp4Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    switch (yytype)
    {
            case 147: // "constant string"

#line 225 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< std::string > (); }
#line 366 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 148: // "integer"

#line 225 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< int64_t > (); }
#line 373 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 149: // "floating point"

#line 225 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< double > (); }
#line 380 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 150: // "boolean"

#line 225 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< bool > (); }
#line 387 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 165: // value

#line 225 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElementPtr > (); }
#line 394 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 169: // map_value

#line 225 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElementPtr > (); }
#line 401 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 207: // socket_type

#line 225 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElementPtr > (); }
#line 408 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 210: // outbound_interface_value

#line 225 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElementPtr > (); }
#line 415 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 220: // db_type

#line 225 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElementPtr > (); }
#line 422 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 297: // hr_mode

#line 225 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElementPtr > (); }
#line 429 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 442: // ncr_protocol_value

#line 225 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElementPtr > (); }
#line 436 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 450: // replace_client_name_value

#line 225 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElementPtr > (); }
#line 443 "dhcp4_parser.cc" // lalr1.cc:636
        break;


      default:
        break;
    }
    yyo << ')';
  }
#endif

  inline
  void
  Dhcp4Parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  Dhcp4Parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  Dhcp4Parser::yypop_ (unsigned int n)
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

  inline Dhcp4Parser::state_type
  Dhcp4Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  Dhcp4Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Dhcp4Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Dhcp4Parser::parse ()
  {
    // State.
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

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (ctx));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

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
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 165: // value
      case 169: // map_value
      case 207: // socket_type
      case 210: // outbound_interface_value
      case 220: // db_type
      case 297: // hr_mode
      case 442: // ncr_protocol_value
      case 450: // replace_client_name_value
        yylhs.value.build< ElementPtr > ();
        break;

      case 150: // "boolean"
        yylhs.value.build< bool > ();
        break;

      case 149: // "floating point"
        yylhs.value.build< double > ();
        break;

      case 148: // "integer"
        yylhs.value.build< int64_t > ();
        break;

      case 147: // "constant string"
        yylhs.value.build< std::string > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 234 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.NO_KEYWORD; }
#line 690 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 4:
#line 235 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.CONFIG; }
#line 696 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 6:
#line 236 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.DHCP4; }
#line 702 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 8:
#line 237 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.INTERFACES_CONFIG; }
#line 708 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 10:
#line 238 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.SUBNET4; }
#line 714 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 12:
#line 239 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.POOLS; }
#line 720 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 14:
#line 240 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.RESERVATIONS; }
#line 726 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 16:
#line 241 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.DHCP4; }
#line 732 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 18:
#line 242 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.OPTION_DEF; }
#line 738 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 20:
#line 243 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.OPTION_DATA; }
#line 744 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 22:
#line 244 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.HOOKS_LIBRARIES; }
#line 750 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 24:
#line 245 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.DHCP_DDNS; }
#line 756 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 26:
#line 253 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location))); }
#line 762 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 27:
#line 254 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new DoubleElement(yystack_[0].value.as< double > (), ctx.loc2pos(yystack_[0].location))); }
#line 768 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 28:
#line 255 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location))); }
#line 774 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 29:
#line 256 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location))); }
#line 780 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 30:
#line 257 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new NullElement(ctx.loc2pos(yystack_[0].location))); }
#line 786 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 31:
#line 258 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ctx.stack_.back(); ctx.stack_.pop_back(); }
#line 792 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 32:
#line 259 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ctx.stack_.back(); ctx.stack_.pop_back(); }
#line 798 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 33:
#line 262 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Push back the JSON value on the stack
    ctx.stack_.push_back(yystack_[0].value.as< ElementPtr > ());
}
#line 807 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 34:
#line 267 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // This code is executed when we're about to start parsing
    // the content of the map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 818 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 35:
#line 272 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // map parsing completed. If we ever want to do any wrap up
    // (maybe some sanity checking), this would be the best place
    // for it.
}
#line 828 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 36:
#line 278 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ctx.stack_.back(); ctx.stack_.pop_back(); }
#line 834 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 39:
#line 285 "dhcp4_parser.yy" // lalr1.cc:859
    {
                  // map containing a single entry
                  ctx.stack_.back()->set(yystack_[2].value.as< std::string > (), yystack_[0].value.as< ElementPtr > ());
                  }
#line 843 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 40:
#line 289 "dhcp4_parser.yy" // lalr1.cc:859
    {
                  // map consisting of a shorter map followed by
                  // comma and string:value
                  ctx.stack_.back()->set(yystack_[2].value.as< std::string > (), yystack_[0].value.as< ElementPtr > ());
                  }
#line 853 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 41:
#line 296 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(l);
}
#line 862 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 42:
#line 299 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // list parsing complete. Put any sanity checking here
}
#line 870 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 45:
#line 307 "dhcp4_parser.yy" // lalr1.cc:859
    {
                  // List consisting of a single element.
                  ctx.stack_.back()->add(yystack_[0].value.as< ElementPtr > ());
                  }
#line 879 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 46:
#line 311 "dhcp4_parser.yy" // lalr1.cc:859
    {
                  // List ending with , and a value.
                  ctx.stack_.back()->add(yystack_[0].value.as< ElementPtr > ());
                  }
#line 888 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 47:
#line 318 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // List parsing about to start
}
#line 896 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 48:
#line 320 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // list parsing complete. Put any sanity checking here
    //ctx.stack_.pop_back();
}
#line 905 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 51:
#line 329 "dhcp4_parser.yy" // lalr1.cc:859
    {
                          ElementPtr s(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
                          ctx.stack_.back()->add(s);
                          }
#line 914 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 52:
#line 333 "dhcp4_parser.yy" // lalr1.cc:859
    {
                          ElementPtr s(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
                          ctx.stack_.back()->add(s);
                          }
#line 923 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 53:
#line 344 "dhcp4_parser.yy" // lalr1.cc:859
    {
    const std::string& where = ctx.contextName();
    const std::string& keyword = yystack_[1].value.as< std::string > ();
    error(yystack_[1].location,
          "got unexpected keyword \"" + keyword + "\" in " + where + " map.");
}
#line 934 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 54:
#line 354 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // This code is executed when we're about to start parsing
    // the content of the map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 945 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 55:
#line 359 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // map parsing completed. If we ever want to do any wrap up
    // (maybe some sanity checking), this would be the best place
    // for it.

    // Dhcp4 is required
    ctx.require("Dhcp4", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
}
#line 958 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 64:
#line 383 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // This code is executed when we're about to start parsing
    // the content of the map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("Dhcp4", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.DHCP4);
}
#line 971 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 65:
#line 390 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // No global parameter is required
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 981 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 66:
#line 398 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the Dhcp4 map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 991 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 67:
#line 402 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // No global parameter is required
    // parsing completed
}
#line 1000 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 94:
#line 439 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr prf(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("valid-lifetime", prf);
}
#line 1009 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 95:
#line 444 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr prf(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("renew-timer", prf);
}
#line 1018 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 96:
#line 449 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr prf(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("rebind-timer", prf);
}
#line 1027 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 97:
#line 454 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr dpp(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("decline-probation-period", dpp);
}
#line 1036 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 98:
#line 459 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr echo(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("echo-client-id", echo);
}
#line 1045 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 99:
#line 464 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr match(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("match-client-id", match);
}
#line 1054 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 100:
#line 470 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr i(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("interfaces-config", i);
    ctx.stack_.push_back(i);
    ctx.enter(ctx.INTERFACES_CONFIG);
}
#line 1065 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 101:
#line 475 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // No interfaces config param is required
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1075 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 108:
#line 491 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the interfaces-config map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 1085 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 109:
#line 495 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // No interfaces config param is required
    // parsing completed
}
#line 1094 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 110:
#line 500 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("interfaces", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1105 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 111:
#line 505 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1114 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 112:
#line 510 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.DHCP_SOCKET_TYPE);
}
#line 1122 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 113:
#line 512 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("dhcp-socket-type", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 1131 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 114:
#line 517 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("raw", ctx.loc2pos(yystack_[0].location))); }
#line 1137 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 115:
#line 518 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("udp", ctx.loc2pos(yystack_[0].location))); }
#line 1143 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 116:
#line 521 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.OUTBOUND_INTERFACE);
}
#line 1151 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 117:
#line 523 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("outbound-interface", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 1160 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 118:
#line 528 "dhcp4_parser.yy" // lalr1.cc:859
    {
    yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("same-as-inbound", ctx.loc2pos(yystack_[0].location)));
}
#line 1168 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 119:
#line 530 "dhcp4_parser.yy" // lalr1.cc:859
    {
    yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("use-routing", ctx.loc2pos(yystack_[0].location)));
    }
#line 1176 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 120:
#line 534 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr b(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("re-detect", b);
}
#line 1185 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 121:
#line 540 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr i(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("lease-database", i);
    ctx.stack_.push_back(i);
    ctx.enter(ctx.LEASE_DATABASE);
}
#line 1196 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 122:
#line 545 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // The type parameter is required
    ctx.require("type", ctx.loc2pos(yystack_[2].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1207 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 123:
#line 552 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr i(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hosts-database", i);
    ctx.stack_.push_back(i);
    ctx.enter(ctx.HOSTS_DATABASE);
}
#line 1218 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 124:
#line 557 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // The type parameter is required
    ctx.require("type", ctx.loc2pos(yystack_[2].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1229 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 140:
#line 583 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.DATABASE_TYPE);
}
#line 1237 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 141:
#line 585 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("type", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 1246 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 142:
#line 590 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("memfile", ctx.loc2pos(yystack_[0].location))); }
#line 1252 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 143:
#line 591 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("mysql", ctx.loc2pos(yystack_[0].location))); }
#line 1258 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 144:
#line 592 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("postgresql", ctx.loc2pos(yystack_[0].location))); }
#line 1264 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 145:
#line 593 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("cql", ctx.loc2pos(yystack_[0].location))); }
#line 1270 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 146:
#line 596 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1278 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 147:
#line 598 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr user(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("user", user);
    ctx.leave();
}
#line 1288 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 148:
#line 604 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1296 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 149:
#line 606 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr pwd(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("password", pwd);
    ctx.leave();
}
#line 1306 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 150:
#line 612 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1314 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 151:
#line 614 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr h(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("host", h);
    ctx.leave();
}
#line 1324 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 152:
#line 620 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr p(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("port", p);
}
#line 1333 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 153:
#line 625 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1341 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 154:
#line 627 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr name(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("name", name);
    ctx.leave();
}
#line 1351 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 155:
#line 633 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr n(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("persist", n);
}
#line 1360 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 156:
#line 638 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr n(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("lfc-interval", n);
}
#line 1369 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 157:
#line 643 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr n(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("readonly", n);
}
#line 1378 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 158:
#line 648 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr n(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("connect-timeout", n);
}
#line 1387 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 159:
#line 653 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1395 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 160:
#line 655 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr cp(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("contact-points", cp);
    ctx.leave();
}
#line 1405 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 161:
#line 661 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1413 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 162:
#line 663 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr ks(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("keyspace", ks);
    ctx.leave();
}
#line 1423 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 163:
#line 670 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("host-reservation-identifiers", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.HOST_RESERVATION_IDENTIFIERS);
}
#line 1434 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 164:
#line 675 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1443 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 172:
#line 691 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr duid(new StringElement("duid", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(duid);
}
#line 1452 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 173:
#line 696 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr hwaddr(new StringElement("hw-address", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(hwaddr);
}
#line 1461 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 174:
#line 701 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr circuit(new StringElement("circuit-id", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(circuit);
}
#line 1470 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 175:
#line 706 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr client(new StringElement("client-id", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(client);
}
#line 1479 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 176:
#line 711 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr flex_id(new StringElement("flex-id", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(flex_id);
}
#line 1488 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 177:
#line 716 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hooks-libraries", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.HOOKS_LIBRARIES);
}
#line 1499 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 178:
#line 721 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1508 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 183:
#line 734 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 1518 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 184:
#line 738 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // The library hooks parameter is required
    ctx.require("library", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
}
#line 1528 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 185:
#line 744 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the hooks-libraries list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 1538 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 186:
#line 748 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // The library hooks parameter is required
    ctx.require("library", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    // parsing completed
}
#line 1548 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 192:
#line 763 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1556 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 193:
#line 765 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr lib(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("library", lib);
    ctx.leave();
}
#line 1566 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 194:
#line 771 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1574 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 195:
#line 773 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("parameters", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 1583 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 196:
#line 779 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("expired-leases-processing", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.EXPIRED_LEASES_PROCESSING);
}
#line 1594 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 197:
#line 784 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // No expired lease parameter is required
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1604 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 206:
#line 802 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr value(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("reclaim-timer-wait-time", value);
}
#line 1613 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 207:
#line 807 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr value(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("flush-reclaimed-timer-wait-time", value);
}
#line 1622 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 208:
#line 812 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr value(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hold-reclaimed-time", value);
}
#line 1631 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 209:
#line 817 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr value(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("max-reclaim-leases", value);
}
#line 1640 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 210:
#line 822 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr value(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("max-reclaim-time", value);
}
#line 1649 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 211:
#line 827 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr value(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("unwarned-reclaim-cycles", value);
}
#line 1658 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 212:
#line 835 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("subnet4", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.SUBNET4);
}
#line 1669 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 213:
#line 840 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1678 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 218:
#line 860 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 1688 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 219:
#line 864 "dhcp4_parser.yy" // lalr1.cc:859
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
#line 1714 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 220:
#line 886 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the subnet4 list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 1724 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 221:
#line 890 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // The subnet subnet4 parameter is required
    ctx.require("subnet", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    // parsing completed
}
#line 1734 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 248:
#line 928 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1742 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 249:
#line 930 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr subnet(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("subnet", subnet);
    ctx.leave();
}
#line 1752 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 250:
#line 936 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1760 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 251:
#line 938 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr iface(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("4o6-interface", iface);
    ctx.leave();
}
#line 1770 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 252:
#line 944 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1778 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 253:
#line 946 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr iface(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("4o6-interface-id", iface);
    ctx.leave();
}
#line 1788 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 254:
#line 952 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1796 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 255:
#line 954 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr iface(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("4o6-subnet", iface);
    ctx.leave();
}
#line 1806 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 256:
#line 960 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1814 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 257:
#line 962 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr iface(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("interface", iface);
    ctx.leave();
}
#line 1824 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 258:
#line 968 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1832 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 259:
#line 970 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr iface(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("interface-id", iface);
    ctx.leave();
}
#line 1842 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 260:
#line 976 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.CLIENT_CLASS);
}
#line 1850 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 261:
#line 978 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr cls(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("client-class", cls);
    ctx.leave();
}
#line 1860 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 262:
#line 984 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.RESERVATION_MODE);
}
#line 1868 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 263:
#line 986 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("reservation-mode", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 1877 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 264:
#line 991 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("disabled", ctx.loc2pos(yystack_[0].location))); }
#line 1883 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 265:
#line 992 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("out-of-pool", ctx.loc2pos(yystack_[0].location))); }
#line 1889 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 266:
#line 993 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("all", ctx.loc2pos(yystack_[0].location))); }
#line 1895 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 267:
#line 996 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr id(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("id", id);
}
#line 1904 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 268:
#line 1001 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr rc(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("rapid-commit", rc);
}
#line 1913 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 269:
#line 1008 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("shared-networks", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.SHARED_NETWORK);
}
#line 1924 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 270:
#line 1013 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1933 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 275:
#line 1028 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 1943 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 276:
#line 1032 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
}
#line 1951 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 294:
#line 1061 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("option-def", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.OPTION_DEF);
}
#line 1962 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 295:
#line 1066 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1971 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 296:
#line 1074 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 1980 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 297:
#line 1077 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // parsing completed
}
#line 1988 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 302:
#line 1093 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 1998 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 303:
#line 1097 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // The name, code and type option def parameters are required.
    ctx.require("name", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.require("code", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.require("type", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
}
#line 2010 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 304:
#line 1108 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the option-def list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 2020 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 305:
#line 1112 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // The name, code and type option def parameters are required.
    ctx.require("name", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.require("code", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.require("type", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    // parsing completed
}
#line 2032 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 319:
#line 1142 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr code(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("code", code);
}
#line 2041 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 321:
#line 1149 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2049 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 322:
#line 1151 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr prf(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("type", prf);
    ctx.leave();
}
#line 2059 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 323:
#line 1157 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2067 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 324:
#line 1159 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr rtypes(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("record-types", rtypes);
    ctx.leave();
}
#line 2077 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 325:
#line 1165 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2085 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 326:
#line 1167 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr space(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("space", space);
    ctx.leave();
}
#line 2095 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 328:
#line 1175 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2103 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 329:
#line 1177 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr encap(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("encapsulate", encap);
    ctx.leave();
}
#line 2113 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 330:
#line 1183 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr array(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("array", array);
}
#line 2122 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 331:
#line 1192 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("option-data", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.OPTION_DATA);
}
#line 2133 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 332:
#line 1197 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2142 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 337:
#line 1216 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 2152 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 338:
#line 1220 "dhcp4_parser.yy" // lalr1.cc:859
    {
    /// @todo: the code or name parameters are required.
    ctx.stack_.pop_back();
}
#line 2161 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 339:
#line 1228 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the option-data list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 2171 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 340:
#line 1232 "dhcp4_parser.yy" // lalr1.cc:859
    {
    /// @todo: the code or name parameters are required.
    // parsing completed
}
#line 2180 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 353:
#line 1263 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2188 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 354:
#line 1265 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr data(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("data", data);
    ctx.leave();
}
#line 2198 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 357:
#line 1275 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr space(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("csv-format", space);
}
#line 2207 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 358:
#line 1280 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr persist(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("always-send", persist);
}
#line 2216 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 359:
#line 1288 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("pools", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.POOLS);
}
#line 2227 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 360:
#line 1293 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2236 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 365:
#line 1308 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 2246 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 366:
#line 1312 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // The pool parameter is required.
    ctx.require("pool", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
}
#line 2256 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 367:
#line 1318 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the pool list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 2266 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 368:
#line 1322 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // The pool parameter is required.
    ctx.require("pool", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    // parsing completed
}
#line 2276 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 376:
#line 1339 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2284 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 377:
#line 1341 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr pool(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("pool", pool);
    ctx.leave();
}
#line 2294 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 378:
#line 1347 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2302 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 379:
#line 1349 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->combine_set("user-context", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 2311 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 380:
#line 1354 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2319 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 381:
#line 1356 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr e(new MapElement(ctx.loc2pos(yystack_[3].location)));
    e->set("comment", yystack_[0].value.as< ElementPtr > ());
    ctx.stack_.back()->combine_set("user-context", e);
    ctx.leave();
}
#line 2330 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 382:
#line 1366 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("reservations", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.RESERVATIONS);
}
#line 2341 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 383:
#line 1371 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2350 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 388:
#line 1384 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 2360 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 389:
#line 1388 "dhcp4_parser.yy" // lalr1.cc:859
    {
    /// @todo: an identifier parameter is required.
    ctx.stack_.pop_back();
}
#line 2369 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 390:
#line 1393 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the reservations list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 2379 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 391:
#line 1397 "dhcp4_parser.yy" // lalr1.cc:859
    {
    /// @todo: an identifier parameter is required.
    // parsing completed
}
#line 2388 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 409:
#line 1426 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2396 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 410:
#line 1428 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr next_server(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("next-server", next_server);
    ctx.leave();
}
#line 2406 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 411:
#line 1434 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2414 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 412:
#line 1436 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr srv(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("server-hostname", srv);
    ctx.leave();
}
#line 2424 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 413:
#line 1442 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2432 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 414:
#line 1444 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr bootfile(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("boot-file-name", bootfile);
    ctx.leave();
}
#line 2442 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 415:
#line 1450 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2450 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 416:
#line 1452 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr addr(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ip-address", addr);
    ctx.leave();
}
#line 2460 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 417:
#line 1458 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2468 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 418:
#line 1460 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr d(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("duid", d);
    ctx.leave();
}
#line 2478 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 419:
#line 1466 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2486 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 420:
#line 1468 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr hw(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hw-address", hw);
    ctx.leave();
}
#line 2496 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 421:
#line 1474 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2504 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 422:
#line 1476 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr hw(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("client-id", hw);
    ctx.leave();
}
#line 2514 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 423:
#line 1482 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2522 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 424:
#line 1484 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr hw(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("circuit-id", hw);
    ctx.leave();
}
#line 2532 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 425:
#line 1490 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2540 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 426:
#line 1492 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr hw(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("flex-id", hw);
    ctx.leave();
}
#line 2550 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 427:
#line 1498 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2558 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 428:
#line 1500 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr host(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hostname", host);
    ctx.leave();
}
#line 2568 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 429:
#line 1506 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr c(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("client-classes", c);
    ctx.stack_.push_back(c);
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2579 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 430:
#line 1511 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2588 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 431:
#line 1519 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("relay", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.RELAY);
}
#line 2599 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 432:
#line 1524 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2608 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 433:
#line 1529 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2616 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 434:
#line 1531 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr ip(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ip-address", ip);
    ctx.leave();
}
#line 2626 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 435:
#line 1540 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("client-classes", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.CLIENT_CLASSES);
}
#line 2637 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 436:
#line 1545 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2646 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 439:
#line 1554 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 2656 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 440:
#line 1558 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // The name client class parameter is required.
    ctx.require("name", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
}
#line 2666 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 454:
#line 1584 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2674 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 455:
#line 1586 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr test(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("test", test);
    ctx.leave();
}
#line 2684 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 456:
#line 1596 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr time(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("dhcp4o6-port", time);
}
#line 2693 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 457:
#line 1603 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("control-socket", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.CONTROL_SOCKET);
}
#line 2704 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 458:
#line 1608 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2713 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 463:
#line 1621 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2721 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 464:
#line 1623 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr stype(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("socket-type", stype);
    ctx.leave();
}
#line 2731 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 465:
#line 1629 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2739 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 466:
#line 1631 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr name(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("socket-name", name);
    ctx.leave();
}
#line 2749 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 467:
#line 1639 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("dhcp-ddns", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.DHCP_DDNS);
}
#line 2760 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 468:
#line 1644 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // The enable updates DHCP DDNS parameter is required.
    ctx.require("enable-updates", ctx.loc2pos(yystack_[2].location), ctx.loc2pos(yystack_[0].location));
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2771 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 469:
#line 1651 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the dhcp-ddns map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 2781 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 470:
#line 1655 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // The enable updates DHCP DDNS parameter is required.
    ctx.require("enable-updates", ctx.loc2pos(yystack_[3].location), ctx.loc2pos(yystack_[0].location));
    // parsing completed
}
#line 2791 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 488:
#line 1682 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr b(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("enable-updates", b);
}
#line 2800 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 489:
#line 1687 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2808 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 490:
#line 1689 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr s(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("qualifying-suffix", s);
    ctx.leave();
}
#line 2818 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 491:
#line 1695 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2826 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 492:
#line 1697 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr s(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("server-ip", s);
    ctx.leave();
}
#line 2836 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 493:
#line 1703 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr i(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("server-port", i);
}
#line 2845 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 494:
#line 1708 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2853 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 495:
#line 1710 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr s(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("sender-ip", s);
    ctx.leave();
}
#line 2863 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 496:
#line 1716 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr i(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("sender-port", i);
}
#line 2872 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 497:
#line 1721 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr i(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("max-queue-size", i);
}
#line 2881 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 498:
#line 1726 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NCR_PROTOCOL);
}
#line 2889 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 499:
#line 1728 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("ncr-protocol", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 2898 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 500:
#line 1734 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("UDP", ctx.loc2pos(yystack_[0].location))); }
#line 2904 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 501:
#line 1735 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("TCP", ctx.loc2pos(yystack_[0].location))); }
#line 2910 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 502:
#line 1738 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NCR_FORMAT);
}
#line 2918 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 503:
#line 1740 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr json(new StringElement("JSON", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ncr-format", json);
    ctx.leave();
}
#line 2928 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 504:
#line 1746 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr b(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("always-include-fqdn", b);
}
#line 2937 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 505:
#line 1751 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr b(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("override-no-update", b);
}
#line 2946 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 506:
#line 1756 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr b(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("override-client-update", b);
}
#line 2955 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 507:
#line 1761 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.REPLACE_CLIENT_NAME);
}
#line 2963 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 508:
#line 1763 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("replace-client-name", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 2972 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 509:
#line 1769 "dhcp4_parser.yy" // lalr1.cc:859
    {
      yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("when-present", ctx.loc2pos(yystack_[0].location))); 
      }
#line 2980 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 510:
#line 1772 "dhcp4_parser.yy" // lalr1.cc:859
    {
      yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("never", ctx.loc2pos(yystack_[0].location)));
      }
#line 2988 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 511:
#line 1775 "dhcp4_parser.yy" // lalr1.cc:859
    {
      yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("always", ctx.loc2pos(yystack_[0].location)));
      }
#line 2996 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 512:
#line 1778 "dhcp4_parser.yy" // lalr1.cc:859
    {
      yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("when-not-present", ctx.loc2pos(yystack_[0].location)));
      }
#line 3004 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 513:
#line 1781 "dhcp4_parser.yy" // lalr1.cc:859
    {
      error(yystack_[0].location, "boolean values for the replace-client-name are "
                "no longer supported");
      }
#line 3013 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 514:
#line 1787 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3021 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 515:
#line 1789 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr s(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("generated-prefix", s);
    ctx.leave();
}
#line 3031 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 516:
#line 1797 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3039 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 517:
#line 1799 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("Dhcp6", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 3048 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 518:
#line 1804 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3056 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 519:
#line 1806 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("DhcpDdns", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 3065 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 520:
#line 1811 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3073 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 521:
#line 1813 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("Control-agent", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 3082 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 522:
#line 1823 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("Logging", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.LOGGING);
}
#line 3093 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 523:
#line 1828 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 3102 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 527:
#line 1845 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("loggers", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.LOGGERS);
}
#line 3113 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 528:
#line 1850 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 3122 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 531:
#line 1862 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(l);
    ctx.stack_.push_back(l);
}
#line 3132 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 532:
#line 1866 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
}
#line 3140 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 540:
#line 1881 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr dl(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("debuglevel", dl);
}
#line 3149 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 541:
#line 1886 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3157 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 542:
#line 1888 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr sev(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("severity", sev);
    ctx.leave();
}
#line 3167 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 543:
#line 1894 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("output_options", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.OUTPUT_OPTIONS);
}
#line 3178 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 544:
#line 1899 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 3187 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 547:
#line 1908 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 3197 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 548:
#line 1912 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
}
#line 3205 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 555:
#line 1926 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3213 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 556:
#line 1928 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr sev(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("output", sev);
    ctx.leave();
}
#line 3223 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 557:
#line 1934 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr flush(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("flush", flush);
}
#line 3232 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 558:
#line 1939 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr maxsize(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("maxsize", maxsize);
}
#line 3241 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 559:
#line 1944 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr maxver(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("maxver", maxver);
}
#line 3250 "dhcp4_parser.cc" // lalr1.cc:859
    break;


#line 3254 "dhcp4_parser.cc" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
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
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
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

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
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

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  Dhcp4Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  Dhcp4Parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

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
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short int Dhcp4Parser::yypact_ninf_ = -695;

  const signed char Dhcp4Parser::yytable_ninf_ = -1;

  const short int
  Dhcp4Parser::yypact_[] =
  {
     130,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,    12,    20,    62,    84,    97,    99,   108,
     120,   126,   137,   144,   158,   180,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
      20,  -109,     9,   138,    63,    19,    75,    27,    49,   204,
     193,   -25,   251,  -695,   131,   168,   196,   190,   215,  -695,
    -695,  -695,  -695,  -695,   223,  -695,    31,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,   234,   240,  -695,  -695,  -695,  -695,
    -695,   249,   253,   256,   257,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,   277,  -695,  -695,  -695,    92,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,   278,   121,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,   300,   302,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,   122,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,   147,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,   227,   276,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,   287,  -695,  -695,   315,
    -695,  -695,  -695,   317,  -695,  -695,   299,   320,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,   323,
     342,  -695,  -695,  -695,  -695,   339,   347,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,   182,  -695,  -695,
    -695,   348,  -695,  -695,   374,  -695,   375,   376,  -695,  -695,
     377,   378,   379,  -695,  -695,  -695,   183,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,    20,    20,  -695,   237,   381,   382,   383,
     384,   385,  -695,     9,  -695,   386,   241,   242,   389,   390,
     391,   392,   393,   252,   254,   255,   258,   395,   397,   400,
     401,   403,   404,   405,   406,   263,   408,   409,   138,  -695,
     410,   411,   412,   267,    63,  -695,    96,   414,   416,   417,
     418,   419,   420,   421,   422,   423,   280,   279,   426,   427,
     428,   429,    19,  -695,   430,    75,  -695,   431,   432,   433,
     434,   435,   436,   437,   438,  -695,    27,  -695,   439,   440,
     297,   442,   443,   444,   301,  -695,   204,   445,   306,   307,
    -695,   193,   446,   448,   -42,  -695,   308,   449,   451,   311,
     456,   316,   318,   459,   461,   322,   324,   325,   463,   466,
     251,  -695,  -695,  -695,   469,   470,   471,    20,    20,    20,
    -695,   473,  -695,  -695,   329,   337,   338,   474,   475,  -695,
    -695,  -695,  -695,   482,   483,   484,   486,   487,   488,   489,
     490,  -695,   491,   492,  -695,   495,   164,    42,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,   493,   500,
    -695,  -695,  -695,   349,   357,   358,   501,   502,    20,   363,
     364,   365,  -695,  -695,   303,   366,   509,   510,  -695,   369,
    -695,   495,   371,   372,   373,   380,   387,   388,   394,  -695,
     396,   398,  -695,   399,   402,   407,  -695,  -695,   413,  -695,
    -695,  -695,   415,    20,  -695,  -695,   424,   425,  -695,   441,
    -695,  -695,    18,   447,  -695,  -695,  -695,    73,   450,  -695,
      20,   138,   452,  -695,  -695,  -695,    63,  -695,  -695,  -695,
     169,   169,   514,   515,   516,   517,   -27,    45,   518,   194,
     141,   251,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
     522,  -695,    96,  -695,  -695,  -695,   521,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,   523,   453,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,   208,  -695,   212,  -695,  -695,   216,  -695,  -695,
    -695,  -695,   525,   527,   528,   529,   532,  -695,  -695,  -695,
     231,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,   275,  -695,   531,   535,  -695,  -695,
     533,   539,  -695,  -695,   538,   544,  -695,  -695,   542,   547,
    -695,  -695,  -695,  -695,  -695,  -695,    74,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,   110,  -695,   545,   549,  -695,   551,
     552,   553,   554,   555,   557,   289,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,   290,  -695,  -695,  -695,   291,
     454,   455,  -695,  -695,   558,   550,  -695,  -695,   560,   564,
    -695,  -695,   561,  -695,   559,   452,  -695,  -695,   566,   569,
     570,   571,   457,   345,   458,   460,   464,   573,   574,   169,
    -695,  -695,    19,  -695,   514,   204,  -695,   515,   193,  -695,
     516,    50,  -695,   517,   -27,  -695,    45,  -695,   -25,  -695,
     518,   465,   467,   468,   472,   476,   477,   194,  -695,   575,
     576,   141,  -695,  -695,  -695,   562,   578,  -695,    75,  -695,
     521,    27,  -695,   523,   579,  -695,   577,  -695,   343,   462,
     479,   480,  -695,  -695,  -695,  -695,  -695,   481,   485,  -695,
     305,  -695,   581,  -695,   582,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,   327,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,   333,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,   494,   496,  -695,  -695,   497,
     334,  -695,   583,  -695,   498,   580,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,    50,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,   128,
    -695,  -695,   105,   580,  -695,  -695,   588,  -695,  -695,  -695,
     335,  -695,  -695,  -695,  -695,  -695,   589,   499,   590,   105,
    -695,   591,  -695,   504,  -695,   592,  -695,  -695,   222,  -695,
      46,   592,  -695,  -695,   594,   596,   599,   341,  -695,  -695,
    -695,  -695,  -695,  -695,   600,   503,   506,   507,    46,  -695,
     505,  -695,  -695,  -695,  -695,  -695
  };

  const unsigned short int
  Dhcp4Parser::yydefact_[] =
  {
       0,     2,     4,     6,     8,    10,    12,    14,    16,    18,
      20,    22,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     1,    41,    34,    30,
      29,    26,    27,    28,    33,     3,    31,    32,    54,     5,
      66,     7,   108,     9,   220,    11,   367,    13,   390,    15,
     296,    17,   304,    19,   339,    21,   185,    23,   469,    25,
      43,    37,     0,     0,     0,     0,     0,   392,     0,   306,
     341,     0,     0,    45,     0,    44,     0,     0,    38,    64,
     522,   516,   518,   520,     0,    63,     0,    56,    58,    60,
      61,    62,    59,   100,     0,     0,   409,   411,   413,   121,
     123,     0,     0,     0,     0,   212,   294,   331,   269,   163,
     435,   177,   196,     0,   457,   467,    93,     0,    68,    70,
      71,    72,    73,    88,    89,    76,    77,    78,    79,    83,
      84,    74,    75,    81,    82,    90,    91,    92,    80,    85,
      86,    87,   110,   112,   116,     0,     0,   102,   104,   105,
     106,   107,   439,   250,   252,   254,   359,   378,   380,   248,
     256,   258,     0,     0,   262,   260,   382,   431,   247,   224,
     225,   226,   238,     0,   222,   229,   242,   243,   244,   230,
     231,   234,   236,   232,   233,   227,   228,   245,   246,   235,
     239,   240,   241,   237,   376,   375,   372,     0,   369,   371,
     373,   374,   429,   417,   419,   423,   421,   427,   425,   415,
     408,   404,     0,   393,   394,   405,   406,   407,   401,   396,
     402,   398,   399,   400,   403,   397,     0,   321,   153,     0,
     325,   323,   328,     0,   317,   318,     0,   307,   308,   310,
     320,   311,   312,   313,   327,   314,   315,   316,   353,     0,
       0,   351,   352,   355,   356,     0,   342,   343,   345,   346,
     347,   348,   349,   350,   192,   194,   189,     0,   187,   190,
     191,     0,   489,   491,     0,   494,     0,     0,   498,   502,
       0,     0,     0,   507,   514,   487,     0,   471,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,    42,     0,     0,    35,     0,     0,     0,     0,
       0,     0,    53,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,   109,   441,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   221,     0,     0,   368,     0,     0,     0,
       0,     0,     0,     0,     0,   391,     0,   297,     0,     0,
       0,     0,     0,     0,     0,   305,     0,     0,     0,     0,
     340,     0,     0,     0,     0,   186,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   470,    46,    39,     0,     0,     0,     0,     0,     0,
      57,     0,    98,    99,     0,     0,     0,     0,     0,    94,
      95,    96,    97,     0,     0,     0,     0,     0,     0,     0,
       0,   456,     0,     0,    69,     0,     0,     0,   120,   103,
     454,   452,   453,   447,   448,   449,   450,   451,     0,   442,
     443,   445,   446,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   267,   268,     0,     0,     0,     0,   223,     0,
     370,     0,     0,     0,     0,     0,     0,     0,     0,   395,
       0,     0,   319,     0,     0,     0,   330,   309,     0,   357,
     358,   344,     0,     0,   188,   488,     0,     0,   493,     0,
     496,   497,     0,     0,   504,   505,   506,     0,     0,   472,
       0,     0,     0,   517,   519,   521,     0,   410,   412,   414,
       0,     0,   214,   298,   333,   271,     0,     0,   179,     0,
       0,     0,    47,   111,   114,   115,   113,   118,   119,   117,
       0,   440,     0,   251,   253,   255,   361,    36,   379,   381,
     249,   257,   259,   264,   265,   266,   263,   261,   384,     0,
     377,   430,   418,   420,   424,   422,   428,   426,   416,   322,
     154,   326,   324,   329,   354,   193,   195,   490,   492,   495,
     500,   501,   499,   503,   509,   510,   511,   512,   513,   508,
     515,    40,     0,   527,     0,   524,   526,     0,   140,   146,
     148,   150,     0,     0,     0,     0,     0,   159,   161,   139,
       0,   125,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,     0,   218,     0,   215,   216,   302,
       0,   299,   300,   337,     0,   334,   335,   275,     0,   272,
     273,   172,   173,   174,   175,   176,     0,   165,   167,   168,
     169,   170,   171,   437,     0,   183,     0,   180,   181,     0,
       0,     0,     0,     0,     0,     0,   198,   200,   201,   202,
     203,   204,   205,   463,   465,     0,   459,   461,   462,     0,
      49,     0,   444,   365,     0,   362,   363,   388,     0,   385,
     386,   433,     0,    65,     0,     0,   523,   101,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     122,   124,     0,   213,     0,   306,   295,     0,   341,   332,
       0,     0,   270,     0,     0,   164,     0,   436,     0,   178,
       0,     0,     0,     0,     0,     0,     0,     0,   197,     0,
       0,     0,   458,   468,    51,     0,    50,   455,     0,   360,
       0,   392,   383,     0,     0,   432,     0,   525,     0,     0,
       0,     0,   152,   155,   156,   157,   158,     0,     0,   126,
       0,   217,     0,   301,     0,   336,   293,   292,   282,   283,
     285,   279,   280,   281,   291,   290,     0,   277,   284,   286,
     287,   288,   289,   274,   166,   438,     0,   182,   206,   207,
     208,   209,   210,   211,   199,     0,     0,   460,    48,     0,
       0,   364,     0,   387,     0,     0,   142,   143,   144,   145,
     141,   147,   149,   151,   160,   162,   219,   303,   338,     0,
     276,   184,   464,   466,    52,   366,   389,   434,   531,     0,
     529,   278,     0,     0,   528,   543,     0,   541,   539,   535,
       0,   533,   537,   538,   536,   530,     0,     0,     0,     0,
     532,     0,   540,     0,   534,     0,   542,   547,     0,   545,
       0,     0,   544,   555,     0,     0,     0,     0,   549,   551,
     552,   553,   554,   546,     0,     0,     0,     0,     0,   548,
       0,   557,   558,   559,   550,   556
  };

  const short int
  Dhcp4Parser::yypgoto_[] =
  {
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,   -49,  -695,   117,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,   104,  -695,  -695,  -695,   -62,
    -695,  -695,  -695,   273,  -695,  -695,  -695,  -695,    86,   270,
     -52,   -51,   -34,  -695,  -695,   -29,  -695,  -695,    69,   274,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,    80,  -105,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,   -69,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -117,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -121,  -695,  -695,  -695,  -116,   229,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -126,  -695,  -695,  -695,  -695,  -695,  -695,
    -694,  -695,  -695,  -695,   -95,  -695,  -695,  -695,   -92,   269,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -687,  -695,
    -695,  -695,  -519,  -695,  -686,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -100,  -695,  -695,  -205,   -66,  -695,  -695,
    -695,  -695,  -695,   -91,  -695,  -695,  -695,   -90,  -695,   260,
    -695,   -46,  -695,  -695,  -695,  -695,  -695,   -38,  -695,  -695,
    -695,  -695,  -695,   -50,  -695,  -695,  -695,   -93,  -695,  -695,
    -695,   -89,  -695,   247,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -120,  -695,  -695,  -695,  -110,
     284,  -695,  -695,   -60,  -695,   -31,  -695,  -695,  -695,  -695,
    -695,  -107,  -695,  -695,  -695,  -119,  -695,   282,   -45,  -695,
     -44,  -695,   -37,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -685,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
     109,  -695,  -695,  -695,  -695,  -695,  -695,  -695,   -87,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,   124,   264,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,   -32,  -695,  -695,  -695,  -186,  -695,  -695,  -201,  -695,
    -695,  -695,  -695,  -695,  -695,  -206,  -695,  -695,  -215,  -695,
    -695,  -695,  -695,  -695
  };

  const short int
  Dhcp4Parser::yydefgoto_[] =
  {
      -1,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    34,    35,    36,    61,   558,    77,
      78,    37,    60,    74,    75,   543,   690,   755,   756,   116,
      39,    62,    86,    87,    88,   307,    41,    63,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   315,   146,   147,
      43,    64,   148,   340,   149,   341,   546,   150,   342,   549,
     151,   126,   321,   127,   322,   620,   621,   622,   708,   830,
     623,   709,   624,   710,   625,   711,   626,   235,   379,   628,
     629,   630,   631,   632,   717,   633,   718,   128,   331,   656,
     657,   658,   659,   660,   661,   662,   129,   333,   666,   667,
     668,   738,    57,    71,   267,   268,   269,   392,   270,   393,
     130,   334,   675,   676,   677,   678,   679,   680,   681,   682,
     131,   327,   636,   637,   638,   722,    45,    65,   173,   174,
     175,   353,   176,   347,   177,   348,   178,   349,   179,   354,
     180,   355,   181,   359,   182,   358,   566,   183,   184,   132,
     330,   648,   649,   650,   731,   796,   797,   133,   328,    51,
      68,   640,   641,   642,   725,    53,    69,   236,   237,   238,
     239,   240,   241,   242,   378,   243,   382,   244,   381,   245,
     246,   383,   247,   134,   329,   644,   645,   646,   728,    55,
      70,   255,   256,   257,   258,   259,   387,   260,   261,   262,
     263,   186,   350,   694,   695,   696,   758,    47,    66,   197,
     198,   199,   364,   187,   351,   188,   352,   189,   360,   698,
     699,   700,   761,    49,    67,   212,   213,   214,   135,   318,
     136,   319,   137,   320,   218,   374,   219,   368,   220,   369,
     221,   371,   222,   370,   223,   373,   224,   372,   225,   367,
     193,   361,   702,   764,   138,   332,   664,   346,   458,   459,
     460,   461,   462,   550,   139,   140,   336,   685,   686,   687,
     749,   688,   750,   141,   337,    59,    72,   286,   287,   288,
     289,   397,   290,   398,   291,   292,   400,   293,   294,   295,
     403,   592,   296,   404,   297,   298,   299,   300,   408,   599,
     301,   409,    89,   309,    90,   310,    91,   311,    92,   308,
     604,   605,   606,   704,   849,   850,   852,   860,   861,   862,
     863,   868,   864,   866,   878,   879,   880,   887,   888,   889,
     894,   890,   891,   892
  };

  const unsigned short int
  Dhcp4Parser::yytable_[] =
  {
      85,   252,   226,   168,   195,   210,   200,   234,   251,   266,
     285,    73,    26,   169,   170,   185,   196,   211,   663,    79,
     190,   191,   215,   216,   253,    27,   152,    28,   192,    29,
     217,   171,   254,   590,   313,   201,   172,   792,    76,   314,
      95,    96,    97,    98,   793,   795,   802,   264,   265,    96,
      97,    98,   152,   651,   652,   653,   654,   152,   655,   547,
     548,   101,   102,   103,   264,   265,   153,   154,   155,    38,
     107,    95,    96,    97,    98,   142,   143,   734,   107,   144,
     735,   156,   145,   157,   158,   159,   160,   161,   162,   163,
     164,    40,   101,   102,   103,   338,   105,   165,   166,   106,
     339,   107,   228,   202,    42,   167,    44,   203,   204,   205,
     206,   207,   208,   736,   209,    46,   737,   160,    96,    97,
      98,   164,    84,   165,   344,   362,   107,    48,   165,   345,
     363,   853,    80,    50,   854,   591,   167,   302,   194,   157,
     158,    81,    82,    83,    52,   792,   106,   107,   228,    93,
     365,    54,   793,   795,   802,   366,    84,   228,    94,    95,
      96,    97,    98,    99,   100,    56,    84,    30,    31,    32,
      33,   303,   883,   450,    84,   884,   885,   886,   544,   545,
     101,   102,   103,   104,   105,   394,   410,    58,   106,   107,
     395,   411,   594,   595,   596,   597,   608,    84,   305,   108,
     304,   609,   610,   611,   612,   613,   614,   615,   616,   617,
     618,   338,   794,   109,   110,   705,   703,   805,   306,   344,
     706,   228,    84,   598,   707,   881,   111,   312,   882,   112,
     855,   227,   856,   857,   719,   375,   113,   114,   316,   720,
     115,   683,   684,    84,   317,   228,   248,   229,   230,   249,
     250,    85,    84,   323,   412,   413,   228,   324,   229,   230,
     325,   326,   231,   232,   233,     1,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,   452,   719,   376,
     453,   335,   343,   721,   451,    84,   669,   670,   671,   672,
     673,   674,   747,   751,   410,   377,   454,   748,   752,   753,
     168,   455,   456,   195,   356,   200,   357,   385,   362,   457,
     169,   170,   185,   836,   210,   196,    84,   190,   191,   380,
     794,   384,   252,   386,   234,   192,   211,   388,   171,   251,
     839,   215,   216,   172,   201,   840,   394,   365,   869,   217,
      84,   841,   845,   870,   898,   253,   389,   390,   285,   899,
     391,    84,   396,   254,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   523,   524,
     525,   826,   827,   828,   829,   563,   564,   565,   399,   401,
     402,   405,   406,   407,   414,   415,   416,   417,   418,   419,
     421,   422,   423,   424,   425,   426,   427,   428,    84,   433,
     429,   434,   430,   431,   435,   436,   432,   437,   438,   439,
     440,   441,   442,   443,   445,   446,   447,   448,   463,   559,
     464,   465,   466,   467,   468,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   479,   481,   482,   483,   484,   485,
     486,   487,   488,   490,   491,   492,   493,   494,   495,   498,
     502,   496,   503,   506,   586,   507,   499,   500,   505,   508,
     509,   627,   627,   512,   510,   513,   511,   517,   619,   619,
     518,   601,   514,   520,   515,   516,   527,   521,   522,   285,
     526,   530,   531,   452,   528,   529,   453,   532,   533,   534,
     451,   535,   536,   537,   538,   773,   553,   539,   540,   541,
     542,   551,   454,   552,   554,   555,   556,   455,   456,    28,
     560,   561,   562,   567,   568,   457,   570,   569,   572,   573,
     574,   635,   639,   643,   647,   665,   691,   575,   693,   712,
     697,   713,   714,   715,   576,   577,   716,   723,   724,   726,
     701,   578,   727,   579,   729,   580,   581,   730,   732,   582,
     733,   739,   740,   760,   583,   741,   742,   743,   744,   745,
     584,   746,   585,   766,   759,   593,   762,   763,   818,   765,
     768,   587,   588,   769,   770,   771,   603,   777,   778,   815,
     816,   819,   825,   824,   557,   571,   420,   848,   589,   837,
     838,   846,   867,   871,   873,   607,   875,   600,   895,   877,
     896,   754,   757,   897,   900,   772,   774,   602,   444,   831,
     775,   634,   776,   808,   779,   809,   810,   804,   449,   807,
     811,   814,   806,   504,   812,   813,   832,   833,   834,   781,
     780,   478,   835,   803,   851,   782,   783,   785,   501,   784,
     821,   842,   822,   843,   844,   847,   497,   872,   820,   480,
     627,   876,   905,   901,   902,   903,   823,   619,   489,   252,
     168,   692,   791,   234,   817,   689,   251,   865,   874,   786,
     169,   170,   185,   767,   519,   893,   266,   190,   191,   787,
     788,   798,   253,   904,     0,   192,   799,   800,   171,     0,
     254,     0,     0,   172,   801,     0,   195,   789,   200,   210,
       0,     0,   790,     0,     0,     0,     0,     0,   196,     0,
       0,   211,     0,     0,     0,     0,   215,   216,     0,     0,
       0,     0,     0,     0,   217,     0,     0,   201,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     791,     0,     0,     0,     0,     0,     0,   786,     0,     0,
       0,     0,     0,   859,     0,     0,     0,   787,   788,   798,
     858,     0,     0,     0,   799,   800,     0,     0,     0,     0,
     859,     0,   801,     0,     0,   789,     0,   858,     0,     0,
     790
  };

  const short int
  Dhcp4Parser::yycheck_[] =
  {
      62,    70,    68,    65,    66,    67,    66,    69,    70,    71,
      72,    60,     0,    65,    65,    65,    66,    67,   537,    10,
      65,    65,    67,    67,    70,     5,     7,     7,    65,     9,
      67,    65,    70,    15,     3,    66,    65,   731,   147,     8,
      21,    22,    23,    24,   731,   731,   731,    89,    90,    22,
      23,    24,     7,    80,    81,    82,    83,     7,    85,    17,
      18,    42,    43,    44,    89,    90,    47,    48,    49,     7,
      51,    21,    22,    23,    24,    12,    13,     3,    51,    16,
       6,    62,    19,    64,    65,    66,    67,    68,    69,    70,
      71,     7,    42,    43,    44,     3,    46,    78,    79,    50,
       8,    51,    52,    76,     7,    86,     7,    80,    81,    82,
      83,    84,    85,     3,    87,     7,     6,    67,    22,    23,
      24,    71,   147,    78,     3,     3,    51,     7,    78,     8,
       8,     3,   123,     7,     6,   117,    86,     6,    63,    64,
      65,   132,   133,   134,     7,   839,    50,    51,    52,    11,
       3,     7,   839,   839,   839,     8,   147,    52,    20,    21,
      22,    23,    24,    25,    26,     7,   147,   147,   148,   149,
     150,     3,   126,    77,   147,   129,   130,   131,    14,    15,
      42,    43,    44,    45,    46,     3,     3,     7,    50,    51,
       8,     8,   119,   120,   121,   122,    27,   147,     8,    61,
       4,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,     3,   731,    75,    76,     3,     8,   736,     3,     3,
       8,    52,   147,   150,     8,     3,    88,     4,     6,    91,
     125,    27,   127,   128,     3,     8,    98,    99,     4,     8,
     102,   100,   101,   147,     4,    52,    53,    54,    55,    56,
      57,   313,   147,     4,   303,   304,    52,     4,    54,    55,
       4,     4,    58,    59,    60,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   346,     3,     3,
     346,     4,     4,     8,   346,   147,    92,    93,    94,    95,
      96,    97,     3,     3,     3,     8,   346,     8,     8,     8,
     362,   346,   346,   365,     4,   365,     4,     8,     3,   346,
     362,   362,   362,     8,   376,   365,   147,   362,   362,     4,
     839,     4,   391,     3,   386,   362,   376,     4,   362,   391,
       3,   376,   376,   362,   365,     8,     3,     3,     3,   376,
     147,     8,     8,     8,     3,   391,     4,     8,   410,     8,
       3,   147,     4,   391,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   417,   418,
     419,    28,    29,    30,    31,    72,    73,    74,     4,     4,
       4,     4,     4,     4,   147,     4,     4,     4,     4,     4,
       4,   150,   150,     4,     4,     4,     4,     4,   147,     4,
     148,     4,   148,   148,     4,     4,   148,     4,     4,     4,
       4,   148,     4,     4,     4,     4,     4,   150,     4,   468,
       4,     4,     4,     4,     4,     4,     4,     4,   148,   150,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,   148,     4,     4,     4,     4,
       4,   150,     4,     4,   503,     4,   150,   150,   150,   148,
       4,   530,   531,     4,   148,     4,   148,     4,   530,   531,
       4,   520,   150,     4,   150,   150,   147,     7,     7,   541,
       7,     7,     7,   552,   147,   147,   552,     5,     5,     5,
     552,     5,     5,     5,     5,   150,   147,     7,     7,     7,
       5,     8,   552,     3,   147,   147,     5,   552,   552,     7,
     147,   147,   147,   147,     5,   552,   147,     7,   147,   147,
     147,     7,     7,     7,     7,     7,     4,   147,     7,     4,
       7,     4,     4,     4,   147,   147,     4,     6,     3,     6,
      87,   147,     3,   147,     6,   147,   147,     3,     6,   147,
       3,     6,     3,     3,   147,     4,     4,     4,     4,     4,
     147,     4,   147,     4,     6,   118,     6,     3,     6,     8,
       4,   147,   147,     4,     4,     4,   124,     4,     4,     4,
       4,     3,     5,     4,   467,   481,   313,     7,   147,     8,
       8,     8,     4,     4,     4,   526,     5,   147,     4,     7,
       4,   147,   147,     4,     4,   148,   148,   521,   338,   147,
     150,   531,   148,   148,   719,   148,   148,   734,   344,   740,
     148,   747,   738,   394,   148,   148,   147,   147,   147,   724,
     722,   362,   147,   733,   839,   725,   727,   730,   391,   728,
     760,   147,   761,   147,   147,   147,   386,   148,   758,   365,
     719,   147,   147,   150,   148,   148,   763,   719,   376,   728,
     722,   552,   731,   725,   751,   541,   728,   853,   869,   731,
     722,   722,   722,   705,   410,   881,   738,   722,   722,   731,
     731,   731,   728,   898,    -1,   722,   731,   731,   722,    -1,
     728,    -1,    -1,   722,   731,    -1,   758,   731,   758,   761,
      -1,    -1,   731,    -1,    -1,    -1,    -1,    -1,   758,    -1,
      -1,   761,    -1,    -1,    -1,    -1,   761,   761,    -1,    -1,
      -1,    -1,    -1,    -1,   761,    -1,    -1,   758,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     839,    -1,    -1,    -1,    -1,    -1,    -1,   839,    -1,    -1,
      -1,    -1,    -1,   852,    -1,    -1,    -1,   839,   839,   839,
     852,    -1,    -1,    -1,   839,   839,    -1,    -1,    -1,    -1,
     869,    -1,   839,    -1,    -1,   839,    -1,   869,    -1,    -1,
     839
  };

  const unsigned short int
  Dhcp4Parser::yystos_[] =
  {
       0,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,     0,     5,     7,     9,
     147,   148,   149,   150,   165,   166,   167,   172,     7,   181,
       7,   187,     7,   201,     7,   277,     7,   358,     7,   374,
       7,   310,     7,   316,     7,   340,     7,   253,     7,   426,
     173,   168,   182,   188,   202,   278,   359,   375,   311,   317,
     341,   254,   427,   165,   174,   175,   147,   170,   171,    10,
     123,   132,   133,   134,   147,   180,   183,   184,   185,   453,
     455,   457,   459,    11,    20,    21,    22,    23,    24,    25,
      26,    42,    43,    44,    45,    46,    50,    51,    61,    75,
      76,    88,    91,    98,    99,   102,   180,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   212,   214,   238,   247,
     261,   271,   300,   308,   334,   379,   381,   383,   405,   415,
     416,   424,    12,    13,    16,    19,   199,   200,   203,   205,
     208,   211,     7,    47,    48,    49,    62,    64,    65,    66,
      67,    68,    69,    70,    71,    78,    79,    86,   180,   191,
     192,   193,   196,   279,   280,   281,   283,   285,   287,   289,
     291,   293,   295,   298,   299,   334,   352,   364,   366,   368,
     379,   381,   383,   401,    63,   180,   334,   360,   361,   362,
     364,   366,    76,    80,    81,    82,    83,    84,    85,    87,
     180,   334,   376,   377,   378,   379,   381,   383,   385,   387,
     389,   391,   393,   395,   397,   399,   308,    27,    52,    54,
      55,    58,    59,    60,   180,   228,   318,   319,   320,   321,
     322,   323,   324,   326,   328,   330,   331,   333,    53,    56,
      57,   180,   228,   322,   328,   342,   343,   344,   345,   346,
     348,   349,   350,   351,    89,    90,   180,   255,   256,   257,
     259,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   180,   428,   429,   430,   431,
     433,   435,   436,   438,   439,   440,   443,   445,   446,   447,
     448,   451,     6,     3,     4,     8,     3,   186,   460,   454,
     456,   458,     4,     3,     8,   198,     4,     4,   380,   382,
     384,   213,   215,     4,     4,     4,     4,   272,   309,   335,
     301,   239,   406,   248,   262,     4,   417,   425,     3,     8,
     204,   206,   209,     4,     3,     8,   408,   284,   286,   288,
     353,   365,   367,   282,   290,   292,     4,     4,   296,   294,
     369,   402,     3,     8,   363,     3,     8,   400,   388,   390,
     394,   392,   398,   396,   386,     8,     3,     8,   325,   229,
       4,   329,   327,   332,     4,     8,     3,   347,     4,     4,
       8,     3,   258,   260,     3,     8,     4,   432,   434,     4,
     437,     4,     4,   441,   444,     4,     4,     4,   449,   452,
       3,     8,   165,   165,   147,     4,     4,     4,     4,     4,
     184,     4,   150,   150,     4,     4,     4,     4,     4,   148,
     148,   148,   148,     4,     4,     4,     4,     4,     4,     4,
       4,   148,     4,     4,   190,     4,     4,     4,   150,   200,
      77,   180,   228,   308,   334,   379,   381,   383,   409,   410,
     411,   412,   413,     4,     4,     4,     4,     4,     4,     4,
       4,     4,   148,   150,     4,     4,     4,     4,   280,     4,
     361,     4,     4,     4,     4,     4,     4,     4,     4,   378,
       4,     4,   148,     4,     4,     4,   150,   320,     4,   150,
     150,   344,     4,     4,   256,   150,     4,     4,   148,     4,
     148,   148,     4,     4,   150,   150,   150,     4,     4,   429,
       4,     7,     7,   165,   165,   165,     7,   147,   147,   147,
       7,     7,     5,     5,     5,     5,     5,     5,     5,     7,
       7,     7,     5,   176,    14,    15,   207,    17,    18,   210,
     414,     8,     3,   147,   147,   147,     5,   167,   169,   165,
     147,   147,   147,    72,    73,    74,   297,   147,     5,     7,
     147,   176,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   165,   147,   147,   147,
      15,   117,   442,   118,   119,   120,   121,   122,   150,   450,
     147,   165,   189,   124,   461,   462,   463,   199,    27,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,   180,
     216,   217,   218,   221,   223,   225,   227,   228,   230,   231,
     232,   233,   234,   236,   216,     7,   273,   274,   275,     7,
     312,   313,   314,     7,   336,   337,   338,     7,   302,   303,
     304,    80,    81,    82,    83,    85,   240,   241,   242,   243,
     244,   245,   246,   293,   407,     7,   249,   250,   251,    92,
      93,    94,    95,    96,    97,   263,   264,   265,   266,   267,
     268,   269,   270,   100,   101,   418,   419,   420,   422,   428,
     177,     4,   411,     7,   354,   355,   356,     7,   370,   371,
     372,    87,   403,     8,   464,     3,     8,     8,   219,   222,
     224,   226,     4,     4,     4,     4,     4,   235,   237,     3,
       8,     8,   276,     6,     3,   315,     6,     3,   339,     6,
       3,   305,     6,     3,     3,     6,     3,     6,   252,     6,
       3,     4,     4,     4,     4,     4,     4,     3,     8,   421,
     423,     3,     8,     8,   147,   178,   179,   147,   357,     6,
       3,   373,     6,     3,   404,     8,     4,   462,     4,     4,
       4,     4,   148,   150,   148,   150,   148,     4,     4,   217,
     279,   275,   318,   314,   342,   338,   180,   191,   192,   193,
     196,   228,   271,   289,   293,   295,   306,   307,   334,   379,
     381,   383,   401,   304,   241,   293,   255,   251,   148,   148,
     148,   148,   148,   148,   264,     4,     4,   419,     6,     3,
     360,   356,   376,   372,     4,     5,    28,    29,    30,    31,
     220,   147,   147,   147,   147,   147,     8,     8,     8,     3,
       8,     8,   147,   147,   147,     8,     8,   147,     7,   465,
     466,   307,   467,     3,     6,   125,   127,   128,   180,   228,
     468,   469,   470,   471,   473,   466,   474,     4,   472,     3,
       8,     4,   148,     4,   469,     5,   147,     7,   475,   476,
     477,     3,     6,   126,   129,   130,   131,   478,   479,   480,
     482,   483,   484,   476,   481,     4,     4,     4,     3,     8,
       4,   150,   148,   148,   479,   147
  };

  const unsigned short int
  Dhcp4Parser::yyr1_[] =
  {
       0,   151,   153,   152,   154,   152,   155,   152,   156,   152,
     157,   152,   158,   152,   159,   152,   160,   152,   161,   152,
     162,   152,   163,   152,   164,   152,   165,   165,   165,   165,
     165,   165,   165,   166,   168,   167,   169,   170,   170,   171,
     171,   173,   172,   174,   174,   175,   175,   177,   176,   178,
     178,   179,   179,   180,   182,   181,   183,   183,   184,   184,
     184,   184,   184,   184,   186,   185,   188,   187,   189,   189,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   191,   192,   193,   194,   195,   196,
     198,   197,   199,   199,   200,   200,   200,   200,   202,   201,
     204,   203,   206,   205,   207,   207,   209,   208,   210,   210,
     211,   213,   212,   215,   214,   216,   216,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     219,   218,   220,   220,   220,   220,   222,   221,   224,   223,
     226,   225,   227,   229,   228,   230,   231,   232,   233,   235,
     234,   237,   236,   239,   238,   240,   240,   241,   241,   241,
     241,   241,   242,   243,   244,   245,   246,   248,   247,   249,
     249,   250,   250,   252,   251,   254,   253,   255,   255,   255,
     256,   256,   258,   257,   260,   259,   262,   261,   263,   263,
     264,   264,   264,   264,   264,   264,   265,   266,   267,   268,
     269,   270,   272,   271,   273,   273,   274,   274,   276,   275,
     278,   277,   279,   279,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   282,   281,
     284,   283,   286,   285,   288,   287,   290,   289,   292,   291,
     294,   293,   296,   295,   297,   297,   297,   298,   299,   301,
     300,   302,   302,   303,   303,   305,   304,   306,   306,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   309,   308,   311,   310,   312,   312,
     313,   313,   315,   314,   317,   316,   318,   318,   319,   319,
     320,   320,   320,   320,   320,   320,   320,   320,   321,   322,
     323,   325,   324,   327,   326,   329,   328,   330,   332,   331,
     333,   335,   334,   336,   336,   337,   337,   339,   338,   341,
     340,   342,   342,   343,   343,   344,   344,   344,   344,   344,
     344,   344,   345,   347,   346,   348,   349,   350,   351,   353,
     352,   354,   354,   355,   355,   357,   356,   359,   358,   360,
     360,   361,   361,   361,   361,   361,   363,   362,   365,   364,
     367,   366,   369,   368,   370,   370,   371,   371,   373,   372,
     375,   374,   376,   376,   377,   377,   378,   378,   378,   378,
     378,   378,   378,   378,   378,   378,   378,   378,   378,   380,
     379,   382,   381,   384,   383,   386,   385,   388,   387,   390,
     389,   392,   391,   394,   393,   396,   395,   398,   397,   400,
     399,   402,   401,   404,   403,   406,   405,   407,   407,   408,
     293,   409,   409,   410,   410,   411,   411,   411,   411,   411,
     411,   411,   411,   412,   414,   413,   415,   417,   416,   418,
     418,   419,   419,   421,   420,   423,   422,   425,   424,   427,
     426,   428,   428,   429,   429,   429,   429,   429,   429,   429,
     429,   429,   429,   429,   429,   429,   429,   429,   430,   432,
     431,   434,   433,   435,   437,   436,   438,   439,   441,   440,
     442,   442,   444,   443,   445,   446,   447,   449,   448,   450,
     450,   450,   450,   450,   452,   451,   454,   453,   456,   455,
     458,   457,   460,   459,   461,   461,   462,   464,   463,   465,
     465,   467,   466,   468,   468,   469,   469,   469,   469,   469,
     470,   472,   471,   474,   473,   475,   475,   477,   476,   478,
     478,   479,   479,   479,   479,   481,   480,   482,   483,   484
  };

  const unsigned char
  Dhcp4Parser::yyr2_[] =
  {
       0,     2,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     4,     1,     0,     1,     3,
       5,     0,     4,     0,     1,     1,     3,     0,     4,     0,
       1,     1,     3,     2,     0,     4,     1,     3,     1,     1,
       1,     1,     1,     1,     0,     6,     0,     4,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       0,     6,     1,     3,     1,     1,     1,     1,     0,     4,
       0,     4,     0,     4,     1,     1,     0,     4,     1,     1,
       3,     0,     6,     0,     6,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     4,     1,     1,     1,     1,     0,     4,     0,     4,
       0,     4,     3,     0,     4,     3,     3,     3,     3,     0,
       4,     0,     4,     0,     6,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     6,     0,
       1,     1,     3,     0,     4,     0,     4,     1,     3,     1,
       1,     1,     0,     4,     0,     4,     0,     6,     1,     3,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     0,     6,     0,     1,     1,     3,     0,     4,
       0,     4,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     1,     1,     1,     3,     3,     0,
       6,     0,     1,     1,     3,     0,     4,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     6,     0,     4,     0,     1,
       1,     3,     0,     4,     0,     4,     0,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     0,     4,     0,     4,     0,     4,     1,     0,     4,
       3,     0,     6,     0,     1,     1,     3,     0,     4,     0,
       4,     0,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     4,     1,     1,     3,     3,     0,
       6,     0,     1,     1,     3,     0,     4,     0,     4,     1,
       3,     1,     1,     1,     1,     1,     0,     4,     0,     4,
       0,     4,     0,     6,     0,     1,     1,     3,     0,     4,
       0,     4,     0,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       4,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       4,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       4,     0,     6,     0,     4,     0,     6,     1,     3,     0,
       4,     0,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     4,     3,     0,     6,     1,
       3,     1,     1,     0,     4,     0,     4,     0,     6,     0,
       4,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       4,     0,     4,     3,     0,     4,     3,     3,     0,     4,
       1,     1,     0,     4,     3,     3,     3,     0,     4,     1,
       1,     1,     1,     1,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     6,     1,     3,     1,     0,     6,     1,
       3,     0,     4,     1,     3,     1,     1,     1,     1,     1,
       3,     0,     4,     0,     6,     1,     3,     0,     4,     1,
       3,     1,     1,     1,     1,     0,     4,     3,     3,     3
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Dhcp4Parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\",\"", "\":\"", "\"[\"",
  "\"]\"", "\"{\"", "\"}\"", "\"null\"", "\"Dhcp4\"",
  "\"interfaces-config\"", "\"interfaces\"", "\"dhcp-socket-type\"",
  "\"raw\"", "\"udp\"", "\"outbound-interface\"", "\"same-as-inbound\"",
  "\"use-routing\"", "\"re-detect\"", "\"echo-client-id\"",
  "\"match-client-id\"", "\"next-server\"", "\"server-hostname\"",
  "\"boot-file-name\"", "\"lease-database\"", "\"hosts-database\"",
  "\"type\"", "\"memfile\"", "\"mysql\"", "\"postgresql\"", "\"cql\"",
  "\"user\"", "\"password\"", "\"host\"", "\"port\"", "\"persist\"",
  "\"lfc-interval\"", "\"readonly\"", "\"connect-timeout\"",
  "\"contact-points\"", "\"keyspace\"", "\"valid-lifetime\"",
  "\"renew-timer\"", "\"rebind-timer\"", "\"decline-probation-period\"",
  "\"subnet4\"", "\"4o6-interface\"", "\"4o6-interface-id\"",
  "\"4o6-subnet\"", "\"option-def\"", "\"option-data\"", "\"name\"",
  "\"data\"", "\"code\"", "\"space\"", "\"csv-format\"", "\"always-send\"",
  "\"record-types\"", "\"encapsulate\"", "\"array\"",
  "\"shared-networks\"", "\"pools\"", "\"pool\"", "\"user-context\"",
  "\"comment\"", "\"subnet\"", "\"interface\"", "\"interface-id\"",
  "\"id\"", "\"rapid-commit\"", "\"reservation-mode\"", "\"disabled\"",
  "\"out-of-pool\"", "\"all\"", "\"host-reservation-identifiers\"",
  "\"client-classes\"", "\"test\"", "\"client-class\"", "\"reservations\"",
  "\"duid\"", "\"hw-address\"", "\"circuit-id\"", "\"client-id\"",
  "\"hostname\"", "\"flex-id\"", "\"relay\"", "\"ip-address\"",
  "\"hooks-libraries\"", "\"library\"", "\"parameters\"",
  "\"expired-leases-processing\"", "\"reclaim-timer-wait-time\"",
  "\"flush-reclaimed-timer-wait-time\"", "\"hold-reclaimed-time\"",
  "\"max-reclaim-leases\"", "\"max-reclaim-time\"",
  "\"unwarned-reclaim-cycles\"", "\"dhcp4o6-port\"", "\"control-socket\"",
  "\"socket-type\"", "\"socket-name\"", "\"dhcp-ddns\"",
  "\"enable-updates\"", "\"qualifying-suffix\"", "\"server-ip\"",
  "\"server-port\"", "\"sender-ip\"", "\"sender-port\"",
  "\"max-queue-size\"", "\"ncr-protocol\"", "\"ncr-format\"",
  "\"always-include-fqdn\"", "\"override-no-update\"",
  "\"override-client-update\"", "\"replace-client-name\"",
  "\"generated-prefix\"", "\"tcp\"", "\"JSON\"", "\"when-present\"",
  "\"never\"", "\"always\"", "\"when-not-present\"", "\"Logging\"",
  "\"loggers\"", "\"output_options\"", "\"output\"", "\"debuglevel\"",
  "\"severity\"", "\"flush\"", "\"maxsize\"", "\"maxver\"", "\"Dhcp6\"",
  "\"DhcpDdns\"", "\"Control-agent\"", "TOPLEVEL_JSON", "TOPLEVEL_DHCP4",
  "SUB_DHCP4", "SUB_INTERFACES4", "SUB_SUBNET4", "SUB_POOL4",
  "SUB_RESERVATION", "SUB_OPTION_DEFS", "SUB_OPTION_DEF",
  "SUB_OPTION_DATA", "SUB_HOOKS_LIBRARY", "SUB_DHCP_DDNS",
  "\"constant string\"", "\"integer\"", "\"floating point\"",
  "\"boolean\"", "$accept", "start", "$@1", "$@2", "$@3", "$@4", "$@5",
  "$@6", "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "value", "sub_json",
  "map2", "$@13", "map_value", "map_content", "not_empty_map",
  "list_generic", "$@14", "list_content", "not_empty_list", "list_strings",
  "$@15", "list_strings_content", "not_empty_list_strings",
  "unknown_map_entry", "syntax_map", "$@16", "global_objects",
  "global_object", "dhcp4_object", "$@17", "sub_dhcp4", "$@18",
  "global_params", "global_param", "valid_lifetime", "renew_timer",
  "rebind_timer", "decline_probation_period", "echo_client_id",
  "match_client_id", "interfaces_config", "$@19",
  "interfaces_config_params", "interfaces_config_param", "sub_interfaces4",
  "$@20", "interfaces_list", "$@21", "dhcp_socket_type", "$@22",
  "socket_type", "outbound_interface", "$@23", "outbound_interface_value",
  "re_detect", "lease_database", "$@24", "hosts_database", "$@25",
  "database_map_params", "database_map_param", "database_type", "$@26",
  "db_type", "user", "$@27", "password", "$@28", "host", "$@29", "port",
  "name", "$@30", "persist", "lfc_interval", "readonly", "connect_timeout",
  "contact_points", "$@31", "keyspace", "$@32",
  "host_reservation_identifiers", "$@33",
  "host_reservation_identifiers_list", "host_reservation_identifier",
  "duid_id", "hw_address_id", "circuit_id", "client_id", "flex_id",
  "hooks_libraries", "$@34", "hooks_libraries_list",
  "not_empty_hooks_libraries_list", "hooks_library", "$@35",
  "sub_hooks_library", "$@36", "hooks_params", "hooks_param", "library",
  "$@37", "parameters", "$@38", "expired_leases_processing", "$@39",
  "expired_leases_params", "expired_leases_param",
  "reclaim_timer_wait_time", "flush_reclaimed_timer_wait_time",
  "hold_reclaimed_time", "max_reclaim_leases", "max_reclaim_time",
  "unwarned_reclaim_cycles", "subnet4_list", "$@40",
  "subnet4_list_content", "not_empty_subnet4_list", "subnet4", "$@41",
  "sub_subnet4", "$@42", "subnet4_params", "subnet4_param", "subnet",
  "$@43", "subnet_4o6_interface", "$@44", "subnet_4o6_interface_id",
  "$@45", "subnet_4o6_subnet", "$@46", "interface", "$@47", "interface_id",
  "$@48", "client_class", "$@49", "reservation_mode", "$@50", "hr_mode",
  "id", "rapid_commit", "shared_networks", "$@51",
  "shared_networks_content", "shared_networks_list", "shared_network",
  "$@52", "shared_network_params", "shared_network_param",
  "option_def_list", "$@53", "sub_option_def_list", "$@54",
  "option_def_list_content", "not_empty_option_def_list",
  "option_def_entry", "$@55", "sub_option_def", "$@56",
  "option_def_params", "not_empty_option_def_params", "option_def_param",
  "option_def_name", "code", "option_def_code", "option_def_type", "$@57",
  "option_def_record_types", "$@58", "space", "$@59", "option_def_space",
  "option_def_encapsulate", "$@60", "option_def_array", "option_data_list",
  "$@61", "option_data_list_content", "not_empty_option_data_list",
  "option_data_entry", "$@62", "sub_option_data", "$@63",
  "option_data_params", "not_empty_option_data_params",
  "option_data_param", "option_data_name", "option_data_data", "$@64",
  "option_data_code", "option_data_space", "option_data_csv_format",
  "option_data_always_send", "pools_list", "$@65", "pools_list_content",
  "not_empty_pools_list", "pool_list_entry", "$@66", "sub_pool4", "$@67",
  "pool_params", "pool_param", "pool_entry", "$@68", "user_context",
  "$@69", "comment", "$@70", "reservations", "$@71", "reservations_list",
  "not_empty_reservations_list", "reservation", "$@72", "sub_reservation",
  "$@73", "reservation_params", "not_empty_reservation_params",
  "reservation_param", "next_server", "$@74", "server_hostname", "$@75",
  "boot_file_name", "$@76", "ip_address", "$@77", "duid", "$@78",
  "hw_address", "$@79", "client_id_value", "$@80", "circuit_id_value",
  "$@81", "flex_id_value", "$@82", "hostname", "$@83",
  "reservation_client_classes", "$@84", "relay", "$@85", "relay_map",
  "$@86", "client_classes", "$@87", "client_classes_list", "$@88",
  "client_class_params", "not_empty_client_class_params",
  "client_class_param", "client_class_name", "client_class_test", "$@89",
  "dhcp4o6_port", "control_socket", "$@90", "control_socket_params",
  "control_socket_param", "control_socket_type", "$@91",
  "control_socket_name", "$@92", "dhcp_ddns", "$@93", "sub_dhcp_ddns",
  "$@94", "dhcp_ddns_params", "dhcp_ddns_param", "enable_updates",
  "qualifying_suffix", "$@95", "server_ip", "$@96", "server_port",
  "sender_ip", "$@97", "sender_port", "max_queue_size", "ncr_protocol",
  "$@98", "ncr_protocol_value", "ncr_format", "$@99",
  "always_include_fqdn", "override_no_update", "override_client_update",
  "replace_client_name", "$@100", "replace_client_name_value",
  "generated_prefix", "$@101", "dhcp6_json_object", "$@102",
  "dhcpddns_json_object", "$@103", "control_agent_json_object", "$@104",
  "logging_object", "$@105", "logging_params", "logging_param", "loggers",
  "$@106", "loggers_entries", "logger_entry", "$@107", "logger_params",
  "logger_param", "debuglevel", "severity", "$@108", "output_options_list",
  "$@109", "output_options_list_content", "output_entry", "$@110",
  "output_params_list", "output_params", "output", "$@111", "flush",
  "maxsize", "maxver", YY_NULLPTR
  };

#if PARSER4_DEBUG
  const unsigned short int
  Dhcp4Parser::yyrline_[] =
  {
       0,   234,   234,   234,   235,   235,   236,   236,   237,   237,
     238,   238,   239,   239,   240,   240,   241,   241,   242,   242,
     243,   243,   244,   244,   245,   245,   253,   254,   255,   256,
     257,   258,   259,   262,   267,   267,   278,   281,   282,   285,
     289,   296,   296,   303,   304,   307,   311,   318,   318,   325,
     326,   329,   333,   344,   354,   354,   370,   371,   375,   376,
     377,   378,   379,   380,   383,   383,   398,   398,   407,   408,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,   435,   436,   439,   444,   449,   454,   459,   464,
     470,   470,   481,   482,   485,   486,   487,   488,   491,   491,
     500,   500,   510,   510,   517,   518,   521,   521,   528,   530,
     534,   540,   540,   552,   552,   564,   565,   568,   569,   570,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     583,   583,   590,   591,   592,   593,   596,   596,   604,   604,
     612,   612,   620,   625,   625,   633,   638,   643,   648,   653,
     653,   661,   661,   670,   670,   680,   681,   684,   685,   686,
     687,   688,   691,   696,   701,   706,   711,   716,   716,   726,
     727,   730,   731,   734,   734,   744,   744,   754,   755,   756,
     759,   760,   763,   763,   771,   771,   779,   779,   790,   791,
     794,   795,   796,   797,   798,   799,   802,   807,   812,   817,
     822,   827,   835,   835,   848,   849,   852,   853,   860,   860,
     886,   886,   897,   898,   902,   903,   904,   905,   906,   907,
     908,   909,   910,   911,   912,   913,   914,   915,   916,   917,
     918,   919,   920,   921,   922,   923,   924,   925,   928,   928,
     936,   936,   944,   944,   952,   952,   960,   960,   968,   968,
     976,   976,   984,   984,   991,   992,   993,   996,  1001,  1008,
    1008,  1019,  1020,  1024,  1025,  1028,  1028,  1036,  1037,  1040,
    1041,  1042,  1043,  1044,  1045,  1046,  1047,  1048,  1049,  1050,
    1051,  1052,  1053,  1054,  1061,  1061,  1074,  1074,  1083,  1084,
    1087,  1088,  1093,  1093,  1108,  1108,  1122,  1123,  1126,  1127,
    1130,  1131,  1132,  1133,  1134,  1135,  1136,  1137,  1140,  1142,
    1147,  1149,  1149,  1157,  1157,  1165,  1165,  1173,  1175,  1175,
    1183,  1192,  1192,  1204,  1205,  1210,  1211,  1216,  1216,  1228,
    1228,  1240,  1241,  1246,  1247,  1252,  1253,  1254,  1255,  1256,
    1257,  1258,  1261,  1263,  1263,  1271,  1273,  1275,  1280,  1288,
    1288,  1300,  1301,  1304,  1305,  1308,  1308,  1318,  1318,  1328,
    1329,  1332,  1333,  1334,  1335,  1336,  1339,  1339,  1347,  1347,
    1354,  1354,  1366,  1366,  1376,  1377,  1380,  1381,  1384,  1384,
    1393,  1393,  1402,  1403,  1406,  1407,  1411,  1412,  1413,  1414,
    1415,  1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,  1426,
    1426,  1434,  1434,  1442,  1442,  1450,  1450,  1458,  1458,  1466,
    1466,  1474,  1474,  1482,  1482,  1490,  1490,  1498,  1498,  1506,
    1506,  1519,  1519,  1529,  1529,  1540,  1540,  1550,  1551,  1554,
    1554,  1564,  1565,  1568,  1569,  1572,  1573,  1574,  1575,  1576,
    1577,  1578,  1579,  1582,  1584,  1584,  1596,  1603,  1603,  1613,
    1614,  1617,  1618,  1621,  1621,  1629,  1629,  1639,  1639,  1651,
    1651,  1661,  1662,  1665,  1666,  1667,  1668,  1669,  1670,  1671,
    1672,  1673,  1674,  1675,  1676,  1677,  1678,  1679,  1682,  1687,
    1687,  1695,  1695,  1703,  1708,  1708,  1716,  1721,  1726,  1726,
    1734,  1735,  1738,  1738,  1746,  1751,  1756,  1761,  1761,  1769,
    1772,  1775,  1778,  1781,  1787,  1787,  1797,  1797,  1804,  1804,
    1811,  1811,  1823,  1823,  1836,  1837,  1841,  1845,  1845,  1857,
    1858,  1862,  1862,  1870,  1871,  1874,  1875,  1876,  1877,  1878,
    1881,  1886,  1886,  1894,  1894,  1904,  1905,  1908,  1908,  1916,
    1917,  1920,  1921,  1922,  1923,  1926,  1926,  1934,  1939,  1944
  };

  // Print the state stack on the debug stream.
  void
  Dhcp4Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Dhcp4Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // PARSER4_DEBUG


#line 14 "dhcp4_parser.yy" // lalr1.cc:1167
} } // isc::dhcp
#line 4401 "dhcp4_parser.cc" // lalr1.cc:1167
#line 1949 "dhcp4_parser.yy" // lalr1.cc:1168


void
isc::dhcp::Dhcp4Parser::error(const location_type& loc,
                              const std::string& what)
{
    ctx.error(loc, what);
}
