#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"

using testing::ContainerEq;

namespace {

template <typename T>
class OnehotTest : public ::testing::Test {
protected:
    virtual int32_t test_matlib_onehot(
        int32_t dtype, int32_t m, int32_t n,
        void *x, int32_t incX,
        float alpha,
        float *a, int32_t ldA)
    {
        return rindow_matlib_s_onehot(dtype,m,n,x,incX,alpha,a,ldA);
    }
    virtual int32_t test_matlib_onehot(
        int32_t dtype, int32_t m, int32_t n,
        void *x, int32_t incX,
        double alpha,
        double *a, int32_t ldA)
    {
        return rindow_matlib_d_onehot(dtype,m,n,x,incX,alpha,a,ldA);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(OnehotTest, TestTypes);

TYPED_TEST(OnehotTest, normal) {
    const int32_t indexDtype = rindow_matlib_dtype_int32;
    const int32_t M = 4;
    const int32_t N = 3;
    const int32_t incX = 1;
    const int32_t ldA  = N;
    int32_t rc;

    int32_t X[M] = {0,1,2,0};
    TypeParam alpha = 1;
    TypeParam A[M][N] = {
        {0,0,0},
        {0,0,0},
        {0,0,0},
        {0,0,0}
    };

    rc = this->test_matlib_onehot(
        indexDtype,
        M,              // int32_t m,
        N,              // int32_t n,
        X,              // *x,
        incX,           // int32_t incX,
        alpha,          // alpha,
        (TypeParam *)A, // *a,
        ldA             // int32_t ldA
    );
    ASSERT_EQ(0,rc);

    int32_t R1[M] = {0,1,2,0};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {
        {1,0,0},
        {0,1,0},
        {0,0,1},
        {1,0,0}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}
TYPED_TEST(OnehotTest, minus) {
    const int32_t indexDtype = rindow_matlib_dtype_int32;
    const int32_t M = 4;
    const int32_t N = 3;
    const int32_t incX = 1;
    const int32_t ldA  = N;
    int32_t rc;

    int32_t X[M] = {0,1,2,0};
    TypeParam alpha = -2;
    TypeParam A[M][N] = {
        {1,1,1},
        {1,1,1},
        {1,1,1},
        {1,1,1}
    };

    rc = this->test_matlib_onehot(
        indexDtype,
        M,              // int32_t m,
        N,              // int32_t n,
        X,              // *x,
        incX,           // int32_t incX,
        alpha,          // alpha,
        (TypeParam *)A, // *a,
        ldA             // int32_t ldA
    );
    ASSERT_EQ(0,rc);

    int32_t R1[M] = {0,1,2,0};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {
        {-1, 1, 1},
        { 1,-1, 1},
        { 1, 1,-1},
        {-1, 1, 1}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}
}
