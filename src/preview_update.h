#ifndef obconf__preview_update_h
#define obconf__preview_update_h

#include <openbox/render.h>
#include <gtk/gtk.h>

void preview_update_all();

void preview_update_set_list_store       (GtkListStore *ls);
void preview_update_set_active_font      (RrFont *f);
void preview_update_set_inactive_font    (RrFont *f);
void preview_update_set_menu_header_font (RrFont *f);
void preview_update_set_menu_item_font   (RrFont *f);
void preview_update_set_osd_font         (RrFont *f);
void preview_update_set_title_layout     (const gchar *layout);

#endif
