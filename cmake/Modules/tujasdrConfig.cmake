INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_TUJASDR tujasdr)

FIND_PATH(
    TUJASDR_INCLUDE_DIRS
    NAMES tujasdr/api.h
    HINTS $ENV{TUJASDR_DIR}/include
        ${PC_TUJASDR_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    TUJASDR_LIBRARIES
    NAMES gnuradio-tujasdr
    HINTS $ENV{TUJASDR_DIR}/lib
        ${PC_TUJASDR_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TUJASDR DEFAULT_MSG TUJASDR_LIBRARIES TUJASDR_INCLUDE_DIRS)
MARK_AS_ADVANCED(TUJASDR_LIBRARIES TUJASDR_INCLUDE_DIRS)

