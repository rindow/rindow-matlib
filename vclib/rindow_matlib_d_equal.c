#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_equal not found\n";
typedef void (CALLBACK* PFNrindow_matlib_d_equal)( /* rindow_matlib_d_equal */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */,
    double *            /* y */,
    int32_t            /* incY */
);
static PFNrindow_matlib_d_equal _g_rindow_matlib_d_equal = NULL;
void rindow_matlib_d_equal(
    int32_t            n,
    double *            x,
    int32_t            incX,
    double *            y,
    int32_t            incY
)
{
    if(_g_rindow_matlib_d_equal==NULL) {
        _g_rindow_matlib_d_equal = rindow_load_rindowmatlib_func("rindow_matlib_d_equal"); 
        if(_g_rindow_matlib_d_equal==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_d_equal(
        n,
        x,
        incX,
        y,
        incY    
    );
}
