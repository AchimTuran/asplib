find_package(KissFFT REQUIRED)

file (GLOB_RECURSE ASPLIB_FFT_SOURCES
      "FFT/*.c"
      "FFT/*.cpp")

file (GLOB_RECURSE ASPLIB_FFT_HEADERS
      "FFT/*.h"
      "FFT/*.hpp")
      
#foreach(_file ${ASPLIB_FFT_SOURCES})
#message(STATUS "file=${_file}")
#endforeach()

asplib_source_group("${ASPLIB_FFT_SOURCES}")
asplib_source_group("${ASPLIB_FFT_HEADERS}")

add_library(asplib_FFT STATIC ${ASPLIB_FFT_SOURCES} ${ASPLIB_FFT_HEADERS})
add_library(asplib::FFT ALIAS asplib_FFT)
target_link_libraries(asplib_FFT PRIVATE asplib::Core KissFFT::KissFFT)
target_include_directories(asplib_FFT
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/FFT
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

set_property(TARGET asplib_FFT PROPERTY FOLDER "modules")

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};FFT" CACHE INTERNAL "")

# install all specific module files
asplib_install_module("FFT" "${ASPLIB_FFT_HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_FFT DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
