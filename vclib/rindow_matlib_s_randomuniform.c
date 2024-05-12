#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_randomuniform not found\n";
typedef void (CALLBACK* PFNrindow_matlib_s_randomuniform)( /* rindow_matlib_s_randomuniform */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */,
    float            /* low */,
    float            /* high */,
    int32_t            /* seed */
);
static PFNrindow_matlib_s_randomuniform _g_rindow_matlib_s_randomuniform = NULL;
void rindow_matlib_s_randomuniform(
    int32_t            n,
    float *            x,
    int32_t            incX,
    float            low,
    float            high,
    int32_t            seed
)
{
    if(_g_rindow_matlib_s_randomuniform==NULL) {
        _g_rindow_matlib_s_randomuniform = rindow_load_rindowmatlib_func("rindow_matlib_s_randomuniform"); 
        if(_g_rindow_matlib_s_randomuniform==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_s_randomuniform(
        n,
        x,
        incX,
        low,
        high,
        seed    
    );
}
