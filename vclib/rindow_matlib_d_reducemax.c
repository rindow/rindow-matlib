#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_reducemax not found\n";
typedef void (CALLBACK* PFNrindow_matlib_d_reducemax)( /* rindow_matlib_d_reducemax */
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* k */,
    double *            /* a */,
    double *            /* b */
);
static PFNrindow_matlib_d_reducemax _g_rindow_matlib_d_reducemax = NULL;
void rindow_matlib_d_reducemax(
    int32_t            m,
    int32_t            n,
    int32_t            k,
    double *            a,
    double *            b
)
{
    if(_g_rindow_matlib_d_reducemax==NULL) {
        _g_rindow_matlib_d_reducemax = rindow_load_rindowmatlib_func("rindow_matlib_d_reducemax"); 
        if(_g_rindow_matlib_d_reducemax==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_d_reducemax(
        m,
        n,
        k,
        a,
        b    
    );
}
