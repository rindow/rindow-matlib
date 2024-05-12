#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_reducegather not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_d_reducegather)( /* rindow_matlib_d_reducegather */
    int32_t            /* reverse */,
    int32_t            /* addMode */,
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* numClass */,
    int32_t            /* dtype */,
    void *            /* x */,
    double *            /* a */,
    double *            /* b */
);
static PFNrindow_matlib_d_reducegather _g_rindow_matlib_d_reducegather = NULL;
int32_t rindow_matlib_d_reducegather(
    int32_t            reverse,
    int32_t            addMode,
    int32_t            m,
    int32_t            n,
    int32_t            numClass,
    int32_t            dtype,
    void *            x,
    double *            a,
    double *            b
)
{
    if(_g_rindow_matlib_d_reducegather==NULL) {
        _g_rindow_matlib_d_reducegather = rindow_load_rindowmatlib_func("rindow_matlib_d_reducegather"); 
        if(_g_rindow_matlib_d_reducegather==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_d_reducegather(
        reverse,
        addMode,
        m,
        n,
        numClass,
        dtype,
        x,
        a,
        b    
    );
}
