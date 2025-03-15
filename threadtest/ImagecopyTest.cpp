#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"
#include <stdbool.h>

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class ImagecopyTest : public ::testing::Test {
protected:
    virtual void test_matlib_imagecopy(
        int32_t height,
        int32_t width,
        int32_t channels,
        float *a,
        float *b,
        int32_t channelsFirst,
        int32_t heightShift,
        int32_t widthShift,
        int32_t verticalFlip,
        int32_t horizontalFlip,
        int32_t rgbFlip
    ) {
        rindow_matlib_s_imagecopy(
            height,
            width,
            channels,
            a,
            b,
            channelsFirst,
            heightShift,
            widthShift,
            verticalFlip,
            horizontalFlip,
            rgbFlip
        );
    }
    virtual void test_matlib_imagecopy(
        int32_t height,
        int32_t width,
        int32_t channels,
        double *a,
        double *b,
        int32_t channelsFirst,
        int32_t heightShift,
        int32_t widthShift,
        int32_t verticalFlip,
        int32_t horizontalFlip,
        int32_t rgbFlip
    ) {
        rindow_matlib_d_imagecopy(
            height,
            width,
            channels,
            a,
            b,
            channelsFirst,
            heightShift,
            widthShift,
            verticalFlip,
            horizontalFlip,
            rgbFlip
        );
    }
    virtual void test_matlib_imagecopy(
        int32_t height,
        int32_t width,
        int32_t channels,
        uint8_t *a,
        uint8_t *b,
        int32_t channelsFirst,
        int32_t heightShift,
        int32_t widthShift,
        int32_t verticalFlip,
        int32_t horizontalFlip,
        int32_t rgbFlip
    ) {
        rindow_matlib_i8_imagecopy(
            height,
            width,
            channels,
            a,
            b,
            channelsFirst,
            heightShift,
            widthShift,
            verticalFlip,
            horizontalFlip,
            rgbFlip
        );
    }
};
typedef ::testing::Types<float, double, uint8_t> TestTypes;
TYPED_TEST_SUITE(ImagecopyTest, TestTypes);

TYPED_TEST(ImagecopyTest, Simple) {
    const int32_t channelsFirst  = false;
    const int32_t height = 3;
    const int32_t width = 3;
    const int32_t channels = 1;

    TypeParam a[height][width][channels];
    Utils::range<TypeParam>(0,height*width*channels,(TypeParam*)a);
    TypeParam b[height][width][channels];
    Utils::zeros<TypeParam>(height*width*channels,(TypeParam*)b);

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        1,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam R1[height][width][channels] = {
        {{0},{1},{2}},
        {{0},{1},{2}},
        {{3},{4},{5}}
    };
    EXPECT_THAT(R1, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        -1,     // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R2[height][width][channels] = {
        {{3},{4},{5}},
        {{6},{7},{8}},
        {{6},{7},{8}}
    };
    EXPECT_THAT(R2, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        0,      // heightShift,
        1,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R3[height][width][channels] = {
        {{0},{0},{1}},
        {{3},{3},{4}},
        {{6},{6},{7}}
    };
    EXPECT_THAT(R3, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        0,      // heightShift,
        -1,     // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R4[height][width][channels] = {
        {{1},{2},{2}},
        {{4},{5},{5}},
        {{7},{8},{8}}
    };
    EXPECT_THAT(R4, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        1,      // heightShift,
        1,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R5[height][width][channels] = {
        {{0},{0},{1}},
        {{0},{0},{1}},
        {{3},{3},{4}}
    };
    EXPECT_THAT(R5, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        -1,     // heightShift,
        -1,     // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam R6[height][width][channels] = {
        {{4},{5},{5}},
        {{7},{8},{8}},
        {{7},{8},{8}}
    };
    EXPECT_THAT(R6, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        true,   // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R7[height][width][channels] = {
        {{6},{7},{8}},
        {{3},{4},{5}},
        {{0},{1},{2}}
    };
    EXPECT_THAT(R7, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R8[height][width][channels] = {
        {{2},{1},{0}},
        {{5},{4},{3}},
        {{8},{7},{6}}
    };
    EXPECT_THAT(R8, ContainerEq(b));
}
TYPED_TEST(ImagecopyTest, Complex) {
    const int32_t channelsFirst  = false;
    const int32_t height = 4;
    const int32_t width = 4;
    const int32_t channels = 1;

    TypeParam a[height][width][channels];
    Utils::range<TypeParam>(0,height*width*channels,(TypeParam*)a); 
    TypeParam b[height][width][channels];
    Utils::zeros<TypeParam>(height*width*channels,(TypeParam*)b); 

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        1,      // heightShift,
        0,      // widthShift,
        true,   // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R1[height][width][channels] = {
        {{12},{13},{14},{15}},
        {{12},{13},{14},{15}},
        {{ 8},{ 9},{10},{11}},
        {{ 4},{ 5},{ 6},{ 7}}
    };
    EXPECT_THAT(R1, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        -1,     // heightShift,
        0,      // widthShift,
        true,   // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R2[height][width][channels] = {
        {{8},{9},{10},{11}},
        {{4},{5},{ 6},{ 7}},
        {{0},{1},{ 2},{ 3}},
        {{0},{1},{ 2},{ 3}}
    };
    EXPECT_THAT(R2, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        0,      // heightShift,
        1,      // widthShift,
        false,  // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R3[height][width][channels] = {
        {{ 3},{ 3},{ 2},{ 1}},
        {{ 7},{ 7},{ 6},{ 5}},
        {{11},{11},{10},{ 9}},
        {{15},{15},{14},{13}}
    };
    EXPECT_THAT(R3, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        0,      // heightShift,
        -1,     // widthShift,
        false,  // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R4[height][width][channels] = {
        {{ 2},{ 1},{ 0},{ 0}},
        {{ 6},{ 5},{ 4},{ 4}},
        {{10},{ 9},{ 8},{ 8}},
        {{14},{13},{12},{12}}
    };
    EXPECT_THAT(R4, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        true,   // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R5[height][width][channels] = {
        {{15},{14},{13},{12}},
        {{11},{10},{ 9},{ 8}},
        {{ 7},{ 6},{ 5},{ 4}},
        {{ 3},{ 2},{ 1},{ 0}}
    };
    EXPECT_THAT(R5, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        1,      // heightShift,
        0,      // widthShift,
        true,   // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R6[height][width][channels] = {
        {{15},{14},{13},{12}},
        {{15},{14},{13},{12}},
        {{11},{10},{ 9},{ 8}},
        {{ 7},{ 6},{ 5},{ 4}}
    };
    EXPECT_THAT(R6, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        -1,     // heightShift,
        0,      // widthShift,
        true,   // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R7[height][width][channels] = {
        {{11},{10},{9},{8}},
        {{ 7},{ 6},{5},{4}},
        {{ 3},{ 2},{1},{0}},
        {{ 3},{ 2},{1},{0}}
    };
    EXPECT_THAT(R7, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        0,      // heightShift,
        1,      // widthShift,
        true,   // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R8[height][width][channels] = {
        {{15},{15},{14},{13}},
        {{11},{11},{10},{ 9}},
        {{ 7},{ 7},{ 6},{ 5}},
        {{ 3},{ 3},{ 2},{ 1}}
    };
    EXPECT_THAT(R8, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        0,      // heightShift,
        -1,     // widthShift,
        true,   // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R9[height][width][channels] = {
        {{14},{13},{12},{12}},
        {{10},{ 9},{ 8},{ 8}},
        {{ 6},{ 5},{ 4},{ 4}},
        {{ 2},{ 1},{ 0},{ 0}}
    };
    EXPECT_THAT(R9, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        1,      // heightShift,
        1,      // widthShift,
        true,   // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R10[height][width][channels] = {
        {{15},{15},{14},{13}},
        {{15},{15},{14},{13}},
        {{11},{11},{10},{ 9}},
        {{ 7},{ 7},{ 6},{ 5}}
    };
    EXPECT_THAT(R10, ContainerEq(b));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        -1,     // heightShift,
        -1,     // widthShift,
        true,   // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R11[height][width][channels] = {
        {{10},{9},{8},{8}},
        {{ 6},{5},{4},{4}},
        {{ 2},{1},{0},{0}},
        {{ 2},{1},{0},{0}}
    };
    EXPECT_THAT(R11, ContainerEq(b));
}
TYPED_TEST(ImagecopyTest, WithRGB) {
    const int32_t channelsFirst  = false;
    const int32_t height   = 3;
    const int32_t width    = 2;
    const int32_t channels = 3;

    TypeParam a[height][width][channels] = {
        {{1,2,3},
         {1,2,3}},

        {{4,5,6},
         {4,5,6}},

        {{7,8,9},
         {7,8,9}},
    };
    TypeParam b[height][width][channels];
    Utils::zeros<TypeParam>(height*width*channels,(TypeParam*)b);

    // ============heightShift==================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        1,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R1[height][width][channels] = {
        {{1,2,3},
         {1,2,3}},

        {{1,2,3},
         {1,2,3}},

        {{4,5,6},
         {4,5,6}},
    };
    EXPECT_THAT(R1, ContainerEq(b));

    // ===========flip rgb===================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        true    // rgbFlip
    );

    TypeParam R2[height][width][channels] = {
        {{3,2,1},
         {3,2,1}},

        {{6,5,4},
         {6,5,4}},

        {{9,8,7},
         {9,8,7}},
    };
    EXPECT_THAT(R2, ContainerEq(b));
}
TYPED_TEST(ImagecopyTest, WithRGBA) {
    const int32_t channelsFirst  = false;
    const int32_t height   = 3;
    const int32_t width    = 2;
    const int32_t channels = 4;

    TypeParam a[height][width][channels] = {
        {{1,2,3,4},
         {1,2,3,4}},

        {{4,5,6,7},
         {4,5,6,7}},

        {{7,8,9,10},
         {7,8,9,10}},
    };
    TypeParam b[height][width][channels];
    Utils::zeros<TypeParam>(height*width*channels,(TypeParam*)b);

    // ============flip rgb with alpha==================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        true    // rgbFlip
    );

    TypeParam R1[height][width][channels] = {
        {{3,2,1,4},
         {3,2,1,4}},

        {{6,5,4,7},
         {6,5,4,7}},

        {{9,8,7,10},
         {9,8,7,10}},
    };
    EXPECT_THAT(R1, ContainerEq(b));
}
TYPED_TEST(ImagecopyTest, channelsFirst) {
    const int32_t channelsFirst  = true;
    const int32_t height   = 2;
    const int32_t width    = 3;
    const int32_t channels = 3;

    TypeParam a[channels][height][width] = {
        {{1,2,3},
         {4,5,6}},

        {{11,12,13},
         {14,15,16}},

        {{21,22,23},
         {24,25,26}}
    };
    TypeParam b[channels][height][width];
    Utils::zeros<TypeParam>(height*width*channels,(TypeParam*)b);

    // ============heightShift==================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        1,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R1[channels][height][width] = {
        {{ 1, 2, 3},
         { 1, 2, 3}},

        {{11,12,13},
         {11,12,13}},

        {{21,22,23},
         {21,22,23}},
    };
    EXPECT_THAT(R1, ContainerEq(b));

    // ===========widthShift===================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        0,      // heightShift,
        1,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R2[channels][height][width] = {
        {{ 1, 1, 2},
         { 4, 4, 5}},

        {{11,11,12},
         {14,14,15}},

        {{21,21,22},
         {24,24,25}},
    };
    EXPECT_THAT(R2, ContainerEq(b));

    // ===========verticalFlip===================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        true,   // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R3[channels][height][width] = {
        {{ 4, 5, 6},
         { 1, 2, 3}},

        {{14,15,16},
         {11,12,13}},

        {{24,25,26},
         {21,22,23}},
    };
    EXPECT_THAT(R3, ContainerEq(b));

    // ===========horizontalFlip===================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );

    TypeParam R4[channels][height][width] = {
        {{ 3, 2, 1},
         { 6, 5, 4}},

        {{13,12,11},
         {16,15,14}},

        {{23,22,21},
         {26,25,24}},
    };
    EXPECT_THAT(R4, ContainerEq(b));

    // ===========rgbFlip===================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        (TypeParam*)a,
        (TypeParam*)b,
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        true    // rgbFlip
    );

    TypeParam R5[channels][height][width] = {
        {{21,22,23},
         {24,25,26}},

        {{11,12,13},
         {14,15,16}},

        {{1,2,3},
         {4,5,6}},
    };
    EXPECT_THAT(R5, ContainerEq(b));
}
}
