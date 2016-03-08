project(asplib)

cmake_minimum_required(VERSION 2.6)
enable_language(CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

include(cmake/UseMultiArch.cmake)
include(cmake/asplib_helpers.cmake)

set(asplib_NAME asplib)
set(asplib_DESCRIPTION "asplib (Achim's Signal Processing LIBrary), is a small and lightweight C++ library for digital signal processing.")
set(asplib_VERSION_MAJOR 0)
set(asplib_VERSION_MINOR 2)
set(asplib_VERSION_PATCH 0)

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

set(asplib_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
                        ${PROJECT_SOURCE_DIR}/Biquads
                        ${CMAKE_INSTALL_PREFIX}/include/asplib)

include_directories(${asplib_INCLUDE_DIRS})

set(SOURCES Biquads/Biquad_Native/asplib_Biquad_Native.cpp
            Biquads/apslib_BiquadFactory.cpp)
          
set(HEADERS interfaces/asplib_IBaseBiquad.h
            Biquads/apslib_BiquadFactory.h
            Biquads/Biquad_Native/asplib_Biquad_Native.h
            asplib_utils/constants_typedefs/asplib_constants.h
            asplib_utils/constants_typedefs/asplib_typedefs.h
            asplib_utils/os/asplib_base_os.h
            asplib_utils/os/windows/windows_definitions.h
            asplib_utils/os/linux/linux_definitions.h)
            
asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(asplib ${SOURCES} ${HEADERS})
target_link_libraries(asplib ${asplib_LIBRARIES})
set_target_properties(asplib PROPERTIES VERSION ${asplib_VERSION_MAJOR}.${asplib_VERSION_MINOR}.${asplib_VERSION_PATCH}
                                                SOVERSION ${asplib_VERSION_MAJOR}.0)


install(TARGETS asplib DESTINATION ${CMAKE_INSTALL_LIBDIR})

asplib_install_with_folder("${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
        
IF(NOT WIN32)
  # Pkgconfig
  include(cmake/PkgConfigHandler.cmake)
  configure_pc_file(asplib asplib.pc.in
                           asplib.pc
                           ${CMAKE_INSTALL_PREFIX}
                           ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
                           ${CMAKE_INSTALL_PREFIX}/include)

  install(FILES ${CMAKE_BINARY_DIR}/asplib.pc
          DESTINATION ${CMAKE_INSTALL_LIBDIR_NOARCH}/pkgconfig)
ENDIF(NOT WIN32)

# config mode
configure_file(asplib-config.cmake.in
               asplib-config.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/asplib-config.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR_NOARCH}/asplib)
