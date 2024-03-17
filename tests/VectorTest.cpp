#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"

using testing::ContainerEq;

namespace {

template <typename T>
class VectorTest : public ::testing::Test {
protected:
    virtual std::vector<T> test_alloc(int32_t n)
    {
        return std::vector<T>(n);
    }

    virtual std::vector<T> test_allocOnes(int32_t n)
    {
        auto vector = std::vector<T>(n);
        std::fill(vector.begin(), vector.end(), (T)1);
        return vector;
    }

    virtual void test_zeros(std::vector<T>& vector)
    {
        std::fill(vector.begin(), vector.end(), (T)0);
    }

    virtual void test_ones(std::vector<T>& vector)
    {
        std::fill(vector.begin(), vector.end(), (T)1);
    }

    virtual void test_noref(std::vector<T> vector)
    {
        std::fill(vector.begin(), vector.end(), (T)1);
    }
};
typedef ::testing::Types<float, double, int32_t> TestTypes;
TYPED_TEST_SUITE(VectorTest, TestTypes);

TYPED_TEST(VectorTest, normal) {
    const int32_t N = 3;

    auto X0 = this->test_allocOnes(N);
    std::vector<TypeParam> R0 = {1, 1, 1};
    EXPECT_TRUE(std::equal(R0.cbegin(),R0.cend(),X0.cbegin()));

    auto X = this->test_alloc(N);
    std::vector<TypeParam> R1 = {0, 0, 0};
    EXPECT_TRUE(std::equal(R1.cbegin(),R1.cend(),X.cbegin()));

    this->test_ones(X);
    std::vector<TypeParam> R2 = {1, 1, 1};
    EXPECT_TRUE(std::equal(R2.cbegin(),R2.cend(),X.cbegin()));

    this->test_zeros(X);
    std::vector<TypeParam> R3 = {0, 0, 0};
    EXPECT_TRUE(std::equal(R3.cbegin(),R3.cend(),X.cbegin()));

    this->test_noref(X);
    std::vector<TypeParam> R4 = {0, 0, 0};
    EXPECT_TRUE(std::equal(R4.cbegin(),R4.cend(),X.cbegin()));

}
}
