/* -*- indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*-

   windows.c for ObConf, the configuration tool for Openbox
   Copyright (c) 2003-2008   Dana Jansens
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

#define PLACE_ON_ALL    0
#define PLACE_ON_ACTIVE 1
#define PLACE_ON_MOUSE 2

static void enable_stuff();

void windows_setup_tab()
{
    GtkWidget *w;
    gchar *s;

    mapping = TRUE;

    w = get_widget("focus_new");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
                                 tree_get_bool("focus/focusNew", TRUE));

    w = get_widget("place_mouse");
    s = tree_get_string("placement/policy", "Smart");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
                                 !g_ascii_strcasecmp(s, "UnderMouse"));
    g_free(s);

    w = get_widget("place_center");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
                                 tree_get_bool("placement/center", TRUE));

    w = get_widget("place_active_popup");
    s = tree_get_string("placement/monitor", "Any");
    if (!g_ascii_strcasecmp(s, "Active"))
        gtk_option_menu_set_history(GTK_OPTION_MENU(w), PLACE_ON_ACTIVE);
    else if (!g_ascii_strcasecmp(s, "Mouse"))
        gtk_option_menu_set_history(GTK_OPTION_MENU(w), PLACE_ON_MOUSE);
    else
        gtk_option_menu_set_history(GTK_OPTION_MENU(w), PLACE_ON_ALL);
    g_free(s);

    enable_stuff();

    mapping = FALSE;
}

static void enable_stuff()
{
    GtkWidget *w;
    gboolean b;

    w = get_widget("place_mouse");
    b = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w));
    w = get_widget("place_center");
    gtk_widget_set_sensitive(w, !b);
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
    enable_stuff();
}

void on_place_center_toggled(GtkToggleButton *w, gpointer data)
{
    if (mapping) return;

    tree_set_bool("placement/center", gtk_toggle_button_get_active(w));
}

void on_place_active_popup_all_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("placement/monitor", "Any");
}

void on_place_active_popup_active_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("placement/monitor", "Active");
}

void on_place_active_popup_mouse_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("placement/monitor", "Mouse");
}
