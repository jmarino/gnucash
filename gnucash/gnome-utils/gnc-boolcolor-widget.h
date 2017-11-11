/*
 * gnc-boolcolor-widget.c -- Color chooser with check button widget
 *
 * Copyright (C) 1997, 1998, 1999, 2000 Free Software Foundation
 * All rights reserved.
 *
 * Gnucash is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Gnucash is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, contact:
 *
 * Free Software Foundation           Voice:  +1-617-542-5942
 * 51 Franklin Street, Fifth Floor    Fax:    +1-617-542-2652
 * Boston, MA  02110-1301,  USA       gnu@gnu.org
 *
 */
/*
  @NOTATION@
 */

#ifndef GNC_BOOLCOLOR_WIDGET_H
#define GNC_BOOLCOLOR_WIDGET_H

#include <glib.h>


#define GNC_TYPE_BOOLCOLOR          (gnc_boolcolor_get_type ())
#define GNC_BOOLCOLOR(obj)          G_TYPE_CHECK_INSTANCE_CAST (obj, gnc_boolcolor_get_type(), GNCBoolColor)
#define GNC_BOOLCOLOR_CLASS(klass)  G_TYPE_CHECK_CLASS_CAST (klass, gnc_boolcolor_get_type(), GNCBoolColorClass)
#define GNC_IS_BOOLCOLOR(obj)       G_TYPE_CHECK_INSTANCE_TYPE (obj, gnc_boolcolor_get_type ())


typedef struct
{
    GtkBox hbox;

    GtkWidget *check_button;
    GtkWidget *color_button;

    int        disposed;
} GNCBoolColor;

typedef struct
{
    GtkBoxClass parent_class;
    void (*changed) (GNCBoolColor *gde);
} GNCBoolColorClass;

GType     gnc_boolcolor_get_type         (void);

GtkWidget *gnc_boolcolor_new             (gchar *name, gboolean use_alpha);
void      gnc_boolcolor_set_state        (GNCBoolColor *gdc, gboolean state);
void      gnc_boolcolor_set_color        (GNCBoolColor *gdc, GdkRGBA *color);
void      gnc_boolcolor_set_color_from_string (GNCBoolColor *gbc, gchar *color_str);
gboolean  gnc_boolcolor_get_state        (GNCBoolColor *gbc);
void      gnc_boolcolor_get_color        (GNCBoolColor *gbc, GdkRGBA *color);

#endif
