#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "rindow/matlib.h"
#include "common.hpp"
#include <limits>
#include <cmath>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
struct _RIMax {
    int32_t index;
    T value;
};


template <typename T>
using RIMax = struct _RIMax<T>;

template <typename T>
class IMax
{
private:
    static RIMax<T> kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        T maxVal=x[0];
        int32_t maxIdx=0;
        for(int32_t i = cell.begin; i < cell.end; i++) {
            if(maxVal<x[i*incX] || std::isnan(maxVal)) {
                maxVal = x[i*incX];
                maxIdx = i;
            }
        }
        RIMax<T> result = {maxIdx,maxVal};
        return result;
    }

    static RIMax<T> compare(
        RIMax<T> initialValue,
        RIMax<T> value
    )
    {
        if(initialValue.index<0) {
            return value;
        }
        if(initialValue.value<value.value || std::isnan(initialValue.value)) {
            return value;
        } else {
            return initialValue;
        }
    }

    static RIMax<T> kernel_int(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        T maxVal=x[0];
        int32_t maxIdx=0;
        for(int32_t i = cell.begin; i < cell.end; i++) {
            if(maxVal<x[i*incX]) {
                maxVal = x[i*incX];
                maxIdx = i;
            }
        }
        RIMax<T> result = {maxIdx,maxVal};
        return result;
    }

    static RIMax<T> compare_int(
        RIMax<T> initialValue,
        RIMax<T> value
    )
    {
        if(initialValue.index<0) {
            return value;
        }
        if(initialValue.value<value.value) {
            return value;
        } else {
            return initialValue;
        }
    }
public:
    static int32_t execute(
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        if(n <= 0) {
            return 0;
        }

        RIMax<T> initialValue = { -1, std::numeric_limits<T>::min() };
        RIMax<T> result = ParallelOperation::reduce(n,compare,initialValue,kernel,n,x,incX);
        return result.index;
    }

    static int32_t execute_int(
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        if(n <= 0) {
            return 0;
        }

        RIMax<T> initialValue = { -1, std::numeric_limits<T>::min() };
        RIMax<T> result = ParallelOperation::reduce(n,compare_int,initialValue,kernel_int,n,x,incX);
        return result.index;
    }

};

}

extern "C" {
int32_t rindow_matlib_s_imax(int32_t n, float *x, int32_t incX)
{
    int32_t ret;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ret = IMax<float>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}

int32_t rindow_matlib_d_imax(int32_t n, double *x, int32_t incX)
{
    int32_t ret;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ret = IMax<double>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}

int32_t rindow_matlib_i_imax(int32_t dtype, int32_t n,void *x, int32_t incX)
{
    int32_t ret=0;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            ret = IMax<int8_t>::execute_int(n, (int8_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            ret = IMax<uint8_t>::execute_int(n, (uint8_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int16: {
            ret = IMax<int16_t>::execute_int(n, (int16_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            ret = IMax<uint16_t>::execute_int(n, (uint16_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int32: {
            ret = IMax<int32_t>::execute_int(n, (int32_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            ret = IMax<uint32_t>::execute_int(n, (uint32_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int64: {
            ret = IMax<int64_t>::execute_int(n, (int64_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            ret = IMax<uint64_t>::execute_int(n, (uint64_t*)x, incX);
            break;
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}
}
