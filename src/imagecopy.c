#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_IMAGECOPY_TEMPLATE(a,b) \
    for(int32_t c=0;c<channels;c++) {    \
        int32_t sy = y*directionY+biasY; \
        int32_t sx = x*directionX+biasX; \
        if(sy<0) {                       \
            sy = 0;                      \
        } else if(sy>=height) {          \
            sy = height-1;               \
        }                                \
        if(sx<0) {                       \
            sx = 0;                      \
        } else if(sx>=width) {           \
            sx = width-1;                \
        }                                \
        int32_t srcc = (rgbFlip&&c<3)?(2-c):c; \
        b[y*ldY+x*ldX+c*ldC] =                 \
            a[sy*ldY+sx*ldX+srcc*ldC];         \
    }


void rindow_matlib_s_imagecopy(
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
)
{
    int32_t ldX;
    int32_t ldY;
    int32_t ldC;
    int32_t directionX;
    int32_t directionY;
    int32_t biasX;
    int32_t biasY;

    if(channelsFirst) {
        ldC = width*height;
        ldY = width;
        ldX = 1;
    } else {
        ldY = width*channels;
        ldX = channels;
        ldC = 1;
    }
    directionY = directionX = 1;
    biasY = biasX = 0;
    if(verticalFlip) {
        directionY = -directionY;
        biasY = height-1;
    }
    if(horizontalFlip) {
        directionX = -directionX;
        biasX = width-1;
    }
    biasY -= heightShift*directionY;
    biasX -= widthShift*directionX;

    if(height>width) {
        int32_t y;
        #pragma omp parallel for
        for(y=0;y<height;y++) {
            for(int32_t x=0;x<width;x++) {
                RINDOW_MATLIB_IMAGECOPY_TEMPLATE(a,b)
            }
        }
    } else {
        int32_t x;
        #pragma omp parallel for
        for(x=0;x<width;x++) {
            for(int32_t y=0;y<height;y++) {
                RINDOW_MATLIB_IMAGECOPY_TEMPLATE(a,b)
            }
        }

    }
}

void rindow_matlib_d_imagecopy(
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
)
{
    int32_t ldX;
    int32_t ldY;
    int32_t ldC;
    int32_t directionX;
    int32_t directionY;
    int32_t biasX;
    int32_t biasY;

    if(channelsFirst) {
        ldC = width*height;
        ldY = width;
        ldX = 1;
    } else {
        ldY = width*channels;
        ldX = channels;
        ldC = 1;
    }
    directionY = directionX = 1;
    biasY = biasX = 0;
    if(verticalFlip) {
        directionY = -directionY;
        biasY = height-1;
    }
    if(horizontalFlip) {
        directionX = -directionX;
        biasX = width-1;
    }
    biasY -= heightShift*directionY;
    biasX -= widthShift*directionX;

    if(height>width) {
        int32_t y;
        #pragma omp parallel for
        for(y=0;y<height;y++) {
            for(int32_t x=0;x<width;x++) {
                RINDOW_MATLIB_IMAGECOPY_TEMPLATE(a,b)
            }
        }
    } else {
        int32_t x;
        #pragma omp parallel for
        for(x=0;x<width;x++) {
            for(int32_t y=0;y<height;y++) {
                RINDOW_MATLIB_IMAGECOPY_TEMPLATE(a,b)
            }
        }

    }
}

void rindow_matlib_i8_imagecopy(
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
)
{
    int32_t ldX;
    int32_t ldY;
    int32_t ldC;
    int32_t directionX;
    int32_t directionY;
    int32_t biasX;
    int32_t biasY;

    if(channelsFirst) {
        ldC = width*height;
        ldY = width;
        ldX = 1;
    } else {
        ldY = width*channels;
        ldX = channels;
        ldC = 1;
    }
    directionY = directionX = 1;
    biasY = biasX = 0;
    if(verticalFlip) {
        directionY = -directionY;
        biasY = height-1;
    }
    if(horizontalFlip) {
        directionX = -directionX;
        biasX = width-1;
    }
    biasY -= heightShift*directionY;
    biasX -= widthShift*directionX;

    if(height>width) {
        int32_t y;
        #pragma omp parallel for
        for(y=0;y<height;y++) {
            for(int32_t x=0;x<width;x++) {
                RINDOW_MATLIB_IMAGECOPY_TEMPLATE(a,b)
            }
        }
    } else {
        int32_t x;
        #pragma omp parallel for
        for(x=0;x<width;x++) {
            for(int32_t y=0;y<height;y++) {
                RINDOW_MATLIB_IMAGECOPY_TEMPLATE(a,b)
            }
        }

    }
}
