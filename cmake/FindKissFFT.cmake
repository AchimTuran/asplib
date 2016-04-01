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

if(NOT KISS_FFT_FOUND)
  include(ExternalProject)

  message(STATUS "KissFFT was not found. Try to download KissFFT from https://github.com/AchimTuran/KissFFT")
  ExternalProject_Add(KissFFT
                      LOG_DOWNLOAD    1
                      LOG_UPDATE      1
                      LOG_CONFIGURE   1
                      LOG_BUILD       1
                      LOG_INSTALL     1
                      
                      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/depends/output
                                 -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                      
                      #PREFIX          ${CMAKE_BINARY_DIR}/depends/build/KissFFT
                      TMP_DIR         ${CMAKE_BINARY_DIR}/depends/build/KissFFT/KissFFT-tmp
                      STAMP_DIR       ${CMAKE_BINARY_DIR}/depends/build/KissFFT/KissFFT-stamp
                      
                      BINARY_DIR      ${CMAKE_BINARY_DIR}/depends/build/KissFFT/KissFFT-project
                      
                      #INSTALL_DIR     ${CMAKE_BINARY_DIR}/depends/include
                      SOURCE_DIR      ${CMAKE_BINARY_DIR}/depends/src/KissFFT
        
                      GIT_REPOSITORY  https://github.com/AchimTuran/KissFFT
                      GIT_TAG         master)
            
  set(KISS_FFT_FOUND TRUE)
  set(KISS_FFT_LIBRARIES ${CMAKE_BINARY_DIR}/depends/output/lib/KissFFT.lib)
  set(KISS_FFT_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/depends/output/include)
endif()

# handle the QUIETLY and REQUIRED arguments and set KISS_FFT_FOUND to TRUE if
# all listed variables are TRUE
include("FindPackageHandleStandardArgs")
find_package_handle_standard_args(KISS_FFT DEFAULT_MSG KISS_FFT_INCLUDE_DIRS KISS_FFT_LIBRARIES)
mark_as_advanced(KISS_FFT_INCLUDE_DIRS KISS_FFT_LIBRARIES KISS_FFT_DEFINITIONS KISS_FFT_FOUND)

message(STATUS "KISS_FFT_INCLUDE_DIRS=${KISS_FFT_INCLUDE_DIRS}")
message(STATUS "KISS_FFT_LIBRARIES=${KISS_FFT_LIBRARIES}")
