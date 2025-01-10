#ifndef SYMTAB_H
#define SYMTAB_H

#include "types.h"
#include "intermediate.h"

// Συναρτήσεις πίνακα συμβόλων
void init_symbol_table(void);
symbol_entry* lookup_symbol(const char* name);
void add_symbol(const char* name, symbol_type_t type);
void free_symbol_table(void);

#endif 