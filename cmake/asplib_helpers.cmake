function(asplib_source_group FILE_LIST)
  foreach(_FILE ${FILE_LIST})
    if(IS_ABSOLUTE ${_FILE})
      get_filename_component(${_FILE} ${_FILE} ABSOLUTE)
      #file(RELATIVE_PATH relFile ${CMAKE_CURRENT_SOURCE_DIR} ${_FILE})
      get_filename_component(FILEDIRECTORY ${_FILE} DIRECTORY)
      #message(STATUS "FILEDIRECTORY=${FILEDIRECTORY}")
      file(RELATIVE_PATH FILEDIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} ${FILEDIRECTORY})
    else()
      #message(STATUS "FILEDIRECTORY=${FILEDIRECTORY}")
      get_filename_component(FILEDIRECTORY ${_FILE} DIRECTORY)
    endif()

    #message(STATUS "_FILE=${_FILE}")
    if(NOT "${FILEDIRECTORY}" STREQUAL "")
      string(REPLACE "/" "\\" FILE_FILEGROUP ${FILEDIRECTORY})
    else()
      set(FILE_FILEGROUP "")
    endif()
    get_filename_component(FILENAME ${_FILE} NAME)
    
    #message(STATUS "Filename: ${FILENAME}, Directory: ${FILEDIRECTORY}, Group: ${FILE_FILEGROUP}")
    
    source_group("${FILE_FILEGROUP}" FILES ${_FILE})
  endforeach()
endfunction(asplib_source_group)


function(asplib_install_with_folder FILE_LIST FOLDER)
  foreach(_FILE ${FILE_LIST})
    get_filename_component(HEADERDIR ${_FILE} DIRECTORY)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_FILE} DESTINATION ${FOLDER}/${HEADERDIR})
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
