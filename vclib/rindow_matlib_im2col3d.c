#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_im2col3d not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_im2col3d)( /* rindow_matlib_im2col3d */
    int32_t            /* dtype */,
    int32_t            /* reverse */,
    void *            /* images_data */,
    int32_t            /* images_size */,
    int32_t            /* batches */,
    int32_t            /* im_d */,
    int32_t            /* im_h */,
    int32_t            /* im_w */,
    int32_t            /* channels */,
    int32_t            /* filter_d */,
    int32_t            /* filter_h */,
    int32_t            /* filter_w */,
    int32_t            /* stride_d */,
    int32_t            /* stride_h */,
    int32_t            /* stride_w */,
    int32_t            /* padding */,
    int32_t            /* channels_first */,
    int32_t            /* dilation_d */,
    int32_t            /* dilation_h */,
    int32_t            /* dilation_w */,
    int32_t            /* cols_channels_first */,
    void *            /* cols_data */,
    int32_t            /* cols_size */
);
static PFNrindow_matlib_im2col3d _g_rindow_matlib_im2col3d = NULL;
int32_t rindow_matlib_im2col3d(
    int32_t            dtype,
    int32_t            reverse,
    void *            images_data,
    int32_t            images_size,
    int32_t            batches,
    int32_t            im_d,
    int32_t            im_h,
    int32_t            im_w,
    int32_t            channels,
    int32_t            filter_d,
    int32_t            filter_h,
    int32_t            filter_w,
    int32_t            stride_d,
    int32_t            stride_h,
    int32_t            stride_w,
    int32_t            padding,
    int32_t            channels_first,
    int32_t            dilation_d,
    int32_t            dilation_h,
    int32_t            dilation_w,
    int32_t            cols_channels_first,
    void *            cols_data,
    int32_t            cols_size
)
{
    if(_g_rindow_matlib_im2col3d==NULL) {
        _g_rindow_matlib_im2col3d = rindow_load_rindowmatlib_func("rindow_matlib_im2col3d"); 
        if(_g_rindow_matlib_im2col3d==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_im2col3d(
        dtype,
        reverse,
        images_data,
        images_size,
        batches,
        im_d,
        im_h,
        im_w,
        channels,
        filter_d,
        filter_h,
        filter_w,
        stride_d,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_d,
        dilation_h,
        dilation_w,
        cols_channels_first,
        cols_data,
        cols_size    
    );
}
