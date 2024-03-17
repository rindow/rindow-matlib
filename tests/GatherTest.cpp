#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdbool.h>
#include <numeric>

using testing::ContainerEq;

namespace {

template <typename T>
class GatherTest : public ::testing::Test {
protected:
    void calcGatherShapes(
        std::initializer_list<int32_t> shapeX,
        std::initializer_list<int32_t> shapeA,
        int32_t *m,int32_t *n,int32_t *k,int32_t *numClass, int32_t *outputSize
        )
    {
        auto postfixShape = shapeA.begin();
        *numClass = *postfixShape++;
        *m = 1;
        *n = std::accumulate(shapeX.begin(), shapeX.end(), 1, std::multiplies<int32_t>());
        *k = std::accumulate(postfixShape, shapeA.end(), 1, std::multiplies<int32_t>());
        *outputSize = (*n)*(*k);
    }   

    int32_t calcScatterShapes(
        std::initializer_list<int32_t> shapeX,
        std::initializer_list<int32_t> shapeA,
        int32_t numClass,
        int32_t *m,int32_t *n,int32_t *k, int32_t *outputSize
        )
    {
        int32_t ndimX = (int32_t)shapeX.size();
        int32_t tmpA = std::accumulate(shapeA.begin(), shapeA.begin()+ndimX, 1, std::multiplies<int32_t>());
        *n = std::accumulate(shapeX.begin(), shapeX.end(), 1, std::multiplies<int32_t>());
        if(tmpA!=*n) {
            return tmpA;
        }
        *k = std::accumulate(shapeA.begin()+ndimX, shapeA.end(), 1, std::multiplies<int32_t>());
        *m = 1;
        *outputSize = numClass*(*k);
        return 0;
    }   

    virtual int32_t test_matlib_gather(
        int32_t reverse, int32_t addMode, int32_t n, int32_t k,
        int32_t numClass, int32_t dtype, int32_t *x,
        float *a, float *b
    )
    {
        return rindow_matlib_s_gather(reverse,addMode,n,k,numClass,dtype,x,a,b);
    }

    virtual int32_t test_matlib_gather(
        int32_t reverse, int32_t addMode, int32_t n, int32_t k,
        int32_t numClass, int32_t dtype, int32_t *x,
        double *a, double *b
    )
    {
        return rindow_matlib_d_gather(reverse,addMode,n,k,numClass,dtype,x,a,b);
    }

    virtual int32_t test_matlib_gather(
        int32_t reverse, int32_t addMode, int32_t n, int32_t k,
        int32_t numClass, int32_t dtype, int32_t *x,
        int32_t *a, int32_t *b
    )
    {
        int32_t data_dtype = rindow_matlib_dtype_int32;
        return rindow_matlib_i_gather(reverse,addMode,n,k,numClass,dtype,x,data_dtype,a,b);
    }

};
typedef ::testing::Types<float, double, int32_t> TestTypes;
TYPED_TEST_SUITE(GatherTest, TestTypes);

TYPED_TEST(GatherTest, GatherNormal1Dby1D) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeX = 4;
    const int32_t shapeA = 10;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;

    int32_t X[shapeX] = {3,2,1,1};
    TypeParam A[shapeA] = {10,11,12,13,14,15,16,17,18,19};
    this->calcGatherShapes({shapeX},{shapeA},&m,&n,&k,&numClass,&outputSize);
    TypeParam B[shapeX];
    
    ASSERT_EQ(1,      m);
    ASSERT_EQ(shapeX, n);
    ASSERT_EQ(1,      k);
    ASSERT_EQ(shapeA, numClass);
    ASSERT_EQ(shapeX, outputSize);

    int32_t rc = this->test_matlib_gather(reverse,addMode,n,k,numClass,dtype,X,A,B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeX] = {13,12,11,11};
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherTest, GatherNormal1Dby2D) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeX0 = 2;
    const int32_t shapeX1 = 3;
    const int32_t shapeA = 10;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;

    int32_t X[shapeX0*shapeX1] = {
        3,2,1,
        1,2,3
    };
    TypeParam A[shapeA] = {10,11,12,13,14,15,16,17,18,19};
    this->calcGatherShapes({shapeX0,shapeX1},{shapeA},&m,&n,&k,&numClass,&outputSize);
    TypeParam B[shapeX0*shapeX1];
    
    ASSERT_EQ(1,                m);
    ASSERT_EQ(shapeX0*shapeX1,  n);
    ASSERT_EQ(1,                k);
    ASSERT_EQ(shapeA,           numClass);
    ASSERT_EQ(shapeX0*shapeX1,  outputSize);

    int32_t rc = this->test_matlib_gather(reverse,addMode,n,k,numClass,dtype,X,A,B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeX0*shapeX1] = {
        13,12,11,
        11,12,13
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherTest, GatherNormal2Dby1D) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeX = 2;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 3;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;

    int32_t X[shapeX] = {
        3,1
    };
    TypeParam A[shapeA0*shapeA1] = {
        0, 0, 3,
        0, 0, 4,
        0, 2, 0,
        1, 0, 0
    };
    this->calcGatherShapes({shapeX},{shapeA0,shapeA1},&m,&n,&k,&numClass,&outputSize);
    TypeParam B[shapeX*shapeA1];
    
    ASSERT_EQ(1,                m);
    ASSERT_EQ(shapeX,           n);
    ASSERT_EQ(shapeA1,          k);
    ASSERT_EQ(shapeA0,          numClass);
    ASSERT_EQ(shapeX*shapeA1,   outputSize);

    int32_t rc = this->test_matlib_gather(reverse,addMode,n,k,numClass,dtype,X,A,B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeX*shapeA1] = {
        1,0,0,
        0,0,4
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherTest, GatherNormal2Dby2D) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeX0 = 2;
    const int32_t shapeX1 = 3;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 3;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;

    int32_t X[shapeX0*shapeX1] = {
        2, 1, 0,
        1, 2, 3,
    };
    TypeParam A[shapeA0*shapeA1] = {
        1, 0, 0,
        0, 2, 0,
        0, 0, 3,
        4, 0, 0
    };
    this->calcGatherShapes({shapeX0,shapeX1},{shapeA0,shapeA1},&m,&n,&k,&numClass,&outputSize);
    TypeParam B[shapeX0*shapeX1*shapeA1];
    
    ASSERT_EQ(1,                m);
    ASSERT_EQ(shapeX0*shapeX1,  n);
    ASSERT_EQ(shapeA1,          k);
    ASSERT_EQ(shapeA0,          numClass);
    ASSERT_EQ(shapeX0*shapeX1*shapeA1,  outputSize);

    int32_t rc = this->test_matlib_gather(reverse,addMode,n,k,numClass,dtype,X,A,B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeX0*shapeX1*shapeA1] = {
        0,0,3,
        0,2,0,
        1,0,0,

        0,2,0,
        0,0,3,
        4,0,0,
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(GatherTest, ScatterAddExNormal1Dby1D) {
    const int32_t reverse = true; 
    const int32_t addMode = true;
    const int32_t shapeX = 4;
    const int32_t shapeA = 4;
    const int32_t shapeB = 10;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;

    int32_t X[shapeX] = {3,2,1,4};
    TypeParam A[shapeA] = {13,12,11,14};
    TypeParam B[shapeB] = {1,1,1,1,1,1,1,1,1,1};
    this->calcGatherShapes({shapeX},{shapeB},&m,&n,&k,&numClass,&outputSize);
    
    ASSERT_EQ(1,      m);
    ASSERT_EQ(shapeX, n);
    ASSERT_EQ(1,      k);
    ASSERT_EQ(shapeB, numClass);
    ASSERT_EQ(shapeA, outputSize);

    int32_t rc = this->test_matlib_gather(reverse,addMode,n,k,numClass,dtype,X,B,A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB] = {1,12,13,14,15,1,1,1,1,1};
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(GatherTest, ScatterAddExNormal2Dby1D) {
    const int32_t reverse = true; 
    const int32_t addMode = true;
    const int32_t shapeX = 2;
    const int32_t shapeA0 = shapeX;
    const int32_t shapeA1 = 3;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = shapeA1;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;

    int32_t X[shapeX] = {1,1};
    TypeParam A[shapeA0][shapeA1] = {{1,1,1},{1,1,1}};
    TypeParam B[shapeB0][shapeB1] = {{1,1,1},{1,1,1},{1,1,1},{1,1,1}};
    this->calcGatherShapes({shapeX},{shapeB0,shapeB1},&m,&n,&k,&numClass,&outputSize);
    
    ASSERT_EQ(1,      m);
    ASSERT_EQ(shapeX, n);
    ASSERT_EQ(shapeB1, k);
    ASSERT_EQ(shapeB0, numClass);
    ASSERT_EQ(shapeA0*shapeA1, outputSize);

    int32_t rc = this->test_matlib_gather(reverse,addMode,n,k,numClass,dtype,X,(TypeParam*)B,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1] = {{1,1,1},{3,3,3},{1,1,1},{1,1,1}};
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(GatherTest, testScatterExNormal2Dby2D) {
    const int32_t reverse = true;
    const int32_t addMode = false;
    const int32_t numClass = 10;
    const int32_t shapeX = 2*3;
    const int32_t shapeA = 2*3;
    const int32_t shapeB = numClass*shapeA/shapeX;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;
    int32_t rc;

    int32_t X[shapeX] = {
        3,2,1,
        5,6,7
    };
    TypeParam A[shapeA] = {
        13,12,11,
        15,16,17
    };
    TypeParam B[shapeB] = {100,100,100,100,100,100,100,100,100,100};
    rc = this->calcScatterShapes({2,3},{2,3},numClass,&m,&n,&k,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(1,                m);
    ASSERT_EQ(shapeX,           n);
    ASSERT_EQ(shapeA/shapeX,    k);
    ASSERT_EQ(shapeB,           outputSize);

    rc = this->test_matlib_gather(reverse,addMode,n,k,numClass,dtype,X,B,A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB] = {100,11,12,13,100,15,16,17,100,100};
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(GatherTest, testScatterExNormal2Dby1D) {
    const int32_t reverse = true;
    const int32_t addMode = false;
    const int32_t numClass = 4;
    const int32_t shapeX = 2;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 3;
    const int32_t shapeB = numClass*shapeA1;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;
    int32_t rc;

    int32_t X[shapeX] = {3,1};
    TypeParam A[shapeA0*shapeA1] = {
        1,2,3,
        2,3,4
    };
    TypeParam B[shapeB] = {100,100,100,100,100,100,100,100,100,100,100,100};
    rc = this->calcScatterShapes({shapeX},{shapeA0,shapeA1},numClass,&m,&n,&k,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(1,        m);
    ASSERT_EQ(shapeX,   n);
    ASSERT_EQ(shapeA1,  k);
    ASSERT_EQ(shapeB,   outputSize);

    rc = this->test_matlib_gather(reverse,addMode,n,k,numClass,dtype,X,B,A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB] = {
        100, 100, 100,
          2,   3,   4,
        100, 100, 100,
          1,   2,   3
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(GatherTest, testScatterExNormal3Dby2D) {
    const int32_t reverse = true;
    const int32_t addMode = false;
    const int32_t numClass = 4;
    const int32_t shapeX0 = 2;
    const int32_t shapeX1 = 3;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 3;
    const int32_t shapeA2 = 3;
    const int32_t shapeB = numClass*shapeA2;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;
    int32_t rc;

    int32_t X[shapeX0*shapeX1] = {
        2, 1, 0,
        1, 2, 3
    };
    TypeParam A[shapeA0*shapeA1*shapeA2] = {
        0,0,3,
        0,2,0,
        1,0,0,

        0,2,0,
        0,0,3,
        4,0,0
    };
    TypeParam B[shapeB] = {
        100,100,100,
        100,100,100,
        100,100,100,
        100,100,100
    };
    rc = this->calcScatterShapes({shapeX0,shapeX1},{shapeA0,shapeA1,shapeA2},numClass,&m,&n,&k,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(1,                m);
    ASSERT_EQ(shapeX0*shapeX1,  n);
    ASSERT_EQ(shapeA2,          k);
    ASSERT_EQ(shapeB,           outputSize);

    rc = this->test_matlib_gather(reverse,addMode,n,k,numClass,dtype,X,B,A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB] = {
        1, 0, 0,
        0, 2, 0,
        0, 0, 3,
        4, 0, 0
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
}
