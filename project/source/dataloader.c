#include "dataloader.h"
#include <string.h>

static void flush_sscanf_buffer(FILE* fd){
    // extract one symbol from file to flush scanf buffer
    // needed to read symbols one by one
    if (fd == NULL) return;
    char buf = '\0';
    fscanf(fd, "%c", &buf);
    return;
}

blog_t* make_blog(FILE* fd_in){
    if (fd_in == NULL) return NULL;
    fprintf(stdout, "Reads blog contents from specified file\n");
    int posts_total = 0;

    if (fscanf(fd_in, "%d", &posts_total) != 1){
        fprintf(stderr, "Invalid number of posts entered, stopping...\n");
        return NULL;
    }
    if (posts_total <= 0){
        fprintf(stderr, "Invalid number of posts entered, stopping...\n");
        return NULL;
    }
    flush_sscanf_buffer(fd_in);
    blog_t* new_blog = create_blog();
    if (new_blog == NULL){
        fprintf(stderr, "Failed to allocate memory for the blog\n");
        return NULL;
    }
    for (size_t i = 0; i < posts_total; ++i){
        // fprintf(stderr, "Checkpoint: before make_post()\n");
        post_t* buf_post = make_post(fd_in);
        if (add_post(new_blog, buf_post) != 0){
            delete_post(buf_post);
            delete_blog(new_blog);
            return NULL;
        }
        // fprintf(stderr, "Checkpoint: before delete_post()\n");
        delete_post(buf_post);
        // fprintf(stderr, "Checkpoint: after delete_post()\n");
        // turns out some errors were coming from that buf_post above
        // how can realloc'ed memory (pointers) not be initialized?
        // however, there are no more leaks at all which is pleasing
        // needs further testing and deploying!
    }
    
    fprintf(stdout, "Succesfully read blog data from file\n");
    return new_blog;
}

post_t* make_post(FILE* fd_in){
    if (fd_in == NULL) return NULL;
    char DATE_BUFFER[4+1+2+1+2+1];
    char TITLE_BUFFER[BUFFER_SIZE];
    char BODY_BUFFER[BUFFER_SIZE];
    char buffer_flusher = '\0';
    size_t buf_size = 0;

    if (fscanf(fd_in, "%[^\n]s", TITLE_BUFFER) == 0) return NULL;
    flush_sscanf_buffer(fd_in);
    if (fscanf(fd_in, "%[^\n]s", DATE_BUFFER) == 0) return NULL;
    flush_sscanf_buffer(fd_in);
    if (fscanf(fd_in, "%[^\n]s", BODY_BUFFER) == 0) return NULL;
    flush_sscanf_buffer(fd_in);

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
    flush_sscanf_buffer(fd_in);
    for (size_t i = 0; i < n_tags; ++i){
        if (fscanf(fd_in, "%[^\n]s", BODY_BUFFER) == 0){
            delete_post(new_post);
            return NULL;
        }
        flush_sscanf_buffer(fd_in);
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
    flush_sscanf_buffer(fd_in);
    for (size_t i = 0; i < n_comments; ++i){
        if (fscanf(fd_in, "%[^\n]s", DATE_BUFFER) == 0){
            delete_post(new_post);
            return NULL;
        }
        flush_sscanf_buffer(fd_in);
        if (fscanf(fd_in, "%[^\n]s", BODY_BUFFER) == 0){
            delete_post(new_post);
            return NULL;
        }
        flush_sscanf_buffer(fd_in);
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
    flush_sscanf_buffer(fd_in);
    for (size_t i = 0; i < n_votes; ++i){
        if (fscanf(fd_in, "%[^\n]s", DATE_BUFFER) == 0){
            delete_post(new_post);
            return NULL;
        }
        flush_sscanf_buffer(fd_in);
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

int make_time_period(FILE* fd_in, size_t *years, size_t *months, size_t *days){
    if (fd_in == NULL) return OPEN_FILE_ERROR;
    if (years == NULL) return EMPTY_PTR_ERROR;
    if (months == NULL) return EMPTY_PTR_ERROR;
    if (days == NULL) return EMPTY_PTR_ERROR;

    fprintf(stdout, "Enter time delta for search. Format is \"Y;M;D;\" (Years, months, days):\n");
    if (fscanf(fd_in, "%lu;%lu;%lu;", years, months, days) != 3){
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

