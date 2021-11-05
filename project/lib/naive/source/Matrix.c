#include "Matrix.h"

static int* init_cells(const dims_t size){
    const size_t area = size.ROWS * size.COLS;
    return (int*)calloc(area, sizeof(int));
}

matrix_t* create_matrix(const dims_t size){
    matrix_t *new_matrix = (matrix_t*)malloc(sizeof(matrix_t));
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
