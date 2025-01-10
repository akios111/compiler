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

/* Debugging function - μόνο όταν ορίζεται DEBUG */
#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

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
^[Cc*].*\n      { DEBUG_PRINT("COMMENT at line %d\n", line_number); }
^\$.*\n         { DEBUG_PRINT("COMMENT at line %d\n", line_number); }
[ \t]+          /* ignore whitespace */ ;
\n              { DEBUG_PRINT("Line number increased from %d to %d\n", line_number, line_number + 1);
                  line_number++; }

    /* Λτικέτες */
^{DIGIT}+       { DEBUG_PRINT("LABEL(%s) at line %d\n", yytext, line_number);
                  yylval.ival = atoi(yytext); return LABEL; }

    /* Λέξεις-κλειδιά - case insensitive */
(?i:PROGRAM)    { DEBUG_PRINT("PROGRAM at line %d\n", line_number); return PROGRAM; }
(?i:FUNCTION)   { DEBUG_PRINT("FUNCTION at line %d\n", line_number); return FUNCTION; }
(?i:SUBROUTINE) { DEBUG_PRINT("SUBROUTINE at line %d\n", line_number); return SUBROUTINE; }
(?i:END)        { DEBUG_PRINT("END at line %d\n", line_number); return END; }
(?i:COMMON)     { DEBUG_PRINT("COMMON at line %d\n", line_number); return COMMON; }
(?i:INTEGER)    { DEBUG_PRINT("INTEGER at line %d\n", line_number); return INTEGER; }
(?i:REAL)       { DEBUG_PRINT("REAL at line %d\n", line_number); return REAL; }
(?i:COMPLEX)    { DEBUG_PRINT("COMPLEX at line %d\n", line_number); return COMPLEX; }
(?i:LOGICAL)    { DEBUG_PRINT("LOGICAL at line %d\n", line_number); return LOGICAL; }
(?i:CHARACTER)  { DEBUG_PRINT("CHARACTER at line %d\n", line_number); return CHARACTER; }
(?i:STRING)     { DEBUG_PRINT("STRING at line %d\n", line_number); return STRING; }
(?i:LIST)       { DEBUG_PRINT("LIST at line %d\n", line_number); return LIST; }
(?i:DATA)       { DEBUG_PRINT("DATA token at line %d\n", line_number); in_data = 1; return DATA; }
(?i:CONTINUE)   { DEBUG_PRINT("CONTINUE at line %d\n", line_number); return CONTINUE; }
(?i:GOTO)       { DEBUG_PRINT("GOTO at line %d\n", line_number); return GOTO; }
(?i:CALL)       { DEBUG_PRINT("CALL at line %d\n", line_number); return CALL; }
(?i:READ)       { DEBUG_PRINT("READ at line %d\n", line_number); return READ; }
(?i:WRITE)      { DEBUG_PRINT("WRITE at line %d\n", line_number); in_write = 1; return WRITE; }
(?i:IF)         { DEBUG_PRINT("IF at line %d\n", line_number); return IF; }
(?i:THEN)       { DEBUG_PRINT("THEN at line %d\n", line_number); return THEN; }
(?i:ELSE)       { DEBUG_PRINT("ELSE at line %d\n", line_number); return ELSE; }
(?i:ENDIF)      { DEBUG_PRINT("ENDIF at line %d\n", line_number); return ENDIF; }
(?i:DO)         { DEBUG_PRINT("DO at line %d\n", line_number); return DO; }
(?i:ENDDO)      { DEBUG_PRINT("ENDDO at line %d\n", line_number); return ENDDO; }
(?i:STOP)       { DEBUG_PRINT("STOP at line %d\n", line_number); return STOP; }
(?i:RETURN)     { DEBUG_PRINT("RETURN at line %d\n", line_number); return RETURN; }

    /* Λογικοί τελεστές */
"\.not\."      { DEBUG_PRINT("NOT at line %d\n", line_number); return NOT; }
"\.and\."      { DEBUG_PRINT("AND at line %d\n", line_number); return AND; }
"\.or\."       { DEBUG_PRINT("OR at line %d\n", line_number); return OR; }
"\.gt\."       { DEBUG_PRINT("GT at line %d\n", line_number); return GT; }
"\.lt\."       { DEBUG_PRINT("LT at line %d\n", line_number); return LT; }
"\.eq\."       { DEBUG_PRINT("EQ at line %d\n", line_number); return EQ; }
"\.le\."       { DEBUG_PRINT("LE at line %d\n", line_number); return LE; }
"\.true\."     { DEBUG_PRINT("TRUE at line %d\n", line_number); return TRUE; }
"\.false\."    { DEBUG_PRINT("FALSE at line %d\n", line_number); return FALSE; }

    /* Ακέραιες σταθερές */
0               { DEBUG_PRINT("ICONST(0) at line %d\n", line_number); 
                  yylval.ival = 0; return ICONST; }
{NONZERO}{DIGIT}* { 
                  DEBUG_PRINT("ICONST(%s) at line %d\n", yytext, line_number);
                  yylval.ival = atoi(yytext); return ICONST; }
0[Xx]{HEXDIGIT}+ { 
                  DEBUG_PRINT("ICONST(0x%s) at line %d\n", yytext+2, line_number);
                  yylval.ival = strtol(yytext, NULL, 16); return ICONST; }
0[Oo]{OCTDIGIT}+ { 
                  DEBUG_PRINT("ICONST(0o%s) at line %d\n", yytext+2, line_number);
                  yylval.ival = strtol(yytext+2, NULL, 8); return ICONST; }
0[Bb]1{BINDIGIT}* { 
                  DEBUG_PRINT("ICONST(0b%s) at line %d\n", yytext+2, line_number);
                  yylval.ival = strtol(yytext+2, NULL, 2); return ICONST; }
[0-9]+B                  { yylval.ival = strtol(yytext, NULL, 2); return ICONST; }
[0-9]+\.[0-9]*B         { yylval.rval = strtod(yytext, NULL); return RCONST; }
[0-9]*\.[0-9]+B         { yylval.rval = strtod(yytext, NULL); return RCONST; }

    /* Πραγματικές σταθερές */
{DIGIT}+\.{DIGIT}*[Ee][+-]?{DIGIT}+ |    /* για 314.159E-31 */
{DIGIT}+[Ee][+-]?{DIGIT}+ |              /* για 5E2 */
{DIGIT}*\.{DIGIT}+[Ee][+-]?{DIGIT}+ |    /* για .314159E-31 */
{DIGIT}+\.{DIGIT}* |                      /* για 5. και 5.0 */
{DIGIT}*\.{DIGIT}+ { 
    DEBUG_PRINT("RCONST(%s) at line %d\n", yytext, line_number);
    yylval.rval = atof(yytext); 
    return RCONST; 
}

    /* Strings */
\"[^\"]*\"     { DEBUG_PRINT("STRING_LITERAL(%s) at line %d\n", yytext, line_number);
                 yylval.sval = strdup(yytext+1);
                 yylval.sval[strlen(yylval.sval)-1] = '\0';
                 return STRING_LITERAL; }

    /* Αναγνωριστικά */
{ID_START}{ID_CHAR}* {
    char *p = yytext;
    int len = strlen(p);
    int has_underscore = 0;
    int valid = 1;
    char next;

    /* Αποθήκευση του ID χωρίς το υπόλοιπο αρχείο */
    char id_buf[256];
    strncpy(id_buf, yytext, sizeof(id_buf)-1);
    id_buf[sizeof(id_buf)-1] = '\0';

    DEBUG_PRINT("DEBUG LEXER: Starting ID analysis for '%s'\n", id_buf);
    DEBUG_PRINT("DEBUG LEXER: Current line (len=%d): '%s'\n", line_pos, line_buf);
    DEBUG_PRINT("DEBUG LEXER: Position: %d\n", line_pos);
    next = input();
    DEBUG_PRINT("DEBUG LEXER: Next char will be: '%c'\n", next);
    unput(next);

    /* Έλεγχος για διαδοχικά _ και _ στο τέλος */
    for(int i = 0; i < len && i < sizeof(id_buf)-1; i++) {
        if (p[i] == '_') {
            has_underscore = 1;
            if (i < len-1 && p[i+1] == '_') {
                valid = 0;
                DEBUG_PRINT("DEBUG LEXER: Found consecutive underscores\n");
                break;
            }
        }
    }

    /* Αν έχει _ πρέπει να τελειώνει σε _ */
    if (has_underscore && p[len-1] != '_') {
        valid = 0;
        DEBUG_PRINT("DEBUG LEXER: Underscore found but not at end\n");
    }

    if (valid) {
        DEBUG_PRINT("DEBUG LEXER: Valid ID '%s' found\n", id_buf);
        DEBUG_PRINT("ID(%s) at line %d\n", id_buf, line_number);
        yylval.sval = strdup(id_buf);
        return ID;
    } else {
        DEBUG_PRINT("Lexical error at line %d: Invalid identifier '%s'\n", 
               line_number, id_buf);
        DEBUG_PRINT("Line: %s\n", line_buf);
        DEBUG_PRINT("      ");
        for(int i=0; i < line_pos-yyleng; i++) DEBUG_PRINT(" ");
        DEBUG_PRINT("^\n");
    }
}

    /* Τελεστές και σύμβολα */
"("[ \t]*"="    { DEBUG_PRINT("LPAREN_ASSIGN at line %d\n", line_number); return LPAREN_ASSIGN; }
"("             { DEBUG_PRINT("LPAREN at line %d\n", line_number); return LPAREN; }
"="             { DEBUG_PRINT("ASSIGN at line %d\n", line_number); return ASSIGN; }
"+"             { DEBUG_PRINT("ADDOP(+) at line %d\n", line_number); return ADDOP; }
"-"             { DEBUG_PRINT("ADDOP(-) at line %d\n", line_number); return ADDOP; }
"*"             { 
                  DEBUG_PRINT("STAR/MULOP at line %d\n", line_number); 
                  if (in_write || in_data) {
                    DEBUG_PRINT("Treating * as STAR (in_write=%d, in_data=%d)\n", 
                           in_write, in_data);
                    return STAR;
                  } else {
                    DEBUG_PRINT("Treating * as MULOP\n");
                    return MULOP;
                  }
                }
"/"             { DEBUG_PRINT("DIVOP at line %d\n", line_number); return DIVOP; }
"**"            { DEBUG_PRINT("POWEROP at line %d\n", line_number); return POWEROP; }
")"             { DEBUG_PRINT("RPAREN at line %d\n", line_number); return RPAREN; }
","             { DEBUG_PRINT("COMMA at line %d\n", line_number); return COMMA; }
":"             { DEBUG_PRINT("COLON at line %d\n", line_number); return COLON; }
"_"             { DEBUG_PRINT("UNDERSCORE at line %d\n", line_number); return '_'; }

    /* Χειρισμός σφαλμάτων - τώρα πιο συγκεκριμένος */
"["             { lexical_error("Invalid token '['"); }
"]"             { lexical_error("Invalid token ']'"); }
.               { 
    char msg[100];
    snprintf(msg, sizeof(msg), "Invalid character '%s'", yytext);
    lexical_error(msg);
}

    /* Συλλογή χαρακτήρων για εκτύπωση γραμμής σε σφάλματα */
.|\n {
    if (collecting_line) {
        if (yytext[0] == '\n') {
            line_buf[line_pos] = '\0';
            line_pos = 0;
        } else if (line_pos < sizeof(line_buf)-1) {
            line_buf[line_pos] = yytext[0];
            line_pos++;
            line_buf[line_pos] = '\0';
        }
        collecting_line = 0;  /* Σταματάμε τη συλλογή μέχρι τον επόμενο χαρακτήρα */
        yyless(0);
    } else {
        collecting_line = 1;  /* Επανενεργοποιούμε τη συλλογή για τον επόμενο χαρακτήρα */
        /* Προωθούμε τον χαρακτήρα στον επόμενο κανόνα */
        REJECT;
    }
}

%%

int yywrap(void) {
    return 1;
}
