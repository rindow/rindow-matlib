#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_SQRT_SIMD_TEMPLATE(x,i,sqrtfunc) \
    x[i] = sqrtfunc(x[i]);

#define RINDOW_MATLIB_SQRT_TEMPLATE(x,i,incX,sqrtfunc) \
    x[i*incX] = sqrtfunc(x[i*incX]);


void rindow_matlib_s_sqrt(int32_t n, float *x, int32_t incX)
{
    if(incX==1 && n<256) {
        int32_t i;
        #pragma omp simd
        for(i=0;i<n;i++) {
            RINDOW_MATLIB_SQRT_SIMD_TEMPLATE(x,i,sqrtf)
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        for(i=0;i<n;i++) {
            RINDOW_MATLIB_SQRT_TEMPLATE(x,i,incX,sqrtf)
        }
    }
}

void rindow_matlib_d_sqrt(int32_t n, double *x, int32_t incX)
{
    if(incX==1 && n<256) {
        int32_t i;
        #pragma omp simd
        for(i=0;i<n;i++) {
            RINDOW_MATLIB_SQRT_SIMD_TEMPLATE(x,i,sqrt)
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        for(i=0;i<n;i++) {
            RINDOW_MATLIB_SQRT_TEMPLATE(x,i,incX,sqrt)
        }
    }
}
