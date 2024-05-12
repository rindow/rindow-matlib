#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_i_imax not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_i_imax)( /* rindow_matlib_i_imax */
    int32_t            /* dtype */,
    int32_t            /* n */,
    void *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_i_imax _g_rindow_matlib_i_imax = NULL;
int32_t rindow_matlib_i_imax(
    int32_t            dtype,
    int32_t            n,
    void *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_i_imax==NULL) {
        _g_rindow_matlib_i_imax = rindow_load_rindowmatlib_func("rindow_matlib_i_imax"); 
        if(_g_rindow_matlib_i_imax==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_i_imax(
        dtype,
        n,
        x,
        incX    
    );
}
