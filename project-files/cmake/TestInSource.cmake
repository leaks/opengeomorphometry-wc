# Disallow in-source building
string( COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" FLM_IN_SRC )

# Now do the test
if( L3D_IN_SRC )
	message( FATAL_ERROR
		"flowmapr must be built out of source.  Please remove the CMakeCache.txt, then create a separate build directory and run cmake from there.  i.e. mkdir build; cd build; cmake ..;"
	)
endif()
