#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <limits>

using testing::ContainerEq;

namespace {

template <typename T>
class MinimumTest : public ::testing::Test {
protected:
    virtual void test_matlib_minimum(
        int32_t m,
        int32_t n,
        float *a, int32_t ldA,
        float *x, int32_t incX
        )
    {
        rindow_matlib_s_minimum(m,n,a,ldA,x,incX);
    }
    virtual void test_matlib_minimum(
        int32_t m,
        int32_t n,
        double *a, int32_t ldA,
        double *x, int32_t incX
        )
    {
        rindow_matlib_d_minimum(m,n,a,ldA,x,incX);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(MinimumTest, TestTypes);

TYPED_TEST(MinimumTest, once) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t ldA = N;
    const int32_t incX = 1;

    TypeParam A[M*N] = {
        -1, 0, 1, 2, 3, 4
    };
    TypeParam X[N] =
      { -2,-1, 2, 4, 3, 2 };

    this->test_matlib_minimum(M,N,A,ldA,X,incX);

    TypeParam R1[N] =
      { -2,-1, 2, 4, 3, 2 };
    EXPECT_THAT(R1, ContainerEq(X));

    TypeParam R2[M*N] = {
        -2,-1, 1, 2, 3, 2
    };
    EXPECT_THAT(R2, ContainerEq(A));
}
TYPED_TEST(MinimumTest, broadcast) {
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t ldA = N;
    const int32_t incX = 1;

    TypeParam A[M*N] = {
        -1, 0, 4,
         2, 4, 1
    };
    TypeParam X[N] =
      { -2, 1, 2 };

    this->test_matlib_minimum(M,N,A,ldA,X,incX);

    TypeParam R1[N] =
      { -2, 1, 2 };
    EXPECT_THAT(R1, ContainerEq(X));

    TypeParam R2[M*N] = {
        -2, 0, 2,
        -2, 1, 1
    };
    EXPECT_THAT(R2, ContainerEq(A));
}
TYPED_TEST(MinimumTest, not_a_number_with_num) {
    const int32_t M = 6;
    const int32_t N = 1;
    const int32_t ldA = N;
    const int32_t incX = 1;
    TypeParam inf = std::numeric_limits<TypeParam>::infinity();

    TypeParam A[M*N] = { 1, 0, -1, inf,-inf, NAN };
    TypeParam X[N] = { 0 };
    this->test_matlib_minimum(M,N,A,ldA,X,incX);

    EXPECT_EQ((TypeParam)0, A[0]);
    EXPECT_EQ((TypeParam)0, A[1]);
    EXPECT_EQ((TypeParam)-1,A[2]);
    EXPECT_EQ((TypeParam)0, A[3]);
    EXPECT_EQ((TypeParam)-inf,A[4]);
    EXPECT_TRUE(std::isnan(A[5]));
}
TYPED_TEST(MinimumTest, not_a_number_with_nan) {
    const int32_t M = 6;
    const int32_t N = 1;
    const int32_t ldA = N;
    const int32_t incX = 1;
    TypeParam inf = std::numeric_limits<TypeParam>::infinity();

    TypeParam A[M*N] = { 1, 0, -1, inf,-inf, NAN };
    TypeParam X[N] = { NAN };
    this->test_matlib_minimum(M,N,A,ldA,X,incX);

    EXPECT_TRUE(std::isnan(A[0]));
    EXPECT_TRUE(std::isnan(A[1]));
    EXPECT_TRUE(std::isnan(A[2]));
    EXPECT_TRUE(std::isnan(A[3]));
    EXPECT_TRUE(std::isnan(A[4]));
    EXPECT_TRUE(std::isnan(A[5]));
}
}
