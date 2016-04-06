#if(NOT ASPLIB_MODULES_PATH)
#  set(ASPLIB_MODULES_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
#endif()

#message(STATUS "ASPLIB_MODULES_TO_BUILD=${ASPLIB_MODULES_TO_BUILD}")

## build all asplib modules or only user specific with automatic depency handling
#if("${ASPLIB_MODULES_TO_BUILD}" STREQUAL "all" OR "${ASPLIB_MODULES_TO_BUILD}" STREQUAL "supported asplib modules")
#  # for better module handling convert ASPLIB_MODULES_PATH to a absolute path
#  get_filename_component(_modules_path "${ASPLIB_MODULES_PATH}" ABSOLUTE)
  
#  # search for possible module folders
#  file(GLOB _asplib_modules RELATIVE "${_modules_path}" "${_modules_path}/*")
#  foreach(_mod ${_asplib_modules})
#    if(EXISTS "${_modules_path}/${_mod}/CMakeLists.txt")
#      # only add modules folders to ASPLIB_MODULES, which include a CMakeLists.txt
#      get_filename_component(_module_name ${_mod} NAME)

#      list(APPEND ASPLIB_MODULES ${_module_name})
#    endif()
#  endforeach()
  
#  foreach(_mod ${ASPLIB_MODULES})
#    message(STATUS "  ${_mod}")
#  endforeach()
#else()
#  set(ASPLIB_MODULES "${ASPLIB_MODULES_TO_BUILD}")
#endif()

## register the requested asplib modules
#asplib_include_modules(${ASPLIB_MODULES_PATH} "${ASPLIB_MODULES}")

## output the registered asplib modules
#message(STATUS "supported asplib modules")
#message(STATUS "Available asplib modules:")
#foreach(_mod ${ASPLIB_SUPPORTED_COMPONENTS})
#  message(STATUS "  ${_mod}")
#endforeach()
#message(STATUS "supported asplib modules")

option(BUILD_AUDIOINTERFACES      "Build Audio Interfaces"     OFF)
option(BUILD_BIQUAD               "Build biquad"               OFF)
option(BUILD_COMPUTEDEVICEINFO    "Build Compute Device Info"  OFF)
option(BUILD_FFT                  "Build FFT"                  OFF)
option(BUILD_IIR                  "Build IIR"                  OFF)
option(BUILD_LOGGER               "Build Logger"               OFF)
option(BUILD_SIGNALS              "Build Signals"              OFF)
option(BUILD_SPECTRUMVISPROCESSOR "Build SpectrumVisProcessor" OFF)
option(BUILD_TIMER                "Build Timer"                OFF)

set(AVAILABLE_MODULES AUDIOINTERFACES BIQUAD COMPUTEDEVICEINFO FFT IIR LOGGER SIGNALS SPECTRUMVISPROCESSOR TIMER)

if(${ASPLIB_MODULES_TO_BUILD} MATCHES "all")# OR NOT DEFINED ${ASPLIB_MODULES_TO_BUILD})
  option(BUILD_AUDIOINTERFACES      "Build Audio Interfaces"     ON)
  option(BUILD_BIQUAD               "Build biquad"               ON)
  option(BUILD_COMPUTEDEVICEINFO    "Build Compute Device Info"  ON)
  option(BUILD_FFT                  "Build FFT"                  ON)
  option(BUILD_IIR                  "Build IIR"                  ON)
  option(BUILD_LOGGER               "Build Logger"               ON)
  option(BUILD_SIGNALS              "Build Signals"              ON)
  option(BUILD_SPECTRUMVISPROCESSOR "Build SpectrumVisProcessor" ON)
  option(BUILD_TIMER                "Build Timer"                ON)
else()
  message(STATUS "ASPLIB_MODULES_TO_BUILD=${ASPLIB_MODULES_TO_BUILD}")
  foreach(_mod ${ASPLIB_MODULES_TO_BUILD})
    string(TOUPPER ${_mod} _MODULE)
    message(STATUS "_MODULE=${_MODULE}")
    foreach(_avmod ${AVAILABLE_MODULES})
      if(${_avmod} MATCHES ${_MODULE})
        set(BUILD_${_MODULE} ON)
        message(STATUS "BUILD_${_MODULE}=${BUILD_${_MODULE}}")
      endif()
    endforeach()
  endforeach()
endif()


add_subdirectory(Core)
set(ASPLIB_SUPPORTED_COMPONENTS "Core" CACHE INTERNAL "supported asplib modules")

if(BUILD_AUDIOINTERFACES)
  add_subdirectory(audioInterfaces)
  #set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};AudioInterfaces" CACHE INTERNAL "supported asplib modules")
endif()

if(BUILD_BIQUAD)
  add_subdirectory(Biquad)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Biquad" CACHE INTERNAL "supported asplib modules")
endif()

if(BUILD_COMPUTEDEVICEINFO)
  add_subdirectory(ComputeDeviceInfo)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};ComputeDeviceInfo" CACHE INTERNAL "supported asplib modules")
endif()

if(BUILD_FFT OR BUILD_SPECTRUMVISPROCESSOR)
  add_subdirectory(FFT)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};FFT" CACHE INTERNAL "supported asplib modules")
endif()

if(BUILD_IIR)
  add_subdirectory(IIR)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};IIR" CACHE INTERNAL "supported asplib modules")
endif()

if(BUILD_LOGGER)
  add_subdirectory(Logger)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Logger" CACHE INTERNAL "supported asplib modules")
endif()

if(BUILD_SIGNALS)
  add_subdirectory(Signals)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Signals" CACHE INTERNAL "supported asplib modules")
endif()

if(BUILD_SPECTRUMVISPROCESSOR)
  add_subdirectory(SpectrumVisProcessor)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};SpectrumVisProcessor" CACHE INTERNAL "supported asplib modules")
endif()

if(BUILD_TIMER)
  add_subdirectory(Timer)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Timer" CACHE INTERNAL "supported asplib modules")
endif()
