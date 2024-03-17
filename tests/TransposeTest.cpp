#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"

using testing::ContainerEq;

namespace {

template <typename T>
class TransposeTest : public ::testing::Test {
protected:
    virtual int32_t test_matlib_transpose(
        int32_t ndim, int32_t *shape, int32_t *perm,
        float *a, float *b)
    {
        return rindow_matlib_s_transpose(
            ndim,   // int32_t ndim,
            shape,  // int32_t shape[ndim],
            perm,   // int32_t perm[ndim],
            a,      // float a[N]
            b       // float b[N]
        );
    }

    virtual int32_t test_matlib_transpose(
        int32_t ndim, int32_t *shape, int32_t *perm,
        double *a, double *b)
    {
        return rindow_matlib_d_transpose(
            ndim,   // int32_t ndim,
            shape,  // int32_t shape[ndim],
            perm,   // int32_t perm[ndim],
            a,      // float a[N]
            b       // float b[N]
        );
    }

    virtual int32_t test_matlib_transpose(
        int32_t ndim, int32_t *shape, int32_t *perm,
        int32_t *a, int32_t *b)
    {
        int32_t dtype = rindow_matlib_dtype_int32;
        return rindow_matlib_i_transpose(
            dtype,  // int32_t dtype,
            ndim,   // int32_t ndim,
            shape,  // int32_t shape[ndim],
            perm,   // int32_t perm[ndim],
            a,      // float a[N]
            b       // float b[N]
        );
    }
};
typedef ::testing::Types<float, double, int32_t> TestTypes;
TYPED_TEST_SUITE(TransposeTest, TestTypes);


TYPED_TEST(TransposeTest, Array1DNormal) {
    const int32_t N = 3;

    TypeParam a[N] = {1, 2, 3};
    TypeParam b[N] = {0, 0, 0};

    const int32_t ndim = 1;
    int32_t shape[ndim] = { N };
    int32_t perm[ndim]  = { 0 };
    int32_t rc;

    rc = this->test_matlib_transpose(
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],
        perm,   // int32_t perm[ndim],
        a,      // a[N]
        b       // b[N]
    );
    ASSERT_EQ(0, rc);
    TypeParam R1[N] = {1,2,3};
    EXPECT_THAT(R1, ContainerEq(a));
    TypeParam R2[N] = {1,2,3};
    EXPECT_THAT(R2, ContainerEq(b));
}

TYPED_TEST(TransposeTest, Array2DNormal) {
    const int32_t M = 2;
    const int32_t N = 3;
    // float
    TypeParam a[M*N] = {
        1, 2, 3,
        4, 5, 6
    };
    TypeParam b[M*N] = {
        0, 0, 0,
        0, 0, 0
    };

    const int32_t ndim = 2;
    int32_t shape[ndim] = { M, N };
    int32_t perm[ndim]  = { 1, 0 };
    int32_t rc;

    rc = this->test_matlib_transpose(
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],}
        perm,   // int32_t perm[ndim],
        a,      // a[N]
        b       // b[N]
    );
    ASSERT_EQ(0, rc);
    TypeParam R1[M*N] = {
        1, 2, 3,
        4, 5, 6
    };
    EXPECT_THAT(R1, ContainerEq(a));
    TypeParam R2[N*M] = {
        1, 4,  
        2, 5,
        3, 6
    };
    EXPECT_THAT(R2, ContainerEq(b));
}

TYPED_TEST(TransposeTest, Array3DNormal) {
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t K = 4;
    const int32_t SIZE = M*N*K;

    TypeParam a[SIZE];
    for(int i=0; i<SIZE; i++) { a[i] = (TypeParam)i; }
    TypeParam b[SIZE];
    for(int i=0; i<SIZE; i++) { b[i] = 0; }

    const int32_t ndim = 3;
    int32_t shape[ndim] = { M, N, K };
    int32_t perm[ndim]  = { 2, 1, 0 };
    int32_t rc;

    rc = this->test_matlib_transpose(
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],}
        perm,   // int32_t perm[ndim],
        a,      // float a[N]
        b       // float b[N]
    );
    ASSERT_EQ(0, rc);
    TypeParam R1[SIZE];
    for(int i=0; i<SIZE; i++) { R1[i] = (TypeParam)i; }
    EXPECT_THAT(R1, ContainerEq(a));
    TypeParam R2[SIZE];
    for(int h=0;h<K;h++) {
        for(int i=0;i<N;i++) {
            for(int j=0;j<M;j++) {
                R2[h*N*M+i*M+j] = (TypeParam)(j*N*K+i*K+h);
            }
        }
    }
    EXPECT_THAT(R2, ContainerEq(b));
}

TYPED_TEST(TransposeTest, Array3D2Normal) {
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t K = 4;
    const int32_t SIZE = M*N*K;

    TypeParam a[SIZE];
    for(int i=0; i<SIZE; i++) { a[i] = (TypeParam)i; }
    TypeParam b[SIZE];
    for(int i=0; i<SIZE; i++) { b[i] = 0; }

    const int32_t ndim = 3;
    int32_t shape[ndim] = { M, N, K };
    int32_t perm[ndim]  = { 0, 2, 1 };
    int32_t rc;

    rc = this->test_matlib_transpose(
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],}
        perm,   // int32_t perm[ndim],
        a,      // a[N]
        b       // b[N]
    );
    ASSERT_EQ(0, rc);
    TypeParam R1[SIZE];
    for(int i=0; i<SIZE; i++) { R1[i] = (TypeParam)i; }
    EXPECT_THAT(R1, ContainerEq(a));
    TypeParam R2[SIZE];
    for(int h=0;h<M;h++) {
        for(int i=0;i<K;i++) {
            for(int j=0;j<N;j++) {
                R2[h*K*N+i*N+j] = (TypeParam)(h*K*N+j*K+i);
            }
        }
    }
    EXPECT_THAT(R2, ContainerEq(b));
}

}