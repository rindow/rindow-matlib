#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_sum not found\n";
typedef float (CALLBACK* PFNrindow_matlib_s_sum)( /* rindow_matlib_s_sum */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_s_sum _g_rindow_matlib_s_sum = NULL;
float rindow_matlib_s_sum(
    int32_t            n,
    float *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_s_sum==NULL) {
        _g_rindow_matlib_s_sum = rindow_load_rindowmatlib_func("rindow_matlib_s_sum"); 
        if(_g_rindow_matlib_s_sum==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_s_sum(
        n,
        x,
        incX    
    );
}
