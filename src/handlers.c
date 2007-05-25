/* -*- indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*-

   handlers.h for ObConf, the configuration tool for Openbox
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
#include "gettext.h"
#include "openbox/render.h"

#include <string.h>
#include <ctype.h>
#include <gdk/gdkx.h>

static gboolean mapping;
static GList *themes;
static GtkListStore *desktop_store;
static int num_desktops;
static GList *desktop_names;
static GtkListStore *theme_store;

static void on_desktop_names_cell_edited(GtkCellRendererText *cell,
                                         const gchar *path_string,
                                         const gchar *new_text,
                                         gpointer data);

static void on_theme_names_selection_changed(GtkTreeSelection *sel, 
                                             gpointer data);


void setup_behavior_tab()
{
  GtkWidget *winresist  = glade_xml_get_widget(glade, "resist_window");
  GtkWidget *edgeresist = glade_xml_get_widget(glade, "resist_edge");
  GtkSizeGroup *group1  = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

  gtk_size_group_add_widget(group1, winresist);
  gtk_size_group_add_widget(group1, edgeresist);

  GtkWidget *winresist_l  = glade_xml_get_widget(glade, "resist_window_label");
  GtkWidget *edgeresist_l = glade_xml_get_widget(glade, "resist_edge_label");
  GtkSizeGroup *group2    = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

  gtk_size_group_add_widget(group2, winresist_l);
  gtk_size_group_add_widget(group2, edgeresist_l);
}

void setup_dock_tab()
{
    GtkWidget *posi = glade_xml_get_widget(glade, "dock_position");
    GtkWidget *dir  = glade_xml_get_widget(glade, "dock_direction");
    GtkSizeGroup *group1 = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

    gtk_size_group_add_widget(group1, posi);
    gtk_size_group_add_widget(group1, dir);

    GtkWidget *posi_l = glade_xml_get_widget(glade, "dock_position_label");
    GtkWidget *dir_l  = glade_xml_get_widget(glade, "dock_direction_label");
    GtkSizeGroup *group2 = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

    gtk_size_group_add_widget(group2, posi_l);
    gtk_size_group_add_widget(group2, dir_l);
}



gboolean on_main_window_delete_event(GtkWidget *w, GdkEvent *e, gpointer d)
{
    gtk_main_quit();
    return FALSE;
}

void on_close_clicked()
{
    gtk_main_quit();
}

void on_about_clicked()
{
    GtkWidget *parent;
    GtkWidget *about;

    parent = glade_xml_get_widget(glade, "main_window");
    about  = glade_xml_get_widget(glade, "about_window");

    gtk_window_set_transient_for(GTK_WINDOW(about), GTK_WINDOW(parent));
    gtk_widget_show(about);
}

void on_about_close_clicked()
{
    GtkWidget *about;
    
    about = glade_xml_get_widget(glade, "about_window");
    
    gtk_widget_hide(about);
}

void on_about_window_delete_event()
{
    GtkWidget *about;

    about = glade_xml_get_widget(glade, "about_window");

    gtk_widget_hide(about);
}

void setup_focus_mouse(GtkWidget *w)
{
    gboolean b;

    mapping = TRUE;

    b = tree_get_bool("focus/followMouse", FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), b);

    {
        GtkWidget *delay   = glade_xml_get_widget(glade, "focus_delay");
        GtkWidget *delay_l = glade_xml_get_widget(glade, "focus_delay_label");
        GtkWidget *delay_u = glade_xml_get_widget(glade,
                                                  "focus_delay_label_units");
        GtkWidget *raise   = glade_xml_get_widget(glade, "focus_raise");
        gtk_widget_set_sensitive(delay, b);
        gtk_widget_set_sensitive(delay_l, b);
        gtk_widget_set_sensitive(delay_u, b);
        gtk_widget_set_sensitive(raise, b);
    }

    mapping = FALSE;
}

void setup_focus_delay(GtkWidget *w)
{
    mapping = TRUE;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w),
                              tree_get_int("focus/focusDelay", 0));
    mapping = FALSE;
}

void setup_focus_raise(GtkWidget *w)
{
    mapping = TRUE;
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
                          tree_get_bool("focus/raiseOnFocus", FALSE));
    mapping = FALSE;
}

void setup_focus_new(GtkWidget *w)
{
    mapping = TRUE;
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
                                 tree_get_bool("focus/focusNew", TRUE));
    mapping = FALSE;
}

void setup_place_mouse(GtkWidget *w)
{
    gchar *s;

    mapping = TRUE;
    s = tree_get_string("placement/policy", "Smart");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
                                 !g_ascii_strcasecmp(s, "UnderMouse"));
    g_free(s);
    mapping = FALSE;
}

void setup_resist_window(GtkWidget *w)
{
    mapping = TRUE;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w),
                              tree_get_int("resistance/strength", 10));
    mapping = FALSE;
}

void setup_resist_edge(GtkWidget *w)
{
    mapping = TRUE;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w),
                              tree_get_int("resistance/screen_edge_strength",
                                           20));
    mapping = FALSE;
}

void setup_resize_contents(GtkWidget *w)
{
    mapping = TRUE;
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
                                 tree_get_bool("resize/drawContents", TRUE));
    mapping = FALSE;
}

void setup_dock_position(GtkWidget *w)
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
        s = glade_xml_get_widget(glade, "dock_float_x");
        gtk_widget_set_sensitive(s, f);
        s = glade_xml_get_widget(glade, "dock_float_y");
        gtk_widget_set_sensitive(s, f);
        s = glade_xml_get_widget(glade, "dock_float_label");
        gtk_widget_set_sensitive(s, f);
        s = glade_xml_get_widget(glade, "dock_float_label_x");
        gtk_widget_set_sensitive(s, f);
    }

    mapping = FALSE;
}

void setup_dock_float_x(GtkWidget *w)
{
    mapping = TRUE;

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w),
                              tree_get_int("dock/floatingX", 0));

    mapping = FALSE;
}

void setup_dock_float_y(GtkWidget *w)
{
    mapping = TRUE;

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w),
                              tree_get_int("dock/floatingY", 0));

    mapping = FALSE;
}

void setup_dock_stacking(GtkWidget *top, GtkWidget *normal, GtkWidget *bottom)
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

void setup_dock_direction(GtkWidget *w)
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

void setup_dock_hide(GtkWidget *w)
{
    gboolean b;

    mapping = TRUE;

    b = tree_get_bool("dock/autoHide", FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), b);

    {
        GtkWidget *delay   = glade_xml_get_widget(glade, "dock_hide_delay");
        GtkWidget *delay_l = glade_xml_get_widget(glade, "dock_hide_label");
        GtkWidget *delay_u = glade_xml_get_widget(glade, 
                                                  "dock_hide_label_units");
        gtk_widget_set_sensitive(delay, b);
        gtk_widget_set_sensitive(delay_l, b);
        gtk_widget_set_sensitive(delay_u, b);
    }

    mapping = FALSE;
}

void setup_dock_hide_delay(GtkWidget *w)
{
    mapping = TRUE;

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w),
                              tree_get_int("dock/hideDelay", 300));

    mapping = FALSE;
}

static void add_theme_dir(const gchar *dirname)
{
    GDir *dir;
    const gchar *n;

    if ((dir = g_dir_open(dirname, 0, NULL))) {
        while ((n = g_dir_read_name(dir))) {
            {
                gchar *full;
                full = g_build_filename(dirname, n, "openbox-3",
                                        "themerc", NULL);
                if (!g_file_test(full,
                                 G_FILE_TEST_IS_REGULAR |
                                 G_FILE_TEST_IS_SYMLINK))
                    n = NULL;
                g_free(full);
            }

            if (n) {
                themes = g_list_append(themes, g_strdup(n));
            }
        }
        g_dir_close(dir);
    }
}

void setup_theme_names(GtkWidget *w)
{
    GtkCellRenderer *render;
    GtkTreeViewColumn *column;
    gchar *name;
    gchar *p;
    GList *it, *next;
    gint i;
    GtkTreeSelection *select;

    mapping = TRUE;

    name = tree_get_string("theme/name", "TheBear");

    for (it = themes; it; it = g_list_next(it))
        g_list_free(it->data);
    g_list_free(themes);
    themes = NULL;

    p = g_build_filename(g_get_home_dir(), ".themes", NULL);
    add_theme_dir(p);
    g_free(p);

    {
        GSList *it;
        for (it = parse_xdg_data_dir_paths(); it; it = g_slist_next(it)) {
            p = g_build_filename(it->data, "themes", NULL);
            add_theme_dir(p);
            g_free(p);
        }
    }

    add_theme_dir(THEMEDIR);

    themes = g_list_sort(themes, (GCompareFunc) strcasecmp);

    /* widget setup */
    theme_store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_BOOLEAN);
    gtk_tree_view_set_model(GTK_TREE_VIEW(w), GTK_TREE_MODEL(theme_store));
    g_object_unref (theme_store);

    gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(w)),
                                GTK_SELECTION_SINGLE);

    render = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes
        ("Name", render, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(w), column);

    /* return to regular scheduled programming */
    i = 0;
    for (it = themes; it; it = next) {
        GtkTreeIter iter;

        next = g_list_next(it);

        /* remove duplicates */
        if (next && !strcmp(it->data, next->data)) {
            g_free(it->data);
            themes = g_list_delete_link(themes, it);
            continue;
        }

        gtk_list_store_append(theme_store, &iter);
        gtk_list_store_set(theme_store, &iter,
                           0, it->data,
                           1, TRUE,
                           -1);

        if(!strcmp(name, it->data)) {
            GtkTreePath *path;
            path = gtk_tree_path_new_from_indices(i, -1);
            gtk_tree_view_set_cursor(GTK_TREE_VIEW(w), path, NULL, FALSE);
        }


        ++i;
    }

    /* setup the selection handler */
    select = gtk_tree_view_get_selection(GTK_TREE_VIEW (w));
    gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);
    g_signal_connect (G_OBJECT(select), "changed",
                      G_CALLBACK(on_theme_names_selection_changed),
                      NULL);

    g_free(name);

    mapping = FALSE;
}

void setup_title_layout(GtkWidget *w)
{
    gchar *layout;

    mapping = TRUE;

    layout = tree_get_string("theme/titleLayout", "NLIMC");
    gtk_entry_set_text(GTK_ENTRY(w), layout);
    g_free(layout);

    mapping = FALSE;
}

void setup_desktop_num(GtkWidget *w)
{
    mapping = TRUE;

    num_desktops = tree_get_int("desktops/number", 4);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w), num_desktops);

    mapping = FALSE;
}

void setup_window_border(GtkWidget *w)
{
    gboolean border;

    mapping = TRUE;

    border = tree_get_bool("theme/keepBorder", TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), border);

    mapping = FALSE;
}

void setup_font_active(GtkWidget *w)
{
    gchar *fontstring;
    gchar *name, **names;
    gchar *size;
    gchar *weight;
    gchar *slant;

    mapping = TRUE;

    name = tree_get_string("theme/font:place=ActiveWindow/name", "Sans");
    size = tree_get_string("theme/font:place=ActiveWindow/size", "8");
    weight = tree_get_string("theme/font:place=ActiveWindow/weight", "");
    slant = tree_get_string("theme/font:place=ActiveWindow/slant", "");

    /* get only the first font in the string */
    names = g_strsplit(name, ",", 0);
    g_free(name);
    name = g_strdup(names[0]);
    g_strfreev(names);

    /* don't use "normal" in the gtk string */
    if (!g_ascii_strcasecmp(weight, "normal")) {
        g_free(weight); weight = g_strdup("");
    }
    if (!g_ascii_strcasecmp(slant, "normal")) {
        g_free(slant); slant = g_strdup("");
    }

    fontstring = g_strdup_printf("%s %s %s %s", name, weight, slant, size);
    gtk_font_button_set_font_name(GTK_FONT_BUTTON(w), fontstring);
    g_free(fontstring);
    g_free(slant);
    g_free(weight);
    g_free(size);
    g_free(name);

    mapping = FALSE;
}

void setup_font_inactive(GtkWidget *w)
{
    gchar *fontstring;
    gchar *name, **names;
    gchar *size;
    gchar *weight;
    gchar *slant;

    mapping = TRUE;

    name = tree_get_string("theme/font:place=InactiveWindow/name", "Sans");
    size = tree_get_string("theme/font:place=InactiveWindow/size", "8");
    weight = tree_get_string("theme/font:place=InactiveWindow/weight", "");
    slant = tree_get_string("theme/font:place=InactiveWindow/slant", "");

    /* get only the first font in the string */
    names = g_strsplit(name, ",", 0);
    g_free(name);
    name = g_strdup(names[0]);
    g_strfreev(names);

    fontstring = g_strdup_printf("%s %s %s %s", name, weight, slant, size);
    gtk_font_button_set_font_name(GTK_FONT_BUTTON(w), fontstring);
    g_free(fontstring);
    g_free(slant);
    g_free(weight);
    g_free(size);
    g_free(name);

    mapping = FALSE;
}

void setup_font_menu_header(GtkWidget *w)
{
    gchar *fontstring;
    gchar *name, **names;
    gchar *size;
    gchar *weight;
    gchar *slant;

    mapping = TRUE;

    name = tree_get_string("theme/font:place=MenuHeader/name", "Sans");
    size = tree_get_string("theme/font:place=MenuHeader/size", "8");
    weight = tree_get_string("theme/font:place=MenuHeader/weight", "");
    slant = tree_get_string("theme/font:place=MenuHeader/slant", "");

    /* get only the first font in the string */
    names = g_strsplit(name, ",", 0);
    g_free(name);
    name = g_strdup(names[0]);
    g_strfreev(names);

    fontstring = g_strdup_printf("%s %s %s %s", name, weight, slant, size);
    gtk_font_button_set_font_name(GTK_FONT_BUTTON(w), fontstring);
    g_free(fontstring);
    g_free(slant);
    g_free(weight);
    g_free(size);
    g_free(name);

    mapping = FALSE;
}

void setup_font_menu_item(GtkWidget *w)
{
    gchar *fontstring;
    gchar *name, **names;
    gchar *size;
    gchar *weight;
    gchar *slant;

    mapping = TRUE;

    name = tree_get_string("theme/font:place=MenuItem/name", "Sans");
    size = tree_get_string("theme/font:place=MenuItem/size", "8");
    weight = tree_get_string("theme/font:place=MenuItem/weight", "");
    slant = tree_get_string("theme/font:place=MenuItem/slant", "");

    /* get only the first font in the string */
    names = g_strsplit(name, ",", 0);
    g_free(name);
    name = g_strdup(names[0]);
    g_strfreev(names);

    fontstring = g_strdup_printf("%s %s %s %s", name, weight, slant, size);
    gtk_font_button_set_font_name(GTK_FONT_BUTTON(w), fontstring);
    g_free(fontstring);
    g_free(slant);
    g_free(weight);
    g_free(size);
    g_free(name);

    mapping = FALSE;
}

void setup_font_display(GtkWidget *w)
{
    gchar *fontstring;
    gchar *name, **names;
    gchar *size;
    gchar *weight;
    gchar *slant;

    mapping = TRUE;

    name = tree_get_string("theme/font:place=OnScreenDisplay/name", "Sans");
    size = tree_get_string("theme/font:place=OnScreenDisplay/size", "8");
    weight = tree_get_string("theme/font:place=OnScreenDisplay/weight", "");
    slant = tree_get_string("theme/font:place=OnScreenDisplay/slant", "");

    /* get only the first font in the string */
    names = g_strsplit(name, ",", 0);
    g_free(name);
    name = g_strdup(names[0]);
    g_strfreev(names);

    fontstring = g_strdup_printf("%s %s %s %s", name, weight, slant, size);
    gtk_font_button_set_font_name(GTK_FONT_BUTTON(w), fontstring);
    g_free(fontstring);
    g_free(slant);
    g_free(weight);
    g_free(size);
    g_free(name);

    mapping = FALSE;
}

static void reset_desktop_names()
{
    GtkTreeIter it;
    xmlNodePtr n;
    gint i;
    GList *lit;

    gtk_list_store_clear(desktop_store);

    for (lit = desktop_names; lit; lit = g_list_next(lit))
        g_free(lit->data);
    g_list_free(desktop_names);
    desktop_names = NULL;

    i = 0;
    n = tree_get_node("desktops/names", NULL)->children;
    while (n) {
        gchar *name;

        if (!xmlStrcmp(n->name, (const xmlChar*)"name")) {
            name = parse_string(doc, n);

            desktop_names = g_list_append(desktop_names, name);

            gtk_list_store_append(desktop_store, &it);
            gtk_list_store_set(desktop_store, &it,
                               0, (name[0] ? name : _("(Unnamed desktop)")),
                               1, TRUE,
                               -1);
            ++i;
        }

        n = n->next;
    }

    while (i < num_desktops) {
        gchar *name = g_strdup("");

        desktop_names = g_list_append(desktop_names, name);

        gtk_list_store_append(desktop_store, &it);
        gtk_list_store_set(desktop_store, &it,
                           0, _("(Unnamed desktop)"),
                           1, TRUE,
                           -1);
        ++i;
    }
}

void setup_desktop_names(GtkWidget *w)
{
    GtkCellRenderer *render;
    GtkTreeViewColumn *column;

    mapping = TRUE;

    desktop_store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_BOOLEAN);
    gtk_tree_view_set_model(GTK_TREE_VIEW(w), GTK_TREE_MODEL(desktop_store));
    g_object_unref (desktop_store);

    gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(w)),
                                GTK_SELECTION_SINGLE);

    render = gtk_cell_renderer_text_new();
    g_signal_connect(render, "edited",
                     G_CALLBACK (on_desktop_names_cell_edited),
                     NULL);

    column = gtk_tree_view_column_new_with_attributes
        ("Name", render, "text", 0, "editable", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(w), column);

    reset_desktop_names();

    mapping = FALSE;
}


/***********************************************************************/

void on_window_border_toggled(GtkToggleButton *w, gpointer data)
{
    gboolean b;

    if (mapping) return;

    b = gtk_toggle_button_get_active(w);
    tree_set_bool("theme/keepBorder", b);
}

static void on_font_set(GtkFontButton *w, const gchar *place)
{
    gchar *c;
    gchar *font, *node;
    const gchar *size = NULL;
    const gchar *bold = NULL;
    const gchar *italic = NULL;

    if (mapping) return;

    font = g_strdup(gtk_font_button_get_font_name(w));
    while ((c = strrchr(font, ' '))) {
        if (!bold && !italic && !size && atoi(c+1))
            size = c+1;
        else if (!bold && !italic && !g_ascii_strcasecmp(c+1, "italic"))
            italic = c+1;
        else if (!bold && !g_ascii_strcasecmp(c+1, "bold"))
            bold = c+1;
        else
            break;
        *c = '\0';
    }
    if (!bold) bold = "Normal";
    if (!italic) italic = "Normal";

    node = g_strdup_printf("theme/font:place=%s/name", place);
    tree_set_string(node, font);
    g_free(node);

    node = g_strdup_printf("theme/font:place=%s/size", place);
    tree_set_string(node, size);
    g_free(node);

    node = g_strdup_printf("theme/font:place=%s/weight", place);
    tree_set_string(node, bold);
    g_free(node);

    node = g_strdup_printf("theme/font:place=%s/slant", place);
    tree_set_string(node, italic);
    g_free(node);

    g_free(font);
}

void on_font_active_font_set(GtkFontButton *w, gpointer data)
{
    on_font_set(w, "ActiveWindow");
}

void on_font_inactive_font_set(GtkFontButton *w, gpointer data)
{
    on_font_set(w, "InactiveWindow");
}

void on_font_menu_header_font_set(GtkFontButton *w, gpointer data)
{
    on_font_set(w, "MenuHeader");
}

void on_font_menu_item_font_set(GtkFontButton *w, gpointer data)
{
    on_font_set(w, "MenuItem");
}

void on_font_display_font_set(GtkFontButton *w, gpointer data)
{
    on_font_set(w, "OnScreenDisplay");
}

void on_focus_mouse_toggled(GtkToggleButton *w, gpointer data)
{
    gboolean b;

    if (mapping) return;

    b = gtk_toggle_button_get_active(w);
    tree_set_bool("focus/followMouse", b);

    {
        GtkWidget *delay   = glade_xml_get_widget(glade, "focus_delay");
        GtkWidget *delay_l = glade_xml_get_widget(glade, "focus_delay_label");
        GtkWidget *delay_u = glade_xml_get_widget(glade,
                                                  "focus_delay_label_units");
        GtkWidget *raise   = glade_xml_get_widget(glade, "focus_raise");
        gtk_widget_set_sensitive(delay, b);
        gtk_widget_set_sensitive(delay_l, b);
        gtk_widget_set_sensitive(delay_u, b);
        gtk_widget_set_sensitive(raise, b);
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

void on_dock_top_left_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "TopLeft");

    {
        GtkWidget *s;
        s = glade_xml_get_widget(glade, "dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
    }
}

void on_dock_top_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "Top");

    {
        GtkWidget *s;
        s = glade_xml_get_widget(glade, "dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
    }
}

void on_dock_top_right_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "TopRight");

    {
        GtkWidget *s;
        s = glade_xml_get_widget(glade, "dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
    }
}

void on_dock_left_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "Left");

    {
        GtkWidget *s;
        s = glade_xml_get_widget(glade, "dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
    }
}

void on_dock_right_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "Right");

    {
        GtkWidget *s;
        s = glade_xml_get_widget(glade, "dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
        
    }
}

void on_dock_bottom_left_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "BottomLeft");

    {
        GtkWidget *s;
        s = glade_xml_get_widget(glade, "dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
    }
}

void on_dock_bottom_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "Bottom");

    {
        GtkWidget *s;
        s = glade_xml_get_widget(glade, "dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
    }
}

void on_dock_bottom_right_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "BottomRight");

    {
        GtkWidget *s;
        s = glade_xml_get_widget(glade, "dock_float_x");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_y");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label");
        gtk_widget_set_sensitive(s, FALSE);
        s = glade_xml_get_widget(glade, "dock_float_label_x");
        gtk_widget_set_sensitive(s, FALSE);
    }
}

void on_dock_floating_activate(GtkMenuItem *w, gpointer data)
{
    if (mapping) return;

    tree_set_string("dock/position", "Floating");

    {
        GtkWidget *s;
        s = glade_xml_get_widget(glade, "dock_float_x");
        gtk_widget_set_sensitive(s, TRUE);
        s = glade_xml_get_widget(glade, "dock_float_y");
        gtk_widget_set_sensitive(s, TRUE);
         s = glade_xml_get_widget(glade, "dock_float_label");
        gtk_widget_set_sensitive(s, TRUE);
         s = glade_xml_get_widget(glade, "dock_float_label_x");
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

void on_theme_name_changed(GtkOptionMenu *w, gpointer data)
{
    const gchar *name;

    if (mapping) return;

    name = g_list_nth_data(themes, gtk_option_menu_get_history(w));

    if (name)
        tree_set_string("theme/name", name);
}

void on_theme_names_selection_changed(GtkTreeSelection *sel, gpointer data)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    const gchar *name;

    if (mapping) return;

    if(gtk_tree_selection_get_selected(sel, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 0, &name, -1);
    }

    if(name)
      tree_set_string("theme/name", name);
}

void on_title_layout_changed(GtkEntry *w, gpointer data)
{
    gchar *layout;
    gchar *it, *it2;
    gboolean n, d, s, l, i, m, c;

    if (mapping) return;

    layout = g_strdup(gtk_entry_get_text(w));

    n = d = s = l = i = m = c = FALSE;

    for (it = layout; *it; ++it) {
        gboolean *b;

        switch (*it) {
        case 'N':
        case 'n':
            b = &n;
            break;
        case 'd':
        case 'D':
            b = &d;
            break;
        case 's':
        case 'S':
            b = &s;
            break;
        case 'l':
        case 'L':
            b = &l;
            break;
        case 'i':
        case 'I':
            b = &i;
            break;
        case 'm':
        case 'M':
            b = &m;
            break;
        case 'c':
        case 'C':
            b = &c;
            break;
        default:
            b = NULL;
            break;
        }

        if (!b || *b) {
            /* drop the letter */
            for (it2 = it; *it2; ++it2)
                *it2 = *(it2+1);
        } else {
            *it = toupper(*it);
            *b = TRUE;
        }
    }

    gtk_entry_set_text(w, layout);
    tree_set_string("theme/titleLayout", layout);
    g_free(layout);
}

static void set_desktop_names()
{
    gchar **s;
    GList *lit;
    xmlNodePtr n, c;
    gint num = 0, last = -1;

    n = tree_get_node("desktops/names", NULL);
    while ((c = n->children)) {
        xmlUnlinkNode(c);
        xmlFreeNode(c);
    }

    for (lit = desktop_names; lit; lit = g_list_next(lit)) {
        if (((gchar*)lit->data)[0]) /* not empty */
            last = num;
        ++num;
    }

    num = 0;
    for (lit = desktop_names; lit && num <= last; lit = g_list_next(lit)) {
        xmlNewTextChild(n, NULL, "name", lit->data);
        ++num;
    }
    tree_apply();

    /* make openbox re-set the property */
    XDeleteProperty(GDK_DISPLAY(), GDK_ROOT_WINDOW(),
                    XInternAtom(GDK_DISPLAY(), "_NET_DESKTOP_NAMES", FALSE));
}

static void set_desktop_number()
{
    XEvent ce;

    tree_set_int("desktops/number", num_desktops);

    ce.xclient.type = ClientMessage;
    ce.xclient.message_type = XInternAtom(GDK_DISPLAY(),
                                          "_NET_NUMBER_OF_DESKTOPS",
                                          FALSE);
    ce.xclient.display = GDK_DISPLAY();
    ce.xclient.window = GDK_ROOT_WINDOW();
    ce.xclient.format = 32;
    ce.xclient.data.l[0] = num_desktops;
    ce.xclient.data.l[1] = 0;
    ce.xclient.data.l[2] = 0;
    ce.xclient.data.l[3] = 0;
    ce.xclient.data.l[4] = 0;
    XSendEvent(GDK_DISPLAY(), GDK_ROOT_WINDOW(), FALSE,
               SubstructureNotifyMask | SubstructureRedirectMask,
               &ce);
}

void on_desktop_num_value_changed(GtkSpinButton *w, gpointer data)
{
    if (mapping) return;

    num_desktops = gtk_spin_button_get_value(w);

    set_desktop_number();

    reset_desktop_names();
}

static void on_desktop_names_cell_edited(GtkCellRendererText *cell,
                                         const gchar *path_string,
                                         const gchar *new_text,
                                         gpointer data)
{
    if (mapping) return;

    GtkTreePath *path = gtk_tree_path_new_from_string (path_string);
    GtkTreeIter it;
    gchar *old_text;
    GList *lit;
    gint i;

    gtk_tree_model_get_iter(GTK_TREE_MODEL(desktop_store), &it, path);

    gtk_tree_model_get(GTK_TREE_MODEL(desktop_store), &it, 0, &old_text, -1);
    g_free(old_text);

    i = gtk_tree_path_get_indices(path)[0];
    lit = g_list_nth(desktop_names, i);

    g_free(lit->data);
    lit->data = g_strdup(new_text);
    if (new_text[0]) /* not empty */
        gtk_list_store_set(desktop_store, &it, 0, lit->data, -1);
    else
        gtk_list_store_set(desktop_store, &it, 0, _("(Unnamed desktop)"), -1);

    set_desktop_names();
}
