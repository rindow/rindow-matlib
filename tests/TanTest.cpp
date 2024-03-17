#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <limits>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class TanTest : public ::testing::Test {
protected:
    virtual void test_matlib_tan(int32_t n, float *x, int32_t incX)
    {
        rindow_matlib_s_tan(n, x, incX);
    }

    virtual void test_matlib_tan(int32_t n, double *x, int32_t incX)
    {
        rindow_matlib_d_tan(n, x, incX);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(TanTest, TestTypes);

TYPED_TEST(TanTest, normal) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam X[N] = {1, 2, 3, 4, 5, 6};

    this->test_matlib_tan(N, X, incX);

    TypeParam R1[N] = {tanf(1),tanf(2),tanf(3),tanf(4),tanf(5),tanf(6)};

    EXPECT_TRUE(Utils::isclose(N,R1,X));
}
TYPED_TEST(TanTest, non_a_number) {
    const int32_t M = 1;
    const int32_t N = 5;
    const int32_t incX = 1;
    TypeParam inf = std::numeric_limits<TypeParam>::infinity();

    TypeParam X[N] = {
        0,
        (TypeParam)M_PI,
        (TypeParam)M_PI/2,
        -(TypeParam)M_PI/2,
        NAN
    };

    this->test_matlib_tan(N, X, incX);

    TypeParam zero = 0;
    EXPECT_EQ(X[0],     0);
    EXPECT_LT(abs(X[1]),1e-7);  // not zero
    EXPECT_GT(abs(X[2]),1e+7);  // not inf
    EXPECT_GT(abs(X[3]),1e+7);  // not inf
    EXPECT_TRUE(std::isnan( X[4]));
}

}
