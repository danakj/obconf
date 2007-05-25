SUBDIRS = po

localedir       = $(datadir)/locale
gladedir        = $(datadir)/obconf
pixmapdir       = $(datadir)/pixmaps
themedir        = $(datadir)/openbox/themes
desktopdir      = $(datadir)/applications

AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = \
	src/obconf

src_obconf_CPPFLAGS = \
	$(OBLIBS_CFLAGS) \
	$(STARTUP_CFLAGS) \
	$(GTK_CFLAGS) \
	$(GLADE_CFLAGS) \
	$(GDK_PIXBUF_CFLAGS) \
	$(XML_CFLAGS) \
	-DLOCALEDIR=\"$(localedir)\" \
	-DPIXMAPDIR=\"$(pixmapdir)\" \
	-DGLADEDIR=\"$(gladedir)\" \
	-DTHEMEDIR=\"$(themedir)\" \
	-DG_LOG_DOMAIN=\"Obconf\"
src_obconf_LDADD = \
	$(OBLIBS_LIBS) \
	$(STARTUP_LIBS) \
	$(GTK_LIBS) \
	$(GLADE_LIBS) \
	$(GDK_PIXBUF_LIBS) \
	$(LIBINTL)
src_obconf_SOURCES = \
	src/gettext.h \
	src/main.c \
	src/main.h \
	src/handlers.c \
	src/handlers.h \
	src/tree.c \
	src/tree.h

dist_glade_DATA = \
	src/obconf.glade

dist_noinst_DATA = \
	src/obconf.gladep

dist_desktop_DATA = \
	obconf.desktop

EXTRA_DIST = \
	config.rpath \
	mkinstalldirs \
	README \
	COPYING \
	AUTHORS \
	m4/codeset.m4 \
	m4/gettext.m4 \
	m4/glibc21.m4 \
	m4/iconv.m4 \
	m4/intdiv0.m4 \
	m4/inttypes-pri.m4 \
	m4/inttypes.m4 \
	m4/inttypes_h.m4 \
	m4/isc-posix.m4 \
	m4/lcmessage.m4 \
	m4/lib-ld.m4 \
	m4/lib-link.m4 \
	m4/lib-prefix.m4 \
	m4/nls.m4 \
	m4/po.m4 \
	m4/progtest.m4 \
	m4/stdint_h.m4 \
	m4/uintmax_t.m4 \
	m4/ulonglong.m4 \
	debian/changelog.in \
	debian/compat \
	debian/control \
	debian/menu \
	debian/rules \
	config.rpath

