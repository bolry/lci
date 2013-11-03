cmake_minimum_required (VERSION 2.8)
project(LCI)

if( CMAKE_COMPILER_IS_GNUCC)
	set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")
endif( CMAKE_COMPILER_IS_GNUCC)

if( CMAKE_COMPILER_IS_GNUCXX)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")
endif( CMAKE_COMPILER_IS_GNUCXX)

add_subdirectory(src)
add_subdirectory(googlemock)
add_subdirectory(test)
