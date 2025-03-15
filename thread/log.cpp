#include "rindow/matlib.h"
#include "common.hpp"
#include <cmath>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Log
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        for(int32_t i = cell.begin; i < cell.end; i++) {
            x[i * incX] = std::log(x[i * incX]);
        }
    }

public:
    static void execute(int32_t n, T *x, int32_t incX)
    {
        if(n <= 0) {
            return;
        }

        ParallelOperation::execute(n,kernel,n,x,incX);
    }
};

}

extern "C" {
void rindow_matlib_s_log(int32_t n, float *x, int32_t incX)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Log<float>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_log(int32_t n, double *x, int32_t incX)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Log<double>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}

}
