#ifndef INTERMEDIATE_H
#define INTERMEDIATE_H

#include "types.h"

/* Binary operation types */
typedef enum {
    BIN_ADD,
    BIN_SUB,
    BIN_MUL,
    BIN_DIV,
    BIN_MOD,
    BIN_AND,
    BIN_OR,
    BIN_LT,
    BIN_LE,
    BIN_GT,
    BIN_GE,
    BIN_EQ,
    BIN_NE
} bin_op_t;

/* Intermediate code operation types */
typedef enum {
    IC_NONE,
    IC_BINARY,
    IC_ASSIGN,
    IC_ARRAY,  /* Added for array access */
    IC_CALL,
    IC_RETURN,
    IC_IF,
    IC_WHILE,
    IC_GOTO,
    IC_IF_GOTO,
    IC_LABEL,
    IC_DO_START,
    IC_DO_END,
    IC_UNARY    /* Added for unary operations */
} ic_type_t;

/* Temporary variable structure */
typedef struct temp_var {
    char name[32];
    type_t type;
    struct temp_var* next;
} temp_var;

/* Intermediate code node structure */
typedef struct ic_node {
    ic_type_t type;
    bin_op_t bin_op;    /* For binary operations */
    char* result;
    char* arg1;
    char* arg2;
    int label;
    struct ic_node* next;
    ic_type_t op;       /* Operation type */
    union {
        struct {
            struct ic_node* left;
            struct ic_node* right;
        } binary;
        struct {
            struct ic_node* operand;
        } unary;
        struct {
            char* name;
            int index;
        } array;
        struct {
            char* name;
            struct ic_node** args;
            int arg_count;
        } call;
    } u;                /* Union for different node types */
} ic_node;

typedef struct ic_node ic_node_t;  /* Add typedef for ic_node_t */

/* Global variables */
extern ic_node* ic_head;

/* Function declarations */
temp_var* new_temp(type_t type);
void free_temp(temp_var* var);
void init_temp_vars(void);
void cleanup_temp_vars(void);
ic_node* gen_binary(bin_op_t op, temp_var* result, temp_var* op1, temp_var* op2);
ic_node* gen_assign(temp_var* result, temp_var* value);
ic_node* gen_goto(const char* label);
ic_node* gen_if_goto(temp_var* cond, temp_var* target);
ic_node* gen_label(const char* label);
ic_node* gen_call(const char* func, ic_node** args, int num_args);
ic_node* gen_do_start(const char* var, temp_var* start, temp_var* end);
ic_node* gen_do_end(ic_node* start);

#endif /* INTERMEDIATE_H */ 