#include "rindow/matlib.h"
#include "common.h"

#define RINDOW_MATLIB_ADD_TEMPLATE(alpha,x,j,incX,a,i,incAi,incAj) \
    a[i*incAi+j*incAj] += (alpha*x[j*incX]);

void rindow_matlib_s_add(
    int32_t trans,int32_t m,int32_t n,float alpha,float *x, int32_t incX,float *a, int32_t ldA)
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
                RINDOW_MATLIB_ADD_TEMPLATE(alpha,x,j,incX,a,i,incAi,incAj)
            }
        }
    } else {
        int32_t j;
        #pragma omp parallel for
        for(j=0; j<cols; j++) {
            for(int32_t i=0;i<rows; i++) {
                RINDOW_MATLIB_ADD_TEMPLATE(alpha,x,j,incX,a,i,incAi,incAj)
            }
        }
    }
}

void rindow_matlib_d_add(
    int32_t trans,int32_t m,int32_t n,double alpha,double *x, int32_t incX,double *a, int32_t ldA)
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
                RINDOW_MATLIB_ADD_TEMPLATE(alpha,x,j,incX,a,i,incAi,incAj)
            }
        }
    } else {
        int32_t j;
        #pragma omp parallel for
        for(j=0; j<cols; j++) {
            for(int32_t i=0;i<rows; i++) {
                RINDOW_MATLIB_ADD_TEMPLATE(alpha,x,j,incX,a,i,incAi,incAj)
            }
        }
    }
}
