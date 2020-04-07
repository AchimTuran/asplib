project(AudioInterfaces)

set(SOURCES asplib/AudioInterfaces/PortAudio/asplib_IPortAudio.cpp)

set(HEADERS asplib/AudioInterfaces/PortAudio/asplib_IPortAudio.h)

asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")
      
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(asplib::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
                           PRIVATE asplib 
                           PUBLIC ${portaudio_INCLUDE_DIRS}
                           INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/asplib>
                                     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                     $<INSTALL_INTERFACE:include>
                                     $<INSTALL_INTERFACE:include/asplib>)
target_link_libraries(${PROJECT_NAME} PUBLIC asplib::Core ${portaudio_LIBRARY})

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "asplib modules")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

# install all specific module files
if (NOT ASPLIB_DISABLE_PACKAGING)
  asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include)
  install(TARGETS ${PROJECT_NAME} EXPORT asplib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif()
