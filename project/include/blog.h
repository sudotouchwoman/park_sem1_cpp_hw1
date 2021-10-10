#ifndef _BLOG_H
#define _BLOG_H

#include <stdlib.h>
#include "post.h"

typedef struct post_collection{
    post_t** posts;
    size_t n_posts;
    size_t c_posts;
} blog_t;

enum blog_errors{
    BLOG_RESIZE_ERROR = 1,
    BLOG_APPEND_ERROR,
    BLOG_DELETE_ERROR
};

blog_t* create_blog();
blog_t* copy_blog(const blog_t*);
int delete_blog(blog_t*);
int add_post(blog_t*, const post_t*);

#endif // _BLOG_H
