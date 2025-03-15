#include "rindow/matlib.h"
#include "common.h"
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <exception>

namespace {

template <typename T>
/*
 * This function is unofficial.
 * It may be removed or changed without notice.
*/
class Gathernd
{
private:
    static int32_t kernel(
        int32_t i,
        int32_t j,
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t indexDepth,
        int32_t paramShape[],
        int32_t paramSize,
        T a[],
        int32_t x[],
        T b[]
        )
    {
        int32_t errcode = 0;
        int32_t offset=0;
        for(int32_t h=0; h<indexDepth; ++h) {
            offset *= paramShape[h];
            int32_t selector = x[i*n*indexDepth + j*indexDepth + h];
            if(selector<0||selector>=paramShape[h]) {
                errcode = RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
                break;
            }
            offset += selector;
        }
        if(errcode) {
            return errcode;
        }
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
                for(int32_t pos=0;pos<k;++pos) {
                    *to += *from;
                    to++;
                    from++;
                }
            } else {
                int32_t value_width = sizeof(T);
                memcpy(to, from,k*value_width);
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
        int32_t k,
        int32_t indexDepth,
        int32_t paramShape[],
        T a[],
        int32_t x[],
        T b[]
    )
    {
        int32_t value_width = sizeof(T);
        int32_t errcode = 0;
        int32_t ldB = k;
        if(m<=0||n<=0) {
            return 0;
        }
        int32_t paramSize = 1;
        for(int32_t h=0; h<indexDepth; ++h) {
            if(paramShape[h]<=0) {
                return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
            }
            paramSize *= paramShape[h];
        }
        int32_t parallel_n = rindow_matlib_common_get_num_threads();
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

        if(!scatteradd_mode) {
            if(para_m) { // parallel for batchs
                int32_t i;
                #pragma omp parallel for
                for(i=0;i<m;i++) {
                    for(int32_t j=0; j<n; j++) {
                        int32_t ec = kernel(i,j,reverse,addMode,m,n,k,indexDepth,paramShape,paramSize,a,x,b);
                        if(ec) {
                            errcode = ec;
                        }
                    }
                }
            } else { // parallel for broadcast on n
                int32_t j;
                #pragma omp parallel for
                for(j=0; j<n; j++) {
                    for(int32_t i=0;i<m;i++) {
                        int32_t ec = kernel(i,j,reverse,addMode,m,n,k,indexDepth,paramShape,paramSize,a,x,b);
                        if(ec) {
                            errcode = ec;
                        }
                    }
                }
            }
        } else { // broadcast on reverse and addmode
            int32_t cell_size = n / parallel_n;
            int32_t remainder = n - cell_size * parallel_n;
            // n*[m,p0,p1,k]
            T *a_buf = (T*)calloc((parallel_n-1)*m*paramSize*k,value_width);
            if(a_buf==NULL) {
                return RINDOW_MATLIB_E_MEM_ALLOC_FAILURE;
            }

            int32_t thread_id;
            #pragma omp parallel for
            for(thread_id=0; thread_id<parallel_n; thread_id++) {
                int32_t begin;
                int32_t end;
                begin = thread_id * cell_size;
                if(thread_id == parallel_n - 1) {
                    end = (thread_id+1) * cell_size + remainder;
                } else {
                    end = (thread_id+1) * cell_size;
                }

                for(int32_t i=0; i<m; i++) {
                    for(int32_t j=begin; j<end; j++) {
                        T *a_sub;
                        if(thread_id==0) {
                            a_sub = a;
                        } else {
                            a_sub = &a_buf[(thread_id-1)*m*paramSize*k];
                        }
                        int32_t ec = kernel(i,j,reverse,addMode,m,n,k,indexDepth,paramShape,paramSize,a_sub,x,b);
                        if(ec) {
                            errcode = ec;
                        }
                    }
                }
            }

            int32_t pos=0;
            #pragma omp parallel for
            for(pos=0;pos<m*paramSize*k;pos++) {
                for(int32_t thread_id=1; thread_id<parallel_n; thread_id++) {
                    // n*[m,p0,p1,k]
                    a[pos] += a_buf[m*paramSize*k*(thread_id-1)+pos];
                }
            }
            free(a_buf);
        }

        return errcode;
    }
};

}


extern "C" {
int32_t rindow_matlib_s_gathernd(
    int32_t reverse,
    int32_t addMode,
    int32_t m,              // batch num
    int32_t n,              // broadcast
    int32_t k,              // details
    int32_t indexDepth,     // depth of indexParams
    int32_t *paramShape,    // indexParams
    float *a,               // params
    int32_t *x,             // indices
    float *b                // expanded elements
)
{
    int32_t errcode;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Gathernd<float>::execute(
        reverse,
        addMode,
        m,
        n,
        k,
        indexDepth,
        paramShape,
        a,
        x,
        b
    );
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_d_gathernd(
    int32_t reverse,
    int32_t addMode,
    int32_t m,              // batch num
    int32_t n,              // broadcast
    int32_t k,              // details
    int32_t indexDepth,     // depth of indexParams
    int32_t *paramShape,    // indexParams
    double *a,              // params
    int32_t *x,             // indices
    double *b               // expanded elements
)
{
    int32_t errcode;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Gathernd<double>::execute(
        reverse,
        addMode,
        m,
        n,
        k,
        indexDepth,
        paramShape,
        a,
        x,
        b
    );
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
    int32_t errcode;
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

