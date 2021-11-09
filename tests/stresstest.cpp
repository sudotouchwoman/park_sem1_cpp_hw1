#include <string>
#include <cstdlib>
#include <fstream>
#include "gtest/gtest.h"

extern "C" {
#include "Processing.h"
#include "MatrixIO.h"
}

class StressTestFixture: public testing::Test {
    protected:
        const char * task_file_name = "../../examples/test.txt";
        const char * answer_file_name = "../../examples/answer.txt";
        FILE * in = nullptr;
        int * true_transposed = nullptr;
        void SetUp() override { loadTest(); loadAnswer(); }
        void TearDown() override { fclose(in); delete[] true_transposed; }
        void loadTest();
        void loadAnswer();
        FILE *const provideFile() { return in; }
};

void StressTestFixture::loadTest() {
    in = fopen(task_file_name, "r");
}

void StressTestFixture::loadAnswer() {
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

TEST_F(StressTestFixture, transpose_by_side_diagonal) {
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
