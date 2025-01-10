#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"
#include "intermediate.h"

static symbol_table_t symbol_table = {NULL, 0};

// Δημιουργία προσωρινής μεταβλητής
static int temp_counter = 0;

temp_var* new_temp(type_t* type) {
    temp_var* temp = malloc(sizeof(temp_var));
    char name[32];
    sprintf(name, "t%d", temp_counter++);
    temp->name = strdup(name);
    temp->type = type;
    return temp;
}

void free_temp(temp_var* temp) {
    if (temp) {
        free(temp->name);
        free(temp);
    }
}

// Εισαγωγή συμβόλου στον πίνακα
symbol_entry* insert_symbol(const char* name, symbol_type_t type) {
    symbol_entry* entry = malloc(sizeof(symbol_entry));
    entry->name = strdup(name);
    entry->type = type;
    entry->temp = new_temp(type);
    entry->scope = symbol_table.current_scope;
    entry->next = symbol_table.entries;
    symbol_table.entries = entry;
    return entry;
}

// Αναζήτηση συμβόλου
symbol_entry* lookup_symbol(const char* name) {
    symbol_entry* current = symbol_table.entries;
    while (current) {
        if (strcmp(current->name, name) == 0 &&
            current->scope <= symbol_table.current_scope) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Διαχείριση εμβέλειας
void enter_scope(void) {
    symbol_table.current_scope++;
}

void exit_scope(void) {
    // Αφαίρεση συμβόλων του τρέχοντος scope
    symbol_entry** current = &symbol_table.entries;
    while (*current) {
        if ((*current)->scope == symbol_table.current_scope) {
            symbol_entry* to_remove = *current;
            *current = to_remove->next;
            free(to_remove->name);
            free_temp(to_remove->temp);
            free(to_remove);
        } else {
            current = &(*current)->next;
        }
    }
    symbol_table.current_scope--;
}

// Βοηθητικές συναρτήσεις για τη δημιουργία κώδικα
ic_node_t* gen_goto(const char* label) {
    ic_node_t* node = malloc(sizeof(ic_node_t));
    node->type = IC_GOTO;
    node->u.jump.label = strdup(label);
    node->next = NULL;
    return node;
}

ic_node_t* gen_label(const char* label) {
    ic_node_t* node = malloc(sizeof(ic_node_t));
    node->type = IC_LABEL;
    node->u.label.label = strdup(label);
    node->next = NULL;
    return node;
}

ic_node_t* gen_assign(temp_var* result, temp_var* value) {
    ic_node_t* node = malloc(sizeof(ic_node_t));
    node->type = IC_ASSIGN;
    node->u.assign.result = result;
    node->u.assign.op1 = value;
    node->u.assign.op2 = NULL;
    node->next = NULL;
    return node;
}

ic_node_t* gen_do_start(const char* var, temp_var* start, temp_var* end) {
    ic_node_t* node = malloc(sizeof(ic_node_t));
    node->type = IC_DO_START;
    node->u.loop.bounds.start = start->value.ival;
    node->u.loop.bounds.end = end->value.ival;
    node->u.loop.bounds.step = 1;
    node->next = NULL;
    return node;
}

ic_node_t* gen_do_end(ic_node_t* start) {
    ic_node_t* node = malloc(sizeof(ic_node_t));
    node->type = IC_DO_END;
    node->next = NULL;
    return node;
} 