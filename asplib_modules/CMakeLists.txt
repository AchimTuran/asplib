if(NOT ASPLIB_MODULES_PATH)
  set(ASPLIB_MODULES_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

message(STATUS "---------Processing asplib modules----------")

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
                   
foreach(asplib_module ${ASPLIB_MODULES})
  message(STATUS "asplib_module=${asplib_module}")
endforeach()

asplib_include_modules(${ASPLIB_MODULES_PATH} "${ASPLIB_MODULES}")#OPTIONAL ${ASPLIB_MODULES_OPTIONAL_PATH})
