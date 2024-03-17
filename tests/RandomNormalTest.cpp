#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdlib.h>
#include <numeric>

using testing::ContainerEq;

namespace {

template <typename T>
class RandomNormalTest : public ::testing::Test {
protected:
    virtual void test_matlib_randomnormal(
        int32_t n,
        float *x, int32_t incX,
        float mean,
        float scale,
        int32_t seed)
    {
        rindow_matlib_s_randomnormal(n,x,incX,mean,scale,seed);
    }
    virtual void test_matlib_randomnormal(
        int32_t n,
        double *x, int32_t incX,
        double mean,
        double scale,
        int32_t seed)
    {
        rindow_matlib_d_randomnormal(n,x,incX,mean,scale,seed);
    }
    virtual int32_t test_get_dtype(
        float value
    )
    {
        return rindow_matlib_dtype_float32;
    }
    virtual int32_t test_get_dtype(
        double value
    )
    {
        return rindow_matlib_dtype_float64;
    }
    T stddev(int32_t n, T X[])
    {
        T sum = 0.0;
        for (int32_t i = 0; i < n; i++) {
            sum += X[i];
        }
        T avg = sum / n;

        T variance = 0.0;
        for (int32_t i = 0; i < n; i++) {
          variance += (X[i] - avg) * (X[i] - avg);
        }
        variance /= n;

        T stddev_calc = sqrt(variance);

        std::cout << "avg=" << avg << std::endl;
        std::cout << "variance=" << variance << std::endl;
        std::cout << "stddev=" << stddev_calc << std::endl;
        return stddev_calc;
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(RandomNormalTest, TestTypes);

TYPED_TEST(RandomNormalTest, normal) {
    const int32_t M = 1;
    const int32_t N = 1000;
    const int32_t incX = 1;
    const int32_t seed = 0;

    TypeParam mean  = 0.0;
    TypeParam scale = 1.0;
    TypeParam X1[N];
    TypeParam X2[N];

    this->test_matlib_randomnormal(
        N,
        X1, incX,
        mean,
        scale,
        seed
    );
    this->test_matlib_randomnormal(
        N,
        X2, incX,
        mean,
        scale,
        seed
    );
    TypeParam sum1 = std::accumulate(std::begin(X1), std::end(X1), (TypeParam)0);
    TypeParam sum2 = std::accumulate(std::begin(X2), std::end(X2), (TypeParam)0);
    EXPECT_NE(sum1,sum2);

    TypeParam mean1 = sum1/(TypeParam)N;
    TypeParam mean2 = sum2/(TypeParam)N;
    EXPECT_LT(abs(mean1),(TypeParam)0.1);
    EXPECT_LT(abs(mean2),(TypeParam)0.1);

    TypeParam min1 = std::accumulate(std::begin(X1), std::end(X1), X1[0], [&] (TypeParam a, TypeParam b) { return std::min(a,b);});
    TypeParam min2 = std::accumulate(std::begin(X2), std::end(X2), X2[0], [&] (TypeParam a, TypeParam b) { return std::min(a,b);});
    TypeParam max1 = std::accumulate(std::begin(X1), std::end(X1), X1[0], [&] (TypeParam a, TypeParam b) { return std::max(a,b);});
    TypeParam max2 = std::accumulate(std::begin(X2), std::end(X2), X2[0], [&] (TypeParam a, TypeParam b) { return std::max(a,b);});
    EXPECT_LT(min1,0);
    EXPECT_LT(min2,0);
    EXPECT_GT(max1,0);
    EXPECT_GT(max2,0);

    TypeParam stddev_calc = this->stddev(N,X1);
    EXPECT_LT((stddev_calc-scale),0.1);
}
TYPED_TEST(RandomNormalTest, scale) {
    const int32_t M = 1;
    const int32_t N = 10000;
    const int32_t incX = 1;
    const int32_t seed = 0;

    TypeParam mean  = 0.0;
    TypeParam scale = 2.0;
    TypeParam X1[N];
    TypeParam X2[N];

    this->test_matlib_randomnormal(
        N,
        X1, incX,
        mean,
        scale,
        seed
    );
    this->test_matlib_randomnormal(
        N,
        X2, incX,
        mean,
        scale,
        seed
    );
    TypeParam sum1 = std::accumulate(std::begin(X1), std::end(X1), (TypeParam)0);
    TypeParam sum2 = std::accumulate(std::begin(X2), std::end(X2), (TypeParam)0);
    EXPECT_NE(sum1,sum2);

    TypeParam mean1 = sum1/(TypeParam)N;
    TypeParam mean2 = sum2/(TypeParam)N;
    EXPECT_LT(abs(mean1),(TypeParam)0.1);
    EXPECT_LT(abs(mean2),(TypeParam)0.1);

    TypeParam min1 = std::accumulate(std::begin(X1), std::end(X1), X1[0], [&] (TypeParam a, TypeParam b) { return std::min(a,b);});
    TypeParam min2 = std::accumulate(std::begin(X2), std::end(X2), X2[0], [&] (TypeParam a, TypeParam b) { return std::min(a,b);});
    TypeParam max1 = std::accumulate(std::begin(X1), std::end(X1), X1[0], [&] (TypeParam a, TypeParam b) { return std::max(a,b);});
    TypeParam max2 = std::accumulate(std::begin(X2), std::end(X2), X2[0], [&] (TypeParam a, TypeParam b) { return std::max(a,b);});
    EXPECT_LT(min1,0);
    EXPECT_LT(min2,0);
    EXPECT_GT(max1,0);
    EXPECT_GT(max2,0);

    TypeParam stddev_calc = this->stddev(N,X1);
    EXPECT_LT((stddev_calc-scale),0.1);
}
}
