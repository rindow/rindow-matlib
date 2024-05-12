#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_common_get_nprocs not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_common_get_nprocs)( /* rindow_matlib_common_get_nprocs */
    void            /*  */
);
static PFNrindow_matlib_common_get_nprocs _g_rindow_matlib_common_get_nprocs = NULL;
int32_t rindow_matlib_common_get_nprocs(
    void            
)
{
    if(_g_rindow_matlib_common_get_nprocs==NULL) {
        _g_rindow_matlib_common_get_nprocs = rindow_load_rindowmatlib_func("rindow_matlib_common_get_nprocs"); 
        if(_g_rindow_matlib_common_get_nprocs==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_common_get_nprocs(
    
    );
}
