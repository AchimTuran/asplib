file (GLOB_RECURSE SOURCES
      "IIR/*.c"
      "IIR/*.cpp")

file (GLOB_RECURSE HEADERS
      "IIR/*.h"
      "IIR/*.hpp")
      
asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(asplib_IIR STATIC ${SOURCES} ${HEADERS})
add_library(asplib::IIR ALIAS asplib_IIR)
target_include_directories(asplib_IIR INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(asplib_IIR PRIVATE asplib::Core)

set_property(TARGET asplib_IIR PROPERTY FOLDER "modules")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

# install all specific module files
asplib_install_module("IIR" "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_IIR DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
