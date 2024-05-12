#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_less not found\n";
typedef void (CALLBACK* PFNrindow_matlib_s_less)( /* rindow_matlib_s_less */
    int32_t            /* m */,
    int32_t            /* n */,
    float *            /* a */,
    int32_t            /* ldA */,
    float *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_s_less _g_rindow_matlib_s_less = NULL;
void rindow_matlib_s_less(
    int32_t            m,
    int32_t            n,
    float *            a,
    int32_t            ldA,
    float *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_s_less==NULL) {
        _g_rindow_matlib_s_less = rindow_load_rindowmatlib_func("rindow_matlib_s_less"); 
        if(_g_rindow_matlib_s_less==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_s_less(
        m,
        n,
        a,
        ldA,
        x,
        incX    
    );
}
