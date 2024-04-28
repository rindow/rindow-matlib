cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Threads REQUIRED)

set(RINDOW_MATLIB_VERSION ${PROJECT_VERSION})
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/rindow_matlib_config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/rindow_matlib_config.h"
)

file(GLOB_RECURSE srcfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ./*.c)

add_library(rindowmatlib SHARED ${srcfiles})
target_compile_options(rindowmatlib PRIVATE -DRINDOW_COMPILING_DLL)
if (MSVC)
    target_compile_options(rindowmatlib PRIVATE /openmp:experimental)
elseif(APPLE)
    find_package(OpenMP REQUIRED)
    if(OpenMP_FOUND)
        target_link_libraries(rindowmatlib PRIVATE OpenMP::OpenMP_C)
    endif()
elseif(UNIX)
    find_package(OpenMP REQUIRED)
    if(OpenMP_FOUND)
        target_link_libraries(rindowmatlib PRIVATE OpenMP::OpenMP_C)
    endif()
endif()

target_include_directories(rindowmatlib PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(rindowmatlib PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")


if (MSVC)
    install(
        TARGETS rindowmatlib
        RUNTIME DESTINATION bin
        COMPONENT ${PROJECT_NAME}
    )
    install(
        TARGETS rindowmatlib
        ARCHIVE DESTINATION lib
        COMPONENT ${PROJECT_NAME}
    )
elseif(UNIX)
    install(
        TARGETS rindowmatlib
        DESTINATION lib/rindowmatlib-openmp
        COMPONENT ${PROJECT_NAME}
    )
endif()

add_subdirectory(./seq)
