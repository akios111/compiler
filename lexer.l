%{
#include <stdio.h>
#include "token.h"
#include "parser.tab.h"

int column = 1;
%}

%option noyywrap
%option yylineno

DIGIT    [0-9]
ID       [a-zA-Z][a-zA-Z0-9_]*
WS       [ \t\r\n]

%%

"DO"        { column += yyleng; return DO; }
"END"       { column += yyleng; return END; }
"FOR"       { column += yyleng; return FOR; }
"TO"        { column += yyleng; return TO; }
"STEP"      { column += yyleng; return STEP; }

{DIGIT}+    { 
    yylval.integer = atoi(yytext);
    column += yyleng;
    return INTEGER;
}

{ID}        {
    yylval.string = strdup(yytext);
    column += yyleng;
    return IDENTIFIER;
}

"="         { column++; return '='; }
"+"         { column++; return '+'; }
"-"         { column++; return '-'; }
"*"         { column++; return '*'; }
"/"         { column++; return '/'; }

{WS}+       { column += yyleng; }

.           { 
    printf("Lexical error: Unexpected character '%s' at line %d, column %d\n", 
           yytext, yylineno, column);
    column++;
}

%% 