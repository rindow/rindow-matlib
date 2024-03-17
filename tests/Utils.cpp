#include <algorithm>
#include <initializer_list>
#include <memory>
#include <type_traits>
#include <numeric>
#include <stdbool.h>

#include "Utils.h"

namespace RindowTest {

template <typename T>
void Utils::fill(int32_t size, T *x, T value) {
    for(int32_t i=0;i<size;++i) {
        x[i] = value;
    }
}

template <typename T>
void Utils::zeros(int32_t size, T *x) {
    for(int32_t i=0;i<size;++i) {
        x[i] = 0;
    }
}
template <typename T>
void Utils::ones(int32_t size, T *x) {
    for(int32_t i=0;i<size;++i) {
        x[i] = 1;
    }
}
template <typename T>
void Utils::range(int32_t size, T *x) {
    for(int32_t i=0;i<size;++i) {
        x[i] = (T)i;
    }
}

template <typename T>
std::unique_ptr<T[]> Utils::array(std::initializer_list<T> values) {
    size_t size = values.size();

    std::unique_ptr<T[]> arrayVal(new T[size]);

    std::copy_n(values.begin(), size, arrayVal.get());

    return arrayVal;
}
template void Utils::fill<float>(int32_t size, float *x, float value);
template void Utils::zeros<float>(int32_t size, float *x);
template void Utils::ones<float>(int32_t size, float *x);
template void Utils::range<float>(int32_t size, float *x);
template std::unique_ptr<float[]> Utils::array<float>(std::initializer_list<float> values);
template void Utils::fill<double>(int32_t size, double *x, double value);
template void Utils::zeros<double>(int32_t size, double *x);
template void Utils::ones<double>(int32_t size, double *x);
template void Utils::range<double>(int32_t size, double *x);
template std::unique_ptr<double[]> Utils::array<double>(std::initializer_list<double> values);
template void Utils::fill<int32_t>(int32_t size, int32_t *x, int32_t value);
template void Utils::zeros<int32_t>(int32_t size, int32_t *x);
template void Utils::ones<int32_t>(int32_t size, int32_t *x);
template void Utils::range<int32_t>(int32_t size, int32_t *x);
template std::unique_ptr<int32_t[]> Utils::array<int32_t>(std::initializer_list<int32_t> values);
template void Utils::fill<uint8_t>(int32_t size, uint8_t *x, uint8_t value);
template void Utils::zeros<uint8_t>(int32_t size, uint8_t *x);
template void Utils::ones<uint8_t>(int32_t size, uint8_t *x);
template void Utils::range<uint8_t>(int32_t size, uint8_t *x);
template std::unique_ptr<uint8_t[]> Utils::array<uint8_t>(std::initializer_list<uint8_t> values);

template <typename T>
bool Utils::isclose(int32_t size, T *trues, T *x) {
    T rtol = (T)1e-04;
    return isclose(size,trues,x,rtol);
}

template <typename T>
bool Utils::isclose(int32_t size, T *trues, T *x,T rtol) {
    T atol = (T)1e-07;
    return isclose(size,trues,x,rtol,atol);
}

template <typename T>
bool Utils::isclose(int32_t size, T *trues, T *x, T rtol, T atol) {
    if(rtol==0.0) {
        rtol = (T)1e-04;
    }
    if(atol==0.0) {
        atol = (T)1e-07;
    }
    T amaxDiff = 0;
    for(int32_t i=0; i<size; i++) {
        T v = trues[i] - x[i];
        if(v<0) {
            v = -v;
        }
        if(v>amaxDiff) {
            amaxDiff = v;
        }
    }
    T amaxX = 0;
    for(int32_t i=0; i<size; i++) {
        T v = x[i];
        if(v<0) {
            v = -v;
        }
        v *= rtol;
        if(v>amaxX) {
            amaxX = v;
        }
    }
    amaxX += atol;
    if(amaxDiff > amaxX) {
        return false;
    }
    return true;
}
template bool Utils::isclose<float>(int32_t size, float *trues, float *x);
template bool Utils::isclose<float>(int32_t size, float *trues, float *x, float rtol);
template bool Utils::isclose<float>(int32_t size, float *trues, float *x, float rtol, float atol);
template bool Utils::isclose<double>(int32_t size, double *trues, double *x);
template bool Utils::isclose<double>(int32_t size, double *trues, double *x, double rtol);
template bool Utils::isclose<double>(int32_t size, double *trues, double *x, double rtol, double atol);

}
