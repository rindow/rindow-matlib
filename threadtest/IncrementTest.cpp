#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"

using testing::ContainerEq;

namespace {

template <typename T>
class IncrementTest : public ::testing::Test {
protected:
    virtual void test_matlib_increment(int32_t n, float *x, int32_t incX, float alpha, float beta)
    {
        rindow_matlib_s_increment(n,x,incX,alpha,beta);
    }

    virtual void test_matlib_increment(int32_t n, double *x, int32_t incX, double alpha, double beta)
    {
        rindow_matlib_d_increment(n,x,incX,alpha,beta);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(IncrementTest, TestTypes);

TYPED_TEST(IncrementTest, normal) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam X[N] = {1, 2, 3, 4, 5, 6};

    // X := X + 1
    this->test_matlib_increment(
        N,
        X,
        incX,
        1,  // alpha
        1   // beta
    );

    TypeParam R1[N] = { 2, 3, 4, 5, 6, 7 };
    EXPECT_THAT(R1, ContainerEq(X));

    // X := 8 - X
    this->test_matlib_increment(
        N,
        X,
        incX,
        -1, // alpha
        8   // beta
    );

    TypeParam R2[N] = { 6, 5, 4, 3, 2, 1 };
    EXPECT_THAT(R2, ContainerEq(X));

    // X := 2 * X
    this->test_matlib_increment(
        N,
        X,
        incX,
        2,  // alpha
        0   // beta
    );

    TypeParam R3[N] = { 12, 10, 8, 6, 4, 2 };
    EXPECT_THAT(R3, ContainerEq(X));
}
}
