CMAKE_MINIMUM_REQUIRED( VERSION 3.5 )

SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/ CACHE PATH "Single output directory for building all libraries." FORCE )
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/ CACHE PATH "Single output directory for building all executables." FORCE )

SET( DL_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dl/CMakeModules )
INCLUDE( ${DL_MODULE_PATH}/CMake_lib_macros.cmake )

# Process subdirectories
ADD_SUBDIRECTORY( dl )
