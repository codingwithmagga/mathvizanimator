find_package(PkgConfig)
pkg_check_modules(PKG_CAIRO cairo)

find_path(
  CAIRO_INCLUDE_DIR
  NAMES cairo.h
  HINTS
    ${PKG_CAIRO_INCLUDEDIR}
    ${PKG_CAIRO_INCLUDE_DIRS}
  PATH_SUFFIXES cairo
)

FIND_LIBRARY(CAIRO_LIBRARY
    NAMES cairo
    HINTS 
      ${PKG_CAIRO_LIBDIR}
      ${PKG_CAIRO_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  Cairo
  REQUIRED_VARS
    CAIRO_INCLUDE_DIR
    CAIRO_LIBRARY
)

set(CAIRO_INCLUDE_DIRS ${CAIRO_INCLUDE_DIR})
set(CAIRO_LIBRARIES ${CAIRO_LIBRARY})

mark_as_advanced(CAIRO_INCLUDE_DIRS CAIRO_LIBRARIES)

if(CAIRO_FOUND AND NOT TARGET Cairo::Cairo)
    add_library(Cairo::Cairo INTERFACE IMPORTED)
    target_include_directories(
      Cairo::Cairo
      INTERFACE ${CAIRO_INCLUDE_DIRS}
      )
    target_link_libraries(
      Cairo::Cairo
      INTERFACE ${CAIRO_LIBRARIES}
      )
endif()
