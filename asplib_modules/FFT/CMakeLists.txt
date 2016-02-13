asplib_require_module(Core)

find_package(KissFFT REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/FFT
                    ${asplib_Core_INCLUDE_DIRS}
                    ${KissFFT_INCLUDE_DIRS})
                    
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

set_property(TARGET asplib_FFT PROPERTY FOLDER "modules")

target_link_libraries(asplib_FFT asplib_Core)

set(asplib_FFT_INCLUDE_DIRS  ${CMAKE_CURRENT_SOURCE_DIR}
                              CACHE INTERNAL "asplib_Core: Include Directories" FORCE)

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};FFT" CACHE INTERNAL "")
