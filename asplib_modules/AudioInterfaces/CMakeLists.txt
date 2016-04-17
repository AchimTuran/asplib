project(AudioInterfaces)

#asplib_require_module(Core)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}
#                    ${asplib_Core_INCLUDE_DIRS})

#find_package(portaudio REQUIRED)

set(SOURCES AudioInterfaces/PortAudio/asplib_IPortAudio.cpp)

set(HEADERS AudioInterfaces/PortAudio/asplib_IPortAudio.h)
      
#asplib_source_group("${SOURCES}")
#asplib_source_group("${HEADERS}")
#add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

#set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "modules")
#set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

# install all specific module files
#asplib_install_module("${PROJECT_NAME}" "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
#install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
