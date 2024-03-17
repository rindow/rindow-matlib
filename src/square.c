#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_SQUARE_SIMD_TEMPLATE(x,i) \
        x[i] = x[i] * x[i];

#define RINDOW_MATLIB_SQUARE_TEMPLATE(x,i,incX) \
        x[i*incX] = x[i*incX] * x[i*incX];

void rindow_matlib_s_square(int32_t n, float *x, int32_t incX)
{
    if(incX==1 && n<256) {
        int32_t i;
        #pragma omp simd
        for(i=0;i<n;i++) {
            RINDOW_MATLIB_SQUARE_SIMD_TEMPLATE(x,i)
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        for(i=0;i<n;i++) {
            RINDOW_MATLIB_SQUARE_TEMPLATE(x,i,incX)
        }
    }
}

void rindow_matlib_d_square(int32_t n, double *x, int32_t incX)
{
    if(incX==1 && n<256) {
        int32_t i;
        #pragma omp simd
        for(i=0;i<n;i++) {
            RINDOW_MATLIB_SQUARE_SIMD_TEMPLATE(x,i)
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        for(i=0;i<n;i++) {
            RINDOW_MATLIB_SQUARE_TEMPLATE(x,i,incX)
        }
    }
}

