/* -*- indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*-

   actions.h for ObConf, the configuration tool for Openbox
   Copyright (c) 2007        Justin Stallard

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

#ifndef obconf__actions_h
#define obconf__actions_h

#include <gtk/gtk.h>

typedef enum {
    A_EXECUTE,
    A_SHOW_MENU,
    A_NEXT_WINDOW,
    A_PREVIOUS_WINDOW,
    A_DIRECTIONAL_FOCUS_NORTH,
    A_DIRECTIONAL_FOCUS_SOUTH,
    A_DIRECTIONAL_FOCUS_EAST,
    A_DIRECTIONAL_FOCUS_WEST,
    A_DIRECTIONAL_FOCUS_NORTH_WEST,
    A_DIRECTIONAL_FOCUS_NORTH_EAST,
    A_DIRECTIONAL_FOCUS_SOUTH_WEST,
    A_DIRECTIONAL_FOCUS_SOUTH_EAST,
    A_DESKTOP,
    A_DESKTOP_NEXT,
    A_DESKTOP_PREVIOUS,
    A_DESKTOP_LEFT,
    A_DESKTOP_RIGHT,
    A_DESKTOP_UP,
    A_DESKTOP_DOWN,
    A_DESKTOP_LAST,
    A_ADD_DESKTOP_LAST,
    A_REMOVE_DESKTOP_LAST,
    A_ADD_DESKTOP_CURRENT,
    A_REMOVE_DESKTOP_CURRENT,
    A_TOGGLE_SHOW_DESKTOP,
    A_SHOW_DESKTOP,
    A_UNSHOW_DESKTOP,
    A_TOGGLE_DOCK_AUTOHIDE,
    A_RECONFIGURE,
    A_RESTART,
    A_EXIT,
    A_DEBUG,
    A_ACTIVATE,
    A_FOCUS,
    A_RAISE,
    A_LOWER,
    A_RAISE_LOWER,
    A_UNFOCUS,
    A_FOCUS_TO_BOTTOM,
    A_ICONIFY,
    A_CLOSE,
    A_TOGGLE_SHADE,
    A_SHADE,
    A_UNSHADE,
    A_TOGGLE_OMNIPRESENT,
    A_TOGGLE_MAXIMIZE_FULL,
    A_MAXIMIZE_FULL,
    A_UNMAXIMIZE_FULL,
    A_TOGGLE_MAXIMIZE_VERT,
    A_MAXIMIZE_VERT,
    A_UNMAXIMIZE_VERT,
    A_TOGGLE_MAXIMIZE_HORZ,
    A_MAXIMIZE_HORZ,
    A_UNMAXIMIZE_HORZ,
    A_TOGGLE_FULLSCREEN,
    A_TOGGLE_DECORATIONS,
    A_SEND_TO_DESKTOP,
    A_SEND_TO_DESKTOP_NEXT,
    A_SEND_TO_DESKTOP_PREVIOUS,
    A_SEND_TO_DESKTOP_LEFT,
    A_SEND_TO_DESKTOP_RIGHT,
    A_SEND_TO_DESKTOP_UP,
    A_SEND_TO_DESKTOP_DOWN,
    A_MOVE,
    A_RESIZE,
    A_MOVE_TO_CENTER,
    A_MOVE_RELATIVE,
    A_RESIZE_RELATIVE,
    A_MOVE_TO_EDGE_NORTH,
    A_MOVE_TO_EDGE_SOUTH,
    A_MOVE_TO_EDGE_WEST,
    A_MOVE_TO_EDGE_EAST,
    A_MOVE_FROM_EDGE_NORTH,
    A_MOVE_FROM_EDGE_SOUTH,
    A_MOVE_FROM_EDGE_WEST,
    A_MOVE_FROM_EDGE_EAST,
    A_GROW_TO_EDGE_NORTH,
    A_GROW_TO_EDGE_SOUTH,
    A_GROW_TO_EDGE_WEST,
    A_GROW_TO_EDGE_EAST,
    A_SHADE_LOWER,
    A_UNSHADE_RAISE,
    A_TOGGLE_ALWAYS_ON_TOP,
    A_TOGGLE_ALWAYS_ON_BOTTOM,
    A_SEND_TO_TOP_LAYER,
    A_SEND_TO_BOTTOM_LAYER,
    A_SEND_TO_NORMAL_LAYER,

    NUM_ACTIONS
} Action;

typedef enum {
    O_EXECUTE,
    O_MENU,
    O_DIALOG,
    O_ALL_DESKTOPS,
    O_PANELS,
    O_INCLUDE_DESKTOP,
    O_LINEAR,
    O_DESKTOP,
    O_WRAP,
    O_STRING,
    O_FOLLOW,
    O_EDGE,
    O_X,
    O_Y,
    O_LEFT,
    O_RIGHT,
    O_UP,
    O_DOWN,
    O_NUM_OPTIONS
} Option;


gchar * action_option_get_text_from_name(Option name);
Option action_option_get_name_from_text(const gchar *act, const gchar *opt);
GType action_option_get_type_from_text(const gchar *act, const gchar *opt);
gchar * action_get_text_from_name(Action name);
Action action_get_name_from_text(const gchar *text);

#endif
