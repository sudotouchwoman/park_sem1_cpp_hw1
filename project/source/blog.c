#include "blog.h"

#include <string.h>

blog_t* create_blog(){
    blog_t* new_blog = (blog_t*)malloc(sizeof(blog_t));
    if (new_blog == NULL) return NULL;

    new_blog->posts = NULL;
    return new_blog;
}

blog_t* copy_blog(const blog_t* reference){
    if (reference == NULL) return NULL;
    
    blog_t* blog_copy = (blog_t*)malloc(sizeof(blog_t));
    if (blog_copy == NULL) return NULL;
    blog_copy->posts = NULL;

    if (reference->posts != NULL && reference->c_posts > 0){
        post_t** posts_copy = (post_t**)malloc(sizeof(post_t*) * (reference->c_posts));
        if (posts_copy == NULL){
            delete_blog(blog_copy);
            return NULL;
        }
        for (size_t i = 0; i < reference->c_posts; ++i){
            if (add_post(blog_copy, reference->posts[i]) != 0){
                delete_blog(blog_copy);
                return NULL;
            }
        }
        // blog_copy->n_posts = reference->n_posts;
        // blog_copy->c_posts = reference->c_posts;
    }
    else{
        blog_copy->n_posts = 0;
        blog_copy->c_posts = 0;
        blog_copy->posts = NULL;
    }
    return blog_copy;
}

int delete_blog(blog_t* to_remove){
    if (to_remove == NULL) return BLOG_DELETE_ERROR;
    if (to_remove->posts != NULL){
        for (size_t i = 0; i < to_remove->c_posts; ++i){
            delete_post(to_remove->posts[i]);
        }
        free(to_remove->posts);
    }
    return 0;
}

static int resize_posts(post_t** old_posts, size_t* capacity){
    if (old_posts == NULL) return BLOG_RESIZE_ERROR;
    if (capacity == NULL) return BLOG_RESIZE_ERROR;

    size_t new_capacity = *capacity ? *capacity : 1;

    post_t** resized_posts = realloc(old_posts, sizeof(post_t*) * (new_capacity * 2));
    if (resized_posts == NULL) return BLOG_RESIZE_ERROR;

    *capacity = (new_capacity * 2);
    old_posts = resized_posts;
    return 0;
}

int add_post(blog_t* where, const post_t* post_to_add){
    if (where == NULL) return BLOG_APPEND_ERROR;
    if (post_to_add == NULL) return BLOG_APPEND_ERROR;

    post_t* new_post = copy_post(post_to_add);
    if (new_post == NULL) return BLOG_APPEND_ERROR;

    if (where->n_posts == where->c_posts){
        if (resize_posts(where->posts, &where->c_posts) != 0){
            delete_post(new_post);
            return BLOG_APPEND_ERROR;
        }
    }
    where->posts[where->n_posts++] = new_post;
    return 0;
}
