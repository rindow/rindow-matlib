#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"
#include <stdbool.h>
#include <numeric>
#include <iostream>

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class GatherbTest : public ::testing::Test {
protected:
    int32_t calcGatherbShapes(
        std::initializer_list<int32_t> params,
        std::initializer_list<int32_t> indices,
        int32_t axis,
        int32_t batchDims,
        int32_t detailDepth,
        int32_t indexDepth,
        int32_t *batches,int32_t *m,int32_t *n,int32_t *k,int32_t *len,int32_t *numClass, int32_t *outputSize
        )
    {
        int32_t indices_ndim = (int32_t)indices.size();
        int32_t params_ndim = (int32_t)params.size();
        if(batchDims<0||batchDims>=params_ndim) {
            std::cout << "batchDims:" << batchDims << std::endl;
            std::cout << "params_ndim:" << params_ndim << std::endl;
            return -1;
        }
        if(batchDims<0||batchDims>indices_ndim) {
            std::cout << "batchDims:" << batchDims << std::endl;
            std::cout << "indices_ndim:" << indices_ndim << std::endl;
            return -2;
        }
        if(batchDims>axis||axis>=params_ndim) {
            std::cout << "batchDims:" << batchDims << std::endl;
            std::cout << "axis:" << axis << std::endl;
            std::cout << "params_ndim:" << params_ndim << std::endl;
            return -3;
        }
        if(axis>detailDepth||detailDepth>params_ndim) {
            std::cout << "axis:" << axis << std::endl;
            std::cout << "params_ndim:" << params_ndim << std::endl;
            std::cout << "detailDepth:" << detailDepth << std::endl;
            return -4;
        }
        if(batchDims>indexDepth||indexDepth>indices_ndim) {
            std::cout << "batchDims:" << batchDims << std::endl;
            std::cout << "indexDepth:" << indexDepth << std::endl;
            std::cout << "indices_ndim:" << indices_ndim << std::endl;
            return -5;
        }

        *batches = std::accumulate(params.begin(), params.begin()+batchDims, 1, std::multiplies<int32_t>());
        *m = std::accumulate(params.begin()+batchDims, params.begin()+axis, 1, std::multiplies<int32_t>());
        *numClass = std::accumulate(params.begin()+axis, params.begin()+axis+1, 1, std::multiplies<int32_t>());
        *k = std::accumulate(params.begin()+axis+1, params.begin()+detailDepth, 1, std::multiplies<int32_t>());
        *len = std::accumulate(params.begin()+detailDepth, params.end(), 1, std::multiplies<int32_t>());

        int32_t batchesX = std::accumulate(indices.begin(), indices.begin()+batchDims, 1, std::multiplies<int32_t>());
        *n = std::accumulate(indices.begin()+batchDims, indices.begin()+indexDepth, 1, std::multiplies<int32_t>());
        int32_t kX = std::accumulate(indices.begin()+indexDepth, indices.end(), 1, std::multiplies<int32_t>());

        if(*batches!=batchesX) {
            std::cout << "batches:" << *batches << std::endl;
            std::cout << "batchesX:" << batchesX << std::endl;
            return -6;
        }
        if(*k!=kX) {
            std::cout << "k:" << *k << std::endl;
            std::cout << "kX:" << kX << std::endl;
            return -7;
        }
        *outputSize = (*batches)*(*m)*(*n)*(*k)*(*len);
        return 0;
    }   

    int32_t calcScatterbShapes(
        std::initializer_list<int32_t> indices,
        std::initializer_list<int32_t> updates,
        std::initializer_list<int32_t> outputs,
        int32_t axis,
        int32_t batchDims,
        int32_t detailDepth,
        int32_t indexDepth,
        int32_t *batches,int32_t *m,int32_t *n,int32_t *k,int32_t *len,int32_t *numClass, int32_t *outputSize
        )
    {
        int32_t errcode = calcGatherbShapes(
            outputs,
            indices,
            axis,
            batchDims,
            detailDepth,
            indexDepth,
            batches,m,n,k,len,numClass,outputSize
        );
        if(errcode!=0) {
            return errcode;
        }
        if(outputs.size()<detailDepth) {
            std::cout << "outputs.size:" << outputs.size() << std::endl;
            std::cout << "detailDepth:" << detailDepth << std::endl;
            return -8;
        }
        int32_t outer_dims = axis-batchDims;
        int32_t index_dims = indexDepth-batchDims;
        int32_t inner_dims = detailDepth-1-axis;
        int32_t detail_dims = (int32_t)(outputs.size())-detailDepth;
        if(batchDims+outer_dims+1+inner_dims+detail_dims!=outputs.size()) {
            std::cout << "outputs.size:" << outputs.size() << std::endl;
            std::cout << "batchDims:" << batchDims << std::endl;
            std::cout << "outer_dims:" << outer_dims << std::endl;
            std::cout << "index_dims:" << index_dims << std::endl;
            std::cout << "detail_dims:" << detail_dims << std::endl;
            return -9;
        }

        int32_t batchesU = std::accumulate(updates.begin(), updates.begin()+batchDims, 1, std::multiplies<int32_t>());
        int32_t mU = std::accumulate(updates.begin()+batchDims, updates.begin()+batchDims+outer_dims, 1, std::multiplies<int32_t>());
        int32_t nU = std::accumulate(updates.begin()+batchDims+outer_dims, updates.begin()+batchDims+outer_dims+index_dims, 1, std::multiplies<int32_t>());
        int32_t kU = std::accumulate(updates.begin()+batchDims+outer_dims+index_dims, updates.begin()+batchDims+outer_dims+index_dims+inner_dims, 1, std::multiplies<int32_t>());
        int32_t lenU = std::accumulate(updates.begin()+batchDims+outer_dims+index_dims+inner_dims, updates.end(), 1, std::multiplies<int32_t>());
        if(*batches!=batchesU) {
            std::cout << "batches:" << *batches << std::endl;
            std::cout << "batchesU:" << batchesU << std::endl;
            return -10;
        }
        if(*m!=mU) {
            std::cout << "m:" << *m << std::endl;
            std::cout << "mU:" << mU << std::endl;
            return -11;
        }
        if(*n!=nU) {
            std::cout << "n:" << *n << std::endl;
            std::cout << "nU:" << nU << std::endl;
            return -12;
        }
        if(*k!=kU) {
            std::cout << "k:" << *k << std::endl;
            std::cout << "kU:" << kU << std::endl;
            return -13;
        }
        if(*len!=lenU) {
            std::cout << "len:" << *len << std::endl;
            std::cout << "lenU:" << lenU << std::endl;
            return -14;
        }
        *outputSize = (*batches)*(*m)*(*numClass)*(*k)*(*len);
        return 0;
    }   

    virtual int32_t test_matlib_gatherb(
        int32_t reverse, int32_t addMode,
        int32_t batches, int32_t m, int32_t n, int32_t k, int32_t len,
        int32_t numClass,
        float *a, int32_t *x, float *b
    )
    {
        return rindow_matlib_s_gatherb(reverse,addMode,batches,m,n,k,len,numClass,a,x,b);
    }

    virtual int32_t test_matlib_gatherb(
        int32_t reverse, int32_t addMode,
        int32_t batches, int32_t m, int32_t n, int32_t k, int32_t len,
        int32_t numClass,
        double *a, int32_t *x, double *b
    )
    {
        return rindow_matlib_d_gatherb(reverse,addMode,batches,m,n,k,len,numClass,a,x,b);
    }

    virtual int32_t test_matlib_gatherb(
        int32_t reverse, int32_t addMode,
        int32_t batches, int32_t m, int32_t n, int32_t k, int32_t len,
        int32_t numClass,
        int32_t *a, int32_t *x, int32_t *b
    )
    {
        int32_t dtype = rindow_matlib_dtype_int32;
        return rindow_matlib_i_gatherb(reverse,addMode,batches,m,n,k,len,numClass,dtype,a,x,b);
    }

};
typedef ::testing::Types<float, double, int32_t> TestTypes;
TYPED_TEST_SUITE(GatherbTest, TestTypes);

TYPED_TEST(GatherbTest, GatherbNormal1Dby1DDims0Axis0) {
    // axis = 0
    // A:   1D [numClass]
    // X:   1D [n]
    // res: 1D [n]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t batchDims = 0;
    const int32_t axis = 0;
    const int32_t detailDepth = axis+1;
    const int32_t indexDepth = 1; // x.ndim()
    const int32_t shapeA0 = 4;
    const int32_t shapeX0 = 3;
    const int32_t shapeB0 = 3;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0];
    Utils::range<TypeParam>(0,shapeA0,(TypeParam*)A);
    int32_t X[shapeX0] = {2,2,1};
    rc = this->calcGatherbShapes({shapeA0},{shapeX0},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0];
    
    ASSERT_EQ(1,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(3,        n);
    ASSERT_EQ(4,        numClass);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(1,        len);
    ASSERT_EQ(shapeB0,  outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0] = {2,2,1};
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, GatherbNormal2Dby1DDims1Axis1) {
    // batchDims = 1
    // axis = 1
    // A:   2D [batches,numClass]
    // X:   1D [batches]
    // res: 2D [batches]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 1;
    const int32_t detailDepth = axis+1;
    const int32_t indexDepth = 1; // x.ndim()
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 3;
    const int32_t shapeX0 = 4;
    const int32_t shapeB0 = 4;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1];
    Utils::range<TypeParam>(0,shapeA0*shapeA1,(TypeParam*)A);
    int32_t X[shapeX0] = {2,2,1,0};
    rc = this->calcGatherbShapes({shapeA0,shapeA1},{shapeX0},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0];
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(1,        n);
    ASSERT_EQ(3,        numClass);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(1,        len);
    ASSERT_EQ(shapeB0,  outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0] = {2,5,7,9};
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, GatherbNormal3Dby1DDims1Axis0) {
    // batchDims = 1
    // axis = 1
    // A:   3D [bathces,numClass,len]
    // X:   1D [bathces]
    // res: 2D [bathces,len]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 1;
    const int32_t detailDepth = axis+1;
    const int32_t indexDepth = 1; // x.ndim()
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 3;
    const int32_t shapeA2 = 2;
    const int32_t shapeX0 = 4;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 2;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2];
    Utils::range<TypeParam>(0,shapeA0*shapeA1*shapeA2,(TypeParam*)A);
    int32_t X[shapeX0] = {2,2,1,0};
    rc = this->calcGatherbShapes({shapeA0,shapeA1,shapeA2},{shapeX0},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1];
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(1,        n);
    ASSERT_EQ(3,        numClass);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(2,        len);
    ASSERT_EQ(shapeB0*shapeB1, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1] = {
        {4,5},
        {10,11},
        {14,15},
        {18,19},
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(GatherbTest, GatherbNormal3Dby2DDims1Axis1) {
    // for argmax
    // batchDims = 1    // batchDims=axis
    // axis = 1         // default: axis=batchDims
    // detailDepth = 3  // detailDepth=params->ndim()
    // indexDepth = 1   // indexDepth=batchDims
    // A:   2D [batches,numClass,k]
    // X:   2D [batches,k]
    // res: 2D [batches,k]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 1;
    const int32_t detailDepth = 3; // params->ndim()
    const int32_t indexDepth = 1;  // batchDims
    const int32_t shapeA0 = 3;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 4;
    const int32_t shapeX0 = 3;
    const int32_t shapeX1 = 4;
    const int32_t shapeB0 = 3;
    const int32_t shapeB1 = 4;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2];
    Utils::range<TypeParam>(0,shapeA0*shapeA1*shapeA2,(TypeParam*)A);
    int32_t X[shapeX0][shapeX1] = {
        {1,1,1,1},
        {1,1,1,1},
        {1,1,1,1}
    };
    rc = this->calcGatherbShapes({shapeA0,shapeA1,shapeA2},{shapeX0,shapeX1},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1];
    
    ASSERT_EQ(3,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(1,        n);
    ASSERT_EQ(2,        numClass);
    ASSERT_EQ(4,        k);
    ASSERT_EQ(1,        len);
    ASSERT_EQ(shapeB0*shapeB1, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1] = {
        { 4, 5, 6, 7},
        {12,13,14,15},
        {20,21,22,23},
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, GatherbNormal3Dby1DDims1Axis2) {
    // batchDims = 1
    // axis = 2
    // A:   3D [batches,m,numClass]
    // X:   1D [batches]
    // res: 2D [batches,m]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 2;
    const int32_t detailDepth = axis+1; // default
    const int32_t indexDepth = 1;  // default:x->ndim()
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 3;
    const int32_t shapeX0 = 4;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 2;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2];
    Utils::range<TypeParam>(0,shapeA0*shapeA1*shapeA2,(TypeParam*)A);
    int32_t X[shapeX0] = {2,2,1,0};
    rc = this->calcGatherbShapes({shapeA0,shapeA1,shapeA2},{shapeX0},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1];
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(2,        m);
    ASSERT_EQ(1,        n);
    ASSERT_EQ(3,        numClass);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(1,        len);
    ASSERT_EQ(shapeB0*shapeB1, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1] = {
        { 2, 5},
        { 8,11},
        {13,16},
        {18,21}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, GatherbNormal4Dby1DDims1Axis2) {
    // batchDims = 1
    // axis = 2
    // A:   4D [batches,m,numClass,len]
    // X:   1D [batches]
    // res: 3D [batches,m,len]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 2;
    const int32_t detailDepth = axis+1; // default
    const int32_t indexDepth = 1;  // default:x->ndim()
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 3;
    const int32_t shapeA3 = 2;
    const int32_t shapeX0 = 4;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 2;
    const int32_t shapeB2 = 2;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2][shapeA3];
    Utils::range<TypeParam>(0,shapeA0*shapeA1*shapeA2*shapeA3,(TypeParam*)A);
    int32_t X[shapeX0] = {2,2,1,0};
    rc = this->calcGatherbShapes({shapeA0,shapeA1,shapeA2,shapeA3},{shapeX0},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1][shapeB2];
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(2,        m);
    ASSERT_EQ(1,        n);
    ASSERT_EQ(3,        numClass);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(2,        len);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{ 4, 5},{10,11}},
        {{16,17},{22,23}},
        {{26,27},{32,33}},
        {{36,37},{42,43}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, GatherbNormal2Dby2DDims1Axis1) {
    // batchDims = 1
    // axis = 1
    // A:   2D [batches,numClass]
    // X:   2D [batches,n]
    // res: 2D [batches,n]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 1;
    const int32_t detailDepth = axis+1; // default
    const int32_t indexDepth = 2;  // default:x->ndim()
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 3;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 2;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 2;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1];
    Utils::range<TypeParam>(0,shapeA0*shapeA1,(TypeParam*)A);
    int32_t X[shapeX0][shapeX1] = {
        {0,1},
        {1,0},
        {2,2},
        {1,0}
    };
    rc = this->calcGatherbShapes({shapeA0,shapeA1},{shapeX0,shapeX1},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1];
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(2,        n);
    ASSERT_EQ(3,        numClass);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(1,        len);
    ASSERT_EQ(shapeB0*shapeB1, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1] = {
        {0,1},
        {4,3},
        {8,8},
        {10,9}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, GatherbNormal3Dby3DDims1Axis1Detail3ind2) {
    // batchDims = 1
    // axis = 1
    // detailDepth = 3
    // indexDepth = 2
    // A:   3D [batches,numClass,k]
    // X:   3D [batches,n,k]
    // res: 3D [batches,n,k]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 1;
    const int32_t detailDepth = 3; // not default
    const int32_t indexDepth = 2;  // not default
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 5;
    const int32_t shapeA2 = 2;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 3;
    const int32_t shapeX2 = 2;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 3;
    const int32_t shapeB2 = 2;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2];
    Utils::range<TypeParam>(0,shapeA0*shapeA1*shapeA2,(TypeParam*)A);
    int32_t X[shapeX0][shapeX1][shapeX2] = {
        {{0,1},{0,1},{0,1}},
        {{1,0},{1,0},{1,0}},
        {{2,2},{2,2},{2,2}},
        {{1,0},{1,0},{1,0}}
    };
    rc = this->calcGatherbShapes({shapeA0,shapeA1,shapeA2},{shapeX0,shapeX1,shapeX2},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1][shapeB2];
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(3,        n);
    ASSERT_EQ(5,        numClass);
    ASSERT_EQ(2,        k);
    ASSERT_EQ(1,        len);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{ 0, 3},{ 0, 3},{ 0, 3}},
        {{12,11},{12,11},{12,11}},
        {{24,25},{24,25},{24,25}},
        {{32,31},{32,31},{32,31}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, GatherbNormal4Dby3DDims1Axis1Detail3ind2) {
    // batchDims = 1
    // axis = 1
    // detailDepth = 3
    // indexDepth = 2
    // A:   4D [batches,numClass,k,len]
    // X:   3D [batches,n,k]
    // res: 4D [batches,n,k,len]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 1;
    const int32_t detailDepth = 3; // not default
    const int32_t indexDepth = 2;  // not default
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 5;
    const int32_t shapeA2 = 2;
    const int32_t shapeA3 = 3;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 3;
    const int32_t shapeX2 = 2;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 3;
    const int32_t shapeB2 = 2;
    const int32_t shapeB3 = 3;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2][shapeA3];
    Utils::range<TypeParam>(0,shapeA0*shapeA1*shapeA2*shapeA3,(TypeParam*)A);
    int32_t X[shapeX0][shapeX1][shapeX2] = {
        {{0,1},{0,1},{0,1}},
        {{1,0},{1,0},{1,0}},
        {{2,2},{2,2},{2,2}},
        {{1,0},{1,0},{1,0}}
    };
    rc = this->calcGatherbShapes({shapeA0,shapeA1,shapeA2,shapeA3},{shapeX0,shapeX1,shapeX2},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1][shapeB2][shapeB3];
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(3,        n);
    ASSERT_EQ(5,        numClass);
    ASSERT_EQ(2,        k);
    ASSERT_EQ(3,        len);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2*shapeB3, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2][shapeB3] = {
        {{{ 0, 1, 2},{ 9,10,11}},
         {{ 0, 1, 2},{ 9,10,11}},
         {{ 0, 1, 2},{ 9,10,11}},},
        {{{36,37,38},{33,34,35}},
         {{36,37,38},{33,34,35}},
         {{36,37,38},{33,34,35}},},
        {{{72,73,74},{75,76,77}},
         {{72,73,74},{75,76,77}},
         {{72,73,74},{75,76,77}},},
        {{{96,97,98},{93,94,95}},
         {{96,97,98},{93,94,95}},
         {{96,97,98},{93,94,95}},}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, GatherbNormal5Dby3DDims1Axis2Detail4ind2) {
    // batchDims = 1
    // axis = 2
    // detailDepth = 4
    // indexDepth = 2
    // A:   5D [batches,m,numClass,k,len]
    // X:   3D [batches,n,k]
    // res: 5D [batches,m,n,k,len]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 2;
    const int32_t detailDepth = 4; // not default
    const int32_t indexDepth = 2;  // not default
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 5;
    const int32_t shapeA3 = 2;
    const int32_t shapeA4 = 3;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 3;
    const int32_t shapeX2 = 2;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 2;
    const int32_t shapeB2 = 3;
    const int32_t shapeB3 = 2;
    const int32_t shapeB4 = 3;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2][shapeA3][shapeA4];
    Utils::range<TypeParam>(0,shapeA0*shapeA1*shapeA2*shapeA3*shapeA4,(TypeParam*)A);
    int32_t X[shapeX0][shapeX1][shapeX2] = {
        {{0,1},{0,1},{0,1}},
        {{1,0},{1,0},{1,0}},
        {{2,2},{2,2},{2,2}},
        {{1,0},{1,0},{1,0}}
    };
    rc = this->calcGatherbShapes({shapeA0,shapeA1,shapeA2,shapeA3,shapeA4},{shapeX0,shapeX1,shapeX2},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1][shapeB2][shapeB3][shapeB4];
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(2,        m);
    ASSERT_EQ(3,        n);
    ASSERT_EQ(5,        numClass);
    ASSERT_EQ(2,        k);
    ASSERT_EQ(3,        len);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2*shapeB3*shapeB4, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2][shapeB3][shapeB4] = {
        {{{{ 0, 1, 2},{ 9,10,11}},
          {{ 0, 1, 2},{ 9,10,11}},
          {{ 0, 1, 2},{ 9,10,11}}},
         {{{30,31,32},{39,40,41}},
          {{30,31,32},{39,40,41}},
          {{30,31,32},{39,40,41}}}},
        {{{{66,67,68},{63,64,65}},
          {{66,67,68},{63,64,65}},
          {{66,67,68},{63,64,65}}},
         {{{96,97,98},{93,94,95}},
          {{96,97,98},{93,94,95}},
          {{96,97,98},{93,94,95}}}},
        {{{{132,133,134},{135,136,137}},
          {{132,133,134},{135,136,137}},
          {{132,133,134},{135,136,137}}},
         {{{162,163,164},{165,166,167}},
          {{162,163,164},{165,166,167}},
          {{162,163,164},{165,166,167}}}},
        {{{{186,187,188},{183,184,185}},
          {{186,187,188},{183,184,185}},
          {{186,187,188},{183,184,185}}},
         {{{216,217,218},{213,214,215}},
          {{216,217,218},{213,214,215}},
          {{216,217,218},{213,214,215}}}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, ScatterbNormal1Dby1DDims0Axis0) {
    // axis = 0
    // A:   1D [n]
    // X:   1D [n]
    // res: 1D [numClass]
    const int32_t reverse = true;
    const int32_t addMode = false;
    const int32_t batchDims = 0;
    const int32_t axis = 0;
    const int32_t detailDepth = axis+1;
    const int32_t indexDepth = 1; // x.ndim()
    const int32_t shapeA0 = 3;
    const int32_t shapeX0 = 3;
    const int32_t shapeB0 = 4;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0] = {2,2,1};
    int32_t X[shapeX0] = {2,2,1};
    rc = this->calcScatterbShapes({shapeX0},{shapeA0},{shapeB0},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0];
    Utils::fill<TypeParam>(shapeB0, (TypeParam*)B, 0);
    
    ASSERT_EQ(1,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(3,        n);
    ASSERT_EQ(4,        numClass);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(1,        len);
    ASSERT_EQ(shapeB0,  outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0] = {0,1,2,0};
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, ScatterbNormal2Dby1DDims1Axis1) {
    // batchDims = 1
    // axis = 1
    // A:   2D [batches]
    // X:   1D [batches]
    // res: 2D [batches,numClass]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 1;
    const int32_t detailDepth = axis+1;
    const int32_t indexDepth = 1; // x.ndim()
    const int32_t shapeA0 = 4;
    const int32_t shapeX0 = 4;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 3;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0] = {2,5,7,9};
    int32_t X[shapeX0] = {2,2,1,0};
    rc = this->calcScatterbShapes({shapeX0},{shapeA0},{shapeB0,shapeB1},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1];
    Utils::fill<TypeParam>(shapeB0*shapeB1, (TypeParam*)B, 0);
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(1,        n);
    ASSERT_EQ(3,        numClass);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(1,        len);
    ASSERT_EQ(shapeB0*shapeB1,  outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1] = {
        {0,0,2},
        {0,0,5},
        {0,7,0},
        {9,0,0}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, ScatterbNormal3Dby1DDims1Axis0) {
    // batchDims = 1
    // axis = 1
    // A:   2D [bathces,len]
    // X:   1D [bathces]
    // res: 3D [bathces,numClass,len]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 1;
    const int32_t detailDepth = axis+1;
    const int32_t indexDepth = 1; // x.ndim()
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeX0 = 4;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 3;
    const int32_t shapeB2 = 2;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1] = {
        {4,5},
        {10,11},
        {14,15},
        {18,19},
    };
    int32_t X[shapeX0] = {2,2,1,0};
    rc = this->calcScatterbShapes({shapeX0},{shapeA0,shapeA1},{shapeB0,shapeB1,shapeB2},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1][shapeB2];
    Utils::fill<TypeParam>(shapeB0*shapeB1*shapeB2, (TypeParam*)B, 0);
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(1,        n);
    ASSERT_EQ(3,        numClass);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(2,        len);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{0,0},{0,0},{4,5}},
        {{0,0},{0,0},{10,11}},
        {{0,0},{14,15},{0,0}},
        {{18,19},{0,0},{0,0}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, ScatterbNormal3Dby2DDims1Axis1) {
    // for argmax
    // batchDims = 1    // batchDims=axis
    // axis = 1         // default: axis=batchDims
    // detailDepth = 3  // detailDepth=params->ndim()
    // indexDepth = 1   // indexDepth=batchDims
    // A:   2D [batches,k]
    // X:   2D [batches,k]
    // res: 2D [batches,numClass,k]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 1;
    const int32_t detailDepth = 3; // params->ndim()
    const int32_t indexDepth = 1;  // batchDims
    const int32_t shapeA0 = 3;
    const int32_t shapeA1 = 4;
    const int32_t shapeX0 = 3;
    const int32_t shapeX1 = 4;
    const int32_t shapeB0 = 3;
    const int32_t shapeB1 = 2;
    const int32_t shapeB2 = 4;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1] = {
        { 4, 5, 6, 7},
        {12,13,14,15},
        {20,21,22,23}
    };
    int32_t X[shapeX0][shapeX1] = {
        {1,1,1,1},
        {1,1,1,1},
        {1,1,1,1}
    };
    rc = this->calcScatterbShapes({shapeX0,shapeX1},{shapeA0,shapeA1},{shapeB0,shapeB1,shapeB2},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1][shapeB2];
    Utils::fill<TypeParam>(shapeB0*shapeB1*shapeB2, (TypeParam*)B, 0);
    
    ASSERT_EQ(3,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(1,        n);
    ASSERT_EQ(2,        numClass);
    ASSERT_EQ(4,        k);
    ASSERT_EQ(1,        len);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{0,0,0,0},{4,5,6,7}},
        {{0,0,0,0},{12,13,14,15}},
        {{0,0,0,0},{20,21,22,23}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, ScatterbNormal3Dby1DDims1Axis2) {
    // batchDims = 1
    // axis = 2
    // A:   2D [batches,m]
    // X:   1D [batches]
    // res: 3D [batches,m,numClass]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 2;
    const int32_t detailDepth = axis+1; // default
    const int32_t indexDepth = 1;  // default:x->ndim()
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeX0 = 4;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 2;
    const int32_t shapeB2 = 3;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1] = {
        { 2, 5},
        { 8,11},
        {13,16},
        {18,21}
    };
    int32_t X[shapeX0] = {2,2,1,0};
    rc = this->calcScatterbShapes({shapeX0},{shapeA0,shapeA1},{shapeB0,shapeB1,shapeB2},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1][shapeB2];
    Utils::fill<TypeParam>(shapeB0*shapeB1*shapeB2, (TypeParam*)B, 0);
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(2,        m);
    ASSERT_EQ(1,        n);
    ASSERT_EQ(3,        numClass);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(1,        len);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{0,0,2},{0,0,5}},
        {{0,0,8},{0,0,11}},
        {{0,13,0},{0,16,0}},
        {{18,0,0},{21,0,0}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, ScatterbNormal4Dby1DDims1Axis2) {
    // batchDims = 1
    // axis = 2
    // A:   3D [batches,m,len]
    // X:   1D [batches]
    // res: 4D [batches,m,numClass,len]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 2;
    const int32_t detailDepth = axis+1; // default
    const int32_t indexDepth = 1;  // default:x->ndim()
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 2;
    const int32_t shapeX0 = 4;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 2;
    const int32_t shapeB2 = 3;
    const int32_t shapeB3 = 2;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{ 4, 5},{10,11}},
        {{16,17},{22,23}},
        {{26,27},{32,33}},
        {{36,37},{42,43}}
    };
    int32_t X[shapeX0] = {2,2,1,0};
    rc = this->calcScatterbShapes({shapeX0},{shapeA0,shapeA1,shapeA2},{shapeB0,shapeB1,shapeB2,shapeB3},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1][shapeB2][shapeB3];
    Utils::fill<TypeParam>(shapeB0*shapeB1*shapeB2*shapeB3, (TypeParam*)B, 0);
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(2,        m);
    ASSERT_EQ(1,        n);
    ASSERT_EQ(3,        numClass);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(2,        len);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2*shapeB3, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2][shapeB3] = {
        {{{0,0},{0,0},{4,5}},  {{0,0},{0,0},{10,11}}},
        {{{0,0},{0,0},{16,17}},{{0,0},{0,0},{22,23}}},
        {{{0,0},{26,27},{0,0}},{{0,0},{32,33},{0,0}}},
        {{{36,37},{0,0},{0,0}},{{42,43},{0,0},{0,0}}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, ScatterbNormal2Dby2DDims1Axis1) {
    // batchDims = 1
    // axis = 1
    // A:   2D [batches,n]
    // X:   2D [batches,n]
    // res: 2D [batches,numClass]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 1;
    const int32_t detailDepth = axis+1; // default
    const int32_t indexDepth = 2;  // default:x->ndim()
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 2;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 3;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1] = {
        {0,1},
        {4,3},
        {8,8},
        {10,9}
    };
    int32_t X[shapeX0][shapeX1] = {
        {0,1},
        {1,0},
        {2,2},
        {1,0}
    };
    rc = this->calcScatterbShapes({shapeX0,shapeX1},{shapeA0,shapeA1},{shapeB0,shapeB1},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1];
    Utils::fill<TypeParam>(shapeB0*shapeB1, (TypeParam*)B, 0);
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(2,        n);
    ASSERT_EQ(3,        numClass);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(1,        len);
    ASSERT_EQ(shapeB0*shapeB1, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1] = {
        {0,1,0},
        {3,4,0},
        {0,0,8},
        {9,10,0}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, ScatterbNormal3Dby3DDims1Axis1Detail3ind2) {
    // batchDims = 1
    // axis = 1
    // detailDepth = 3
    // indexDepth = 2
    // A:   3D [batches,n,k]
    // X:   3D [batches,n,k]
    // res: 3D [batches,numClass,k]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 1;
    const int32_t detailDepth = 3; // not default
    const int32_t indexDepth = 2;  // not default
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 3;
    const int32_t shapeA2 = 2;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 3;
    const int32_t shapeX2 = 2;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 5;
    const int32_t shapeB2 = 2;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{ 0, 3},{ 0, 3},{ 0, 3}},
        {{12,11},{12,11},{12,11}},
        {{24,25},{24,25},{24,25}},
        {{32,31},{32,31},{32,31}}
    };
    int32_t X[shapeX0][shapeX1][shapeX2] = {
        {{0,1},{0,1},{0,1}},
        {{1,0},{1,0},{1,0}},
        {{2,2},{2,2},{2,2}},
        {{1,0},{1,0},{1,0}}
    };
    rc = this->calcScatterbShapes({shapeX0,shapeX1,shapeX2},{shapeA0,shapeA1,shapeA2},{shapeB0,shapeB1,shapeB2},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1][shapeB2];
    Utils::fill<TypeParam>(shapeB0*shapeB1*shapeB2, (TypeParam*)B, 0);
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(3,        n);
    ASSERT_EQ(5,        numClass);
    ASSERT_EQ(2,        k);
    ASSERT_EQ(1,        len);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{0,0}, {0,3}, {0,0},  {0,0},{0,0}},
        {{0,11},{12,0},{0,0},  {0,0},{0,0}},
        {{0,0}, {0,0}, {24,25},{0,0},{0,0}},
        {{0,31},{32,0},{0,0},  {0,0},{0,0}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, ScatterbNormal4Dby3DDims1Axis1Detail3ind2) {
    // batchDims = 1
    // axis = 1
    // detailDepth = 3
    // indexDepth = 2
    // A:   4D [batches,n,k,len]
    // X:   3D [batches,n,k]
    // res: 4D [batches,numClass,k,len]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 1;
    const int32_t detailDepth = 3; // not default
    const int32_t indexDepth = 2;  // not default
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 3;
    const int32_t shapeA2 = 2;
    const int32_t shapeA3 = 3;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 3;
    const int32_t shapeX2 = 2;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 5;
    const int32_t shapeB2 = 2;
    const int32_t shapeB3 = 3;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2][shapeA3] = {
        {{{ 0, 1, 2},{ 9,10,11}},
         {{ 0, 1, 2},{ 9,10,11}},
         {{ 0, 1, 2},{ 9,10,11}},},
        {{{36,37,38},{33,34,35}},
         {{36,37,38},{33,34,35}},
         {{36,37,38},{33,34,35}},},
        {{{72,73,74},{75,76,77}},
         {{72,73,74},{75,76,77}},
         {{72,73,74},{75,76,77}},},
        {{{96,97,98},{93,94,95}},
         {{96,97,98},{93,94,95}},
         {{96,97,98},{93,94,95}},}
    };
    int32_t X[shapeX0][shapeX1][shapeX2] = {
        {{0,1},{0,1},{0,1}},
        {{1,0},{1,0},{1,0}},
        {{2,2},{2,2},{2,2}},
        {{1,0},{1,0},{1,0}}
    };
    rc = this->calcScatterbShapes({shapeX0,shapeX1,shapeX2},{shapeA0,shapeA1,shapeA2,shapeA3},{shapeB0,shapeB1,shapeB2,shapeB3},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1][shapeB2][shapeB3];
    Utils::fill<TypeParam>(shapeB0*shapeB1*shapeB2*shapeB3, (TypeParam*)B, 0);
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(3,        n);
    ASSERT_EQ(5,        numClass);
    ASSERT_EQ(2,        k);
    ASSERT_EQ(3,        len);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2*shapeB3, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2][shapeB3] = {
        {{{0,1,2},   {0,0,0}},
         {{0,0,0},   {9,10,11}},
         {{0,0,0},   {0,0,0}},
         {{0,0,0},   {0,0,0}},
         {{0,0,0},   {0,0,0}}},
        {{{0,0,0},   {33,34,35}},
         {{36,37,38},{0,0,0}},
         {{0,0,0},   {0,0,0}},
         {{0,0,0},   {0,0,0}},
         {{0,0,0},   {0,0,0}}},
        {{{0,0,0},   {0,0,0}},
         {{0,0,0},   {0,0,0}},
         {{72,73,74},{75,76,77}},
         {{0,0,0},   {0,0,0}},
         {{0,0,0},   {0,0,0}}},
        {{{0,0,0},   {93,94,95}},
         {{96,97,98},{0,0,0}},
         {{0,0,0},   {0,0,0}},
         {{0,0,0},   {0,0,0}},
         {{0,0,0},   {0,0,0}}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, ScatterbNormal5Dby3DDims1Axis2Detail4ind2) {
    // batchDims = 1
    // axis = 2
    // detailDepth = 4
    // indexDepth = 2
    // res: 5D [batches,m,n,k,len]
    // X:   3D [batches,n,k]
    // A:   5D [batches,m,numClass,k,len]
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t batchDims = 1;
    const int32_t axis = 2;
    const int32_t detailDepth = 4; // not default
    const int32_t indexDepth = 2;  // not default
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 3;
    const int32_t shapeA3 = 2;
    const int32_t shapeA4 = 3;
    const int32_t shapeX0 = 4;
    const int32_t shapeX1 = 3;
    const int32_t shapeX2 = 2;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 2;
    const int32_t shapeB2 = 5;
    const int32_t shapeB3 = 2;
    const int32_t shapeB4 = 3;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1][shapeA2][shapeA3][shapeA4] = {
        {{{{ 0, 1, 2},{ 9,10,11}},
          {{ 0, 1, 2},{ 9,10,11}},
          {{ 0, 1, 2},{ 9,10,11}}},
         {{{30,31,32},{39,40,41}},
          {{30,31,32},{39,40,41}},
          {{30,31,32},{39,40,41}}}},
        {{{{66,67,68},{63,64,65}},
          {{66,67,68},{63,64,65}},
          {{66,67,68},{63,64,65}}},
         {{{96,97,98},{93,94,95}},
          {{96,97,98},{93,94,95}},
          {{96,97,98},{93,94,95}}}},
        {{{{132,133,134},{135,136,137}},
          {{132,133,134},{135,136,137}},
          {{132,133,134},{135,136,137}}},
         {{{162,163,164},{165,166,167}},
          {{162,163,164},{165,166,167}},
          {{162,163,164},{165,166,167}}}},
        {{{{186,187,188},{183,184,185}},
          {{186,187,188},{183,184,185}},
          {{186,187,188},{183,184,185}}},
         {{{216,217,218},{213,214,215}},
          {{216,217,218},{213,214,215}},
          {{216,217,218},{213,214,215}}}}
    };
    int32_t X[shapeX0][shapeX1][shapeX2] = {
        {{0,1},{0,1},{0,1}},
        {{1,0},{1,0},{1,0}},
        {{2,2},{2,2},{2,2}},
        {{1,0},{1,0},{1,0}}
    };
    rc = this->calcScatterbShapes({shapeX0,shapeX1,shapeX2},{shapeA0,shapeA1,shapeA2,shapeA3,shapeA4},{shapeB0,shapeB1,shapeB2,shapeB3,shapeB4},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0][shapeB1][shapeB2][shapeB3][shapeB4];
    Utils::fill<TypeParam>(shapeB0*shapeB1*shapeB2*shapeB3*shapeB4, (TypeParam*)B, 0);
    
    ASSERT_EQ(4,        batches);
    ASSERT_EQ(2,        m);
    ASSERT_EQ(3,        n);
    ASSERT_EQ(5,        numClass);
    ASSERT_EQ(2,        k);
    ASSERT_EQ(3,        len);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2*shapeB3*shapeB4, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2][shapeB3][shapeB4] = {
        {{{{ 0, 1, 2},{ 0, 0, 0}},
          {{ 0, 0, 0},{ 9,10,11}},
          {{ 0, 0, 0},{ 0, 0, 0}},
          {{ 0, 0, 0},{ 0, 0, 0}},
          {{ 0, 0, 0},{ 0, 0, 0}}},
         {{{30,31,32},{ 0, 0, 0}},
          {{ 0, 0, 0},{39,40,41}},
          {{ 0, 0, 0},{ 0, 0, 0}},
          {{ 0, 0, 0},{ 0, 0, 0}},
          {{ 0, 0, 0},{ 0, 0, 0}}}},
        {{{{ 0, 0, 0},{63,64,65}},
          {{66,67,68},{ 0, 0, 0}},
          {{ 0, 0, 0},{ 0, 0, 0}},
          {{ 0, 0, 0},{ 0, 0, 0}},
          {{ 0, 0, 0},{ 0, 0, 0}}},
         {{{ 0, 0, 0},{93,94,95}},
          {{96,97,98},{ 0, 0, 0}},
          {{ 0, 0, 0},{ 0, 0, 0}},
          {{ 0, 0, 0},{ 0, 0, 0}},
          {{ 0, 0, 0},{ 0, 0, 0}}}},
        {{{{  0,  0,  0},{  0,  0,  0}},
          {{  0,  0,  0},{  0,  0,  0}},
          {{132,133,134},{135,136,137}},
          {{  0,  0,  0},{  0,  0,  0}},
          {{  0,  0,  0},{  0,  0,  0}}},
         {{{  0,  0,  0},{  0,  0,  0}},
          {{  0,  0,  0},{  0,  0,  0}},
          {{162,163,164},{165,166,167}},
          {{  0,  0,  0},{  0,  0,  0}},
          {{  0,  0,  0},{  0,  0,  0}}}},
        {{{{  0,  0,  0},{183,184,185}},
          {{186,187,188},{  0,  0,  0}},
          {{  0,  0,  0},{  0,  0,  0}},
          {{  0,  0,  0},{  0,  0,  0}},
          {{  0,  0,  0},{  0,  0,  0}}},
         {{{  0,  0,  0},{213,214,215}},
          {{216,217,218},{  0,  0,  0}},
          {{  0,  0,  0},{  0,  0,  0}},
          {{  0,  0,  0},{  0,  0,  0}},
          {{  0,  0,  0},{  0,  0,  0}}}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, ScatterbAddNormal1Dby1DDims0Axis0) {
    // for Embedding backward
    // axis = 0
    // A:   1D [n]
    // X:   1D [n]
    // res: 1D [numClass]
    const int32_t reverse = true;
    const int32_t addMode = true;
    const int32_t batchDims = 0;
    const int32_t axis = 0;
    const int32_t detailDepth = axis+1;
    const int32_t indexDepth = 1; // x.ndim()
    const int32_t shapeA0 = 3;
    const int32_t shapeX0 = 3;
    const int32_t shapeB0 = 4;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0] = {2,2,1};
    int32_t X[shapeX0] = {2,2,1};
    rc = this->calcScatterbShapes({shapeX0},{shapeA0},{shapeB0},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0];
    Utils::fill<TypeParam>(shapeB0, (TypeParam*)B, 0);
    
    ASSERT_EQ(1,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(3,        n);
    ASSERT_EQ(4,        numClass);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(1,        len);
    ASSERT_EQ(shapeB0,  outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)B,(int32_t*)X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0] = {0,1,4,0};
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, GatherbErrorSelectorOutOfRange) {
    // axis = 0
    // A:   1D [numClass]
    // X:   1D [n]
    // res: 1D [n]
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t batchDims = 0;
    const int32_t axis = 0;
    const int32_t detailDepth = axis+1;
    const int32_t indexDepth = 1; // x.ndim()
    const int32_t shapeA0 = 4;
    const int32_t shapeX0 = 3;
    const int32_t shapeB0 = 3;
    int32_t batches;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t len;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0];
    Utils::range<TypeParam>(0,shapeA0,(TypeParam*)A);
    int32_t X[shapeX0] = {2,4,1};
    rc = this->calcGatherbShapes({shapeA0},{shapeX0},axis,batchDims,detailDepth,indexDepth,&batches,&m,&n,&k,&len,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    TypeParam B[shapeB0];
    
    ASSERT_EQ(1,        batches);
    ASSERT_EQ(1,        m);
    ASSERT_EQ(3,        n);
    ASSERT_EQ(4,        numClass);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(1,        len);
    ASSERT_EQ(shapeB0,  outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,batches,m,n,k,len,numClass,(TypeParam*)A,(int32_t*)X,(TypeParam*)B);
    ASSERT_EQ(RINDOW_MATLIB_E_PERM_OUT_OF_RANGE,rc);
}

}
