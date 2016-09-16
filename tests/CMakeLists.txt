# Set project name
SET( PROJECT_NAME tests )
PROJECT( ${PROJECT_NAME} )

# Find core dependencies
FIND_PACKAGE( OpenCV REQUIRED )

# Set project source files
SET( PROJECT_SOURCES
    tests.cpp
    )

# Configure project paths
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/dl/include/
	${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    )
LINK_DIRECTORIES( ${CMAKE_BINARY_DIR}/bin )

# Configure project output
ADD_EXECUTABLE( ${PROJECT_NAME} 
    ${PROJECT_SOURCES}
    )

# Link project dependencies
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} 
    debug dl_d optimized dl
	${OpenCV_LIBS}
    )
