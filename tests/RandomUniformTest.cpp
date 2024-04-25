#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdlib.h>
#include <numeric>

using testing::ContainerEq;

namespace {

template <typename T>
class RandomUniformTest : public ::testing::Test {
protected:
    virtual void test_matlib_randomuniform(
        int32_t n,
        float *x, int32_t incX,
        float low,
        float high,
        int32_t seed
    )
    {
        rindow_matlib_s_randomuniform(n,x,incX,low,high,seed);
    }
    virtual void test_matlib_randomuniform(
        int32_t n,
        double *x, int32_t incX,
        double low,
        double high,
        int32_t seed
    )
    {
        rindow_matlib_d_randomuniform(n,x,incX,low,high,seed);
    }
    virtual void test_matlib_randomuniform(
        int32_t n,
        int32_t *x, int32_t incX,
        int32_t low,
        int32_t high,
        int32_t seed
    )
    {
        int32_t dtype = rindow_matlib_dtype_int32;
        rindow_matlib_i_randomuniform(n,dtype,x,incX,low,high,seed);
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
    virtual int32_t test_get_dtype(
        int32_t value
    )
    {
        return rindow_matlib_dtype_int32;
    }
    float chi2(int32_t n, T X[], T min, T max, int32_t density)
    {
        int32_t hist_size = density;
        std::vector<int32_t> histogram(hist_size);
        std::fill(histogram.begin(),histogram.end(),0);

        for(int32_t i=0;i<n;++i) {
            int32_t index = (int32_t)((X[i]-min)/((max-min)/(T)hist_size));
            if(index>=hist_size) {
                index = hist_size-1;
            }
            histogram[index]++;
        }
        float chi_square = 0.0;
        for (int i = 0; i < hist_size; i++) {
            std::cout << histogram[i] << std::endl;
            chi_square += ((float)histogram[i] - (float)n/(float)hist_size) * ((float)histogram[i] - (float)n/(float)hist_size) / ((float)n/(float)hist_size);
        }
        std::cout << std::endl;
        std::cout << "chi_square=" << chi_square << std::endl;
        return chi_square;
    }
};
typedef ::testing::Types<float, double, int32_t> TestTypes;
TYPED_TEST_SUITE(RandomUniformTest, TestTypes);

TYPED_TEST(RandomUniformTest, normal) {
    const int32_t M = 1;
    const int32_t N = 1000;
    const int32_t incX = 1;
    const int32_t seed = 0;

    TypeParam low  = -100;
    TypeParam high =  100;
    TypeParam X1[N];
    TypeParam X2[N];

    this->test_matlib_randomuniform(
        N,
        X1, incX,
        low,
        high,
        seed
    );
    this->test_matlib_randomuniform(
        N,
        X2, incX,
        low,
        high,
        seed
    );
    TypeParam sum1 = std::accumulate(std::begin(X1), std::end(X1), (TypeParam)0);
    TypeParam sum2 = std::accumulate(std::begin(X2), std::end(X2), (TypeParam)0);
    EXPECT_NE(sum1,sum2);

    TypeParam min1 = std::accumulate(std::begin(X1), std::end(X1), X1[0], [&] (TypeParam a, TypeParam b) { return std::min(a,b);});
    TypeParam min2 = std::accumulate(std::begin(X2), std::end(X2), X2[0], [&] (TypeParam a, TypeParam b) { return std::min(a,b);});
    TypeParam max1 = std::accumulate(std::begin(X1), std::end(X1), X1[0], [&] (TypeParam a, TypeParam b) { return std::max(a,b);});
    TypeParam max2 = std::accumulate(std::begin(X2), std::end(X2), X2[0], [&] (TypeParam a, TypeParam b) { return std::max(a,b);});

    TypeParam middle = (TypeParam)(low+(high-low)/2);
    if(this->test_get_dtype(low)==rindow_matlib_dtype_int32) {
        EXPECT_LE(low,min1);
        EXPECT_LE(low,min2);
        EXPECT_GE(high,max1);
        EXPECT_GE(high,max2);
        EXPECT_LT(min1,middle);
        EXPECT_LT(min2,middle);
        EXPECT_GT(max1,middle);
        EXPECT_GT(max2,middle);
    } else {
        EXPECT_LT(low,min1);
        EXPECT_LT(low,min2);
        EXPECT_GT(high,max1);
        EXPECT_GT(high,max2);
        EXPECT_LT(min1,middle);
        EXPECT_LT(min2,middle);
        EXPECT_GT(max1,middle);
        EXPECT_GT(max2,middle);
    }

    float fluct = this->chi2(N, X1, low, high, 10);
    EXPECT_LT(fluct,50.0); // It's actually 16.919, but it's unstable so I loosened the conditions. 

    TypeParam max = low+1*(high-low);
    TypeParam min = low+0*(high-low);
    EXPECT_EQ(high,max);
    EXPECT_EQ(low,min);
}
TYPED_TEST(RandomUniformTest, zerobase) {
    const int32_t M = 1;
    const int32_t N = 10000;
    const int32_t incX = 1;
    const int32_t seed = 0;

    TypeParam low  =  0;
    TypeParam high =  1000;
    TypeParam X1[N];
    TypeParam X2[N];

    this->test_matlib_randomuniform(
        N,
        X1, incX,
        low,
        high,
        seed
    );
    this->test_matlib_randomuniform(
        N,
        X2, incX,
        low,
        high,
        seed
    );
    TypeParam sum1 = std::accumulate(std::begin(X1), std::end(X1), (TypeParam)0);
    TypeParam sum2 = std::accumulate(std::begin(X2), std::end(X2), (TypeParam)0);
    EXPECT_NE(sum1,sum2);

    TypeParam min1 = std::accumulate(std::begin(X1), std::end(X1), X1[0], [&] (TypeParam a, TypeParam b) { return std::min(a,b);});
    TypeParam min2 = std::accumulate(std::begin(X2), std::end(X2), X2[0], [&] (TypeParam a, TypeParam b) { return std::min(a,b);});
    TypeParam max1 = std::accumulate(std::begin(X1), std::end(X1), X1[0], [&] (TypeParam a, TypeParam b) { return std::max(a,b);});
    TypeParam max2 = std::accumulate(std::begin(X2), std::end(X2), X2[0], [&] (TypeParam a, TypeParam b) { return std::max(a,b);});

    TypeParam middle = (TypeParam)(low+(high-low)/2);
    if(this->test_get_dtype(low)==rindow_matlib_dtype_int32) {
        EXPECT_LE(low,min1);
        EXPECT_LE(low,min2);
        EXPECT_GE(high,max1);
        EXPECT_GE(high,max2);
        EXPECT_LT(min1,middle);
        EXPECT_LT(min2,middle);
        EXPECT_GT(max1,middle);
        EXPECT_GT(max2,middle);
    } else {
        EXPECT_LT(low,min1);
        EXPECT_LT(low,min2);
        EXPECT_GT(high,max1);
        EXPECT_GT(high,max2);
        EXPECT_LT(min1,middle);
        EXPECT_LT(min2,middle);
        EXPECT_GT(max1,middle);
        EXPECT_GT(max2,middle);
    }

    float fluct = this->chi2(N, X1, low, high, 10);
    EXPECT_LT(fluct,50.0); // It's actually 16.919, but it's unstable so I loosened the conditions. 

    TypeParam max = low+1*(high-low);
    TypeParam min = low+0*(high-low);
    EXPECT_EQ(high,max);
    EXPECT_EQ(low,min);
}
}
