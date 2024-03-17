#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_NOT_TEMPLATE(i,n,x,incX) \
    for(i=0; i<n; i++) { \
        if(x[i*incX] == 0) \
            x[i*incX] = 1; \
        else \
            x[i*incX] = 0; \
    }

void rindow_matlib_s_not(
    int32_t n, float *x, int32_t incX)
{
    int32_t i;
    #pragma omp parallel for
    RINDOW_MATLIB_NOT_TEMPLATE(i,n,x,incX)
}

void rindow_matlib_d_not(
    int32_t n, double *x, int32_t incX)
{
    int32_t i;
    #pragma omp parallel for
    RINDOW_MATLIB_NOT_TEMPLATE(i,n,x,incX)
}


void rindow_matlib_i_not(
    int32_t dtype, int32_t n, void *x, int32_t incX)
{
    if(!rindow_matlib_common_dtype_is_int(dtype)&&
        !rindow_matlib_common_dtype_is_bool(dtype)) {
        return;
    }
    size_t valueSize = rindow_matlib_common_dtype_to_valuesize(dtype);
    if(valueSize==0) {
        return;
    }

    switch (dtype)
    {
        case rindow_matlib_dtype_bool: {
            uint8_t *typed_x=x;
            int32_t i;
            #pragma omp parallel for
            for(i=0;i<n;i++) {
                if(typed_x[i*incX]==0) {
                    typed_x[i*incX] = 1;
                } else {
                    typed_x[i*incX] = 0;
                }
            }
            break;
        }
        case rindow_matlib_dtype_int8: {
            int8_t *typed_x=x;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_NOT_TEMPLATE(i,n,typed_x,incX)
            break;
        }
        case rindow_matlib_dtype_uint8: {
            uint8_t *typed_x=x;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_NOT_TEMPLATE(i,n,typed_x,incX)
            break;
        }
        case rindow_matlib_dtype_int16: {
            int16_t *typed_x=x;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_NOT_TEMPLATE(i,n,typed_x,incX)
            break;
        }
        case rindow_matlib_dtype_uint16: {
            uint16_t *typed_x=x;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_NOT_TEMPLATE(i,n,typed_x,incX)
            break;
        }
        case rindow_matlib_dtype_int32: {
            int32_t *typed_x=x;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_NOT_TEMPLATE(i,n,typed_x,incX)
            break;
        }
        case rindow_matlib_dtype_uint32: {
            uint32_t *typed_x=x;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_NOT_TEMPLATE(i,n,typed_x,incX)
            break;
        }
        case rindow_matlib_dtype_int64: {
            int64_t *typed_x=x;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_NOT_TEMPLATE(i,n,typed_x,incX)
            break;
        }
        case rindow_matlib_dtype_uint64: {
            uint64_t *typed_x=x;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_NOT_TEMPLATE(i,n,typed_x,incX)
            break;
        }
        default: {
            break;
        }
    }
}
