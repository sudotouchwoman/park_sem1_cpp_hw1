#include "dataloader.h"
#include <string.h>

blog_t* make_blog(FILE* fd_in){
    if (fd_in == NULL) return NULL;
    size_t posts_total = 0;

    if (fscanf(fd_in, "%lu", &posts_total) != 1){
        fprintf(stderr, "Invalid number of posts entered, stopping...\n");
        return NULL;
    }
    blog_t* new_blog = create_blog();
    if (new_blog == NULL){
        fprintf(stderr, "Failed to allocate memory for the blog\n");
        return NULL;
    }
    for (size_t i = 0; i < posts_total; ++i){
        post_t* buf_post = make_post(fd_in);
        if (add_post(new_blog, buf_post) != 0){
            delete_post(buf_post);
            delete_blog(new_blog);
            return NULL;
        }
        delete_post(buf_post);
    }
    
    fprintf(stdout, "Read blog data from file\n");
    return new_blog;
}

post_t* make_post(FILE* fd_in){
    if (fd_in == NULL) return NULL;
    char DATE_BUFFER[4+1+2+1+2+1];
    char TITLE_BUFFER[BUFFER_SIZE];
    char BODY_BUFFER[BUFFER_SIZE];
    // char *getline_buffer_title;
    // char *getline_buffer_date;
    // char *getline_buffer_body;
    size_t buf_size = 0;

    // if (getline(&getline_buffer_title, &buf_size, fd_in) == -1) return NULL;
    // if (getline(&getline_buffer_date, &buf_size, fd_in) == -1) return NULL;
    // if (getline(&getline_buffer_body, &buf_size, fd_in) == -1) return NULL;
    
    if (fgets(TITLE_BUFFER, BUFFER_SIZE, fd_in) == NULL) return NULL;
    if (fgets(DATE_BUFFER, 4+1+2+1+2+1, fd_in) == NULL) return NULL;
    if (fgets(BODY_BUFFER, BUFFER_SIZE, fd_in) == NULL) return NULL;

    TITLE_BUFFER[strlen(TITLE_BUFFER) - 1] = '\0';
    DATE_BUFFER[strlen(DATE_BUFFER) - 1] = '\0';
    BODY_BUFFER[strlen(BODY_BUFFER) - 1] = '\0';

    // create new empty post
    post_t* new_post = create_post(TITLE_BUFFER, BODY_BUFFER, DATE_BUFFER);
    if (new_post == NULL) return NULL;

    size_t n_tags = 0;
    size_t n_comments = 0;
    size_t n_votes = 0;

    // read hashtags
    if (fscanf(fd_in, "%lu", &n_tags) != 1){
        delete_post(new_post);
        return NULL;
    }
    for (size_t i = 0; i < n_tags; ++i){
        // if (getline(&getline_buffer_body, &buf_size, fd_in) == -1){
        if (fgets(BODY_BUFFER, BUFFER_SIZE, fd_in) == NULL){
            delete_post(new_post);
            return NULL;
        }
        BODY_BUFFER[strlen(BODY_BUFFER) -1 ] = '\0';
        if (add_tag(new_post, BODY_BUFFER) != 0){
            delete_post(new_post);
            return NULL;
        }
    }

    // read comments
    if (fscanf(fd_in, "%lu", &n_comments) != 1){
        delete_post(new_post);
        return NULL;
    }
    for (size_t i = 0; i < n_comments; ++i){
        // if (getline(&getline_buffer_body, &buf_size, fd_in) == -1){
        if (fgets(BODY_BUFFER, BUFFER_SIZE, fd_in) == NULL){
            delete_post(new_post);
            return NULL;
        }
        BODY_BUFFER[strlen(BODY_BUFFER) -1 ] = '\0';
        if (fgets(BODY_BUFFER, BUFFER_SIZE, fd_in) == NULL){
        // if (getline(&getline_buffer_date, &buf_size, fd_in) == -1){
            delete_post(new_post);
            return NULL;
        }
        comment_t* buf_comment = create_comment(DATE_BUFFER, BODY_BUFFER);
        if (add_comment(new_post, buf_comment) != 0){
            delete_comment(buf_comment);
            delete_post(new_post);
            return NULL;
        }
        delete_comment(buf_comment);
    }

    // read votes
    if (fscanf(fd_in, "%lu", &n_votes) != 1){
        delete_post(new_post);
        return NULL;
    }
    for (size_t i = 0; i < n_votes; ++i){
        if (fgets(DATE_BUFFER, 4+1+2+1+2+1, fd_in) == NULL){
        // if (getline(&getline_buffer_date, &buf_size, fd_in) == -1){
            delete_post(new_post);
            return NULL;
        }
        DATE_BUFFER[strlen(DATE_BUFFER) -1 ] = '\0';
        vote_t* buf_vote = create_vote(DATE_BUFFER);
        if (add_vote(new_post, buf_vote) != 0){
            delete_vote(buf_vote);
            delete_post(new_post);
            return NULL;
        }
        delete_vote(buf_vote);
    }

    return new_post;
}

int make_offset(FILE* fd_in, size_t *years, size_t *months, size_t *days){
    if (fd_in == NULL) return OPEN_FILE_ERROR;
    if (years == NULL) return EMPTY_PTR_ERROR;
    if (months == NULL) return EMPTY_PTR_ERROR;
    if (days == NULL) return EMPTY_PTR_ERROR;

    fprintf(stdout, "Enter time delta for search. Format is YYYY-MM-DD:");
    if (fscanf(fd_in, "%lu-%lu-%lu", years, months, days) != 3){
        fprintf(stdout, "Entered data is of invalid format, stopping work...\n");
        return FORMAT_ERROR;
    }
    return 0;
}

void free_used_resources(FILE* user_in_fd, FILE* blog_in_fd, blog_t* main_blog, blog_t* most_upvoted_blog){
    if (user_in_fd != NULL) fclose(user_in_fd);
    if (blog_in_fd != NULL) fclose(blog_in_fd);
    delete_blog(main_blog);
    delete_blog(most_upvoted_blog);
    return;
}

