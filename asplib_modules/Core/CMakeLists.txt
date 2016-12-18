project(Core)

set(SOURCES asplib/Core/Exceptions/asplib_StringException.cpp
            asplib/Core/ExtendedStructs/asplib_ExtendedStructContainer.cpp
            asplib/Core/ExtendedStructs/asplib_ExtendedStructs.cpp)

set(HEADERS asplib/Core/Buffers/asplib_IBufferBase.h
            asplib/Core/Buffers/asplib_ITBuffer.h
            asplib/Core/Buffers/asplib_TBaseBuffer.h
            asplib/Core/Buffers/asplib_TBaseFrameBuffer.h
            asplib/Core/Buffers/asplib_TFrameBuffer.h
            asplib/Core/Buffers/asplib_TFrameRingBuffer.h
            asplib/Core/Buffers/asplib_TRingBuffer.h
            asplib/Core/Constants_Typedefs/asplib_Constants.h
            asplib/Core/Constants_Typedefs/asplib_Typedefs.h
            asplib/Core/Exceptions/asplib_IException.h
            asplib/Core/Exceptions/asplib_StringException.h
            asplib/Core/os/asplib_os.h
            asplib/Core/os/android/android_definitions.h
            asplib/Core/os/darwin_ios/darwin_ios_definitions.h
            asplib/Core/os/darwin_osx/darwin_osx_definitions.h
            asplib/Core/os/freebsd/freebsd_definitions.h
            asplib/Core/os/linux/linux_definitions.h
            asplib/Core/os/raspberry_pi/raspberry_pi_definitions.h
            asplib/Core/os/windows/windows_definitions.h
            asplib/Core/Strings/asplib_stdStringUtils.h

            asplib/Core/AutoFactory/asplib_TAutoFactory.hpp
            asplib/Core/AutoFactory/asplib_TProductStatistics.hpp
            asplib/Core/Buffers/asplib_BufferTypes.hpp
            asplib/Core/Constants_Typedefs/asplib_DataFmtDefines.hpp
            asplib/Core/Constants_Typedefs/asplib_ExtendedStructIDs.hpp
            asplib/Core/ExtendedStructs/asplib_ExtendedStructContainer.hpp
            asplib/Core/ExtendedStructs/asplib_ExtendedStructs.hpp
            
            asplib/Core/ComplexVector/asplib_TComplexVector.hpp
            asplib/Core/ComplexVector/asplib_ComplexVector_NativeFloat.hpp)

asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(asplib::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
MESSAGE(STATUS "---Core Include = ${CMAKE_CURRENT_SOURCE_DIR}/asplib")
target_include_directories(${PROJECT_NAME}
                           PRIVATE asplib
                           INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/asplib>
                                     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                     $<INSTALL_INTERFACE:include>
                                     $<INSTALL_INTERFACE:include/asplib>)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "asplib modules")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

# install all specific module files
asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include)
install(TARGETS ${PROJECT_NAME} EXPORT asplib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
