#ifndef _MATRIX_H
#define _MATRIX_H

#include <stdlib.h>

enum matrix_errors{
    OK = 0,
    ERR_EMPTY_MATRIX
};

typedef struct{
    size_t ROWS;
    size_t COLS;
} dims_t;

typedef struct {
    int *cells;
    dims_t dims;
} matrix_t;

matrix_t* create_matrix(const dims_t);
int mirror_matrix(matrix_t *);
int delete_matrix(matrix_t*);
size_t index(const dims_t matrix_dims, const dims_t point){
    return matrix_dims.COLS * point.ROWS + point.COLS;
}

#endif // _MATRIX_H
