#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <limits>
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class TanhTest : public ::testing::Test {
protected:
    virtual void test_matlib_tanh(int32_t n, float *x, int32_t incX)
    {
        rindow_matlib_s_tanh(n, x, incX);
    }

    virtual void test_matlib_tanh(int32_t n, double *x, int32_t incX)
    {
        rindow_matlib_d_tanh(n, x, incX);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(TanhTest, TestTypes);

TYPED_TEST(TanhTest, Normal) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam X[N] = {1, 2, 3, 4, 5, 6};

    this->test_matlib_tanh(N, X, incX);

    TypeParam R1[N] = {tanhf(1),tanhf(2),tanhf(3),tanhf(4),tanhf(5),tanhf(6)};

    EXPECT_TRUE(Utils::isclose(N,R1,X));
}
TYPED_TEST(TanhTest, non_a_number) {
    const int32_t M = 1;
    const int32_t N = 4;
    const int32_t incX = 1;
    TypeParam inf = std::numeric_limits<TypeParam>::infinity();

    TypeParam X[N] = {0, inf, -inf, NAN};

    this->test_matlib_tanh(N, X, incX);

    TypeParam zero = 0;
    EXPECT_EQ(zero, X[0]);
    EXPECT_EQ(1,    X[1]);
    EXPECT_EQ(-1,   X[2]);
    EXPECT_TRUE(std::isnan( X[3]));
}

}
