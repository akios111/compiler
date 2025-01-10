#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scope.h"
#include "symbol.h"
#include "error.h"

// Τρέχουσα εμβέλεια
static scope_t* current_scope = NULL;

// Δημιουργία νέας εμβέλειας
scope_t* create_scope(scope_type_t type, scope_t* parent) {
    scope_t* scope = malloc(sizeof(scope_t));
    scope->type = type;
    scope->parent = parent;
    scope->symbols = NULL;
    scope->num_symbols = 0;
    scope->level = parent ? parent->level + 1 : 0;
    return scope;
}

// Είσοδος σε νέα εμβέλεια
void enter_scope(scope_type_t type) {
    scope_t* new_scope = create_scope(type, current_scope);
    current_scope = new_scope;
}

// Έξοδος από την τρέχουσα εμβέλεια
void exit_scope(void) {
    if (!current_scope) {
        report_error(ERROR_SCOPE, "Cannot exit global scope");
        return;
    }
    
    scope_t* parent = current_scope->parent;
    free_scope(current_scope);
    current_scope = parent;
}

// Προσθήκη συμβόλου στην τρέχουσα εμβέλεια
void add_symbol_to_scope(symbol_t* symbol) {
    if (!current_scope || !symbol) return;
    
    // Έλεγχος για διπλό ορισμό
    if (lookup_symbol_in_scope(current_scope, symbol->name)) {
        report_error(ERROR_SYMBOL, "Symbol '%s' already defined in current scope", 
                    symbol->name);
        return;
    }
    
    // Επέκταση πίνακα συμβόλων
    current_scope->num_symbols++;
    current_scope->symbols = realloc(current_scope->symbols, 
                                   current_scope->num_symbols * sizeof(symbol_t*));
    
    current_scope->symbols[current_scope->num_symbols - 1] = symbol;
    symbol->scope = current_scope;
}

// Αναζήτηση συμβόλου στην τρέχουσα εμβέλεια
symbol_t* lookup_symbol_in_scope(scope_t* scope, const char* name) {
    if (!scope || !name) return NULL;
    
    for (int i = 0; i < scope->num_symbols; i++) {
        if (strcmp(scope->symbols[i]->name, name) == 0) {
            return scope->symbols[i];
        }
    }
    return NULL;
}

// Αναζήτηση συμβόλου σε όλες τις εμβέλειες
symbol_t* lookup_symbol(const char* name) {
    if (!name) return NULL;
    
    scope_t* scope = current_scope;
    while (scope) {
        symbol_t* symbol = lookup_symbol_in_scope(scope, name);
        if (symbol) return symbol;
        scope = scope->parent;
    }
    return NULL;
}

// Απελευθέρωση εμβέλειας
void free_scope(scope_t* scope) {
    if (!scope) return;
    
    // Απελευθέρωση συμβόλων
    for (int i = 0; i < scope->num_symbols; i++) {
        free_symbol(scope->symbols[i]);
    }
    
    free(scope->symbols);
    free(scope);
}

// Λήψη τρέχουσας εμβέλειας
scope_t* get_current_scope(void) {
    return current_scope;
}

// Έλεγχος αν είμαστε στην καθολική εμβέλεια
bool is_global_scope(void) {
    return current_scope == NULL || current_scope->parent == NULL;
} 