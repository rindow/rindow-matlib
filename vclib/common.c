#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"

static HMODULE _h_rindowmatlib = NULL;
static char msg_load_error[]  = "Load error: rindowmatlib\n";
void* rindow_load_rindowmatlib_func(char *funcname)
{
    if(_h_rindowmatlib==NULL) {
        _h_rindowmatlib = LoadLibraryA( "rindowmatlib.dll" );
        if(_h_rindowmatlib==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_load_error, sizeof(msg_load_error), NULL, NULL);
            return NULL;
        }
    }
    return GetProcAddress( _h_rindowmatlib, funcname );
}
