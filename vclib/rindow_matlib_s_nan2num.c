#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_nan2num not found\n";
typedef void (CALLBACK* PFNrindow_matlib_s_nan2num)( /* rindow_matlib_s_nan2num */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */,
    float            /* alpha */
);
static PFNrindow_matlib_s_nan2num _g_rindow_matlib_s_nan2num = NULL;
void rindow_matlib_s_nan2num(
    int32_t            n,
    float *            x,
    int32_t            incX,
    float            alpha
)
{
    if(_g_rindow_matlib_s_nan2num==NULL) {
        _g_rindow_matlib_s_nan2num = rindow_load_rindowmatlib_func("rindow_matlib_s_nan2num"); 
        if(_g_rindow_matlib_s_nan2num==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_s_nan2num(
        n,
        x,
        incX,
        alpha    
    );
}
