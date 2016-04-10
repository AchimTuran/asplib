project(Core)
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

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:include>
                           $<INSTALL_INTERFACE:include/asplib>)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "modules")

# install all specific module files
asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/${CMAKE_PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
