cmake_minimum_required( VERSION 2.8)
project( LCI)

if( CMAKE_COMPILER_IS_GNUCC)
	set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")
endif( CMAKE_COMPILER_IS_GNUCC)

if( CMAKE_COMPILER_IS_GNUCXX)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")
endif( CMAKE_COMPILER_IS_GNUCXX)

add_library( lcicore core.c util.c)
include_directories( "${LCI_SOURCE_DIR}")
link_directories( "${LCI_BINARY_DIR}")
add_executable( lci main.c)
target_link_libraries( lci lcicore)

add_subdirectory( googlemock)
add_executable( fake-lint-nt.exe fake-lint-nt.c)
add_executable( fake-flint fake-flint.c)
enable_testing()
include_directories(
	"${gtest_SOURCE_DIR}/include"
	"${gmock_SOURCE_DIR}/include"
	"${LCI_SOURCE_DIR}")
add_executable( unit_test test-core.cpp)
target_link_libraries( unit_test lcicore gmock_main)
add_test( unit_test unit_test)
