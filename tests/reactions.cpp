#include "gtest/gtest.h"

extern "C"{
    #include "reactions.h"
}

TEST(ReactionsTest, create_vote){
    vote_t *buf_vote = create_vote("2020-10-02");
    EXPECT_NE(buf_vote, nullptr);
    EXPECT_EQ(create_vote(nullptr), nullptr);
    delete_vote(buf_vote);
}

TEST(ReactionsTest, delete_vote){
    vote_t *buf_vote = create_vote("2020-10-02");
    EXPECT_EQ(delete_vote(buf_vote), 0);
    EXPECT_EQ(delete_vote(nullptr), EMPTY_VOTE);
}

TEST(ReactionsTest, copy_vote){
    vote_t *buf_vote = create_vote("2020-10-02");
    vote_t *duplicate = copy_vote(buf_vote);
    
    EXPECT_EQ(copy_vote(nullptr), nullptr);
    EXPECT_EQ(copy_vote(create_vote(nullptr)), nullptr);
    EXPECT_NE(duplicate, nullptr);

    delete_vote(buf_vote);
    delete_vote(duplicate);
}

TEST(ReactionsTest, create_comment){
    comment_t *buf_comment = create_comment("2021-10-13", "Working on final unit-tests");
    EXPECT_NE(buf_comment, nullptr);
    EXPECT_EQ(create_comment(nullptr, "nullptr"), nullptr);
    EXPECT_EQ(create_comment("2015-04-18", nullptr), nullptr);
    delete_comment(buf_comment);
}

TEST(ReactionsTest, delete_comment){
    comment_t *buf_comment = create_comment("2016-11-29", "Working on final unit-tests");
    EXPECT_EQ(delete_comment(buf_comment), 0);
    EXPECT_EQ(delete_comment(nullptr), EMPTY_COMMENT);
}

TEST(ReactionsTest, copy_comment){
    comment_t *buf_comment = create_comment("2020-05-05", "Unit-test for copying comments");
    comment_t *duplicate = copy_comment(buf_comment);

    EXPECT_EQ(copy_comment(nullptr), nullptr);
    EXPECT_EQ(copy_comment(create_comment(nullptr, "Comment without specified date")), nullptr);
    EXPECT_EQ(copy_comment(create_comment("2005-10-24", nullptr)), nullptr);
    EXPECT_NE(duplicate, nullptr);

    delete_comment(buf_comment);
    delete_comment(duplicate);
}

int main(int n_args, char* args[]){
    ::testing::InitGoogleTest(&n_args, args);
    return RUN_ALL_TESTS();
}

