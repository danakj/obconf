/* -*- indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*-

   main.c for ObConf, the configuration tool for Openbox
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
#include "archive.h"
#include "theme.h"
#include "appearance.h"
#include "behavior.h"
#include "desktops.h"
#include "dock.h"
#include "preview_update.h"
#include "gettext.h"

#include <gdk/gdkx.h>
#define SN_API_NOT_YET_FROZEN
#include <libsn/sn.h>
#undef SN_API_NOT_YET_FROZEN
#include <stdlib.h>

GtkWidget *mainwin = NULL;

GladeXML *glade;
xmlDocPtr doc;
xmlNodePtr root;
RrInstance *rrinst;

static gchar *obc_theme_install = NULL;
static gchar *obc_theme_archive = NULL;

void obconf_error(gchar *msg)
{
    GtkWidget *d;

    d = gtk_message_dialog_new(mainwin ? GTK_WINDOW(mainwin) : NULL,
                               GTK_DIALOG_DESTROY_WITH_PARENT,
                               GTK_MESSAGE_ERROR,
                               GTK_BUTTONS_CLOSE,
                               "%s", msg);
    g_signal_connect_swapped(GTK_OBJECT(d), "response",
                             G_CALLBACK(gtk_widget_destroy),
                             GTK_OBJECT(d));
    gtk_widget_show(d);
}

static void print_version()
{
    g_print("ObConf %s\n", PACKAGE_VERSION);
    g_print(_("Copyright (c)"));
    g_print(" 2003-2007   Dana Jansens\n");
    g_print(_("Copyright (c)"));
    g_print(" 2003        Tim Riley\n\n");
    g_print(" 2007        Javeed Shaikh\n\n");
    g_print("This program comes with ABSOLUTELY NO WARRANTY.\n");
    g_print("This is free software, and you are welcome to redistribute it\n");
    g_print("under certain conditions. See the file COPYING for details.\n\n");

    exit(EXIT_SUCCESS);
}

static void print_help()
{
    g_print(_("Syntax: obconf [options] [ARCHIVE.obt]\n"));
    g_print(_("\nOptions:\n"));
    g_print(_("  --help                Display this help and exit\n"));
    g_print(_("  --version             Display the version and exit\n"));
    g_print(_("  --install ARCHIVE.obt Install the given theme archive and select it\n"));
    g_print(_("  --archive THEME       Create a theme archive from the given theme directory\n"));
    g_print(_("\nPlease report bugs at %s\n\n"), PACKAGE_BUGREPORT);
    
    exit(EXIT_SUCCESS);
}

static void parse_args(int argc, char **argv)
{
    int i;

    for (i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--help"))
            print_help();
        if (!strcmp(argv[i], "--version"))
            print_version();
        else if (!strcmp(argv[i], "--install")) {
            if (i == argc - 1) /* no args left */
                g_printerr(_("--install requires an argument\n"));
            else
                obc_theme_install = argv[++i];
        }
        else if (!strcmp(argv[i], "--archive")) {
            if (i == argc - 1) /* no args left */
                g_printerr(_("--archive requires an argument\n"));
            else
                obc_theme_archive = argv[++i];
        } else
            obc_theme_install = argv[i];
    }
}

int main(int argc, char **argv)
{
    gchar *p;

    gtk_init(&argc, &argv);
    parse_args(argc, argv);

    if (obc_theme_archive) {
        archive_create(obc_theme_archive);
        return;
    }

    p = g_build_filename(GLADEDIR, "obconf.glade", NULL);
    glade = glade_xml_new(p, NULL, NULL);
    g_free(p);

    if (!glade) {
        obconf_error("Failed to load the obconf.glade interface file. You "
                     "have probably failed to install ObConf properly.");
        return 1;
    }

    parse_paths_startup();
    rrinst = RrInstanceNew(GDK_DISPLAY(), gdk_x11_get_default_screen());

    xmlIndentTreeOutput = 1;
    if (!parse_load_rc(NULL, &doc, &root)) {
        obconf_error("Failed to load an rc.xml. You have probably failed to "
                     "install Openbox properly.");
        return 1;
    }

    glade_xml_signal_autoconnect(glade);

    {
        gchar *s = g_strdup_printf
            ("<span weight=\"bold\" size=\"xx-large\">ObConf %s</span>",
             PACKAGE_VERSION);
        gtk_label_set_markup(GTK_LABEL
                             (glade_xml_get_widget(glade, "title_label")), s);
        g_free(s);
    }

    theme_setup_tab();
    appearance_setup_tab();

    behavior_setup_tab();
    behavior_setup_focus_mouse(get_widget("focus_mouse"));
    behavior_setup_focus_raise(get_widget("focus_raise"));
    behavior_setup_focus_last(get_widget("focus_raise"));
    behavior_setup_focus_delay(get_widget("focus_delay"));
    behavior_setup_focus_new(get_widget("focus_new"));
    behavior_setup_place_mouse(get_widget("place_mouse"));
    behavior_setup_resist_window(get_widget("resist_window"));
    behavior_setup_resist_edge(get_widget("resist_edge"));
    behavior_setup_resize_contents(get_widget("resize_contents"));

    desktops_setup_tab();

    dock_setup_tab();
    dock_setup_position(get_widget("dock_position"));
    dock_setup_float_x(get_widget("dock_float_x"));
    dock_setup_float_y(get_widget("dock_float_y"));
    dock_setup_stacking(get_widget("dock_stack_top"),
                        get_widget("dock_stack_normal"),
                        get_widget("dock_stack_bottom"));
    dock_setup_direction(get_widget("dock_direction"));
    dock_setup_hide(get_widget("dock_hide"));
    dock_setup_hide_delay(get_widget("dock_hide_delay"));

    mainwin = get_widget("main_window");

    if (obc_theme_install)
        theme_install(obc_theme_install);

    /* the main window is not shown here ! it is shown when the theme previews
       are completed */
    gtk_main();

    preview_update_set_active_font(NULL);
    preview_update_set_inactive_font(NULL);
    preview_update_set_menu_header_font(NULL);
    preview_update_set_menu_item_font(NULL);
    preview_update_set_osd_font(NULL);
    preview_update_set_title_layout(NULL);

    RrInstanceFree(rrinst);
    parse_paths_shutdown();

    xmlFreeDoc(doc);
    return 0;
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

void obconf_show_main()
{
    SnDisplay *sn_d;
    SnLauncheeContext *sn_cx;

    gtk_widget_show_all(mainwin);

    sn_d = sn_display_new(GDK_DISPLAY_XDISPLAY(gdk_display_get_default()),
                          NULL, NULL);

    sn_cx = sn_launchee_context_new_from_environment
        (sn_d, gdk_screen_get_number(gdk_display_get_default_screen
                                     (gdk_display_get_default())));

    if (sn_cx)
        sn_launchee_context_setup_window
            (sn_cx, GDK_WINDOW_XWINDOW(GDK_WINDOW(mainwin->window)));

    if (sn_cx)
        sn_launchee_context_complete(sn_cx);

    if (sn_cx)
        sn_launchee_context_unref(sn_cx);
    sn_display_unref(sn_d);
}
