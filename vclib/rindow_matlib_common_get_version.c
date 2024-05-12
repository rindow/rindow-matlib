#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_common_get_version not found\n";
typedef char* (CALLBACK* PFNrindow_matlib_common_get_version)( /* rindow_matlib_common_get_version */
    void            /*  */
);
static PFNrindow_matlib_common_get_version _g_rindow_matlib_common_get_version = NULL;
char* rindow_matlib_common_get_version(
    void            
)
{
    if(_g_rindow_matlib_common_get_version==NULL) {
        _g_rindow_matlib_common_get_version = rindow_load_rindowmatlib_func("rindow_matlib_common_get_version"); 
        if(_g_rindow_matlib_common_get_version==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_common_get_version(
    
    );
}
