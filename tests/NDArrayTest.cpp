#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "ndarray.h"
#include <stdbool.h>

using testing::ContainerEq;
using rindow::math::NDArray;
using rindow::math::ndarray_t;

namespace {

template <typename T>
class NDArrayTest : public ::testing::Test {
protected:
};
typedef ::testing::Types<float, int32_t> TestTypes;
TYPED_TEST_SUITE(NDArrayTest, TestTypes);

TYPED_TEST(NDArrayTest, Information) {
    ndarray_t<TypeParam> a = NDArray<TypeParam>::alloc({2,3});
    EXPECT_EQ(2, a->ndim());
    EXPECT_EQ(2, a->shape().size());
    EXPECT_EQ(2, a->shape()[0]);
    EXPECT_EQ(3, a->shape()[1]);
    EXPECT_EQ(0, a->offset());
    EXPECT_EQ(3, a->at(1)->offset());
    EXPECT_EQ(2, a->size());
    EXPECT_EQ(3, a->at(0)->size());
    EXPECT_EQ(6, a->num_items());
    EXPECT_EQ(3, a->at(0)->num_items());

    // alloc with signed int32
    const int32_t M = 2;
    const int32_t N = 3;
    ndarray_t<TypeParam> a2 = NDArray<TypeParam>::alloc({M,N});
    EXPECT_EQ(2, a2->ndim());

    // alloc with unsigned int32
    const uint32_t M3 = 2;
    const uint32_t N3 = 3;
    ndarray_t<TypeParam> a3 = NDArray<TypeParam>::alloc({M3,N3});
    EXPECT_EQ(2, a3->ndim());

}
TYPED_TEST(NDArrayTest, setvalues) {
    ndarray_t<TypeParam> a = NDArray<TypeParam>::alloc({2,3});
    a->at(0)->at(0)->scalar() = 1;
    a->at(0)->at(1)->scalar() = 2;
    a->at(0)->at(2)->scalar() = 3;
    a->at(1)->at(0)->scalar() = 4;
    a->at(1)->at(1)->scalar() = 5;
    a->at(1)->at(2)->scalar() = 6;
    TypeParam X1[6];
    std::copy_n(a->buffer()->begin(), 6, X1);
    TypeParam R1[6] = {1,2,3,4,5,6};
    EXPECT_THAT(R1, ContainerEq(X1));

    a->at({0,0}) = 6;
    a->at({0,1}) = 5;
    a->at({0,2}) = 4;
    a->at({1,0}) = 3;
    a->at({1,1}) = 2;
    a->at({1,2}) = 1;
    TypeParam X2[6];
    std::copy_n(a->buffer()->begin(), 6, X2);
    TypeParam R2[6] = {6,5,4,3,2,1};
    EXPECT_THAT(R2, ContainerEq(X2));
}
TYPED_TEST(NDArrayTest, range) {
    // limit
    ndarray_t<TypeParam> a = NDArray<TypeParam>::range(6);
    TypeParam X[6];
    std::copy_n(a->buffer()->begin(), 6, X);
    TypeParam R1[6] = {0,1,2,3,4,5};
    EXPECT_THAT(R1, ContainerEq(X));
    // start, limit
    ndarray_t<TypeParam> a2 = NDArray<TypeParam>::range(1,7);
    TypeParam X2[6];
    std::copy_n(a2->buffer()->begin(), 6, X2);
    TypeParam R2[6] = {1,2,3,4,5,6};
    EXPECT_THAT(R2, ContainerEq(X2));
    // start, limit, delta
    ndarray_t<TypeParam> a3 = NDArray<TypeParam>::range(6,0,-1);
    TypeParam X3[6];
    std::copy_n(a3->buffer()->begin(), 6, X3);
    TypeParam R3[6] = {6,5,4,3,2,1};
    EXPECT_THAT(R3, ContainerEq(X3));
}
TYPED_TEST(NDArrayTest, reshapesimple) {
    ndarray_t<TypeParam> a = NDArray<TypeParam>::range(1,7);
    EXPECT_EQ(1, a->ndim());
    EXPECT_EQ(6, a->shape()[0]);

    ndarray_t<TypeParam> a1 = a->reshape({2,3});
    EXPECT_EQ(2, a1->ndim());
    EXPECT_EQ(2, a1->shape()[0]);
    EXPECT_EQ(3, a1->shape()[1]);
    
    EXPECT_EQ(1, a1->at({0,0}));
    EXPECT_EQ(2, a1->at({0,1}));
    EXPECT_EQ(3, a1->at({0,2}));
    EXPECT_EQ(4, a1->at({1,0}));
    EXPECT_EQ(5, a1->at({1,1}));
    EXPECT_EQ(6, a1->at({1,2}));

    ndarray_t<TypeParam> a2 = a1->reshape({3,2});
    EXPECT_EQ(2, a1->ndim());
    EXPECT_EQ(3, a2->shape()[0]);
    EXPECT_EQ(2, a2->shape()[1]);
    
    EXPECT_EQ(1, a2->at({0,0}));
    EXPECT_EQ(2, a2->at({0,1}));
    EXPECT_EQ(3, a2->at({1,0}));
    EXPECT_EQ(4, a2->at({1,1}));
    EXPECT_EQ(5, a2->at({2,0}));
    EXPECT_EQ(6, a2->at({2,1}));

    ndarray_t<TypeParam> a3 = a2->reshape({6});
    EXPECT_EQ(1, a3->ndim());
    EXPECT_EQ(6, a3->shape()[0]);
    
    EXPECT_EQ(1, a3->at({0}));
    EXPECT_EQ(2, a3->at({1}));
    EXPECT_EQ(3, a3->at({2}));
    EXPECT_EQ(4, a3->at({3}));
    EXPECT_EQ(5, a3->at({4}));
    EXPECT_EQ(6, a3->at({5}));
}
TYPED_TEST(NDArrayTest, bufferOffset6) {
    ndarray_t<TypeParam> a = NDArray<TypeParam>::range(1,7)->reshape({2,3});
    EXPECT_EQ(2, a->ndim());
    EXPECT_EQ(2, a->shape()[0]);
    EXPECT_EQ(3, a->shape()[1]);
    EXPECT_EQ(2, a->size());
    EXPECT_EQ(6, a->num_items());
    EXPECT_EQ(0, a->offset());

    ndarray_t<TypeParam> a1 = a->at(1);
    EXPECT_EQ(1, a1->ndim());
    EXPECT_EQ(3, a1->shape()[0]);
    EXPECT_EQ(3, a1->size());
    EXPECT_EQ(3, a1->num_items());
    EXPECT_EQ(3, a1->offset());
    EXPECT_EQ(4, a1->at({0}));
    EXPECT_EQ(5, a1->at({1}));
    EXPECT_EQ(6, a1->at({2}));

    ndarray_t<TypeParam> a2 = a->reshape({3,2})->at(2);
    EXPECT_EQ(1, a2->ndim());
    EXPECT_EQ(2, a2->shape()[0]);
    EXPECT_EQ(2, a2->size());
    EXPECT_EQ(2, a2->num_items());
    EXPECT_EQ(4, a2->offset());
    EXPECT_EQ(5, a2->at({0}));
    EXPECT_EQ(6, a2->at({1}));

}
TYPED_TEST(NDArrayTest, bufferOffset24) {
    ndarray_t<TypeParam> a = NDArray<TypeParam>::range(1,25)->reshape({2,3,4});
    EXPECT_EQ(3, a->ndim());
    EXPECT_EQ(2, a->shape()[0]);
    EXPECT_EQ(3, a->shape()[1]);
    EXPECT_EQ(4, a->shape()[2]);
    EXPECT_EQ(2, a->size());
    EXPECT_EQ(24,a->num_items());
    EXPECT_EQ(0, a->offset());

    ndarray_t<TypeParam> a1 = a->at(1);
    EXPECT_EQ(2, a1->ndim());
    EXPECT_EQ(3, a1->shape()[0]);
    EXPECT_EQ(4, a1->shape()[1]);
    EXPECT_EQ(3, a1->size());
    EXPECT_EQ(12,a1->num_items());
    EXPECT_EQ(12,a1->offset());
    EXPECT_EQ(13,a1->at({0,0}));
    EXPECT_EQ(16,a1->at({0,3}));
    EXPECT_EQ(21,a1->at({2,0}));
    EXPECT_EQ(24,a1->at({2,3}));
    EXPECT_EQ(24,a->at({1,2,3}));

    ndarray_t<TypeParam> a2 = a1->at(2);
    EXPECT_EQ(1, a2->ndim());
    EXPECT_EQ(4, a2->shape()[0]);
    EXPECT_EQ(4, a2->size());
    EXPECT_EQ(4, a2->num_items());
    EXPECT_EQ(20,a2->offset());
    EXPECT_EQ(21,a2->at({0}));
    EXPECT_EQ(24,a2->at({3}));
    EXPECT_EQ(24,a->at({1,2,3}));

    ndarray_t<TypeParam> a3 = a->reshape({4,3,2})->at(3);
    EXPECT_EQ(2, a3->ndim());
    EXPECT_EQ(3, a3->shape()[0]);
    EXPECT_EQ(2, a3->shape()[1]);
    EXPECT_EQ(3, a3->size());
    EXPECT_EQ(6, a3->num_items());
    EXPECT_EQ(18,a3->offset());
    EXPECT_EQ(19,a3->at({0,0}));
    EXPECT_EQ(24,a3->at({2,1}));
    EXPECT_EQ(24,a->reshape({4,3,2})->at({3,2,1}));
}
TYPED_TEST(NDArrayTest, array) {
    // limit
    ndarray_t<TypeParam> a = NDArray<TypeParam>::array({1,2,3,4,5,6});
    TypeParam X[6];
    std::copy_n(a->buffer()->begin(), 6, X);
    TypeParam R1[6] = {1,2,3,4,5,6};
    EXPECT_THAT(R1, ContainerEq(X));
}
TYPED_TEST(NDArrayTest, fill) {
    // limit
    ndarray_t<TypeParam> a = NDArray<TypeParam>::fill({2,3},123);
    EXPECT_EQ(2, a->ndim());
    EXPECT_EQ(2, a->shape()[0]);
    EXPECT_EQ(3, a->shape()[1]);
    TypeParam X[6];
    std::copy_n(a->buffer()->begin(), 6, X);
    TypeParam R1[6] = {123,123,123,123,123,123};
    EXPECT_THAT(R1, ContainerEq(X));

    // alloc with signed int32
    const int32_t M = 2;
    const int32_t N = 3;
    ndarray_t<TypeParam> a2 = NDArray<TypeParam>::fill({M,N},123);
    EXPECT_EQ(2, a2->ndim());

    // alloc with unsigned int32
    const uint32_t M3 = 2;
    const uint32_t N3 = 3;
    ndarray_t<TypeParam> a3 = NDArray<TypeParam>::fill({M3,N3},123);
    EXPECT_EQ(2, a3->ndim());
}
TYPED_TEST(NDArrayTest, ones) {
    // limit
    ndarray_t<TypeParam> a = NDArray<TypeParam>::ones({2,3});
    EXPECT_EQ(2, a->ndim());
    EXPECT_EQ(2, a->shape()[0]);
    EXPECT_EQ(3, a->shape()[1]);
    TypeParam X[6];
    std::copy_n(a->buffer()->begin(), 6, X);
    TypeParam R1[6] = {1,1,1,1,1,1};
    EXPECT_THAT(R1, ContainerEq(X));

    // alloc with signed int32
    const int32_t M = 2;
    const int32_t N = 3;
    ndarray_t<TypeParam> a2 = NDArray<TypeParam>::ones({M,N});
    EXPECT_EQ(2, a2->ndim());

    // alloc with unsigned int32
    const uint32_t M3 = 2;
    const uint32_t N3 = 3;
    ndarray_t<TypeParam> a3 = NDArray<TypeParam>::ones({M3,N3});
    EXPECT_EQ(2, a3->ndim());
}
TYPED_TEST(NDArrayTest, zeros) {
    // limit
    ndarray_t<TypeParam> a = NDArray<TypeParam>::zeros({2,3});
    EXPECT_EQ(2, a->ndim());
    EXPECT_EQ(2, a->shape()[0]);
    EXPECT_EQ(3, a->shape()[1]);
    TypeParam X[6];
    std::copy_n(a->buffer()->begin(), 6, X);
    TypeParam R1[6] = {0,0,0,0,0,0};
    EXPECT_THAT(R1, ContainerEq(X));

    // alloc with signed int32
    const int32_t M = 2;
    const int32_t N = 3;
    ndarray_t<TypeParam> a2 = NDArray<TypeParam>::zeros({M,N});
    EXPECT_EQ(2, a2->ndim());

    // alloc with unsigned int32
    const uint32_t M3 = 2;
    const uint32_t N3 = 3;
    ndarray_t<TypeParam> a3 = NDArray<TypeParam>::zeros({M3,N3});
    EXPECT_EQ(2, a3->ndim());
}
TYPED_TEST(NDArrayTest, RawDatas) {
    ndarray_t<TypeParam> a = NDArray<TypeParam>::range(1,7)->reshape({2,3});
    EXPECT_EQ(0, a->offset());
    EXPECT_EQ(6, a->num_items());

    auto buffer = a->buffer();  // shared_ptr<std::vector>
    TypeParam X1[6];
    std::copy_n(buffer->begin(), 6, X1);
    TypeParam R1[6] = {1,2,3,4,5,6};
    EXPECT_THAT(R1, ContainerEq(X1));

    TypeParam* data = a->data();
    TypeParam X2[6];
    std::copy_n(data, 6, X2);
    TypeParam R2[6] = {1,2,3,4,5,6};
    EXPECT_THAT(R2, ContainerEq(X2));

    // with offset
    ndarray_t<TypeParam> a1 = a->at(1);
    EXPECT_EQ(3, a1->offset());
    EXPECT_EQ(3, a1->num_items());

    auto buffer1 = a1->buffer();  // shared_ptr<std::vector>
    TypeParam X3[6];
    std::copy_n(buffer1->begin(), 6, X3);
    TypeParam R3[6] = {1,2,3,4,5,6};
    EXPECT_THAT(R3, ContainerEq(X3));

    TypeParam* data1 = a1->data();
    TypeParam X4[3];
    std::copy_n(data1, 3, X4);
    TypeParam R4[3] = {4,5,6};
    EXPECT_THAT(R4, ContainerEq(X4));
}
TYPED_TEST(NDArrayTest, is_scalar) {
    ndarray_t<TypeParam> a = NDArray<TypeParam>::range(1,7)->reshape({2,3});

    EXPECT_EQ(false, a->is_scalar());
    EXPECT_EQ(false, a->at(0)->is_scalar());
    EXPECT_EQ(false, a->at(1)->is_scalar());
    EXPECT_EQ(true, a->at(0)->at(0)->is_scalar());
    EXPECT_EQ(true, a->at(1)->at(2)->is_scalar());
}
TYPED_TEST(NDArrayTest, square_brackets) {
    ndarray_t<TypeParam> a = NDArray<TypeParam>::range(1,7)->reshape({2,3});

    EXPECT_EQ(false, ((*a)[0])->is_scalar());
    EXPECT_EQ(false, ((*a)[1])->is_scalar());
    EXPECT_EQ(true, ((*((*a)[0]))[0])->is_scalar());
    EXPECT_EQ(true, ((*((*a)[1]))[2])->is_scalar());

    EXPECT_EQ(1, ((*a)[{0,0}]));
    EXPECT_EQ(2, ((*a)[{0,1}]));
    EXPECT_EQ(3, ((*a)[{0,2}]));
    EXPECT_EQ(4, ((*a)[{1,0}]));
    EXPECT_EQ(5, ((*a)[{1,1}]));
    EXPECT_EQ(6, ((*a)[{1,2}]));
}
TYPED_TEST(NDArrayTest, for_in) {
    ndarray_t<TypeParam> a = NDArray<TypeParam>::range(1,7)->reshape({2,3});

    TypeParam X[2][3];
    int i=0;
    for(auto& v: *a) {
        int j=0;
        for(auto& vv: *v) {
            X[i][j] = vv->scalar();
            ++j;
        }
        ++i;
    }
    TypeParam R[2][3] = {{1,2,3},{4,5,6}};
    EXPECT_THAT(R, ContainerEq(X));
}
TYPED_TEST(NDArrayTest, for_begin) {
    ndarray_t<TypeParam> a = NDArray<TypeParam>::range(1,7)->reshape({2,3});

    TypeParam X[2][3];
    int i=0;
    for(auto ii=a->begin();ii!=a->end();++ii) {
        int j=0;
        auto v = a->at(ii);
        for(auto jj=v->begin();jj!=v->end();++jj) {
            auto vv = v->at(jj);
            X[i][j] = vv->scalar();
            ++j;
        }
        ++i;
    }
    TypeParam R[2][3] = {{1,2,3},{4,5,6}};
    EXPECT_THAT(R, ContainerEq(X));
}
TYPED_TEST(NDArrayTest, for_vbegin) {
    ndarray_t<TypeParam> a = NDArray<TypeParam>::range(1,7)->reshape({2,3});

    TypeParam X[6];
    int i=0;
    for(auto ii=a->vbegin();ii!=a->vend();++ii) {
        X[i] = *ii;
        ++i;
    }
    TypeParam R[6] = {1,2,3,4,5,6};
    EXPECT_THAT(R, ContainerEq(X));

    // with offset
    ndarray_t<TypeParam> a2 = a->at(1);
    TypeParam X2[3];
    int i2=0;
    for(auto ii2=a2->vbegin();ii2!=a2->vend();++ii2) {
        X2[i2] = *ii2;
        ++i2;
    }
    TypeParam R2[3] = {4,5,6};
    EXPECT_THAT(R2, ContainerEq(X2));

}
TYPED_TEST(NDArrayTest, accumulate) {
    ndarray_t<TypeParam> a = NDArray<TypeParam>::range(1,7)->reshape({2,3});

    TypeParam sum = std::accumulate(a->vbegin(),a->vend(),(TypeParam)0);
    EXPECT_EQ(21, sum);
}
TYPED_TEST(NDArrayTest, for_each) {
    ndarray_t<TypeParam> a = NDArray<TypeParam>::range(1,7)->reshape({2,3});

    // get from array
    TypeParam sum = 0;
    std::for_each(a->vbegin(),a->vend(),[&sum] (TypeParam v) mutable {
        sum += v;
    });
    EXPECT_EQ(21, sum);

    // put into array
    TypeParam plus = 10;
    std::for_each(a->vbegin(),a->vend(),[plus] (TypeParam& v) mutable {
        v += plus;
    });

    TypeParam X[2][3];
    std::copy_n(a->buffer()->begin(), 6, (TypeParam *)X);
    TypeParam R[2][3] = {{11,12,13},{14,15,16}};
    EXPECT_THAT(R, ContainerEq(X));
}
}
