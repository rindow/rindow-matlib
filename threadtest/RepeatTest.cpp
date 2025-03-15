#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <numeric>

using testing::ContainerEq;

namespace {

template <typename T>
class RepeatTest : public ::testing::Test {
protected:
    int32_t calcShapeAxisNull(
        std::initializer_list<int32_t> shape,
        int32_t repeats,
        int32_t *m,int32_t *k, int32_t *outputSize
        )
    {
        int32_t ndim = (int32_t)shape.size();
        if(repeats<0 || ndim<=0) {
            std::cout << "repeats=" << repeats << ",ndim=" << ndim << std::endl;
            throw std::invalid_argument("Invalid repeats.");
        }
        *k = std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int32_t>());
        *m = 1;
        *outputSize = (*k)*(repeats);
        return 0;
    }
    int32_t calcShape(
        std::initializer_list<int32_t> shape,
        int32_t repeats,
        int32_t axis,
        int32_t *m,int32_t *k, int32_t *outputSize
        )
    {
        int32_t ndim = (int32_t)shape.size();
        if(repeats<0 || ndim<=0) {
            std::cout << "axis=" << axis << ",ndim=" << ndim << std::endl;
            throw std::invalid_argument("Invalid repeats.");
        }
        if(axis<0) {
            axis = ndim+axis;
        }
        if(axis>ndim) {
            std::cout << "axis=" << axis << ",ndim=" << ndim << std::endl;
            throw std::invalid_argument("Invalid axis.");
        }
        *m = std::accumulate(shape.begin(), shape.begin()+axis, 1, std::multiplies<int32_t>());
        *k = std::accumulate(shape.begin()+axis, shape.end(), 1, std::multiplies<int32_t>());
        *outputSize = (*m)*repeats*(*k);
        return 0;
    }

    virtual void test_matlib_repeat(
        int32_t m,
        int32_t k,
        int32_t repeats,
        float *a,
        float *b
    )
    {
        return rindow_matlib_s_repeat(m,k,repeats,a,b);
    }

    virtual void test_matlib_repeat(
        int32_t m,
        int32_t k,
        int32_t repeats,
        double *a,
        double *b
    )
    {
        return rindow_matlib_d_repeat(m,k,repeats,a,b);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(RepeatTest, TestTypes);

TYPED_TEST(RepeatTest, Array1DAxis0) {
    const int32_t repeats2 = 2;
    const int32_t repeats1 = 1;
    const int32_t shapeA = 3;
    const int32_t axis = 0;
    int32_t m;
    int32_t k;
    int32_t outputSize;

    TypeParam A[shapeA] = {1,2,3};
    TypeParam B[repeats2][shapeA];
    this->calcShape({shapeA},repeats2,axis,&m,&k,&outputSize);
    
    ASSERT_EQ(1,        m);
    ASSERT_EQ(shapeA,   k);
    ASSERT_EQ(repeats2*shapeA,   outputSize);

    this->test_matlib_repeat(
        m,k,repeats2,
        (TypeParam*)A,
        (TypeParam*)B
    );

    TypeParam R1[shapeA] = {1,2,3};
    EXPECT_THAT(R1, ContainerEq(A));

    TypeParam R2[repeats2][shapeA] = {
        {1,2,3},
        {1,2,3}
    };
    EXPECT_THAT(R2, ContainerEq(B));
}
TYPED_TEST(RepeatTest, Array1DAxis1) {
    const int32_t repeats2 = 2;
    const int32_t shapeA = 3;
    const int32_t axis = 1;
    int32_t m;
    int32_t k;
    int32_t outputSize;

    TypeParam A[shapeA] = {1,2,3};

    // repeats = 2
    TypeParam B2[shapeA][repeats2];
    this->calcShape({shapeA},repeats2,axis,&m,&k,&outputSize);
    
    ASSERT_EQ(shapeA,   m);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(shapeA*repeats2,   outputSize);

    this->test_matlib_repeat(
        m,k,repeats2,
        (TypeParam*)A,
        (TypeParam*)B2
    );

    TypeParam R1[shapeA] = {1,2,3};
    EXPECT_THAT(R1, ContainerEq(A));

    TypeParam R2[shapeA][repeats2] = {
        {1,1},
        {2,2},
        {3,3}
    };
    EXPECT_THAT(R2, ContainerEq(B2));
}
TYPED_TEST(RepeatTest, Array2DAxis1) {
    const int32_t repeats2 = 2;
    const int32_t repeats1 = 1;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 3;
    const int32_t axis = 1;
    int32_t m;
    int32_t k;
    int32_t outputSize;

    TypeParam A[shapeA0][shapeA1] = {
        {1,2,3},
        {4,5,6}
    };

    // repeats = 2
    TypeParam B2[shapeA0][repeats2][shapeA1];
    this->calcShape({shapeA0,shapeA1},repeats2,axis,&m,&k,&outputSize);
    
    ASSERT_EQ(shapeA0,  m);
    ASSERT_EQ(shapeA1,  k);
    ASSERT_EQ(shapeA0*repeats2*shapeA1,   outputSize);

    this->test_matlib_repeat(
        m,k,repeats2,
        (TypeParam*)A,
        (TypeParam*)B2
    );

    TypeParam R1[shapeA0][shapeA1] = {
        {1,2,3},
        {4,5,6}
    };
    EXPECT_THAT(R1, ContainerEq(A));

    TypeParam R2[shapeA0][repeats2][shapeA1] = {
        {{1,2,3},{1,2,3}},
        {{4,5,6},{4,5,6}},
    };
    EXPECT_THAT(R2, ContainerEq(B2));

    // repeats = 1
    TypeParam B1[shapeA0][repeats1][shapeA1];
    this->calcShape({shapeA0,shapeA1},repeats1,axis,&m,&k,&outputSize);
    
    ASSERT_EQ(shapeA0,  m);
    ASSERT_EQ(shapeA1,  k);
    ASSERT_EQ(shapeA0*repeats1*shapeA1,   outputSize);

    this->test_matlib_repeat(
        m,k,repeats1,
        (TypeParam*)A,
        (TypeParam*)B1
    );

    TypeParam R3[shapeA0][shapeA1] = {
        {1,2,3},
        {4,5,6}
    };
    EXPECT_THAT(R3, ContainerEq(A));

    TypeParam R4[shapeA0][repeats1][shapeA1] = {
        {{1,2,3}},
        {{4,5,6}},
    };
    EXPECT_THAT(R4, ContainerEq(B1));
}
TYPED_TEST(RepeatTest, Array2DAxisNull) {
    const int32_t repeats2 = 2;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 3;
    int32_t m;
    int32_t k;
    int32_t outputSize;

    TypeParam A[shapeA0][shapeA1] = {
        {1,2,3},
        {4,5,6}
    };

    // repeats = 2
    TypeParam B2[repeats2*shapeA0*shapeA1];
    this->calcShapeAxisNull({shapeA0,shapeA1},repeats2,&m,&k,&outputSize);
    
    ASSERT_EQ(1,                m);
    ASSERT_EQ(shapeA0*shapeA1,  k);
    ASSERT_EQ(repeats2*shapeA0*shapeA1,   outputSize);

    this->test_matlib_repeat(
        m,k,repeats2,
        (TypeParam*)A,
        (TypeParam*)B2
    );

    TypeParam R1[shapeA0][shapeA1] = {
        {1,2,3},
        {4,5,6}
    };
    EXPECT_THAT(R1, ContainerEq(A));

    TypeParam R2[repeats2*shapeA0*shapeA1] = {
        1,2,3,  4,5,6,
        1,2,3,  4,5,6
    };
    EXPECT_THAT(R2, ContainerEq(B2));

}
}