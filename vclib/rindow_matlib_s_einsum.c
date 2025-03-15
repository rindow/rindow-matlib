#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_einsum not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_s_einsum)( /* rindow_matlib_s_einsum */
    const int32_t            /* depth */,
    const int32_t *            /* sizeOfIndices */,
    const float *            /* a */,
    const int32_t *            /* ldA */,
    const float *            /* b */,
    const int32_t *            /* ldB */,
    float *            /* c */,
    const int32_t            /* ndimC */
);
static PFNrindow_matlib_s_einsum _g_rindow_matlib_s_einsum = NULL;
int32_t rindow_matlib_s_einsum(
    const int32_t            depth,
    const int32_t *            sizeOfIndices,
    const float *            a,
    const int32_t *            ldA,
    const float *            b,
    const int32_t *            ldB,
    float *            c,
    const int32_t            ndimC
)
{
    if(_g_rindow_matlib_s_einsum==NULL) {
        _g_rindow_matlib_s_einsum = rindow_load_rindowmatlib_func("rindow_matlib_s_einsum"); 
        if(_g_rindow_matlib_s_einsum==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_s_einsum(
        depth,
        sizeOfIndices,
        a,
        ldA,
        b,
        ldB,
        c,
        ndimC    
    );
}
