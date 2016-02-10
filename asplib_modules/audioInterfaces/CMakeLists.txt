find_package(portaudio REQUIRED)

file (GLOB_RECURSE ASPLIB_AUDIO_INTERFACES_SOURCES
      "AudioInterfaces/*.c"
      "AudioInterfaces/*.cpp")

file (GLOB_RECURSE ASPLIB_AUDIO_INTERFACES_HEADERS
      "AudioInterfaces/*.h"
      "AudioInterfaces/*.hpp")
      
#foreach(_file ${ASPLIB_AUDIO_INTERFACES_SOURCES})
#message(STATUS "file=${_file}")
#endforeach()

asplib_source_group("${ASPLIB_AUDIO_INTERFACES_SOURCES}")
asplib_source_group("${ASPLIB_AUDIO_INTERFACES_HEADERS}")
add_library(asplib_AudioInterfaces STATIC ${ASPLIB_AUDIO_INTERFACES_SOURCES} ${ASPLIB_AUDIO_INTERFACES_HEADERS})

set_property(TARGET asplib_AudioInterfaces PROPERTY FOLDER "modules")
