function(asplib_source_group FILE_LIST)
  foreach(_FILE ${FILE_LIST})
    get_filename_component(${_FILE} ${_FILE} ABSOLUTE)
    file(RELATIVE_PATH relFile ${CMAKE_CURRENT_SOURCE_DIR} ${_FILE})
    get_filename_component(FILEDIRECTORY ${_FILE} DIRECTORY)
    #message(STATUS "FILEDIRECTORY=${FILEDIRECTORY}")
    file(RELATIVE_PATH FILEDIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} ${FILEDIRECTORY})
    #message(STATUS "REL_FILEDIRECTORY=${FILEDIRECTORY}")
    

    #message(STATUS "_FILE=${_FILE}")
    string(REPLACE "/" "\\" FILE_FILEGROUP ${FILEDIRECTORY})
    get_filename_component(FILENAME ${_FILE} NAME)
    
    #message(STATUS "Filename: ${FILENAME}, Directory: ${FILEDIRECTORY}, Group: ${FILE_FILEGROUP}")
    
    source_group("${FILE_FILEGROUP}" FILES ${_FILE})
  endforeach()
endfunction(asplib_source_group)

function(asplib_install_with_folder FILE_LIST FOLDER)
  foreach(_FILE ${FILE_LIST})
    get_filename_component(HEADERDIR ${_FILE} DIRECTORY)
    install(FILES ${PROJECT_SOURCE_DIR}/${_FILE} DESTINATION ${FOLDER}/${HEADERDIR})
  endforeach()
endfunction()


macro(asplib_require_module Module)
  list(FIND ASPLIB_SUPPORTED_COMPONENTS "${Module}" _modFound)

  if(_modFound EQUAL -1)
    set(asplib_required_modules "${asplib_required_modules};${Module}" CACHE INTERNAL "Temporary variable to store needed module dependencies")
  endif()
endmacro()


macro(asplib_include_modules ModulesPath ModuleRequests)
  if(DEFINED ASPLIB_INITIAL_MODULES)
    message(FATAL_ERROR "asplib has already include modules! Calling asplib_include_modules twice is not allowed!")
  endif()
  
  get_filename_component(_modules_path "${ModulesPath}" ABSOLUTE)
  message(STATUS "Processing modules in ${_modules_path}")
  file(GLOB _asplib_modules RELATIVE "${_modules_path}" "${_modules_path}/*")

  if(NOT _asplib_modules)
    message(FATAL_ERROR "The path: ${_modules_path} does not contain any asplib modules!")
  endif()
  
  list(SORT _asplib_modules)  
  foreach(_requestedModule ${ModuleRequests})
    foreach(_asplibModule ${_asplib_modules})
      get_filename_component(_modpath "${_modules_path}/${_asplibModule}" ABSOLUTE)
      #message(STATUS "_requestedModule=${_requestedModule}  _asplibModule=${_asplibModule}  _modpath=${_modpath}")
      
      if(${_requestedModule} STREQUAL ${_asplibModule})
        if(EXISTS "${_modpath}/CMakeLists.txt")
          message(STATUS "Adding ${_requestedModule} module")
          add_subdirectory("${_modpath}")
        else()
          message(FATAL_ERROR "The asplib module: ${_requestedModule} was not found!")
        endif()
      endif()
    endforeach()    
  endforeach()
endmacro()
