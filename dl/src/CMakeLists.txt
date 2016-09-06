# Set project name
SET( PROJECT_NAME dl )
PROJECT( ${PROJECT_NAME} )

# Find core dependencies
INCLUDE_PACKAGE_CUDA()
INCLUDE_PACKAGE_CVMLAUX()

# Set project source files
FILE( GLOB PROJECT_SRCS
    ${CMAKE_SOURCE_DIR}/dl/include/dl.h
    *.h
	*.cpp
)

# Configure project paths
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/dl/include/ )

# Configure project output
SET( CMAKE_DEBUG_POSTFIX _d )
ADD_LIBRARY( ${PROJECT_NAME} SHARED ${PROJECT_SRCS} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} LINK_PRIVATE ${TARGET_LINK_LIBS} )
