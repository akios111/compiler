#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "codegen.h"

#define HASH_SIZE 211
#define MAX_COMMON_VARS 100

// Δομή για COMMON block
typedef struct common_block {
    char* name;
    symbol_entry** vars;
    int num_vars;
    struct common_block* next;
} common_block;

// Δομή για DATA item
typedef struct data_item {
    symbol_entry* symbol;
    int* values;
    int num_values;
    struct data_item* next;
} data_item;

static symbol_entry* hash_table[HASH_SIZE];
static common_block* common_blocks = NULL;
static data_item* data_items = NULL;

// Απλή hash function
static unsigned int hash(const char* str) {
    unsigned int hash = 0;
    while (*str) hash = hash * 31 + *str++;
    return hash % HASH_SIZE;
}

// Αρχικοποίηση πίνακα συμβόλων
void init_symbol_table(void) {
    memset(hash_table, 0, sizeof(hash_table));
    common_blocks = NULL;
    data_items = NULL;
}

// Εισαγωγή συμβόλου
void add_symbol(const char* name, symbol_type_t type) {
    unsigned int h = hash(name);
    symbol_entry* entry = malloc(sizeof(symbol_entry));
    entry->name = strdup(name);
    entry->type = type;
    entry->temp = new_temp(type);  // Δημιουργία προσωρινής μεταβλητής
    entry->next = hash_table[h];
    hash_table[h] = entry;
    printf("Added symbol: %s, type: %d\n", name, type);  // Debugging εκτύπωση
}

// Αναζήτηση συμβόλου
symbol_entry* lookup_symbol(const char* name) {
    unsigned int h = hash(name);
    symbol_entry* entry = hash_table[h];
    while (entry) {
        if (strcmp(entry->name, name) == 0) {
            printf("Found symbol: %s\n", name);  // Debugging εκτύπωση
            return entry;
        }
        entry = entry->next;
    }
    printf("Symbol not found: %s\n", name);  // Debugging εκτύπωση
    return NULL;
}

// Προσθήκη common block
void add_common_block(const char* name, char** vars, int num_vars) {
    // Έλεγχος για διπλό ορισμό
    common_block* existing = common_blocks;
    while (existing) {
        if (strcmp(existing->name, name) == 0) {
            fprintf(stderr, "Error: Common block %s already defined\n", name);
            return;
        }
        existing = existing->next;
    }
    
    // Δημιουργία νέου common block
    common_block* block = malloc(sizeof(common_block));
    block->name = strdup(name);
    block->vars = malloc(num_vars * sizeof(symbol_entry*));
    block->num_vars = num_vars;
    
    // Προσθήκη μεταβλητών
    for (int i = 0; i < num_vars; i++) {
        symbol_entry* sym = lookup_symbol(vars[i]);
        if (!sym) {
            // Αν η μεταβλητή δεν υπάρχει, τη δημιουργούμε
            add_symbol(vars[i], TYPE_INTEGER); // Default type
            sym = lookup_symbol(vars[i]);
        }
        block->vars[i] = sym;
    }
    
    // Προσθήκη στη λίστα common blocks
    block->next = common_blocks;
    common_blocks = block;
    
    printf("Added common block: %s with %d variables\n", name, num_vars);
}

// Επεξεργασία data item
void process_data_item(const char* name, int* values, int num_values) {
    // Εύρεση του συμβόλου
    symbol_entry* sym = lookup_symbol(name);
    if (!sym) {
        fprintf(stderr, "Error: Symbol %s not found for DATA statement\n", name);
        return;
    }
    
    // Δημιουργία νέου data item
    data_item* item = malloc(sizeof(data_item));
    item->symbol = sym;
    item->values = malloc(num_values * sizeof(int));
    memcpy(item->values, values, num_values * sizeof(int));
    item->num_values = num_values;
    
    // Προσθήκη στη λίστα data items
    item->next = data_items;
    data_items = item;
    
    printf("Added DATA values for %s: ", name);
    for (int i = 0; i < num_values; i++) {
        printf("%d ", values[i]);
    }
    printf("\n");
}

// Απελευθέρωση μνήμης
void free_symbol_table(void) {
    // Απελευθέρωση συμβόλων
    for (int i = 0; i < HASH_SIZE; i++) {
        symbol_entry* entry = hash_table[i];
        while (entry) {
            symbol_entry* next = entry->next;
            free(entry->name);
            free(entry->temp);
            free(entry);
            entry = next;
        }
    }
    
    // Απελευθέρωση common blocks
    common_block* block = common_blocks;
    while (block) {
        common_block* next = block->next;
        free(block->name);
        free(block->vars);
        free(block);
        block = next;
    }
    
    // Απελευθέρωση data items
    data_item* item = data_items;
    while (item) {
        data_item* next = item->next;
        free(item->values);
        free(item);
        item = next;
    }
} 