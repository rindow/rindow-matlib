#include "rindow/matlib.h"
#include "common.hpp"
#include <cmath>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Softmax
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t m,
        int32_t n,
        T *a, int32_t ldA
    )
    {
        for(int32_t i = cell.begin; i < cell.end; i++) {
            T *ax = &a[i*ldA];
            T max_a;

            max_a = ax[0];
            for(int j=1;j<n;j++) {
                if(!(max_a>=ax[j])) {
                    max_a = ax[j];
                }
            }

            T sum_exp = 0;
            //T c = 0; // Compensation term: Cumulative sum by Kahan summation
            for(int32_t j=0;j<n;j++) {
                T t = std::exp(ax[j]-max_a); \
                sum_exp += t;
                ax[j] = t;
        
                //T exp_val = std::exp(ax[j]-max_a);
                //T y = exp_val - c;
                //T t = sum_exp + y;
                //c = (t - sum_exp) - y;
                //sum_exp = t;
                //ax[j] = exp_val;
            }
            for(int32_t j=0;j<n;j++) {
                ax[j] = ax[j] / sum_exp;
            }
        }
    }

public:
    static void execute(int32_t m, int32_t n, T *a, int32_t ldA)
    {
        if(m <= 0 || n <= 0) {
            return;
        }

        ParallelOperation::execute(m,kernel,m,n,a,ldA);
    }
};

}

extern "C" {
void rindow_matlib_s_softmax(
    int32_t m, int32_t n, float *a, int32_t ldA)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Softmax<float>::execute(m, n, a, ldA);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_softmax(
    int32_t m, int32_t n, double *a, int32_t ldA)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Softmax<double>::execute(m, n, a, ldA);
    RINDOW_END_CLEAR_EXCEPTION;
}

}
