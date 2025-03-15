#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T,typename IDX>
class DoReduceArgmax
{
private:
    static IDX argMax(
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        int32_t idx = 0;
        T a = x[0];
        for(int32_t i=1;i<n;i++) {
            if(a<x[i*incX]) {
                idx = i;
                a = x[i*incX];
            }
        }
        return idx;
    }

    static void kernel(
        ParallelOperation::cellInfo cell,
        bool para_m,
        int32_t m,
        int32_t n,
        int32_t k,
        T *a,
        IDX *b
    )
    {
        int32_t begin_i,end_i,begin_j,end_j;
        if(para_m) {
            begin_i=cell.begin;
            end_i=cell.end;
            begin_j=0;
            end_j=k;
        } else {
            begin_i=0;
            end_i=m;
            begin_j=cell.begin;
            end_j=cell.end;
        }
        int32_t ldA = n*k;
        int32_t ldB = k;
        for(int32_t i=begin_i; i<end_i; i++) {
            for(int32_t j=begin_j; j<end_j; j++) {
                b[i*ldB+j] = argMax(n,&a[i*ldA+j],k);
            }
        }
    }

public:
    static void doExecute(
        int32_t m,
        int32_t n,
        int32_t k,
        T *a,
        IDX *b
        )
    {
        if(m <= 0 || n <= 0 || k <= 0) {
            return;
        }
        int32_t parallel;
        bool para_m;
        if(m>k) {
            parallel = m;
            para_m = true;
        } else {
            parallel = k;
            para_m = false;
        }

        ParallelOperation::execute(parallel,kernel,para_m,m,n,k,a,b);
    }
};

template <typename T>
class ReduceArgmax
{
public:
    static void execute(
        int32_t m,
        int32_t n,
        int32_t k,
        T *a,
        int32_t dtype,
        void *b
    )
    {
        switch(dtype) {
            case rindow_matlib_dtype_int8: {
                DoReduceArgmax<T,int8_t>::doExecute(m, n, k, a, (int8_t*)b);
                break;
            }
            case rindow_matlib_dtype_uint8: {
                DoReduceArgmax<T,uint8_t>::doExecute(m, n, k, a, (uint8_t*)b);
                break;
            }
            case rindow_matlib_dtype_int16: {
                DoReduceArgmax<T,int16_t>::doExecute(m, n, k, a, (int16_t*)b);
                break;
            }
            case rindow_matlib_dtype_uint16: {
                DoReduceArgmax<T,uint16_t>::doExecute(m, n, k, a, (uint16_t*)b);
                break;
            }
            case rindow_matlib_dtype_int32: {
                DoReduceArgmax<T,int32_t>::doExecute(m, n, k, a, (int32_t*)b);
                break;
            }
            case rindow_matlib_dtype_uint32: {
                DoReduceArgmax<T,uint32_t>::doExecute(m, n, k, a, (uint32_t*)b);
                break;
            }
            case rindow_matlib_dtype_int64: {
                DoReduceArgmax<T,int64_t>::doExecute(m, n, k, a, (int64_t*)b);
                break;
            }
            case rindow_matlib_dtype_uint64: {
                DoReduceArgmax<T,uint64_t>::doExecute(m, n, k, a, (uint64_t*)b);
                break;
            }
            //case rindow_matlib_dtype_bool: {
            //    DoReduceArgmax<T,uint8_t>::doExecute(m, n, k, a, (uint8_t*)b);
            //    break;
            //}
        }
    }
};
}

extern "C" {
void rindow_matlib_s_reduceargmax(
    int32_t m,
    int32_t n,
    int32_t k,
    float *a,
    int32_t dtype,
    void *b
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ReduceArgmax<float>::execute(m, n, k, a, dtype, b);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_reduceargmax(
    int32_t m,
    int32_t n,
    int32_t k,
    double *a,
    int32_t dtype,
    void *b
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ReduceArgmax<double>::execute(m, n, k, a, dtype, b);
    RINDOW_END_CLEAR_EXCEPTION;
}
}
