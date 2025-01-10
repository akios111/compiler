#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "intermediate.h"
#include "types.h"
#include "codegen.h"
#include "optimization.h"

// Καθολικές μεταβλητές
extern int yyparse(void);
extern FILE* yyin;
ic_node* ic_head = NULL;
static int label_counter = 0;

// Επιλογές βελτιστοποίησης
struct {
    int optimization_level;
    bool verify_optimizations;
    bool show_optimization_stats;
    bool loop_optimizations;
    bool vectorization;
    bool parallelization;
} options = {
    .optimization_level = 0,
    .verify_optimizations = false,
    .show_optimization_stats = false,
    .loop_optimizations = true,
    .vectorization = false,
    .parallelization = false
};

// Στατιστικά βελτιστοποίησης
struct {
    int functions_inlined;
    int loops_unrolled;
    int variables_eliminated;
    int expressions_simplified;
    int memory_accesses_optimized;
} stats = {0};

// Δηλώσεις συναρτήσεων
void generate_code(FILE* out);
void generate_instruction(ic_node* node, FILE* out);

int main(int argc, char* argv[]) {
    // Επεξεργασία παραμέτρων γραμμής εντολών
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-O0") == 0) {
            options.optimization_level = 0;
        }
        else if (strcmp(argv[i], "-O1") == 0) {
            options.optimization_level = 1;
        }
        else if (strcmp(argv[i], "-O2") == 0) {
            options.optimization_level = 2;
        }
        else if (strcmp(argv[i], "-O3") == 0) {
            options.optimization_level = 3;
        }
        else if (strcmp(argv[i], "-verify") == 0) {
            options.verify_optimizations = true;
        }
        else if (strcmp(argv[i], "-stats") == 0) {
            options.show_optimization_stats = true;
        }
        else if (strcmp(argv[i], "-vec") == 0) {
            options.vectorization = true;
        }
        else if (strcmp(argv[i], "-par") == 0) {
            options.parallelization = true;
        }
        else {
            // Άνοιγμα αρχείου εισόδου
            yyin = fopen(argv[i], "r");
            if (!yyin) {
                fprintf(stderr, "Cannot open input file: %s\n", argv[i]);
                return 1;
            }
        }
    }

    // Συντακτική ανάλυση
    if (yyparse() != 0) {
        fprintf(stderr, "Parsing failed\n");
        return 1;
    }

    // Βελτιστοποίηση
    if (options.optimization_level > 0) {
        optimize_code();
    }

    // Παραγωγή κώδικα
    FILE* out = fopen("output.asm", "w");
    if (!out) {
        fprintf(stderr, "Cannot create output file\n");
        return 1;
    }

    generate_code(out);
    fclose(out);

    return 0;
}

// Παραγωγή κώδικα
void generate_code(FILE* out) {
    // Επικεφαλίδα
    fprintf(out, "; Generated code\n\n");
    fprintf(out, "section .text\n");
    fprintf(out, "global main\n\n");

    // Παραγωγή εντολών
    ic_node* current = ic_head;
    while (current) {
        generate_instruction(current, out);
        current = current->next;
    }
}

// Παραγωγή μιας εντολής
void generate_instruction(ic_node* node, FILE* out) {
    if (!node) return;

    switch (node->type) {
        case IC_BINARY:
            // Παραγωγή κώδικα για δυαδικές πράξεις
            fprintf(out, "\tmov eax, [%s]\n", node->arg1);
            switch (node->op) {
                case IC_ADD:
                    fprintf(out, "\tadd eax, [%s]\n", node->arg2);
                    break;
                case IC_SUB:
                    fprintf(out, "\tsub eax, [%s]\n", node->arg2);
                    break;
                case IC_MUL:
                    fprintf(out, "\timul eax, [%s]\n", node->arg2);
                    break;
                case IC_DIV:
                    fprintf(out, "\tcdq\n");
                    fprintf(out, "\tidiv dword [%s]\n", node->arg2);
                    break;
                default:
                    fprintf(stderr, "Warning: Unknown binary operation %d\n", node->op);
                    break;
            }
            fprintf(out, "\tmov [%s], eax\n", node->result);
            break;

        case IC_ASSIGN:
            if (node->arg1) {
                fprintf(out, "\tmov eax, [%s]\n", node->arg1);
                fprintf(out, "\tmov [%s], eax\n", node->result);
            } else {
                fprintf(out, "\tmov dword [%s], 0\n", node->result);
            }
            break;

        case IC_GOTO:
            fprintf(out, "\tjmp L%d\n", node->label);
            break;

        case IC_IF_GOTO:
            fprintf(out, "\tmov eax, [%s]\n", node->result);
            fprintf(out, "\tcmp eax, 0\n");
            fprintf(out, "\tjnz L%d\n", node->label);
            break;

        case IC_LABEL:
            fprintf(out, "L%d:\n", node->label);
            break;

        case IC_CALL:
            // Αποθήκευση παραμέτρων στη στοίβα (από δεξιά προς αριστερά)
            if (node->arg1) {
                for (int i = node->label - 1; i >= 0; i--) {
                    fprintf(out, "\tpush dword [%s]\n", ((char**)node->arg1)[i]);
                }
            }
            fprintf(out, "\tcall %s\n", node->result);
            // Καθαρισμός στοίβας αν υπάρχουν παράμετροι
            if (node->label > 0) {
                fprintf(out, "\tadd esp, %d\n", node->label * 4);
            }
            break;

        case IC_DO_START:
            // Αρχικοποίηση μετρητή βρόχου
            fprintf(out, "\tmov eax, [%s]\n", node->arg1);
            fprintf(out, "\tmov [%s], eax\n", node->result);
            fprintf(out, "L%d:\n", label_counter);  // Αρχή βρόχου
            // Έλεγχος τερματισμού
            fprintf(out, "\tmov eax, [%s]\n", node->result);
            fprintf(out, "\tcmp eax, [%s]\n", node->arg2);
            fprintf(out, "\tjg L%d\n", label_counter + 1);  // Έξοδος αν counter > end
            label_counter += 2;
            break;

        case IC_DO_END:
            // Αύξηση μετρητή και επιστροφή στην αρχή
            fprintf(out, "\tmov eax, [%s]\n", node->result);
            fprintf(out, "\tinc eax\n");
            fprintf(out, "\tmov [%s], eax\n", node->result);
            fprintf(out, "\tjmp L%d\n", label_counter - 2);
            fprintf(out, "L%d:\n", label_counter - 1);  // Ετικέτα εξόδου
            break;

        default:
            fprintf(stderr, "Warning: Unknown instruction type %d\n", node->type);
            break;
    }
}