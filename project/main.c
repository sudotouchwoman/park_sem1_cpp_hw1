#include "dataloader.h"
#include "processing.h"

#define USER_INPUT_FILE "../data/user_input.txt"
#define USER_BLOG_FILE "../data/user_blog.txt"

int main(int argc, char* argv[]){

    FILE *user_in = fopen(USER_INPUT_FILE, "rt");
    FILE *blog_in = fopen(USER_BLOG_FILE, "rt");

    if (argc == 2){
        if (argv[1] == "-user-provided\0"){
            fclose(user_in);
            user_in = stdin;
        }
    }


    if (user_in == NULL){
        fprintf(stderr, "Failed to open user file!\n");
        return EXIT_FAILURE;
    }
    if (blog_in == NULL){
        fprintf(stderr, "Failed to open blog file!\n");
        free_used_resources(user_in, NULL, NULL, NULL); 
        return EXIT_FAILURE;
    }

    size_t time_period[3];
    if (make_time_period(user_in, &time_period[0], &time_period[1], &time_period[2]) != 0){
        fprintf(stderr, "Failed to collect time delta info!\n");
        free_used_resources(user_in, blog_in, NULL, NULL); 
        return EXIT_FAILURE;
    }
    blog_t* input_blog = make_blog(blog_in);
    if (input_blog == NULL){
        fprintf(stderr, "Failed to collect blog info!\n");
        free_used_resources(user_in, blog_in, NULL, NULL); 
        return EXIT_FAILURE;
    }
    blog_t* most_hyped = select_most_hyped(input_blog, time_period);
    if (most_hyped == NULL){
        fprintf(stderr, "Failed to extract info about most hyped posts!\n");
        free_used_resources(user_in, blog_in, input_blog, NULL); 
        return EXIT_FAILURE;
    }
    print_most_hyped(stdout, most_hyped);

    free_used_resources(user_in, blog_in, input_blog, most_hyped);

    return 0;
}
