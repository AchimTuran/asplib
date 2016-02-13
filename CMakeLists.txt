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

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

foreach(path ${CMAKE_PREFIX_PATH})
  message(STATUS "path=${path}")
endforeach()

if(NOT ASPLIB_MODULES_TO_BUILD)
  set(ASPLIB_MODULES_TO_BUILD "all")
endif()


project(asplib)

cmake_minimum_required(VERSION 2.6)
enable_language(CXX)

include(cmake/UseMultiArch.cmake)
include(cmake/asplib_helpers.cmake)

if(NOT ${ASPLIB_BUILD_COUNT})
  set(ASPLIB_BUILD_COUNT 0)
endif()

set(asplib_NAME asplib)
set(asplib_DESCRIPTION "asplib (Achim's Signal Processing LIBrary), is a small and lightweight C++ library for digital signal processing.")
set(asplib_VERSION_MAJOR 0)
set(asplib_VERSION_MINOR 2)
set(asplib_VERSION_PATCH ${ASPLIB_BUILD_COUNT})

if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "-DTARGET_LINUX")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(CMAKE_CXX_FLAGS "-DTARGET_WINDOWS")
  # Enable the orginization of projects with folders
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

if (NOT WIN32)
  add_definitions(-fPIC -g -O2)
endif()

add_subdirectory(asplib_modules)

#install(TARGETS asplib DESTINATION ${CMAKE_INSTALL_LIBDIR})

asplib_install_with_folder("${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
        
IF(NOT WIN32)
  # Pkgconfig
  include(cmake/PkgConfigHandler.cmake)
  configure_pc_file(asplib cmake/asplib.pc.in
                           asplib.pc
                           ${CMAKE_INSTALL_PREFIX}
                           ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
                           ${CMAKE_INSTALL_PREFIX}/include)

  install(FILES ${CMAKE_BINARY_DIR}/asplib.pc
          DESTINATION ${CMAKE_INSTALL_LIBDIR_NOARCH}/pkgconfig)
ENDIF(NOT WIN32)

# config mode
configure_file(cmake/asplib-config.cmake.in
               asplib-config.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/asplib-config.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR_NOARCH}/asplib)
