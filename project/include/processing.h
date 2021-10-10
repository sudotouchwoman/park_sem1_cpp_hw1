#ifndef _PROCESSING_H
#define _PROCESSING_H

#include "blog.h"
#include "post.h"

int lies_in_offset(const char*, const char*, size_t years, size_t months, size_t days);
size_t find_reaction_of_post(const post_t*, size_t time_period[3]);
size_t find_max_reaction(const blog_t*, size_t time_period[3]);
blog_t* select_most_hyped(blog_t*, size_t time_period[3]);

#endif // _PROCESSING_H
