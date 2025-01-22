%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "globals.h"
#include "intermediate.h"
#include "symtab.h"
#include "error.h"
#include "types.h"
#include "error_recovery.h"
#include "tokens.h"
#include "token_buffer.h"  /* Add token buffer header */
#include "parser_utils.h"  /* Add this include */
#include "validation.h"    /* Add validation header */

/* Global variables */
extern int yylineno;
extern char* yytext;
int debug_parser = 1;  // Enable parser debugging
bool in_subprogram = false; // Track if we're in a subroutine/function

/* Function declarations */
int yylex(void);
void yyerror(const char* msg);
#define DEBUG_PARSER(msg, ...) if(debug_parser) printf("Parser: " msg "\n", ##__VA_ARGS__)

/* Forward declarations */
int check_array_declared(const char* name);
int check_repeat_count(const char* count);
int check_variable_declared(const char* name);
int check_numeric_variables(const char* var1, const char* var2);
void check_program_structure(void);
static void enter_program(const char* name);
static void exit_program(void);

/* Function implementations */
int check_array_declared(const char* name) {
    // TODO: Implement array declaration check
    return 1; // Temporarily return true
}

int check_repeat_count(const char* count) {
    // TODO: Implement repeat count validation
    return 1; // Temporarily return true
}

int check_variable_declared(const char* name) {
    // TODO: Implement variable declaration check
    return 1; // Temporarily return true
}

void check_program_structure(void) {
    if (!in_program) {
        report_error(ERR_SYNTAX, SEV_ERROR, 
            "Missing PROGRAM declaration at start of file", 
            "Every FORT program must begin with: PROGRAM name");
        return;
    }
    if (!program_name) {
        report_error(ERR_SYNTAX, SEV_ERROR, 
            "Program name not specified in PROGRAM declaration",
            "Example: PROGRAM example");
        return;
    }
}

static void enter_program(const char* name) {
    if (in_program) {
        report_error(ERR_SYNTAX, SEV_ERROR, "Multiple PROGRAM declarations found", name);
        return;
    }
    if (!name) {
        report_error(ERR_SYNTAX, SEV_ERROR, "Invalid program name", NULL);
        return;
    }
    in_program = true;
    program_name = strdup(name);
    DEBUG_PARSER("Entered program: %s", name);
}

static void exit_program(void) {
    if (program_name) {
        free(program_name);
        program_name = NULL;
    }
    in_program = false;
    DEBUG_PARSER("Exiting program");
}

void yyerror(const char* msg) {
    char error_msg[256];
    snprintf(error_msg, sizeof(error_msg), "Syntax error at line %d: %s", yylineno, msg);
    error_count++;
    report_error(ERR_SYNTAX, SEV_ERROR, error_msg, NULL);
}
%}

%union {
    int ival;
    double rval;
    char *sval;
    struct {
        double real;
        double imag;
    } complex_val;
    struct {
        char *name;
        int is_array;
        int dimensions;
    } id_info;
}

/* Token declarations */
%token <sval> ICONST RCONST ID STRING_LITERAL
%token INTEGER REAL COMPLEX LOGICAL CHARACTER STRING
%token DATA COMMON
%token IF THEN ELSE ENDIF
%token DO ENDDO
%token READ WRITE
%token CALL RETURN STOP CONTINUE GOTO
%token FUNCTION SUBROUTINE END PROGRAM
%token TRUE FALSE
%token NEWLINE
%token DOLLAR

/* Operators */
%token COMMA ASSIGN
%token OR AND NOT
%token GT LT EQ LE GE NE
%token PLUS MINUS
%token STAR SLASH POWER
%token LPAREN RPAREN
%token LBRACKET RBRACKET

/* Type declarations */
%type <sval> program program_header declarations declaration type
%type <sval> variable_list variable array_reference
%type <sval> statement statements
%type <sval> assignment_statement if_statement do_statement
%type <sval> call_statement io_statement goto_statement
%type <sval> expression logical_expression relation simple_expression
%type <sval> term factor primary
%type <sval> data_list data_item data_values data_value
%type <sval> index_list argument_list
%type <sval> constant

/* Operator precedence */
%left COMMA
%right ASSIGN
%left OR
%left AND
%right NOT
%left GT LT EQ LE GE NE
%left PLUS MINUS
%left STAR SLASH
%right POWER
%right UMINUS
%left LPAREN RPAREN

/* Resolve dangling else */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

/* Resolve expression list conflicts */
%left COMMA_PREC
%left EXPRESSION_LIST

/* Resolve statement conflicts */
%left STATEMENT_END
%left NEWLINE

/* Resolve array reference conflicts */
%left ARRAY_REF
%left INDEX

/* Resolve function call conflicts */
%left FUNCTION_CALL
%left ARGUMENT

/* Resolve DO loop conflicts */
%left DO_CONTROL
%left DO_BODY

/* Start symbol */
%start program

%%
program: program_header declarations statements END
    {
        check_program_structure();
        exit_program();
    }
    | error
    { 
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), 
            "Invalid program structure at line %d. Program must follow format:\n"
            "PROGRAM name\n  declarations\n  statements\nEND", yylineno);
        report_error(ERR_SYNTAX, SEV_ERROR, error_msg, NULL);
        yyerrok; 
    }
    ;

program_header: PROGRAM ID NEWLINE
    {
        enter_program($2);
        free($2);
    }
    ;

declarations: /* empty */
    {
        $$ = strdup("");
    }
    | declarations declaration
    {
        char temp[2048];
        snprintf(temp, sizeof(temp), "%s%s", $1, $2);
        $$ = strdup(temp);
        free($1);
        free($2);
    }
    | declarations declaration NEWLINE
    {
        $$ = $2;
    }
    | declarations error NEWLINE
    {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), 
            "Invalid declaration at line %d. Expected: type variable_list", yylineno);
        report_error(ERR_SYNTAX, SEV_ERROR, error_msg, NULL);
        yyerrok;
        $$ = strdup("");
    }
    ;

statements: statement NEWLINE
    {
        $$ = $1;
    }
    | statements statement NEWLINE
    {
        char temp[2048];
        snprintf(temp, sizeof(temp), "%s%s", $1, $2);
        $$ = strdup(temp);
        free($1);
        free($2);
    }
    | statements error NEWLINE
    {
        report_error(ERR_SYNTAX, SEV_ERROR, "Invalid statement", NULL);
        yyerrok;
        $$ = $1;
    }
    ;

statement: assignment_statement
    {
        $$ = $1;
    }
    | if_statement
    {
        $$ = $1;
    }
    | do_statement
    {
        $$ = $1;
    }
    | call_statement
    {
        $$ = $1;
    }
    | io_statement
    {
        $$ = $1;
    }
    | goto_statement
    {
        $$ = $1;
    }
    | RETURN
    {
        if (!in_subprogram) {
            report_error(ERR_SYNTAX, SEV_ERROR, 
                "RETURN statement outside subroutine/function", NULL);
        }
        $$ = strdup("RETURN");
    }
    | STOP
    {
        $$ = strdup("STOP");
    }
    | CONTINUE
    {
        if (!in_loop) {
            report_error(ERR_SYNTAX, SEV_ERROR,
                "CONTINUE statement outside DO loop", NULL);
        }
        $$ = strdup("CONTINUE");
    }
    | ICONST statement
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s %s", $1, $2);
        $$ = strdup(temp);
        free($1);
        free($2);
    }
    | error
    {
        report_error(ERR_SYNTAX, SEV_ERROR, "Invalid statement", NULL);
        yyerrok;
        $$ = strdup("");
    }
    ;

assignment_statement: variable ASSIGN expression
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s = %s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    ;

variable: ID
    {
        if (!check_variable_declared($1)) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg),
                "Undeclared variable '%s' at line %d", $1, yylineno);
            report_error(ERR_TYPE, SEV_ERROR, error_msg, NULL);
        }
        $$ = $1;
    }
    | array_reference
    {
        $$ = $1;
    }
    ;

expression: logical_expression
    {
        $$ = $1;
    }
    ;

logical_expression: relation
    {
        $$ = $1;
    }
    | logical_expression AND relation
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s .AND. %s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    | logical_expression OR relation
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s .OR. %s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    | NOT relation
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), ".NOT. %s", $2);
        $$ = strdup(temp);
        free($2);
    }
    ;

relation: simple_expression
    {
        $$ = $1;
    }
    | simple_expression GT simple_expression
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s .GT. %s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    | simple_expression LT simple_expression
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s .LT. %s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    | simple_expression GE simple_expression
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s .GE. %s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    | simple_expression LE simple_expression
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s .LE. %s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    | simple_expression EQ simple_expression
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s .EQ. %s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    | simple_expression NE simple_expression
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s .NE. %s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    ;

simple_expression: term
    {
        $$ = $1;
    }
    | simple_expression PLUS term
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s + %s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    | simple_expression MINUS term
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s - %s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    ;

term: factor
    {
        $$ = $1;
    }
    | term STAR factor
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s * %s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    | term SLASH factor
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s / %s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    | term POWER factor
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s ** %s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    ;

factor: primary
    {
        $$ = $1;
    }
    | MINUS primary %prec UMINUS
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "-%s", $2);
        $$ = strdup(temp);
        free($2);
    }
    ;

primary: ICONST
    {
        $$ = $1;
    }
    | RCONST
    {
        $$ = $1;
    }
    | STRING_LITERAL
    {
        $$ = $1;
    }
    | TRUE
    {
        $$ = strdup(".TRUE.");
    }
    | FALSE
    {
        $$ = strdup(".FALSE.");
    }
    | variable
    {
        $$ = $1;
    }
    | LPAREN expression RPAREN
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "(%s)", $2);
        $$ = strdup(temp);
        free($2);
    }
    ;

array_reference: ID LPAREN index_list RPAREN  %prec ARRAY_REF
    {
        if (!check_array_declared($1)) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg),
                "Array '%s' not declared at line %d", $1, yylineno);
            report_error(ERR_TYPE, SEV_ERROR, error_msg, NULL);
        }
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s(%s)", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    ;

index_list: expression
    {
        $$ = $1;
    }
    | index_list COMMA expression
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s,%s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    ;

argument_list: expression
    {
        $$ = $1;
    }
    | argument_list COMMA expression
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s,%s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    ;

goto_statement: GOTO ICONST
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "GOTO %s", $2);
        $$ = strdup(temp);
        free($2);
    }
    | GOTO variable COMMA LPAREN variable_list RPAREN
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "GOTO %s,(%s)", $2, $5);
        $$ = strdup(temp);
        free($2);
        free($5);
    }
    ;

io_statement: READ variable_list
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "READ %s", $2);
        $$ = strdup(temp);
        free($2);
    }
    | READ LPAREN variable_list RPAREN variable_list
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "READ(%s) %s", $3, $5);
        $$ = strdup(temp);
        free($3);
        free($5);
    }
    | WRITE variable_list
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "WRITE %s", $2);
        $$ = strdup(temp);
        free($2);
    }
    | WRITE LPAREN variable_list RPAREN variable_list
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "WRITE(%s) %s", $3, $5);
        $$ = strdup(temp);
        free($3);
        free($5);
    }
    ;

call_statement: CALL ID
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "CALL %s", $2);
        $$ = strdup(temp);
        free($2);
    }
    | CALL ID LPAREN argument_list RPAREN
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "CALL %s(%s)", $2, $4);
        $$ = strdup(temp);
        free($2);
        free($4);
    }
    ;

data_list: data_item
    {
        $$ = $1;
    }
    | data_list COMMA data_item
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s,%s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    ;

data_item: variable SLASH data_values SLASH
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s/%s/", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    ;

data_values: data_value
    {
        $$ = $1;
    }
    | data_values COMMA data_value
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s,%s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    ;

data_value: constant
    {
        $$ = $1;
    }
    | ICONST STAR constant
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s*%s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    ;

constant: ICONST { $$ = $1; }
    | RCONST { $$ = $1; }
    | STRING_LITERAL { $$ = $1; }
    | TRUE { $$ = strdup(".TRUE."); }
    | FALSE { $$ = strdup(".FALSE."); }
    ;

variable_list: variable
    {
        $$ = $1;
    }
    | variable_list COMMA variable
    {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s,%s", $1, $3);
        $$ = strdup(temp);
        free($1);
        free($3);
    }
    ;

if_statement: IF LPAREN logical_expression RPAREN statements ENDIF
    {
        char temp[2048];
        snprintf(temp, sizeof(temp), "IF (%s)\n%sENDIF", $3, $5);
        $$ = strdup(temp);
        free($3);
        free($5);
    }
    | IF LPAREN logical_expression RPAREN THEN statements ENDIF
    {
        char temp[2048];
        snprintf(temp, sizeof(temp), "IF (%s) THEN\n%sENDIF", $3, $6);
        $$ = strdup(temp);
        free($3);
        free($6);
    }
    | IF LPAREN logical_expression RPAREN THEN statements ELSE statements ENDIF
    {
        char temp[2048];
        snprintf(temp, sizeof(temp), "IF (%s) THEN\n%sELSE\n%sENDIF", $3, $6, $8);
        $$ = strdup(temp);
        free($3);
        free($6);
        free($8);
    }
    ;

do_statement: DO variable ASSIGN expression COMMA expression statements ENDDO
    {
        char temp[2048];
        snprintf(temp, sizeof(temp), "DO %s = %s,%s\n%sENDDO", $2, $4, $6, $7);
        $$ = strdup(temp);
        free($2);
        free($4);
        free($6);
        free($7);
    }
    | DO variable ASSIGN expression COMMA expression COMMA expression statements ENDDO
    {
        char temp[2048];
        snprintf(temp, sizeof(temp), "DO %s = %s,%s,%s\n%sENDDO", $2, $4, $6, $8, $9);
        $$ = strdup(temp);
        free($2);
        free($4);
        free($6);
        free($8);
        free($9);
    }
    ;

declaration: type variable_list NEWLINE
    {
        process_declaration($2, current_type);
        $$ = $2;
    }
    | DATA data_list NEWLINE
    {
        $$ = $2;
    }
    | COMMON SLASH ID SLASH variable_list NEWLINE
    {
        process_common_block($3, $5);
        char temp[1024];
        snprintf(temp, sizeof(temp), "COMMON /%s/ %s", $3, $5);
        $$ = strdup(temp);
        free($3);
        free($5);
    }
    | error NEWLINE
    {
        report_error(ERR_SYNTAX, SEV_ERROR, "Invalid declaration", NULL);
        yyerrok;
        $$ = strdup("");
    }
    ;

type: INTEGER 
    { 
        current_type = TYPE_INTEGER; 
        $$ = strdup("INTEGER"); 
    }
    | REAL 
    { 
        current_type = TYPE_REAL; 
        $$ = strdup("REAL"); 
    }
    | COMPLEX 
    { 
        current_type = TYPE_COMPLEX; 
        $$ = strdup("COMPLEX"); 
    }
    | LOGICAL 
    { 
        current_type = TYPE_LOGICAL; 
        $$ = strdup("LOGICAL"); 
    }
    | CHARACTER 
    { 
        current_type = TYPE_CHARACTER; 
        $$ = strdup("CHARACTER"); 
    }
    | STRING 
    { 
        current_type = TYPE_STRING; 
        $$ = strdup("STRING"); 
    }
    ;

%%

int check_numeric_variables(const char* var1, const char* var2) {
    symbol_type_t type1 = get_symbol_type(var1);
    symbol_type_t type2 = get_symbol_type(var2);
    
    return (type1 == TYPE_INTEGER || type1 == TYPE_REAL) &&
           (type2 == TYPE_INTEGER || type2 == TYPE_REAL);
}


