#ifndef _REACTIONS_H
#define _REACTIONS_H

#include <stdlib.h>

enum reactions_errors{
    EMPTY_DATE = 1,
    EMPTY_TEXT,
    EMPTY_COMMENT,
    EMPTY_VOTE
};

typedef struct blog_post_comment{
    char* date_comment;
    char* text_comment;
} comment_t;

typedef struct blog_post_vote{
    char* date_vote;
} vote_t;

comment_t* create_comment(const char *date, const char *text);
comment_t* copy_comment(const comment_t*);
int delete_comment(comment_t*);

vote_t* create_vote(const char *date);
vote_t* copy_vote(const vote_t*);
int delete_vote(vote_t*);

#endif // _REACTIONS_H
