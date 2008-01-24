/* -*- indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*-

   keyboard.c for ObConf, the configuration tool for Openbox
   Copyright (c) 2007   Justin Stallard

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
#include "keyboard.h"
#include "actions.h"

#include "gettext.h"

#include <openbox/parse.h>

#define SHIFT   "Shift"
#define CONTROL "Control"
#define ALT     "Alt"
#define SUPER   "Super"
#define META    "Meta"
#define HYPER   "Hyper"

enum
{
    KEY_COL,
    ACTION_COL,
    OPTION_COL,
    OVAL_STR_COL,
    OVAL_BOOL_COL,
    OVAL_INT_COL,
    OVAL_TYPE_STR_COL,
    OVAL_TYPE_BOOL_COL,
    OVAL_TYPE_INT_COL,
    EDITABLE_COL,
    NUM_COLS
};

static GtkTreeStore *key_store;
static GtkListStore *action_store;

static guint chain_press_id;
static guint chain_release_id;
static guint key_press_id;
static guint key_release_id;


gboolean on_chain_quit_key_key_release_event(GtkWidget *w,
                                             GdkEventKey *event,
                                             gpointer data);
gboolean on_chain_quit_key_grab_key_press(GtkWidget *w,
                                          GdkEventKey *event,
                                          gpointer data);
gboolean on_chain_quit_key_grab_key_release(GtkWidget *w,
                                            GdkEventKey *event,
                                            gpointer data);
gboolean on_key_bindings_key_press_event(GtkWidget *w,
                                         GdkEventKey *event,
                                         gpointer it);
gboolean on_key_bindings_key_release_event(GtkWidget *w,
                                           GdkEventKey *event,
                                           gpointer it);
void on_tool_edit_clicked(GtkToolButton *w,
                          gpointer data);
void on_tool_add_chain_clicked(GtkToolButton *w,
                               gpointer data);
void on_tool_new_clicked(GtkToolButton *w,
                         gpointer data);
void on_tool_add_action_clicked(GtkToolButton *w,
                                gpointer data);
void on_tool_delete_clicked(GtkToolButton *w,
                            gpointer data);
void on_chain_quit_key_changed(GtkEntry *w, gpointer data);
void on_action_edited(GtkCellRendererText *w,
                      const gchar *path_string,
                      const gchar *new_text,
                      gpointer data);
void on_cell_renderer_toggle_toggled(GtkCellRendererToggle *w,
                                     const gchar *path_string,
                                     gpointer data);
void on_cell_renderer_text_edited(GtkCellRendererText *w,
                                  const gchar *path_string,
                                  const gchar *new_text,
                                  gpointer data);
void on_cell_renderer_spin_edited(GtkCellRendererText *w,
                                  const gchar *path_string,
                                  const gchar *new_text,
                                  gpointer data);

void keyboard_option_add_new(GtkTreeIter *parent,
                             const gchar *oname,
                             GType otype,
                             gpointer oval,
                             GtkTreeIter *newret);
void keyboard_key_store_add_default_options(GtkTreeIter *it);
void keyboard_fill_key_store(GtkTreeIter *parent, xmlNode *a_node);
void keyboard_write_bindings();
void keyboard_fill_action_store();



gboolean on_chain_quit_key_key_release_event(GtkWidget *w,
                                             GdkEventKey *event,
                                             gpointer data)
{
    GdkKeymapKey key;
    gchar *key_name;

    key.keycode = event->hardware_keycode;
    key.group = 0;
    key.level = 0;

    key_name = gdk_keyval_name(gdk_keymap_lookup_key(NULL, &key));

    /* if key pressed is not "Return", do nothing and let the default handler
     * take action. */
    if (strncmp(key_name, "Return", strlen("Return")))
    {
        return FALSE;
    } else {
        /* "Return" was pressed...prepare for user to set a keybinding */

        /* grab the keyboard */
        gdk_keyboard_grab(gtk_widget_get_parent_window(w), FALSE, event->time);

        /* disable current handler */
        g_signal_handler_disconnect(w, chain_release_id);

        /* enable new handler, along with key-release-event handler */
        chain_press_id = g_signal_connect
                                (w, "key-press-event",
                                 G_CALLBACK(on_chain_quit_key_grab_key_press),
                                 NULL);
        chain_release_id = g_signal_connect
                               (w, "key-release-event",
                                G_CALLBACK(on_chain_quit_key_grab_key_release),
                                NULL);

        /* default handler does not run */
        return TRUE;
    }
}

gboolean on_chain_quit_key_grab_key_press(GtkWidget *w,
                                          GdkEventKey *event,
                                          gpointer data)
{
    gchar c[3];
    GdkKeymapKey key;
    gchar *key_name;
    GString *txt;

    key.keycode = event->hardware_keycode;
    key.group = 0;
    key.level = 0;

    key_name = gdk_keyval_name(gdk_keymap_lookup_key(NULL, &key));

    c[1] = '-';
    c[2] = '\0';

    txt = g_string_new(gtk_entry_get_text(GTK_ENTRY(w)));

    /* if the last char in the entry is not a '-', then the user has started
     * entering the new keybinding, so clear the entry first...              */
    if (txt->str[txt->len - 1] != (gchar) '-')
        gtk_entry_set_text(GTK_ENTRY(w), "");

    if (!strncmp(key_name, SHIFT, strlen(SHIFT)))
        c[0] = 'S';
    else if (!strncmp(key_name, CONTROL, strlen(CONTROL)))
        c[0] = 'C';
    else if (!strncmp(key_name, ALT, strlen(ALT)))
        c[0] = 'A';
    else if (!strncmp(key_name, SUPER, strlen(SUPER)))
        c[0] = 'W';
    else if (!strncmp(key_name, META, strlen(SUPER)))
        c[0] = 'C';
    else if (!strncmp(key_name, HYPER, strlen(HYPER)))
        c[0] = 'H';
    else {
        c[0] = '\0';
        gtk_entry_append_text(GTK_ENTRY(w), key_name);
    }
    gtk_entry_append_text(GTK_ENTRY(w), c);


    return TRUE;
}

gboolean on_chain_quit_key_grab_key_release(GtkWidget *w,
                                            GdkEventKey *event,
                                            gpointer data)
{
    GdkKeymapKey key;
    gchar *key_name;
    gchar *txt;
    gint len;

    key.keycode = event->hardware_keycode;
    key.group = 0;
    key.level = 0;

    key_name = gdk_keyval_name(gdk_keymap_lookup_key(NULL, &key));
    len = strlen(gtk_entry_get_text(GTK_ENTRY(w)));
    txt = malloc(sizeof(gchar) * (len + 1));
    strcpy(txt, gtk_entry_get_text(GTK_ENTRY(w)));

    len--;
    if (txt[len] == (gchar) '-') {
        len--;
        while (len >= 0 && txt[len] != '-')
        {
            len--;
        }
        txt[len + 1] = '\0';

        gtk_entry_set_text(GTK_ENTRY(w), txt);
        gtk_entry_append_text(GTK_ENTRY(w), key_name);
    }

    g_free(txt);

    /* done getting keybinding */

    /* ungrab keyboard */
    gdk_keyboard_ungrab(event->time);

    /* disable current handler */
    g_signal_handler_disconnect(w, chain_press_id);
    g_signal_handler_disconnect(w, chain_release_id);

    /* re-enable old handler */
    chain_release_id = g_signal_connect
                            (w, "key-release-event",
                             G_CALLBACK(on_chain_quit_key_key_release_event),
                             NULL);

    return TRUE;
}

gboolean on_key_bindings_key_press_event(GtkWidget *w,
                                         GdkEventKey *event,
                                         gpointer it)
{
    gchar c[3];
    GdkKeymapKey key;
    gchar *key_name;
    gchar *txt;
    gchar *new_text;
    gint len;

    key.keycode = event->hardware_keycode;
    key.group = 0;
    key.level = 0;

    key_name = gdk_keyval_name(gdk_keymap_lookup_key(NULL, &key));

    c[1] = '-';
    c[2] = '\0';

    gtk_tree_model_get(GTK_TREE_MODEL(key_store), it, KEY_COL, &txt, -1);
    len = strlen(txt);

    /* if the last char in the entry is not a '-', then the user has started
     * entering the new keybinding, so clear the entry first...              */
    if (txt[len - 1] != (gchar) '-')
    {
        g_free(txt);
        gtk_tree_store_set(key_store, (GtkTreeIter *) it,
                           KEY_COL, "", -1);
    }

    if (!strncmp(key_name, SHIFT, strlen(SHIFT)))
    {
        c[0] = 'S';
    }
    else if (!strncmp(key_name, CONTROL, strlen(CONTROL)))
    {
        c[0] = 'C';
    }
    else if (!strncmp(key_name, ALT, strlen(ALT)))
    {
        c[0] = 'A';
    }
    else if (!strncmp(key_name, SUPER, strlen(SUPER)))
    {
        c[0] = 'W';
    }
    else if (!strncmp(key_name, META, strlen(SUPER)))
    {
        c[0] = 'C';
    }
    else if (!strncmp(key_name, HYPER, strlen(HYPER)))
    {
        c[0] = 'H';
    }
    else
    {
        c[0] = '\0';
        gtk_tree_model_get(GTK_TREE_MODEL(key_store), it, KEY_COL, &txt, -1);
        new_text = g_strconcat(txt, key_name, NULL);
        g_free(txt);
        gtk_tree_store_set(key_store, (GtkTreeIter *) it,
                           KEY_COL, new_text, -1);
    }

    gtk_tree_model_get(GTK_TREE_MODEL(key_store), it, KEY_COL, &txt, -1);
    new_text = g_strconcat(txt, c, NULL);
    g_free(txt);
    gtk_tree_store_set(key_store, (GtkTreeIter *) it,
                       KEY_COL, new_text, -1);


    return TRUE;
}

gboolean on_key_bindings_key_release_event(GtkWidget *w,
                                           GdkEventKey *event,
                                           gpointer it)
{
    GdkKeymapKey key;
    gchar *key_name;
    gchar *txt;
    gchar *old_text;
    gchar *new_text;
    gint len;

    key.keycode = event->hardware_keycode;
    key.group = 0;
    key.level = 0;

    key_name = gdk_keyval_name(gdk_keymap_lookup_key(NULL, &key));
    gtk_tree_model_get(GTK_TREE_MODEL(key_store), it, KEY_COL, &txt, -1);
    len = strlen(txt);
    old_text = g_strdup(txt);

    len--;
    if (old_text[len] == (gchar) '-') {
        len--;
        while (len >= 0 && old_text[len] != '-')
        {
            len--;
        }
        old_text[len + 1] = '\0';

        new_text = g_strconcat(old_text, key_name, NULL);
        gtk_tree_store_set(key_store, (GtkTreeIter *) it,
                           KEY_COL, new_text, -1);
        g_free(txt);
    }

    g_free(old_text);

    /* done getting keybinding */

    /* ungrab keyboard */
    gdk_keyboard_ungrab(event->time);

    g_free(it);

    /* disable current handler */
    g_signal_handler_disconnect(w, key_press_id);
    g_signal_handler_disconnect(w, key_release_id);

    keyboard_write_bindings();

    return TRUE;
}

void on_tool_edit_clicked(GtkToolButton *w,
                          gpointer data)
{
    GtkWidget *entry;
    GtkWidget *tv;
    GtkTreeIter *it;
    gchar *text;

    tv = get_widget("key_bindings");
    it = g_malloc(sizeof(GtkTreeIter));
    if (!gtk_tree_selection_get_selected
            (gtk_tree_view_get_selection(GTK_TREE_VIEW(tv)), NULL, it))
    {
        g_free(it);
        return;
    }

    /* make sure selection is a keybinding */
    gtk_tree_model_get(GTK_TREE_MODEL(key_store), it, ACTION_COL, &text, -1);

    if (text != NULL)
    {
        g_free(it);
        return;
    }

    gtk_tree_model_get(GTK_TREE_MODEL(key_store), it, OPTION_COL, &text, -1);

    if (text != NULL)
    {
        g_free(it);
        return;
    }

    gtk_tree_view_scroll_to_cell
        (GTK_TREE_VIEW(tv),
         gtk_tree_model_get_path(GTK_TREE_MODEL(key_store), it),
         NULL, TRUE, .5, 0);

    gtk_widget_grab_focus(tv);

    /* etc, etc */
    key_press_id = g_signal_connect
                               (tv, "key-press-event",
                                G_CALLBACK(on_key_bindings_key_press_event),
                                it);
    key_release_id = g_signal_connect
                               (tv, "key-release-event",
                                G_CALLBACK(on_key_bindings_key_release_event),
                                it);

    gtk_tree_model_get(GTK_TREE_MODEL(key_store), it, KEY_COL, &text, -1);
    g_free(text);
    gtk_tree_store_set(key_store, (GtkTreeIter *) it,
                       KEY_COL, "Enter Keybinding", -1);

    gdk_keyboard_grab
        (gtk_widget_get_parent_window(tv), FALSE, GDK_CURRENT_TIME);
}

void on_tool_add_chain_clicked(GtkToolButton *w,
                               gpointer data)
{
    GtkWidget *entry;
    GtkWidget *tv;
    GtkTreeIter it, parent, new;
    gchar *text;

    tv = get_widget("key_bindings");
    if (!gtk_tree_selection_get_selected
            (gtk_tree_view_get_selection(GTK_TREE_VIEW(tv)), NULL, &it))
    {
        return;
    }

    /* make sure selection is a keybinding */
    gtk_tree_model_get(GTK_TREE_MODEL(key_store), &it, OPTION_COL, &text, -1);

    if (text != NULL)
    {
        return;
    }

    gtk_tree_model_get(GTK_TREE_MODEL(key_store), &it, ACTION_COL, &text, -1);

    if (text != NULL)
    {
        return;
    }

    /* ensure this keybinding doesn't have any actions */
    parent = it;
    if (gtk_tree_model_iter_children
            (GTK_TREE_MODEL(key_store), &it, &parent))
    {
        do
        {
            gtk_tree_model_get
                (GTK_TREE_MODEL(key_store), &it, ACTION_COL, &text, -1);

            if (text != NULL)
            {
                return;
            }
        } while (gtk_tree_model_iter_next(GTK_TREE_MODEL(key_store), &it));
    }

    /* add the keychain */

    gtk_tree_store_append(key_store, &new, &parent);

    gtk_tree_view_expand_to_path
        (GTK_TREE_VIEW(tv),
         gtk_tree_model_get_path(GTK_TREE_MODEL(key_store), &new));
    gtk_tree_view_scroll_to_cell
        (GTK_TREE_VIEW(tv),
         gtk_tree_model_get_path(GTK_TREE_MODEL(key_store), &new),
         NULL, FALSE, 0, 0);
    gtk_tree_selection_select_iter
        (gtk_tree_view_get_selection(GTK_TREE_VIEW(tv)), &new);

    on_tool_edit_clicked(NULL, NULL);
}

void on_tool_new_clicked(GtkToolButton *w,
                         gpointer data)
{
    GtkWidget *entry;
    GtkWidget *tv;
    GtkTreeIter it, parent, new;
    gchar *text;

    tv = get_widget("key_bindings");
    if (!gtk_tree_selection_get_selected
            (gtk_tree_view_get_selection(GTK_TREE_VIEW(tv)), NULL, &it))
    {
        return;
    }

    /* deal with the topmost ancestor of the selection */
    while (gtk_tree_store_iter_depth(key_store, &it) > 0)
    {
        gtk_tree_model_iter_parent(GTK_TREE_MODEL(key_store), &parent, &it);
        it = parent;
    }

    /* insert new keybinding */
    gtk_tree_store_insert_after(key_store, &new, NULL, &it);

    gtk_tree_view_expand_to_path
        (GTK_TREE_VIEW(tv),
         gtk_tree_model_get_path(GTK_TREE_MODEL(key_store), &new));
    gtk_tree_view_scroll_to_cell
        (GTK_TREE_VIEW(tv),
         gtk_tree_model_get_path(GTK_TREE_MODEL(key_store), &new),
         NULL, FALSE, 0, 0);
    gtk_tree_selection_select_iter
        (gtk_tree_view_get_selection(GTK_TREE_VIEW(tv)), &new);

    on_tool_edit_clicked(NULL, NULL);
}

void on_tool_add_action_clicked(GtkToolButton *w,
                                gpointer data)
{
    GtkWidget *entry;
    GtkWidget *tv;
    GtkTreeIter it, parent, new;
    gchar *text;

    tv = get_widget("key_bindings");
    if (!gtk_tree_selection_get_selected
            (gtk_tree_view_get_selection(GTK_TREE_VIEW(tv)), NULL, &it))
    {
        return;
    }

    /* make sure selection is a keybinding or action */
    gtk_tree_model_get(GTK_TREE_MODEL(key_store), &it, OPTION_COL, &text, -1);

    if (text != NULL)
    {
        gtk_tree_model_iter_parent(GTK_TREE_MODEL(key_store), &parent, &it);
        it = parent;
    }

    gtk_tree_model_get(GTK_TREE_MODEL(key_store), &it, ACTION_COL, &text, -1);

    if (text != NULL)
    {
        gtk_tree_store_insert_after(key_store, &new, NULL, &it);

        text = action_get_text_from_name(0);
        gtk_tree_store_set(key_store, &new,
                                            ACTION_COL, text,
                                            EDITABLE_COL, TRUE,
                                            -1);
        g_free(text);
        keyboard_key_store_add_default_options(&new);
    }
    else
    {
        /* The selection must be a keybinding */
        parent = it;
        if (gtk_tree_model_iter_children
                (GTK_TREE_MODEL(key_store), &it, &parent))
        {
            /* keychains can't have actions, just keys */
            do
            {
                gtk_tree_model_get
                    (GTK_TREE_MODEL(key_store), &it, KEY_COL, &text, -1);

                if (text != NULL)
                {
                    return;
                }
            } while (gtk_tree_model_iter_next(GTK_TREE_MODEL(key_store), &it));
        }

        gtk_tree_store_insert_after(key_store, &new, &parent, NULL);

        text = action_get_text_from_name(0);
        gtk_tree_store_set(key_store, &new,
                                            ACTION_COL, text,
                                            EDITABLE_COL, TRUE,
                                            -1);
        g_free(text);
        keyboard_key_store_add_default_options(&new);
    }

    gtk_tree_view_expand_to_path
        (GTK_TREE_VIEW(tv),
         gtk_tree_model_get_path(GTK_TREE_MODEL(key_store), &new));
    gtk_tree_view_scroll_to_cell
        (GTK_TREE_VIEW(tv),
         gtk_tree_model_get_path(GTK_TREE_MODEL(key_store), &new),
         NULL, FALSE, 0, 0);
    gtk_tree_selection_select_iter
        (gtk_tree_view_get_selection(GTK_TREE_VIEW(tv)), &new);

    keyboard_write_bindings();
}

void on_tool_delete_clicked(GtkToolButton *w,
                            gpointer data)
{
    GtkWidget *entry;
    GtkWidget *tv;
    GtkTreeIter it;
    gchar *text;

    tv = get_widget("key_bindings");
    if (!gtk_tree_selection_get_selected
            (gtk_tree_view_get_selection(GTK_TREE_VIEW(tv)), NULL, &it))
    {
        return;
    }

    /* make sure selection is a keybinding or action */
    gtk_tree_model_get(GTK_TREE_MODEL(key_store), &it, OPTION_COL, &text, -1);

    if (text != NULL)
    {
        return;
    }

    gtk_tree_store_remove(key_store, &it);

    keyboard_write_bindings();
}

void on_chain_quit_key_changed(GtkEntry *w, gpointer data)
{
    gchar *chain;

    chain = g_strdup(gtk_entry_get_text(w));
    tree_set_string("keyboard/chainQuitKey", chain);

    g_free(chain);
}

void on_action_edited(GtkCellRendererText *w,
                      const gchar *path_string,
                      const gchar *new_text,
                      gpointer data)
{
    GtkTreePath *path;
    GtkTreeIter it, child;
    gchar *old_text;
    GtkWidget *tv;

    path = gtk_tree_path_new_from_string(path_string);
    gtk_tree_model_get_iter(GTK_TREE_MODEL(key_store), &it, path);

    gtk_tree_model_get(GTK_TREE_MODEL(key_store), &it,
                       ACTION_COL, &old_text, -1);

    if (!strcmp(new_text, old_text))
        return;

    g_free(old_text);

    gtk_tree_store_set(key_store, &it, ACTION_COL, g_strdup(new_text), -1);

    /* remove old options */
    if (gtk_tree_model_iter_children(GTK_TREE_MODEL(key_store), &child, &it))
    {
        while (gtk_tree_store_remove(key_store, &child));
    }

    /* add default options */
    keyboard_key_store_add_default_options(&it);

    /* expand so options are visible*/
    tv = get_widget("key_bindings");
    gtk_tree_view_expand_row(GTK_TREE_VIEW(tv), path, FALSE);

    keyboard_write_bindings();
}

void on_cell_renderer_toggle_toggled(GtkCellRendererToggle *w,
                                     const gchar *path_string,
                                     gpointer data)
{
    GtkTreePath *path;
    GtkTreeIter iter;
    gboolean oldval;
    gboolean tmp;

    path = gtk_tree_path_new_from_string(path_string);
    gtk_tree_model_get_iter(GTK_TREE_MODEL(key_store), &iter, path);

    gtk_tree_model_get(GTK_TREE_MODEL(key_store), &iter,
                       OVAL_BOOL_COL, &oldval,
                       -1);

    gtk_tree_store_set(key_store, &iter, OVAL_BOOL_COL, !oldval, -1);

    keyboard_write_bindings();
}

void on_cell_renderer_text_edited(GtkCellRendererText *w,
                                  const gchar *path_string,
                                  const gchar *new_text,
                                  gpointer data)
{
    GtkTreePath *path;
    GtkTreeIter it;
    gchar *old_text;

    path = gtk_tree_path_new_from_string(path_string);
    gtk_tree_model_get_iter(GTK_TREE_MODEL(key_store), &it, path);

    gtk_tree_model_get(GTK_TREE_MODEL(key_store), &it, 0, &old_text, -1);
    g_free(old_text);

    gtk_tree_store_set(key_store, &it, OVAL_STR_COL, g_strdup(new_text), -1);

    keyboard_write_bindings();
}

void on_cell_renderer_spin_edited(GtkCellRendererText *w,
                                  const gchar *path_string,
                                  const gchar *new_text,
                                  gpointer data)
{
    GtkTreePath *path;
    GtkTreeIter it;
    gchar *old_text;
    const gchar *c;

    for (c = new_text; *c != '\0'; c++)
    {
        if (!g_ascii_isdigit(*c))
            return;
    }

    path = gtk_tree_path_new_from_string(path_string);
    gtk_tree_model_get_iter(GTK_TREE_MODEL(key_store), &it, path);

    gtk_tree_model_get(GTK_TREE_MODEL(key_store), &it, 0, &old_text, -1);
    g_free(old_text);

    gtk_tree_store_set(key_store, &it, OVAL_STR_COL, g_strdup(new_text), -1);

    keyboard_write_bindings();
}


void keyboard_option_add_new(GtkTreeIter *parent,
                             const gchar *oname,
                             GType otype,
                             gpointer oval,
                             GtkTreeIter *newret)
{
    GtkTreeIter new;
    GtkAdjustment *adj;
    gchar buf[16];

    gtk_tree_store_append(key_store, &new, parent);

    switch (otype)
    {
    case G_TYPE_NONE:
        gtk_tree_store_set(key_store, &new,
                           OPTION_COL, oname,
                           -1);
        break;
    case G_TYPE_STRING:
        gtk_tree_store_set(key_store, &new,
                           OPTION_COL, oname,
                           OVAL_STR_COL, (gchar *) oval,
                           OVAL_TYPE_STR_COL, TRUE,
                           EDITABLE_COL, FALSE,
                           -1);
        break;
    case G_TYPE_BOOLEAN:
        gtk_tree_store_set(key_store, &new,
                           OPTION_COL, oname,
                           OVAL_BOOL_COL, *((gboolean*)oval),
                           OVAL_TYPE_BOOL_COL, TRUE,
                           EDITABLE_COL, FALSE,
                           -1);
        break;
    case G_TYPE_INT:
        adj = GTK_ADJUSTMENT(gtk_adjustment_new
                             (*((gint*)oval),
                              -5000, 5000, 1, 1, 0));
        g_ascii_dtostr(buf, 16, (gdouble)*((gint*)oval));
        gtk_tree_store_set(key_store, &new,
                           OPTION_COL, oname,
                           OVAL_INT_COL, G_OBJECT(adj),
                           OVAL_STR_COL, buf,
                           OVAL_TYPE_INT_COL, TRUE,
                           EDITABLE_COL, FALSE,
                           -1);
        break;
    }
    if (newret) *newret = new;
}

void keyboard_key_store_add_default_options(GtkTreeIter *it)
{
    gchar *atext;
    Action action;
    GtkTreeIter chld;
    gboolean b;
    gint i;

    gtk_tree_model_get(GTK_TREE_MODEL(key_store), it, ACTION_COL, &atext, -1);

    action = action_get_name_from_text(atext);

    switch (action) {
    case A_EXECUTE:
        keyboard_option_add_new(it, "execute", G_TYPE_STRING, "", NULL);
        keyboard_option_add_new(it, "startupnotify", G_TYPE_NONE, NULL, &chld);
        b = FALSE;
        keyboard_option_add_new(&chld, "enabled", G_TYPE_BOOLEAN, &b, NULL);
        keyboard_option_add_new(&chld, "wmclass", G_TYPE_STRING, "", NULL);
        keyboard_option_add_new(&chld, "name", G_TYPE_STRING, "", NULL);
        keyboard_option_add_new(&chld, "icon", G_TYPE_STRING, "", NULL);
        break;
    case A_SHOW_MENU:
        keyboard_option_add_new(it, "menu", G_TYPE_STRING, "", NULL);
        break;
    case A_NEXT_WINDOW:
    case A_PREVIOUS_WINDOW:
        b = TRUE;
        keyboard_option_add_new(it, "dialog", G_TYPE_BOOLEAN, &b, NULL);
        b = FALSE;
        keyboard_option_add_new(it, "allDesktops", G_TYPE_BOOLEAN, &b, NULL);
        keyboard_option_add_new(it, "panels", G_TYPE_BOOLEAN, &b, NULL);
        keyboard_option_add_new(it, "desktop", G_TYPE_BOOLEAN, &b, NULL);
        keyboard_option_add_new(it, "linear", G_TYPE_BOOLEAN, &b, NULL);
        break;
    case A_DIRECTIONAL_FOCUS_NORTH:
    case A_DIRECTIONAL_FOCUS_SOUTH:
    case A_DIRECTIONAL_FOCUS_EAST:
    case A_DIRECTIONAL_FOCUS_WEST:
    case A_DIRECTIONAL_FOCUS_NORTH_WEST:
    case A_DIRECTIONAL_FOCUS_NORTH_EAST:
    case A_DIRECTIONAL_FOCUS_SOUTH_WEST:
    case A_DIRECTIONAL_FOCUS_SOUTH_EAST:
        b = TRUE;
        keyboard_option_add_new(it, "dialog", G_TYPE_BOOLEAN, &b, NULL);
        break;
    case A_DESKTOP:
        i = 1;
        keyboard_option_add_new(it, "desktop", G_TYPE_INT, &i, NULL);
        break;
    case A_DESKTOP_NEXT:
    case A_DESKTOP_PREVIOUS:
    case A_DESKTOP_LEFT:
    case A_DESKTOP_RIGHT:
    case A_DESKTOP_UP:
    case A_DESKTOP_DOWN:
        b = TRUE;
        keyboard_option_add_new(it, "dialog", G_TYPE_BOOLEAN, &b, NULL);
        keyboard_option_add_new(it, "wrap", G_TYPE_BOOLEAN, &b, NULL);
        break;
    case A_DESKTOP_LAST:
    case A_ADD_DESKTOP_LAST:
    case A_REMOVE_DESKTOP_LAST:
    case A_ADD_DESKTOP_CURRENT:
    case A_REMOVE_DESKTOP_CURRENT:
    case A_TOGGLE_SHOW_DESKTOP:
    case A_SHOW_DESKTOP:
    case A_UNSHOW_DESKTOP:
    case A_TOGGLE_DOCK_AUTOHIDE:
    case A_RECONFIGURE:
    case A_RESTART:
    case A_EXIT:
        break;
    case A_DEBUG:
        keyboard_option_add_new(it, "string", G_TYPE_STRING, "", NULL);
        break;

    case A_ACTIVATE:
    case A_FOCUS:
    case A_RAISE:
    case A_LOWER:
    case A_RAISE_LOWER:
    case A_UNFOCUS:
    case A_FOCUS_TO_BOTTOM:
    case A_ICONIFY:
    case A_CLOSE:
    case A_TOGGLE_SHADE:
    case A_SHADE:
    case A_UNSHADE:
    case A_TOGGLE_OMNIPRESENT:
    case A_TOGGLE_MAXIMIZE_FULL:
    case A_MAXIMIZE_FULL:
    case A_UNMAXIMIZE_FULL:
    case A_TOGGLE_MAXIMIZE_VERT:
    case A_MAXIMIZE_VERT:
    case A_UNMAXIMIZE_VERT:
    case A_TOGGLE_MAXIMIZE_HORZ:
    case A_MAXIMIZE_HORZ:
    case A_UNMAXIMIZE_HORZ:
    case A_TOGGLE_FULLSCREEN:
    case A_TOGGLE_DECORATIONS:
        break;
    case A_SEND_TO_DESKTOP:
        i = 1;
        keyboard_option_add_new(it, "desktop", G_TYPE_INT, &i, NULL);
        b = TRUE;
        keyboard_option_add_new(it, "follow", G_TYPE_BOOLEAN, &b, NULL);
        break;
    case A_SEND_TO_DESKTOP_NEXT:
    case A_SEND_TO_DESKTOP_PREVIOUS:
    case A_SEND_TO_DESKTOP_LEFT:
    case A_SEND_TO_DESKTOP_RIGHT:
    case A_SEND_TO_DESKTOP_UP:
    case A_SEND_TO_DESKTOP_DOWN:
        b = TRUE;
        keyboard_option_add_new(it, "follow", G_TYPE_BOOLEAN, &b, NULL);
        keyboard_option_add_new(it, "wrap", G_TYPE_BOOLEAN, &b, NULL);
        break;
    case A_MOVE:
        break;
    case A_RESIZE:
        keyboard_option_add_new(it, "edge", G_TYPE_STRING, "", NULL);
        break;
    case A_MOVE_TO_CENTER:
        break;
    case A_MOVE_RELATIVE:
        i = 0;
        keyboard_option_add_new(it, "x", G_TYPE_INT, &i, NULL);
        keyboard_option_add_new(it, "y", G_TYPE_INT, &i, NULL);
        break;
    case A_RESIZE_RELATIVE:
        i = 0;
        keyboard_option_add_new(it, "left", G_TYPE_INT, &i, NULL);
        keyboard_option_add_new(it, "right", G_TYPE_INT, &i, NULL);
        keyboard_option_add_new(it, "up", G_TYPE_INT, &i, NULL);
        keyboard_option_add_new(it, "down", G_TYPE_INT, &i, NULL);
        break;
    case A_MOVE_TO_EDGE_NORTH:
    case A_MOVE_TO_EDGE_SOUTH:
    case A_MOVE_TO_EDGE_WEST:
    case A_MOVE_TO_EDGE_EAST:
    case A_MOVE_FROM_EDGE_NORTH:
    case A_MOVE_FROM_EDGE_SOUTH:
    case A_MOVE_FROM_EDGE_WEST:
    case A_MOVE_FROM_EDGE_EAST:
    case A_GROW_TO_EDGE_NORTH:
    case A_GROW_TO_EDGE_SOUTH:
    case A_GROW_TO_EDGE_WEST:
    case A_GROW_TO_EDGE_EAST:
    case A_SHADE_LOWER:
    case A_UNSHADE_RAISE:
    case A_TOGGLE_ALWAYS_ON_TOP:
    case A_TOGGLE_ALWAYS_ON_BOTTOM:
    case A_SEND_TO_TOP_LAYER:
    case A_SEND_TO_BOTTOM_LAYER:
    case A_SEND_TO_NORMAL_LAYER:
        break;

    case NUM_ACTIONS:
    default:
        break;
    }

    return;
}

void keyboard_fill_key_store(GtkTreeIter *parent, xmlNode *a_node)
{
    xmlNode *cur_node = NULL;
    GtkTreeIter iter;
    gchar *attr;
    gchar *option;
    GType otype;
    GtkAdjustment *adj;


    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (!strcmp("keybind", cur_node->name)) {
            /* a keybind */
            gtk_tree_store_append(key_store, &iter, parent);
            parse_attr_string("key", cur_node, &attr);

            gtk_tree_store_set(key_store, &iter, KEY_COL, attr, -1);
        } else if (!strcmp("action", cur_node->name)) {
            /* an action */
            gtk_tree_store_append(key_store, &iter, parent);
            parse_attr_string("name", cur_node, &attr);

            gtk_tree_store_set(key_store, &iter,
                               ACTION_COL, attr,
                               EDITABLE_COL, TRUE,
                               -1);

            keyboard_key_store_add_default_options(&iter);
        } else if (!strcmp("action", cur_node->parent->name) ||
                   !strcmp("startupnotify", cur_node->parent->name)) {
            /* an option for an action */
            gboolean next_child;

            next_child =
                gtk_tree_model_iter_children(GTK_TREE_MODEL(key_store),
                                             &iter,
                                             parent);
            parse_attr_string("name", cur_node->parent, &attr);

            while (next_child) {
                gtk_tree_model_get(GTK_TREE_MODEL(key_store), &iter,
                                   OPTION_COL, &option, -1);
                if (option && !strcmp(option, cur_node->name)) {
                    otype = action_option_get_type_from_text
                        (attr, cur_node->name);
                    switch (otype) {
                    case G_TYPE_STRING:
                        gtk_tree_store_set(key_store, &iter,
                                           OVAL_STR_COL,
                                           parse_string(doc, cur_node),
                                           -1);
                        break;
                    case G_TYPE_BOOLEAN:
                        gtk_tree_store_set(key_store, &iter,
                                           OVAL_BOOL_COL,
                                           parse_bool(doc, cur_node),
                                           -1);
                        break;
                    case G_TYPE_INT:
                        adj = GTK_ADJUSTMENT(gtk_adjustment_new
                                             (parse_int(doc, cur_node),
                                              -5000, 5000, 1, 1, 0));
                        gtk_tree_store_set(key_store, &iter,
                                           OVAL_INT_COL, G_OBJECT(adj),
                                           OVAL_STR_COL,
                                           parse_string(doc, cur_node),
                                           -1);
                        break;
                    }

                    break;
                }
                next_child =
                    gtk_tree_model_iter_next(GTK_TREE_MODEL(key_store), &iter);
            }
        }

        keyboard_fill_key_store(&iter, cur_node->children);
    }
}

void keyboard_write_bindings_tree(GtkTreeIter *it_parent, xmlNodePtr xml_parent)
{
    GtkTreeIter iter;
    xmlNodePtr new_node;
    gchar *key;
    gchar *action;
    gchar *option;
    GType optype;
    gchar *ovalstr;
    gboolean ovalbool;

    if (!gtk_tree_model_iter_children
        (GTK_TREE_MODEL(key_store), &iter, it_parent))
    {
        return;
    }

    do
    {
        gtk_tree_model_get(GTK_TREE_MODEL(key_store), &iter,
                           KEY_COL, &key,
                           ACTION_COL, &action,
                           OPTION_COL, &option, -1);
        if (key != NULL)
        {
            new_node = xmlNewChild(xml_parent, NULL, "keybind", NULL);
            xmlSetProp(new_node, "key", key);
        }
        else if (action != NULL)
        {
            new_node = xmlNewChild(xml_parent, NULL, "action", NULL);
            xmlSetProp(new_node, "name", action);
        }
        else if (option != NULL)
        {
            g_free(action);
            gtk_tree_model_get(GTK_TREE_MODEL(key_store), it_parent,
                               ACTION_COL, &action, -1);
            optype = action_option_get_type_from_text(action, option);

            switch (optype)
            {
            case G_TYPE_NONE:
                new_node =
                    xmlNewTextChild(xml_parent, NULL, option, "");
                break;
            case G_TYPE_STRING:
            case G_TYPE_INT:
                gtk_tree_model_get(GTK_TREE_MODEL(key_store), &iter,
                                   OVAL_STR_COL, &ovalstr, -1);
                new_node =
                    xmlNewTextChild(xml_parent, NULL, option, ovalstr);
                g_free(ovalstr);
                break;
            case G_TYPE_BOOLEAN:
                gtk_tree_model_get(GTK_TREE_MODEL(key_store), &iter,
                                   OVAL_BOOL_COL, &ovalbool, -1);
                new_node =
                    xmlNewTextChild
                    (xml_parent, NULL, option, ovalbool?"yes":"no");
                break;
            default:
                break;
            }
        }

        g_free(key);
        g_free(action);
        g_free(option);

        keyboard_write_bindings_tree(&iter, new_node);
    } while (gtk_tree_model_iter_next(GTK_TREE_MODEL(key_store), &iter));
}

void keyboard_write_bindings()
{
    xmlNodePtr kb, c;

    kb = tree_get_node("keyboard", NULL);
    while (c = kb->children)
    {
        xmlUnlinkNode(c);
        xmlFreeNode(c);
    }

    on_chain_quit_key_changed(GTK_ENTRY(get_widget("chain_quit_key")), NULL);

    keyboard_write_bindings_tree(NULL, kb);

    tree_apply();
}

void keyboard_fill_action_store()
{
    GtkTreeIter it;
    gint i;
    gchar *action;
    action_store = gtk_list_store_new(1, G_TYPE_STRING);

    for (i = 0; i < NUM_ACTIONS; i++)
    {
        action = action_get_text_from_name(i);

        gtk_list_store_append(action_store, &it);
        gtk_list_store_set(action_store, &it, 0, action, -1);

        g_free(action);
    }
}


void keyboard_setup_tab()
{
    GtkCellRenderer *render;
    GtkTreeViewColumn *column;
    GtkWidget *w;
    GtkTreeIter tree_iter;
    gchar *chainquitkey;
    xmlNode *keyboard_node = tree_get_node("keyboard", NULL)->children;

    /* signal handlers */

    w = get_widget("chain_quit_key");
    chainquitkey = tree_get_string("keyboard/chainQuitKey", "C-g");
    gtk_entry_set_text(GTK_ENTRY(w), chainquitkey);
    g_free(chainquitkey);
    chain_release_id = g_signal_connect
        (w, "key-release-event",
         G_CALLBACK(on_chain_quit_key_key_release_event),
         NULL);

    /* treeview setup */
    w = get_widget("key_bindings");

    key_store = gtk_tree_store_new(NUM_COLS,
                                   G_TYPE_STRING,
                                   G_TYPE_STRING,
                                   G_TYPE_STRING,
                                   G_TYPE_STRING,
                                   G_TYPE_BOOLEAN,
                                   G_TYPE_OBJECT, /* a GtkAdjustment */
                                   G_TYPE_BOOLEAN,
                                   G_TYPE_BOOLEAN,
                                   G_TYPE_BOOLEAN,
                                   G_TYPE_BOOLEAN);
    gtk_tree_view_set_model(GTK_TREE_VIEW(w), GTK_TREE_MODEL(key_store));

    g_object_unref(key_store);
    gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(w)),
                                GTK_SELECTION_SINGLE);

    /* load data */
    keyboard_fill_key_store(NULL, keyboard_node);
    keyboard_fill_action_store();

    /* key column */
    render = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes
        ("Key", render, "text", KEY_COL, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(w), column);

    /* action column */
    render = gtk_cell_renderer_combo_new();
    g_signal_connect(render, "edited",
                     G_CALLBACK(on_action_edited), NULL);
    g_object_set(render, "has-entry", FALSE,
                 "model", action_store,
                 "text-column", 0,
                 NULL);
    column = gtk_tree_view_column_new_with_attributes
        ("Action", render, "text", ACTION_COL,
         "editable", EDITABLE_COL,
         NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(w), column);

    /* option column */
    render = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes
        ("Option", render, "text", OPTION_COL, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(w), column);

    /* option value column */
    render = gtk_cell_renderer_text_new();
    g_signal_connect(render, "edited",
                     G_CALLBACK(on_cell_renderer_text_edited), NULL);
    column = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(column, "Option Value");
    gtk_tree_view_column_pack_start(column, render, TRUE);
    gtk_tree_view_column_set_attributes
        (column, render, "text", OVAL_STR_COL,
         "editable", OVAL_TYPE_STR_COL,
         "visible", OVAL_TYPE_STR_COL, NULL);

    render = gtk_cell_renderer_toggle_new();
    g_signal_connect(render, "toggled",
                     G_CALLBACK(on_cell_renderer_toggle_toggled), NULL);
    gtk_tree_view_column_pack_start(column, render, TRUE);
    gtk_tree_view_column_set_attributes
        (column, render, "active", OVAL_BOOL_COL,
         "visible", OVAL_TYPE_BOOL_COL, NULL);

    render = gtk_cell_renderer_spin_new();
    g_signal_connect(render, "edited",
                     G_CALLBACK(on_cell_renderer_spin_edited), NULL);
    gtk_tree_view_column_pack_start(column, render, TRUE);
    gtk_tree_view_column_set_attributes
        (column, render, "adjustment", OVAL_INT_COL,
         "text", OVAL_STR_COL,
         "editable", OVAL_TYPE_INT_COL,
         "visible", OVAL_TYPE_INT_COL, NULL);

    gtk_tree_view_append_column(GTK_TREE_VIEW(w), column);
}
