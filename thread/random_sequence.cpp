#include "rindow/matlib.h"
#include "common.hpp"
#include <random>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class RandomSequence
{
private:
    static int32_t random(
        int32_t low,
        int32_t high,
        std::default_random_engine& engine
    )
    {
        return (engine()%(high-low+1))+low;
    }

public:
    static void execute(
        int32_t n,
        int32_t size,
        T *x, int32_t incX,
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
        std::default_random_engine engine(seed);

        for(int32_t i=0;i<n;i++) {
            x[i*incX] = i;
        }

        for(int32_t i=0;i<size;i++) {
            T tmp;
            int32_t idx = random(i, n-1, engine);
            tmp = x[i*incX];
            x[i*incX] = x[idx*incX];
            x[idx*incX] = tmp;
        }
    }
};

}

extern "C" {
void rindow_matlib_i_randomsequence(
    int32_t n,
    int32_t size,
    int32_t dtype,
    void *x, int32_t incX,
    int32_t seed
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            RandomSequence<int8_t>::execute(n, size, (int8_t*)x, incX, seed);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            RandomSequence<uint8_t>::execute(n, size, (uint8_t*)x, incX, seed);
            break;
        }
        case rindow_matlib_dtype_int16: {
            RandomSequence<int16_t>::execute(n, size, (int16_t*)x, incX, seed);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            RandomSequence<uint16_t>::execute(n, size, (uint16_t*)x, incX, seed);
            break;
        }
        case rindow_matlib_dtype_int32: {
            RandomSequence<int32_t>::execute(n, size, (int32_t*)x, incX, seed);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            RandomSequence<uint32_t>::execute(n, size, (uint32_t*)x, incX, seed);
            break;
        }
        case rindow_matlib_dtype_int64: {
            RandomSequence<int64_t>::execute(n, size, (int64_t*)x, incX, seed);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            RandomSequence<uint64_t>::execute(n, size, (uint64_t*)x, incX, seed);
            break;
        }
        //case rindow_matlib_dtype_bool: {
        //    RandomSequence<uint8_t>::execute(n, size, (uint8_t *)x, incX, seed);
        //    break;
        //}
    }
    RINDOW_END_CLEAR_EXCEPTION;
}

}
