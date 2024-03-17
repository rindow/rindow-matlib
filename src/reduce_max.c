#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_REDUCEMAX_TEMPLATE(a,b,maxfunc) \
        b[i*ldB+j] = maxfunc(n,&a[i*ldA+j],k);


void rindow_matlib_s_reducemax(
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
                RINDOW_MATLIB_REDUCEMAX_TEMPLATE(a,b,rindow_matlib_common_s_max)
            }
        }
   } else {
        int32_t j;
        #pragma omp parallel for
        for(j=0; j<k; j++) {
            for(int32_t i=0; i<m; i++) {
                RINDOW_MATLIB_REDUCEMAX_TEMPLATE(a,b,rindow_matlib_common_s_max)
            }
        }
   }
}

void rindow_matlib_d_reducemax(
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
                RINDOW_MATLIB_REDUCEMAX_TEMPLATE(a,b,rindow_matlib_common_d_max)
            }
        }
   } else {
        int32_t j;
        #pragma omp parallel for
        for(j=0; j<k; j++) {
            for(int32_t i=0; i<m; i++) {
                RINDOW_MATLIB_REDUCEMAX_TEMPLATE(a,b,rindow_matlib_common_d_max)
            }
        }
   }
}

