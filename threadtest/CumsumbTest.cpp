#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdbool.h>
#include <math.h>

using testing::ContainerEq;

namespace {

template <typename T>
class CumsumbTest : public ::testing::Test {
protected:
    virtual void test_matlib_cumsumb(
        int32_t m,
        int32_t n,
        int32_t k,
        float *a,
        int32_t exclusive,
        int32_t reverse,
        float *b
    )
    {
        rindow_matlib_s_cumsumb(m, n, k, a, exclusive, reverse, b);
    }

    virtual void test_matlib_cumsumb(
        int32_t m,
        int32_t n,
        int32_t k,
        double *a,
        int32_t exclusive,
        int32_t reverse,
        double *b
    )
    {
        rindow_matlib_d_cumsumb(m, n, k, a, exclusive, reverse, b);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(CumsumbTest, TestTypes);

TYPED_TEST(CumsumbTest, CumsumbNormal) {
    const int32_t M = 1;
    const int32_t N = 4;
    const int32_t K = 1;
    int32_t exclusive = false;
    int32_t reverse = false;
    TypeParam A[N] = {1,2,1,2};
    TypeParam A2[N] = {1,2,3,4};
    TypeParam B[N];

    this->test_matlib_cumsumb(M,N,K, A, exclusive, reverse, B);

    TypeParam R1[N] = {1,3,4,6};
    EXPECT_THAT(R1, ContainerEq(B));

    this->test_matlib_cumsumb(M,N,K, A2, exclusive, reverse, B);

    TypeParam R2[N] = {1,3,6,10};
    EXPECT_THAT(R2, ContainerEq(B));
}

TYPED_TEST(CumsumbTest, CumsumbExcusive) {
    const int32_t M = 1;
    const int32_t N = 4;
    const int32_t K = 1;
    int32_t exclusive = true;
    int32_t reverse = false;
    TypeParam A[N] = {1,2,1,2};
    TypeParam A2[N] = {1,2,3,4};
    TypeParam B[N];

    this->test_matlib_cumsumb(M,N,K, A, exclusive, reverse, B);

    TypeParam R1[N] = {0,1,3,4};
    EXPECT_THAT(R1, ContainerEq(B));

    this->test_matlib_cumsumb(M,N,K, A2, exclusive, reverse, B);

    TypeParam R2[N] = {0,1,3,6};
    EXPECT_THAT(R2, ContainerEq(B));
}

TYPED_TEST(CumsumbTest, CumsumbReverse) {
    const int32_t M = 1;
    const int32_t N = 4;
    const int32_t K = 1;
    int32_t exclusive = false;
    int32_t reverse = true;
    TypeParam A[N] = {1,2,1,2};
    TypeParam A2[N] = {1,2,3,4};
    TypeParam B[N];

    this->test_matlib_cumsumb(M,N,K, A, exclusive, reverse, B);

    TypeParam R1[N] = {6,5,3,2};
    EXPECT_THAT(R1, ContainerEq(B));

    this->test_matlib_cumsumb(M,N,K, A2, exclusive, reverse, B);

    TypeParam R2[N] = {10,9,7,4};
    EXPECT_THAT(R2, ContainerEq(B));
}

TYPED_TEST(CumsumbTest, CumsumbExclusiveAndReverse) {
    const int32_t M = 1;
    const int32_t N = 4;
    const int32_t K = 1;
    int32_t exclusive = true;
    int32_t reverse = true;
    TypeParam A2[N] = {1,2,3,4};
    TypeParam B[N];

    this->test_matlib_cumsumb(M,N,K, A2, exclusive, reverse, B);

    TypeParam R2[N] = {9,7,4,0};
    EXPECT_THAT(R2, ContainerEq(B));
}

TYPED_TEST(CumsumbTest, CumsumbNaN) {
    const int32_t M = 1;
    const int32_t N = 4;
    const int32_t K = 1;
    int32_t exclusive = false;
    int32_t reverse = false;
    TypeParam A[N] = {1,2,NAN,2};
    TypeParam B[N];

    this->test_matlib_cumsumb(M,N,K, A, exclusive, reverse, B);

    EXPECT_EQ(1.0, B[0]);
    EXPECT_EQ(3.0, B[1]);
    EXPECT_TRUE(isnan(B[2]));
    EXPECT_TRUE(isnan(B[3]));
}

TYPED_TEST(CumsumbTest, CumsumbNaNReverse) {
    const int32_t M = 1;
    const int32_t N = 4;
    const int32_t K = 1;
    int32_t exclusive = false;
    int32_t reverse = true;
    TypeParam A[N] = {1,2,NAN,2};
    TypeParam B[N];

    this->test_matlib_cumsumb(M,N,K, A, exclusive, reverse, B);

    EXPECT_TRUE(isnan(B[0]));
    EXPECT_TRUE(isnan(B[1]));
    EXPECT_TRUE(isnan(B[2]));
    EXPECT_EQ(2.0, B[3]);
}

TYPED_TEST(CumsumbTest, CumsumbWithAxis0) {
    const int32_t M = 1;
    const int32_t N = 3;
    const int32_t K = 9;
    int32_t exclusive = false;
    int32_t reverse = false;
    TypeParam A[3][3][3] = {
        {{ 1, 2, 3},{ 4, 5, 6},{ 7, 8, 9}},
        {{11,12,13},{14,15,16},{17,18,19}},
        {{21,22,23},{24,25,26},{27,28,29}}
    };
    TypeParam B[3][3][3];

    this->test_matlib_cumsumb(M,N,K, (TypeParam *)A, exclusive, reverse, (TypeParam *)B);

    TypeParam R1[3][3][3] = {
            {{ 1,  2,  3},
             { 4,  5,  6},
             { 7,  8,  9}},
            {{12, 14, 16},
             {18, 20, 22},
             {24, 26, 28}},
            {{33, 36, 39},
             {42, 45, 48},
             {51, 54, 57}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(CumsumbTest, CumsumbWithAxis1) {
    const int32_t M = 3;
    const int32_t N = 3;
    const int32_t K = 3;
    int32_t exclusive = false;
    int32_t reverse = false;
    TypeParam A[3][3][3] = {
        {{ 1, 2, 3},{ 4, 5, 6},{ 7, 8, 9}},
        {{11,12,13},{14,15,16},{17,18,19}},
        {{21,22,23},{24,25,26},{27,28,29}}
    };
    TypeParam B[3][3][3];

    this->test_matlib_cumsumb(M,N,K, (TypeParam *)A, exclusive, reverse, (TypeParam *)B);

    TypeParam R1[3][3][3] = {
            {{ 1,  2,  3},
             { 5,  7,  9},
             {12, 15, 18}},
            {{11, 12, 13},
             {25, 27, 29},
             {42, 45, 48}},
            {{21, 22, 23},
             {45, 47, 49},
             {72, 75, 78}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(CumsumbTest, CumsumbWithAxis2) {
    const int32_t M = 9;
    const int32_t N = 3;
    const int32_t K = 1;
    int32_t exclusive = false;
    int32_t reverse = false;
    TypeParam A[3][3][3] = {
        {{ 1, 2, 3},{ 4, 5, 6},{ 7, 8, 9}},
        {{11,12,13},{14,15,16},{17,18,19}},
        {{21,22,23},{24,25,26},{27,28,29}}
    };
    TypeParam B[3][3][3];

    this->test_matlib_cumsumb(M,N,K, (TypeParam *)A, exclusive, reverse, (TypeParam *)B);

    TypeParam R1[3][3][3] = {
            {{ 1,  3,  6},
             { 4,  9, 15},
             { 7, 15, 24}},
            {{11, 23, 36},
             {14, 29, 45},
             {17, 35, 54}},
            {{21, 43, 66},
             {24, 49, 75},
             {27, 55, 84}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(CumsumbTest, CumsumbWithAxis1Exclusive) {
    const int32_t M = 3;
    const int32_t N = 3;
    const int32_t K = 3;
    int32_t exclusive = true;
    int32_t reverse = false;
    TypeParam A[3][3][3] = {
        {{ 1, 2, 3},{ 4, 5, 6},{ 7, 8, 9}},
        {{11,12,13},{14,15,16},{17,18,19}},
        {{21,22,23},{24,25,26},{27,28,29}}
    };
    TypeParam B[3][3][3];

    this->test_matlib_cumsumb(M,N,K, (TypeParam *)A, exclusive, reverse, (TypeParam *)B);

    TypeParam R1[3][3][3] = {
           {{ 0,  0,  0},
            { 1,  2,  3},
            { 5,  7,  9}},
           {{ 0,  0,  0},
            {11, 12, 13},
            {25, 27, 29}},
           {{ 0,  0,  0},
            {21, 22, 23},
            {45, 47, 49}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(CumsumbTest, CumsumbWithAxis1Reverse) {
    const int32_t M = 3;
    const int32_t N = 3;
    const int32_t K = 3;
    int32_t exclusive = false;
    int32_t reverse = true;
    TypeParam A[3][3][3] = {
        {{ 1, 2, 3},{ 4, 5, 6},{ 7, 8, 9}},
        {{11,12,13},{14,15,16},{17,18,19}},
        {{21,22,23},{24,25,26},{27,28,29}}
    };
    TypeParam B[3][3][3];

    this->test_matlib_cumsumb(M,N,K, (TypeParam *)A, exclusive, reverse, (TypeParam *)B);

    TypeParam R1[3][3][3] = {
           {{12, 15, 18},
            {11, 13, 15},
            { 7,  8,  9}},
           {{42, 45, 48},
            {31, 33, 35},
            {17, 18, 19}},
           {{72, 75, 78},
            {51, 53, 55},
            {27, 28, 29}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(CumsumbTest, CumsumbWithAxis1ExclusiveReverse) {
    const int32_t M = 3;
    const int32_t N = 3;
    const int32_t K = 3;
    int32_t exclusive = true;
    int32_t reverse = true;
    TypeParam A[3][3][3] = {
        {{ 1, 2, 3},{ 4, 5, 6},{ 7, 8, 9}},
        {{11,12,13},{14,15,16},{17,18,19}},
        {{21,22,23},{24,25,26},{27,28,29}}
    };
    TypeParam B[3][3][3];

    this->test_matlib_cumsumb(M,N,K, (TypeParam *)A, exclusive, reverse, (TypeParam *)B);

    TypeParam R1[3][3][3] = {
           {{11, 13, 15},
            { 7,  8,  9},
            { 0,  0,  0}},
           {{31, 33, 35},
            {17, 18, 19},
            { 0,  0,  0}},
           {{51, 53, 55},
            {27, 28, 29},
            { 0,  0,  0}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

}
