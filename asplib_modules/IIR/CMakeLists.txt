file (GLOB_RECURSE ASPLIB_IIR_SOURCES
      "IIR/*.c"
      "IIR/*.cpp")

file (GLOB_RECURSE ASPLIB_IIR_HEADERS
      "IIR/*.h"
      "IIR/*.hpp")
      
asplib_source_group("${ASPLIB_IIR_SOURCES}")
asplib_source_group("${ASPLIB_IIR_HEADERS}")

add_library(asplib_IIR STATIC ${ASPLIB_IIR_SOURCES} ${ASPLIB_IIR_HEADERS})
add_library(asplib::IIR ALIAS asplib_IIR)
target_include_directories(asplib_IIR INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(asplib_IIR PRIVATE asplib::Core)

set_property(TARGET asplib_IIR PROPERTY FOLDER "modules")

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};IIR" CACHE INTERNAL "")

# install all specific module files
asplib_install_module("IIR" "${ASPLIB_IIR_HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_IIR DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
