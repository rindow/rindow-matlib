#include "rindow/matlib.h"
#include "common.h"
#include <math.h>

#define RINDOW_MATLIB_COS_TEMPLATE(x,i,incX,cosfunc) \
    x[i*incX] = cosfunc(x[i*incX]);


void rindow_matlib_s_cos(int32_t n, float *x, int32_t incX)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<n;i++) {
        RINDOW_MATLIB_COS_TEMPLATE(x,i,incX,cosf)
    }
}

void rindow_matlib_d_cos(int32_t n, double *x, int32_t incX)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<n;i++) {
        RINDOW_MATLIB_COS_TEMPLATE(x,i,incX,cos)
    }
}
