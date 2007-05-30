AC_PREREQ([2.54])
AC_INIT([obconf], [1.7.0], [http://bugzilla.icculus.org])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/main.c])

AC_PREFIX_DEFAULT([/usr/local])
test "$prefix" = "NONE" && prefix=$ac_default_prefix

# Pick compiler specific/build target flags, and set $CVS
AM_PROG_CC_C_O
AC_C_CONST
AC_C_INLINE

AC_PROG_INSTALL

LDFLAGS="$LDFLAGS -export-dynamic"

ALL_LINGUAS=""
AM_GNU_GETTEXT_VERSION(0.15)
AM_GNU_GETTEXT([external])

PKG_CHECK_MODULES(OPENBOX, [obrender-3.0 obparser-3.0])
AC_SUBST(OPENBOX_CFLAGS)
AC_SUBST(OPENBOX_LIBS)

PKG_CHECK_MODULES(LIBSTARTUPNOTIFICATION, [libstartup-notification-1.0])
AC_SUBST(LIBSTARTUPNOTIFICATION_CFLAGS)
AC_SUBST(LIBSTARTUPNOTIFICATION_LIBS)

PKG_CHECK_MODULES(GTK, [gtk+-2.0])
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

PKG_CHECK_MODULES(GDK_PIXBUF, [gdk-pixbuf-2.0])
AC_SUBST(GDK_PIXBUF_CFLAGS)
AC_SUBST(GDK_PIXBUF_LIBS)

PKG_CHECK_MODULES(GLADE, [libglade-2.0])
AC_SUBST(GLADE_CFLAGS)
AC_SUBST(GLADE_LIBS)

# zlib
AC_CHECK_HEADER([zlib.h], [ZLIB_LIBS="-lz"],
  AC_MSG_ERROR([Could not find zlib.h]))
AC_CHECK_LIB([z], [gzopen],,
  AC_MSG_ERROR([Could not find gzopen in -lz.]))
AC_SUBST(ZLIB_LIBS)

# libtar
AC_CHECK_HEADER([libtar.h], [LIBTAR_LIBS="-ltar"],
  AC_MSG_ERROR([Could not find libtar.h]))
AC_CHECK_LIB([tar], [tar_open],,
  AC_MSG_ERROR([Could not find tar_open in -ltar.]))
AC_SUBST(LIBTAR_LIBS)

AC_CONFIG_FILES([
  Makefile
  po/Makefile.in
])
AC_OUTPUT

AC_MSG_RESULT
AC_MSG_RESULT([configure complete, now type \"make\"])


