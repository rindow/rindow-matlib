#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

void rindow_matlib_s_reciprocal(int32_t n, float *x, int32_t incX, float alpha, float beta)
{
    if(incX==1 && n<256) {
        int32_t i;
        #pragma omp simd
        for(i=0;i<n;i++) {
            float t;
            t = (float)alpha * x[i] + (float)beta;
            x[i] = 1 / t;
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        for(i=0;i<n;i++) {
            float t;
            t = (float)alpha * x[i*incX] + (float)beta;
            x[i*incX] = 1 / t;
        }
    }
}

void rindow_matlib_d_reciprocal(int32_t n, double *x, int32_t incX, double alpha, double beta)
{
    if(incX==1 && n<256) {
        int32_t i;
        #pragma omp simd
        for(i=0;i<n;i++) {
            double t;
            t = (double)alpha * x[i] + (double)beta;
            x[i] = 1 / t;
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        for(i=0;i<n;i++) {
            double t;
            t = (double)alpha * x[i*incX] + (double)beta;
            x[i*incX] = 1 / t;
        }
    }
}

