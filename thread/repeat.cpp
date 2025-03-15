#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Repeat
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        bool para_m,
        int32_t m,
        int32_t k,
        int32_t repeats,
        T *a,
        T *b
    )
    {
        int32_t begin_i,end_i,begin_j,end_j;
        if(para_m) {
            begin_i=cell.begin;
            end_i=cell.end;
            begin_j=0;
            end_j=repeats;
        } else {
            begin_i=0;
            end_i=m;
            begin_j=cell.begin;
            end_j=cell.end;
        }
        int32_t ldA = k;
        int32_t ldB = k;
        int32_t bytes = k*sizeof(T);
        for(int32_t i=begin_i; i<end_i; i++) {
            for(int32_t j=begin_j; j<end_j; j++) {
                memcpy(&(b[i*repeats*ldB+j*ldB]), &(a[i*ldA]), bytes);
            }
        }
    }

public:
    static void execute(
        int32_t m,
        int32_t k,
        int32_t repeats,
        T *a,
        T *b
        )
    {
        if(m <= 0 || k <= 0 || repeats <= 0 ) {
            return;
        }
        int32_t parallel;
        bool para_m;
        if(m>repeats) {
            parallel = m;
            para_m = true;
        } else {
            parallel = repeats;
            para_m = false;
        }

        ParallelOperation::execute(parallel,kernel,para_m,m,k,repeats,a,b);
    }
};
}

extern "C" {
void rindow_matlib_s_repeat(
    int32_t m,
    int32_t k,
    int32_t repeats,
    float *a,
    float *b
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Repeat<float>::execute(m, k, repeats, a, b);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_repeat(
    int32_t m,
    int32_t k,
    int32_t repeats,
    double *a,
    double *b
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Repeat<double>::execute(m, k, repeats, a, b);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_i_repeat(
    int32_t m,
    int32_t k,
    int32_t repeats,
    int32_t dtype,
    void *a,
    void *b
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            Repeat<int8_t>::execute(m, k, repeats, (int8_t*)a, (int8_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            Repeat<uint8_t>::execute(m, k, repeats, (uint8_t*)a, (uint8_t*)b);
            break;
        }
        case rindow_matlib_dtype_int16: {
            Repeat<int16_t>::execute(m, k, repeats, (int16_t*)a, (int16_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            Repeat<uint16_t>::execute(m, k, repeats, (uint16_t*)a, (uint16_t*)b);
            break;
        }
        case rindow_matlib_dtype_int32: {
            Repeat<int32_t>::execute(m, k, repeats, (int32_t*)a, (int32_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            Repeat<uint32_t>::execute(m, k, repeats, (uint32_t*)a, (uint32_t*)b);
            break;
        }
        case rindow_matlib_dtype_int64: {
            Repeat<int64_t>::execute(m, k, repeats, (int64_t*)a, (int64_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            Repeat<uint64_t>::execute(m, k, repeats, (uint64_t*)a, (uint64_t*)b);
            break;
        }
        case rindow_matlib_dtype_bool: {
            Repeat<uint8_t>::execute(m, k, repeats, (uint8_t *)a, (uint8_t*)b);
            break;
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
}
}
