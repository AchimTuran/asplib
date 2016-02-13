if(NOT ASPLIB_MODULES_PATH)
  set(ASPLIB_MODULES_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

message(STATUS "ASPLIB_MODULES_TO_BUILD=${ASPLIB_MODULES_TO_BUILD}")

message(STATUS "---------Processing asplib modules----------")

set(ASPLIB_SUPPORTED_COMPONENTS "" CACHE INTERNAL "supported asplib components")
set(asplib_required_modules "" CACHE INTERNAL "Temporary variable to store needed module dependencies")

if("${ASPLIB_MODULES_TO_BUILD}" STREQUAL "all" OR "${ASPLIB_MODULES_TO_BUILD}" STREQUAL "")
  # for better module handling convert ASPLIB_MODULES_PATH to a absolute path
  get_filename_component(_modules_path "${ASPLIB_MODULES_PATH}" ABSOLUTE)
  
  # search for possible module folders
  file(GLOB _asplib_modules ABSOLUTE "${_modules_path}" "${_modules_path}/*")
  foreach(_mod ${_asplib_modules})
    if(EXISTS "${_mod}/CMakeLists.txt")
      # only add modules folders, which include a CMakeLists.txt and add them to ASPLIB_MODULES
      get_filename_component(_module_name ${_mod} NAME)

      list(APPEND ASPLIB_MODULES ${_module_name})
    endif()
  endforeach()
else()
  set(ASPLIB_MODULES "${ASPLIB_MODULES_TO_BUILD}")
endif()

asplib_include_modules(${ASPLIB_MODULES_PATH} "${ASPLIB_MODULES}")

message(STATUS "")
message(STATUS "Available asplib modules:")
foreach(_mod ${ASPLIB_SUPPORTED_COMPONENTS})
  message(STATUS "  ${_mod}")
endforeach()
message(STATUS "")