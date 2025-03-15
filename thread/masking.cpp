#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {
const int32_t mode_set = 0;
const int32_t mode_add = 1;
const int32_t mode_or = 10000;

template <typename T>
class Masking
{

private:
    static void apply_masking(
        T *a,
        int32_t index,
        T value,
        int32_t mode
    ) {
        if(mode==mode_set) {
            a[index] =  value;
        } else if(mode==mode_or) {
            a[index] = a[index] ? a[index] : value;
        } else {
            a[index] +=  value;
        }
    }

    static void kernel_simple(
        ParallelOperation::cellInfo cell,
        T fill,
        int32_t mode,
        uint8_t *x,
        T *a
    )
    {
        int32_t begin_i,end_i;
        begin_i=cell.begin;
        end_i=cell.end;

        for(int32_t i=begin_i; i<end_i; i++) {
            if(!x[i]) {
                apply_masking(a,i,fill,mode);
            }
        }
    }

    static void kernel_broadcast(
        ParallelOperation::cellInfo cell,
        bool para_m,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t len,
        T fill,
        int32_t mode,
        uint8_t *x,
        T *a
    )
    {
        int32_t begin_i,end_i,begin_j,end_j;
        if(para_m) {
            begin_i=cell.begin;
            end_i=cell.end;
            begin_j=0;
            end_j=n;
        } else {
            begin_i=0;
            end_i=m;
            begin_j=cell.begin;
            end_j=cell.end;
        }

        if(k==1) {
            for(int32_t i=begin_i; i<end_i; i++) {
                for(int32_t j=begin_j; j<end_j; j++) {
                    if(!x[i]) {
                        for(int32_t l=0; l<len; l++) {
                            apply_masking(a,((i*n+j)*len+l),fill,mode);
                        }
                    }
                }
            }
        } else {
            for(int32_t i=begin_i; i<end_i; i++) {
                for(int32_t j=begin_j; j<end_j; j++) {
                    for(int32_t h=0; h<k; h++) {
                        if(!x[i*k+h]) {
                            for(int32_t l=0; l<len; l++) {
                                apply_masking(a,(((i*n+j)*k+h)*len+l),fill,mode);
                            }
                        }
                    }
                }
            }
        }
    }

    static void kernel_broadcast_inner_ngtk(
        ParallelOperation::cellInfo cell,
        int32_t n,
        int32_t k,
        int32_t len,
        T fill,
        int32_t mode,
        uint8_t *x,
        T *a
    )
    {
        int32_t begin_h,end_h;
        begin_h=cell.begin;
        end_h=cell.end;

        for(int32_t j=0; j<n; j++) {
            for(int32_t h=begin_h; h<end_h; h++) {
                if(!x[h]) {
                    for(int32_t l=0; l<len; l++) {
                        apply_masking(a,((j*k+h)*len+l),fill,mode);
                    }
                }
            }
        }
    }

public:
    static void execute(
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t len,
        T fill,
        int32_t mode,
        uint8_t *x, 
        T *a
    )
    {
        if(m <= 0 || n <= 0 || k <=0) {
            return;
        }

        if(m==1 && n==1 && len==1) { // simple masking (same shape X and A)
            ParallelOperation::execute(k,kernel_simple,fill,mode,x,a);
        } else if(m==1 && n<k) {
            ParallelOperation::execute(k,kernel_broadcast_inner_ngtk,n,k,len,fill,mode,x,a);
        } else {
            int32_t parallel;
            bool para_m;
            if(m>n) {
                parallel = m;
                para_m = true;
            } else {
                parallel = n;
                para_m = false;
            }
            ParallelOperation::execute(parallel,kernel_broadcast,para_m,m,n,k,len,fill,mode,x,a);
        }
    }
};

}

extern "C" {
void rindow_matlib_s_masking(
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t len,
    float fill,
    int32_t mode,
    uint8_t *x, 
    float *a
) {
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Masking<float>::execute(m, n, k, len, fill, mode, x, a);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_masking(
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t len,
    double fill,
    int32_t mode,
    uint8_t *x, 
    double *a
) {
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Masking<double>::execute(m, n, k, len, fill, mode, x, a);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_i_masking(
    int32_t dtype,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t len,
    void *fill,
    int32_t mode,
    uint8_t *x, 
    void *a
) {
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    int32_t vmode = mode;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            Masking<int8_t>::execute(m, n, k, len, *((int8_t*)fill), mode, x, (int8_t*)a);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            Masking<uint8_t>::execute(m, n, k, len, *((uint8_t*)fill), mode, x, (uint8_t*)a);
            break;
        }
        case rindow_matlib_dtype_int16: {
            Masking<int16_t>::execute(m, n, k, len, *((int16_t*)fill), mode, x, (int16_t*)a);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            Masking<uint16_t>::execute(m, n, k, len, *((uint16_t*)fill), mode, x, (uint16_t*)a);
            break;
        }
        case rindow_matlib_dtype_int32: {
            Masking<int32_t>::execute(m, n, k, len, *((int32_t*)fill), mode, x, (int32_t*)a);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            Masking<uint32_t>::execute(m, n, k, len, *((uint32_t*)fill), mode, x, (uint32_t*)a);
            break;
        }
        case rindow_matlib_dtype_int64: {
            Masking<int64_t>::execute(m, n, k, len, *((int64_t*)fill), mode, x, (int64_t*)a);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            Masking<uint64_t>::execute(m, n, k, len, *((uint64_t*)fill), mode, x, (uint64_t*)a);
            break;
        }
        case rindow_matlib_dtype_bool: {
            int32_t mode;
            if(vmode==mode_set) {
                mode = mode_set;
            } else {
                mode = mode_or;
            }
            Masking<uint8_t>::execute(m, n, k, len, *((uint8_t*)fill), mode, x, (uint8_t*)a);
            break;
        }
        default: {
            break;
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
}
}
