#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"

using testing::ContainerEq;

namespace {

template <typename T>
class PowTest : public ::testing::Test {
protected:
    virtual void test_matlib_pow(
        int32_t trans,int32_t m,int32_t n,
        float *a, int32_t ldA,
        float *x, int32_t incX)
    {
        rindow_matlib_s_pow(trans,m,n,a,ldA,x,incX);
    }
    virtual void test_matlib_pow(
        int32_t trans,int32_t m,int32_t n,
        double *a, int32_t ldA,
        double *x, int32_t incX)
    {
        rindow_matlib_d_pow(trans,m,n,a,ldA,x,incX);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(PowTest, TestTypes);

TYPED_TEST(PowTest, once) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 1;
    const int32_t N = 3;
    const int32_t incX = 1;
    const int32_t ldA  = N;

    // rows<cols
    TypeParam X[N] = {4,3,2};
    TypeParam A[M][N] = {{1,2,3}};
    this->test_matlib_pow(
        trans,          // int32_t trans,
        M,              // int32_t m,
        N,              // int32_t n,
        (TypeParam *)A, // *a,
        ldA,            // int32_t ldA
        (TypeParam *)X, // *x,
        incX            // int32_t incX,
    );
    TypeParam R1[N] = {4,3,2};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {{1,8,9}};
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(PowTest, broadcast) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;
    const int32_t incX = 1;
    const int32_t ldA  = N;

    // rows>cols
    TypeParam X[N] = {3,2};
    TypeParam A[M][N] = {{1,2}, {3,4}, {5,6}};
    this->test_matlib_pow(
        trans,          // int32_t trans,
        M,              // int32_t m,
        N,              // int32_t n,
        (TypeParam *)A, // *a,
        ldA,            // int32_t ldA
        (TypeParam *)X, // *x,
        incX            // int32_t incX,
    );
    TypeParam R1[N] = {3,2};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {{1,4},{27,16},{125,36}};
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(PowTest, trans_rowsGTcols) {
    const int32_t trans = RINDOW_MATLIB_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;
    const int32_t incX = 1;
    const int32_t ldA  = N;

    // rows>cols
    TypeParam X[M] = {4, 3, 2};
    TypeParam A[M][N] = {{1,2}, {3,4}, {5,6}};
    this->test_matlib_pow(
        trans,          // int32_t trans,
        M,              // int32_t m,
        N,              // int32_t n,
        (TypeParam *)A, // *a,
        ldA,            // int32_t ldA
        (TypeParam *)X, // *x,
        incX            // int32_t incX,
    );
    TypeParam R1[M] = {4,3,2};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {{1,16},{27,64},{25,36}};
    EXPECT_THAT(R2, ContainerEq(A));
}
TYPED_TEST(PowTest, trans_rowsLTcols) {
    const int32_t trans = RINDOW_MATLIB_TRANS;
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t incX = 1;
    const int32_t ldA  = N;

    // rows<cols
    TypeParam X[M] = {3, 2};
    TypeParam A[M][N] = {{1,2,3}, {4,5,6}};
    this->test_matlib_pow(
        trans,          // int32_t trans,
        M,              // int32_t m,
        N,              // int32_t n,
        (TypeParam *)A, // *a,
        ldA,            // int32_t ldA
        (TypeParam *)X, // *x,
        incX            // int32_t incX,
    );
    TypeParam R1[M] = {3,2};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {{1,8,27},{16,25,36}};
    EXPECT_THAT(R2, ContainerEq(A));
}
}
