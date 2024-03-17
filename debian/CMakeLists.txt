add_custom_target(
    pkgconfig ALL
    DEPENDS rindowmatlib.pc
)
add_custom_command(
    OUTPUT rindowmatlib.pc
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/rindowmatlib.pc.in
    COMMAND sed -e s@%RINDOW_MATLIB_VERSION%@${PROJECT_VERSION}@ ${CMAKE_CURRENT_SOURCE_DIR}/rindowmatlib.pc.in | sed -e s@%INSTALL_DIR%@${CMAKE_INSTALL_PREFIX}@ > ${CMAKE_CURRENT_BINARY_DIR}/rindowmatlib.pc)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/rindowmatlib.pc
    DESTINATION lib/pkgconfig
    COMPONENT ${PROJECT_NAME}
)

add_custom_target(debianpostinst ALL DEPENDS postinst)
add_custom_command(
    OUTPUT postinst
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/postinst.in
    COMMAND sed -e s@%INSTALL_DIR%@${CMAKE_INSTALL_PREFIX}@ ${CMAKE_CURRENT_SOURCE_DIR}/postinst.in > ${CMAKE_CURRENT_BINARY_DIR}/postinst
    COMMAND chmod 555 ${CMAKE_CURRENT_BINARY_DIR}/postinst
)
add_custom_target(debianprerm ALL DEPENDS prerm)
add_custom_command(
    OUTPUT prerm
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/prerm.in
    COMMAND sed -e s@%INSTALL_DIR%@${CMAKE_INSTALL_PREFIX}@ ${CMAKE_CURRENT_SOURCE_DIR}/prerm.in > ${CMAKE_CURRENT_BINARY_DIR}/prerm
    COMMAND chmod 555 ${CMAKE_CURRENT_BINARY_DIR}/prerm
)
