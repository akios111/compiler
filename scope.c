#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "scope.h"  // Include this first for scope_t definition
#include "error.h"
#include "symtab.h"
#include "types.h"

#define MAX_SCOPE_DEPTH 100

static int current_scope_level = 0;
static bool has_error = false;
static scope_t* current_scope = NULL;  // Add global scope pointer

// Forward declarations
static scope_t* create_scope(scope_type_t type, scope_t* parent);
scope_t* get_scope_by_level(int level);

void init_scope(void) {
    current_scope_level = 0;
    if (current_scope) {
        free(current_scope);
    }
    current_scope = malloc(sizeof(scope_t));
    if (current_scope) {
        current_scope->type = SCOPE_GLOBAL;
        current_scope->parent = NULL;
        current_scope->level = 0;
    }
}

void enter_scope(void) {
    if (current_scope_level >= MAX_SCOPE_DEPTH - 1) {
        report_error(ERR_SCOPE, SEV_ERROR, "Maximum scope depth exceeded", NULL);
        has_error = true;
        return;
    }
    current_scope_level++;
    current_scope = create_scope(SCOPE_BLOCK, current_scope);
}

void exit_scope(void) {
    if (current_scope_level > 0) {
        print_scope_symbols(current_scope_level);
        scope_t* parent = current_scope->parent;
        free_scope(current_scope);
        current_scope = parent;
        current_scope_level--;
    }
}

int get_current_scope(void) {
    return current_scope_level;
}

bool is_global_scope(void) {
    return current_scope_level == 0;
}

void add_symbol_to_scope(symbol_t* symbol) {
    if (!symbol || !current_scope) {
        report_error(ERR_INVALID, SEV_ERROR, "Invalid symbol or scope", NULL);
        return;
    }

    // Check for duplicate in current scope
    for (int i = 0; i < current_scope->num_symbols; i++) {
        if (strcmp(current_scope->symbols[i]->name, symbol->name) == 0) {
            report_error(ERR_REDECLARATION, SEV_ERROR, "Symbol already declared in current scope", symbol->name);
            return;
        }
    }

    // Add symbol to current scope
    current_scope->num_symbols++;
    current_scope->symbols = realloc(current_scope->symbols, 
                                   current_scope->num_symbols * sizeof(symbol_t*));
    if (!current_scope->symbols) {
        report_error(ERR_INVALID, SEV_ERROR, "Memory allocation failed", NULL);
        current_scope->num_symbols--;
        return;
    }
    current_scope->symbols[current_scope->num_symbols - 1] = symbol;
}

// Create a new scope
static scope_t* create_scope(scope_type_t type, scope_t* parent) {
    scope_t* scope = (scope_t*)malloc(sizeof(scope_t));
    if (!scope) {
        report_error(ERR_INVALID, SEV_FATAL, "Memory allocation failed for scope", NULL);
        return NULL;
    }
    
    scope->type = type;
    scope->parent = parent;
    scope->symbols = NULL;
    scope->num_symbols = 0;
    scope->level = parent ? parent->level + 1 : 0;
    
    return scope;
}

// Free scope and its resources
void free_scope(scope_t* scope) {
    if (!scope) {
        return;
    }
    
    // Free symbols array
    free(scope->symbols);
    free(scope);
}

// Get scope by level
scope_t* get_scope_by_level(int level) {
    scope_t* scope = current_scope;
    while (scope && scope->level != level) {
        scope = scope->parent;
    }
    return scope;
}

// Print scope symbols
void print_scope_symbols(int scope_level) {
    scope_t* scope = get_scope_by_level(scope_level);
    if (scope) {
        printf("Scope level %d symbols:\n", scope_level);
        for (int i = 0; i < scope->num_symbols; i++) {
            printf("%s\n", scope->symbols[i]->name);
        }
    }
} 