project(Timer)

set(SOURCES Timer/asplib_CPUTimer.cpp)

set(HEADERS Timer/asplib_CPUTimer.h)
      
asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(asplib::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
                           PRIVATE ${PROJECT_NAME}
                           INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                     $<INSTALL_INTERFACE:include>
                                     $<INSTALL_INTERFACE:include/asplib>)
target_link_libraries(${PROJECT_NAME} PRIVATE asplib::Core)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "modules")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

# install all specific module files
asplib_install_module("${PROJECT_NAME}" "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
