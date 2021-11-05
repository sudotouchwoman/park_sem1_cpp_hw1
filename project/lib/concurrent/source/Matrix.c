#include <sys/mman.h>

#include "Matrix.h"

static int* init_cells(const dims_t size){
    const size_t area = size.ROWS * size.COLS;
    return (int*)mmap(
        NULL, sizeof(int) * area, PROT_WRITE | PROT_READ,
        MAP_ANONYMOUS | MAP_SHARED, -1, 0);
}

static int free_cells(int* mapped, const dims_t size){
    if (mapped == NULL || mapped == MAP_FAILED){
        return ERR_EMPTY_MATRIX;
    }
    const size_t area = size.ROWS * size.COLS;
    int status = munmap(mapped, area);
    return status;
}

matrix_t* create_matrix(const dims_t size){
    matrix_t *new_matrix = (matrix_t*)malloc(sizeof(matrix_t));
    if (new_matrix == NULL){
        return NULL;
    }
    new_matrix->cells = init_cells(size);
    if (new_matrix->cells == MAP_FAILED){
        free(new_matrix);
        return NULL;
    }
    new_matrix->dims = size;
    return new_matrix;
}

int delete_matrix(matrix_t *to_remove){
    if (to_remove == NULL){
        return ERR_EMPTY_MATRIX;
    }
    if (to_remove->cells == NULL){
        free(to_remove);
        return ERR_EMPTY_MATRIX;
    }
    free_cells(to_remove->cells, to_remove->dims);
    free(to_remove);
    return OK;
}
