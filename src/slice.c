#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_SLICE_TEMPLATE(value_type,dtype,a,y,value_width) \
        int32_t pa = (i0+startAxis0)*n*k*size*incA+ \
                     (i1+startAxis1)*k*size*incA+ \
                     (i2+startAxis2)*size*incA; \
        int32_t py = i0*sizeAxis1*sizeAxis2*size+ \
                     i1*sizeAxis2*size*incY+ \
                     i2*size*incY; \
        value_type *from = &(a[pa*value_width]); \
        value_type *to   = &(y[py*value_width]); \
        int32_t fromInc = incA; \
        int32_t toInc   = incY; \
        if(reverse) {   \
            value_type *tmp;  \
            tmp = from; \
            from = to;  \
            to = tmp;   \
        } \
        if(addMode) { \
            rindow_matlib_common_add_ex(dtype, size, from, fromInc, to, toInc); \
        } else { \
            rindow_matlib_common_copy_ex(dtype, size, from, fromInc, to, toInc); \
        }

void rindow_matlib_s_slice(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t size,
    float *a, int32_t incA,
    float *y, int32_t incY,
    int32_t startAxis0,
    int32_t sizeAxis0,
    int32_t startAxis1,
    int32_t sizeAxis1,
    int32_t startAxis2,
    int32_t sizeAxis2
    )
{
    int32_t dtype = rindow_matlib_dtype_float32;
    if(sizeAxis0>sizeAxis2) {
        if(sizeAxis0>sizeAxis1) {
            // size0 > size1 > size2
            int32_t i0;
            #pragma omp parallel for
            for(i0=0;i0<sizeAxis0;i0++) {
                for(int32_t i1=0;i1<sizeAxis1;i1++){
                    for(int32_t i2=0;i2<sizeAxis2;i2++){
                        RINDOW_MATLIB_SLICE_TEMPLATE(float,dtype,a,y,1)
                    }
                }
            }
        } else {
            // size1 > size0 > size2
            int32_t i1;
            #pragma omp parallel for
            for(i1=0;i1<sizeAxis1;i1++){
                for(int32_t i0=0;i0<sizeAxis0;i0++) {
                    for(int32_t i2=0;i2<sizeAxis2;i2++){
                        RINDOW_MATLIB_SLICE_TEMPLATE(float,dtype,a,y,1)
                    }
                }
            }
        }
    } else {
            // size2 > size0 or size1
            int32_t i2;
            #pragma omp parallel for
            for(i2=0;i2<sizeAxis2;i2++){
                for(int32_t i0=0;i0<sizeAxis0;i0++) {
                    for(int32_t i1=0;i1<sizeAxis1;i1++){
                        RINDOW_MATLIB_SLICE_TEMPLATE(float,dtype,a,y,1)
                    }
                }
            }
    }
}

void rindow_matlib_d_slice(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t size,
    double *a, int32_t incA,
    double *y, int32_t incY,
    int32_t startAxis0,
    int32_t sizeAxis0,
    int32_t startAxis1,
    int32_t sizeAxis1,
    int32_t startAxis2,
    int32_t sizeAxis2
    )
{
    int32_t dtype = rindow_matlib_dtype_float64;
    if(sizeAxis0>sizeAxis2) {
        if(sizeAxis0>sizeAxis1) {
            // size0 > size1 > size2
            int32_t i0;
            #pragma omp parallel for
            for(i0=0;i0<sizeAxis0;i0++) {
                for(int32_t i1=0;i1<sizeAxis1;i1++){
                    for(int32_t i2=0;i2<sizeAxis2;i2++){
                        RINDOW_MATLIB_SLICE_TEMPLATE(double,dtype,a,y,1)
                    }
                }
            }
        } else {
            // size1 > size0 > size2
            int32_t i1;
            #pragma omp parallel for
            for(i1=0;i1<sizeAxis1;i1++){
                for(int32_t i0=0;i0<sizeAxis0;i0++) {
                    for(int32_t i2=0;i2<sizeAxis2;i2++){
                        RINDOW_MATLIB_SLICE_TEMPLATE(double,dtype,a,y,1)
                    }
                }
            }
        }
    } else {
            // size2 > size0 or size1
            int32_t i2;
            #pragma omp parallel for
            for(i2=0;i2<sizeAxis2;i2++){
                for(int32_t i0=0;i0<sizeAxis0;i0++) {
                    for(int32_t i1=0;i1<sizeAxis1;i1++){
                        RINDOW_MATLIB_SLICE_TEMPLATE(double,dtype,a,y,1)
                    }
                }
            }
    }
}

void rindow_matlib_i_slice(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t size,
    int32_t dtype,
    void *a, int32_t incA,
    void *y, int32_t incY,
    int32_t startAxis0,
    int32_t sizeAxis0,
    int32_t startAxis1,
    int32_t sizeAxis1,
    int32_t startAxis2,
    int32_t sizeAxis2
    )
{
    if(!rindow_matlib_common_dtype_is_int(dtype) &&
        !rindow_matlib_common_dtype_is_bool(dtype)) {
        return;
    }
    int32_t value_width = rindow_matlib_common_dtype_to_valuesize(dtype);
    if(value_width==0) {
        return;
    }
    char *ac = a;
    char *yc = y;

    if(sizeAxis0>sizeAxis2) {
        if(sizeAxis0>sizeAxis1) {
            // size0 > size1 > size2
            int32_t i0;
            #pragma omp parallel for
            for(i0=0;i0<sizeAxis0;i0++) {
                for(int32_t i1=0;i1<sizeAxis1;i1++){
                    for(int32_t i2=0;i2<sizeAxis2;i2++){
                        RINDOW_MATLIB_SLICE_TEMPLATE(char,dtype,ac,yc,value_width)
                    }
                }
            }
        } else {
            // size1 > size0 > size2
            int32_t i1;
            #pragma omp parallel for
            for(i1=0;i1<sizeAxis1;i1++){
                for(int32_t i0=0;i0<sizeAxis0;i0++) {
                    for(int32_t i2=0;i2<sizeAxis2;i2++){
                        RINDOW_MATLIB_SLICE_TEMPLATE(char,dtype,ac,yc,value_width)
                    }
                }
            }
        }
    } else {
            // size2 > size0 or size1
            int32_t i2;
            #pragma omp parallel for
            for(i2=0;i2<sizeAxis2;i2++){
                for(int32_t i0=0;i0<sizeAxis0;i0++) {
                    for(int32_t i1=0;i1<sizeAxis1;i1++){
                        RINDOW_MATLIB_SLICE_TEMPLATE(char,dtype,ac,yc,value_width)
                    }
                }
            }
    }
}
