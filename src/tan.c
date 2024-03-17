#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_TAN_TEMPLATE(x,i,incX,tanfunc) \
        x[i*incX] = tanfunc(x[i*incX]);


void rindow_matlib_s_tan(int32_t n, float *x, int32_t incX)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<n;i++) {
        RINDOW_MATLIB_TAN_TEMPLATE(x,i,incX,tanf)
    }
}

void rindow_matlib_d_tan(int32_t n, double *x, int32_t incX)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<n;i++) {
        RINDOW_MATLIB_TAN_TEMPLATE(x,i,incX,tan)
    }
}
