#include "rindow/matlib.h"
#include "common.h"
#include <string.h>


#define RINDOW_MATLIB_REPEAT_TEMPLATE(a,b) \
        memcpy(&(b[i*repeats*ldB+j*ldB]), &(a[i*ldA]), bytes); \


void rindow_matlib_s_repeat(
    int32_t m,
    int32_t k,
    int32_t repeats,
    float *a,
    float *b
)
{
    int32_t ldA = k;
    int32_t ldB = k;
    int32_t bytes = k*sizeof(float);
    if(m>repeats) {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<m; i++) {
            for(int32_t j=0; j<repeats; j++) {
                RINDOW_MATLIB_REPEAT_TEMPLATE(a,b)
            }
        }
   } else {
        int32_t j;
        #pragma omp parallel for
        for(j=0; j<repeats; j++) {
            for(int32_t i=0; i<m; i++) {
                RINDOW_MATLIB_REPEAT_TEMPLATE(a,b)
            }
        }
   }
}

void rindow_matlib_d_repeat(
    int32_t m,
    int32_t k,
    int32_t repeats,
    double *a,
    double *b
)
{
    int32_t ldA = k;
    int32_t ldB = k;
    int32_t bytes = k*sizeof(double);
    if(m>repeats) {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<m; i++) {
            for(int32_t j=0; j<repeats; j++) {
                RINDOW_MATLIB_REPEAT_TEMPLATE(a,b)
            }
        }
   } else {
        int32_t j;
        #pragma omp parallel for
        for(j=0; j<repeats; j++) {
            for(int32_t i=0; i<m; i++) {
                RINDOW_MATLIB_REPEAT_TEMPLATE(a,b)
            }
        }
   }
}

void rindow_matlib_i_repeat(
    int32_t m,
    int32_t k,
    int32_t repeats,
    int32_t dtype,
    void *a,
    void *b
)
{
    int32_t value_width = rindow_matlib_common_dtype_to_valuesize(dtype);
    if(value_width==0) {
        return;
    }
    int32_t ldA = k*value_width;
    int32_t ldB = k*value_width;
    int32_t bytes = k*value_width;
    char *ac=a;
    char *bc=b;
    if(m>repeats) {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<m; i++) {
            for(int32_t j=0; j<repeats; j++) {
                RINDOW_MATLIB_REPEAT_TEMPLATE(ac,bc)
            }
        }
   } else {
        int32_t j;
        #pragma omp parallel for
        for(j=0; j<repeats; j++) {
            for(int32_t i=0; i<m; i++) {
                RINDOW_MATLIB_REPEAT_TEMPLATE(ac,bc)
            }
        }
   }
}

