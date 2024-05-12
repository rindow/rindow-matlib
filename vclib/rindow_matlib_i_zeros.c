#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_i_zeros not found\n";
typedef void (CALLBACK* PFNrindow_matlib_i_zeros)( /* rindow_matlib_i_zeros */
    int32_t            /* dtype */,
    int32_t            /* n */,
    void *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_i_zeros _g_rindow_matlib_i_zeros = NULL;
void rindow_matlib_i_zeros(
    int32_t            dtype,
    int32_t            n,
    void *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_i_zeros==NULL) {
        _g_rindow_matlib_i_zeros = rindow_load_rindowmatlib_func("rindow_matlib_i_zeros"); 
        if(_g_rindow_matlib_i_zeros==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_i_zeros(
        dtype,
        n,
        x,
        incX    
    );
}
