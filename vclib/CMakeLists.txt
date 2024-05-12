file(GLOB_RECURSE srcfiles.vclib RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c)

if(MSVC)
    add_library(rindowmatlib.vclib STATIC ${srcfiles.vclib})
    set_target_properties(rindowmatlib.vclib PROPERTIES OUTPUT_NAME rindowmatlib)
    target_compile_options(rindowmatlib.vclib PRIVATE /Zl)
    target_include_directories(rindowmatlib.vclib PUBLIC "${PROJECT_SOURCE_DIR}/include")
endif()

if(MSVC)
    install(
        TARGETS rindowmatlib.vclib
        ARCHIVE DESTINATION lib
        RENAME rindowmatlib.lib
        COMPONENT ${PROJECT_NAME}
    )
endif()
