file (GLOB_RECURSE SOURCES
      "biquad/*.c"
      "Biquad/*.cpp")

file (GLOB_RECURSE HEADERS
      "Biquad/*.h"
      "Biquad/*.hpp")

asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(asplib_Biquad STATIC ${SOURCES} ${HEADERS})
add_library(asplib::Biquad ALIAS asplib_Biquad)
target_include_directories(asplib_Biquad INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(asplib_Biquad PRIVATE asplib::Core)

set_property(TARGET asplib_Biquad PROPERTY FOLDER "modules")

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Biquad" CACHE INTERNAL "")

# install all specific module files
asplib_install_module("Biquad" "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_Biquad DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
