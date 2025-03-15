cmake_minimum_required(VERSION 3.14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/thread)

file(GLOB_RECURSE testfilesthread RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ./*.cpp)

add_executable(test_thread ${testfilesthread})

target_link_libraries(test_thread GTest::gtest_main)
if(MSVC)
  target_link_libraries(test_thread rindowmatlib.thread)
else()
  target_link_libraries(test_thread rindowmatlib.thread)
endif()
target_include_directories(test_thread PUBLIC ${gmock_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(test_thread)

find_package(Threads REQUIRED)

add_test(
  NAME thread_test
  COMMAND test_thread
  CONFIGURATIONS Release
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/thread
)
