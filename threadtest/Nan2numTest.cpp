#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <limits>
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class Nan2numTest : public ::testing::Test {
protected:
    virtual void test_matlib_nan2num(
        int32_t n,
        float *x, int32_t incX,
        float alpha)
    {
        rindow_matlib_s_nan2num(n, x, incX, alpha);
    }

    virtual void test_matlib_nan2num(
        int32_t n,
        double *x, int32_t incX,
        double alpha)
    {
        rindow_matlib_d_nan2num(n, x, incX, alpha);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(Nan2numTest, TestTypes);

TYPED_TEST(Nan2numTest, normal) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;

    TypeParam alpha = 0;
    TypeParam X[N] = {NAN,2,NAN,4,NAN,6};

    this->test_matlib_nan2num(N, X, incX, alpha);

    TypeParam R1[N] = {0,2,0,4,0,6};
    EXPECT_THAT(R1, ContainerEq(X));
}
TYPED_TEST(Nan2numTest, alphaone) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;

    TypeParam alpha = 1;
    TypeParam X[N] = {NAN,2,NAN,4,NAN,6};

    this->test_matlib_nan2num(N, X, incX, alpha);

    TypeParam R1[N] = {1,2,1,4,1,6};
    EXPECT_THAT(R1, ContainerEq(X));
}

}
