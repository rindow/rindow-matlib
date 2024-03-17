#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class SinTest : public ::testing::Test {
protected:
    virtual void test_matlib_sin(int32_t n, float *x, int32_t incX)
    {
        rindow_matlib_s_sin(n, x, incX);
    }

    virtual void test_matlib_sin(int32_t n, double *x, int32_t incX)
    {
        rindow_matlib_d_sin(n, x, incX);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(SinTest, TestTypes);

TYPED_TEST(SinTest, normal) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam X[N] = {1,2,3,4,5,6};

    this->test_matlib_sin(N, X, incX);

    TypeParam R1[N] = {sinf(1),sinf(2),sinf(3),sinf(4),sinf(5),sinf(6)};
    EXPECT_TRUE(Utils::isclose(N,R1,X));
}

}
