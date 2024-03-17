#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_REDUCEARGMAX_TEMPLATE(a,b,maxfunc) \
        int64_t index = maxfunc(n,&a[i*ldA+j],k); \
        rindow_matlib_common_set_integer(dtype, b, 1, i*ldB+j, index);


void rindow_matlib_s_reduceargmax(
    int32_t m,
    int32_t n,
    int32_t k,
    float *a,
    int32_t dtype,
    void *b
)
{
    int32_t ldA = n*k;
    int32_t ldB = k;
    if(m>k) {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<m; i++) {
            for(int32_t j=0; j<k; j++) {
                RINDOW_MATLIB_REDUCEARGMAX_TEMPLATE(a,b,rindow_matlib_common_s_argmax)
            }
        }
   } else {
        int32_t j;
        #pragma omp parallel for
        for(j=0; j<k; j++) {
            for(int32_t i=0; i<m; i++) {
                RINDOW_MATLIB_REDUCEARGMAX_TEMPLATE(a,b,rindow_matlib_common_s_argmax)
            }
        }
   }
}

void rindow_matlib_d_reduceargmax(
    int32_t m,
    int32_t n,
    int32_t k,
    double *a,
    int32_t dtype,
    void *b
)
{
    int32_t ldA = n*k;
    int32_t ldB = k;
    if(m>k) {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<m; i++) {
            for(int32_t j=0; j<k; j++) {
                RINDOW_MATLIB_REDUCEARGMAX_TEMPLATE(a,b,rindow_matlib_common_d_argmax)
            }
        }
   } else {
        int32_t j;
        #pragma omp parallel for
        for(j=0; j<k; j++) {
            for(int32_t i=0; i<m; i++) {
                RINDOW_MATLIB_REDUCEARGMAX_TEMPLATE(a,b,rindow_matlib_common_d_argmax)
            }
        }
   }
}

