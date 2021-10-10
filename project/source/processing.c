#include "processing.h"

#include <stdio.h>

int lies_in_offset(const char* reference, const char* date_to_check, size_t years, size_t months, size_t days){
    if (reference == NULL) return 0;
    if (date_to_check == NULL) return 0;

    size_t zero_day = 0;
    size_t zero_month = 0;
    size_t zero_year = 0;

    size_t target_day = 0;
    size_t target_month = 0;
    size_t target_year = 0;

    sscanf(reference, "%lu-%lu-%lu", &zero_year, &zero_month, &zero_day);
    sscanf(date_to_check, "%lu-%lu-%lu", &target_year, &target_month, &target_day);

    if ( ((target_day - zero_day) < days) && ((target_month - zero_month) < months) && ((target_year - zero_year) < years) ) return 1;
    return 0;
}

size_t find_reaction_of_post(const post_t* post_to_check, size_t time_period[3]){
    if (post_to_check == NULL) return 0;
    if (post_to_check->comments == NULL) return 0;
    if (post_to_check->votes == NULL) return 0;

    size_t reaction = 0;

    for (size_t i = 0; i < post_to_check->n_comments; ++i){
        if (lies_in_offset(
            post_to_check->post_date,
            post_to_check->comments[i]->date_comment,
            time_period[0],
            time_period[1],
            time_period[2]))
            ++reaction;
    }
    for (size_t i = 0; i < post_to_check->n_votes; ++i){
        if (lies_in_offset(
            post_to_check->post_date,
            post_to_check->votes[i]->date_vote,
            time_period[0],
            time_period[1],
            time_period[2]))
            ++reaction;
    }
    return reaction;
}

size_t find_max_reaction(const blog_t* target_blog, size_t time_period[3]){
    if (target_blog == NULL) return 0;
    if (target_blog->posts == NULL) return 0;

    size_t max_reaction = 0;

    for (size_t i = 0; i < target_blog->n_posts; ++i){
        
        size_t current_reaction = find_reaction_of_post(target_blog->posts[i], time_period);
        if (current_reaction > max_reaction) max_reaction = current_reaction;
    }
    return max_reaction;
}

blog_t* select_most_hyped(blog_t* target_blog, size_t time_period[3]){
    if (target_blog == NULL) return NULL;
    if (target_blog->posts == NULL) return NULL;

    size_t max_reaction = find_max_reaction(target_blog, time_period);
    if (!max_reaction) return NULL;

    blog_t* most_hyped = create_blog();
    if (most_hyped == NULL) return NULL;

    for (size_t i = 0; i < target_blog->n_posts; ++i){
        if (!(target_blog->posts[i]->n_comments + target_blog->posts[i]->n_votes == max_reaction)) continue;
        if (add_post(most_hyped, target_blog->posts[i]) != 0){
            delete_blog(most_hyped);
            return NULL;
        }
    }

    return most_hyped;
}
