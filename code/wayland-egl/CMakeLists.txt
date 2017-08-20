cmake_minimum_required(VERSION 3.0)
project(wayland-egl-demo)

add_definitions("-std=c++11")

# include custom find package modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# find packages
find_package(Wayland REQUIRED)
find_package(WaylandEGL REQUIRED)
find_package(EGL REQUIRED)
find_package(GLES REQUIRED)

# set source files, include dirs and libs
set(source_files src/main.cc
                 src/wayland_app.cc
                 src/wayland_surface.cc)

set(include_dirs ${WAYLAND_INCLUDE_DIRS}
                 ${WAYLAND_EGL_INCLUDE_DIRS}
                 ${EGL_INCLUDE_DIRS}
                 ${GLES_INCLUDE_DIRS}
                 ${CMAKE_SOURCE_DIR}/include)

set(libraries ${WAYLAND_LIBRARIES}
              ${WAYLAND_EGL_LIBRARIES}
              ${EGL_LIBRARIES}
              ${GLES_LIBRARIES})

# include directories
include_directories(${include_dirs})

# build project
add_executable(wayland-egl-demo ${source_files})
target_link_libraries(wayland-egl-demo ${libraries})
