#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symtab.h"
#include "error.h"
#include "types.h"
#include "globals.h"

// Global variables
symbol_t symbol_table[MAX_SYMBOLS];
int symbol_count = 0;

common_block_t common_blocks[MAX_COMMON_BLOCKS];
int common_block_count = 0;

static char* current_loop_var = NULL;

// Convert string to uppercase
static char* to_upper(const char* s) {
    char* p = strdup(s);
    for(char* c = p; *c; c++) *c = toupper(*c);
    return p;
}

// Initialize symbol table
void init_symtab() {
    symbol_count = 0;
    common_block_count = 0;
    current_loop_var = NULL;
}

// Add symbol to table
int add_symbol(const char* name, int type) {
    if (symbol_count >= MAX_SYMBOLS) {
        report_error(ERR_TYPE, SEV_ERROR, "Symbol table full", NULL);
        return 0;
    }
    
    char* upper_name = to_upper(name);
    symbol_t* sym = &symbol_table[symbol_count];
    sym->name = upper_name;
    sym->type = type;
    sym->is_array = 0;
    sym->dimensions = 0;
    sym->dim_sizes = NULL;
    sym->common_block = NULL;
    
    if (type == TYPE_PROGRAM) {
        set_program_declaration();
    }
    
    symbol_count++;
    return 1;
}

// Add array symbol to table
int add_array_symbol(const char* name, int type) {
    if (symbol_count >= MAX_SYMBOLS) {
        report_error(ERR_TYPE, SEV_ERROR, "Symbol table full", NULL);
        return 0;
    }
    
    char* upper_name = to_upper(name);
    symbol_t* sym = &symbol_table[symbol_count];
    sym->name = upper_name;
    sym->type = type;
    sym->is_array = 1;
    sym->dimensions = 0;
    sym->dim_sizes = NULL;
    sym->common_block = NULL;
    
    symbol_count++;
    return 1;
}

// Lookup symbol in table
symbol_t* lookup_symbol(const char* name) {
    char* upper_name = to_upper(name);
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, upper_name) == 0) {
            free(upper_name);
            return &symbol_table[i];
        }
    }
    free(upper_name);
    return NULL;
}

// Get symbol type
int get_symbol_type(const char* name) {
    symbol_t* sym = lookup_symbol(name);
    if (!sym) return TYPE_UNKNOWN;
    return sym->type;
}

// Check if symbol is array
int is_array(const char* name) {
    symbol_t* sym = lookup_symbol(name);
    if (sym) {
        return sym->is_array;
    }
    
    return 0;
}

// Check array dimensions
int check_array_dimensions(const char* name, int dimensions) {
    symbol_t* sym = lookup_symbol(name);
    if (!sym || !sym->is_array) return 0;
    sym->dimensions = dimensions;
    return 1;
}

// Check array dimension validity
int check_array_dimension(const char* dim_name) {
    if (!dim_name) return 0;
    symbol_t* sym = lookup_symbol(dim_name);
    if (!sym) return 1; // Allow numeric literals
    return sym->type == TYPE_INTEGER && !sym->is_array;
}

// Check assignment type compatibility
int check_assignment_types(const char* target, const char* source) {
    int target_type = get_symbol_type(target);
    int source_type = get_symbol_type(source);
    
    if (target_type == TYPE_UNKNOWN) {
        report_error(ERR_TYPE, SEV_ERROR, "Variable not declared", target);
        return 0;
    }
    
    if (source_type == TYPE_UNKNOWN) {
        report_error(ERR_TYPE, SEV_ERROR, "Variable not declared", source);
        return 0;
    }
    
    return target_type == source_type;
}

// Check data values type compatibility
int check_data_values_type(const char* values, int expected_type) {
    // TODO: Implement type checking for data values
    return 1;
}

// Check complex number components
int check_complex_components(const char* real_part, const char* imag_part) {
    if (!real_part || !imag_part) return 0;
    
    // Check if components are numeric literals
    char* endptr;
    strtod(real_part, &endptr);
    if (*endptr == '\0') return 1;
    
    strtod(imag_part, &endptr);
    if (*endptr == '\0') return 1;
    
    // Check if components are declared numeric variables
    symbol_t* real_sym = lookup_symbol(real_part);
    symbol_t* imag_sym = lookup_symbol(imag_part);
    
    if (real_sym && (real_sym->type == TYPE_INTEGER || real_sym->type == TYPE_REAL) &&
        imag_sym && (imag_sym->type == TYPE_INTEGER || imag_sym->type == TYPE_REAL)) {
        return 1;
    }
    
    return 0;
}

/* has_program_declaration moved to globals.c */

// Process variables in COMMON block
int process_common_block(const char* block_name, const char* variables) {
    if (!block_name || !variables) {
        return 0;  // Error
    }
    // Process the variables
    char* var_list = strdup(variables);
    char* var = strtok(var_list, ",");
    int success = 1;  // Track success/failure
    
    while (var != NULL) {
        // Remove leading/trailing whitespace
        while (isspace(*var)) var++;
        char* end = var + strlen(var) - 1;
        while (end > var && isspace(*end)) end--;
        *(end + 1) = '\0';
        
        // Check if variable exists
        symbol_t* sym = lookup_symbol(var);
        if (!sym) {
            char msg[256];
            snprintf(msg, sizeof(msg), "Variable '%s' not found", var);
            report_error(ERR_COMMON, SEV_ERROR, msg, block_name);
            success = 0;
        } else if (sym->common_block) {
            char msg[256];
            snprintf(msg, sizeof(msg), "Variable '%s' already in COMMON block", var);
            report_error(ERR_COMMON, SEV_ERROR, msg, sym->common_block);
            success = 0;
        } else {
            sym->common_block = strdup(block_name);
        }
        
        var = strtok(NULL, ",");
    }
    
    free(var_list);
    return success;
}

// Check function call
int check_function_call(const char* name, const char* args) {
    symbol_t* sym = lookup_symbol(name);
    if (!sym) return 0;
    if (sym->type != TYPE_FUNCTION) return 0;
    return 1;
}

// Set current loop variable
void set_current_loop_var(const char* var) {
    if (current_loop_var) free(current_loop_var);
    current_loop_var = strdup(var);
}

// Check loop variable type
int check_loop_variable_type(void) {
    if (!current_loop_var) return 0;
    symbol_t* var = lookup_symbol(current_loop_var);
    if (!var) return 0;
    return (var->type == TYPE_INTEGER && !var->is_array);
}

// Count expressions in list
int count_expressions(const char* expr_list) {
    if (!expr_list || !*expr_list) return 0;
    
    int count = 1;
    const char* p = expr_list;
    while ((p = strchr(p, ',')) != NULL) {
        count++;
        p++;
    }
    return count;
}

// Check if identifiers in list are valid
int check_valid_identifiers(const char* id_list) {
    char* list = strdup(id_list);
    char* token = strtok(list, ",");
    
    while (token != NULL) {
        // Remove leading/trailing whitespace
        while (isspace(*token)) token++;
        char* end = token + strlen(token) - 1;
        while (end > token && isspace(*end)) end--;
        *(end + 1) = '\0';
        
        // Check first character is letter
        if (!isalpha(token[0])) {
            free(list);
            return 0;
        }
        
        // Check remaining characters are alphanumeric or underscore
        for (int i = 1; token[i] != '\0'; i++) {
            if (!isalnum(token[i]) && token[i] != '_') {
                free(list);
                return 0;
            }
        }
        
        token = strtok(NULL, ",");
    }
    
    free(list);
    return 1;
}

// Check for duplicate common block
int is_duplicate_common_block(const char* block_name) {
    for (int i = 0; i < common_block_count; i++) {
        if (strcmp(common_blocks[i].name, block_name) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check data types match
int check_data_types(const char* data_decl) {
    // TODO: Implement type checking
    return 1;
}

// Count dimensions in array declaration
int count_dimensions(const char* dim_list) {
    int count = 0;
    for (int i = 0; dim_list[i] != '\0'; i++) {
        if (dim_list[i] == ',') count++;
    }
    return count + 1;
}

// Check array assignment
int check_array_assignment(const char* array_name, const char* expr) {
    symbol_t* sym = lookup_symbol(array_name);
    if (!sym || !sym->is_array) return 0;
    
    // TODO: Check dimensions match
    return 1;
}

// Check variables in expression exist
int check_expression_variables(const char* expr) {
    // TODO: Parse expression and check variables
    return 1;
}

// Check logical expression is valid
int check_logical_expression(const char* expr) {
    // TODO: Validate logical expression
    return 1;
}

// Check loop bounds are valid
int check_loop_bounds(const char* bounds) {
    // TODO: Validate loop bounds
    return 1;
}

// Add check_relation_types implementation
int check_relation_types(const char* expr) {
    // Basic implementation - can be enhanced based on requirements
    if (!expr) return 0;
    // For now, just return true as we'll implement proper type checking later
    return 1;
}

/* Function to check if a symbol is declared */
int is_declared(const char* name) {
    return get_symbol_type(name) != TYPE_UNKNOWN;
}
