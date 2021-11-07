#include "MatrixIO.h"
#include <string.h>

static int init_dims(FILE *const in, dims_t *const dims){
    if (in == NULL) return ERR_NULL_FD;
    if (dims == NULL) return ERR_NULLPTR;
    if (fscanf(in, "%lu %lu", &dims->ROWS, &dims->COLS) != 2) return ERR_WRONG_DIMS;
    fgetc(in);
    return OK; 
}

matrix_t *init_matrix(FILE *const in){

    dims_t mx_dims = { .COLS = 0, .ROWS = 0 };
    if (init_dims(in, &mx_dims) != OK) return NULL;

    fprintf(stderr, "The dimensions are: %lu rows, %lu cols\n", mx_dims.ROWS, mx_dims.COLS);

    matrix_t *mx = create_matrix(mx_dims);
    if (mx == NULL) return NULL;

    char row_buffer[ROW_BUFFER_SIZE];

    // read the matrix from file
    // read line into buffer first, then read from buffer number by number via sscanf
    for (size_t y = 0; y < mx_dims.ROWS; ++y){
        const char* str = fgets(row_buffer, ROW_BUFFER_SIZE, in);
        if (str == NULL){
            delete_matrix(mx);
            return NULL;
        }
        // fprintf(stderr, "Buffer contains: %s", row_buffer);
        // fprintf(stderr, "%s", row_buffer);
        // fprintf(stderr, "this was row %lu", y);
        char *row = row_buffer;
        for (size_t x = 0; x < mx_dims.COLS; ++x){
            int n = 0;
            const dims_t point = { .COLS = x, .ROWS = y};
            if (sscanf(row, "%d%n", &mx->cells[idx(mx_dims, point)], &n) != 1){
                delete_matrix(mx);
                return NULL;
            }
            row += n;
            // fprintf(stderr, "read this: %d , index is %lu\n", mx->cells[idx(mx_dims, point)], idx(mx_dims, point));
        }
    }

    return mx;
    // read dims_t from the file first
    // then create_matrix and fill its contents

    // read from file to some kind of buffer and
    // fill the matrix with corresponding values

    // what if i used mmap for file io?
    // that's what it was kinda designed for
}

int pretty_print_matrix(FILE *const out, const matrix_t *const mx){
    if (out == NULL)
        return ERR_NULL_FD;
    if (mx == NULL)
        return ERR_NULLPTR;
    if (mx->cells == NULL)
        return ERR_NULLPTR;

    const dims_t dm = mx->dims;

    fprintf(out, "Pretty prints matrix\n");

    for (size_t y = 0; y < dm.ROWS; ++y){
        for (size_t x = 0; x < dm.COLS; ++x){
            const dims_t point = { .COLS = x, .ROWS = y };
            const size_t cell = idx(dm, point);
            fprintf(out, " %d ", mx->cells[cell]);
        }
        fprintf(out, "\n");
    }
    fprintf(out, "Printed!\n");
    return OK;
}

int free_used_resources(FILE *in, FILE *out){
    if (in != NULL){
        fclose(in);
        fprintf(stderr, "Closed in file\n");
    }
    if (out != NULL){
        fclose(out);
        fprintf(stderr, "Closed out file\n");
    }
    return OK;
}
