set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/seq)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/seq)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/seq)

file(GLOB_RECURSE srcfiles.seq RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../*.c ../*.cpp)

if(UNIX AND NOT APPLE)
    add_library(rindowmatlib.seq SHARED ${srcfiles.seq})
    set_target_properties(rindowmatlib.seq PROPERTIES OUTPUT_NAME rindowmatlib)
    target_compile_options(rindowmatlib.seq PRIVATE -DRINDOW_COMPILING_DLL)
    target_include_directories(rindowmatlib.seq PUBLIC "${PROJECT_SOURCE_DIR}/include")
    target_include_directories(rindowmatlib.seq PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/..")
endif()

if(UNIX AND NOT APPLE)
    install(
        TARGETS rindowmatlib.seq
        DESTINATION lib/rindowmatlib-serial
        RENAME rindowmatlib.so
        COMPONENT ${PROJECT_NAME}
    )
endif()
