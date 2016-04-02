#include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# FIXME: Don't use globbing for source files
# https://cmake.org/cmake/help/latest/command/file.html?highlight=glob#file

file (GLOB_RECURSE ASPLIB_CORE_SOURCES
      "Core/*.c"
      "Core/*.cpp")

file (GLOB_RECURSE ASPLIB_CORE_HEADERS
      "Core/*.h"
      "Core/*.hpp")
      
#foreach(_file ${ASPLIB_CORE_SOURCES})
#message(STATUS "file=${_file}")
#endforeach()

asplib_source_group("${ASPLIB_CORE_SOURCES}")
asplib_source_group("${ASPLIB_CORE_HEADERS}")

add_library(asplib_Core STATIC ${ASPLIB_CORE_SOURCES} ${ASPLIB_CORE_HEADERS})
add_library(asplib::Core ALIAS asplib_Core)
target_include_directories(asplib_Core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set_property(TARGET asplib_Core PROPERTY FOLDER "modules")

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Core" CACHE INTERNAL "")

# install all specific module files
asplib_install_module("Core" "${ASPLIB_CORE_HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_Core DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
