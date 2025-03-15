#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

#include "rindow/matlib.h"
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class EinsumTest : public ::testing::Test {
protected:
    virtual int32_t test_matlib_einsum(
        const int32_t depth,
        const int32_t *dims,
        const float *a,
        const int32_t *ldA,
        const float *b,
        const int32_t *ldB,
        float *c,
        const int32_t ndimC
    ) {
        return rindow_matlib_s_einsum(
            depth,
            dims,
            a,
            ldA,
            b,
            ldB,
            c,
            ndimC
        );
    }
    virtual int32_t test_matlib_einsum(
        const int32_t depth,
        const int32_t *dims,
        const double *a,
        const int32_t *ldA,
        const double *b,
        const int32_t *ldB,
        double *c,
        const int32_t ndimC
    ) {
        return rindow_matlib_d_einsum(
            depth,
            dims,
            a,
            ldA,
            b,
            ldB,
            c,
            ndimC
        );
    }
    virtual int build_lds(
        const int32_t depth,
        const int32_t *sizeOfIndices,
        const int32_t rank,
        const int32_t *label,
        const int32_t *shape,
        int32_t *lds
    ) {
        for(int32_t axis=0;axis<depth;axis++) {
            lds[axis] = 0;
        }
        int32_t ld=1;
        for(int32_t axis=rank-1;axis>=0;axis--) {
            if(axis>=depth) {
                return -1;
            }
            const int32_t lbl = label[axis];
            if(lbl>=depth) {
                return -2;
            }
            bool broadcast = false;
            if(shape!=nullptr && shape[axis]==1) {
                broadcast = true;
            }
            if(!broadcast) {
                lds[lbl] += ld;
            }
            ld *= sizeOfIndices[lbl];
        }
        return 0;
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(EinsumTest, TestTypes);

TYPED_TEST(EinsumTest, gemm_simple) {
    const int32_t depth = 3;
    const int32_t ndimA = 2;
    const int32_t ndimB = 2;
    const int32_t ndimC = 2;
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t K = 4;
    int32_t rc;

    // equation = mn,nk->mk
    TypeParam A[M][N] = {
        { 1, 2, 3},
        { 4, 5, 6}
    };
    TypeParam B[N][K] = {
        { 1, 2, 3, 4},
        { 5, 6, 7, 8},
        { 9,10,11,12}
    };
    TypeParam C[M][K];
    Utils::zeros(M*K,(TypeParam *)C);

    int32_t sizeOfIndices[depth] = {M,K,N};
    int32_t labelA[ndimA] = {0,2};
    int32_t labelB[ndimB] = {2,1};
    int32_t ldA[depth];
    int32_t ldB[depth];

    rc = this->build_lds(depth,sizeOfIndices,ndimA,labelA,nullptr,ldA);
    ASSERT_EQ(0, rc);
    rc = this->build_lds(depth,sizeOfIndices,ndimB,labelB,nullptr,ldB);
    ASSERT_EQ(0, rc);

    rc = this->test_matlib_einsum(
        depth,
        sizeOfIndices,
        (TypeParam *)A,
        ldA,
        (TypeParam *)B,
        ldB,
        (TypeParam *)C,
        ndimC
    );
    ASSERT_EQ(0, rc);

    TypeParam R1[M][N] = {
        { 1, 2, 3},
        { 4, 5, 6}
    };
    EXPECT_THAT(R1, ContainerEq(A));

    TypeParam R2[N][K] = {
        { 1, 2, 3, 4},
        { 5, 6, 7, 8},
        { 9,10,11,12}
    };
    EXPECT_THAT(R2, ContainerEq(B));

    TypeParam R3[M][K] = {
        { 38, 44, 50, 56},
        { 83, 98,113,128}
    };
    EXPECT_THAT(R3, ContainerEq(C));
}
TYPED_TEST(EinsumTest, gemm_cross3d2d) {
    const int32_t depth = 4;
    const int32_t ndimA = 3;
    const int32_t ndimB = 2;
    const int32_t ndimC = 3;
    const int32_t Da = 4;
    const int32_t Db = 3;
    const int32_t Dc = 2;
    const int32_t Dd = 5;
    int32_t rc;

    // equation = abc,dc->abd
    TypeParam A[Da][Db][Dc];
    TypeParam B[Dd][Dc];
    TypeParam C[Da][Db][Dd];
    Utils::range(0,Da*Db*Dc,(TypeParam *)A);
    Utils::range(0,Dd*Dc,(TypeParam *)B);
    Utils::zeros(Da*Db*Dd,(TypeParam *)C);

    int32_t sizeOfIndices[depth] = {Da,Db,Dd,Dc};
    int32_t labelA[ndimA] = {0,1,3};
    int32_t labelB[ndimB] = {2,3};
    int32_t ldA[depth];
    int32_t ldB[depth];

    rc = this->build_lds(depth,sizeOfIndices,ndimA,labelA,nullptr,ldA);
    ASSERT_EQ(0, rc);
    rc = this->build_lds(depth,sizeOfIndices,ndimB,labelB,nullptr,ldB);
    ASSERT_EQ(0, rc);

    rc = this->test_matlib_einsum(
        depth,
        sizeOfIndices,
        (TypeParam *)A,
        ldA,
        (TypeParam *)B,
        ldB,
        (TypeParam *)C,
        ndimC
    );
    ASSERT_EQ(0, rc);

    TypeParam R1[Da][Db][Dd] = {
       {{  1,   3,   5,   7,   9},
        {  3,  13,  23,  33,  43},
        {  5,  23,  41,  59,  77}},

       {{  7,  33,  59,  85, 111},
        {  9,  43,  77, 111, 145},
        { 11,  53,  95, 137, 179}},

       {{ 13,  63, 113, 163, 213},
        { 15,  73, 131, 189, 247},
        { 17,  83, 149, 215, 281}},

       {{ 19,  93, 167, 241, 315},
        { 21, 103, 185, 267, 349},
        { 23, 113, 203, 293, 383}}
    };
    EXPECT_THAT(R1, ContainerEq(C));

}
TYPED_TEST(EinsumTest, mul_broadcast) {
    const int32_t depth = 2;
    const int32_t ndimA = 2;
    const int32_t ndimB = 1;
    const int32_t ndimC = 2;
    const int32_t Da = 2;
    const int32_t Db = 3;
    int32_t rc;

    // equation = ab,b->ab
    TypeParam A[Da][Db];
    TypeParam B[Db];
    TypeParam C[Da][Db];
    Utils::range(1, 1+Da*Db,(TypeParam *)A);
    Utils::range(1, 1+Db,(TypeParam *)B);
    Utils::zeros(Da*Db,(TypeParam *)C);

    int32_t sizeOfIndices[depth] = {Da,Db};
    int32_t labelA[ndimA] = {0,1};
    int32_t labelB[ndimB] = {1};
    int32_t ldA[depth];
    int32_t ldB[depth];

    rc = this->build_lds(depth,sizeOfIndices,ndimA,labelA,nullptr,ldA);
    ASSERT_EQ(0, rc);
    rc = this->build_lds(depth,sizeOfIndices,ndimB,labelB,nullptr,ldB);
    ASSERT_EQ(0, rc);

    rc = this->test_matlib_einsum(
        depth,
        sizeOfIndices,
        (TypeParam *)A,
        ldA,
        (TypeParam *)B,
        ldB,
        (TypeParam *)C,
        ndimC
    );
    ASSERT_EQ(0, rc);

    TypeParam R1[Da][Db] = {
        { 1,  4,  9},
        { 4, 10, 18}
    };
    EXPECT_THAT(R1, ContainerEq(C));
}
TYPED_TEST(EinsumTest, mul_broadcastA_withshape) {
    const int32_t depth = 2;
    const int32_t ndimA = 2;
    const int32_t ndimB = 2;
    const int32_t ndimC = 2;
    const int32_t Da = 2;
    const int32_t Db = 3;
    int32_t rc;

    // equation =  ...b,...b->...b
    TypeParam A[1][Db];
    TypeParam B[Da][Db];
    TypeParam C[Da][Db];
    Utils::range(1, 1+Db,(TypeParam *)A);
    Utils::range(1, 1+Da*Db,(TypeParam *)B);
    Utils::zeros(Da*Db,(TypeParam *)C);

    int32_t sizeOfIndices[depth] = {Da,Db};
    int32_t labelA[ndimA] = {0,1};
    int32_t labelB[ndimB] = {0,1};
    int32_t shapeA[ndimA] = {1,Db};
    int32_t ldA[depth];
    int32_t ldB[depth];

    rc = this->build_lds(depth,sizeOfIndices,ndimA,labelA,shapeA,ldA);
    ASSERT_EQ(0, rc);
    rc = this->build_lds(depth,sizeOfIndices,ndimB,labelB,nullptr,ldB);
    ASSERT_EQ(0, rc);

    rc = this->test_matlib_einsum(
        depth,
        sizeOfIndices,
        (TypeParam *)A,
        ldA,
        (TypeParam *)B,
        ldB,
        (TypeParam *)C,
        ndimC
    );
    ASSERT_EQ(0, rc);

    TypeParam R1[Da][Db] = {
        { 1,  4,  9},
        { 4, 10, 18}
    };
    EXPECT_THAT(R1, ContainerEq(C));
}
TYPED_TEST(EinsumTest, mul_broadcastB_withshape) {
    const int32_t depth = 2;
    const int32_t ndimA = 2;
    const int32_t ndimB = 2;
    const int32_t ndimC = 2;
    const int32_t Da = 2;
    const int32_t Db = 3;
    int32_t rc;

    // equation =  ...b,...b->...b
    TypeParam A[Da][Db];
    TypeParam B[1][Db];
    TypeParam C[Da][Db];
    Utils::range(1, 1+Da*Db,(TypeParam *)A);
    Utils::range(1, 1+Db,(TypeParam *)B);
    Utils::zeros(Da*Db,(TypeParam *)C);

    int32_t sizeOfIndices[depth] = {Da,Db};
    int32_t labelA[ndimA] = {0,1};
    int32_t labelB[ndimB] = {0,1};
    int32_t shapeB[ndimB] = {1,Db};
    int32_t ldA[depth];
    int32_t ldB[depth];

    rc = this->build_lds(depth,sizeOfIndices,ndimA,labelA,nullptr,ldA);
    ASSERT_EQ(0, rc);
    rc = this->build_lds(depth,sizeOfIndices,ndimB,labelB,shapeB,ldB);
    ASSERT_EQ(0, rc);

    rc = this->test_matlib_einsum(
        depth,
        sizeOfIndices,
        (TypeParam *)A,
        ldA,
        (TypeParam *)B,
        ldB,
        (TypeParam *)C,
        ndimC
    );
    ASSERT_EQ(0, rc);

    TypeParam R1[Da][Db] = {
        { 1,  4,  9},
        { 4, 10, 18}
    };
    EXPECT_THAT(R1, ContainerEq(C));
}

}
