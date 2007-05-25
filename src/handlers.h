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

#ifndef obconf__handlers_h
#define obconf__handlers_h

void setup_behavior_tab();
void setup_dock_tab();
void setup_focus_mouse(GtkWidget *w);
void setup_focus_delay(GtkWidget *w);
void setup_focus_new(GtkWidget *w);
void setup_focus_raise(GtkWidget *w);
void setup_focus_desktop(GtkWidget *w);
void setup_place_mouse(GtkWidget *w);
void setup_resist_window(GtkWidget *w);
void setup_resist_edge(GtkWidget *w);
void setup_resize_contents(GtkWidget *w);
void setup_dock_position(GtkWidget *w);
void setup_dock_float_x(GtkWidget *w);
void setup_dock_float_y(GtkWidget *w);
void setup_dock_stacking(GtkWidget *top, GtkWidget *normal, GtkWidget *bottom);
void setup_dock_direction(GtkWidget *w);
void setup_dock_hide(GtkWidget *w);
void setup_dock_hide_delay(GtkWidget *w);
void setup_theme_names(GtkWidget *w);
void setup_title_layout(GtkWidget *w);
void setup_window_border(GtkWidget *w);
void setup_font_active(GtkWidget *w);
void setup_font_inactive(GtkWidget *w);
void setup_font_menu_header(GtkWidget *w);
void setup_font_menu_item(GtkWidget *w);
void setup_font_display(GtkWidget *w);
void setup_desktop_num(GtkWidget *w);
void setup_desktop_names(GtkWidget *w);

#endif
