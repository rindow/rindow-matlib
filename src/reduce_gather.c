#include "rindow/matlib.h"
#include "common.h"
#include <math.h>
#include <string.h>

int32_t rindow_matlib_s_reducegather(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t numClass,
    int32_t dtype,
    void *x,
    float *a,
    float *b
)
{
    int32_t errcode = 0;
    int64_t checktype;

    if(rindow_matlib_common_get_integer(dtype, x, 1, 0, &checktype)) {
        return RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
    }

    int32_t ldX = n;
    int32_t ldA = n*numClass;
    int32_t ldB = n;

    int32_t i;
    #pragma omp parallel for
    for(i=0; i<m; i++) {
        for(int32_t j=0; j<n; j++) {
            int64_t selector;
            rindow_matlib_common_get_integer(dtype, x, 1, i*ldX+j, &selector);
            if(selector<0||selector>=numClass) {
                errcode = RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
                continue;
            }
            float *from = &(a[i*ldA+j+selector*ldB]);
            float *to = &(b[i*ldB+j]);
            if(reverse) {
                float *tmp;
                tmp = from;
                from = to;
                to = tmp;
            }
            if(addMode) {
                *to += *from;
            } else {
                *to = *from;
            }
        }
    }
    return errcode;
}

int32_t rindow_matlib_d_reducegather(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t numClass,
    int32_t dtype,
    void *x,
    double *a,
    double *b
)
{
    int32_t errcode = 0;
    int64_t checktype;

    if(rindow_matlib_common_get_integer(dtype, x, 1, 0, &checktype)) {
        return RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
    }

    int32_t ldX = n;
    int32_t ldA = n*numClass;
    int32_t ldB = n;

    int32_t i;
    #pragma omp parallel for
    for(i=0; i<m; i++) {
        for(int32_t j=0; j<n; j++) {
            int64_t selector;
            rindow_matlib_common_get_integer(dtype, x, 1, i*ldX+j, &selector);
            if(selector<0||selector>=numClass) {
                errcode = RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
                continue;
            }
            double *from = &(a[i*ldA+j+selector*ldB]);
            double *to = &(b[i*ldB+j]);
            if(reverse) {
                double *tmp;
                tmp = from;
                from = to;
                to = tmp;
            }
            if(addMode) {
                *to += *from;
            } else {
                *to = *from;
            }
        }
    }
    return errcode;
}


int32_t rindow_matlib_i_reducegather(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t numClass,
    int32_t dtype,
    void *x,
    int32_t data_dtype,
    void *a,
    void *b
)
{
    int32_t errcode = 0;
    int64_t checktype;

    if(rindow_matlib_common_get_integer(dtype, x, 1, 0, &checktype)) {
        return RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
    }
    if(!rindow_matlib_common_dtype_is_int(data_dtype) &&
        !rindow_matlib_common_dtype_is_bool(data_dtype)) {
        return RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
    }
    int32_t value_width = rindow_matlib_common_dtype_to_valuesize(data_dtype);
    if(value_width==0) {
        return RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
    }
    char *ax = a;
    char *bx = b;

    int32_t ldX = n;
    int32_t ldA = n*numClass;
    int32_t ldB = n;

    int32_t i;
    #pragma omp parallel for
    for(i=0; i<m; i++) {
        for(int32_t j=0; j<n; j++) {
            int64_t selector;
            rindow_matlib_common_get_integer(dtype, x, 1, i*ldX+j, &selector);
            if(selector<0||selector>=numClass) {
                errcode = RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
                continue;
            }
            char *from = &(ax[(i*ldA+j+selector*ldB)*value_width]);
            char *to = &(bx[(i*ldB+j)*value_width]);
            if(reverse) {
                char *tmp;
                tmp = from;
                from = to;
                to = tmp;
            }
            if(addMode) {
                rindow_matlib_common_i_add(1, data_dtype, from, to);
            } else {
                memcpy(to, from, value_width);
            }
        }
    }
    return errcode;
}

