#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Sum
{
public:
    static T kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        T sum = 0;
        for(int32_t i = cell.begin; i < cell.end; i++) {
            sum += x[i * incX];
        }
        return sum;
    }

    static T reduce(
        T initialValue,
        T value
    )
    {
        return initialValue+value;
    }

    static int32_t kernel_bool(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        int32_t sum = 0;
        for(int32_t i = cell.begin; i < cell.end; i++) {
            if(x[i * incX]!=0) {
                sum += 1;
            }
        }
        return sum;
    }

    static int32_t reduce_bool(
        int32_t initialValue,
        int32_t value
    )
    {
        return initialValue+(int32_t)value;
    }

    static T execute(int32_t n, T *x, int32_t incX)
    {
        if(n <= 0) {
            return 0;
        }

        T initialValue = 0;
        return ParallelOperation::reduce(n,reduce,initialValue,kernel,n,x,incX);
    }

    static int32_t execute_bool(int32_t n, T *x, int32_t incX)
    {
        if(n <= 0) {
            return 0;
        }

        int32_t initialValue = 0;
        return ParallelOperation::reduce(n,reduce_bool,initialValue,kernel_bool,n,x,incX);
    }
};

}

extern "C" {
float rindow_matlib_s_sum(int32_t n, float *x, int32_t incX)
{
    float ret;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ret = Sum<float>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}

double rindow_matlib_d_sum(int32_t n, double *x, int32_t incX)
{
    double ret;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ret = Sum<double>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}

int64_t rindow_matlib_i_sum(int32_t dtype, int32_t n,void *x, int32_t incX)
{
    int64_t ret=0;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            ret =(int64_t)Sum<int8_t>::execute(n, (int8_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            ret =(int64_t)Sum<uint8_t>::execute(n, (uint8_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int16: {
            ret =(int64_t)Sum<int16_t>::execute(n, (int16_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            ret =(int64_t)Sum<uint16_t>::execute(n, (uint16_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int32: {
            ret =(int64_t)Sum<int32_t>::execute(n, (int32_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            ret =(int64_t)Sum<uint32_t>::execute(n, (uint32_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int64: {
            ret =(int64_t)Sum<int64_t>::execute(n, (int64_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            ret =(int64_t)Sum<uint64_t>::execute(n, (uint64_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_bool: {
            ret =(int64_t)Sum<uint8_t>::execute_bool(n, (uint8_t *)x, incX);
            break;
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}

}
