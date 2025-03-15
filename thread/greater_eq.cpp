#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

/**
 *     A[m,n] := 1 (A[m,n] >= X[n])
 *     A[m,n] := 0 (A[m,n] <  X[n])
 */

template <typename T>
class GreaterEqual
{
private:
    static void kernel_sub(
        T *a,
        int32_t i,
        int32_t ldA,
        T *x,
        int32_t j,
        int32_t incX
    )
    {
        // *** CAUTION ***
        // if NaN set 0.0
        // if equal set 1.0
        if(a[i*ldA+j] >= x[j*incX]) {
            a[i*ldA+j] = 1.0;
        } else {
            a[i*ldA+j] = 0.0;
        }
    }

    static void kernel(
        ParallelOperation::cellInfo cell,
        bool para_m,
        int32_t m,
        int32_t n,
        T *a,
        int32_t ldA,
        T *x,
        int32_t incX
    )
    {
        if(para_m) {
            for(int32_t i = cell.begin; i < cell.end; i++) {
                for(int32_t j=0;j<n; j++) {
                    kernel_sub(a,i,ldA,x,j,incX);
                }
            }
        } else {
            for(int32_t i=0;i<m; i++) {
                for(int32_t j = cell.begin; j < cell.end; j++) {
                    kernel_sub(a,i,ldA,x,j,incX);
                }
            }
        }
    }

public:
    static void execute(
        int32_t m,
        int32_t n,
        T *a,
        int32_t ldA,
        T *x,
        int32_t incX
    )
    {
        if(n<=0||m<=0) {
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
void rindow_matlib_s_greater_equal(
    int32_t m,int32_t n,float *a, int32_t ldA,float *x, int32_t incX
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    GreaterEqual<float>::execute(m, n, a, ldA, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_greater_equal(
    int32_t m,int32_t n,double *a, int32_t ldA,double *x, int32_t incX
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    GreaterEqual<double>::execute(m, n, a, ldA, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}
}
