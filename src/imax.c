#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

int32_t rindow_matlib_s_imax(int32_t n,float *x, int32_t incX)
{
    int32_t resultIdx = 0;
    float resultMax = x[0];
    #pragma omp parallel
    {
        int32_t i;
        float maxVal=x[0];
        int32_t maxIdx=0;
        #pragma omp for
        for(i=1; i<n; i++) {
            if(maxVal<x[i*incX] || isnan(maxVal)) {
                maxVal = x[i*incX];
                maxIdx = i;
            }
        }
        #pragma omp critical
        {
            if(resultMax<maxVal || isnan(resultMax)) {
                resultMax = maxVal;
                resultIdx = maxIdx;
            }
        }
    }
    return resultIdx;
}

int32_t rindow_matlib_d_imax(int32_t n,double *x, int32_t incX)
{
    int32_t resultIdx = 0;
    double resultMax = x[0];
    #pragma omp parallel
    {
        int32_t i;
        double maxVal=x[0];
        int32_t maxIdx=0;
        #pragma omp for
        for(i=1; i<n; i++) {
            if(maxVal<x[i*incX] || isnan(maxVal)) {
                maxVal = x[i*incX];
                maxIdx = i;
            }
        }
        #pragma omp critical
        {
            if(resultMax<maxVal || isnan(resultMax)) {
                resultMax = maxVal;
                resultIdx = maxIdx;
            }
        }
    }
    return resultIdx;
}

#define RINDOW_MATLIB_IMAX_TEMPLATE1(data_type) \
    data_type *pDataX=x; \
    data_type resultMax = pDataX[0];

#define RINDOW_MATLIB_IMAX_TEMPLATE2(data_type) \
    int32_t i; \
    data_type maxVal=pDataX[0]; \
    int32_t maxIdx=0;

#define RINDOW_MATLIB_IMAX_TEMPLATE3(data_type) \
    for(i=1; i<n; i++) { \
        if(maxVal<pDataX[i*incX]) { \
            maxVal = pDataX[i*incX]; \
            maxIdx = i; \
        } \
    }

#define RINDOW_MATLIB_IMAX_TEMPLATE4(data_type) \
    { \
        if(resultMax<maxVal) { \
            resultMax = maxVal; \
            resultIdx = maxIdx; \
        } \
    }

int32_t rindow_matlib_i_imax(int32_t dtype, int32_t n,void *x, int32_t incX)
{
    int32_t resultIdx = 0;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            RINDOW_MATLIB_IMAX_TEMPLATE1(int8_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMAX_TEMPLATE2(int8_t)
                #pragma omp for
                RINDOW_MATLIB_IMAX_TEMPLATE3(int8_t)
                #pragma omp critical
                RINDOW_MATLIB_IMAX_TEMPLATE4(int8_t)
            }
            break;
        }
        case rindow_matlib_dtype_uint8: {
            RINDOW_MATLIB_IMAX_TEMPLATE1(uint8_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMAX_TEMPLATE2(uint8_t)
                #pragma omp for
                RINDOW_MATLIB_IMAX_TEMPLATE3(uint8_t)
                #pragma omp critical
                RINDOW_MATLIB_IMAX_TEMPLATE4(uint8_t)
            }
            break;
        }
        case rindow_matlib_dtype_int16: {
            RINDOW_MATLIB_IMAX_TEMPLATE1(int16_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMAX_TEMPLATE2(int16_t)
                #pragma omp for
                RINDOW_MATLIB_IMAX_TEMPLATE3(int16_t)
                #pragma omp critical
                RINDOW_MATLIB_IMAX_TEMPLATE4(int16_t)
            }
            break;
        }
        case rindow_matlib_dtype_uint16: {
            RINDOW_MATLIB_IMAX_TEMPLATE1(uint16_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMAX_TEMPLATE2(uint16_t)
                #pragma omp for
                RINDOW_MATLIB_IMAX_TEMPLATE3(uint16_t)
                #pragma omp critical
                RINDOW_MATLIB_IMAX_TEMPLATE4(uint16_t)
            }
            break;
        }
        case rindow_matlib_dtype_int32: {
            RINDOW_MATLIB_IMAX_TEMPLATE1(int32_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMAX_TEMPLATE2(int32_t)
                #pragma omp for
                RINDOW_MATLIB_IMAX_TEMPLATE3(int32_t)
                #pragma omp critical
                RINDOW_MATLIB_IMAX_TEMPLATE4(int32_t)
            }
            break;
        }
        case rindow_matlib_dtype_uint32: {
            RINDOW_MATLIB_IMAX_TEMPLATE1(uint32_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMAX_TEMPLATE2(uint32_t)
                #pragma omp for
                RINDOW_MATLIB_IMAX_TEMPLATE3(uint32_t)
                #pragma omp critical
                RINDOW_MATLIB_IMAX_TEMPLATE4(uint32_t)
            }
            break;
        }
        case rindow_matlib_dtype_int64: {
            RINDOW_MATLIB_IMAX_TEMPLATE1(int64_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMAX_TEMPLATE2(int64_t)
                #pragma omp for
                RINDOW_MATLIB_IMAX_TEMPLATE3(int64_t)
                #pragma omp critical
                RINDOW_MATLIB_IMAX_TEMPLATE4(int64_t)
            }
            break;
        }
        case rindow_matlib_dtype_uint64: {
            RINDOW_MATLIB_IMAX_TEMPLATE1(uint64_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMAX_TEMPLATE2(uint64_t)
                #pragma omp for
                RINDOW_MATLIB_IMAX_TEMPLATE3(uint64_t)
                #pragma omp critical
                RINDOW_MATLIB_IMAX_TEMPLATE4(uint64_t)
            }
            break;
        }
    }
    return resultIdx;
}
