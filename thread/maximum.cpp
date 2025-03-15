#include "rindow/matlib.h"
#include "common.hpp"
#include <cmath>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Maximum
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        bool para_m,
        int32_t m, int32_t n,
        T *a, int32_t ldA,
        T *x, int32_t incX
    )
    {
        int32_t begin_i,end_i,begin_j,end_j;
        if(para_m) {
            begin_i=cell.begin;
            end_i=cell.end;
            begin_j=0;
            end_j=n;
        } else {
            begin_i=0;
            end_i=m;
            begin_j=cell.begin;
            end_j=cell.end;
        }
        for(int32_t i=begin_i; i<end_i; i++) {
            for(int32_t j=begin_j; j<end_j; j++) {
                T value = x[j*incX];
                if(std::isnan(value)) {
                    a[i*ldA+j] = value;
                } else {
                    // *** CAUTION ***
                    // if NaN then don't set alpha
                    if(a[i*ldA+j] < value) {
                        a[i*ldA+j] = value;
                    }
                }
            }
        }
    }

public:
    static void execute(
        int32_t m, int32_t n,
        T *a, int32_t ldA,
        T *x, int32_t incX
    )
    {
        if(m <= 0 || n <= 0) {
            return;
        }
        int32_t parallel;
        bool para_m;
        if(m>n) {
            parallel = m;
            para_m = true;
        } else {
            parallel = n;
            para_m = false;
        }

        ParallelOperation::execute(parallel,kernel,para_m,m,n,a,ldA,x,incX);
    }
};

}

extern "C" {
void rindow_matlib_s_maximum(
    int32_t m,
    int32_t n,
    float *a, int32_t ldA,
    float *x, int32_t incX
    )
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Maximum<float>::execute(m, n, a, ldA, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_maximum(
    int32_t m,
    int32_t n,
    double *a, int32_t ldA,
    double *x, int32_t incX
    )
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Maximum<double>::execute(m, n, a, ldA, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}
}
