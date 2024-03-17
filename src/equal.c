#include "rindow/matlib.h"
#include "common.h"

#define RINDOW_MATLIB_EQUAL_TEMPLATE(i,n,x,incX,y,incY) \
    for(i=0; i<n; i++) { \
        if(x[i*incX] == y[i*incY]) \
            y[i*incY] = 1; \
        else \
            y[i*incY] = 0; \
    }

void rindow_matlib_s_equal(
    int32_t n, float *x, int32_t incX, float *y, int32_t incY)
{
    int32_t i;
    #pragma omp parallel for
    RINDOW_MATLIB_EQUAL_TEMPLATE(i,n,x,incX,y,incY)
}

void rindow_matlib_d_equal(
    int32_t n, double *x, int32_t incX, double *y, int32_t incY)
{
    int32_t i;
    #pragma omp parallel for
    RINDOW_MATLIB_EQUAL_TEMPLATE(i,n,x,incX,y,incY)
}


void rindow_matlib_i_equal(
    int32_t dtype, int32_t n, void *x, int32_t incX, void *y, int32_t incY)
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
            uint8_t *typed_y=y;
            int32_t i;
            #pragma omp parallel for
            for(i=0;i<n;i++) {
                if((typed_x[i*incX] && typed_y[i*incY]) ||  // 1 == 1
                    (!typed_x[i*incX] && !typed_y[i*incY])  // 0 == 0
                ) {
                    typed_y[i*incY] = 1;
                } else {
                    typed_y[i*incY] = 0;
                }
            }
            break;
        }
        case rindow_matlib_dtype_int8: {
            int8_t *typed_x=x;
            int8_t *typed_y=y;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_EQUAL_TEMPLATE(i,n,typed_x,incX,typed_y,incY)
            break;
        }
        case rindow_matlib_dtype_uint8: {
            uint8_t *typed_x=x;
            uint8_t *typed_y=y;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_EQUAL_TEMPLATE(i,n,typed_x,incX,typed_y,incY)
            break;
        }
        case rindow_matlib_dtype_int16: {
            int16_t *typed_x=x;
            int16_t *typed_y=y;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_EQUAL_TEMPLATE(i,n,typed_x,incX,typed_y,incY)
            break;
        }
        case rindow_matlib_dtype_uint16: {
            uint16_t *typed_x=x;
            uint16_t *typed_y=y;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_EQUAL_TEMPLATE(i,n,typed_x,incX,typed_y,incY)
            break;
        }
        case rindow_matlib_dtype_int32: {
            int32_t *typed_x=x;
            int32_t *typed_y=y;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_EQUAL_TEMPLATE(i,n,typed_x,incX,typed_y,incY)
            break;
        }
        case rindow_matlib_dtype_uint32: {
            uint32_t *typed_x=x;
            uint32_t *typed_y=y;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_EQUAL_TEMPLATE(i,n,typed_x,incX,typed_y,incY)
            break;
        }
        case rindow_matlib_dtype_int64: {
            int64_t *typed_x=x;
            int64_t *typed_y=y;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_EQUAL_TEMPLATE(i,n,typed_x,incX,typed_y,incY)
            break;
        }
        case rindow_matlib_dtype_uint64: {
            uint64_t *typed_x=x;
            uint64_t *typed_y=y;
            int32_t i;
            #pragma omp parallel for
            RINDOW_MATLIB_EQUAL_TEMPLATE(i,n,typed_x,incX,typed_y,incY)
            break;
        }
        default: {
            break;
        }
    }
}
