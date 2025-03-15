#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class SquareTest : public ::testing::Test {
protected:
    virtual void test_matlib_square(
        int32_t n,
        float *x, int32_t incX)
    {
        rindow_matlib_s_square(n, x, incX);
    }

    virtual void test_matlib_square(
        int32_t n,
        double *x, int32_t incX)
    {
        rindow_matlib_d_square(n, x, incX);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(SquareTest, TestTypes);

TYPED_TEST(SquareTest, normal) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam X[N] = {1,2,3,4,5,6};

    this->test_matlib_square(N, X, incX);

    TypeParam R1[N] = {1,4,9,16,25,36};
    EXPECT_TRUE(Utils::isclose(N,R1,X));
}

}
