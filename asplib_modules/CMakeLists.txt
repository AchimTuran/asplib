if(ASPLIB_DEPENDS_SRC_PREFIX) # if ASPLIB_DEPENDS_SRC_PREFIX is defined use this directory for depenencies
  set(DEPENDS_SRC_PREFIX "-DASPLIB_DEPENDS_SRC_PREFIX=${ASPLIB_DEPENDS_SRC_PREFIX}")
else()
  set(DEPENDS_SRC_PREFIX "")
endif()

option(BUILD_AUDIOINTERFACES      "Build Audio Interfaces"     OFF)
option(BUILD_BIQUAD               "Build biquad"               OFF)
option(BUILD_COMPUTEDEVICEINFO    "Build Compute Device Info"  OFF)
option(BUILD_DYNAMICS             "Build Dynamics"             OFF)
option(BUILD_FFT                  "Build FFT"                  OFF)
option(BUILD_IIR                  "Build IIR"                  OFF)
option(BUILD_LOGGER               "Build Logger"               OFF)
option(BUILD_SIGNALS              "Build Signals"              OFF)
option(BUILD_SPECTRUMVISPROCESSOR "Build SpectrumVisProcessor" OFF)
option(BUILD_TIMER                "Build Timer"                OFF)
option(BUILD_RESAMPLING           "Build Resampling"           OFF)
option(BUILD_CONVOLUTION          "Build Convolution"          OFF)

set(AVAILABLE_MODULES AUDIOINTERFACES BIQUAD COMPUTEDEVICEINFO FFT IIR LOGGER SIGNALS SPECTRUMVISPROCESSOR TIMER)

if("${ASPLIB_MODULES_TO_BUILD}" STREQUAL "all" OR "${ASPLIB_MODULES_TO_BUILD}" STREQUAL "")# OR NOT ${ASPLIB_MODULES_TO_BUILD})
  set(BUILD_AUDIOINTERFACES OFF)
  set(BUILD_BIQUAD ON)
  set(BUILD_COMPUTEDEVICEINFO ON)
  set(BUILD_DYNAMICS ON)
  set(BUILD_FFT ON)
  set(BUILD_IIR ON)
  set(BUILD_LOGGER ON)
  set(BUILD_SIGNALS ON)
  set(BUILD_SPECTRUMVISPROCESSOR ON)
  set(BUILD_TIMER ON)
  set(BUILD_RESAMPLING ON)
  set(BUILD_CONVOLUTION ON)
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


include(../cmake/gitCloneProject.cmake)

message(STATUS "Download ARM_NEON_2_x86_SSE of is handled by asplib")
git_clone_project(PROJ                NEON2SSE
                  GIT_REPOSITORY      https://github.com/intel/ARM_NEON_2_x86_SSE.git
                  GIT_TAG             master)

add_subdirectory(Core)
set(ASPLIB_SUPPORTED_COMPONENTS "Core" CACHE INTERNAL "supported asplib modules")

if(BUILD_AUDIOINTERFACES)
  find_package(portaudio QUIET)
  
  if(NOT ASPLIB_PortAudio_BUILD) # only define this ASPLIB_PortAudio_BUILD once to determine if PortAudio is build by asplib or not
    if(NOT ${portaudio_FOUND} OR ${ASPLIB_FORCE_DEPENCY_BUILDS})
      set(ASPLIB_PortAudio_BUILD ON CACHE INTERNAL "If this variable is ON then asplib build system will build PortAudio else it should be handled outside of asplib")
    else()
      set(ASPLIB_PortAudio_BUILD OFF CACHE INTERNAL "If this variable is ON then asplib build system will build PortAudio else it should be handled outside of asplib")
    endif()
  endif()

  if(${ASPLIB_PortAudio_BUILD})
    if(MSVC)
      set(PA_USE_WASAPI ON)
      set(PA_USE_WDMKS ON)
      set(PA_USE_WMME ON)
      #set(PA_USE_ASIO ON)
      set(PA_UNICODE_BUILD ON)
    endif()
  
    message(STATUS "Build of PortAudio is handled by asplib")
    git_clone_project(PROJ                portaudio
                      GIT_REPOSITORY      https://github.com/kodi-adsp/portaudio_cmake
                      GIT_TAG             cmake)
    #include(../cmake/cmake_helpers/importURLProject.cmake)
    #importURLProject(PROJECT portaudio
    #                 URL "http://www.portaudio.com/archives"
    #                 DOWNLOAD_NAME "pa_stable_v19_20140130.tgz")
    set(portaudio_INCLUDE_DIRS "${portaudio_SOURCE_DIR}/include")
    add_subdirectory(${portaudio_SOURCE_DIR} ${portaudio_BINARY_DIR})
    if(MSVC)
      message(STATUS "patching PortAudio")
      set(PATCH_TOOL ${CMAKE_CURRENT_SOURCE_DIR}/../tools/windows/patch.exe)
      execute_process(COMMAND ${PATCH_TOOL} -p0 -i ${CMAKE_CURRENT_SOURCE_DIR}/../depends/windows/portaudio/portaudio_remove_ksguid.lib.patch WORKING_DIRECTORY ${portaudio_SOURCE_DIR})
    endif()
    set(portaudio_LIBRARY portaudio_static)
    set_property(TARGET portaudio PROPERTY FOLDER "contrib")
    set_property(TARGET portaudio_static PROPERTY FOLDER "contrib")
  endif()

  add_subdirectory(AudioInterfaces)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};AudioInterfaces" CACHE INTERNAL "supported asplib modules")
endif()

if(BUILD_BIQUAD)
  add_subdirectory(Biquad)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Biquad" CACHE INTERNAL "supported asplib modules")
endif()

if(BUILD_COMPUTEDEVICEINFO)
  add_subdirectory(ComputeDeviceInfo)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};ComputeDeviceInfo" CACHE INTERNAL "supported asplib modules")
endif()

if(BUILD_FFT OR BUILD_SPECTRUMVISPROCESSOR OR BUILD_CONVOLUTION)
  find_package(KissFFT 1.30 QUIET)
  
  if(NOT ASPLIB_KissFFT_BUILD) # only define this ASPLIB_KissFFT_BUILD once to determine if KissFFT is build by asplib or not
    if(NOT ${KissFFT_FOUND} OR ${ASPLIB_FORCE_DEPENCY_BUILDS})
      set(ASPLIB_KissFFT_BUILD ON CACHE INTERNAL "If this variable is ON then asplib build system will build KissFFT else it should be handled outside of asplib")
    else()
      set(ASPLIB_KissFFT_BUILD OFF CACHE INTERNAL "If this variable is ON then asplib build system will build KissFFT else it should be handled outside of asplib")
    endif()
  endif()

  if(${ASPLIB_KissFFT_BUILD})
    message(STATUS "Build of KissFFT is handled by asplib")
    git_clone_project(PROJ                KissFFT
                      GIT_REPOSITORY      https://github.com/AchimTuran/KissFFT
                      GIT_TAG             master)
    add_subdirectory(${KissFFT_SOURCE_DIR} ${KissFFT_BINARY_DIR})
  endif()

  add_subdirectory(FFT)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};FFT" CACHE INTERNAL "supported asplib modules")
endif()

if(BUILD_CONVOLUTION)
  add_subdirectory(Convolution)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Convolution" CACHE INTERNAL "supported asplib modules")
endif()

if(BUILD_IIR)
  add_subdirectory(IIR)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};IIR" CACHE INTERNAL "supported asplib modules")
endif()

if(BUILD_RESAMPLING)
  add_subdirectory(Resampling)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Resampling" CACHE INTERNAL "supported asplib modules")
endif()

if(BUILD_DYNAMICS)
  add_subdirectory(Dynamics)
  set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};Dynamics" CACHE INTERNAL "supported asplib modules")
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
