asplib_require_module(Core)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${asplib_Core_INCLUDE_DIRS})

file (GLOB_RECURSE ASPLIB_SIGNALS_SOURCES
      "Signals/*.c"
      "Signals/*.cpp")

file (GLOB_RECURSE ASPLIB_SIGNALS_HEADERS
      "Signals/*.h"
      "Signals/*.hpp")
      
#foreach(_file ${ASPLIB_SIGNALS_SOURCES})
#message(STATUS "file=${_file}")
#endforeach()

asplib_source_group("${ASPLIB_SIGNALS_SOURCES}")
asplib_source_group("${ASPLIB_SIGNALS_HEADERS}")
add_library(asplib_Signals STATIC ${ASPLIB_SIGNALS_SOURCES} ${ASPLIB_SIGNALS_HEADERS})

set_property(TARGET asplib_Signals PROPERTY FOLDER "modules")

target_link_libraries(asplib_Signals asplib_Core)

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Signals" CACHE INTERNAL "")
