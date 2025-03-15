#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"

using testing::ContainerEq;

namespace {

template <typename T>
class MaskingTest : public ::testing::Test {
protected:
    virtual void test_matlib_masking(
        int32_t m,int32_t n,int32_t k,int32_t len,
        float fill,
        int32_t mode,
        uint8_t *x,
        float *a
    ) {
        rindow_matlib_s_masking(m,n,k,len,fill,mode,x,a);
    }
    virtual void test_matlib_masking(
        int32_t m,int32_t n,int32_t k,int32_t len,
        double fill,
        int32_t mode,
        uint8_t *x,
        double *a
    ) {
        rindow_matlib_d_masking(m,n,k,len,fill,mode,x,a);
    }
    virtual void test_matlib_masking(
        int32_t m,int32_t n,int32_t k,int32_t len,
        int32_t *fill,
        int32_t mode,
        uint8_t *x,
        int32_t *a
    ) {
        int32_t dtype = rindow_matlib_dtype_int32;
        rindow_matlib_i_masking(dtype,m,n,k,len,fill,mode,x,a);
    }
    virtual void test_matlib_masking(
        int32_t m,int32_t n,int32_t k,int32_t len,
        uint8_t *fill,
        int32_t mode,
        uint8_t *x,
        uint8_t *a
    ) {
        int32_t dtype = rindow_matlib_dtype_bool;
        rindow_matlib_i_masking(dtype,m,n,k,len,fill,mode,x,a);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(MaskingTest, TestTypes);

TYPED_TEST(MaskingTest, simple) {
    const int32_t M = 1;
    const int32_t N = 1;
    const int32_t K = 6;
    const int32_t LEN = 1;
    const int32_t mode = 0;  // set mode

    // rows<cols
    uint8_t X[M][K] = {{true,true,false,false,false,true}};
    TypeParam A[M][N][K] = {{{1,10,100,-1,-10,-100}}};
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (TypeParam)0.0, // fill
        mode,
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {{true,true,false,false,false,true}};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K] = {{{1,10,  0, 0,  0,-100}}};
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, broadcast_inner_mGTn) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;
    const int32_t K = 1;
    const int32_t LEN = 1;
    const int32_t mode = 0;  // set mode

    // m>n
    uint8_t X[M][K] = {{true},{false},{true}};
    TypeParam A[M][N][K] = {
        {{ 11},{-12}},
        {{-21},{ 22}},
        {{ 31},{-32}}
    };
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (TypeParam)0.0, // fill
        mode,
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {{true},{false},{true}};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K] = {
        {{ 11},{-12}},
        {{  0},{  0}},
        {{ 31},{-32}}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, broadcast_inner_mLTn) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t K = 1;
    const int32_t LEN = 1;
    const int32_t mode = 0;  // set mode

    // m<n
    uint8_t X[M][K] = {{true},{false}};
    TypeParam A[M][N][K] = {
        {{ 11},{ 12},{-13}},
        {{-21},{ 22},{ 23}}
    };
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (TypeParam)0,   // fill
        mode,
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {{true},{false}};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K] = {
        {{ 11},{ 12},{-13}},
        {{  0},{  0},{  0}}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, broadcast_outer_nGTk) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 1;
    const int32_t N = 3;
    const int32_t K = 2;
    const int32_t LEN = 1;
    const int32_t mode = 0;  // set mode

    // n<k
    uint8_t X[M][K] = {{true,false}};
    TypeParam A[M][N][K] = {
        {{ 11,-12},
         {-21, 22},
         { 31,-32}}
    };
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (TypeParam)0.0, // fill
        mode,
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {{true,false}};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K] = {
        {{ 11,  0},
         {-21,  0},
         { 31,  0}}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, broadcast_outer_nLTk) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 1;
    const int32_t N = 2;
    const int32_t K = 3;
    const int32_t LEN = 1;
    const int32_t mode = 0;  // set mode

    // n<k
    uint8_t X[M][K] = {{true,false,true}};
    TypeParam A[M][N][K] = {
        {{ 11, 12,-13},
         {-21, 22, 23}}
    };
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (TypeParam)0.0, // fill
        mode,
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {{true,false,true}};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K] = {
        {{ 11,  0,-13},
         {-21,  0, 23}}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, broadcast_both_mGTn) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;
    const int32_t K = 4;
    const int32_t LEN = 1;
    const int32_t mode = 0;  // set mode

    // n<k
    uint8_t X[M][K] = {
        {true,false,false,false},
        {true,true, false,false},
        {true,true, true, false}
    };
    TypeParam A[M][N][K] = {
        {{ 111, 112,-113, 114},{-121, 122, 123, 124}},
        {{ 211, 212,-213, 214},{-221, 222, 223, 224}},
        {{ 311, 312,-313, 314},{-321, 322, 323, 324}}
    };
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (TypeParam)0.0, // fill
        mode,
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {
        {true,false,false,false},
        {true,true, false,false},
        {true,true, true, false}
    };
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K] = {
        {{ 111,   0,   0,   0},{-121,   0,   0,   0}},
        {{ 211, 212,   0,   0},{-221, 222,   0,   0}},
        {{ 311, 312,-313,   0},{-321, 322, 323,   0}}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, broadcast_both_mLTn) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t K = 4;
    const int32_t LEN = 1;
    const int32_t mode = 0;  // set mode

    // n<k
    uint8_t X[M][K] = {
        {true,true, false,false},
        {true,true, true, false}
    };
    TypeParam A[M][N][K] = {
        {{ 111, 112,-113, 114},{-121, 122, 123, 124},{-131, 132, 133, 134}},
        {{ 211, 212,-213, 214},{-221, 222, 223, 224},{-231, 232, 233, 234}}
    };
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (TypeParam)0.0, // fill
        mode,
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {
        {true,true, false,false},
        {true,true, true, false}
    };
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K] = {
        {{ 111, 112,   0,   0},{-121, 122,   0,   0},{-131, 132,   0,   0}},
        {{ 211, 212,-213,   0},{-221, 222, 223,   0},{-231, 232, 233,   0}}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, simple_len) {
    const int32_t M = 1;
    const int32_t N = 1;
    const int32_t K = 6;
    const int32_t LEN = 2;
    const int32_t mode = 0;  // set mode

    // rows<cols
    uint8_t X[M][K] = {{true,true,false,false,false,true}};
    TypeParam A[M][N][K][LEN] = {{{{1,-1},{10,-10},{100,-100},{-1,1},{-10,10},{-100,100}}}};
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (TypeParam)0.0, // fill
        mode,
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {{true,true,false,false,false,true}};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K][LEN] = {{{{1,-1},{10,-10},{0,0},{0,0},{0,0},{-100,100}}}};
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, broadcast_outer_nLTk_len) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 1;
    const int32_t N = 2;
    const int32_t K = 3;
    const int32_t LEN = 2;
    const int32_t mode = 0;  // set mode

    // n<k
    uint8_t X[M][K] = {{true,false,true}};
    TypeParam A[M][N][K][LEN] = {
        {{{11,-11}, {12,-12}, {-13,13}},
         {{-21,21}, {22,-22}, {23,-23}}}
    };
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (TypeParam)0.0, // fill
        mode,
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {{true,false,true}};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K][LEN] = {
        {{{11,-11}, {0,0}, {-13,13}},
         {{-21,21}, {0,0}, {23,-23}}}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, broadcast_both_mGTn_len) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;
    const int32_t K = 4;
    const int32_t LEN = 2;
    const int32_t mode = 0;  // set mode

    // n<k
    uint8_t X[M][K] = {
        {true,false,false,false},
        {true,true, false,false},
        {true,true, true, false}
    };
    TypeParam A[M][N][K][LEN] = {
        {{ {111,-111}, {112,-112},{-113,113}, {114,-114}},{{-121,121}, {122,-122}, {123,-123}, {124,-124}}},
        {{ {211,-211}, {212,-212},{-213,213}, {214,-214}},{{-221,221}, {222,-222}, {223,-223}, {224,-224}}},
        {{ {311,-311}, {312,-312},{-313,313}, {314,-314}},{{-321,321}, {322,-322}, {323,-323}, {324,-324}}}
    };
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (TypeParam)0.0, // fill
        mode,
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {
        {true,false,false,false},
        {true,true, false,false},
        {true,true, true, false}
    };
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K][LEN] = {
        {{ {111,-111}, {  0,   0},{   0,  0},{0,0}},{{-121,121}, {  0,   0}, {  0,   0},{0,0}}},
        {{ {211,-211}, {212,-212},{   0,  0},{0,0}},{{-221,221}, {222,-222}, {  0,   0},{0,0}}},
        {{ {311,-311}, {312,-312},{-313,313},{0,0}},{{-321,321}, {322,-322}, {323,-323},{0,0}}}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, broadcast_both_mLTn_len) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t K = 4;
    const int32_t LEN = 2;
    const int32_t mode = 0;  // set mode

    // n<k
    uint8_t X[M][K] = {
        {true,true, false,false},
        {true,true, true, false}
    };
    TypeParam A[M][N][K][LEN] = {
        {{{111,-111},{112,-112},{-113,113},{114,-114}},{{-121,121},{122,-122},{123,-123},{124,-124}},{{-131,131},{132,-132},{133,-133},{134,-134}}},
        {{{211,-211},{212,-212},{-213,213},{214,-214}},{{-221,221},{222,-222},{223,-223},{224,-224}},{{-231,231},{232,-232},{233,-233},{234,-234}}}
    };
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (TypeParam)0.0, // fill
        mode,
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {
        {true,true, false,false},
        {true,true, true, false}
    };
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K][LEN] = {
        {{ {111,-111},{112,-112},{   0,  0},{0,0}},{{-121,121},{122,-122},{  0,   0},{0,0}},{{-131,131},{132,-132},{  0,   0},{0,0}}},
        {{ {211,-211},{212,-212},{-213,213},{0,0}},{{-221,221},{222,-222},{223,-223},{0,0}},{{-231,231},{232,-232},{233,-233},{0,0}}}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}
TYPED_TEST(MaskingTest, simple_addMode) {
    const int32_t M = 1;
    const int32_t N = 1;
    const int32_t K = 6;
    const int32_t LEN = 1;
    const int32_t mode = 1;  // add mode

    // rows<cols
    uint8_t X[M][K] = {{true,true,false,false,false,true}};
    TypeParam A[M][N][K] = {{{1,10,100,-1,-10,-100}}};
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (TypeParam)-1000.0, // fill
        mode,
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {{true,true,false,false,false,true}};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K] = {{{1,10,-900,-1001,-1010,-100}}};
    EXPECT_THAT(R2, ContainerEq(A));
}
TYPED_TEST(MaskingTest, simple_int32) {
    const int32_t M = 1;
    const int32_t N = 1;
    const int32_t K = 6;
    const int32_t LEN = 1;
    const int32_t mode = 0;  // set mode

    // rows<cols
    uint8_t X[M][K] = {{true,true,false,false,false,true}};
    int32_t A[M][N][K] = {{{1,10,100,-1,-10,-100}}};
    int32_t fill = 0;
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (int32_t *)&fill,// fill
        mode,
        (uint8_t*)X,    // *x,
        (int32_t *)A  // *a,
    );
    uint8_t R1[M][K] = {{true,true,false,false,false,true}};
    EXPECT_THAT(R1, ContainerEq(X));
    int32_t R2[M][N][K] = {{{1,10,0,0,0,-100}}};
    EXPECT_THAT(R2, ContainerEq(A));
}
TYPED_TEST(MaskingTest, simple_int32_add) {
    const int32_t M = 1;
    const int32_t N = 1;
    const int32_t K = 6;
    const int32_t LEN = 1;
    const int32_t mode = 1;  // add mode

    // rows<cols
    uint8_t X[M][K] = {{true,true,false,false,false,true}};
    int32_t A[M][N][K] = {{{1,10,100,-1,-10,-100}}};
    int32_t fill = -1000;
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (int32_t *)&fill,// fill
        mode,
        (uint8_t*)X,    // *x,
        (int32_t *)A  // *a,
    );
    uint8_t R1[M][K] = {{true,true,false,false,false,true}};
    EXPECT_THAT(R1, ContainerEq(X));
    int32_t R2[M][N][K] = {{{1,10,-900,-1001,-1010,-100}}};
    EXPECT_THAT(R2, ContainerEq(A));
}
TYPED_TEST(MaskingTest, simple_bool) {
    const int32_t M = 1;
    const int32_t N = 1;
    const int32_t K = 6;
    const int32_t LEN = 1;
    const int32_t mode = 0;  // set mode

    // rows<cols
    uint8_t X[M][K] =     {{true, true, false,false,false,true }};
    uint8_t A[M][N][K] = {{{true, false,true, false,true, true }}};
    uint8_t fill = false;
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (uint8_t *)&fill,// fill
        mode,
        (uint8_t*)X,    // *x,
        (uint8_t *)A  // *a,
    );
    uint8_t R1[M][K] =       {{true, true, false,false,false,true }};
    EXPECT_THAT(R1, ContainerEq(X));
    uint8_t R2[M][N][K] =   {{{true, false,false,false,false,true }}};
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, simple_bool_add) {
    const int32_t M = 1;
    const int32_t N = 1;
    const int32_t K = 6;
    const int32_t LEN = 1;
    const int32_t mode = 1;  // add mode

    // rows<cols
    uint8_t X[M][K] =     {{true, true, false,false,false,true }};
    uint8_t A[M][N][K] = {{{   1,    0,     1,    0,    1,   1 }}};
    uint8_t fill = true;
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        LEN,            // int32_t len,
        (uint8_t *)&fill,// fill
        mode,
        (uint8_t*)X,    // *x,
        (uint8_t *)A  // *a,
    );
    uint8_t R1[M][K] =       {{true, true, false,false,false,true }};
    EXPECT_THAT(R1, ContainerEq(X));
    uint8_t R2[M][N][K] =   {{{   1,    0,     1,    1,    1,   1 }}};
    EXPECT_THAT(R2, ContainerEq(A));
}

}
