# CMakeLists for flowmapr

# Set a project name
project( opengeomorphometry )

# Program output name
set( APP_NAME flowmapr )

# Minimum CMake req
cmake_minimum_required( VERSION 2.8 )

# Set the module path - Extra cmake files
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR} )

# Disallow in-source building - It's messy
include( TestInSource.cmake )

# Debug option - others later maybe?
option( DEBUG "Debug mode" 0 )

# Did we set prefix?
if( PREFIX )
	string( REGEX REPLACE "^~" "$ENV{HOME}" PREFIX ${PREFIX} )
	get_filename_component( PREFIX_ABSOLUTE ${PREFIX} ABSOLUTE )
	set( CMAKE_INSTALL_PREFIX ${PREFIX} CACHE PATH "Install path prefix." FORCE )
else()
	set( PREFIX ${CMAKE_INSTALL_PREFIX} CACHE PATH "Install path prefix." )
endif()

# Mess with some rpath stuff for in-dir testing (may or may not do anything)
set( CMAKE_INSTALL_RPATH ${LIBS_DIR} )
set( CMAKE_ISNTALL_NAME_DIR ${LIBS_DIR} )

# Import some stuff, stuff is good, 'n stuff
# XXX:  Other [in-source] library imports here
include( CheckIncludeFile )
include( CheckCSourceCompiles )

# Prepare boost, static x64 won't work in linux
set( Boost_USE_STATIC_LIBS 0 )

# Search for some libraries we'll require
find_package( Boost REQUIRED COMPONENTS filesystem system thread )
find_package( Boost REQUIRED )

# A whole lot of unix gcc/g++ mess
set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb -W -Wall -Wno-long-long -Wno-unused" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=c++0x" )
set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -W -Wall -Wno-long-long -Wno-unused" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=c++0x" )

# This probably does nothing in Linux, I can't remember
if( DEBUG )
	set( CMAKE_BUILD_TYPE Debug )
else()
	set( CMAKE_BUILD_TYPE Release )
endif()

# Placeholder for some definitions (-DSOMEDEF=1, whatever) - #ifdef capable
set( DEFINITIONS
	${DEFINITIONS}
	LINUX
	BOOST_ALL_NO_LIB
)
set_directory_properties( PROPERTIES COMPILE_DEFINITIONS "${DEFINITIONS}" )

# Hmm.. It doesn't like this passed to file()
set( TOP_PROJ_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../files" )

# Directories that we include from, allowing #include "stuff.h"
include_directories(
	${CMAKE_INSTALL_PREFIX}/include
	${TOP_PROJ_DIR}/shared/include
	${TOP_PROJ_DIR}/shared/include/Utility
	${TOP_PROJ_DIR}/shared/include/DataFiles
	${TOP_PROJ_DIR}/flowmapr/include
	${Boost_INCLUDE_DIRS}
)

# File it up (Wow this won't take a variable)
file( GLOB_RECURSE EXECUTABLE_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	
#	../../files/flowmapr/source/DBFFile.cpp
	../../files/flowmapr/source/DEMProcess.cpp
#	../../files/flowmapr/source/InterNNManager.cpp
#	../../files/flowmapr/source/InterNNThread.cpp
#	../../files/flowmapr/source/LandMap.cpp
#	../../files/flowmapr/source/LandMapDLG.cpp
	../../files/flowmapr/source/LandmapManager.cpp
	../../files/flowmapr/source/LandmapThread.cpp
	../../files/flowmapr/source/main.cpp
#	../../files/flowmapr/source/ProgressBarDialog.cpp
#	../../files/flowmapr/source/SortD.cpp
#	../../files/flowmapr/source/STRArray.cpp
	../../files/flowmapr/source/ThreadManager.cpp
	../../files/flowmapr/source/VOLDFile.cpp
	
	# There are two of these...
#	../../files/shared/source/ThreadManager.cpp
	
	../../files/shared/source/DataFiles/CSVReader.cpp
	../../files/shared/source/DataFiles/CSVWriter.cpp
	../../files/shared/source/DataFiles/TXTReader.cpp
	../../files/shared/source/DataFiles/TXTWriter.cpp
	../../files/shared/source/Utility/Logger.cpp
	../../files/shared/source/Utility/Settings.cpp
	../../files/shared/source/Utility/Sort.cpp
	../../files/shared/source/Utility/Util.cpp
)

# Some custom targets (make run, make debug, trickery)
ADD_CUSTOM_TARGET( run COMMAND ./flowmapr DEPENDS ${APP_NAME} )
ADD_CUSTOM_TARGET( debug COMMAND gdb flowmapr DEPENDS ${APP_NAME} )

# Add the binary
add_executable( ${APP_NAME} ${EXECUTABLE_SRCS} )

# Link it up
target_link_libraries( ${APP_NAME}
	${Boost_LIBRARIES}
)

# Set the install up
install( TARGETS ${APP_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

# Spew some blah
message( "
flowmapr configured.
Maybe.

Debug mode:	${DEBUG}
Install dir:	${CMAKE_INSTALL_PREFIX}
" )
