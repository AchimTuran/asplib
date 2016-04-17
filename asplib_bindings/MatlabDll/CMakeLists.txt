project(MatlabDll)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(HEADERS asplib_MatlabDll.h
            typedefs.h)

set(SOURCES asplib_MatlabDll.cpp)

set(MATLAB_SCRIPTS  asplib_createBiquad.m
                    asplib_init.m
                    asplib_load_MatlabDll.m
                    asplib_processBiquads.m
                    asplib_runBiquadTest.m
                    asplib_setBiquadGain.m
                    asplib_setBiquadGains.m
                    asplib_unload_MatlabDll.m)

asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                           INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                     $<INSTALL_INTERFACE:include>
                                     $<INSTALL_INTERFACE:include/asplib/bindings>
                                     $<INSTALL_INTERFACE:include/asplib/bindings/MatlabDll>)

target_link_libraries(${PROJECT_NAME} PRIVATE asplib::Biquad)
 
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "bindings")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})
 
asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/${CMAKE_PROJECT_NAME}/bindings/MatlabDll)
asplib_install_module(${PROJECT_NAME} "${MATLAB_SCRIPTS}" ${CMAKE_INSTALL_PREFIX}/include/${CMAKE_PROJECT_NAME}/bindings/MatlabDll)
install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/asplib/bindings/MatlabDll)
