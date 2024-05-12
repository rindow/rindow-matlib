#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_transpose not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_d_transpose)( /* rindow_matlib_d_transpose */
    int32_t            /* ndim */,
    int32_t *            /* shape */,
    int32_t *            /* perm */,
    double *            /* a */,
    double *            /* b */
);
static PFNrindow_matlib_d_transpose _g_rindow_matlib_d_transpose = NULL;
int32_t rindow_matlib_d_transpose(
    int32_t            ndim,
    int32_t *            shape,
    int32_t *            perm,
    double *            a,
    double *            b
)
{
    if(_g_rindow_matlib_d_transpose==NULL) {
        _g_rindow_matlib_d_transpose = rindow_load_rindowmatlib_func("rindow_matlib_d_transpose"); 
        if(_g_rindow_matlib_d_transpose==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_d_transpose(
        ndim,
        shape,
        perm,
        a,
        b    
    );
}
