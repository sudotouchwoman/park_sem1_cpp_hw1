#include "Processing.h"
#include <stdio.h>

const size_t idx(const dims_t mx_dims, const dims_t point){
    return mx_dims.COLS * point.ROWS + point.COLS;
}

const size_t idx_T(const dims_t mx_dims, const dims_t point){
    const size_t new_col = mx_dims.ROWS - point.ROWS - 1;
    const size_t new_row = mx_dims.COLS - point.COLS - 1;
    return mx_dims.ROWS * new_row + new_col;
}

static int* init_cells(const dims_t size){
    const size_t area = size.ROWS * size.COLS;
    return (int*)mmap(
        NULL, sizeof(int) * area, PROT_WRITE | PROT_READ,
        MAP_ANONYMOUS | MAP_SHARED, -1, 0);
}

static int free_cells(int* mapped, const dims_t size){
    if (mapped == NULL || mapped == MAP_FAILED){
        return MX_ERR_EMPTY_MX;
    }
    const size_t area = size.ROWS * size.COLS;
    int status = munmap(mapped, area);
    return status;
}

shared_matrix_t* create_matrix(const dims_t size){
    shared_matrix_t *new_matrix = (shared_matrix_t*)malloc(sizeof(shared_matrix_t));
    if (new_matrix == NULL){
        return NULL;
    }
    new_matrix->cells = init_cells(size);
    if (new_matrix->cells == MAP_FAILED){
        free(new_matrix);
        return NULL;
    }
    new_matrix->dims = size;
    fprintf(stderr, "created shared matrix\n");
    return new_matrix;
}

int delete_matrix(shared_matrix_t *to_remove){
    if (to_remove == NULL){
        return MX_ERR_EMPTY_MX;
    }
    if (to_remove->cells == NULL){
        free(to_remove);
        return MX_ERR_EMPTY_MX;
    }
    free_cells(to_remove->cells, to_remove->dims);
    free(to_remove);
    fprintf(stderr, "deleted shared matrix\n");
    return MX_OK;
}
