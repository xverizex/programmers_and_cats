cmake_minimum_required (VERSION 3.22)

set (CMAKE_CXX_COMPILER gcc)

project (office VERSION 1.0)

add_subdirectory (src)

file (COPY assets DESTINATION ${CMAKE_BINARY_DIR})
