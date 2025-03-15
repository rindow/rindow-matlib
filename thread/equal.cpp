#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Equal
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX,
        T *y,
        int32_t incY
    )
    {
        for(int32_t i = cell.begin; i < cell.end; i++) {
            if(x[i * incX]==y[i * incY]) {
                y[i * incY] = 1;
            } else {
                y[i * incY] = 0;
            }
        }
    }

    static void bool_kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX,
        T *y,
        int32_t incY
    )
    {
        for(int32_t i = cell.begin; i < cell.end; i++) {
            if((x[i*incX] && y[i*incY]) ||      // 1 == 1
                (!x[i*incX] && !y[i*incY])   // 0 == 0
            ) {
                y[i * incY] = 1;
            } else {
                y[i * incY] = 0;
            }
        }
    }

public:
    static void execute(int32_t n, T *x, int32_t incX, T *y, int32_t incY)
    {
        if(n <= 0) {
            return;
        }

        ParallelOperation::execute(n,kernel,n,x,incX,y,incY);
    }

    static void execute_bool(int32_t n, T *x, int32_t incX, T *y, int32_t incY)
    {
        if(n <= 0) {
            return;
        }

        ParallelOperation::execute(n,bool_kernel,n,x,incX,y,incY);
    }
};

}

extern "C" {
void rindow_matlib_s_equal(
    int32_t n, float *x, int32_t incX, float *y, int32_t incY)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Equal<float>::execute(n, x, incX, y, incY);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_equal(
    int32_t n, double *x, int32_t incX, double *y, int32_t incY)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Equal<double>::execute(n, x, incX, y, incY);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_i_equal(
    int32_t dtype, int32_t n,void *x, int32_t incX,void *y, int32_t incY)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            Equal<int8_t>::execute(n, (int8_t*)x, incX, (int8_t*)y, incY);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            Equal<uint8_t>::execute(n, (uint8_t*)x, incX, (uint8_t*)y, incY);
            break;
        }
        case rindow_matlib_dtype_int16: {
            Equal<int16_t>::execute(n, (int16_t*)x, incX, (int16_t*)y, incY);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            Equal<uint16_t>::execute(n, (uint16_t*)x, incX, (uint16_t*)y, incY);
            break;
        }
        case rindow_matlib_dtype_int32: {
            Equal<int32_t>::execute(n, (int32_t*)x, incX, (int32_t*)y, incY);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            Equal<uint32_t>::execute(n, (uint32_t*)x, incX, (uint32_t*)y, incY);
            break;
        }
        case rindow_matlib_dtype_int64: {
            Equal<int64_t>::execute(n, (int64_t*)x, incX, (int64_t*)y, incY);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            Equal<uint64_t>::execute(n, (uint64_t*)x, incX, (uint64_t*)y, incY);
            break;
        }
        case rindow_matlib_dtype_bool: {
            Equal<uint8_t>::execute_bool(n, (uint8_t *)x, incX, (uint8_t*)y, incY);
            break;
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
}

}
