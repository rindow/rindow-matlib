#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <limits>

using testing::ContainerEq;

namespace {

template <typename T>
class IsnanTest : public ::testing::Test {
protected:
    virtual void test_matlib_isnan(int32_t n, float *x, int32_t incX)
    {
        return rindow_matlib_s_isnan(n, x, incX);
    }

    virtual void test_matlib_isnan(int32_t n, double *x, int32_t incX)
    {
        return rindow_matlib_d_isnan(n, x, incX);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(IsnanTest, TestTypes);

TYPED_TEST(IsnanTest, normal) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam X[N] = {NAN,2,NAN,4,NAN,6};
    TypeParam q_nan = std::numeric_limits<TypeParam>::quiet_NaN();
    TypeParam s_nan = std::numeric_limits<TypeParam>::signaling_NaN();

    X[0] = q_nan;
    X[1] = 2;
    X[2] = s_nan;
    X[3] = 4;
    X[4] = s_nan;
    X[5] = 6;

    this->test_matlib_isnan(N, X, incX);

    TypeParam R1[N] = { 1,0,1,0,1,0 };
    EXPECT_THAT(R1, ContainerEq(X));
}

}
