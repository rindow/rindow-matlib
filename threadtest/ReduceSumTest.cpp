#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <numeric>
#include <limits>

using testing::ContainerEq;

namespace {

template <typename T>
class ReduceSumTest : public ::testing::Test {
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

    virtual void test_matlib_reducesum(
        int32_t m,int32_t n,int32_t k,
        float *a,
        float *b
    )
    {
        return rindow_matlib_s_reducesum(m,n,k,a,b);
    }

    virtual void test_matlib_reducesum(
        int32_t m,int32_t n,int32_t k,
        double *a,
        double *b
    )
    {
        return rindow_matlib_d_reducesum(m,n,k,a,b);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(ReduceSumTest, TestTypes);

TYPED_TEST(ReduceSumTest, Array2DAxis0) {
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 3;
    const int32_t shapeB = shapeA1;
    const int32_t axis = 0;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;

    TypeParam A[shapeA0][shapeA1] = {
        {1,2,3},
        {4,5,6}
    };
    TypeParam B[shapeB];
    this->calcShape({shapeA0,shapeA1},axis,&m,&n,&k,&outputSize);
    
    ASSERT_EQ(1,        m);
    ASSERT_EQ(shapeA0,  n);
    ASSERT_EQ(shapeA1,  k);
    ASSERT_EQ(shapeB,   outputSize);

    this->test_matlib_reducesum(
        m,n,k,
        (TypeParam*)A,
        (TypeParam*)B
    );

    TypeParam R1[shapeB] = {5,7,9};
    EXPECT_THAT(R1, ContainerEq(B));

    //  minus
    TypeParam A2[shapeA0][shapeA1] = {
        {-1,-2,-3},
        {-4,-5,-6}
    };
    TypeParam B2[shapeB];

    this->test_matlib_reducesum(
        m,n,k,
        (TypeParam*)A2,
        (TypeParam*)B2
    );

    TypeParam R2[shapeB] = {-5,-7,-9};
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(ReduceSumTest, Array2DAxis1) {
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 3;
    const int32_t shapeB = shapeA0;
    const int32_t axis = 1;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;

    TypeParam A[shapeA0][shapeA1] = {
        {1,2,3},
        {4,5,6}
    };
    TypeParam B[shapeB];
    this->calcShape({shapeA0,shapeA1},axis,&m,&n,&k,&outputSize);
    
    ASSERT_EQ(shapeA0,  m);
    ASSERT_EQ(shapeA1,  n);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(shapeB,   outputSize);

    this->test_matlib_reducesum(
        m,n,k,
        (TypeParam*)A,
        (TypeParam*)B
    );

    TypeParam R1[shapeB] = {6,15};
    EXPECT_THAT(R1, ContainerEq(B));

    //  minus
    TypeParam A2[shapeA0][shapeA1] = {
        {-1,-2,-3},
        {-4,-5,-6}
    };
    TypeParam B2[shapeB];

    this->test_matlib_reducesum(
        m,n,k,
        (TypeParam*)A2,
        (TypeParam*)B2
    );

    TypeParam R2[shapeB] = {-6,-15};
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(ReduceSumTest, Array3DAxis0) {
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 2;
    const int32_t shapeB0 = shapeA1;
    const int32_t shapeB1 = shapeA2;
    const int32_t axis = 0;
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
    TypeParam B[shapeB0][shapeB1];
    this->calcShape({shapeA0,shapeA1,shapeA2},axis,&m,&n,&k,&outputSize);
    
    ASSERT_EQ(1,                m);
    ASSERT_EQ(shapeA0,          n);
    ASSERT_EQ(shapeA1*shapeA2,  k);
    ASSERT_EQ(shapeB0*shapeB1,  outputSize);

    this->test_matlib_reducesum(
        m,n,k,
        (TypeParam*)A,
        (TypeParam*)B
    );

    TypeParam R1[shapeB0][shapeB1] = {{6,8},{10,12}};
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(ReduceSumTest, Array3DAxis1) {
    const int32_t shapeA0 = 3;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 4;
    const int32_t shapeB0 = shapeA0;
    const int32_t shapeB1 = shapeA2;
    const int32_t axis = 1;
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
    TypeParam B[shapeB0][shapeB1];
    this->calcShape({shapeA0,shapeA1,shapeA2},axis,&m,&n,&k,&outputSize);
    
    ASSERT_EQ(shapeA0,          m);
    ASSERT_EQ(shapeA1,          n);
    ASSERT_EQ(shapeA2,          k);
    ASSERT_EQ(shapeB0*shapeB1,  outputSize);

    this->test_matlib_reducesum(
        m,n,k,
        (TypeParam*)A,
        (TypeParam*)B
    );

    TypeParam R1[shapeB0][shapeB1] = {
        {4,6},
        {12,14},
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(ReduceSumTest, Array2DAxis1WithNaN) {
    const int32_t shapeA0 = 7;
    const int32_t shapeA1 = 2;
    const int32_t shapeB = shapeA0;
    const int32_t axis = 1;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;
    TypeParam inf = std::numeric_limits<TypeParam>::infinity();

    TypeParam A[shapeA0][shapeA1] = {
        {1.0, 2.0},
        {inf, 1.0},
        {-inf,1.0},
        {inf, inf},
        {-inf,inf},
        {0.0, NAN},
        {inf, NAN}
    };
    TypeParam B[shapeB];
    this->calcShape({shapeA0,shapeA1},axis,&m,&n,&k,&outputSize);
    
    ASSERT_EQ(shapeA0,  m);
    ASSERT_EQ(shapeA1,  n);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(shapeB,   outputSize);

    this->test_matlib_reducesum(
        m,n,k,
        (TypeParam*)A,
        (TypeParam*)B
    );

    EXPECT_EQ(3,        B[0]);
    EXPECT_EQ(inf,      B[1]);
    EXPECT_EQ(-inf,     B[2]);
    EXPECT_EQ(inf,      B[3]);
    EXPECT_TRUE(std::isnan(B[4]));
    EXPECT_TRUE(std::isnan(B[5]));
    EXPECT_TRUE(std::isnan(B[6]));
}

}