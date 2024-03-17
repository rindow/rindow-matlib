#include "rindow/matlib.h"
#include "common.h"
#include <string.h>

void rindow_matlib_fill(int32_t dtype, int32_t n, void *value, void *x, int32_t incX)
{
    size_t value_size = rindow_matlib_common_dtype_to_valuesize(dtype);
    char *vx = x;
    size_t step = incX*value_size;

    int32_t i;
    #pragma omp parallel for
    for(i=0;i<n;i++) {
        memcpy(&(vx[i*step]),value,value_size);
    }
}
