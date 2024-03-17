#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

// if NaN set NaN
// Compatible with reduce_max of tensorflow 2.6
#define RINDOW_MATLIB_SOFTMAX_MAXSUB(valuetype,expfunc,n,a,ldA) \
    valuetype *ax = &a[i*ldA]; \
    valuetype t,max_a,sum_exp; \
    \
    max_a = ax[0]; \
    for(int j=1;j<n;j++) { \
        if(!(max_a>=ax[j])) { \
            max_a = ax[j]; \
        } \
    } \
    \
    sum_exp = 0; \
    for(int32_t j=0;j<n;j++) { \
        t = expfunc(ax[j]-max_a); \
        sum_exp += t; \
        ax[j] = t; \
    } \
    for(int32_t j=0;j<n;j++) { \
        ax[j] = ax[j] / sum_exp; \
    } \


void rindow_matlib_s_softmax(
    int32_t m, int32_t n, float *a, int32_t ldA)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<m;i++) {
        RINDOW_MATLIB_SOFTMAX_MAXSUB(float,expf,n,a,ldA)
    }
}

void rindow_matlib_d_softmax(
    int32_t m, int32_t n, double *a, int32_t ldA)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<m;i++) {
        RINDOW_MATLIB_SOFTMAX_MAXSUB(double,exp,n,a,ldA)
    }
}
