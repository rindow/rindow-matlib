#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_i_transpose not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_i_transpose)( /* rindow_matlib_i_transpose */
    int32_t            /* dtype */,
    int32_t            /* ndim */,
    int32_t *            /* shape */,
    int32_t *            /* perm */,
    void *            /* a */,
    void *            /* b */
);
static PFNrindow_matlib_i_transpose _g_rindow_matlib_i_transpose = NULL;
int32_t rindow_matlib_i_transpose(
    int32_t            dtype,
    int32_t            ndim,
    int32_t *            shape,
    int32_t *            perm,
    void *            a,
    void *            b
)
{
    if(_g_rindow_matlib_i_transpose==NULL) {
        _g_rindow_matlib_i_transpose = rindow_load_rindowmatlib_func("rindow_matlib_i_transpose"); 
        if(_g_rindow_matlib_i_transpose==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_i_transpose(
        dtype,
        ndim,
        shape,
        perm,
        a,
        b    
    );
}
