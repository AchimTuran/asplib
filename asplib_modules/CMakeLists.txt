if(NOT ASPLIB_MODULES_PATH)
  set(ASPLIB_MODULES_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

message(STATUS "ASPLIB_MODULES_TO_BUILD=${ASPLIB_MODULES_TO_BUILD}")

message(STATUS "---------Processing asplib modules----------")

set(ASPLIB_SUPPORTED_COMPONENTS "" CACHE INTERNAL "supported asplib components")
set(asplib_required_modules "" CACHE INTERNAL "Temporary variable to store needed module dependencies")

if("${ASPLIB_MODULES_TO_BUILD}" STREQUAL "all")
  set(ASPLIB_MODULES  "Core"
                      "Logger"
                      "Timer"
                      "ComputeDeviceInfo"
                      
                      "IIR"
                      "Biquad"
                      
                      "FFT"
                      "Signals"
                      "SpectrumVisProcessor"

                      "AudioInterfaces")
else()
  set(ASPLIB_MODULES "${ASPLIB_MODULES_TO_BUILD}")
endif()

asplib_include_modules(${ASPLIB_MODULES_PATH} "${ASPLIB_MODULES}")

message(STATUS "Available asplib modules:")
foreach(_mod ${ASPLIB_SUPPORTED_COMPONENTS})
  message(STATUS "  ${_mod}")
endforeach()
