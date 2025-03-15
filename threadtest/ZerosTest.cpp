#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <complex>

#include "rindow/matlib.h"

using testing::ContainerEq;

using complex64_t = std::complex<float>;

namespace {

template <typename T>
class ZerosTest : public ::testing::Test {
protected:
    virtual void test_matlib_zeros(int32_t n, float *x, int32_t incX)
    {
        rindow_matlib_s_zeros(n, x, incX);
    }

    virtual void test_matlib_zeros(int32_t n, double *x, int32_t incX)
    {
        rindow_matlib_d_zeros(n, x, incX);
    }

    virtual void test_matlib_zeros(int32_t n, int32_t *x, int32_t incX)
    {
        int32_t dtype = rindow_matlib_dtype_int32;
        rindow_matlib_i_zeros(dtype, n, x, incX);
    }

    virtual void test_matlib_zeros(int32_t n, bool *x, int32_t incX)
    {
        int32_t dtype = rindow_matlib_dtype_bool;
        rindow_matlib_i_zeros(dtype, n, x, incX);
    }

    virtual void test_matlib_zeros(int32_t n, complex64_t *x, int32_t incX)
    {
        int32_t dtype = rindow_matlib_dtype_complex64;
        rindow_matlib_i_zeros(dtype, n, x, incX);
    }

};
typedef ::testing::Types<float, double, int32_t, bool, complex64_t> TestTypes;
TYPED_TEST_SUITE(ZerosTest, TestTypes);

TYPED_TEST(ZerosTest, FillNormal) {
    const int32_t N = 9;
    const int32_t incX = 1;

    TypeParam X[N] = {
        (TypeParam)99,(TypeParam)99,(TypeParam)99,
        (TypeParam)99,(TypeParam)99,(TypeParam)99,
        (TypeParam)99,(TypeParam)99,(TypeParam)99
    };

    this->test_matlib_zeros(N, X, incX);

    TypeParam R1[N] = {0,0,0,0,0,0,0,0,0};
    EXPECT_THAT(R1, ContainerEq(X));
}

TYPED_TEST(ZerosTest, FillStep2) {
    const int32_t N = 4;
    const int32_t incX = 2;

    TypeParam X[N*incX] = {
        (TypeParam)99,(TypeParam)99,(TypeParam)99,
        (TypeParam)99,(TypeParam)99,(TypeParam)99,
        (TypeParam)99,(TypeParam)99
    };

    this->test_matlib_zeros(N, X, incX);

    TypeParam R1[N*incX] = {
        0,(TypeParam)99,0,(TypeParam)99,
        0,(TypeParam)99,0,(TypeParam)99
    };
    EXPECT_THAT(R1, ContainerEq(X));
}

}
