#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <limits>
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class LogTest : public ::testing::Test {
protected:
    virtual void test_matlib_log(int32_t n, float *x, int32_t incX)
    {
        rindow_matlib_s_log(n, x, incX);
    }

    virtual void test_matlib_log(int32_t n, double *x, int32_t incX)
    {
        rindow_matlib_d_log(n, x, incX);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(LogTest, TestTypes);

TYPED_TEST(LogTest, Normal) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam X[N] = {1, 2, 3, 4, 5, 6};

    this->test_matlib_log(N, X, incX);

    TypeParam R1[N] = {logf(1),logf(2),logf(3),logf(4),logf(5),logf(6)};

    EXPECT_TRUE(Utils::isclose(N,R1,X));
}
TYPED_TEST(LogTest, non_a_number) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam inf = std::numeric_limits<TypeParam>::infinity();

    TypeParam X[N] = {1, 0, -1, inf, -inf, NAN};

    this->test_matlib_log(N, X, incX);

    TypeParam zero = 0;
    EXPECT_EQ(zero,     X[0]);
    EXPECT_TRUE(-inf==  X[1]);
    EXPECT_TRUE(std::isnan( X[2]));
    EXPECT_TRUE(inf==   X[3]);
    EXPECT_TRUE(std::isnan( X[4]));
    EXPECT_TRUE(std::isnan( X[5]));
}

}
