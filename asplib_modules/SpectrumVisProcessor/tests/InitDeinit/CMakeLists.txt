include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${KissFFT_INCLUDE_DIRS}
                    ${asplib_SpectrumVisProcessor_INCLUDE_DIRS}
                    ${asplib_Core_INCLUDE_DIRS}
                    ${asplib_FFT_INCLUDE_DIRS})
                    
find_package(KissFFT REQUIRED)

file (GLOB_RECURSE ASPLIB_TEST_INIT_DEINIT_SPECTRUM_VIS_PROCESSOR_SOURCES
      "*.c"
      "*.cpp")

file (GLOB_RECURSE ASPLIB_TEST_INIT_DEINIT_SPECTRUM_VIS_PROCESSOR_HEADERS
      "*.h"
      "*.hpp")
      
if(NOT ASPLIB_TEST_INIT_DEINIT_SPECTRUM_VIS_PROCESSOR_SOURCES)
  set(${ASPLIB_TEST_INIT_DEINIT_SPECTRUM_VIS_PROCESSOR_SOURCES} "")
endif()

if(NOT ASPLIB_TEST_INIT_DEINIT_SPECTRUM_VIS_PROCESSOR_HEADERS)
  set(${ASPLIB_TEST_INIT_DEINIT_SPECTRUM_VIS_PROCESSOR_HEADERS} "")
endif()
      
#foreach(_file ${ASPLIB_AUDIO_INTERFACES_SOURCES})
#message(STATUS "file=${_file}")
#endforeach()

#asplib_source_group("${ASPLIB_TEST_INIT_DEINIT_SPECTRUM_VIS_PROCESSOR_SOURCES}")
#asplib_source_group("${ASPLIB_TEST_INIT_DEINIT_SPECTRUM_VIS_PROCESSOR_HEADERS}")
add_executable(asplib_TestSpectrumVisProcessor_InitDeinit ${ASPLIB_TEST_INIT_DEINIT_SPECTRUM_VIS_PROCESSOR_SOURCES}
                                                          ${ASPLIB_TEST_INIT_DEINIT_SPECTRUM_VIS_PROCESSOR_HEADERS})

#KissFFT_LIBRARY_DIRS
message(STATUS "KissFFT_LIBRARIES=${KissFFT_LIBRARIES}")
message(STATUS "KissFFT_LIBRARY_DIRS=${KissFFT_LIBRARY_DIRS}")
target_link_libraries(asplib_TestSpectrumVisProcessor_InitDeinit asplib_Core asplib_FFT asplib_SpectrumVisProcessor ${KissFFT_LIBRARIES})

set_property(TARGET asplib_TestSpectrumVisProcessor_InitDeinit PROPERTY FOLDER "tests")
