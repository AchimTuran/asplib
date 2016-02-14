asplib_require_module(Core)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${asplib_Core_INCLUDE_DIRS})

file (GLOB_RECURSE ASPLIB_BIQUAD_SOURCES
      "biquad/*.c"
      "Biquad/*.cpp")

file (GLOB_RECURSE ASPLIB_BIQUAD_HEADERS
      "Biquad/*.h"
      "Biquad/*.hpp")
      
#foreach(_file ${ASPLIB_BIQUAD_SOURCES})
#message(STATUS "file=${_file}")
#endforeach()

asplib_source_group("${ASPLIB_BIQUAD_SOURCES}")
asplib_source_group("${ASPLIB_BIQUAD_HEADERS}")
add_library(asplib_Biquad STATIC ${ASPLIB_BIQUAD_SOURCES} ${ASPLIB_BIQUAD_HEADERS})

set_property(TARGET asplib_Biquad PROPERTY FOLDER "modules")

# add asplib_Core as dependency
target_link_libraries(asplib_Biquad asplib_Core)

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Biquad" CACHE INTERNAL "")

# install all specific module files
asplib_install_module("Biquad" "${ASPLIB_BIQUAD_HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_Biquad DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
