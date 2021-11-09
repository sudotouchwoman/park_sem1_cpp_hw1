#include "Processing.h"

const size_t idx(const dims_t md, const dims_t point) {
    // basic 2D to 1D index expression
    // so far I allocate 1D array
    return md.COLS * point.ROWS + point.COLS;
}

const size_t idx_T(const dims_t md, const dims_t point) {
    // this one is a bit tricky,
    // but to sum up all the math behind it,
    // this expression makes 1D array index
    // for given ORIGINAL matrix dimensions
    // and given point (after "transposition", if I can reffer to in so,
    // the axes are inverted but of same size with the original)
    const size_t new_col = md.ROWS - point.ROWS - 1;
    const size_t new_row = md.COLS - point.COLS - 1;
    return md.ROWS * new_row + new_col;
}

static int* init_cells(const dims_t size) {
    // use mmap for concurrent version in contrast to calloc
    const size_t area = size.ROWS * size.COLS;
    return (int*)mmap(
        NULL, sizeof(int) * area, PROT_WRITE | PROT_READ,
        MAP_ANONYMOUS | MAP_SHARED, -1, 0);
}

static int free_cells(int* mapped, const dims_t size) {
    // use munmap instead of free for this version
    // note MAP_FAILED macro in contrast to NULL
    if (mapped == NULL || mapped == MAP_FAILED) {
        return MX_ERR_EMPTY_MX;
    }
    const size_t area = size.ROWS * size.COLS;
    int status = munmap(mapped, area);
    return status;
}

shared_matrix_t* create_matrix(const dims_t size) {
    shared_matrix_t *new_matrix = (shared_matrix_t*)calloc(1, sizeof(shared_matrix_t));
    if (new_matrix == NULL) {
        return NULL;
    }
    new_matrix->cells = init_cells(size);
    if (new_matrix->cells == MAP_FAILED) {
        free(new_matrix);
        return NULL;
    }
    new_matrix->dims = size;
    return new_matrix;
}

int delete_matrix(shared_matrix_t *to_remove) {
    if (to_remove == NULL) {
        return MX_ERR_EMPTY_MX;
    }
    if (to_remove->cells == NULL) {
        free(to_remove);
        return MX_ERR_EMPTY_MX;
    }
    free_cells(to_remove->cells, to_remove->dims);
    free(to_remove);
    return MX_OK;
}
