%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intermediate.h"
#include "fort.tab.h"

int line_number = 1;
int in_write = 0;  /* Flag για να ξέρουμε αν είμαστε μέσα σε WRITE statement */
int in_data = 0;   /* Flag για DATA statement */
char line_buf[1024] = "";  /* Buffer για την τρέχουσα γραμμή */
int line_pos = 0;          /* Θέση στο line_buf */
int collecting_line = 1;   /* Flag για να ελέγχουμε αν συλλέγουμε γραμμή */
int error_count = 0;       /* Μετρητής λαθών */

/* Function για αναφορά λεκτικών λαθών */
void lexical_error(const char* msg) {
    fprintf(stderr, "Lexical error at line %d: %s\n", line_number, msg);
    fprintf(stderr, "Line: %s\n", line_buf);
    fprintf(stderr, "      ");
    for(int i=0; i < line_pos-yyleng; i++) fprintf(stderr, " ");
    fprintf(stderr, "^\n");
    error_count++;
}
%}

/* Ορισμοί για αναγνώριση αριθμών σε διάφορες βάσεις */
DIGIT       [0-9]
NONZERO     [1-9]
HEXDIGIT    [0-9A-Fa-f]
OCTDIGIT    [0-7]
BINDIGIT    [01]
ID_START    [A-Za-z]
ID_CHAR     [A-Za-z0-9_]

%%

    /* Σχόλια - δεν αυξάνουμε το line_number εδώ γιατί θα το κάνει ο \n κανόνας */
^[Cc*].*\n      { line_number++; }
"$"[^\n]*\n    { 
    /* Έλεγχος για σωστή σύνταξη σχολίων */
    char *comment = yytext + 1; /* Παραλείπουμε το $ */
    while (*comment == ' ' || *comment == '\t') comment++; /* Παραλείπουμε τα whitespace */
    if (*comment == '\n') {
        fprintf(stderr, "Warning: Empty comment at line %d\n", line_number);
        fprintf(stderr, "Comments should contain text after the $ symbol\n");
        fprintf(stderr, "Found: %s", yytext);
    }
    line_number++; 
}
[ \t]+          /* ignore whitespace */ ;
\n              { line_number++; }

    /* Λτικέτες */
^{DIGIT}+       { yylval.ival = atoi(yytext); return LABEL; }

    /* Λέξεις-κλειδιά - case insensitive */
(?i:PROGRAM)    { return PROGRAM; }
(?i:FUNCTION)   { return FUNCTION; }
(?i:SUBROUTINE) { return SUBROUTINE; }
(?i:END)        { return END; }
(?i:COMMON)     { return COMMON; }
(?i:INTEGER)    { return INTEGER; }
(?i:REAL)       { return REAL; }
(?i:COMPLEX)    { return COMPLEX; }
(?i:LOGICAL)    { return LOGICAL; }
(?i:CHARACTER)  { return CHARACTER; }
(?i:STRING)     { return STRING; }
(?i:LIST)       { return LIST; }
(?i:DATA)       { in_data = 1; return DATA; }
(?i:CONTINUE)   { return CONTINUE; }
(?i:GOTO)       { return GOTO; }
(?i:CALL)       { return CALL; }
(?i:READ)       { return READ; }
(?i:WRITE)      { in_write = 1; return WRITE; }
(?i:IF)         { return IF; }
(?i:THEN)       { return THEN; }
(?i:ELSE)       { return ELSE; }
(?i:ENDIF)      { return ENDIF; }
(?i:DO)         { return DO; }
(?i:ENDDO)      { return ENDDO; }
(?i:STOP)       { return STOP; }
(?i:RETURN)     { return RETURN; }

    /* Λογικοί τελεστές */
"\.not\."      { return NOT; }
"\.and\."      { return AND; }
"\.or\."       { return OR; }
"\.gt\."       { return GT; }
"\.lt\."       { return LT; }
"\.eq\."       { return EQ; }
"\.le\."       { return LE; }
"\.true\."     { return TRUE; }
"\.false\."    { return FALSE; }

    /* Ακέραιες σταθερές */
0               { yylval.ival = 0; return ICONST; }
{NONZERO}{DIGIT}* { yylval.ival = atoi(yytext); return ICONST; }
0[Xx]{HEXDIGIT}+ { yylval.ival = strtol(yytext, NULL, 16); return ICONST; }
0[Oo]{OCTDIGIT}+ { yylval.ival = strtol(yytext+2, NULL, 8); return ICONST; }
0[Bb]{BINDIGIT}+ { yylval.ival = strtol(yytext+2, NULL, 2); return ICONST; }

    /* Δυαδικές σταθερές με B suffix */
[01]+B          { 
    char* endptr;
    yylval.ival = strtol(yytext, &endptr, 2);
    return ICONST; 
}
[2-9][0-9]*B    { 
    char msg[100];
    snprintf(msg, sizeof(msg), "Invalid binary constant '%s' - binary constants must contain only 0s and 1s", yytext);
    lexical_error(msg);
    yylval.ival = 0;
    return ICONST;
}
[01]+\.[0-9]*B  { 
    char msg[100];
    snprintf(msg, sizeof(msg), "Invalid binary constant '%s' - FORT does not support floating point binary numbers", yytext);
    lexical_error(msg);
    yylval.rval = 0.0;
    return RCONST;
}

    /* Πραγματικές σταθερές */
{DIGIT}+\.{DIGIT}*[Ee][+-]?{DIGIT}+ |    /* για 314.159E-31 */
{DIGIT}+[Ee][+-]?{DIGIT}+ |              /* για 5E2 */
{DIGIT}*\.{DIGIT}+[Ee][+-]?{DIGIT}+ |    /* για .314159E-31 */
{DIGIT}+\.{DIGIT}* |                      /* για 5. και 5.0 */
{DIGIT}*\.{DIGIT}+ { 
    yylval.rval = atof(yytext); 
    return RCONST; 
}

    /* Strings */
\"[^\"]*\"     { 
    yylval.sval = strdup(yytext+1);
    yylval.sval[strlen(yylval.sval)-1] = '\0';
    return STRING_LITERAL; 
}

    /* Αναγνωριστικά */
{ID_START}{ID_CHAR}* {
    char *p = yytext;
    int len = strlen(p);
    int has_underscore = 0;
    int valid = 1;

    /* Έλεγχος για διαδοχικά _ και _ στο τέλος */
    for(int i = 0; i < len; i++) {
        if (p[i] == '_') {
            has_underscore = 1;
            if (i < len-1 && p[i+1] == '_') {
                valid = 0;
                break;
            }
        }
    }

    /* Αν έχει _ πρέπει να τελειώνει σε _ */
    if (has_underscore && p[len-1] != '_') {
        valid = 0;
    }

    if (valid) {
        yylval.sval = strdup(yytext);
        return ID;
    } else {
        fprintf(stderr, "Lexical error at line %d: Invalid identifier '%s'\n", 
               line_number, yytext);
        fprintf(stderr, "Line: %s\n", line_buf);
        fprintf(stderr, "      ");
        for(int i=0; i < line_pos-yyleng; i++) fprintf(stderr, " ");
        fprintf(stderr, "^\n");
    }
}

    /* Τελεστές και σύμβολα */
"("[ \t]*"="    { return LPAREN_ASSIGN; }
"("             { return LPAREN; }
"="             { return ASSIGN; }
"+"             { return ADDOP; }
"-"             { return ADDOP; }
"*"             { 
    if (in_write || in_data) {
        return STAR;
    } else {
        return MULOP;
    }
}
"/"             { return DIVOP; }
"**"            { return POWEROP; }
")"             { return RPAREN; }
","             { return COMMA; }
":"             { return COLON; }
"_"             { return '_'; }

    /* Χειρισμός σφαλμάτων */
"["             { lexical_error("Invalid token '['"); }
"]"             { lexical_error("Invalid token ']'"); }
.               { 
    char msg[100];
    snprintf(msg, sizeof(msg), "Invalid character '%s'", yytext);
    lexical_error(msg);
}

%%

int yywrap(void) {
    return 1;
}
