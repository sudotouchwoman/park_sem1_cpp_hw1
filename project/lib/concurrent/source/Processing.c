#include "Processing.h"

static int move_cursor(const dims_t md, dims_t *const cursor) {
    // move cursor to the next cell of the matrix
    // | * * * * * * * * * # |
    // | * * * * * * * * * # |
    // | * * * * * * * * * # |
    // | * * * * * * * * * % |
    // in the schema above, `#` represent positioins
    // where the cursor should move to the next line
    // in case of `%` we reached the end of matrix
    // so the processing should stop
    // in this case `PROC_EOMATRIX` status code is returned
    if (cursor == NULL) return PROC_ERR_NULLPTR;
    if ((md.ROWS - cursor->ROWS == 1) && (md.COLS - cursor->COLS == 1)) return PROC_EOMATRIX;

    if (md.COLS - cursor->COLS == 1) {
        // in case we reached end of row
        // move cursor to the next line
        cursor->COLS = 0;
        ++cursor->ROWS;
        return PROC_OK;
    }
    // otherwise increment the column attribute
    ++cursor->COLS;
    return PROC_OK;
}

static int map_element(const shared_matrix_t *const mx, int *const shared_cells, const dims_t cursor) {
    // copy the value from original matrix to the new one
    // so far I did not found an in-place solution so
    // 2 matrices are utilized in the algo
    // the rules of computing indices are implemented in `idx`
    // and `idx_T` functions
    const size_t index = idx(mx->dims, cursor);
    const size_t T_index = idx_T(mx->dims, cursor);
    shared_cells[T_index] = mx->cells[index];
    // I do not bound-check the cursor position
    // as this function is only called for validated one
    // actually, I double-check this in `move_cursor`
    return PROC_OK;
}

static void map_range(const size_t from, const size_t to, const shared_matrix_t *const mx, int *const shared_cells) {
    size_t cursor = from;
    dims_t cursor_asdims = { .ROWS = from / mx->dims.COLS, .COLS = from % mx->dims.COLS };
    while (cursor++ <= to) {
        map_element(mx, shared_cells, cursor_asdims);
        if (move_cursor(mx->dims, &cursor_asdims) != PROC_OK) break;
    }
    return;
}

shared_matrix_t* transpose_by_side_diagonal(const shared_matrix_t *const mx) {
    // concurrent version uses fork syscall
    // to produce child processes
    // the amount of latter is calculated using total number of logical cores
    // the matrix processing is equally (or almost equally)
    // split between the processes
    // in case there is a remainder, each process would process
    // one extra cell -> the last one is left with less work to do!
    // e.g., if there are 6 cores with 2 threads each on my 5600H,
    // it has 12 cpus from c's pov
    // if then there is a 100 by 100 matrix to process,
    // each process will work with 1e4 / 12 = <pulls calc from the pocket> = 83 cells per child
    // but 0.(3) cells are left orphant thus the final split is 84 cells per each child
    // but the last which recieves 84 - 8 = 76 cells
    if (mx == NULL) return NULL;

    const dims_t md = mx->dims;
    const dims_t T_dims = { .COLS = md.ROWS, .ROWS = md.COLS };

    shared_matrix_t *shared_mx = create_matrix(T_dims);
    if (shared_mx == NULL) {
        return NULL;
    }

    const size_t n_cpus = (size_t)get_nprocs();
    const size_t area = md.COLS * md.ROWS;

    if (n_cpus == 0 || area == 0) {
        // this definetely means something went wrong earlier,
        // but just in case
        delete_matrix(shared_mx);
        return NULL;
    }

    const size_t max_processes = n_cpus > area ? area : n_cpus;
    const size_t cells_per_child = area / max_processes + (area % max_processes ? 1 : 0);
    const pid_t parent = getpid();

    int *const shared_cells = shared_mx->cells; // in sake of shortness

    for (size_t i = 0; i < n_cpus; ++i) {
        const pid_t pid = fork();
        if (pid == PROC_BAD_FORK) {
            waitpid(0, NULL, 0);
            delete_matrix(shared_mx);
            return NULL;
        }
        if (pid == PROC_ISCHILD) {
            const size_t from = cells_per_child * i > area ? area : cells_per_child * i;
            const size_t to = cells_per_child * (i+1) > area ? area : cells_per_child * (i+1);
            // in case there is nothing left to work with
            if (from == to) exit(EXIT_SUCCESS);
           
            map_range(from, to - 1, mx, shared_cells);
            exit(EXIT_SUCCESS);
        }
    }

    const pid_t pid = getpid();
    if (pid != parent) {
        // child
        // this is basically impossible,
        // but once again just in case
        exit(EXIT_FAILURE);
    } else { 
        // parent
        waitpid(0, NULL, 0);
    }

    return shared_mx;
}
