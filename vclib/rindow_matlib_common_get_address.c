#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_common_get_address not found\n";
typedef void* (CALLBACK* PFNrindow_matlib_common_get_address)( /* rindow_matlib_common_get_address */
    int32_t            /* dtype */,
    void *            /* buffer */,
    int32_t            /* offset */
);
static PFNrindow_matlib_common_get_address _g_rindow_matlib_common_get_address = NULL;
void* rindow_matlib_common_get_address(
    int32_t            dtype,
    void *            buffer,
    int32_t            offset
)
{
    if(_g_rindow_matlib_common_get_address==NULL) {
        _g_rindow_matlib_common_get_address = rindow_load_rindowmatlib_func("rindow_matlib_common_get_address"); 
        if(_g_rindow_matlib_common_get_address==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_common_get_address(
        dtype,
        buffer,
        offset    
    );
}
