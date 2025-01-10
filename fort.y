%{
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
%}

%union {
    int ival;
    double rval;
    char *sval;
}

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

%%

/* Κύριος κανόνας που επιτρέπει πολλαπλές υπορουτίνες */
program_unit: program
           | program_unit subroutine
           ;

program: declarations_and_statements END
       { 
           DEBUG("program -> declarations_and_statements END\n");
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

/* Κανόνες για υπορουτίνες */
subroutine: SUBROUTINE subroutine_name LPAREN parameter_list RPAREN declarations_and_statements END
          { DEBUG("subroutine -> SUBROUTINE ID(params) body END\n"); }
          | SUBROUTINE subroutine_name LPAREN RPAREN declarations_and_statements END
          { DEBUG("subroutine -> SUBROUTINE ID() body END\n"); }
          | error END
          { 
              yyerrok;
              fprintf(stderr, "Error: Invalid subroutine declaration\n");
              DEBUG("Error recovery in subroutine\n");
          }
          ;

subroutine_name: ID
               { DEBUG("subroutine_name -> ID\n"); }
               ;

parameter_list: ID
             { DEBUG("parameter_list -> ID\n"); }
             | parameter_list COMMA ID
             { DEBUG("parameter_list -> parameter_list COMMA ID\n"); }
             ;

declarations_and_statements: /* empty */
                         { DEBUG("declarations_and_statements -> empty\n"); }
                         | declarations_and_statements declaration
                         { DEBUG("declarations_and_statements -> declarations_and_statements declaration\n"); }
                         | declarations_and_statements statement
                         { DEBUG("declarations_and_statements -> declarations_and_statements statement\n"); }
                         | error
                         { 
                             yyerrok;
                             yyclearin;
                             DEBUG("Error recovery: skipping to next statement\n");
                         }
                         ;

declaration: type declarator_list
          | type LIST declarator_list  
          | DATA data_list
          | LOGICAL ID LPAREN expression RPAREN
          { printf("DEBUG PARSER: Found logical array declaration: %s\n", $2); }
          | error
          { 
              yyerrok;
              yyclearin;
              fprintf(stderr, "Error: Invalid declaration\n");
              DEBUG("Error recovery in declaration\n");
          }
          ;

type: INTEGER { DEBUG("type -> INTEGER\n"); }
    | REAL    { DEBUG("type -> REAL\n"); }
    | COMPLEX { DEBUG("type -> COMPLEX\n"); }
    | LOGICAL { DEBUG("type -> LOGICAL\n"); }
    | CHARACTER { DEBUG("type -> CHARACTER\n"); }
    | STRING  { DEBUG("type -> STRING\n"); }
    ;

declarator_list: declarator
               { DEBUG("declarator_list -> declarator\n"); }
               | declarator_list COMMA declarator
               { DEBUG("declarator_list -> declarator_list COMMA declarator\n"); }
               ;

declarator: ID
         { printf("DEBUG PARSER: Found simple declarator ID='%s'\n", $1); }
         | array_declarator
         ;

array_declarator: ID LPAREN expression RPAREN
                { 
                    printf("DEBUG PARSER: Found array declarator: %s\n", $1);
                    /* Έλεγχος για μη έγκυρη σύνταξη πίνακα */
                    if (error_count > 0) {
                        fprintf(stderr, "Error: Invalid array syntax - FORT only supports () for arrays\n");
                    }
                }
                ;

statement: labeled_statement
        | unlabeled_statement
        | error
        { 
            yyerrok;
            yyclearin;
            DEBUG("Error recovery in statement\n");
        }
        ;

labeled_statement: LABEL unlabeled_statement
                { printf("DEBUG PARSER: Found labeled statement with label %d\n", $1); }
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
                   { printf("DEBUG PARSER: Found ID assignment: %s = expr\n", $1); }
                   | array_reference ASSIGN expression
                   { printf("DEBUG PARSER: Found array assignment\n"); }
                   | LPAREN lvalue RPAREN ASSIGN expression
                   { printf("DEBUG PARSER: Found parenthesized assignment\n"); }
                   | error
                   { 
                       yyerrok;
                       yyclearin;
                       DEBUG("Error recovery in assignment\n");
                   }
                   ;

lvalue: ID
      | array_reference
      ;

expression: primary
         | NOT expression
         { DEBUG("expression -> NOT expression\n"); }
         | ADDOP expression %prec NOT
         { DEBUG("expression -> ADDOP expression (unary)\n"); }
         | expression AND expression
         { DEBUG("expression -> expression AND expression\n"); }
         | expression OR expression
         { DEBUG("expression -> expression OR expression\n"); }
         | expression relational_op expression
         { DEBUG("expression -> expression relop expression\n"); }
         | expression ADDOP expression
         { DEBUG("expression -> expression ADDOP expression\n"); }
         | expression MULOP expression
         { DEBUG("expression -> expression MULOP expression\n"); }
         | expression DIVOP expression
         { DEBUG("expression -> expression DIVOP expression\n"); }
         | expression POWEROP expression
         { DEBUG("expression -> expression POWEROP expression\n"); }
         | LPAREN expression RPAREN
         { DEBUG("expression -> (expression)\n"); }
         | LPAREN LPAREN expression RPAREN RPAREN
         { DEBUG("expression -> ((expression))\n"); }
         | error
         { 
             yyerrok;
             yyclearin;
             DEBUG("Error recovery in expression\n");
         }
         ;

primary: ID
       { DEBUG("primary -> ID\n"); }
       | constant
       { DEBUG("primary -> constant\n"); }
       | function_call
       { DEBUG("primary -> function_call\n"); }
       | array_reference
       { DEBUG("primary -> array_reference\n"); }
       ;

function_call: ID LPAREN argument_list RPAREN
             { printf("DEBUG PARSER: Found function call: %s\n", $1); }
             | ID LPAREN RPAREN
             { printf("DEBUG PARSER: Found function call with no args: %s\n", $1); }
             ;

constant: ICONST
        | RCONST
        | STRING_LITERAL
        | TRUE
        | FALSE
        | ICONST 'B'    /* Binary constant */
        | RCONST 'B'    /* Binary constant with decimal point */
        ;

relational_op: GT | LT | EQ | LE ;

if_statement: IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE
           { printf("DEBUG PARSER: Found if statement\n"); }
           | IF LPAREN expression RPAREN THEN block ENDIF
           { printf("DEBUG PARSER: Found if-then statement\n"); }
           | IF LPAREN expression RPAREN THEN block ELSE block ENDIF
           { printf("DEBUG PARSER: Found if-then-else statement\n"); }
           | IF LPAREN expression RPAREN arithmetic_labels
           { printf("DEBUG PARSER: Found arithmetic if statement\n"); }
           | error
           { 
               yyerrok;
               yyclearin;
               DEBUG("Error recovery in if statement\n");
           }
           ;

arithmetic_labels: ICONST COMMA ICONST COMMA ICONST
                { printf("DEBUG PARSER: Found arithmetic if labels: %d,%d,%d\n", $1, $3, $5); }
                | LABEL COMMA LABEL COMMA LABEL
                { printf("DEBUG PARSER: Found arithmetic if with label variables\n"); }
                ;

block: /* empty */
     { printf("DEBUG PARSER: Starting empty block\n"); }
     | block declaration
     { printf("DEBUG PARSER: Added declaration to block\n"); }
     | block statement
     { printf("DEBUG PARSER: Added statement to block\n"); }
     ;

do_statement: DO do_control block ENDDO
           { printf("DEBUG PARSER: Found complete DO loop\n"); }
           | error ENDDO
           { 
               yyerrok;
               DEBUG("Error recovery in do statement\n");
           }
           ;

do_control: ID ASSIGN expression COMMA expression
         { printf("DEBUG PARSER: Found DO control: %s = expr1, expr2\n", $1); }
         | ID ASSIGN expression COMMA expression COMMA expression
         { printf("DEBUG PARSER: Found DO control with step: %s = expr1, expr2, expr3\n", $1); }
         ;

array_reference: ID LPAREN argument_list RPAREN
               { printf("DEBUG PARSER: Found array reference: %s\n", $1); }
               | ID LPAREN RPAREN
               { printf("DEBUG PARSER: Found empty array reference: %s\n", $1); }
               ;

argument_list: expression
             { printf("DEBUG PARSER: Found single argument\n"); }
             | argument_list COMMA expression
             { printf("DEBUG PARSER: Added another argument\n"); }
             ;

call_statement: CALL ID
             { DEBUG("call_statement -> CALL ID\n"); }
             | CALL ID LPAREN argument_list RPAREN
             { DEBUG("call_statement -> CALL ID(args)\n"); }
             | CALL ID LPAREN RPAREN
             { DEBUG("call_statement -> CALL ID()\n"); }
             ;

goto_statement: GOTO label_expr
             { printf("DEBUG PARSER: Found goto with label\n"); }
             | GOTO ID COMMA LPAREN label_list RPAREN
             { printf("DEBUG PARSER: Found computed goto\n"); }
             ;

label_expr: ICONST
         { printf("DEBUG PARSER: Found label constant\n"); }
         | ID
         { printf("DEBUG PARSER: Found label ID\n"); }
         ;

label_list: ICONST
         { DEBUG("label_list -> ICONST\n"); }
         | label_list COMMA ICONST
         { DEBUG("label_list -> label_list,ICONST\n"); }
         ;

read_statement: READ read_list
             { printf("DEBUG PARSER: Found simple READ\n"); }
             | READ LPAREN read_args RPAREN read_list
             { printf("DEBUG PARSER: Found READ with args\n"); }
             | READ LPAREN read_args RPAREN COMMA read_list
             { printf("DEBUG PARSER: Found READ with args and comma\n"); }
             | error
             { 
                 yyerrok;
                 yyclearin;
                 DEBUG("Error recovery in read statement\n");
             }
             ;

read_list: read_item
        { DEBUG("read_list -> read_item\n"); }
        | read_list COMMA read_item
        { DEBUG("read_list -> read_list COMMA read_item\n"); }
        ;

read_item: ID
        { DEBUG("read_item -> ID\n"); }
        | array_reference
        { DEBUG("read_item -> array_reference\n"); }
        ;

read_args: expression
        { printf("DEBUG PARSER: Found read arg expression\n"); }
        | read_args COMMA expression
        { printf("DEBUG PARSER: Found multiple read args\n"); }
        | read_args COMMA range_spec
        { printf("DEBUG PARSER: Found read args with range\n"); }
        | range_spec
        { printf("DEBUG PARSER: Found read range spec\n"); }
        ;

range_spec: ID ASSIGN expression COMMA expression
         { printf("DEBUG PARSER: Found range: %s = expr1, expr2\n", $1); }
         ;

write_statement: WRITE LPAREN STAR COMMA STAR RPAREN write_list
               | error
               { 
                   yyerrok;
                   yyclearin;
                   DEBUG("Error recovery in write statement\n");
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
            DEBUG("Error recovery in data list\n");
        }
        ;

data_item: ID DIVOP value_list DIVOP
        { 
          extern int in_data; 
          in_data = 0;
          DEBUG("data_item -> ID/value_list/\n"); 
        }
        ;

value_list: value
         | value_list COMMA value
         | value_list COMMA STAR value  /* για *"string2" */
         ;

value: constant
     | range_list
     | STAR STRING_LITERAL  /* για *"string2" */
     ;

range_list: LPAREN range_value RPAREN
         | LPAREN range_list COMMA range_value RPAREN
         ;

range_value: number COLON number
          ;

number: RCONST
      | ICONST
      | ADDOP RCONST    /* για -2.0 */
      | ADDOP ICONST    /* για -2 */
      ;

%%