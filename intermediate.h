#ifndef INTERMEDIATE_H
#define INTERMEDIATE_H

#include <stdbool.h>
#include "types.h"

// Τύποι εντολών ενδιάμεσου κώδικα
typedef enum {
    IC_BINARY,   // Γενική δυαδική πράξη
    IC_ADD,      // Πρόσθεση
    IC_SUB,      // Αφαίρεση
    IC_MUL,      // Πολλαπλασιασμός
    IC_DIV,      // Διαίρεση
    IC_ASSIGN,   // Ανάθεση
    IC_GOTO,     // Άλμα χωρίς συνθήκη
    IC_IF_GOTO,  // Υπό συνθήκη άλμα
    IC_LABEL,    // Ετικέτα
    IC_CALL,     // Κλήση συνάρτησης
    IC_DO_START, // Αρχή DO loop
    IC_DO_END    // Τέλος DO loop
} ic_type_t;

// Προσωρινή μεταβλητή
typedef struct temp_var {
    char* name;
    symbol_type_t type;
    bool is_constant;
    union {
        int ival;
        float fval;
    } value;
} temp_var;

// Κόμβος ενδιάμεσου κώδικα
typedef struct ic_node {
    ic_type_t type;      // Τύπος εντολής
    ic_type_t op;        // Τύπος πράξης (για IC_BINARY)
    char* result;        // Αποτέλεσμα
    char* arg1;          // Πρώτο όρισμα
    char* arg2;          // Δεύτερο όρισμα
    char* arg3;          // Τρίτο όρισμα (για DO loops)
    int label;          // Για labels και gotos
    struct ic_node* next; // Επόμενος κόμβος
} ic_node;

// Καθολικές μεταβλητές
extern ic_node* ic_head;

// Συναρτήσεις χειρισμού ενδιάμεσου κώδικα
void append_code(ic_node* code);
void backpatch(ic_node* list, const char* label);
ic_node* make_list(int instr);
int next_instr(void);

#endif 