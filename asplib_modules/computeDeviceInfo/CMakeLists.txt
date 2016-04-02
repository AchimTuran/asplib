file (GLOB_RECURSE ASPLIB_COMPUTE_DEVICE_INFO_SOURCES
      "ComputeDeviceInfo/*.c"
      "ComputeDeviceInfo/*.cpp")

file (GLOB_RECURSE ASPLIB_COMPUTE_DEVICE_INFO_HEADERS
      "ComputeDeviceInfo/*.h"
      "ComputeDeviceInfo/*.hpp")
      
#foreach(_file ${ASPLIB_COMPUTE_DEVICE_INFO_SOURCES})
#message(STATUS "file=${_file}")
#endforeach()

asplib_source_group("${ASPLIB_COMPUTE_DEVICE_INFO_SOURCES}")
asplib_source_group("${ASPLIB_COMPUTE_DEVICE_INFO_HEADERS}")

add_library(asplib_ComputeDeviceInfo STATIC ${ASPLIB_COMPUTE_DEVICE_INFO_SOURCES} ${ASPLIB_COMPUTE_DEVICE_INFO_HEADERS})
add_library(asplib::ComputeDeviceInfo ALIAS asplib_ComputeDeviceInfo)
target_include_directories(asplib_ComputeDeviceInfo INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(asplib_ComputeDeviceInfo PRIVATE asplib::Core)

set_property(TARGET asplib_ComputeDeviceInfo PROPERTY FOLDER "modules")

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};ComputeDeviceInfo" CACHE INTERNAL "")

# install all specific module files
asplib_install_module("ComputeDeviceInfo" "${ASPLIB_COMPUTE_DEVICE_INFO_HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_ComputeDeviceInfo DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
