#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_reducesum not found\n";
typedef void (CALLBACK* PFNrindow_matlib_d_reducesum)( /* rindow_matlib_d_reducesum */
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* k */,
    double *            /* a */,
    double *            /* b */
);
static PFNrindow_matlib_d_reducesum _g_rindow_matlib_d_reducesum = NULL;
void rindow_matlib_d_reducesum(
    int32_t            m,
    int32_t            n,
    int32_t            k,
    double *            a,
    double *            b
)
{
    if(_g_rindow_matlib_d_reducesum==NULL) {
        _g_rindow_matlib_d_reducesum = rindow_load_rindowmatlib_func("rindow_matlib_d_reducesum"); 
        if(_g_rindow_matlib_d_reducesum==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_d_reducesum(
        m,
        n,
        k,
        a,
        b    
    );
}
