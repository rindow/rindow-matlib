#include "rindow/matlib.h"
#include "common.h"
#include <math.h>

/**
 *     A[m,n] := 1 (A[m,n] >= X[n])
 *     A[m,n] := 0 (A[m,n] <  X[n])
 */


// *** CAUTION ***
// if NaN set 0.0
// if equal set 1.0
#define RINDOW_MATLIB_GREATER_EQUAL_TEMPLATE(a,i,ldA,x,j,incX) \
    if(a[i*ldA+j] >= x[j*incX]) {   \
        a[i*ldA+j] = 1.0;           \
    } else {                        \
        a[i*ldA+j] = 0.0;           \
    }                               \

void rindow_matlib_s_greater_equal(
    int32_t m,int32_t n,float *a, int32_t ldA,float *x, int32_t incX)
{
    if(m>n) {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<m; i++) {
            for(int32_t j=0;j<n; j++) {
                RINDOW_MATLIB_GREATER_EQUAL_TEMPLATE(a,i,ldA,x,j,incX)
            }
        }
    } else {
        int32_t j;
        #pragma omp parallel for
        for(j=0; j<n; j++) {
            for(int32_t i=0;i<m; i++) {
                RINDOW_MATLIB_GREATER_EQUAL_TEMPLATE(a,i,ldA,x,j,incX)
            }
        }
    }
}

void rindow_matlib_d_greater_equal(
    int32_t m,int32_t n,double *a, int32_t ldA,double *x, int32_t incX)
{
    if(m>n) {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<m; i++) {
            for(int32_t j=0;j<n; j++) {
                RINDOW_MATLIB_GREATER_EQUAL_TEMPLATE(a,i,ldA,x,j,incX)
            }
        }
    } else {
        int32_t j;
        #pragma omp parallel for
        for(j=0; j<n; j++) {
            for(int32_t i=0;i<m; i++) {
                RINDOW_MATLIB_GREATER_EQUAL_TEMPLATE(a,i,ldA,x,j,incX)
            }
        }
    }
}
