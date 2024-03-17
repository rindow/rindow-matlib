#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_REDUCESUM_TEMPLATE(a,b,sumfunc) \
        b[i*ldB+j] = sumfunc(n,&a[i*ldA+j],k);


void rindow_matlib_s_reducesum(
    int32_t m,
    int32_t n,
    int32_t k,
    float *a,
    float *b
)
{
    int32_t ldA = n*k;
    int32_t ldB = k;
    if(m>k) {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<m; i++) {
            for(int32_t j=0; j<k; j++) {
                RINDOW_MATLIB_REDUCESUM_TEMPLATE(a,b,rindow_matlib_common_s_sum_sb)
            }
        }
   } else {
        int32_t j;
        #pragma omp parallel for
        for(j=0; j<k; j++) {
            for(int32_t i=0; i<m; i++) {
                RINDOW_MATLIB_REDUCESUM_TEMPLATE(a,b,rindow_matlib_common_s_sum_sb)
            }
        }
   }
}

void rindow_matlib_d_reducesum(
    int32_t m,
    int32_t n,
    int32_t k,
    double *a,
    double *b
)
{
    int32_t ldA = n*k;
    int32_t ldB = k;
    if(m>k) {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<m; i++) {
            for(int32_t j=0; j<k; j++) {
                RINDOW_MATLIB_REDUCESUM_TEMPLATE(a,b,rindow_matlib_common_d_sum_sb)
            }
        }
   } else {
        int32_t j;
        #pragma omp parallel for
        for(j=0; j<k; j++) {
            for(int32_t i=0; i<m; i++) {
                RINDOW_MATLIB_REDUCESUM_TEMPLATE(a,b,rindow_matlib_common_d_sum_sb)
            }
        }
   }
}

