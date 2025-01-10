#ifndef SYMTAB_H
#define SYMTAB_H

#include "types.h"

/* Δομή συμβόλου */
typedef struct symbol {
    char *name;
    symbol_type_t type;
    int is_array;
    int array_size;
    struct symbol *next;
} symbol;

/* Διαχείριση πίνακα συμβόλων */
void init_symbol_table(void);
void add_symbol(const char *name, symbol_type_t type);
symbol* lookup_symbol(const char *name);
void free_symbol_table(void);
int check_symbol_declared(const char *name);

/* Συναρτήσεις για COMMON blocks και DATA */
void add_common_block(const char *name, char **vars, int num_vars);
void process_data_item(const char *name, int *values, int num_values);

/* Συναρτήσεις ελέγχου */
int is_valid_array_dimension(void);
int check_data_type_compatibility(const char *var_name);
int check_loop_variable_type(void);
void set_current_loop_var(const char *var_name);
int check_expression_type(void);
int is_valid_function_call(const char *func_name);
int check_array_bounds(const char *name, int index);
int check_array_index_type(const char *array_name, const char *index_name);

/* Βοηθητικές συναρτήσεις */
const char* type_to_string(symbol_type_t type);

#endif 