#include "rindow/matlib.h"
#include "common.h"

void rindow_matlib_s_cumsum(
    int32_t n,
    float *x, int32_t incX,
    int32_t exclusive,
    int32_t reverse,
    float *y, int32_t incY)
{
    float value = 0.0;
    int32_t idxX,idxY;

    if(reverse) {
        idxX = 0;
        idxY = incY*(n-1);
        incY = -incY;
    } else {
        idxX = 0;
        idxY = 0;
    }

    if(exclusive) {
        for(int32_t i=0;i<n;i++,idxX+=incX,idxY+=incY) {
            y[idxY] = value;
            value += x[idxX];
        }
    } else {
        for(int32_t i=0;i<n;i++,idxX+=incX,idxY+=incY) {
            value += x[idxX];
            y[idxY] = value;
        }
    }
}

void rindow_matlib_d_cumsum(
    int32_t n,
    double *x, int32_t incX,
    int32_t exclusive,
    int32_t reverse,
    double *y, int32_t incY)
{
    double value = 0.0;
    int32_t idxX,idxY;

    if(reverse) {
        idxX = 0;
        idxY = incY*(n-1);
        incY = -incY;
    } else {
        idxX = 0;
        idxY = 0;
    }

    if(exclusive) {
        for(int32_t i=0;i<n;i++,idxX+=incX,idxY+=incY) {
            y[idxY] = value;
            value += x[idxX];
        }
    } else {
        for(int32_t i=0;i<n;i++,idxX+=incX,idxY+=incY) {
            value += x[idxX];
            y[idxY] = value;
        }
    }
}
