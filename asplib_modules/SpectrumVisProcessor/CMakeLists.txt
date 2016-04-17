project(SpectrumVisProcessor)

set(SOURCES SpectrumVisProcessor/asplib_SpectrumVisProcessor.cpp
            SpectrumVisProcessor/asplib_SpectrumVisProcessorConfigurator.cpp
            SpectrumVisProcessor/AxisScaler/asplib_AxisScalerFactory.cpp
            SpectrumVisProcessor/FFTSmoother/asplib_FFTSmootherFactory.cpp
            SpectrumVisProcessor/Interfaces/asplib_ProcessFactory.cpp
            SpectrumVisProcessor/Remapper/asplib_RemapperFactory.cpp
            SpectrumVisProcessor/SpectrumCalculation/asplib_SpectrumCalculationFactory.cpp)

set(HEADERS SpectrumVisProcessor/asplib_SpectrumVisProcessor.hpp
            SpectrumVisProcessor/asplib_SpectrumVisProcessorConfigurator.hpp
            SpectrumVisProcessor/AxisScaler/asplib_AxisScalerFactory.hpp
            SpectrumVisProcessor/AxisScaler/asplib_AxisScalerOptions.hpp
            SpectrumVisProcessor/AxisScaler/Common/asplib_AxisScalerProducts.hpp
            SpectrumVisProcessor/AxisScaler/Log10Scaler/asplib_TLog10Scaler.hpp
            SpectrumVisProcessor/AxisScaler/Log10Scaler/asplib_TLog10ScalerOptions.hpp
            SpectrumVisProcessor/FFTSmoother/asplib_FFTSmootherFactory.hpp
            SpectrumVisProcessor/FFTSmoother/asplib_FFTSmootherOptions.hpp
            SpectrumVisProcessor/FFTSmoother/Common/asplib_FFTSmootherProducts.hpp
            SpectrumVisProcessor/FFTSmoother/dlbeerSmoother/asplib_dlbeerSmootherOptions.hpp
            SpectrumVisProcessor/FFTSmoother/dlbeerSmoother/asplib_TdlbeerSmoother.hpp
            SpectrumVisProcessor/FFTSmoother/dlbeerSmoother/asplib_TFeedbackSmoother.hpp
            SpectrumVisProcessor/Interfaces/asplib_IProcess.hpp
            SpectrumVisProcessor/Interfaces/asplib_ProcessFactory.hpp
            SpectrumVisProcessor/Remapper/asplib_RemapperFactory.hpp
            SpectrumVisProcessor/Remapper/asplib_RemapperOptions.hpp
            SpectrumVisProcessor/Remapper/Common/asplib_RemapperProducts.hpp
            SpectrumVisProcessor/Remapper/GammaCorrectorRemapper/asplib_TGammaCorrector.hpp
            SpectrumVisProcessor/Remapper/GammaCorrectorRemapper/asplib_TGammaCorrectorOptions.hpp
            SpectrumVisProcessor/SpectrumCalculation/asplib_SpectrumCalculationFactory.hpp
            SpectrumVisProcessor/SpectrumCalculation/Common/asplib_SpectrumCalculationProducts.hpp
            SpectrumVisProcessor/SpectrumCalculation/SquareRootSpectrumCalculation/asplib_TSquareRoot.hpp
            SpectrumVisProcessor/SpectrumCalculation/SquareSpectrumCalculation/asplib_TSquare.hpp)
      
asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
                           PRIVATE ${PROJECT_NAME}
                           INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                     $<INSTALL_INTERFACE:include>
                                     $<INSTALL_INTERFACE:include/asplib>)
target_link_libraries(${PROJECT_NAME} PRIVATE asplib::Core asplib::FFT)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "modules")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

add_subdirectory(tests)

# install all specific module files
asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/${CMAKE_PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
