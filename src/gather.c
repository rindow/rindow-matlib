#include "rindow/matlib.h"
#include "common.h"
#include <string.h>

#define RINDOW_MATLIB_GATHER_S_ADD_FUNC(k, from, to) \
    rindow_matlib_common_s_add(k, from, to)

#define RINDOW_MATLIB_GATHER_D_ADD_FUNC(k, from, to) \
    rindow_matlib_common_d_add(k, from, to)

#define RINDOW_MATLIB_GATHER_I_ADD_FUNC(k, from, to) \
    rindow_matlib_common_i_add(k, data_dtype, from, to)

#define RINDOW_MATLIB_GATHER_S_SETADD_FUNC(to,from) *to += *from
#define RINDOW_MATLIB_GATHER_D_SETADD_FUNC(to,from) *to += *from
#define RINDOW_MATLIB_GATHER_I_SETADD_FUNC(to,from) rindow_matlib_common_i_add(1, data_dtype, from, to)
#define RINDOW_MATLIB_GATHER_S_SETSET_FUNC(to,from) *to = *from
#define RINDOW_MATLIB_GATHER_D_SETSET_FUNC(to,from) *to = *from
#define RINDOW_MATLIB_GATHER_I_SETSET_FUNC(to,from) memcpy(to, from, value_width)

#define RINDOW_MATLIB_GATHER_S_GETINC_FUNC(sum,b,pos) sum += b[pos]
#define RINDOW_MATLIB_GATHER_D_GETINC_FUNC(sum,b,pos) sum += b[pos]
#define RINDOW_MATLIB_GATHER_I_GETINC_FUNC(sum,b,pos) rindow_matlib_common_i_add(1, data_dtype, &b[(pos)*value_width], &sum)
#define RINDOW_MATLIB_GATHER_S_SETINC_FUNC(a,pos,v) (a[pos] += v)
#define RINDOW_MATLIB_GATHER_D_SETINC_FUNC(a,pos,v) (a[pos] += v)
#define RINDOW_MATLIB_GATHER_I_SETINC_FUNC(a,pos,v) rindow_matlib_common_i_add(1,data_dtype,&v,&a[(pos)*value_width])

#define RINDOW_MATLIB_GATHER_S_POSADD_FUNC(a,offA,b,offB,pos) a[(offA)+(pos)] += b[(offB)+(pos)]
#define RINDOW_MATLIB_GATHER_D_POSADD_FUNC(a,offA,b,offB,pos) a[(offA)+(pos)] += b[(offB)+(pos)]
#define RINDOW_MATLIB_GATHER_I_POSADD_FUNC(a,offA,b,offB,pos) \
    rindow_matlib_common_i_add(1, data_dtype, &b[(offB)+(pos)*value_width], &a[(offA)+(pos)*value_width])

//
// serial copy and addition algorithm
//
#define RINDOW_MATLIB_GATHER_NORMAL_TEMPLATE(a,b,datatype,functype) \
    for(i=0; i<n; i++) { \
        int64_t selector = 0; \
        rindow_matlib_common_get_integer(dtype, x, 1, i, &selector); \
        if(selector<0||selector>=numClass) { \
            errcode = RINDOW_MATLIB_E_PERM_OUT_OF_RANGE; \
            continue; \
        } \
        datatype *from = &(a[selector*ldIndex]); \
        datatype *to = &(b[i*ldB]); \
        if(reverse) { \
            datatype *tmp; \
            tmp = from; \
            from = to; \
            to = tmp; \
        } \
        if(k==1) { \
            if(addMode) { \
                RINDOW_MATLIB_GATHER_##functype##_SETADD_FUNC(to,from); \
            } else { \
                RINDOW_MATLIB_GATHER_##functype##_SETSET_FUNC(to,from); \
            } \
        } else { \
            if(addMode) { \
                RINDOW_MATLIB_GATHER_##functype##_ADD_FUNC(k, from, to); \
            } else { \
                memcpy(to, from,k*value_width); \
            } \
        } \
    }


//
// parallel addition algorithm for numClass
//
#define RINDOW_MATLIB_GATHER_REVERSEADD_NUMCLASS_TEMPLATE(a,b,datatype,functype) \
    for(i=0;i<numClass;i++) { \
        for(int32_t p=0;p<k;p++) { \
            datatype sum = 0; \
            for(int32_t j=0;j<n;j++) { \
                int64_t selector = 0; \
                rindow_matlib_common_get_integer(dtype, x, 1, j, &selector); \
                if(selector==(int64_t)i) { \
                    RINDOW_MATLIB_GATHER_##functype##_GETINC_FUNC(sum,b,p+j*k); \
                } \
            } \
            RINDOW_MATLIB_GATHER_##functype##_SETINC_FUNC(a,i*k+p,sum); \
        } \
    }

//
// parallel addition algorithm for k
//
#define RINDOW_MATLIB_GATHER_REVERSEADD_TEMPLATE(a,b,datatype,functype) \
    for(i=0;i<k;i++) { \
        for(int32_t j=0;j<n;j++) { \
            int64_t selector = 0; \
            rindow_matlib_common_get_integer(dtype, x, 1, j, &selector); \
            if(selector<0||selector>=numClass) { \
                errcode = RINDOW_MATLIB_E_PERM_OUT_OF_RANGE; \
                continue; \
            } \
            RINDOW_MATLIB_GATHER_##functype##_POSADD_FUNC(a,selector*ldIndex,b,j*ldB,i); \
        } \
    }


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
    int32_t value_width = sizeof(float);

    int32_t errcode = 0;
    int32_t ldB = k;
    int32_t ldIndex = k;
    int64_t checktype;

    if(rindow_matlib_common_get_integer(dtype, x, 1, 0, &checktype)) {
        return RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
    }

    if(reverse&&addMode) {
        int32_t i;
        // parallel addition algorithm
        //#pragma omp parallel for
        //RINDOW_MATLIB_GATHER_REVERSEADD_TEMPLATE(a,b,float,S)

        // serial addition
        RINDOW_MATLIB_GATHER_NORMAL_TEMPLATE(a,b,float,S)
    } else {
        int32_t i;
        #pragma omp parallel for
        RINDOW_MATLIB_GATHER_NORMAL_TEMPLATE(a,b,float,S)
    }
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
    int32_t value_width = sizeof(double);

    int32_t errcode = 0;
    int32_t ldB = k;
    int32_t ldIndex = k;
    int64_t checktype;

    if(rindow_matlib_common_get_integer(dtype, x, 1, 0, &checktype)) {
        return RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
    }

    if(reverse&&addMode) {
        int32_t i;
        // parallel addition algorithm
        //#pragma omp parallel for
        //RINDOW_MATLIB_GATHER_REVERSEADD_TEMPLATE(a,b,double,D)

        // serial addition
        RINDOW_MATLIB_GATHER_NORMAL_TEMPLATE(a,b,double,D)
    } else {
        int32_t i;
        #pragma omp parallel for
        RINDOW_MATLIB_GATHER_NORMAL_TEMPLATE(a,b,double,D)
    }
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
    int32_t errcode = 0;
    int32_t ldB = k;
    int32_t ldIndex = k;
    int64_t checktype;

    if(rindow_matlib_common_get_integer(dtype, x, 1, 0, &checktype)) {
        return RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
    }
    if(!rindow_matlib_common_dtype_is_int(data_dtype) &&
        !rindow_matlib_common_dtype_is_bool(data_dtype)) {
        return RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
    }
    int32_t value_width = rindow_matlib_common_dtype_to_valuesize(data_dtype);
    if(value_width==0) {
        return RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
    }
    ldIndex *= value_width;
    ldB *= value_width;
    char *ax = a;
    char *bx = b;

    if(reverse&&addMode) {
        int32_t i;
        // parallel addition algorithm
        //#pragma omp parallel for
        //RINDOW_MATLIB_GATHER_REVERSEADD_TEMPLATE(ax,bx,int64_t,I)

        // serial addition
        RINDOW_MATLIB_GATHER_NORMAL_TEMPLATE(ax,bx,char,I)
    } else {
        int32_t i;
        #pragma omp parallel for
        RINDOW_MATLIB_GATHER_NORMAL_TEMPLATE(ax,bx,char,I)
    }

//    int32_t i;
//    #pragma omp parallel for
//    for(i=0; i<n; i++) {
//        int64_t selector;
//        rindow_matlib_common_get_integer(dtype, x, 1, i, &selector);
//        if(selector<0||selector>=numClass) {
//            errcode = RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
//            continue;
//        }
//        char *from = &(ax[selector*ldIndex*value_width]);
//        char *to = &(bx[i*ldB*value_width]);
//        if(reverse) {
//            char *tmp;
//            tmp = from;
//            from = to;
//            to = tmp;
//        }
//        if(addMode) {
//            rindow_matlib_common_i_add(k, data_dtype, from, to);
//        } else {
//            memcpy(to, from, k*value_width);
//        }
//    }

    return errcode;
}
