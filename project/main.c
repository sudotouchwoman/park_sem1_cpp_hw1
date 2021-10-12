#include "dataloader.h"
#include "processing.h"

#define USER_INPUT_FILE "../data/user_input_ok.txt"
#define USER_BLOG_FILE "../data/user_blog_ok.txt"

int main(int argc, char* argv[]){

    // initialize file descriptors to be used in the application
    // one containing user-specified time period and another containing collection of posts

    FILE *user_in = fopen(USER_INPUT_FILE, "rt");
    FILE *blog_in = fopen(USER_BLOG_FILE, "rt");

    if (user_in == NULL){
        fprintf(stderr, "Failed to open user file!\n");
        return EXIT_FAILURE;
    }
    if (blog_in == NULL){
        fprintf(stderr, "Failed to open blog file!\n");
        free_used_resources(user_in, NULL, NULL, NULL); 
        return EXIT_FAILURE;
    }

    // shorter way of storing time period
    size_t time_period[3] = {0, 0, 0};
    // set period values from data provided in user_in file (of format Y;M;D;)
    if (make_time_period(user_in, &time_period[0], &time_period[1], &time_period[2]) != 0){
        fprintf(stderr, "Failed to collect time delta info!\n");
        free_used_resources(user_in, blog_in, NULL, NULL); 
        return EXIT_FAILURE;
    }
    // try to create and fill initial post collection
    // returns NULL if anything wrong has happened
    blog_t* input_blog = make_blog(blog_in);
    if (input_blog == NULL){
        fprintf(stderr, "Failed to collect blog info!\n");
        free_used_resources(user_in, blog_in, NULL, NULL); 
        return EXIT_FAILURE;
    }
    // select the post or set of posts which have most reactions (votes and comments)
    blog_t* most_hyped = select_most_hyped(input_blog, time_period);
    if (most_hyped == NULL){
        fprintf(stderr, "Failed to extract info about most hyped posts!\n");
        free_used_resources(user_in, blog_in, input_blog, NULL); 
        return EXIT_FAILURE;
    }
    // if we came this far, it means that everything worked out smoothly
    // just pretty-print the contents of all posts in resulting collection
    print_most_hyped(stdout, most_hyped);

    // of course, all allocated resources must be freed (file descriptors and custom structs)
    free_used_resources(user_in, blog_in, input_blog, most_hyped);

    return 0;
}
