#include "rindow/matlib.h"
#include "common.h"

#define RINDOW_MATLIB_CUMSUM_TEMPLATE(i,n,x,incX,exclusive,reverse,y,incY,type) \
    type value = 0.0; \
    int32_t idxX,idxY,incYY; \
    if(reverse) { \
        idxX = i*n*incX; \
        idxY = ((i+1)*n-1)*incY; \
        incYY = -incY; \
    } else { \
        idxX = i*n*incX; \
        idxY = i*n*incY; \
        incYY = incY; \
    } \
    if(exclusive) { \
        for(int32_t j=0;j<n;j++,idxX+=incX,idxY+=incYY) { \
            y[idxY] = value; \
            value += x[idxX]; \
        } \
    } else { \
        for(int32_t j=0;j<n;j++,idxX+=incX,idxY+=incYY) { \
            value += x[idxX]; \
            y[idxY] = value; \
        } \
    } \


void rindow_matlib_s_cumsum(
    int32_t n,
    float *x, int32_t incX,
    int32_t exclusive,
    int32_t reverse,
    float *y, int32_t incY)
{
    RINDOW_MATLIB_CUMSUM_TEMPLATE(0,n,x,incX,exclusive,reverse,y,incY,float);
}

void rindow_matlib_d_cumsum(
    int32_t n,
    double *x, int32_t incX,
    int32_t exclusive,
    int32_t reverse,
    double *y, int32_t incY)
{
    RINDOW_MATLIB_CUMSUM_TEMPLATE(0,n,x,incX,exclusive,reverse,y,incY,double);
}
