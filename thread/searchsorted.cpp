#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T, typename IDX>
class DoSearchSorted
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t m,
        int32_t n,
        T *a, int32_t ldA,
        T *x, int32_t incX,
        int32_t right,
        IDX *y, int32_t incY
    )
    {
        for(int32_t i=cell.begin; i<cell.end; i++) {
            T value = x[i*incX];
            int32_t j;
            if(right) {
                for(j=0;j<n;j++) {
                    if(!(value>=a[i*ldA+j])) {
                        break;
                    }
                }
            } else {
                for(j=0;j<n;j++) {
                    if(!(value>a[i*ldA+j])) {
                        break;
                    }
                }
            }
            y[i*incY] = j;
        }
    }

public:
    static void doExecute(
        int32_t m,
        int32_t n,
        T *a, int32_t ldA,
        T *x, int32_t incX,
        int32_t right,
        IDX *y, int32_t incY
        )
    {
        if(m <= 0 || n <= 0) {
            return;
        }
        ParallelOperation::execute(m,kernel,m,n,a,ldA,x,incX,right,y,incY);
    }
};

template <typename T>
class SearchSorted
{
public:
    static void execute(
        int32_t m,
        int32_t n,
        T *a, int32_t ldA,
        T *x, int32_t incX,
        int32_t right,
        int32_t dtype,
        void *y, int32_t incY
        )
    {
        switch(dtype) {
            case rindow_matlib_dtype_int8: {
                DoSearchSorted<T,int8_t>::doExecute(m, n, a, ldA, x, incX, right, (int8_t*)y, incY);
                break;
            }
            case rindow_matlib_dtype_uint8: {
                DoSearchSorted<T,uint8_t>::doExecute(m, n, a, ldA, x, incX, right, (uint8_t*)y, incY);
                break;
            }
            case rindow_matlib_dtype_int16: {
                DoSearchSorted<T,int16_t>::doExecute(m, n, a, ldA, x, incX, right, (int16_t*)y, incY);
                break;
            }
            case rindow_matlib_dtype_uint16: {
                DoSearchSorted<T,uint16_t>::doExecute(m, n, a, ldA, x, incX, right, (uint16_t*)y, incY);
                break;
            }
            case rindow_matlib_dtype_int32: {
                DoSearchSorted<T,int32_t>::doExecute(m, n, a, ldA, x, incX, right, (int32_t*)y, incY);
                break;
            }
            case rindow_matlib_dtype_uint32: {
                DoSearchSorted<T,uint32_t>::doExecute(m, n, a, ldA, x, incX, right, (uint32_t*)y, incY);
                break;
            }
            case rindow_matlib_dtype_int64: {
                DoSearchSorted<T,int64_t>::doExecute(m, n, a, ldA, x, incX, right, (int64_t*)y, incY);
                break;
            }
            case rindow_matlib_dtype_uint64: {
                DoSearchSorted<T,uint64_t>::doExecute(m, n, a, ldA, x, incX, right, (uint64_t*)y, incY);
                break;
            }
            //case rindow_matlib_dtype_bool: {
            //    DoSearchSorted<T,uint8_t>::doExecute(m, n, a, ldA, x, incX, right, (uint8_t*)y, incY);
            //    break;
            //}
        }
    }
};
}

extern "C" {
void rindow_matlib_s_searchsorted(
    int32_t m,
    int32_t n,
    float *a, int32_t ldA,
    float *x, int32_t incX,
    int32_t right,
    int32_t dtype, void *y, int32_t incY
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    SearchSorted<float>::execute(m, n, a, ldA, x, incX, right, dtype, y, incY);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_searchsorted(
    int32_t m,
    int32_t n,
    double *a, int32_t ldA,
    double *x, int32_t incX,
    int32_t right,
    int32_t dtype, void *y, int32_t incY
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    SearchSorted<double>::execute(m, n, a, ldA, x, incX, right, dtype, y, incY);
    RINDOW_END_CLEAR_EXCEPTION;
}

}
