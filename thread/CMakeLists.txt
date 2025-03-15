cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/thread)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/thread)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/thread)
if(APPLE)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/opt/local/share/cmake")
endif()

find_package(Threads REQUIRED)

set(RINDOW_MATLIB_VERSION ${PROJECT_VERSION})
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/rindow_matlib_config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/rindow_matlib_config.h"
)

file(GLOB_RECURSE srcfiles.thread RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ./*.c ./*.cpp)

add_library(rindowmatlib.thread SHARED ${srcfiles.thread})
set_target_properties(rindowmatlib.thread PROPERTIES OUTPUT_NAME rindowmatlib)
target_compile_options(rindowmatlib.thread PRIVATE -DRINDOW_COMPILING_DLL)
if (MSVC)
    target_compile_options(rindowmatlib.thread PRIVATE /openmp:experimental)
elseif(APPLE)
    find_package(OpenMP REQUIRED)
    if(OpenMP_FOUND)
        target_link_libraries(rindowmatlib.thread PRIVATE OpenMP::OpenMP_C)
    endif()
elseif(UNIX)
    find_package(OpenMP REQUIRED)
    if(OpenMP_FOUND)
        target_link_libraries(rindowmatlib.thread PRIVATE OpenMP::OpenMP_C)
    endif()
endif()

target_include_directories(rindowmatlib.thread PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(rindowmatlib.thread PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

if (MSVC)
    install(
        TARGETS rindowmatlib.thread
        #RUNTIME DESTINATION bin/thread
        RUNTIME DESTINATION bin
        COMPONENT ${PROJECT_NAME}
    )
    install(
        TARGETS rindowmatlib.thread
        ARCHIVE DESTINATION lib/import/thread
        COMPONENT ${PROJECT_NAME}
    )
elseif(APPLE)
    install(
        TARGETS rindowmatlib.thread
        #DESTINATION lib/thread
        DESTINATION lib
        RENAME rindowmatlib.dylib
        COMPONENT ${PROJECT_NAME}
    )
elseif(UNIX)
    install(
        TARGETS rindowmatlib.thread
        DESTINATION lib/rindowmatlib-thread
        RENAME rindowmatlib.so
        COMPONENT ${PROJECT_NAME}
    )
endif()
