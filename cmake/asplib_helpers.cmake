function(asplib_source_group FILE_LIST)
  foreach(_FILE ${FILE_LIST})
    get_filename_component(${_FILE} ${_FILE} ABSOLUTE)
    file(RELATIVE_PATH relFile ${CMAKE_CURRENT_SOURCE_DIR} ${_FILE})
    get_filename_component(FILEDIRECTORY ${_FILE} DIRECTORY)
    #message(STATUS "FILEDIRECTORY=${FILEDIRECTORY}")
    file(RELATIVE_PATH FILEDIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} ${FILEDIRECTORY})  

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
    install(FILES ${CMAKE_SOURCE_DIR}/${_FILE} DESTINATION ${FOLDER}/${HEADERDIR})
  endforeach()
endfunction()


function(asplib_install_module MODULE_NAME FILE_LIST FOLDER)
  foreach(_FILE ${FILE_LIST})
    # remove absolute path to this module
    string(REPLACE "${CMAKE_SOURCE_DIR}/asplib_modules/${MODULE_NAME}/" "" _FILE "${_FILE}")
    get_filename_component(HEADERDIR ${_FILE} DIRECTORY)
        
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_FILE} DESTINATION ${FOLDER}/${HEADERDIR})
  endforeach()
endfunction()


#macro(asplib_require_module Module)
#  list(FIND ASPLIB_SUPPORTED_COMPONENTS "${Module}" _modFound)

#  if(_modFound EQUAL -1)
#    set(asplib_required_modules "${asplib_required_modules};${Module}" CACHE INTERNAL "Temporary variable to store needed module dependencies")
#  endif()
#endmacro()


macro(asplib_include_modules ModulesPath ModuleRequests) 
  get_filename_component(_modules_path "${ModulesPath}" ABSOLUTE)
  message(STATUS "Processing modules in ${_modules_path}")

  foreach(_requestedModule ${ModuleRequests})
    get_filename_component(_absModPath "${_modules_path}/${_requestedModule}" ABSOLUTE)
      
    list(FIND ASPLIB_SUPPORTED_COMPONENTS "${_requestedModule}" _modFound)
      
    # only process modules that are not registered in the CACHE variable
    if(_modFound EQUAL -1)   
      if(EXISTS "${_absModPath}/CMakeLists.txt")
        message(STATUS "Adding ${_requestedModule} module")
        add_subdirectory("${_absModPath}")
      else()
        message(FATAL_ERROR "The asplib module: ${_requestedModule} was not found!")
      endif()

      foreach(_requiredModule ${asplib_required_modules})
        get_filename_component(_absModPath "${_modules_path}/${_requiredModule}" ABSOLUTE)
        if(EXISTS "${_absModPath}/CMakeLists.txt")
          message(STATUS "Adding ${_requiredModule} module")
          add_subdirectory("${_absModPath}")
        else()
          message(FATAL_ERROR "The asplib module: ${_requestedModule} was not found!")
        endif()
      endforeach()
      
      # reset required modules CACHE variable for the next loop iteration
      set(asplib_required_modules "" CACHE INTERNAL "Temporary variable to store needed module dependencies")    
    endif()
  endforeach()
endmacro()
