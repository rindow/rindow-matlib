#include "rindow/matlib.h"
#include <math.h>
#include <stdlib.h>
#include "common.h"

#define RINDOW_MATLIB_RANDOM_SEQUENCE_TEMPLATE(value_type) \
        value_type *data = (value_type *)x; \
        for(int32_t i=0;i<n;i++) { \
            data[i*incX] = i; \
        } \
        for(int32_t i=0;i<size;i++) { \
            value_type tmp; \
            int32_t idx; \
            idx = rindow_matlib_i_random_sequence_sub(i,n-1); \
            tmp = data[i*incX]; \
            data[i*incX] = data[idx*incX]; \
            data[idx*incX] = tmp; \
        }

static inline int32_t rindow_matlib_i_random_sequence_sub(
    int32_t low,int32_t high)
{
    if(high-low==RINDOW_MATLIB_COMMON_RAND_MAX) {
        return rindow_matlib_common_rand();
    }
    return (rindow_matlib_common_rand()%(high-low+1))+low;
}

void rindow_matlib_i_randomsequence(
    int32_t n,
    int32_t size,
    int32_t dtype,
    void *x, int32_t incX,
    int32_t seed
)
{
    if(seed!=0) {
        rindow_matlib_common_srand(seed);
    }

    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            RINDOW_MATLIB_RANDOM_SEQUENCE_TEMPLATE(int8_t)
            break;
        }
        case rindow_matlib_dtype_uint8: {
            RINDOW_MATLIB_RANDOM_SEQUENCE_TEMPLATE(uint8_t)
            break;
        }
        case rindow_matlib_dtype_int16: {
            RINDOW_MATLIB_RANDOM_SEQUENCE_TEMPLATE(int16_t)
            break;
        }
        case rindow_matlib_dtype_uint16: {
            RINDOW_MATLIB_RANDOM_SEQUENCE_TEMPLATE(uint16_t)
            break;
        }
        case rindow_matlib_dtype_int32: {
            RINDOW_MATLIB_RANDOM_SEQUENCE_TEMPLATE(int32_t)
            break;
        }
        case rindow_matlib_dtype_uint32: {
            RINDOW_MATLIB_RANDOM_SEQUENCE_TEMPLATE(uint32_t)
            break;
        }
        case rindow_matlib_dtype_int64: {
            RINDOW_MATLIB_RANDOM_SEQUENCE_TEMPLATE(int64_t)
            break;
        }
        case rindow_matlib_dtype_uint64: {
            RINDOW_MATLIB_RANDOM_SEQUENCE_TEMPLATE(uint64_t)
            break;
        }
    }
}

