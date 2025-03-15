#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Increment
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
        if(incX==1) {
            for(int32_t i = cell.begin; i < cell.end; i++) {
                x[i] = alpha * x[i] + beta;
            }
        } else {
            for(int32_t i = cell.begin; i < cell.end; i++) {
                x[i*incX] = alpha * x[i*incX] + beta;
            }
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
void rindow_matlib_s_increment(int32_t n, float *x, int32_t incX, float alpha, float beta)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Increment<float>::execute(n, x, incX, alpha, beta);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_increment(int32_t n, double *x, int32_t incX, double alpha, double beta)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Increment<double>::execute(n, x, incX, alpha, beta);
    RINDOW_END_CLEAR_EXCEPTION;
}

}
