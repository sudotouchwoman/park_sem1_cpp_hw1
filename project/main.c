#include "Mirror.h"
#include <stdlib.h>

int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr, "No file specified, stopped.\n");
        return EXIT_FAILURE;
    }
    FILE *in = fopen(argv[1], "r");
    if (in == NULL){
        fprintf(stderr, "Failed to open specified file!\n");
        return EXIT_FAILURE;
    }
    matrix_t *mx = init_matrix(in);
    if (mx == NULL){
        fprintf(stderr, "Failed to init matrix!\n");
        free_used_resources(in, NULL);
        return EXIT_FAILURE;
    }
    if (pretty_print_matrix(stdout, mx) != OK){
        fprintf(stderr, "Failed to write matrix\n");
        free_used_resources(in, NULL);
        delete_matrix(mx);
        return EXIT_FAILURE;
    }
    matrix_t *mx_T = transpose_by_side_diagonal(mx);
    if (mx_T == NULL){
        fprintf(stderr, "Error while transposing\n");
        free_used_resources(in, NULL);
        delete_matrix(mx);
        return EXIT_FAILURE;
    }
    if (pretty_print_matrix(stdout, mx_T) != OK){
        fprintf(stderr, "Error while printing transposed\n");
        free_used_resources(in, NULL);
        delete_matrix(mx);
        delete_matrix(mx_T);
        return EXIT_FAILURE;
    }
    free_used_resources(in, NULL);
    delete_matrix(mx);
    delete_matrix(mx_T);
    fprintf(stderr, "Finished, OK\n");
    return EXIT_SUCCESS;
}