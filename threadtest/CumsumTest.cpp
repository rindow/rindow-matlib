#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdbool.h>
#include <math.h>

using testing::ContainerEq;

namespace {

template <typename T>
class CumsumTest : public ::testing::Test {
protected:
    virtual void test_matlib_cumsum(
        int32_t n,
        float *x, int32_t incX,
        int32_t exclusive,int32_t reverse,
        float *y, int32_t incY)
    {
        rindow_matlib_s_cumsum(n, x, incX, exclusive, reverse, y, incY);
    }

    virtual void test_matlib_cumsum(
        int32_t n,
        double *x, int32_t incX,
        int32_t exclusive,int32_t reverse,
        double *y, int32_t incY)
    {
        rindow_matlib_d_cumsum(n, x, incX, exclusive, reverse, y, incY);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(CumsumTest, TestTypes);

TYPED_TEST(CumsumTest, CumsumNormal) {
    const int32_t N = 4;
    int32_t exclusive = false;
    int32_t reverse = false;
    const int32_t incX = 1;
    const int32_t incY = 1;
    TypeParam X[N] = {1,2,1,2};
    TypeParam Y[N];

    this->test_matlib_cumsum(N, X, incX, exclusive, reverse, Y, incY);

    TypeParam R1[N] = {1,3,4,6};
    EXPECT_THAT(R1, ContainerEq(Y));
}

TYPED_TEST(CumsumTest, CumsumExclusive) {
    const int32_t N = 4;
    int32_t exclusive = true;
    int32_t reverse = false;
    const int32_t incX = 1;
    const int32_t incY = 1;
    TypeParam X[N] = {1,2,1,2};
    TypeParam Y[N];

    this->test_matlib_cumsum(N, X, incX, exclusive, reverse, Y, incY);

    TypeParam R1[N] = {0,1,3,4};
    EXPECT_THAT(R1, ContainerEq(Y));
}

TYPED_TEST(CumsumTest, CumsumReverse) {
    const int32_t N = 4;
    int32_t exclusive = false;
    int32_t reverse = true;
    const int32_t incX = 1;
    const int32_t incY = 1;
    TypeParam X[N] = {1,2,1,2};
    TypeParam Y[N];

    this->test_matlib_cumsum(N, X, incX, exclusive, reverse, Y, incY);

    TypeParam R1[N] = {6,4,3,1};
    EXPECT_THAT(R1, ContainerEq(Y));
}

TYPED_TEST(CumsumTest, CumsumNaN) {
    const int32_t N = 4;
    int32_t exclusive = false;
    int32_t reverse = false;
    const int32_t incX = 1;
    const int32_t incY = 1;
    TypeParam X[N] = {1,2,NAN,2};
    TypeParam Y[N];

    this->test_matlib_cumsum(N, X, incX, exclusive, reverse, Y, incY);

    EXPECT_EQ(1.0, Y[0]);
    EXPECT_EQ(3.0, Y[1]);
    EXPECT_TRUE(isnan(Y[2]));
    EXPECT_TRUE(isnan(Y[3]));
}

TYPED_TEST(CumsumTest, CumsumNaNReverse) {
    const int32_t N = 4;
    int32_t exclusive = false;
    int32_t reverse = true;
    const int32_t incX = 1;
    const int32_t incY = 1;
    TypeParam X[N] = {1,2,NAN,2};
    TypeParam Y[N];

    this->test_matlib_cumsum(N, X, incX, exclusive, reverse, Y, incY);

    EXPECT_TRUE(isnan(Y[0]));
    EXPECT_TRUE(isnan(Y[1]));
    EXPECT_EQ(3.0, Y[2]);
    EXPECT_EQ(1.0, Y[3]);
}
}
