#include "rindow/matlib.h"
#include "common.hpp"
#include <string.h>
#include <complex>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Zeros
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        if(incX==1) {
            int32_t valueSize = sizeof(T);
            memset(&(x[cell.begin]),0,(cell.end-cell.begin)*valueSize);
        } else {
            for(int32_t i = cell.begin; i < cell.end; i++) {
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
void rindow_matlib_s_zeros(int32_t n, float *x, int32_t incX)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Zeros<float>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_zeros(int32_t n, double *x, int32_t incX)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Zeros<double>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_i_zeros(int32_t dtype, int32_t n,void *x, int32_t incX)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            Zeros<int8_t>::execute(n, (int8_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            Zeros<uint8_t>::execute(n, (uint8_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int16: {
            Zeros<int16_t>::execute(n, (int16_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            Zeros<uint16_t>::execute(n, (uint16_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int32: {
            Zeros<int32_t>::execute(n, (int32_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            Zeros<uint32_t>::execute(n, (uint32_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int64: {
            Zeros<int64_t>::execute(n, (int64_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            Zeros<uint64_t>::execute(n, (uint64_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_bool: {
            Zeros<uint8_t>::execute(n, (uint8_t *)x, incX);
            break;
        }
        case rindow_matlib_dtype_complex64: {
            Zeros<std::complex<float>>::execute(n, (std::complex<float> *)x, incX);
            break;
        }
        case rindow_matlib_dtype_complex128: {
            Zeros<std::complex<double>>::execute(n, (std::complex<double> *)x, incX);
            break;
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
}

}
