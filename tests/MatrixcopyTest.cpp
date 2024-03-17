#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdbool.h>

using testing::ContainerEq;

namespace {

template <typename T>
class MatrixcopyTest : public ::testing::Test {
protected:
    virtual void test_matlib_matrixcopy(
        int32_t trans, int32_t m, int32_t n,
        float alpha,
        float *a, int32_t ldA,
        float *b, int32_t ldB
    ) {
        rindow_matlib_s_matrixcopy(
            trans, m, n,
            alpha,
            a, ldA,
            b, ldB
        );
    }
    virtual void test_matlib_matrixcopy(
        int32_t trans, int32_t m, int32_t n,
        double alpha,
        double *a, int32_t ldA,
        double *b, int32_t ldB
    ) {
        rindow_matlib_d_matrixcopy(
            trans, m, n,
            alpha,
            a, ldA,
            b, ldB
        );
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(MatrixcopyTest, TestTypes);

TYPED_TEST(MatrixcopyTest, leftcopy_rowsGTcols) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t ldA = N;
    const int32_t ldB = N+1;

    // rows>cols
    TypeParam alpha = 1;
    TypeParam A1[M*N] = {
        1,2,3,
        4,5,6
    };
    TypeParam B1[M*ldB] = {
        0,0,0,0,
        0,0,0,0
    };

    this->test_matlib_matrixcopy(
        trans,  // trans, 
        M, N,
        alpha,  // alpha,
        A1, ldA,
        B1, ldB
    );

    TypeParam R1[M*ldB] = {
        1,2,3,0,
        4,5,6,0
    };
    EXPECT_THAT(R1, ContainerEq(B1));
}
TYPED_TEST(MatrixcopyTest, leftcopy_rowsLTcols) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;
    const int32_t ldA = N;
    const int32_t ldB = N+1;

    // rows<cols
    TypeParam alpha = 1;
    TypeParam A1[M*N] = {
        1,2,
        3,4,
        5,6
    };
    TypeParam B1[M*ldB] = {
        0,0,0,
        0,0,0,
        0,0,0
    };

    this->test_matlib_matrixcopy(
        trans,  // trans, 
        M, N,
        alpha,  // alpha,
        A1, ldA,
        B1, ldB
    );

    TypeParam R1[M*ldB] = {
        1,2,0,
        3,4,0,
        5,6,0
    };
    EXPECT_THAT(R1, ContainerEq(B1));
}
TYPED_TEST(MatrixcopyTest, trans_rowsGTcols) {
    const int32_t trans = RINDOW_MATLIB_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;
    const int32_t ldA = N;
    const int32_t ldB = M;

    // rows>cols
    TypeParam alpha = -1;
    TypeParam A1[M*N] = {
        1,2,
        3,4,
        5,6
    };
    TypeParam B1[N*M] = {
        0,0,0,
        0,0,0
    };

    this->test_matlib_matrixcopy(
        trans,  // trans, 
        M, N,
        alpha,  // alpha,
        A1, ldA,
        B1, ldB
    );

    TypeParam R1[N*M] = {
        -1,-3,-5,
        -2,-4,-6
    };
    EXPECT_THAT(R1, ContainerEq(B1));
}
TYPED_TEST(MatrixcopyTest, trans_rowsLTcols) {
    const int32_t trans = RINDOW_MATLIB_TRANS;
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t ldA = N;
    const int32_t ldB = M;

    // rows<cols
    TypeParam alpha = -1;
    TypeParam A1[M*N] = {
        1,2,3,
        4,5,6
    };
    TypeParam B1[N*M] = {
        0,0,
        0,0,
        0,0
    };

    this->test_matlib_matrixcopy(
        trans,  // trans, 
        M, N,
        alpha,  // alpha,
        A1, ldA,
        B1, ldB
    );

    TypeParam R1[N*M] = {
        -1,-4,
        -2,-5,
        -3,-6
    };
    EXPECT_THAT(R1, ContainerEq(B1));
}
}
