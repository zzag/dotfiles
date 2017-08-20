find_package(PkgConfig)
pkg_check_modules(PC_LIBXML QUIET wayland-client)

find_path(WAYLAND_INCLUDE_DIRS wayland-client.h
          HINTS ${PC_WAYLAND_INCLUDEDIR} ${PC_WAYLAND_INCLUDE_DIRS})

find_library(WAYLAND_LIBRARIES wayland-client
          HINTS ${PC_WAYLAND_LIBDIR} ${PC_WAYLAND_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Wayland DEFAULT_MSG
          WAYLAND_LIBRARIES WAYLAND_INCLUDE_DIRS)

