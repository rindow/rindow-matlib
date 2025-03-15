#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_i_bandpart not found\n";
typedef void (CALLBACK* PFNrindow_matlib_i_bandpart)( /* rindow_matlib_i_bandpart */
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* k */,
    int32_t            /* dtype */,
    void *            /* a */,
    int32_t            /* lower */,
    int32_t            /* upper */
);
static PFNrindow_matlib_i_bandpart _g_rindow_matlib_i_bandpart = NULL;
void rindow_matlib_i_bandpart(
    int32_t            m,
    int32_t            n,
    int32_t            k,
    int32_t            dtype,
    void *            a,
    int32_t            lower,
    int32_t            upper
)
{
    if(_g_rindow_matlib_i_bandpart==NULL) {
        _g_rindow_matlib_i_bandpart = rindow_load_rindowmatlib_func("rindow_matlib_i_bandpart"); 
        if(_g_rindow_matlib_i_bandpart==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_i_bandpart(
        m,
        n,
        k,
        dtype,
        a,
        lower,
        upper    
    );
}
