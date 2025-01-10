/* Υλοποίηση νέων λειτουργιών για έλεγχο τύπων */

#include <stdbool.h>
#include <stdlib.h>
#include "types.h"

bool is_numeric_type(const type_t* type) {
    if (!type) return false;
    return type->kind == TYPE_INTEGER || type->kind == TYPE_REAL;
}

bool is_string_type(const type_t* type) {
    if (!type) return false;
    return type->kind == TYPE_STRING || type->kind == TYPE_CHARACTER;
}

bool is_composite_type(const type_t* type) {
    if (!type) return false;
    
    switch(type->kind) {
        case TYPE_LIST:
        case TYPE_RECORD:
        case TYPE_ARRAY:
            return true;
        default:
            return false;
    }
} 