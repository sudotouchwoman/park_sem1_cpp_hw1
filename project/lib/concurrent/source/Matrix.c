#include "Matrix.h"
#include <sys/mman.h>

matrix_t* create_matrix(const size_t rows, const size_t columns){
    matrix_t *new_matrix = (matrix_t*)malloc(sizeof(matrix_t));
    if (new_matrix == NULL){
        return NULL;
    }
    new_matrix->cells = (int**)malloc(sizeof(int*) * rows);
    if (new_matrix->cells == NULL){
        free(new_matrix);
        return NULL;
    }
    for (size_t i = 0; i < rows; ++i){
        new_matrix->cells[i] = (int*)mmap(
                NULL, sizeof(int) * columns, PROT_READ | PROT_WRITE,
                MAP_ANONYMOUS | MAP_SHARED, -1, 0);
        if (new_matrix->cells[i] == MAP_FAILED){
            for (size_t j = 0; j < i; ++j){
                munmap(new_matrix->cells[j], sizeof(int) * columns);
            }
            free(new_matrix->cells);
            free(new_matrix);
            return NULL;
        }
    }
    // for (size_t i = 0; i < m; ++i){
    //     new_matrix->cells[i] = (int*)malloc(sizeof(int) * m);
    //     if (new_matrix->cells[i] == NULL){
    //         for (size_t j = 0; j < i; ++i){
    //             free(new_matrix->cells[j]);
    //         }
    //         free(new_matrix->cells);
    //         free(new_matrix);
    //         return NULL;
    //     }
    // }
    new_matrix->M_ROWS = rows;
    new_matrix->N_COLUMNS = columns;
    return new_matrix;
}

int delete_matrix(matrix_t *to_remove){
    if (to_remove == NULL){
        return ERR_EMPTY_MATRIX;
    }
    free(to_remove);
    return OK;
}
