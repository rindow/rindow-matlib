#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class SoftmaxTest : public ::testing::Test {
protected:
    virtual void test_matlib_softmax(
        int32_t m, int32_t n,
        float *a, int32_t ldA)
    {
        rindow_matlib_s_softmax(m,n,a,ldA);
    }

    virtual void test_matlib_softmax(
        int32_t m, int32_t n,
        double *a, int32_t ldA)
    {
        rindow_matlib_d_softmax(m,n,a,ldA);
    }

};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(SoftmaxTest, TestTypes);

TYPED_TEST(SoftmaxTest, normal) {
    const int32_t M = 5;
    const int32_t N = 5;
    const int32_t ldA = N;

    TypeParam A[M][N] = {
        {-1.0,-0.5,0.0,0.5,1.0},
        {-1.0,-0.5,0.0,0.5,1.0},
        {-1.0,-0.5,0.0,0.5,1.0},
        {-1.0,-0.5,0.0,0.5,1.0},
        {-1.0,-0.5,0.0,0.5,1.0},
    };

    this->test_matlib_softmax(M, N, (TypeParam*)A, ldA);

    TypeParam R1[M][N] = {
        {0.05801f ,0.09564f ,0.15769f, 0.25999f, 0.42865f},
        {0.05801f ,0.09564f ,0.15769f, 0.25999f, 0.42865f},
        {0.05801f ,0.09564f ,0.15769f, 0.25999f, 0.42865f},
        {0.05801f ,0.09564f ,0.15769f, 0.25999f, 0.42865f},
        {0.05801f ,0.09564f ,0.15769f, 0.25999f, 0.42865f},
    };
    EXPECT_TRUE(Utils::isclose(M*N,(TypeParam*)R1,(TypeParam*)A));
}

}
