#ifndef _MIRROR_H
#define _MIRROR_H

#include <stdio.h>
#include "Matrix.h"

enum common_errors{
    ERR_NULL_FD = 1,
    ERR_WRONG_DIMS,
    ERR_NULLPTR
};

matrix_t *init_matrix(FILE *const in);
int pretty_print_matrix(FILE *const out, const matrix_t *const);
int free_used_resources(FILE *in, FILE *out, matrix_t *);

#endif // _MIRROR_H
