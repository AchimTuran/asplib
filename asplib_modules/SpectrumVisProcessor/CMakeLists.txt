project(SpectrumVisProcessor)

set(SOURCES asplib/SpectrumVisProcessor/asplib_SpectrumVisProcessor.cpp
            asplib/SpectrumVisProcessor/asplib_SpectrumVisProcessorConfigurator.cpp
            asplib/SpectrumVisProcessor/AxisScaler/asplib_AxisScalerFactory.cpp
            asplib/SpectrumVisProcessor/FFTSmoother/asplib_FFTSmootherFactory.cpp
            asplib/SpectrumVisProcessor/Interfaces/asplib_ProcessFactory.cpp
            asplib/SpectrumVisProcessor/Remapper/asplib_RemapperFactory.cpp
            asplib/SpectrumVisProcessor/SpectrumCalculation/asplib_SpectrumCalculationFactory.cpp)

set(HEADERS asplib/SpectrumVisProcessor/asplib_SpectrumVisProcessor.hpp
            asplib/SpectrumVisProcessor/asplib_SpectrumVisProcessorConfigurator.hpp
            asplib/SpectrumVisProcessor/AxisScaler/asplib_AxisScalerFactory.hpp
            asplib/SpectrumVisProcessor/AxisScaler/asplib_AxisScalerOptions.hpp
            asplib/SpectrumVisProcessor/AxisScaler/Common/asplib_AxisScalerProducts.hpp
            asplib/SpectrumVisProcessor/AxisScaler/Log10Scaler/asplib_TLog10Scaler.hpp
            asplib/SpectrumVisProcessor/AxisScaler/Log10Scaler/asplib_TLog10ScalerOptions.hpp
            asplib/SpectrumVisProcessor/FFTSmoother/asplib_FFTSmootherFactory.hpp
            asplib/SpectrumVisProcessor/FFTSmoother/asplib_FFTSmootherOptions.hpp
            asplib/SpectrumVisProcessor/FFTSmoother/Common/asplib_FFTSmootherProducts.hpp
            asplib/SpectrumVisProcessor/FFTSmoother/dlbeerSmoother/asplib_dlbeerSmootherOptions.hpp
            asplib/SpectrumVisProcessor/FFTSmoother/dlbeerSmoother/asplib_TdlbeerSmoother.hpp
            asplib/SpectrumVisProcessor/FFTSmoother/dlbeerSmoother/asplib_TFeedbackSmoother.hpp
            asplib/SpectrumVisProcessor/FFTSmoother/AttackReleaseSmoother/asplib_TAttackReleaseSmootherOptions.hpp
            asplib/SpectrumVisProcessor/FFTSmoother/AttackReleaseSmoother/asplib_TAttackReleaseSmoother.hpp
            asplib/SpectrumVisProcessor/FFTSmoother/AttackReleaseSmoother/asplib_TAttackReleaseFilter.hpp
            asplib/SpectrumVisProcessor/Interfaces/asplib_IProcess.hpp
            asplib/SpectrumVisProcessor/Interfaces/asplib_ProcessFactory.hpp
            asplib/SpectrumVisProcessor/Remapper/asplib_RemapperFactory.hpp
            asplib/SpectrumVisProcessor/Remapper/asplib_RemapperOptions.hpp
            asplib/SpectrumVisProcessor/Remapper/Common/asplib_RemapperProducts.hpp
            asplib/SpectrumVisProcessor/Remapper/GammaCorrectorRemapper/asplib_TGammaCorrector.hpp
            asplib/SpectrumVisProcessor/Remapper/GammaCorrectorRemapper/asplib_TGammaCorrectorOptions.hpp
            asplib/SpectrumVisProcessor/SpectrumCalculation/asplib_SpectrumCalculationFactory.hpp
            asplib/SpectrumVisProcessor/SpectrumCalculation/Common/asplib_SpectrumCalculationProducts.hpp
            asplib/SpectrumVisProcessor/SpectrumCalculation/SquareRootSpectrumCalculation/asplib_TSquareRoot.hpp
            asplib/SpectrumVisProcessor/SpectrumCalculation/SquareSpectrumCalculation/asplib_TSquare.hpp)
      
asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(asplib::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
                           PRIVATE asplib
                           INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/asplib>
                                     $<INSTALL_INTERFACE:include>
                                     $<INSTALL_INTERFACE:include/asplib>)
target_link_libraries(${PROJECT_NAME} PUBLIC asplib::FFT)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "modules")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

# install all specific module files
asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include)
install(TARGETS ${PROJECT_NAME} EXPORT asplib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
