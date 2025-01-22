#ifndef INTERMEDIATE_H
#define INTERMEDIATE_H

#include "types.h"

/* Intermediate code operation types */
typedef enum {
    IC_BINARY,
    IC_UNARY,
    IC_ASSIGN,
    IC_GOTO,
    IC_IF_GOTO,
    IC_LABEL,
    IC_CALL,
    IC_DO_START,
    IC_DO_END
} ic_type_t;

/* Temporary variable structure */
typedef struct temp_var {
    char* name;
    symbol_type_t type;
    struct temp_var* next;
} temp_var;

/* Intermediate code node structure */
typedef struct ic_node {
    ic_type_t type;
    char* result;
    char* arg1;
    char* arg2;
    int label;
    struct ic_node* next;
} ic_node;

/* Global variables */
extern ic_node* ic_head;

/* Function declarations */
temp_var* new_temp(symbol_type_t type);
ic_node* gen_binary(ic_type_t op, temp_var* result, temp_var* op1, temp_var* op2);
ic_node* gen_assign(temp_var* result, temp_var* value);
ic_node* gen_goto(const char* label);
ic_node* gen_if_goto(temp_var* cond, temp_var* target);
ic_node* gen_label(const char* label);
ic_node* gen_call(const char* func, ic_node** args, int num_args);
ic_node* gen_do_start(const char* var, temp_var* start, temp_var* end);
ic_node* gen_do_end(ic_node* start);

#endif /* INTERMEDIATE_H */ 