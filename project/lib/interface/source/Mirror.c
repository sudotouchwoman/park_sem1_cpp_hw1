#include "Mirror.h"

matrix_t *init_matrix(FILE *in){

    // read dims_t from the file first
    // then create_matrix and fill its contents
    // matrix_t *new_matrix = create_matrix(size);
    // if (new_matrix == NULL) return NULL;

    // read from file to some kind of buffer and
    // fill the matrix with corresponding values


}

int pretty_print_matrix(FILE *out, const matrix_t *matrix){
    if (out == NULL)
        return ERR_NULL_FD;
    if (matrix == NULL)
        return ERR_NULL_PTR;
    if (matrix->cells == NULL)
        return ERR_NULL_PTR;

    for (size_t y = 0; y < matrix->dims.ROWS; ++y){
        for (size_t x = 0; x < matrix->dims.COLS; ++x){
            const dims_t point = {
                .COLS = x,
                .ROWS = y
            };
            const size_t idx = index(matrix->dims, point);
            fprintf(out, " %d ", matrix->cells[idx]);
        }
        fprintf(out, "\n");
    }
    return OK;
}

int free_used_resources(FILE *in, FILE *out, matrix_t *matrix){
    if (in != NULL) fclose(in);
    if (out != NULL) fclose(out);
    delete_matrix(matrix);
    return OK;
}
