#ifdef _MSC_VER
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#endif
#include <math.h>
#include <stdlib.h>
#include "rindow/matlib.h"
#include "common.h"

static inline float rindow_matlib_s_random_normal_sub(
    float mean,float scale)
{
    float x,y;

    x=(float)((rindow_matlib_common_rand()%(RINDOW_MATLIB_COMMON_RAND_MAX-1))+1)/(float)RINDOW_MATLIB_COMMON_RAND_MAX;
    y=(float)((rindow_matlib_common_rand()%(RINDOW_MATLIB_COMMON_RAND_MAX-1))+1)/(float)RINDOW_MATLIB_COMMON_RAND_MAX;

    return sqrtf((float)(-2)*logf(x))*cosf((float)2*(float)M_PI*y)*scale+mean;
}

static inline double rindow_matlib_d_random_normal_sub(
    double mean,double scale)
{
    double x,y;

    x=(double)((rindow_matlib_common_rand()%(RINDOW_MATLIB_COMMON_RAND_MAX-1))+1)/(double)RINDOW_MATLIB_COMMON_RAND_MAX;
    y=(double)((rindow_matlib_common_rand()%(RINDOW_MATLIB_COMMON_RAND_MAX-1))+1)/(double)RINDOW_MATLIB_COMMON_RAND_MAX;

    return sqrt(-2*log(x))*cos(2*M_PI*y)*scale+mean;
}

void rindow_matlib_s_randomnormal(
    int32_t n,
    float *x, int32_t incX,
    float mean,
    float scale,
    int32_t seed
)
{
    if(seed!=0) {
        rindow_matlib_common_srand(seed);
    }
    for(int32_t i=0; i<n; i++) {
        x[i*incX] = rindow_matlib_s_random_normal_sub(mean,scale);
    }
}

void rindow_matlib_d_randomnormal(
    int32_t n,
    double *x, int32_t incX,
    double mean,
    double scale,
    int32_t seed
)
{
    if(seed!=0) {
        rindow_matlib_common_srand(seed);
    }
    for(int32_t i=0; i<n; i++) {
        x[i*incX] = rindow_matlib_d_random_normal_sub(mean,scale);
    }
}
