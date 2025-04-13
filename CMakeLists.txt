cmake_minimum_required(VERSION 3.14)
#include (CMakePrintSystemInformation)
#set(CMAKE_CONFIGURATION_TYPES Debug Release)
#if(APPLE)
#  set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build architectures for macOS" FORCE)
#endif()
enable_testing()

project(rindow-matlib VERSION 1.1.1 LANGUAGES CXX C)
#set(PROJECT_VERSION_MAJOR 1)
#set(PROJECT_VERSION_MINOR 1)
#set(PROJECT_VERSION_PATCH 1)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(APPLE)
  set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
elseif(UNIX)
  execute_process(
    COMMAND lsb_release -sr
    OUTPUT_VARIABLE UBUNTU_RELEASE
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  set(CPACK_DEBIAN_PACKAGE_RELEASE "${UBUNTU_RELEASE}")
endif()
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Packing)

add_subdirectory(./src)
add_subdirectory(./include)

if(UNIX AND NOT APPLE)
    add_subdirectory(./debian)
endif()
if(MSVC)
  add_subdirectory(./vclib)
endif()

add_subdirectory(./thread)

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
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(./threadtest)
add_subdirectory(./tests)


message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CPACK_PACKAGING_INSTALL_PREFIX: ${CPACK_PACKAGING_INSTALL_PREFIX}")
message(STATUS "CPACK_COMPONENTS_ALL: ${CPACK_COMPONENTS_ALL}")
message(STATUS "CPACK_PACKAGE_VERSION: ${CPACK_PACKAGE_VERSION}")
