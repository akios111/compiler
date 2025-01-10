/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "fort.y"

#include <stdio.h>
#include <stdlib.h>
#include "intermediate.h"

extern int line_number;
extern FILE* yyin;
extern char* yytext;
extern char line_buf[];
extern int line_pos;
extern int error_count;

int yylex(void);

void yyerror(const char* msg) {
    fprintf(stderr, "\nError at line %d: %s\n", line_number, msg);
    fprintf(stderr, "Line: %s\n", line_buf);
    fprintf(stderr, "      ");
    for(int i = 0; i < line_pos-1; i++) fprintf(stderr, " ");
    fprintf(stderr, "^\n");
    error_count++;
}

/* Συνάρτηση για συγχρονισμό μετά από λάθος */
void sync_to_next_statement() {
    extern int yylex(void);
    int token;
    while ((token = yylex())) {
        if (token == '\n') {
            line_number++;
            break;
        }
    }
}

#define DEBUG(...) printf("DEBUG: " __VA_ARGS__)

#line 109 "fort.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "fort.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_UMINUS = 3,                     /* UMINUS  */
  YYSYMBOL_UPLUS = 4,                      /* UPLUS  */
  YYSYMBOL_LOWER_THAN_ELSE = 5,            /* LOWER_THAN_ELSE  */
  YYSYMBOL_ELSE = 6,                       /* ELSE  */
  YYSYMBOL_PROGRAM = 7,                    /* PROGRAM  */
  YYSYMBOL_FUNCTION = 8,                   /* FUNCTION  */
  YYSYMBOL_SUBROUTINE = 9,                 /* SUBROUTINE  */
  YYSYMBOL_END = 10,                       /* END  */
  YYSYMBOL_COMMON = 11,                    /* COMMON  */
  YYSYMBOL_INTEGER = 12,                   /* INTEGER  */
  YYSYMBOL_REAL = 13,                      /* REAL  */
  YYSYMBOL_COMPLEX = 14,                   /* COMPLEX  */
  YYSYMBOL_LOGICAL = 15,                   /* LOGICAL  */
  YYSYMBOL_CHARACTER = 16,                 /* CHARACTER  */
  YYSYMBOL_STRING = 17,                    /* STRING  */
  YYSYMBOL_LIST = 18,                      /* LIST  */
  YYSYMBOL_DATA = 19,                      /* DATA  */
  YYSYMBOL_CONTINUE = 20,                  /* CONTINUE  */
  YYSYMBOL_GOTO = 21,                      /* GOTO  */
  YYSYMBOL_CALL = 22,                      /* CALL  */
  YYSYMBOL_READ = 23,                      /* READ  */
  YYSYMBOL_WRITE = 24,                     /* WRITE  */
  YYSYMBOL_IF = 25,                        /* IF  */
  YYSYMBOL_THEN = 26,                      /* THEN  */
  YYSYMBOL_ENDIF = 27,                     /* ENDIF  */
  YYSYMBOL_DO = 28,                        /* DO  */
  YYSYMBOL_ENDDO = 29,                     /* ENDDO  */
  YYSYMBOL_STOP = 30,                      /* STOP  */
  YYSYMBOL_RETURN = 31,                    /* RETURN  */
  YYSYMBOL_LABEL = 32,                     /* LABEL  */
  YYSYMBOL_ID = 33,                        /* ID  */
  YYSYMBOL_ICONST = 34,                    /* ICONST  */
  YYSYMBOL_RCONST = 35,                    /* RCONST  */
  YYSYMBOL_STRING_LITERAL = 36,            /* STRING_LITERAL  */
  YYSYMBOL_ASSIGN = 37,                    /* ASSIGN  */
  YYSYMBOL_ADDOP = 38,                     /* ADDOP  */
  YYSYMBOL_MULOP = 39,                     /* MULOP  */
  YYSYMBOL_DIVOP = 40,                     /* DIVOP  */
  YYSYMBOL_POWEROP = 41,                   /* POWEROP  */
  YYSYMBOL_LPAREN = 42,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 43,                    /* RPAREN  */
  YYSYMBOL_COMMA = 44,                     /* COMMA  */
  YYSYMBOL_STAR = 45,                      /* STAR  */
  YYSYMBOL_LBRACK = 46,                    /* LBRACK  */
  YYSYMBOL_RBRACK = 47,                    /* RBRACK  */
  YYSYMBOL_NOT = 48,                       /* NOT  */
  YYSYMBOL_AND = 49,                       /* AND  */
  YYSYMBOL_OR = 50,                        /* OR  */
  YYSYMBOL_GT = 51,                        /* GT  */
  YYSYMBOL_LT = 52,                        /* LT  */
  YYSYMBOL_EQ = 53,                        /* EQ  */
  YYSYMBOL_LE = 54,                        /* LE  */
  YYSYMBOL_TRUE = 55,                      /* TRUE  */
  YYSYMBOL_FALSE = 56,                     /* FALSE  */
  YYSYMBOL_COLON = 57,                     /* COLON  */
  YYSYMBOL_LPAREN_ASSIGN = 58,             /* LPAREN_ASSIGN  */
  YYSYMBOL_59_B_ = 59,                     /* 'B'  */
  YYSYMBOL_YYACCEPT = 60,                  /* $accept  */
  YYSYMBOL_program_unit = 61,              /* program_unit  */
  YYSYMBOL_program = 62,                   /* program  */
  YYSYMBOL_subroutine = 63,                /* subroutine  */
  YYSYMBOL_subroutine_name = 64,           /* subroutine_name  */
  YYSYMBOL_parameter_list = 65,            /* parameter_list  */
  YYSYMBOL_declarations_and_statements = 66, /* declarations_and_statements  */
  YYSYMBOL_declaration = 67,               /* declaration  */
  YYSYMBOL_type = 68,                      /* type  */
  YYSYMBOL_declarator_list = 69,           /* declarator_list  */
  YYSYMBOL_declarator = 70,                /* declarator  */
  YYSYMBOL_array_declarator = 71,          /* array_declarator  */
  YYSYMBOL_statement = 72,                 /* statement  */
  YYSYMBOL_labeled_statement = 73,         /* labeled_statement  */
  YYSYMBOL_unlabeled_statement = 74,       /* unlabeled_statement  */
  YYSYMBOL_assignment_statement = 75,      /* assignment_statement  */
  YYSYMBOL_lvalue = 76,                    /* lvalue  */
  YYSYMBOL_expression = 77,                /* expression  */
  YYSYMBOL_primary = 78,                   /* primary  */
  YYSYMBOL_function_call = 79,             /* function_call  */
  YYSYMBOL_constant = 80,                  /* constant  */
  YYSYMBOL_relational_op = 81,             /* relational_op  */
  YYSYMBOL_if_statement = 82,              /* if_statement  */
  YYSYMBOL_arithmetic_labels = 83,         /* arithmetic_labels  */
  YYSYMBOL_block = 84,                     /* block  */
  YYSYMBOL_do_statement = 85,              /* do_statement  */
  YYSYMBOL_do_control = 86,                /* do_control  */
  YYSYMBOL_array_reference = 87,           /* array_reference  */
  YYSYMBOL_argument_list = 88,             /* argument_list  */
  YYSYMBOL_call_statement = 89,            /* call_statement  */
  YYSYMBOL_goto_statement = 90,            /* goto_statement  */
  YYSYMBOL_label_expr = 91,                /* label_expr  */
  YYSYMBOL_label_list = 92,                /* label_list  */
  YYSYMBOL_read_statement = 93,            /* read_statement  */
  YYSYMBOL_read_list = 94,                 /* read_list  */
  YYSYMBOL_read_item = 95,                 /* read_item  */
  YYSYMBOL_read_args = 96,                 /* read_args  */
  YYSYMBOL_range_spec = 97,                /* range_spec  */
  YYSYMBOL_write_statement = 98,           /* write_statement  */
  YYSYMBOL_write_list = 99,                /* write_list  */
  YYSYMBOL_write_item = 100,               /* write_item  */
  YYSYMBOL_data_list = 101,                /* data_list  */
  YYSYMBOL_data_item = 102,                /* data_item  */
  YYSYMBOL_value_list = 103,               /* value_list  */
  YYSYMBOL_value = 104,                    /* value  */
  YYSYMBOL_range_list = 105,               /* range_list  */
  YYSYMBOL_range_value = 106,              /* range_value  */
  YYSYMBOL_number = 107                    /* number  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   740

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  60
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  144
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  255

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   313


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    59,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    88,    88,    89,    92,    99,   109,   111,   113,   121,
     125,   127,   132,   133,   135,   137,   145,   146,   147,   148,
     150,   159,   160,   161,   162,   163,   164,   167,   169,   173,
     175,   178,   188,   189,   190,   198,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   214,   216,   218,   220,
     228,   229,   232,   233,   235,   237,   239,   241,   243,   245,
     247,   249,   251,   253,   255,   263,   265,   267,   269,   273,
     275,   279,   280,   281,   282,   283,   284,   285,   288,   288,
     288,   288,   290,   292,   294,   296,   298,   306,   308,   313,
     314,   316,   320,   322,   329,   331,   335,   337,   341,   343,
     347,   349,   351,   355,   357,   361,   363,   367,   369,   373,
     375,   377,   379,   387,   389,   393,   395,   399,   401,   403,
     405,   409,   413,   414,   422,   423,   426,   427,   430,   431,
     432,   440,   448,   449,   450,   453,   454,   455,   458,   459,
     462,   465,   466,   467,   468
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "UMINUS", "UPLUS",
  "LOWER_THAN_ELSE", "ELSE", "PROGRAM", "FUNCTION", "SUBROUTINE", "END",
  "COMMON", "INTEGER", "REAL", "COMPLEX", "LOGICAL", "CHARACTER", "STRING",
  "LIST", "DATA", "CONTINUE", "GOTO", "CALL", "READ", "WRITE", "IF",
  "THEN", "ENDIF", "DO", "ENDDO", "STOP", "RETURN", "LABEL", "ID",
  "ICONST", "RCONST", "STRING_LITERAL", "ASSIGN", "ADDOP", "MULOP",
  "DIVOP", "POWEROP", "LPAREN", "RPAREN", "COMMA", "STAR", "LBRACK",
  "RBRACK", "NOT", "AND", "OR", "GT", "LT", "EQ", "LE", "TRUE", "FALSE",
  "COLON", "LPAREN_ASSIGN", "'B'", "$accept", "program_unit", "program",
  "subroutine", "subroutine_name", "parameter_list",
  "declarations_and_statements", "declaration", "type", "declarator_list",
  "declarator", "array_declarator", "statement", "labeled_statement",
  "unlabeled_statement", "assignment_statement", "lvalue", "expression",
  "primary", "function_call", "constant", "relational_op", "if_statement",
  "arithmetic_labels", "block", "do_statement", "do_control",
  "array_reference", "argument_list", "call_statement", "goto_statement",
  "label_expr", "label_list", "read_statement", "read_list", "read_item",
  "read_args", "range_spec", "write_statement", "write_list", "write_item",
  "data_list", "data_item", "value_list", "value", "range_list",
  "range_value", "number", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-188)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-13)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     296,    15,    76,  -188,   329,  -188,  -188,    22,     7,  -188,
      42,  -188,  -188,  -188,  -188,    48,  -188,  -188,     5,  -188,
      46,    53,    -5,    54,    57,    75,  -188,  -188,   534,     2,
      82,  -188,    -3,  -188,  -188,  -188,  -188,  -188,  -188,    73,
    -188,  -188,  -188,  -188,  -188,  -188,    86,  -188,    91,  -188,
      84,    93,  -188,    97,  -188,  -188,   103,   107,   124,  -188,
     110,  -188,   111,   152,   128,  -188,    42,  -188,   152,    12,
     107,   125,  -188,   134,   132,   126,  -188,  -188,   152,    26,
     152,    28,   145,   141,    71,  -188,    24,   139,   140,  -188,
     152,   168,   152,  -188,  -188,   669,  -188,  -188,  -188,  -188,
      45,  -188,   151,   147,   150,   543,   152,   362,   669,  -188,
     669,    79,   172,   126,   152,   134,   669,  -188,   395,    92,
     560,   203,   178,  -188,   -33,  -188,  -188,  -188,   181,  -188,
     104,   152,    96,  -188,  -188,   -18,   168,   577,   -18,   152,
     152,   152,   152,   152,   152,  -188,  -188,  -188,  -188,   152,
     -17,   124,  -188,   173,   519,   594,  -188,  -188,  -188,  -188,
     152,   152,   611,  -188,  -188,   428,   395,   184,  -188,  -188,
    -188,   129,   176,   186,   179,  -188,  -188,   177,  -188,   133,
    -188,   628,  -188,   138,   645,  -188,    77,   194,   194,   194,
     -18,   686,   669,   151,   110,   669,  -188,   196,    42,  -188,
     230,   198,  -188,  -188,   152,   669,   669,  -188,  -188,   461,
    -188,  -188,  -188,    40,  -188,    40,   535,  -188,  -188,   210,
     152,  -188,  -188,   110,   192,   263,   214,   215,   662,  -188,
     213,  -188,  -188,  -188,  -188,   669,  -188,   669,   218,  -188,
    -188,  -188,   221,   222,   152,  -188,   192,   494,   225,   233,
     669,  -188,  -188,  -188,  -188
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    15,     0,     2,     0,     5,     1,     0,     0,     3,
      20,     4,    21,    22,    23,    24,    25,    26,     0,    43,
       0,     0,     0,     0,     0,     0,    44,    45,     0,     0,
       0,    13,     0,    14,    32,    33,    36,    37,    38,     0,
      39,    40,    41,    42,     8,     9,     0,    93,     0,   130,
       0,    18,   128,   106,   105,   103,   100,   115,     0,   116,
     109,   113,     0,     0,     0,    89,    49,    35,     0,     0,
      50,     0,    51,     0,    29,    16,    27,    30,     0,     0,
       0,     0,     0,     0,     0,    64,    65,    71,    72,    73,
       0,     0,     0,    74,    75,   117,    52,    67,    66,    68,
       0,   120,     0,     0,    65,     0,     0,     0,    46,    97,
      98,     0,     0,    17,     0,     0,    47,    10,     0,     0,
       0,     0,     0,   135,     0,   132,   136,   129,     0,   102,
       0,     0,     0,    76,    77,    54,     0,     0,    53,     0,
       0,     0,     0,     0,     0,    78,    79,    80,    81,     0,
       0,     0,   114,     0,     0,     0,    92,    90,    91,    96,
       0,     0,     0,    28,    15,     0,     0,     0,    19,   142,
     141,     0,     0,     0,     0,   137,   131,     0,   107,     0,
     101,     0,    70,     0,     0,    62,    58,    59,    60,    61,
      55,    56,    57,     0,   110,   118,   119,     0,    34,    89,
       0,     0,    82,    85,     0,    99,    48,    31,     7,     0,
      11,   144,   143,     0,   138,     0,     0,   133,   104,     0,
       0,    69,    62,   111,     0,     0,     0,     0,    94,     6,
       0,   140,    73,   134,   108,   121,    73,   126,   122,   124,
      89,    83,     0,     0,     0,   139,     0,     0,     0,     0,
      95,   125,    84,    88,    87
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -188,  -188,  -188,  -188,  -188,  -188,  -114,  -104,  -188,   195,
     144,  -188,  -105,  -188,   -27,  -188,  -188,   -49,  -188,  -188,
     -76,  -188,  -188,  -188,  -187,  -188,  -188,    -4,   -74,  -188,
    -188,  -188,  -188,  -188,  -142,   169,  -188,   119,  -188,  -188,
      27,  -188,   199,  -188,  -160,   171,    85,    74
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     9,    46,   119,     4,    31,    32,    75,
      76,    77,    33,    34,    35,    36,    71,   110,    96,    97,
      98,   149,    37,   203,   107,    38,    65,    99,   111,    40,
      41,    55,   179,    42,    60,    61,   100,   101,    43,   238,
     239,    51,    52,   124,   125,   126,   173,   174
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      39,    67,   158,   157,   165,   123,    49,   176,   194,    95,
     130,   177,   225,    85,   105,    73,    57,   217,    59,   108,
     139,   140,   141,   142,    39,     5,    72,   193,    57,   116,
      74,   120,    44,   145,   146,   147,   148,    58,    50,    68,
      45,   135,   137,   138,    69,   104,    87,    88,    89,   202,
      90,   223,   209,   247,    91,   109,   233,   155,   183,   117,
      92,   131,    87,    88,    89,   162,   132,    93,    94,   118,
     121,    47,    85,   122,   169,   170,     6,     7,   171,    53,
      54,    48,   181,    93,    94,     8,    56,   184,   150,   151,
     186,   187,   188,   189,   190,   191,    62,    85,    59,    63,
     192,   123,   195,    39,   104,    87,    88,    89,    64,    90,
      78,   205,   206,    91,   129,    70,   140,   141,   142,    92,
     158,   157,   159,   160,    81,    85,    93,    94,    79,   104,
      87,    88,    89,    80,    90,   166,   167,    82,    91,   182,
     123,    83,   158,   157,    92,    84,    59,   180,   160,    69,
      39,    93,    94,    85,   102,   228,   103,    86,    87,    88,
      89,    39,    90,   211,   212,   106,    91,    74,   112,    85,
     115,   235,    92,    67,   114,   237,   218,   219,    50,    93,
      94,   221,   160,   128,    57,   104,    87,    88,    89,    59,
      90,   153,   132,    85,    91,   250,    39,   237,   133,   134,
      92,   104,    87,    88,    89,    39,    90,    93,    94,   161,
     136,    87,    88,    89,   175,   178,    92,   210,   197,   121,
     213,    39,   216,    93,    94,   104,    87,    88,   236,   214,
      90,    66,    93,    94,    91,   142,   215,   169,   170,   224,
      92,   171,   227,    39,   234,   121,   242,    93,    94,   243,
      19,    20,    21,    22,    23,    24,   245,   253,    25,   163,
      26,    27,   246,    29,    10,   248,   249,   254,   113,   240,
     196,   152,    30,   251,   226,    12,    13,    14,    15,    16,
      17,   127,    18,    19,    20,    21,    22,    23,    24,   231,
     241,    25,   172,    26,    27,    28,    29,     1,   230,     0,
       0,     0,     0,     0,     0,    30,   -12,     0,   -12,   -12,
     -12,   -12,   -12,   -12,     0,   -12,   -12,   -12,   -12,   -12,
     -12,   -12,     0,     0,   -12,     0,   -12,   -12,   -12,   -12,
      10,     0,     0,     0,     0,     0,     0,     0,   -12,    11,
       0,    12,    13,    14,    15,    16,    17,     0,    18,    19,
      20,    21,    22,    23,    24,     0,     0,    25,     0,    26,
      27,    28,    29,    10,     0,     0,     0,     0,     0,     0,
       0,    30,     0,     0,    12,    13,    14,    15,    16,    17,
       0,    18,    19,    20,    21,    22,    23,    24,     0,     0,
      25,   156,    26,    27,    28,    29,   164,     0,     0,     0,
       0,     0,     0,     0,    30,   -12,     0,   -12,   -12,   -12,
     -12,   -12,   -12,     0,   -12,   -12,   -12,   -12,   -12,   -12,
     -12,     0,     0,   -12,     0,   -12,   -12,   -12,   -12,    10,
       0,     0,     0,     0,     0,     0,     0,   -12,   208,     0,
      12,    13,    14,    15,    16,    17,     0,    18,    19,    20,
      21,    22,    23,    24,     0,     0,    25,     0,    26,    27,
      28,    29,    10,     0,     0,     0,     0,     0,     0,     0,
      30,   229,     0,    12,    13,    14,    15,    16,    17,     0,
      18,    19,    20,    21,    22,    23,    24,     0,     0,    25,
       0,    26,    27,    28,    29,    10,     0,     0,     0,     0,
       0,     0,     0,    30,     0,     0,    12,    13,    14,    15,
      16,    17,     0,    18,    19,    20,    21,    22,    23,    24,
     198,   252,    25,     0,    26,    27,    28,    29,     0,     0,
       0,     0,     0,     0,     0,    66,    30,     0,     0,    19,
      20,    21,    22,    23,    24,   199,     0,    25,     0,    26,
      27,   200,    29,   201,    19,    20,    21,    22,    23,    24,
       0,    30,    25,     0,    26,    27,     0,    29,     0,    87,
      88,   232,     0,     0,     0,     0,    30,   121,     0,     0,
     122,   139,   140,   141,   142,     0,   154,     0,     0,     0,
      93,    94,   143,   144,   145,   146,   147,   148,   139,   140,
     141,   142,     0,   168,     0,     0,     0,     0,     0,   143,
     144,   145,   146,   147,   148,   139,   140,   141,   142,     0,
     185,     0,     0,     0,     0,     0,   143,   144,   145,   146,
     147,   148,   139,   140,   141,   142,     0,     0,   204,     0,
       0,     0,     0,   143,   144,   145,   146,   147,   148,   139,
     140,   141,   142,     0,   207,     0,     0,     0,     0,     0,
     143,   144,   145,   146,   147,   148,   139,   140,   141,   142,
       0,     0,   220,     0,     0,     0,     0,   143,   144,   145,
     146,   147,   148,   139,   140,   141,   142,     0,   222,     0,
       0,     0,     0,     0,   143,   144,   145,   146,   147,   148,
     139,   140,   141,   142,     0,     0,   244,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,     0,   143,   144,
     145,   146,   147,   148,   139,   140,   141,   142,     0,     0,
       0,     0,     0,     0,     0,   143,     0,   145,   146,   147,
     148
};

static const yytype_int16 yycheck[] =
{
       4,    28,   107,   107,   118,    81,     1,    40,   150,    58,
      84,    44,   199,     1,    63,    18,    33,   177,    22,    68,
      38,    39,    40,    41,    28,    10,    30,    44,    33,    78,
      33,    80,    10,    51,    52,    53,    54,    42,    33,    37,
      33,    90,    91,    92,    42,    33,    34,    35,    36,   154,
      38,   193,   166,   240,    42,    43,   216,   106,   132,    33,
      48,    37,    34,    35,    36,   114,    42,    55,    56,    43,
      42,    29,     1,    45,    34,    35,     0,     1,    38,    33,
      34,    33,   131,    55,    56,     9,    33,   136,    43,    44,
     139,   140,   141,   142,   143,   144,    42,     1,   102,    42,
     149,   177,   151,   107,    33,    34,    35,    36,    33,    38,
      37,   160,   161,    42,    43,    33,    39,    40,    41,    48,
     225,   225,    43,    44,    40,     1,    55,    56,    42,    33,
      34,    35,    36,    42,    38,    43,    44,    44,    42,    43,
     216,    44,   247,   247,    48,    42,   150,    43,    44,    42,
     154,    55,    56,     1,    44,   204,    45,    33,    34,    35,
      36,   165,    38,    34,    35,    37,    42,    33,    43,     1,
      44,   220,    48,   200,    42,   224,    43,    44,    33,    55,
      56,    43,    44,    42,    33,    33,    34,    35,    36,   193,
      38,    44,    42,     1,    42,   244,   200,   246,    59,    59,
      48,    33,    34,    35,    36,   209,    38,    55,    56,    37,
      42,    34,    35,    36,    36,    34,    48,    33,    45,    42,
      44,   225,    45,    55,    56,    33,    34,    35,    36,    43,
      38,     1,    55,    56,    42,    41,    57,    34,    35,    43,
      48,    38,    44,   247,    34,    42,    32,    55,    56,    34,
      20,    21,    22,    23,    24,    25,    43,    32,    28,   115,
      30,    31,    44,    33,     1,    44,    44,    34,    73,     6,
     151,   102,    42,   246,    44,    12,    13,    14,    15,    16,
      17,    82,    19,    20,    21,    22,    23,    24,    25,   215,
      27,    28,   121,    30,    31,    32,    33,     1,   213,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    10,    -1,    12,    13,
      14,    15,    16,    17,    -1,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    28,    -1,    30,    31,    32,    33,
       1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    10,
      -1,    12,    13,    14,    15,    16,    17,    -1,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    28,    -1,    30,
      31,    32,    33,     1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    -1,    12,    13,    14,    15,    16,    17,
      -1,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      28,    29,    30,    31,    32,    33,     1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    10,    -1,    12,    13,    14,
      15,    16,    17,    -1,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    28,    -1,    30,    31,    32,    33,     1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    10,    -1,
      12,    13,    14,    15,    16,    17,    -1,    19,    20,    21,
      22,    23,    24,    25,    -1,    -1,    28,    -1,    30,    31,
      32,    33,     1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    10,    -1,    12,    13,    14,    15,    16,    17,    -1,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    28,
      -1,    30,    31,    32,    33,     1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    -1,    12,    13,    14,    15,
      16,    17,    -1,    19,    20,    21,    22,    23,    24,    25,
       1,    27,    28,    -1,    30,    31,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    42,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    -1,    28,    -1,    30,
      31,    32,    33,    34,    20,    21,    22,    23,    24,    25,
      -1,    42,    28,    -1,    30,    31,    -1,    33,    -1,    34,
      35,    36,    -1,    -1,    -1,    -1,    42,    42,    -1,    -1,
      45,    38,    39,    40,    41,    -1,    43,    -1,    -1,    -1,
      55,    56,    49,    50,    51,    52,    53,    54,    38,    39,
      40,    41,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,    52,    53,    54,    38,    39,    40,    41,    -1,
      43,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    38,    39,    40,    41,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    49,    50,    51,    52,    53,    54,    38,
      39,    40,    41,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,    52,    53,    54,    38,    39,    40,    41,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    49,    50,    51,
      52,    53,    54,    38,    39,    40,    41,    -1,    43,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,    52,    53,    54,
      38,    39,    40,    41,    -1,    -1,    44,    38,    39,    40,
      41,    49,    50,    51,    52,    53,    54,    -1,    49,    50,
      51,    52,    53,    54,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    52,    53,
      54
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,    61,    62,    66,    10,     0,     1,     9,    63,
       1,    10,    12,    13,    14,    15,    16,    17,    19,    20,
      21,    22,    23,    24,    25,    28,    30,    31,    32,    33,
      42,    67,    68,    72,    73,    74,    75,    82,    85,    87,
      89,    90,    93,    98,    10,    33,    64,    29,    33,     1,
      33,   101,   102,    33,    34,    91,    33,    33,    42,    87,
      94,    95,    42,    42,    33,    86,     1,    74,    37,    42,
      33,    76,    87,    18,    33,    69,    70,    71,    37,    42,
      42,    40,    44,    44,    42,     1,    33,    34,    35,    36,
      38,    42,    48,    55,    56,    77,    78,    79,    80,    87,
      96,    97,    44,    45,    33,    77,    37,    84,    77,    43,
      77,    88,    43,    69,    42,    44,    77,    33,    43,    65,
      77,    42,    45,    80,   103,   104,   105,   102,    42,    43,
      88,    37,    42,    59,    59,    77,    42,    77,    77,    38,
      39,    40,    41,    49,    50,    51,    52,    53,    54,    81,
      43,    44,    95,    44,    43,    77,    29,    67,    72,    43,
      44,    37,    77,    70,     1,    66,    43,    44,    43,    34,
      35,    38,   105,   106,   107,    36,    40,    44,    34,    92,
      43,    77,    43,    88,    77,    43,    77,    77,    77,    77,
      77,    77,    77,    44,    94,    77,    97,    45,     1,    26,
      32,    34,    72,    83,    44,    77,    77,    43,    10,    66,
      33,    34,    35,    44,    43,    57,    45,   104,    43,    44,
      44,    43,    43,    94,    43,    84,    44,    44,    77,    10,
     106,   107,    36,   104,    34,    77,    36,    77,    99,   100,
       6,    27,    32,    34,    44,    43,    44,    84,    44,    44,
      77,   100,    27,    32,    34
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    60,    61,    61,    62,    62,    63,    63,    63,    64,
      65,    65,    66,    66,    66,    66,    67,    67,    67,    67,
      67,    68,    68,    68,    68,    68,    68,    69,    69,    70,
      70,    71,    72,    72,    72,    73,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    75,    75,    75,    75,
      76,    76,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    78,    78,    78,    78,    79,
      79,    80,    80,    80,    80,    80,    80,    80,    81,    81,
      81,    81,    82,    82,    82,    82,    82,    83,    83,    84,
      84,    84,    85,    85,    86,    86,    87,    87,    88,    88,
      89,    89,    89,    90,    90,    91,    91,    92,    92,    93,
      93,    93,    93,    94,    94,    95,    95,    96,    96,    96,
      96,    97,    98,    98,    99,    99,   100,   100,   101,   101,
     101,   102,   103,   103,   103,   104,   104,   104,   105,   105,
     106,   107,   107,   107,   107
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     2,     2,     7,     6,     2,     1,
       1,     3,     0,     2,     2,     1,     2,     3,     2,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     4,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     5,     1,
       1,     1,     1,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     5,     1,     1,     1,     1,     1,     4,
       3,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     5,     7,     9,     5,     1,     5,     5,     0,
       2,     2,     4,     2,     5,     7,     4,     3,     1,     3,
       2,     5,     4,     2,     6,     1,     1,     1,     3,     2,
       5,     6,     1,     1,     3,     1,     1,     1,     3,     3,
       1,     5,     7,     1,     1,     3,     1,     1,     1,     3,
       1,     4,     1,     3,     4,     1,     1,     2,     3,     5,
       3,     1,     1,     2,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4: /* program: declarations_and_statements END  */
#line 93 "fort.y"
       { 
           DEBUG("program -> declarations_and_statements END\n");
           if (error_count > 0) {
               fprintf(stderr, "\nCompilation completed with %d error(s)\n", error_count);
           }
       }
#line 1491 "fort.tab.c"
    break;

  case 5: /* program: error END  */
#line 100 "fort.y"
       { 
           yyerrok;
           if (error_count > 0) {
               fprintf(stderr, "\nCompilation completed with %d error(s)\n", error_count);
           }
       }
#line 1502 "fort.tab.c"
    break;

  case 6: /* subroutine: SUBROUTINE subroutine_name LPAREN parameter_list RPAREN declarations_and_statements END  */
#line 110 "fort.y"
          { DEBUG("subroutine -> SUBROUTINE ID(params) body END\n"); }
#line 1508 "fort.tab.c"
    break;

  case 7: /* subroutine: SUBROUTINE subroutine_name LPAREN RPAREN declarations_and_statements END  */
#line 112 "fort.y"
          { DEBUG("subroutine -> SUBROUTINE ID() body END\n"); }
#line 1514 "fort.tab.c"
    break;

  case 8: /* subroutine: error END  */
#line 114 "fort.y"
          { 
              yyerrok;
              fprintf(stderr, "Error: Invalid subroutine declaration\n");
              DEBUG("Error recovery in subroutine\n");
          }
#line 1524 "fort.tab.c"
    break;

  case 9: /* subroutine_name: ID  */
#line 122 "fort.y"
               { DEBUG("subroutine_name -> ID\n"); }
#line 1530 "fort.tab.c"
    break;

  case 10: /* parameter_list: ID  */
#line 126 "fort.y"
             { DEBUG("parameter_list -> ID\n"); }
#line 1536 "fort.tab.c"
    break;

  case 11: /* parameter_list: parameter_list COMMA ID  */
#line 128 "fort.y"
             { DEBUG("parameter_list -> parameter_list COMMA ID\n"); }
#line 1542 "fort.tab.c"
    break;

  case 12: /* declarations_and_statements: %empty  */
#line 132 "fort.y"
                         { DEBUG("declarations_and_statements -> empty\n"); }
#line 1548 "fort.tab.c"
    break;

  case 13: /* declarations_and_statements: declarations_and_statements declaration  */
#line 134 "fort.y"
                         { DEBUG("declarations_and_statements -> declarations_and_statements declaration\n"); }
#line 1554 "fort.tab.c"
    break;

  case 14: /* declarations_and_statements: declarations_and_statements statement  */
#line 136 "fort.y"
                         { DEBUG("declarations_and_statements -> declarations_and_statements statement\n"); }
#line 1560 "fort.tab.c"
    break;

  case 15: /* declarations_and_statements: error  */
#line 138 "fort.y"
                         { 
                             yyerrok;
                             yyclearin;
                             DEBUG("Error recovery: skipping to next statement\n");
                         }
#line 1570 "fort.tab.c"
    break;

  case 19: /* declaration: LOGICAL ID LPAREN expression RPAREN  */
#line 149 "fort.y"
          { printf("DEBUG PARSER: Found logical array declaration: %s\n", (yyvsp[-3].sval)); }
#line 1576 "fort.tab.c"
    break;

  case 20: /* declaration: error  */
#line 151 "fort.y"
          { 
              yyerrok;
              yyclearin;
              fprintf(stderr, "Error: Invalid declaration\n");
              DEBUG("Error recovery in declaration\n");
          }
#line 1587 "fort.tab.c"
    break;

  case 21: /* type: INTEGER  */
#line 159 "fort.y"
              { DEBUG("type -> INTEGER\n"); }
#line 1593 "fort.tab.c"
    break;

  case 22: /* type: REAL  */
#line 160 "fort.y"
              { DEBUG("type -> REAL\n"); }
#line 1599 "fort.tab.c"
    break;

  case 23: /* type: COMPLEX  */
#line 161 "fort.y"
              { DEBUG("type -> COMPLEX\n"); }
#line 1605 "fort.tab.c"
    break;

  case 24: /* type: LOGICAL  */
#line 162 "fort.y"
              { DEBUG("type -> LOGICAL\n"); }
#line 1611 "fort.tab.c"
    break;

  case 25: /* type: CHARACTER  */
#line 163 "fort.y"
                { DEBUG("type -> CHARACTER\n"); }
#line 1617 "fort.tab.c"
    break;

  case 26: /* type: STRING  */
#line 164 "fort.y"
              { DEBUG("type -> STRING\n"); }
#line 1623 "fort.tab.c"
    break;

  case 27: /* declarator_list: declarator  */
#line 168 "fort.y"
               { DEBUG("declarator_list -> declarator\n"); }
#line 1629 "fort.tab.c"
    break;

  case 28: /* declarator_list: declarator_list COMMA declarator  */
#line 170 "fort.y"
               { DEBUG("declarator_list -> declarator_list COMMA declarator\n"); }
#line 1635 "fort.tab.c"
    break;

  case 29: /* declarator: ID  */
#line 174 "fort.y"
         { printf("DEBUG PARSER: Found simple declarator ID='%s'\n", (yyvsp[0].sval)); }
#line 1641 "fort.tab.c"
    break;

  case 31: /* array_declarator: ID LPAREN expression RPAREN  */
#line 179 "fort.y"
                { 
                    printf("DEBUG PARSER: Found array declarator: %s\n", (yyvsp[-3].sval));
                    /* Έλεγχος για μη έγκυρη σύνταξη πίνακα */
                    if (error_count > 0) {
                        fprintf(stderr, "Error: Invalid array syntax - FORT only supports () for arrays\n");
                    }
                }
#line 1653 "fort.tab.c"
    break;

  case 34: /* statement: error  */
#line 191 "fort.y"
        { 
            yyerrok;
            yyclearin;
            DEBUG("Error recovery in statement\n");
        }
#line 1663 "fort.tab.c"
    break;

  case 35: /* labeled_statement: LABEL unlabeled_statement  */
#line 199 "fort.y"
                { printf("DEBUG PARSER: Found labeled statement with label %d\n", (yyvsp[-1].ival)); }
#line 1669 "fort.tab.c"
    break;

  case 46: /* assignment_statement: ID ASSIGN expression  */
#line 215 "fort.y"
                   { printf("DEBUG PARSER: Found ID assignment: %s = expr\n", (yyvsp[-2].sval)); }
#line 1675 "fort.tab.c"
    break;

  case 47: /* assignment_statement: array_reference ASSIGN expression  */
#line 217 "fort.y"
                   { printf("DEBUG PARSER: Found array assignment\n"); }
#line 1681 "fort.tab.c"
    break;

  case 48: /* assignment_statement: LPAREN lvalue RPAREN ASSIGN expression  */
#line 219 "fort.y"
                   { printf("DEBUG PARSER: Found parenthesized assignment\n"); }
#line 1687 "fort.tab.c"
    break;

  case 49: /* assignment_statement: error  */
#line 221 "fort.y"
                   { 
                       yyerrok;
                       yyclearin;
                       DEBUG("Error recovery in assignment\n");
                   }
#line 1697 "fort.tab.c"
    break;

  case 53: /* expression: NOT expression  */
#line 234 "fort.y"
         { DEBUG("expression -> NOT expression\n"); }
#line 1703 "fort.tab.c"
    break;

  case 54: /* expression: ADDOP expression  */
#line 236 "fort.y"
         { DEBUG("expression -> ADDOP expression (unary)\n"); }
#line 1709 "fort.tab.c"
    break;

  case 55: /* expression: expression AND expression  */
#line 238 "fort.y"
         { DEBUG("expression -> expression AND expression\n"); }
#line 1715 "fort.tab.c"
    break;

  case 56: /* expression: expression OR expression  */
#line 240 "fort.y"
         { DEBUG("expression -> expression OR expression\n"); }
#line 1721 "fort.tab.c"
    break;

  case 57: /* expression: expression relational_op expression  */
#line 242 "fort.y"
         { DEBUG("expression -> expression relop expression\n"); }
#line 1727 "fort.tab.c"
    break;

  case 58: /* expression: expression ADDOP expression  */
#line 244 "fort.y"
         { DEBUG("expression -> expression ADDOP expression\n"); }
#line 1733 "fort.tab.c"
    break;

  case 59: /* expression: expression MULOP expression  */
#line 246 "fort.y"
         { DEBUG("expression -> expression MULOP expression\n"); }
#line 1739 "fort.tab.c"
    break;

  case 60: /* expression: expression DIVOP expression  */
#line 248 "fort.y"
         { DEBUG("expression -> expression DIVOP expression\n"); }
#line 1745 "fort.tab.c"
    break;

  case 61: /* expression: expression POWEROP expression  */
#line 250 "fort.y"
         { DEBUG("expression -> expression POWEROP expression\n"); }
#line 1751 "fort.tab.c"
    break;

  case 62: /* expression: LPAREN expression RPAREN  */
#line 252 "fort.y"
         { DEBUG("expression -> (expression)\n"); }
#line 1757 "fort.tab.c"
    break;

  case 63: /* expression: LPAREN LPAREN expression RPAREN RPAREN  */
#line 254 "fort.y"
         { DEBUG("expression -> ((expression))\n"); }
#line 1763 "fort.tab.c"
    break;

  case 64: /* expression: error  */
#line 256 "fort.y"
         { 
             yyerrok;
             yyclearin;
             DEBUG("Error recovery in expression\n");
         }
#line 1773 "fort.tab.c"
    break;

  case 65: /* primary: ID  */
#line 264 "fort.y"
       { DEBUG("primary -> ID\n"); }
#line 1779 "fort.tab.c"
    break;

  case 66: /* primary: constant  */
#line 266 "fort.y"
       { DEBUG("primary -> constant\n"); }
#line 1785 "fort.tab.c"
    break;

  case 67: /* primary: function_call  */
#line 268 "fort.y"
       { DEBUG("primary -> function_call\n"); }
#line 1791 "fort.tab.c"
    break;

  case 68: /* primary: array_reference  */
#line 270 "fort.y"
       { DEBUG("primary -> array_reference\n"); }
#line 1797 "fort.tab.c"
    break;

  case 69: /* function_call: ID LPAREN argument_list RPAREN  */
#line 274 "fort.y"
             { printf("DEBUG PARSER: Found function call: %s\n", (yyvsp[-3].sval)); }
#line 1803 "fort.tab.c"
    break;

  case 70: /* function_call: ID LPAREN RPAREN  */
#line 276 "fort.y"
             { printf("DEBUG PARSER: Found function call with no args: %s\n", (yyvsp[-2].sval)); }
#line 1809 "fort.tab.c"
    break;

  case 82: /* if_statement: IF LPAREN expression RPAREN statement  */
#line 291 "fort.y"
           { printf("DEBUG PARSER: Found if statement\n"); }
#line 1815 "fort.tab.c"
    break;

  case 83: /* if_statement: IF LPAREN expression RPAREN THEN block ENDIF  */
#line 293 "fort.y"
           { printf("DEBUG PARSER: Found if-then statement\n"); }
#line 1821 "fort.tab.c"
    break;

  case 84: /* if_statement: IF LPAREN expression RPAREN THEN block ELSE block ENDIF  */
#line 295 "fort.y"
           { printf("DEBUG PARSER: Found if-then-else statement\n"); }
#line 1827 "fort.tab.c"
    break;

  case 85: /* if_statement: IF LPAREN expression RPAREN arithmetic_labels  */
#line 297 "fort.y"
           { printf("DEBUG PARSER: Found arithmetic if statement\n"); }
#line 1833 "fort.tab.c"
    break;

  case 86: /* if_statement: error  */
#line 299 "fort.y"
           { 
               yyerrok;
               yyclearin;
               DEBUG("Error recovery in if statement\n");
           }
#line 1843 "fort.tab.c"
    break;

  case 87: /* arithmetic_labels: ICONST COMMA ICONST COMMA ICONST  */
#line 307 "fort.y"
                { printf("DEBUG PARSER: Found arithmetic if labels: %d,%d,%d\n", (yyvsp[-4].ival), (yyvsp[-2].ival), (yyvsp[0].ival)); }
#line 1849 "fort.tab.c"
    break;

  case 88: /* arithmetic_labels: LABEL COMMA LABEL COMMA LABEL  */
#line 309 "fort.y"
                { printf("DEBUG PARSER: Found arithmetic if with label variables\n"); }
#line 1855 "fort.tab.c"
    break;

  case 89: /* block: %empty  */
#line 313 "fort.y"
     { printf("DEBUG PARSER: Starting empty block\n"); }
#line 1861 "fort.tab.c"
    break;

  case 90: /* block: block declaration  */
#line 315 "fort.y"
     { printf("DEBUG PARSER: Added declaration to block\n"); }
#line 1867 "fort.tab.c"
    break;

  case 91: /* block: block statement  */
#line 317 "fort.y"
     { printf("DEBUG PARSER: Added statement to block\n"); }
#line 1873 "fort.tab.c"
    break;

  case 92: /* do_statement: DO do_control block ENDDO  */
#line 321 "fort.y"
           { printf("DEBUG PARSER: Found complete DO loop\n"); }
#line 1879 "fort.tab.c"
    break;

  case 93: /* do_statement: error ENDDO  */
#line 323 "fort.y"
           { 
               yyerrok;
               DEBUG("Error recovery in do statement\n");
           }
#line 1888 "fort.tab.c"
    break;

  case 94: /* do_control: ID ASSIGN expression COMMA expression  */
#line 330 "fort.y"
         { printf("DEBUG PARSER: Found DO control: %s = expr1, expr2\n", (yyvsp[-4].sval)); }
#line 1894 "fort.tab.c"
    break;

  case 95: /* do_control: ID ASSIGN expression COMMA expression COMMA expression  */
#line 332 "fort.y"
         { printf("DEBUG PARSER: Found DO control with step: %s = expr1, expr2, expr3\n", (yyvsp[-6].sval)); }
#line 1900 "fort.tab.c"
    break;

  case 96: /* array_reference: ID LPAREN argument_list RPAREN  */
#line 336 "fort.y"
               { printf("DEBUG PARSER: Found array reference: %s\n", (yyvsp[-3].sval)); }
#line 1906 "fort.tab.c"
    break;

  case 97: /* array_reference: ID LPAREN RPAREN  */
#line 338 "fort.y"
               { printf("DEBUG PARSER: Found empty array reference: %s\n", (yyvsp[-2].sval)); }
#line 1912 "fort.tab.c"
    break;

  case 98: /* argument_list: expression  */
#line 342 "fort.y"
             { printf("DEBUG PARSER: Found single argument\n"); }
#line 1918 "fort.tab.c"
    break;

  case 99: /* argument_list: argument_list COMMA expression  */
#line 344 "fort.y"
             { printf("DEBUG PARSER: Added another argument\n"); }
#line 1924 "fort.tab.c"
    break;

  case 100: /* call_statement: CALL ID  */
#line 348 "fort.y"
             { DEBUG("call_statement -> CALL ID\n"); }
#line 1930 "fort.tab.c"
    break;

  case 101: /* call_statement: CALL ID LPAREN argument_list RPAREN  */
#line 350 "fort.y"
             { DEBUG("call_statement -> CALL ID(args)\n"); }
#line 1936 "fort.tab.c"
    break;

  case 102: /* call_statement: CALL ID LPAREN RPAREN  */
#line 352 "fort.y"
             { DEBUG("call_statement -> CALL ID()\n"); }
#line 1942 "fort.tab.c"
    break;

  case 103: /* goto_statement: GOTO label_expr  */
#line 356 "fort.y"
             { printf("DEBUG PARSER: Found goto with label\n"); }
#line 1948 "fort.tab.c"
    break;

  case 104: /* goto_statement: GOTO ID COMMA LPAREN label_list RPAREN  */
#line 358 "fort.y"
             { printf("DEBUG PARSER: Found computed goto\n"); }
#line 1954 "fort.tab.c"
    break;

  case 105: /* label_expr: ICONST  */
#line 362 "fort.y"
         { printf("DEBUG PARSER: Found label constant\n"); }
#line 1960 "fort.tab.c"
    break;

  case 106: /* label_expr: ID  */
#line 364 "fort.y"
         { printf("DEBUG PARSER: Found label ID\n"); }
#line 1966 "fort.tab.c"
    break;

  case 107: /* label_list: ICONST  */
#line 368 "fort.y"
         { DEBUG("label_list -> ICONST\n"); }
#line 1972 "fort.tab.c"
    break;

  case 108: /* label_list: label_list COMMA ICONST  */
#line 370 "fort.y"
         { DEBUG("label_list -> label_list,ICONST\n"); }
#line 1978 "fort.tab.c"
    break;

  case 109: /* read_statement: READ read_list  */
#line 374 "fort.y"
             { printf("DEBUG PARSER: Found simple READ\n"); }
#line 1984 "fort.tab.c"
    break;

  case 110: /* read_statement: READ LPAREN read_args RPAREN read_list  */
#line 376 "fort.y"
             { printf("DEBUG PARSER: Found READ with args\n"); }
#line 1990 "fort.tab.c"
    break;

  case 111: /* read_statement: READ LPAREN read_args RPAREN COMMA read_list  */
#line 378 "fort.y"
             { printf("DEBUG PARSER: Found READ with args and comma\n"); }
#line 1996 "fort.tab.c"
    break;

  case 112: /* read_statement: error  */
#line 380 "fort.y"
             { 
                 yyerrok;
                 yyclearin;
                 DEBUG("Error recovery in read statement\n");
             }
#line 2006 "fort.tab.c"
    break;

  case 113: /* read_list: read_item  */
#line 388 "fort.y"
        { DEBUG("read_list -> read_item\n"); }
#line 2012 "fort.tab.c"
    break;

  case 114: /* read_list: read_list COMMA read_item  */
#line 390 "fort.y"
        { DEBUG("read_list -> read_list COMMA read_item\n"); }
#line 2018 "fort.tab.c"
    break;

  case 115: /* read_item: ID  */
#line 394 "fort.y"
        { DEBUG("read_item -> ID\n"); }
#line 2024 "fort.tab.c"
    break;

  case 116: /* read_item: array_reference  */
#line 396 "fort.y"
        { DEBUG("read_item -> array_reference\n"); }
#line 2030 "fort.tab.c"
    break;

  case 117: /* read_args: expression  */
#line 400 "fort.y"
        { printf("DEBUG PARSER: Found read arg expression\n"); }
#line 2036 "fort.tab.c"
    break;

  case 118: /* read_args: read_args COMMA expression  */
#line 402 "fort.y"
        { printf("DEBUG PARSER: Found multiple read args\n"); }
#line 2042 "fort.tab.c"
    break;

  case 119: /* read_args: read_args COMMA range_spec  */
#line 404 "fort.y"
        { printf("DEBUG PARSER: Found read args with range\n"); }
#line 2048 "fort.tab.c"
    break;

  case 120: /* read_args: range_spec  */
#line 406 "fort.y"
        { printf("DEBUG PARSER: Found read range spec\n"); }
#line 2054 "fort.tab.c"
    break;

  case 121: /* range_spec: ID ASSIGN expression COMMA expression  */
#line 410 "fort.y"
         { printf("DEBUG PARSER: Found range: %s = expr1, expr2\n", (yyvsp[-4].sval)); }
#line 2060 "fort.tab.c"
    break;

  case 123: /* write_statement: error  */
#line 415 "fort.y"
               { 
                   yyerrok;
                   yyclearin;
                   DEBUG("Error recovery in write statement\n");
               }
#line 2070 "fort.tab.c"
    break;

  case 130: /* data_list: error  */
#line 433 "fort.y"
        { 
            yyerrok;
            yyclearin;
            DEBUG("Error recovery in data list\n");
        }
#line 2080 "fort.tab.c"
    break;

  case 131: /* data_item: ID DIVOP value_list DIVOP  */
#line 441 "fort.y"
        { 
          extern int in_data; 
          in_data = 0;
          DEBUG("data_item -> ID/value_list/\n"); 
        }
#line 2090 "fort.tab.c"
    break;


#line 2094 "fort.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 471 "fort.y"
