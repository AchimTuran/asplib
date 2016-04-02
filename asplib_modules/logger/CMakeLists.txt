file (GLOB_RECURSE ASPLIB_LOGGER_SOURCES
      "Logger/*.c"
      "Logger/*.cpp")

file (GLOB_RECURSE ASPLIB_LOGGER_HEADERS
      "Logger/*.h"
      "Logger/*.hpp")
      
asplib_source_group("${ASPLIB_LOGGER_SOURCES}")
asplib_source_group("${ASPLIB_LOGGER_HEADERS}")

add_library(asplib_Logger STATIC ${ASPLIB_LOGGER_SOURCES} ${ASPLIB_LOGGER_HEADERS})
add_library(asplib::Logger ALIAS asplib_Logger)
target_link_libraries(asplib_Logger PRIVATE asplib::Core)

set_property(TARGET asplib_Logger PROPERTY FOLDER "modules")

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Logger" CACHE INTERNAL "")

# install all specific module files
asplib_install_module("Logger" "${ASPLIB_LOGGER_HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_Logger DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
