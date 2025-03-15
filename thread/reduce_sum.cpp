#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class ReduceSum
{
private:
    static T sum(
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        T a=0;
        for(int32_t i=0; i<n; i++) {
            a += x[i*incX];
        }
        return a;
    }

    static void kernel(
        ParallelOperation::cellInfo cell,
        bool para_m,
        int32_t m,
        int32_t n,
        int32_t k,
        T *a,
        T *b
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
                b[i*ldB+j] = sum(n,&a[i*ldA+j],k);
            }
        }
    }

public:
    static void execute(
        int32_t m,
        int32_t n,
        int32_t k,
        T *a,
        T *b
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
}

extern "C" {
void rindow_matlib_s_reducesum(
    int32_t m,
    int32_t n,
    int32_t k,
    float *a,
    float *b
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ReduceSum<float>::execute(m, n, k, a, b);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_reducesum(
    int32_t m,
    int32_t n,
    int32_t k,
    double *a,
    double *b
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ReduceSum<double>::execute(m, n, k, a, b);
    RINDOW_END_CLEAR_EXCEPTION;
}
}
