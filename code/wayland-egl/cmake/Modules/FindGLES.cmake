find_package(PkgConfig)
pkg_check_modules(PC_GLES QUIET glesv2)

find_path(GLES_INCLUDE_DIRS NAMES GLES2/gl2.h
    HINTS ${PC_GLES_INCLUDEDIR} ${PC_GLES_INCLUDE_DIRS})

find_library(GLES_LIBRARIES NAMES GLESv2
    HINTS ${PC_GLES_LIBDIR} ${PC_GLES_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLES
    DEFAULT_MSG GLES_INCLUDE_DIRS GLES_LIBRARIES)
mark_as_advanced(GLES_INCLUDE_DIRS GLES_LIBRARIES)
