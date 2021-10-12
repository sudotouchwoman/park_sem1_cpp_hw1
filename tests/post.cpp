#include "gtest/gtest.h"

extern "C"{
    #include "post.h"
}

TEST(PostTest, create_post){
    post_t *buf_post = nullptr;

    EXPECT_EQ(create_post(nullptr, "Body", "2020-02-02"), nullptr);
    EXPECT_EQ(create_post("Title", nullptr, nullptr), nullptr);
    EXPECT_EQ(create_post("Title", "Body", nullptr), nullptr);
    EXPECT_EQ(create_post("Title", nullptr, "2021-12-29"), nullptr);

    buf_post = create_post("This post is OK", "it has all the required args", "2019-05-10");

    EXPECT_NE(buf_post, nullptr);

    EXPECT_NE(buf_post->title, nullptr);
    EXPECT_NE(buf_post->post_date, nullptr);
    EXPECT_NE(buf_post->body, nullptr);

    EXPECT_EQ(buf_post->c_comments, 0);
    EXPECT_EQ(buf_post->n_comments, 0);
    EXPECT_EQ(buf_post->c_votes, 0);
    EXPECT_EQ(buf_post->n_votes, 0);
    EXPECT_EQ(buf_post->c_tags, 0);
    EXPECT_EQ(buf_post->n_tags, 0);

    EXPECT_EQ(buf_post->tags, nullptr);
    EXPECT_EQ(buf_post->comments, nullptr);
    EXPECT_EQ(buf_post->votes, nullptr);

    delete_post(buf_post);    
}

TEST(PostTest, copy_post){
    post_t *buf_post = nullptr;
    post_t *duplicate = nullptr;

    buf_post = create_post("Test", "post to copy", "2021-01-01");
    duplicate = copy_post(buf_post);

    EXPECT_EQ(copy_post(nullptr), nullptr);
    EXPECT_NE(duplicate, nullptr);

    EXPECT_STREQ(buf_post->title, duplicate->title);
    EXPECT_STREQ(buf_post->body, duplicate->body);
    EXPECT_STREQ(buf_post->post_date, duplicate->post_date);

    delete_post(buf_post);
    delete_post(duplicate);
}

TEST(PostTest, delete_post){
    post_t *buf_post = create_post("Test", "post deletion", "2021-10-12");

    EXPECT_EQ(delete_post(nullptr), POST_DELETE_ERROR);
    EXPECT_EQ(delete_post(buf_post), 0);
}

TEST(PostTest, add_tag){
    post_t *buf_post = create_post("Test", "adding tags", "2021-01-01");

    EXPECT_EQ(add_tag(nullptr, "#nullptr"), POST_APPEND_ERROR);
    EXPECT_EQ(add_tag(buf_post, "#googletest"), 0);
    EXPECT_EQ(add_tag(buf_post, "#dry"), 0);
    EXPECT_EQ(add_tag(buf_post, "#cmake"), 0);
    EXPECT_EQ(add_tag(buf_post, "#vscode"), 0);
    EXPECT_EQ(add_tag(buf_post, nullptr), POST_APPEND_ERROR);

    EXPECT_EQ(delete_post(buf_post), 0);
}

TEST(PostTest, add_comment){
    post_t *buf_post = create_post("Test", "adding comments", "2021-01-01");
    comment_t * buf_comment = create_comment("2021-10-12", "Deadline is coming");

    EXPECT_EQ(add_comment(nullptr, create_comment("2021-10-13", nullptr)), POST_APPEND_ERROR);
    EXPECT_EQ(add_comment(buf_post, create_comment("2021-10-13", nullptr)), POST_APPEND_ERROR);
    EXPECT_EQ(add_comment(nullptr, buf_comment), POST_APPEND_ERROR);
    EXPECT_EQ(add_comment(buf_post, nullptr), POST_APPEND_ERROR);
    EXPECT_EQ(add_comment(buf_post, buf_comment), 0);

    EXPECT_EQ(delete_post(buf_post), 0);
    EXPECT_EQ(delete_comment(buf_comment), 0);
}

TEST(PostTest, add_vote){
    post_t *buf_post = create_post("Test", "adding votes", "2021-01-01");
    vote_t * buf_vote = create_vote("2021-10-12");

    EXPECT_EQ(add_vote(nullptr, create_vote(nullptr)), POST_APPEND_ERROR);
    EXPECT_EQ(add_vote(buf_post, create_vote(nullptr)), POST_APPEND_ERROR);
    EXPECT_EQ(add_vote(nullptr, buf_vote), POST_APPEND_ERROR);
    EXPECT_EQ(add_vote(buf_post, nullptr), POST_APPEND_ERROR);
    EXPECT_EQ(add_vote(buf_post, buf_vote), 0);

    EXPECT_EQ(delete_post(buf_post), 0);
    EXPECT_EQ(delete_vote(buf_vote), 0); 
}

int main(int n_args, char* args[]){
    ::testing::InitGoogleTest(&n_args, args);
    return RUN_ALL_TESTS();
}

