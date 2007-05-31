/* -*- indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*-

   keybindings.c for ObConf, the configuration tool for Openbox
   Copyright (c) 2007        Dana Jansens

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

static gboolean      mapping = FALSE;
static gchar        *saved_text = NULL;
static GtkListStore *binding_store;

static gboolean validate_key(const gchar *s);
static void on_key_cell_edited(GtkCellRendererText *cell,
                               const gchar *path_string,
                               const gchar *new_text,
                               gpointer data);

void keybindings_setup_tab()
{
    GtkWidget *w;
    GtkCellRenderer *render;
    GtkTreeViewColumn *column;
    GtkTreeSelection *select;
    gchar *s;

    mapping = TRUE;

    w = get_widget("chain_quit_key");
    s = tree_get_string("keyboard/chainQuitKey", "C-g");
    gtk_entry_set_text(GTK_ENTRY(w), s);
    g_free(s);

    /* widget setup */
    w = get_widget("key_bindings");
    binding_store = gtk_list_store_new(2, G_TYPE_STRING, GTK_TYPE_COMBO_BOX);
    gtk_tree_view_set_model(GTK_TREE_VIEW(w), GTK_TREE_MODEL(binding_store));
    g_object_unref (binding_store);

    gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(w)),
                                GTK_SELECTION_SINGLE);

    /* text column for the keys */
    render = gtk_cell_renderer_text_new();
    g_signal_connect(render, "edited",
                     G_CALLBACK(on_key_cell_edited), NULL);
    column = gtk_tree_view_column_new_with_attributes
        ("Key", render, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(w), column);

    /* combo box column, for the actions */
    render = gtk_cell_renderer_combo_new();
    column = gtk_tree_view_column_new_with_attributes
        ("Action", render, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(w), column);

    mapping = FALSE;
}

static void on_key_cell_edited(GtkCellRendererText *cell,
                               const gchar *path_string,
                               const gchar *new_text,
                               gpointer data)
{
    if (mapping) return;

    if (!validate_key(new_text)) {
        g_print("bad key binding: %s\n", new_text);
    } else {
        GtkTreePath *path;
        GtkTreeIter it;
        gchar *old_text;

        path = gtk_tree_path_new_from_string (path_string);
        gtk_tree_model_get_iter(GTK_TREE_MODEL(binding_store), &it, path);

        gtk_tree_model_get(GTK_TREE_MODEL(binding_store), &it, 0,
                           &old_text, -1);
        g_free(old_text);

        gtk_list_store_set(binding_store, &it, 0, new_text, -1);

        //tree_set_string("keyboard/keybind:key=%s", s);
    }
}

void on_chain_quit_key_focus_in(GtkEntry *w, gpointer data)
{
    g_assert(saved_text == NULL);
    saved_text = g_strdup(gtk_entry_get_text(w));
}

void on_chain_quit_key_focus_out(GtkEntry *w, gpointer data)
{
    const gchar *s;

    if (mapping) return;

    s = gtk_entry_get_text(w);
    if (!validate_key(s)) {
        g_print("bad key binding: %s\n", s);
        gtk_entry_set_text(w, saved_text);
    } else
        tree_set_string("keyboard/chainQuitKey", s);

    g_free(saved_text);
    saved_text = NULL;
}

static gboolean validate_key(const gchar *s)
{
    const gchar *next;
    const gchar *valid[] = { "Mod1", "Mod2", "Mod3", "Mod4", "Mod5",
                             "Control", "C", "Alt", "A", "Meta", "M",
                             "Super", "W", "Shift", "S", "Hyper", "H",
                             NULL };

    while ((next = strchr(s, '-'))) {
        /* it's a modifier, validate it */
        const gchar **it;
        gboolean found = FALSE;

        for (it = valid; *it && !found; ++it)
            if (!g_ascii_strncasecmp(*it, s, strlen(*it)))
                found = TRUE;

        if (!found) {
            g_print("Invalid modifier\n");
            return FALSE;
        }

        s = next + 1; /* skip past the '-' */
    }
    /* we're at the real key part */
    if (!gdk_keyval_from_name(s)) {
        g_print("Invalid key: %s\n", s);
        return FALSE;
    }

    return TRUE;
}
