# - Try to find KissFFT
# Once done this will define
#
# KISS_FFT_FOUND          - system has KissFFT
# KISS_FFT_INCLUDE_DIRS   - the KissFFT include directory
# KISS_FFT_LIBRARIES      - The KissFFT libraries

if(PKG_CONFIG_FOUND)
  pkg_check_modules(KISS_FFT KissFFT)
  list(APPEND KISS_FFT_INCLUDE_DIRS ${KISS_FFT_INCLUDEDIR})
endif()

if(NOT KISS_FFT_FOUND)
  #message(STATUS "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
  find_path(KISS_FFT_INCLUDE_DIRS   "kissfft.hh"
                                    "kiss_fft.h"
                                    "_kiss_fft_guts.h"
                                    PATH_SUFFIXES "KissFFT*")

  find_library(KISS_FFT_LIBRARIES
               NAMES "KissFFT"
               PATH_SUFFIXES "KissFFT*" )
endif()

# handle the QUIETLY and REQUIRED arguments and set SAMPLERATE_FOUND to TRUE if
# all listed variables are TRUE
include("FindPackageHandleStandardArgs")
find_package_handle_standard_args(KissFFT DEFAULT_MSG KISS_FFT_INCLUDE_DIRS KISS_FFT_LIBRARIES)

mark_as_advanced(KISS_FFT_INCLUDE_DIRS KISS_FFT_LIBRARIES)

#message(STATUS "KISS_FFT_INCLUDE_DIRS=${KISS_FFT_INCLUDE_DIRS}")
#message(STATUS "KISS_FFT_LIBRARIES=${KISS_FFT_LIBRARIES}")
