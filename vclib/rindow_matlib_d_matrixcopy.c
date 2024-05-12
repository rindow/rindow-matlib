#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_matrixcopy not found\n";
typedef void (CALLBACK* PFNrindow_matlib_d_matrixcopy)( /* rindow_matlib_d_matrixcopy */
    int32_t            /* trans */,
    int32_t            /* m */,
    int32_t            /* n */,
    double            /* alpha */,
    double *            /* a */,
    int32_t            /* ldA */,
    double *            /* b */,
    int32_t            /* ldB */
);
static PFNrindow_matlib_d_matrixcopy _g_rindow_matlib_d_matrixcopy = NULL;
void rindow_matlib_d_matrixcopy(
    int32_t            trans,
    int32_t            m,
    int32_t            n,
    double            alpha,
    double *            a,
    int32_t            ldA,
    double *            b,
    int32_t            ldB
)
{
    if(_g_rindow_matlib_d_matrixcopy==NULL) {
        _g_rindow_matlib_d_matrixcopy = rindow_load_rindowmatlib_func("rindow_matlib_d_matrixcopy"); 
        if(_g_rindow_matlib_d_matrixcopy==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_d_matrixcopy(
        trans,
        m,
        n,
        alpha,
        a,
        ldA,
        b,
        ldB    
    );
}
