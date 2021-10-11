#ifndef _LOADER_H
#define _LOADER_H

#include <stdlib.h>
#include <stdio.h>

#include "blog.h"
#include "post.h"

#define BUFFER_SIZE 256

enum input_errors{
    OPEN_FILE_ERROR = 1,
    EMPTY_PTR_ERROR,
    EMPTY_POST_PTR,
    FORMAT_ERROR
};

blog_t* make_blog(FILE*);
post_t* make_post(FILE*);
int make_time_period(FILE*, size_t *years, size_t *months, size_t *days);
void free_used_resources(FILE*, FILE*, blog_t*, blog_t*);

#endif // _LOADER_H
