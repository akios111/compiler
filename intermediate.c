#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intermediate.h"
#include "error.h"

// Καθολικές μεταβλητές
static int temp_counter = 0;
static int label_counter = 0;

// Στατιστικά βελτιστοποίησης
typedef struct {
    int loops_unrolled;
    int vars_eliminated;
    int instructions_moved;
} optimization_stats_t;

static optimization_stats_t stats = {0};

// Δημιουργία προσωρινής μεταβλητής
static char* new_temp() {
    char* temp = malloc(20);
    sprintf(temp, "t%d", temp_counter++);
    return temp;
}

// Δημιουργία ετικέτας
static char* new_label() {
    char* label = malloc(20);
    sprintf(label, "L%d", label_counter++);
    return label;
}

// Δημιουργία δυαδικού κόμβου
ic_node_t* create_binary_node(ic_type_t op, ic_node_t* left, ic_node_t* right) {
    ic_node_t* node = malloc(sizeof(ic_node_t));
    node->op = op;
    node->u.binary.left = left;
    node->u.binary.right = right;
    node->next = NULL;
    return node;
}

// Δημιουργία μοναδιαίου κόμβου
ic_node_t* create_unary_node(ic_type_t op, ic_node_t* operand) {
    ic_node_t* node = malloc(sizeof(ic_node_t));
    node->op = op;
    node->u.unary.operand = operand;
    node->next = NULL;
    return node;
}

// Δημιουργία κόμβου πίνακα
ic_node_t* create_array_node(const char* name, int index) {
    ic_node_t* node = malloc(sizeof(ic_node_t));
    node->op = IC_ARRAY;
    node->u.array.name = strdup(name);
    node->u.array.index = index;
    node->next = NULL;
    return node;
}

// Δημιουργία κόμβου κλήσης
ic_node_t* create_call_node(const char* name, ic_node_t** args, int arg_count) {
    ic_node_t* node = malloc(sizeof(ic_node_t));
    node->op = IC_CALL;
    node->u.call.name = strdup(name);
    node->u.call.args = args;
    node->u.call.arg_count = arg_count;
    node->next = NULL;
    return node;
}

// Απελευθέρωση κόμβου
void free_ic_node(ic_node_t* node) {
    if (!node) return;
    
    switch(node->op) {
        case IC_BINARY:
            free_ic_node(node->u.binary.left);
            free_ic_node(node->u.binary.right);
            break;
        case IC_UNARY:
            free_ic_node(node->u.unary.operand);
            break;
        case IC_ARRAY:
            free(node->u.array.name);
            break;
        case IC_CALL:
            free(node->u.call.name);
            for(int i = 0; i < node->u.call.arg_count; i++) {
                free_ic_node(node->u.call.args[i]);
            }
            free(node->u.call.args);
            break;
        default:
            break;
    }
    
    free(node);
} 