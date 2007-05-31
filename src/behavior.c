/* -*- indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*-

   behavior.h for ObConf, the configuration tool for Openbox
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

static void behavior_enable_stuff();

void behavior_setup_tab()
{
    GtkWidget *w, *winresist, *edgeresist;
    GtkSizeGroup *group;
    gchar *s;

    mapping = TRUE;

    winresist  = get_widget("resist_window");
    edgeresist = get_widget("resist_edge");
    group      = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
    gtk_size_group_add_widget(group, winresist);
    gtk_size_group_add_widget(group, edgeresist);

    winresist  = get_widget("resist_window_label");
    edgeresist = get_widget("resist_edge_label");
    group      = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
    gtk_size_group_add_widget(group, winresist);
    gtk_size_group_add_widget(group, edgeresist);

    w = get_widget("focus_mouse");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
                                 tree_get_bool("focus/followMouse", FALSE));

    w = get_widget("focus_delay");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w),
                              tree_get_int("focus/focusDelay", 0));

    w = get_widget("focus_raise");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
                                 tree_get_bool("focus/raiseOnFocus", FALSE));

    w = get_widget("focus_last");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
                                 tree_get_bool("focus/focusLast", FALSE));

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

    mapping = FALSE;
}

static void behavior_enable_stuff()
{
    GtkWidget *w;
    gboolean b;

    w = get_widget("focus_mouse");
    b = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w));

    w = get_widget("focus_delay");
    gtk_widget_set_sensitive(w, b);
    w = get_widget("focus_delay_label");
    gtk_widget_set_sensitive(w, b);
    w = get_widget("focus_delay_label_units");
    gtk_widget_set_sensitive(w, b);
    w = get_widget("focus_raise");
    gtk_widget_set_sensitive(w, b);
    w = get_widget("focus_last");
    gtk_widget_set_sensitive(w, b);
}

void on_focus_mouse_toggled(GtkToggleButton *w, gpointer data)
{
    gboolean b;

    if (mapping) return;

    b = gtk_toggle_button_get_active(w);
    tree_set_bool("focus/followMouse", b);

    {
        GtkWidget *delay   = get_widget("focus_delay");
        GtkWidget *delay_l = get_widget("focus_delay_label");
        GtkWidget *delay_u = get_widget("focus_delay_label_units");
        GtkWidget *raise   = get_widget("focus_raise");
        GtkWidget *last    = get_widget("focus_last");
        gtk_widget_set_sensitive(delay, b);
        gtk_widget_set_sensitive(delay_l, b);
        gtk_widget_set_sensitive(delay_u, b);
        gtk_widget_set_sensitive(raise, b);
        gtk_widget_set_sensitive(last, b);
    }
}

void on_focus_delay_value_changed(GtkSpinButton *w, gpointer data)
{
    if (mapping) return;

    tree_set_int("focus/focusDelay",
                 gtk_spin_button_get_value_as_int(w));
}

void on_focus_raise_toggled(GtkToggleButton *w, gpointer data)
{
    if (mapping) return;

    tree_set_bool("focus/raiseOnFocus", gtk_toggle_button_get_active(w));
}

void on_focus_last_toggled(GtkToggleButton *w, gpointer data)
{
    if (mapping) return;

    tree_set_bool("focus/focusLast", gtk_toggle_button_get_active(w));
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
