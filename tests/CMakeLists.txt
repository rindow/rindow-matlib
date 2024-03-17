cmake_minimum_required(VERSION 3.14)

## If you want to build the Release version, type the following command
#set(CMAKE_CONFIGURATION_TYPES Debug Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE testfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ./*.cpp)

add_executable(test_main ${testfiles})
target_link_libraries(test_main GTest::gtest_main)
target_link_libraries(test_main rindowmatlib)
target_include_directories(test_main PUBLIC ${gmock_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(test_main)

find_package(Threads REQUIRED)

add_test(
  NAME main_test
  COMMAND test_main
  CONFIGURATIONS Release
)

set(INSTALL_TARGETS ${INSTALL_TARGETS} EXCLUDING gmock gmock_main gtest gtest_main)
