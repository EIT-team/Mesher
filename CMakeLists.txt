cmake_minimum_required(VERSION 2.6.2)
project( Mesher )
include(cmake/FindMATLAB.cmake)
include_directories("src/")

add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(test)
