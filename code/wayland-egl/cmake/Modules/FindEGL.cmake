find_package(PkgConfig)
pkg_check_modules(PC_EGL QUIET egl)

find_path(EGL_INCLUDE_DIRS NAMES EGL/egl.h
    HINTS ${PC_EGL_INCLUDEDIR} ${PC_EGL_INCLUDE_DIRS})

find_library(EGL_LIBRARIES NAMES EGL
    HINTS ${PC_EGL_LIBDIR} ${PC_EGL_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(EGL
    DEFAULT_MSG EGL_INCLUDE_DIRS EGL_LIBRARIES)
mark_as_advanced(EGL_INCLUDE_DIRS EGL_LIBRARIES)
