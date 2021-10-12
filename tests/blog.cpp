#include "gtest/gtest.h"

extern "C"{
    #include "blog.h"
}

TEST(BlogTest, create_blog){
    blog_t *buf_blog = nullptr;
    buf_blog = create_blog();
    EXPECT_NE(buf_blog, nullptr);
    
    EXPECT_EQ((buf_blog->posts), nullptr);
    EXPECT_EQ((buf_blog->c_posts), 0);
    EXPECT_EQ((buf_blog->n_posts), 0);
    
    EXPECT_EQ(delete_blog(buf_blog), 0);
    EXPECT_EQ(delete_blog(nullptr), BLOG_DELETE_ERROR);    
}

TEST(BlogTest, copy_blog){
    blog_t *buf_blog = nullptr;
    blog_t *duplicate = nullptr;

    EXPECT_EQ(copy_blog(buf_blog), nullptr);

    buf_blog = create_blog();
    duplicate = copy_blog(buf_blog);
    EXPECT_NE(buf_blog, nullptr);
    EXPECT_NE(duplicate, nullptr);

    delete_blog(buf_blog);
    delete_blog(duplicate);
}

TEST(BlogTest, add_post){

    blog_t *buf_blog = nullptr;
    post_t *post_to_add = nullptr;

    EXPECT_EQ(add_post(buf_blog, post_to_add), BLOG_APPEND_ERROR);
    buf_blog = create_blog();
    EXPECT_EQ(add_post(buf_blog, post_to_add), BLOG_APPEND_ERROR);
    post_to_add = create_post("Title", "Body", "2021-12-29");
    EXPECT_NE(post_to_add, nullptr);
    EXPECT_EQ(add_post(buf_blog, post_to_add), 0);
    EXPECT_EQ(add_post(buf_blog, post_to_add), 0);
    EXPECT_EQ(add_post(buf_blog, post_to_add), 0);
    EXPECT_EQ(add_post(buf_blog, nullptr), BLOG_APPEND_ERROR);

    delete_post(post_to_add);
    delete_blog(buf_blog);
}


int main(int n_args, char* args[]){
    ::testing::InitGoogleTest(&n_args, args);
    return RUN_ALL_TESTS();
}

