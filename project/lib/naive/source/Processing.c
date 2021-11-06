#include "Processing.h"

matrix_t* transpose_by_side_diagonal(matrix_t const *const mx){
    if (mx == NULL) return NULL;

    const dims_t mx_dims = mx->dims;
    const dims_t mxT_dims = {
        .ROWS = mx_dims.COLS,
        .COLS = mx_dims.ROWS
    };
    
    matrix_t * transposed = create_matrix(mxT_dims);
    if (transposed == NULL) return NULL;

    for (size_t y = 0; y < mx_dims.ROWS; ++y){
        for (size_t x = 0; x < mx_dims.COLS; ++x){

            const dims_t point = { .ROWS = y, .COLS = x };
            const size_t index = idx(mx_dims, point);
            const size_t T_index = idx_T(mx_dims, point);
            
            transposed->cells[T_index] = mx->cells[index];;
        }
    }
    return transposed;
}
