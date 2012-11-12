# program name and version
AC_INIT(gfal2-plugin-xrootd, 0.1.0)

# set up automake and libtool
AM_INIT_AUTOMAKE(foreign)
AM_CONFIG_HEADER(config.h)
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

# we use c++ compiler
AC_PROG_CXX
AC_LANG(C++)

# standard headers and functions
AC_CHECK_HEADERS([iostream regex.h string.h sys/stat.h time.h])
AC_CHECK_FUNCS([memset strncmp])

# library must be installed in gfal2 plugin dir
pkglibdir='${libdir}/gfal2-plugins'
AC_SUBST(pkglibdir)

# pkg-config needed for glib and gfal2
AC_PATH_TOOL(PKG_CONFIG, pkg-config, no)
if test "x$PKG_CONFIG" = "xno"; then
  AC_MSG_ERROR([ *** pkg-config not found])
else
  pkgconfigdir=${libdir}/pkgconfig
  AC_SUBST(pkgconfigdir)
fi

# glib
PKG_CHECK_MODULES(GLIB, glib-2.0, [], [AC_MSG_ERROR(Could not find glib with pkg-config)])
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

SAVE_CPPFLAGS=$CPPFLAGS
CPPFLAGS="$CPPFLAGS $GLIB_CFLAGS"
AC_CHECK_HEADER(glib.h, [], [AC_MSG_ERROR(Could not find glib headers)])
CPPFLAGS=$SAVE_CPPFLAGS

SAVE_LDFLAGS=$LDFLAGS
LDFLAGS="$LDFLAGS $GLIB_LIBS"
AC_CHECK_LIB([glib-2.0], [g_set_error], [], [AC_MSG_ERROR(Could not find glib)])
LDFLAGS=$SAVE_LDFLAGS

# GFAL2
PKG_CHECK_MODULES(GFAL2, gfal2, [], [AC_MSG_ERROR(Could not find gfal2 with pkg-config)])
AC_SUBST(GFAL2_CFLAGS)
AC_SUBST(GFAL2_LIBS)

SAVE_CPPFLAGS=$CPPFLAGS
CPPFLAGS="$CPPFLAGS $GFAL2_CFLAGS"
AC_CHECK_HEADER(gfal_api.h, [], [AC_MSG_ERROR(Could not find GFAL2 headers)])
CPPFLAGS=$SAVE_CPPFLAGS

SAVE_LDFLAGS=$LDFLAGS
LDFLAGS="$LDFLAGS $GFAL2_LIBS"
AC_CHECK_LIB([gfal2], [_init], [], [AC_MSG_ERROR(Could not find GFAL2 library)])
LDFLAGS=$SAVE_LDFLAGS

# xrootd
AC_ARG_WITH(xrootd, [  --with-xrootd=(PATH)    Xrootd location])
XROOTD_CPPFLAGS="-I/usr/include/xrootd"
if test ! "x$with_xrootd" = "x" ; then
  XROOTD_LOCATION="$with_xrootd"
  XROOTD_CPPFLAGS="-I$XROOTD_LOCATION/include/xrootd"
  if test -d $XROOTD_LOCATION/lib64; then
    XROOTD_LDFLAGS="-L$XROOTD_LOCATION/lib64"
  else
    XROOTD_LDFLAGS="-L$XROOTD_LOCATION/lib"
  fi
fi

SAVE_CPPFLAGS=$CPPFLAGS
CPPFLAGS="$CPPFLAGS $XROOTD_CPPFLAGS"
AC_CHECK_HEADER(XrdClient/XrdClient.hh, [], [AC_MSG_ERROR(Could not find xrootd headers)] [#include <iostream>])
CPPFLAGS=$SAVE_CPPFLAGS

SAVE_LDFLAGS=$LDFLAGS
LDFLAGS="$LDFLAGS $XROOTD_LDFLAGS"
AC_CHECK_LIB([XrdClient], [_init],
             [XROOTD_LIBS="$XROOTD_LDFLAGS -lXrdClient -lXrdPosix"], [AC_MSG_ERROR(Could not find xrootd client library)])
LDFLAGS=$SAVE_LDFLAGS

AC_SUBST(XROOTD_CPPFLAGS)
AC_SUBST(XROOTD_LIBS)

# Makefiles

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
