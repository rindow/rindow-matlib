#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdlib.h>
#include <numeric>

using testing::ContainerEq;

namespace {

template <typename T>
class RandomSequenceTest : public ::testing::Test {
protected:
    virtual void test_i_matlib_randomsequence(
        int32_t n,
        int32_t size,
        int32_t dtype,
        void *x, int32_t incX,
        int32_t seed
    )
    {
        rindow_matlib_i_randomsequence(n,size,dtype,x,incX,seed);
    }
    virtual int32_t test_get_dtype(
        int32_t value
    )
    {
        return rindow_matlib_dtype_int32;
    }
    virtual int32_t test_get_dtype(
        uint32_t value
    )
    {
        return rindow_matlib_dtype_uint32;
    }
    virtual bool is_unique(int32_t size, T X[]) {
        for(int32_t i=0;i<size;i++) {
            auto result = std::find(X, X+i, X[i]);
            if(X+i != result) {
                return false;
            }
        }
        return true;
    }

    float chi2(int32_t n, T X[], int32_t density)
    {
        int32_t hist_size = density;
        std::vector<int32_t> histogram(hist_size);
        std::fill(histogram.begin(),histogram.end(),0);
        std::vector<int32_t> xx(n);
        for(int32_t i=0;i<n;++i) {
            xx[i] = (int32_t)X[i];
        }
        int32_t min = std::accumulate(xx.begin(), xx.end(), xx.front(), [&] (int32_t a, int32_t b) { return std::min(a,b);});
        int32_t max = std::accumulate(xx.begin(), xx.end(), xx.front(), [&] (int32_t a, int32_t b) { return std::max(a,b);});

        for(int32_t i=0;i<n;++i) {
            int32_t index = (int32_t)((xx[i]-min)/((max-min)/(T)hist_size));
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

    int32_t runs(int32_t n, T X[])
    {
        int32_t runs_test = 0;
        for(int32_t i=1;i<n;++i) {
            if(X[i-1] < X[i]) {
                ++runs_test;
            }
        }
        std::cout << "runs=" << runs_test << std::endl;
        return runs_test;
    }
};
typedef ::testing::Types<int32_t, uint32_t> TestTypes;
TYPED_TEST_SUITE(RandomSequenceTest, TestTypes);

TYPED_TEST(RandomSequenceTest, normal) {
    const int32_t M = 1;
    const int32_t N = 10000;
    const int32_t size = N;
    const int32_t incX = 1;
    const int32_t seed = 0;
    TypeParam value = 0;
    const int32_t dtype = this->test_get_dtype(value);

    TypeParam X1[N];
    TypeParam X2[N];

    this->test_i_matlib_randomsequence(
        N,
        size,
        dtype,
        X1,incX,
        seed
    );
    this->test_i_matlib_randomsequence(
        N,
        size,
        dtype,
        X2,incX,
        seed
    );
    EXPECT_TRUE(this->is_unique(size,X1));
    EXPECT_TRUE(this->is_unique(size,X2));

    int32_t lower = N/4;
    int32_t upper = N-N/4;
    EXPECT_GT(this->runs(N, X1),lower);
    EXPECT_LT(this->runs(N, X1),upper);
    EXPECT_GT(this->runs(N, X2),lower);
    EXPECT_LT(this->runs(N, X2),upper);
}
}
