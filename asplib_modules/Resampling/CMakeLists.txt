project(Resampling)

set(SOURCES asplib/Resampling/asplib_ResamplingFactory.cpp
            asplib/Resampling/Decimator/asplib_Decimator.cpp)

set(HEADERS asplib/Resampling/Decimator/asplib_Decimator.hpp
            asplib/Resampling/Decimator/asplib_DecimatorOptions.hpp
            asplib/Resampling/Common/asplib_ResamplingProducts.hpp
            asplib/Resampling/Interfaces/asplib_IResampling.hpp
            asplib/Resampling/asplib_ResamplingFactory.hpp
            asplib/Resampling/asplib_ResamplingFactory.hpp
            asplib/Resampling/asplib_ResamplingOptions.hpp)
      
asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(asplib::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
                           PRIVATE asplib
                           INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/asplib>
                                     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                     $<INSTALL_INTERFACE:include>
                                     $<INSTALL_INTERFACE:include/asplib>)
target_link_libraries(${PROJECT_NAME} PRIVATE asplib::Core)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "asplib modules")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

# install all specific module files
if (NOT ASPLIB_DISABLE_PACKAGING)
  asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include)
  install(TARGETS ${PROJECT_NAME} EXPORT asplib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif()
