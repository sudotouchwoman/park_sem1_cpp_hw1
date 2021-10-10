#ifndef _POST_H
#define _POST_H

#include <stdlib.h>
#include "reactions.h"

typedef struct blog_element{
    char* title;
    char* body;
    char* post_date;

    char** tags;
    vote_t** votes;
    comment_t** comments;

    size_t n_tags;
    size_t n_votes;
    size_t n_comments;

    size_t c_tags;
    size_t c_votes;
    size_t c_comments;
} post_t;

enum post_errors{
    POST_RESIZE_ERROR = 1,
    POST_APPEND_ERROR,
    POST_DELETE_ERROR
};

post_t* create_post(const char* title, const char* body, const char* post_date);
post_t* copy_post(const post_t*);
int delete_post(post_t*);

int add_tag(post_t*, const char*);
int add_comment(post_t*, const comment_t*);
int add_vote(post_t*, const vote_t*);

#endif // _POST_H
