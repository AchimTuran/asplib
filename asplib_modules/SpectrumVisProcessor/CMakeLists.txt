asplib_require_module(Core)
asplib_require_module(FFT)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/SpectrumVisProcessor
                    ${asplib_Core_INCLUDE_DIRS}
                    ${asplib_FFT_INCLUDE_DIRS})

file(GLOB_RECURSE ASPLIB_SPECTRUM_VIS_PROCESSOR_SOURCES
      "SpectrumVisProcessor/*.c"
      "SpectrumVisProcessor/*.cpp")

file(GLOB_RECURSE ASPLIB_SPECTRUM_VIS_PROCESSOR_HEADERS
      "SpectrumVisProcessor/*.h"
      "SpectrumVisProcessor/*.hpp")
      
asplib_source_group("${ASPLIB_SPECTRUM_VIS_PROCESSOR_SOURCES}")
asplib_source_group("${ASPLIB_SPECTRUM_VIS_PROCESSOR_HEADERS}")
add_library(asplib_SpectrumVisProcessor STATIC  ${ASPLIB_SPECTRUM_VIS_PROCESSOR_SOURCES}
                                                ${ASPLIB_SPECTRUM_VIS_PROCESSOR_HEADERS})

set_property(TARGET asplib_SpectrumVisProcessor PROPERTY FOLDER "modules")

target_link_libraries(asplib_SpectrumVisProcessor asplib_Core asplib_FFT)

add_subdirectory(tests)

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};SpectrumVisProcessor" CACHE INTERNAL "")

# install all specific module files
asplib_install_module("SpectrumVisProcessor" "${ASPLIB_SPECTRUM_VIS_PROCESSOR_HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_SpectrumVisProcessor DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
