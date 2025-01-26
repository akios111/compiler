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
    "None",
    "Declaration",
    "Array",
    "Range",
    "Common Block",
    "Type",
    "Invalid",
    "Redeclaration",
    "Syntax",
    "Scope",
    "Undefined",
    "Constant",
    "Statement",
    "Data",
    "Subroutine",
    "Label",
    "Function",
    "Expression",
    "I/O",
    "Semantic",
    "Duplicate"
};

/* Severity level strings */
static const char* severity_str[] = {
    "Warning",
    "Error",
    "Fatal Error"
};

void report_error(error_type_t type, error_severity_t severity, const char* msg, const char* detail) {
    if (severity == SEV_ERROR || severity == SEV_FATAL) {
        error_count++;
    } else {
        warning_count++;
    }
    
    fprintf(stderr, "Line %d: %s %s: %s\n", 
            line_number, 
            severity_str[severity],
            error_type_str[type],
            msg);
            
    if (detail) {
        fprintf(stderr, "Detail: %s\n", detail);
    }
    
    // Print the line where the error occurred
    if (current_line_buf[0] != '\0') {
        fprintf(stderr, "%s\n", current_line_buf);
    }
    
    if (severity == SEV_FATAL || error_count >= MAX_ERRORS) {
        exit(1);
    }
}

void report_warning(const char* msg, const char* detail) {
    report_error(ERR_NONE, SEV_WARNING, msg, detail);
}

void report_semantic_error(const char* msg) {
    report_error(ERR_SEMANTIC, SEV_ERROR, msg, NULL);
}

void type_error(const char* msg) {
    report_error(ERR_TYPE, SEV_ERROR, msg, NULL);
}

void report_type_error(const char* expected, const char* found) {
    char buf[256];
    snprintf(buf, sizeof(buf), "Expected %s but found %s", expected, found);
    report_error(ERR_TYPE, SEV_ERROR, buf, NULL);
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

void report_declaration_error(const char* identifier, const char* context) {
    char error_msg[512];
    if (!in_program && !in_subprogram) {
        snprintf(error_msg, sizeof(error_msg), 
            "Declaration of '%s' outside any program unit", 
            identifier);
        report_context_error(ERR_SCOPE, SEV_ERROR, error_msg,
            "All declarations must be inside a PROGRAM, FUNCTION, or SUBROUTINE");
    } else {
        snprintf(error_msg, sizeof(error_msg), 
            "Invalid declaration of '%s'", 
            identifier);
        report_context_error(ERR_DECLARATION, SEV_ERROR, error_msg, context);
    }
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
    if (error_count > 0 || warning_count > 0) {
        fprintf(stderr, "\nCompilation finished with %d error(s) and %d warning(s)\n",
                error_count, warning_count);
    }
} 