#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_sqrt not found\n";
typedef void (CALLBACK* PFNrindow_matlib_s_sqrt)( /* rindow_matlib_s_sqrt */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_s_sqrt _g_rindow_matlib_s_sqrt = NULL;
void rindow_matlib_s_sqrt(
    int32_t            n,
    float *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_s_sqrt==NULL) {
        _g_rindow_matlib_s_sqrt = rindow_load_rindowmatlib_func("rindow_matlib_s_sqrt"); 
        if(_g_rindow_matlib_s_sqrt==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_s_sqrt(
        n,
        x,
        incX    
    );
}
