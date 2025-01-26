#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symtab.h"
#include "error.h"
#include "types.h"
#include "globals.h"
#include "scope.h"

#define MAX_SYMBOLS 1000
#define SYMBOL_TABLE_SIZE 211

/* Global variables */
int symbol_count = 0;
symbol_t* global_symbols = NULL;  // For mips_layout.c
symbol_t* local_symbols = NULL;   // For mips_layout.c

typedef struct symbol_entry {
    symbol_t symbol;
    struct symbol_entry* next;
} symbol_entry_t;

static symbol_entry_t* hash_table[SYMBOL_TABLE_SIZE];

static unsigned int hash(const char* str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % SYMBOL_TABLE_SIZE;
}

/* Function implementations */
void init_symbol_table(void) {
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        hash_table[i] = NULL;
    }
    init_scope();
}

symbol_t* add_symbol(const char* name, type_t type) {
    unsigned int index = hash(name);
    symbol_entry_t* entry = malloc(sizeof(symbol_entry_t));
    
    if (!entry) {
        report_error(ERR_INVALID, SEV_ERROR, "Memory allocation failed", "symbol table entry");
        return NULL;
    }
    
    strncpy(entry->symbol.name, name, MAX_ID_LENGTH);
    entry->symbol.type = type;
    entry->symbol.kind = SYMBOL_VARIABLE;
    entry->symbol.num_dimensions = 0;
    entry->next = hash_table[index];
    hash_table[index] = entry;
    
    add_symbol_to_scope(&entry->symbol);
    
    return &entry->symbol;
}

symbol_t* add_array_symbol(const char* name, type_t type, int dimensions) {
    symbol_t* sym = add_symbol(name, type);
    if (sym) {
        sym->kind = SYMBOL_ARRAY;
        sym->num_dimensions = 1;  // Start with 1 dimension
        sym->dimensions[0] = dimensions;  // Set the size of first dimension
    }
    return sym;
}

symbol_t* add_function_symbol(const char* name, type_t return_type) {
    symbol_t* sym = add_symbol(name, return_type);
    if (sym) {
        sym->kind = SYMBOL_FUNCTION;
    }
    return sym;
}

symbol_t* lookup_symbol(const char* name) {
    if (!name) return NULL;
    
    int current = get_current_scope();
    while (current >= 0) {
        symbol_t* symbol = lookup_symbol_in_scope(name, current);
        if (symbol) return symbol;
        current--;
    }
    return NULL;
}

symbol_t* lookup_symbol_in_scope(const char* name, int scope) {
    unsigned int index = hash(name);
    symbol_entry_t* entry = hash_table[index];
    
    while (entry) {
        if (strcmp(entry->symbol.name, name) == 0) {
            return &entry->symbol;
        }
        entry = entry->next;
    }
    return NULL;
}

void remove_symbol(const char* name) {
    unsigned int index = hash(name);
    symbol_entry_t* entry = hash_table[index];
    symbol_entry_t* prev = NULL;
    
    while (entry) {
        if (strcmp(entry->symbol.name, name) == 0) {
            if (prev) {
                prev->next = entry->next;
            } else {
                hash_table[index] = entry->next;
            }
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

void print_symbol_table(void) {
    printf("\nSymbol Table Contents:\n");
    printf("=====================\n");
    
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        symbol_entry_t* entry = hash_table[i];
        while (entry) {
            printf("Name: %-20s Type: %-10s Kind: %-10s\n",
                   entry->symbol.name,
                   get_type_str(entry->symbol.type),
                   entry->symbol.kind == SYMBOL_VARIABLE ? "Variable" :
                   entry->symbol.kind == SYMBOL_ARRAY ? "Array" :
                   entry->symbol.kind == SYMBOL_FUNCTION ? "Function" : "Unknown");
            entry = entry->next;
        }
    }
    printf("\n");
}

int declare_variable(const char* name, type_t type) {
    if (lookup_symbol_in_scope(name, get_current_scope())) {
        report_error(ERR_REDECLARATION, SEV_ERROR, "Variable already declared", name);
        return 0;
    }
    
    symbol_t* sym = add_symbol(name, type);
    if (!sym) {
        report_error(ERR_INVALID, SEV_ERROR, "Failed to declare variable", name);
        return 0;
    }
    
    return 1;
}

int declare_array(const char* name, type_t type, int dimensions) {
    if (lookup_symbol_in_scope(name, get_current_scope())) {
        report_error(ERR_REDECLARATION, SEV_ERROR, "Array already declared", name);
        return 0;
    }
    
    if (dimensions <= 0) {
        report_error(ERR_INVALID, SEV_ERROR, "Invalid array dimensions", name);
        return 0;
    }
    
    symbol_t* sym = add_array_symbol(name, type, dimensions);
    if (!sym) {
        report_error(ERR_INVALID, SEV_ERROR, "Failed to declare array", name);
        return 0;
    }
    
    return 1;
}

int process_common_block(const char* block_name, const char** var_names, type_t* types, int num_vars) {
    if (!block_name || !var_names || !types || num_vars <= 0) {
        report_error(ERR_INVALID, SEV_ERROR, "Invalid common block parameters", block_name ? block_name : "unknown");
        return 0;
    }
    
    // For now, we'll just add the variables to the global scope
    for (int i = 0; i < num_vars; i++) {
        if (!declare_variable(var_names[i], types[i])) {
            report_error(ERR_INVALID, SEV_ERROR, "Failed to add variable to common block", var_names[i]);
            return 0;
        }
    }
    
    return 1;
}
