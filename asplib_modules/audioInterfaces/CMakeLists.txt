#asplib_require_module(Core)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}
#                    ${asplib_Core_INCLUDE_DIRS})

#find_package(portaudio REQUIRED)

file (GLOB_RECURSE SOURCES
      "AudioInterfaces/*.c"
      "AudioInterfaces/*.cpp")

file (GLOB_RECURSE HEADERS
      "AudioInterfaces/*.h"
      "AudioInterfaces/*.hpp")
      
#asplib_source_group("${SOURCES}")
#asplib_source_group("${HEADERS}")
#add_library(asplib_AudioInterfaces STATIC ${SOURCES} ${HEADERS})

#set_property(TARGET asplib_AudioInterfaces PROPERTY FOLDER "modules")

# install all specific module files
#asplib_install_module("AudioInterfaces" "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
#install(TARGETS asplib_AudioInterfaces DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
