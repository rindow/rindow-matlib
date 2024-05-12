#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_slice not found\n";
typedef void (CALLBACK* PFNrindow_matlib_s_slice)( /* rindow_matlib_s_slice */
    int32_t            /* reverse */,
    int32_t            /* addMode */,
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* k */,
    int32_t            /* size */,
    float *            /* a */,
    int32_t            /* incA */,
    float *            /* y */,
    int32_t            /* incY */,
    int32_t            /* startAxis0 */,
    int32_t            /* sizeAxis0 */,
    int32_t            /* startAxis1 */,
    int32_t            /* sizeAxis1 */,
    int32_t            /* startAxis2 */,
    int32_t            /* sizeAxis2 */
);
static PFNrindow_matlib_s_slice _g_rindow_matlib_s_slice = NULL;
void rindow_matlib_s_slice(
    int32_t            reverse,
    int32_t            addMode,
    int32_t            m,
    int32_t            n,
    int32_t            k,
    int32_t            size,
    float *            a,
    int32_t            incA,
    float *            y,
    int32_t            incY,
    int32_t            startAxis0,
    int32_t            sizeAxis0,
    int32_t            startAxis1,
    int32_t            sizeAxis1,
    int32_t            startAxis2,
    int32_t            sizeAxis2
)
{
    if(_g_rindow_matlib_s_slice==NULL) {
        _g_rindow_matlib_s_slice = rindow_load_rindowmatlib_func("rindow_matlib_s_slice"); 
        if(_g_rindow_matlib_s_slice==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_s_slice(
        reverse,
        addMode,
        m,
        n,
        k,
        size,
        a,
        incA,
        y,
        incY,
        startAxis0,
        sizeAxis0,
        startAxis1,
        sizeAxis1,
        startAxis2,
        sizeAxis2    
    );
}
