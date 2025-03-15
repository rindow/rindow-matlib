set(CPACK_COMPONENTS_ALL ${PROJECT_NAME})
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Rindow Math Matrix library")
set(CPACK_PACKAGE_VENDOR "Rindow")

set(CPACK_VERBATIM_VARIABLES YES)

set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
SET(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_SOURCE_DIR}/packages")

# https://unix.stackexchange.com/a/11552/254512
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")#/${CMAKE_PROJECT_VERSION}")

#set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
#set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
#set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
#file(STRINGS "/etc/os-release" data_list REGEX "^(ID|VERSION_ID|VERSION_CODENAME)=")
#foreach(_var ${data_list})
#    if("${_var}" MATCHES "^(ID)=(.*)$")
#      set(_var_id "${CMAKE_MATCH_2}")
#    elseif("${_var}" MATCHES "^(VERSION_ID)=(.*)$")
#      set(_var_version_id "${CMAKE_MATCH_2}")
#    elseif("${_var}" MATCHES "^(VERSION_CODENAME)=(.*)$")
#      set(_var_codename "${CMAKE_MATCH_2}")
#    endif()
#endforeach()
#string(STRIP "${_var_id}" _var_id)
#string(STRIP "${_var_version_id}" _var_version_id)
#string(STRIP "${_var_codename}" _var_codename)
## Remove any enclosing quotation marks
#string(REGEX REPLACE "^\"(.*)\"$" "\\1" _var_id "${_var_id}")
#string(REGEX REPLACE "^\"(.*)\"$" "\\1" _var_version_id "${_var_version_id}")
#string(REGEX REPLACE "^\"(.*)\"$" "\\1" _var_codename "${_var_codename}")
#set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}-1+${_var_id}${_var_version_id}")

set(CPACK_PACKAGE_CONTACT "Rindow Developers <rindow@users.noreply.github.com>")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://rindow.github.io/mathematics/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/debian/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)#ONE_PER_GROUP)

if(APPLE)
elseif(UNIX)
  execute_process(
    COMMAND dpkg-query -W -f=\${Version} libc6
    OUTPUT_VARIABLE LIBC6_VERSION_FULL
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  string(REGEX MATCH "^([0-9]+\\.[0-9]+)" LIBC6_VERSION "${LIBC6_VERSION_FULL}")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= ${LIBC6_VERSION}), libstdc++6")
  set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${PROJECT_BINARY_DIR}/debian/postinst;${PROJECT_BINARY_DIR}/debian/prerm")
  set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
endif()

set(CPACK_DEB_COMPONENT_INSTALL YES)

if(MSVC)
  set(CPACK_GENERATOR "ZIP")
  set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
  set(CPACK_PACKAGING_INSTALL_PREFIX ".")
elseif(APPLE)
  set(CPACK_GENERATOR "TGZ")
  set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
elseif(UNIX)
  set(CPACK_GENERATOR "DEB")
endif()

include(CPack)

if(UNIX)
  set(CMAKE_INSTALL_PREFIX "${CPACK_PACKAGING_INSTALL_PREFIX}")
endif()
