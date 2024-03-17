#include "rindow/matlib.h"
#include "common.h"
#include <math.h>

#define RINDOW_MATLIB_EXP_TEMPLATE(x,i,incX,expfunc) \
    x[i*incX] = expfunc(x[i*incX]);


void rindow_matlib_s_exp(int32_t n, float *x, int32_t incX)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<n;i++) {
        RINDOW_MATLIB_EXP_TEMPLATE(x,i,incX,expf)
    }
}

void rindow_matlib_d_exp(int32_t n, double *x, int32_t incX)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<n;i++) {
        RINDOW_MATLIB_EXP_TEMPLATE(x,i,incX,exp)
    }
}

