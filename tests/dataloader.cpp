#include "gtest/gtest.h"

extern "C"{
    #include "dataloader.h"
}

#define OK_INPUT    "../data/user_blog_ok.txt"
#define BAD_INPUT   "../data/user_blog_bad.txt"

#define OK_POST "../data/post_ok.txt"
#define BAD_POST "../data/post_bad.txt"

#define OK_TIME "../data/user_input_ok.txt"
#define BAD_TIME "../data/user_input_bad.txt"

TEST(DataloaderTest, open_files){
    FILE *buf_fd = nullptr;
    buf_fd = fopen(OK_INPUT, "r");
    ASSERT_NE(buf_fd, nullptr); fclose(buf_fd);
    buf_fd = fopen(BAD_INPUT, "r");
    ASSERT_NE(buf_fd, nullptr); fclose(buf_fd);
    buf_fd = fopen(OK_POST, "r");
    ASSERT_NE(buf_fd, nullptr); fclose(buf_fd);
    buf_fd = fopen(BAD_POST, "r");
    ASSERT_NE(buf_fd, nullptr); fclose(buf_fd);
    buf_fd = fopen(OK_TIME, "r");
    ASSERT_NE(buf_fd, nullptr); fclose(buf_fd);
    buf_fd = fopen(BAD_TIME, "r");
    ASSERT_NE(buf_fd, nullptr); fclose(buf_fd);
}

TEST(DataloaderTest, make_blog){
    FILE *fd = nullptr;
    blog_t* buf_blog = nullptr;

    fd = fopen(OK_INPUT, "r");
    buf_blog = make_blog(fd);
    EXPECT_NE(buf_blog, nullptr);
    free_used_resources(nullptr, fd, buf_blog, nullptr);
    
    fd = fopen(BAD_INPUT, "r");
    buf_blog = make_blog(fd);
    EXPECT_EQ(buf_blog, nullptr);
    free_used_resources(nullptr, fd, buf_blog, nullptr);
}

TEST(DataloaderTest, make_post){
    FILE *fd = NULL;
    post_t* buf_post = nullptr;

    fd = fopen(OK_POST, "r");
    buf_post = make_post(fd);
    EXPECT_NE(buf_post, nullptr);
    delete_post(buf_post); fclose(fd);

    fd = fopen(BAD_POST, "r");
    buf_post = make_post(fd);
    EXPECT_EQ(make_post(fd), nullptr); 
    delete_post(buf_post); fclose(fd);
}

TEST(DataloaderTest, make_time_period){
    FILE *fd = nullptr;
    fd = fopen(OK_TIME, "r");
    timedelta_t offset;
    EXPECT_EQ(make_time_period(fd, &offset), 0);
    EXPECT_EQ(make_time_period(fd, nullptr), EMPTY_PTR_ERROR);
    fclose(fd);
    fd = fopen(BAD_TIME, "r");
    EXPECT_EQ(make_time_period(fd, &offset), FORMAT_ERROR);
    fclose(fd);
}

int main(int n_args, char* args[]){
    ::testing::InitGoogleTest(&n_args, args);
    return RUN_ALL_TESTS();
}

