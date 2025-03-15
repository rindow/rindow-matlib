#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Gathernd
{
private:
    static int32_t kernel_sub(
        int32_t i,
        int32_t j,
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t indexDepth,
        int32_t *paramShape,
        int32_t paramSize,
        T *a,
        int32_t *x,
        T *b
    )
    {
        int32_t errcode = 0;
        int32_t offset=0;
        for(int32_t h=0;h<indexDepth;++h) {
            offset *= paramShape[h];
            int32_t selector = x[i*n*indexDepth + j*indexDepth + h];
            if(selector<0||selector>=paramShape[h]) {
                errcode = RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
                break;
            }
            offset += selector;
        }
        if(errcode!=0) {
            return errcode;
        }
        int32_t selector = x[i];
        T *from = &a[i*paramSize*k + offset*k];
        T *to = &b[i*n*k + j*k];
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
        return errcode;
    }

    static int32_t kernel(
        ParallelOperation::cellInfo cell,
        bool para_m,
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t indexDepth,
        int32_t *paramShape,
        int32_t paramSize,
        T *a,
        int32_t *x,
        T *b
    )
    {
        int32_t errcode = 0;
        if(para_m) {
            for(int32_t i=cell.begin; i<cell.end; i++) {
                for(int32_t j=0; j<n; j++) {
                    int32_t ec = kernel_sub(
                        i,j,
                        reverse,addMode,
                        m,n,k,
                        indexDepth,paramShape,paramSize,
                        a,
                        x,
                        b
                    );
                    if(ec!=0) {
                        errcode = ec;
                    }
                }
            }
        } else {
            for(int32_t i=0; i<m; i++) {
                for(int32_t j=cell.begin; j<cell.end; j++) {
                    int32_t ec = kernel_sub(
                        i,j,
                        reverse,addMode,
                        m,n,k,
                        indexDepth,paramShape,paramSize,
                        a,
                        x,
                        b
                    );
                    if(ec!=0) {
                        errcode = ec;
                    }
                }
            }
        }
        return errcode;
    }

    static int32_t scatterAddKernel(
        ParallelOperation::cellInfo cell,
        int32_t blockSize,      // blockSize = m*paramSize*k
        T *a_buf,
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t indexDepth,
        int32_t *paramShape,
        int32_t paramSize,
        T *a,
        int32_t *x,
        T *b
    )
    {
        int32_t errcode = 0;
        for(int32_t i=0; i<m; i++) {
            for(int32_t j=cell.begin; j<cell.end; j++) {
                T *a_sub;
                if(cell.id==0) {
                    a_sub = a;
                } else {
                    //  a_sub: (m, (paramShape), k)
                    a_sub = &a_buf[(cell.id-1)*blockSize];
                }
                int32_t ec = kernel_sub(i,j,reverse,addMode,m,n,k,indexDepth,paramShape,paramSize,a_sub,x,b);
                if(ec) {
                    errcode = ec;
                }
            }
        }
        return errcode;
    }

    static void sumKernel(
        ParallelOperation::cellInfo cell,
        int32_t parallel_n,
        int32_t blockSize,      // blockSize = m*paramSize*k
        T *a,
        T *a_buf
    )
    {
        for(int32_t pos=cell.begin; pos<cell.end; pos++) {
            for(int32_t thread_id=1; thread_id<parallel_n; thread_id++) {
                // (parallel_n-1)*[m,p0,p1,k]
                a[pos] += a_buf[blockSize*(thread_id-1)+pos];
            }
        }
    }

    static int32_t scatterAdd(
        int32_t parallel_n,
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t indexDepth,
        int32_t *paramShape,
        int32_t paramSize,
        T *a,
        int32_t *x,
        T *b
    )
    {
        int32_t value_width = sizeof(T);
        int32_t errcode = 0;
        // n*[m,p0,p1,k]
        int32_t blockSize = m*paramSize*k;
        std::vector<T> a_buf((parallel_n-1)*blockSize, 0);

        errcode = ParallelOperation::executeAndGetCode<int32_t>(
            n,scatterAddKernel,
            blockSize,a_buf.data(),
            reverse,addMode,m,n,k,indexDepth,paramShape,paramSize,a,x,b
        );
        if(errcode) {
            return errcode;
        }
        ParallelOperation::execute(blockSize,sumKernel,parallel_n,blockSize,a,a_buf.data());
        return errcode;
    }

public:
    /**
     * A: (m, (paramShape), k)
     * X: (m, n, index_depth)
     * B: (m, n, k)
     * B(m, n, k) := A(m,(X(m,n)),k)
     */
    static int32_t execute(
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t indexDepth,
        int32_t *paramShape,
        T *a,
        int32_t *x,
        T *b
    )
    {
        if(m <= 0 || n <= 0 || k <= 0) {
            return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
        }
        int32_t paramSize = 1;
        for(int32_t h=0; h<indexDepth; ++h) {
            if(paramShape[h]<=0) {
                return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
            }
            paramSize *= paramShape[h];
        }
        int32_t parallel_n = (int32_t)ParallelOperation::getMaxThreads();
        if(n<parallel_n) {
            parallel_n = n;
        }
        bool para_m = false;
        bool scatteradd_mode = false;
        if(reverse&&addMode) {
            if(m>=parallel_n) {
                para_m = true;
                scatteradd_mode = false;
            } else {
                para_m = false;
                scatteradd_mode = true;
            }
        } else {
            if(m>=n) {
                para_m = true;
                scatteradd_mode = false;
            } else {
                para_m = false;
                scatteradd_mode = false;
            }
        }

        if(scatteradd_mode) {
            return scatterAdd(parallel_n,reverse,addMode,m,n,k,indexDepth,paramShape,paramSize,a,x,b);
        } else {
            int32_t parallel;
            if(para_m) {
                parallel = m;
            } else {
                parallel = n;
            }
            return ParallelOperation::executeAndGetCode<int32_t>(
                parallel,kernel,
                para_m,
                reverse,addMode,m,n,k,indexDepth,paramShape,paramSize,a,x,b
            );
        }
    }
};

}

extern "C" {
int32_t rindow_matlib_s_gathernd(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t indexDepth,
    int32_t *paramShape,
    float *a,
    int32_t *x,
    float *b
)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Gathernd<float>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,a,x,b);
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_d_gathernd(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t indexDepth,
    int32_t *paramShape,
    double *a,
    int32_t *x,
    double *b
)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Gathernd<double>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,a,x,b);
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_i_gathernd(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t indexDepth,
    int32_t *paramShape,
    int32_t dtype,
    void *a,
    int32_t *x,
    void *b
)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            errcode = Gathernd<int8_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(int8_t*)a,x,(int8_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            errcode = Gathernd<uint8_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(uint8_t*)a,x,(uint8_t*)b);
            break;
        }
        case rindow_matlib_dtype_int16: {
            errcode = Gathernd<int16_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(int16_t*)a,x,(int16_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            errcode = Gathernd<uint16_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(uint16_t*)a,x,(uint16_t*)b);
            break;
        }
        case rindow_matlib_dtype_int32: {
            errcode = Gathernd<int32_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(int32_t*)a,x,(int32_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            errcode = Gathernd<uint32_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(uint32_t*)a,x,(uint32_t*)b);
            break;
        }
        case rindow_matlib_dtype_int64: {
            errcode = Gathernd<int64_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(int64_t*)a,x,(int64_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            errcode = Gathernd<uint64_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(uint64_t*)a,x,(uint64_t*)b);
            break;
        }
        case rindow_matlib_dtype_bool: {
            errcode = Gathernd<uint8_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(uint8_t*)a,x,(uint8_t*)b);
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

