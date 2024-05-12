#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_i_sum not found\n";
typedef int64_t (CALLBACK* PFNrindow_matlib_i_sum)( /* rindow_matlib_i_sum */
    int32_t            /* dtype */,
    int32_t            /* n */,
    void *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_i_sum _g_rindow_matlib_i_sum = NULL;
int64_t rindow_matlib_i_sum(
    int32_t            dtype,
    int32_t            n,
    void *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_i_sum==NULL) {
        _g_rindow_matlib_i_sum = rindow_load_rindowmatlib_func("rindow_matlib_i_sum"); 
        if(_g_rindow_matlib_i_sum==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_i_sum(
        dtype,
        n,
        x,
        incX    
    );
}
