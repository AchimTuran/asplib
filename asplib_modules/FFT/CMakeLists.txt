project(FFT)

find_package(KissFFT REQUIRED)

set(SOURCES FFT/asplib_FFTFactory.cpp
            FFT/FFTWindowing/asplib_FFTWindowFunctions.cpp
            FFT/FFTWindowing/asplib_FFTWindowingFactory.cpp)

set(HEADERS FFT/FFTWindowing/asplib_FFTWindowFunctions.h
            FFT/FFTWindowing/asplib_FFTWindowingFactory.h
            FFT/Interfaces/asplib_IFFT.h

            FFT/asplib_FFTFactory.hpp
            FFT/asplib_FFTOptions.hpp
            FFT/Common/asplib_TZeroCpyConverter.hpp
            FFT/KissFFT/asplib_KissFFTCpxOptions.hpp
            FFT/KissFFT/asplib_KissFFTProducts.hpp
            FFT/KissFFT/asplib_KissFFTRealOptions.hpp
            FFT/KissFFT/asplib_TKissFFT.hpp
            FFT/KissFFT/asplib_TRKissFFT.hpp)

asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}::Core KissFFT::KissFFT)
target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}
                           INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                     $<INSTALL_INTERFACE:include>
                                     $<INSTALL_INTERFACE:include/asplib>)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "modules")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

# install all specific module files
asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/${CMAKE_PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
