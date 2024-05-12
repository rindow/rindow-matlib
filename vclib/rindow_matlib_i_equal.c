#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_i_equal not found\n";
typedef void (CALLBACK* PFNrindow_matlib_i_equal)( /* rindow_matlib_i_equal */
    int32_t            /* dtype */,
    int32_t            /* n */,
    void *            /* x */,
    int32_t            /* incX */,
    void *            /* y */,
    int32_t            /* incY */
);
static PFNrindow_matlib_i_equal _g_rindow_matlib_i_equal = NULL;
void rindow_matlib_i_equal(
    int32_t            dtype,
    int32_t            n,
    void *            x,
    int32_t            incX,
    void *            y,
    int32_t            incY
)
{
    if(_g_rindow_matlib_i_equal==NULL) {
        _g_rindow_matlib_i_equal = rindow_load_rindowmatlib_func("rindow_matlib_i_equal"); 
        if(_g_rindow_matlib_i_equal==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_i_equal(
        dtype,
        n,
        x,
        incX,
        y,
        incY    
    );
}
