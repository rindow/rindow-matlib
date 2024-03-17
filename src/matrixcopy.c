#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

void rindow_matlib_s_matrixcopy(
    int32_t trans, int32_t m, int32_t n, float alpha,
    float *a, int32_t ldA, float *b, int32_t ldB)
{
    if(trans==RINDOW_MATLIB_NO_TRANS) {
        if(m>n) {
            int32_t i;
            #pragma omp parallel for
            for(i=0;i<m;i++) {
                for(int32_t j=0;j<n;j++) {
                    b[i*ldB+j] = alpha * a[i*ldA+j];
                }
            }
        } else {
            int32_t j;
            #pragma omp parallel for
            for(j=0;j<n;j++) {
                for(int32_t i=0;i<m;i++) {
                    b[i*ldB+j] = alpha * a[i*ldA+j];
                }
            }
        }
    } else if(trans==RINDOW_MATLIB_TRANS) {
        if(m>n) {
            int32_t i;
            #pragma omp parallel for
            for(i=0;i<m;i++) {
                for(int32_t j=0;j<n;j++) {
                    b[j*ldB+i] = alpha * a[i*ldA+j];
                }
            }
        } else {
            int32_t j;
            #pragma omp parallel for
            for(j=0;j<n;j++) {
                for(int32_t i=0;i<m;i++) {
                    b[j*ldB+i] = alpha * a[i*ldA+j];
                }
            }
        }
    }
}

void rindow_matlib_d_matrixcopy(
    int32_t trans, int32_t m, int32_t n, double alpha,
    double *a, int32_t ldA, double *b, int32_t ldB)
{
    if(trans==RINDOW_MATLIB_NO_TRANS) {
        if(m>n) {
            int32_t i;
            #pragma omp parallel for
            for(i=0;i<m;i++) {
                for(int32_t j=0;j<n;j++) {
                    b[i*ldB+j] = alpha * a[i*ldA+j];
                }
            }
        } else {
            int32_t j;
            #pragma omp parallel for
            for(j=0;j<n;j++) {
                for(int32_t i=0;i<m;i++) {
                    b[i*ldB+j] = alpha * a[i*ldA+j];
                }
            }
        }
    } else if(trans==RINDOW_MATLIB_TRANS) {
        if(m>n) {
            int32_t i;
            #pragma omp parallel for
            for(i=0;i<m;i++) {
                for(int32_t j=0;j<n;j++) {
                    b[j*ldB+i] = alpha * a[i*ldA+j];
                }
            }
        } else {
            int32_t j;
            #pragma omp parallel for
            for(j=0;j<n;j++) {
                for(int32_t i=0;i<m;i++) {
                    b[j*ldB+i] = alpha * a[i*ldA+j];
                }
            }
        }
    }
}
