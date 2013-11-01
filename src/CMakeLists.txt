add_subdirectory(lib)
add_executable(lci main.c)
target_link_libraries(lci lcicore)
