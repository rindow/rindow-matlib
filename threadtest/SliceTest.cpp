#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdbool.h>
#include <numeric>

using testing::ContainerEq;

namespace {

template <typename T>
class AbstractSliceTest : public ::testing::Test {
protected:
    void calcGatherShapes(
        std::initializer_list<int32_t> shape,
        std::initializer_list<int32_t> begin,
        std::initializer_list<int32_t> size,
        int32_t *m,int32_t *n,int32_t *k,
        int32_t *itemSize,
        int32_t *startAxis0,int32_t *sizeAxis0,
        int32_t *startAxis1,int32_t *sizeAxis1,
        int32_t *startAxis2,int32_t *sizeAxis2,
        int32_t *outputSize
        )
    {
        int32_t ndimBegin = (int32_t)(begin.size());
        if(ndimBegin<1||ndimBegin>3) {
            throw std::invalid_argument("begin must has 1 or 2 or 3 integer.");
        }
        int32_t ndimSize = (int32_t)(size.size());
        if(ndimSize<1||ndimSize>3) {
            throw std::invalid_argument("Size must has 1 or 2 or 3 integer.");
        }
        if(ndimBegin!=ndimSize){
            throw std::invalid_argument("Unmatch shape of begin and size.");
        }
        int32_t ndimInput = (int32_t)(shape.size());
        if(ndimInput<ndimBegin){
            throw std::invalid_argument("shape rank is low to slice.");
        }

        // ndim = 0
        *m = *(shape.begin());
        *startAxis0 = *(begin.begin());
        if(*startAxis0<0){
            *startAxis0 = *m+*startAxis0;
        }
        if(*startAxis0<0||*startAxis0>=*m){
            throw std::invalid_argument("start of axis 0 is invalid value.");
        }
        *sizeAxis0 = *(size.begin());
        if(*sizeAxis0<0){
            *sizeAxis0 = *m-*startAxis0+*sizeAxis0+1;
        }
        if(*sizeAxis0<1||*startAxis0+*sizeAxis0>*m){
            throw std::invalid_argument("size of axis 0 is invalid value.");
        }

        // ndim = 1
        if(ndimBegin<=1){
            *n = 1;
            *startAxis1 = 0;
            *sizeAxis1 = 1;
        } else {
            *n = *(shape.begin()+1);
            *startAxis1 = *(begin.begin()+1);
            if(*startAxis1<0){
                *startAxis1 = *n+*startAxis1;
            }
            if(*startAxis1<0||*startAxis1>=*n){
                throw std::invalid_argument("start of axis 1 is invalid value.:begin.");
            }
            *sizeAxis1 = *(size.begin()+1);
            if(*sizeAxis1<0){
                *sizeAxis1 = *n-*startAxis1+*sizeAxis1+1;
            }
            if(*sizeAxis1<1||*startAxis1+*sizeAxis1>*n){
                throw std::invalid_argument("size of axis 1 is invalid value.");
            }
        }

        // ndim = 2
        if(ndimBegin<=2){
            *k = 1;
            *startAxis2 = 0;
            *sizeAxis2 = 1;
        } else {
            *k = *(shape.begin()+2);
            *startAxis2 = *(begin.begin()+2);
            if(*startAxis2<0){
                *startAxis2 = *k+*startAxis2;
            }
            if(*startAxis2<0||*startAxis2>=*k){
                throw std::invalid_argument("start of axis 2 is invalid value.:begin");
            }
            *sizeAxis2 = *(size.begin()+2);
            if(*sizeAxis2<0){
                *sizeAxis2 = *k-*startAxis2+*sizeAxis2+1;
            }
            if(*sizeAxis2<1||*startAxis2+*sizeAxis2>*k){
                throw std::invalid_argument("size of axis 2 is invalid value.");
            }
        }
        *itemSize = std::accumulate(shape.begin()+ndimBegin, shape.end(), 1, std::multiplies<int32_t>());
        *outputSize = *sizeAxis0;
        if(ndimBegin>=2){
            *outputSize *= *sizeAxis1;
        }
        if(ndimBegin>=3){
            *outputSize *= *sizeAxis2;
        }
        *outputSize *= *itemSize;
    }   
    virtual void test_matlib_slice(
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t size,
        float *a, int32_t incA,
        float *y, int32_t incY,
        int32_t startAxis0,
        int32_t sizeAxis0,
        int32_t startAxis1,
        int32_t sizeAxis1,
        int32_t startAxis2,
        int32_t sizeAxis2
        )
    {
        return rindow_matlib_s_slice(
            reverse,addMode,
            m,n,k,size,
            a, incA,
            y, incY,
            startAxis0,sizeAxis0,
            startAxis1,sizeAxis1,
            startAxis2,sizeAxis2
        );
    }

    virtual void test_matlib_slice(
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t size,
        double *a, int32_t incA,
        double *y, int32_t incY,
        int32_t startAxis0,
        int32_t sizeAxis0,
        int32_t startAxis1,
        int32_t sizeAxis1,
        int32_t startAxis2,
        int32_t sizeAxis2
        )
    {
        return rindow_matlib_d_slice(
            reverse,addMode,
            m,n,k,size,
            a, incA,
            y, incY,
            startAxis0,sizeAxis0,
            startAxis1,sizeAxis1,
            startAxis2,sizeAxis2
        );
    }

    virtual void test_matlib_slice(
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t size,
        int32_t *a, int32_t incA,
        int32_t *y, int32_t incY,
        int32_t startAxis0,
        int32_t sizeAxis0,
        int32_t startAxis1,
        int32_t sizeAxis1,
        int32_t startAxis2,
        int32_t sizeAxis2
        )
    {
        int32_t data_dtype = rindow_matlib_dtype_int32;
        return rindow_matlib_i_slice(
            reverse,addMode,
            m,n,k,size,
            data_dtype,
            a, incA,
            y, incY,
            startAxis0,sizeAxis0,
            startAxis1,sizeAxis1,
            startAxis2,sizeAxis2
        );
    }

    virtual void test_matlib_slice(
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t size,
        uint8_t *a, int32_t incA,
        uint8_t *y, int32_t incY,
        int32_t startAxis0,
        int32_t sizeAxis0,
        int32_t startAxis1,
        int32_t sizeAxis1,
        int32_t startAxis2,
        int32_t sizeAxis2
        )
    {
        int32_t data_dtype = rindow_matlib_dtype_bool;
        return rindow_matlib_i_slice(
            reverse,addMode,
            m,n,k,size,
            data_dtype,
            a, incA,
            y, incY,
            startAxis0,sizeAxis0,
            startAxis1,sizeAxis1,
            startAxis2,sizeAxis2
        );
    }

    template <typename TY>
    void test_exec(
        bool reverse,
        TY* input,
        std::initializer_list<int32_t> shape,
        std::initializer_list<int32_t> begin,
        std::initializer_list<int32_t> size,
        TY* output,
        int32_t outBuffSize
    )
    {
        const int32_t incA = 1;
        const int32_t incY = 1;

        int32_t m;
        int32_t n;
        int32_t k;
        int32_t itemSize;
        int32_t startAxis0;
        int32_t sizeAxis0;
        int32_t startAxis1;
        int32_t sizeAxis1;
        int32_t startAxis2;
        int32_t sizeAxis2;
        int32_t outputSize;

        this->calcGatherShapes(
            shape,
            begin,
            size,
            &m,&n,&k,
            &itemSize,
            &startAxis0,&sizeAxis0,
            &startAxis1,&sizeAxis1,
            &startAxis2,&sizeAxis2,
            &outputSize
        );
        std::cout << "itemSize=" << itemSize << std::endl;
        std::cout << "startAxis0=" << startAxis0 << ",sizeAxis0=" << sizeAxis0 << std::endl;
        std::cout << "startAxis1=" << startAxis1 << ",sizeAxis1=" << sizeAxis1 << std::endl;
        std::cout << "startAxis2=" << startAxis1 << ",sizeAxis2=" << sizeAxis2 << std::endl;
        std::cout << "outputSize=" << outputSize << std::endl;
        std::cout << "outBuffSize=" << outBuffSize << std::endl;
        ASSERT_EQ(outBuffSize,outputSize);

        this->test_matlib_slice(
            (int32_t)reverse,
            (int32_t)false, // addMode,
            m,
            n,
            k,
            itemSize,
            input, incA,
            output, incY,
            startAxis0,
            sizeAxis0,
            startAxis1,
            sizeAxis1,
            startAxis2,
            sizeAxis2
        );
    }
};

template <typename T>
class SliceTest : public AbstractSliceTest<T> {};

typedef ::testing::Types<float, double, int32_t> TestTypes;
TYPED_TEST_SUITE(SliceTest, TestTypes);

TYPED_TEST(SliceTest, slice3d_axis1_1to2) {
    const int32_t reverse = false;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 4;
    const int32_t shapeA2 = 3;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{ 0, 1, 2},
         { 3, 4, 5},
         { 6, 7, 8},
         { 9,10,11}},

        {{12,13,14},
         {15,16,17},
         {18,19,20},
         {21,22,23}},
    };

    //         slice axis1
    //             v
    TypeParam B[2][2][3];

    this->test_exec(
        reverse,
        (TypeParam*)A,
        {shapeA0,shapeA1,shapeA2}, // shape
        { 0, 1},    // begin
        {-1, 2},    // size
        (TypeParam*)B,
        (int32_t)(sizeof(B)/sizeof(TypeParam)) // outputSize
    );

    TypeParam R1[2][2][3] = {
        {{ 3, 4, 5},
         { 6, 7, 8}},
        {{15,16,17},
         {18,19,20}},
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(SliceTest, slice3d_axis1_1to1) {
    const int32_t reverse = false;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 4;
    const int32_t shapeA2 = 3;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{ 0, 1, 2},
         { 3, 4, 5},
         { 6, 7, 8},
         { 9,10,11}},

        {{12,13,14},
         {15,16,17},
         {18,19,20},
         {21,22,23}},
    };

    //         slice axis1
    //             v
    TypeParam B[2][1][3];

    this->test_exec(
        reverse,
        (TypeParam*)A,
        {shapeA0,shapeA1,shapeA2}, // shape
        { 0, 1},    // begin
        {-1, 1},    // size
        (TypeParam*)B,
        (int32_t)(sizeof(B)/sizeof(TypeParam)) // outputSize
    );

    TypeParam R1[2][1][3] = {
        {{ 3, 4, 5}},
        {{15,16,17}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(SliceTest, slice3d_axis1_last1) {
    const int32_t reverse = false;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 4;
    const int32_t shapeA2 = 3;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{ 0, 1, 2},
         { 3, 4, 5},
         { 6, 7, 8},
         { 9,10,11}},

        {{12,13,14},
         {15,16,17},
         {18,19,20},
         {21,22,23}},
    };

    //         slice axis1 last1
    //             v
    TypeParam B[2][1][3];

    this->test_exec(
        reverse,
        (TypeParam*)A,
        {shapeA0,shapeA1,shapeA2}, // shape
        { 0,-1},    // begin
        {-1, 1},    // size
        (TypeParam*)B,
        (int32_t)(sizeof(B)/sizeof(TypeParam)) // outputSize
    );

    TypeParam R1[2][1][3] = {
        {{ 9,10,11}},
        {{21,22,23}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(SliceTest, slice3d_axis0_1to1) {
    const int32_t reverse = false;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 4;
    const int32_t shapeA2 = 3;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{ 0, 1, 2},
         { 3, 4, 5},
         { 6, 7, 8},
         { 9,10,11}},

        {{12,13,14},
         {15,16,17},
         {18,19,20},
         {21,22,23}},
    };

    //   slice axis0
    //          v
    TypeParam B[1][4][3];

    this->test_exec(
        reverse,
        (TypeParam*)A,
        {shapeA0,shapeA1,shapeA2}, // shape
        { 1 },    // begin
        { 1 },    // size
        (TypeParam*)B,
        (int32_t)(sizeof(B)/sizeof(TypeParam)) // outputSize
    );

    TypeParam R1[1][4][3] = {
        {{12,13,14},
         {15,16,17},
         {18,19,20},
         {21,22,23}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(SliceTest, slice3d_axis2_1to1) {
    const int32_t reverse = false;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 4;
    const int32_t shapeA2 = 3;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{ 0, 1, 2},
         { 3, 4, 5},
         { 6, 7, 8},
         { 9,10,11}},

        {{12,13,14},
         {15,16,17},
         {18,19,20},
         {21,22,23}},
    };

    //         slice axis2
    //                v
    TypeParam B[2][4][1];

    this->test_exec(
        reverse,
        (TypeParam*)A,
        {shapeA0,shapeA1,shapeA2}, // shape
        { 0, 0, 1},    // begin
        {-1,-1, 1},    // size
        (TypeParam*)B,
        (int32_t)(sizeof(B)/sizeof(TypeParam)) // outputSize
    );

    TypeParam R1[2][4][1] = {
        {{ 1},
         { 4},
         { 7},
         {10}},

        {{13},
         {16},
         {19},
         {22}},
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(SliceTest, slice2d_axis1_1to2) {
    const int32_t reverse = false;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 4;

    TypeParam A[shapeA0][shapeA1] = {
        {0,1,2,3},
        {4,5,6,7}
    };

    //      slice axis2
    //             v
    TypeParam B[2][2];

    this->test_exec(
        reverse,
        (TypeParam*)A,
        {shapeA0,shapeA1}, // shape
        { 0, 1},    // begin
        {-1, 2},    // size
        (TypeParam*)B,
        (int32_t)(sizeof(B)/sizeof(TypeParam)) // outputSize
    );

    TypeParam R1[2][2] = {
        {1,2},
        {5,6}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(SliceTest, slice2d_all) {
    const int32_t reverse = false;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 4;

    TypeParam A[shapeA0][shapeA1] = {
        {0,1,2,3},
        {4,5,6,7}
    };

    //          slice 
    //          v  v
    TypeParam B[2][4];

    this->test_exec(
        reverse,
        (TypeParam*)A,
        {shapeA0,shapeA1}, // shape
        { 0, 0},    // begin
        { 2, 4},    // size
        (TypeParam*)B,
        (int32_t)(sizeof(B)/sizeof(TypeParam)) // outputSize
    );

    TypeParam R1[2][4] = {
        {0,1,2,3},
        {4,5,6,7}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(SliceTest, slice4d_axis012_1tolast) {
    const int32_t reverse = false;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 2;
    const int32_t shapeA3 = 3;

    TypeParam A[shapeA0][shapeA1][shapeA2][shapeA3] = {
        {{{ 0, 1, 2},
          { 3, 4, 5}},
         {{ 6, 7, 8},
          { 9,10,11}}},
        {{{12,13,14},
          {15,16,17}},
         {{18,19,20},
          {21,22,23}}},
    };

    //slice axis0  1  2
    //          v  v  v
    TypeParam B[1][1][1][3];

    this->test_exec(
        reverse,
        (TypeParam*)A,
        {shapeA0,shapeA1,shapeA2,shapeA3}, // shape
        { 1, 1, 1},    // begin
        {-1,-1,-1},    // size
        (TypeParam*)B,
        (int32_t)(sizeof(B)/sizeof(TypeParam)) // outputSize
    );

    TypeParam R1[1][1][1][3] = {
        {{{21,22,23}}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(SliceTest, slice4d_axis01_1tolast) {
    const int32_t reverse = false;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 2;
    const int32_t shapeA3 = 3;

    TypeParam A[shapeA0][shapeA1][shapeA2][shapeA3] = {
        {{{ 0, 1, 2},
          { 3, 4, 5}},
         {{ 6, 7, 8},
          { 9,10,11}}},
        {{{12,13,14},
          {15,16,17}},
         {{18,19,20},
          {21,22,23}}},
    };

    //slice axis0  1
    //          v  v
    TypeParam B[1][1][2][3];

    this->test_exec(
        reverse,
        (TypeParam*)A,
        {shapeA0,shapeA1,shapeA2,shapeA3}, // shape
        { 1, 1},    // begin
        {-1,-1},    // size
        (TypeParam*)B,
        (int32_t)(sizeof(B)/sizeof(TypeParam)) // outputSize
    );

    TypeParam R1[1][1][2][3] = {
        {{{18,19,20},
          {21,22,23}}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(SliceTest, slice4d_axis0_1tolast) {
    const int32_t reverse = false;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 2;
    const int32_t shapeA3 = 3;

    TypeParam A[shapeA0][shapeA1][shapeA2][shapeA3] = {
        {{{ 0, 1, 2},
          { 3, 4, 5}},
         {{ 6, 7, 8},
          { 9,10,11}}},
        {{{12,13,14},
          {15,16,17}},
         {{18,19,20},
          {21,22,23}}},
    };

    //  slice axis0
    //          v
    TypeParam B[1][2][2][3];

    this->test_exec(
        reverse,
        (TypeParam*)A,
        {shapeA0,shapeA1,shapeA2,shapeA3}, // shape
        { 1},    // begin
        {-1},    // size
        (TypeParam*)B,
        (int32_t)(sizeof(B)/sizeof(TypeParam)) // outputSize
    );

    TypeParam R1[1][2][2][3] = {
        {{{12,13,14},
          {15,16,17}},
         {{18,19,20},
          {21,22,23}}},
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(SliceTest, stick3d_axis1_1to2) {
    const int32_t reverse = true;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 4;
    const int32_t shapeA2 = 3;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{ 0, 0, 0},
         { 0, 0, 0},
         { 0, 0, 0},
         { 0, 0, 0}},
        {{ 0, 0, 0},
         { 0, 0, 0},
         { 0, 0, 0},
         { 0, 0, 0}}
    };

    //     slice axis1
    //             v
    TypeParam B[2][2][3] = {
        {{ 0, 1, 2},
         { 3, 4, 5}},
        {{ 6, 7, 8},
         { 9,10,11}}
    };

    this->test_exec(
        reverse,
        (TypeParam*)A,
        {shapeA0,shapeA1,shapeA2}, // shape
        { 0, 1},    // begin
        {-1, 2},    // size
        (TypeParam*)B,
        (int32_t)(sizeof(B)/sizeof(TypeParam)) // outputSize
    );

    TypeParam R1[2][4][3] = {
        {{ 0, 0, 0},
         { 0, 1, 2},
         { 3, 4, 5},
         { 0, 0, 0}},
        {{ 0, 0, 0},
         { 6, 7, 8},
         { 9,10,11},
         { 0, 0, 0}}
    };
    EXPECT_THAT(R1, ContainerEq(A));
}
TYPED_TEST(SliceTest, stick3d_axis1_1to1) {
    const int32_t reverse = true;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 4;
    const int32_t shapeA2 = 3;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{ 0, 0, 0},
         { 0, 0, 0},
         { 0, 0, 0},
         { 0, 0, 0}},
        {{ 0, 0, 0},
         { 0, 0, 0},
         { 0, 0, 0},
         { 0, 0, 0}}
    };

    //     slice axis1
    //             v
    TypeParam B[2][1][3] = {
        {{ 0, 1, 2}},
        {{ 3, 4, 5}}
    };

    this->test_exec(
        reverse,
        (TypeParam*)A,
        {shapeA0,shapeA1,shapeA2}, // shape
        { 0, 1},    // begin
        {-1, 1},    // size
        (TypeParam*)B,
        (int32_t)(sizeof(B)/sizeof(TypeParam)) // outputSize
    );

    TypeParam R1[2][4][3] = {
        {{ 0, 0, 0},
         { 0, 1, 2},
         { 0, 0, 0},
         { 0, 0, 0}},
        {{ 0, 0, 0},
         { 3, 4, 5},
         { 0, 0, 0},
         { 0, 0, 0}}
    };
    EXPECT_THAT(R1, ContainerEq(A));
}
TYPED_TEST(SliceTest, stick3d_axis0_1to1) {
    const int32_t reverse = true;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 4;
    const int32_t shapeA2 = 3;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{ 0, 0, 0},
         { 0, 0, 0},
         { 0, 0, 0},
         { 0, 0, 0}},
        {{ 0, 0, 0},
         { 0, 0, 0},
         { 0, 0, 0},
         { 0, 0, 0}}
    };

    //   slice axis0
    //          v
    TypeParam B[1][4][3] = {
        {{ 0, 1, 2},
         { 3, 4, 5},
         { 6, 7, 8},
         { 9,10,11}}
    };

    this->test_exec(
        reverse,
        (TypeParam*)A,
        {shapeA0,shapeA1,shapeA2}, // shape
        { 1},    // begin
        { 1},    // size
        (TypeParam*)B,
        (int32_t)(sizeof(B)/sizeof(TypeParam)) // outputSize
    );

    TypeParam R1[2][4][3] = {
        {{ 0, 0, 0},
         { 0, 0, 0},
         { 0, 0, 0},
         { 0, 0, 0}},
        {{ 0, 1, 2},
         { 3, 4, 5},
         { 6, 7, 8},
         { 9,10,11}}
    };
    EXPECT_THAT(R1, ContainerEq(A));
}
TYPED_TEST(SliceTest, stick2d_axis1_1to2) {
    const int32_t reverse = true;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 4;

    TypeParam A[shapeA0][shapeA1] = {
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
    };

    //     slice axis1
    //             v
    TypeParam B[2][2] = {
        { 0, 1},
        { 2, 3}
    };

    this->test_exec(
        reverse,
        (TypeParam*)A,
        {shapeA0,shapeA1}, // shape
        { 0, 1},    // begin
        {-1, 2},    // size
        (TypeParam*)B,
        (int32_t)(sizeof(B)/sizeof(TypeParam)) // outputSize
    );

    TypeParam R1[2][4] = {
        { 0, 0, 1, 0},
        { 0, 2, 3, 0},
    };
    EXPECT_THAT(R1, ContainerEq(A));
}
TYPED_TEST(SliceTest, stick4d_axis2_1to2) {
    const int32_t reverse = true;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 4;
    const int32_t shapeA3 = 3;

    TypeParam A[shapeA0][shapeA1][shapeA2][shapeA3] = {
        {{{ 0, 0, 0},
          { 0, 0, 0},
          { 0, 0, 0},
          { 0, 0, 0}},
         {{ 0, 0, 0},
          { 0, 0, 0},
          { 0, 0, 0},
          { 0, 0, 0}}},
        {{{ 0, 0, 0},
          { 0, 0, 0},
          { 0, 0, 0},
          { 0, 0, 0}},
         {{ 0, 0, 0},
          { 0, 0, 0},
          { 0, 0, 0},
          { 0, 0, 0}}}
    };

    //        slice axis2
    //                v
    TypeParam B[2][2][2][3] = {
        {{{ 0, 1, 2},
          { 3, 4, 5}},
         {{ 6, 7, 8},
          { 9,10,11}}},
        {{{12,13,14},
          {15,16,17}},
         {{18,19,20},
          {21,22,23}}}
    };

    this->test_exec(
        reverse,
        (TypeParam*)A,
        {shapeA0,shapeA1,shapeA2,shapeA3}, // shape
        { 0, 0, 1},    // begin
        {-1,-1, 2},    // size
        (TypeParam*)B,
        (int32_t)(sizeof(B)/sizeof(TypeParam)) // outputSize
    );

    TypeParam R1[2][2][4][3] = {
        {{{ 0, 0, 0},
          { 0, 1, 2},
          { 3, 4, 5},
          { 0, 0, 0}},
         {{ 0, 0, 0},
          { 6, 7, 8},
          { 9,10,11},
          { 0, 0, 0}}},
        {{{ 0, 0, 0},
          {12,13,14},
          {15,16,17},
          { 0, 0, 0}},
         {{ 0, 0, 0},
          {18,19,20},
          {21,22,23},
          { 0, 0, 0}}}
    };
    EXPECT_THAT(R1, ContainerEq(A));
}

TYPED_TEST(SliceTest, bool_slice3d_axis1_1to2) {
    const int32_t reverse = false;
    const int32_t shapeA0 = 2;
    const int32_t shapeA1 = 4;
    const int32_t shapeA2 = 3;

    uint8_t A[shapeA0][shapeA1][shapeA2] = {
        {{ 1, 0, 0},
         { 1, 1, 0},
         { 1, 1, 1},
         { 0, 1, 1}},

        {{ 0, 0, 1},
         { 0, 1, 1},
         { 1, 1, 1},
         { 1, 1, 0}},
    };

    //         slice axis1
    //             v
    uint8_t B[2][2][3];

    this->test_exec(
        reverse,
        (uint8_t*)A,
        {shapeA0,shapeA1,shapeA2}, // shape
        { 0, 1},    // begin
        {-1, 2},    // size
        (uint8_t*)B,
        (int32_t)(sizeof(B)/sizeof(uint8_t)) // outputSize
    );

    uint8_t R1[2][2][3] = {
        {{ 1, 1, 0},
         { 1, 1, 1}},
        {{ 0, 1, 1},
         { 1, 1, 1}},
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

}
