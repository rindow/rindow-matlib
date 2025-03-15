#include "rindow/matlib.h"
#include "common.hpp"
#include <cmath>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Isnan
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        if(incX==1) {
            for(int32_t i = cell.begin; i < cell.end; i++) {
                if(std::isnan(x[i])) {
                    x[i] = 1.0;
                } else {
                    x[i] = 0.0;
                }
            }
        } else {
            for(int32_t i = cell.begin; i < cell.end; i++) {
                if(std::isnan(x[i*incX])) {
                    x[i*incX] = 1.0;
                } else {
                    x[i*incX] = 0.0;
                }
            }
        }
    }

public:
    static void execute(
        int32_t n,
        T *x,
        int32_t incX
        )
    {
        if(n <= 0) {
            return;
        }

        ParallelOperation::execute(n,kernel,n,x,incX);
    }
};

}

extern "C" {
void rindow_matlib_s_isnan(int32_t n, float *x, int32_t incX)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Isnan<float>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_isnan(int32_t n, double *x, int32_t incX)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Isnan<double>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}

}
