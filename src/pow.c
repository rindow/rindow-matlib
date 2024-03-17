#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_POW_TEMPLATE(a,i,incAi,incAj,x,j,incX,powfunc) \
    a[i*incAi+j*incAj] = powfunc(a[i*incAi+j*incAj],x[j*incX]);

void rindow_matlib_s_pow(
    int32_t trans,int32_t m,int32_t n,float *a, int32_t ldA,float *x, int32_t incX)
{
    int32_t rows,cols,incAi,incAj;
    if(trans==RINDOW_MATLIB_NO_TRANS) {
        rows = m; cols = n;
        incAi = ldA; incAj = 1;
    } else if(trans==RINDOW_MATLIB_TRANS) {
        rows = n; cols = m;
        incAi = 1;   incAj = ldA;
    } else {
        return;
    }

    if(rows>cols) {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<rows; i++) {
            for(int32_t j=0; j<cols; j++) {
                RINDOW_MATLIB_POW_TEMPLATE(a,i,incAi,incAj,x,j,incX,powf)
            }
        }
    } else {
        int32_t j;
        #pragma omp parallel for
        for(j=0; j<cols; j++) {
            for(int32_t i=0;i<rows; i++) {
                RINDOW_MATLIB_POW_TEMPLATE(a,i,incAi,incAj,x,j,incX,powf)
            }
        }
    }
}

void rindow_matlib_d_pow(
    int32_t trans,int32_t m,int32_t n,double *a, int32_t ldA,double *x, int32_t incX)
{
    int32_t rows,cols,incAi,incAj;
    if(trans==RINDOW_MATLIB_NO_TRANS) {
        rows = m; cols = n;
        incAi = ldA; incAj = 1;
    } else if(trans==RINDOW_MATLIB_TRANS) {
        rows = n; cols = m;
        incAi = 1;   incAj = ldA;
    } else {
        return;
    }

    if(rows>cols) {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<rows; i++) {
            for(int32_t j=0; j<cols; j++) {
                RINDOW_MATLIB_POW_TEMPLATE(a,i,incAi,incAj,x,j,incX,pow)
            }
        }
    } else {
        int32_t j;
        #pragma omp parallel for
        for(j=0; j<cols; j++) {
            for(int32_t i=0;i<rows; i++) {
                RINDOW_MATLIB_POW_TEMPLATE(a,i,incAi,incAj,x,j,incX,pow)
            }
        }
    }
}
