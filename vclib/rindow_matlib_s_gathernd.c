#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_gathernd not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_s_gathernd)( /* rindow_matlib_s_gathernd */
    int32_t            /* reverse */,
    int32_t            /* addMode */,
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* k */,
    int32_t            /* indexDepth */,
    int32_t *            /* paramShape */,
    float *            /* a */,
    int32_t *            /* x */,
    float *            /* b */
);
static PFNrindow_matlib_s_gathernd _g_rindow_matlib_s_gathernd = NULL;
int32_t rindow_matlib_s_gathernd(
    int32_t            reverse,
    int32_t            addMode,
    int32_t            m,
    int32_t            n,
    int32_t            k,
    int32_t            indexDepth,
    int32_t *            paramShape,
    float *            a,
    int32_t *            x,
    float *            b
)
{
    if(_g_rindow_matlib_s_gathernd==NULL) {
        _g_rindow_matlib_s_gathernd = rindow_load_rindowmatlib_func("rindow_matlib_s_gathernd"); 
        if(_g_rindow_matlib_s_gathernd==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_s_gathernd(
        reverse,
        addMode,
        m,
        n,
        k,
        indexDepth,
        paramShape,
        a,
        x,
        b    
    );
}
