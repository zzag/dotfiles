find_package(PkgConfig)
pkg_check_modules(PC_WAYLAND QUIET wayland-egl)

find_path(WAYLAND_EGL_INCLUDE_DIRS wayland-egl.h
          HINTS ${PC_WAYLAND_EGL_INCLUDEDIR} ${PC_WAYLAND_EGL_INCLUDE_DIRS})

find_library(WAYLAND_EGL_LIBRARIES wayland-egl
          HINTS ${PC_WAYLAND_EGL_LIBDIR} ${PC_WAYLAND_EGL_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WaylandEGL DEFAULT_MSG
          WAYLAND_EGL_LIBRARIES WAYLAND_EGL_INCLUDE_DIRS)
mark_as_advanced(WAYLAND_EGL_INCLUDE_DIRS WAYLAND_EGL_LIBRARIES)
