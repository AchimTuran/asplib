project(Core)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# FIXME: Don't use globbing for source files
# https://cmake.org/cmake/help/latest/command/file.html?highlight=glob#file

set(SOURCES Core/Exceptions/asplib_StringException.cpp
            Core/ExtendedStructs/asplib_ExtendedStructContainer.cpp
            Core/ExtendedStructs/asplib_ExtendedStructs.cpp)

set(HEADERS Core/Buffers/asplib_IBufferBase.h
            Core/Buffers/asplib_ITBuffer.h
            Core/Buffers/asplib_TBaseBuffer.h
            Core/Buffers/asplib_TBaseFrameBuffer.h
            Core/Buffers/asplib_TFrameBuffer.h
            Core/Buffers/asplib_TFrameRingBuffer.h
            Core/Buffers/asplib_TRingBuffer.h
            Core/Constants_Typedefs/asplib_Constants.h
            Core/Constants_Typedefs/asplib_Typedefs.h
            Core/Exceptions/asplib_IException.h
            Core/Exceptions/asplib_StringException.h
            Core/os/asplib_os.h
            Core/os/android/android_definitions.h
            Core/os/darwin_ios/darwin_ios_definitions.h
            Core/os/darwin_osx/darwin_osx_definitions.h
            Core/os/freebsd/freebsd_definitions.h
            Core/os/linux/linux_definitions.h
            Core/os/raspberry_pi/raspberry_pi.h
            Core/os/windows/windows_definitions.h
            Core/Strings/asplib_stdStringUtils.h

            Core/AutoFactory/asplib_TAutoFactory.hpp
            Core/AutoFactory/asplib_TProductStatistics.hpp
            Core/Buffers/asplib_BufferTypes.hpp
            Core/Constants_Typedefs/asplib_DataFmtDefines.hpp
            Core/Constants_Typedefs/asplib_ExtendedStructIDs.hpp
            Core/ExtendedStructs/asplib_ExtendedStructContainer.hpp
            Core/ExtendedStructs/asplib_ExtendedStructs.hpp)

asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
                           PRIVATE ${PROJECT_NAME}
                           INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                     $<INSTALL_INTERFACE:include>
                                     $<INSTALL_INTERFACE:include/${CMAKE_PROJECT_NAME}>)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "modules")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME}_${PROJECT_NAME})

# install all specific module files
asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/${CMAKE_PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
