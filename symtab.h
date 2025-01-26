#ifndef SYMTAB_H
#define SYMTAB_H

#include "types.h"
#include "scope.h"

#define MAX_SYMBOLS 1000

/* Function declarations for symbol table management */
void init_symbol_table(void);
symbol_t* add_symbol(const char* name, type_t type);
symbol_t* add_array_symbol(const char* name, type_t type, int dimensions);
symbol_t* add_function_symbol(const char* name, type_t return_type);
symbol_t* lookup_symbol(const char* name);
void remove_symbol(const char* name);

/* Type checking and validation */
type_t get_symbol_type(const char* name);
int is_array(const char* name);
int check_array_dimensions(const char* name, int dimensions);
int check_array_index(const char* array_name, const char* index_name);
int check_function_call(const char* name);
int check_loop_variable_type(const char* name);
int check_numeric_types(const char* var1, const char* var2);
int check_array_assignment(const char* name);
int check_variable_exists(const char* name);
int is_function(const char* name);

/* Declaration functions */
int declare_variable(const char* name, type_t type);
int declare_array(const char* name, type_t type, int dimensions);
int process_common_block(const char* block_name, const char** var_names, type_t* types, int num_vars);

/* Debug and display functions */
void print_symbol_table(void);

/* External symbol lists */
extern symbol_t* global_symbols;
extern symbol_t* local_symbols;

#endif /* SYMTAB_H */ 