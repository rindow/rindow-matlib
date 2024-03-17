#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdint.h>

#include "rindow/matlib.h"
#include <stdbool.h>

using testing::ContainerEq;

namespace {
TEST(AstypeTest, AstypeNormal) {
    const int N = 5;
    int32_t i32X[N] = {-1,0,1,2,3};
    int32_t i32R1[N] = {-1,0,1,2,3};

    // int32 -> float32
    float sf32Y[N] = {1,2,3,4,5};
    rindow_matlib_astype(
        N,                          // int32_t n,
        rindow_matlib_dtype_int32,  // int32_t from_dtype,
        &i32X,                      // void *x,
        1,                          // int32_t incX,
        rindow_matlib_dtype_float32,// int32_t to_dtype,
        &sf32Y,                     // void *y,
        1                           // int32_t incY,
    );
    EXPECT_THAT(i32R1, ContainerEq(i32X));
    float sRf32[N] = {-1,0,1,2,3};
    EXPECT_THAT(sRf32, ContainerEq(sf32Y));

    // int32 -> float64
    double sf64Y[N] = {1,2,3,4,5};
    rindow_matlib_astype(
        N,                          // int32_t n,
        rindow_matlib_dtype_int32,  // int32_t from_dtype,
        &i32X,                      // void *x,
        1,                          // int32_t incX,
        rindow_matlib_dtype_float64,// int32_t to_dtype,
        &sf64Y,                        // void *y,
        1                           // int32_t incY,
    );
    EXPECT_THAT(i32R1, ContainerEq(i32X));
    double sRf64[N] = {-1,0,1,2,3};
    EXPECT_THAT(sRf64, ContainerEq(sf64Y));

    // int32 -> int8
    int8_t i8Y[N] = {1,2,3,4,5};
    rindow_matlib_astype(
        N,                          // int32_t n,
        rindow_matlib_dtype_int32,  // int32_t from_dtype,
        &i32X,                      // void *x,
        1,                          // int32_t incX,
        rindow_matlib_dtype_int8,// int32_t to_dtype,
        &i8Y,                        // void *y,
        1                           // int32_t incY,
    );
    EXPECT_THAT(i32R1, ContainerEq(i32X));
    int8_t i8R[N] = {-1,0,1,2,3};
    EXPECT_THAT(i8R, ContainerEq(i8Y));

    // int32 -> int16
    int16_t i16Y[N] = {1,2,3,4,5};
    rindow_matlib_astype(
        N,                          // int32_t n,
        rindow_matlib_dtype_int32,  // int32_t from_dtype,
        &i32X,                      // void *x,
        1,                          // int32_t incX,
        rindow_matlib_dtype_int16,// int32_t to_dtype,
        &i16Y,                        // void *y,
        1                           // int32_t incY,
    );
    EXPECT_THAT(i32R1, ContainerEq(i32X));
    int16_t i16R[N] = {-1,0,1,2,3};
    EXPECT_THAT(i16R, ContainerEq(i16Y));

    // int32 -> int32
    int32_t i32Y[N] = {1,2,3,4,5};
    rindow_matlib_astype(
        N,                          // int32_t n,
        rindow_matlib_dtype_int32,  // int32_t from_dtype,
        &i32X,                      // void *x,
        1,                          // int32_t incX,
        rindow_matlib_dtype_int32,// int32_t to_dtype,
        &i32Y,                        // void *y,
        1                           // int32_t incY,
    );
    EXPECT_THAT(i32R1, ContainerEq(i32X));
    int32_t i32R[N] = {-1,0,1,2,3};
    EXPECT_THAT(i32R, ContainerEq(i32Y));

    // int32 -> int64
    int64_t i64Y[N] = {1,2,3,4,5};
    rindow_matlib_astype(
        N,                          // int32_t n,
        rindow_matlib_dtype_int32,  // int32_t from_dtype,
        &i32X,                      // void *x,
        1,                          // int32_t incX,
        rindow_matlib_dtype_int64,// int32_t to_dtype,
        &i64Y,                        // void *y,
        1                           // int32_t incY,
    );
    EXPECT_THAT(i32R1, ContainerEq(i32X));
    int64_t i64R[N] = {-1,0,1,2,3};
    EXPECT_THAT(i64R, ContainerEq(i64Y));

    // int8_t -> int64_t
    int8_t i8X[N] = {-1,0,1,2,3};
    int64_t i8i64Y[N] = {1,2,3,4,5};
    rindow_matlib_astype(
        N,                          // int32_t n,
        rindow_matlib_dtype_int8,   // int32_t from_dtype,
        &i8X,                       // void *x,
        1,                          // int32_t incX,
        rindow_matlib_dtype_int64,  // int32_t to_dtype,
        &i8i64Y,                      // void *y,
        1                           // int32_t incY,
    );
    int8_t i8R1[N] = {-1,0,1,2,3};
    EXPECT_THAT(i8R1, ContainerEq(i8X));
    int64_t i64R2[N] = {-1,0,1,2,3};
    EXPECT_THAT(i64R2, ContainerEq(i8i64Y));

    // int8_t -> uint64_t
    uint64_t u64Y[N] = {1,2,3,4,5};
    rindow_matlib_astype(
        N,                          // int32_t n,
        rindow_matlib_dtype_int8,   // int32_t from_dtype,
        &i8X,                       // void *x,
        1,                          // int32_t incX,
        rindow_matlib_dtype_uint64, // int32_t to_dtype,
        &u64Y,                      // void *y,
        1                           // int32_t incY,
    );
    EXPECT_THAT(i8R1, ContainerEq(i8X));
    uint64_t u64R2[N] = {18446744073709551615UL,0,1,2,3};
    EXPECT_THAT(u64R2, ContainerEq(u64Y));

    // uint8_t -> int64_t
    uint8_t u8X[N] = {255,0,1,2,3};
    rindow_matlib_astype(
        N,                          // int32_t n,
        rindow_matlib_dtype_uint8,  // int32_t from_dtype,
        &u8X,                       // void *x,
        1,                          // int32_t incX,
        rindow_matlib_dtype_int64,  // int32_t to_dtype,
        &i64Y,                      // void *y,
        1                           // int32_t incY,
    );
    uint8_t u8R1[N] = {255,0,1,2,3};
    EXPECT_THAT(u8R1, ContainerEq(u8X));
    int64_t i64R3[N] = {255,0,1,2,3};
    EXPECT_THAT(i64R3, ContainerEq(i64Y));
}

}
