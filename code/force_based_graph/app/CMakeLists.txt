find_package(PkgConfig)
pkg_check_modules(GTK QUIET gtk+-3.0)
pkg_check_modules(CAIRO QUIET cairo)

include_directories(${GTK_INCLUDE_DIRS}
                    ${CAIRO_INCLUDE_DIRS})

link_directories(${GTK_LIBRARY_DIRS})

add_executable(app app.cc)
target_link_libraries(app ForceBasedRender
                          ${CAIRO_LIBRARIES}
                          ${GTK_LIBRARIES})
