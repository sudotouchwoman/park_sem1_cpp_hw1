#ifndef _LOADER_H
#define _LOADER_H

#include <stdlib.h>
#include <stdio.h>

#include "blog.h"
#include "post.h"
#include "processing.h"

#define BUFFER_SIZE 256
// each iso date string is of format YYYY-MM-DD + '\0'
#define ISO_DATE_SIZE 4+1+2+1+2+1

enum input_errors{
    OPEN_FILE_ERROR = 1,
    EMPTY_PTR_ERROR,
    EMPTY_POST_PTR,
    FORMAT_ERROR
};

blog_t* make_blog(FILE*);
post_t* make_post(FILE*);
int make_time_period(FILE*, timedelta_t *);
void free_used_resources(FILE*, FILE*, blog_t*, blog_t*);

#endif // _LOADER_H
