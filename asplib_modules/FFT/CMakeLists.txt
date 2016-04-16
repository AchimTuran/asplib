project(FFT)
find_package(KissFFT REQUIRED)

file (GLOB_RECURSE SOURCES
      "FFT/*.c"
      "FFT/*.cpp")

file (GLOB_RECURSE HEADERS
      "FFT/*.h"
      "FFT/*.hpp")

asplib_source_group("${SOURCES}")
asplib_source_group("${HEADERS}")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}::Core KissFFT::KissFFT)
target_include_directories(${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/FFT
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:include>
            $<INSTALL_INTERFACE:include/asplib>
)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "modules")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

# install all specific module files
asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/${CMAKE_PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
