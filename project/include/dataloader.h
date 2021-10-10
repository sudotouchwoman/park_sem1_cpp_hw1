#ifndef _LOADER_H
#define _LOADER_H

#include <stdlib.h>
#include <stdio.h>

#include "blog.h"
#include "post.h"

enum input_errors{
    OPEN_FILE_ERROR = 1,
    EMPTY_PTR_ERROE
};

blog_t* make_blog(FILE*);
post_t* make_post(FILE*);
int make_offset(FILE*, size_t *years, size_t *months, size_t *days);

#endif // _LOADER_H
