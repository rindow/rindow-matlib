#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

const int32_t mode_set = 0;
const int32_t mode_add = 1;
const int32_t mode_or = 10000;

#define RINDOW_MATLIB_MASKING_APPLY_TEMPLATE(a,index,value) \
    if(mode==mode_set) { \
        a[index] =  value; \
    } else if(mode==mode_or) { \
        a[index] = a[index] ? a[index] : value; \
    } else { \
        a[index] +=  value; \
    } \

// *** CAUTION ***
#define RINDOW_MATLIB_MASKING_TEMPLATE(n,k,len,i,j,fill,x,a) \
    for(int32_t h=0; h<k; h++) { \
        if(!x[i*k+h]) { \
            for(int l=0;l<len;l++) { \
                RINDOW_MATLIB_MASKING_APPLY_TEMPLATE(a,(((i*n+j)*k+h)*len+l),fill) \
            } \
        } \
    } \

#define RINDOW_MATLIB_MASKING_SIMPLE_TEMPLATE \
    for(h=0; h<k; h++) { \
        if(!x[h]) { \
            for(int l=0;l<len;l++) { \
                RINDOW_MATLIB_MASKING_APPLY_TEMPLATE(a,(h*len+l),fill) \
            } \
        } \
    } \

#define RINDOW_MATLIB_MASKING_BROADCASTINNER_TEMPLATE \
    for(h=0; h<k; h++) { \
        for(int32_t j=0; j<n; j++) { \
            if(!x[h]) { \
                for(int l=0;l<len;l++) { \
                    RINDOW_MATLIB_MASKING_APPLY_TEMPLATE(a,((j*k+h)*len+l),fill) \
                } \
            } \
        } \
    } \

#define RINDOW_MATLIB_MASKING_MGEN_TEMPLATE \
    for(i=0; i<m; i++) { \
        for(int32_t j=0; j<n; j++) { \
            RINDOW_MATLIB_MASKING_TEMPLATE(n,k,len,i,j,fill,x,a) \
        } \
    } \

#define RINDOW_MATLIB_MASKING_MLTN_TEMPLATE \
    for(j=0; j<n; j++) { \
        for(int32_t i=0;i<m; i++) { \
            RINDOW_MATLIB_MASKING_TEMPLATE(n,k,len,i,j,fill,x,a) \
        } \
    } \


void rindow_matlib_s_masking(
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t len,
    float fill,
    int32_t mode,
    uint8_t *x, 
    float *a
) {
    if(m <= 0 || n <= 0 || k <=0) {
        return;
    }
    if(m==1 && n==1) {  // simple masking (same shape X and A)
        int32_t h;
        #pragma omp parallel for
        for(h=0; h<k; h++) {
            if(!x[h]) {
                for(int l=0;l<len;l++) {
                    RINDOW_MATLIB_MASKING_APPLY_TEMPLATE(a,(h*len+l),fill)
                }
            }
        }
    } else if(m==1 && n<k) {// broadcast for inner shape and n<k
        int32_t h;
        #pragma omp parallel for
        for(h=0; h<k; h++) {
            for(int32_t j=0; j<n; j++) {
                if(!x[h]) {
                    for(int l=0;l<len;l++) {
                        RINDOW_MATLIB_MASKING_APPLY_TEMPLATE(a,((j*k+h)*len+l),fill)
                    }
                }
            }
        }
    } else {
        if(m>n) {
            int32_t i;
            #pragma omp parallel for
            for(i=0; i<m; i++) {
                for(int32_t j=0; j<n; j++) {
                    RINDOW_MATLIB_MASKING_TEMPLATE(n,k,len,i,j,fill,x,a)
                }
            }
        } else {
            int32_t j;
            #pragma omp parallel for
            for(j=0; j<n; j++) {
                for(int32_t i=0;i<m; i++) {
                    RINDOW_MATLIB_MASKING_TEMPLATE(n,k,len,i,j,fill,x,a)
                }
            }
        }
    }
}

void rindow_matlib_d_masking(
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t len,
    double fill,
    int32_t mode,
    uint8_t *x, 
    double *a
) {
    if(m <= 0 || n <= 0 || k <=0) {
        return;
    }
    if(m==1 && n==1) {  // simple masking (same shape X and A)
        int32_t h;
        #pragma omp parallel for
        for(h=0; h<k; h++) {
            if(!x[h]) {
                for(int l=0;l<len;l++) {
                    RINDOW_MATLIB_MASKING_APPLY_TEMPLATE(a,(h*len+l),fill)
                }
            }
        }

    } else if(m==1 && n<k) {// broadcast for inner shape and n<k
        int32_t h;
        #pragma omp parallel for
        for(h=0; h<k; h++) {
            for(int32_t j=0; j<n; j++) {
                if(!x[h]) {
                    for(int l=0;l<len;l++) {
                        RINDOW_MATLIB_MASKING_APPLY_TEMPLATE(a,((j*k+h)*len+l),fill)
                    }
                }
            }
        }

    } else {
        if(m>n) {
            int32_t i;
            #pragma omp parallel for
            for(i=0; i<m; i++) {
                for(int32_t j=0; j<n; j++) {
                    RINDOW_MATLIB_MASKING_TEMPLATE(n,k,len,i,j,fill,x,a)
                }
            }
        } else {
            int32_t j;
            #pragma omp parallel for
            for(j=0; j<n; j++) {
                for(int32_t i=0;i<m; i++) {
                    RINDOW_MATLIB_MASKING_TEMPLATE(n,k,len,i,j,fill,x,a)
                }
            }
        }
    }
}

void rindow_matlib_i_masking(
    int32_t dtype,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t len,
    void *vfill,
    int32_t mode,
    uint8_t *x, 
    void *va
) {
    if(m <= 0 || n <= 0 || k <=0) {
        return;
    }
    int32_t vmode = mode;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            int8_t fill = *((int8_t*)vfill);
            int8_t *a = (int8_t*)va;
            if(m==1 && n==1) {  // simple masking (same shape X and A)
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_SIMPLE_TEMPLATE
        
            } else if(m==1 && n<k) {// broadcast for inner shape and n<k
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_BROADCASTINNER_TEMPLATE
        
            } else {
                if(m>n) {
                    int32_t i;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MGEN_TEMPLATE
        
                } else {
                    int32_t j;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MLTN_TEMPLATE
                }
            }
            break;
        }
        case rindow_matlib_dtype_uint8: {
            uint8_t fill = *((uint8_t*)vfill);
            uint8_t *a = (uint8_t*)va;
            if(m==1 && n==1) {  // simple masking (same shape X and A)
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_SIMPLE_TEMPLATE
        
            } else if(m==1 && n<k) {// broadcast for inner shape and n<k
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_BROADCASTINNER_TEMPLATE
        
            } else {
                if(m>n) {
                    int32_t i;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MGEN_TEMPLATE
        
                } else {
                    int32_t j;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MLTN_TEMPLATE
                }
            }
            break;
        }
        case rindow_matlib_dtype_int16: {
            int16_t fill = *((int16_t*)vfill);
            int16_t *a = (int16_t*)va;
            if(m==1 && n==1) {  // simple masking (same shape X and A)
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_SIMPLE_TEMPLATE
        
            } else if(m==1 && n<k) {// broadcast for inner shape and n<k
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_BROADCASTINNER_TEMPLATE
        
            } else {
                if(m>n) {
                    int32_t i;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MGEN_TEMPLATE
        
                } else {
                    int32_t j;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MLTN_TEMPLATE
                }
            }
            break;
        }
        case rindow_matlib_dtype_uint16: {
            uint16_t fill = *((uint16_t*)vfill);
            uint16_t *a = (uint16_t*)va;
            if(m==1 && n==1) {  // simple masking (same shape X and A)
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_SIMPLE_TEMPLATE
        
            } else if(m==1 && n<k) {// broadcast for inner shape and n<k
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_BROADCASTINNER_TEMPLATE
        
            } else {
                if(m>n) {
                    int32_t i;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MGEN_TEMPLATE
        
                } else {
                    int32_t j;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MLTN_TEMPLATE
                }
            }
            break;
        }
        case rindow_matlib_dtype_int32: {
            int32_t fill = *((int32_t*)vfill);
            int32_t *a = (int32_t*)va;
            if(m==1 && n==1) {  // simple masking (same shape X and A)
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_SIMPLE_TEMPLATE
        
            } else if(m==1 && n<k) {// broadcast for inner shape and n<k
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_BROADCASTINNER_TEMPLATE
        
            } else {
                if(m>n) {
                    int32_t i;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MGEN_TEMPLATE
        
                } else {
                    int32_t j;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MLTN_TEMPLATE
                }
            }
            break;
        }
        case rindow_matlib_dtype_uint32: {
            uint32_t fill = *((uint32_t*)vfill);
            uint32_t *a = (uint32_t*)va;
            if(m==1 && n==1) {  // simple masking (same shape X and A)
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_SIMPLE_TEMPLATE
        
            } else if(m==1 && n<k) {// broadcast for inner shape and n<k
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_BROADCASTINNER_TEMPLATE
        
            } else {
                if(m>n) {
                    int32_t i;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MGEN_TEMPLATE
        
                } else {
                    int32_t j;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MLTN_TEMPLATE
                }
            }
            break;
        }
        case rindow_matlib_dtype_int64: {
            int64_t fill = *((int64_t*)vfill);
            int64_t *a = (int64_t*)va;
            if(m==1 && n==1) {  // simple masking (same shape X and A)
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_SIMPLE_TEMPLATE
        
            } else if(m==1 && n<k) {// broadcast for inner shape and n<k
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_BROADCASTINNER_TEMPLATE
        
            } else {
                if(m>n) {
                    int32_t i;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MGEN_TEMPLATE
        
                } else {
                    int32_t j;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MLTN_TEMPLATE
                }
            }
            break;
        }
        case rindow_matlib_dtype_uint64: {
            uint64_t fill = *((uint64_t*)vfill);
            uint64_t *a = (uint64_t*)va;
            if(m==1 && n==1) {  // simple masking (same shape X and A)
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_SIMPLE_TEMPLATE
        
            } else if(m==1 && n<k) {// broadcast for inner shape and n<k
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_BROADCASTINNER_TEMPLATE
        
            } else {
                if(m>n) {
                    int32_t i;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MGEN_TEMPLATE
        
                } else {
                    int32_t j;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MLTN_TEMPLATE
                }
            }
            break;
        }
        case rindow_matlib_dtype_bool: {
            uint8_t fill = *((uint8_t*)vfill);
            uint8_t *a = (uint8_t*)va;
            int32_t mode;
            if(vmode==mode_set) {
                mode = mode_set;
            } else {
                mode = mode_or;
            }
            if(m==1 && n==1) {  // simple masking (same shape X and A)
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_SIMPLE_TEMPLATE
        
            } else if(m==1 && n<k) {// broadcast for inner shape and n<k
                int32_t h;
                #pragma omp parallel for
                RINDOW_MATLIB_MASKING_BROADCASTINNER_TEMPLATE
        
            } else {
                if(m>n) {
                    int32_t i;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MGEN_TEMPLATE
        
                } else {
                    int32_t j;
                    #pragma omp parallel for
                    RINDOW_MATLIB_MASKING_MLTN_TEMPLATE
                }
            }
            break;
        }
        default: {
            break;
        }
    }
}
