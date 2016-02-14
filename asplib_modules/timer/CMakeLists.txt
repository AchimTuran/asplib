asplib_require_module(Core)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${asplib_Core_INCLUDE_DIRS})

file (GLOB_RECURSE ASPLIB_TIMER_SOURCES
      "Timer/*.c"
      "Timer/*.cpp")

file (GLOB_RECURSE ASPLIB_TIMER_HEADERS
      "Timer/*.h"
      "Timer/*.hpp")
      
#foreach(_file ${ASPLIB_INTERFACES_SOURCES})
#message(STATUS "file=${_file}")
#endforeach()

asplib_source_group("${ASPLIB_TIMER_SOURCES}")
asplib_source_group("${ASPLIB_TIMER_HEADERS}")
add_library(asplib_Timer STATIC ${ASPLIB_TIMER_SOURCES} ${ASPLIB_TIMER_HEADERS})

set_property(TARGET asplib_Timer PROPERTY FOLDER "modules")

target_link_libraries(asplib_Timer asplib_Core)

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Timer" CACHE INTERNAL "")

# install all specific module files
asplib_install_module("Timer" "${ASPLIB_TIMER_HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_Timer DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
