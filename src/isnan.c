#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_ISNAN_TEMPLATE(x,i,incX) \
    if(isnan(x[i*incX])) { \
        x[i*incX] = 1.0;   \
    } else {               \
        x[i*incX] = 0.0;   \
    }

void rindow_matlib_s_isnan(int32_t n, float *x, int32_t incX)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<n;i++) {
        RINDOW_MATLIB_ISNAN_TEMPLATE(x,i,incX)
    }
}

void rindow_matlib_d_isnan(int32_t n, double *x, int32_t incX)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<n;i++) {
        RINDOW_MATLIB_ISNAN_TEMPLATE(x,i,incX)
    }
}
