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
#include "token_buffer.h"
#include "parser_utils.h"
#include "validation.h"

/* Global variables */
extern int yylineno;
extern char* yytext;
int debug_parser = 1;  // Enable parser debugging
extern bool in_subprogram;  // Track if we're in a subroutine/function
extern int error_count;  // Track number of errors
extern type_t current_type;
extern int in_declaration_context;

/* Function declarations */
int yylex(void);
void yyerror(const char* msg);
#define DEBUG_PARSER(msg, ...) if(debug_parser) printf("Parser: " msg "\n", ##__VA_ARGS__)

/* Error reporting helper */
void report_context_error(error_type_t type, error_severity_t severity, const char* msg, const char* context) {
    char error_msg[512];
    snprintf(error_msg, sizeof(error_msg), "Line %d: %s\nContext: %s", 
             yylineno, msg, context ? context : "Not available");
    report_error(type, severity, error_msg, NULL);
}

/* Forward declarations */
int check_array_declared(const char* name);
extern int check_array_dimension(const char* dim);
int check_variable_declared(const char* name);
int check_numeric_variables(const char* var1, const char* var2);
void check_program_structure(void);
void exit_program(void);
int validate_array_index(const char* array_name, const char* index_expr);
void validate_array_indices(const char* array_name, const char* indices);
void validate_function_args(const char* func_name, const char* args);

/* Function implementations */
int check_array_declared(const char* name) {
    if (!lookup_symbol(name)) {
        report_error(ERR_UNDEFINED, SEV_ERROR, "Array not declared", name);
    }
    return 1;
}

int check_variable_declared(const char* name) {
    if (!lookup_symbol(name)) {
        report_error(ERR_UNDEFINED, SEV_ERROR, "Variable not declared", name);
    }
    return 1;
}

void check_program_structure(void) {
    if (!program_name && !in_subprogram) {
        report_error(ERR_SYNTAX, SEV_ERROR, 
            "Program name not specified in PROGRAM declaration",
            "Example: PROGRAM example");
        return;
    }
    if (in_program && in_subprogram) {
        report_error(ERR_SYNTAX, SEV_ERROR,
            "Cannot mix PROGRAM with FUNCTION/SUBROUTINE",
            "PROGRAM and FUNCTION/SUBROUTINE must be in separate files");
        return;
    }
}

void exit_program(void) {
    if (program_name) {
        free(program_name);
        program_name = NULL;
    }
    in_program = false;
    DEBUG_PARSER("Exiting program");
}

void yyerror(const char* msg) {
    char context[256];
    snprintf(context, sizeof(context), "Near token: %s", yytext);
    report_context_error(ERR_SYNTAX, SEV_ERROR, msg, context);
}

int validate_array_index(const char* array_name, const char* index_expr) {
    symbol_t* sym = lookup_symbol(index_expr);
    if (sym) {
        if (sym->type != TYPE_INTEGER) {
            report_error(ERR_TYPE, SEV_ERROR, "Array index must be INTEGER", index_expr);
            return 0;
        }
    }
    return 1;
}
%}

%union {
    char* sval;
    int ival;
    double rval;
    int bval;
    struct {
        char* text;
        type_t type;
        symbol_kind_t kind;
    } expr;
}

%token <sval> ID STRING_LITERAL
%token <ival> ICONST
%token <rval> RCONST
%token <bval> BCONST
%token PROGRAM END INTEGER REAL COMPLEX LOGICAL CHARACTER STRING
%token DATA COMMON IF THEN ELSE ENDIF DO ENDDO READ WRITE
%token CALL RETURN STOP CONTINUE GOTO FUNCTION SUBROUTINE DOLLAR
%token TRUE FALSE ASSIGN PLUS MINUS STAR SLASH POWER
%token AND OR NOT GT LT GE LE EQ NE
%token LPAREN RPAREN LBRACKET RBRACKET COMMA COLON NEWLINE

%left OR
%left AND
%right NOT
%left GT LT GE LE EQ NE
%left PLUS MINUS
%left STAR SLASH
%right POWER
%right UMINUS
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%nonassoc ARRAY_ACCESS
%nonassoc FUNCTION_CALL

/* Declare types for non-terminals */
%type <expr> expression term factor primary var_reference
%type <sval> expression_list program program_header declarations declaration type vars
%type <sval> statements statement labeled_statement unlabeled_statement function_argument_list
%type <sval> simple_statement compound_statement assignment_statement if_statement
%type <sval> logical_expression logical_term logical_factor relational_expression
%type <sval> value value_list common_block data_stmt data_items data_item
%type <sval> if_then_statement if_then_else_statement if_logical_statement if_computed_statement
%type <sval> computed_expression goto_statement label_list label
%type <sval> call_statement io_statement io_list io_item
%type <sval> loop_statement

%%

program
    : program_header declarations statements END
    {
        check_program_structure();
        exit_program();
    }
    ;

program_header
    : PROGRAM ID NEWLINE
    {
        $$ = $2;  /* Use the ID's value */
        program_name = strdup($2);
        in_program = true;
        set_program_declaration();  // This sets both in_program and has_program
        set_error_context(CONTEXT_PROGRAM);
        DEBUG_PARSER("Program header: %s", $2);
    }
    | SUBROUTINE ID NEWLINE
    {
        $$ = $2;  /* Use the ID's value */
        in_subprogram = true;
        set_error_context(CONTEXT_PROGRAM);
        DEBUG_PARSER("Subroutine: %s", $2);
    }
    | FUNCTION ID NEWLINE
    {
        $$ = $2;  /* Use the ID's value */
        in_subprogram = true;
        set_error_context(CONTEXT_PROGRAM);
        DEBUG_PARSER("Function: %s", $2);
    }
    | error NEWLINE
    {
        check_program_declaration();  // Use centralized error handling
    }
    ;

declarations
    : /* empty */ { $$ = strdup(""); }
    | declarations declaration { $$ = $2; }
    | declarations common_block { $$ = $2; }
    | declarations data_stmt { $$ = $2; }
    ;

declaration
    : type { 
        in_declaration_context = true; 
        set_error_context(CONTEXT_DECLARATION);
    } vars NEWLINE { 
        in_declaration_context = false; 
        set_error_context(CONTEXT_PROGRAM);
        $$ = $3;
    }
    ;

type
    : INTEGER { current_type = TYPE_INTEGER; $$ = strdup("INTEGER"); }
    | REAL { current_type = TYPE_REAL; $$ = strdup("REAL"); }
    | COMPLEX { current_type = TYPE_COMPLEX; $$ = strdup("COMPLEX"); }
    | LOGICAL { current_type = TYPE_LOGICAL; $$ = strdup("LOGICAL"); }
    | CHARACTER { current_type = TYPE_CHARACTER; $$ = strdup("CHARACTER"); }
    | STRING { current_type = TYPE_STRING; $$ = strdup("STRING"); }
    ;

vars
    : vars COMMA var_reference { 
        $$ = strdup($3.text);
    }
    | var_reference { 
        $$ = strdup($1.text);
    }
    ;

var_reference
    : ID LPAREN expression RPAREN
    {
        symbol_t* sym = lookup_symbol($1);
        if (!sym) {
            yyerror("Undefined identifier");
            $$.type = TYPE_ERROR;
        } else if (sym->kind == SYMBOL_ARRAY) {
            // This is an array access
            char buf[1024];
            snprintf(buf, sizeof(buf), "%s[%s]", $1, $3.text);
            $$.text = strdup(buf);
            $$.type = sym->type;
            $$.kind = SYMBOL_ARRAY;
           } else {
            yyerror("Invalid use of identifier");
            $$.type = TYPE_ERROR;
        }
    }
    | ID LPAREN expression_list RPAREN
    {
        symbol_t* sym = lookup_symbol($1);
        if (!sym) {
            yyerror("Undefined identifier");
            $$.type = TYPE_ERROR;
        } else if (sym->kind == SYMBOL_FUNCTION) {
            // This is a function call
            validate_function_args($1, $3);
            char buf[1024];
            snprintf(buf, sizeof(buf), "%s(%s)", $1, $3);
            $$.text = strdup(buf);
            $$.type = sym->type;
            $$.kind = SYMBOL_FUNCTION;
           } else {
            yyerror("Invalid use of identifier");
            $$.type = TYPE_ERROR;
        }
    }
    | ID
    {
        symbol_t* sym = lookup_symbol($1);
        if (!sym) {
            yyerror("Undefined identifier");
            $$.type = TYPE_ERROR;
        } else {
            $$.text = strdup($1);
            $$.type = sym->type;
            $$.kind = sym->kind;
        }
    }
    ;

statements
    : /* empty */ { $$ = strdup(""); }
    | statements statement NEWLINE { $$ = $2; }
    | statements NEWLINE { $$ = strdup(""); }  /* Handle blank lines */
    ;

statement
    : labeled_statement
    | unlabeled_statement
    ;

labeled_statement
    : label unlabeled_statement
    ;

unlabeled_statement
    : simple_statement
    | compound_statement
    ;

simple_statement
    : assignment_statement
    | call_statement
    | goto_statement
    | io_statement
    | CONTINUE    { $$ = strdup("CONTINUE"); }
    | STOP       { $$ = strdup("STOP"); }
    | RETURN     { $$ = strdup("RETURN"); }
    ;

compound_statement
    : if_statement
    | loop_statement
    ;

assignment_statement
    : var_reference ASSIGN expression
    {
        if (!is_compatible_types($1.type, $3.type)) {
            report_error(ERR_TYPE, SEV_ERROR, "Type mismatch in assignment", NULL);
        }
        char temp[256];
        snprintf(temp, sizeof(temp), "%s=%s", $1.text, $3.text);
        $$ = strdup(temp);
    }
    ;

if_statement
    : if_then_statement
    | if_then_else_statement
    | if_logical_statement
    | if_computed_statement
    ;

if_then_statement
    : IF LPAREN logical_expression RPAREN THEN statements ENDIF
    {
        char temp[512];
        snprintf(temp, sizeof(temp), "IF(%s)THEN\n%s\nENDIF", $3, $6);
        $$ = strdup(temp);
    }
    ;

if_then_else_statement
    : IF LPAREN logical_expression RPAREN THEN statements ELSE statements ENDIF
    {
        char temp[512];
        snprintf(temp, sizeof(temp), "IF(%s)THEN\n%s\nELSE\n%s\nENDIF", $3, $6, $8);
        $$ = strdup(temp);
    }
    ;

if_logical_statement
    : IF LPAREN logical_expression RPAREN simple_statement
    {
        char temp[512];
        snprintf(temp, sizeof(temp), "IF(%s)%s", $3, $5);
        $$ = strdup(temp);
    }
    ;

if_computed_statement
    : IF LPAREN computed_expression RPAREN label COMMA label COMMA label
    {
        char temp[512];
        snprintf(temp, sizeof(temp), "IF(%s)%s,%s,%s", $3, $5, $7, $9);
        $$ = strdup(temp);
    }
    ;

computed_expression
    : expression { $$ = strdup($1.text); }
    ;

logical_expression
    : logical_term
    | logical_expression OR logical_term
    {
        char temp[256];
        snprintf(temp, sizeof(temp), "%s.OR.%s", $1, $3);
        $$ = strdup(temp);
    }
    ;

logical_term
    : logical_factor
    | logical_term AND logical_factor
    {
        char temp[256];
        snprintf(temp, sizeof(temp), "%s.AND.%s", $1, $3);
        $$ = strdup(temp);
    }
    ;

logical_factor
    : relational_expression { $$ = $1; }
    | NOT logical_factor { char temp[256]; snprintf(temp, sizeof(temp), ".NOT.%s", $2); $$ = strdup(temp); }
    | LPAREN logical_expression RPAREN { char temp[256]; snprintf(temp, sizeof(temp), "(%s)", $2); $$ = strdup(temp); }
    | BCONST { $$ = $1 ? strdup(".TRUE.") : strdup(".FALSE."); }
    ;

relational_expression
    : expression GT expression { 
        char temp[256]; 
        snprintf(temp, sizeof(temp), "%s.GT.%s", $1.text, $3.text); 
             $$ = strdup(temp);
         }
    | expression LT expression { 
        char temp[256]; 
        snprintf(temp, sizeof(temp), "%s.LT.%s", $1.text, $3.text); 
             $$ = strdup(temp);
         }
    | expression GE expression { 
        char temp[256]; 
        snprintf(temp, sizeof(temp), "%s.GE.%s", $1.text, $3.text); 
             $$ = strdup(temp);
         }
    | expression LE expression { 
        char temp[256]; 
        snprintf(temp, sizeof(temp), "%s.LE.%s", $1.text, $3.text); 
             $$ = strdup(temp);
         }
    | expression EQ expression { 
        char temp[256]; 
        snprintf(temp, sizeof(temp), "%s.EQ.%s", $1.text, $3.text); 
             $$ = strdup(temp);
         }
    | expression NE expression { 
        char temp[256]; 
        snprintf(temp, sizeof(temp), "%s.NE.%s", $1.text, $3.text); 
             $$ = strdup(temp);
         }
    ;

loop_statement
    : DO var_reference ASSIGN expression COMMA expression COMMA expression statements ENDDO
         { 
        char temp[512];
        snprintf(temp, sizeof(temp), "DO %s=%s,%s,%s\n%s\nENDDO", 
                $2.text, $4.text, $6.text, $8.text, $9);
             $$ = strdup(temp);
         }
    | DO var_reference ASSIGN expression COMMA expression statements ENDDO
         { 
        char temp[512];
        snprintf(temp, sizeof(temp), "DO %s=%s,%s\n%s\nENDDO", 
                $2.text, $4.text, $6.text, $7);
             $$ = strdup(temp);
         }
    ;

goto_statement
    : GOTO label
         { 
        $$ = $2;
         }
    | GOTO var_reference COMMA LPAREN label_list RPAREN
         { 
        char temp[256];
        snprintf(temp, sizeof(temp), "GOTO %s,(%s)", $2.text, $5);
             $$ = strdup(temp);
        DEBUG_PARSER("Computed GOTO statement with variable: %s", $2.text);
    }
    ;

label_list
    : label_list COMMA label
    {
        char temp[256];
        snprintf(temp, sizeof(temp), "%s,%s", $1, $3);
        $$ = strdup(temp);
    }
    | label
    {
           $$ = $1;
       }
       ;

label
    : ICONST { char buf[32]; snprintf(buf, sizeof(buf), "%d", $1); $$ = strdup(buf); }
    ;

call_statement
    : CALL ID
    {
        $$ = $2;  /* Use function name */
        DEBUG_PARSER("CALL statement for: %s", $2);
    }
    | CALL ID LPAREN function_argument_list RPAREN
    {
        char temp[256];
                snprintf(temp, sizeof(temp), "%s(%s)", $2, $4);
                $$ = strdup(temp);
        DEBUG_PARSER("CALL statement with args for: %s", $2);
            }
    ;

function_argument_list
    : function_argument_list COMMA expression
        { 
            char temp[256];
        snprintf(temp, sizeof(temp), "%s,%s", $1, $3.text);
            $$ = strdup(temp);
        }
    | expression
    {
        $$ = strdup($1.text);
    }
            ;

io_statement
    : READ LPAREN io_list RPAREN
        {
            char temp[256];
        snprintf(temp, sizeof(temp), "READ(%s)", $3);
            $$ = strdup(temp);
        }
    | WRITE LPAREN io_list RPAREN
        {
            char temp[256];
        snprintf(temp, sizeof(temp), "WRITE(%s)", $3);
            $$ = strdup(temp);
        }
        ;

io_list
    : io_list COMMA io_item
        {
            char temp[256];
        snprintf(temp, sizeof(temp), "%s,%s", $1, $3);
            $$ = strdup(temp);
        }
    | io_item
    {
        $$ = $1;
    }
    ;

io_item
    : expression
    {
        $$ = strdup($1.text);
    }
    | ID ASSIGN expression COMMA expression  /* Format specifier */
    {
        char temp[256];
        snprintf(temp, sizeof(temp), "%s=%s,%s", $1, $3.text, $5.text);
        $$ = strdup(temp);
    }
    ;

expression
    : term
    {
        $$ = $1;
    }
    | expression PLUS term
    {
        if (!is_numeric_type($1.type) || !is_numeric_type($3.type)) {
            report_error(ERR_TYPE, SEV_ERROR, "Non-numeric operands for '+'", NULL);
            $$.type = TYPE_ERROR;
        } else {
            char buf[256];
            snprintf(buf, sizeof(buf), "%s+%s", $1.text, $3.text);
            $$.text = strdup(buf);
            $$.type = promote_type($1.type, $3.type);
            $$.kind = SYMBOL_EXPRESSION;
        }
    }
    | expression MINUS term
    {
        if (!is_numeric_type($1.type) || !is_numeric_type($3.type)) {
            report_error(ERR_TYPE, SEV_ERROR, "Non-numeric operands for '-'", NULL);
            $$.type = TYPE_ERROR;
        } else {
            char buf[256];
            snprintf(buf, sizeof(buf), "%s-%s", $1.text, $3.text);
            $$.text = strdup(buf);
            $$.type = promote_type($1.type, $3.type);
            $$.kind = SYMBOL_EXPRESSION;
        }
    }
    ;

term
    : factor
    {
        $$ = $1;
    }
    | term STAR factor
    {
        if (!is_numeric_type($1.type) || !is_numeric_type($3.type)) {
            report_error(ERR_TYPE, SEV_ERROR, "Non-numeric operands for '*'", NULL);
            $$.type = TYPE_ERROR;
        } else {
            char buf[256];
            snprintf(buf, sizeof(buf), "%s*%s", $1.text, $3.text);
            $$.text = strdup(buf);
            $$.type = promote_type($1.type, $3.type);
            $$.kind = SYMBOL_EXPRESSION;
        }
    }
    | term SLASH factor
    {
        if (!is_numeric_type($1.type) || !is_numeric_type($3.type)) {
            report_error(ERR_TYPE, SEV_ERROR, "Non-numeric operands for '/'", NULL);
            $$.type = TYPE_ERROR;
        } else {
            char buf[256];
            snprintf(buf, sizeof(buf), "%s/%s", $1.text, $3.text);
            $$.text = strdup(buf);
            $$.type = promote_type($1.type, $3.type);
            $$.kind = SYMBOL_EXPRESSION;
        }
    }
    ;

factor
    : primary
    {
                  $$ = $1;
              }
    | MINUS factor %prec UMINUS
    {
        if (!is_numeric_type($2.type)) {
            report_error(ERR_TYPE, SEV_ERROR, "Non-numeric operand for unary '-'", NULL);
            $$.type = TYPE_ERROR;
        } else {
            char buf[256];
            snprintf(buf, sizeof(buf), "-%s", $2.text);
            $$.text = strdup(buf);
            $$.type = $2.type;
            $$.kind = SYMBOL_EXPRESSION;
        }
    }
    | LPAREN expression RPAREN
    {
        $$ = $2;
    }
    ;

primary
    : var_reference
    {
        $$ = $1;
    }
    | ICONST
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", $1);
        $$.text = strdup(buf);
        $$.type = TYPE_INTEGER;
        $$.kind = SYMBOL_CONSTANT;
    }
    | RCONST
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "%g", $1);
        $$.text = strdup(buf);
        $$.type = TYPE_REAL;
        $$.kind = SYMBOL_CONSTANT;
    }
    ;

common_block
    : COMMON SLASH ID SLASH vars NEWLINE
    {
        // Create arrays for variable names and types
        const char** var_names = malloc(sizeof(char*) * MAX_SYMBOLS);
        type_t* types = malloc(sizeof(type_t) * MAX_SYMBOLS);
        int num_vars = 0;
        
        // Parse the vars string to extract names and types
        char* var_list = strdup($5);
        char* token = strtok(var_list, ",");
        while (token && num_vars < MAX_SYMBOLS) {
            var_names[num_vars] = strdup(token);
            types[num_vars] = current_type;  // Use current_type for all variables
            num_vars++;
            token = strtok(NULL, ",");
        }
        
        process_common_block($3, var_names, types, num_vars);
        
        // Clean up
        for (int i = 0; i < num_vars; i++) {
            free((void*)var_names[i]);
        }
        free(var_names);
        free(types);
        free(var_list);
        
            $$ = $3;
        }
    ;

data_stmt
    : DATA data_items NEWLINE
    {
        $$ = $2;  /* Use data items value */
        DEBUG_PARSER("DATA statement with items: %s", $2);
    }
    ;

data_items
    : data_items COMMA data_item { $$ = $1; }
    | data_item { $$ = $1; }
    ;

data_item
    : vars SLASH value_list SLASH { $$ = $1; }
    ;

value_list
    : value_list COMMA value { $$ = $1; }
    | value { $$ = $1; }
    ;

value
    : ICONST 
    { 
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", $1);
        $$ = strdup(buf);
    }
    | RCONST 
    { 
        char buf[32];
        snprintf(buf, sizeof(buf), "%g", $1);
        $$ = strdup(buf);
    }
    | BCONST 
    { 
        $$ = strdup($1 ? ".TRUE." : ".FALSE.");
    }
    | STRING_LITERAL { $$ = $1; }
    | LPAREN RCONST COLON RCONST RPAREN
    {
        char buf[64];
        snprintf(buf, sizeof(buf), "(%g:%g)", $2, $4);
        $$ = strdup(buf);
    }
    ;

expression_list
    : expression
    {
        $$ = strdup($1.text);
    }
    | expression_list COMMA expression
    {
        char buf[256];
        snprintf(buf, sizeof(buf), "%s,%s", $1, $3.text);
        $$ = strdup(buf);
    }
    ;

%%

int check_numeric_variables(const char* var1, const char* var2) {
    symbol_t* sym1 = lookup_symbol(var1);
    symbol_t* sym2 = lookup_symbol(var2);
    
    if (!sym1 || !sym2) return 0;
    
    return (sym1->type == TYPE_INTEGER || sym1->type == TYPE_REAL) &&
           (sym2->type == TYPE_INTEGER || sym2->type == TYPE_REAL);
}