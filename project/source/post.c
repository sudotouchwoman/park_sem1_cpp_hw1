#include "post.h"

#include <string.h>

post_t* create_post(const char* title, const char* body, const char* post_date){
    if (title == NULL) return NULL;
    if (body == NULL) return NULL;
    if (post_date == NULL) return NULL;

    char* new_body = (char*)malloc(sizeof(char) * (strlen(body) + 1));
    if (new_body == NULL) return NULL;

    char* new_title = (char*)malloc(sizeof(char) * (strlen(title) + 1));
    if (new_title == NULL){
        free(new_body);
        return NULL;
    }

    char* new_post_date = (char*)malloc(sizeof(char) * (strlen(post_date) + 1));
    if (new_post_date == NULL){
        free(new_body);
        free(new_title);
        return NULL;
    }

    post_t* new_post = (post_t*)malloc(sizeof(post_t));
    if (new_post == NULL){
        free(new_body);
        free(new_title);
        free(new_post_date);
        return NULL;
    }

    strcpy(new_body, body);
    strcpy(new_title, title);
    strcpy(new_post_date, post_date);

    new_post->title = new_title;
    new_post->body = new_body;
    new_post->post_date = new_post_date;

    new_post->n_comments = 0;
    new_post->n_votes = 0;
    new_post->n_tags = 0;

    new_post->c_comments = 0;
    new_post->c_votes = 0;
    new_post->c_tags = 0;

    new_post->tags = NULL;
    new_post->comments = NULL;
    new_post->votes = NULL;

    return new_post;
}

post_t* copy_post(const post_t* reference){
    if (reference == NULL) return NULL;

    post_t* post_copy = create_post(reference->title, reference->body, reference->post_date);
    if (post_copy == NULL) return NULL;
    post_copy->comments = NULL;
    post_copy->tags = NULL;
    post_copy->votes = NULL;

    if (reference->tags != NULL && reference->n_tags > 0){
        for (size_t i = 0; i < reference->n_tags; ++i){
            if (add_tag(post_copy, reference->tags[i]) != 0){
                delete_post(post_copy);
                return NULL;
            }
        }
        // post_copy->n_tags = reference->n_tags;
        // post_copy->c_tags = reference->c_tags;
    }
    if (reference->votes != NULL && reference->n_votes > 0){
        for (size_t i = 0; i < reference->n_votes; ++i){
            if (add_vote(post_copy, reference->votes[i]) != 0){
                delete_post(post_copy);
                return NULL;
            }
        }
        // post_copy->n_votes = reference->n_votes;
        // post_copy->c_votes = reference->c_votes;
    }
    
    if (reference->comments != NULL && reference->n_comments > 0){
        for (size_t i = 0; i < reference->n_comments; ++i){
            if (add_comment(post_copy, reference->comments[i]) != 0){
                delete_post(post_copy);
                return NULL;
            }
        }
        // post_copy->n_comments = reference->n_comments;
        // post_copy->c_comments = reference->c_comments;
    }
    
    return post_copy;
}

int delete_post(post_t* to_remove){
    if (to_remove == NULL) return POST_DELETE_ERROR;

    if (to_remove->title != NULL) free(to_remove->title);
    if (to_remove->body != NULL) free(to_remove->body);
    if (to_remove->post_date != NULL) free(to_remove->post_date);
    if (to_remove->comments != NULL){
        for (size_t i = 0; i < to_remove->c_comments; ++i) delete_comment(to_remove->comments[i]);
        free(to_remove->comments);
    }
    if (to_remove->tags != NULL){
        for (size_t i = 0; i < to_remove->c_tags; ++i) free(to_remove->tags[i]);
        free(to_remove->tags);
    }
    if (to_remove->votes != NULL){
        for (size_t i = 0; i < to_remove->c_votes; ++i) delete_vote(to_remove->votes[i]);
        free(to_remove->votes);
    }
    free(to_remove);
    return 0;
}

static int resize_tags(char*** old_tags, size_t* capacity){
    if (old_tags == NULL) return POST_RESIZE_ERROR;
    if (capacity == NULL) return POST_RESIZE_ERROR;

    size_t new_capacity = *capacity ? *capacity : 1;

    char** resized_tags = realloc(*old_tags, sizeof(char*) * (new_capacity * 2));
    if (resized_tags == NULL) return POST_RESIZE_ERROR;

    *capacity = (new_capacity * 2);
    *old_tags = resized_tags;
    return 0;
}

static int resize_votes(vote_t*** old_votes, size_t* capacity){
    if (old_votes == NULL) return POST_RESIZE_ERROR;
    if (capacity == NULL) return POST_RESIZE_ERROR;

    size_t new_capacity = *capacity ? *capacity : 1;

    vote_t** resized_votes = realloc(*old_votes, sizeof(vote_t*) * (new_capacity * 2));
    if (resized_votes == NULL) return POST_RESIZE_ERROR;
    // for (size_t i = 0; i < new_capacity*2; ++i) resized_votes[i] = NULL;

    *capacity = (new_capacity * 2);
    *old_votes = resized_votes;
    return 0;
}

static int resize_comments(comment_t*** old_comments, size_t* capacity){
    if (old_comments == NULL) return POST_RESIZE_ERROR;
    if (capacity == NULL) return POST_RESIZE_ERROR;

    size_t new_capacity = *capacity ? *capacity : 1;

    comment_t** resized_comments = realloc(*old_comments, sizeof(comment_t*) * (new_capacity * 2));
    if (resized_comments == NULL) return POST_RESIZE_ERROR;
    // for (size_t i = 0; i < new_capacity*2; ++i) resized_comments[i] = NULL;

    *capacity = (new_capacity * 2);
    *old_comments = resized_comments;
    return 0;
}

static int init_tag(post_t* where){
    if (where == NULL) return POST_APPEND_ERROR;
    char** new_tag = (char**)malloc(sizeof(char*));
    if (new_tag == NULL) return POST_APPEND_ERROR;
    where->tags = new_tag;
    where->c_tags = 1;
    return 0;
}

static int init_comment(post_t* where){
    if (where == NULL) return POST_APPEND_ERROR;
    comment_t** new_comment = (comment_t**)malloc(sizeof(comment_t*));
    if (new_comment == NULL) return POST_APPEND_ERROR;
    where->comments = new_comment;
    where->c_comments = 1;
    return 0;
}

static int init_vote(post_t* where){
    if (where == NULL) return POST_APPEND_ERROR;
    vote_t** new_vote = (vote_t**)malloc(sizeof(vote_t*));
    if (new_vote == NULL) return POST_APPEND_ERROR;
    where->votes = new_vote;
    where->c_votes = 1;
    return 0;
}

int add_tag(post_t* where, const char* tag){
    if (tag == NULL) return POST_APPEND_ERROR;
    if (where == NULL) return POST_APPEND_ERROR;
    
    char* new_tag = (char*)malloc(sizeof(char) * (strlen(tag) + 1));
    if (new_tag == NULL) return POST_APPEND_ERROR;

    strcpy(new_tag, tag);
    if (where->c_tags == 0){
        if (init_tag(where) != 0){
            free(new_tag);
            return POST_APPEND_ERROR;
        }
        where->tags[where->n_tags++] = new_tag;
        return 0;
    }
    if (where->n_tags == where->c_tags){
        if (resize_tags(&where->tags, &where->c_tags) != 0){
            free(new_tag);
            return POST_APPEND_ERROR;
        }
    }
    where->tags[where->n_tags++] = new_tag;
    return 0;
}

int add_comment(post_t* where, const comment_t* comment){
    if (comment == NULL) return POST_APPEND_ERROR;
    if (where == NULL) return POST_APPEND_ERROR;

    comment_t* new_comment = copy_comment(comment);
    if (new_comment == NULL) return POST_APPEND_ERROR;

    if (where->c_comments == 0){
        if (init_comment(where) != 0){
            free(new_comment);
            return POST_APPEND_ERROR;
        }
        where->comments[where->n_comments++] = new_comment;
        return 0;
    }

    if (where->n_comments == where->c_comments){
        if (resize_comments(&where->comments, &where->c_comments) != 0){
            delete_comment(new_comment);
            return POST_APPEND_ERROR;
        }
    }
    where->comments[where->n_comments++] = new_comment;
    return 0;
}

int add_vote(post_t* where, const vote_t* vote){
    if (vote == NULL) return POST_APPEND_ERROR;
    if (where == NULL) return POST_APPEND_ERROR;

    vote_t* new_vote = copy_vote(vote);
    if (new_vote == NULL) return POST_APPEND_ERROR;

    if (where->c_votes == 0){
        if (init_vote(where) != 0){
            free(new_vote);
            return POST_APPEND_ERROR;
        }
        where->votes[where->n_votes++] = new_vote;
        return 0;
    }

    if (where->n_votes == where->c_votes){
        if (resize_votes(&where->votes, &where->c_votes) != 0){
            delete_vote(new_vote);
            return POST_APPEND_ERROR;
        }
    }
    where->votes[where->n_votes++] = new_vote;
    return 0;
}
