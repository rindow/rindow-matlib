#include "rindow/matlib.h"
#include "common.h"
#include <string.h>

void rindow_matlib_s_bandpart(
    int32_t m, int32_t n, int32_t k,
    float *a,
    int32_t lower, int32_t upper)
{
    if(m>n) {
        int32_t batch;
        #pragma omp parallel for
        for(batch=0;batch<m;batch++) {
            for(int32_t i=0; i<n; i++) {
                for(int32_t j=0;j<k;j++) {
                    if((lower >= 0 && (i-j) > lower) || (upper >= 0 && (j-i) > upper)) {
                        a[batch*n*k+i*k+j] = 0;
                    }
                }
            }
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<n; i++) {
            for(int32_t batch=0;batch<m;batch++) {
                for(int32_t j=0;j<k;j++) {
                    if((lower >= 0 && (i-j) > lower) || (upper >= 0 && (j-i) > upper)) {
                        a[batch*n*k+i*k+j] = 0;
                    }
                }
            }
        }
    }
}

void rindow_matlib_d_bandpart(
    int32_t m, int32_t n, int32_t k,
    double *a,
    int32_t lower, int32_t upper)
{
    if(m>n) {
        int32_t batch;
        #pragma omp parallel for
        for(batch=0;batch<m;batch++) {
            for(int32_t i=0; i<n; i++) {
                for(int32_t j=0;j<k;j++) {
                    if((lower >= 0 && (i-j) > lower) || (upper >= 0 && (j-i) > upper)) {
                        a[batch*n*k+i*k+j] = 0;
                    }
                }
            }
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<n; i++) {
            for(int32_t batch=0;batch<m;batch++) {
                for(int32_t j=0;j<k;j++) {
                    if((lower >= 0 && (i-j) > lower) || (upper >= 0 && (j-i) > upper)) {
                        a[batch*n*k+i*k+j] = 0;
                    }
                }
            }
        }
    }
}

void rindow_matlib_i_bandpart(
    int32_t m, int32_t n, int32_t k,
    int32_t dtype,
    void *a,
    int32_t lower, int32_t upper)
{
    int32_t value_width = rindow_matlib_common_dtype_to_valuesize(dtype);
    if(value_width==0) {
        return;
    }
    char *ac=a;
    if(m>n) {
        int32_t batch;
        #pragma omp parallel for
        for(batch=0;batch<m;batch++) {
            for(int32_t i=0; i<n; i++) {
                for(int32_t j=0;j<k;j++) {
                    if((lower >= 0 && (i-j) > lower) || (upper >= 0 && (j-i) > upper)) {
                        memset(&ac[(batch*n*k+i*k+j)*value_width],0,value_width);
                    }
                }
            }
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<n; i++) {
            for(int32_t batch=0;batch<m;batch++) {
                for(int32_t j=0;j<k;j++) {
                    if((lower >= 0 && (i-j) > lower) || (upper >= 0 && (j-i) > upper)) {
                        memset(&ac[(batch*n*k+i*k+j)*value_width],0,value_width);
                    }
                }
            }
        }
    }
}
