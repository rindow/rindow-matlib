#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"
#include <stdbool.h>
#include <numeric>
#include <vector>

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class Im2col2dTest : public ::testing::Test {
protected:
    std::vector<T> inputImages(
        int32_t batches,
        int32_t im_h,
        int32_t im_w,
        int32_t channels,
        int32_t channels_first
        )
    {
        auto images = std::vector<T>(
            batches*
            im_h*im_w*
            channels
        );
        Utils::range<T>(
            batches*
            im_h*im_w*
            channels,
            images.data()
        );
        //if(channels_first) {
        //    images = images->reshape({
        //        batches,
        //        channels,
        //        im_h,
        //        im_w
        //    });
        //} else {
        //    images = images->reshape({
        //        batches,
        //        im_h,
        //        im_w,
        //        channels
        //    });
        //}
        return images;
    }   

    std::vector<T> outputCols(
        int32_t batches,
        int32_t im_h,
        int32_t im_w,
        int32_t channels,
        int32_t kernel_h,
        int32_t kernel_w,
        int32_t stride_h,
        int32_t stride_w,
        int32_t padding,
        int32_t channels_first,
        int32_t dilation_h,
        int32_t dilation_w,
        int32_t cols_channels_first
        )
    {
        int32_t out_h = ((im_h-(kernel_h-1)*dilation_h-1)/stride_h)+1;
        int32_t out_w = ((im_w-(kernel_w-1)*dilation_w-1)/stride_w)+1;
        int32_t padding_h;
        int32_t padding_w;
        if(padding) {
            padding_h = ((im_h-1)*stride_h-im_h+(kernel_h-1)*dilation_h+1)/2;
            padding_w = ((im_w-1)*stride_w-im_w+(kernel_w-1)*dilation_w+1)/2;
            out_h = im_h;
            out_w = im_w;
        } else {
            padding_h = 0;
            padding_w = 0;
        }

        auto cols = std::vector<T>(
            batches*
            out_h*out_w*
            kernel_h*kernel_w*
            channels
        );
        Utils::zeros<T>(
            batches*
            out_h*out_w*
            kernel_h*kernel_w*
            channels,
            cols.data()
        );
        //if(cols_channels_first) {
        //    cols = cols->reshape({
        //        batches,
        //        out_h,out_w,
        //        channels,
        //        kernel_h,kernel_w
        //    });
        //} else {
        //    cols = cols->reshape({
        //        batches,
        //        out_h,out_w,
        //        kernel_h,kernel_w,
        //        channels
        //    });
        //}
        return cols;
    }

    std::vector<T> trueCols(
        int32_t batches,
        int32_t im_h,
        int32_t im_w,
        int32_t channels,
        int32_t kernel_h,
        int32_t kernel_w,
        int32_t stride_h,
        int32_t stride_w,
        int32_t padding,
        int32_t channels_first,
        int32_t dilation_h,
        int32_t dilation_w,
        int32_t cols_channels_first
        )
    {
        int32_t out_h = ((im_h-(kernel_h-1)*dilation_h-1)/stride_h)+1;
        int32_t out_w = ((im_w-(kernel_w-1)*dilation_w-1)/stride_w)+1;
        int32_t padding_h;
        int32_t padding_w;
        if(padding) {
            padding_h = ((im_h-1)*stride_h-im_h+(kernel_h-1)*dilation_h+1)/2;
            padding_w = ((im_w-1)*stride_w-im_w+(kernel_w-1)*dilation_w+1)/2;
            out_h = im_h;
            out_w = im_w;
        } else {
            padding_h = 0;
            padding_w = 0;
        }

        auto cols = std::vector<T>(
            batches*
            out_h*out_w*
            kernel_h*kernel_w*
            channels
        );
        Utils::zeros<T>(
            batches*
            out_h*out_w*
            kernel_h*kernel_w*
            channels,
            cols.data()
        );
        //if(cols_channels_first) {
        //    cols = cols->reshape({
        //        batches,
        //        out_h,out_w,
        //        channels,
        //        kernel_h,kernel_w
        //    });
        //} else {
        //    cols = cols->reshape({
        //        batches,
        //        out_h,out_w,
        //        kernel_h,kernel_w,
        //        channels
        //    });
        //}

        //auto truesBuffer = cols->buffer();
        for(int32_t batch_id=0;batch_id<batches;batch_id++) {
            for(int32_t channel_id=0;channel_id<channels;channel_id++) {
                for(int32_t im_y=0;im_y<out_h;im_y++) {
                    for(int32_t im_x=0;im_x<out_w;im_x++) {
                        for(int32_t kernel_y=0;kernel_y<kernel_h;kernel_y++) {
                            for(int32_t kernel_x=0;kernel_x<kernel_w;kernel_x++) {
                                int32_t input_y = im_y*stride_h+kernel_y*dilation_h-padding_h;
                                int32_t input_x = im_x*stride_w+kernel_x*dilation_w-padding_w;
                                int32_t input_id;
                                if(channels_first) {
                                    input_id = (((batch_id*channels+channel_id)*im_h+input_y)*im_w+input_x);
                                } else {
                                    input_id = (((batch_id*im_h+input_y)*im_w+input_x)*channels+channel_id);
                                }
                                int32_t cols_id;
                                if(cols_channels_first) {
                                    cols_id = (((((batch_id*out_h+im_y)*out_w+im_x)
                                                *channels+channel_id)*kernel_h+kernel_y)*kernel_w+kernel_x);
                                } else {
                                    cols_id = (((((batch_id*out_h+im_y)*out_w+im_x)
                                                *kernel_h+kernel_y)*kernel_w+kernel_x)*channels+channel_id);
                                }
                                if(input_y>=0 && input_y<im_h && input_x>=0 && input_x<im_w) {
                                    //truesBuffer->at(cols_id) = (T)input_id;
                                    cols[cols_id] = (T)input_id;
                                }
                            }
                        }
                    }
                }
            }
        }
        return cols;
    }

    std::vector<T> trueImages(
        int32_t batches,
        int32_t im_h,
        int32_t im_w,
        int32_t channels,
        int32_t kernel_h,
        int32_t kernel_w,
        int32_t stride_h,
        int32_t stride_w,
        int32_t padding,
        int32_t channels_first,
        int32_t dilation_h,
        int32_t dilation_w,
        int32_t cols_channels_first,
        std::vector<T>& trueTrueCols
        )
    {
        int32_t out_h = ((im_h-(kernel_h-1)*dilation_h-1)/stride_h)+1;
        int32_t out_w = ((im_w-(kernel_w-1)*dilation_w-1)/stride_w)+1;
        int32_t padding_h;
        int32_t padding_w;
        if(padding) {
            padding_h = ((im_h-1)*stride_h-im_h+(kernel_h-1)*dilation_h+1)/2;
            padding_w = ((im_w-1)*stride_w-im_w+(kernel_w-1)*dilation_w+1)/2;
            out_h = im_h;
            out_w = im_w;
        } else {
            padding_h = 0;
            padding_w = 0;
        }

        auto imagesTrues = std::vector<T>(
            batches*
            im_h*im_w*
            channels
        );
        Utils::zeros<T>(
            batches*
            im_h*im_w*
            channels,
            imagesTrues.data()
        );
        //if(channels_first) {
        //    imagesTrues = imagesTrues->reshape({
        //        batches,
        //        channels,
        //        im_h,
        //        im_w
        //    });
        //} else {
        //    imagesTrues = imagesTrues->reshape({
        //        batches,
        //        im_h,
        //        im_w,
        //        channels
        //    });
        //}

        //auto truesBuffer = trueTrueCols->buffer();
        //auto imageBuffer = imagesTrues->buffer();
        for(int32_t batch_id=0;batch_id<batches;batch_id++) {
            for(int32_t channel_id=0;channel_id<channels;channel_id++) {
                for(int32_t im_y=0;im_y<out_h;im_y++) {
                    for(int32_t im_x=0;im_x<out_w;im_x++) {
                        for(int32_t kernel_y=0;kernel_y<kernel_h;kernel_y++) {
                            for(int32_t kernel_x=0;kernel_x<kernel_w;kernel_x++) {
                                int32_t input_y = im_y*stride_h+kernel_y*dilation_h-padding_h;
                                int32_t input_x = im_x*stride_w+kernel_x*dilation_w-padding_w;
                                int32_t input_id;
                                if(channels_first) {
                                    input_id = (((batch_id*channels+channel_id)*im_h+input_y)*im_w+input_x);
                                } else {
                                    input_id = (((batch_id*im_h+input_y)*im_w+input_x)*channels+channel_id);
                                }
                                int32_t cols_id;
                                if(cols_channels_first) {
                                    cols_id = (((((batch_id*out_h+im_y)*out_w+im_x)
                                                *channels+channel_id)*kernel_h+kernel_y)*kernel_w+kernel_x);
                                } else {
                                    cols_id = (((((batch_id*out_h+im_y)*out_w+im_x)
                                                *kernel_h+kernel_y)*kernel_w+kernel_x)*channels+channel_id);
                                }
                                if(input_y>=0 && input_y<im_h && input_x>=0 && input_x<im_w) {
                                    //T value = imageBuffer->at(input_id);
                                    //imageBuffer->at(input_id) = value + truesBuffer->at(cols_id);
                                    imagesTrues[input_id] += trueTrueCols[cols_id];
                                }
                            }
                        }
                    }
                }
            }
        }
        return imagesTrues;
    }

    virtual int32_t test_get_dtype(
        float value
    )
    {
        return rindow_matlib_dtype_float32;
    }

    virtual int32_t test_get_dtype(
        double value
    )
    {
        return rindow_matlib_dtype_float64;
    }

    virtual void test_exec(
        int32_t dtype,
        int32_t batches,
        int32_t im_h,
        int32_t im_w,
        int32_t channels,
        int32_t kernel_h,
        int32_t kernel_w,
        int32_t stride_h,
        int32_t stride_w,
        int32_t padding,
        int32_t channels_first,
        int32_t dilation_h,
        int32_t dilation_w,
        int32_t cols_channels_first
    )
    {
        int32_t rc;

        std::vector<T> images = inputImages(
            batches,
            im_h,im_w,
            channels,
            channels_first
        );
        std::vector<T> cols = outputCols(
            batches,
            im_h,
            im_w,
            channels,
            kernel_h,
            kernel_w,
            stride_h,
            stride_w,
            padding,
            channels_first,
            dilation_h,
            dilation_w,
            cols_channels_first
        );

        rc = rindow_matlib_im2col2d(
            dtype,
            false, // reverse,
            (T*)(images.data()),
            (int32_t)(images.size()),
            batches,

            im_h,
            im_w,
            channels,
            kernel_h,
            kernel_w,

            stride_h,
            stride_w,
            padding,
            channels_first,
            dilation_h,

            dilation_w,
            cols_channels_first,
            (T*)(cols.data()),
            (int32_t)(cols.size())
        );
        ASSERT_EQ(0,rc);

        auto trues = trueCols(
            batches,
            im_h,
            im_w,
            channels,
            kernel_h,
            kernel_w,
            stride_h,
            stride_w,
            padding,
            channels_first,
            dilation_h,
            dilation_w,
            cols_channels_first
        );

        auto equal = std::equal(
            cols.begin(),cols.end(),
            trues.begin());
        EXPECT_TRUE(equal);

        auto newImages = inputImages(
            batches,
            im_h,im_w,
            channels,
            channels_first
        );
        std::fill(newImages.begin(),newImages.end(),(T)0);

        rc = rindow_matlib_im2col2d(
            dtype,
            true, // reverse,
            (T*)(newImages.data()),
            (int32_t)(newImages.size()),
            batches,

            im_h,
            im_w,
            channels,
            kernel_h,
            kernel_w,

            stride_h,
            stride_w,
            padding,
            channels_first,
            dilation_h,

            dilation_w,
            cols_channels_first,
            (T*)(cols.data()),
            (int32_t)(cols.size())
        );
        ASSERT_EQ(0,rc);

        auto truesImg = trueImages(
            batches,
            im_h,
            im_w,
            channels,
            kernel_h,
            kernel_w,
            stride_h,
            stride_w,
            padding,
            channels_first,
            dilation_h,
            dilation_w,
            cols_channels_first,
            trues
        );

        auto equal2 = std::equal(
            newImages.begin(),newImages.end(),
            truesImg.begin());
        EXPECT_TRUE(equal2);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(Im2col2dTest, TestTypes);

TYPED_TEST(Im2col2dTest, normal) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, kernel_h) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 4;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, kernel_w) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 4;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, stride_h) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 2;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, stride_w) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 2;
    const int32_t padding = false;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, dilation_h) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = false;
    const int32_t dilation_h = 2;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, dilation_w) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 2;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, normal_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = true;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, kernel_h_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 4;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = true;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, kernel_w_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 4;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = true;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, stride_h_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 2;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = true;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, stride_w_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 2;
    const int32_t padding = false;
    const int32_t channels_first = true;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, dilation_h_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = true;
    const int32_t dilation_h = 2;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, dilation_w_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = true;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 2;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, normal_padding) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = true;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, kernel_h_padding) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 4;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = true;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, kernel_w_padding) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 4;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = true;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, stride_h_padding) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 2;
    const int32_t stride_w = 1;
    const int32_t padding = true;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, stride_w_padding) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 2;
    const int32_t padding = true;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, dilation_h_padding) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = true;
    const int32_t channels_first = false;
    const int32_t dilation_h = 2;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = false;
    
    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, dilation_w_padding) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = true;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 2;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, normal_cols_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = true;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, kernel_h_cols_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 4;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = true;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, kernel_w_cols_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 4;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = true;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, stride_h_cols_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 2;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = true;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, stride_w_cols_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 2;
    const int32_t padding = false;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = true;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, dilation_h_cols_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = false;
    const int32_t dilation_h = 2;
    const int32_t dilation_w = 1;
    const int32_t cols_channels_first = true;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col2dTest, dilation_w_cols_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_h = 8;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_h = 3;
    const int32_t kernel_w = 3;
    const int32_t stride_h = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t channels_first = false;
    const int32_t dilation_h = 1;
    const int32_t dilation_w = 2;
    const int32_t cols_channels_first = true;

    this->test_exec(
        dtype,
        batches,
        im_h,
        im_w,
        channels,
        kernel_h,
        kernel_w,
        stride_h,
        stride_w,
        padding,
        channels_first,
        dilation_h,
        dilation_w,
        cols_channels_first
    );
}

}
