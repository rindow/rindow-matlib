#include "rindow/matlib.h"
#include "common.hpp"
#include <cmath>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Nan2num
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX,
        T alpha
    )
    {
        for(int32_t i = cell.begin; i < cell.end; i++) {
            if(std::isnan(x[i*incX])) {
                x[i*incX] = alpha;
            }
        }
    }

public:
    static void execute(int32_t n, T *x, int32_t incX, T alpha)
    {
        if(n <= 0) {
            return;
        }

        ParallelOperation::execute(n,kernel,n,x,incX,alpha);
    }
};

}

extern "C" {
void rindow_matlib_s_nan2num(int32_t n, float *x, int32_t incX, float alpha)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Nan2num<float>::execute(n, x, incX, alpha);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_nan2num(int32_t n, double *x, int32_t incX, double alpha)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Nan2num<double>::execute(n, x, incX, alpha);
    RINDOW_END_CLEAR_EXCEPTION;
}

}
