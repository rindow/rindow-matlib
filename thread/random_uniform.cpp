#include "rindow/matlib.h"
#include "common.hpp"
#include <random>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class RandomUniform
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX,
        T low,
        T high,
        int32_t seed
    )
    {
        std::default_random_engine engine(seed+cell.id);
        std::uniform_real_distribution<T> dist(low, high);

        for(int32_t i = cell.begin; i < cell.end; i++) {
            x[i*incX] = dist(engine);
        }
    }

    static void kernel_int(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX,
        T low,
        T high,
        int32_t seed
    )
    {
        std::default_random_engine engine(seed+cell.id);
        std::uniform_int_distribution<T> dist(low, high);

        for(int32_t i = cell.begin; i < cell.end; i++) {
            x[i*incX] = dist(engine);
        }
    }

public:
    static void execute(
        int32_t n,
        T *x, int32_t incX,
        T low,
        T high,
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
        ParallelOperation::execute(n,kernel,n, x, incX, low, high, seed);
    }

    static void execute_int(
        int32_t n,
        T *x, int32_t incX,
        T low,
        T high,
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
        ParallelOperation::execute(n,kernel_int,n, x, incX, low, high, seed);
    }
};

}

extern "C" {
void rindow_matlib_s_randomuniform(
    int32_t n,
    float *x, int32_t incX,
    float low,
    float high,
    int32_t seed
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    RandomUniform<float>::execute(n, x, incX, low, high, seed);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_randomuniform(
    int32_t n,
    double *x, int32_t incX,
    double low,
    double high,
    int32_t seed
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    RandomUniform<double>::execute(n, x, incX, low, high, seed);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_i_randomuniform(
    int32_t n,
    int32_t dtype,
    void *x, int32_t incX,
    int32_t low,
    int32_t high,
    int32_t seed
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        //case rindow_matlib_dtype_int8: {
        //    RandomUniform<int8_t>::execute_int(n, (int8_t*)x, incX, low, high, seed);
        //    break;
        //}
        //case rindow_matlib_dtype_uint8: {
        //    RandomUniform<uint8_t>::execute_int(n, (uint8_t*)x, incX, low, high, seed);
        //    break;
        //}
        case rindow_matlib_dtype_int16: {
            RandomUniform<int16_t>::execute_int(n, (int16_t*)x, incX, low, high, seed);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            RandomUniform<uint16_t>::execute_int(n, (uint16_t*)x, incX, low, high, seed);
            break;
        }
        case rindow_matlib_dtype_int32: {
            RandomUniform<int32_t>::execute_int(n, (int32_t*)x, incX, low, high, seed);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            RandomUniform<uint32_t>::execute_int(n, (uint32_t*)x, incX, low, high, seed);
            break;
        }
        case rindow_matlib_dtype_int64: {
            RandomUniform<int64_t>::execute_int(n, (int64_t*)x, incX, low, high, seed);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            RandomUniform<uint64_t>::execute_int(n, (uint64_t*)x, incX, low, high, seed);
            break;
        }
        //case rindow_matlib_dtype_bool: {
        //    RandomUniform<uint8_t>::execute_int(n, (uint8_t *)x, incX, low, high, seed);
        //    break;
        //}
    }
    RINDOW_END_CLEAR_EXCEPTION;
}

}
