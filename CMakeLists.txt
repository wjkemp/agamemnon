#	Copyright (c) 2017 Willem Kemp
#
#	Permission is hereby granted, free of charge, to any person obtaining a copy
#	of this software and associated documentation files (the "Software"), to deal
#	in the Software without restriction, including without limitation the rights
#	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#	copies of the Software, and to permit persons to whom the Software is
#	furnished to do so, subject to the following conditions:
#
#	The above copyright notice and this permission notice shall be included in all
#	copies or substantial portions of the Software.
#
#	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#	SOFTWARE.

project(agamemnon)
cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")

set (
	SRC_COMMON
	src/commands.cpp
	src/expressions.cpp
	src/logging.cpp
	src/main.cpp
	src/script_context.cpp
	src/script_exception.cpp
	src/textutils.cpp
	src/variable.cpp
)


include_directories(src)

add_executable(agamemnon ${SRC_COMMON}  src/memory_region.cpp)
target_link_libraries(agamemnon PRIVATE jsoncpp)

add_executable(agamemnon_test ${SRC_COMMON} src/memory_region_stub.cpp)
target_link_libraries(agamemnon_test PRIVATE jsoncpp)
