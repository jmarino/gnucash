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


#include <config.h>

#include <gtk/gtk.h>

#include "gnc-engine.h"
#include "dialog-utils.h"
#include "gnc-boolcolor-widget.h"


enum
{
    CHANGED,
    LAST_SIGNAL
};

static QofLogModule log_module = GNC_MOD_GUI;
static guint boolcolor_signals [LAST_SIGNAL] = { 0 };

static void gnc_boolcolor_init         (GNCBoolColor      *gde);
static void gnc_boolcolor_class_init   (GNCBoolColorClass *klass);
static void gnc_boolcolor_dispose      (GObject          *object);
static void gnc_boolcolor_finalize     (GObject          *object);
static void gnc_boolcolor_toggled_cb (GtkWidget *widget, GNCBoolColor *gbc);
static void gnc_boolcolor_color_set_cb (GtkWidget *widget, GNCBoolColor *gbc);
static void create_children (GNCBoolColor *gbc, gchar *name);


static GtkBoxClass *parent_class;


/**
 * gnc_boolcolor_get_type:
 *
 * Returns the GType for the GNCBoolColor widget
 */
GType
gnc_boolcolor_get_type (void)
{
    static GType boolcolor_type = 0;

    if (boolcolor_type == 0)
    {
        static const GTypeInfo boolcolor_info =
        {
            sizeof (GNCBoolColorClass),
            NULL,
            NULL,
            (GClassInitFunc) gnc_boolcolor_class_init,
            NULL,
            NULL,
            sizeof (GNCBoolColor),
            0, /* n_preallocs */
            (GInstanceInitFunc) gnc_boolcolor_init,
            NULL,
        };

        boolcolor_type = g_type_register_static (GTK_TYPE_BOX,
                         "GNCBoolColor",
                         &boolcolor_info, 0);
    }

    return boolcolor_type;
}


static void
gnc_boolcolor_class_init (GNCBoolColorClass *klass)
{
    GtkContainerClass *container_class = (GtkContainerClass *) klass;
    GObjectClass *object_class = (GObjectClass *) klass;

    object_class->dispose = gnc_boolcolor_dispose;
    object_class->finalize = gnc_boolcolor_finalize;

    parent_class = g_type_class_ref(GTK_TYPE_BOX);

    boolcolor_signals [CHANGED] =
        g_signal_new ("changed",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (GNCBoolColorClass, changed),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__VOID,
                      G_TYPE_NONE, 0);

    klass->changed = NULL;
}


static void
gnc_boolcolor_init (GNCBoolColor *gbc)
{
    gtk_orientable_set_orientation (GTK_ORIENTABLE(gbc), GTK_ORIENTATION_HORIZONTAL);

    // Set the style context for this widget so it can be easily manipulated with css
    gnc_widget_set_style_context (GTK_WIDGET(gbc), "GncBoolColor");

    gbc->disposed = FALSE;
}


static void
gnc_boolcolor_finalize (GObject *object)
{
    g_return_if_fail (object != NULL);
    g_return_if_fail (GNC_IS_BOOLCOLOR (object));
    if (G_OBJECT_CLASS (parent_class)->finalize)
        (* G_OBJECT_CLASS (parent_class)->finalize) (object);
}


static void
gnc_boolcolor_dispose (GObject *object)
{
    GNCBoolColor *gbc;

    g_return_if_fail (object != NULL);
    g_return_if_fail (GNC_IS_BOOLCOLOR (object));

    gbc = GNC_BOOLCOLOR (object);

    if (gbc->disposed)
        return;

    gbc->disposed = TRUE;

    /* Only explicitly destroy the toplevel elements */
    gtk_widget_destroy (gbc->check_button);
    gbc->check_button = NULL;

    gtk_widget_destroy (gbc->color_button);
    gbc->color_button = NULL;

    if (G_OBJECT_CLASS (parent_class)->dispose)
        (* G_OBJECT_CLASS (parent_class)->dispose) (object);
}


static void
gnc_boolcolor_toggled_cb (GtkWidget *widget, GNCBoolColor *gbc)
{
    ENTER("widget %p, gbc %p", widget, gbc);
    g_signal_emit (gbc, boolcolor_signals [CHANGED], 0);
    LEAVE(" ");
}


static void
gnc_boolcolor_color_set_cb (GtkWidget *widget, GNCBoolColor *gbc)
{
    ENTER("widget %p, gbc %p", widget, gbc);
    g_signal_emit (gbc, boolcolor_signals [CHANGED], 0);
    LEAVE(" ");
}


static void
create_children (GNCBoolColor *gbc, gchar *name)
{
    GtkWidget *label;
    gchar *name_colon;

    name_colon = g_strndup (name, strlen (name) + 2);  // copy with extra byte for ':'
    name_colon[strlen (name)] = ':';

    /* Create the check box */
    gbc->check_button  = gtk_check_button_new_with_label (name_colon);
    gtk_box_pack_start (GTK_BOX (gbc), gbc->check_button, TRUE, TRUE, 0);
    label = gtk_bin_get_child (GTK_BIN (gbc->check_button));
    gnc_label_set_alignment (label, 1.0, 0.5);
    gtk_widget_show (gbc->check_button);
    g_free (name_colon);

    /* Create the color chooser button. */
    gbc->color_button = gtk_color_button_new();
    gtk_box_pack_start (GTK_BOX (gbc), gbc->color_button, TRUE, TRUE, 0);
    gtk_widget_show (gbc->color_button);

    g_signal_connect (gbc->check_button, "toggled",
                      G_CALLBACK(gnc_boolcolor_toggled_cb), gbc);
    g_signal_connect (gbc->color_button, "color-set",
                      G_CALLBACK(gnc_boolcolor_color_set_cb), gbc);
}


/**
 * gnc_boolcolor_new:
 *
 * Creates a new GNCBoolColor widget which can be used to provide
 * an on/off entry with a color associated.
 *
 * Returns a GNCBoolColor widget.
 */
GtkWidget *
gnc_boolcolor_new (gchar *name, gboolean use_alpha)
{
    GNCBoolColor *gbc;

    gbc = g_object_new (GNC_TYPE_BOOLCOLOR, NULL, NULL);
    create_children (gbc, name);
    gnc_boolcolor_set_state (gbc, TRUE);
    gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(gbc->color_button), use_alpha);

    return GTK_WIDGET (gbc);
}


/**
 * gnc_boolcolor_set_state:
 * @gbc: the GNCBoolColor widget
 * @state: State of the GtkCheckButton in the widget
 *
 * Changes the state of the GtkCheckButton in the GNCBoolColor widget
 * to @state.
 */
void
gnc_boolcolor_set_state (GNCBoolColor *gbc, gboolean state)
{
    g_return_if_fail (gbc != NULL);
    g_return_if_fail (GNC_IS_BOOLCOLOR (gbc));
    gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(gbc->check_button), state );
}


/**
 * gnc_boolcolor_set_color:
 * @gbc: the GNCBoolColor widget
 * @color: Color to set for the GtkColorChooser widget
 *
 * Changes the color in the GtkColorChooser in the GNCBoolColor widget
 * to the one given by @color.
 */
void
gnc_boolcolor_set_color (GNCBoolColor *gbc, GdkRGBA *color)
{
    g_return_if_fail (gbc != NULL);
    g_return_if_fail (color != NULL);
    g_return_if_fail (GNC_IS_BOOLCOLOR (gbc));
    gtk_color_chooser_set_rgba( GTK_COLOR_CHOOSER(gbc->color_button), color );
}


/**
 * gnc_boolcolor_set_color_from_string:
 * @gbc: the GNCBoolColor widget
 * @color: Color to set for the GtkColorChooser widget. Color string is passed to gdk_rgba_parse.
 *
 * Changes the color in the GtkColorChooser in the GNCBoolColor widget
 * to the one given by @color.
 */
void
gnc_boolcolor_set_color_from_string (GNCBoolColor *gbc, gchar *color_str)
{
    GdkRGBA color;
    g_return_if_fail (gbc != NULL);
    g_return_if_fail (color_str != NULL);
    g_return_if_fail (GNC_IS_BOOLCOLOR (gbc));
    if (gdk_rgba_parse (&color, color_str))
    {
        gtk_color_chooser_set_rgba (GTK_COLOR_CHOOSER (gbc->color_button), &color);
    }
    else
    {
        DEBUG ("GNCBoolColor: error parsing color: %s", color_str);
    }
}


/**
 * gnc_boolcolor_get_state:
 * @gbc: The GNCBoolColor widget
 *
 * Returns the state of the GtkCheckButton in the GNCBoolColor widget.
 */
gboolean
gnc_boolcolor_get_state (GNCBoolColor *gbc)
{
    g_return_val_if_fail (gbc != NULL, 0);
    g_return_val_if_fail (GNC_IS_BOOLCOLOR (gbc), 0);
    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gbc->check_button));
}


/**
 * gnc_boolcolor_get_color:
 * @gbc: The GNCBoolColor widget
 *
 * Returns the color selected.
 */
void
gnc_boolcolor_get_color (GNCBoolColor *gbc, GdkRGBA *color)
{
    g_return_if_fail (gbc != NULL && color != NULL);
    g_return_if_fail (GNC_IS_BOOLCOLOR (gbc));
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(gbc->color_button), color);
}
