#include "reactions.h"

#include <string.h>

// routines for comments and votes
// are used to create, copy and safely delete these items

comment_t* create_comment(const char *date, const char *text){
    if (date == NULL) return NULL;
    if (text == NULL) return NULL;

    char* new_date = (char*)malloc(sizeof(char) * (strlen(date) + 1));
    if (new_date == NULL) return NULL;

    char* new_text = (char*)malloc(sizeof(char) * (strlen(text) + 1));
    if (new_text == NULL){
        free(new_date);
        return NULL;
    }

    comment_t* new_comment = (comment_t*)malloc(sizeof(comment_t));
    if (new_comment ==  NULL){
        free(new_date);
        free(new_text);
        return NULL;
    }

    strncpy(new_date, date, strlen(date) + 1);
    strncpy(new_text, text, strlen(text) + 1);
    new_comment->date_comment = new_date;
    new_comment->text_comment = new_text;
    
    return new_comment;
}

comment_t* copy_comment(const comment_t* reference){
    if (reference == NULL) return NULL;
    return create_comment(reference->date_comment, reference->text_comment);
}

int delete_comment(comment_t* to_remove){
    if (to_remove == NULL) return EMPTY_COMMENT;
    if (to_remove->date_comment != NULL) { free(to_remove->date_comment); to_remove->date_comment = NULL; }
    if (to_remove->text_comment != NULL) { free(to_remove->text_comment); to_remove->text_comment = NULL; }
    free(to_remove);
    to_remove = NULL;
    return EXIT_SUCCESS;
}

vote_t* create_vote(const char *date){
    if (date == NULL) return NULL;

    char* new_date = (char*)malloc(sizeof(char) * (strlen(date) + 1));
    if (new_date == NULL) return NULL;

    vote_t* new_vote = (vote_t*)malloc(sizeof(vote_t));
    if (new_vote == NULL){
        free(new_date);
        return NULL;
    }

    strncpy(new_date, date, strlen(date) + 1);
    new_vote->date_vote = new_date;

    return new_vote;
}

vote_t* copy_vote(const vote_t* reference){
    if (reference == NULL) return NULL;
    return create_vote(reference->date_vote);
}

int delete_vote(vote_t* to_remove){
    if (to_remove == NULL) return EMPTY_VOTE;
    if (to_remove->date_vote != NULL) { free(to_remove->date_vote); to_remove->date_vote = NULL; }
    free(to_remove);
    to_remove = NULL;
    return EXIT_SUCCESS;
}
