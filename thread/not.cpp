#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Not
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        for(int32_t i = cell.begin; i < cell.end; i++) {
            if(x[i*incX] == 0) {
                x[i*incX] = 1;
            } else {
                x[i*incX] = 0;
            }
        }
    }

public:
    static void execute(int32_t n, T *x, int32_t incX)
    {
        if(n <= 0) {
            return;
        }

        ParallelOperation::execute(n,kernel,n,x,incX);
    }
};

}

extern "C" {
void rindow_matlib_s_not(
    int32_t n, float *x, int32_t incX)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Not<float>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_not(
    int32_t n, double *x, int32_t incX)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Not<double>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_i_not(
    int32_t dtype, int32_t n, void *x, int32_t incX)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            Not<int8_t>::execute(n, (int8_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            Not<uint8_t>::execute(n, (uint8_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int16: {
            Not<int16_t>::execute(n, (int16_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            Not<uint16_t>::execute(n, (uint16_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int32: {
            Not<int32_t>::execute(n, (int32_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            Not<uint32_t>::execute(n, (uint32_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int64: {
            Not<int64_t>::execute(n, (int64_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            Not<uint64_t>::execute(n, (uint64_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_bool: {
            Not<uint8_t>::execute(n, (uint8_t *)x, incX);
            break;
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
}

}
