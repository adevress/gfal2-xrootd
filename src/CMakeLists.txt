cmake_minimum_required (VERSION 2.6)

pkg_check_modules(GFAL2_PKG  REQUIRED gfal2>=2.1.1)
pkg_check_modules(GLIB2_PKG REQUIRED glib-2.0)
find_package(XROOTD REQUIRED)

add_definitions(${GFAL2_PKG_CFLAGS})
add_definitions(${GLIB2_PKG_CFLAGS})


include_directories(${GFAL2_PKG_INCLUDE_DIRS}
                    ${GLIB2_PKG_INCLUDE_DIRS}
                    ${XROOTD_INCLUDE_DIR})

FILE(GLOB src_xrootd "*.cpp")

add_library (gfal_plugin_xrootd SHARED ${src_xrootd})

target_link_libraries (gfal_plugin_xrootd ${GFAL2_PKG_LIBRARIES})
target_link_libraries (gfal_plugin_xrootd XrdClient XrdPosix) 

install(TARGETS gfal_plugin_xrootd
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR})
