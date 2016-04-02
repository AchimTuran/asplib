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
add_library(asplib::Signals ALIAS asplib_Signals)
target_include_directories(asplib_Signals INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(asplib_Signals PRIVATE asplib::Core)

set_property(TARGET asplib_Signals PROPERTY FOLDER "modules")

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Signals" CACHE INTERNAL "")

# install all specific module files
asplib_install_module("Signals" "${ASPLIB_SIGNALS_HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_Signals DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
