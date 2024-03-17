#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

//#ifndef _OPENMP
/*
static void *s_sum_kernel(void *varg)
{
    arg_s_sum_kernel_t *arg=varg;
    int32_t i;
    float sum=0;
    int32_t n = arg->n;
    float* x = arg->x;
    int32_t incX = arg->incX;
    for(i=0; i<arg->n; i++) {
        sum += x[i*incX];
    }
    arg->sum = sum;

    return NULL;
}
static void *d_sum_kernel(void *varg)
{
    arg_d_sum_kernel_t *arg=varg;
    int32_t i;
    double sum=0;
    int32_t n = arg->n;
    double* x = arg->x;
    int32_t incX = arg->incX;
    for(i=0; i<arg->n; i++) {
        sum += x[i*incX];
    }
    arg->sum = sum;

    return NULL;
}
*/
//#endif

float rindow_matlib_s_sum(int32_t n,float *x, int32_t incX)
{
    float sum = 0;
//#ifdef _OPENMP
    int32_t i;
    #pragma omp parallel for reduction(+:sum)
    for(i=0; i<n; i++) {
        sum += x[i*incX];
    }
    return sum;
//#else
/*
    int32_t num_thread = rindow_matlib_common_get_nprocs();
    int32_t cell_size = n/num_thread;
    int32_t remainder = n-cell_size*num_thread;
    //printf("#num_thread=%d\n",num_thread);
    //printf("#cell_size=%d\n",cell_size);
    //printf("#remainder=%d\n",remainder);
    arg_s_sum_kernel_t *th_arg=calloc(num_thread,sizeof(arg_s_sum_kernel_t));
    int32_t i;
    for(i=0;i<num_thread;i++) {
        if(i==num_thread-1) {
            th_arg[i].n = cell_size+remainder;
        } else {
            th_arg[i].n = cell_size;
        }
        th_arg[i].x = &x[i*cell_size*incX];
        th_arg[i].incX = incX;
        th_arg[i].sum = 0.0;
        pthread_create( &(th_arg[i].tid), NULL, &s_sum_kernel, &th_arg[i]);
    }
    for(i=0;i<num_thread;i++) {
        pthread_join(th_arg[i].tid,NULL);
        sum += th_arg[i].sum;
    }
    free(th_arg);
    return tsum+sum;
*/
//#endif
}

double rindow_matlib_d_sum(int32_t n,double *x, int32_t incX)
{
    double sum = 0;
//#ifdef _OPENMP
    int32_t i;
    #pragma omp parallel for reduction(+:sum)
    for(i=0; i<n; i++) {
        sum += x[i*incX];
    }
    return sum;
//#else

/*
    int32_t num_thread = rindow_matlib_common_get_nprocs();
    int32_t cell_size = n/num_thread;
    int32_t remainder = n-cell_size*num_thread;
    //printf("#num_thread=%d\n",num_thread);
    //printf("#cell_size=%d\n",cell_size);
    //printf("#remainder=%d\n",remainder);
    arg_d_sum_kernel_t *th_arg=calloc(num_thread,sizeof(arg_d_sum_kernel_t));
    int32_t i;
    for(i=0;i<num_thread;i++) {
        if(i==num_thread-1) {
            th_arg[i].n = cell_size+remainder;
        } else {
            th_arg[i].n = cell_size;
        }
        th_arg[i].x = &x[i*cell_size*incX];
        th_arg[i].incX = incX;
        th_arg[i].sum = 0.0;
        pthread_create( &(th_arg[i].tid), NULL, &d_sum_kernel, &th_arg[i]);
    }
    for(i=0;i<num_thread;i++) {
        pthread_join(th_arg[i].tid,NULL);
        sum += th_arg[i].sum;
    }
    free(th_arg);
    return tsum+sum;
*/
//#endif
}



#define RINDOW_MATLIB_SUM_TEMPLATE(data_type) \
    for(i=0; i<n; i++) {\
        sum += pDataX[i*incX];\
    }

int64_t rindow_matlib_i_sum(int32_t dtype, int32_t n,void *x, int32_t incX)
{
    int32_t i;
    int64_t sum=0;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            int8_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(int8_t)
            break;
        }
        case rindow_matlib_dtype_uint8: {
            uint8_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(uint8_t)
            break;
        }
        case rindow_matlib_dtype_int16: {
            int16_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(int16_t)
            break;
        }
        case rindow_matlib_dtype_uint16: {
            uint16_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(uint16_t)
            break;
        }
        case rindow_matlib_dtype_int32: {
            int32_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(int32_t)
            break;
        }
        case rindow_matlib_dtype_uint32: {
            uint32_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(uint32_t)
            break;
        }
        case rindow_matlib_dtype_int64: {
            int64_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(int64_t)
            break;
        }
        case rindow_matlib_dtype_uint64: {
            uint64_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(uint64_t)
            break;
        }
        case rindow_matlib_dtype_bool: {
                uint8_t *pDataX=x;
                #pragma omp parallel for reduction(+:sum)
                for (i=0; i<n; i++) {
                    if(pDataX[i*incX]!=0) {
                        sum++;
                    }
                }
            break;
        }
    }
    return sum;
}
