#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

float rindow_matlib_s_sum(int32_t n,float *x, int32_t incX)
{
    float sum = 0;
//#ifdef _OPENMP
    int32_t i;
    #pragma omp parallel for reduction(+:sum)
    for(i=0; i<n; i++) {
        sum += x[i*incX];
    }
    return sum;
}

double rindow_matlib_d_sum(int32_t n,double *x, int32_t incX)
{
    double sum = 0;
    int32_t i;
    #pragma omp parallel for reduction(+:sum)
    for(i=0; i<n; i++) {
        sum += x[i*incX];
    }
    return sum;
}



#define RINDOW_MATLIB_SUM_TEMPLATE(data_type) \
    for(i=0; i<n; i++) {\
        sum += pDataX[i*incX];\
    }

int64_t rindow_matlib_i_sum(int32_t dtype, int32_t n,void *x, int32_t incX)
{
    int32_t i;
    int64_t result_sum;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            int8_t sum=0;
            int8_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(int8_t)
            result_sum = (int64_t)sum;
            break;
        }
        case rindow_matlib_dtype_uint8: {
            uint8_t sum=0;
            uint8_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(uint8_t)
            result_sum = (int64_t)sum;
            break;
        }
        case rindow_matlib_dtype_int16: {
            int16_t sum=0;
            int16_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(int16_t)
            result_sum = (int64_t)sum;
            break;
        }
        case rindow_matlib_dtype_uint16: {
            uint16_t sum=0;
            uint16_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(uint16_t)
            result_sum = (int64_t)sum;
            break;
        }
        case rindow_matlib_dtype_int32: {
            int32_t sum=0;
            int32_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(int32_t)
            result_sum = (int64_t)sum;
            break;
        }
        case rindow_matlib_dtype_uint32: {
            uint32_t sum=0;
            uint32_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(uint32_t)
            result_sum = (int64_t)sum;
            break;
        }
        case rindow_matlib_dtype_int64: {
            int64_t sum=0;
            int64_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(int64_t)
            result_sum = (int64_t)sum;
            break;
        }
        case rindow_matlib_dtype_uint64: {
            uint64_t sum=0;
            uint64_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(uint64_t)
            result_sum = (int64_t)sum;
            break;
        }
        case rindow_matlib_dtype_bool: {
                uint32_t sum=0;
                uint8_t *pDataX=x;
                #pragma omp parallel for reduction(+:sum)
                for (i=0; i<n; i++) {
                    if(pDataX[i*incX]!=0) {
                        sum++;
                    }
                }
            result_sum = (int64_t)sum;
            break;
        }
    }
    return result_sum;
}
