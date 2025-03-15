#include "rindow/matlib.h"
#include "common.hpp"
#include <random>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class RandomNormal
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX,
        T mean,
        T scale,
        int32_t seed
    )
    {
        std::default_random_engine engine(seed+cell.id);
        std::normal_distribution<T> dist(mean, scale);

        for(int32_t i = cell.begin; i < cell.end; i++) {
            x[i*incX] = dist(engine);
        }
    }

public:
    static void execute(
        int32_t n,
        T *x, int32_t incX,
        T mean,
        T scale,
        int32_t seed
    )
    {
        if(n <= 0) {
            return;
        }
        if(seed==0) {
            std::random_device seed_gen;
            seed = seed_gen();
        }
        ParallelOperation::execute(n,kernel,n, x, incX, mean, scale, seed);
    }
};

}

extern "C" {
void rindow_matlib_s_randomnormal(
    int32_t n,
    float *x, int32_t incX,
    float mean,
    float scale,
    int32_t seed
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    RandomNormal<float>::execute(n, x, incX, mean, scale, seed);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_randomnormal(
    int32_t n,
    double *x, int32_t incX,
    double mean,
    double scale,
    int32_t seed
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    RandomNormal<double>::execute(n, x, incX, mean, scale, seed);
    RINDOW_END_CLEAR_EXCEPTION;
}

}
