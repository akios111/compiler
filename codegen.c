#ifndef CODEGEN_C
#define CODEGEN_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

static int label_counter = 0;
static int temp_counter = 0;

// Δημιουργία νέας ετικέτας
char* new_label(void) {
    char buf[32];
    sprintf(buf, "L%d", label_counter++);
    return strdup(buf);
}

// Δημιουργία νέας προσωρινής μεταβλητής
temp_var* new_temp(symbol_type_t type) {
    temp_var* t = malloc(sizeof(temp_var));
    char buf[32];
    sprintf(buf, "t%d", temp_counter++);
    t->name = strdup(buf);
    t->type = type;
    t->is_constant = false;
    return t;
}

// Προσθήκη κώδικα στη λίστα
void append_code(ic_node* code) {
    if (!code) return;
    
    // Αν η λίστα είναι κενή
    if (!ic_head) {
        ic_head = code;
        return;
    }
    
    // Προσθήκη στο τέλος της λίστας
    ic_node* current = ic_head;
    while (current->next) {
        current = current->next;
    }
    current->next = code;
}

// Ενημέρωση ετικετών
void backpatch(ic_node* list, const char* label) {
    while (list) {
        if (list->type == IC_GOTO || list->type == IC_IF_GOTO) {
            list->label = atoi(label + 1); // Αφαιρούμε το 'L' από την ετικέτα
        }
        list = list->next;
    }
}

// Δημιουργία λίστας από μια εντολή
ic_node* make_list(int instr) {
    ic_node* node = malloc(sizeof(ic_node));
    node->type = IC_GOTO;
    node->label = instr;
    node->next = NULL;
    return node;
}

// Επόμενος αριθμός εντολής
int next_instr(void) {
    static int counter = 0;
    return counter++;
}

// Παραγωγή εντολών
ic_node* gen_binary(ic_type_t op, temp_var* result, temp_var* op1, temp_var* op2) {
    ic_node* node = malloc(sizeof(ic_node));
    node->type = op;
    node->result = strdup(result->name);
    node->arg1 = strdup(op1->name);
    node->arg2 = strdup(op2->name);
    node->next = NULL;
    return node;
}

ic_node* gen_assign(temp_var* result, temp_var* value) {
    ic_node* node = malloc(sizeof(ic_node));
    node->type = IC_ASSIGN;
    node->result = strdup(result->name);
    if (value) {
        node->arg1 = strdup(value->name);
    } else {
        node->arg1 = NULL;
    }
    node->arg2 = NULL;
    node->next = NULL;
    return node;
}

ic_node* gen_goto(const char* label) {
    ic_node* node = malloc(sizeof(ic_node));
    node->type = IC_GOTO;
    node->label = atoi(label + 1); // Αφαιρούμε το 'L' από την ετικέτα
    node->next = NULL;
    return node;
}

ic_node* gen_if_goto(temp_var* cond, temp_var* target) {
    ic_node* node = malloc(sizeof(ic_node));
    node->type = IC_IF_GOTO;
    node->result = strdup(cond->name);
    node->label = atoi(target->name + 1); // Αφαιρούμε το 'L' από την ετικέτα
    node->next = NULL;
    return node;
}

ic_node* gen_label(const char* label) {
    ic_node* node = malloc(sizeof(ic_node));
    node->type = IC_LABEL;
    node->label = atoi(label + 1); // Αφαιρούμε το 'L' από την ετικέτα
    node->next = NULL;
    return node;
}

ic_node* gen_call(const char* func, ic_node** args, int num_args) {
    ic_node* node = malloc(sizeof(ic_node));
    node->type = IC_CALL;
    node->result = strdup(func);
    
    // Αποθήκευση ορισμάτων
    if (num_args > 0 && args) {
        node->arg1 = malloc(num_args * sizeof(char*));
        for (int i = 0; i < num_args; i++) {
            ((char**)node->arg1)[i] = strdup(args[i]->result);
        }
        node->label = num_args; // Χρησιμοποιούμε το label για να αποθηκεύσουμε τον αριθμό των ορισμάτων
    } else {
        node->arg1 = NULL;
        node->label = 0;
    }
    
    node->next = NULL;
    return node;
}

ic_node* gen_do_start(const char* var, temp_var* start, temp_var* end) {
    ic_node* node = malloc(sizeof(ic_node));
    node->type = IC_DO_START;
    node->result = strdup(var);
    node->arg1 = strdup(start->name);
    node->arg2 = strdup(end->name);
    node->next = NULL;
    return node;
}

ic_node* gen_do_end(ic_node* start) {
    ic_node* node = malloc(sizeof(ic_node));
    node->type = IC_DO_END;
    node->result = NULL;
    node->arg1 = NULL;
    node->arg2 = NULL;
    node->next = NULL;
    return node;
}

#endif