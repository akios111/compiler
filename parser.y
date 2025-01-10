%{
#include <stdio.h>
#include "intermediate.h"
#include "symbol.h"

extern int yylex(void);
extern int yylineno;
extern char* yytext;
void yyerror(const char* s);

ic_node_t* ic_head = NULL;
%}

%union {
    int integer;
    char* string;
    struct ic_node* node;
}

%token <integer> INTEGER
%token <string> IDENTIFIER
%token DO END FOR TO STEP

%type <node> statement expression term factor

%%

program:
    statement_list
    ;

statement_list:
    statement
    | statement_list statement
    ;

statement:
    DO IDENTIFIER '=' expression TO expression     { 
        temp_var* start = new_temp(create_basic_type(TYPE_INTEGER));
        temp_var* end = new_temp(create_basic_type(TYPE_INTEGER));
        $$ = gen_do_start($2, start, end);
    }
    | END                                         { 
        $$ = gen_do_end(ic_head); 
    }
    | IDENTIFIER '=' expression                   {
        symbol_entry* sym = lookup_symbol($1);
        if (!sym) sym = insert_symbol($1, SYMBOL_VAR);
        $$ = gen_assign(sym->temp, ((ic_node_t*)$3)->u.assign.result);
    }
    ;

expression:
    term                                          { $$ = $1; }
    | expression '+' term                         { 
        $$ = create_binary_node(IC_ADD, $1, $3);
    }
    | expression '-' term                         {
        $$ = create_binary_node(IC_SUB, $1, $3);
    }
    ;

term:
    factor                                        { $$ = $1; }
    | term '*' factor                            {
        $$ = create_binary_node(IC_MUL, $1, $3);
    }
    | term '/' factor                            {
        $$ = create_binary_node(IC_DIV, $1, $3);
    }
    ;

factor:
    INTEGER                                       {
        temp_var* temp = new_temp(create_basic_type(TYPE_INTEGER));
        temp->value.ival = $1;
        $$ = gen_assign(temp, NULL);
    }
    | IDENTIFIER                                  {
        symbol_entry* sym = lookup_symbol($1);
        if (!sym) {
            yyerror("Undefined variable");
            YYERROR;
        }
        $$ = gen_assign(new_temp(sym->type), sym->temp);
    }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
} 