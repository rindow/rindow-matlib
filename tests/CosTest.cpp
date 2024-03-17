#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class CosTest : public ::testing::Test {
protected:
    virtual void test_matlib_cos(int32_t n, float *x, int32_t incX)
    {
        rindow_matlib_s_cos(n, x, incX);
    }

    virtual void test_matlib_cos(int32_t n, double *x, int32_t incX)
    {
        rindow_matlib_d_cos(n, x, incX);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(CosTest, TestTypes);

TYPED_TEST(CosTest, normal) {
    const int32_t M = 1;
    const int32_t N = 4;
    const int32_t incX = 1;
    TypeParam X[N] = {1.0, 2.0, 4.0, 9.0};

    this->test_matlib_cos(N, X, incX);

    TypeParam R1[N] = {cosf(1),cosf(2),cosf(4),cosf(9)};
    EXPECT_TRUE(Utils::isclose(N,R1,X));
}

}
