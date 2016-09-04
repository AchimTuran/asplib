project(Dynamics)

set(SOURCES asplib/Dynamics/asplib_DynamicsFactory.cpp
            asplib/Dynamics/Compressor/asplib_DynamicsCompressor.cpp)

set(HEADERS asplib/Dynamics/Compressor/asplib_DynamicsCompressor.hpp
            asplib/Dynamics/Compressor/asplib_DynamicsCompressorOptions.hpp
            asplib/Dynamics/Common/asplib_DynamicsProducts.hpp
            asplib/Dynamics/Interfaces/asplib_IDynamics.hpp
            asplib/Dynamics/asplib_DynamicsFactory.hpp
            asplib/Dynamics/asplib_DynamicsFactory.hpp
            asplib/Dynamics/asplib_DynamicsOptions.hpp)
      
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
asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include)
install(TARGETS ${PROJECT_NAME} EXPORT asplib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
