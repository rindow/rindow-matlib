#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T,typename TO>
class DoAstype
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX,
        TO *y,
        int32_t incY
    )
    {
        int32_t errcode = 0;
        for(int32_t i=cell.begin; i<cell.end; i++) {
            y[i] = (TO)x[i];
        }
    }

    static void kernel_bool(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX,
        TO *y,
        int32_t incY
    )
    {
        int32_t errcode = 0;
        for(int32_t i=cell.begin; i<cell.end; i++) {
            y[i] = (x[i] == 0) ? 0 : 1;
        }
    }

public:
    static int32_t doExecute(
        int32_t n,
        T *x,
        int32_t incX,
        TO *y,
        int32_t incY
    )
    {
        if(n <= 0) {
            return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
        }

        ParallelOperation::execute(n,kernel,n,x,incX,y,incY);
        return 0;
    }

    static int32_t doExecute_bool(
        int32_t n,
        T *x,
        int32_t incX,
        TO *y,
        int32_t incY
    )
    {
        if(n <= 0) {
            return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
        }

        ParallelOperation::execute(n,kernel_bool,n,x,incX,y,incY);
        return 0;
    }
};

template <typename T>
class Astype
{
public:
    static int32_t execute(
        int32_t n,
        T *x, int32_t incX,
        int32_t to_dtype,
        void *y, int32_t incY
    )
    {
        int32_t errcode = 0;
        switch(to_dtype) {
            case rindow_matlib_dtype_float32: {
                errcode = DoAstype<T,float>::doExecute(n, x, incX, (float*)y, incY);
                break;
            }
            case rindow_matlib_dtype_float64: {
                errcode = DoAstype<T,double>::doExecute(n, x, incX, (double*)y, incY);
                break;
            }
            case rindow_matlib_dtype_int8: {
                errcode = DoAstype<T,int8_t>::doExecute(n, x, incX, (int8_t*)y, incY);
                break;
            }
            case rindow_matlib_dtype_uint8: {
                errcode = DoAstype<T,uint8_t>::doExecute(n, x, incX, (uint8_t*)y, incY);
                break;
            }
            case rindow_matlib_dtype_int16: {
                errcode = DoAstype<T,int16_t>::doExecute(n, x, incX, (int16_t*)y, incY);
                break;
            }
            case rindow_matlib_dtype_uint16: {
                errcode = DoAstype<T,uint16_t>::doExecute(n, x, incX, (uint16_t*)y, incY);
                break;
            }
            case rindow_matlib_dtype_int32: {
                errcode = DoAstype<T,int32_t>::doExecute(n, x, incX, (int32_t*)y, incY);
                break;
            }
            case rindow_matlib_dtype_uint32: {
                errcode = DoAstype<T,uint32_t>::doExecute(n, x, incX, (uint32_t*)y, incY);
                break;
            }
            case rindow_matlib_dtype_int64: {
                errcode = DoAstype<T,int64_t>::doExecute(n, x, incX, (int64_t*)y, incY);
                break;
            }
            case rindow_matlib_dtype_uint64: {
                errcode = DoAstype<T,uint64_t>::doExecute(n, x, incX, (uint64_t*)y, incY);
                break;
            }
            case rindow_matlib_dtype_bool: {
                errcode = DoAstype<T,uint8_t>::doExecute_bool(n, x, incX, (uint8_t*)y, incY);
                break;
            }
            default: {
                errcode = RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
            }
        }
        return errcode;
    }
};

}

extern "C" {
int32_t rindow_matlib_astype(
    int32_t n, int32_t from_dtype, void *x, int32_t incX, int32_t to_dtype, void *y, int32_t incY)
{
    int32_t errcode;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(from_dtype) {
        case rindow_matlib_dtype_float32: {
            errcode = Astype<float>::execute(n,(float*)x,incX,to_dtype,y,incY);
            break;
        }
        case rindow_matlib_dtype_float64: {
            errcode = Astype<double>::execute(n,(double*)x,incX,to_dtype,y,incY);
            break;
        }
        case rindow_matlib_dtype_int8: {
            errcode = Astype<int8_t>::execute(n,(int8_t*)x,incX,to_dtype,y,incY);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            errcode = Astype<uint8_t>::execute(n,(uint8_t*)x,incX,to_dtype,y,incY);
            break;
        }
        case rindow_matlib_dtype_int16: {
            errcode = Astype<int16_t>::execute(n,(int16_t*)x,incX,to_dtype,y,incY);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            errcode = Astype<uint16_t>::execute(n,(uint16_t*)x,incX,to_dtype,y,incY);
            break;
        }
        case rindow_matlib_dtype_int32: {
            errcode = Astype<int32_t>::execute(n,(int32_t*)x,incX,to_dtype,y,incY);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            errcode = Astype<uint32_t>::execute(n,(uint32_t*)x,incX,to_dtype,y,incY);
            break;
        }
        case rindow_matlib_dtype_int64: {
            errcode = Astype<int64_t>::execute(n,(int64_t*)x,incX,to_dtype,y,incY);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            errcode = Astype<uint64_t>::execute(n,(uint64_t*)x,incX,to_dtype,y,incY);
            break;
        }
        case rindow_matlib_dtype_bool: {
            errcode = Astype<uint8_t>::execute(n,(uint8_t*)x,incX,to_dtype,y,incY);
            break;
        }
        default: {
            errcode = RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

}

