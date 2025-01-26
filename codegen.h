#ifndef CODEGEN_H
#define CODEGEN_H

#include "intermediate.h"

// Συναρτήσεις παραγωγής κώδικα
temp_var* new_temp(type_t type);
ic_node* gen_binary(bin_op_t op, temp_var* result, temp_var* op1, temp_var* op2);
ic_node* gen_assign(temp_var* result, temp_var* value);
ic_node* gen_goto(const char* label);
ic_node* gen_if_goto(temp_var* cond, temp_var* target);
ic_node* gen_label(const char* label);
ic_node* gen_call(const char* func, ic_node** args, int num_args);
ic_node* gen_do_start(const char* var, temp_var* start, temp_var* end);
ic_node* gen_do_end(ic_node* start);

#endif 