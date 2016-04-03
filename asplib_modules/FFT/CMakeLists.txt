find_package(KissFFT REQUIRED)

file (GLOB_RECURSE SOURCES
      "FFT/*.c"
      "FFT/*.cpp")

file (GLOB_RECURSE HEADERS
      "FFT/*.h"
      "FFT/*.hpp")

asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(asplib_FFT STATIC ${SOURCES} ${HEADERS})
add_library(asplib::FFT ALIAS asplib_FFT)
target_link_libraries(asplib_FFT PRIVATE asplib::Core KissFFT::KissFFT)
target_include_directories(asplib_FFT
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/FFT
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

set_property(TARGET asplib_FFT PROPERTY FOLDER "modules")

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};FFT" CACHE INTERNAL "")

# install all specific module files
asplib_install_module("FFT" "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_FFT DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
