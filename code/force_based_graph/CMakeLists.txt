cmake_minimum_required(VERSION 3.0)
project(lab)

add_definitions(-std=c++11)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(lib)

add_subdirectory(app)