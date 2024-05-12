#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_rsqrt not found\n";
typedef void (CALLBACK* PFNrindow_matlib_s_rsqrt)( /* rindow_matlib_s_rsqrt */
    int32_t            /* n */,
    float            /* alpha */,
    float *            /* x */,
    int32_t            /* incX */,
    float            /* beta */
);
static PFNrindow_matlib_s_rsqrt _g_rindow_matlib_s_rsqrt = NULL;
void rindow_matlib_s_rsqrt(
    int32_t            n,
    float            alpha,
    float *            x,
    int32_t            incX,
    float            beta
)
{
    if(_g_rindow_matlib_s_rsqrt==NULL) {
        _g_rindow_matlib_s_rsqrt = rindow_load_rindowmatlib_func("rindow_matlib_s_rsqrt"); 
        if(_g_rindow_matlib_s_rsqrt==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_s_rsqrt(
        n,
        alpha,
        x,
        incX,
        beta    
    );
}
