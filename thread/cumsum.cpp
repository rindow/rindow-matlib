#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Cumsum
{
public:
    static void execute(
        int32_t n,
        T *x, int32_t incX,
        int32_t exclusive,
        int32_t reverse,
        T *y, int32_t incY)
    {
        if(n <= 0) {
            return;
        }
        int32_t idxX,idxY;

        if(reverse) {
            idxX = 0;
            idxY = n*incY - incY;
            incY = -incY;
        } else {
            idxX = 0;
            idxY = 0;
        }

        if(exclusive) {
            T value = 0.0;
            for(int32_t j=0;j<n;j++,idxX+=incX,idxY+=incY) {
                y[idxY] = value;
                value += x[idxX];
            }
        } else {
            T value = 0.0;
            for(int32_t j=0;j<n;j++,idxX+=incX,idxY+=incY) {
                value += x[idxX];
                y[idxY] = value;
            }
        }
    }
};

}

extern "C" {
void rindow_matlib_s_cumsum(
    int32_t n,
    float *x, int32_t incX,
    int32_t exclusive,
    int32_t reverse,
    float *y, int32_t incY)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Cumsum<float>::execute(n,x,incX,exclusive,reverse,y,incY);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_cumsum(
    int32_t n,
    double *x, int32_t incX,
    int32_t exclusive,
    int32_t reverse,
    double *y, int32_t incY)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Cumsum<double>::execute(n,x,incX,exclusive,reverse,y,incY);
    RINDOW_END_CLEAR_EXCEPTION;
}
}

