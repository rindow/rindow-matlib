#include "rindow/matlib.h"
#include "common.h"
#include <string.h>
#include <memory.h>

// float
static void s_transCopySub(
    int32_t n,
    float *a, int32_t incA,
    float *b, int32_t incB
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
    float *a,
    float *b
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

int32_t rindow_matlib_s_transpose(
    int32_t ndim,
    int32_t *shape,
    int32_t *perm,
    float *a,
    float *b
    )
{
    if(ndim<=0) {
        return RINDOW_MATLIB_E_MEM_ALLOC_FAILURE;
    }
    int32_t* strides = calloc(ndim,sizeof(int32_t));
    if(strides==NULL) {
        return RINDOW_MATLIB_E_MEM_ALLOC_FAILURE;
    }
    int32_t* targetStrides = calloc(ndim,sizeof(int32_t));
    if(targetStrides==NULL) {
        free(strides);
        return RINDOW_MATLIB_E_MEM_ALLOC_FAILURE;
    }
    int32_t stride = 1;
    int32_t targetStride = 1;
    for(int32_t dimDepth=ndim-1;dimDepth>=0;dimDepth--) {
        strides[dimDepth] = stride;
        stride *= shape[dimDepth];
        int32_t targDepth = perm[dimDepth];
        if(targDepth>=ndim) {
            free(strides);
            free(targetStrides);
            return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
        }
        targetStrides[targDepth] = targetStride;
        targetStride *= shape[targDepth];
    }
    if(stride!=targetStride) {
        free(strides);
        free(targetStrides);
        return RINDOW_MATLIB_E_DUP_AXIS;
    }

    ndim--;
    int32_t repeat = *shape;
    stride = *strides;
    targetStride = *targetStrides;

    if(ndim<=0) {
        memcpy(b,a,repeat*sizeof(float));
        return RINDOW_MATLIB_SUCCESS;
    }

    int pos;
    #pragma omp parallel for
    for(pos=0; pos<repeat; pos++) {
        s_transCopy(
            ndim-1,shape+1,strides+1,targetStrides+1,
            a+(stride*pos),b+(targetStride*pos)
        );
    }

    free(strides);
    free(targetStrides);
    return RINDOW_MATLIB_SUCCESS;
}


// double
static void d_transCopySub(
    int32_t n,
    double *a, int32_t incA,
    double *b, int32_t incB
)
{
    for(int i=0;i<n;i++) {
        *b = *a;
        a += incA;
        b += incB;
    }
}

static void d_transCopy(
    int32_t ndim,
    int32_t *shape,
    int32_t *strides,
    int32_t *targetStrides,
    double *a,
    double *b
    )
{
    int32_t repeat = *shape;
    int32_t stride = *strides;
    int32_t targetStride = *targetStrides;

    if(ndim<=0) {
        d_transCopySub(repeat,a,stride,b,targetStride);
        return;
    }

    for(int pos=0; pos<repeat; pos++) {
        d_transCopy(
            ndim-1,shape+1,strides+1,targetStrides+1,
            a+(stride*pos),b+(targetStride*pos));
    }
}

int32_t rindow_matlib_d_transpose(
    int32_t ndim,
    int32_t *shape,
    int32_t *perm,
    double *a,
    double *b
    )
{
    if(ndim<=0) {
        return RINDOW_MATLIB_E_MEM_ALLOC_FAILURE;
    }
    int32_t* strides = calloc(ndim,sizeof(int32_t));
    if(strides==NULL) {
        return RINDOW_MATLIB_E_MEM_ALLOC_FAILURE;
    }
    int32_t* targetStrides = calloc(ndim,sizeof(int32_t));
    if(targetStrides==NULL) {
        free(strides);
        return RINDOW_MATLIB_E_MEM_ALLOC_FAILURE;
    }
    int32_t stride = 1;
    int32_t targetStride = 1;
    for(int32_t dimDepth=ndim-1;dimDepth>=0;dimDepth--) {
        strides[dimDepth] = stride;
        stride *= shape[dimDepth];
        int32_t targDepth = perm[dimDepth];
        if(targDepth>=ndim) {
            free(strides);
            free(targetStrides);
            return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
        }
        targetStrides[targDepth] = targetStride;
        targetStride *= shape[targDepth];
    }
    if(stride!=targetStride) {
        free(strides);
        free(targetStrides);
        return RINDOW_MATLIB_E_DUP_AXIS;
    }

    ndim--;
    int32_t repeat = *shape;
    stride = *strides;
    targetStride = *targetStrides;

    if(ndim<=0) {
        memcpy(b,a,repeat*sizeof(double));
        return RINDOW_MATLIB_SUCCESS;
    }

    int pos;
    #pragma omp parallel for
    for(pos=0; pos<repeat; pos++) {
        d_transCopy(
            ndim-1,shape+1,strides+1,targetStrides+1,
            a+(stride*pos),b+(targetStride*pos)
        );
    }

    free(strides);
    free(targetStrides);
    return RINDOW_MATLIB_SUCCESS;
}

static void int_transCopy(
    int32_t dtype,
    int32_t ndim,
    int32_t *shape,
    int32_t *strides,
    int32_t *targetStrides,
    void *a,
    void *b
    )
{
    int32_t repeat = *shape;
    int32_t stride = *strides;
    int32_t targetStride = *targetStrides;

    if(ndim<=0) {
        rindow_matlib_common_copy_ex(dtype,repeat,a,stride,b,targetStride);
        return;
    }

    size_t value_bytes = rindow_matlib_common_dtype_to_valuesize(dtype);
    for(int pos=0; pos<repeat; pos++) {
        void* a_next = (int8_t*)a+(value_bytes*stride*pos);
        void* b_next = (int8_t*)b+(value_bytes*targetStride*pos);
        int_transCopy(
            dtype,ndim-1,shape+1,strides+1,targetStrides+1,
            a_next,b_next);
    }
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
    if(ndim<=0) {
        return RINDOW_MATLIB_E_MEM_ALLOC_FAILURE;
    }
    int32_t* strides = calloc(ndim,sizeof(int32_t));
    if(strides==NULL) {
        return RINDOW_MATLIB_E_MEM_ALLOC_FAILURE;
    }
    int32_t* targetStrides = calloc(ndim,sizeof(int32_t));
    if(targetStrides==NULL) {
        free(strides);
        return RINDOW_MATLIB_E_MEM_ALLOC_FAILURE;
    }
    int32_t stride = 1;
    int32_t targetStride = 1;
    for(int32_t dimDepth=ndim-1;dimDepth>=0;dimDepth--) {
        strides[dimDepth] = stride;
        stride *= shape[dimDepth];
        int32_t targDepth = perm[dimDepth];
        if(targDepth>=ndim) {
            free(strides);
            free(targetStrides);
            return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
        }
        targetStrides[targDepth] = targetStride;
        targetStride *= shape[targDepth];
    }
    if(stride!=targetStride) {
        free(strides);
        free(targetStrides);
        return RINDOW_MATLIB_E_DUP_AXIS;
    }

    ndim--;
    int32_t repeat = *shape;
    stride = *strides;
    targetStride = *targetStrides;
    size_t value_bytes = rindow_matlib_common_dtype_to_valuesize(dtype);

    if(ndim<=0) {
        memcpy(b,a,repeat*value_bytes);
        return RINDOW_MATLIB_SUCCESS;
    }

    int pos;
    #pragma omp parallel for
    for(pos=0; pos<repeat; pos++) {
        void* a_next = (int8_t*)a+(value_bytes*stride*pos);
        void* b_next = (int8_t*)b+(value_bytes*targetStride*pos);
        int_transCopy(
            dtype,ndim-1,shape+1,strides+1,targetStrides+1,
            a_next,b_next);
    }

    free(strides);
    free(targetStrides);
    return RINDOW_MATLIB_SUCCESS;
}
