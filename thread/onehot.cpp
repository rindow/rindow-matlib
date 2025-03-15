#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T,typename IDX>
class DoOnehot
{
private:
    static int32_t kernel(
        ParallelOperation::cellInfo cell,
        int32_t m,
        int32_t n,
        IDX *x,
        int32_t incX,
        T alpha,
        T *a,
        int32_t ldA
    )
    {
        int32_t errcode = 0;
        for(int32_t i=cell.begin; i<cell.end; i++) {
            int32_t selector = (int32_t)x[i*incX];
            if(selector>=0 && selector<n) {
                a[i*ldA+selector] += alpha;
            } else {
                errcode = RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
            }
        }
        return errcode;
    }

public:
    static int32_t doExecute(
        int32_t m,
        int32_t n,
        IDX *x,
        int32_t incX,
        T alpha,
        T *a,
        int32_t ldA
    )
    {
        if(n <= 0 || m <= 0 ) {
            return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
        }

        return ParallelOperation::executeAndGetCode<int32_t>(m,kernel,m,n,x,incX,alpha,a,ldA);
    }
};

template <typename T>
class Onehot
{
public:
    static int32_t execute(
        int32_t dtype,
        int32_t m,
        int32_t n,
        void *x,
        int32_t incX,
        T alpha,
        T *a,
        int32_t ldA
    )
    {
        int32_t errcode = 0;
        switch(dtype) {
            case rindow_matlib_dtype_int8: {
                errcode = DoOnehot<T,int8_t>::doExecute(m, n, (int8_t*)x, incX, alpha, a, ldA);
                break;
            }
            case rindow_matlib_dtype_uint8: {
                errcode = DoOnehot<T,uint8_t>::doExecute(m, n, (uint8_t*)x, incX, alpha, a, ldA);
                break;
            }
            case rindow_matlib_dtype_int16: {
                errcode = DoOnehot<T,int16_t>::doExecute(m, n, (int16_t*)x, incX, alpha, a, ldA);
                break;
            }
            case rindow_matlib_dtype_uint16: {
                errcode = DoOnehot<T,uint16_t>::doExecute(m, n, (uint16_t*)x, incX, alpha, a, ldA);
                break;
            }
            case rindow_matlib_dtype_int32: {
                errcode = DoOnehot<T,int32_t>::doExecute(m, n, (int32_t*)x, incX, alpha, a, ldA);
                break;
            }
            case rindow_matlib_dtype_uint32: {
                errcode = DoOnehot<T,uint32_t>::doExecute(m, n, (uint32_t*)x, incX, alpha, a, ldA);
                break;
            }
            case rindow_matlib_dtype_int64: {
                errcode = DoOnehot<T,int64_t>::doExecute(m, n, (int64_t*)x, incX, alpha, a, ldA);
                break;
            }
            case rindow_matlib_dtype_uint64: {
                errcode = DoOnehot<T,uint64_t>::doExecute(m, n, (uint64_t*)x, incX, alpha, a, ldA);
                break;
            }
            case rindow_matlib_dtype_bool: {
                errcode = DoOnehot<T,uint8_t>::doExecute(m, n, (uint8_t*)x, incX, alpha, a, ldA);
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
int32_t rindow_matlib_s_onehot(
    int32_t dtype, int32_t m, int32_t n, void *x, int32_t incX, float alpha, float *a, int32_t ldA)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Onehot<float>::execute(dtype, m, n, x, incX, alpha, a, ldA);
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_d_onehot(
    int32_t dtype, int32_t m, int32_t n, void *x, int32_t incX, double alpha, double *a, int32_t ldA)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Onehot<double>::execute(dtype, m, n, x, incX, alpha, a, ldA);
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}
}

