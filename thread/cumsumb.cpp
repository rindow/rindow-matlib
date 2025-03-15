#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Cumsumb
{
private:
private:
    static void kernel_sub(
        int32_t i,
        int32_t m,
        int32_t n,
        int32_t k,
        T *a,
        int32_t exclusive,
        int32_t reverse,
        T *b
    )
    {
        int32_t ida;
        int32_t ldA;
        int32_t idb;
        int32_t ldB;
        if(reverse) {
            ida = i*n*k+(n-1)*k;
            ldA = -k;
            idb = i*n*k+(n-1)*k;
            ldB = -k;
        } else {
            ida = i*n*k;
            ldA = k;
            idb = i*n*k;
            ldB = k;
        }
        if(exclusive) {
            for(int32_t h=0;  h<k; h++) {
                b[idb+h] = 0;
            }
            for(int32_t j=0; j<n-1; j++) {
                idb += ldB;
                for(int32_t h=0; h<k; h++) {
                    b[idb+h] = b[idb-ldB+h] + a[ida+h];
                }
                ida += ldA;
            }
        } else {
            for(int32_t h=0; h<k; h++) {
                b[idb+h] = a[ida+h];
            }
            for(int32_t j=0; j<n-1; j++) {
                idb += ldB;
                ida += ldA;
                for(int32_t h=0; h<k; h++) {
                    b[idb+h] = b[idb-ldB+h] + a[ida+h];
                }
            }
        }
    }

    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t m,
        int32_t n,
        int32_t k,
        T *a,
        int32_t exclusive,
        int32_t reverse,
        T *b
    )
    {
        for(int32_t i = cell.begin; i < cell.end; i++) {
            kernel_sub(i,m,n,k,a,exclusive,reverse,b);
        }
    }

public:
    static void execute(
        int32_t m,
        int32_t n,
        int32_t k,
        T *a,
        int32_t exclusive,
        int32_t reverse,
        T *b
    )
    {
        if(m <= 0 || n <= 0 || k <= 0) {
            return;
        }
        if(m==1) {
            kernel_sub(0,m,n,k,a,exclusive,reverse,b);
        } else {
            ParallelOperation::execute(
                m,kernel,
                m,n,k,a,exclusive,reverse,b
            );
        }

        return;
    }
};

}

extern "C" {
void rindow_matlib_s_cumsumb(
    int32_t m,
    int32_t n,
    int32_t k,
    float *a,
    int32_t exclusive,
    int32_t reverse,
    float *b
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Cumsumb<float>::execute(
        m,n,k,
        a,
        exclusive,
        reverse,
        b
    );
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_cumsumb(
    int32_t m,
    int32_t n,
    int32_t k,
    double *a,
    int32_t exclusive,
    int32_t reverse,
    double *b
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Cumsumb<double>::execute(
        m,n,k,
        a,
        exclusive,
        reverse,
        b
    );
    RINDOW_END_CLEAR_EXCEPTION;
}
}
