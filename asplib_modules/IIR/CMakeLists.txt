asplib_require_module(Core)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${asplib_Core_INCLUDE_DIRS})

file (GLOB_RECURSE ASPLIB_IIR_SOURCES
      "IIR/*.c"
      "IIR/*.cpp")

file (GLOB_RECURSE ASPLIB_IIR_HEADERS
      "IIR/*.h"
      "IIR/*.hpp")
      
asplib_source_group("${ASPLIB_IIR_SOURCES}")
asplib_source_group("${ASPLIB_IIR_HEADERS}")
add_library(asplib_IIR STATIC ${ASPLIB_IIR_SOURCES} ${ASPLIB_IIR_HEADERS})

set_property(TARGET asplib_IIR PROPERTY FOLDER "modules")

target_link_libraries(asplib_IIR asplib_Core)

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};IIR" CACHE INTERNAL "")
