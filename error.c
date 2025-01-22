#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "globals.h"

/* Maximum number of errors to report before stopping */
#define MAX_ERRORS 50

extern int line_number;
extern char current_line_buf[];
extern int current_line_pos;

extern int error_count;
static int warning_count = 0;

/* Error type strings for better messages */
static const char* error_type_str[] = {
    "Syntax",
    "Declaration",
    "Type",
    "Scope",
    "Undefined",
    "Constant",
    "Array",
    "Statement",
    "DATA",
    "COMMON",
    "Subroutine",
    "Label"
};

/* Severity strings */
static const char* severity_str[] = {
    "Warning",
    "Error",
    "Fatal"
};

void report_error(error_type_t type, error_severity_t severity, const char* msg, const char* detail) {
    if (error_count >= MAX_ERRORS) {
        print_error_summary();
        exit(1);
    }

    /* Print error location and type */
    fprintf(stderr, "\nLine %d: %s %s: %s", line_number, error_type_str[type], severity_str[severity], msg);
    
    /* Print additional details if available */
    if (detail) {
        fprintf(stderr, "\n  Details: %s", detail);
    }
    
    /* Print current line for context */
    if (current_line_buf[0] != '\0') {
        fprintf(stderr, "\n  Context: %s", current_line_buf);
        /* Print caret pointing to error position */
        if (current_line_pos > 0) {
            fprintf(stderr, "\n           ");
            for (int i = 0; i < current_line_pos - 1; i++) {
                fprintf(stderr, " ");
            }
            fprintf(stderr, "^");
        }
    }

    /* Print suggestion based on error type */
    switch (type) {
        case ERR_SYNTAX:
            fprintf(stderr, "\n  Suggestion: Check for missing semicolons, parentheses, or keywords");
            break;
        case ERR_DECLARATION:
            fprintf(stderr, "\n  Suggestion: Ensure variable is declared before use and follows FORT syntax");
            break;
        case ERR_TYPE:
            fprintf(stderr, "\n  Suggestion: Check variable types match in assignment/operation");
            break;
        case ERR_SCOPE:
            fprintf(stderr, "\n  Suggestion: Verify variable/function declarations are in the correct scope");
            break;
        case ERR_UNDEFINED:
            fprintf(stderr, "\n  Suggestion: Ensure all variables and functions are declared before use");
            break;
        case ERR_CONSTANT:
            fprintf(stderr, "\n  Suggestion: Check constant format matches FORT specifications");
            break;
        case ERR_ARRAY:
            fprintf(stderr, "\n  Suggestion: Verify array dimensions and index expressions");
            break;
        case ERR_STATEMENT:
            fprintf(stderr, "\n  Suggestion: Check statement syntax and placement");
            break;
        case ERR_DATA:
            fprintf(stderr, "\n  Suggestion: Check DATA statement format and value types");
            break;
        case ERR_COMMON:
            fprintf(stderr, "\n  Suggestion: Verify COMMON block syntax and variable declarations");
            break;
        case ERR_SUBROUTINE:
            fprintf(stderr, "\n  Suggestion: Check subroutine declaration and call syntax");
            break;
        case ERR_LABEL:
            fprintf(stderr, "\n  Suggestion: Verify label format and ensure all referenced labels exist");
            break;
        case ERR_FUNCTION:
            fprintf(stderr, "\n  Suggestion: Check function declaration and call syntax");
            break;
        case ERR_EXPRESSION:
            fprintf(stderr, "\n  Suggestion: Verify expression syntax and operator precedence");
            break;
        case ERR_IO:
            fprintf(stderr, "\n  Suggestion: Check READ/WRITE statement format and arguments");
            break;
    }
    
    fprintf(stderr, "\n");

    if (severity >= SEV_ERROR) {
        error_count++;
    } else {
        warning_count++;
    }
}

void report_warning(const char* msg, const char* detail) {
    report_error(ERR_SYNTAX, SEV_WARNING, msg, detail);
}

void report_semantic_error(const char* msg) {
    report_error(ERR_TYPE, SEV_ERROR, msg, NULL);
}

void type_error(const char* msg) {
    report_error(ERR_TYPE, SEV_ERROR, msg, NULL);
}

void report_type_error(const char* expected, const char* found) {
    char detail[1024];
    snprintf(detail, sizeof(detail), "Expected type: %s\nFound type: %s", expected, found);
    report_error(ERR_TYPE, SEV_ERROR, "Type mismatch", detail);
}

void check_declaration(const char* identifier, const char* type) {
    if (!in_program) {
        report_error(ERR_SCOPE, SEV_ERROR,
            "Declaration outside program scope",
            identifier);
        return;
    }
    
    /* Check for valid FORT types */
    if (strcmp(type, "INTEGER") != 0 && 
        strcmp(type, "REAL") != 0 && 
        strcmp(type, "COMPLEX") != 0 && 
        strcmp(type, "LOGICAL") != 0 && 
        strcmp(type, "CHARACTER") != 0) {
        char detail[256];
        snprintf(detail, sizeof(detail), 
                "Type '%s' is not a valid FORT type. Valid types are: INTEGER, REAL, COMPLEX, LOGICAL, CHARACTER",
                type);
        report_error(ERR_TYPE, SEV_ERROR, "Invalid type in declaration", detail);
    }
}

void check_data_statement(const char* var, const char* value) {
    if (!in_program) {
        report_error(ERR_DATA, SEV_ERROR,
            "DATA statement outside program scope",
            var);
        return;
    }
    
    /* Check for invalid DATA statement formats */
    if (strchr(value, ':') != NULL) {
        report_error(ERR_DATA, SEV_ERROR,
            "Invalid DATA statement format",
            "Range notation (:) not allowed in DATA statements");
        return;
    }

    if (strchr(value, '*') != NULL) {
        report_error(ERR_DATA, SEV_ERROR,
            "Invalid DATA statement format",
            "Asterisk (*) not allowed in DATA statements");
        return;
    }

    /* Check for type mismatches in DATA statements */
    if ((strstr(value, ".true.") != NULL || strstr(value, ".false.") != NULL) &&
        !strstr(var, "LOGICAL")) {
        report_error(ERR_DATA, SEV_ERROR,
            "Type mismatch in DATA statement",
            "Logical values can only be assigned to LOGICAL variables");
    }
}

void check_array_declaration(const char* array_name, const char* dims) {
    /* Check for non-constant dimensions */
    if (strchr(dims, 'g') != NULL || strchr(dims, 'y') != NULL || 
        strchr(dims, 'x') != NULL || strchr(dims, 'i') != NULL) {
        report_error(ERR_ARRAY, SEV_ERROR,
            "Invalid array dimension",
            "Array bounds must be constant expressions");
    }
}

void check_expression(const char* expr) {
    /* Check for invalid operators or constants */
    if (strstr(expr, "0x") != NULL || strstr(expr, "0B") != NULL) {
        report_error(ERR_CONSTANT, SEV_ERROR,
            "Invalid constant format",
            "Hexadecimal and binary constants are not supported in FORT");
    }
}

void report_program_structure_error(void) {
    report_error(ERR_SYNTAX, SEV_ERROR,
        "Invalid program structure",
        "Program must follow format:\nPROGRAM name\n  declarations\n  statements\nEND");
}

void report_declaration_error(const char* found) {
    char detail[256];
    snprintf(detail, sizeof(detail),
            "Found: %s\n"
            "All declarations must follow the PROGRAM statement and come before any executable statements",
            found);
    report_error(ERR_DECLARATION, SEV_ERROR, "Invalid declaration placement", detail);
}

void report_invalid_identifier(const char* detail) {
    report_error(ERR_DECLARATION, SEV_ERROR, "Invalid identifier", detail);
}

int get_error_count(void) {
    return error_count;
}

void reset_error_count(void) {
    error_count = 0;
    warning_count = 0;
}

void print_error_summary(void) {
    fprintf(stderr, "\nCompilation Summary:\n");
    fprintf(stderr, "  Errors: %d\n", error_count);
    fprintf(stderr, "  Warnings: %d\n", warning_count);
    if (error_count > 0) {
        fprintf(stderr, "\nFix the above errors and try again.\n");
    }
} 