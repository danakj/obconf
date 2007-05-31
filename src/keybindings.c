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

static gboolean mapping = FALSE;
static gchar   *saved_text = NULL;

static gboolean validate_key(const gchar *s);

void keybindings_setup_tab()
{
    GtkWidget *w;
    gchar *s;

    mapping = TRUE;

    w = get_widget("chain_quit_key");
    s = tree_get_string("keyboard/chainQuitKey", "C-g");
    gtk_entry_set_text(GTK_ENTRY(w), s);
    g_free(s);

    mapping = FALSE;
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
