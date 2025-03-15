#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_cumsumb not found\n";
typedef void (CALLBACK* PFNrindow_matlib_s_cumsumb)( /* rindow_matlib_s_cumsumb */
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* k */,
    float *            /* a */,
    int32_t            /* exclusive */,
    int32_t            /* reverse */,
    float *            /* b */
);
static PFNrindow_matlib_s_cumsumb _g_rindow_matlib_s_cumsumb = NULL;
void rindow_matlib_s_cumsumb(
    int32_t            m,
    int32_t            n,
    int32_t            k,
    float *            a,
    int32_t            exclusive,
    int32_t            reverse,
    float *            b
)
{
    if(_g_rindow_matlib_s_cumsumb==NULL) {
        _g_rindow_matlib_s_cumsumb = rindow_load_rindowmatlib_func("rindow_matlib_s_cumsumb"); 
        if(_g_rindow_matlib_s_cumsumb==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_s_cumsumb(
        m,
        n,
        k,
        a,
        exclusive,
        reverse,
        b    
    );
}
