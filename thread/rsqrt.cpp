#include "rindow/matlib.h"
#include "common.hpp"
#include <cmath>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Rsqrt
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T alpha,
        T *x, int32_t incX,
        T beta
    )
    {
        for(int32_t i = cell.begin; i < cell.end; i++) {
            x[i*incX] = 1 / (alpha * std::sqrt(x[i*incX]) + beta);
        }
    }

public:
    static void execute(
        int32_t n,
        T alpha,
        T *x, int32_t incX,
        T beta
    )
    {
        if(n <= 0) {
            return;
        }

        ParallelOperation::execute(n,kernel,n,alpha,x,incX,beta);
    }
};

}

extern "C" {
void rindow_matlib_s_rsqrt(
    int32_t n,
    float alpha,
    float *x, int32_t incX,
    float beta
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Rsqrt<float>::execute(n, alpha, x, incX, beta);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_rsqrt(
    int32_t n,
    double alpha,
    double *x, int32_t incX,
    double beta
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Rsqrt<double>::execute(n, alpha, x, incX, beta);
    RINDOW_END_CLEAR_EXCEPTION;
}

}
