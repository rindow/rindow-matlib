#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Gatherb
{
private:
    static int32_t kernel_sub(
        int32_t batch_id,
        int32_t i,
        int32_t j,
        int32_t h,
        int32_t reverse,
        int32_t addMode,
        int32_t batches,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t len,
        int32_t numClass,
        T *a,
        int32_t *x,
        T *b
        )
    {
        int32_t selector = x[(batch_id*n+j)*k+h];
        if(selector<0||selector>=numClass) {
            return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
        }

        T *from = &a[(((batch_id*m+i)*numClass+selector)*k+h)*len];
        T *to = &b[(((batch_id*m+i)*n+j)*k+h)*len];
        if(reverse) {
            T *tmp;
            tmp = from;
            from = to;
            to = tmp;
        }
        if(len==1) {
            if(addMode) {
                *to += *from;
            } else {
                *to = *from;
            }
        } else {
            if(addMode) {
                for(int32_t pos=0;pos<len;++pos) {
                    *to += *from;
                    to++;
                    from++;
                }
            } else {
                int32_t value_width = sizeof(T);
                memcpy(to, from, len*value_width);
            }
        }
        return 0;
    }

    static int32_t kernel(
        ParallelOperation::cellInfo cell,
        bool para_m,
        int32_t reverse,
        int32_t addMode,
        int32_t batches,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t len,
        int32_t numClass,
        T *a,
        int32_t *x,
        T *b
    )
    {
        int32_t errcode = 0;
        if(para_m) { // parallel for batchs
            for(int32_t id=cell.begin; id<cell.end; id++) {
                int32_t batch_id = id/m;
                int32_t i = id%m;
                for(int32_t j=0; j<n; j++) {
                    for(int32_t h=0; h<k; h++) {
                        int32_t ec = kernel_sub(batch_id,i,j,h,reverse,addMode,batches,m,n,k,len,numClass,a,x,b);
                        if(ec) {
                            errcode = ec;
                        }
                    }
                }
            }
        } else { // parallel for broadcast on n
            for(int32_t j=cell.begin; j<cell.end; j++) {
                for(int32_t h=0; h<k; h++) {
                    for(int32_t batch_id=0; batch_id<batches; batch_id++) {
                        for(int32_t i=0;i<m;i++) {
                            int32_t ec = kernel_sub(batch_id,i,j,h,reverse,addMode,batches,m,n,k,len,numClass,a,x,b);
                            if(ec) {
                                errcode = ec;
                            }
                        }
                    }
                }
            }
        }
        return errcode;
    }

    static int32_t scatterAddKernel(
        ParallelOperation::cellInfo cell,
        int32_t blockSize,
        T *a_buf,
        int32_t reverse,
        int32_t addMode,
        int32_t batches,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t len,
        int32_t numClass,
        T *a,
        int32_t *x,
        T *b
    )
    {
        int32_t errcode = 0;
        for(int32_t batch_id=0; batch_id<batches; batch_id++) {
            for(int32_t i=0; i<m; i++) {
                for(int32_t j=cell.begin; j<cell.end; j++) {
                    for(int32_t h=0; h<k; h++) {
                        T *a_sub;
                        if(cell.id==0) {
                            a_sub = a;
                        } else {
                            a_sub = &a_buf[(cell.id-1)*blockSize];
                        }
                        int32_t ec = kernel_sub(batch_id,i,j,h,reverse,addMode,batches,m,n,k,len,numClass,a_sub,x,b);
                        if(ec) {
                            errcode = ec;
                        }
                    }
                }
            }
        }

        return errcode;
    }

    static void sumKernel(
        ParallelOperation::cellInfo cell,
        int32_t parallel_n,
        int32_t blockSize,
        T *a,
        T *a_buf
    )
    {
        for(int32_t pos=cell.begin; pos<cell.end; pos++) {
            for(int32_t thread_id=1; thread_id<parallel_n; thread_id++) {
                // n*[batches,m,numClass,k,len]
                a[pos] += a_buf[(thread_id-1)*blockSize+pos];
            }
        }
    }

    static int32_t scatterAdd(
        int32_t parallel_n,
        int32_t reverse,
        int32_t addMode,
        int32_t batches,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t len,
        int32_t numClass,
        T *a,
        int32_t *x,
        T *b
    )
    {
        int32_t value_width = sizeof(T);
        int32_t errcode = 0;
        int32_t size_m = batches*m;

        // n*[m,p0,p1,k]
        int32_t blockSize = size_m*numClass*k*len;
        std::vector<T> a_buf((parallel_n-1)*blockSize, 0);

        errcode = ParallelOperation::executeAndGetCode<int32_t>(
            n,scatterAddKernel,
            blockSize,a_buf.data(),
            reverse,addMode,batches,m,n,k,len,numClass,a,x,b
        );
        if(errcode) {
            return errcode;
        }
        ParallelOperation::execute(blockSize,sumKernel,parallel_n,blockSize,a,a_buf.data());
        return errcode;
    }

public:
    static int32_t execute(
        int32_t reverse,
        int32_t addMode,
        int32_t batches,        // batch num
        int32_t m,              // outer
        int32_t n,              // broadcast
        int32_t k,              // inner
        int32_t len,            // details
        int32_t numClass,       // indexParams
        T *a,                   // params
        int32_t *x,             // indices
        T *b                    // updates
    )
    {
        int32_t value_width = sizeof(T);
        int32_t errcode = 0;
        int32_t ldB = k;
        if(batches<=0||m<=0||n<=0||len<=0||numClass<=0) {
            return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
        }
        int32_t parallel_n = (int32_t)ParallelOperation::getMaxThreads();
        if(n<parallel_n) {
            parallel_n = n;
        }
        int32_t size_m = batches*m;
        bool para_m = false;
        bool scatteradd_mode = false;
        if(reverse&&addMode) {
            if(size_m>=parallel_n) {
                para_m = true;
                scatteradd_mode = false;
            } else {
                para_m = false;
                scatteradd_mode = true;
            }
        } else {
            if(size_m>=n) {
                para_m = true;
                scatteradd_mode = false;
            } else {
                para_m = false;
                scatteradd_mode = false;
            }
        }

        if(!scatteradd_mode) {
            int32_t parallel;
            if(para_m) {
                parallel = size_m;
            } else {
                parallel = n;
            }
            return ParallelOperation::executeAndGetCode<int32_t>(
                parallel,kernel,
                para_m,
                reverse,addMode,batches,m,n,k,len,numClass,a,x,b
            );
        } else { // broadcast on reverse and addmode
            return scatterAdd(parallel_n,reverse,addMode,batches,m,n,k,len,numClass,a,x,b);
        }

        return errcode;
    }
};

}


extern "C" {
int32_t rindow_matlib_s_gatherb(
    int32_t reverse,
    int32_t addMode,
    int32_t batches,        // batch num
    int32_t m,              // outer
    int32_t n,              // broadcast
    int32_t k,              // inner
    int32_t len,            // details
    int32_t numClass,       // indexParams
    float *a,               // params
    int32_t *x,             // indices
    float *b                // updates
)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Gatherb<float>::execute(
        reverse,
        addMode,
        batches,
        m,
        n,
        k,
        len,
        numClass,
        a,
        x,
        b
    );
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_d_gatherb(
    int32_t reverse,
    int32_t addMode,
    int32_t batches,        // batch num
    int32_t m,              // outer
    int32_t n,              // broadcast
    int32_t k,              // inner
    int32_t len,            // details
    int32_t numClass,       // indexParams
    double *a,               // params
    int32_t *x,             // indices
    double *b                // updates
)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Gatherb<double>::execute(
        reverse,
        addMode,
        batches,
        m,
        n,
        k,
        len,
        numClass,
        a,
        x,
        b
    );
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_i_gatherb(
    int32_t reverse,
    int32_t addMode,
    int32_t batches,        // batch num
    int32_t m,              // outer
    int32_t n,              // broadcast
    int32_t k,              // inner
    int32_t len,            // details
    int32_t numClass,       // indexParams
    int32_t dtype,
    void *a,               // params
    int32_t *x,             // indices
    void *b                // updates
)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            errcode = Gatherb<int8_t>::execute(reverse,addMode,batches,m,n,k,len,numClass,(int8_t*)a,x,(int8_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            errcode = Gatherb<uint8_t>::execute(reverse,addMode,batches,m,n,k,len,numClass,(uint8_t*)a,x,(uint8_t*)b);
            break;
        }
        case rindow_matlib_dtype_int16: {
            errcode = Gatherb<int16_t>::execute(reverse,addMode,batches,m,n,k,len,numClass,(int16_t*)a,x,(int16_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            errcode = Gatherb<uint16_t>::execute(reverse,addMode,batches,m,n,k,len,numClass,(uint16_t*)a,x,(uint16_t*)b);
            break;
        }
        case rindow_matlib_dtype_int32: {
            errcode = Gatherb<int32_t>::execute(reverse,addMode,batches,m,n,k,len,numClass,(int32_t*)a,x,(int32_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            errcode = Gatherb<uint32_t>::execute(reverse,addMode,batches,m,n,k,len,numClass,(uint32_t*)a,x,(uint32_t*)b);
            break;
        }
        case rindow_matlib_dtype_int64: {
            errcode = Gatherb<int64_t>::execute(reverse,addMode,batches,m,n,k,len,numClass,(int64_t*)a,x,(int64_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            errcode = Gatherb<uint64_t>::execute(reverse,addMode,batches,m,n,k,len,numClass,(uint64_t*)a,x,(uint64_t*)b);
            break;
        }
        case rindow_matlib_dtype_bool: {
            errcode = Gatherb<uint8_t>::execute(reverse,addMode,batches,m,n,k,len,numClass,(uint8_t*)a,x,(uint8_t*)b);
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
