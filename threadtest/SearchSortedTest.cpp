#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"

using testing::ContainerEq;

namespace {

template <typename T>
class SearchSortedTest : public ::testing::Test {
protected:
    virtual void test_matlib_searchsorted(
        int32_t m,
        int32_t n,
        float *a, int32_t ldA,
        float *x, int32_t incX,
        int32_t right,
        int32_t dtype, void *y, int32_t incY)
    {
        return rindow_matlib_s_searchsorted(m,n,a,ldA,x,incX,right,dtype,y,incY);
    }
    virtual void test_matlib_searchsorted(
        int32_t m,
        int32_t n,
        double *a, int32_t ldA,
        double *x, int32_t incX,
        int32_t right,
        int32_t dtype, void *y, int32_t incY)
    {
        return rindow_matlib_d_searchsorted(m,n,a,ldA,x,incX,right,dtype,y,incY);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(SearchSortedTest, TestTypes);

TYPED_TEST(SearchSortedTest, shared) {
    const int32_t indexDtype = rindow_matlib_dtype_int32;
    const int32_t M = 3;
    const int32_t N = 5;
    const int32_t ldA  = 0; // shared mode
    const int32_t incX = 1;
    const int32_t incY = 1;
    const int32_t right = false;

    TypeParam A[N] = {1, 3, 5, 7, 9};
    TypeParam X[M] = {0, 5, 10};
    int32_t Y[M];

    this->test_matlib_searchsorted(
        M,              // int32_t m,
        N,              // int32_t n,
        (TypeParam *)A, // *a,
        ldA,            // int32_t ldA,
        X,              // *x,
        incX,           // int32_t incX,
        right,          // int32_t right,
        indexDtype,     // int32_t index_dtype,
        Y,              // void *y,
        incY            // int32_t incY
    );

    int32_t R1[M] = {0,2,5};
    EXPECT_THAT(R1, ContainerEq(Y));
}
TYPED_TEST(SearchSortedTest, shared_right) {
    const int32_t indexDtype = rindow_matlib_dtype_int32;
    const int32_t M = 3;
    const int32_t N = 5;
    const int32_t ldA  = 0; // shared mode
    const int32_t incX = 1;
    const int32_t incY = 1;
    const int32_t right = true;

    TypeParam A[N] = {1, 3, 5, 7, 9};
    TypeParam X[M] = {0, 5, 10};
    int32_t Y[M];

    this->test_matlib_searchsorted(
        M,              // int32_t m,
        N,              // int32_t n,
        (TypeParam *)A, // *a,
        ldA,            // int32_t ldA,
        X,              // *x,
        incX,           // int32_t incX,
        right,          // int32_t right,
        indexDtype,     // int32_t index_dtype,
        Y,              // void *y,
        incY            // int32_t incY
    );

    int32_t R1[M] = {0, 3, 5};
    EXPECT_THAT(R1, ContainerEq(Y));
}
TYPED_TEST(SearchSortedTest, individual) {
    const int32_t indexDtype = rindow_matlib_dtype_int32;
    const int32_t M = 3;
    const int32_t N = 5;
    const int32_t ldA  = 5; // individual mode
    const int32_t incX = 1;
    const int32_t incY = 1;
    const int32_t right = false;

    TypeParam A[M][N] = {
        {1,   3,  5,   7,   9},
        {1,   2,  3,   4,   5},
        {0, 100, 20, 300, 400}
    };
    TypeParam X[M] = {0, 5, 10};
    int32_t Y[M];

    this->test_matlib_searchsorted(
        M,              // int32_t m,
        N,              // int32_t n,
        (TypeParam *)A, // *a,
        ldA,            // int32_t ldA,
        X,              // *x,
        incX,           // int32_t incX,
        right,          // int32_t right,
        indexDtype,     // int32_t index_dtype,
        Y,              // void *y,
        incY            // int32_t incY
    );

    int32_t R1[M] = {0, 4, 1};
    EXPECT_THAT(R1, ContainerEq(Y));
}
TYPED_TEST(SearchSortedTest, individual_right) {
    const int32_t indexDtype = rindow_matlib_dtype_int32;
    const int32_t M = 3;
    const int32_t N = 5;
    const int32_t ldA  = 5; // individual mode
    const int32_t incX = 1;
    const int32_t incY = 1;
    const int32_t right = true;

    TypeParam A[M][N] = {
        {1,   3,   5,   7,   9},
        {1,   2,   3,   4,   5},
        {0, 100, 200, 300, 400}
    };
    TypeParam X[M] = {0, 5, 10};
    int32_t Y[M];

    this->test_matlib_searchsorted(
        M,              // int32_t m,
        N,              // int32_t n,
        (TypeParam *)A, // *a,
        ldA,            // int32_t ldA,
        X,              // *x,
        incX,           // int32_t incX,
        right,          // int32_t right,
        indexDtype,     // int32_t index_dtype,
        Y,              // void *y,
        incY            // int32_t incY
    );

    int32_t R1[M] = {0, 5, 1};
    EXPECT_THAT(R1, ContainerEq(Y));
}
}
