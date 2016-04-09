#include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# FIXME: Don't use globbing for source files
# https://cmake.org/cmake/help/latest/command/file.html?highlight=glob#file

file (GLOB_RECURSE SOURCES
      "Core/*.c"
      "Core/*.cpp")

file (GLOB_RECURSE HEADERS
      "Core/*.h"
      "Core/*.hpp")

asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(asplib_Core STATIC ${SOURCES} ${HEADERS})
add_library(asplib::Core ALIAS asplib_Core)
target_include_directories(asplib_Core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set_property(TARGET asplib_Core PROPERTY FOLDER "modules")

# install all specific module files
asplib_install_module("Core" "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_Core DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
