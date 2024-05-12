#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_randomnormal not found\n";
typedef void (CALLBACK* PFNrindow_matlib_d_randomnormal)( /* rindow_matlib_d_randomnormal */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */,
    double            /* mean */,
    double            /* scale */,
    int32_t            /* seed */
);
static PFNrindow_matlib_d_randomnormal _g_rindow_matlib_d_randomnormal = NULL;
void rindow_matlib_d_randomnormal(
    int32_t            n,
    double *            x,
    int32_t            incX,
    double            mean,
    double            scale,
    int32_t            seed
)
{
    if(_g_rindow_matlib_d_randomnormal==NULL) {
        _g_rindow_matlib_d_randomnormal = rindow_load_rindowmatlib_func("rindow_matlib_d_randomnormal"); 
        if(_g_rindow_matlib_d_randomnormal==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_d_randomnormal(
        n,
        x,
        incX,
        mean,
        scale,
        seed    
    );
}
