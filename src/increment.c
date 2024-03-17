#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

void rindow_matlib_s_increment(int32_t n, float *x, int32_t incX, float alpha, float beta)
{
    if(incX==1 && n<256) {
        int32_t i;
        #pragma omp simd
        for(i=0;i<n;i++) {
            x[i] = alpha * x[i] + beta;
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        for(i=0;i<n;i++) {
            x[i*incX] = alpha * x[i*incX] + beta;
        }
    }
}

void rindow_matlib_d_increment(int32_t n, double *x, int32_t incX, double alpha, double beta)
{
    if(incX==1 && n<256) {
        int32_t i;
        #pragma omp simd
        for(i=0;i<n;i++) {
            x[i] = alpha * x[i] + beta;
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        for(i=0;i<n;i++) {
            x[i*incX] = alpha * x[i*incX] + beta;
        }
    }
}

