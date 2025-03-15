#include "rindow/matlib.h"
#include "common.hpp"
#include <vector>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Einsum4p1
{
private:
    static void calc_indices(
        int32_t index,
        int32_t dim0,
        int32_t dim1,
        int32_t dim2,
        int32_t dim3,
        int32_t *idx0,
        int32_t *idx1,
        int32_t *idx2,
        int32_t *idx3
    )
    {
        *idx3 = index % dim3;
        index /= dim3;
        *idx2 = index % dim2;
        index /= dim2;
        *idx1 = index % dim1;
        index /= dim1;
        *idx0 = index;
    }

    static int32_t kernel(
        ParallelOperation::cellInfo cell,
        int32_t dim0,
        int32_t dim1,
        int32_t dim2,
        int32_t dim3,
        int32_t dim4,
        T *a,
        int32_t ldA0,
        int32_t ldA1,
        int32_t ldA2,
        int32_t ldA3,
        int32_t ldA4,
        T *b,
        int32_t ldB0,
        int32_t ldB1,
        int32_t ldB2,
        int32_t ldB3,
        int32_t ldB4,
        T *c
    )
    {
        int32_t idx0;
        int32_t idx1;
        int32_t idx2;
        int32_t idx3;
        int32_t indexC = cell.begin;

        calc_indices(
            cell.begin,
            dim0,
            dim1,
            dim2,
            dim3,
            &idx0,
            &idx1,
            &idx2,
            &idx3
        );
        for(; idx0<dim0; idx0++) {
            for(; idx1<dim1; idx1++) {
                for(; idx2<dim2; idx2++) {
                    for(; idx3<dim3; idx3++) {
                        int32_t indexA =
                            idx0*ldA0 +
                            idx1*ldA1 +
                            idx2*ldA2 +
                            idx3*ldA3;
                        int32_t indexB =
                            idx0*ldB0 +
                            idx1*ldB1 +
                            idx2*ldB2 +
                            idx3*ldB3;
                        T sumC = 0;
                        for(int32_t idx4=0; idx4<dim4; idx4++) {
                            sumC += a[indexA+idx4*ldA4]*b[indexB+idx4*ldB4];
                        }
                        c[indexC] = sumC;
                        indexC++;
                        if(indexC>=cell.end) {
                            return 0;
                        }
                    }
                    idx3 = 0;
                }
                idx2 = 0;
            }
            idx1 = 0;
        }
        return 0;
    }

public:
    static int32_t execute(
        int32_t dim0,
        int32_t dim1,
        int32_t dim2,
        int32_t dim3,
        int32_t dim4,
        T *a,
        int32_t ldA0,
        int32_t ldA1,
        int32_t ldA2,
        int32_t ldA3,
        int32_t ldA4,
        T *b,
        int32_t ldB0,
        int32_t ldB1,
        int32_t ldB2,
        int32_t ldB3,
        int32_t ldB4,
        T *c
    )
    {
        if(dim0<=0 || dim1<=0 || dim2<=0 || dim3<=0 || dim4<=0 ||
            ldA0<0 || ldA1<0 || ldA2<0 || ldA3<0 || ldA4<0 ||
            ldB0<0 || ldB1<0 || ldB2<0 || ldB3<0 || ldB4<0) {
            return RINDOW_MATLIB_E_INVALID_SHAPE_OR_PARAM;
        }
        int32_t size=dim0*dim1*dim2*dim3;

        int32_t errcode = ParallelOperation::executeAndGetCode<int32_t>(
            size,      // num of items for parallel
            kernel,
            dim0,
            dim1,
            dim2,
            dim3,
            dim4,
            a,
            ldA0,
            ldA1,
            ldA2,
            ldA3,
            ldA4,
            b,
            ldB0,
            ldB1,
            ldB2,
            ldB3,
            ldB4,
            c
        );
        return errcode;
    }
};

}

extern "C" {
int32_t rindow_matlib_s_einsum4p1(
    int32_t dim0,
    int32_t dim1,
    int32_t dim2,
    int32_t dim3,
    int32_t dim4,
    float *a,
    int32_t ldA0,
    int32_t ldA1,
    int32_t ldA2,
    int32_t ldA3,
    int32_t ldA4,
    float *b,
    int32_t ldB0,
    int32_t ldB1,
    int32_t ldB2,
    int32_t ldB3,
    int32_t ldB4,
    float *c
)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Einsum4p1<float>::execute(
        dim0,
        dim1,
        dim2,
        dim3,
        dim4,
        a,
        ldA0,
        ldA1,
        ldA2,
        ldA3,
        ldA4,
        b,
        ldB0,
        ldB1,
        ldB2,
        ldB3,
        ldB4,
        c
    );
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_d_einsum4p1(
    int32_t dim0,
    int32_t dim1,
    int32_t dim2,
    int32_t dim3,
    int32_t dim4,
    double *a,
    int32_t ldA0,
    int32_t ldA1,
    int32_t ldA2,
    int32_t ldA3,
    int32_t ldA4,
    double *b,
    int32_t ldB0,
    int32_t ldB1,
    int32_t ldB2,
    int32_t ldB3,
    int32_t ldB4,
    double *c
)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Einsum4p1<double>::execute(
        dim0,
        dim1,
        dim2,
        dim3,
        dim4,
        a,
        ldA0,
        ldA1,
        ldA2,
        ldA3,
        ldA4,
        b,
        ldB0,
        ldB1,
        ldB2,
        ldB3,
        ldB4,
        c
    );
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}
}
