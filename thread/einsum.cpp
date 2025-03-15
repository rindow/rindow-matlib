#include "rindow/matlib.h"
#include "common.hpp"
#include <vector>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Einsum
{
private:
    static void calc_indices(
        const int32_t depth,
        const int32_t ndim,
        const int32_t index,
        const int32_t *sizeOfIndices,
        std::vector<int32_t> &indices
    )
    {
        int32_t idx = index;
        for(int32_t axis=ndim-1; axis>=0; axis--) {
            int32_t size = sizeOfIndices[axis];
            indices[axis] = idx % size;
            idx /= size;
        }
    }

    static int32_t calc_index(
        const int32_t depth,
        std::vector<int32_t> &indices,  
        const int32_t *lds
    )
    {
        int32_t index = 0;
        for(int32_t axis=0; axis<depth; axis++) {
            index += indices[axis]*lds[axis];
        }
        return index;
    }

    static int32_t next_indices(
        const int32_t depth,
        const int32_t ndim,
        const int32_t *sizeOfIndices,
        std::vector<int32_t> &indices
    ) {
        int32_t i = depth - 1;
        while(i >= 0) {
            if(indices[i] < sizeOfIndices[i]-1) {
                break;
            }
            indices[i] = 0;
            i--;
        }
        if(i>=0) {
            indices[i]++;
        }
        if(i < ndim) {
            // done all
            return 0;
        }
        return 1;
    }

    static int32_t kernel(
        ParallelOperation::cellInfo cell,
        const int32_t depth,
        const int32_t *sizeOfIndices,
        const T *a,
        const int32_t *ldA,
        const T *b,
        const int32_t *ldB,
        T *c,
        const int32_t ndimC
    )
    {
        std::vector<int32_t> indices(depth,0);
        calc_indices(depth,ndimC,cell.begin,sizeOfIndices,indices);
        for(int32_t indexC=cell.begin; indexC<cell.end; indexC++) {
            T sumC = 0;
            for(;;) {
                int32_t indexA = calc_index(depth,indices,ldA);
                if(indexA<0) {
                    return indexA-1000;
                }
                int32_t indexB = calc_index(depth,indices,ldB);
                if(indexB<0) {
                    return indexB-2000;
                }
                sumC += a[indexA]*b[indexB];
    
                // next indices
                if(!next_indices(depth,ndimC,sizeOfIndices,indices)) {
                    break;
                }
            }
            c[indexC] = sumC;
        }
        return 0;
    }

public:
    static int32_t execute(
        const int32_t depth,
        const int32_t *sizeOfIndices,
        const T *a,
        const int32_t *ldA,
        const T *b,
        const int32_t *ldB,
        T *c,
        const int32_t ndimC
    )
    {
        if(depth<=0 || ndimC<=0) {
            return RINDOW_MATLIB_E_MEM_ALLOC_FAILURE;
        }
        if(depth<ndimC) {
            return RINDOW_MATLIB_E_INVALID_SHAPE_OR_PARAM;
        }
        int32_t sizeC=1;
        for(int32_t i=0;i<ndimC;i++) {
            sizeC *= sizeOfIndices[i];
        }

        int32_t errcode = ParallelOperation::executeAndGetCode<int32_t>(
            sizeC,      // num of items for parallel
            kernel,
            depth,
            sizeOfIndices,
            a,
            ldA,
            b,
            ldB,
            c,
            ndimC
        );
        return errcode;
    }
};

}

extern "C" {
int32_t rindow_matlib_s_einsum(
    const int32_t depth,
    const int32_t *sizeOfIndices,
    const float *a,
    const int32_t *ldA,
    const float *b,
    const int32_t *ldB,
    float *c,
    const int32_t ndimC
)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Einsum<float>::execute(
        depth,
        sizeOfIndices,
        a,
        ldA,
        b,
        ldB,
        c,
        ndimC
    );
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_d_einsum(
    const int32_t depth,
    const int32_t *sizeOfIndices,
    const double *a,
    const int32_t *ldA,
    const double *b,
    const int32_t *ldB,
    double *c,
    const int32_t ndimC
    )
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Einsum<double>::execute(
        depth,
        sizeOfIndices,
        a,
        ldA,
        b,
        ldB,
        c,
        ndimC
    );
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}
}
