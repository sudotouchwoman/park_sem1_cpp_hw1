#include "Mirror.h"

// matrix_t *init_matrix(FILE *const in){

//     // read dims_t from the file first
//     // then create_matrix and fill its contents
//     // matrix_t *new_matrix = create_matrix(size);
//     // if (new_matrix == NULL) return NULL;

//     // read from file to some kind of buffer and
//     // fill the matrix with corresponding values

//     // what if i used mmap for file io?
//     // that's what it was kinda designed for

// }

int pretty_print_matrix(FILE *const out, const matrix_t *const mx){
    if (out == NULL)
        return ERR_NULL_FD;
    if (mx == NULL)
        return ERR_NULLPTR;
    if (mx->cells == NULL)
        return ERR_NULLPTR;

    for (size_t y = 0; y < mx->dims.ROWS; ++y){
        for (size_t x = 0; x < mx->dims.COLS; ++x){
            const dims_t point = {
                .COLS = x,
                .ROWS = y
            };
            const size_t cell = idx(mx->dims, point);
            fprintf(out, " %d ", mx->cells[cell]);
        }
        fprintf(out, "\n");
    }
    return MX_OK;
}

int free_used_resources(FILE *in, FILE *out, matrix_t *mx){
    if (in != NULL) fclose(in);
    if (out != NULL) fclose(out);
    delete_matrix(mx);
    return MX_OK;
}
