#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_SEARCHSORTED_TEMPLATE(valuetype,a,x,y) \
    valuetype value = x[i*incX];      \
    int32_t j;                        \
    if(right) {                       \
        for(j=0;j<n;j++) {            \
            if(!(value>=a[i*ldA+j])) { \
                break;                \
            }                         \
        }                             \
    } else {                          \
        for(j=0;j<n;j++) {            \
            if(!(value>a[i*ldA+j])) {  \
                break;                \
            }                         \
        }                             \
    }                                 \
    rindow_matlib_common_set_integer(dtype, y, incY, i, j); \


void rindow_matlib_s_searchsorted(
    int32_t m,
    int32_t n,
    float *a, int32_t ldA,
    float *x, int32_t incX,
    int32_t right,
    int32_t dtype, void *y, int32_t incY)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<m;i++) {
        RINDOW_MATLIB_SEARCHSORTED_TEMPLATE(float,a,x,y)
    }
}

void rindow_matlib_d_searchsorted(
    int32_t m,
    int32_t n,
    double *a, int32_t ldA,
    double *x, int32_t incX,
    int32_t right,
    int32_t dtype, void *y, int32_t incY)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<m;i++) {
        RINDOW_MATLIB_SEARCHSORTED_TEMPLATE(double,a,x,y)
    }
}
