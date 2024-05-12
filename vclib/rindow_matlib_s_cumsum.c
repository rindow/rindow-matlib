#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_cumsum not found\n";
typedef void (CALLBACK* PFNrindow_matlib_s_cumsum)( /* rindow_matlib_s_cumsum */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */,
    int32_t            /* exclusive */,
    int32_t            /* reverse */,
    float *            /* y */,
    int32_t            /* incY */
);
static PFNrindow_matlib_s_cumsum _g_rindow_matlib_s_cumsum = NULL;
void rindow_matlib_s_cumsum(
    int32_t            n,
    float *            x,
    int32_t            incX,
    int32_t            exclusive,
    int32_t            reverse,
    float *            y,
    int32_t            incY
)
{
    if(_g_rindow_matlib_s_cumsum==NULL) {
        _g_rindow_matlib_s_cumsum = rindow_load_rindowmatlib_func("rindow_matlib_s_cumsum"); 
        if(_g_rindow_matlib_s_cumsum==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_s_cumsum(
        n,
        x,
        incX,
        exclusive,
        reverse,
        y,
        incY    
    );
}
