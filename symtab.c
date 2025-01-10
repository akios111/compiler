#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "types.h"

extern int error_count;
extern int line_number;
extern char line_buf[];

#define HASH_SIZE 211

static symbol* hash_table[HASH_SIZE];
static char* current_loop_var = NULL;

// Απλή hash function
static unsigned int hash(const char* str) {
    unsigned int hash = 0;
    while (*str) hash = hash * 31 + *str++;
    return hash % HASH_SIZE;
}

// Αρχικοποίηση πίνακα συμβόλων
void init_symbol_table(void) {
    memset(hash_table, 0, sizeof(hash_table));
    current_loop_var = NULL;
}

// Εισαγωγή συμβόλου
void add_symbol(const char* name, symbol_type_t type) {
    symbol *sym = lookup_symbol(name);
    if (sym) {
        fprintf(stderr, "Error: Redeclaration of variable '%s'\n", name);
        fprintf(stderr, "Previous declaration was as %s\n", type_to_string(sym->type));
        error_count++;
        return;
    }

    symbol *new_symbol = (symbol *)malloc(sizeof(symbol));
    new_symbol->name = strdup(name);
    new_symbol->type = type;
    new_symbol->is_array = 0;
    new_symbol->array_size = 0;
    new_symbol->next = hash_table[hash(name)];
    hash_table[hash(name)] = new_symbol;
    printf("Added symbol: %s, type: %s\n", name, type_to_string(type));
}

// Αναζήτηση συμβόλου
symbol* lookup_symbol(const char* name) {
    unsigned int h = hash(name);
    symbol* entry = hash_table[h];
    while (entry) {
        if (strcmp(entry->name, name) == 0) {
            printf("Found symbol: %s\n", name);
            return entry;
        }
        entry = entry->next;
    }
    printf("Symbol not found: %s\n", name);
    return NULL;
}

// Απελευθέρωση μνήμης
void free_symbol_table(void) {
    for (int i = 0; i < HASH_SIZE; i++) {
        symbol* entry = hash_table[i];
        while (entry) {
            symbol* next = entry->next;
            free(entry->name);
            free(entry);
            entry = next;
        }
        hash_table[i] = NULL;
    }
    if (current_loop_var) {
        free(current_loop_var);
        current_loop_var = NULL;
    }
}

// Έλεγχος αν ένα σύμβολο έχει δηλωθεί
int check_symbol_declared(const char* name) {
    symbol* entry = lookup_symbol(name);
    if (!entry) {
        fprintf(stderr, "Error: Undeclared variable '%s'\n", name);
        return 0;
    }
    return 1;
}

// Νλεγχος εγκυρότητας διάστασης πίνακα
int is_valid_array_dimension(void) {
    extern char* yytext;
    symbol* entry = lookup_symbol(yytext);
    
    // Αν είναι αριθμητική σταθερά
    if (strspn(yytext, "0123456789") == strlen(yytext)) {
        return 1;
    }
    
    // Αν είναι μεταβλητή, πρέπει να είναι INTEGER
    if (entry) {
        return entry->type == TYPE_INTEGER;
    }
    
    return 0;
}

// Έλεγχος συμβατότητας τύπων για DATA
int check_data_type_compatibility(const char* var_name) {
    extern char* yytext;
    symbol* var = lookup_symbol(var_name);
    if (!var) return 0;
    
    // Έλεγχος συμβατότητας τύπων
    switch (var->type) {
        case TYPE_INTEGER:
            // Για INTEGER δεχόμαστε μόνο ακέραιους
            return strspn(yytext, "0123456789") == strlen(yytext);
            
        case TYPE_REAL:
            // Για REAL δεχόμαστε και ακέραιους και πραγματικούς
            return 1;
            
        case TYPE_COMPLEX:
            // Για COMPLEX ελέγχουμε τη μορφή (a:b)
            return (yytext[0] == '(' && strchr(yytext, ':') != NULL);
            
        case TYPE_LOGICAL:
            // Για LOGICAL μόνο .true. ή .false.
            return (strcmp(yytext, ".true.") == 0 || strcmp(yytext, ".false.") == 0);
            
        case TYPE_STRING:
            // Για STRING ελέγχουμε αν είναι string literal
            return (yytext[0] == '"' || yytext[0] == '\'');
            
        default:
            return 0;
    }
}

// Ορισμός τρέχουσας μεταβλητής βρόχου
void set_current_loop_var(const char* var_name) {
    if (current_loop_var) free(current_loop_var);
    current_loop_var = strdup(var_name);
}

// Έλεγχος τύπου μεταβλητής βρόχου
int check_loop_variable_type(void) {
    if (!current_loop_var) return 0;
    
    symbol* var = lookup_symbol(current_loop_var);
    if (!var) return 0;
    
    // Η μεταβλητή ελέγχου πρέπει να είναι INTEGER και όχι πίνακας
    return (var->type == TYPE_INTEGER && !var->is_array);
}

// Έλεγχος τύπου έκφρασης
int check_expression_type(void) {
    extern char* yytext;
    symbol* entry = lookup_symbol(yytext);
    
    // Αν είναι μεταβλητή, πρέπει να έχει δηλωθεί
    if (entry) return 1;
    
    // Αν είναι σταθερά, επιτρέπεται
    if (strspn(yytext, "0123456789.") > 0) return 1;
    
    // Αν είναι string literal
    if (yytext[0] == '"' || yytext[0] == '\'') return 1;
    
    return 0;
}

// Έλεγχος εγκυρότητας κλήσης συνάρτησης
int is_valid_function_call(const char* func_name) {
    // Λίστα με τις ενσωματωμένες συναρτήσεις της FORT
    static const char* builtin_funcs[] = {
        "abs", "sqrt", "sin", "cos", "exp", "log",
        NULL
    };
    
    // Έλεγχος για ενσωματωμένες συναρτήσεις
    for (int i = 0; builtin_funcs[i]; i++) {
        if (strcmp(func_name, builtin_funcs[i]) == 0) {
            return 1;
        }
    }
    
    // Αν δεν είναι ενσωματωμένη, ελέγχουμε αν έχει δηλωθεί
    symbol* entry = lookup_symbol(func_name);
    return (entry != NULL && entry->type == TYPE_FUNCTION);
}

// Μετατροπή τύπου σε string
const char* type_to_string(symbol_type_t type) {
    switch (type) {
        case TYPE_INTEGER: return "INTEGER";
        case TYPE_REAL: return "REAL";
        case TYPE_COMPLEX: return "COMPLEX";
        case TYPE_LOGICAL: return "LOGICAL";
        case TYPE_STRING: return "STRING";
        default: return "UNKNOWN";
    }
}

int check_array_bounds(const char *name, int index) {
    symbol *sym = lookup_symbol(name);
    if (!sym) {
        fprintf(stderr, "Error: Array '%s' is not declared\n", name);
        return 0;
    }
    if (!sym->is_array) {
        fprintf(stderr, "Error: '%s' is not an array\n", name);
        return 0;
    }
    if (index < 1 || index > sym->array_size) {
        fprintf(stderr, "Error: Array index %d out of bounds for array '%s' [1..%d] at line %d\n", 
                index, name, sym->array_size, line_number);
        fprintf(stderr, "Found: %s\n", line_buf);
        return 0;
    }
    return 1;
}

int check_array_index_type(const char *array_name, const char *index_name) {
    symbol *index_sym = lookup_symbol(index_name);
    if (!index_sym) {
        fprintf(stderr, "Error: Array index variable '%s' is not declared\n", index_name);
        return 0;
    }
    if (index_sym->type != TYPE_INTEGER) {
        fprintf(stderr, "Error: Array index '%s' must be of type INTEGER\n", index_name);
        return 0;
    }
    return 1;
} 