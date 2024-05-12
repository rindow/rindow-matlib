#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_repeat not found\n";
typedef void (CALLBACK* PFNrindow_matlib_d_repeat)( /* rindow_matlib_d_repeat */
    int32_t            /* m */,
    int32_t            /* k */,
    int32_t            /* repeats */,
    double *            /* a */,
    double *            /* b */
);
static PFNrindow_matlib_d_repeat _g_rindow_matlib_d_repeat = NULL;
void rindow_matlib_d_repeat(
    int32_t            m,
    int32_t            k,
    int32_t            repeats,
    double *            a,
    double *            b
)
{
    if(_g_rindow_matlib_d_repeat==NULL) {
        _g_rindow_matlib_d_repeat = rindow_load_rindowmatlib_func("rindow_matlib_d_repeat"); 
        if(_g_rindow_matlib_d_repeat==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_d_repeat(
        m,
        k,
        repeats,
        a,
        b    
    );
}
