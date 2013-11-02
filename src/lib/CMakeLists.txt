add_library(lcicore core.c util.c)
include_directories("${lci_SOURCE_DIR}/src")
link_directories("${lci_BINARY_DIR}")
