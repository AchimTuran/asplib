file (GLOB_RECURSE SOURCES
      "Timer/*.c"
      "Timer/*.cpp")

file (GLOB_RECURSE HEADERS
      "Timer/*.h"
      "Timer/*.hpp")
      
asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(asplib_Timer STATIC ${SOURCES} ${HEADERS})
add_library(asplib::Timer ALIAS asplib_Timer)
target_include_directories(asplib_Timer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(asplib_Timer PRIVATE asplib::Core)

set_property(TARGET asplib_Timer PROPERTY FOLDER "modules")

# install all specific module files
asplib_install_module("Timer" "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_Timer DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
