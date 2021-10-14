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
    // read blog data from the specified file
    // the first line should contain number of posts
    // call make_post() and abort if anything goes wrong in the process
    if (fd_in == NULL) return NULL;
    fprintf(stdout, "Reads blog contents from specified file\n");
    int posts_total = 0;

    if (fscanf(fd_in, "Total posts: %d", &posts_total) != 1){
        fprintf(stderr, "Invalid number of posts entered, stopping...\n");
        return NULL;
    }
    if (posts_total <= 0){
        fprintf(stderr, "Invalid number of posts entered, stopping...\n");
        return NULL;
    }
    // have to manually flush sscanf buffer to read strings containing spaces
    flush_sscanf_buffer(fd_in);
    // create empty blog instance, without memory overhead
    blog_t* new_blog = create_blog();
    if (new_blog == NULL){
        fprintf(stderr, "Failed to allocate memory for the blog\n");
        return NULL;
    }
    for (size_t i = 0; i < posts_total; ++i){
        // try to collect data and create posts (posts_total times)
        // if add_post returned non-zero, buf_post might be NULL (i.e. reading it contents aboorted)
        // thus, free all memory and return 
        post_t* buf_post = make_post(fd_in);
        if (add_post(new_blog, buf_post) != 0){
            delete_post(buf_post);
            delete_blog(new_blog);
            fprintf(stderr, "Post data collection aborted\n");
            return NULL;
        }
        delete_post(buf_post);
    }
    
    fprintf(stdout, "Succesfully read blog data from file\n");
    return new_blog;
}

post_t* make_post(FILE* fd_in){
    // parse contents from given file and create post
    if (fd_in == NULL) return NULL;
    char DATE_BUFFER[ISO_DATE_SIZE];
    char TITLE_BUFFER[BUFFER_SIZE];
    char BODY_BUFFER[BUFFER_SIZE];
    char buffer_flusher = '\0';
    size_t buf_size = 0;
    int scanf_status = 0;

    // read general info about post from specified file
    // abort if anything goes wrong (EOF or no string)
    scanf_status = fscanf(fd_in, "Title: %[^\n]s", TITLE_BUFFER);
    if (scanf_status == 0 || scanf_status == EOF) return NULL;
    flush_sscanf_buffer(fd_in);
    scanf_status = fscanf(fd_in, "Created on %[^\n]s", DATE_BUFFER);
    if (scanf_status == 0 || scanf_status == EOF) return NULL;
    flush_sscanf_buffer(fd_in);
    scanf_status = fscanf(fd_in, "%[^\n]s", BODY_BUFFER);
    if (scanf_status == 0 || scanf_status == EOF) return NULL;
    flush_sscanf_buffer(fd_in);

    // create new empty post
    // post "constructor" takes 3 arguments to initialize: title, date and body
    // there exist posts without tags, comments and votes, eh?
    post_t* new_post = create_post(TITLE_BUFFER, BODY_BUFFER, DATE_BUFFER);
    if (new_post == NULL) return NULL;

    size_t n_tags = 0;
    size_t n_comments = 0;
    size_t n_votes = 0;

    // read hashtags and append to the post
    // each hashtag takes a separate line
    if (fscanf(fd_in, "Tags: %lu", &n_tags) != 1){
        delete_post(new_post);
        return NULL;
    }
    flush_sscanf_buffer(fd_in);
    for (size_t i = 0; i < n_tags; ++i){
        scanf_status = fscanf(fd_in, "%[^\n]s", BODY_BUFFER);
        if (scanf_status == 0 || scanf_status == EOF){
            delete_post(new_post);
            return NULL;
        }
        flush_sscanf_buffer(fd_in);
        if (add_tag(new_post, BODY_BUFFER) != 0){
            delete_post(new_post);
            return NULL;
        }
    }

    // read comments and append to the post
    // comments take 2 lines: 1 line of text and 1 line with iso-date
    if (fscanf(fd_in, "Comments: %lu", &n_comments) != 1){
        delete_post(new_post);
        return NULL;
    }
    flush_sscanf_buffer(fd_in);
    for (size_t i = 0; i < n_comments; ++i){
        scanf_status = fscanf(fd_in, "%[^\n]s", DATE_BUFFER);
        if (scanf_status == 0 || scanf_status == EOF){
            delete_post(new_post);
            return NULL;
        }
        flush_sscanf_buffer(fd_in);
        
        scanf_status = fscanf(fd_in, "%[^\n]s", BODY_BUFFER);
        if (scanf_status == 0 || scanf_status == EOF){
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

    // read votes and append to the post
    // votes take 1 line with iso-date
    if (fscanf(fd_in, "Votes: %lu", &n_votes) != 1){
        delete_post(new_post);
        return NULL;
    }
    flush_sscanf_buffer(fd_in);
    for (size_t i = 0; i < n_votes; ++i){
        scanf_status = fscanf(fd_in, "%[^\n]s", DATE_BUFFER);
        if (scanf_status == 0 || scanf_status == EOF){
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

int make_time_period(FILE* fd_in, timedelta_t *offset){
    // read time delta from specified file
    // time delta is a sequence of numbers representing offset (days, months and years)
    // e.g., to recreate given task, we should write 0;1;0; (0 years, 1 month, 0 days)
    if (fd_in == NULL) return OPEN_FILE_ERROR;
    if (offset == NULL) return EMPTY_PTR_ERROR;

    fprintf(stdout, "Enter time delta for search. Format is \"Y;M;D;\" (Years, months, days):\n");
    if (fscanf(fd_in, "%lu;%lu;%lu;", &offset->years, &offset->months, &offset->days) != 3){
        fprintf(stdout, "Entered data is of invalid format, stopping work...\n");
        return FORMAT_ERROR;
    }
    fprintf(stdout, "Collected data: %lu years, %lu months, %lu days\n", offset->years, offset->months, offset->days);
    return 0;
}

void free_used_resources(FILE* user_in_fd, FILE* blog_in_fd, blog_t* main_blog, blog_t* most_upvoted_blog){
    // close file descriptors
    // and free all the allocated memory via delete_blog()
    if (user_in_fd != NULL) fclose(user_in_fd);
    if (blog_in_fd != NULL) fclose(blog_in_fd);
    delete_blog(main_blog);
    delete_blog(most_upvoted_blog);
    return;
}

