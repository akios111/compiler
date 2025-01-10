#include <stdio.h>
#include <string.h>
#include "error.h"

extern int line_number;
extern char line_buf[];
extern int line_pos;

/* Η συνάρτηση yyerror ορίζεται στο fort.y */
extern void yyerror(const char* msg);

void semantic_error(const char* msg) {
    fprintf(stderr, "\nSemantic error at line %d: %s\n", line_number, msg);
    fprintf(stderr, "Line: %s\n", line_buf);
}

void type_error(const char* expected, const char* found) {
    fprintf(stderr, "\nType error at line %d: Expected %s but found %s\n", 
            line_number, expected, found);
    fprintf(stderr, "Line: %s\n", line_buf);
} 