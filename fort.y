%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intermediate.h"
#include "symtab.h"

extern int line_number;
extern FILE* yyin;
extern char* yytext;
extern char line_buf[];
extern int line_pos;
extern int error_count;

int yylex(void);
void sync_to_next_statement(void);

/* Debugging function - μόνο όταν ορίζεται DEBUG */
#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

void yyerror(const char* msg) {
    fprintf(stderr, "\nError at line %d: %s\n", line_number, msg);
    fprintf(stderr, "Line: %s\n", line_buf);
    fprintf(stderr, "      ");
    for(int i = 0; i < line_pos-1; i++) fprintf(stderr, " ");
    fprintf(stderr, "^\n");
    
    if (strstr(msg, "syntax error")) {
        if (strstr(line_buf, "[")) {
            fprintf(stderr, "FORT Error: Arrays must use parentheses (), not square brackets []\n");
            fprintf(stderr, "Example: array(1), matrix(i,j)\n");
        }
        else if (strstr(line_buf, "integer") && strstr(line_buf, "subroutine")) {
            fprintf(stderr, "FORT Error: Parameter types must be declared inside subroutine body\n");
            fprintf(stderr, "Correct format:\n");
            fprintf(stderr, "  SUBROUTINE name(x,y,z)\n");
            fprintf(stderr, "    INTEGER x\n");
            fprintf(stderr, "    REAL y\n");
            fprintf(stderr, "    ...\n");
            fprintf(stderr, "  END\n");
        }
        else if (strstr(line_buf, "data")) {
            fprintf(stderr, "FORT Error: Invalid DATA statement syntax\n");
            fprintf(stderr, "Correct format: DATA var/value_list/\n");
            fprintf(stderr, "Examples:\n");
            fprintf(stderr, "  DATA x/1, 2, 3/\n");
            fprintf(stderr, "  DATA y/1.0, 2.5, -3.14/\n");
            fprintf(stderr, "  DATA str/'text', 'more'/\n");
            fprintf(stderr, "Note: * is not allowed for repetition in FORT\n");
        }
        else if (strstr(line_buf, "do")) {
            fprintf(stderr, "FORT Error: Invalid DO loop syntax\n");
            fprintf(stderr, "Correct format: DO var=expr1,expr2[,expr3]\n");
            fprintf(stderr, "Example: DO i=1,10,2\n");
            fprintf(stderr, "Note: Loop variable must be INTEGER and not an array\n");
        }
        else if (strstr(line_buf, "if")) {
            fprintf(stderr, "FORT Error: Invalid IF statement syntax\n");
            fprintf(stderr, "Valid formats:\n");
            fprintf(stderr, "1. IF (condition) statement\n");
            fprintf(stderr, "2. IF (condition) THEN ... ENDIF\n");
            fprintf(stderr, "3. IF (condition) THEN ... ELSE ... ENDIF\n");
            fprintf(stderr, "4. IF (expr) label1,label2,label3\n");
        }
    }
    
    error_count++;
}
%}

%union {
    int ival;
    double rval;
    char *sval;
}

/* Δήλωση όλων των tokens */
%token PROGRAM FUNCTION SUBROUTINE END COMMON 
%token INTEGER REAL COMPLEX LOGICAL CHARACTER STRING LIST
%token DATA CONTINUE GOTO CALL READ WRITE
%token IF THEN ENDIF DO ENDDO STOP RETURN
%token <ival> LABEL
%token <sval> ID
%token <ival> ICONST
%token <rval> RCONST
%token <sval> STRING_LITERAL
%token ASSIGN ADDOP MULOP DIVOP POWEROP
%token LPAREN RPAREN COMMA STAR
%token LBRACK RBRACK
%token NOT AND OR GT LT EQ LE TRUE FALSE
%token COLON
%token LPAREN_ASSIGN

/* Δηλώσεις προτεραιότητας */
%right ASSIGN
%left OR
%left AND
%right NOT
%left GT LT EQ LE
%left ADDOP
%left MULOP DIVOP
%right POWEROP
%right UMINUS UPLUS  /* For unary minus/plus */
%left COMMA
%left LPAREN RPAREN
%left LBRACK RBRACK
%left COLON

/* Δηλώσεις συσχέτισης για τα if-then-else */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

/* Δηλώσεις προτεραιότητας για τα DO loops */
%nonassoc DO
%nonassoc ENDDO

%{
void sync_to_next_statement() {
    int token;
    int found_sync = 0;
    
    DEBUG_PRINT("Starting sync_to_next_statement\n");
    while ((token = yylex())) {
        DEBUG_PRINT("Found token %d\n", token);
        if (token == '\n' || token == 0) {
            found_sync = 1;
            break;
        }
        switch (token) {
            case IF:
            case DO:
            case READ:
            case WRITE:
            case CALL:
            case GOTO:
            case INTEGER:
            case REAL:
            case COMPLEX:
            case LOGICAL:
            case CHARACTER:
            case DATA:
                found_sync = 1;
                DEBUG_PRINT("Found sync point at token %d\n", token);
                break;
            default:
                continue;
        }
        if (found_sync) break;
    }
    
    if (!found_sync) {
        fprintf(stderr, "Warning: Could not synchronize to next statement - skipping line\n");
        while ((token = yylex()) && token != '\n' && token != 0);
    }
    DEBUG_PRINT("Sync complete\n");
}
%}

%%

program_unit: program
           | program_unit subroutine
           | error
           { 
               yyerrok; 
               sync_to_next_statement();
               fprintf(stderr, "Error: Invalid program unit - attempting to continue\n");
           }
           ;

program: declarations_and_statements END
       { 
           DEBUG_PRINT("Program END reached\n");
           if (error_count > 0) {
               fprintf(stderr, "\nCompilation completed with %d error(s)\n", error_count);
           }
       }
       | error END
       { 
           yyerrok;
           if (error_count > 0) {
               fprintf(stderr, "\nCompilation completed with %d error(s)\n", error_count);
           }
       }
       ;

subroutine: SUBROUTINE subroutine_name LPAREN parameter_list RPAREN declarations_and_statements END
          { 
              DEBUG_PRINT("Subroutine END reached\n");
          }
          | SUBROUTINE subroutine_name LPAREN RPAREN declarations_and_statements END
          { 
              DEBUG_PRINT("Subroutine END reached (no parameters)\n");
          }
          | SUBROUTINE subroutine_name error END
          {
              yyerrok;
              fprintf(stderr, "Error: Invalid subroutine declaration at line %d\n", line_number);
              fprintf(stderr, "Parameter types must be declared inside the subroutine body\n");
              fprintf(stderr, "Correct format:\n");
              fprintf(stderr, "  SUBROUTINE name(x,y,z)\n");
              fprintf(stderr, "    INTEGER x\n");
              fprintf(stderr, "    REAL y\n");
              fprintf(stderr, "    ...\n");
              fprintf(stderr, "  END\n");
              fprintf(stderr, "Found: %s\n", line_buf);
          }
          | error END
          { 
              yyerrok;
              fprintf(stderr, "Error: Invalid subroutine declaration at line %d\n", line_number);
              fprintf(stderr, "Correct format: SUBROUTINE name(params) ... END\n");
              fprintf(stderr, "Found: %s\n", line_buf);
          }
          ;

subroutine_name: ID
               | error '\n'
               { 
                   yyerrok;
                   yyclearin;
                   fprintf(stderr, "Error: Invalid subroutine name - skipping to next line\n");
               }
               ;

parameter_list: ID
             { 
                 DEBUG_PRINT("Adding parameter %s\n", $1);
                 add_symbol($1, TYPE_INTEGER); 
             }
             | parameter_list COMMA ID
             { 
                 DEBUG_PRINT("Adding parameter %s\n", $3);
                 add_symbol($3, TYPE_INTEGER); 
             }
             | error RPAREN
             { 
                 yyerrok;
                 yyclearin;
                 fprintf(stderr, "Error: Invalid parameter list at line %d\n", line_number);
                 fprintf(stderr, "Parameters must be simple identifiers separated by commas\n");
                 fprintf(stderr, "Example: SUBROUTINE name(x,y,z)\n");
                 fprintf(stderr, "Types must be declared inside the subroutine body\n");
             }
             ;

declarations_and_statements: /* empty */
                         | declarations_and_statements declaration
                         | declarations_and_statements statement
                         | error
                         { 
                             yyerrok;
                             sync_to_next_statement();
                             fprintf(stderr, "Error: Invalid statement or declaration - attempting to continue\n");
                         }
                         ;

declaration: type declarator_list
          | type LIST declarator_list  
          | DATA data_list
          | LOGICAL ID LPAREN expression RPAREN
          | error '\n'
          { 
              yyerrok;
              yyclearin;
              fprintf(stderr, "Error: Invalid declaration - skipping to next line\n");
          }
          ;

type: INTEGER
    | REAL
    | COMPLEX
    | LOGICAL
    | CHARACTER
    | STRING
    ;

declarator_list: declarator
               | declarator_list COMMA declarator
               ;

declarator: ID
         | array_declarator
         ;

array_declarator: ID LPAREN expression RPAREN
                | ID error RPAREN
                ;

statement: labeled_statement
        | unlabeled_statement
        | error
        { 
            yyerrok;
            sync_to_next_statement();
            fprintf(stderr, "Error: Invalid statement - attempting to continue\n");
        }
        ;

labeled_statement: LABEL unlabeled_statement
                ;

unlabeled_statement: assignment_statement
                  | if_statement
                  | do_statement
                  | call_statement
                  | goto_statement
                  | read_statement
                  | write_statement
                  | CONTINUE
                  | STOP
                  | RETURN
                  ;

assignment_statement: ID ASSIGN expression
                   | array_reference ASSIGN expression
                   | LPAREN lvalue RPAREN ASSIGN expression
                   | error
                   { 
                       yyclearin;
                       DEBUG_PRINT("Error recovery in assignment\n");
                   }
                   ;

lvalue: ID
      | array_reference
      ;

expression: primary
         | LPAREN expression RPAREN
         | expression ADDOP expression
         | expression MULOP expression
         | expression DIVOP expression
         | expression POWEROP expression
         | ADDOP expression %prec UMINUS
         | expression relational_op expression
         | NOT expression
         | expression AND expression
         | expression OR expression
         | error
         {
             fprintf(stderr, "Error: Invalid expression at line %d\n", line_number);
             fprintf(stderr, "Valid expressions include:\n");
             fprintf(stderr, "- Variables and constants\n");
             fprintf(stderr, "- Array references: array(index)\n");
             fprintf(stderr, "- Function calls: func(args)\n");
             fprintf(stderr, "- Arithmetic: +, -, *, /, **\n");
             fprintf(stderr, "- Logical: .and., .or., .not.\n");
             fprintf(stderr, "- Relational: .gt., .lt., .eq., .le.\n");
             sync_to_next_statement();
         }
         ;

primary: ID
        { 
            DEBUG_PRINT("Checking if %s is declared\n", $1);
            if (!check_symbol_declared($1)) {
                error_count++;
            }
        }
        | constant
        | array_reference
        | function_call
        ;

function_call: ID LPAREN argument_list RPAREN
            {
                DEBUG_PRINT("Function call to %s\n", $1);
                if (strcmp($1, "car") == 0 || strcmp($1, "cdr") == 0 || 
                    strcmp($1, "re") == 0 || strcmp($1, "im") == 0) {
                    fprintf(stderr, "Error: Function '%s' is not allowed in FORT at line %d\n", $1, line_number);
                    fprintf(stderr, "These are special functions from other languages and are not part of FORT\n");
                    fprintf(stderr, "Found: %s\n", line_buf);
                    error_count++;
                }
                else if (!is_valid_function_call($1)) {
                    fprintf(stderr, "Error: Unknown function '%s' at line %d\n", $1, line_number);
                    fprintf(stderr, "Valid functions are:\n");
                    fprintf(stderr, "- Built-in: abs, sqrt, sin, cos, exp, log\n");
                    fprintf(stderr, "- User-defined functions must be declared before use\n");
                    fprintf(stderr, "Found: %s\n", line_buf);
                    error_count++;
                }
            }
            | CALL ID LPAREN argument_list RPAREN
            { 
                DEBUG_PRINT("Subroutine call to %s\n", $2);
                if (!check_symbol_declared($2)) {
                    fprintf(stderr, "Error: Undefined subroutine '%s' at line %d\n", $2, line_number);
                    fprintf(stderr, "Found: %s\n", line_buf);
                    error_count++;
                }
            }
            | CALL ID LPAREN RPAREN
            { 
                DEBUG_PRINT("Subroutine call to %s (no arguments)\n", $2);
                if (!check_symbol_declared($2)) {
                    fprintf(stderr, "Error: Undefined subroutine '%s' at line %d\n", $2, line_number);
                    fprintf(stderr, "Found: %s\n", line_buf);
                    error_count++;
                }
            }
            | CALL error
            {
                yyerrok;
                fprintf(stderr, "Error: Invalid subroutine call at line %d\n", line_number);
                fprintf(stderr, "Valid formats:\n");
                fprintf(stderr, "  CALL subroutine_name\n");
                fprintf(stderr, "  CALL subroutine_name()\n");
                fprintf(stderr, "  CALL subroutine_name(arg1, arg2, ...)\n");
                fprintf(stderr, "Found: %s\n", line_buf);
            }
            ;

constant: ICONST
        | RCONST
        | STRING_LITERAL
        | TRUE
        | FALSE
        | ICONST 'B'    /* Binary constant */
        | RCONST 'B'    /* Binary constant with decimal point */
        ;

relational_op: GT
             | LT
             | EQ
             | LE
             ;

if_statement: IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE
           | IF LPAREN expression RPAREN THEN block ENDIF
           | IF LPAREN expression RPAREN THEN block ELSE block ENDIF
           | IF LPAREN expression RPAREN arithmetic_labels
           | IF error RPAREN
           ;

arithmetic_labels: ICONST COMMA ICONST COMMA ICONST
                | LABEL COMMA LABEL COMMA LABEL
                | error
                {
                    fprintf(stderr, "Error: Invalid arithmetic IF labels at line %d\n", line_number);
                    fprintf(stderr, "Arithmetic IF requires exactly three labels or constants\n");
                    fprintf(stderr, "Format: IF (expr) label1,label2,label3\n");
                    fprintf(stderr, "The program branches to:\n");
                    fprintf(stderr, "  label1 if expr < 0\n");
                    fprintf(stderr, "  label2 if expr = 0\n");
                    fprintf(stderr, "  label3 if expr > 0\n");
                }
                ;

block: /* empty */
     | block declaration
     | block statement
     | error '\n'
     { 
         yyerrok;
         yyclearin;
         fprintf(stderr, "Error: Invalid block statement - skipping to next line\n");
     }
     ;

do_statement: DO do_control block ENDDO
            { 
                DEBUG_PRINT("DO loop complete\n");
                if (!check_loop_variable_type()) {
                    fprintf(stderr, "Error: DO loop control variable must be INTEGER and not an array at line %d\n", line_number);
                    fprintf(stderr, "Found: %s\n", line_buf);
                    error_count++;
                }
            }
            | error ENDDO
            { 
                yyerrok;
            }
            ;

do_control: ID ASSIGN expression COMMA expression
         { 
             DEBUG_PRINT("DO loop control: %s\n", $1);
             set_current_loop_var($1);
             if (!check_loop_variable_type()) {
                 fprintf(stderr, "Error: DO loop control variable must be INTEGER at line %d\n", line_number);
                 fprintf(stderr, "Variable '%s' is either not declared, not INTEGER, or is an array\n", $1);
                 fprintf(stderr, "Found: %s\n", line_buf);
                 error_count++;
             }
         }
         | ID ASSIGN expression COMMA expression COMMA expression
         { 
             DEBUG_PRINT("DO loop control with step: %s\n", $1);
             set_current_loop_var($1);
             if (!check_loop_variable_type()) {
                 fprintf(stderr, "Error: DO loop control variable must be INTEGER at line %d\n", line_number);
                 fprintf(stderr, "Variable '%s' is either not declared, not INTEGER, or is an array\n", $1);
                 fprintf(stderr, "Found: %s\n", line_buf);
                 error_count++;
             }
         }
         | error
         {
             yyerrok;
             fprintf(stderr, "Error: Invalid DO loop control at line %d\n", line_number);
             fprintf(stderr, "DO loop control must be in one of these formats:\n");
             fprintf(stderr, "1. DO i=start,end\n");
             fprintf(stderr, "2. DO i=start,end,step\n");
             fprintf(stderr, "where i is an INTEGER variable (not an array)\n");
             fprintf(stderr, "Found: %s\n", line_buf);
         }
         ;

array_reference: ID LPAREN argument_list RPAREN
                {
                    DEBUG_PRINT("Array reference: %s\n", $1);
                    if (!check_symbol_declared($1)) {
                        error_count++;
                    } else {
                        symbol *sym = lookup_symbol($1);
                        if (!sym->is_array) {
                            fprintf(stderr, "Error: '%s' is not an array at line %d\n", $1, line_number);
                            error_count++;
                        }
                        if (strstr(line_buf, "-") || strstr(line_buf, ".")) {
                            fprintf(stderr, "Error: Array indices must be positive integers at line %d\n", line_number);
                            fprintf(stderr, "Found: %s\n", line_buf);
                            error_count++;
                        }
                        if (sym->is_array) {
                            int index = atoi(yytext);
                            if (!check_array_bounds($1, index)) {
                                error_count++;
                            }
                        }
                    }
                }
                | ID error '\n'
                {
                    yyerrok;
                    yyclearin;
                    fprintf(stderr, "Error: Invalid array reference at line %d\n", line_number);
                    fprintf(stderr, "Arrays must use parentheses (), not square brackets []\n");
                    fprintf(stderr, "Example: array(1), matrix(i,j)\n");
                    fprintf(stderr, "Found: %s\n", line_buf);
                    sync_to_next_statement();
                }
                ;

argument_list: expression
             | argument_list COMMA expression
             ;

call_statement: CALL ID
             | CALL ID LPAREN argument_list RPAREN
             | CALL ID LPAREN RPAREN
             | CALL error
             ;

goto_statement: GOTO label_expr
             | GOTO ID COMMA LPAREN label_list RPAREN
             ;

label_expr: ICONST
         | ID
         ;

label_list: ICONST
         | label_list COMMA ICONST
         ;

read_statement: READ read_list
             | READ LPAREN read_args RPAREN read_list
             | READ LPAREN read_args RPAREN COMMA read_list
             | error
             { 
                 fprintf(stderr, "Error: Invalid READ statement at line %d\n", line_number);
                 fprintf(stderr, "Valid formats:\n");
                 fprintf(stderr, "  READ var1, var2, ...\n");
                 fprintf(stderr, "  READ(unit) var1, var2, ...\n");
                 fprintf(stderr, "  READ(unit,*) var1, var2, ...\n");
                 sync_to_next_statement();
             }
             ;

read_list: read_item
        | read_list COMMA read_item
        | error
        {
            fprintf(stderr, "Error: Invalid READ list at line %d\n", line_number);
            fprintf(stderr, "READ list must be comma-separated variables or array references\n");
            sync_to_next_statement();
        }
        ;

read_item: ID
        | array_reference
        ;

read_args: expression
        | read_args COMMA expression
        | read_args COMMA range_spec
        | range_spec
        | error
        {
            fprintf(stderr, "Error: Invalid READ arguments at line %d\n", line_number);
            fprintf(stderr, "READ arguments must be unit numbers or format specifiers\n");
            sync_to_next_statement();
        }
        ;

range_spec: ID ASSIGN expression COMMA expression
         ;

write_statement: WRITE LPAREN STAR COMMA STAR RPAREN write_list
               | error
               { 
                   yyerrok;
                   yyclearin;
                   DEBUG_PRINT("Error recovery in write statement\n");
               }
               ;

write_list: write_item
         | write_list COMMA write_item
         ;

write_item: expression
         | STRING_LITERAL
         ;

data_list: data_item
        | data_list COMMA data_item
        | error
        { 
            yyerrok;
            yyclearin;
            DEBUG_PRINT("Error recovery in data list\n");
        }
        ;

data_item: ID DIVOP value_list DIVOP
        { 
            extern int in_data; 
            in_data = 0;
            if (!check_data_type_compatibility($1)) {
                fprintf(stderr, "Error: Type mismatch in DATA statement at line %d\n", line_number);
                fprintf(stderr, "Values must match the declared type of variable '%s'\n", $1);
                fprintf(stderr, "INTEGER accepts: integer constants\n");
                fprintf(stderr, "REAL accepts: real or integer constants\n");
                fprintf(stderr, "COMPLEX accepts: (real:imag) format\n");
                fprintf(stderr, "STRING accepts: string literals\n");
                fprintf(stderr, "LOGICAL accepts: .true. or .false.\n");
                fprintf(stderr, "Found: %s\n", line_buf);
                error_count++;
            }
            if (strstr(line_buf, "*")) {
                fprintf(stderr, "Error: Repetition operator * is not allowed in FORT at line %d\n", line_number);
                fprintf(stderr, "Found: %s\n", line_buf);
                fprintf(stderr, "Use explicit values instead\n");
                error_count++;
            }
        }
        | error DIVOP
        {
            yyerrok;
            fprintf(stderr, "Error: Invalid DATA statement at line %d\n", line_number);
            fprintf(stderr, "DATA values must be constants matching variable type:\n");
            fprintf(stderr, "  DATA x/1, 2, 3/        -- for INTEGER\n");
            fprintf(stderr, "  DATA y/1.0, 2.5, -3.14/ -- for REAL\n");
            fprintf(stderr, "  DATA z/(5.0:2.0)/      -- for COMPLEX\n");
            fprintf(stderr, "  DATA str/'text'/       -- for STRING\n");
            fprintf(stderr, "  DATA flag/.true./      -- for LOGICAL\n");
            fprintf(stderr, "Note: * for repetition is not allowed in FORT\n");
        }
        ;

value_list: value
         | value_list COMMA value
         | value_list COMMA STAR value  /* για *"string2" */
         {
             fprintf(stderr, "Error: Repetition operator * is not allowed in FORT at line %d\n", line_number);
             fprintf(stderr, "Found: %s\n", line_buf);
             fprintf(stderr, "Use explicit values instead\n");
             error_count++;
         }
         | error
         {
             yyerrok;
             fprintf(stderr, "Error: Invalid value in DATA statement at line %d\n", line_number);
             fprintf(stderr, "Values must be:\n");
             fprintf(stderr, "- Integer constants (e.g., 42)\n");
             fprintf(stderr, "- Real constants (e.g., 3.14)\n");
             fprintf(stderr, "- String literals (e.g., 'text')\n");
             fprintf(stderr, "- Logical values (.true. or .false.)\n");
             fprintf(stderr, "Note: * for repetition is not allowed\n");
         }
         ;

value: constant
     | range_list
     | STAR STRING_LITERAL  /* για *"string2" */
     {
         fprintf(stderr, "Error: Repetition operator * is not allowed in FORT at line %d\n", line_number);
         fprintf(stderr, "Found: %s\n", line_buf);
         fprintf(stderr, "Use explicit string values instead\n");
         error_count++;
     }
     ;

range_list: LPAREN range_value RPAREN
         | LPAREN range_list COMMA range_value RPAREN
         ;

range_value: number COLON number
          {
              if (strstr(line_buf, ":")) {
                  fprintf(stderr, "Note: Using (a:b) format for complex numbers at line %d\n", line_number);
                  fprintf(stderr, "Make sure the variable is declared as COMPLEX\n");
              }
          }
          | error
          {
              yyerrok;
              fprintf(stderr, "Error: Invalid range in DATA statement at line %d\n", line_number);
              fprintf(stderr, "For complex numbers use the format: (real:imag)\n");
              fprintf(stderr, "Example: DATA z/(5.0:2.0)/\n");
              fprintf(stderr, "Note: Variable must be declared as COMPLEX\n");
          }
          ;

number: RCONST
      | ICONST
      | ADDOP RCONST    /* για -2.0 */
      | ADDOP ICONST    /* για -2 */
      ;

%%