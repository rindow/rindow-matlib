#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class BandpartTest : public ::testing::Test {
protected:
    virtual void test_matlib_bandpart(
        int32_t m, int32_t n, int32_t k,
        float *a,int32_t lower, int32_t upper)
    {
        rindow_matlib_s_bandpart(m, n, k, a, lower, upper);
    }

    virtual void test_matlib_bandpart(
        int32_t m, int32_t n, int32_t k,
        double *a,int32_t lower, int32_t upper)
    {
        rindow_matlib_d_bandpart(m, n, k, a, lower, upper);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(BandpartTest, TestTypes);

TYPED_TEST(BandpartTest, Array3DUpper) {
    const int32_t M = 2;
    const int32_t N = 5;
    const int32_t K = 5;
    const int32_t lower = 0;
    const int32_t upper = -1;

    TypeParam A[M][N][K];
    Utils::ones(M*N*K,(TypeParam*)A);

    this->test_matlib_bandpart(M,N,K,(TypeParam*)A,lower,upper);
    TypeParam R1[M][N][K] = {
        {{1,1,1,1,1},
         {0,1,1,1,1},
         {0,0,1,1,1},
         {0,0,0,1,1},
         {0,0,0,0,1}},

        {{1,1,1,1,1},
         {0,1,1,1,1},
         {0,0,1,1,1},
         {0,0,0,1,1},
         {0,0,0,0,1}}
    };
    EXPECT_THAT(R1, ContainerEq(A));
}

TYPED_TEST(BandpartTest, Array2DLower) {
    const int32_t M = 1;
    const int32_t N = 5;
    const int32_t K = 5;
    const int32_t lower = -1;
    const int32_t upper = 0;

    TypeParam A[M][N][K];
    Utils::ones(M*N*K,(TypeParam*)A);

    this->test_matlib_bandpart(M,N,K,(TypeParam*)A,lower,upper);
    TypeParam R1[M][N][K] = {
        {{1,0,0,0,0},
         {1,1,0,0,0},
         {1,1,1,0,0},
         {1,1,1,1,0},
         {1,1,1,1,1}},
    };
    EXPECT_THAT(R1, ContainerEq(A));
}

TYPED_TEST(BandpartTest, Array2DIdentity) {
    const int32_t M = 1;
    const int32_t N = 5;
    const int32_t K = 5;
    const int32_t lower = 0;
    const int32_t upper = 0;

    TypeParam A[M][N][K];
    Utils::ones(M*N*K,(TypeParam*)A);

    this->test_matlib_bandpart(M,N,K,(TypeParam*)A,lower,upper);
    TypeParam R1[M][N][K] = {
        {{1,0,0,0,0},
         {0,1,0,0,0},
         {0,0,1,0,0},
         {0,0,0,1,0},
         {0,0,0,0,1}},
    };
    EXPECT_THAT(R1, ContainerEq(A));
}

TYPED_TEST(BandpartTest, Array2DUpper1) {
    const int32_t M = 1;
    const int32_t N = 5;
    const int32_t K = 5;
    const int32_t lower = 0;
    const int32_t upper = 1;

    TypeParam A[M][N][K];
    Utils::ones(M*N*K,(TypeParam*)A);

    this->test_matlib_bandpart(M,N,K,(TypeParam*)A,lower,upper);
    TypeParam R1[M][N][K] = {
        {{1,1,0,0,0},
         {0,1,1,0,0},
         {0,0,1,1,0},
         {0,0,0,1,1},
         {0,0,0,0,1}},
    };
    EXPECT_THAT(R1, ContainerEq(A));
}

TYPED_TEST(BandpartTest, Array2DLower1) {
    const int32_t M = 1;
    const int32_t N = 5;
    const int32_t K = 5;
    const int32_t lower = 1;
    const int32_t upper = 0;

    TypeParam A[M][N][K];
    Utils::ones(M*N*K,(TypeParam*)A);

    this->test_matlib_bandpart(M,N,K,(TypeParam*)A,lower,upper);
    TypeParam R1[M][N][K] = {
        {{1,0,0,0,0},
         {1,1,0,0,0},
         {0,1,1,0,0},
         {0,0,1,1,0},
         {0,0,0,1,1}},
    };
    EXPECT_THAT(R1, ContainerEq(A));
}
}
