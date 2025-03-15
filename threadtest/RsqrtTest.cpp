#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"
#include <limits>

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class RsqrtTest : public ::testing::Test {
protected:
    virtual void test_matlib_rsqrt(
        int32_t n,
        float alpha,
        float *x,int32_t incX,
        float beta)
    {
        rindow_matlib_s_rsqrt(n,alpha,x,incX,beta);
    }

    virtual void test_matlib_rsqrt(
        int32_t n,
        double alpha,
        double *x,int32_t incX,
        double beta)
    {
        rindow_matlib_d_rsqrt(n,alpha,x,incX,beta);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(RsqrtTest, TestTypes);

TYPED_TEST(RsqrtTest, normal) {
    // X := 1 / sqrt(X)
    const int32_t M = 1;
    const int32_t N = 4;
    const int32_t incX = 1;

    TypeParam X[N] = {1,4,16,64};
    TypeParam alpha = 1;
    TypeParam beta = 0;

    this->test_matlib_rsqrt(N, alpha, X, incX, beta);

    TypeParam R1[N] = {1/(TypeParam)1,1/(TypeParam)2,1/(TypeParam)4,1/(TypeParam)8};
    EXPECT_THAT(R1, ContainerEq(X));
    EXPECT_TRUE(Utils::isclose(N,R1,X));
}
TYPED_TEST(RsqrtTest, alpha_beta) {
    // X := 1 / ( 1 - sqrt(X))
    const int32_t M = 1;
    const int32_t N = 4;
    const int32_t incX = 1;

    TypeParam X[N] = {4,9,16,64};
    TypeParam alpha = -1;
    TypeParam beta = 1;

    this->test_matlib_rsqrt(N, alpha, X, incX, beta);

    TypeParam R1[N] = {1/(1-(TypeParam)2),1/(1-(TypeParam)3),1/(1-(TypeParam)4),1/(1-(TypeParam)8)};
    EXPECT_TRUE(Utils::isclose(N,R1,X));
}
TYPED_TEST(RsqrtTest, not_a_number) {
    const int32_t M = 1;
    const int32_t N = 7;
    const int32_t incX = 1;
    TypeParam inf = std::numeric_limits<TypeParam>::infinity();

    TypeParam X[N] = {4,1,0,-1,inf,-inf,NAN};
    TypeParam alpha = 1;
    TypeParam beta = 0;

    this->test_matlib_rsqrt(N, alpha, X, incX, beta);

    TypeParam zero = 0;
    EXPECT_EQ(0.5,         X[0]);
    EXPECT_EQ(1.0,         X[1]);
    EXPECT_EQ(inf,         X[2]);
    EXPECT_TRUE(std::isnan(X[3]));
    EXPECT_EQ(0,           X[4]);
    EXPECT_TRUE(std::isnan(X[5]));
    EXPECT_TRUE(std::isnan(X[6]));
}
}
