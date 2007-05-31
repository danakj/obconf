#include "main.h"
#include "tree.h"
#include "gettext.h"

#include <gdk/gdkx.h>

static gboolean mapping = FALSE;

static GtkListStore *desktop_store;
static int num_desktops;
static GList *desktop_names;

static void reset_desktop_names();
static void desktops_set_names();
static void desktops_set_number();
static void on_desktop_names_cell_edited(GtkCellRendererText *cell,
                                         const gchar *path_string,
                                         const gchar *new_text,
                                         gpointer data);

void desktops_setup_num(GtkWidget *w)
{
    mapping = TRUE;

    num_desktops = tree_get_int("desktops/number", 4);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w), num_desktops);

    mapping = FALSE;
}

void desktops_setup_names(GtkWidget *w)
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

void on_desktop_num_value_changed(GtkSpinButton *w, gpointer data)
{
    if (mapping) return;

    num_desktops = gtk_spin_button_get_value(w);

    desktops_set_number();

    reset_desktop_names();
}

static void on_desktop_names_cell_edited(GtkCellRendererText *cell,
                                         const gchar *path_string,
                                         const gchar *new_text,
                                         gpointer data)
{
    GtkTreePath *path;
    GtkTreeIter it;
    gchar *old_text;
    GList *lit;
    gint i;

    if (mapping) return;

    path = gtk_tree_path_new_from_string (path_string);
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

    desktops_set_names();
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

static void desktops_set_names()
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

static void desktops_set_number()
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