#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "rindow/matlib.h"
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class TopKTest : public ::testing::Test {
protected:
    virtual void test_matlib_top_k(
        int32_t m, int32_t n, float *a, 
        int32_t ldA, int32_t k, int32_t sorted,
        float *top_values, int32_t *top_indices
    ) {
        rindow_matlib_s_top_k(m, n, a, ldA, k, sorted, top_values, top_indices);
    }

    virtual void test_matlib_top_k(
        int32_t m, int32_t n, double *a, 
        int32_t ldA, int32_t k, int32_t sorted,
        double *top_values, int32_t *top_indices
    ) {
        rindow_matlib_d_top_k(m, n, a, ldA, k, sorted, top_values, top_indices);
    }
};

typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(TopKTest, TestTypes);

TYPED_TEST(TopKTest, normal) {
    const int32_t m = 2; // Number of rows or vectors
    const int32_t n = 5; // Number of columns or elements in each vector
    const int32_t ldA = n; // Leading dimension of the input
    const int32_t k = 3; // Number of top elements to find
    const int32_t sorted = 1; // Whether to return sorted values and indices

    TypeParam a[m][n] = {
        {1.0, 2.0, 3.0, 4.0, 5.0},
        {5.0, 4.0, 3.0, 2.0, 1.0}
    };

    TypeParam top_values[m][k];
    int32_t top_indices[m][k];

    this->test_matlib_top_k(m, n, (TypeParam*)a, ldA, k, sorted, (TypeParam*)top_values, (int32_t*)top_indices);

    // Expected results
    TypeParam expected_values[m][k] = {
        {5.0, 4.0, 3.0},
        {5.0, 4.0, 3.0}
    };

    int32_t expected_indices[m][k] = {
        {4, 3, 2},
        {0, 1, 2}
    };
    

    // Compare the top K values and indices with expected results
    for (int32_t i = 0; i < m; i++) {
        EXPECT_TRUE(Utils::isclose(k, (TypeParam*)expected_values[i], top_values[i]));
        EXPECT_EQ(std::vector<int32_t>(expected_indices[i], expected_indices[i] + k),
                  std::vector<int32_t>(top_indices[i], top_indices[i] + k));
    }
}
} // namespace
