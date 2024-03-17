#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdbool.h>
#include <numeric>

using testing::ContainerEq;

namespace {

template <typename T>
class ReduceGatherTest : public ::testing::Test {
protected:
    int32_t calcGatherShapes(
        std::initializer_list<int32_t> shapeX,
        std::initializer_list<int32_t> shapeA,
        int32_t axis,
        int32_t *m,int32_t *n,int32_t *k,int32_t *numClass, int32_t *outputSize
        )
    {
        int32_t ndim = (int32_t)shapeA.size();
        if(axis<0) {
            axis = ndim+axis;
        }
        *m = std::accumulate(shapeA.begin(), shapeA.begin()+axis, 1, std::multiplies<int32_t>());
        *numClass = *(shapeA.begin()+axis);
        *n = std::accumulate(shapeA.begin()+axis+1, shapeA.end(), 1, std::multiplies<int32_t>());
        *k = 1;
        *outputSize = (*n)*(*m);
        int32_t sizeX = std::accumulate(shapeX.begin(), shapeX.end(), 1, std::multiplies<int32_t>());
        if(sizeX!=*outputSize) {
            return *outputSize;
        }
        return 0;
    }

    int32_t calcScatterShapes(
        std::initializer_list<int32_t> shapeX,
        std::initializer_list<int32_t> shapeA,
        int32_t axis,
        int32_t numClass,
        int32_t *m,int32_t *n,int32_t *k, int32_t *outputSize
        )
    {
        int32_t ndim = (int32_t)shapeA.size();
        if(axis<0) {
            axis = ndim+axis;
        }
        if(std::accumulate(shapeX.begin(), shapeX.end(), 1, std::multiplies<int32_t>())!=
            std::accumulate(shapeA.begin(), shapeA.end(), 1, std::multiplies<int32_t>())) {
            return 1000;
        }
        *m = std::accumulate(shapeA.begin(), shapeA.begin()+axis, 1, std::multiplies<int32_t>());
        *n = std::accumulate(shapeA.begin()+axis, shapeA.end(), 1, std::multiplies<int32_t>());
        *k = 1;
        *outputSize = (*m)*numClass*(*n);
        return 0;
    }   

    virtual int32_t test_matlib_reducegather(
        int32_t reverse,int32_t addMode,int32_t m,int32_t n,
        int32_t numClass,int32_t dtype, int32_t *x,
        float *a,float *b
    )
    {
        return rindow_matlib_s_reducegather(reverse,addMode,m,n,numClass,dtype,x,a,b);
    }

    virtual int32_t test_matlib_reducegather(
        int32_t reverse,int32_t addMode,int32_t m,int32_t n,
        int32_t numClass,int32_t dtype, int32_t *x,
        double *a, double *b
    )
    {
        return rindow_matlib_d_reducegather(reverse,addMode,m,n,numClass,dtype,x,a,b);
    }

    virtual int32_t test_matlib_reducegather(
        int32_t reverse,int32_t addMode,int32_t m,int32_t n,
        int32_t numClass,int32_t dtype, int32_t *x,
        int32_t *a,int32_t *b
    )
    {
        int32_t data_dtype = rindow_matlib_dtype_int32;
        return rindow_matlib_i_reducegather(reverse,addMode,m,n,numClass,dtype,x,data_dtype,a,b);
    }
};
typedef ::testing::Types<float, double, int32_t> TestTypes;
TYPED_TEST_SUITE(ReduceGatherTest, TestTypes);

TYPED_TEST(ReduceGatherTest, GatherNormal1DbyAxis0) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeX = 3;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 3;
    const int32_t axis = 0;
    const int32_t shapeB = shapeA1;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    int32_t X[shapeX] = {3,2,1};
    TypeParam A[shapeA0*shapeA1] = {
        0, 0, 3,
        0, 0, 4,
        0, 2, 0,
        1, 0, 0
    };
    TypeParam B[shapeB];
    rc = this->calcGatherShapes({shapeX},{shapeA0,shapeA1},axis,&m,&n,&k,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(1,        m);
    ASSERT_EQ(shapeA1,  n);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(shapeA0,  numClass);
    ASSERT_EQ(shapeB,   outputSize);

    rc = this->test_matlib_reducegather(reverse,addMode,m,n,numClass,dtype,X,A,B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB] = {1,2,4};
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(ReduceGatherTest, GatherNormal2DbyAxis0) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeX0 = 2;
    const int32_t shapeX1 = 4;
    const int32_t shapeA0 = 3;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 4;
    const int32_t axis = 0;
    const int32_t shapeB = shapeA1*shapeA2;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    int32_t X[shapeX0*shapeX1] = {
        0,2,2,1,
        2,0,1,2
    };
    TypeParam A[shapeA0*shapeA1*shapeA2] = {
        1, 0, 0, 0,
        0, 6, 0, 0,

        0, 0, 0, 4,
        0, 0, 7, 0,

        0, 2, 3, 0,
        5, 0, 0, 8
    };
    TypeParam B[shapeB];
    rc = this->calcGatherShapes({shapeX0,shapeX1},{shapeA0,shapeA1,shapeA2},axis,&m,&n,&k,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(1,                m);
    ASSERT_EQ(shapeA1*shapeA2,  n);
    ASSERT_EQ(1,                k);
    ASSERT_EQ(shapeA0,          numClass);
    ASSERT_EQ(shapeB,           outputSize);

    rc = this->test_matlib_reducegather(reverse,addMode,m,n,numClass,dtype,X,A,B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB] = {
        1,2,3,4,
        5,6,7,8,
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(ReduceGatherTest, GatherNormal1DbyAxis1) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeX = 4;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 3;
    const int32_t axis = 1;
    const int32_t shapeB = shapeA0;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    int32_t X[shapeX] = {2,2,1,0};
    TypeParam A[shapeA0*shapeA1] = {
        0, 0, 3,
        0, 0, 4,
        0, 2, 0,
        1, 0, 0
    };
    TypeParam B[shapeB];
    rc = this->calcGatherShapes({shapeX},{shapeA0,shapeA1},axis,&m,&n,&k,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(shapeA0,  m);
    ASSERT_EQ(1,        n);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(shapeA1,  numClass);
    ASSERT_EQ(shapeB,   outputSize);

    rc = this->test_matlib_reducegather(reverse,addMode,m,n,numClass,dtype,X,A,B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB] = {3,4,2,1};
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(ReduceGatherTest, GatherNormal2DbyAxis1) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeX0 = 3;
    const int32_t shapeX1 = 4;
    const int32_t shapeA0 = 3;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 4;
    const int32_t axis = 1;
    const int32_t shapeB = shapeA0*shapeA2;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    int32_t X[shapeX0*shapeX1] = {
        1,0,1,0,
        1,0,1,0,
        1,0,1,0,
    };
    TypeParam A[shapeA0*shapeA1*shapeA2] = {
        0, 2, 0, 4,
        1, 0, 3, 0,

        0, 6, 0, 8,
        5, 0, 7, 0,

        0,10, 0,12,
        9, 0,11, 0
    };
    TypeParam B[shapeB];
    rc = this->calcGatherShapes({shapeX0,shapeX1},{shapeA0,shapeA1,shapeA2},axis,&m,&n,&k,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(shapeA0,          m);
    ASSERT_EQ(shapeA2,          n);
    ASSERT_EQ(1,                k);
    ASSERT_EQ(shapeA1,          numClass);
    ASSERT_EQ(shapeB,           outputSize);

    rc = this->test_matlib_reducegather(reverse,addMode,m,n,numClass,dtype,X,A,B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9,10,11,12
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(ReduceGatherTest, ScatterAddExNormal1DbyAxis0) {
    const int32_t reverse = true; 
    const int32_t addMode = true;
    const int32_t shapeX = 3;
    const int32_t shapeA = 3;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 3;
    const int32_t axis = 0;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    int32_t X[shapeX] = {3,2,0};
    TypeParam A[shapeA] = {1,2,3};
    TypeParam B[shapeB0*shapeB1] = {
        1,1,1,
        1,1,1,
        1,1,1,
        1,1,1
    };
    rc = this->calcGatherShapes({shapeX},{shapeB0,shapeB1},axis,&m,&n,&k,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(1,        m);
    ASSERT_EQ(shapeB1,  n);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(shapeB0,  numClass);
    ASSERT_EQ(shapeA,   outputSize);

    rc = this->test_matlib_reducegather(reverse,addMode,m,n,numClass,dtype,X,B,A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0*shapeB1] = {
        1, 1, 4,
        1, 1, 1,
        1, 3, 1,
        2, 1, 1
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(ReduceGatherTest, ScatterAddExNormal1DbyAxis1) {
    const int32_t reverse = true; 
    const int32_t addMode = true;
    const int32_t shapeX = 4;
    const int32_t shapeA = 4;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 3;
    const int32_t axis = 1;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;
    int32_t rc;

    int32_t X[shapeX] = {0,1,2,0};
    TypeParam A[shapeA] = {1,5,9,10};
    TypeParam B[shapeB0*shapeB1] = {
        1,1,1,
        1,1,1,
        1,1,1,
        1,1,1
    };
    rc = this->calcGatherShapes({shapeX},{shapeB0,shapeB1},axis,&m,&n,&k,&numClass,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(shapeB0,  m);
    ASSERT_EQ(1,        n);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(shapeB1,  numClass);
    ASSERT_EQ(shapeA,   outputSize);

    rc = this->test_matlib_reducegather(reverse,addMode,m,n,numClass,dtype,X,B,A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0*shapeB1] = {
         2, 1, 1,
         1, 6, 1,
         1, 1,10,
        11, 1, 1
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(ReduceGatherTest, ScatterNormal1DbyAxis0) {
    const int32_t reverse = true;
    const int32_t addMode = false;
    const int32_t shapeX = 3;
    const int32_t shapeA = 3;
    const int32_t axis = 0;
    const int32_t numClass = 4;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 3;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;
    int32_t rc;

    int32_t X[shapeX] = {3,2,0};
    TypeParam A[shapeA] = {1,2,3};
    TypeParam B[shapeB0*shapeB1] = {
        100, 100, 100,
        100, 100, 100,
        100, 100, 100,
        100, 100, 100
    };
    rc = this->calcScatterShapes({shapeX},{shapeA},axis,numClass,&m,&n,&k,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(1,                m);
    ASSERT_EQ(shapeA,           n);
    ASSERT_EQ(1,                k);
    ASSERT_EQ(numClass,         numClass);
    ASSERT_EQ(shapeB0*shapeB1,  outputSize);

    rc = this->test_matlib_reducegather(reverse,addMode,m,n,numClass,dtype,X,B,A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0*shapeB1] = {
        100, 100,   3,
        100, 100, 100,
        100,   2, 100,
          1, 100, 100
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(ReduceGatherTest, ScatterNormal2DbyAxis0) {
    const int32_t reverse = true;
    const int32_t addMode = false;
    const int32_t shapeX0 = 2;
    const int32_t shapeX1 = 4;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 4;
    const int32_t axis = 0;
    const int32_t numClass = 3;
    const int32_t shapeB0 = 3;
    const int32_t shapeB1 = 2;
    const int32_t shapeB2 = 4;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;
    int32_t rc;

    int32_t X[shapeX0*shapeX1] = {
        0,2,2,1,
        2,0,1,2
    };
    TypeParam A[shapeA0*shapeA1] = {
        1,2,3,4,
        5,6,7,8
    };
    TypeParam B[shapeB0*shapeB1*shapeB2] = {
        100, 100, 100, 100,
        100, 100, 100, 100,

        100, 100, 100, 100,
        100, 100, 100, 100,

        100, 100, 100, 100,
        100, 100, 100, 100
    };
    rc = this->calcScatterShapes({shapeX0,shapeX1},{shapeA0,shapeA1},axis,numClass,&m,&n,&k,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(1,                m);
    ASSERT_EQ(shapeA0*shapeA1,  n);
    ASSERT_EQ(1,                k);
    ASSERT_EQ(numClass,         numClass);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2,  outputSize);

    rc = this->test_matlib_reducegather(reverse,addMode,m,n,numClass,dtype,X,B,A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0*shapeB1*shapeB2] = {
          1, 100, 100, 100,
        100,   6, 100, 100,

        100, 100, 100,   4,
        100, 100,   7, 100,

        100,   2,   3, 100,
          5, 100, 100,   8
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(ReduceGatherTest, ScatterNormal1DbyAxis1) {
    const int32_t reverse = true;
    const int32_t addMode = false;
    const int32_t shapeX = 4;
    const int32_t shapeA = 4;
    const int32_t axis = 1;
    const int32_t numClass = 3;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 3;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;
    int32_t rc;

    int32_t X[shapeX] = {0,1,2,0};
    TypeParam A[shapeA] = {1,5,9,10};
    TypeParam B[shapeB0*shapeB1] = {
        100, 100, 100,
        100, 100, 100,
        100, 100, 100,
        100, 100, 100
    };
    rc = this->calcScatterShapes({shapeX},{shapeA},axis,numClass,&m,&n,&k,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(shapeA,           m);
    ASSERT_EQ(1,                n);
    ASSERT_EQ(1,                k);
    ASSERT_EQ(numClass,         numClass);
    ASSERT_EQ(shapeB0*shapeB1,  outputSize);

    rc = this->test_matlib_reducegather(reverse,addMode,m,n,numClass,dtype,X,B,A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0*shapeB1] = {
          1,100,100,
        100,  5,100,
        100,100,  9,
         10,100,100
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(ReduceGatherTest, ScatterNormal2DbyAxis1) {
    const int32_t reverse = true;
    const int32_t addMode = false;
    const int32_t shapeX0 = 3;
    const int32_t shapeX1 = 4;
    const int32_t shapeA0 = 3;
    const int32_t shapeA1 = 4;
    const int32_t axis = 1;
    const int32_t numClass = 2;
    const int32_t shapeB0 = 3;
    const int32_t shapeB1 = 2;
    const int32_t shapeB2 = 4;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;
    int32_t rc;

    int32_t X[shapeX0*shapeX1] = {
        1,0,1,0,
        1,0,1,0,
        1,0,1,0
    };
    TypeParam A[shapeA0*shapeA1] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9,10,11,12
    };
    TypeParam B[shapeB0*shapeB1*shapeB2] = {
        100, 100, 100, 100,
        100, 100, 100, 100,

        100, 100, 100, 100,
        100, 100, 100, 100,

        100, 100, 100, 100,
        100, 100, 100, 100
    };
    rc = this->calcScatterShapes({shapeX0,shapeX1},{shapeA0,shapeA1},axis,numClass,&m,&n,&k,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(shapeA0,          m);
    ASSERT_EQ(shapeA1,          n);
    ASSERT_EQ(1,                k);
    ASSERT_EQ(numClass,         numClass);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2,  outputSize);

    rc = this->test_matlib_reducegather(reverse,addMode,m,n,numClass,dtype,X,B,A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0*shapeB1*shapeB2] = {
        100,   2, 100,   4,
          1, 100,   3, 100,

        100,   6, 100,   8,
          5, 100,   7, 100,

        100,  10, 100,  12,
          9, 100,  11, 100
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(ReduceGatherTest, ScatterNormal3DbyAxis1) {
    const int32_t reverse = true;
    const int32_t addMode = false;
    const int32_t shapeX0 = 3;
    const int32_t shapeX1 = 2;
    const int32_t shapeX2 = 2;
    const int32_t shapeA0 = 3;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 2;
    const int32_t axis = 1;
    const int32_t numClass = 2;
    const int32_t shapeB0 = 3;
    const int32_t shapeB1 = 2;
    const int32_t shapeB2 = 2;
    const int32_t shapeB3 = 2;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;
    int32_t rc;

    int32_t X[shapeX0*shapeX1*shapeX2] = {
        1,0,  1,0,
        1,0,  1,0,
        1,0,  1,0,
    };
    TypeParam A[shapeA0*shapeA1*shapeA2] = {
        1, 2,   3, 4,
        5, 6,   7, 8,
        9,10,  11,12
    };
    TypeParam B[shapeB0*shapeB1*shapeB2*shapeB3] = {
        100,100,  100,100,
        100,100,  100,100,

        100,100,  100,100,
        100,100,  100,100,

        100,100,  100,100,
        100,100,  100,100
    };
    rc = this->calcScatterShapes({shapeX0,shapeX1,shapeX2},{shapeA0,shapeA1,shapeA2},axis,numClass,&m,&n,&k,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(shapeA0,          m);
    ASSERT_EQ(shapeA1*shapeA2,  n);
    ASSERT_EQ(1,                k);
    ASSERT_EQ(numClass,         numClass);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2*shapeB3,  outputSize);

    rc = this->test_matlib_reducegather(reverse,addMode,m,n,numClass,dtype,X,B,A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0*shapeB1*shapeB2*shapeB3] = {
        100, 2,   100, 4,
        1, 100,   3, 100,

        100, 6,   100, 8,
        5, 100,   7, 100,

        100,10,   100,12,
        9, 100,   11,100
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
}
