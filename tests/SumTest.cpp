#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <limits>

using testing::ContainerEq;

namespace {

template <typename T>
class SumTest : public ::testing::Test {
protected:
    virtual float test_matlib_sum(
        int32_t n,
        float *x, int32_t incX)
    {
        return rindow_matlib_s_sum(n,x,incX);
    }

    virtual double test_matlib_sum(
        int32_t n,
        double *x, int32_t incX)
    {
        return rindow_matlib_d_sum(n,x,incX);
    }

    virtual int64_t test_matlib_sum(
        int32_t n,
        int32_t *x, int32_t incX)
    {
        int32_t dtype = rindow_matlib_dtype_int32;
        return rindow_matlib_i_sum(dtype,n,x,incX);
    }

    virtual int64_t test_matlib_sum(
        int32_t n,
        bool *x, int32_t incX)
    {
        int32_t dtype = rindow_matlib_dtype_bool;
        return rindow_matlib_i_sum(dtype,n,x,incX);
    }

    virtual int32_t test_get_dtype(
        float value
    )
    {
        return rindow_matlib_dtype_float32;
    }
    virtual int32_t test_get_dtype(
        double value
    )
    {
        return rindow_matlib_dtype_float64;
    }
    virtual int32_t test_get_dtype(
        int32_t value
    )
    {
        return rindow_matlib_dtype_int32;
    }
};
typedef ::testing::Types<float, double, int32_t> TestTypes;
TYPED_TEST_SUITE(SumTest, TestTypes);

TYPED_TEST(SumTest, normal) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;

    TypeParam X[N] = {1,2,-3,-4,5,-6};
    //TypeParam inf = std::numeric_limits<TypeParam>::infinity();

    int32_t dtype = this->test_get_dtype(X[0]);
    if(dtype==rindow_matlib_dtype_int32) {
        int64_t sum = (int64_t)this->test_matlib_sum(N, X, incX);
        EXPECT_EQ((1+2-3-4+5-6), sum);
    } else {
        TypeParam sum = (TypeParam)this->test_matlib_sum(N, X, incX);
        EXPECT_EQ((1+2-3-4+5-6), sum);
    }

    // bool
    bool boolX[N] = {true,false,true,false,true,false};
    int64_t sumbool = this->test_matlib_sum(N, boolX, incX);
    EXPECT_EQ(3, sumbool);
}
}
