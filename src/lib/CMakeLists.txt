add_library(lcicore core.c util.c)
include_directories("${LCI_SOURCE_DIR}/src")
link_directories("${LCI_BINARY_DIR}")
