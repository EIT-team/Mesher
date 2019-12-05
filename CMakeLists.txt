
cmake_minimum_required(VERSION 2.6.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-std=c++11)

enable_testing()

project( Mesher )
include(cmake/FindMATLAB.cmake)
include_directories("src/")

add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(test)
