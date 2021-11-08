#include <string>
#include <cstdlib>
#include "gtest/gtest.h"

extern "C" {
#include "Matrix.h"
#include "MatrixIO.h"
}

class MatrixFixture: public testing::Test {
    protected:
        int* fake_matrix = nullptr;
        void SetUp() override {};
        void TearDown() override { delete[] fake_matrix; }
        const int* const mock_matrix(const size_t rows, const size_t cols);
        const size_t area(const size_t a, const size_t b) { return a * b; }
};

const int* const MatrixFixture::mock_matrix(const size_t rows, const size_t cols) {
    delete[] fake_matrix;
    fake_matrix = new int[rows * cols]();
    for (size_t i = 0; i < rows * cols; ++i)
        fake_matrix[i] = rand() % 100;
    return fake_matrix;
}

TEST_F(MatrixFixture, create_matrix) {
    const dims_t md = { 10, 10 };
    matrix_t * const test_mx = create_matrix(md);

    EXPECT_NE(test_mx, nullptr);
    EXPECT_NE(test_mx->cells, nullptr);
    EXPECT_EQ(test_mx->dims.COLS, md.COLS);
    EXPECT_EQ(test_mx->dims.ROWS, md.ROWS);
    EXPECT_EQ(delete_matrix(test_mx), MX_OK);
}

TEST_F(MatrixFixture, init_matrix_fromarray) {
    const dims_t md = { 100, 100 };

    const int* const mocked_mx = mock_matrix(md.ROWS, md.COLS);
    matrix_t *const test_mx = init_matrix_fromarray(mocked_mx, md);

    EXPECT_NE(test_mx, nullptr);
    EXPECT_NE(test_mx->cells, nullptr);
    EXPECT_EQ(test_mx->dims.COLS, md.COLS);
    EXPECT_EQ(test_mx->dims.ROWS, md.ROWS);

    for (size_t i = 0; i < area(md.ROWS, md.COLS); ++i)
        EXPECT_EQ(test_mx->cells[i], mocked_mx[i]);

    EXPECT_EQ(delete_matrix(test_mx), MX_OK);

    matrix_t *const bad_init = init_matrix_fromarray(nullptr, md);
    EXPECT_EQ(bad_init, nullptr);
}

TEST_F(MatrixFixture, delete_matrix) {
    EXPECT_EQ(delete_matrix(nullptr), MX_ERR_EMPTY_MX);
}

int main(int n_args, char* args[]) {
    ::testing::InitGoogleTest(&n_args, args);
    return RUN_ALL_TESTS();
}
