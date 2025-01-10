#ifndef TYPES_H
#define TYPES_H

typedef enum {
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_COMPLEX,
    TYPE_LOGICAL,
    TYPE_CHARACTER
} symbol_type_t;

typedef struct symbol_entry {
    char* name;
    symbol_type_t type;
    struct temp_var* temp;
    struct symbol_entry* next;
} symbol_entry;

// Δηλώσεις συναρτήσεων
symbol_entry* lookup_symbol(const char* name);
void add_common_block(const char* name, char** vars, int num_vars);
void process_data_item(const char* name, int* values, int num_values);

#endif