#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"

using testing::ContainerEq;

namespace {

template <typename T>
class AddTest : public ::testing::Test {
protected:
    virtual void test_matlib_add(
        int32_t trans, int32_t m, int32_t n,
        float alpha, float *x, int32_t incX, float *a, int32_t ldA)
    {
        rindow_matlib_s_add(trans, m, n, alpha, x, incX, a, ldA);
    }
    virtual void test_matlib_add(
        int32_t trans, int32_t m, int32_t n,
        double alpha, double *x, int32_t incX, double *a, int32_t ldA)
    {
        rindow_matlib_d_add(trans, m, n, alpha, x, incX, a, ldA);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(AddTest, TestTypes);

TYPED_TEST(AddTest, once) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;
    const int32_t ldA  = N;

    // rows<cols
    TypeParam alpha = 1.0;
    TypeParam X[N] = {1, 2, 3, 4, 5, 6};
    TypeParam A[M][N] = {{1,10,100,-1,-10,-100}};
    this->test_matlib_add(
        trans,          // int32_t trans,
        M,              // int32_t m,
        N,              // int32_t n,
        alpha,          // alpha,
        (TypeParam *)X, // *x,
        incX,           // int32_t incX,
        (TypeParam *)A, // *a,
        ldA             // int32_t ldA
    );
    TypeParam R1[N] = {1,2,3,4,5,6};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {{2,12,103,3,-5,-94}};
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(AddTest, broadcastAndScale) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;
    const int32_t incX = 1;
    const int32_t ldA  = N;

    // rows>cols
    TypeParam alpha = -1.0;
    TypeParam X[N] = {1, 2};
    TypeParam A[M][N] = {{10,20}, {100,200}, {-10,-100}};
    this->test_matlib_add(
        trans,          // int32_t trans,
        M,              // int32_t m,
        N,              // int32_t n,
        alpha,          // alpha,
        (TypeParam *)X, // *x,
        incX,           // int32_t incX,
        (TypeParam *)A, // *a,
        ldA             // int32_t ldA
    );
    TypeParam R1[N] = {1,2};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {{9,18},{99,198},{-11,-102}};
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(AddTest, trans) {
    const int32_t trans = RINDOW_MATLIB_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;
    const int32_t incX = 1;
    const int32_t ldA  = N;

    TypeParam alpha = 1.0;
    TypeParam X[M] = {1, 2, 3};
    TypeParam A[M][N] = {{1,10}, {100,-1}, {-10,-100}};
    this->test_matlib_add(
        trans,          // int32_t trans,
        M,              // int32_t m,
        N,              // int32_t n,
        alpha,          // alpha,
        (TypeParam *)X, // *x,
        incX,           // int32_t incX,
        (TypeParam *)A, // *a,
        ldA             // int32_t ldA
    );
    TypeParam R1[M] = {1,2,3};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {{2,11},{102,1},{-7,-97}};
    EXPECT_THAT(R2, ContainerEq(A));
}
}
