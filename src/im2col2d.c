#include <math.h>
#include "rindow/matlib.h"
#include "common.h"

static inline int rindow_matlib_im2col2d_copyCell(
    int32_t dtype,
    int32_t reverse,
    void *images_data,
    int32_t images_size,
    int32_t images_pos,
    int32_t im_h,
    int32_t im_w,
    int32_t channels,
    int32_t channel_step,
    int32_t filter_h_step,
    int32_t filter_w_step,
    int32_t vim_y,
    int32_t vim_x,
    int32_t vfilter_h,
    int32_t vfilter_w,
    int32_t dilation_h,
    int32_t dilation_w,
    void *out_data,
    int32_t out_size,
    int32_t out_pos,
    int32_t out_filter_step,
    int32_t out_channel_step
    )
{
    int32_t vfilter_y;
    int32_t vfilter_x;
    int32_t filter_h_pos;
    int32_t filter_w_pos;
    int32_t out_filter_pos;
    int32_t input_y;
    int32_t input_x;
    int32_t channel_pos;
    int32_t out_channel_pos;
    int32_t c;

    filter_h_pos = images_pos;
    out_filter_pos = out_pos;
    for(vfilter_y=0; vfilter_y<vfilter_h; vfilter_y+=dilation_h) {
        filter_w_pos = filter_h_pos;
        for(vfilter_x=0; vfilter_x<vfilter_w; vfilter_x+=dilation_w) {
            channel_pos = filter_w_pos;
            out_channel_pos = out_filter_pos;
            input_y = vim_y+vfilter_y;
            input_x = vim_x+vfilter_x;
            for(c=0; c<channels; c++) {
                if(input_y<0 || input_y>=im_h ||
                   input_x<0 || input_x>=im_w) {
                    if(out_channel_pos<0 ||out_channel_pos>=out_size) {
                        return RINDOW_MATLIB_E_COLS_OUT_OF_RANGE;
                    }
                    if(!reverse) {
                        if(dtype == rindow_matlib_dtype_float32) {
                            ((float*)(out_data))[out_channel_pos]
                                = 0;
                        } else {
                            ((double*)(out_data))[out_channel_pos]
                                = 0;
                        }
                    }
                } else {
                    if(channel_pos<0 ||channel_pos>=images_size) {
                        return RINDOW_MATLIB_E_IMAGES_OUT_OF_RANGE;
                    }
                    if(out_channel_pos<0 ||out_channel_pos>=out_size) {
                        return RINDOW_MATLIB_E_COLS_OUT_OF_RANGE;
                    }
                    if(!reverse) {
                        if(dtype== rindow_matlib_dtype_float32) {
                            ((float*)(out_data))[out_channel_pos]
                                = ((float*)(images_data))[channel_pos];
                        } else {
                            ((double*)(out_data))[out_channel_pos]
                                = ((double*)(images_data))[channel_pos];
                        }
                    } else {
                        // Sum for Back propagation
                        if(dtype== rindow_matlib_dtype_float32) {
                            ((float*)(images_data))[channel_pos]
                                += ((float*)(out_data))[out_channel_pos];
                        } else {
                            ((double*)(images_data))[channel_pos]
                                += ((double*)(out_data))[out_channel_pos];
                        }
                    }
                }
                out_channel_pos += out_channel_step;
                channel_pos += channel_step;
            }
            out_filter_pos += out_filter_step;
            filter_w_pos += filter_w_step;
        }
        filter_h_pos += filter_h_step;
    }
    return 0;
}


int32_t rindow_matlib_im2col2d(
    int32_t dtype,
    int32_t reverse,
    void *images_data,
    int32_t images_size,
    int32_t batches,

    int32_t im_h,
    int32_t im_w,
    int32_t channels,
    int32_t filter_h,
    int32_t filter_w,

    int32_t stride_h,
    int32_t stride_w,
    int32_t padding,
    int32_t channels_first,
    int32_t dilation_h,

    int32_t dilation_w,
    int32_t cols_channels_first,
    void *cols_data,
    int32_t cols_size
    )
{
    if(dtype!=rindow_matlib_dtype_float32 && dtype!=rindow_matlib_dtype_float64 ) {
        return RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
    }
    if((batches*im_h*im_w*channels)!=images_size) {
        return RINDOW_MATLIB_E_UNMATCH_IMAGE_BUFFER_SIZE;
    }
    int32_t out_h = ((im_h-(filter_h-1)*dilation_h-1)/stride_h)+1;
    int32_t out_w = ((im_w-(filter_w-1)*dilation_w-1)/stride_w)+1;
    if(out_h<=0 || out_w<=0) {
        return RINDOW_MATLIB_E_INVALID_SHAPE_OR_PARAM;
    }
    int32_t padding_h;
    int32_t padding_w;
    if(padding) {
        if((batches*
            im_h*filter_h*
            im_w*filter_w*
            channels)!=cols_size) {
            return RINDOW_MATLIB_E_UNMATCH_COLS_BUFFER_SIZE;
        }
        padding_h = ((im_h-1)*stride_h-im_h+(filter_h-1)*dilation_h+1)/2;
        padding_w = ((im_w-1)*stride_w-im_w+(filter_w-1)*dilation_w+1)/2;
        out_h = im_h;
        out_w = im_w;
    } else {
        if((batches*
            out_h*filter_h*
            out_w*filter_w*
            channels)!=cols_size) {
            return RINDOW_MATLIB_E_UNMATCH_COLS_BUFFER_SIZE;
        }
        padding_h = 0;
        padding_w = 0;
    }
    int32_t im_w_step;
    int32_t im_h_step;
    int32_t channel_step;
    int32_t batch_step;
    if(channels_first) {
        im_w_step =    1;
        im_h_step =    im_w;
        channel_step = im_w*im_h;
        batch_step =   im_w*im_h*channels;
    } else {
        channel_step = 1;
        im_w_step =  channels;
        im_h_step =  channels*im_w;
        batch_step = channels*im_w*im_h;
    }
    int32_t stride_w_step = im_w_step*stride_w;
    int32_t stride_h_step = im_h_step*stride_h;
    int32_t filter_w_step = im_w_step*dilation_w;
    int32_t filter_h_step = im_h_step*dilation_h;

    int32_t out_filter_step;
    int32_t out_channel_step;
    if(cols_channels_first) {
        out_filter_step = 1;
        out_channel_step = filter_h*filter_w;
    } else {
        out_filter_step = channels;
        out_channel_step = 1;
    }
    int32_t out_cell_step = filter_h*filter_w*channels;
    int32_t batch_out_step = out_h*out_w*out_cell_step;

    int32_t batch_pos = -im_h_step*padding_h-im_w_step*padding_w;
    //out_pos = cols_offset;

    int32_t vim_h = out_h*stride_h;
    int32_t vim_w = out_w*stride_w;
    int32_t vfilter_h = filter_h*dilation_h;
    int32_t vfilter_w = filter_w*dilation_w;
    int32_t errcode = 0;

    int32_t batch;
    #pragma omp parallel for
    for(batch=0; batch<batches;batch++) {
        int32_t stride_h_pos = batch_pos+batch*batch_step;
        int32_t out_pos  = batch*batch_out_step;

        for(int32_t vim_y=0;vim_y<vim_h;vim_y+=stride_h){
            int32_t stride_w_pos = stride_h_pos;
            for(int32_t vim_x=0;vim_x<vim_w;vim_x+=stride_w) {
                if(errcode==0) {
                    int rc;
                    rc = rindow_matlib_im2col2d_copyCell(
                        dtype,
                        reverse,
                        images_data,
                        images_size,
                        stride_w_pos,
                        im_h,
                        im_w,
                        channels,
                        channel_step,
                        filter_h_step,
                        filter_w_step,
                        vim_y-padding_h,
                        vim_x-padding_w,
                        vfilter_h,
                        vfilter_w,
                        dilation_h,
                        dilation_w,
                        cols_data,
                        cols_size,
                        out_pos,
                        out_filter_step,
                        out_channel_step
                    );
                    if(rc) {
                        errcode = rc;
                    }
                }
                stride_w_pos += stride_w_step;
                out_pos += out_cell_step;
            }
            stride_h_pos += stride_h_step;
        }
    }

    return errcode;
}
