#ifndef _MATRIX_H
#define _MATRIX_H

#include <stdlib.h>

enum matrix_errors {
    MX_OK = 0,
    MX_ERR_EMPTY_MX,
    MX_ERR
};

typedef struct {
    size_t ROWS;
    size_t COLS;
} dims_t;

typedef struct {
    int *cells;
    dims_t dims;
} matrix_t;

matrix_t* transpose_by_side_diagonal(matrix_t const *const mx);

matrix_t* create_matrix(const dims_t);
int delete_matrix(matrix_t*);

const size_t idx(const dims_t md, const dims_t point);
const size_t idx_T(const dims_t md, const dims_t point);

#endif // _MATRIX_H
