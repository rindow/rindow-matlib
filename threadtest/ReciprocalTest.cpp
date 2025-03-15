#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"
#include <limits>

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class ReciprocalTest : public ::testing::Test {
protected:
    virtual void test_matlib_reciprocal(
        int32_t n,
        float *x, int32_t incX,
        float alpha,
        float beta)
    {
        rindow_matlib_s_reciprocal(n,x,incX,alpha,beta);
    }

    virtual void test_matlib_reciprocal(
        int32_t n,
        double *x, int32_t incX,
        double alpha,
        double beta)
    {
        rindow_matlib_d_reciprocal(n,x,incX,alpha,beta);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(ReciprocalTest, TestTypes);

TYPED_TEST(ReciprocalTest, normal) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;

    TypeParam X[N] = {1,2,4, 8,16,32};
    TypeParam alpha = 1;
    TypeParam beta = 0;

    this->test_matlib_reciprocal(N, X, incX, alpha, beta);

    TypeParam R1[N] = {1,0.5,0.25,  0.125,0.0625,0.03125};
    EXPECT_TRUE(Utils::isclose(N,R1,X));
}
TYPED_TEST(ReciprocalTest, beta) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;

    TypeParam X[N] = {0,1,3, 7,15,31};
    TypeParam alpha = 1;
    TypeParam beta = 1;

    this->test_matlib_reciprocal(N, X, incX, alpha, beta);

    TypeParam R1[N] = {1,0.5,0.25,  0.125,0.0625,0.03125};
    EXPECT_TRUE(Utils::isclose(N,R1,X));
}
TYPED_TEST(ReciprocalTest, alpha_beta) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;

    TypeParam X[N] = {31,30,28, 24,16,0};
    TypeParam alpha = -1;
    TypeParam beta = 32;

    this->test_matlib_reciprocal(N, X, incX, alpha, beta);

    TypeParam R1[N] = {1,0.5,0.25,  0.125,0.0625, 0.03125};
    EXPECT_TRUE(Utils::isclose(N,R1,X));
}
TYPED_TEST(ReciprocalTest, not_a_number) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam inf = std::numeric_limits<TypeParam>::infinity();

    TypeParam X[N] = {4,2,0, inf,-inf,NAN};
    TypeParam alpha = 1;
    TypeParam beta = 0;

    this->test_matlib_reciprocal(N, X, incX, alpha, beta);

    TypeParam zero = 0;
    EXPECT_EQ(0.25,        X[0]);
    EXPECT_EQ(0.5,         X[1]);
    EXPECT_TRUE(inf==      X[2]);
    EXPECT_EQ(0,           X[3]);
    EXPECT_EQ(0,           X[4]);
    EXPECT_TRUE(std::isnan(X[5]));
}
}
