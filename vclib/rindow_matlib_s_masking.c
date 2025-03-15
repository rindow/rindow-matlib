#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_masking not found\n";
typedef void (CALLBACK* PFNrindow_matlib_s_masking)( /* rindow_matlib_s_masking */
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* k */,
    int32_t            /* len */,
    float            /* fill */,
    int32_t            /* mode */,
    uint8_t *            /* x */,
    float *            /* a */
);
static PFNrindow_matlib_s_masking _g_rindow_matlib_s_masking = NULL;
void rindow_matlib_s_masking(
    int32_t            m,
    int32_t            n,
    int32_t            k,
    int32_t            len,
    float            fill,
    int32_t            mode,
    uint8_t *            x,
    float *            a
)
{
    if(_g_rindow_matlib_s_masking==NULL) {
        _g_rindow_matlib_s_masking = rindow_load_rindowmatlib_func("rindow_matlib_s_masking"); 
        if(_g_rindow_matlib_s_masking==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_s_masking(
        m,
        n,
        k,
        len,
        fill,
        mode,
        x,
        a    
    );
}
