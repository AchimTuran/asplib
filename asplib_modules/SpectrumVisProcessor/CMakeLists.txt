file(GLOB_RECURSE SOURCES
      "SpectrumVisProcessor/*.c"
      "SpectrumVisProcessor/*.cpp")

file(GLOB_RECURSE HEADERS
      "SpectrumVisProcessor/*.h"
      "SpectrumVisProcessor/*.hpp")
      
asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(asplib_SpectrumVisProcessor STATIC  ${SOURCES} ${HEADERS})
add_library(asplib::SpectrumVisProcessor ALIAS asplib_SpectrumVisProcessor)
target_include_directories(asplib_SpectrumVisProcessor
                           PRIVATE SpectrumVisProcessor
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(asplib_SpectrumVisProcessor PRIVATE asplib::Core asplib::FFT)

set_property(TARGET asplib_SpectrumVisProcessor PROPERTY FOLDER "modules")

add_subdirectory(tests)

set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};SpectrumVisProcessor" CACHE INTERNAL "")

# install all specific module files
asplib_install_module("SpectrumVisProcessor" "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib_SpectrumVisProcessor DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
