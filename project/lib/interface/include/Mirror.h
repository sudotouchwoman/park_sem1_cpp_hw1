#ifndef _MIRROR_H
#define _MIRROR_H

#include <stdio.h>
#include "Matrix.h"

enum common_errors{
    ERR_NULL_FD = 1,
    ERR_WRONG_DIMS
};

typedef struct{
    const size_t rows;
    const size_t columns;
} dims_t;

matrix_t *init_matrix(FILE *fd, const size_t rows, const size_t columns);
int free_used_resources(FILE *fd, matrix_t *);

#endif // _MIRROR_H
