project(Logger)

set(SOURCES asplib/Logger/asplib_DefaultLogger.cpp
            asplib/Logger/ILogger.cpp
            asplib/Logger/LoggerFactory.cpp
            asplib/Logger/LoggerFile.cpp
            asplib/Logger/LoggerStdout.cpp
            asplib/Logger/Logger_types.cpp)

set(HEADERS asplib/Logger/asplib_DefaultLogger.h
            asplib/Logger/ILogger.h
            asplib/Logger/LoggerFactory.h
            asplib/Logger/LoggerFile.h
            asplib/Logger/LoggerStdout.h
            asplib/Logger/Logger_types.h)
      
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

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "modules")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

# install all specific module files
asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include)
install(TARGETS ${PROJECT_NAME} EXPORT asplib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
