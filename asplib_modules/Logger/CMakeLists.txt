project(Logger)

set(SOURCES Logger/asplib_DefaultLogger.cpp
            Logger/ILogger.cpp
            Logger/LoggerFactory.cpp
            Logger/LoggerFile.cpp
            Logger/LoggerStdout.cpp
            Logger/Logger_types.cpp)

set(HEADERS Logger/asplib_DefaultLogger.h
            Logger/ILogger.h
            Logger/LoggerFactory.h
            Logger/LoggerFile.h
            Logger/LoggerStdout.h
            Logger/Logger_types.h)
      
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
asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS ${PROJECT_NAME} EXPORT asplib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
