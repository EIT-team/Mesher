
cmake_minimum_required(VERSION 2.6.2)

add_compile_options(-std=c++11)

set(CMAKE_BUILD_TYPE "Release")

enable_testing()

project( Mesher )
include(cmake/FindMATLAB.cmake)
include_directories("src/")

add_subdirectory(src)
add_subdirectory(test)
