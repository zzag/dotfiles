cmake_minimum_required(VERSION 2.8)
project(rendering-demo)


# enable C++11
add_compile_options(-std=c++11)

# use custom find modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# find wayland
find_package(Wayland REQUIRED)

include_directories(${WAYLAND_INCLUDE_DIRS})
set(LIBS ${WAYLAND_LIBRARIES})

add_executable(rendering-demo main.cc app.cc surface.cc)
target_link_libraries(rendering-demo ${LIBS})

