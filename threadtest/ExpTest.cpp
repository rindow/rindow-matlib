#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class ExpTest : public ::testing::Test {
protected:
    virtual void test_matlib_exp(int32_t n, float *x, int32_t incX)
    {
        rindow_matlib_s_exp(n, x, incX);
    }

    virtual void test_matlib_exp(int32_t n, double *x, int32_t incX)
    {
        rindow_matlib_d_exp(n, x, incX);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(ExpTest, TestTypes);

TYPED_TEST(ExpTest, ExpNormal) {
    const int32_t M = 1;
    const int32_t N = 4;
    const int32_t incX = 1;
    TypeParam X[N] = {1.0, 2.0, 4.0, 9.0};

    this->test_matlib_exp(N, X, incX);

    TypeParam R1[N] = {expf(1),expf(2),expf(4),expf(9)};
    EXPECT_TRUE(Utils::isclose(N,R1,X));
}

}
