#include <string>
#include <cstdlib>
#include <fstream>
#include "gtest/gtest.h"

extern "C" {
#include "Processing.h"
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

class ProcessingFixture: public testing::Test {
    protected:
        const char * task_file_name = "examples/test.txt";
        const char * answer_file_name = "examples/answer.txt";
        FILE * in = nullptr;
        int * true_transposed = nullptr;
        void SetUp() override { loadTest(); loadAnswer(); }
        void TearDown() override { fclose(in); delete[] true_transposed; }
        void loadTest();
        void loadAnswer();
        FILE *const provideFile() { return in; }
};

void ProcessingFixture::loadTest() {
    in = fopen(task_file_name, "r");
}

void ProcessingFixture::loadAnswer() {
    std::ifstream in;
    in.open(answer_file_name);

    size_t cols = 0, rows = 0;
    in >> cols >> rows;
    const size_t area = cols * rows;

    true_transposed = new int[area]();
    for (size_t i = 0; i < area; ++i)
        in >> true_transposed[i];

    in.close();
}

TEST_F(MatrixFixture, transpose_by_side_diagonal) {
    const dims_t md = { 10, 100 };

    const int* const mocked_mx = mock_matrix(md.ROWS, md.COLS);
    matrix_t *const test_mx = init_matrix_fromarray(mocked_mx, md);
    matrix_t *const transposed = transpose_by_side_diagonal(test_mx);

    EXPECT_NE(transposed, nullptr);
    EXPECT_NE(transposed->cells, nullptr);
    EXPECT_EQ(transposed->dims.COLS, test_mx->dims.ROWS);
    EXPECT_EQ(transposed->dims.ROWS, test_mx->dims.COLS);

    EXPECT_EQ(delete_matrix(test_mx), MX_OK);
    EXPECT_EQ(delete_matrix(transposed), MX_OK);
}

TEST_F(ProcessingFixture, transpose_by_side_diagonal) {
    FILE *const in = provideFile();
    ASSERT_NE(in, nullptr);

    matrix_t *const mocked_mx = init_matrix(in);
    ASSERT_NE(mocked_mx, nullptr);

    matrix_t *const transposed = transpose_by_side_diagonal(mocked_mx);
    EXPECT_NE(transposed, nullptr);
    EXPECT_NE(transposed->cells, nullptr);

    const size_t area = transposed->dims.ROWS * transposed->dims.COLS;

    for (size_t i = 0; i < area; ++i)
        EXPECT_EQ(transposed->cells[i], true_transposed[i]);

    EXPECT_EQ(delete_matrix(mocked_mx), MX_OK);
    EXPECT_EQ(delete_matrix(transposed), MX_OK);
}

int main(int n_args, char* args[]) {
    ::testing::InitGoogleTest(&n_args, args);
    return RUN_ALL_TESTS();
}
