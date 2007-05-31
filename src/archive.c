#include "theme.h"
#include "main.h"
#include "gettext.h"

#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define gtk_msg(type, args...) \
{                                                                        \
    GtkWidget *msgw;                                                     \
    msgw = gtk_message_dialog_new(GTK_WINDOW(mainwin),                   \
                                  GTK_DIALOG_DESTROY_WITH_PARENT |       \
                                  GTK_DIALOG_MODAL,                      \
                                  type,                                  \
                                  GTK_BUTTONS_OK,                        \
                                  args);                                 \
    gtk_dialog_run(GTK_DIALOG(msgw));                                    \
    gtk_widget_destroy(msgw);                                            \
}

static gchar *get_theme_dir();
static gboolean change_dir(const gchar *dir);
static gchar* name_from_file(const gchar *path);
static gchar* name_from_dir(const gchar *dir);
static gboolean install_theme_to(const gchar *name, const gchar *file,
                                 const gchar *to);
static gboolean create_theme_archive(const gchar *dir, const gchar *name,
                                     const gchar *to);

gchar* archive_install(const gchar *path)
{
    gchar *dest;
    gchar *name;

    if (!(dest = get_theme_dir()))
        return NULL;

    if (!(name = name_from_file(path)))
        return NULL;

    if (install_theme_to(name, path, dest)) {
        gtk_msg(GTK_MESSAGE_INFO, _("\"%s\" was installed to %s"), name, dest);
    } else {
        g_free(name);
        name = NULL;
    }

    g_free(dest);

    return name;
}

void archive_create(const gchar *path)
{
    gchar *name;
    gchar *dest;

    if (!(name = name_from_dir(path)))
        return;

    {
        gchar *file;
        file = g_strdup_printf("%s.obt", name);
        dest = g_build_path(G_DIR_SEPARATOR_S,
                            g_get_current_dir(), file, NULL);
        g_free(file);
    }

    if (create_theme_archive(path, name, dest))
        gtk_msg(GTK_MESSAGE_INFO, _("\"%s\" was successfully created"),
                dest);

    g_free(dest);
    g_free(name);
}

static gboolean create_theme_archive(const gchar *dir, const gchar *name,
                                     const gchar *to)
{
    gchar *glob;
    gchar **argv;
    gchar *errtxt = NULL;
    gchar *parentdir;
    gint exitcode;
    GError *e = NULL;

    glob = g_strdup_printf("%s/openbox-3/", name);

    parentdir = g_build_path(G_DIR_SEPARATOR_S, dir, "..", NULL);

    argv = g_new(gchar*, 7);
    argv[0] = g_strdup("tar");
    argv[1] = g_strdup("-c");
    argv[2] = g_strdup("-z");
    argv[3] = g_strdup("-f");
    argv[4] = g_strdup(to);
    argv[5] = g_strdup(glob);
    argv[6] = NULL;
    if (g_spawn_sync(parentdir, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL,
                     NULL, &errtxt, &exitcode, &e))
    {
        if (exitcode != EXIT_SUCCESS)
            gtk_msg(GTK_MESSAGE_ERROR,
                    _("Unable to create the theme archive \"%s\".\nThe following errors were reported:\n%s"),
                    to, errtxt);

    }
    else
        gtk_msg(GTK_MESSAGE_ERROR, _("Unable to run the \"tar\" command: %s"),
                e->message);

    g_strfreev(argv);
    if (e) g_error_free(e);
    g_free(errtxt);
    g_free(parentdir);
    g_free(glob);
    return exitcode == EXIT_SUCCESS;
}

static gchar *get_theme_dir()
{
    gchar *dir;
    gint r;

    dir = g_build_path(G_DIR_SEPARATOR_S, g_get_home_dir(), ".themes", NULL);
    r = mkdir(dir, 0777);
    if (r == -1 && errno != EEXIST) {
        gtk_msg(GTK_MESSAGE_ERROR,
                _("Unable to create directory \"%s\": %s"),
                dir, strerror(errno));
        g_free(dir);
        dir = NULL;
    }

    return dir;
}

static gchar* name_from_dir(const gchar *dir)
{
    gchar *rc;
    struct stat st;
    gboolean r;

    rc = g_build_path(G_DIR_SEPARATOR_S, dir, "openbox-3", "themerc", NULL);

    r = (stat(rc, &st) == 0 && S_ISREG(st.st_mode));
    g_free(rc);

    if (!r) {
        gtk_msg(GTK_MESSAGE_ERROR,
                _("\"%s\" does not appear to be a valid Openbox theme directory"),
                dir);
        return NULL;
    }
    return g_path_get_basename(dir);
}

static gchar* name_from_file(const gchar *path)
{
    /* decipher the theme name from the file name */
    gchar *fname = g_path_get_basename(path);
    gint len = strlen(fname);
    gchar *name = NULL;

    if (len > 4 &&
        (fname[len-4] == '.' && fname[len-3] == 'o' &&
         fname[len-2] == 'b' && fname[len-1] == 't'))
    {
        fname[len-4] = '\0';
        name = g_strdup(fname);
        fname[len-4] = '.';
    }

    if (name == NULL)
        gtk_msg(GTK_MESSAGE_ERROR,
                _("Unable to determine the theme's name from \"%s\".  File name should be ThemeName.obt."), fname);

    return name;
}

static gboolean change_dir(const gchar *dir)
{
    if (chdir(dir) == -1) {
        gtk_msg(GTK_MESSAGE_ERROR, _("Unable to move to directory \"%s\": %s"),
                dir, strerror(errno));
        return FALSE;
    }
    return TRUE;
}

static gboolean install_theme_to(const gchar *name, const gchar *file,
                                 const gchar *to)
{
    gchar *glob;
    gchar **argv;
    gchar *errtxt = NULL;
    gint exitcode;
    GError *e = NULL;

    glob = g_strdup_printf("%s/openbox-3/", name);

    argv = g_new(gchar*, 7);
    argv[0] = g_strdup("tar");
    argv[1] = g_strdup("-x");
    argv[2] = g_strdup("-z");
    argv[3] = g_strdup("-f");
    argv[4] = g_strdup(file);
    argv[5] = g_strdup(glob);
    argv[6] = NULL;
    if (!g_spawn_sync(to, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL,
                      NULL, &errtxt, &exitcode, &e))
        gtk_msg(GTK_MESSAGE_ERROR, _("Unable to run the \"tar\" command: %s"),
                e->message);
    g_strfreev(argv);
    if (e) g_error_free(e);

    if (exitcode != EXIT_SUCCESS)
        gtk_msg(GTK_MESSAGE_ERROR,
                _("Unable to extract the file \"%s\".\nPlease ensure that \"%s\" is writable and that the file is a valid Openbox theme archive.\nThe following errors were reported:\n%s"),
                file, to, errtxt);

    g_free(errtxt);
    g_free(glob);
    return exitcode == EXIT_SUCCESS;
}
