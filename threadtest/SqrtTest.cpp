#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"
#include <limits>

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class SqrtTest : public ::testing::Test {
protected:
    virtual void test_matlib_sqrt(
        int32_t n,
        float *x, int32_t incX)
    {
        rindow_matlib_s_sqrt(n, x, incX);
    }

    virtual void test_matlib_sqrt(
        int32_t n,
        double *x, int32_t incX)
    {
        rindow_matlib_d_sqrt(n, x, incX);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(SqrtTest, TestTypes);

TYPED_TEST(SqrtTest, normal) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam X[N] = {1,4,9,16,25,36};

    this->test_matlib_sqrt(N, X, incX);

    TypeParam R1[N] = {1,2,3,4,5,6};
    EXPECT_TRUE(Utils::isclose(N,R1,X));
}
TYPED_TEST(SqrtTest, non_a_number) {
    const int32_t M = 1;
    const int32_t N = 7;
    const int32_t incX = 1;
    TypeParam inf = std::numeric_limits<TypeParam>::infinity();

    TypeParam X[N] = {4,1,0,-1,inf,-inf,NAN};

    this->test_matlib_sqrt(N, X, incX);

    EXPECT_EQ(2.0,     X[0]);
    EXPECT_EQ(1.0,     X[1]);
    EXPECT_EQ(0.0,     X[2]);
    EXPECT_TRUE(std::isnan( X[3])); // -NAN
    EXPECT_TRUE(inf==   X[4]);
    EXPECT_TRUE(std::isnan( X[5]));
    EXPECT_TRUE(std::isnan( X[6]));
}

}
