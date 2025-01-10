#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <stdbool.h>
#include "intermediate.h"

// Συναρτήσεις βελτιστοποίησης βρόχων
void set_unroll_factor(ic_node* loop, int factor);
void mark_for_vectorization(ic_node* loop);
void mark_for_parallelization(ic_node* loop);
void optimize_code(void);

#endif 