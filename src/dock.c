/* -*- indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*-

   dock.c for ObConf, the configuration tool for Openbox
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

void dock_setup_tab()
{
    GtkWidget *posi, *dir;
    GtkWidget *posi_l, *dir_l;
    GtkSizeGroup *group1, *group2;

    posi   = get_widget("dock_position");
    dir    = get_widget("dock_direction");
    group1 = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

    gtk_size_group_add_widget(group1, posi);
    gtk_size_group_add_widget(group1, dir);

    posi_l = get_widget("dock_position_label");
    dir_l  = get_widget("dock_direction_label");
    group2 = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

    gtk_size_group_add_widget(group2, posi_l);
    gtk_size_group_add_widget(group2, dir_l);
}

void dock_setup_position(GtkWidget *w)
{
    gchar *s;
    gboolean f;

    mapping = TRUE;

    s = tree_get_string("dock/position", "TopLeft");
    f = FALSE;

    if (!strcasecmp(s, "Top"))
        gtk_option_menu_set_history(GTK_OPTION_MENU(w), 1);
    else if (!strcasecmp(s, "TopRight"))
        gtk_option_menu_set_history(GTK_OPTION_MENU(w), 2);
    else if (!strcasecmp(s, "Left"))
        gtk_option_menu_set_history(GTK_OPTION_MENU(w), 3);
    else if (!strcasecmp(s, "Right"))
        gtk_option_menu_set_history(GTK_OPTION_MENU(w), 4);
    else if (!strcasecmp(s, "BottomLeft"))
        gtk_option_menu_set_history(GTK_OPTION_MENU(w), 5);
    else if (!strcasecmp(s, "Bottom"))
        gtk_option_menu_set_history(GTK_OPTION_MENU(w), 6);
    else if (!strcasecmp(s, "BottomRight"))
        gtk_option_menu_set_history(GTK_OPTION_MENU(w), 7);
    else if (!strcasecmp(s, "Floating")) {
        gtk_option_menu_set_history(GTK_OPTION_MENU(w), 8);
        f = TRUE;
    } else
        gtk_option_menu_set_history(GTK_OPTION_MENU(w), 0);
    g_free(s);

    {
        GtkWidget *s;
        s = get_widget("dock_float_x");
        gtk_widget_set_sensitive(s, f);
        s = get_widget("dock_float_y");
        gtk_widget_set_sensitive(s, f);
        s = get_widget("dock_float_label");
        gtk_widget_set_sensitive(s, f);
        s = get_widget("dock_float_label_x");
        gtk_widget_set_sensitive(s, f);
    }

    mapping = FALSE;
}

void dock_setup_float_x(GtkWidget *w)
{
    mapping = TRUE;

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w),
                              tree_get_int("dock/floatingX", 0));

    mapping = FALSE;
}

void dock_setup_float_y(GtkWidget *w)
{
    mapping = TRUE;

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w),
                              tree_get_int("dock/floatingY", 0));

    mapping = FALSE;
}

void dock_setup_stacking(GtkWidget *top, GtkWidget *normal, GtkWidget *bottom)
{
    gchar *s;

    mapping = TRUE;

    s = tree_get_string("dock/stacking", "Top");

    if(!strcasecmp(s, "Normal"))
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(normal), TRUE);
    else if(!strcasecmp(s, "Bottom"))
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bottom), TRUE);
    else
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(top), TRUE);
    g_free(s);
    
    mapping = FALSE;
}

void dock_setup_direction(GtkWidget *w)
{
    gchar *s;

    mapping = TRUE;

    s = tree_get_string("dock/direction", "Vertical");

    if (!strcasecmp(s, "Horizontal"))
        gtk_option_menu_set_history(GTK_OPTION_MENU(w), 1);
    else
        gtk_option_menu_set_history(GTK_OPTION_MENU(w), 0);
    g_free(s);

    mapping = FALSE;
}

void dock_setup_hide(GtkWidget *w)
{
    gboolean b;

    mapping = TRUE;

    b = tree_get_bool("dock/autoHide", FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), b);

    {
        GtkWidget *delay   = get_widget("dock_hide_delay");
        GtkWidget *delay_l = get_widget("dock_hide_label");
        GtkWidget *delay_u = get_widget("dock_hide_label_units");
        gtk_widget_set_sensitive(delay, b);
        gtk_widget_set_sensitive(delay_l, b);
        gtk_widget_set_sensitive(delay_u, b);
    }

    mapping = FALSE;
}

void dock_setup_hide_delay(GtkWidget *w)
{
    mapping = TRUE;

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w),
                              tree_get_int("dock/hideDelay", 300));

    mapping = FALSE;
}


void on_dock_top_left_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "TopLeft");

    {
        GtkWidget *s;
        s = get_widget("dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
    }
}

void on_dock_top_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "Top");

    {
        GtkWidget *s;
        s = get_widget("dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
    }
}

void on_dock_top_right_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "TopRight");

    {
        GtkWidget *s;
        s = get_widget("dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
    }
}

void on_dock_left_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "Left");

    {
        GtkWidget *s;
        s = get_widget("dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
    }
}

void on_dock_right_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "Right");

    {
        GtkWidget *s;
        s = get_widget("dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
    }
}

void on_dock_bottom_left_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "BottomLeft");

    {
        GtkWidget *s;
        s = get_widget("dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
    }
}

void on_dock_bottom_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "Bottom");

    {
        GtkWidget *s;
        s = get_widget("dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
    }
}

void on_dock_bottom_right_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "BottomRight");

    {
        GtkWidget *s;
        s = get_widget("dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = get_widget("dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
    }
}

void on_dock_floating_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "Floating");

    {
        GtkWidget *s;
        s = get_widget("dock_float_x");
        gtk_widget_set_sensitive(s, TRUE);
        s = get_widget("dock_float_y");
        gtk_widget_set_sensitive(s, TRUE);
        s = get_widget("dock_float_label");
        gtk_widget_set_sensitive(s, TRUE);
        s = get_widget("dock_float_label_x");
        gtk_widget_set_sensitive(s, TRUE);
    }
}

void on_dock_float_x_value_changed(GtkSpinButton *w, gpointer data)
{
    if (mapping) return;

    tree_set_int("dock/floatingX", gtk_spin_button_get_value_as_int(w));
}

void on_dock_float_y_value_changed(GtkSpinButton *w, gpointer data)
{
    if (mapping) return;

    tree_set_int("dock/floatingY", gtk_spin_button_get_value_as_int(w));
}

void on_dock_stacking_top_toggled(GtkToggleButton *w, gpointer data)
{
    if (mapping) return;

    if(gtk_toggle_button_get_active(w))
        tree_set_string("dock/stacking", "Top");
}

void on_dock_stacking_normal_toggled(GtkToggleButton *w, gpointer data)
{
    if (mapping) return;

    if(gtk_toggle_button_get_active(w))
        tree_set_string("dock/stacking", "Normal");
}

void on_dock_stacking_bottom_toggled(GtkToggleButton *w, gpointer data)
{
    if (mapping) return;

    if(gtk_toggle_button_get_active(w))
        tree_set_string("dock/stacking", "Bottom");
}

void on_dock_horizontal_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/direction", "Horizontal");
}

void on_dock_vertical_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/direction", "Vertical");
}

void on_dock_hide_toggled(GtkToggleButton *w, gpointer data)
{
    if (mapping) return;

    tree_set_bool("dock/autoHide", gtk_toggle_button_get_active(w));
    {
        GtkWidget *delay   = glade_xml_get_widget(glade, "dock_hide_delay");
        GtkWidget *delay_l = glade_xml_get_widget(glade, "dock_hide_label");
        GtkWidget *delay_u = glade_xml_get_widget(glade, 
                                                  "dock_hide_label_units");
        gtk_widget_set_sensitive(delay, gtk_toggle_button_get_active(w));
        gtk_widget_set_sensitive(delay_l, gtk_toggle_button_get_active(w));
        gtk_widget_set_sensitive(delay_u, gtk_toggle_button_get_active(w));
    }
}

void on_dock_hide_delay_value_changed(GtkSpinButton *w, gpointer data)
{
    if (mapping) return;

    tree_set_int("dock/hideDelay",
                 gtk_spin_button_get_value_as_int(w));
}

