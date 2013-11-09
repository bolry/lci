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
enable_testing()
include_directories(
	"${gtest_SOURCE_DIR}/include"
	"${gmock_SOURCE_DIR}/include"
	"${LCI_SOURCE_DIR}/src")
add_executable(unit_test test-core.cpp)
target_link_libraries(unit_test lcicore gmock_main)
add_test(unit_test unit_test)
