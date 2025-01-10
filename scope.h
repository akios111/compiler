#ifndef SCOPE_H
#define SCOPE_H

#include <stdbool.h>
#include "symtab.h"

// Τύποι εμβέλειας
typedef enum {
    SCOPE_GLOBAL,
    SCOPE_FUNCTION,
    SCOPE_BLOCK,
    SCOPE_LOOP,
    SCOPE_IF
} scope_type_t;

// Δομή εμβέλειας
typedef struct scope {
    scope_type_t type;
    struct scope* parent;
    symbol_entry_t** symbols;
    int num_symbols;
    int level;
} scope_t;

// Συναρτήσεις διαχείρισης εμβέλειας
scope_t* create_scope(scope_type_t type, scope_t* parent);
void enter_scope(scope_type_t type);
void exit_scope(void);
void add_symbol_to_scope(symbol_entry_t* symbol);
symbol_entry_t* lookup_symbol_in_scope(const scope_t* scope, const char* name);
symbol_entry_t* lookup_symbol(const char* name);
void free_scope(scope_t* scope);
const scope_t* get_current_scope(void);
bool is_global_scope(void);

#endif