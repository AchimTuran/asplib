cmake_minimum_required(VERSION 2.6)
project(asplib CXX)

set(asplib_NAME asplib)
set(asplib_DESCRIPTION "asplib (Achim's Signal Processing LIBrary), is a small and lightweight C++ library for digital signal processing.")
set(asplib_VERSION_MAJOR ${APP_VERSION_MAJOR})
set(asplib_VERSION_MINOR ${APP_VERSION_MINOR})
set(asplib_VERSION_PATCH 0)

include(UseMultiArch.cmake)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
include_directories(${CMAKE_MODULE_PATH})

# Enable pic
if(NOT WIN32)
    set(CMAKE_POSITION_INDEPENDENT_CODE 1)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "-DTARGET_LINUX")
endif()
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_CXX_FLAGS "-DTARGET_WINDOWS")
endif()

if (NOT WIN32)
  add_definitions(-g -O2)
endif()

# Find and include KissFFT
find_package(KissFFT REQUIRED)
include_directories(${KISSFFT_INCLUDE_DIR})

set(ASPLIB_SOURCES SpectrumVisProcessor/asplib_SpectrumVisProcessorConfigurator.cpp
                   SpectrumVisProcessor/asplib_SpectrumVisProcessor.cpp
                   SpectrumVisProcessor/Remapper/asplib_RemapperFactory.cpp
                   SpectrumVisProcessor/FFTSmoother/asplib_FFTSmootherFactory.cpp
                   SpectrumVisProcessor/AxisScaler/asplib_AxisScalerFactory.cpp
                   asplib_utils/base/extendedStructs/asplib_ExtendedStructContainer.cpp
                   asplib_utils/exceptions/asplib_StringException.cpp
                   asplib_utils/fft/asplib_FFTFactory.cpp
                   asplib_utils/fft/fftWindowing/fftWindowingFactory.cpp
                   asplib_utils/fft/fftWindowing/fftWindowFunctions.cpp)

file(GLOB_RECURSE ASPLIB_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h" "*.hpp")
				   
# set(ASPLIB_HEADERS asplib_utils/constants_typedefs/asplib_constants.h
                   # asplib_utils/constants_typedefs/asplib_typedefs.h
                   # asplib_utils/constants_typedefs/asplib_dataFmtDefines.hpp
                   # asplib_utils/os/asplib_base_os.h
                   # asplib_utils/os/windows/windows_definitions.h
                   # asplib_utils/os/linux/linux_definitions.h
                   # SpectrumVisProcessor/asplib_SpectrumVisProcessorConfigurator.hpp
                   # SpectrumVisProcessor/asplib_SpectrumVisProcessor.hpp
                   # SpectrumVisProcessor/Remapper/asplib_RemapperFactory.hpp
                   # SpectrumVisProcessor/Remapper/GammaCorrectorRemapper/asplib_TGammaCorrector.hpp
                   # SpectrumVisProcessor/Remapper/common/RemapperProducts.hpp
                   # SpectrumVisProcessor/FFTSmoother/asplib_FFTSmootherFactory.hpp
                   # SpectrumVisProcessor/FFTSmoother/dlbeerSmoother/asplib_TFeedbackSmoother.hpp
                   # SpectrumVisProcessor/FFTSmoother/dlbeerSmoother/asplib_TdlbeerSmoother.hpp
                   # SpectrumVisProcessor/FFTSmoother/dlbeerSmoother/asplib_dlbeerSmootherOptions.hpp
                   # SpectrumVisProcessor/FFTSmoother/common/FFTSmootherProducts.hpp
                   # SpectrumVisProcessor/AxisScaler/asplib_AxisScalerFactory.hpp
                   # SpectrumVisProcessor/AxisScaler/Log10Scaler/asplib_TLog10Scaler.hpp
                   # SpectrumVisProcessor/AxisScaler/common/AxisScalerProducts.hpp
                   # IIRFilter/TFirstOrderFeedback.hpp
                   # interfaces/asplib_IProcess.hpp
                   # interfaces/asplib_IFFT.h
                   # asplib_utils/base/autoFactory/asplib_TAutoFactory.hpp
                   # asplib_utils/base/autoFactory/asplib_TProductStatistics.hpp
                   # asplib_utils/base/extendedStructs/asplib_ExtendedStructContainer.hpp
                   # asplib_utils/base/extendedStructs/asplib_ExtendedStructs.hpp
                   # asplib_utils/buffers/asplib_bufferTypes.hpp
                   # asplib_utils/buffers/IBufferBase.h
                   # asplib_utils/buffers/ITBuffer.h
                   # asplib_utils/buffers/TBaseBuffer.h
                   # asplib_utils/buffers/TBaseFrameBuffer.h
                   # asplib_utils/buffers/TFrameBuffer.h
                   # asplib_utils/buffers/TFrameRingBuffer.h
                   # asplib_utils/buffers/TRingBuffer.h
                   # asplib_utils/exceptions/asplib_StringException.h
                   # asplib_utils/exceptions/asplib_IException.h
                   # asplib_utils/fft/asplib_FFTFactory.hpp
                   # asplib_utils/fft/KissFFT/asplib_TKissFFT.hpp
                   # asplib_utils/fft/KissFFT/asplib_TRKissFFT.hpp
                   # asplib_utils/fft/KissFFT/KissFFTProducts.hpp
                   # asplib_utils/fft/fftWindowing/fftWindowingFactory.h
                   # asplib_utils/fft/fftWindowing/fftWindowFunctions.h
                   # asplib_utils/fft/common/asplib_TZeroCpyConverter.hpp)

# Configure asplibConfig.cmake.in
set(ASPLIB_LIBRARY_DIR  "${CMAKE_INSTALL_PREFIX}/lib")
set(ASPLIB_INCLUDE_DIR  "${CMAKE_INSTALL_PREFIX}/include/asplib")
set(ASPLIB_LIBRARIES    "${KISSFFT_LIBRARY}")
set(ASPLIB_INCLUDE_DIRS "${KISSFFT_INCLUDE_DIR}")
configure_file(asplibConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/asplibConfig.cmake" @ONLY)

add_library(asplib STATIC ${ASPLIB_SOURCES} ${ASPLIB_HEADERS})
install(TARGETS asplib DESTINATION ${CMAKE_INSTALL_LIBDIR} EXPORT asplib)

# Install all header files
foreach ( HEADER ${ASPLIB_HEADERS} )
    get_filename_component( DIR ${HEADER} DIRECTORY )
    install( FILES ${HEADER} DESTINATION "${ASPLIB_INCLUDE_DIR}/${DIR}" )
endforeach()

# Install asplibConfig.cmake
if(WIN32 AND NOT CYGWIN)
    set(DEF_INSTALL_CMAKE_DIR asplib)
else()
    set(DEF_INSTALL_CMAKE_DIR lib/asplib)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")
install(FILES "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/asplibConfig.cmake" DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
