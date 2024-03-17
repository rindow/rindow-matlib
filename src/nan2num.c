#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_NAN2NUM_TEMPLATE(x,i,incX,alpha) \
    if(isnan(x[i*incX])) { \
        x[i*incX] = alpha; \
    }


void rindow_matlib_s_nan2num(int32_t n, float *x, int32_t incX, float alpha)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<n;i++) {
        RINDOW_MATLIB_NAN2NUM_TEMPLATE(x,i,incX,alpha)
    }
}

void rindow_matlib_d_nan2num(int32_t n, double *x, int32_t incX, double alpha)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<n;i++) {
        RINDOW_MATLIB_NAN2NUM_TEMPLATE(x,i,incX,alpha)
    }
}
