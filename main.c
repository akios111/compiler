#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "symtab.h"

extern FILE* yyin;
extern int yyparse(void);
extern void init_symtab(void);
extern void init_line_buffer(void);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s input_file\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
        return 1;
    }

    init_symbol_table();
    init_line_buffer();

    if (yyparse() == 0 && error_count == 0) {
        printf("Compilation successful\n");
        return 0;
    }

    return 1;
}