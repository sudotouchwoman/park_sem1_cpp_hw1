#ifndef _PROCESSING_H
#define _PROCESSING_H

#include "blog.h"
#include "post.h"

int lies_in_offset(const char*, const char*);
size_t find_max_reaction(blog_t*);
blog_t* select_most_hyped(blog_t*);

#endif // _PROCESSING_H
