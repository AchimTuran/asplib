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

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "-DTARGET_LINUX")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(CMAKE_CXX_FLAGS "-DTARGET_WINDOWS")
endif()

if (NOT WIN32)
  add_definitions(-fPIC -g -O2)
endif()

include_directories(${PROJECT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/Biquads)

set(SOURCES Biquads/Biquad_Native/asplib_Biquad_Native.cpp
            Biquads/apslib_BiquadFactory.cpp)

set(HEADERS interfaces/asplib_IBaseBiquad.h
            asplib_utils/constants_typedefs/asplib_constants.h
            asplib_utils/constants_typedefs/asplib_typedefs.h
            asplib_utils/os/asplib_base_os.h
            asplib_utils/os/windows/windows_definitions.h
            asplib_utils/os/linux/linux_definitions.h
            Biquads/apslib_BiquadFactory.h
            Biquads/Biquad_Native/asplib_Biquad_Native.h)

add_library(asplib ${SOURCES} ${PLAT_SOURCES})
target_link_libraries(asplib ${asplib_LIBRARIES})

install(TARGETS asplib DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/interfaces/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/asplib/interfaces
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/asplib_utils/constants_typedefs/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/asplib/asplib_utils/constants_typedefs
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/asplib_utils/os/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/asplib/asplib_utils/os/
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/asplib_utils/os/windows/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/asplib/asplib_utils/os/windows/ 
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/asplib_utils/os/linux/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/asplib/asplib_utils/os/linux/
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/asplib_utils/computeDeviceInfo/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/asplib/asplib_utils/computeDeviceInfo/
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/Biquads/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/asplib/Biquads/ 
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/Biquads/Biquad_Native/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/asplib/Biquads/Biquad_Native/
        FILES_MATCHING PATTERN "*.h")
