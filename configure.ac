AC_PREREQ([2.54])
AC_INIT([obconf], [2.0.4], [http://bugzilla.icculus.org])
AM_INIT_AUTOMAKE([subdir-objects foreign dist-xz])
AC_CONFIG_SRCDIR([src/main.c])

AC_PREFIX_DEFAULT([/usr/local])
test "$prefix" = "NONE" && prefix=$ac_default_prefix

# Pick compiler specific/build target flags, and set $CVS
AM_PROG_CC_C_O
AC_C_CONST
AC_C_INLINE

AC_PROG_INSTALL

CHECK_EXPORT_DYNAMIC
LDFLAGS="$LDFLAGS $EXPORT_DYNAMIC_FLAGS"

AM_GNU_GETTEXT_VERSION(0.15)
AM_GNU_GETTEXT([external])

PKG_CHECK_MODULES(OPENBOX, [obrender-3.5 obt-3.5])
AC_SUBST(OPENBOX_CFLAGS)
AC_SUBST(OPENBOX_LIBS)

PKG_CHECK_MODULES(LIBSTARTUPNOTIFICATION, [libstartup-notification-1.0])
AC_SUBST(LIBSTARTUPNOTIFICATION_CFLAGS)
AC_SUBST(LIBSTARTUPNOTIFICATION_LIBS)

PKG_CHECK_MODULES(GTK, [gtk+-3.0])
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

PKG_CHECK_MODULES(GDK_PIXBUF, [gdk-pixbuf-2.0])
AC_SUBST(GDK_PIXBUF_CFLAGS)
AC_SUBST(GDK_PIXBUF_LIBS)

AC_CHECK_PROG(TAR, [tar], [yes], [no])
if test "$TAR" = "no"; then
  AC_MSG_ERROR([Unable to find the tar command.])
fi

AC_CHECK_PROG(GZIP, [gzip], [yes], [no])
if test "$GZIP" = "no"; then
  AC_MSG_ERROR([Unable to find the gzip command.])
fi

AC_CHECK_PROG(GUNZIP, [gunzip], [yes], [no])
if test "$GUNZIP" = "no"; then
  AC_MSG_ERROR([Unable to find the gunzip command.])
fi

AC_CONFIG_FILES([
  Makefile
  po/Makefile.in
])
AC_OUTPUT

AC_MSG_RESULT
AC_MSG_RESULT([configure complete, now type \"make\"])
