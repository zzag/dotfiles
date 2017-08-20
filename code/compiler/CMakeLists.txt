cmake_minimum_required(VERSION 3.0)
project(tstc)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "LLVM version ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVMConfig.cmake: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})

add_definitions(${LLVM_DEFITIONS})
add_definitions(-std=c++11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(src)
add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(doc)
