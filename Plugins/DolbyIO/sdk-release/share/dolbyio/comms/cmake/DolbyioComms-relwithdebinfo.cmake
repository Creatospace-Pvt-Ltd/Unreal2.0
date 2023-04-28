#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "DolbyioComms::media" for configuration "RelWithDebInfo"
set_property(TARGET DolbyioComms::media APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(DolbyioComms::media PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/dolbyio_comms_media.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/dolbyio_comms_media.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS DolbyioComms::media )
list(APPEND _IMPORT_CHECK_FILES_FOR_DolbyioComms::media "${_IMPORT_PREFIX}/lib/dolbyio_comms_media.lib" "${_IMPORT_PREFIX}/bin/dolbyio_comms_media.dll" )

# Import target "DolbyioComms::sdk" for configuration "RelWithDebInfo"
set_property(TARGET DolbyioComms::sdk APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(DolbyioComms::sdk PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/dolbyio_comms_sdk.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/dolbyio_comms_sdk.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS DolbyioComms::sdk )
list(APPEND _IMPORT_CHECK_FILES_FOR_DolbyioComms::sdk "${_IMPORT_PREFIX}/lib/dolbyio_comms_sdk.lib" "${_IMPORT_PREFIX}/bin/dolbyio_comms_sdk.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
