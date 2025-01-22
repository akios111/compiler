%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "types.h"
#include "fort.tab.h"
#include "symtab.h"
#include "error.h"
#include "globals.h"
#include "token_buffer.h"

/* Error types */
#define ERR_LEXICAL 1
#define ERR_SYNTAX  2
#define ERR_SEMANTIC 3
#define SEV_ERROR   1
#define SEV_WARNING 2

/* Ensure proper handling of tokens */
#define YY_NO_UNPUT
#define YY_NO_INPUT
#define YY_DECL int yylex(void)

int column = 1;

#define DEBUG_TOKEN(name) if(debug_lexer) printf("Token: %s [%s]\n", name, yytext)

/* Override the default YY_INPUT to check buffered tokens first */
#define YY_INPUT(buf,result,max_size) { \
    int tok = get_buffered_token(); \
    if (tok) { \
        buf[0] = tok; \
        result = 1; \
    } else { \
        errno=0; \
        while ((result = (int) fread(buf, 1, max_size, yyin))==0 && ferror(yyin)) { \
            if (errno != EINTR) { \
                YY_FATAL_ERROR("input in flex scanner failed"); \
                break; \
            } \
            errno=0; \
            clearerr(yyin); \
        } \
    } \
}

%}

%option noyywrap
%option yylineno
%option nounput
%option never-interactive
%option case-insensitive

DIGIT       [0-9]
LETTER      [a-z]
ID          {LETTER}[a-z0-9_]*
WS          [ \t\r]
NEWLINE     \n
INTEGER     {DIGIT}+
REAL        {DIGIT}+"."{DIGIT}*([Ee][+-]?{DIGIT}+)?|{DIGIT}*"."{DIGIT}+([Ee][+-]?{DIGIT}+)?
HEX         0[xX][0-9A-Fa-f]+
OCT         0[oO][0-7]+
BIN         0[bB][01]+
STRING      \"[^\"]*\"

%%

"PROGRAM"      { DEBUG_TOKEN("PROGRAM"); return PROGRAM; }
"END"          { DEBUG_TOKEN("END"); return END; }
"INTEGER"      { DEBUG_TOKEN("INTEGER"); return INTEGER; }
"REAL"         { DEBUG_TOKEN("REAL"); return REAL; }
"COMPLEX"      { DEBUG_TOKEN("COMPLEX"); return COMPLEX; }
"LOGICAL"      { DEBUG_TOKEN("LOGICAL"); return LOGICAL; }
"CHARACTER"    { DEBUG_TOKEN("CHARACTER"); return CHARACTER; }
"STRING"       { DEBUG_TOKEN("STRING"); return STRING; }
"DATA"         { DEBUG_TOKEN("DATA"); return DATA; }
"COMMON"       { DEBUG_TOKEN("COMMON"); return COMMON; }
"IF"           { DEBUG_TOKEN("IF"); return IF; }
"THEN"         { DEBUG_TOKEN("THEN"); return THEN; }
"ELSE"         { DEBUG_TOKEN("ELSE"); return ELSE; }
"ENDIF"        { DEBUG_TOKEN("ENDIF"); return ENDIF; }
"DO"           { DEBUG_TOKEN("DO"); return DO; }
"ENDDO"        { DEBUG_TOKEN("ENDDO"); return ENDDO; }
"READ"         { DEBUG_TOKEN("READ"); return READ; }
"WRITE"        { DEBUG_TOKEN("WRITE"); return WRITE; }
"CALL"         { DEBUG_TOKEN("CALL"); return CALL; }
"RETURN"       { DEBUG_TOKEN("RETURN"); return RETURN; }
"STOP"         { DEBUG_TOKEN("STOP"); return STOP; }
"CONTINUE"     { DEBUG_TOKEN("CONTINUE"); return CONTINUE; }
"GOTO"         { DEBUG_TOKEN("GOTO"); return GOTO; }
"FUNCTION"     { DEBUG_TOKEN("FUNCTION"); return FUNCTION; }
"SUBROUTINE"   { DEBUG_TOKEN("SUBROUTINE"); return SUBROUTINE; }

"$"            { DEBUG_TOKEN("DOLLAR"); return DOLLAR; }
{INTEGER}      { yylval.sval = strdup(yytext); DEBUG_TOKEN("ICONST"); return ICONST; }
{REAL}         { yylval.sval = strdup(yytext); DEBUG_TOKEN("RCONST"); return RCONST; }
{STRING}       { yylval.sval = strdup(yytext); DEBUG_TOKEN("STRING_LITERAL"); return STRING_LITERAL; }
"*"            { DEBUG_TOKEN("STAR"); return STAR; }
"/"            { DEBUG_TOKEN("SLASH"); return SLASH; }
","            { DEBUG_TOKEN("COMMA"); return COMMA; }
"("            { DEBUG_TOKEN("LPAREN"); return LPAREN; }
")"            { DEBUG_TOKEN("RPAREN"); return RPAREN; }
"["            { DEBUG_TOKEN("LBRACKET"); return LBRACKET; }
"]"            { DEBUG_TOKEN("RBRACKET"); return RBRACKET; }
"="            { DEBUG_TOKEN("ASSIGN"); return ASSIGN; }
"+"            { DEBUG_TOKEN("PLUS"); return PLUS; }
"-"            { DEBUG_TOKEN("MINUS"); return MINUS; }
"**"           { DEBUG_TOKEN("POWER"); return POWER; }
".AND."        { DEBUG_TOKEN("AND"); return AND; }
".OR."         { DEBUG_TOKEN("OR"); return OR; }
".NOT."        { DEBUG_TOKEN("NOT"); return NOT; }
".GT."         { DEBUG_TOKEN("GT"); return GT; }
".LT."         { DEBUG_TOKEN("LT"); return LT; }
".GE."         { DEBUG_TOKEN("GE"); return GE; }
".LE."         { DEBUG_TOKEN("LE"); return LE; }
".EQ."         { DEBUG_TOKEN("EQ"); return EQ; }
".NE."         { DEBUG_TOKEN("NE"); return NE; }
".TRUE."       { DEBUG_TOKEN("TRUE"); return TRUE; }
".FALSE."      { DEBUG_TOKEN("FALSE"); return FALSE; }

{ID}           { yylval.sval = strdup(yytext); DEBUG_TOKEN("ID"); return ID; }
{WS}           ; /* Skip whitespace */
{NEWLINE}      { yylineno++; DEBUG_TOKEN("NEWLINE"); return NEWLINE; }
.              { 
    char error_msg[256];
    snprintf(error_msg, sizeof(error_msg), 
        "Invalid character '%c' at line %d", yytext[0], yylineno);
    report_error(ERR_SYNTAX, SEV_ERROR, error_msg, NULL);
}

%%
