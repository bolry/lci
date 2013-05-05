cmake_minimum_required (VERSION 2.8)
project(LCI)
add_library (lcicore core.c)
include_directories (${LCI_SOURCE_DIR})
link_directories (${LCI_BINARY_DIR})
add_executable (lci main.c)
target_link_libraries (lci lcicore)
add_subdirectory(googlemock)
enable_testing()
include_directories(
	${gtest_SOURCE_DIR}/include
	${gtest_SOURCE_DIR}
	${gmock_SOURCE_DIR}/include
	${gmock_SOURCE_DIR})
add_executable(unit_test core_test.cpp)
target_link_libraries(unit_test gmock_main lcicore)
add_test(unit_test unit_test)
