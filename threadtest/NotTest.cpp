#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class NotTest : public ::testing::Test {
protected:
    virtual void test_matlib_not(
        int32_t n,
        float *x, int32_t incX)
    {
        rindow_matlib_s_not(n, x, incX);
    }

    virtual void test_matlib_not(
        int32_t n,
        double *x, int32_t incX)
    {
        rindow_matlib_d_not(n, x, incX);
    }

    virtual void test_matlib_not(
        int32_t n,
        int32_t *x, int32_t incX)
    {
        int32_t dtype = rindow_matlib_dtype_int32;
        rindow_matlib_i_not(dtype, n, x, incX);
    }

    virtual void test_matlib_not(
        int32_t n,
        bool *x, int32_t incX)
    {
        int32_t dtype = rindow_matlib_dtype_bool;
        rindow_matlib_i_not(dtype, n, x, incX);
    }
};
typedef ::testing::Types<float, double, int32_t> TestTypes;
TYPED_TEST_SUITE(NotTest, TestTypes);

TYPED_TEST(NotTest, normal) {
    const int32_t M = 1;
    const int32_t N = 3;
    const int32_t incX = 1;

    TypeParam X1[N] = {100, 0, -1000};
    this->test_matlib_not(N, X1, incX);
    TypeParam R1[N] = { 0, 1, 0 };
    EXPECT_THAT(R1, ContainerEq(X1));

    bool X2[N] = {true,true,false};
    this->test_matlib_not(N, X2, incX);
    bool R2[N] = {false,false,true};
    EXPECT_THAT(R2, ContainerEq(X2));
}

}
