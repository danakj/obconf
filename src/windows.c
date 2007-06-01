/* -*- indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*-

   windows.c for ObConf, the configuration tool for Openbox
   Copyright (c) 2003-2007   Dana Jansens
   Copyright (c) 2003        Tim Riley

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   See the COPYING file for a copy of the GNU General Public License.
*/

#include "main.h"
#include "tree.h"

static gboolean mapping = FALSE;

#define POPUP_NONPIXEL 0
#define POPUP_ALWAYS   1
#define POPUP_NEVER    2

void windows_setup_tab()
{
    GtkWidget *w, *w1, *w2;
    GtkSizeGroup *group;
    gchar *s;
    gint pos;

    mapping = TRUE;

    w1    = get_widget("resist_window");
    w2    = get_widget("resist_edge");
    group = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
    gtk_size_group_add_widget(group, w1);
    gtk_size_group_add_widget(group, w2);

    w1    = get_widget("resist_window_label");
    w2    = get_widget("resist_edge_label");
    group = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
    gtk_size_group_add_widget(group, w1);
    gtk_size_group_add_widget(group, w2);

    w = get_widget("focus_new");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
                                 tree_get_bool("focus/focusNew", TRUE));

    w = get_widget("resize_contents");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
                                 tree_get_bool("resize/drawContents", TRUE));

    w = get_widget("resist_window");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w),
                              tree_get_int("resistance/strength", 10));

    w = get_widget("resist_edge");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w),
                              tree_get_int("resistance/screen_edge_strength",
                                           20));

    w = get_widget("place_mouse");
    s = tree_get_string("placement/policy", "Smart");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
                                 !g_ascii_strcasecmp(s, "UnderMouse"));
    g_free(s);

    w = get_widget("resize_popup");
    s = tree_get_string("resize/popupShow", "NonPixel");
    if (!strcasecmp(s, "Always"))     pos = POPUP_ALWAYS;
    else if (!strcasecmp(s, "Never")) pos = POPUP_NEVER;
    else                              pos = POPUP_NONPIXEL;
    g_free(s);
    gtk_option_menu_set_history(GTK_OPTION_MENU(w), pos);

    mapping = FALSE;
}

void on_focus_new_toggled(GtkToggleButton *w, gpointer data)
{
    if (mapping) return;

    tree_set_bool("focus/focusNew", gtk_toggle_button_get_active(w));
}

void on_place_mouse_toggled(GtkToggleButton *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("placement/policy",
                    (gtk_toggle_button_get_active(w) ?
                     "UnderMouse" : "Smart"));
}

void on_resist_window_value_changed(GtkSpinButton *w, gpointer data)
{
    if (mapping) return;

    tree_set_int("resistance/strength", gtk_spin_button_get_value_as_int(w));
}

void on_resist_edge_value_changed(GtkSpinButton *w, gpointer data)
{
    if (mapping) return;

    tree_set_int("resistance/screen_edge_strength",
                 gtk_spin_button_get_value_as_int(w));
}

void on_resize_contents_toggled(GtkToggleButton *w, gpointer data)
{
    if (mapping) return;

    tree_set_bool("resize/drawContents", gtk_toggle_button_get_active(w));
}

void on_resize_popup_nonpixel_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("resize/popupShow", "NonPixel");
}

void on_resize_popup_always_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("resize/popupShow", "Always");
}

void on_resize_popup_never_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("resize/popupShow", "Never");
}
