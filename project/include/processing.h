#ifndef _PROCESSING_H
#define _PROCESSING_H

#include <stdio.h>

#include "blog.h"
#include "post.h"

int lies_in_offset(const char*, const char*, size_t years, size_t months, size_t days);
size_t find_reaction_of_post(const post_t*, size_t time_period[3]);
size_t find_max_reaction(const blog_t*, size_t time_period[3]);
blog_t* select_most_hyped(const blog_t*, size_t time_period[3]);
int print_most_hyped(FILE*, const blog_t*);
int print_post_contents(FILE*, const post_t*);

#endif // _PROCESSING_H
