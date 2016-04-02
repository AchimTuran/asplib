# - Try to find KissFFT
# Once done this will define the following variables
#
# KISS_FFT_FOUND          - system has KissFFT
# KISS_FFT_INCLUDE_DIRS   - the KissFFT include directory
# KISS_FFT_LIBRARIES      - The KissFFT libraries
#
# and the following imported targets
#
#   KissFFT::KissFFT   - The KissFFT library

if(PKG_CONFIG_FOUND)
  pkg_check_modules(PC_KissFFT KissFFT)
endif()

#message(STATUS "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
find_path(KISS_FFT_INCLUDE_DIR NAMES kissfft.hh kiss_fft.h _kiss_fft_guts.h
                               PATH_SUFFIXES "KissFFT*"
                               PATHS ${PC_KissFFT_INCLUDEDIR})

find_library(KISS_FFT_LIBRARY NAMES KissFFT
                              PATH_SUFFIXES "KissFFT*"
                              PATHS ${PC_KissFFT_LIBDIR})

if(NOT KISS_FFT_INCLUDE_DIR AND NOT KISS_FFT_LIBRARIES)
  message(STATUS "KissFFT was not found. Downloading KissFFT from https://github.com/AchimTuran/KissFFT")
  include(ExternalProject)
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

  set(KISS_FFT_INCLUDE_DIR ${CMAKE_BINARY_DIR}/depends/output/include)
  set(KISS_FFT_LIBRARY ${CMAKE_BINARY_DIR}/depends/output/lib/KissFFT.lib)

  # Workaround for: https://cmake.org/Bug/view.php?id=15052
  file(MAKE_DIRECTORY ${KISS_FFT_INCLUDE_DIR})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(KISS_FFT DEFAULT_MSG KISS_FFT_INCLUDE_DIR KISS_FFT_LIBRARY)
mark_as_advanced(KISS_FFT_INCLUDE_DIR KISS_FFT_LIBRARY)

if(KISS_FFT_FOUND)
  set(KISS_FFT_INCLUDE_DIRS ${KISS_FFT_INCLUDE_DIR})
  set(KISS_FFT_LIBRARIES ${KISS_FFT_LIBRARY})

  if(NOT TARGET KissFFT::KissFFT)
    add_library(KissFFT::KissFFT UNKNOWN IMPORTED)
    set_target_properties(KissFFT::KissFFT PROPERTIES
                                           IMPORTED_LOCATION "${KISS_FFT_LIBRARY}"
                                           INTERFACE_INCLUDE_DIRECTORIES "${KISS_FFT_INCLUDE_DIR}")
    if(TARGET KissFFT)
      add_dependencies(KissFFT::KissFFT KissFFT)
    endif()
  endif()
endif()
