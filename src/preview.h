#ifndef obconf__preview_h
#define obconf__preview_h

#include <glib.h>

#include <openbox/font.h>
#include <gdk/gdkpixbuf.h>

GdkPixbuf *preview_theme(gchar *name, gchar *titlelayout,
                         RrFont *active_window_font,
                         RrFont *inactive_window_font,
                         RrFont *menu_title_font,
                         RrFont *menu_item_font,
                         RrFont *osd_font);

#endif
