#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_sin not found\n";
typedef void (CALLBACK* PFNrindow_matlib_d_sin)( /* rindow_matlib_d_sin */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_sin _g_rindow_matlib_d_sin = NULL;
void rindow_matlib_d_sin(
    int32_t            n,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_sin==NULL) {
        _g_rindow_matlib_d_sin = rindow_load_rindowmatlib_func("rindow_matlib_d_sin"); 
        if(_g_rindow_matlib_d_sin==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_d_sin(
        n,
        x,
        incX    
    );
}
