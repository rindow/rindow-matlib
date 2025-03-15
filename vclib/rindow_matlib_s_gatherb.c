#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_gatherb not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_s_gatherb)( /* rindow_matlib_s_gatherb */
    int32_t            /* reverse */,
    int32_t            /* addMode */,
    int32_t            /* batches */,
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* k */,
    int32_t            /* len */,
    int32_t            /* numClass */,
    float *            /* a */,
    int32_t *            /* x */,
    float *            /* b */
);
static PFNrindow_matlib_s_gatherb _g_rindow_matlib_s_gatherb = NULL;
int32_t rindow_matlib_s_gatherb(
    int32_t            reverse,
    int32_t            addMode,
    int32_t            batches,
    int32_t            m,
    int32_t            n,
    int32_t            k,
    int32_t            len,
    int32_t            numClass,
    float *            a,
    int32_t *            x,
    float *            b
)
{
    if(_g_rindow_matlib_s_gatherb==NULL) {
        _g_rindow_matlib_s_gatherb = rindow_load_rindowmatlib_func("rindow_matlib_s_gatherb"); 
        if(_g_rindow_matlib_s_gatherb==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_s_gatherb(
        reverse,
        addMode,
        batches,
        m,
        n,
        k,
        len,
        numClass,
        a,
        x,
        b    
    );
}
