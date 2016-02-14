asplib_require_module(Core)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${asplib_Core_INCLUDE_DIRS})

#find_package(portaudio REQUIRED)

file (GLOB_RECURSE ASPLIB_AUDIO_INTERFACES_SOURCES
      "AudioInterfaces/*.c"
      "AudioInterfaces/*.cpp")

file (GLOB_RECURSE ASPLIB_AUDIO_INTERFACES_HEADERS
      "AudioInterfaces/*.h"
      "AudioInterfaces/*.hpp")
      
#asplib_source_group("${ASPLIB_AUDIO_INTERFACES_SOURCES}")
#asplib_source_group("${ASPLIB_AUDIO_INTERFACES_HEADERS}")
#add_library(asplib_AudioInterfaces STATIC ${ASPLIB_AUDIO_INTERFACES_SOURCES} ${ASPLIB_AUDIO_INTERFACES_HEADERS})

#set_property(TARGET asplib_AudioInterfaces PROPERTY FOLDER "modules")

#set(ASPLIB_SUPPORTED_COMPONENTS "${ASPLIB_SUPPORTED_COMPONENTS};AudioInterfaces" CACHE INTERNAL "abc")

# install all specific module files
#asplib_install_module("AudioInterfaces" "${ASPLIB_AUDIO_INTERFACES_HEADERS}" ${CMAKE_INSTALL_PREFIX}/include/asplib)
#install(TARGETS asplib_AudioInterfaces DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
