#include "Matrix.h"

static int* init_cells(const dims_t size){
    const size_t area = size.ROWS * size.COLS;
    return (int*)calloc(area, sizeof(int));
}

const size_t idx(const dims_t mx_dims, const dims_t point){
    return mx_dims.COLS * point.ROWS + point.COLS;
}

const size_t idx_T(const dims_t mx_dims, const dims_t point){
    const size_t new_col = mx_dims.ROWS - point.ROWS - 1;
    const size_t new_row = mx_dims.COLS - point.COLS - 1;
    return mx_dims.ROWS * new_row + new_col;
}

matrix_t* create_matrix(const dims_t size){
    matrix_t *new_matrix = (matrix_t*)calloc(1, sizeof(matrix_t));
    if (new_matrix == NULL){
        return NULL;
    }
    new_matrix->cells = init_cells(size);
    if (new_matrix->cells == NULL){
        free(new_matrix);
        return NULL;
    }
    new_matrix->dims = size;
    return new_matrix;
}

int delete_matrix(matrix_t *to_remove){
    if (to_remove == NULL){
        return MX_ERR_EMPTY_MX;
    }
    if (to_remove->cells == NULL){
        free(to_remove);
        return MX_ERR_EMPTY_MX;
    }
    free(to_remove->cells);
    free(to_remove);
    return MX_OK;
}
