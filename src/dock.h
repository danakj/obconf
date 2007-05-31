#ifndef obconf__dock_h
#define obconf__dock_h

#include <gtk/gtk.h>

void dock_setup_position   (GtkWidget *w);
void dock_setup_float_x    (GtkWidget *w);
void dock_setup_float_y    (GtkWidget *w);
void dock_setup_stacking   (GtkWidget *top,
                            GtkWidget *normal, GtkWidget *bottom);
void dock_setup_direction  (GtkWidget *w);
void dock_setup_hide       (GtkWidget *w);
void dock_setup_hide_delay (GtkWidget *w);

#endif
