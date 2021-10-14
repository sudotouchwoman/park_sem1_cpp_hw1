#include "gtest/gtest.h"

extern "C"{
    #include "processing.h"
    #include "dataloader.h"
}

#define POST_PATH "../data/post_ok.txt"
#define BLOG_PATH "../data/user_blog_ok.txt"

TEST(ProcessingTest, lies_in_offset){
    const timedelta_t offset_3m = create_timedelta(0, 3, 0);

    EXPECT_EQ(lies_in_offset("2021-01-01", "2021-01-30", offset_3m), 1);
    EXPECT_EQ(lies_in_offset("2021-01-01", "2021-05-01", offset_3m), 0);
    EXPECT_EQ(lies_in_offset("2021-01-01", "2022-01-30", offset_3m), 0);

    const timedelta_t offset_15d = create_timedelta(0, 0, 15);
    EXPECT_EQ(lies_in_offset("2021-05-01", "2021-05-30", offset_15d), 0);
    EXPECT_EQ(lies_in_offset("2021-05-01", "2021-01-30", offset_15d), 0);
    EXPECT_EQ(lies_in_offset("2021-05-01", "2022-05-10", offset_15d), 0);
    EXPECT_EQ(lies_in_offset("2021-05-01", "2021-05-10", offset_15d), 1);
}

TEST(ProcessingTest, find_reaction_of_post){
    FILE *fd = fopen(POST_PATH, "r");
    ASSERT_NE(fd, nullptr);

    post_t *sample_post = make_post(fd);
    ASSERT_NE(sample_post, nullptr);

    const timedelta_t month_period = create_timedelta(0, 1, 0);
    const timedelta_t two_months = create_timedelta(0, 2, 0);

    EXPECT_EQ(find_reaction_of_post(sample_post, month_period), 2);
    EXPECT_EQ(find_reaction_of_post(sample_post, two_months), 3);

    delete_post(sample_post);    
    fclose(fd);
}

TEST(ProcessingTest, find_max_reaction){
    FILE *fd = fopen(BLOG_PATH, "r");
    ASSERT_NE(fd, nullptr);

    blog_t *post_collection = make_blog(fd);
    ASSERT_NE(post_collection, nullptr);

    const timedelta_t month_period = create_timedelta(0, 1, 0);
    const timedelta_t two_months = create_timedelta(0, 2, 0);

    EXPECT_EQ(find_max_reaction(post_collection, month_period), 3);
    EXPECT_EQ(find_max_reaction(post_collection, two_months), 6);

    delete_blog(post_collection);
    fclose(fd);
}

TEST(ProcessingTest, select_most_hyped){
    FILE *fd = fopen(BLOG_PATH, "r");
    ASSERT_NE(fd, nullptr);

    blog_t *post_collection = make_blog(fd);
    ASSERT_NE(post_collection, nullptr);

    const timedelta_t month_period = create_timedelta(0, 1, 0);
    EXPECT_EQ(select_most_hyped(nullptr, month_period), nullptr);

    blog_t *most_hyped = select_most_hyped(post_collection, month_period);
    EXPECT_NE(most_hyped, nullptr);

    delete_blog(post_collection);
    delete_blog(most_hyped);
    fclose(fd); 
}

int main(int n_args, char* args[]){
    ::testing::InitGoogleTest(&n_args, args);
    return RUN_ALL_TESTS();
}

