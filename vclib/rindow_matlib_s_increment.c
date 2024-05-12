#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_increment not found\n";
typedef void (CALLBACK* PFNrindow_matlib_s_increment)( /* rindow_matlib_s_increment */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */,
    float            /* alpha */,
    float            /* beta */
);
static PFNrindow_matlib_s_increment _g_rindow_matlib_s_increment = NULL;
void rindow_matlib_s_increment(
    int32_t            n,
    float *            x,
    int32_t            incX,
    float            alpha,
    float            beta
)
{
    if(_g_rindow_matlib_s_increment==NULL) {
        _g_rindow_matlib_s_increment = rindow_load_rindowmatlib_func("rindow_matlib_s_increment"); 
        if(_g_rindow_matlib_s_increment==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_s_increment(
        n,
        x,
        incX,
        alpha,
        beta    
    );
}
