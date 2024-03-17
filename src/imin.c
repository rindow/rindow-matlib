#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

int32_t rindow_matlib_s_imin(int32_t n,float *x, int32_t incX)
{
    int32_t resultIdx = 0;
    float resultMin = x[0];
    #pragma omp parallel
    {
        int32_t i;
        float minVal=x[0];
        int32_t minIdx=0;
        #pragma omp for
        for(i=0; i<n; i++) {
            if(minVal>x[i*incX] || isnan(minVal)) {
                minVal = x[i*incX];
                minIdx = i;
            }
        }
        #pragma omp critical
        {
            if(resultMin>minVal || isnan(resultMin)) {
                resultMin = minVal;
                resultIdx = minIdx;
            }
        }
    }
    return resultIdx;
}

int32_t rindow_matlib_d_imin(int32_t n,double *x, int32_t incX)
{
    int32_t resultIdx = 0;
    double resultMin = x[0];
    #pragma omp parallel
    {
        int32_t i;
        double minVal=x[0];
        int32_t minIdx=0;
        #pragma omp for
        for(i=1; i<n; i++) {
            if(minVal>x[i*incX] || isnan(minVal)) {
                minVal = x[i*incX];
                minIdx = i;
            }
        }
        #pragma omp critical
        {
            if(resultMin>minVal || isnan(resultMin)) {
                resultMin = minVal;
                resultIdx = minIdx;
            }
        }
    }
    return resultIdx;
}

#define RINDOW_MATLIB_IMIN_TEMPLATE1(data_type) \
    data_type *pDataX=x; \
    data_type resultMin = pDataX[0];

#define RINDOW_MATLIB_IMIN_TEMPLATE2(data_type) \
    int32_t i; \
    data_type minVal=pDataX[0]; \
    int32_t minIdx=0;

#define RINDOW_MATLIB_IMIN_TEMPLATE3(data_type) \
    for(i=1; i<n; i++) { \
        if(minVal>pDataX[i*incX]) { \
            minVal = pDataX[i*incX]; \
            minIdx = i; \
        } \
    }

#define RINDOW_MATLIB_IMIN_TEMPLATE4(data_type) \
    { \
        if(resultMin>minVal) { \
            resultMin = minVal; \
            resultIdx = minIdx; \
        } \
    }

int32_t rindow_matlib_i_imin(int32_t dtype, int32_t n,void *x, int32_t incX)
{
    int32_t resultIdx = 0;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            RINDOW_MATLIB_IMIN_TEMPLATE1(int8_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMIN_TEMPLATE2(int8_t)
                #pragma omp for
                RINDOW_MATLIB_IMIN_TEMPLATE3(int8_t)
                #pragma omp critical
                RINDOW_MATLIB_IMIN_TEMPLATE4(int8_t)
            }
            break;
        }
        case rindow_matlib_dtype_uint8: {
            RINDOW_MATLIB_IMIN_TEMPLATE1(uint8_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMIN_TEMPLATE2(uint8_t)
                #pragma omp for
                RINDOW_MATLIB_IMIN_TEMPLATE3(uint8_t)
                #pragma omp critical
                RINDOW_MATLIB_IMIN_TEMPLATE4(uint8_t)
            }
            break;
        }
        case rindow_matlib_dtype_int16: {
            RINDOW_MATLIB_IMIN_TEMPLATE1(int16_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMIN_TEMPLATE2(int16_t)
                #pragma omp for
                RINDOW_MATLIB_IMIN_TEMPLATE3(int16_t)
                #pragma omp critical
                RINDOW_MATLIB_IMIN_TEMPLATE4(int16_t)
            }
            break;
        }
        case rindow_matlib_dtype_uint16: {
            RINDOW_MATLIB_IMIN_TEMPLATE1(uint16_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMIN_TEMPLATE2(uint16_t)
                #pragma omp for
                RINDOW_MATLIB_IMIN_TEMPLATE3(uint16_t)
                #pragma omp critical
                RINDOW_MATLIB_IMIN_TEMPLATE4(uint16_t)
            }
            break;
        }
        case rindow_matlib_dtype_int32: {
            RINDOW_MATLIB_IMIN_TEMPLATE1(int32_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMIN_TEMPLATE2(int32_t)
                #pragma omp for
                RINDOW_MATLIB_IMIN_TEMPLATE3(int32_t)
                #pragma omp critical
                RINDOW_MATLIB_IMIN_TEMPLATE4(int32_t)
            }
            break;
        }
        case rindow_matlib_dtype_uint32: {
            RINDOW_MATLIB_IMIN_TEMPLATE1(uint32_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMIN_TEMPLATE2(uint32_t)
                #pragma omp for
                RINDOW_MATLIB_IMIN_TEMPLATE3(uint32_t)
                #pragma omp critical
                RINDOW_MATLIB_IMIN_TEMPLATE4(uint32_t)
            }
            break;
        }
        case rindow_matlib_dtype_int64: {
            RINDOW_MATLIB_IMIN_TEMPLATE1(int64_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMIN_TEMPLATE2(int64_t)
                #pragma omp for
                RINDOW_MATLIB_IMIN_TEMPLATE3(int64_t)
                #pragma omp critical
                RINDOW_MATLIB_IMIN_TEMPLATE4(int64_t)
            }
            break;
        }
        case rindow_matlib_dtype_uint64: {
            RINDOW_MATLIB_IMIN_TEMPLATE1(uint64_t)
            #pragma omp parallel
            {
                RINDOW_MATLIB_IMIN_TEMPLATE2(uint64_t)
                #pragma omp for
                RINDOW_MATLIB_IMIN_TEMPLATE3(uint64_t)
                #pragma omp critical
                RINDOW_MATLIB_IMIN_TEMPLATE4(uint64_t)
            }
            break;
        }
    }
    return resultIdx;
}
