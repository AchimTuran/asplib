# - Try to find KissFFT
# Once done this will define
#
# PORT_AUDIO_FOUND          - system has KissFFT
# PORT_AUDIO_INCLUDE_DIRS   - the KissFFT include directory
# PORT_AUDIO_LIBRARIES      - The KissFFT libraries

if(PKG_CONFIG_FOUND)
  pkg_check_modules(PORT_AUDIO portaudio)
  list(APPEND PORT_AUDIO_INCLUDE_DIRS ${PORT_AUDIO_INCLUDEDIR})
endif()

if(NOT PORT_AUDIO_FOUND)
  message(STATUS "FINDPortAudio.cmake CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
  find_path(PORT_AUDIO_INCLUDE_DIRS #"pa_asio.h"
                                    "pa_jack.h"
                                    #"pa_linux_alsa.h"
                                    #"pa_mac_core.h"
                                    "pa_win_ds.h"
                                    "pa_win_wasapi.h"
                                    "pa_win_waveformat.h"
                                    "pa_win_wdmks.h"
                                    "pa_win_wmme.h"
                                    "portaudio.h"
                                    PATH_SUFFIXES "portaudio*")

  find_library(PORT_AUDIO_LIBRARIES
               NAMES "portaudio"
               PATH_SUFFIXES "portaudio*" )
endif()

# handle the QUIETLY and REQUIRED arguments and set SAMPLERATE_FOUND to TRUE if
# all listed variables are TRUE
include("FindPackageHandleStandardArgs")
find_package_handle_standard_args(portaudio DEFAULT_MSG PORT_AUDIO_INCLUDE_DIRS PORT_AUDIO_LIBRARIES)

mark_as_advanced(PORT_AUDIO_INCLUDE_DIRS PORT_AUDIO_LIBRARIES)

message(STATUS "PORT_AUDIO_INCLUDE_DIRS=${PORT_AUDIO_INCLUDE_DIRS}")
message(STATUS "PORT_AUDIO_LIBRARIES=${PORT_AUDIO_LIBRARIES}")
