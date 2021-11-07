#include "Processing.h"
#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <stdio.h>

static int get_next_point(const dims_t mx_dims, dims_t *const point){
    if (point == NULL) return PROC_ERR_NULLPTR;
    if ((mx_dims.ROWS - point->ROWS == 1) && (mx_dims.COLS - point->COLS == 1)) return PROC_EOMATRIX;

    if (mx_dims.COLS - point->COLS == 1){
        point->COLS = 0;
        ++point->ROWS;
        return PROC_OK;
    }
    ++point->COLS;
    return PROC_OK;
}

static int map_element(const matrix_t *const mx, int *const shared_cells, const dims_t point){

    const size_t index = idx(mx->dims, point);
    const size_t T_index = idx_T(mx->dims, point);
    shared_cells[T_index] = mx->cells[index];

    return PROC_OK;
}

static void map_range(const size_t from, const size_t to, const matrix_t *const mx, int *const shared_cells){
    
    size_t cursor = from;
    dims_t cursor_asdims = { .ROWS = from / mx->dims.COLS, .COLS = from % mx->dims.COLS };
    while (cursor++ <= to){
        map_element(mx, shared_cells, cursor_asdims);
        if (get_next_point(mx->dims, &cursor_asdims) != PROC_OK) break;
    }
    return;
}

matrix_t* transpose_by_side_diagonal(const matrix_t *const mx){
    fprintf(stderr, "entered transpose\n");
    if (mx == NULL) return NULL;

    const dims_t mx_dims = mx->dims;
    const dims_t T_dims = { .COLS = mx_dims.ROWS, .ROWS = mx_dims.COLS };
    
    shared_matrix_t *shared_mx = create_matrix(T_dims);
    if (shared_mx == NULL){
        return NULL;
    }
    fprintf(stderr, "created temporary shared matrix\n");

    const size_t n_cpus = (size_t)get_nprocs();
    const size_t area = mx_dims.COLS * mx_dims.ROWS;
    fprintf(stderr, "got number of cores: %lu\n", n_cpus);

    if (n_cpus == 0 || area == 0){
        delete_matrix(shared_mx);
        return NULL;
    }

    const size_t max_processes = n_cpus > area ? area : n_cpus;
    const size_t cells_per_child = area / max_processes + (area % max_processes ? 1 : 0);
    const pid_t parent = getpid();

    int *const shared_cells = shared_mx->cells;

    fprintf(stderr, "got number of cells per child: %lu\n", cells_per_child);

    for (size_t i = 0; i < n_cpus; ++i){
        const pid_t pid = fork();
        if (pid == PROC_BAD_FORK){
            wait(NULL);
            delete_matrix(shared_mx);
            return NULL;
        }
        if (pid == PROC_ISCHILD){
            const size_t from = cells_per_child * i > area ? area : cells_per_child * i;
            const size_t to = cells_per_child * (i+1) > area ? area : cells_per_child * (i+1);
            if (from == to){
                fprintf(stderr, "the process has nothing to do as the bounds are: %lu-%lu", from, to);
                exit(EXIT_SUCCESS);
            }
            map_range(from, to - 1, mx, shared_cells);
            exit(EXIT_SUCCESS);
        }
        if (pid == PROC_ISCHILD){
            exit(EXIT_SUCCESS);
        }
    }

    const pid_t pid = getpid();
    if (pid != parent){
        fprintf(stderr, "Found zombie process! Killing it.\n");
        exit(EXIT_FAILURE);
    }
    else{
        int status = 5;
        waitpid(0, &status, 0);
        fprintf(stderr, "Killed processes, status is %d\n", status);
    }

    return shared_mx;
    // const size_t n_processes = ?

    // get total number of processes

}