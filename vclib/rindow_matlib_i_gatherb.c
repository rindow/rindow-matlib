#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_i_gatherb not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_i_gatherb)( /* rindow_matlib_i_gatherb */
    int32_t            /* reverse */,
    int32_t            /* addMode */,
    int32_t            /* batches */,
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* k */,
    int32_t            /* len */,
    int32_t            /* numClass */,
    int32_t            /* dtype */,
    void *            /* a */,
    int32_t *            /* x */,
    void *            /* b */
);
static PFNrindow_matlib_i_gatherb _g_rindow_matlib_i_gatherb = NULL;
int32_t rindow_matlib_i_gatherb(
    int32_t            reverse,
    int32_t            addMode,
    int32_t            batches,
    int32_t            m,
    int32_t            n,
    int32_t            k,
    int32_t            len,
    int32_t            numClass,
    int32_t            dtype,
    void *            a,
    int32_t *            x,
    void *            b
)
{
    if(_g_rindow_matlib_i_gatherb==NULL) {
        _g_rindow_matlib_i_gatherb = rindow_load_rindowmatlib_func("rindow_matlib_i_gatherb"); 
        if(_g_rindow_matlib_i_gatherb==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_i_gatherb(
        reverse,
        addMode,
        batches,
        m,
        n,
        k,
        len,
        numClass,
        dtype,
        a,
        x,
        b    
    );
}
