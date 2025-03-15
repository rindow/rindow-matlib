#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_einsum4p1 not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_s_einsum4p1)( /* rindow_matlib_s_einsum4p1 */
    int32_t            /* dim0 */,
    int32_t            /* dim1 */,
    int32_t            /* dim2 */,
    int32_t            /* dim3 */,
    int32_t            /* dim4 */,
    float *            /* a */,
    int32_t            /* ldA0 */,
    int32_t            /* ldA1 */,
    int32_t            /* ldA2 */,
    int32_t            /* ldA3 */,
    int32_t            /* ldA4 */,
    float *            /* b */,
    int32_t            /* ldB0 */,
    int32_t            /* ldB1 */,
    int32_t            /* ldB2 */,
    int32_t            /* ldB3 */,
    int32_t            /* ldB4 */,
    float *            /* c */
);
static PFNrindow_matlib_s_einsum4p1 _g_rindow_matlib_s_einsum4p1 = NULL;
int32_t rindow_matlib_s_einsum4p1(
    int32_t            dim0,
    int32_t            dim1,
    int32_t            dim2,
    int32_t            dim3,
    int32_t            dim4,
    float *            a,
    int32_t            ldA0,
    int32_t            ldA1,
    int32_t            ldA2,
    int32_t            ldA3,
    int32_t            ldA4,
    float *            b,
    int32_t            ldB0,
    int32_t            ldB1,
    int32_t            ldB2,
    int32_t            ldB3,
    int32_t            ldB4,
    float *            c
)
{
    if(_g_rindow_matlib_s_einsum4p1==NULL) {
        _g_rindow_matlib_s_einsum4p1 = rindow_load_rindowmatlib_func("rindow_matlib_s_einsum4p1"); 
        if(_g_rindow_matlib_s_einsum4p1==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_s_einsum4p1(
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
}
