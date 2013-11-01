enable_testing()
include_directories(
	"${gtest_SOURCE_DIR}/include"
	"${gmock_SOURCE_DIR}/include"
	"${lci_SOURCE_DIR}/src")
add_executable(unit_test core_test.cpp)
target_link_libraries(unit_test lcicore gmock_main)
add_test(unit_test unit_test)
