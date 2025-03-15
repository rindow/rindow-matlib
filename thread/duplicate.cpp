#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Duplicate
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        bool para_m,
        int32_t trans,int32_t m,int32_t n,
        T *x, int32_t incX,
        T *a, int32_t ldA
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
        if(trans==RINDOW_MATLIB_NO_TRANS) {
            for(int32_t i=begin_i; i<end_i; i++) {
                for(int32_t j=begin_j; j<end_j; j++) {
                    a[i*ldA+j] = x[j*incX];
                }
            }
        } else {
            for(int32_t i=begin_i; i<end_i; i++) {
                for(int32_t j=begin_j; j<end_j; j++) {
                    a[i*ldA+j] = x[i*incX];
                }
            }
        }
    }

public:
    static void execute(
        int32_t trans,int32_t m,int32_t n,
        T *x, int32_t incX,
        T *a, int32_t ldA
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

        ParallelOperation::execute(parallel,kernel,para_m,trans,m,n,x,incX,a,ldA);
    }
};

}

extern "C" {
void rindow_matlib_s_duplicate(
    int32_t trans,int32_t m,int32_t n,float *x, int32_t incX,float *a, int32_t ldA)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Duplicate<float>::execute(trans, m, n, x, incX, a, ldA);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_duplicate(
    int32_t trans,int32_t m,int32_t n,double *x, int32_t incX,double *a, int32_t ldA)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Duplicate<double>::execute(trans, m, n, x, incX, a, ldA);
    RINDOW_END_CLEAR_EXCEPTION;
}
}
