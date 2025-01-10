#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "intermediate.h"
#include "error.h"

// Πληροφορίες βρόχου για βελτιστοποίηση
typedef struct {
    ic_node_t* start;
    ic_node_t* end;
    loop_bounds_t bounds;
    bool is_parallel;
    bool can_vectorize;
    int unroll_factor;
} loop_info_t;

// Εύρεση ορίων βρόχου
static loop_bounds_t analyze_loop_bounds(ic_node_t* start, ic_node_t* end) {
    loop_bounds_t bounds = {0};
    
    // Ανάλυση αρχικής τιμής
    if (start->op == IC_DO_START) {
        bounds.start = 1; // Default
        bounds.step = 1;  // Default
    }
    
    return bounds;
}

// Έλεγχος αν ο βρόχος μπορεί να εκτελεστεί παράλληλα
static bool check_parallelizable(ic_node_t* start, ic_node_t* end) {
    // Έλεγχος για εξαρτήσεις δεδομένων
    for (ic_node_t* node = start; node != end; node = node->next) {
        if (node->op == IC_ARRAY) {
            // Έλεγχος για εγγραφές στον ίδιο πίνακα
            return false;
        }
    }
    return true;
}

// Έλεγχος αν ο βρόχος μπορεί να διανυσματοποιηθεί
static bool check_vectorizable(ic_node_t* start, ic_node_t* end) {
    // Έλεγχος για πράξεις που υποστηρίζουν διανυσματοποίηση
    for (ic_node_t* node = start; node != end; node = node->next) {
        switch(node->op) {
            case IC_ADD:
            case IC_SUB:
            case IC_MUL:
            case IC_DIV:
                continue;
            default:
                return false;
        }
    }
    return true;
}

// Υπολογισμός παράγοντα ξετυλίγματος βρόχου
static int calculate_unroll_factor(loop_bounds_t bounds) {
    int range = bounds.end - bounds.start;
    
    if (range <= 4) return 1;
    if (range <= 8) return 2;
    if (range <= 16) return 4;
    
    return MAX_UNROLL_SIZE;
}

// Βελτιστοποίηση βρόχου
void optimize_loop(ic_node_t* start, ic_node_t* end) {
    loop_info_t info;
    
    // Ανάλυση ορίων βρόχου
    info.bounds = analyze_loop_bounds(start, end);
    
    // Έλεγχος για παράλληλη εκτέλεση
    info.is_parallel = check_parallelizable(start, end);
    
    // Έλεγχος για διανυσματοποίηση
    info.can_vectorize = check_vectorizable(start, end);
    
    // Υπολογισμός παράγοντα ξετυλίγματος
    info.unroll_factor = calculate_unroll_factor(info.bounds);
    
    // Εφαρμογή βελτιστοποιήσεων
    if (info.is_parallel) {
        report_optimization_error("Loop can be parallelized");
    }
    
    if (info.can_vectorize) {
        report_optimization_error("Loop can be vectorized");
    }
    
    if (info.unroll_factor > 1) {
        report_optimization_error("Loop can be unrolled");
    }
} 