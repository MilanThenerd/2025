/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 53 "genvm-parse.y"

#include "avltrees.h"
#include "genvm.h"
#include "genvm-parse.h"

#define YYERROR_VERBOSE 1

typedef struct id_list {
  struct id_list *next;
  struct id_list **pnext;
  char id[1];
} id_list;

typedef struct operation_info {
  avl_node_t avl;
  const char *name;
  struct id_list *args;
  struct id_list *in, *out, *read;
  int n_in, n_out, n_read;
  int needs_prepare_stack;
  int needs_branch_label;
  int instantiations;
  const char *code;
} operation_info;

typedef struct operation_list {
  struct operation_list *next;
  struct operation_list **pnext;
  operation_info *op;
  id_list *args;
} operation_list;

typedef struct table_info {
  char *name;
  char *entry[256];
  id_list *pool;
} table_info;

#define YYPRINT(fp, tok, val) yyprint (fp, tok, &val)

static int filprintf (Filament *fil, const char *format, ...);
static void yyprint (FILE *fp, int tok, const YYSTYPE *val);
static void yyerror (const char *s);
static operation_info *define_operation (const char *name);
static void set_curr_op_stack (id_list *in, id_list *out);
static operation_list *append_to_operation_list (operation_list *list, const char *name, id_list *args);
static id_list *append_to_id_list (id_list *list, const char *id);
static int emit_id_list (const id_list *list, const char *sep, const char *before, const char *after);
static void set_table (table_info *t, int from, int to, char *value);
static void free_id_list (id_list *list);
static void emit_var_declarations (const char *base, const char *type, int i, int n);
static void emit_var_defines (id_list *list, int sp);
static void emit_operation_invocation (operation_info *op, id_list *args, int sp, int deepest_write);
static void emit_operation_list (operation_list *list);
static void emit_stack_update (int sp, int deepest_write, const char *before, const char *after);
static void free_operation_list (operation_list *list);
static table_info *define_table (char *name);
static void emit_table (table_info *t);
static void free_table (table_info *t);

int counter = 0;
int c_code_on_brace = 0;
int c_args_on_paren = 0;
int errors = 0;
Filament *curr_fil, *out_fil;
operation_info *curr_op, *op_root;
table_info *curr_table;
int from, to;



/* Line 268 of yacc.c  */
#line 143 "genvm-parse.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ID = 258,
     EXPR = 259,
     NUMBER = 260,
     VM_OPERATION = 261,
     VM_TABLE = 262,
     VM_BYTECODE = 263,
     VM_DOTS = 264,
     VM_MINUSMINUS = 265
   };
#endif
/* Tokens.  */
#define ID 258
#define EXPR 259
#define NUMBER 260
#define VM_OPERATION 261
#define VM_TABLE 262
#define VM_BYTECODE 263
#define VM_DOTS 264
#define VM_MINUSMINUS 265




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 127 "genvm-parse.y"

  struct operation_list *oplist;
  struct operation_info *op;
  struct table_info *tab;
  struct id_list *id;
  const char *ctext;
  char *text;
  int num;



/* Line 293 of yacc.c  */
#line 211 "genvm-parse.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 223 "genvm-parse.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   50

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  18
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  44
/* YYNRULES -- Number of states.  */
#define YYNSTATES  70

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   265

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      15,    16,     2,     2,    13,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    17,
       2,    14,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    11,     2,    12,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     9,    12,    13,    14,    21,    23,
      25,    29,    32,    34,    38,    41,    43,    46,    51,    54,
      57,    58,    65,    66,    74,    75,    77,    78,    82,    84,
      85,    88,    91,    93,    94,   100,   102,   103,   104,   105,
     115,   118,   119,   125,   128
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      19,     0,    -1,    19,    20,    -1,    19,    39,    -1,    19,
      28,    -1,    -1,    -1,     7,     3,    21,    11,    22,    12,
      -1,    23,    -1,    24,    -1,    23,    13,    26,    -1,    24,
      26,    -1,    26,    -1,    23,    13,    27,    -1,    24,    27,
      -1,    27,    -1,     5,    14,    -1,     5,     9,     5,    14,
      -1,    25,     3,    -1,    25,    28,    -1,    -1,     8,    38,
      29,    11,    30,    12,    -1,    -1,    30,     3,    31,    15,
      32,    16,    17,    -1,    -1,    33,    -1,    -1,    33,    13,
      34,    -1,    34,    -1,    -1,    35,    36,    -1,    36,     4,
      -1,     4,    -1,    -1,    36,    15,    37,    36,    16,    -1,
       3,    -1,    -1,    -1,    -1,     6,     3,    40,    42,    43,
      41,    11,    44,    12,    -1,    42,     3,    -1,    -1,    15,
      42,    10,    42,    16,    -1,    44,     4,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   155,   155,   156,   157,   159,   163,   162,   168,   169,
     172,   173,   174,   177,   178,   179,   182,   190,   200,   206,
     212,   211,   221,   221,   224,   227,   230,   234,   236,   241,
     241,   246,   248,   251,   250,   256,   259,   264,   266,   263,
     284,   287,   290,   293,   295
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"identifier\"", "\"C expression\"",
  "\"number\"", "\"operation\"", "\"table\"", "\"bytecode\"", "\"..\"",
  "\"--\"", "'{'", "'}'", "','", "'='", "'('", "')'", "';'", "$accept",
  "script", "table", "$@1", "table_items", "table_items.label",
  "table_items.descr", "subscripts", "label", "description", "bytecode",
  "$@2", "operation_list", "$@3", "opt_c_args", "c_args", "c_arg", "$@4",
  "c_arg_body", "$@5", "opt_id", "operation", "$@6", "$@7", "ids",
  "stack_balance", "c_code", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   123,   125,    44,    61,    40,    41,    59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    18,    19,    19,    19,    19,    21,    20,    22,    22,
      23,    23,    23,    24,    24,    24,    25,    25,    26,    27,
      29,    28,    31,    30,    30,    32,    32,    33,    33,    35,
      34,    36,    36,    37,    36,    38,    38,    40,    41,    39,
      42,    42,    43,    44,    44
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     0,     0,     6,     1,     1,
       3,     2,     1,     3,     2,     1,     2,     4,     2,     2,
       0,     6,     0,     7,     0,     1,     0,     3,     1,     0,
       2,     2,     1,     0,     5,     1,     0,     0,     0,     9,
       2,     0,     5,     2,     0
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     1,     0,     0,    36,     2,     4,     3,    37,
       6,    35,    20,    41,     0,     0,     0,     0,    24,    40,
      41,    38,     0,     0,     8,     9,     0,    12,    15,     0,
       0,     0,     0,    16,     7,     0,    11,    14,    18,    19,
      22,    21,    41,    44,     0,    10,    13,     0,     0,     0,
      17,    26,    42,    43,    39,     0,    25,    28,     0,     0,
      29,    32,    30,    23,    27,    31,    33,     0,     0,    34
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     6,    14,    23,    24,    25,    26,    27,    28,
       7,    15,    29,    47,    55,    56,    57,    58,    62,    67,
      12,     8,    13,    31,    16,    21,    49
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -24
static const yytype_int8 yypact[] =
{
     -24,    20,   -24,     5,     6,    29,   -24,   -24,   -24,   -24,
     -24,   -24,   -24,   -24,    12,    23,     2,    28,   -24,   -24,
     -24,   -24,    -3,    24,    25,    28,    22,   -24,   -24,     7,
      21,    26,    30,   -24,   -24,    28,   -24,   -24,   -24,   -24,
     -24,   -24,   -24,   -24,    27,   -24,   -24,    31,    -2,    17,
     -24,    35,   -24,   -24,   -24,    32,    34,   -24,    36,    33,
     -24,   -24,     3,   -24,   -24,   -24,   -24,    36,     0,   -24
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -23,   -22,
      16,   -24,   -24,   -24,   -24,   -24,   -17,   -24,   -18,   -24,
     -24,   -24,   -24,   -24,   -20,   -24,   -24
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -30
static const yytype_int8 yytable[] =
{
      30,    19,    36,    37,    65,    19,    32,    65,     9,    10,
      40,    33,    45,    46,    52,    66,    69,    20,    66,    41,
       2,    53,    48,    17,    19,    38,     3,     4,     5,    54,
       5,    42,    11,    22,    18,    44,    34,    43,    35,   -29,
      61,    50,    39,    64,     0,     0,    51,    60,    59,    68,
      63
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-24))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
      20,     3,    25,    25,     4,     3,     9,     4,     3,     3,
       3,    14,    35,    35,    16,    15,    16,    15,    15,    12,
       0,     4,    42,    11,     3,     3,     6,     7,     8,    12,
       8,    10,     3,     5,    11,     5,    12,    11,    13,     4,
       4,    14,    26,    60,    -1,    -1,    15,    13,    16,    67,
      17
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    19,     0,     6,     7,     8,    20,    28,    39,     3,
       3,     3,    38,    40,    21,    29,    42,    11,    11,     3,
      15,    43,     5,    22,    23,    24,    25,    26,    27,    30,
      42,    41,     9,    14,    12,    13,    26,    27,     3,    28,
       3,    12,    10,    11,     5,    26,    27,    31,    42,    44,
      14,    15,    16,     4,    12,    32,    33,    34,    35,    16,
      13,     4,    36,    17,    34,     4,    15,    37,    36,    16
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:

/* Line 1806 of yacc.c  */
#line 158 "genvm-parse.y"
    { free ((yyvsp[(2) - (2)].text)); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 163 "genvm-parse.y"
    { curr_table = define_table ((yyvsp[(2) - (2)].text)); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 165 "genvm-parse.y"
    { emit_table (curr_table); free_table (curr_table); }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 183 "genvm-parse.y"
    { from = to = (yyvsp[(1) - (2)].num);
	    if (from < 0 || from > 255)
	      {
		yyerror ("invalid table index");
		from = 0, to = -1;
	      }
	   }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 191 "genvm-parse.y"
    { from = (yyvsp[(1) - (4)].num), to = (yyvsp[(3) - (4)].num);
	    if (from < 0 || to < 0 || from > 255 || to > 255)
	      {
		yyerror ("invalid table index");
		from = 0, to = -1;
	      }
	   }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 201 "genvm-parse.y"
    { set_table (curr_table, from, to, (yyvsp[(2) - (2)].text));
	    free ((yyvsp[(2) - (2)].text));
	  }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 207 "genvm-parse.y"
    { set_table (curr_table, from, to, (yyvsp[(2) - (2)].text));
	    free ((yyvsp[(2) - (2)].text));
	  }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 212 "genvm-parse.y"
    { filprintf (out_fil, "%s:\n", (yyvsp[(2) - (2)].text)); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 214 "genvm-parse.y"
    { emit_operation_list ((yyvsp[(5) - (6)].oplist));
	    free_operation_list ((yyvsp[(5) - (6)].oplist));
	    filprintf (out_fil, "  NEXT_BC;\n");
	    (yyval.text) = (yyvsp[(2) - (6)].text);
	  }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 221 "genvm-parse.y"
    { c_args_on_paren = 1; }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 222 "genvm-parse.y"
    { (yyval.oplist) = append_to_operation_list ((yyvsp[(1) - (7)].oplist), (yyvsp[(2) - (7)].text), (yyvsp[(5) - (7)].id)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 224 "genvm-parse.y"
    { (yyval.oplist) = NULL; }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 228 "genvm-parse.y"
    { (yyval.id) = (yyvsp[(1) - (1)].id); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 230 "genvm-parse.y"
    { (yyval.id) = NULL; }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 235 "genvm-parse.y"
    { (yyval.id) = append_to_id_list ((yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].text)); free ((yyvsp[(3) - (3)].text)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 237 "genvm-parse.y"
    { (yyval.id) = append_to_id_list (NULL, (yyvsp[(1) - (1)].text)); free ((yyvsp[(1) - (1)].text)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 241 "genvm-parse.y"
    { curr_fil = filnew (NULL, 0); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 243 "genvm-parse.y"
    { (yyval.text) = fildelete (curr_fil); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 247 "genvm-parse.y"
    { filcat (curr_fil, (yyvsp[(2) - (2)].text)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 249 "genvm-parse.y"
    { filcat (curr_fil, (yyvsp[(1) - (1)].text)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 251 "genvm-parse.y"
    { filcat (curr_fil, "("); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 253 "genvm-parse.y"
    { filcat (curr_fil, ")"); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 257 "genvm-parse.y"
    { (yyval.text) = (yyvsp[(1) - (1)].text); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 259 "genvm-parse.y"
    { asprintf (&(yyval.text), "label%d", ++counter); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 264 "genvm-parse.y"
    { curr_op = define_operation ((yyvsp[(2) - (2)].text)); c_code_on_brace = true; }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 266 "genvm-parse.y"
    { curr_fil = filnew (NULL, 0);
	    filprintf (curr_fil, "#line %d \"vm.def\"\n      ", yylineno + 1);
	  }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 270 "genvm-parse.y"
    { filprintf (curr_fil, "\n#line __oline__ \"vm.inl\"");
	    if (curr_op)
	      {
	        curr_op->args = (yyvsp[(4) - (9)].id);
	        curr_op->code = fildelete (curr_fil);
	        curr_op->needs_prepare_stack = strstr(curr_op->code,
					              "PREPARE_STACK") != NULL;
	        curr_op->needs_branch_label = strstr(curr_op->code,
					             "BRANCH_LABEL") != NULL;
	      }
	    (yyval.op) = curr_op;
	  }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 285 "genvm-parse.y"
    { (yyval.id) = append_to_id_list ((yyvsp[(1) - (2)].id), (yyvsp[(2) - (2)].text)); free ((yyvsp[(2) - (2)].text)); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 287 "genvm-parse.y"
    { (yyval.id) = NULL; }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 291 "genvm-parse.y"
    { set_curr_op_stack ((yyvsp[(2) - (5)].id), (yyvsp[(4) - (5)].id)); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 294 "genvm-parse.y"
    { filcat (curr_fil, (yyvsp[(2) - (2)].text)); }
    break;



/* Line 1806 of yacc.c  */
#line 1750 "genvm-parse.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 298 "genvm-parse.y"


void yyprint (FILE *fp, int tok, const YYSTYPE *val)
{
  switch (tok)
    {
      case NUMBER:
        fprintf (fp, "%d", (val)->num);
        break;
      case ID:
      case EXPR:
        fprintf (fp, "%s", (val)->text);
        break;
    }
}

void
yyerror (const char *s)
{
  errors = 1;
  fprintf (stderr, "vm.def:%d: %s\n", yylineno, s);
}

/* Define an operation that is used into a bytecode declaration.
   We use an AVL tree to store them so that we can do fast
   searches, and we can detect duplicates.  */
operation_info *
define_operation (const char *name)
{
  avl_node_t **p = (avl_node_t **) &op_root;
  operation_info *node;
  operation_info *operation = NULL;

  while (*p)
    {
      int cmp;
      operation = (operation_info *) *p;

      cmp = strcmp(name, operation->name);
      if (cmp < 0)
        p = &(*p)->avl_left;
      else if (cmp > 0)
        p = &(*p)->avl_right;
      else
        {
          yyerror ("duplicate operation");
          return NULL;
        }
    }

  node = (operation_info *) calloc(1, sizeof(operation_info));
  node->avl.avl_parent = (avl_node_t *) operation;
  node->avl.avl_left = node->avl.avl_right = NULL;
  node->name = name;
  *p = &(node->avl);

  avl_rebalance(&node->avl, (avl_node_t **) &op_root);
  return node;
}

operation_info *
get_operation (const char *name)
{
  avl_node_t **p = (avl_node_t **) &op_root;
  operation_info *operation = NULL;

  while (*p)
    {
      int cmp;
      operation = (operation_info *) *p;

      cmp = strcmp(name, operation->name);
      if (cmp < 0)
        p = &(*p)->avl_left;
      else if (cmp > 0)
        p = &(*p)->avl_right;
      else
        return operation;
    }

  return NULL;
}

operation_list *
append_to_operation_list (operation_list *list, const char *name, id_list *args)
{
  operation_list *node, *result;
  operation_info *op = get_operation (name);

  if (!op)
    {
      yyerror ("undefined operation");
      return list;
    }

  node = calloc (1, sizeof (operation_list));
  if (!list)
    result = node;
  else
    {
      result = list;
      *(result->pnext) = node;
    }

  node->op = op;
  node->args = args;
  result->pnext = &(node->next);
  return result;
}

id_list *
append_to_id_list (id_list *list, const char *name)
{
  int len = strlen (name);
  id_list *node = calloc (1, sizeof (id_list) + len);
  id_list *result;
  memcpy (node->id, name, len);

  if (!list)
    result = node;
  else
    {
      result = list;
      *(result->pnext) = node;
    }

  result->pnext = &(node->next);
  return result;
}

int
emit_id_list (const id_list *list, const char *sep, const char *before, const char *after)
{
  int n = 0;
  if (!list)
    return 0;

  filcat (out_fil, before);
  while (list)
    {
      filcat (out_fil, list->id);
      list = list->next;
      filcat (out_fil, list ? sep : after);
      n++;
    }

  return n;
}

void
free_id_list (id_list *list)
{
  id_list *next;
  while (list)
    {
      next = list->next;
      free (list);
      list = next;
    }
}

void
set_curr_op_stack (id_list *in, id_list *out)
{
  id_list *read = in;
  id_list *list;
  id_list *last_read = NULL;
  if (!curr_op)
    return;

  /* Find the names that are a common prefix for the input and output
     stack, and move them to the READ list.  */
  while (in && out && !strcmp (in->id, out->id))
    {
      id_list *next;
      next = out->next;
      free (out);
      out = next;

      /* The in nodes are still referenced through READ. */
      last_read = in;
      in = in->next;
    }

  if (last_read)
    {
      if (in)
        in->pnext = read->pnext;
      last_read->next = NULL;
      read->pnext = &(last_read->next);
    }
  else
    /* The slots are all read and written, none is just read.  */
    read = NULL;

  curr_op->in = in;
  curr_op->out = out;
  curr_op->read = read;

  /* Compute and save the count, we'll use it to combine the
     stack effects.  */
  for (curr_op->n_in = 0, list = curr_op->in; list; list = list->next, curr_op->n_in++);
  for (curr_op->n_out = 0, list = curr_op->out; list; list = list->next, curr_op->n_out++);
  for (curr_op->n_read = 0, list = curr_op->read; list; list = list->next, curr_op->n_read++);
}

void
free_operation_list (operation_list *list)
{
  operation_list *next;
  while (list)
    {
      next = list->next;
      free_id_list (list->args);
      free (list);
      list = next;
    }
}

void
emit_var_declarations (const char *base, const char *type, int first, int n)
{
  int i;
  if (!n)
    return;

  for (i = first; n--; i++)
    filprintf (out_fil, "%s %s%d", i == first ? type : ",", base, i);

  filprintf (out_fil, ";\n");
}

void
emit_var_defines (id_list *list, int sp)
{
  for (; list; list = list->next, sp++)
    filprintf (out_fil, "#define %s %s%d\n", list->id,
	       (sp <= 0 ? "_stack" : "_extra"), (sp < 0 ? -sp : sp));
}

void
emit_operation_list (operation_list *oplist)
{
  operation_list *list;
  int deepest_read = 0, deepest_write = 0, sp,
      max_height = 0, deepest_write_so_far = 0, i;

  if (!oplist)
    return;

  filprintf (out_fil, "  {\n");

  /* Compute the overall effect on the stack of the superoperator.
     The number of elements that are read is usually op->n_read + op->n_in,
     but in the presence of many operations we need to adjust this for
     the balance left by previous operations; the same also holds for
     the number of written elements, which is usually op->in.

     We also track the maximum height of the stack which is the number
     of _EXTRA variables that we need.  */
  for (sp = 0, list = oplist; list; list = list->next)
    {
      operation_info *op = list->op;
      int balance = op->n_out - op->n_in;
      deepest_read = MAX (deepest_read, op->n_read + op->n_in - sp);
      deepest_write = MAX (deepest_write, op->n_in - sp);
      sp += balance;
      max_height = MAX (max_height, sp);
    }

  /* Declare the variables.  */
  emit_var_declarations ("_stack", "    OOP", 0, deepest_read);
  emit_var_declarations ("_extra", "    OOP", 1, max_height);

  /* Read the input items from the stack.  */
  for (i = deepest_read; --i >= 0; )
    filprintf (out_fil, "    _stack%d = STACK_AT (%d);\n", i, i);

  /* We keep track of the stack effect here too: we pass the current balance
     to emit_operation_invocation so that it can assign the correct
     _STACK/_EXTRA variables to the operands of OP, and we pass the
     number of dirty _STACK variables (tracked by DEEPEST_WRITE_SO_FAR)
     so that it can emit the PREPARE_STACK macro if necessary.  */
  for (sp = 0, list = oplist; list; list = list->next)
    {
      operation_info *op = list->op;
      int balance = op->n_out - op->n_in;
      emit_operation_invocation (op, list->args, sp, deepest_write_so_far);
      deepest_write_so_far = MAX (deepest_write_so_far, op->n_in - sp);
      sp += balance;
    }

  /* Write back to the stack the items that were overwritten, and
     emit pushes/pops if the height of the stack has changed.  */
  emit_stack_update (sp, deepest_write, "    ", ";");

  filprintf (out_fil, "  }\n");
}

void
emit_operation_invocation (operation_info *op, struct id_list *args, int sp, int deepest_write)
{
  id_list *list, *actual;

  filprintf (out_fil, "    /* %s (", op->name);
  emit_id_list (op->read, " ", " ", " |");
  emit_id_list (op->in, " ", " ", "");
  filprintf (out_fil, " --");
  emit_id_list (op->read, " ", " ", " |");
  emit_id_list (op->out, " ", " ", "");
  filprintf (out_fil, " ) */\n");

  filprintf (out_fil, "    do\n");
  filprintf (out_fil, "      {\n");

  /* Evaluate the arguments.  */
  for (list = op->args, actual = args; list && actual;
       list = list->next, actual = actual->next)
    filprintf (out_fil, "\tint %s = %s;\n", list->id, actual->id);

  if (list)
    yyerror ("too few parameters");
  if (actual)
    yyerror ("too many parameters");

  if (op->needs_prepare_stack)
    {
      filprintf (out_fil, "#define PREPARE_STACK() do { \\\n");
      emit_stack_update (sp, deepest_write, "    ", "; \\");
      filprintf (out_fil, "  } while (0)\n");
    }

  if (op->needs_branch_label)
    filprintf (out_fil,
	       "#define BRANCH_LABEL(local_label) %s_%d_##local_label\n",
	       op->name, op->instantiations++);

  /* Associate the temporary variables to the stack positions.  */
  emit_var_defines (op->read, sp - op->n_read - op->n_in + 1);
  emit_var_defines (op->in, sp - op->n_in + 1);
  emit_var_defines (op->out, sp - op->n_in + 1);

  filprintf (out_fil, "%s\n", op->code);

  /* Leave things clean.  */
  if (op->needs_branch_label)
    filprintf (out_fil, "#undef BRANCH_LABEL\n\n");

  if (op->needs_prepare_stack)
    filprintf (out_fil, "#undef PREPARE_STACK\n");

  emit_id_list (op->read, "\n#undef ", "#undef ", "\n");
  emit_id_list (op->in, "\n#undef ", "#undef ", "\n");
  emit_id_list (op->out, "\n#undef ", "#undef ", "\n");

  filprintf (out_fil, "      }\n");
  filprintf (out_fil, "    while (0);\n");
}

void
emit_stack_update (int sp, int deepest_write, const char *before, const char *after)
{
  int i;

  for (i = deepest_write; --i >= MAX(0, -sp); )
    filprintf (out_fil, "%sSTACK_AT (%d) = _stack%d%s\n", before, i, i, after);

  /* Either pop the input items in excess, or push the output ones.  */
  if (sp < 0)
    filprintf (out_fil, "%sPOP_N_OOPS (%d)%s\n", before, -sp, after);
  else
    for (i = 1; i <= sp; i++)
      filprintf (out_fil, "%sPUSH_OOP (_extra%d)%s\n", before, i, after);
}
 
void
emit_table (table_info *t)
{
  int i;
  printf ("  static void *%s[256] = {\n", t->name);
  for (i = 0; i < 256; i += 4)
    printf ("    %s%s, %s%s, %s%s, %s%s%c  /* %3d */\n",
	    t->entry[i] ? "&&" : "",
	    t->entry[i] ? t->entry[i] : "NULL",
	    t->entry[i+1] ? "&&" : "",
	    t->entry[i+1] ? t->entry[i+1] : "NULL",
	    t->entry[i+2] ? "&&" : "",
	    t->entry[i+2] ? t->entry[i+2] : "NULL",
	    t->entry[i+3] ? "&&" : "",
	    t->entry[i+3] ? t->entry[i+3] : "NULL",
	    i + 4 == 256 ? ' ' : ',', i);

  printf ("  };\n\n");
}

table_info *
define_table (char *name)
{
  table_info *t = (table_info *) calloc(1, sizeof(table_info));
  t->name = name;

  return t;  
}

void
set_table (table_info *t, int from, int to, char *value)
{
  int i;
  id_list **p_last = t->pool ? t->pool->pnext : &t->pool;
  id_list *last;
  t->pool = append_to_id_list (t->pool, value);
  last = *p_last;

  for (i = from; i <= to; i++)
    t->entry[i] = last->id;
}

void
free_table (table_info *t)
{
  free_id_list (t->pool);
  free (t->name);
  free (t);
}

int
filprintf (Filament *fil, const char *format, ...)
{
  va_list ap;
  STREAM *out = stream_new (fil, SNV_UNLIMITED, NULL, snv_filputc);
  int result;
  va_start (ap, format);
  result = stream_vprintf (out, format, ap);
  va_end (ap);
  stream_delete (out);
  return result;
}

int
main ()
{
  char *code;
  errors = 0;

  printf ("/* Automatically generated by genvm, do not edit!  */\n");

  out_fil = filnew (NULL, 0);
  if (yyparse () || errors)
    exit (1);

  printf ("  goto jump_around;\n");
  code = fildelete (out_fil);
  fputs (code, stdout);
  printf ("jump_around:\n  ;\n");

  exit (0);
}

