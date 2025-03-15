#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "rindow/matlib.h"
#include "Utils.h"
#include <time.h>

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class TopKTest : public ::testing::Test {
protected:
    virtual void test_matlib_topk(
        int32_t m, int32_t n, const float *input,
        int32_t k, int32_t sorted,
        float *values, int32_t *indices
    ) {
        rindow_matlib_s_topk(m, n, input, k, sorted, values, indices);
    }

    virtual void test_matlib_topk(
        int32_t m, int32_t n, const double *input,
        int32_t k, int32_t sorted,
        double *values, int32_t *indices
    ) {
        rindow_matlib_d_topk(m, n, input, k, sorted, values, indices);
    }
};

typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(TopKTest, TestTypes);

TYPED_TEST(TopKTest, normal) {
    const int32_t m = 2;
    const int32_t n = 10;
    const int32_t k = 3;
    const int32_t sorted = 1;
    
    TypeParam input[m][n] = {
        {1.0, 2.0, 3.0, 4.0, 5.0, 1.0, 2.0, 3.0, 4.0, 5.0},
        {5.0, 4.0, 3.0, 2.0, 1.0, 5.0, 4.0, 3.0, 2.0, 1.0}
    };

    TypeParam values[m][k];
    int32_t indices[m][k];

    this->test_matlib_topk(m, n, (TypeParam*)input, k, sorted, (TypeParam*)values, (int32_t*)indices);

    TypeParam R1[m][k] = {
        {5.0, 5.0, 4.0},
        {5.0, 5.0, 4.0}
    };

    EXPECT_TRUE(Utils::isclose(m*k, (TypeParam*)R1, (TypeParam*)values));

    for(int32_t i=0; i<m; i++) {
        for(int32_t j=0; j<k; j++) {
            ASSERT_GE(indices[i][j],0);
            ASSERT_LT(indices[i][j],n);
            EXPECT_TRUE(Utils::isclose(1, (TypeParam*)&R1[i][j], (TypeParam*)&input[i][indices[i][j]]));
        }
    }
}

TYPED_TEST(TopKTest, large) {
    const int32_t m = 2;
    const int32_t n = 5000;
    const int32_t k = 10;
    const int32_t sorted = 1;

    time_t salt;
    TypeParam input[m][n];
    TypeParam values[m][k];
    int32_t indices[m][k];

    srand((unsigned int)time(&salt));
    for (int i = 0; i < m*n; ++i) {
        ((TypeParam*)input)[i] = (TypeParam)rand() / (TypeParam)(RAND_MAX) * 1000;
    }

    this->test_matlib_topk(m, n, (TypeParam*)input, k, sorted, (TypeParam*)values, (int32_t*)indices);

    TypeParam SortedInput[m][n];
    memcpy(&SortedInput,&input,m*n*sizeof(TypeParam));

    for(int i=0; i<m; ++i) {
        std::sort(SortedInput[i],SortedInput[i]+n,std::greater<>());
    }
    for(int i=0; i<m; ++i) {
       EXPECT_TRUE(Utils::isclose(k, (TypeParam*)SortedInput[i], (TypeParam*)values[i]));
    }

    for(int32_t i=0; i<m; i++) {
        for(int32_t j=0; j<k; j++) {
            ASSERT_GE(indices[i][j],0);
            ASSERT_LT(indices[i][j],n);
            EXPECT_TRUE(Utils::isclose(1, (TypeParam*)&SortedInput[i][j], (TypeParam*)&input[i][indices[i][j]]));
        }
    }
}

TYPED_TEST(TopKTest, withoutsorted) {
    const int32_t m = 2;
    const int32_t n = 1000;
    const int32_t k = 100;
    const int32_t sorted = 0;

    time_t salt;
    TypeParam input[m][n];
    TypeParam values[m][k];
    int32_t indices[m][k];

    srand((unsigned int)time(&salt));
    for (int i = 0; i < m*n; ++i) {
        ((TypeParam*)input)[i] = (TypeParam)rand() / (TypeParam)(RAND_MAX) * 1000;
    }

    this->test_matlib_topk(m, n, (TypeParam*)input, k, sorted, (TypeParam*)values, (int32_t*)indices);

    TypeParam SortedInput[m][n];
    memcpy(&SortedInput,&input,m*n*sizeof(TypeParam));

    for(int i=0; i<m; ++i) {
        std::sort(SortedInput[i],SortedInput[i]+n,std::greater<>());
    }
    for(int i=0; i<m; ++i) {
        EXPECT_FALSE(Utils::isclose(k, (TypeParam*)SortedInput[i], (TypeParam*)values[i]));
    }

    for(int32_t i=0; i<m; i++) {
        for(int32_t j=0; j<k; j++) {
            ASSERT_GE(indices[i][j],0);
            ASSERT_LT(indices[i][j],n);
            TypeParam foundValue = input[i][indices[i][j]];
            bool found = false;
            for(int32_t p=0; p<k; p++) {
                if(Utils::isclose(1, (TypeParam*)&SortedInput[i][p], &foundValue)) {
                    found = true;
                }
            }
            EXPECT_TRUE(found);
        }
    }
}

} // namespace
