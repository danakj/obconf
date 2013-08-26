SUBDIRS = po

localedir       = $(datadir)/locale
resourcedir     = $(datadir)/obconf
pixmapdir       = $(datadir)/pixmaps
themedir        = $(datadir)/openbox/themes
desktopdir      = $(datadir)/applications
mimedir         = $(datadir)/mime
mimepkgdir      = $(datadir)/mime/packages
kdemimedir      = $(datadir)/mimelnk/application

AUTOMAKE_OPTIONS = subdir-objects foreign
ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = \
	src/obconf

src_obconf_CPPFLAGS = \
	$(OPENBOX_CFLAGS) \
	$(LIBSTARTUPNOTIFICATION_CFLAGS) \
	$(GTK_CFLAGS) \
	$(GDK_PIXBUF_CFLAGS) \
	$(XML_CFLAGS) \
	-DLOCALEDIR=\"$(localedir)\" \
	-DPIXMAPDIR=\"$(pixmapdir)\" \
	-DGLADEDIR=\"$(gladedir)\" \
	-DTHEMEDIR=\"$(themedir)\" \
	-DG_LOG_DOMAIN=\"Obconf\"
src_obconf_LDADD = \
	$(OPENBOX_LIBS) \
	$(LIBSTARTUPNOTIFICATION_LIBS) \
	$(GTK_LIBS) \
	$(GDK_PIXBUF_LIBS) \
	$(LIBINTL) \
	$(ZLIB_LIBS) \
	$(LIBTAR_LIBS)
src_obconf_SOURCES = \
	src/gettext.h \
	src/about.c \
	src/archive.c \
	src/archive.h \
	src/windows.c \
	src/windows.h \
	src/moveresize.c \
	src/moveresize.h \
	src/margins.c \
	src/margins.h \
	src/mouse.c \
	src/mouse.h \
	src/desktops.c \
	src/desktops.h \
	src/dock.h \
	src/dock.c \
	src/main.c \
	src/main.h \
	src/appearance.c \
	src/appearance.h \
	src/preview.c \
	src/preview.h \
	src/preview_update.c \
	src/preview_update.h \
	src/theme.c \
	src/theme.h \
	src/tree.c \
	src/tree.h

dist_resource_DATA = \
	src/obconf.ui \
	pixmaps/video-display.png

dist_desktop_DATA = \
	obconf.desktop

dist_mimepkg_DATA = \
	obconf.xml

dist_kdemime_DATA = \
	x-openbox-theme.desktop

dist_pixmap_DATA = \
	data/obconf.png

EXTRA_DIST = \
	config.rpath \
	README \
	CHANGELOG \
	COPYING \
	AUTHORS \
	m4/codeset.m4 \
	m4/gettext.m4 \
	m4/glibc21.m4 \
	m4/glibc2.m4 \
	m4/iconv.m4 \
	m4/intdiv0.m4 \
	m4/intmax.m4 \
	m4/inttypes_h.m4 \
	m4/inttypes-h.m4 \
	m4/inttypes-pri.m4 \
	m4/lcmessage.m4 \
	m4/lib-ld.m4 \
	m4/lib-link.m4 \
	m4/lib-prefix.m4 \
	m4/lock.m4 \
	m4/longdouble.m4 \
	m4/longlong.m4 \
	m4/nls.m4 \
	m4/po.m4 \
	m4/printf-posix.m4 \
	m4/progtest.m4 \
	m4/signed.m4 \
	m4/size_max.m4 \
	m4/stdint_h.m4 \
	m4/uintmax_t.m4 \
	m4/ulonglong.m4 \
	m4/visibility.m4 \
	m4/wchar_t.m4 \
	m4/wint_t.m4 \
	m4/xsize.m4 \
	data/video-display.svg

# Packages need to run these...

install-data-hook:
#	update-mime-database $(mimedir)
#	update-desktop-database $(desktopdir)

uninstall-hook:
#	update-mime-database $(mimedir)
#	update-desktop-database $(desktopdir)
