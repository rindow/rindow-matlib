#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <complex>

#include "rindow/matlib.h"

using testing::ContainerEq;

using complex64_t = std::complex<float>;

namespace {

template <typename T>
class FillTest : public ::testing::Test {
protected:
    virtual void test_matlib_fill(int32_t n, float *value, float *x, int32_t incX)
    {
        int32_t dtype = rindow_matlib_dtype_float32;
        rindow_matlib_fill(dtype, n, value, x, incX);
    }

    virtual void test_matlib_fill(int32_t n, double *value, double *x, int32_t incX)
    {
        int32_t dtype = rindow_matlib_dtype_float64;
        rindow_matlib_fill(dtype, n, value, x, incX);
    }

    virtual void test_matlib_fill(int32_t n, int32_t *value, int32_t *x, int32_t incX)
    {
        int32_t dtype = rindow_matlib_dtype_int32;
        rindow_matlib_fill(dtype, n, value, x, incX);
    }

    virtual void test_matlib_fill(int32_t n, bool *value, bool *x, int32_t incX)
    {
        int32_t dtype = rindow_matlib_dtype_bool;
        rindow_matlib_fill(dtype, n, value, x, incX);
    }

    virtual void test_matlib_fill(int32_t n, complex64_t *value, complex64_t *x, int32_t incX)
    {
        int32_t dtype = rindow_matlib_dtype_complex64;
        rindow_matlib_fill(dtype, n, value, x, incX);
    }

};
typedef ::testing::Types<float, double, int32_t, bool, complex64_t> TestTypes;
TYPED_TEST_SUITE(FillTest, TestTypes);

TYPED_TEST(FillTest, FillNormal) {
    const int32_t N = 3;
    const int32_t incX = 1;
    TypeParam X[N];
    TypeParam value = (TypeParam)123;

    this->test_matlib_fill(N, &value, X, incX);

    TypeParam R1[N] = {(TypeParam)123, (TypeParam)123, (TypeParam)123};
    EXPECT_THAT(R1, ContainerEq(X));
}

}
