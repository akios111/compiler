#ifndef SYMTAB_H
#define SYMTAB_H

#include <ctype.h>
#include "types.h"

#define MAX_SYMBOLS 1000
#define MAX_COMMON_BLOCKS 100

typedef struct {
    char* name;
    int type;
    int is_array;
    int dimensions;
    int* dim_sizes;
    char* common_block;
} symbol_t;

typedef struct {
    char* name;
    symbol_t* symbols;
    int symbol_count;
} common_block_t;

extern symbol_t symbol_table[MAX_SYMBOLS];
extern int symbol_count;

extern common_block_t common_blocks[MAX_COMMON_BLOCKS];
extern int common_block_count;

/* Function declarations */
void init_symtab(void);
int add_symbol(const char* name, int type);
int add_array_symbol(const char* name, int type);
symbol_t* lookup_symbol(const char* name);
int is_array(const char* name);
int count_expressions(const char* expr_list);
int check_array_dimensions(const char* name, int dimensions);
int check_array_dimension(const char* dim_name);
int check_assignment_types(const char* target, const char* source);
int get_symbol_type(const char* name);
int check_data_values_type(const char* values, int expected_type);
int check_complex_components(const char* real_part, const char* imag_part);
int has_program_declaration(void);
int process_common_block(const char* block_name, const char* variables);
int check_function_call(const char* name, const char* args);
void set_current_loop_var(const char* var);
int check_loop_variable_type(void);

// Validation functions
int check_valid_identifiers(const char* id_list);
int is_duplicate_common_block(const char* block_name);
int check_data_types(const char* data_decl);
int count_dimensions(const char* dim_list);
int check_array_assignment(const char* array_name, const char* expr);
int check_expression_variables(const char* expr);
int check_logical_expression(const char* expr);
int check_loop_bounds(const char* bounds);

/* Function to check if a symbol is declared */
int is_declared(const char* name);

#endif /* SYMTAB_H */ 