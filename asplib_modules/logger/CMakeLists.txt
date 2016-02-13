asplib_require_module(Core)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${asplib_Core_INCLUDE_DIRS})

file (GLOB_RECURSE ASPLIB_LOGGER_SOURCES
      "Logger/*.c"
      "Logger/*.cpp")

file (GLOB_RECURSE ASPLIB_LOGGER_HEADERS
      "Logger/*.h"
      "Logger/*.hpp")
      
asplib_source_group("${ASPLIB_LOGGER_SOURCES}")
asplib_source_group("${ASPLIB_LOGGER_HEADERS}")
add_library(asplib_Logger STATIC ${ASPLIB_LOGGER_SOURCES} ${ASPLIB_LOGGER_HEADERS})

set_property(TARGET asplib_Logger PROPERTY FOLDER "modules")

target_link_libraries(asplib_Logger asplib_Core)

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Logger" CACHE INTERNAL "")
