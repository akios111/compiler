#ifndef CODEGEN_C
#define CODEGEN_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "intermediate.h"
#include "codegen.h"

/* Global variables */
ic_node* ic_head = NULL;
static int label_counter = 0;

/* Forward declarations */
static const char* get_op_string(ic_type_t op);

/* Code generation functions */
void append_code(ic_node* code) {
    if (!code) return;
    
    if (!ic_head) {
        ic_head = code;
    } else {
        ic_node* current = ic_head;
        while (current->next) {
            current = current->next;
        }
        current->next = code;
    }
}

void generate_code(FILE* out) {
    ic_node* current = ic_head;
    while (current) {
        switch (current->type) {
            case IC_BINARY:
                fprintf(out, "\t%s = %s %s %s\n", 
                        current->result, current->arg1, 
                        get_op_string(current->type), current->arg2);
                break;
                
            case IC_ASSIGN:
                fprintf(out, "\t%s = %s\n", current->result, current->arg1);
                break;
                
            case IC_GOTO:
                fprintf(out, "\tgoto L%d\n", current->label);
                break;
                
            case IC_IF_GOTO:
                fprintf(out, "\tif %s goto L%d\n", current->result, current->label);
                break;
                
            case IC_LABEL:
                fprintf(out, "L%d:\n", current->label);
                break;
                
            case IC_CALL:
                fprintf(out, "\tcall %s\n", current->result);
                break;
                
            case IC_DO_START:
                fprintf(out, "\t%s = %s\n", current->result, current->arg1);
                fprintf(out, "L%d:\n", current->label);
                fprintf(out, "\tif %s > %s goto L%d\n", 
                        current->result, current->arg2, current->label + 1);
                break;
                
            case IC_DO_END:
                fprintf(out, "\t%s = %s + 1\n", current->result, current->result);
                fprintf(out, "\tgoto L%d\n", current->label);
                fprintf(out, "L%d:\n", current->label + 1);
                break;
                
            default:
                fprintf(stderr, "Warning: Unknown instruction type %d\n", current->type);
                break;
        }
        current = current->next;
    }
}

static const char* get_op_string(ic_type_t op) {
    switch (op) {
        case IC_BINARY: return "+";  // Default to addition
        default: return "?";
    }
}

temp_var* new_temp(symbol_type_t type) {
    static int temp_counter = 0;
    temp_var* t = (temp_var*)malloc(sizeof(temp_var));
    
    if (!t) {
        fprintf(stderr, "Error: Memory allocation failed for temporary variable\n");
        exit(1);
    }
    
    char name[32];
    snprintf(name, sizeof(name), "t%d", temp_counter++);
    t->name = strdup(name);
    t->type = type;
    t->next = NULL;
    
    return t;
}

ic_node* gen_binary(ic_type_t op, temp_var* result, temp_var* op1, temp_var* op2) {
    ic_node* node = (ic_node*)malloc(sizeof(ic_node));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for IC node\n");
        exit(1);
    }
    
    node->type = IC_BINARY;
    node->result = result ? strdup(result->name) : NULL;
    node->arg1 = op1 ? strdup(op1->name) : NULL;
    node->arg2 = op2 ? strdup(op2->name) : NULL;
    node->label = 0;
    node->next = NULL;
    
    append_code(node);
    return node;
}

ic_node* gen_assign(temp_var* result, temp_var* value) {
    ic_node* node = (ic_node*)malloc(sizeof(ic_node));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for IC node\n");
        exit(1);
    }
    
    node->type = IC_ASSIGN;
    node->result = result ? strdup(result->name) : NULL;
    node->arg1 = value ? strdup(value->name) : NULL;
    node->arg2 = NULL;
    node->label = 0;
    node->next = NULL;
    
    append_code(node);
    return node;
}

ic_node* gen_goto(const char* label) {
    ic_node* node = (ic_node*)malloc(sizeof(ic_node));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for IC node\n");
        exit(1);
    }
    
    node->type = IC_GOTO;
    node->result = NULL;
    node->arg1 = NULL;
    node->arg2 = NULL;
    node->label = atoi(label);
    node->next = NULL;
    
    append_code(node);
    return node;
}

ic_node* gen_if_goto(temp_var* cond, temp_var* target) {
    ic_node* node = (ic_node*)malloc(sizeof(ic_node));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for IC node\n");
        exit(1);
    }
    
    node->type = IC_IF_GOTO;
    node->result = cond ? strdup(cond->name) : NULL;
    node->arg1 = NULL;
    node->arg2 = NULL;
    node->label = target ? atoi(target->name) : 0;
    node->next = NULL;
    
    append_code(node);
    return node;
}

ic_node* gen_label(const char* label) {
    ic_node* node = (ic_node*)malloc(sizeof(ic_node));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for IC node\n");
        exit(1);
    }
    
    node->type = IC_LABEL;
    node->result = NULL;
    node->arg1 = NULL;
    node->arg2 = NULL;
    node->label = atoi(label);
    node->next = NULL;
    
    append_code(node);
    return node;
}

ic_node* gen_call(const char* func, ic_node** args, int num_args) {
    ic_node* node = (ic_node*)malloc(sizeof(ic_node));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for IC node\n");
        exit(1);
    }
    
    node->type = IC_CALL;
    node->result = strdup(func);
    node->arg1 = NULL;
    node->arg2 = NULL;
    node->label = num_args;
    node->next = NULL;
    
    append_code(node);
    return node;
}

ic_node* gen_do_start(const char* var, temp_var* start, temp_var* end) {
    ic_node* node = (ic_node*)malloc(sizeof(ic_node));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for IC node\n");
        exit(1);
    }
    
    node->type = IC_DO_START;
    node->result = strdup(var);
    node->arg1 = start ? strdup(start->name) : NULL;
    node->arg2 = end ? strdup(end->name) : NULL;
    node->label = label_counter++;
    node->next = NULL;
    
    append_code(node);
    return node;
}

ic_node* gen_do_end(ic_node* start) {
    ic_node* node = (ic_node*)malloc(sizeof(ic_node));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for IC node\n");
        exit(1);
    }
    
    node->type = IC_DO_END;
    node->result = strdup(start->result);
    node->arg1 = NULL;
    node->arg2 = NULL;
    node->label = start->label;
    node->next = NULL;
    
    append_code(node);
    return node;
}

#endif