#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Slice
{
private:
    static void slice(
        int32_t i0,
        int32_t i1,
        int32_t i2,
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t size,
        T *a, int32_t incA,
        T *y, int32_t incY,
        int32_t startAxis0,
        int32_t sizeAxis0,
        int32_t startAxis1,
        int32_t sizeAxis1,
        int32_t startAxis2,
        int32_t sizeAxis2
    )
    {
        int32_t pa = (i0+startAxis0)*n*k*size*incA+
                     (i1+startAxis1)*k*size*incA+
                     (i2+startAxis2)*size*incA;
        int32_t py = i0*sizeAxis1*sizeAxis2*size+
                     i1*sizeAxis2*size*incY+
                     i2*size*incY;
        T *from = &(a[pa]);
        T *to   = &(y[py]);
        int32_t fromInc = incA;
        int32_t toInc   = incY;
        if(reverse) {
            T *tmp;
            tmp = from;
            from = to;
            to = tmp;
        }
        if(addMode) { // add mode
            if(toInc==fromInc) { // simd
                for(int32_t cnt=0; cnt<size; ++cnt) {
                    to[cnt*toInc] += from[cnt*toInc];
                }
            } else {            // no simd
                for(int32_t cnt=0; cnt<size; ++cnt) {
                    to[cnt*toInc] += from[cnt*fromInc];
                }
            }
        } else { // copy mode
            if(toInc==fromInc) { // simd
                for(int32_t cnt=0; cnt<size; ++cnt) {
                    to[cnt*toInc] = from[cnt*toInc];
                }
            } else {            // no simd
                for(int32_t cnt=0; cnt<size; ++cnt) {
                    to[cnt*toInc] = from[cnt*fromInc];
                }
            }
        }
    }

    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t para_mode,
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t size,
        T *a, int32_t incA,
        T *y, int32_t incY,
        int32_t startAxis0,
        int32_t sizeAxis0,
        int32_t startAxis1,
        int32_t sizeAxis1,
        int32_t startAxis2,
        int32_t sizeAxis2
    )
    {
        int32_t begin_0,end_0,begin_1,end_1,begin_2,end_2;
        begin_0=0;
        end_0=sizeAxis0;
        begin_1=0;
        end_1=sizeAxis1;
        begin_2=0;
        end_2=sizeAxis2;
        if(para_mode==0) {
            begin_0=cell.begin;
            end_0=cell.end;
        } else if(para_mode==1) {
            begin_1=cell.begin;
            end_1=cell.end;
        } else {
            begin_2=cell.begin;
            end_2=cell.end;
        }
        for(int32_t i0=begin_0; i0<end_0; i0++) {
            for(int32_t i1=begin_1; i1<end_1; i1++){
                for(int32_t i2=begin_2; i2<end_2; i2++){
                    slice(
                        i0,i1,i2,
                        reverse,addMode,
                        m,n,k,size,
                        a,incA,
                        y,incY,
                        startAxis0,sizeAxis0,
                        startAxis1,sizeAxis1,
                        startAxis2,sizeAxis2
                    );
                }
            }
        }
    }

public:
    static void execute(
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t size,
        T *a, int32_t incA,
        T *y, int32_t incY,
        int32_t startAxis0,
        int32_t sizeAxis0,
        int32_t startAxis1,
        int32_t sizeAxis1,
        int32_t startAxis2,
        int32_t sizeAxis2
    )
    {
        if(m <= 0 || n <= 0|| k <= 0 || size <= 0) {
            return;
        }

        int32_t parallel;
        int32_t para_mode;

        if(sizeAxis0>sizeAxis2) {
            if(sizeAxis0>sizeAxis1) {
                // size0 > size1 > size2
                parallel = sizeAxis0;
                para_mode = 0;
            } else {
                // size1 > size0 > size2
                parallel = sizeAxis1;
                para_mode = 1;
            }
        } else {
                // size2 > size0 or size1
                parallel = sizeAxis2;
                para_mode = 2;
        }
        ParallelOperation::execute(
            parallel,kernel,para_mode,
            reverse,addMode,
            m,n,k,size,
            a,incA,
            y,incY,
            startAxis0,sizeAxis0,
            startAxis1,sizeAxis1,
            startAxis2,sizeAxis2
        );
    }
};

}

extern "C" {
void rindow_matlib_s_slice(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t size,
    float *a, int32_t incA,
    float *y, int32_t incY,
    int32_t startAxis0,
    int32_t sizeAxis0,
    int32_t startAxis1,
    int32_t sizeAxis1,
    int32_t startAxis2,
    int32_t sizeAxis2
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Slice<float>::execute(
        reverse,addMode,
        m,n,k,size,
        a,incA,
        y,incY,
        startAxis0,sizeAxis0,
        startAxis1,sizeAxis1,
        startAxis2,sizeAxis2
    );
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_slice(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t size,
    double *a, int32_t incA,
    double *y, int32_t incY,
    int32_t startAxis0,
    int32_t sizeAxis0,
    int32_t startAxis1,
    int32_t sizeAxis1,
    int32_t startAxis2,
    int32_t sizeAxis2
    )
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Slice<double>::execute(
        reverse,addMode,
        m,n,k,size,
        a,incA,
        y,incY,
        startAxis0,sizeAxis0,
        startAxis1,sizeAxis1,
        startAxis2,sizeAxis2
    );
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_i_slice(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t size,
    int32_t dtype,
    void *a, int32_t incA,
    void *y, int32_t incY,
    int32_t startAxis0,
    int32_t sizeAxis0,
    int32_t startAxis1,
    int32_t sizeAxis1,
    int32_t startAxis2,
    int32_t sizeAxis2
    )
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            Slice<int8_t>::execute(
                reverse,addMode,
                m,n,k,size,
                (int8_t*)a,incA,
                (int8_t*)y,incY,
                startAxis0,sizeAxis0,
                startAxis1,sizeAxis1,
                startAxis2,sizeAxis2
            );
            break;
        }
        case rindow_matlib_dtype_uint8: {
            Slice<uint8_t>::execute(
                reverse,addMode,
                m,n,k,size,
                (uint8_t*)a,incA,
                (uint8_t*)y,incY,
                startAxis0,sizeAxis0,
                startAxis1,sizeAxis1,
                startAxis2,sizeAxis2
            );
            break;
        }
        case rindow_matlib_dtype_int16: {
            Slice<int16_t>::execute(
                reverse,addMode,
                m,n,k,size,
                (int16_t*)a,incA,
                (int16_t*)y,incY,
                startAxis0,sizeAxis0,
                startAxis1,sizeAxis1,
                startAxis2,sizeAxis2
            );
            break;
        }
        case rindow_matlib_dtype_uint16: {
            Slice<uint16_t>::execute(
                reverse,addMode,
                m,n,k,size,
                (uint16_t*)a,incA,
                (uint16_t*)y,incY,
                startAxis0,sizeAxis0,
                startAxis1,sizeAxis1,
                startAxis2,sizeAxis2
            );
            break;
        }
        case rindow_matlib_dtype_int32: {
            Slice<int32_t>::execute(
                reverse,addMode,
                m,n,k,size,
                (int32_t*)a,incA,
                (int32_t*)y,incY,
                startAxis0,sizeAxis0,
                startAxis1,sizeAxis1,
                startAxis2,sizeAxis2
            );
            break;
        }
        case rindow_matlib_dtype_uint32: {
            Slice<uint32_t>::execute(
                reverse,addMode,
                m,n,k,size,
                (uint32_t*)a,incA,
                (uint32_t*)y,incY,
                startAxis0,sizeAxis0,
                startAxis1,sizeAxis1,
                startAxis2,sizeAxis2
            );
            break;
        }
        case rindow_matlib_dtype_int64: {
            Slice<int64_t>::execute(
                reverse,addMode,
                m,n,k,size,
                (int64_t*)a,incA,
                (int64_t*)y,incY,
                startAxis0,sizeAxis0,
                startAxis1,sizeAxis1,
                startAxis2,sizeAxis2
            );
            break;
        }
        case rindow_matlib_dtype_uint64: {
            Slice<uint64_t>::execute(
                reverse,addMode,
                m,n,k,size,
                (uint64_t*)a,incA,
                (uint64_t*)y,incY,
                startAxis0,sizeAxis0,
                startAxis1,sizeAxis1,
                startAxis2,sizeAxis2
            );
            break;
        }
        case rindow_matlib_dtype_bool: {
            Slice<uint8_t>::execute(
                reverse, addMode,
                m,n,k,size,
                (uint8_t*)a,incA,
                (uint8_t*)y,incY,
                startAxis0,sizeAxis0,
                startAxis1,sizeAxis1,
                startAxis2,sizeAxis2
            );
            break;
        }
        default: {
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
}

}

