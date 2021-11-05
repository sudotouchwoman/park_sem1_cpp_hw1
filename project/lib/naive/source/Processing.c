#include "Processing.h"

static int swap(int *const a, int *const b){
    if (a == NULL || b == NULL) return PROC_ERR_NULLPTR;
    const int tmp = *a;
    *a = *b;
    *b = tmp;
    return PROC_OK;
}

static int swap_with_complimentary(matrix_t *const mx, const dims_t point){
    if (mx == NULL) return PROC_ERR_NULLPTR;
    if (mx->cells == NULL) return PROC_ERR_NULLPTR;
    const dims_t complimentary_point = {
        .COLS = mx->dims.COLS - point.COLS,
        .ROWS = mx->dims.ROWS = point.ROWS
    };

    const size_t point_idx = idx(mx->dims, point);
    const size_t compl_idx = idx(mx->dims, complimentary_point);
    
    if (swap(&mx->cells[point_idx], &mx->cells[compl_idx]) != PROC_OK)
        return PROC_ERR;
    return PROC_OK;
}

int mirror_by_side_diagonal(matrix_t *const mx){
    if (mx == NULL) return PROC_ERR_NULLPTR;

    for (size_t y = 0; y < mx->dims.ROWS; ++y){
        for (size_t x = 0; x < mx->dims.COLS - y - 1; ++x){
            const dims_t point = {
                .ROWS = y,
                .COLS = x
            };
            if (swap_with_complimentary(mx, point) != PROC_OK)
                return PROC_ERR;
        }
    }
    return PROC_OK;
}
