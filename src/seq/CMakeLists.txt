set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/seq)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/seq)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/seq)

file(GLOB_RECURSE srcfiles.seq RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../*.c ../*.cpp)

add_library(rindowmatlib.seq SHARED ${srcfiles.seq})
set_target_properties(rindowmatlib.seq PROPERTIES OUTPUT_NAME rindowmatlib)
target_compile_options(rindowmatlib.seq PRIVATE -DRINDOW_COMPILING_DLL)
target_include_directories(rindowmatlib.seq PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(rindowmatlib.seq PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/..")

if(UNIX AND NOT APPLE)
endif()
if (MSVC)
    install(
        TARGETS rindowmatlib.seq
        RUNTIME DESTINATION bin/serial
        COMPONENT ${PROJECT_NAME}
    )
    install(
        TARGETS rindowmatlib.seq
        ARCHIVE DESTINATION lib/import/serial
        COMPONENT ${PROJECT_NAME}
    )
elseif(APPLE)
    install(
        TARGETS rindowmatlib.seq
        DESTINATION lib/serial
        RENAME rindowmatlib.dylib
        COMPONENT ${PROJECT_NAME}
    )
elseif(UNIX)
    install(
        TARGETS rindowmatlib.seq
        DESTINATION lib/rindowmatlib-serial
        RENAME rindowmatlib.so
        COMPONENT ${PROJECT_NAME}
    )
endif()
