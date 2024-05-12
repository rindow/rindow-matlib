#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_notequal not found\n";
typedef void (CALLBACK* PFNrindow_matlib_s_notequal)( /* rindow_matlib_s_notequal */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */,
    float *            /* y */,
    int32_t            /* incY */
);
static PFNrindow_matlib_s_notequal _g_rindow_matlib_s_notequal = NULL;
void rindow_matlib_s_notequal(
    int32_t            n,
    float *            x,
    int32_t            incX,
    float *            y,
    int32_t            incY
)
{
    if(_g_rindow_matlib_s_notequal==NULL) {
        _g_rindow_matlib_s_notequal = rindow_load_rindowmatlib_func("rindow_matlib_s_notequal"); 
        if(_g_rindow_matlib_s_notequal==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_s_notequal(
        n,
        x,
        incX,
        y,
        incY    
    );
}
