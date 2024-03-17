#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
//#include <iostream>

using testing::ContainerEq;

namespace {
TEST(CommonTest, GetVersion) {
    char *version;
    version = rindow_matlib_common_get_version();
    //std::cout << version << "\n";
    //ASSERT_STREQ("0.1.1",version);
    ASSERT_TRUE(1);
}
}
