#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class ReduceGather
{
private:
    static int32_t kernel(
        ParallelOperation::cellInfo cell,
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t numClass,
        int32_t dtype,
        void *x,
        T *a,
        T *b
    )
    {
        int32_t ldX = n;
        int32_t ldA = n*numClass;
        int32_t ldB = n;

        int32_t errcode = 0;
        for(int32_t i=cell.begin; i<cell.end; i++) {
            for(int32_t j=0; j<n; j++) {
                int64_t selector;
                rindow_matlib_common_get_integer(dtype, x, 1, i*ldX+j, &selector);
                if(selector<0||selector>=numClass) {
                    errcode = RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
                    continue;
                }
                T *from = &(a[i*ldA+j+selector*ldB]);
                T *to = &(b[i*ldB+j]);
                if(reverse) {
                    T *tmp;
                    tmp = from;
                    from = to;
                    to = tmp;
                }
                if(addMode) {
                    *to += *from;
                } else {
                    *to = *from;
                }
            }
        }
        return errcode;
    }

public:
    static int32_t execute(
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t numClass,
        int32_t dtype,
        void *x,
        T *a,
        T *b
    )
    {
        if(m <= 0 || n <= 0) {
            return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
        }
        int32_t value_width = sizeof(float);
    
        int32_t errcode = 0;
        int64_t checktype;
    
        if(rindow_matlib_common_get_integer(dtype, x, 1, 0, &checktype)) {
            return RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
        }
    
        errcode = ParallelOperation::executeAndGetCode<int32_t>(
            m,kernel,
            reverse,addMode,m,n,numClass,dtype,x,a,b
        );

        return errcode;
    }
};

}

extern "C" {
int32_t rindow_matlib_s_reducegather(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t numClass,
    int32_t dtype,
    void *x,
    float *a,
    float *b
)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = ReduceGather<float>::execute(reverse,addMode,m,n,numClass,dtype,x,a,b);
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_d_reducegather(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t numClass,
    int32_t dtype,
    void *x,
    double *a,
    double *b
)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = ReduceGather<double>::execute(reverse,addMode,m,n,numClass,dtype,x,a,b);
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_i_reducegather(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t numClass,
    int32_t dtype,
    void *x,
    int32_t data_dtype,
    void *a,
    void *b
)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(data_dtype) {
        case rindow_matlib_dtype_int8: {
            errcode = ReduceGather<int8_t>::execute(reverse,addMode,m,n,numClass,dtype,x,(int8_t*)a,(int8_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            errcode = ReduceGather<uint8_t>::execute(reverse,addMode,m,n,numClass,dtype,x,(uint8_t*)a,(uint8_t*)b);
            break;
        }
        case rindow_matlib_dtype_int16: {
            errcode = ReduceGather<int16_t>::execute(reverse,addMode,m,n,numClass,dtype,x,(int16_t*)a,(int16_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            errcode = ReduceGather<uint16_t>::execute(reverse,addMode,m,n,numClass,dtype,x,(uint16_t*)a,(uint16_t*)b);
            break;
        }
        case rindow_matlib_dtype_int32: {
            errcode = ReduceGather<int32_t>::execute(reverse,addMode,m,n,numClass,dtype,x,(int32_t*)a,(int32_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            errcode = ReduceGather<uint32_t>::execute(reverse,addMode,m,n,numClass,dtype,x,(uint32_t*)a,(uint32_t*)b);
            break;
        }
        case rindow_matlib_dtype_int64: {
            errcode = ReduceGather<int64_t>::execute(reverse,addMode,m,n,numClass,dtype,x,(int64_t*)a,(int64_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            errcode = ReduceGather<uint64_t>::execute(reverse,addMode,m,n,numClass,dtype,x,(uint64_t*)a,(uint64_t*)b);
            break;
        }
        case rindow_matlib_dtype_bool: {
            errcode = ReduceGather<uint8_t>::execute(reverse,addMode,m,n,numClass,dtype,x,(uint8_t*)a,(uint8_t*)b);
            break;
        }
        default: {
            errcode = RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

}

