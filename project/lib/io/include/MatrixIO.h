#ifndef _MATRIX_IO_H
#define _MATRIX_IO_H

#include <stdio.h>
#include "Matrix.h"

enum common_errors{
    OK = 0,
    ERR_NULL_FD,
    ERR_WRONG_DIMS,
    ERR_NULLPTR
};

matrix_t *init_matrix(FILE *const in);
matrix_t *init_matrix_fromarray(const int* const mx_array, const dims_t md);
int pretty_print_matrix(FILE *const out, const matrix_t *const mx);
int free_used_resources(FILE *in, FILE *out);

#endif  // _MATRIX_IO_H
