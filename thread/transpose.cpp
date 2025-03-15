#include "rindow/matlib.h"
#include "common.hpp"
#include <vector>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Transpose
{
private:
    static void s_transCopySub(
        int32_t n,
        T *a, int32_t incA,
        T *b, int32_t incB
    )
    {
    //printf("start s_transCopy\n");
        for(int i=0;i<n;i++) {
            *b = *a;
            a += incA;
            b += incB;
        }
    }

    static void s_transCopy(
        int32_t ndim,
        int32_t *shape,
        int32_t *strides,
        int32_t *targetStrides,
        T *a,
        T *b
        )
    {
        int32_t repeat = *shape;
        int32_t stride = *strides;
        int32_t targetStride = *targetStrides;

    //printf("start s_transCopy\n");
    //printf("ndim=%d\n",ndim);

        if(ndim<=0) {
            s_transCopySub(repeat,a,stride,b,targetStride);
    //printf("end s_transCopy\n");
            return;
        }

        for(int pos=0; pos<repeat; pos++) {
            s_transCopy(
                ndim-1,shape+1,strides+1,targetStrides+1,
                a+(stride*pos),b+(targetStride*pos)
            );
        }

    //printf("end s_transCopy\n");
    }

    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t ndim,
        int32_t *shape,
        int32_t *perm,
        T *a,
        T *b,
        int32_t *strides,
        int32_t *targetStrides,
        int32_t stride,
        int32_t targetStride
    )
    {
        for(int32_t pos=cell.begin; pos<cell.end; pos++) {
            s_transCopy(
                ndim-1,shape+1,strides+1,targetStrides+1,
                a+(stride*pos),b+(targetStride*pos)
            );
        }
    }

public:
    static int32_t execute(
        int32_t ndim,
        int32_t *shape,
        int32_t *perm,
        T *a,
        T *b
    )
    {
        if(ndim<=0) {
            return RINDOW_MATLIB_E_MEM_ALLOC_FAILURE;
        }
        std::vector<int32_t> strides(ndim,0);
        std::vector<int32_t> targetStrides(ndim,0);
        int32_t stride = 1;
        int32_t targetStride = 1;
        for(int32_t dimDepth=ndim-1;dimDepth>=0;dimDepth--) {
            strides[dimDepth] = stride;
            stride *= shape[dimDepth];
            int32_t targDepth = perm[dimDepth];
            if(targDepth>=ndim) {
                return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
            }
            targetStrides[targDepth] = targetStride;
            targetStride *= shape[targDepth];
        }
        if(stride!=targetStride) {
            return RINDOW_MATLIB_E_DUP_AXIS;
        }

        ndim--;
        int32_t repeat = shape[0];
        stride = strides[0];
        targetStride = targetStrides[0];

        if(ndim<=0) {
            memcpy(b,a,repeat*sizeof(T));
            return RINDOW_MATLIB_SUCCESS;
        }

        ParallelOperation::execute(
            repeat,kernel,
            ndim,
            shape,
            perm,
            a,
            b,
            strides.data(),
            targetStrides.data(),
            stride,
            targetStride
        );
        return 0;
    }
};

}

extern "C" {
int32_t rindow_matlib_s_transpose(
    int32_t ndim,
    int32_t *shape,
    int32_t *perm,
    float *a,
    float *b
    )
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Transpose<float>::execute(
        ndim,
        shape,
        perm,
        a,
        b
    );
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_d_transpose(
    int32_t ndim,
    int32_t *shape,
    int32_t *perm,
    double *a,
    double *b
    )
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Transpose<double>::execute(
        ndim,
        shape,
        perm,
        a,
        b
    );
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_i_transpose(
    int32_t dtype,
    int32_t ndim,
    int32_t *shape,
    int32_t *perm,
    void *a,
    void *b
    )
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            errcode = Transpose<int8_t>::execute(ndim,shape,perm,(int8_t*)a,(int8_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            errcode = Transpose<uint8_t>::execute(ndim,shape,perm,(uint8_t*)a,(uint8_t*)b);
            break;
        }
        case rindow_matlib_dtype_int16: {
            errcode = Transpose<int16_t>::execute(ndim,shape,perm,(int16_t*)a,(int16_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            errcode = Transpose<uint16_t>::execute(ndim,shape,perm,(uint16_t*)a,(uint16_t*)b);
            break;
        }
        case rindow_matlib_dtype_int32: {
            errcode = Transpose<int32_t>::execute(ndim,shape,perm,(int32_t*)a,(int32_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            errcode = Transpose<uint32_t>::execute(ndim,shape,perm,(uint32_t*)a,(uint32_t*)b);
            break;
        }
        case rindow_matlib_dtype_int64: {
            errcode = Transpose<int64_t>::execute(ndim,shape,perm,(int64_t*)a,(int64_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            errcode = Transpose<uint64_t>::execute(ndim,shape,perm,(uint64_t*)a,(uint64_t*)b);
            break;
        }
        case rindow_matlib_dtype_bool: {
            errcode = Transpose<uint8_t>::execute(ndim,shape,perm,(uint8_t*)a,(uint8_t*)b);
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
