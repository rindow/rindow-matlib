#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdbool.h>

using testing::ContainerEq;

namespace {

template <typename T>
class EqualTest : public ::testing::Test {
protected:
    virtual void test_matlib_equal(
        int32_t n,
        float *x, int32_t incX,
        float *y, int32_t incY)
    {
        rindow_matlib_s_equal(n,x,incX,y,incY);
    }

    virtual void test_matlib_equal(
        int32_t n,
        double *x, int32_t incX,
        double *y, int32_t incY)
    {
        rindow_matlib_d_equal(n,x,incX,y,incY);
    }
    virtual void test_matlib_equal(
        int32_t n,
        int32_t *x, int32_t incX,
        int32_t *y, int32_t incY)
    {
        int32_t dtype = rindow_matlib_dtype_int32;
        rindow_matlib_i_equal(dtype,n,x,incX,y,incY);
    }
    virtual void test_matlib_equal(
        int32_t n,
        bool *x, int32_t incX,
        bool *y, int32_t incY)
    {
        int32_t dtype = rindow_matlib_dtype_bool;
        rindow_matlib_i_equal(dtype,n,x,incX,y,incY);
    }
};
typedef ::testing::Types<float, double, int32_t> TestTypes;
TYPED_TEST_SUITE(EqualTest, TestTypes);

TYPED_TEST(EqualTest, EqualNormal) {
    const int32_t N = 3;
    const int32_t incX = 1;
    const int32_t incY = 1;
    TypeParam X[N] = {100,10,-1000};
    TypeParam Y[N] = {100,-10,-1000};

    this->test_matlib_equal(N, X, incX, Y, incY);

    TypeParam R1[N] = {1,0,1};
    EXPECT_THAT(R1, ContainerEq(Y));

    // bool
    bool boolX[N] = {true,true,false};
    bool boolY[N] = {true,false,false};

    this->test_matlib_equal(N, boolX, incX, boolY, incY);

    bool boolR1[N] = {1,0,1};
    EXPECT_THAT(boolR1, ContainerEq(boolY));

    EXPECT_EQ(3,sizeof(boolR1));
}

}
