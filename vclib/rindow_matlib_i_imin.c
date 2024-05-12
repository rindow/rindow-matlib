#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_i_imin not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_i_imin)( /* rindow_matlib_i_imin */
    int32_t            /* dtype */,
    int32_t            /* n */,
    void *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_i_imin _g_rindow_matlib_i_imin = NULL;
int32_t rindow_matlib_i_imin(
    int32_t            dtype,
    int32_t            n,
    void *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_i_imin==NULL) {
        _g_rindow_matlib_i_imin = rindow_load_rindowmatlib_func("rindow_matlib_i_imin"); 
        if(_g_rindow_matlib_i_imin==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_i_imin(
        dtype,
        n,
        x,
        incX    
    );
}
