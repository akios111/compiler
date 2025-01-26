#ifndef TYPES_H
#define TYPES_H

#define MAX_ID_LENGTH 31
#define MAX_DIMENSIONS 7

typedef enum {
    TYPE_ERROR = -1,
    TYPE_VOID = 0,
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_COMPLEX,
    TYPE_LOGICAL,
    TYPE_CHARACTER,
    TYPE_STRING,
    TYPE_UNKNOWN
} type_t;

typedef enum {
    SYMBOL_ERROR = -1,
    SYMBOL_VARIABLE,
    SYMBOL_ARRAY,
    SYMBOL_FUNCTION,
    SYMBOL_CONSTANT,
    SYMBOL_EXPRESSION
} symbol_kind_t;

// Union for storing symbol values
typedef union {
    int ival;
    float rval;
    int bval;  // boolean value
    char* sval;  // string value
} value_t;

// Forward declare scope_t instead of struct scope
typedef struct scope scope_t;

typedef struct symbol {
    char name[MAX_ID_LENGTH + 1];
    type_t type;
    symbol_kind_t kind;
    int num_dimensions;
    int dimensions[MAX_DIMENSIONS];
    int offset;         // Memory offset for code generation
    scope_t* scope;     // Pointer to containing scope
    value_t value;      // Value of the symbol
    struct symbol* next;
} symbol_t;

/* Type string conversion function */
const char* get_type_str(type_t type);

// Type promotion function
type_t promote_type(type_t t1, type_t t2);

// Type checking functions
int is_numeric_type(type_t type);
int is_compatible_types(type_t t1, type_t t2);
const char* type_to_string(type_t type);

#endif /* TYPES_H */