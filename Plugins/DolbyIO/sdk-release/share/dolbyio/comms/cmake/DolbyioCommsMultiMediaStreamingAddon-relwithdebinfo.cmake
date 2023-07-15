#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "DolbyioComms::multimedia_streaming_addon" for configuration "RelWithDebInfo"
set_property(TARGET DolbyioComms::multimedia_streaming_addon APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(DolbyioComms::multimedia_streaming_addon PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/dolbyio_comms_multimedia_streaming_addon.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/dolbyio_comms_multimedia_streaming_addon.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS DolbyioComms::multimedia_streaming_addon )
list(APPEND _IMPORT_CHECK_FILES_FOR_DolbyioComms::multimedia_streaming_addon "${_IMPORT_PREFIX}/lib/dolbyio_comms_multimedia_streaming_addon.lib" "${_IMPORT_PREFIX}/bin/dolbyio_comms_multimedia_streaming_addon.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
