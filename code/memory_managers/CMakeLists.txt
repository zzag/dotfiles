cmake_minimum_required(VERSION 2.8)

project(MemoryManagers)

#
# Use C++11
#
if (UNIX)
        add_compile_options(-std=c++11)
endif(UNIX)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    # x64 platform
    add_definitions(-DMM_X86_64)
    message(STATUS "Found x64 platform!!!")
else (CMAKE_SIZEOF_VOID_P EQUAL 8)
    # x86 platform
    add_definitions(-DMM_X86)
    message(STATUS "Found x86 platform!!!")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

#
# Init include directories
#
include_directories(src)

#
# Sources
#
add_subdirectory(src)

#
# test
#
add_subdirectory(gtest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(src)
set(MEMORY_MAMANGER_LIB memory_manager)
enable_testing()
add_subdirectory(test)
