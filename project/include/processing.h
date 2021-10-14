#ifndef _PROCESSING_H
#define _PROCESSING_H

#include <stdio.h>

#include "blog.h"
#include "post.h"

typedef struct time_period{
    size_t years;
    size_t months;
    size_t days;
} timedelta_t;

timedelta_t create_timedelta(size_t, size_t, size_t);

int lies_in_offset(const char*, const char*, const timedelta_t);
size_t find_reaction_of_post(const post_t*, const timedelta_t);
size_t find_max_reaction(const blog_t*, const timedelta_t);
blog_t* select_most_hyped(const blog_t*, const timedelta_t);
int print_most_hyped(FILE*, const blog_t*);
int print_post_contents(FILE*, const post_t*);

#endif // _PROCESSING_H
