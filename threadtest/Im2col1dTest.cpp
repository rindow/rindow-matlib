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
class Im2col1dTest : public ::testing::Test {
protected:
    std::vector<T> inputImages(
        int32_t batches,
        int32_t im_w,
        int32_t channels,
        int32_t channels_first
        )
    {
        auto images = std::vector<T>(
            batches*
            im_w*
            channels
        );
        Utils::range<T>(0,
            batches*
            im_w*
            channels,
            images.data()
        );
        //if(channels_first) {
        //    images = images->reshape({
        //        batches,
        //        channels,
        //        im_w
        //    });
        //} else {
        //    images = images->reshape({
        //        batches,
        //        im_w,
        //        channels
        //    });
        //}
        return images;
    }   

    std::vector<T> outputCols(
        int32_t batches,
        int32_t im_w,
        int32_t channels,
        int32_t kernel_w,
        int32_t stride_w,
        int32_t padding,
        int32_t channels_first,
        int32_t dilation_w,
        int32_t cols_channels_first
        )
    {
        int32_t out_w = ((im_w-(kernel_w-1)*dilation_w-1)/stride_w)+1;
        int32_t padding_w;
        if(padding) {
            padding_w = ((im_w-1)*stride_w-im_w+(kernel_w-1)*dilation_w+1)/2;
            out_w = im_w;
        } else {
            padding_w = 0;
        }

        auto cols = std::vector<T>(
            batches*
            out_w*
            kernel_w*
            channels
        );
        Utils::zeros<T>(
            batches*
            out_w*
            kernel_w*
            channels,
            cols.data()
        );
        //if(cols_channels_first) {
        //    cols = cols->reshape({
        //        batches,
        //        out_w,
        //        channels,
        //        kernel_w
        //    });
        //} else {
        //    cols = cols->reshape({
        //        batches,
        //        out_w,
        //        kernel_w,
        //        channels
        //    });
        //}
        return cols;
    }

    std::vector<T> trueCols(
        int32_t batches,
        int32_t im_w,
        int32_t channels,
        int32_t kernel_w,
        int32_t stride_w,
        int32_t padding,
        int32_t channels_first,
        int32_t dilation_w,
        int32_t cols_channels_first
        )
    {
        int32_t out_w = ((im_w-(kernel_w-1)*dilation_w-1)/stride_w)+1;
        int32_t padding_w;
        if(padding) {
            padding_w = ((im_w-1)*stride_w-im_w+(kernel_w-1)*dilation_w+1)/2;
            out_w = im_w;
        } else {
            padding_w = 0;
        }

        auto cols = std::vector<T>(
            batches*
            out_w*
            kernel_w*
            channels
        );
        Utils::zeros<T>(
            batches*
            out_w*
            kernel_w*
            channels,
            cols.data()
        );
        //if(cols_channels_first) {
        //    cols = cols->reshape({
        //        batches,
        //        out_w,
        //        channels,
        //        kernel_w
        //    });
        //} else {
        //    cols = cols->reshape({
        //        batches,
        //        out_w,
        //        kernel_w,
        //        channels
        //    });
        //}

        //auto truesBuffer = cols->buffer();
        for(int32_t batch_id=0;batch_id<batches;batch_id++) {
            for(int32_t channel_id=0;channel_id<channels;channel_id++) {
                    for(int32_t im_x=0;im_x<out_w;im_x++) {
                            for(int32_t kernel_x=0;kernel_x<kernel_w;kernel_x++) {
                                int32_t input_x = im_x*stride_w+kernel_x*dilation_w-padding_w;
                                int32_t input_id;
                                if(channels_first) {
                                    input_id = ((batch_id*channels+channel_id)*im_w+input_x);
                                } else {
                                    input_id = ((batch_id*im_w+input_x)*channels+channel_id);
                                }
                                int32_t cols_id;
                                if(cols_channels_first) {
                                    cols_id = (((batch_id*out_w+im_x)
                                                *channels+channel_id)*kernel_w+kernel_x);
                                } else {
                                    cols_id = (((batch_id*out_w+im_x)
                                                *kernel_w+kernel_x)*channels+channel_id);
                                }
                                if(input_x>=0 && input_x<im_w) {
                                    //truesBuffer->at(cols_id) = (T)input_id;
                                    cols[cols_id] = (T)input_id;
                                }
                            }
                    }
            }
        }
        return cols;
    }

    std::vector<T> trueImages(
        int32_t batches,
        int32_t im_w,
        int32_t channels,
        int32_t kernel_w,
        int32_t stride_w,
        int32_t padding,
        int32_t channels_first,
        int32_t dilation_w,
        int32_t cols_channels_first,
        std::vector<T>& trueTrueCols
        )
    {
        int32_t out_w = ((im_w-(kernel_w-1)*dilation_w-1)/stride_w)+1;
        int32_t padding_w;
        if(padding) {
            padding_w = ((im_w-1)*stride_w-im_w+(kernel_w-1)*dilation_w+1)/2;
            out_w = im_w;
        } else {
            padding_w = 0;
        }

        auto imagesTrues = std::vector<T>(
            batches*
            im_w*
            channels
        );
        Utils::zeros<T>(
            batches*
            im_w*
            channels,
            imagesTrues.data()
        );
        //if(channels_first) {
        //    imagesTrues = imagesTrues->reshape({
        //        batches,
        //        channels,
        //        im_w
        //    });
        //} else {
        //    imagesTrues = imagesTrues->reshape({
        //        batches,
        //        im_w,
        //        channels
        //    });
        //}

        //auto truesBuffer = trueTrueCols->buffer();
        //auto imageBuffer = imagesTrues->buffer();
        for(int32_t batch_id=0;batch_id<batches;batch_id++) {
            for(int32_t channel_id=0;channel_id<channels;channel_id++) {
                    for(int32_t im_x=0;im_x<out_w;im_x++) {
                            for(int32_t kernel_x=0;kernel_x<kernel_w;kernel_x++) {
                                int32_t input_x = im_x*stride_w+kernel_x*dilation_w-padding_w;
                                int32_t input_id;
                                if(channels_first) {
                                    input_id = ((batch_id*channels+channel_id)*im_w+input_x);
                                } else {
                                    input_id = ((batch_id*im_w+input_x)*channels+channel_id);
                                }
                                int32_t cols_id;
                                if(cols_channels_first) {
                                    cols_id = (((batch_id*out_w+im_x)
                                                *channels+channel_id)*kernel_w+kernel_x);
                                } else {
                                    cols_id = (((batch_id*out_w+im_x)
                                                *kernel_w+kernel_x)*channels+channel_id);
                                }
                                if(input_x>=0 && input_x<im_w) {
                                    //T value = imageBuffer->at(input_id);
                                    //imageBuffer->at(input_id) = value + truesBuffer->at(cols_id);
                                    imagesTrues[input_id] += trueTrueCols[cols_id];
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
        int32_t im_w,
        int32_t channels,
        int32_t kernel_w,
        int32_t stride_w,
        int32_t padding,
        int32_t channels_first,
        int32_t dilation_w,
        int32_t cols_channels_first
    )
    {
        int32_t rc;

        auto images = inputImages(
            batches,
            im_w,
            channels,
            channels_first
        );
        auto cols = outputCols(
            batches,
            im_w,
            channels,
            kernel_w,
            stride_w,
            padding,
            channels_first,
            dilation_w,
            cols_channels_first
        );

        rc = rindow_matlib_im2col1d(
            dtype,
            false, // reverse,
            images.data(),
            (int32_t)images.size(),
            batches,

            im_w,
            channels,
            kernel_w,

            stride_w,
            padding,
            channels_first,

            dilation_w,
            cols_channels_first,
            cols.data(),
            (int32_t)cols.size()
        );
        ASSERT_EQ(0,rc);

        auto trues = trueCols(
            batches,
            im_w,
            channels,
            kernel_w,
            stride_w,
            padding,
            channels_first,
            dilation_w,
            cols_channels_first
        );

        auto equal = std::equal(
            cols.begin(),cols.end(),
            trues.begin());
        EXPECT_TRUE(equal);

        auto newImages = inputImages(
            batches,
            im_w,
            channels,
            channels_first
        );
        std::fill(newImages.begin(),newImages.end(),(T)0);

        rc = rindow_matlib_im2col1d(
            dtype,
            true, // reverse,
            newImages.data(),
            (int32_t)newImages.size(),
            batches,

            im_w,
            channels,
            kernel_w,

            stride_w,
            padding,
            channels_first,

            dilation_w,
            cols_channels_first,
            cols.data(),
            (int32_t)cols.size()
        );
        ASSERT_EQ(0,rc);

        auto truesImg = trueImages(
            batches,
            im_w,
            channels,
            kernel_w,
            stride_w,
            padding,
            channels_first,
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
TYPED_TEST_SUITE(Im2col1dTest, TestTypes);

TYPED_TEST(Im2col1dTest, normal) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t dilation_w = 1;
    const int32_t channels_first = false;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col1dTest, kernel_w) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 2;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t dilation_w = 1;
    const int32_t channels_first = false;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col1dTest, stride_w) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 1;
    const int32_t stride_w = 2;
    const int32_t padding = false;
    const int32_t dilation_w = 1;
    const int32_t channels_first = false;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col1dTest, dilation_w) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t dilation_w = 2;
    const int32_t channels_first = false;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col1dTest, normal_padding) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 1;
    const int32_t stride_w = 1;
    const int32_t padding = true;
    const int32_t dilation_w = 1;
    const int32_t channels_first = false;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col1dTest, kernel_w_padding) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 2;
    const int32_t stride_w = 1;
    const int32_t padding = true;
    const int32_t dilation_w = 1;
    const int32_t channels_first = false;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col1dTest, stride_w_padding) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 1;
    const int32_t stride_w = 2;
    const int32_t padding = true;
    const int32_t dilation_w = 1;
    const int32_t channels_first = false;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col1dTest, dilation_w_padding) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 1;
    const int32_t stride_w = 1;
    const int32_t padding = true;
    const int32_t dilation_w = 2;
    const int32_t channels_first = false;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col1dTest, normal_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t dilation_w = 1;
    const int32_t channels_first = true;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col1dTest, kernel_w_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 2;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t dilation_w = 1;
    const int32_t channels_first = true;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col1dTest, stride_w_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 1;
    const int32_t stride_w = 2;
    const int32_t padding = false;
    const int32_t dilation_w = 1;
    const int32_t channels_first = true;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col1dTest, dilation_w_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t dilation_w = 2;
    const int32_t channels_first = true;
    const int32_t cols_channels_first = false;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col1dTest, normal_cols_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t dilation_w = 1;
    const int32_t channels_first = false;
    const int32_t cols_channels_first = true;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col1dTest, kernel_w_cols_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 2;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t dilation_w = 1;
    const int32_t channels_first = false;
    const int32_t cols_channels_first = true;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col1dTest, stride_w_cols_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 1;
    const int32_t stride_w = 2;
    const int32_t padding = false;
    const int32_t dilation_w = 1;
    const int32_t channels_first = false;
    const int32_t cols_channels_first = true;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
TYPED_TEST(Im2col1dTest, dilation_w_cols_channels_first) {
    TypeParam typeValue = 0;
    const int32_t dtype = this->test_get_dtype(typeValue);

    const int32_t batches = 2;
    const int32_t im_w = 8;
    const int32_t channels = 3;
    const int32_t kernel_w = 1;
    const int32_t stride_w = 1;
    const int32_t padding = false;
    const int32_t dilation_w = 2;
    const int32_t channels_first = false;
    const int32_t cols_channels_first = true;

    this->test_exec(
        dtype,
        batches,
        im_w,
        channels,
        kernel_w,
        stride_w,
        padding,
        channels_first,
        dilation_w,
        cols_channels_first
    );
}
}