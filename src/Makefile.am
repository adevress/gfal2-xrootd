pkglib_LTLIBRARIES = libgfal_plugin_xrootd.la

libgfal_plugin_xrootd_la_SOURCES = gfal_xrootd_plugin_main.h gfal_xrootd_plugin_main.cpp \
                                   gfal_xrootd_plugin_interface.h gfal_xrootd_plugin_interface.cpp \
                                   gfal_xrootd_plugin_utils.h gfal_xrootd_plugin_utils.cpp
libgfal_plugin_xrootd_la_CXXFLAGS = $(GLIB_CFLAGS) $(GFAL2_CFLAGS) $(XROOTD_CPPFLAGS)
libgfal_plugin_xrootd_la_LIBADD = $(GLIB_LIBS) $(GFAL2_LIBS) $(XROOTD_LIBS)
libgfal_plugin_xrootd_la_LDFLAGS = -no-undefined -avoid-version -module
