#include "MatrixIO.h"
#include <string.h>

static int init_dims(FILE *const in, dims_t *const dims) {
    if (in == NULL) return ERR_NULL_FD;
    if (dims == NULL) return ERR_NULLPTR;
    if (fscanf(in, "%lu %lu", &dims->ROWS, &dims->COLS) != 2) return ERR_WRONG_DIMS;
    fgetc(in);  // flush buffer
    return OK;
}

matrix_t *init_matrix(FILE *const in) {
    // read dims_t from the file first
    // then create_matrix and fill its contents

    // what if i used mmap for file io?
    // that's what it was kinda designed for
    dims_t md = { .COLS = 0, .ROWS = 0 };
    if (init_dims(in, &md) != OK) return NULL;

    matrix_t *mx = create_matrix(md);
    if (mx == NULL) return NULL;

    char row_buffer[ROW_BUFFER_SIZE];

    // read the matrix from file
    // read line into buffer first, then read from buffer number by number via sscanf
    for (size_t y = 0; y < md.ROWS; ++y) {
        const char* str = fgets(row_buffer, ROW_BUFFER_SIZE, in);
        if (str == NULL) {
            delete_matrix(mx);
            return NULL;
        }
        char *row = row_buffer;
        for (size_t x = 0; x < md.COLS; ++x) {
            int n = 0;
            const dims_t point = { .COLS = x, .ROWS = y};
            if (sscanf(row, "%d%n", &mx->cells[idx(md, point)], &n) != 1) {
                delete_matrix(mx);
                return NULL;
            }
            row += n;  // move pointer n chars right
        }
    }

    return mx;
}

int pretty_print_matrix(FILE *const out, const matrix_t *const mx) {
    if (out == NULL)
        return ERR_NULL_FD;
    if (mx == NULL)
        return ERR_NULLPTR;
    if (mx->cells == NULL)
        return ERR_NULLPTR;

    const dims_t dm = mx->dims;

    fprintf(out, "Pretty prints matrix\n");

    for (size_t y = 0; y < dm.ROWS; ++y) {
        for (size_t x = 0; x < dm.COLS; ++x) {
            const dims_t point = { .COLS = x, .ROWS = y };
            const size_t cell = idx(dm, point);
            fprintf(out, " %d ", mx->cells[cell]);
        }
        fprintf(out, "\n");
    }
    fprintf(out, "Printed!\n");
    return OK;
}

int free_used_resources(FILE *in, FILE *out) {
    if (in != NULL) fclose(in);
    if (out != NULL) fclose(out);
    return OK;
}
