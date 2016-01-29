function(asplib_source_group FILE_LIST)
  foreach(_FILE ${FILE_LIST})
    get_filename_component(FILEDIRECTORY ${_FILE} DIRECTORY)
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

