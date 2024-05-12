#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_rsqrt not found\n";
typedef void (CALLBACK* PFNrindow_matlib_d_rsqrt)( /* rindow_matlib_d_rsqrt */
    int32_t            /* n */,
    double            /* alpha */,
    double *            /* x */,
    int32_t            /* incX */,
    double            /* beta */
);
static PFNrindow_matlib_d_rsqrt _g_rindow_matlib_d_rsqrt = NULL;
void rindow_matlib_d_rsqrt(
    int32_t            n,
    double            alpha,
    double *            x,
    int32_t            incX,
    double            beta
)
{
    if(_g_rindow_matlib_d_rsqrt==NULL) {
        _g_rindow_matlib_d_rsqrt = rindow_load_rindowmatlib_func("rindow_matlib_d_rsqrt"); 
        if(_g_rindow_matlib_d_rsqrt==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_d_rsqrt(
        n,
        alpha,
        x,
        incX,
        beta    
    );
}
