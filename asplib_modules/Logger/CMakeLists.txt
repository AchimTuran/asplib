file (GLOB_RECURSE SOURCES
      "Logger/*.c"
      "Logger/*.cpp")

file (GLOB_RECURSE HEADERS
      "Logger/*.h"
      "Logger/*.hpp")
      
asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(asplib_Logger STATIC ${SOURCES} ${HEADERS})
add_library(asplib::Logger ALIAS asplib_Logger)
target_link_libraries(asplib_Logger PRIVATE asplib::Core)

set_property(TARGET asplib_Logger PROPERTY FOLDER "modules")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

# install all specific module files
asplib_install_module("Logger" "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_Logger DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
