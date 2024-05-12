#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_common_get_num_threads not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_common_get_num_threads)( /* rindow_matlib_common_get_num_threads */
    void            /*  */
);
static PFNrindow_matlib_common_get_num_threads _g_rindow_matlib_common_get_num_threads = NULL;
int32_t rindow_matlib_common_get_num_threads(
    void            
)
{
    if(_g_rindow_matlib_common_get_num_threads==NULL) {
        _g_rindow_matlib_common_get_num_threads = rindow_load_rindowmatlib_func("rindow_matlib_common_get_num_threads"); 
        if(_g_rindow_matlib_common_get_num_threads==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_common_get_num_threads(
    
    );
}
