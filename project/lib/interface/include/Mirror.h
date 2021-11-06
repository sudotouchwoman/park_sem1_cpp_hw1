#ifndef _MIRROR_H
#define _MIRROR_H

#include <stdio.h>
#include "Matrix.h"

#define ROW_BUFFER_SIZE 20000

enum common_errors{
    OK = 0,
    ERR_NULL_FD,
    ERR_WRONG_DIMS,
    ERR_NULLPTR
};

matrix_t *init_matrix(FILE *const in);
int pretty_print_matrix(FILE *const out, const matrix_t *const mx);
matrix_t* transpose_by_side_diagonal(matrix_t const *const mx);
int free_used_resources(FILE *in, FILE *out);

#endif // _MIRROR_H
