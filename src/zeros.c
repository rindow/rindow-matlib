#include "rindow/matlib.h"
#include "common.h"
#include <string.h>

#define RINDOW_MATLIB_ZEROS_TEMPLATE(x,i,incX) \
    for(i=0;i<n;i++) { \
        x[i*incX] = 0.0; \
    }


void rindow_matlib_s_zeros(int32_t n, float *x, int32_t incX)
{
    if(incX==1) {
        int32_t valueSize = sizeof(float);
        int32_t num_threads = rindow_matlib_common_get_num_threads();
        int32_t fillsize = n/num_threads;
        int32_t i;
        #pragma omp parallel for
        for(i=0;i<num_threads;i++) {
            memset(&(x[i*fillsize]),0,fillsize*valueSize);
        }
        int32_t adiv = n%num_threads;
        if(adiv!=0) {
            memset(&(x[num_threads*fillsize]),0,adiv*valueSize);
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        RINDOW_MATLIB_ZEROS_TEMPLATE(x,i,incX)
    }
}

void rindow_matlib_d_zeros(int32_t n, double *x, int32_t incX)
{
    if(incX==1) {
        int32_t valueSize = sizeof(double);
        int32_t num_threads = rindow_matlib_common_get_num_threads();
        int32_t fillsize = n/num_threads;
        int32_t i;
        #pragma omp parallel for
        for(i=0;i<num_threads;i++) {
            memset(&(x[i*fillsize]),0,fillsize*valueSize);
        }
        int32_t adiv = n%num_threads;
        if(adiv!=0) {
            memset(&(x[num_threads*fillsize]),0,adiv*valueSize);
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        RINDOW_MATLIB_ZEROS_TEMPLATE(x,i,incX)
    }
}

void rindow_matlib_i_zeros(int32_t dtype, int32_t n,void *x, int32_t incX)
{
    int32_t valueSize = (int32_t)rindow_matlib_common_dtype_to_valuesize(dtype);
    if(valueSize==0) {
        return;
    }
    if(incX==1) {
        int32_t num_threads = rindow_matlib_common_get_num_threads();
        int32_t fillsize = n/num_threads;
        int32_t i;
        #pragma omp parallel for
        for(i=0;i<num_threads;i++) {
            memset(&(((char *)x)[i*fillsize*valueSize]),0,fillsize*valueSize);
        }
        int32_t adiv = n%num_threads;
        if(adiv!=0) {
            memset(&(((char *)x)[num_threads*fillsize*valueSize]),0,adiv*valueSize);
        }
    } else {
        int32_t i;
        #pragma omp parallel for
        for(i=0;i<n;i++) {
            memset(&((char *)x)[i*incX*valueSize],0,valueSize);
        }
    }
}
