#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_imax not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_s_imax)( /* rindow_matlib_s_imax */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_s_imax _g_rindow_matlib_s_imax = NULL;
int32_t rindow_matlib_s_imax(
    int32_t            n,
    float *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_s_imax==NULL) {
        _g_rindow_matlib_s_imax = rindow_load_rindowmatlib_func("rindow_matlib_s_imax"); 
        if(_g_rindow_matlib_s_imax==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_s_imax(
        n,
        x,
        incX    
    );
}
