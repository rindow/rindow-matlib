#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_RSQRT_SIMD_TEMPLATE(alpha,beta,x,i,sqrtfunc) \
    x[i] = 1 / (alpha * sqrtfunc(x[i]) + beta);

#define RINDOW_MATLIB_RSQRT_TEMPLATE(alpha,beta,x,incX,i,sqrtfunc) \
    x[i*incX] = 1 / (alpha * sqrtfunc(x[i*incX]) + beta);


void rindow_matlib_s_rsqrt(int32_t n, float alpha, float *x, int32_t incX, float beta)
{
    if(incX==1 && n<256) {
        int32_t i;
        #pragma omp simd
        for(i=0;i<n;i++) {
            RINDOW_MATLIB_RSQRT_SIMD_TEMPLATE(alpha,beta,x,i,sqrtf)
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        for(i=0;i<n;i++) {
            RINDOW_MATLIB_RSQRT_TEMPLATE(alpha,beta,x,incX,i,sqrtf)
        }
    }
}

void rindow_matlib_d_rsqrt(int32_t n, double alpha, double *x, int32_t incX, double beta)
{
    if(incX==1 && n<256) {
        int32_t i;
        #pragma omp simd
        for(i=0;i<n;i++) {
            RINDOW_MATLIB_RSQRT_SIMD_TEMPLATE(alpha,beta,x,i,sqrt)
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        for(i=0;i<n;i++) {
            RINDOW_MATLIB_RSQRT_TEMPLATE(alpha,beta,x,incX,i,sqrt)
        }
    }
}

