#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_i_not not found\n";
typedef void (CALLBACK* PFNrindow_matlib_i_not)( /* rindow_matlib_i_not */
    int32_t            /* dtype */,
    int32_t            /* n */,
    void *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_i_not _g_rindow_matlib_i_not = NULL;
void rindow_matlib_i_not(
    int32_t            dtype,
    int32_t            n,
    void *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_i_not==NULL) {
        _g_rindow_matlib_i_not = rindow_load_rindowmatlib_func("rindow_matlib_i_not"); 
        if(_g_rindow_matlib_i_not==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_i_not(
        dtype,
        n,
        x,
        incX    
    );
}
