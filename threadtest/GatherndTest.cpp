#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"
#include <stdbool.h>
#include <numeric>

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class GatherndTest : public ::testing::Test {
protected:
    int32_t calcGatherndShapes(
        std::initializer_list<int32_t> shapeA,
        std::initializer_list<int32_t> shapeX,
        int32_t batchDims,
        int32_t *m,int32_t *n,int32_t *k,
        int32_t *indexDepth, int32_t *paramShape,
        int32_t *outputSize
        )
    {
        int32_t ndimBatch = (int32_t)shapeX.size();
        int32_t ndimA = (int32_t)shapeA.size();
        if(batchDims<0) {
            return -1;
        }
        if((int32_t)shapeX.size()<batchDims+1) {
            return -2;
        }
        *indexDepth = *(shapeX.begin()+shapeX.size()-1);
        if(*indexDepth>(int32_t)shapeA.size()-batchDims) {
            return -3;
        }
        *m = std::accumulate(shapeX.begin(), shapeX.begin()+batchDims, 1, std::multiplies<int32_t>());
        if(*m!=std::accumulate(shapeA.begin(), shapeA.begin()+batchDims, 1, std::multiplies<int32_t>())) {
            return -4;
        }
        *n = std::accumulate(shapeX.begin()+batchDims, shapeX.end()-1, 1, std::multiplies<int32_t>());
        *k = std::accumulate(shapeA.begin()+batchDims+*indexDepth, shapeA.end(), 1, std::multiplies<int32_t>());
        int32_t dim_i=0;
        for(auto p = shapeA.begin()+batchDims, e = shapeA.begin()+batchDims+*indexDepth; p != e; ++p) {
            paramShape[dim_i++] = *p;
        }
        *outputSize = (*m)*(*n)*(*k);
        return 0;
    }   

    int32_t calcScatterndShapes(
        std::initializer_list<int32_t> shapeX,
        std::initializer_list<int32_t> shapeA,
        std::initializer_list<int32_t> shapeOutput,
        int32_t batchDims,
        int32_t *m,int32_t *n,int32_t *k,
        int32_t *indexDepth, int32_t *paramShape,
        int32_t *outputSize
        )
    {
        int32_t updatesSize;
        int32_t errcode = calcGatherndShapes(
            shapeOutput,
            shapeX,
            batchDims,
            m,n,k,
            indexDepth, paramShape,
            &updatesSize
        );
        if(errcode!=0) {
            return errcode;
        }
        *outputSize = std::accumulate(shapeOutput.begin(), shapeOutput.end(), 1, std::multiplies<int32_t>());
        int32_t updatesSize0 = std::accumulate(shapeA.begin(), shapeA.end(), 1, std::multiplies<int32_t>());
        if(updatesSize!=updatesSize0) {
            return -5;
        }
        return 0;
    }   

    virtual int32_t test_matlib_gathernd(
        int32_t reverse, int32_t addMode, int32_t m, int32_t n, int32_t k,
        int32_t indexDepth, int32_t *paramShape,
        float *a, int32_t *x, float *b
    )
    {
        return rindow_matlib_s_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,a,x,b);
    }

    virtual int32_t test_matlib_gathernd(
        int32_t reverse, int32_t addMode, int32_t m, int32_t n, int32_t k,
        int32_t indexDepth, int32_t *paramShape,
        double *a, int32_t *x, double *b
    )
    {
        return rindow_matlib_d_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,a,x,b);
    }

    virtual int32_t test_matlib_gathernd(
        int32_t reverse, int32_t addMode, int32_t m, int32_t n, int32_t k,
        int32_t indexDepth, int32_t *paramShape,
        int32_t *a, int32_t *x, int32_t *b
    )
    {
        int32_t dtype = rindow_matlib_dtype_int32;
        return rindow_matlib_i_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,dtype,a,x,b);
    }

};
typedef ::testing::Types<float, double, int32_t> TestTypes;
TYPED_TEST_SUITE(GatherndTest, TestTypes);

TYPED_TEST(GatherndTest, GatherndNormal1Dby1DAxis0) {
    // axis = 0
    // 1D indices
    // A:   1D [p0]
    // X:   1D [depth]
    // res: 0D []
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 5;
    const int32_t shapeX0 = 1;
    const int32_t batchDims = 0;
    const int32_t shapeB0 = 1;
    const int32_t indexDepth0 = 1;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0] = {
        0,1,2,3,4
    };
    int32_t X[shapeX0] = {1};
    rc = this->calcGatherndShapes({shapeA0},{shapeX0},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0];
    
    ASSERT_EQ(1,        m);
    ASSERT_EQ(1,        n);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeX0,  outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0] = {1};
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(GatherndTest, GatherndNormal1Dby2DAxis0) {
    // axis = 0
    // 1D indices
    // A:   1D [p0]
    // X:   2D [n,depth]
    // res: 1D [n]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 5;
    const int32_t shapeX0  = 2;
    const int32_t shapeX1  = 1;
    const int32_t batchDims = 0;
    const int32_t shapeB0 = 2;
    const int32_t indexDepth0 = 1;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0] = {
        0,1,2,3,4
    };
    int32_t X[shapeX0][shapeX1] = {{1},{2}};
    rc = this->calcGatherndShapes({shapeA0},{shapeX0,shapeX1},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0];

    ASSERT_EQ(1,           m);
    ASSERT_EQ(shapeX0,     n);
    ASSERT_EQ(1,           k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0] = {
        1,
        2
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(GatherndTest, GatherndNormal2Dby2DAxis0) {
    // axis = 0
    // 1D indices
    // A:   2D [p0,k]
    // X:   2D [n,depth]
    // res: 2D [n,k]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 5;
    const int32_t shapeA1 = 2;
    const int32_t shapeX0 = 2;
    const int32_t shapeX1 = 1;
    const int32_t batchDims = 0;
    const int32_t shapeB0 = 2;
    const int32_t shapeB1 = 2;
    const int32_t indexDepth0 = 1;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1];
    Utils::range<TypeParam>(0,shapeA0*shapeA1,(TypeParam*)A);

    int32_t X[shapeX0][shapeX1] = {{1},{2}};
    rc = this->calcGatherndShapes({shapeA0,shapeA1},{shapeX0,shapeX1},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1];
    
    ASSERT_EQ(1,           m);
    ASSERT_EQ(shapeX0,     n);
    ASSERT_EQ(shapeA1,     k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0*shapeB1,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1] = {
        {2,3},
        {4,5}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherndTest, GatherndNormal3Dby2DAxis0) {
    // axis = 0
    // 2D indices
    // A:   3D [p0,p1,k]
    // X:   2D [n,depth]
    // res: 2D [n,k]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 5;
    const int32_t shapeA1 = 6;
    const int32_t shapeA2 = 2;
    const int32_t shapeX0 = 2;
    const int32_t shapeX1 = 2;
    const int32_t batchDims = 0;
    const int32_t shapeB0 = 2;
    const int32_t shapeB1 = 2;
    const int32_t indexDepth0 = 2;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5,6};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2];
    Utils::range<TypeParam>(0,shapeA0*shapeA1*shapeA2,(TypeParam*)A);

    int32_t X[shapeX0][shapeX1] = {
        {0,1},
        {2,3}
    };
    rc = this->calcGatherndShapes({shapeA0,shapeA1,shapeA2},{shapeX0,shapeX1},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1];
    
    ASSERT_EQ(1,           m);
    ASSERT_EQ(shapeX0,     n);
    ASSERT_EQ(shapeA2,     k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0*shapeB1,     outputSize);


    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1] = {
        {2,3},
        {30,31}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherndTest, GatherndNormal2Dby2DAxis1) {
    // axis = 1
    // 1D indices
    // A:   2D [m,p0]
    // X:   2D [m,depth]
    // res: 1D [m]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 5;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 1;
    const int32_t batchDims = 1;
    const int32_t shapeB0 = 4;
    const int32_t indexDepth0 = 1;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1];
    Utils::range<TypeParam>(0,shapeA0*shapeA1,(TypeParam*)A);

    int32_t X[shapeX0][shapeX1] = {
        {0},
        {1},
        {2},
        {0}
    };
    rc = this->calcGatherndShapes({shapeA0,shapeA1},{shapeX0,shapeX1},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0];
    
    ASSERT_EQ(shapeX0,      m);
    ASSERT_EQ(1,            n);
    ASSERT_EQ(1,            k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0] = {
        0,
        6,
        12,
        15
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherndTest, GatherndNormal3Dby2DAxis1) {
    // axis = 1
    // 1D indices
    // A:   3D [m,p0,k]
    // X:   2D [m,depth]
    // res: 2D [m,k]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 5;
    const int32_t shapeA2 = 2;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 1;
    const int32_t batchDims = 1;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 2;
    const int32_t indexDepth0 = 1;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2];
    Utils::range<TypeParam>(0,shapeA0*shapeA1*shapeA2,(TypeParam*)A);

    int32_t X[shapeX0][shapeX1] = {
        {0},
        {1},
        {2},
        {0}
    };
    rc = this->calcGatherndShapes({shapeA0,shapeA1,shapeA2},{shapeX0,shapeX1},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1];
    
    ASSERT_EQ(shapeX0,      m);
    ASSERT_EQ(1,            n);
    ASSERT_EQ(shapeA2,      k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0*shapeB1,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1] = {
        { 0,  1},
        {12, 13},
        {24, 25},
        {30, 31}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherndTest, GatherndNormal3Dby3DAxis1) {
    // axis = 1
    // 1D indices
    // A:   3D [m,p0,k]
    // X:   3D [m,n,depth]
    // res: 3D [m,n,k]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 5;
    const int32_t shapeA2 = 2;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 2;
    const int32_t shapeX2 = 1;
    const int32_t batchDims = 1;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 2;
    const int32_t shapeB2 = 2;
    const int32_t indexDepth0 = 1;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2];
    Utils::range<TypeParam>(0,shapeA0*shapeA1*shapeA2,(TypeParam*)A);

    int32_t X[shapeX0][shapeX1][shapeX2] = {
        {{0},
         {0}},
        {{1},
         {1}},
        {{2},
         {2}},
        {{0},
         {0}},
    };
    rc = this->calcGatherndShapes({shapeA0,shapeA1,shapeA2},{shapeX0,shapeX1,shapeX2},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1][shapeB2];
    
    ASSERT_EQ(shapeX0,      m);
    ASSERT_EQ(shapeX1,      n);
    ASSERT_EQ(shapeA2,      k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0*shapeB1*shapeB2,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{ 0,  1},
         { 0,  1}},
        {{12, 13},
         {12, 13}},
        {{24, 25},
         {24, 25}},
        {{30, 31},
         {30, 31}},
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherndTest, GatherndNormal4Dby3DAxis1) {
    // axis = 1
    // 2D indices
    // A:   4D [m,p0,p1,k]
    // X:   3D [m,n,depth]
    // res: 3D [m,n,k]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 5;
    const int32_t shapeA2 = 6;
    const int32_t shapeA3 = 2;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 3;
    const int32_t shapeX2 = 2;
    const int32_t batchDims = 1;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 3;
    const int32_t shapeB2 = 2;
    const int32_t indexDepth0 = 2;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5,6};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2][shapeA3];
    Utils::range<TypeParam>(0,shapeA0*shapeA1*shapeA2*shapeA3,(TypeParam*)A);

    int32_t X[shapeX0][shapeX1][shapeX2] = {
        {{0,0},{0,1},{1,0}},
        {{0,1},{1,2},{4,5}},
        {{2,1},{0,1},{0,1}},
        {{1,1},{1,1},{1,1}}
    };
    rc = this->calcGatherndShapes({shapeA0,shapeA1,shapeA2,shapeA3},{shapeX0,shapeX1,shapeX2},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1][shapeB2];
    
    ASSERT_EQ(shapeX0,      m);
    ASSERT_EQ(shapeX1,      n);
    ASSERT_EQ(shapeA3,      k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0*shapeB1*shapeB2,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{  0,   1},
         {  2,   3},
         { 12,  13}},
        {{ 62,  63},
         { 76,  77},
         {118, 119}},
        {{146, 147},
         {122, 123},
         {122, 123}},
        {{194, 195},
         {194, 195},
         {194, 195}},
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherndTest, ScatterndNormal1Dby1DAxis0) {
    // axis = 0
    // 1D indices
    // A:   0D []
    // X:   1D [depth]
    // res: 1D [p0]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t shapeA = 1;
    const int32_t shapeX0 = 1;
    const int32_t batchDims = 0;
    const int32_t shapeB0 = 5;
    const int32_t indexDepth0 = 1;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA] = {1};
    int32_t X[shapeX0] = {1};
    TypeParam B[shapeB0];
    Utils::fill<TypeParam>(shapeB0, (TypeParam*)B, 0);

    rc = this->calcScatterndShapes({shapeX0},{},{shapeB0},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(1,            m);
    ASSERT_EQ(1,            n);
    ASSERT_EQ(1,            k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0] =
        {0,1,0,0,0}
    ;
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherndTest, ScatterndNormal1Dby2DAxis0) {
    // axis = 0
    // 1D indices
    // A:   1D [n]
    // X:   2D [n,depth]
    // res: 1D [p0]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 2;
    const int32_t shapeX0 = 2;
    const int32_t shapeX1 = 1;
    const int32_t batchDims = 0;
    const int32_t shapeB0 = 5;
    const int32_t indexDepth0 = 1;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0] = {1,2};
    int32_t X[shapeX0][shapeX1] = {{1},{2}};
    TypeParam B[shapeB0];
    Utils::fill<TypeParam>(shapeB0, (TypeParam*)B, 0);

    rc = this->calcScatterndShapes({shapeX0,shapeX1},{shapeA0},{shapeB0},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(1,            m);
    ASSERT_EQ(shapeX0,      n);
    ASSERT_EQ(1,            k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0] = {
            0,
            1,
            2,
            0,
            0
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherndTest, ScatterndNormal2Dby2DAxis0) {
    // axis = 0
    // 1D indices
    // A:   2D [n,k]
    // X:   2D [n,depth]
    // res: 2D [p0,k]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 2;
    const int32_t shapeX0 = 2;
    const int32_t shapeX1 = 1;
    const int32_t batchDims = 0;
    const int32_t shapeB0 = 5;
    const int32_t shapeB1 = 2;
    const int32_t indexDepth0 = 1;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1] = {
        {2,3},
        {4,5}
    };
    int32_t X[shapeX0][shapeX1] = {{1},{2}};
    TypeParam B[shapeB0][shapeB1];
    Utils::fill<TypeParam>(shapeB0*shapeB1, (TypeParam*)B, 0);

    rc = this->calcScatterndShapes({shapeX0,shapeX1},{shapeA0,shapeA1},{shapeB0,shapeB1},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(1,            m);
    ASSERT_EQ(shapeX0,      n);
    ASSERT_EQ(shapeA1,      k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0*shapeB1,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1] = {
        {0,0},
        {2,3},
        {4,5},
        {0,0},
        {0,0}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherndTest, ScatterndNormal2Dby2Dby2DindicesAxis0) {
    // axis = 0
    // 2D indices
    // A:   2D [n,k]
    // X:   2D [n,depth]
    // res: 3D [p0,p1,k]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 2;
    const int32_t shapeX0 = 2;
    const int32_t shapeX1 = 2;
    const int32_t batchDims = 0;
    const int32_t shapeB0 = 5;
    const int32_t shapeB1 = 6;
    const int32_t shapeB2 = 2;
    const int32_t indexDepth0 = 2;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5,6};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1] = {
        {2,3},
        {30,31}
    };
    int32_t X[shapeX0][shapeX1] = {
        {0,1},
        {2,3}
    };
    TypeParam B[shapeB0][shapeB1][shapeB2];
    Utils::fill<TypeParam>(shapeB0*shapeB1*shapeB2, (TypeParam*)B, 0);

    rc = this->calcScatterndShapes({shapeX0,shapeX1},{shapeA0,shapeA1},{shapeB0,shapeB1,shapeB2},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(1,            m);
    ASSERT_EQ(shapeX0,      n);
    ASSERT_EQ(shapeA1,      k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0*shapeB1*shapeB2,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{0,0},{2,3},{0,0},{0,0},  {0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},  {0,0},{0,0}},
        {{0,0},{0,0},{0,0},{30,31},{0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},  {0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},  {0,0},{0,0}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherndTest, ScatterndNormal1Dby2DAxis1) {
    // axis = 1
    // 1D indices
    // A:   1D [m]
    // X:   2D [m,depth]
    // res: 2D [m,p0]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 4;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 1;
    const int32_t batchDims = 1;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 5;
    const int32_t indexDepth0 = 1;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0] = {
        0,
        6,
        12,
        15
    };
    int32_t X[shapeX0][shapeX1] = {
        {0},
        {1},
        {2},
        {0}
    };
    TypeParam B[shapeB0][shapeB1];
    Utils::fill<TypeParam>(shapeB0*shapeB1, (TypeParam*)B, 0);

    rc = this->calcScatterndShapes({shapeX0,shapeX1},{shapeA0},{shapeB0,shapeB1},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(shapeX0,      m);
    ASSERT_EQ(1,            n);
    ASSERT_EQ(1,            k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0*shapeB1,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1] = {
        { 0, 0, 0, 0, 0},
        { 0, 6, 0, 0, 0},
        { 0, 0,12, 0, 0},
        {15, 0, 0, 0, 0}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherndTest, ScatterndNormal2Dby2DAxis1) {
    // axis = 1
    // 1D indices
    // A:   2D [m,k]
    // X:   2D [m,depth]
    // res: 3D [m,p0,k]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 1;
    const int32_t batchDims = 1;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 5;
    const int32_t shapeB2 = 2;
    const int32_t indexDepth0 = 1;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1] = {
        { 0,  1},
        {12, 13},
        {24, 25},
        {30, 31}
    };
    int32_t X[shapeX0][shapeX1] = {
        {0},
        {1},
        {2},
        {0}
    };
    TypeParam B[shapeB0][shapeB1][shapeB2];
    Utils::fill<TypeParam>(shapeB0*shapeB1*shapeB2, (TypeParam*)B, 0);

    rc = this->calcScatterndShapes({shapeX0,shapeX1},{shapeA0,shapeA1},{shapeB0,shapeB1,shapeB2},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(shapeX0,      m);
    ASSERT_EQ(1,            n);
    ASSERT_EQ(shapeA1,      k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0*shapeB1*shapeB2,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{0,1},{0,0},{0,0},{0,0},{0,0}},
        {{0,0},{12,13},{0,0},{0,0},{0,0}},
        {{0,0},{0,0},{24,25},{0,0},{0,0}},
        {{30,31},{0,0},{0,0},{0,0},{0,0}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherndTest, ScatterndNormal3Dby3DAxis1) {
    // axis = 1
    // 1D indices
    // A:   3D [m,n,k]
    // X:   3D [m,n,depth]
    // res: 3D [m,p0,k]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 2;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 2;
    const int32_t shapeX2 = 1;
    const int32_t batchDims = 1;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 5;
    const int32_t shapeB2 = 2;
    const int32_t indexDepth0 = 1;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{ 0,  1},
         { 0,  1}},
        {{12, 13},
         {12, 13}},
        {{24, 25},
         {24, 25}},
        {{30, 31},
         {30, 31}}
    };
    int32_t X[shapeX0][shapeX1][shapeX2] = {
        {{0},
         {0}},
        {{1},
         {1}},
        {{2},
         {2}},
        {{0},
         {0}}
    };
    TypeParam B[shapeB0][shapeB1][shapeB2];
    Utils::fill<TypeParam>(shapeB0*shapeB1*shapeB2, (TypeParam*)B, 0);

    rc = this->calcScatterndShapes({shapeX0,shapeX1,shapeX2},{shapeA0,shapeA1,shapeA2},{shapeB0,shapeB1,shapeB2},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(shapeX0,      m);
    ASSERT_EQ(shapeX1,      n);
    ASSERT_EQ(shapeA1,      k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0*shapeB1*shapeB2,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{0,1},{0,0},{0,0},{0,0},{0,0}},
        {{0,0},{12,13},{0,0},{0,0},{0,0}},
        {{0,0},{0,0},{24,25},{0,0},{0,0}},
        {{30,31},{0,0},{0,0},{0,0},{0,0}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherndTest, ScatterndNormal3Dby3Dby2DindcesAxis1) {
    // axis = 1
    // 2D indices
    // A:   3D [m,n,k]
    // X:   3D [m,n,depth]
    // res: 4D [m,p0,p1,k]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 3;
    const int32_t shapeA2 = 2;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 3;
    const int32_t shapeX2 = 2;
    const int32_t batchDims = 1;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 5;
    const int32_t shapeB2 = 6;
    const int32_t shapeB3 = 2;
    const int32_t indexDepth0 = 2;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5,6};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{  0,   1},
         {  2,   3},
         { 12,  13}},
        {{ 62,  63},
         { 76,  77},
         {118, 119}},
        {{146, 147},
         {122, 123},
         {122, 123}},
        {{194, 195},
         {194, 195},
         {194, 195}},
    };
    int32_t X[shapeX0][shapeX1][shapeX2] = {
        {{0,0},{0,1},{1,0}},
        {{0,1},{1,2},{4,5}},
        {{2,1},{0,1},{0,1}},
        {{1,1},{1,1},{1,1}}
    };
    TypeParam B[shapeB0][shapeB1][shapeB2][shapeB3];
    Utils::fill<TypeParam>(shapeB0*shapeB1*shapeB2*shapeB3, (TypeParam*)B, 0);

    rc = this->calcScatterndShapes({shapeX0,shapeX1,shapeX2},{shapeA0,shapeA1,shapeA2},{shapeB0,shapeB1,shapeB2,shapeB3},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(shapeX0,      m);
    ASSERT_EQ(shapeX1,      n);
    ASSERT_EQ(shapeA2,      k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0*shapeB1*shapeB2*shapeB3,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2][shapeB3] = {
        {{{0,1},{2,3},{0,0},{0,0},{0,0},{0,0}},
         {{12,13},{0,0},{0,0},{0,0},{0,0},{0,0}},
         {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
         {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
         {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}},
        {{{0,0},{62,63},{0,0},{0,0},{0,0},{0,0}},
         {{0,0},{0,0},{76,77},{0,0},{0,0},{0,0}},
         {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
         {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
         {{0,0},{0,0},{0,0},{0,0},{0,0},{118,119}}},
        {{{0,0},{122,123},{0,0},{0,0},{0,0},{0,0}},
         {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
         {{0,0},{146,147},{0,0},{0,0},{0,0},{0,0}},
         {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
         {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}},
        {{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
         {{0,0},{194,195},{0,0},{0,0},{0,0},{0,0}},
         {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
         {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
         {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherndTest, ScatterndAddNormal2Dby2Dby2DindicesAxis0) {
    // m = 1
    // axis = 0
    // 2D indices
    // A:   2D [n,k]
    // X:   2D [n,depth]
    // res: 3D [p0,p1,k]
    const int32_t reverse = true; 
    const int32_t addMode = true;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 2;
    const int32_t batchDims = 0;
    const int32_t shapeB0 = 5;
    const int32_t shapeB1 = 6;
    const int32_t shapeB2 = 2;
    const int32_t indexDepth0 = 2;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5,6};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1] = {
        {1,10},
        {2,3},
        {30,31},
        {1,10}
    };
    int32_t X[shapeX0][shapeX1] = {
        {4,5},
        {0,1},
        {2,3},
        {4,5}
    };
    TypeParam B[shapeB0][shapeB1][shapeB2];
    Utils::fill<TypeParam>(shapeB0*shapeB1*shapeB2, (TypeParam*)B, 0);

    rc = this->calcScatterndShapes({shapeX0,shapeX1},{shapeA0,shapeA1},{shapeB0,shapeB1,shapeB2},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(1,            m);
    ASSERT_EQ(shapeX0,      n);
    ASSERT_EQ(shapeA1,      k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0*shapeB1*shapeB2,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{0,0},{2,3},{0,0},{0,0},  {0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},  {0,0},{0,0}},
        {{0,0},{0,0},{0,0},{30,31},{0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},  {0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},  {0,0},{2,20}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherndTest, ScatterndAddNormal3Dby3DAxis1) {
    // m<n
    // axis = 1
    // 1D indices
    // A:   3D [m,n,k]
    // X:   3D [m,n,depth]
    // res: 3D [m,p0,k]
    const int32_t reverse = true; 
    const int32_t addMode = true;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 4;
    const int32_t shapeA2 = 2;
    const int32_t shapeX0 = 2;
    const int32_t shapeX1 = 4;
    const int32_t shapeX2 = 1;
    const int32_t batchDims = 1;
    const int32_t shapeB0 = 2;
    const int32_t shapeB1 = 5;
    const int32_t shapeB2 = 2;
    const int32_t indexDepth0 = 1;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t indexDepth;
    int32_t paramShape[indexDepth0];
    int32_t RParamShape[indexDepth0] = {5};
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{ 0,  1},
         { 0,  1},
         {12, 13},
         {12, 13}},
        {{24, 25},
         {24, 25},
         {30, 31},
         {30, 31}}
    };
    int32_t X[shapeX0][shapeX1][shapeX2] = {
        {{0},
         {0},
         {1},
         {1}},
        {{2},
         {2},
         {0},
         {0}}
    };
    TypeParam B[shapeB0][shapeB1][shapeB2];
    Utils::fill<TypeParam>(shapeB0*shapeB1*shapeB2, (TypeParam*)B, 0);

    rc = this->calcScatterndShapes({shapeX0,shapeX1,shapeX2},{shapeA0,shapeA1,shapeA2},{shapeB0,shapeB1,shapeB2},batchDims,&m,&n,&k,&indexDepth,paramShape,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(shapeX0,      m);
    ASSERT_EQ(shapeX1,      n);
    ASSERT_EQ(shapeA2,      k);
    ASSERT_EQ(indexDepth0, indexDepth);
    EXPECT_THAT(RParamShape, ContainerEq(paramShape));
    ASSERT_EQ(shapeB0*shapeB1*shapeB2,     outputSize);

    rc = this->test_matlib_gathernd(reverse,addMode,m,n,k,indexDepth,paramShape,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{0,2},{24,26},{0,0},{0,0},{0,0}},
        {{60,62},{0,0},{48,50},{0,0},{0,0}},
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
}
