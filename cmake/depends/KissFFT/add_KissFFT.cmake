include(ExternalProject)

if(${ASPLIB_DEPENDS_DOWNLOAD})
  set(KISS_FFT_GIT_DOWNLOAD GIT_REPOSITORY  https://github.com/AchimTuran/KissFFT.git
                            GIT_TAG         V1.30)
  message(STATUS "Use ${KISS_FFT_GIT_DOWNLOAD} to download KissFFT")
else()
  set(KISS_FFT_GIT_DOWNLOAD "")
  message(STATUS "Use ${ASPLIB_DEPENDS_SRC_PREFIX} as local prefix directory to build asplib dependencies")
endif()

ExternalProject_Add(KissFFT_external_build
                    LOG_DOWNLOAD    1
                    LOG_UPDATE      1
                    LOG_CONFIGURE   1
                    LOG_BUILD       1
                    LOG_INSTALL     1
                    
                    
                    CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}"
                               "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}"
                    
                    PREFIX          ${CMAKE_BINARY_DIR}/workspace/KissFFT
                    TMP_DIR         ${CMAKE_BINARY_DIR}/workspace/KissFFT/tmp
                    STAMP_DIR       ${CMAKE_BINARY_DIR}/workspace/KissFFT/stamp
                    
                    BINARY_DIR      ${CMAKE_BINARY_DIR}/workspace/KissFFT/project
                    
                    #INSTALL_DIR     ${CMAKE_BINARY_DIR}/depends/include
                    #SOURCE_DIR      ${ASPLIB_DEPENDS_SRC_PREFIX}/KissFFT
                    SOURCE_DIR      D:/DevProjects/myLibs/asplib_refactor/KissFFT
      
                    ${KISS_FFT_GIT_DOWNLOAD})
