#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Gather
{
private:
    static int32_t kernel(
        ParallelOperation::cellInfo cell,
        int32_t reverse,
        int32_t addMode,
        int32_t n,
        int32_t k,
        int32_t numClass,
        int32_t dtype,
        void *x,
        T *a,
        T *b
    )
    {
        int32_t errcode = 0;
        for(int32_t i=cell.begin; i<cell.end; i++) {
            int64_t selector = 0;
            rindow_matlib_common_get_integer(dtype, x, 1, i, &selector);
            if(selector<0||selector>=numClass) {
                errcode = RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
                continue;
            }
            T *from = &(a[selector*k]);
            T *to = &(b[i*k]);
            if(reverse) {
                T *tmp;
                tmp = from;
                from = to;
                to = tmp;
            }
            if(k==1) {
                if(addMode) {
                    *to += *from;
                } else {
                    *to = *from;
                }
            } else {
                if(addMode) {
                    for(int32_t idx=0; idx<k; idx++) {
                        to[idx] += from[idx];
                    }
                } else {
                    for(int32_t idx=0; idx<k; idx++) {
                        to[idx] = from[idx];
                    }
                }
            }
        }
        return errcode;
    }

public:
    static int32_t execute(
        int32_t reverse,
        int32_t addMode,
        int32_t n,
        int32_t k,
        int32_t numClass,
        int32_t dtype,
        void *x,
        T *a,
        T *b
    )
    {
        if(n <= 0) {
            return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
        }
        int32_t value_width = sizeof(float);
    
        int32_t errcode = 0;
        int64_t checktype;
    
        if(rindow_matlib_common_get_integer(dtype, x, 1, 0, &checktype)) {
            return RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
        }
    
        if(reverse&&addMode) {
            // serial addition
            ParallelOperation::cellInfo cell;
            cell.begin=0;
            cell.end=n;
            errcode = kernel(cell,reverse,addMode,n,k,numClass,dtype,x,a,b);
        } else {
            int32_t parallel = n;
            errcode = ParallelOperation::executeAndGetCode<int32_t>(
                parallel,kernel,
                reverse,addMode,n,k,numClass,dtype,x,a,b
            );
        }

        return errcode;
    }
};

}

extern "C" {
int32_t rindow_matlib_s_gather(
    int32_t reverse,
    int32_t addMode,
    int32_t n,
    int32_t k,
    int32_t numClass,
    int32_t dtype,
    void *x,
    float *a,
    float *b
)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Gather<float>::execute(reverse,addMode,n,k,numClass,dtype,x,a,b);
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_d_gather(
    int32_t reverse,
    int32_t addMode,
    int32_t n,
    int32_t k,
    int32_t numClass,
    int32_t dtype,
    void *x,
    double *a,
    double *b
)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Gather<double>::execute(reverse,addMode,n,k,numClass,dtype,x,a,b);
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_i_gather(
    int32_t reverse,
    int32_t addMode,
    int32_t n,
    int32_t k,
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
            errcode = Gather<int8_t>::execute(reverse,addMode,n,k,numClass,dtype,x,(int8_t*)a,(int8_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            errcode = Gather<uint8_t>::execute(reverse,addMode,n,k,numClass,dtype,x,(uint8_t*)a,(uint8_t*)b);
            break;
        }
        case rindow_matlib_dtype_int16: {
            errcode = Gather<int16_t>::execute(reverse,addMode,n,k,numClass,dtype,x,(int16_t*)a,(int16_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            errcode = Gather<uint16_t>::execute(reverse,addMode,n,k,numClass,dtype,x,(uint16_t*)a,(uint16_t*)b);
            break;
        }
        case rindow_matlib_dtype_int32: {
            errcode = Gather<int32_t>::execute(reverse,addMode,n,k,numClass,dtype,x,(int32_t*)a,(int32_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            errcode = Gather<uint32_t>::execute(reverse,addMode,n,k,numClass,dtype,x,(uint32_t*)a,(uint32_t*)b);
            break;
        }
        case rindow_matlib_dtype_int64: {
            errcode = Gather<int64_t>::execute(reverse,addMode,n,k,numClass,dtype,x,(int64_t*)a,(int64_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            errcode = Gather<uint64_t>::execute(reverse,addMode,n,k,numClass,dtype,x,(uint64_t*)a,(uint64_t*)b);
            break;
        }
        case rindow_matlib_dtype_bool: {
            errcode = Gather<uint8_t>::execute(reverse,addMode,n,k,numClass,dtype,x,(uint8_t*)a,(uint8_t*)b);
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

