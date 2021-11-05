#ifndef _MATRIX_H
#define _MATRIX_H

#include <stdlib.h>

enum matrix_errors{
    OK = 0,
    ERR_EMPTY_MATRIX
};

typedef struct {
    int **cells;
    size_t N_COLUMNS;
    size_t M_ROWS;
} matrix_t;

matrix_t* create_matrix(const size_t rows, const size_t columns);
int delete_matrix(matrix_t*);

#endif // _MATRIX_H
