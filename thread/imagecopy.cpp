#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Imagecopy
{
private:
    static void kernel_sub(
        const int32_t height,
        const int32_t width,
        const int32_t channels,
        T *a,
        T *b,
        const int32_t rgbFlip,

        const int32_t ldY,
        const int32_t ldX,
        const int32_t ldC,
        const int32_t directionY,
        const int32_t directionX,
        const int32_t biasY,
        const int32_t biasX,

        const int32_t y,
        const int32_t x
    )
    {
        for(int32_t c=0;c<channels;c++) {
            int32_t sy = y*directionY+biasY;
            int32_t sx = x*directionX+biasX;
            if(sy<0) {
                sy = 0;
            } else if(sy>=height) {
                sy = height-1;
            }
            if(sx<0) {
                sx = 0;
            } else if(sx>=width) {
                sx = width-1;
            }
            int32_t srcc = (rgbFlip&&c<3)?(2-c):c;
            b[y*ldY+x*ldX+c*ldC] =
                a[sy*ldY+sx*ldX+srcc*ldC];
        }
    }

    static void kernel(
        ParallelOperation::cellInfo cell,
        bool para_h,
        const int32_t height,
        const int32_t width,
        const int32_t channels,
        T *a,
        T *b,
        const int32_t rgbFlip,

        const int32_t ldY,
        const int32_t ldX,
        const int32_t ldC,
        const int32_t directionY,
        const int32_t directionX,
        const int32_t biasY,
        const int32_t biasX
    )
    {
        if(para_h) {
            for(int32_t y=cell.begin; y<cell.end; y++) {
                for(int32_t x=0; x<width; x++) {
                    kernel_sub(
                        height,
                        width,
                        channels,
                        a,
                        b,
                        rgbFlip,
                        ldY,
                        ldX,
                        ldC,
                        directionY,
                        directionX,
                        biasY,
                        biasX,
                        y,
                        x
                    );
                }
            }
        } else {
            for(int32_t y=0; y<height; y++) {
                for(int32_t x=cell.begin; x<cell.end; x++) {
                    kernel_sub(
                        height,
                        width,
                        channels,
                        a,
                        b,
                        rgbFlip,
                        ldY,
                        ldX,
                        ldC,
                        directionY,
                        directionX,
                        biasY,
                        biasX,
                        y,
                        x
                    );
                }
            }
        }
    }

public:
    static void execute(
        const int32_t height,
        const int32_t width,
        const int32_t channels,
        T *a,
        T *b,
        const int32_t channelsFirst,
        const int32_t heightShift,
        const int32_t widthShift,
        const int32_t verticalFlip,
        const int32_t horizontalFlip,
        const int32_t rgbFlip
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

        int32_t parallel;
        bool para_h;
        if(height>width) {
            parallel = height;
            para_h = true;
        } else {
            parallel = width;
            para_h = false;
        }
        ParallelOperation::execute(
            parallel,kernel,
            para_h,
            height,
            width,
            channels,
            a,
            b,
            rgbFlip,

            ldY,
            ldX,
            ldC,
            directionY,
            directionX,
            biasY,
            biasX
        );
    }
};

}

extern "C" {
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
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Imagecopy<float>::execute(
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
    RINDOW_END_CLEAR_EXCEPTION;
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
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Imagecopy<double>::execute(
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
    RINDOW_END_CLEAR_EXCEPTION;
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
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Imagecopy<uint8_t>::execute(
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
    RINDOW_END_CLEAR_EXCEPTION;
}
}
