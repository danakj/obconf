/* -*- indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*-

   actions.c for ObConf, the configuration tool for Openbox
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

#include "actions.h"

gchar * action_option_get_text_from_name(Option name)
{
   switch (name)
   {
        case O_EXECUTE:
            return g_strdup("execute");
            break;
        case O_MENU:
            return g_strdup("menu");
            break;
        case O_DIALOG:
            return g_strdup("dialog");
            break;
        case O_ALL_DESKTOPS:
            return g_strdup("allDesktops");
            break;
        case O_PANELS:
            return g_strdup("panels");
            break;
        case O_INCLUDE_DESKTOP:
            return g_strdup("desktop");
            break;
        case O_LINEAR:
            return g_strdup("linear");
            break;
        case O_DESKTOP:
            return g_strdup("desktop");
            break;
        case O_WRAP:
            return g_strdup("wrap");
            break;
        case O_STRING:
            return g_strdup("string");
            break;
        case O_FOLLOW:
            return g_strdup("follow");
            break;
        case O_EDGE:
            return g_strdup("edge");
            break;
        case O_X:
            return g_strdup("x");
            break;
        case O_Y:
            return g_strdup("y");
            break;
        case O_LEFT:
            return g_strdup("left");
            break;
        case O_RIGHT:
            return g_strdup("right");
            break;
        case O_UP:
            return g_strdup("up");
            break;
        case O_DOWN:
            return g_strdup("down");
            break;
        case O_NUM_OPTIONS:
        default:
            return NULL;
            break;
    }
}

Option action_option_get_name_from_text(const gchar *act, const gchar *opt)
{
    gint i;
    gchar *txt;

    if (!strcmp(opt, "desktop"))
    {
        if (!strcmp(act, "NextWindow") || !strcmp(act, "PreviousWindow"))
        {
            return O_INCLUDE_DESKTOP;
        }
        else
        {
            return O_DESKTOP;
        }
    }

    for (i = 0; i < O_NUM_OPTIONS; i++)
    {
        if (i == O_INCLUDE_DESKTOP || i == O_DESKTOP)
            continue;

        txt = action_option_get_text_from_name(i);

        if (!strcmp(opt, txt))
        {
            g_free(txt);
            return i;
        }
        g_free(txt);
    }

    return -1;
}

GType action_option_get_type_from_text(const gchar *act, const gchar *opt)
{
    Option nopt;

    nopt = action_option_get_name_from_text(act, opt);

    switch (nopt)
    {
        case O_EXECUTE:
        case O_MENU:
        case O_STRING:
        case O_EDGE:
            return G_TYPE_STRING;
            break;
        case O_DIALOG:
        case O_ALL_DESKTOPS:
        case O_PANELS:
        case O_INCLUDE_DESKTOP:
        case O_LINEAR:
        case O_WRAP:
        case O_FOLLOW:
            return G_TYPE_BOOLEAN;
            break;
        case O_DESKTOP:
        case O_X:
        case O_Y:
        case O_LEFT:
        case O_RIGHT:
        case O_UP:
        case O_DOWN:
            return G_TYPE_INT;
            break;
        case O_NUM_OPTIONS:
        default:
            return -1;
            break;
    }
}

gchar * action_get_text_from_name(Action name)
{
    switch (name)
    {
        case A_EXECUTE:
            return g_strdup("Execute");
        case A_SHOW_MENU:
            return g_strdup("ShowMenu");
        case A_NEXT_WINDOW:
            return g_strdup("NextWindow");
        case A_PREVIOUS_WINDOW:
            return g_strdup("PreviousWindow");
        case A_DIRECTIONAL_FOCUS_NORTH:
            return g_strdup("DirectionalFocusNorth");
        case A_DIRECTIONAL_FOCUS_SOUTH:
            return g_strdup("DirectionalFocusSouth");
        case A_DIRECTIONAL_FOCUS_EAST:
            return g_strdup("DirectionalFocusEast");
        case A_DIRECTIONAL_FOCUS_WEST:
            return g_strdup("DirectionalFocusWest");
        case A_DIRECTIONAL_FOCUS_NORTH_WEST:
            return g_strdup("DirectionalFocusNorthWest");
        case A_DIRECTIONAL_FOCUS_NORTH_EAST:
            return g_strdup("DirectionalFocusNorthEast");
        case A_DIRECTIONAL_FOCUS_SOUTH_WEST:
            return g_strdup("DirectionalFocusSouthWest");
        case A_DIRECTIONAL_FOCUS_SOUTH_EAST:
            return g_strdup("DirectionalFocusSouthEast");
        case A_DESKTOP:
            return g_strdup("Desktop");
        case A_DESKTOP_NEXT:
            return g_strdup("DesktopNext");
        case A_DESKTOP_PREVIOUS:
            return g_strdup("DesktopPrevious");
        case A_DESKTOP_LEFT:
            return g_strdup("DesktopLeft");
        case A_DESKTOP_RIGHT:
            return g_strdup("DesktopRight");
        case A_DESKTOP_UP:
            return g_strdup("DesktopUp");
        case A_DESKTOP_DOWN:
            return g_strdup("DesktopDown");
        case A_DESKTOP_LAST:
            return g_strdup("DesktopLast");
        case A_ADD_DESKTOP_LAST:
            return g_strdup("AddDesktopLast");
        case A_REMOVE_DESKTOP_LAST:
            return g_strdup("RemoveDesktopLast");
        case A_ADD_DESKTOP_CURRENT:
            return g_strdup("AddDesktopCurrent");
        case A_REMOVE_DESKTOP_CURRENT:
            return g_strdup("RemoveDesktopCurrent");
        case A_TOGGLE_SHOW_DESKTOP:
            return g_strdup("ToggleShowDesktop");
        case A_SHOW_DESKTOP:
            return g_strdup("ShowDesktop");
        case A_UNSHOW_DESKTOP:
            return g_strdup("UnshowDesktop");
        case A_TOGGLE_DOCK_AUTOHIDE:
            return g_strdup("ToggleDockAutohide");
        case A_RECONFIGURE:
            return g_strdup("Reconfigure");
        case A_RESTART:
            return g_strdup("Restart");
        case A_EXIT:
            return g_strdup("Exit");
        case A_DEBUG:
            return g_strdup("Debug");
        case A_ACTIVATE:
            return g_strdup("Activate");
        case A_FOCUS:
            return g_strdup("Focus");
        case A_RAISE:
            return g_strdup("Raise");
        case A_LOWER:
            return g_strdup("Lower");
        case A_RAISE_LOWER:
            return g_strdup("RaiseLower");
        case A_UNFOCUS:
            return g_strdup("Unfocus");
        case A_FOCUS_TO_BOTTOM:
            return g_strdup("FocusToBottom");
        case A_ICONIFY:
            return g_strdup("Iconify");
        case A_CLOSE:
            return g_strdup("Close");
        case A_TOGGLE_SHADE:
            return g_strdup("ToggleShade");
        case A_SHADE:
            return g_strdup("Shade");
        case A_UNSHADE:
            return g_strdup("Unshade");
        case A_TOGGLE_OMNIPRESENT:
            return g_strdup("ToggleOmnipresent");
        case A_TOGGLE_MAXIMIZE_FULL:
            return g_strdup("ToggleMaximizeFull");
        case A_MAXIMIZE_FULL:
            return g_strdup("MaximizeFull");
        case A_UNMAXIMIZE_FULL:
            return g_strdup("UnmaximizeFull");
        case A_TOGGLE_MAXIMIZE_VERT:
            return g_strdup("ToggleMaximizeVert");
        case A_MAXIMIZE_VERT:
            return g_strdup("MaximizeVert");
        case A_UNMAXIMIZE_VERT:
            return g_strdup("UnmaximizeVert");
        case A_TOGGLE_MAXIMIZE_HORZ:
            return g_strdup("ToggleMaximizeHorz");
        case A_MAXIMIZE_HORZ:
            return g_strdup("MaximizeHorz");
        case A_UNMAXIMIZE_HORZ:
            return g_strdup("UnmaximizeHorz");
        case A_TOGGLE_FULLSCREEN:
            return g_strdup("ToggleFullscreen");
        case A_TOGGLE_DECORATIONS:
            return g_strdup("ToggleDecorations");
        case A_SEND_TO_DESKTOP:
            return g_strdup("SendToDesktop");
        case A_SEND_TO_DESKTOP_NEXT:
            return g_strdup("SendToDesktopNext");
        case A_SEND_TO_DESKTOP_PREVIOUS:
            return g_strdup("SendToDesktopPrevious");
        case A_SEND_TO_DESKTOP_LEFT:
            return g_strdup("SendToDesktopLeft");
        case A_SEND_TO_DESKTOP_RIGHT:
            return g_strdup("SendToDesktopRight");
        case A_SEND_TO_DESKTOP_UP:
            return g_strdup("SendToDesktopUp");
        case A_SEND_TO_DESKTOP_DOWN:
            return g_strdup("SendToDesktopDown");
        case A_MOVE:
            return g_strdup("Move");
        case A_RESIZE:
            return g_strdup("Resize");
        case A_MOVE_TO_CENTER:
            return g_strdup("MoveToCenter");
        case A_MOVE_RELATIVE:
            return g_strdup("MoveRelative");
        case A_RESIZE_RELATIVE:
            return g_strdup("ResizeRelative");
        case A_MOVE_TO_EDGE_NORTH:
            return g_strdup("MoveToEdgeNorth");
        case A_MOVE_TO_EDGE_SOUTH:
            return g_strdup("MoveToEdgeSouth");
        case A_MOVE_TO_EDGE_WEST:
            return g_strdup("MoveToEdgeWest");
        case A_MOVE_TO_EDGE_EAST:
            return g_strdup("MoveToEdgeEast");
        case A_MOVE_FROM_EDGE_NORTH:
            return g_strdup("MoveFromEdgeNorth");
        case A_MOVE_FROM_EDGE_SOUTH:
            return g_strdup("MoveFromEdgeSouth");
        case A_MOVE_FROM_EDGE_WEST:
            return g_strdup("MoveFromEdgeWest");
        case A_MOVE_FROM_EDGE_EAST:
            return g_strdup("MoveFromEdgeEast");
        case A_GROW_TO_EDGE_NORTH:
            return g_strdup("GrowToEdgeNorth");
        case A_GROW_TO_EDGE_SOUTH:
            return g_strdup("GrowToEdgeSouth");
        case A_GROW_TO_EDGE_WEST:
            return g_strdup("GrowToEdgeWest");
        case A_GROW_TO_EDGE_EAST:
            return g_strdup("GrowToEdgeEast");
        case A_SHADE_LOWER:
            return g_strdup("ShadeLower");
        case A_UNSHADE_RAISE:
            return g_strdup("UnshadeRaise");
        case A_TOGGLE_ALWAYS_ON_TOP:
            return g_strdup("ToggleAlwaysOnTop");
        case A_TOGGLE_ALWAYS_ON_BOTTOM:
            return g_strdup("ToggleAlwaysOnBottom");
        case A_SEND_TO_TOP_LAYER:
            return g_strdup("SendToTopLayer");
        case A_SEND_TO_BOTTOM_LAYER:
            return g_strdup("SendToBottomLayer");
        case A_SEND_TO_NORMAL_LAYER:
            return g_strdup("SendToNormalLayer");

        case NUM_ACTIONS:
            return NULL;
    }

    return NULL;
}

Action action_get_name_from_text(const gchar *text)
{
    gint i;
    gchar *tmp;

    for (i = 0; i < NUM_ACTIONS; i++)
    {
        tmp = action_get_text_from_name(i);
        if (!strcmp(text, tmp))
        {
            g_free(tmp);
            return i;
        }
        g_free(tmp);
    }

    return -1;
}
