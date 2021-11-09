#include "Processing.h"

const size_t idx(const dims_t md, const dims_t point){
    return md.COLS * point.ROWS + point.COLS;
}

const size_t idx_T(const dims_t md, const dims_t point){
    const size_t new_col = md.ROWS - point.ROWS - 1;
    const size_t new_row = md.COLS - point.COLS - 1;
    return md.ROWS * new_row + new_col;
}

matrix_t* transpose_by_side_diagonal(matrix_t const *const mx){
    if (mx == NULL) return NULL;

    const dims_t md = mx->dims;
    const dims_t mxT_dims = {
        // new matrix will have inverted dims
        .ROWS = md.COLS,
        .COLS = md.ROWS
    };
    
    matrix_t * transposed = create_matrix(mxT_dims);
    if (transposed == NULL) return NULL;

    for (size_t y = 0; y < md.ROWS; ++y){
        for (size_t x = 0; x < md.COLS; ++x){

            const dims_t point = { .ROWS = y, .COLS = x };
            const size_t index = idx(md, point);
            const size_t T_index = idx_T(md, point);
            
            transposed->cells[T_index] = mx->cells[index];
        }
    }
    return transposed;
}
