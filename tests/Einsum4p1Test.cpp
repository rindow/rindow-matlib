#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

#include "rindow/matlib.h"
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class Einsum4p1Test : public ::testing::Test {
protected:
    virtual int32_t test_matlib_einsum4p1(
        int32_t dim0,
        int32_t dim1,
        int32_t dim2,
        int32_t dim3,
        int32_t dim4,
        float *a,
        int32_t ldA0,
        int32_t ldA1,
        int32_t ldA2,
        int32_t ldA3,
        int32_t ldA4,
        float *b,
        int32_t ldB0,
        int32_t ldB1,
        int32_t ldB2,
        int32_t ldB3,
        int32_t ldB4,
        float *c
    ) {
        return rindow_matlib_s_einsum4p1(
            dim0,
            dim1,
            dim2,
            dim3,
            dim4,
            a,
            ldA0,
            ldA1,
            ldA2,
            ldA3,
            ldA4,
            b,
            ldB0,
            ldB1,
            ldB2,
            ldB3,
            ldB4,
            c
        );
    }
    virtual int32_t test_matlib_einsum4p1(
        int32_t dim0,
        int32_t dim1,
        int32_t dim2,
        int32_t dim3,
        int32_t dim4,
        double *a,
        int32_t ldA0,
        int32_t ldA1,
        int32_t ldA2,
        int32_t ldA3,
        int32_t ldA4,
        double *b,
        int32_t ldB0,
        int32_t ldB1,
        int32_t ldB2,
        int32_t ldB3,
        int32_t ldB4,
        double *c
    ) {
        return rindow_matlib_d_einsum4p1(
            dim0,
            dim1,
            dim2,
            dim3,
            dim4,
            a,
            ldA0,
            ldA1,
            ldA2,
            ldA3,
            ldA4,
            b,
            ldB0,
            ldB1,
            ldB2,
            ldB3,
            ldB4,
            c
        );
    }
    virtual int build_dims(
        const int depth,
        const int32_t rank,
        const int *sizeOfIndices,
        int32_t *dims
    ) {
        int32_t expand = 5-depth-((depth==rank)?1:0);
        for(int32_t i=0;i<5;i++) {
            dims[i] = 1;
        }
        for(int32_t axis=0;axis<depth;axis++) {
            dims[expand+axis] = sizeOfIndices[axis];
        }
        return 0;
    }
    virtual int build_lds(
        const int32_t depth,
        const int32_t rankC,
        const int32_t rank,
        const int32_t *label,
        const int32_t *dims,
        int32_t *lds
    ) {
        int32_t ld=1;
        for(int32_t axis=0;axis<5;axis++) {
            lds[axis] = 0;
        }
        int32_t expand = 5-depth-((depth==rankC)?1:0);
        for(int32_t axis=rank-1;axis>=0;axis--) {
            if(axis>=5) {
                return -1;
            }
            int32_t lbl = label[axis];
            bool broadcast = false;
            if(lbl>=1000) {
                broadcast = true;
                lbl -= 1000;
            }
            lbl += expand;
            //std::cout << "axis=" << axis << ", label[axis]=" << (label[axis]) << ", expand=" << expand << ", lbl=" << lbl;
            if(lbl<5) {
                if(!broadcast) {
                    lds[lbl] += ld;
                }
                ld *= dims[lbl];
                //std::cout << ", dims[lbl]=" << dims[lbl];
            }
            //std::cout << std::endl;
        }
        return 0;
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(Einsum4p1Test, TestTypes);

TYPED_TEST(Einsum4p1Test, gemm_simple) {
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
    int32_t dims[5];
    int32_t ldA[5];
    int32_t ldB[5];

    printf("Indices=%d,%d,%d\n",sizeOfIndices[0],sizeOfIndices[1],sizeOfIndices[2]);
    printf("labelA =%d,%d\n",labelA[0],labelA[1]);
    printf("labelB =%d,%d\n",labelB[0],labelB[1]);
    rc = this->build_dims(depth,ndimC,sizeOfIndices,dims);
    ASSERT_EQ(0, rc);
    printf("dims=%d,%d,%d,%d,%d\n",dims[0],dims[1],dims[2],dims[3],dims[4]);
    rc = this->build_lds(depth,ndimC,ndimA,labelA,dims,ldA);
    ASSERT_EQ(0, rc);
    printf("ldA =%d,%d,%d,%d,%d\n",ldA[0],ldA[1],ldA[2],ldA[3],ldA[4]);
    rc = this->build_lds(depth,ndimC,ndimB,labelB,dims,ldB);
    ASSERT_EQ(0, rc);
    printf("ldB =%d,%d,%d,%d,%d\n",ldB[0],ldB[1],ldB[2],ldB[3],ldB[4]);

    rc = this->test_matlib_einsum4p1(
        dims[0],dims[1],dims[2],dims[3],dims[4],
        (TypeParam *)A,
        ldA[0],ldA[1],ldA[2],ldA[3],ldA[4],
        (TypeParam *)B,
        ldB[0],ldB[1],ldB[2],ldB[3],ldB[4],
        (TypeParam *)C
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
TYPED_TEST(Einsum4p1Test, gemm_cross3d2d) {
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
    int32_t dims[5];
    int32_t ldA[5];
    int32_t ldB[5];

    this->build_dims(depth,ndimC,sizeOfIndices,dims);
    this->build_lds(depth,ndimC,ndimA,labelA,dims,ldA);
    this->build_lds(depth,ndimC,ndimB,labelB,dims,ldB);

    rc = this->test_matlib_einsum4p1(
        dims[0],dims[1],dims[2],dims[3],dims[4],
        (TypeParam *)A,
        ldA[0],ldA[1],ldA[2],ldA[3],ldA[4],
        (TypeParam *)B,
        ldB[0],ldB[1],ldB[2],ldB[3],ldB[4],
        (TypeParam *)C
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
TYPED_TEST(Einsum4p1Test, mul_broadcast) {
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
    int32_t dims[5];
    int32_t ldA[5];
    int32_t ldB[5];

    printf("Indices=%d,%d\n",sizeOfIndices[0],sizeOfIndices[1]);
    printf("labelA =%d,%d\n",labelA[0],labelA[1]);
    printf("labelB =%d\n",labelB[0]);
    rc = this->build_dims(depth,ndimC,sizeOfIndices,dims);
    ASSERT_EQ(0, rc);
    printf("dims=%d,%d,%d,%d,%d\n",dims[0],dims[1],dims[2],dims[3],dims[4]);
    rc = this->build_lds(depth,ndimC,ndimA,labelA,dims,ldA);
    ASSERT_EQ(0, rc);
    printf("ldA =%d,%d,%d,%d,%d\n",ldA[0],ldA[1],ldA[2],ldA[3],ldA[4]);
    rc = this->build_lds(depth,ndimC,ndimB,labelB,dims,ldB);
    ASSERT_EQ(0, rc);
    printf("ldB =%d,%d,%d,%d,%d\n",ldB[0],ldB[1],ldB[2],ldB[3],ldB[4]);

    rc = this->test_matlib_einsum4p1(
        dims[0],dims[1],dims[2],dims[3],dims[4],
        (TypeParam *)A,
        ldA[0],ldA[1],ldA[2],ldA[3],ldA[4],
        (TypeParam *)B,
        ldB[0],ldB[1],ldB[2],ldB[3],ldB[4],
        (TypeParam *)C
    );
    ASSERT_EQ(0, rc);

    TypeParam R1[Da][Db] = {
        { 1,  4,  9},
        { 4, 10, 18}
    };
    EXPECT_THAT(R1, ContainerEq(C));
}
TYPED_TEST(Einsum4p1Test, mul_broadcastA_withshape) {
    const int32_t depth = 2;
    const int32_t ndimA = 2;
    const int32_t ndimB = 2;
    const int32_t ndimC = 2;
    const int32_t Da = 2;
    const int32_t Db = 3;
    const int32_t broadcast = 1000;
    int32_t rc;

    // equation =  ...b,...b->...b
    TypeParam A[1][Db];
    TypeParam B[Da][Db];
    TypeParam C[Da][Db];
    Utils::range(1, 1+Db,(TypeParam *)A);
    Utils::range(1, 1+Da*Db,(TypeParam *)B);
    Utils::zeros(Da*Db,(TypeParam *)C);

    int32_t sizeOfIndices[depth] = {Da,Db};
    int32_t labelA[ndimA] = {0+broadcast,1};
    int32_t labelB[ndimB] = {0,1};
    //int32_t shapeA[ndimA] = {1,Db};
    int32_t dims[5];
    int32_t ldA[5];
    int32_t ldB[5];

    printf("Indices=%d,%d\n",sizeOfIndices[0],sizeOfIndices[1]);
    printf("labelA =%d,%d\n",labelA[0],labelA[1]);
    printf("labelB =%d,%d\n",labelB[0],labelB[1]);
    rc = this->build_dims(depth,ndimC,sizeOfIndices,dims);
    ASSERT_EQ(0, rc);
    printf("dims=%d,%d,%d,%d,%d\n",dims[0],dims[1],dims[2],dims[3],dims[4]);
    rc = this->build_lds(depth,ndimC,ndimA,labelA,dims,ldA);
    ASSERT_EQ(0, rc);
    printf("ldA =%d,%d,%d,%d,%d\n",ldA[0],ldA[1],ldA[2],ldA[3],ldA[4]);
    rc = this->build_lds(depth,ndimC,ndimB,labelB,dims,ldB);
    ASSERT_EQ(0, rc);
    printf("ldB =%d,%d,%d,%d,%d\n",ldB[0],ldB[1],ldB[2],ldB[3],ldB[4]);

    rc = this->test_matlib_einsum4p1(
        dims[0],dims[1],dims[2],dims[3],dims[4],
        (TypeParam *)A,
        ldA[0],ldA[1],ldA[2],ldA[3],ldA[4],
        (TypeParam *)B,
        ldB[0],ldB[1],ldB[2],ldB[3],ldB[4],
        (TypeParam *)C
    );
    ASSERT_EQ(0, rc);

    TypeParam R1[Da][Db] = {
        { 1,  4,  9},
        { 4, 10, 18}
    };
    EXPECT_THAT(R1, ContainerEq(C));
}
TYPED_TEST(Einsum4p1Test, mul_broadcastB_withshape) {
    const int32_t depth = 2;
    const int32_t ndimA = 2;
    const int32_t ndimB = 2;
    const int32_t ndimC = 2;
    const int32_t Da = 2;
    const int32_t Db = 3;
    const int32_t broadcast = 1000;
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
    int32_t labelB[ndimB] = {0+broadcast,1};
    int32_t shapeB[ndimB] = {1,Db};
    int32_t dims[5];
    int32_t ldA[5];
    int32_t ldB[5];

    this->build_dims(depth,ndimC,sizeOfIndices,dims);
    this->build_lds(depth,ndimC,ndimA,labelA,dims,ldA);
    this->build_lds(depth,ndimC,ndimB,labelB,dims,ldB);

    rc = this->test_matlib_einsum4p1(
        dims[0],dims[1],dims[2],dims[3],dims[4],
        (TypeParam *)A,
        ldA[0],ldA[1],ldA[2],ldA[3],ldA[4],
        (TypeParam *)B,
        ldB[0],ldB[1],ldB[2],ldB[3],ldB[4],
        (TypeParam *)C
    );
    ASSERT_EQ(0, rc);

    TypeParam R1[Da][Db] = {
        { 1,  4,  9},
        { 4, 10, 18}
    };
    EXPECT_THAT(R1, ContainerEq(C));
}

}
