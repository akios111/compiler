/* Υλοποίηση νέων λειτουργιών για έλεγχο τύπων */

#include <stdbool.h>
#include <stdlib.h>
#include "types.h"

/* Type string conversion functions */
const char* get_type_str(type_t type) {
    switch (type) {
        case TYPE_UNKNOWN:    return "UNKNOWN";
        case TYPE_INTEGER:    return "INTEGER";
        case TYPE_REAL:       return "REAL";
        case TYPE_COMPLEX:    return "COMPLEX";
        case TYPE_LOGICAL:    return "LOGICAL";
        case TYPE_CHARACTER:  return "CHARACTER";
        case TYPE_STRING:     return "STRING";
        case TYPE_ERROR:      return "ERROR";
        default:             return "INVALID";
    }
}

type_t promote_type(type_t t1, type_t t2) {
    if (t1 == TYPE_ERROR || t2 == TYPE_ERROR)
        return TYPE_ERROR;
    
    // If either type is REAL, result is REAL
    if (t1 == TYPE_REAL || t2 == TYPE_REAL)
        return TYPE_REAL;
    
    // If both types are INTEGER, result is INTEGER
    if (t1 == TYPE_INTEGER && t2 == TYPE_INTEGER)
        return TYPE_INTEGER;
    
    // If either type is COMPLEX, result is COMPLEX
    if (t1 == TYPE_COMPLEX || t2 == TYPE_COMPLEX)
        return TYPE_COMPLEX;
    
    return TYPE_ERROR;
}

int is_numeric_type(type_t type) {
    return type == TYPE_INTEGER || type == TYPE_REAL || type == TYPE_COMPLEX;
}

int is_compatible_types(type_t t1, type_t t2) {
    if (t1 == TYPE_ERROR || t2 == TYPE_ERROR)
        return 0;
    
    // Same types are always compatible
    if (t1 == t2)
        return 1;
    
    // Numeric types are compatible with each other
    if (is_numeric_type(t1) && is_numeric_type(t2))
        return 1;
    
    return 0;
}

const char* type_to_string(type_t type) {
    switch (type) {
        case TYPE_ERROR: return "ERROR";
        case TYPE_VOID: return "VOID";
        case TYPE_INTEGER: return "INTEGER";
        case TYPE_REAL: return "REAL";
        case TYPE_COMPLEX: return "COMPLEX";
        case TYPE_LOGICAL: return "LOGICAL";
        case TYPE_CHARACTER: return "CHARACTER";
        case TYPE_STRING: return "STRING";
        default: return "UNKNOWN";
    }
} 