# Lint compiler interceptor
# Copyright (C) 2013 Bo Rydberg
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required( VERSION 2.8)
project( LCI)

if( CMAKE_COMPILER_IS_GNUCC)
	set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")
endif( CMAKE_COMPILER_IS_GNUCC)

if( CMAKE_COMPILER_IS_GNUCXX)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")
endif( CMAKE_COMPILER_IS_GNUCXX)

add_library( core core.c util.c)
include_directories( "${LCI_SOURCE_DIR}")
link_directories( "${LCI_BINARY_DIR}")
add_executable( lci main.c)
target_link_libraries( lci core)

add_subdirectory( googlemock)
add_executable( fake-lint-nt.exe fake-lint-nt.c)
add_executable( fake-flint fake-flint.c)
enable_testing()
include_directories(
	"${gtest_SOURCE_DIR}/include"
	"${gmock_SOURCE_DIR}/include"
	"${LCI_SOURCE_DIR}")
add_executable( unit_test test-core.cpp)
target_link_libraries( unit_test core gmock_main)
add_test( unit_test unit_test)
