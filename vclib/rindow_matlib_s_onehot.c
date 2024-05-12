#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_onehot not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_s_onehot)( /* rindow_matlib_s_onehot */
    int32_t            /* dtype */,
    int32_t            /* m */,
    int32_t            /* n */,
    void *            /* x */,
    int32_t            /* incX */,
    float            /* alpha */,
    float *            /* a */,
    int32_t            /* ldA */
);
static PFNrindow_matlib_s_onehot _g_rindow_matlib_s_onehot = NULL;
int32_t rindow_matlib_s_onehot(
    int32_t            dtype,
    int32_t            m,
    int32_t            n,
    void *            x,
    int32_t            incX,
    float            alpha,
    float *            a,
    int32_t            ldA
)
{
    if(_g_rindow_matlib_s_onehot==NULL) {
        _g_rindow_matlib_s_onehot = rindow_load_rindowmatlib_func("rindow_matlib_s_onehot"); 
        if(_g_rindow_matlib_s_onehot==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_s_onehot(
        dtype,
        m,
        n,
        x,
        incX,
        alpha,
        a,
        ldA    
    );
}
