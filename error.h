#ifndef ERROR_H
#define ERROR_H

/* Error types */
typedef enum {
    ERR_SYNTAX,          /* Basic syntax errors */
    ERR_DECLARATION,     /* Declaration related errors */
    ERR_TYPE,           /* Type mismatch errors */
    ERR_SCOPE,          /* Scope related errors */
    ERR_UNDEFINED,      /* Undefined identifier errors */
    ERR_CONSTANT,       /* Invalid constant errors */
    ERR_ARRAY,          /* Array related errors */
    ERR_STATEMENT,      /* Invalid statement errors */
    ERR_DATA,           /* DATA statement errors */
    ERR_COMMON,         /* COMMON block errors */
    ERR_SUBROUTINE,     /* Subroutine related errors */
    ERR_LABEL,          /* Label related errors */
    ERR_FUNCTION,       /* Function related errors */
    ERR_EXPRESSION,     /* Expression related errors */
    ERR_IO              /* Input/Output related errors */
} error_type_t;

/* Error severity levels */
typedef enum {
    SEV_WARNING,    /* Warning - compilation continues */
    SEV_ERROR,      /* Error - compilation continues but code generation may be affected */
    SEV_FATAL       /* Fatal error - compilation stops */
} error_severity_t;

/* Function declarations */
void report_error(error_type_t type, error_severity_t severity, const char* msg, const char* detail);
void report_warning(const char* msg, const char* detail);
void report_semantic_error(const char* msg);
void type_error(const char* msg);
void report_type_error(const char* expected, const char* found);
int get_error_count(void);
void reset_error_count(void);
void print_error_summary(void);

/* FORT-specific error checking functions */
void check_declaration(const char* identifier, const char* type);
void check_data_statement(const char* var, const char* value);
void check_common_block(const char* block_name, const char* vars);
void check_array_declaration(const char* array_name, const char* dims);
void check_expression(const char* expr);
void report_program_structure_error(void);
void report_declaration_error(const char* found);
void report_invalid_identifier(const char* detail);

#endif /* ERROR_H */