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

    size_t target_timedelta = (target_day - zero_day) + 30 * (target_month - zero_month) + 365 * (target_year - zero_year);
    size_t timedelta_in_days = days + 30 * months + 365 * years;

    return target_timedelta < timedelta_in_days;
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

blog_t* select_most_hyped(const blog_t* target_blog, size_t time_period[3]){
    if (target_blog == NULL) return NULL;
    if (target_blog->posts == NULL) return NULL;
    if (time_period == NULL) return NULL;

    size_t max_reaction = find_max_reaction(target_blog, time_period);
    if (!max_reaction) return NULL;

    blog_t* most_hyped = create_blog();
    if (most_hyped == NULL) return NULL;

    for (size_t i = 0; i < target_blog->n_posts; ++i){
        if (find_reaction_of_post(target_blog->posts[i], time_period) < max_reaction) continue;
        if (add_post(most_hyped, target_blog->posts[i]) != 0){
            delete_blog(most_hyped);
            return NULL;
        }
    }

    return most_hyped;
}

int print_most_hyped(FILE* fd_out, const blog_t* blog_to_print){
    if (fd_out == NULL) return 1;
    if (blog_to_print == NULL) return 1;
    for (size_t i = 0; i < blog_to_print->n_posts; ++i) print_post_contents(fd_out, blog_to_print->posts[i]);
    return 0;
}

int print_post_contents(FILE* fd_out, const post_t* post_to_print){
    if (fd_out == NULL) return 1;
    if (post_to_print == NULL) return 1;
    // if (post_to_print->title == NULL) return 1;
    // if (post_to_print->post_date == NULL) return 1;
    // if (post_to_print->body == NULL) return 1;
    // if (post_to_print->c_tags == 0) return 1;
    // if (post_to_print->c_comments == 0) return 1;
    // if (post_to_print->c_votes == 0) return 1;

    fprintf(fd_out, "Post contents:\n");
    fprintf(fd_out, "%s (created on %s)\n", post_to_print->title, post_to_print->post_date);
    fprintf(fd_out, "%s", post_to_print->body);
    fprintf(fd_out, "\nTags: ");
    for (size_t i = 0; i < post_to_print->n_tags; ++i) fprintf(fd_out, "%s ", post_to_print->tags[i]);
    fprintf(fd_out, "\nComments:\n");
    for (size_t i = 0; i < post_to_print->n_comments; ++i)
        fprintf(
            fd_out,
            "%s (on %s)\n",
            post_to_print->comments[i]->text_comment,
            post_to_print->comments[i]->date_comment
            );
    fprintf(fd_out, "Votes:\n"); 
    for (size_t i = 0; i < post_to_print->n_votes; ++i)
        fprintf(
            fd_out,
            "%s\n",
            post_to_print->votes[i]->date_vote
            );
    fprintf(fd_out, "End of post\n");
    return 0;
}
