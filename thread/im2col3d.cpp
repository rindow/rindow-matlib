#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Im2col3d
{
private:
    static inline int copyCell(
            int32_t reverse,
            T *images_data,
            int32_t images_size,
            int32_t images_pos,
            int32_t im_d,
            int32_t im_h,
            int32_t im_w,
            int32_t channels,
            int32_t channel_step,
            int32_t filter_d_step,
            int32_t filter_h_step,
            int32_t filter_w_step,
            int32_t vim_z,
            int32_t vim_y,
            int32_t vim_x,
            int32_t vfilter_d,
            int32_t vfilter_h,
            int32_t vfilter_w,
            int32_t dilation_d,
            int32_t dilation_h,
            int32_t dilation_w,
            T *out_data,
            int32_t out_size,
            int32_t out_pos,
            int32_t out_filter_step,
            int32_t out_channel_step
        )
    {
        int32_t vfilter_z;
        int32_t vfilter_y;
        int32_t vfilter_x;
        int32_t filter_d_pos;
        int32_t filter_h_pos;
        int32_t filter_w_pos;
        int32_t out_filter_pos;
        int32_t input_z;
        int32_t input_y;
        int32_t input_x;
        int32_t channel_pos;
        int32_t out_channel_pos;
        int32_t c;

        filter_d_pos = images_pos;
        out_filter_pos = out_pos;
        for(vfilter_z=0; vfilter_z<vfilter_d; vfilter_z+=dilation_d) {
            filter_h_pos = filter_d_pos;
            for(vfilter_y=0; vfilter_y<vfilter_h; vfilter_y+=dilation_h) {
                filter_w_pos = filter_h_pos;
                for(vfilter_x=0; vfilter_x<vfilter_w; vfilter_x+=dilation_w) {
                    channel_pos = filter_w_pos;
                    out_channel_pos = out_filter_pos;
                    input_z = vim_z+vfilter_z;
                    input_y = vim_y+vfilter_y;
                    input_x = vim_x+vfilter_x;
                    for(c=0; c<channels; c++) {
                        if(input_z<0 || input_z>=im_d ||
                           input_y<0 || input_y>=im_h ||
                           input_x<0 || input_x>=im_w) {
                            if(out_channel_pos<0 ||out_channel_pos>=out_size) {
                                return RINDOW_MATLIB_E_COLS_OUT_OF_RANGE;
                            }
                            if(!reverse) {
                                out_data[out_channel_pos] = 0;
                            }
                        } else {
                            if(channel_pos<0 ||channel_pos>=images_size) {
                                return RINDOW_MATLIB_E_IMAGES_OUT_OF_RANGE;
                            }
                            if(out_channel_pos<0 ||out_channel_pos>=out_size) {
                                return RINDOW_MATLIB_E_COLS_OUT_OF_RANGE;
                            }
                            if(!reverse) {
                                out_data[out_channel_pos] = images_data[channel_pos];
                            } else {
                                // Sum for Back propagation
                                images_data[channel_pos] += out_data[out_channel_pos];
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
            filter_d_pos += filter_d_step;
        }
        return 0;
    }

    static int32_t kernel(
        ParallelOperation::cellInfo cell,
        const int32_t reverse,
        T *images_data,
        const int32_t images_size,
        const int32_t im_d,
        const int32_t im_h,
        const int32_t im_w,
        const int32_t channels,
        const int32_t stride_d,
        const int32_t stride_h,
        const int32_t stride_w,
        const int32_t dilation_d,
        const int32_t dilation_h,
        const int32_t dilation_w,
        T *cols_data,
        const int32_t cols_size,

        const int32_t vim_d,
        const int32_t vim_h,
        const int32_t vim_w,
        const int32_t vfilter_d,
        const int32_t vfilter_h,
        const int32_t vfilter_w,
        const int32_t padding_d,
        const int32_t padding_h,
        const int32_t padding_w,
        const int32_t batch_pos,
        const int32_t batch_step,
        const int32_t batch_out_step,
        const int32_t channel_step,
        const int32_t filter_d_step,
        const int32_t filter_h_step,
        const int32_t filter_w_step,
        const int32_t stride_d_step,
        const int32_t stride_h_step,
        const int32_t stride_w_step,
        const int32_t out_filter_step,
        const int32_t out_channel_step,
        const int32_t out_cell_step
    )
    {
        int32_t errcode = 0;
        for(int32_t batch = cell.begin; batch < cell.end; batch++) {
            int32_t stride_d_pos = batch_pos+batch*batch_step;
            int32_t out_pos  = batch*batch_out_step;

            for(int32_t vim_z=0;vim_z<vim_d;vim_z+=stride_d){
                int32_t stride_h_pos = stride_d_pos;
                for(int32_t vim_y=0;vim_y<vim_h;vim_y+=stride_h){
                    int32_t stride_w_pos = stride_h_pos;
                    for(int32_t vim_x=0;vim_x<vim_w;vim_x+=stride_w) {
                        if(errcode==0) {
                            int rc;
                            rc = copyCell(
                                reverse,
                                images_data,
                                images_size,
                                stride_w_pos,
                                im_d,
                                im_h,
                                im_w,
                                channels,
                                channel_step,
                                filter_d_step,
                                filter_h_step,
                                filter_w_step,
                                vim_z-padding_d,
                                vim_y-padding_h,
                                vim_x-padding_w,
                                vfilter_d,
                                vfilter_h,
                                vfilter_w,
                                dilation_d,
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
                stride_d_pos += stride_d_step;
            }
        }
        return errcode;
    }

public:
    static int32_t execute(
        const int32_t reverse,
        T *images_data,
        const int32_t images_size,
        const int32_t batches,

        const int32_t im_d,
        const int32_t im_h,
        const int32_t im_w,
        const int32_t channels,
        const int32_t filter_d,
        const int32_t filter_h,
        const int32_t filter_w,

        const int32_t stride_d,
        const int32_t stride_h,
        const int32_t stride_w,
        const int32_t padding,
        const int32_t channels_first,
        const int32_t dilation_d,
        const int32_t dilation_h,

        const int32_t dilation_w,
        const int32_t cols_channels_first,
        T *cols_data,
        const int32_t cols_size
    )
    {
        if((batches*im_d*im_h*im_w*channels)!=images_size) {
            return RINDOW_MATLIB_E_UNMATCH_IMAGE_BUFFER_SIZE;
        }
        int32_t out_d = ((im_d-(filter_d-1)*dilation_d-1)/stride_d)+1;
        int32_t out_h = ((im_h-(filter_h-1)*dilation_h-1)/stride_h)+1;
        int32_t out_w = ((im_w-(filter_w-1)*dilation_w-1)/stride_w)+1;
        if(out_d<=0 || out_h<=0 || out_w<=0) {
            return RINDOW_MATLIB_E_INVALID_SHAPE_OR_PARAM;
        }
        int32_t padding_d;
        int32_t padding_h;
        int32_t padding_w;
        if(padding) {
            if((batches*
                im_d*filter_d*
                im_h*filter_h*
                im_w*filter_w*
                channels)!=cols_size) {
                return RINDOW_MATLIB_E_UNMATCH_COLS_BUFFER_SIZE;
            }
            padding_d = ((im_d-1)*stride_d-im_d+(filter_d-1)*dilation_d+1)/2;
            padding_h = ((im_h-1)*stride_h-im_h+(filter_h-1)*dilation_h+1)/2;
            padding_w = ((im_w-1)*stride_w-im_w+(filter_w-1)*dilation_w+1)/2;
            out_d = im_d;
            out_h = im_h;
            out_w = im_w;
        } else {
            if((batches*
                out_d*filter_d*
                out_h*filter_h*
                out_w*filter_w*
                channels)!=cols_size) {
                return RINDOW_MATLIB_E_UNMATCH_COLS_BUFFER_SIZE;
            }
            padding_d = 0;
            padding_h = 0;
            padding_w = 0;
        }
        int32_t im_d_step;
        int32_t im_w_step;
        int32_t im_h_step;
        int32_t batch_step;
        int32_t channel_step;
        if(channels_first) {
            im_w_step =    1;
            im_h_step =    im_w;
            im_d_step =    im_w*im_h;
            channel_step = im_w*im_h*im_d;
            batch_step =   im_w*im_h*im_d*channels;
        } else {
            channel_step = 1;
            im_w_step =  channels;
            im_h_step =  channels*im_w;
            im_d_step =  channels*im_w*im_h;
            batch_step = channels*im_w*im_h*im_d;
        }
        int32_t stride_d_step = im_d_step*stride_d;
        int32_t stride_w_step = im_w_step*stride_w;
        int32_t stride_h_step = im_h_step*stride_h;
        int32_t filter_d_step = im_d_step*dilation_d;
        int32_t filter_w_step = im_w_step*dilation_w;
        int32_t filter_h_step = im_h_step*dilation_h;

        int32_t out_filter_step;
        int32_t out_channel_step;
        if(cols_channels_first) {
            out_filter_step = 1;
            out_channel_step = filter_d*filter_h*filter_w;
        } else {
            out_filter_step = channels;
            out_channel_step = 1;
        }
        int32_t out_cell_step = filter_d*filter_h*filter_w*channels;
        int32_t batch_out_step = out_d*out_h*out_w*out_cell_step;

        int32_t batch_pos = -im_d_step*padding_d-im_h_step*padding_h-im_w_step*padding_w;

        int32_t vim_d = out_d*stride_d;
        int32_t vim_h = out_h*stride_h;
        int32_t vim_w = out_w*stride_w;
        int32_t vfilter_d = filter_d*dilation_d;
        int32_t vfilter_h = filter_h*dilation_h;
        int32_t vfilter_w = filter_w*dilation_w;

        int32_t errcode = ParallelOperation::executeAndGetCode<int32_t>(
            batches,
            kernel,
            reverse,
            images_data,
            images_size,
            im_d,
            im_h,
            im_w,
            channels,
            stride_d,
            stride_h,
            stride_w,
            dilation_d,
            dilation_h,
            dilation_w,
            cols_data,
            cols_size,

            vim_d,
            vim_h,
            vim_w,
            vfilter_d,
            vfilter_h,
            vfilter_w,
            padding_d,
            padding_h,
            padding_w,
            batch_pos,
            batch_step,
            batch_out_step,
            channel_step,
            filter_d_step,
            filter_h_step,
            filter_w_step,
            stride_d_step,
            stride_h_step,
            stride_w_step,
            out_filter_step,
            out_channel_step,
            out_cell_step
        );
        return errcode;
    }
};

}

extern "C" {
int32_t rindow_matlib_im2col3d(
    int32_t dtype,
    int32_t reverse,
    void *images_data,
    int32_t images_size,
    int32_t batches,

    int32_t im_d,
    int32_t im_h,
    int32_t im_w,
    int32_t channels,
    int32_t filter_d,
    int32_t filter_h,
    int32_t filter_w,

    int32_t stride_d,
    int32_t stride_h,
    int32_t stride_w,
    int32_t padding,
    int32_t channels_first,
    int32_t dilation_d,
    int32_t dilation_h,

    int32_t dilation_w,
    int32_t cols_channels_first,
    void *cols_data,
    int32_t cols_size
    )
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_float32: {
            errcode = Im2col3d<float>::execute(
                reverse,
                (float*)images_data,images_size,
                batches,im_d,im_h,im_w,channels,
                filter_d,filter_h,filter_w,stride_d,stride_h,stride_w,padding,
                channels_first,
                dilation_d,dilation_h,dilation_w,
                cols_channels_first,
                (float*)cols_data,cols_size
            );
            break;
        }
        case rindow_matlib_dtype_float64: {
            errcode = Im2col3d<double>::execute(
                reverse,
                (double*)images_data,images_size,
                batches,im_d,im_h,im_w,channels,
                filter_d,filter_h,filter_w,stride_d,stride_h,stride_w,padding,
                channels_first,
                dilation_d,dilation_h,dilation_w,
                cols_channels_first,
                (double*)cols_data,cols_size
            );
            break;
        }
        case rindow_matlib_dtype_int8: {
            errcode = Im2col3d<int8_t>::execute(
                reverse,
                (int8_t*)images_data,images_size,
                batches,im_d,im_h,im_w,channels,
                filter_d,filter_h,filter_w,stride_d,stride_h,stride_w,padding,
                channels_first,
                dilation_d,dilation_h,dilation_w,
                cols_channels_first,
                (int8_t*)cols_data,cols_size
            );
            break;
        }
        case rindow_matlib_dtype_uint8: {
            errcode = Im2col3d<uint8_t>::execute(
                reverse,
                (uint8_t*)images_data,images_size,
                batches,im_d,im_h,im_w,channels,
                filter_d,filter_h,filter_w,stride_d,stride_h,stride_w,padding,
                channels_first,
                dilation_d,dilation_h,dilation_w,
                cols_channels_first,
                (uint8_t*)cols_data,cols_size
            );
            break;
        }
        case rindow_matlib_dtype_int16: {
            errcode = Im2col3d<int16_t>::execute(
                reverse,
                (int16_t*)images_data,images_size,
                batches,im_d,im_h,im_w,channels,
                filter_d,filter_h,filter_w,stride_d,stride_h,stride_w,padding,
                channels_first,
                dilation_d,dilation_h,dilation_w,
                cols_channels_first,
                (int16_t*)cols_data,cols_size
            );
            break;
        }
        case rindow_matlib_dtype_uint16: {
            errcode = Im2col3d<uint16_t>::execute(
                reverse,
                (uint16_t*)images_data,images_size,
                batches,im_d,im_h,im_w,channels,
                filter_d,filter_h,filter_w,stride_d,stride_h,stride_w,padding,
                channels_first,
                dilation_d,dilation_h,dilation_w,
                cols_channels_first,
                (uint16_t*)cols_data,cols_size
            );
            break;
        }
        case rindow_matlib_dtype_int32: {
            errcode = Im2col3d<int32_t>::execute(
                reverse,
                (int32_t*)images_data,images_size,
                batches,im_d,im_h,im_w,channels,
                filter_d,filter_h,filter_w,stride_d,stride_h,stride_w,padding,
                channels_first,
                dilation_d,dilation_h,dilation_w,
                cols_channels_first,
                (int32_t*)cols_data,cols_size
            );
            break;
        }
        case rindow_matlib_dtype_uint32: {
            errcode = Im2col3d<uint32_t>::execute(
                reverse,
                (uint32_t*)images_data,images_size,
                batches,im_d,im_h,im_w,channels,
                filter_d,filter_h,filter_w,stride_d,stride_h,stride_w,padding,
                channels_first,
                dilation_d,dilation_h,dilation_w,
                cols_channels_first,
                (uint32_t*)cols_data,cols_size
            );
            break;
        }
        case rindow_matlib_dtype_int64: {
            errcode = Im2col3d<int64_t>::execute(
                reverse,
                (int64_t*)images_data,images_size,
                batches,im_d,im_h,im_w,channels,
                filter_d,filter_h,filter_w,stride_d,stride_h,stride_w,padding,
                channels_first,
                dilation_d,dilation_h,dilation_w,
                cols_channels_first,
                (int64_t*)cols_data,cols_size
            );
            break;
        }
        case rindow_matlib_dtype_uint64: {
            errcode = Im2col3d<uint64_t>::execute(
                reverse,
                (uint64_t*)images_data,images_size,
                batches,im_d,im_h,im_w,channels,
                filter_d,filter_h,filter_w,stride_d,stride_h,stride_w,padding,
                channels_first,
                dilation_d,dilation_h,dilation_w,
                cols_channels_first,
                (uint64_t*)cols_data,cols_size
            );
            break;
        }
        default: {
            errcode = RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
            break;
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

}
