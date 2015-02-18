project(asplib)

cmake_minimum_required(VERSION 2.6)
enable_language(CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

include(UseMultiArch.cmake)

set(asplib_NAME asplib)
set(asplib_DESCRIPTION "asplib (Achim's Signal Processing LIBrary), is a small and lightweight C++ library for digital signal processing.")
set(asplib_VERSION_MAJOR ${APP_VERSION_MAJOR})
set(asplib_VERSION_MINOR ${APP_VERSION_MINOR})
set(asplib_VERSION_PATCH 0)

if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

#set(asplib_INCLUDE_DIRS ${TINYXML_INCLUDE_DIR} "${CMAKE_INSTALL_PREFIX}/include/kodi")
#set(asplib_LIBRARIES ${CMAKE_THREAD_LIBS_INIT} ${TINYXML_LIBRARIES})

set(SOURCES utils/computeDeviceInfo/cpuInfo.cpp)
#include_directories(${TINYXML_INCLUDE_DIR} ${KODI_INCLUDE_DIR})

add_library(asplib ${SOURCES} ${PLAT_SOURCES})
target_link_libraries(asplib ${asplib_LIBRARIES})
#set_target_properties(asplib PROPERTIES VERSION ${asplib_VERSION_MAJOR}.${asplib_VERSION_MINOR}.${asplib_VERSION_PATCH}
#                                              SOVERSION ${asplib_VERSION_MAJOR}.0)

install(TARGETS asplib DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES utils/computeDeviceInfo/cpuInfo_types.h
              utils/computeDeviceInfo/cpuInfo.h
        DESTINATION include/asplib/util)

#IF(NOT WIN32)
  # Pkgconfig
  #include(PkgConfigHandler.cmake)
  #configure_pc_file(kodiutilities kodiutilities.pc.in
  #                              kodiutilities.pc
  #                               ${CMAKE_INSTALL_PREFIX}
  #                               ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
  #                               ${CMAKE_INSTALL_PREFIX}/include)

#  install(FILES ${CMAKE_BINARY_DIR}/kodiutilities.pc
#          DESTINATION ${CMAKE_INSTALL_LIBDIR_NOARCH}/pkgconfig)
#ENDIF(NOT WIN32)

# config mode
#configure_file (kodiutilities-config.cmake.in
#                kodiutilities-config.cmake @ONLY)
#install(FILES ${CMAKE_BINARY_DIR}/kodiutilities-config.cmake
#       DESTINATION ${CMAKE_INSTALL_LIBDIR_NOARCH}/kodiutilities)

# Quell warnings with in-tree builds
#set(KODI_BUILD_DIR ${KODI_BUILD_DIR})
