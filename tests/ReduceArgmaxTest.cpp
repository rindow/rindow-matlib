#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <numeric>

using testing::ContainerEq;

namespace {

template <typename T>
class ReduceArgmaxTest : public ::testing::Test {
protected:
    int32_t calcShape(
        std::initializer_list<int32_t> shape,
        int32_t axis,
        int32_t *m,int32_t *n,int32_t *k, int32_t *outputSize
        )
    {
        int32_t ndim = (int32_t)shape.size();
        if(axis<0) {
            axis = ndim+axis;
        }
        if(axis<0 || axis>ndim-1) {
            std::cout << "axis=" << axis << ",ndim=" << "ndim" << std::endl;
            throw std::invalid_argument("Invalid axis.");
        }
        *m = std::accumulate(shape.begin(), shape.begin()+axis, 1, std::multiplies<int32_t>());
        *n = *(shape.begin()+axis);
        *k = std::accumulate(shape.begin()+axis+1, shape.end(), 1, std::multiplies<int32_t>());
        *outputSize = (*m)*(*k);
        return 0;
    }

    virtual void test_matlib_reduceargmax(
        int32_t m,int32_t n,int32_t k,
        float *a,
        int32_t dtype,
        void *b
    )
    {
        return rindow_matlib_s_reduceargmax(m,n,k,a,dtype,b);
    }

    virtual void test_matlib_reduceargmax(
        int32_t m,int32_t n,int32_t k,
        double *a,
        int32_t dtype,
        void *b
    )
    {
        return rindow_matlib_d_reduceargmax(m,n,k,a,dtype,b);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(ReduceArgmaxTest, TestTypes);

TYPED_TEST(ReduceArgmaxTest, Array2DAxis0) {
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 3;
    const int32_t shapeB = shapeA1;
    const int32_t axis = 0;
    const int32_t IndexDtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;

    TypeParam A[shapeA0][shapeA1] = {
        {1,2,3},
        {4,5,6}
    };
    int32_t B[shapeB];
    this->calcShape({shapeA0,shapeA1},axis,&m,&n,&k,&outputSize);
    
    ASSERT_EQ(1,        m);
    ASSERT_EQ(shapeA0,  n);
    ASSERT_EQ(shapeA1,  k);
    ASSERT_EQ(shapeB,   outputSize);

    this->test_matlib_reduceargmax(
        m,n,k,
        (TypeParam*)A,
        IndexDtype,B
    );

    int32_t R1[shapeB] = {1,1,1};
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(ReduceArgmaxTest, Array2DAxis1) {
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 3;
    const int32_t shapeB = shapeA0;
    const int32_t axis = 1;
    const int32_t IndexDtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;

    TypeParam A[shapeA0][shapeA1] = {
        {1,2,3},
        {4,5,6}
    };
    int32_t B[shapeB];
    this->calcShape({shapeA0,shapeA1},axis,&m,&n,&k,&outputSize);
    
    ASSERT_EQ(shapeA0,  m);
    ASSERT_EQ(shapeA1,  n);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(shapeB,   outputSize);

    this->test_matlib_reduceargmax(
        m,n,k,
        (TypeParam*)A,
        IndexDtype,B
    );

    int32_t R1[shapeB] = {2,2};
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(ReduceArgmaxTest, Array3DAxis0) {
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 2;
    const int32_t shapeB0 = shapeA1;
    const int32_t shapeB1 = shapeA2;
    const int32_t axis = 0;
    const int32_t IndexDtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{1,2},
         {3,4}},
        {{5,6},
         {7,8}}
    };
    int32_t B[shapeB0][shapeB1];
    this->calcShape({shapeA0,shapeA1,shapeA2},axis,&m,&n,&k,&outputSize);
    
    ASSERT_EQ(1,                m);
    ASSERT_EQ(shapeA0,          n);
    ASSERT_EQ(shapeA1*shapeA2,  k);
    ASSERT_EQ(shapeB0*shapeB1,  outputSize);

    this->test_matlib_reduceargmax(
        m,n,k,
        (TypeParam*)A,
        IndexDtype,(int32_t*)B
    );

    int32_t R1[shapeB0][shapeB1] = {{1,1},{1,1}};
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(ReduceArgmaxTest, Array3DAxis1) {
    const int32_t shapeA0 = 3;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 4;
    const int32_t shapeB0 = shapeA0;
    const int32_t shapeB1 = shapeA2;
    const int32_t axis = 1;
    const int32_t IndexDtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{1,4,5,8},
         {2,3,6,7}},
        {{1,4,5,8},
         {2,3,6,7}},
        {{1,4,5,8},
         {2,3,6,7}},
    };
    int32_t B[shapeB0][shapeB1];
    this->calcShape({shapeA0,shapeA1,shapeA2},axis,&m,&n,&k,&outputSize);
    
    ASSERT_EQ(shapeA0,          m);
    ASSERT_EQ(shapeA1,          n);
    ASSERT_EQ(shapeA2,          k);
    ASSERT_EQ(shapeB0*shapeB1,  outputSize);

    this->test_matlib_reduceargmax(
        m,n,k,
        (TypeParam*)A,
        IndexDtype,(int32_t*)B
    );

    int32_t R1[shapeB0][shapeB1] = {
        {1,0,1,0},
        {1,0,1,0},
        {1,0,1,0},
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

}
