#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>
#include "types.h"
#include "intermediate.h"

// Τύποι συμβόλων
typedef enum {
    SYMBOL_VAR,
    SYMBOL_TEMP,
    SYMBOL_LABEL,
    SYMBOL_CONST
} symbol_type_t;

// Προσωρινή μεταβλητή
typedef struct temp_var {
    char* name;
    type_t* type;
    union {
        int ival;
        float fval;
        char* sval;
    } value;
} temp_var;

// Καταχώρηση συμβόλου
typedef struct symbol_entry {
    char* name;
    symbol_type_t type;
    temp_var* temp;
    int scope;
    struct symbol_entry* next;
} symbol_entry;

// Πίνακας συμβόλων
typedef struct {
    symbol_entry* entries;
    int current_scope;
} symbol_table_t;

// Συναρτήσεις διαχείρισης συμβόλων
symbol_entry* insert_symbol(const char* name, symbol_type_t type);
symbol_entry* lookup_symbol(const char* name);
void enter_scope(void);
void exit_scope(void);

// Βοηθητικές συναρτήσεις για ενδιάμεσο κώδικα
struct ic_node* gen_goto(const char* label);
struct ic_node* gen_label(const char* label);
struct ic_node* gen_assign(temp_var* result, temp_var* value);
struct ic_node* gen_do_start(const char* var, temp_var* start, temp_var* end);
struct ic_node* gen_do_end(struct ic_node* start);

// Διαχείριση προσωρινών μεταβλητών
temp_var* new_temp(type_t* type);
void free_temp(temp_var* temp);

#endif