#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"

using testing::ContainerEq;

namespace {

template <typename T>
class MultiplyTest : public ::testing::Test {
protected:
    virtual void test_matlib_multiply(
        int32_t trans,int32_t m,int32_t n,
        float *x, int32_t incX,
        float *a, int32_t ldA
    ) {
        rindow_matlib_s_multiply(trans,m,n,x,incX,a,ldA);
    }
    virtual void test_matlib_multiply(
        int32_t trans,int32_t m,int32_t n,
        double *x, int32_t incX,
        double *a, int32_t ldA
    ) {
        rindow_matlib_d_multiply(trans,m,n,x,incX,a,ldA);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(MultiplyTest, TestTypes);

TYPED_TEST(MultiplyTest, once) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;
    const int32_t ldA  = N;

    // rows<cols
    TypeParam X[N] = {1,2,3,4,5,6};
    TypeParam A[M][N] = {{1,10,100,-1,-10,-100}};
    this->test_matlib_multiply(
        trans,          // int32_t trans,
        M,              // int32_t m,
        N,              // int32_t n,
        (TypeParam *)X, // *x,
        incX,           // int32_t incX,
        (TypeParam *)A, // *a,
        ldA             // int32_t ldA
    );
    TypeParam R1[N] = {1,2,3,4,5,6};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {{1,20,300,-4,-50,-600}};
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MultiplyTest, broadcast) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;
    const int32_t incX = 1;
    const int32_t ldA  = N;

    // rows>cols
    TypeParam X[N] = {2,3};
    TypeParam A[M][N] = {{1,10}, {100,-1}, {-10,-100}};
    this->test_matlib_multiply(
        trans,          // int32_t trans,
        M,              // int32_t m,
        N,              // int32_t n,
        (TypeParam *)X, // *x,
        incX,           // int32_t incX,
        (TypeParam *)A, // *a,
        ldA             // int32_t ldA
    );
    TypeParam R1[N] = {2,3};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {{2,30},{200,-3},{-20,-300}};
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MultiplyTest, trans_rowsGTcols) {
    const int32_t trans = RINDOW_MATLIB_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;
    const int32_t incX = 1;
    const int32_t ldA  = N;

    // rows>cols
    TypeParam X[M] = {2, 3, 5};
    TypeParam A[M][N] = {{1,10}, {100,-1}, {-10,-100}};
    this->test_matlib_multiply(
        trans,          // int32_t trans,
        M,              // int32_t m,
        N,              // int32_t n,
        (TypeParam *)X, // *x,
        incX,           // int32_t incX,
        (TypeParam *)A, // *a,
        ldA             // int32_t ldA
    );
    TypeParam R1[M] = {2,3,5};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {{2,20},{300,-3},{-50,-500}};
    EXPECT_THAT(R2, ContainerEq(A));
}
TYPED_TEST(MultiplyTest, trans_rowsLTcols) {
    const int32_t trans = RINDOW_MATLIB_TRANS;
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t incX = 1;
    const int32_t ldA  = N;

    // rows<cols
    TypeParam X[M] = {2, 3};
    TypeParam A[M][N] = {{1,10,100}, {-1,-10,-100}};
    this->test_matlib_multiply(
        trans,          // int32_t trans,
        M,              // int32_t m,
        N,              // int32_t n,
        (TypeParam *)X, // *x,
        incX,           // int32_t incX,
        (TypeParam *)A, // *a,
        ldA             // int32_t ldA
    );
    TypeParam R1[M] = {2,3};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {{2,20,200},{-3,-30,-300}};
    EXPECT_THAT(R2, ContainerEq(A));
}
}
