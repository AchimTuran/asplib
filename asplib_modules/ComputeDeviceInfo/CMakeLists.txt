file (GLOB_RECURSE SOURCES
      "ComputeDeviceInfo/*.c"
      "ComputeDeviceInfo/*.cpp")

file (GLOB_RECURSE HEADERS
      "ComputeDeviceInfo/*.h"
      "ComputeDeviceInfo/*.hpp")
      
asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(asplib_ComputeDeviceInfo STATIC ${SOURCES} ${HEADERS})
add_library(asplib::ComputeDeviceInfo ALIAS asplib_ComputeDeviceInfo)
target_include_directories(asplib_ComputeDeviceInfo INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(asplib_ComputeDeviceInfo PRIVATE asplib::Core)

set_property(TARGET asplib_ComputeDeviceInfo PROPERTY FOLDER "modules")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

# install all specific module files
asplib_install_module("ComputeDeviceInfo" "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_ComputeDeviceInfo DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
