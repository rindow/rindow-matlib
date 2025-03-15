#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class BandPart
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        bool para_batch,
        int32_t m, int32_t n, int32_t k,
        T *a,
        int32_t lower, int32_t upper
    )
    {
        int32_t begin_batch,end_batch,begin_i,end_i;
        if(para_batch) {
            begin_batch=cell.begin;
            end_batch=cell.end;
            begin_i=0;
            end_i=n;
        } else {
            begin_batch=0;
            end_batch=m;
            begin_i=cell.begin;
            end_i=cell.end;
        }

        for(int32_t batch=begin_batch;batch<end_batch;batch++) {
            for(int32_t i=begin_i; i<end_i; i++) {
                for(int32_t j=0;j<k;j++) {
                    if((lower >= 0 && (i-j) > lower) || (upper >= 0 && (j-i) > upper)) {
                        a[batch*n*k+i*k+j] = 0;
                    }
                }
            }
        }
    }

public:
    static void execute(
        int32_t m, int32_t n, int32_t k,
        T *a,
        int32_t lower, int32_t upper
    )
    {
        if(m <= 0 || n <= 0) {
            return;
        }
        int32_t parallel;
        bool para_batch;
        if(m>n) {
            parallel = m;
            para_batch = true;
        } else {
            parallel = n;
            para_batch = false;
        }

        ParallelOperation::execute(parallel,kernel,para_batch,m,n,k,a,lower,upper);
    }
};
}


extern "C" {
void rindow_matlib_s_bandpart(
    int32_t m, int32_t n, int32_t k,
    float *a,
    int32_t lower, int32_t upper)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    BandPart<float>::execute(m, n, k, a, lower, upper);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_bandpart(
    int32_t m, int32_t n, int32_t k,
    double *a,
    int32_t lower, int32_t upper)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    BandPart<double>::execute(m, n, k, a, lower, upper);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_i_bandpart(
    int32_t m, int32_t n, int32_t k,
    int32_t dtype,
    void *a,
    int32_t lower, int32_t upper)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            BandPart<int8_t>::execute(m, n, k, (int8_t*)a, lower, upper);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            BandPart<uint8_t>::execute(m, n, k, (uint8_t*)a, lower, upper);
            break;
        }
        case rindow_matlib_dtype_int16: {
            BandPart<int16_t>::execute(m, n, k, (int16_t*)a, lower, upper);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            BandPart<uint16_t>::execute(m, n, k, (uint16_t*)a, lower, upper);
            break;
        }
        case rindow_matlib_dtype_int32: {
            BandPart<int32_t>::execute(m, n, k, (int32_t*)a, lower, upper);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            BandPart<uint32_t>::execute(m, n, k, (uint32_t*)a, lower, upper);
            break;
        }
        case rindow_matlib_dtype_int64: {
            BandPart<int64_t>::execute(m, n, k, (int64_t*)a, lower, upper);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            BandPart<uint64_t>::execute(m, n, k, (uint64_t*)a, lower, upper);
            break;
        }
        case rindow_matlib_dtype_bool: {
            BandPart<uint8_t>::execute(m, n, k, (uint8_t*)a, lower, upper);
            break;
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
}

}

