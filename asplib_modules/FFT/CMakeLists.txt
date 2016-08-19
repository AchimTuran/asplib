project(FFT)

if(NOT KissFFT_SOURCE_DIR)
  find_package(KissFFT REQUIRED)
endif()

set(SOURCES asplib/FFT/asplib_FFTFactory.cpp
            asplib/FFT/FFTWindowing/asplib_FFTWindowFunctions.cpp
            asplib/FFT/FFTWindowing/asplib_FFTWindowingFactory.cpp)

set(HEADERS asplib/FFT/FFTWindowing/asplib_FFTWindowFunctions.h
            asplib/FFT/FFTWindowing/asplib_FFTWindowingFactory.h
            asplib/FFT/Interfaces/asplib_IFFT.h

            asplib/FFT/asplib_FFTFactory.hpp
            asplib/FFT/asplib_FFTOptions.hpp
            asplib/FFT/Common/asplib_TZeroCpyConverter.hpp
            asplib/FFT/KissFFT/asplib_KissFFTCpxOptions.hpp
            asplib/FFT/KissFFT/asplib_KissFFTProducts.hpp
            asplib/FFT/KissFFT/asplib_KissFFTRealOptions.hpp
            asplib/FFT/KissFFT/asplib_TKissFFT.hpp
            asplib/FFT/KissFFT/asplib_TRKissFFT.hpp)

asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(asplib::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
                           PRIVATE asplib
                           INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/asplib>
                                     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                     $<BUILD_INTERFACE:${KissFFT_SOURCE_DIR}>
                                     $<INSTALL_INTERFACE:include>
                                     $<INSTALL_INTERFACE:include/asplib>)
target_link_libraries(${PROJECT_NAME} PUBLIC asplib::Core KissFFT::KissFFT)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "asplib modules")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

# install all specific module files
asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include)
install(TARGETS ${PROJECT_NAME} EXPORT asplib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
