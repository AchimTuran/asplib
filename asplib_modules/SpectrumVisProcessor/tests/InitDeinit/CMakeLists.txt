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
target_link_libraries(asplib_TestSpectrumVisProcessor_InitDeinit asplib::Core asplib::FFT asplib::SpectrumVisProcessor)

set_property(TARGET asplib_TestSpectrumVisProcessor_InitDeinit PROPERTY FOLDER "tests")
