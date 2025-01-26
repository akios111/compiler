#ifndef SCOPE_H
#define SCOPE_H

#include <stdbool.h>
#include "types.h"

// Scope types
typedef enum {
    SCOPE_GLOBAL,
    SCOPE_FUNCTION,
    SCOPE_BLOCK,
    SCOPE_LOOP,
    SCOPE_IF
} scope_type_t;

// Scope structure
typedef struct scope {
    scope_type_t type;
    struct scope* parent;
    int level;
    symbol_t** symbols;  // Array of symbol pointers
    int num_symbols;     // Number of symbols in this scope
} scope_t;

// Function declarations
void init_scope(void);
void enter_scope(void);
void exit_scope(void);
int get_current_scope(void);
bool is_global_scope(void);
scope_t* get_scope_by_level(int level);
void free_scope(scope_t* scope);
void add_symbol_to_scope(symbol_t* symbol);
symbol_t* lookup_symbol_in_scope(const char* name, int scope_level);
void print_scope_symbols(int scope_level);

#endif /* SCOPE_H */ 