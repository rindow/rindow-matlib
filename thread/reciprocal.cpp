#include "rindow/matlib.h"
#include "common.hpp"
#include <cmath>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Reciprocal
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX,
        T alpha,
        T beta
    )
    {
        for(int32_t i = cell.begin; i < cell.end; i++) {
            T t = alpha * x[i*incX] + beta;
            x[i*incX] = 1 / t;
        }
    }

public:
    static void execute(
        int32_t n,
        T *x,
        int32_t incX,
        T alpha,
        T beta
        )
    {
        if(n <= 0) {
            return;
        }

        ParallelOperation::execute(n,kernel,n,x,incX,alpha,beta);
    }
};

}

extern "C" {
void rindow_matlib_s_reciprocal(int32_t n, float *x, int32_t incX, float alpha, float beta)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Reciprocal<float>::execute(n, x, incX, alpha, beta);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_reciprocal(int32_t n, double *x, int32_t incX, double alpha, double beta)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Reciprocal<double>::execute(n, x, incX, alpha, beta);
    RINDOW_END_CLEAR_EXCEPTION;
}

}
