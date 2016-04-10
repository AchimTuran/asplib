cmake_minimum_required(VERSION 3.0)
project(asplib VERSION 0.2)

# TODO solve 'Enable C++ exceptions' to 'NO' and I got warning: warning C4530: C++ exception handler used, 
#             but unwind semantics are not enabled. Specify /EHsc'
# Solution: http://stackoverflow.com/questions/552695/how-can-i-switch-off-exception-handling-in-msvc

# make sure CMAKE_PREFIX_PATH is set
if(NOT CMAKE_PREFIX_PATH)
  set(CMAKE_PREFIX_PATH "E:/Kodi/adsp_addons/asplib_depends")
  list(APPEND CMAKE_PREFIX_PATH "E:/Kodi/adsp_addons/asplib_depends/include")
  list(APPEND CMAKE_PREFIX_PATH "E:/Kodi/adsp_addons/asplib_depends/lib")
else()
  file(TO_CMAKE_PATH "${CMAKE_PREFIX_PATH}" CMAKE_PREFIX_PATH)
  list(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}/depends")
  list(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}/depends/include")
  list(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}/depends/lib")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
message(STATUS "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")

foreach(path ${CMAKE_PREFIX_PATH})
  message(STATUS "path=${path}")
endforeach()

if(NOT ASPLIB_MODULES_TO_BUILD)
  set(ASPLIB_MODULES_TO_BUILD all)
endif()

include(cmake/UseMultiArch.cmake)
include(cmake/asplib_helpers.cmake)

# project defines vars such as PROJECT_NAME, PROJECT_VERSION, ...
if(NOT ${ASPLIB_BUILD_COUNT})
  set(ASPLIB_BUILD_COUNT 0)
endif()
set(asplib_VERSION_PATCH ${ASPLIB_BUILD_COUNT})

set(asplib_DESCRIPTION "asplib (Achim's Signal Processing LIBrary), is a small and lightweight C++ library for digital signal processing.")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
  add_definitions(-DTARGET_LINUX)
elseif(CMAKE_SYSTEM_NAME MATCHES Windows)
  add_definitions(-DTARGET_WINDOWS)

  # Enable the orginization of projects with folders
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

add_subdirectory(asplib_modules)
        
if(NOT WIN32)
  # Pkgconfig
  include(cmake/PkgConfigHandler.cmake)
  configure_pc_file(asplib cmake/asplib.pc.in
                           asplib.pc
                           ${CMAKE_INSTALL_PREFIX}
                           ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
                           ${CMAKE_INSTALL_PREFIX}/include)

  install(FILES ${CMAKE_BINARY_DIR}/asplib.pc
          DESTINATION ${CMAKE_INSTALL_LIBDIR_NOARCH}/pkgconfig)
endif()

# config mode
#configure_file(cmake/asplib-config.cmake.in
#               asplib-config.cmake @ONLY)
#install(FILES ${CMAKE_BINARY_DIR}/asplib-config.cmake
#        DESTINATION ${CMAKE_INSTALL_LIBDIR_NOARCH}/asplib)

# Generate and install a CMake config, version and target file
include(CMakePackageConfigHelpers)
set(CMAKE_INSTALL_CMAKECONFIGDIR
    ${CMAKE_INSTALL_LIBDIR_NOARCH}/cmake/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

write_basic_package_version_file(${PROJECT_NAME}-config-version.cmake COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR})

configure_package_config_file(cmake/${PROJECT_NAME}-config.cmake.in ${PROJECT_NAME}-config.cmake
                              INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR})
                              #PATH_VARS INCLUDE_INSTALL_DIR)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR})

install(EXPORT ${PROJECT_NAME} FILE ${PROJECT_NAME}-targets.cmake NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR})

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

# Print project configuration
message(STATUS)
message(STATUS "${CMAKE_PROJECT_NAME} - Configuration:")
message(STATUS "CMAKE_BUILD_TYPE:     ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS)
