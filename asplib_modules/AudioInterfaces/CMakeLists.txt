project(AudioInterfaces)

#find_package(portaudio REQUIRED)

set(SOURCES AudioInterfaces/PortAudio/asplib_IPortAudio.cpp)

set(HEADERS AudioInterfaces/PortAudio/asplib_IPortAudio.h)
      
#add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
#add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
#target_include_directories(${PROJECT_NAME}
                           #PRIVATE ${PROJECT_NAME}
                           #INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                     #$<INSTALL_INTERFACE:include>
                                     #$<INSTALL_INTERFACE:include/${CMAKE_PROJECT_NAME}>)
#target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_PROJECT_NAME}::Core)

#set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "modules")
#set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME}_${PROJECT_NAME})

# install all specific module files
#asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/${CMAKE_PROJECT_NAME})
#install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
