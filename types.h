#ifndef TYPES_H
#define TYPES_H

/* Symbol types */
typedef enum {
    TYPE_UNKNOWN,    /* Unknown or invalid type */
    TYPE_PROGRAM,    /* Program declaration type */
    TYPE_INTEGER,    /* Integer type */
    TYPE_REAL,       /* Real (floating point) type */
    TYPE_COMPLEX,    /* Complex number type (real + imaginary) */
    TYPE_LOGICAL,    /* Logical type (.true. or .false.) */
    TYPE_CHARACTER,  /* Single character type */
    TYPE_STRING,     /* String type */
    TYPE_ARRAY,      /* Array type (used in conjunction with base type) */
    TYPE_FUNCTION    /* Added back */
} symbol_type_t;

/* Complex number structure */
typedef struct {
    double real;     /* Real part */
    double imag;     /* Imaginary part */
} complex_t;

/* Array dimensions structure */
typedef struct {
    int dimensions[7];  /* FORTRAN allows up to 7 dimensions */
    int num_dims;
} array_dims_t;

#endif /* TYPES_H */