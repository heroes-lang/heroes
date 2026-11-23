/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_canvas_gtk.c: the drawing area on GTK 4 (`hero_ui.h`'s A
 * DRAWING AREA, WITH ITS MOUSE AND ITS KEYS; design.md §1.11, §4.19), one
 * member of the archive the compiler puts on every program's link line
 * (`selfhost/cli/layer.hero`), compiled to nothing on macOS and Windows. The
 * GTK arm, `platform/hero_ui_gtk.c`, calls it for a widget of kind
 * `HERO_UI_CANVAS`; its header says who holds what.
 *
 * THE DRAWING is decoded once, as it is given, into operations of this
 * member's own (`hero_ui_canvas_gtk_op`), and replayed by ONE function,
 * `replay`, both when GTK draws the area (the `GtkDrawingArea`'s draw
 * function, on GTK's own cairo context) and for a snapshot (a cairo image
 * surface of ARGB32), so a case reads the pixels the person sees. What the
 * replay keeps of `hero_ui.h`'s semantics, each the macOS arm's as Core
 * Graphics draws it:
 *
 * - a path's points are in the space the transform makes when the path is
 *   painted: cairo turns a point into device space as it is given, so the
 *   path is kept as the list of its parts since the last BEGIN and given to
 *   cairo again, after `cairo_set_matrix`, at each fill, stroke and clip;
 * - SAVE and RESTORE keep the transform (a stack of this member's) and the
 *   clip (cairo's own `cairo_save`); a list that ends with states kept
 *   restores them, and the whole replay stands inside one save of its own,
 *   so GTK's context is given back as it came;
 * - a LINE with no subpath begun starts one, and a QUAD, a CUBIC or a CLOSE
 *   with none is left out; a quadratic curve is the cubic of the same
 *   points, as Core Graphics makes it; a RECT runs from its corner at x, y
 *   to x + width, y first and an ELLIPSE from its rightmost point down
 *   first, the orders Core Graphics' `addRect` and `addEllipse(in:)` give
 *   (measured on this Mac, 2026-11-23), so a path's winding, and so what the
 *   non-zero rule fills, is the same on both systems;
 * - a fill, a stroke or a clip with no path marks nothing and changes
 *   nothing: Core Graphics leaves the clip as it was where its path is
 *   empty (measured the same day), where cairo would clip everything away;
 * - a transform that maps the plane onto a line or a point, or one whose
 *   numbers are no longer finite, is one cairo refuses (`cairo_set_matrix`
 *   puts the context in an error state that stops every drawing after it):
 *   under it a fill, a stroke, a text and a picture mark nothing, and a
 *   clip leaves nothing to draw in, which is the plane mapped onto no area;
 * - a text is ONE line (Pango's single-paragraph mode), its layout's
 *   top-left corner at x, y, its size the font's em in the area's points
 *   (Pango's absolute size, so no screen resolution scales it), bold and
 *   italic from the style bits, the system's font for an empty family, and
 *   fontconfig's nearest for a family the system lacks;
 * - a picture fills its box smoothly (cairo's bilinear filter) with its
 *   edges' pixels padded outwards, so its border does not fade into the
 *   clear area around it, as Core Graphics draws an image.
 *
 * THE POINTER. GTK's own gestures on the area tell the person's: a click
 * gesture the press, with GTK's count of clicks; a drag gesture each move
 * with a button held and the release, which GTK gives the area the press
 * started in until the release however far the pointer goes (its implicit
 * grab); a motion controller the moves with no button held, the entry and
 * the exit. A press gives the area the keys. Acting as the person's pointer
 * (`pointer`) cannot go through events of the system's: GTK 4 has no call
 * that makes a `GdkEvent`, and X11's XTest moves the one real pointer, so a
 * press there would bring the crossing and the moves of the way to it, and
 * X11 has no word for a count of clicks; so a press, a drag and a release
 * are found by GTK's own hit test, `gtk_widget_pick` on the window, the one
 * GTK routes a press by, and given to the same telling the gestures call,
 * and a move, an entry and an exit are given to it directly, as
 * `hero_ui.h` says of a system that routes them by the real pointer alone.
 *
 * THE KEYS. A key controller on the area tells each key GTK gives it while
 * it has the keys, and consumes it, so neither the window's focus moves nor
 * any other handler runs on it; a modifier key on its own is no key of the
 * area's (AppKit gives a view none either). A press of a key already down
 * is the keyboard's repeat: X11's server repeats a held key with presses
 * alone, which GDK asks of it (detectable auto-repeat).
 *
 * GTK'S, CAIRO'S AND PANGO'S FUNCTIONS are found with `dlsym` once, inside
 * `pthread_once`, on GTK 4's library, whose handle reaches cairo, Pango and
 * PangoCairo, the libraries GTK itself draws with, as the GTK arm finds its
 * own and for its reason: a member that named their headers would stop every
 * build on a machine without them. Every number below was read from GTK
 * 4.18.6's, cairo 1.18.4's and Pango 1.56.3's headers by a program compiled
 * against them in the GTK 4 image, 2026-11-23.
 */

#include "hero_ui_canvas_gtk.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if !defined(_WIN32) && !defined(__APPLE__)

#include <dlfcn.h>
#include <math.h>
#include <pthread.h>

#include "hero_layer_alloc.h"
#include "hero_ui.h"
#include "hero_ui_arm.h"

/* The numbers of GTK's, cairo's and Pango's headers this member passes. */
#define HERO_GTK_ACCESSIBLE_ROLE_IMG 20
#define HERO_GTK_PICK_DEFAULT 0
#define HERO_GDK_SHIFT_MASK 1u
#define HERO_GDK_CONTROL_MASK 4u
#define HERO_GDK_ALT_MASK 8u
#define HERO_GDK_BUTTON_PRIMARY 1u
#define HERO_GDK_BUTTON_SECONDARY 3u
#define HERO_CAIRO_FORMAT_ARGB32 0
#define HERO_CAIRO_STATUS_SUCCESS 0
#define HERO_CAIRO_FILTER_BILINEAR 4
#define HERO_CAIRO_EXTEND_PAD 3
#define HERO_PANGO_SCALE 1024
#define HERO_PANGO_WEIGHT_BOLD 700
#define HERO_PANGO_STYLE_ITALIC 2

/* Pango keeps a font's size in 1024ths of a unit in an `int`: the most a
 * text's size can be, in points. */
#define HERO_UI_CANVAS_GTK_TEXT_MOST 2097151.0

/* A snapshot's side, at most (`ui-more-for-native.md`'s contract). */
#define HERO_UI_CANVAS_GTK_SIDE_MOST 8192

/* The states a drawing keeps at once, at most: `hero_ui.c` refuses a 257th
 * (`hero_ui_drawing_checked`). */
#define HERO_UI_CANVAS_GTK_KEPT_MOST 256

/* The keys held down at once this member tells a repeat for. */
#define HERO_UI_CANVAS_GTK_KEYS 32

/* The most a pointer's x or y is told as, in 64ths of a point: `hero_ui.c`'s
 * `HERO_UI_POINT_MOST`, 65,536 points either way. */
#define HERO_UI_CANVAS_GTK_POINT_MOST 4194304.0

/* cairo's `cairo_matrix_t`, as its header lays it out (48 bytes, read with
 * the numbers above). */
typedef struct {
    double xx;
    double yx;
    double xy;
    double yy;
    double x0;
    double y0;
} hero_cairo_matrix;

_Static_assert(sizeof(hero_cairo_matrix) == 48, "cairo_matrix_t is six doubles");

typedef void (*hero_ui_canvas_gtk_callback)(void);
typedef void (*hero_ui_canvas_gtk_draws)(void *area, void *cr, int width, int height, void *data);

typedef struct {
    /* GObject's and GTK 4's. */
    void *(*object_new)(unsigned long type, const char *first, ...);
    void *(*object_ref_sink)(void *object);
    void (*object_unref)(void *object);
    unsigned long (*signal_connect_data)(void *instance, const char *signal, hero_ui_canvas_gtk_callback handler, void *data, void *destroy, int flags);
    unsigned long (*drawing_area_get_type)(void);
    void (*drawing_area_set_draw_func)(void *area, hero_ui_canvas_gtk_draws draw, void *data, void (*destroy)(void *data));
    void (*widget_queue_draw)(void *widget);
    void (*widget_set_focusable)(void *widget, int focusable);
    int (*widget_grab_focus)(void *widget);
    void *(*widget_pick)(void *widget, double x, double y, int flags);
    int (*widget_compute_point)(void *widget, void *target, const float *point, float *out);
    void (*widget_add_controller)(void *widget, void *controller);
    void *(*widget_get_native)(void *widget);
    void (*native_get_surface_transform)(void *native, double *x, double *y);
    void *(*gesture_click_new)(void);
    void *(*gesture_drag_new)(void);
    void (*gesture_single_set_button)(void *gesture, unsigned button);
    unsigned (*gesture_single_get_current_button)(void *gesture);
    void *(*event_controller_motion_new)(void);
    void *(*event_controller_key_new)(void);
    unsigned (*event_controller_get_current_event_state)(void *controller);
    void *(*event_controller_get_current_event)(void *controller);
    int (*event_get_position)(void *event, double *x, double *y);
    int (*key_event_is_modifier)(void *event);
    unsigned (*keyval_to_lower)(unsigned keyval);
    uint32_t (*keyval_to_unicode)(unsigned keyval);
    /* cairo's. */
    void *(*image_surface_create)(int format, int width, int height);
    unsigned char *(*image_surface_get_data)(void *surface);
    int (*image_surface_get_stride)(void *surface);
    int (*surface_status)(void *surface);
    void (*surface_flush)(void *surface);
    void (*surface_mark_dirty)(void *surface);
    void (*surface_destroy)(void *surface);
    void *(*create)(void *surface);
    int (*status)(void *cr);
    void (*destroy)(void *cr);
    void (*save)(void *cr);
    void (*restore)(void *cr);
    void (*get_matrix)(void *cr, hero_cairo_matrix *matrix);
    void (*set_matrix)(void *cr, const hero_cairo_matrix *matrix);
    void (*identity_matrix)(void *cr);
    void (*matrix_translate)(hero_cairo_matrix *matrix, double tx, double ty);
    void (*matrix_scale)(hero_cairo_matrix *matrix, double sx, double sy);
    void (*matrix_rotate)(hero_cairo_matrix *matrix, double radians);
    void (*new_path)(void *cr);
    void (*move_to)(void *cr, double x, double y);
    void (*line_to)(void *cr, double x, double y);
    void (*curve_to)(void *cr, double x1, double y1, double x2, double y2, double x3, double y3);
    void (*close_path)(void *cr);
    void (*set_source_rgba)(void *cr, double red, double green, double blue, double alpha);
    void (*set_source_surface)(void *cr, void *surface, double x, double y);
    void *(*get_source)(void *cr);
    void (*pattern_set_filter)(void *pattern, int filter);
    void (*pattern_set_extend)(void *pattern, int extend);
    void (*set_fill_rule)(void *cr, int rule);
    void (*set_line_width)(void *cr, double width);
    void (*set_line_cap)(void *cr, int cap);
    void (*set_line_join)(void *cr, int join);
    void (*fill)(void *cr);
    void (*stroke)(void *cr);
    void (*clip)(void *cr);
    /* Pango's and PangoCairo's. */
    void *(*create_layout)(void *cr);
    void (*show_layout)(void *cr, void *layout);
    void (*layout_set_text)(void *layout, const char *text, int length);
    void (*layout_set_font_description)(void *layout, const void *description);
    void (*layout_set_single_paragraph_mode)(void *layout, int setting);
    void *(*font_description_new)(void);
    void (*font_description_free)(void *description);
    void (*font_description_set_family)(void *description, const char *family);
    void (*font_description_set_absolute_size)(void *description, double size);
    void (*font_description_set_weight)(void *description, int weight);
    void (*font_description_set_style)(void *description, int style);
} hero_ui_canvas_gtk_functions;

/* Found once inside `pthread_once` and only read after it, as
 * `hero_ui_gtk.c`'s are: the functions are the process's. */
static hero_ui_canvas_gtk_functions hero_ui_canvas_gtk_found_functions;
static bool hero_ui_canvas_gtk_found;
static pthread_once_t hero_ui_canvas_gtk_once = PTHREAD_ONCE_INIT;

/* One function by name into `*slot`; false where the library has none. C11
 * has no conversion between an object pointer and a function pointer, so the
 * address is copied by its bytes, which POSIX makes the same for both
 * (`dlsym`'s own page). */
static bool hero_ui_canvas_gtk_find(void *library, const char *name, void *slot, size_t slot_size) {
    void *found = dlsym(library, name);

    if (found == NULL || slot_size != sizeof found) {
        return false;
    }
    memcpy(slot, &found, sizeof found);
    return true;
}

#define HERO_UI_CANVAS_GTK_FIND(field, name) ok = hero_ui_canvas_gtk_find(gtk, name, &hero_ui_canvas_gtk_found_functions.field, sizeof hero_ui_canvas_gtk_found_functions.field) && ok

static void hero_ui_canvas_gtk_load(void) {
    bool ok = true;
    void *gtk = dlopen("libgtk-4.so.1", RTLD_NOW | RTLD_LOCAL);

    if (gtk == NULL) {
        return;
    }
    HERO_UI_CANVAS_GTK_FIND(object_new, "g_object_new");
    HERO_UI_CANVAS_GTK_FIND(object_ref_sink, "g_object_ref_sink");
    HERO_UI_CANVAS_GTK_FIND(object_unref, "g_object_unref");
    HERO_UI_CANVAS_GTK_FIND(signal_connect_data, "g_signal_connect_data");
    HERO_UI_CANVAS_GTK_FIND(drawing_area_get_type, "gtk_drawing_area_get_type");
    HERO_UI_CANVAS_GTK_FIND(drawing_area_set_draw_func, "gtk_drawing_area_set_draw_func");
    HERO_UI_CANVAS_GTK_FIND(widget_queue_draw, "gtk_widget_queue_draw");
    HERO_UI_CANVAS_GTK_FIND(widget_set_focusable, "gtk_widget_set_focusable");
    HERO_UI_CANVAS_GTK_FIND(widget_grab_focus, "gtk_widget_grab_focus");
    HERO_UI_CANVAS_GTK_FIND(widget_pick, "gtk_widget_pick");
    HERO_UI_CANVAS_GTK_FIND(widget_compute_point, "gtk_widget_compute_point");
    HERO_UI_CANVAS_GTK_FIND(widget_add_controller, "gtk_widget_add_controller");
    HERO_UI_CANVAS_GTK_FIND(widget_get_native, "gtk_widget_get_native");
    HERO_UI_CANVAS_GTK_FIND(native_get_surface_transform, "gtk_native_get_surface_transform");
    HERO_UI_CANVAS_GTK_FIND(gesture_click_new, "gtk_gesture_click_new");
    HERO_UI_CANVAS_GTK_FIND(gesture_drag_new, "gtk_gesture_drag_new");
    HERO_UI_CANVAS_GTK_FIND(gesture_single_set_button, "gtk_gesture_single_set_button");
    HERO_UI_CANVAS_GTK_FIND(gesture_single_get_current_button, "gtk_gesture_single_get_current_button");
    HERO_UI_CANVAS_GTK_FIND(event_controller_motion_new, "gtk_event_controller_motion_new");
    HERO_UI_CANVAS_GTK_FIND(event_controller_key_new, "gtk_event_controller_key_new");
    HERO_UI_CANVAS_GTK_FIND(event_controller_get_current_event_state, "gtk_event_controller_get_current_event_state");
    HERO_UI_CANVAS_GTK_FIND(event_controller_get_current_event, "gtk_event_controller_get_current_event");
    HERO_UI_CANVAS_GTK_FIND(event_get_position, "gdk_event_get_position");
    HERO_UI_CANVAS_GTK_FIND(key_event_is_modifier, "gdk_key_event_is_modifier");
    HERO_UI_CANVAS_GTK_FIND(keyval_to_lower, "gdk_keyval_to_lower");
    HERO_UI_CANVAS_GTK_FIND(keyval_to_unicode, "gdk_keyval_to_unicode");
    HERO_UI_CANVAS_GTK_FIND(image_surface_create, "cairo_image_surface_create");
    HERO_UI_CANVAS_GTK_FIND(image_surface_get_data, "cairo_image_surface_get_data");
    HERO_UI_CANVAS_GTK_FIND(image_surface_get_stride, "cairo_image_surface_get_stride");
    HERO_UI_CANVAS_GTK_FIND(surface_status, "cairo_surface_status");
    HERO_UI_CANVAS_GTK_FIND(surface_flush, "cairo_surface_flush");
    HERO_UI_CANVAS_GTK_FIND(surface_mark_dirty, "cairo_surface_mark_dirty");
    HERO_UI_CANVAS_GTK_FIND(surface_destroy, "cairo_surface_destroy");
    HERO_UI_CANVAS_GTK_FIND(create, "cairo_create");
    HERO_UI_CANVAS_GTK_FIND(status, "cairo_status");
    HERO_UI_CANVAS_GTK_FIND(destroy, "cairo_destroy");
    HERO_UI_CANVAS_GTK_FIND(save, "cairo_save");
    HERO_UI_CANVAS_GTK_FIND(restore, "cairo_restore");
    HERO_UI_CANVAS_GTK_FIND(get_matrix, "cairo_get_matrix");
    HERO_UI_CANVAS_GTK_FIND(set_matrix, "cairo_set_matrix");
    HERO_UI_CANVAS_GTK_FIND(identity_matrix, "cairo_identity_matrix");
    HERO_UI_CANVAS_GTK_FIND(matrix_translate, "cairo_matrix_translate");
    HERO_UI_CANVAS_GTK_FIND(matrix_scale, "cairo_matrix_scale");
    HERO_UI_CANVAS_GTK_FIND(matrix_rotate, "cairo_matrix_rotate");
    HERO_UI_CANVAS_GTK_FIND(new_path, "cairo_new_path");
    HERO_UI_CANVAS_GTK_FIND(move_to, "cairo_move_to");
    HERO_UI_CANVAS_GTK_FIND(line_to, "cairo_line_to");
    HERO_UI_CANVAS_GTK_FIND(curve_to, "cairo_curve_to");
    HERO_UI_CANVAS_GTK_FIND(close_path, "cairo_close_path");
    HERO_UI_CANVAS_GTK_FIND(set_source_rgba, "cairo_set_source_rgba");
    HERO_UI_CANVAS_GTK_FIND(set_source_surface, "cairo_set_source_surface");
    HERO_UI_CANVAS_GTK_FIND(get_source, "cairo_get_source");
    HERO_UI_CANVAS_GTK_FIND(pattern_set_filter, "cairo_pattern_set_filter");
    HERO_UI_CANVAS_GTK_FIND(pattern_set_extend, "cairo_pattern_set_extend");
    HERO_UI_CANVAS_GTK_FIND(set_fill_rule, "cairo_set_fill_rule");
    HERO_UI_CANVAS_GTK_FIND(set_line_width, "cairo_set_line_width");
    HERO_UI_CANVAS_GTK_FIND(set_line_cap, "cairo_set_line_cap");
    HERO_UI_CANVAS_GTK_FIND(set_line_join, "cairo_set_line_join");
    HERO_UI_CANVAS_GTK_FIND(fill, "cairo_fill");
    HERO_UI_CANVAS_GTK_FIND(stroke, "cairo_stroke");
    HERO_UI_CANVAS_GTK_FIND(clip, "cairo_clip");
    HERO_UI_CANVAS_GTK_FIND(create_layout, "pango_cairo_create_layout");
    HERO_UI_CANVAS_GTK_FIND(show_layout, "pango_cairo_show_layout");
    HERO_UI_CANVAS_GTK_FIND(layout_set_text, "pango_layout_set_text");
    HERO_UI_CANVAS_GTK_FIND(layout_set_font_description, "pango_layout_set_font_description");
    HERO_UI_CANVAS_GTK_FIND(layout_set_single_paragraph_mode, "pango_layout_set_single_paragraph_mode");
    HERO_UI_CANVAS_GTK_FIND(font_description_new, "pango_font_description_new");
    HERO_UI_CANVAS_GTK_FIND(font_description_free, "pango_font_description_free");
    HERO_UI_CANVAS_GTK_FIND(font_description_set_family, "pango_font_description_set_family");
    HERO_UI_CANVAS_GTK_FIND(font_description_set_absolute_size, "pango_font_description_set_absolute_size");
    HERO_UI_CANVAS_GTK_FIND(font_description_set_weight, "pango_font_description_set_weight");
    HERO_UI_CANVAS_GTK_FIND(font_description_set_style, "pango_font_description_set_style");
    hero_ui_canvas_gtk_found = ok;
}

/* The functions, or NULL where GTK 4's library or one of them is missing. */
static const hero_ui_canvas_gtk_functions *hero_ui_canvas_gtk_loaded(void) {
    (void)pthread_once(&hero_ui_canvas_gtk_once, hero_ui_canvas_gtk_load);
    return hero_ui_canvas_gtk_found ? &hero_ui_canvas_gtk_found_functions : NULL;
}

/* `kind`, its words written into `words`. */
static int64_t hero_ui_canvas_gtk_said(int64_t kind, char *words, size_t room, const char *said) {
    if (words != NULL && room > 0) {
        (void)snprintf(words, room, "%s", said);
    }
    return kind;
}

/* -- THE DRAWING, DECODED -------------------------------------------------- */

/* One operation of `hero_ui.h`'s display list, its reals widened. */
typedef struct {
    uint8_t code;
    /* FILL's and CLIP's rule; STROKE's end. */
    uint8_t rule;
    /* STROKE's join. */
    uint8_t join;
    /* TEXT's style bits. */
    uint8_t style;
    uint8_t rgba[4];
    double v[6];
    /* TEXT's family, NULL for the system's font, and its text, each ending
     * with a zero byte; the text's own zero bytes, which draw nothing, are
     * left out. */
    char *family;
    char *text;
    /* IMAGE's pixels, a cairo image surface of premultiplied ARGB32, and its
     * columns and rows. */
    void *image;
    int64_t columns;
    int64_t rows;
} hero_ui_canvas_gtk_op;

typedef struct {
    hero_ui_canvas_gtk_op *ops;
    size_t count;
    /* The path's parts since the last BEGIN, as indices into `ops`, filled
     * as the replay walks; room for every part the drawing holds. */
    size_t *path;
} hero_ui_canvas_gtk_drawing;

struct hero_ui_canvas_gtk {
    /* The area's number, 0 once the arm dropped it: then nothing is told. */
    int64_t id;
    void *area;
    hero_ui_canvas_gtk_namer namer;
    hero_ui_canvas_gtk_drawing drawing;
    /* The pointer: the button held since a press in the area (0 for none),
     * that press's clicks, where a drag started, and the last point told. */
    int64_t held;
    int64_t clicks;
    double start_x;
    double start_y;
    double last_x;
    double last_y;
    /* The keys down, by their hardware codes. */
    unsigned down[HERO_UI_CANVAS_GTK_KEYS];
    size_t downs;
};

/* The bytes of a list, read from its front. */
typedef struct {
    const uint8_t *at;
    size_t left;
} hero_ui_canvas_gtk_reader;

static bool hero_ui_canvas_gtk_bytes(hero_ui_canvas_gtk_reader *reader, size_t count, const uint8_t **bytes) {
    if (reader->left < count) {
        return false;
    }
    *bytes = reader->at;
    reader->at += count;
    reader->left -= count;
    return true;
}

static bool hero_ui_canvas_gtk_u8(hero_ui_canvas_gtk_reader *reader, uint8_t *value) {
    const uint8_t *bytes;

    if (!hero_ui_canvas_gtk_bytes(reader, 1, &bytes)) {
        return false;
    }
    *value = bytes[0];
    return true;
}

static bool hero_ui_canvas_gtk_u32(hero_ui_canvas_gtk_reader *reader, size_t width, uint32_t *value) {
    const uint8_t *bytes;
    uint32_t read = 0;

    if (!hero_ui_canvas_gtk_bytes(reader, width, &bytes)) {
        return false;
    }
    for (size_t i = 0; i < width; i += 1) {
        read |= (uint32_t)bytes[i] << (8 * i);
    }
    *value = read;
    return true;
}

/* A real, little-endian binary32, widened; false where it is not finite. */
static bool hero_ui_canvas_gtk_real(hero_ui_canvas_gtk_reader *reader, double *value) {
    uint32_t bits;
    float number;

    if (!hero_ui_canvas_gtk_u32(reader, 4, &bits)) {
        return false;
    }
    memcpy(&number, &bits, sizeof number);
    *value = (double)number;
    return isfinite(*value);
}

static bool hero_ui_canvas_gtk_reals(hero_ui_canvas_gtk_reader *reader, hero_ui_canvas_gtk_op *op, size_t count) {
    for (size_t i = 0; i < count; i += 1) {
        if (!hero_ui_canvas_gtk_real(reader, &op->v[i])) {
            return false;
        }
    }
    return true;
}

static bool hero_ui_canvas_gtk_colour(hero_ui_canvas_gtk_reader *reader, hero_ui_canvas_gtk_op *op) {
    const uint8_t *bytes;

    if (!hero_ui_canvas_gtk_bytes(reader, 4, &bytes)) {
        return false;
    }
    memcpy(op->rgba, bytes, 4);
    return true;
}

/* `count` bytes of UTF-8 as a text ending with a zero byte, its own zero
 * bytes left out; NULL where memory is short. */
static char *hero_ui_canvas_gtk_text(const uint8_t *bytes, size_t count) {
    char *text = hero_layer_alloc(count + 1);
    size_t kept = 0;

    if (text == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < count; i += 1) {
        if (bytes[i] != 0) {
            text[kept++] = (char)bytes[i];
        }
    }
    text[kept] = '\0';
    return text;
}

/* `c` of alpha `a`, premultiplied and rounded. */
static uint32_t hero_ui_canvas_gtk_times(uint32_t c, uint32_t a) {
    return (c * a + 127u) / 255u;
}

/* The picture of `columns` by `rows` pixels of straight RGBA at `pixels` as
 * a cairo image surface, premultiplied ARGB32 in the machine's own order;
 * NULL where cairo could not make it. */
static void *hero_ui_canvas_gtk_picture(const hero_ui_canvas_gtk_functions *f, const uint8_t *pixels, int64_t columns, int64_t rows) {
    void *surface = f->image_surface_create(HERO_CAIRO_FORMAT_ARGB32, (int)columns, (int)rows);
    unsigned char *data;
    int stride;

    if (surface == NULL) {
        return NULL;
    }
    if (f->surface_status(surface) != HERO_CAIRO_STATUS_SUCCESS) {
        f->surface_destroy(surface);
        return NULL;
    }
    f->surface_flush(surface);
    data = f->image_surface_get_data(surface);
    stride = f->image_surface_get_stride(surface);
    if (data == NULL || stride < columns * 4) {
        f->surface_destroy(surface);
        return NULL;
    }
    for (int64_t row = 0; row < rows; row += 1) {
        for (int64_t column = 0; column < columns; column += 1) {
            const uint8_t *in = pixels + (row * columns + column) * 4;
            uint32_t a = in[3];
            uint32_t pixel = a << 24 | hero_ui_canvas_gtk_times(in[0], a) << 16 | hero_ui_canvas_gtk_times(in[1], a) << 8 | hero_ui_canvas_gtk_times(in[2], a);

            memcpy(data + row * stride + column * 4, &pixel, sizeof pixel);
        }
    }
    f->surface_mark_dirty(surface);
    return surface;
}

static void hero_ui_canvas_gtk_free_ops(const hero_ui_canvas_gtk_functions *f, hero_ui_canvas_gtk_op *ops, size_t count) {
    for (size_t i = 0; i < count; i += 1) {
        hero_layer_free(ops[i].family);
        hero_layer_free(ops[i].text);
        if (ops[i].image != NULL) {
            f->surface_destroy(ops[i].image);
        }
    }
    hero_layer_free(ops);
}

static void hero_ui_canvas_gtk_free_drawing(const hero_ui_canvas_gtk_functions *f, hero_ui_canvas_gtk_drawing *drawing) {
    hero_ui_canvas_gtk_free_ops(f, drawing->ops, drawing->count);
    hero_layer_free(drawing->path);
    drawing->ops = NULL;
    drawing->count = 0;
    drawing->path = NULL;
}

/* Reads one operation at `reader` into `op`, its text and its picture made
 * where `make` is true (a first walk only counts): `HERO_UI_OK`, or a refusal
 * whose words name the byte `at`. `kept` counts the states kept. */
static int64_t hero_ui_canvas_gtk_read(const hero_ui_canvas_gtk_functions *f, hero_ui_canvas_gtk_reader *reader, hero_ui_canvas_gtk_op *op, bool make, size_t at, int64_t *kept, char *words, size_t room) {
    char said[160];
    uint8_t code;
    bool read = true;

    memset(op, 0, sizeof *op);
    if (!hero_ui_canvas_gtk_u8(reader, &code)) {
        return HERO_UI_INVALID;
    }
    op->code = code;
    switch (code) {
    case 0x01:
        *kept += 1;
        read = *kept <= HERO_UI_CANVAS_GTK_KEPT_MOST;
        break;
    case 0x02:
        read = *kept > 0;
        *kept -= 1;
        break;
    case 0x10:
    case 0x15:
        break;
    case 0x03:
    case 0x04:
    case 0x11:
    case 0x12:
        read = hero_ui_canvas_gtk_reals(reader, op, 2);
        break;
    case 0x05:
        read = hero_ui_canvas_gtk_reals(reader, op, 1);
        break;
    case 0x13:
        read = hero_ui_canvas_gtk_reals(reader, op, 4);
        break;
    case 0x16:
    case 0x17:
        read = hero_ui_canvas_gtk_reals(reader, op, 4) && op->v[2] >= 0.0 && op->v[3] >= 0.0;
        break;
    case 0x14:
        read = hero_ui_canvas_gtk_reals(reader, op, 6);
        break;
    case 0x20:
        read = hero_ui_canvas_gtk_u8(reader, &op->rule) && op->rule <= 1 && hero_ui_canvas_gtk_colour(reader, op);
        break;
    case 0x22:
        read = hero_ui_canvas_gtk_u8(reader, &op->rule) && op->rule <= 1;
        break;
    case 0x21:
        read = hero_ui_canvas_gtk_reals(reader, op, 1) && op->v[0] > 0.0 && hero_ui_canvas_gtk_u8(reader, &op->rule) && op->rule <= 2 && hero_ui_canvas_gtk_u8(reader, &op->join) && op->join <= 2 && hero_ui_canvas_gtk_colour(reader, op);
        break;
    case 0x30: {
        uint32_t family = 0;
        uint32_t text = 0;
        const uint8_t *family_bytes = NULL;
        const uint8_t *text_bytes = NULL;

        read = hero_ui_canvas_gtk_reals(reader, op, 3) && op->v[2] > 0.0 && hero_ui_canvas_gtk_u8(reader, &op->style) && op->style <= 3 && hero_ui_canvas_gtk_colour(reader, op) && hero_ui_canvas_gtk_u32(reader, 2, &family) && hero_ui_canvas_gtk_u32(reader, 4, &text) && hero_ui_canvas_gtk_bytes(reader, family, &family_bytes) && hero_ui_canvas_gtk_bytes(reader, text, &text_bytes);
        if (read && op->v[2] > HERO_UI_CANVAS_GTK_TEXT_MOST) {
            (void)snprintf(said, sizeof said, "the drawing's text at byte %zu is %g points tall; Pango sizes a font in 1024ths of a unit in an int, so at most %.0f", at, op->v[2], HERO_UI_CANVAS_GTK_TEXT_MOST);
            return hero_ui_canvas_gtk_said(HERO_UI_UNSUPPORTED, words, room, said);
        }
        if (read && make) {
            op->family = family > 0 ? hero_ui_canvas_gtk_text(family_bytes, family) : NULL;
            op->text = hero_ui_canvas_gtk_text(text_bytes, text);
            if ((family > 0 && op->family == NULL) || op->text == NULL) {
                return hero_ui_canvas_gtk_said(HERO_UI_LIMIT, words, room, "the memory for a drawing's text could not be had");
            }
        }
        break;
    }
    case 0x31: {
        uint32_t columns = 0;
        uint32_t rows = 0;
        const uint8_t *pixels = NULL;

        read = hero_ui_canvas_gtk_reals(reader, op, 4) && op->v[2] > 0.0 && op->v[3] > 0.0 && hero_ui_canvas_gtk_u32(reader, 4, &columns) && hero_ui_canvas_gtk_u32(reader, 4, &rows) && columns >= 1 && columns <= HERO_UI_CANVAS_GTK_SIDE_MOST && rows >= 1 && rows <= HERO_UI_CANVAS_GTK_SIDE_MOST && hero_ui_canvas_gtk_bytes(reader, (size_t)columns * rows * 4, &pixels);
        op->columns = columns;
        op->rows = rows;
        if (read && make) {
            op->image = hero_ui_canvas_gtk_picture(f, pixels, columns, rows);
            if (op->image == NULL) {
                (void)snprintf(said, sizeof said, "cairo could not make the drawing's picture of %u by %u pixels at byte %zu: memory is short", columns, rows, at);
                return hero_ui_canvas_gtk_said(HERO_UI_LIMIT, words, room, said);
            }
        }
        break;
    }
    default:
        read = false;
        break;
    }
    if (!read) {
        (void)snprintf(said, sizeof said, "the drawing's operation at byte %zu (code 0x%02x) is not one hero_ui.h describes", at, (unsigned)code);
        return hero_ui_canvas_gtk_said(HERO_UI_INVALID, words, room, said);
    }
    return HERO_UI_OK;
}

/* Decodes the `count` bytes of `list` into `drawing`, read twice: once to
 * count the operations, then into an array of that many. `HERO_UI_OK`, or a
 * refusal with nothing kept. */
static int64_t hero_ui_canvas_gtk_decode(const hero_ui_canvas_gtk_functions *f, const uint8_t *list, size_t count, hero_ui_canvas_gtk_drawing *drawing, char *words, size_t room) {
    hero_ui_canvas_gtk_reader reader = {list, count};
    hero_ui_canvas_gtk_op op;
    size_t ops = 0;
    size_t parts = 0;
    int64_t kept = 0;
    int64_t answer;

    memset(drawing, 0, sizeof *drawing);
    while (reader.left > 0) {
        answer = hero_ui_canvas_gtk_read(f, &reader, &op, false, count - reader.left, &kept, words, room);
        if (answer != HERO_UI_OK) {
            return answer;
        }
        ops += 1;
        parts += op.code >= 0x11 && op.code <= 0x17 ? 1 : 0;
    }
    if (ops == 0) {
        return HERO_UI_OK;
    }
    drawing->ops = hero_layer_alloc_zero(ops, sizeof *drawing->ops);
    drawing->path = hero_layer_alloc_zero(parts > 0 ? parts : 1, sizeof *drawing->path);
    if (drawing->ops == NULL || drawing->path == NULL) {
        hero_layer_free(drawing->ops);
        hero_layer_free(drawing->path);
        memset(drawing, 0, sizeof *drawing);
        return hero_ui_canvas_gtk_said(HERO_UI_LIMIT, words, room, "the memory for a drawing's operations could not be had");
    }
    reader.at = list;
    reader.left = count;
    kept = 0;
    for (size_t i = 0; i < ops; i += 1) {
        answer = hero_ui_canvas_gtk_read(f, &reader, &drawing->ops[i], true, count - reader.left, &kept, words, room);
        if (answer != HERO_UI_OK) {
            hero_ui_canvas_gtk_free_ops(f, drawing->ops, i + 1);
            hero_layer_free(drawing->path);
            memset(drawing, 0, sizeof *drawing);
            return answer;
        }
    }
    drawing->count = ops;
    return HERO_UI_OK;
}

/* -- THE REPLAY ------------------------------------------------------------ */

/* Whether cairo takes `m` as a transform: every number finite and the plane
 * mapped onto an area. */
static bool hero_ui_canvas_gtk_invertible(const hero_cairo_matrix *m) {
    double determinant = m->xx * m->yy - m->yx * m->xy;

    return isfinite(m->xx) && isfinite(m->yx) && isfinite(m->xy) && isfinite(m->yy) && isfinite(m->x0) && isfinite(m->y0) && isfinite(determinant) && determinant != 0.0;
}

/* Gives cairo the path's `parts` again, in the space of the transform set
 * now; false where no part marks anything. */
static bool hero_ui_canvas_gtk_trace(const hero_ui_canvas_gtk_functions *f, void *cr, const hero_ui_canvas_gtk_drawing *drawing, size_t parts) {
    bool begun = false;
    double x = 0.0;
    double y = 0.0;
    double start_x = 0.0;
    double start_y = 0.0;

    f->new_path(cr);
    for (size_t i = 0; i < parts; i += 1) {
        const hero_ui_canvas_gtk_op *op = &drawing->ops[drawing->path[i]];
        const double *v = op->v;

        switch (op->code) {
        case 0x11:
            f->move_to(cr, v[0], v[1]);
            begun = true;
            x = start_x = v[0];
            y = start_y = v[1];
            break;
        case 0x12:
            if (begun) {
                f->line_to(cr, v[0], v[1]);
            } else {
                f->move_to(cr, v[0], v[1]);
                start_x = v[0];
                start_y = v[1];
                begun = true;
            }
            x = v[0];
            y = v[1];
            break;
        case 0x13:
            if (begun) {
                /* The cubic of a quadratic's three points, as Core Graphics
                 * draws a quadratic curve. */
                f->curve_to(cr, x + 2.0 / 3.0 * (v[0] - x), y + 2.0 / 3.0 * (v[1] - y), v[2] + 2.0 / 3.0 * (v[0] - v[2]), v[3] + 2.0 / 3.0 * (v[1] - v[3]), v[2], v[3]);
                x = v[2];
                y = v[3];
            }
            break;
        case 0x14:
            if (begun) {
                f->curve_to(cr, v[0], v[1], v[2], v[3], v[4], v[5]);
                x = v[4];
                y = v[5];
            }
            break;
        case 0x15:
            if (begun) {
                f->close_path(cr);
                x = start_x;
                y = start_y;
            }
            break;
        case 0x16:
            f->move_to(cr, v[0], v[1]);
            f->line_to(cr, v[0] + v[2], v[1]);
            f->line_to(cr, v[0] + v[2], v[1] + v[3]);
            f->line_to(cr, v[0], v[1] + v[3]);
            f->close_path(cr);
            begun = true;
            x = start_x = v[0];
            y = start_y = v[1];
            break;
        case 0x17: {
            /* Four cubic quarters, the circle's constant 0.5523 of each
             * radius out along the tangents. */
            const double k = 0.5522847498307936;
            double rx = v[2] / 2.0;
            double ry = v[3] / 2.0;
            double cx = v[0] + rx;
            double cy = v[1] + ry;

            f->move_to(cr, cx + rx, cy);
            f->curve_to(cr, cx + rx, cy + k * ry, cx + k * rx, cy + ry, cx, cy + ry);
            f->curve_to(cr, cx - k * rx, cy + ry, cx - rx, cy + k * ry, cx - rx, cy);
            f->curve_to(cr, cx - rx, cy - k * ry, cx - k * rx, cy - ry, cx, cy - ry);
            f->curve_to(cr, cx + k * rx, cy - ry, cx + rx, cy - k * ry, cx + rx, cy);
            f->close_path(cr);
            begun = true;
            x = start_x = cx + rx;
            y = start_y = cy;
            break;
        }
        default:
            break;
        }
    }
    return parts > 0;
}

static void hero_ui_canvas_gtk_source(const hero_ui_canvas_gtk_functions *f, void *cr, const uint8_t rgba[4]) {
    f->set_source_rgba(cr, rgba[0] / 255.0, rgba[1] / 255.0, rgba[2] / 255.0, rgba[3] / 255.0);
}

static void hero_ui_canvas_gtk_show_text(const hero_ui_canvas_gtk_functions *f, void *cr, const hero_ui_canvas_gtk_op *op) {
    void *layout;
    void *font;
    double size = op->v[2] * HERO_PANGO_SCALE;

    /* A font smaller than Pango's least unit draws nothing a pixel shows,
     * and Pango would read a size of 0 as its default. */
    if (size < 1.0 || op->text[0] == '\0') {
        return;
    }
    layout = f->create_layout(cr);
    if (layout == NULL) {
        return;
    }
    font = f->font_description_new();
    if (font == NULL) {
        f->object_unref(layout);
        return;
    }
    if (op->family != NULL && op->family[0] != '\0') {
        f->font_description_set_family(font, op->family);
    }
    f->font_description_set_absolute_size(font, size);
    if ((op->style & 1) != 0) {
        f->font_description_set_weight(font, HERO_PANGO_WEIGHT_BOLD);
    }
    if ((op->style & 2) != 0) {
        f->font_description_set_style(font, HERO_PANGO_STYLE_ITALIC);
    }
    f->layout_set_font_description(layout, font);
    f->layout_set_single_paragraph_mode(layout, 1);
    f->layout_set_text(layout, op->text, -1);
    f->new_path(cr);
    f->move_to(cr, op->v[0], op->v[1]);
    hero_ui_canvas_gtk_source(f, cr, op->rgba);
    f->show_layout(cr, layout);
    f->new_path(cr);
    f->font_description_free(font);
    f->object_unref(layout);
}

static void hero_ui_canvas_gtk_show_picture(const hero_ui_canvas_gtk_functions *f, void *cr, const hero_ui_canvas_gtk_op *op, const hero_cairo_matrix *now) {
    hero_cairo_matrix box = *now;
    void *pattern;

    f->matrix_translate(&box, op->v[0], op->v[1]);
    f->matrix_scale(&box, op->v[2] / (double)op->columns, op->v[3] / (double)op->rows);
    if (!hero_ui_canvas_gtk_invertible(&box)) {
        return;
    }
    f->set_matrix(cr, &box);
    f->set_source_surface(cr, op->image, 0.0, 0.0);
    pattern = f->get_source(cr);
    f->pattern_set_filter(pattern, HERO_CAIRO_FILTER_BILINEAR);
    f->pattern_set_extend(pattern, HERO_CAIRO_EXTEND_PAD);
    f->new_path(cr);
    f->move_to(cr, 0.0, 0.0);
    f->line_to(cr, (double)op->columns, 0.0);
    f->line_to(cr, (double)op->columns, (double)op->rows);
    f->line_to(cr, 0.0, (double)op->rows);
    f->close_path(cr);
    f->fill(cr);
}

/* Draws `drawing` on `cr`, which comes and goes in the same state: the
 * function both GTK's draw and the snapshot call. */
static void hero_ui_canvas_gtk_replay(const hero_ui_canvas_gtk_functions *f, const hero_ui_canvas_gtk_drawing *drawing, void *cr) {
    hero_cairo_matrix kept[HERO_UI_CANVAS_GTK_KEPT_MOST];
    hero_cairo_matrix now;
    size_t depth = 0;
    size_t parts = 0;

    if (drawing->count == 0 || f->status(cr) != HERO_CAIRO_STATUS_SUCCESS) {
        return;
    }
    f->save(cr);
    f->get_matrix(cr, &now);
    for (size_t i = 0; i < drawing->count && f->status(cr) == HERO_CAIRO_STATUS_SUCCESS; i += 1) {
        const hero_ui_canvas_gtk_op *op = &drawing->ops[i];
        bool shown = hero_ui_canvas_gtk_invertible(&now);

        switch (op->code) {
        case 0x01:
            if (depth < HERO_UI_CANVAS_GTK_KEPT_MOST) {
                kept[depth++] = now;
                f->save(cr);
            }
            break;
        case 0x02:
            if (depth > 0) {
                now = kept[--depth];
                f->restore(cr);
            }
            break;
        case 0x03:
            f->matrix_translate(&now, op->v[0], op->v[1]);
            break;
        case 0x04:
            f->matrix_scale(&now, op->v[0], op->v[1]);
            break;
        case 0x05:
            f->matrix_rotate(&now, op->v[0]);
            break;
        case 0x10:
            parts = 0;
            break;
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
            drawing->path[parts++] = i;
            break;
        case 0x20:
            if (shown) {
                f->set_matrix(cr, &now);
                if (hero_ui_canvas_gtk_trace(f, cr, drawing, parts)) {
                    f->set_fill_rule(cr, op->rule);
                    hero_ui_canvas_gtk_source(f, cr, op->rgba);
                    f->fill(cr);
                }
            }
            break;
        case 0x21:
            if (shown) {
                f->set_matrix(cr, &now);
                if (hero_ui_canvas_gtk_trace(f, cr, drawing, parts)) {
                    f->set_line_width(cr, op->v[0]);
                    f->set_line_cap(cr, op->rule);
                    f->set_line_join(cr, op->join);
                    hero_ui_canvas_gtk_source(f, cr, op->rgba);
                    f->stroke(cr);
                }
            }
            break;
        case 0x22:
            if (parts == 0) {
                break;
            }
            if (shown) {
                f->set_matrix(cr, &now);
                (void)hero_ui_canvas_gtk_trace(f, cr, drawing, parts);
            } else {
                /* The path mapped onto no area: an empty path clips all. */
                f->identity_matrix(cr);
                f->new_path(cr);
            }
            f->set_fill_rule(cr, op->rule);
            f->clip(cr);
            break;
        case 0x30:
            if (shown) {
                f->set_matrix(cr, &now);
                hero_ui_canvas_gtk_show_text(f, cr, op);
            }
            break;
        case 0x31:
            if (shown) {
                hero_ui_canvas_gtk_show_picture(f, cr, op, &now);
            }
            break;
        default:
            break;
        }
    }
    while (depth > 0) {
        depth -= 1;
        f->restore(cr);
    }
    f->new_path(cr);
    f->restore(cr);
}

/* GTK draws the area: its drawing replayed on GTK's context. */
static void hero_ui_canvas_gtk_drawn(void *area, void *cr, int width, int height, void *data) {
    const hero_ui_canvas_gtk_functions *f = hero_ui_canvas_gtk_loaded();
    hero_ui_canvas_gtk *canvas = data;

    (void)area;
    (void)width;
    (void)height;
    if (f != NULL && canvas != NULL) {
        hero_ui_canvas_gtk_replay(f, &canvas->drawing, cr);
    }
}

/* GTK finalises the area: its record goes with it. */
static void hero_ui_canvas_gtk_gone(void *data) {
    const hero_ui_canvas_gtk_functions *f = hero_ui_canvas_gtk_loaded();
    hero_ui_canvas_gtk *canvas = data;

    if (f != NULL && canvas != NULL) {
        hero_ui_canvas_gtk_free_drawing(f, &canvas->drawing);
    }
    hero_layer_free(canvas);
}

/* -- THE POINTER AND THE KEYS, TOLD ---------------------------------------- */

/* The keys GDK says are held, as `hero_ui.h` adds them up: Control is the
 * primary key off Apple's systems. */
static int64_t hero_ui_canvas_gtk_keys_held(unsigned state) {
    return ((state & HERO_GDK_CONTROL_MASK) != 0 ? HERO_UI_PRIMARY : 0) | ((state & HERO_GDK_SHIFT_MASK) != 0 ? HERO_UI_SHIFT : 0) | ((state & HERO_GDK_ALT_MASK) != 0 ? HERO_UI_ALT : 0);
}

/* A point in 64ths of a point, rounded, held within what `hero_ui.c` takes. */
static int64_t hero_ui_canvas_gtk_sixty_fourths(double value) {
    double scaled = value * 64.0;

    if (!isfinite(scaled)) {
        return 0;
    }
    if (scaled > HERO_UI_CANVAS_GTK_POINT_MOST) {
        scaled = HERO_UI_CANVAS_GTK_POINT_MOST;
    } else if (scaled < -HERO_UI_CANVAS_GTK_POINT_MOST) {
        scaled = -HERO_UI_CANVAS_GTK_POINT_MOST;
    }
    return (int64_t)(scaled + (scaled >= 0.0 ? 0.5 : -0.5));
}

static void hero_ui_canvas_gtk_tell(hero_ui_canvas_gtk *canvas, int64_t phase, double x, double y, int64_t button, int64_t clicks, int64_t held) {
    uint8_t bytes[4];

    if (canvas->id == 0) {
        return;
    }
    bytes[0] = (uint8_t)phase;
    bytes[1] = (uint8_t)button;
    bytes[2] = (uint8_t)(clicks > 255 ? 255 : clicks);
    bytes[3] = (uint8_t)held;
    canvas->last_x = x;
    canvas->last_y = y;
    hero_ui_tell_text(HERO_UI_EVENT_POINTER, canvas->id, hero_ui_canvas_gtk_sixty_fourths(x), hero_ui_canvas_gtk_sixty_fourths(y), bytes, 4);
}

/* `hero_ui.h`'s button of GDK's button `button`. */
static int64_t hero_ui_canvas_gtk_button(unsigned button) {
    return button == HERO_GDK_BUTTON_PRIMARY ? HERO_UI_POINTER_LEFT : button == HERO_GDK_BUTTON_SECONDARY ? HERO_UI_POINTER_RIGHT : HERO_UI_POINTER_OTHER;
}

/* A press, given by GTK's click gesture with its count of clicks: the area
 * takes the keys, as a person's click gives them. */
static void hero_ui_canvas_gtk_pressed(void *gesture, int presses, double x, double y, void *data) {
    const hero_ui_canvas_gtk_functions *f = hero_ui_canvas_gtk_loaded();
    hero_ui_canvas_gtk *canvas = data;

    if (f == NULL) {
        return;
    }
    (void)f->widget_grab_focus(canvas->area);
    canvas->held = hero_ui_canvas_gtk_button(f->gesture_single_get_current_button(gesture));
    canvas->clicks = presses < 1 ? 1 : presses;
    hero_ui_canvas_gtk_tell(canvas, HERO_UI_POINTER_DOWN, x, y, canvas->held, canvas->clicks, hero_ui_canvas_gtk_keys_held(f->event_controller_get_current_event_state(gesture)));
}

static void hero_ui_canvas_gtk_drag_began(void *gesture, double x, double y, void *data) {
    hero_ui_canvas_gtk *canvas = data;

    (void)gesture;
    canvas->start_x = x;
    canvas->start_y = y;
}

static void hero_ui_canvas_gtk_dragged(void *gesture, double dx, double dy, void *data) {
    const hero_ui_canvas_gtk_functions *f = hero_ui_canvas_gtk_loaded();
    hero_ui_canvas_gtk *canvas = data;

    if (f != NULL && canvas->held != 0) {
        hero_ui_canvas_gtk_tell(canvas, HERO_UI_POINTER_DRAG, canvas->start_x + dx, canvas->start_y + dy, canvas->held, 0, hero_ui_canvas_gtk_keys_held(f->event_controller_get_current_event_state(gesture)));
    }
}

/* The release, which GTK's drag gesture gives however far the pointer went;
 * its clicks are its press's. */
static void hero_ui_canvas_gtk_drag_ended(void *gesture, double dx, double dy, void *data) {
    const hero_ui_canvas_gtk_functions *f = hero_ui_canvas_gtk_loaded();
    hero_ui_canvas_gtk *canvas = data;
    int64_t button = canvas->held;

    if (f != NULL && button != 0) {
        canvas->held = 0;
        hero_ui_canvas_gtk_tell(canvas, HERO_UI_POINTER_UP, canvas->start_x + dx, canvas->start_y + dy, button, canvas->clicks, hero_ui_canvas_gtk_keys_held(f->event_controller_get_current_event_state(gesture)));
    }
}

static void hero_ui_canvas_gtk_entered(void *controller, double x, double y, void *data) {
    const hero_ui_canvas_gtk_functions *f = hero_ui_canvas_gtk_loaded();

    if (f != NULL) {
        hero_ui_canvas_gtk_tell(data, HERO_UI_POINTER_ENTER, x, y, 0, 0, hero_ui_canvas_gtk_keys_held(f->event_controller_get_current_event_state(controller)));
    }
}

/* A move with no button held; one with a button held is the drag gesture's. */
static void hero_ui_canvas_gtk_moved(void *controller, double x, double y, void *data) {
    const hero_ui_canvas_gtk_functions *f = hero_ui_canvas_gtk_loaded();
    hero_ui_canvas_gtk *canvas = data;

    if (f != NULL && canvas->held == 0) {
        hero_ui_canvas_gtk_tell(canvas, HERO_UI_POINTER_MOVE, x, y, 0, 0, hero_ui_canvas_gtk_keys_held(f->event_controller_get_current_event_state(controller)));
    }
}

/* The pointer left: GTK's leave carries no point, so the crossing event's
 * own, from its surface into the area's space; the last point told where
 * GTK gives none. */
static void hero_ui_canvas_gtk_left(void *controller, void *data) {
    const hero_ui_canvas_gtk_functions *f = hero_ui_canvas_gtk_loaded();
    hero_ui_canvas_gtk *canvas = data;
    double x = canvas->last_x;
    double y = canvas->last_y;
    void *event;
    void *native;
    double surface_x = 0.0;
    double surface_y = 0.0;
    double shift_x = 0.0;
    double shift_y = 0.0;
    float in[2];
    float out[2];

    if (f == NULL) {
        return;
    }
    event = f->event_controller_get_current_event(controller);
    native = f->widget_get_native(canvas->area);
    if (event != NULL && native != NULL && f->event_get_position(event, &surface_x, &surface_y)) {
        f->native_get_surface_transform(native, &shift_x, &shift_y);
        in[0] = (float)(surface_x - shift_x);
        in[1] = (float)(surface_y - shift_y);
        if (f->widget_compute_point(native, canvas->area, in, out)) {
            x = out[0];
            y = out[1];
        }
    }
    hero_ui_canvas_gtk_tell(canvas, HERO_UI_POINTER_EXIT, x, y, 0, 0, hero_ui_canvas_gtk_keys_held(f->event_controller_get_current_event_state(controller)));
}

/* `code` UTF-8 into `out`, its length; 0 for no character. */
static size_t hero_ui_canvas_gtk_utf8(uint32_t code, char *out) {
    if (code == 0 || (code >= 0xD800 && code <= 0xDFFF) || code > 0x10FFFF) {
        return 0;
    }
    if (code < 0x80) {
        out[0] = (char)code;
        return 1;
    }
    if (code < 0x800) {
        out[0] = (char)(0xC0 | code >> 6);
        out[1] = (char)(0x80 | (code & 0x3F));
        return 2;
    }
    if (code < 0x10000) {
        out[0] = (char)(0xE0 | code >> 12);
        out[1] = (char)(0x80 | (code >> 6 & 0x3F));
        out[2] = (char)(0x80 | (code & 0x3F));
        return 3;
    }
    out[0] = (char)(0xF0 | code >> 18);
    out[1] = (char)(0x80 | (code >> 12 & 0x3F));
    out[2] = (char)(0x80 | (code >> 6 & 0x3F));
    out[3] = (char)(0x80 | (code & 0x3F));
    return 4;
}

/* A key told: its name, one zero byte and what it types, GDK's character
 * of its key value (none for a key GDK maps to none, as an arrow, and none
 * while Control, the primary key, is held). */
static void hero_ui_canvas_gtk_key(const hero_ui_canvas_gtk_functions *f, hero_ui_canvas_gtk *canvas, unsigned keyval, unsigned state, int64_t phase) {
    char text[64];
    size_t named = 0;
    size_t typed = 0;
    int64_t held = hero_ui_canvas_gtk_keys_held(state);

    if (canvas->id == 0) {
        return;
    }
    if (canvas->namer != NULL) {
        named = canvas->namer(f->keyval_to_lower(keyval), text, sizeof text - 6);
    }
    if (named >= sizeof text - 6) {
        named = 0;
    }
    text[named] = '\0';
    if ((held & HERO_UI_PRIMARY) == 0) {
        typed = hero_ui_canvas_gtk_utf8(f->keyval_to_unicode(keyval), text + named + 1);
    }
    hero_ui_tell_text(HERO_UI_EVENT_KEY, canvas->id, held + 256 * phase, 0, (const uint8_t *)text, (int64_t)(named + 1 + typed));
}

static int hero_ui_canvas_gtk_key_pressed(void *controller, unsigned keyval, unsigned code, unsigned state, void *data) {
    const hero_ui_canvas_gtk_functions *f = hero_ui_canvas_gtk_loaded();
    hero_ui_canvas_gtk *canvas = data;
    void *event;
    int64_t phase = 0;

    if (f == NULL) {
        return 0;
    }
    event = f->event_controller_get_current_event(controller);
    if (event != NULL && f->key_event_is_modifier(event)) {
        return 0;
    }
    for (size_t i = 0; i < canvas->downs; i += 1) {
        phase = canvas->down[i] == code ? 1 : phase;
    }
    if (phase == 0 && canvas->downs < HERO_UI_CANVAS_GTK_KEYS) {
        canvas->down[canvas->downs++] = code;
    }
    hero_ui_canvas_gtk_key(f, canvas, keyval, state, phase);
    return 1;
}

static void hero_ui_canvas_gtk_key_released(void *controller, unsigned keyval, unsigned code, unsigned state, void *data) {
    const hero_ui_canvas_gtk_functions *f = hero_ui_canvas_gtk_loaded();
    hero_ui_canvas_gtk *canvas = data;
    void *event;

    if (f == NULL) {
        return;
    }
    event = f->event_controller_get_current_event(controller);
    if (event != NULL && f->key_event_is_modifier(event)) {
        return;
    }
    for (size_t i = 0; i < canvas->downs; i += 1) {
        if (canvas->down[i] == code) {
            canvas->down[i] = canvas->down[--canvas->downs];
            break;
        }
    }
    hero_ui_canvas_gtk_key(f, canvas, keyval, state, 2);
}

/* -- THE CALLS THE GTK ARM MAKES ------------------------------------------- */

static void hero_ui_canvas_gtk_on(const hero_ui_canvas_gtk_functions *f, void *instance, const char *signal, hero_ui_canvas_gtk_callback handler, hero_ui_canvas_gtk *canvas) {
    (void)f->signal_connect_data(instance, signal, handler, canvas, NULL, 0);
}

int64_t hero_ui_canvas_gtk_make(int64_t id, hero_ui_canvas_gtk_namer namer, void **area, hero_ui_canvas_gtk **canvas, char *words, size_t room) {
    const hero_ui_canvas_gtk_functions *f = hero_ui_canvas_gtk_loaded();
    hero_ui_canvas_gtk *made;
    void *widget;
    void *click;
    void *drag;
    void *motion;
    void *keys;

    *area = NULL;
    *canvas = NULL;
    if (f == NULL) {
        return hero_ui_canvas_gtk_said(HERO_UI_UNSUPPORTED, words, room, "GTK 4's library, or cairo, Pango or PangoCairo through it, lacks a function the drawing area calls (GTK 4.12's gtk_widget_compute_point among them)");
    }
    made = hero_layer_alloc_zero(1, sizeof *made);
    if (made == NULL) {
        return hero_ui_canvas_gtk_said(HERO_UI_LIMIT, words, room, "the memory for a drawing area could not be had");
    }
    /* Its role is the image's, fixed as it is made, as GTK fixes a role:
     * one element a screen reader names by its accessible name. */
    widget = f->object_new(f->drawing_area_get_type(), "accessible-role", HERO_GTK_ACCESSIBLE_ROLE_IMG, (const char *)NULL);
    if (widget == NULL) {
        hero_layer_free(made);
        return hero_ui_canvas_gtk_said(HERO_UI_OTHER, words, room, "GTK made no drawing area");
    }
    (void)f->object_ref_sink(widget);
    made->id = id;
    made->area = widget;
    made->namer = namer;
    /* From here the record is GTK's to free, as the area is finalised. */
    f->drawing_area_set_draw_func(widget, hero_ui_canvas_gtk_drawn, made, hero_ui_canvas_gtk_gone);
    f->widget_set_focusable(widget, 1);
    click = f->gesture_click_new();
    f->gesture_single_set_button(click, 0);
    hero_ui_canvas_gtk_on(f, click, "pressed", (hero_ui_canvas_gtk_callback)hero_ui_canvas_gtk_pressed, made);
    f->widget_add_controller(widget, click);
    drag = f->gesture_drag_new();
    f->gesture_single_set_button(drag, 0);
    hero_ui_canvas_gtk_on(f, drag, "drag-begin", (hero_ui_canvas_gtk_callback)hero_ui_canvas_gtk_drag_began, made);
    hero_ui_canvas_gtk_on(f, drag, "drag-update", (hero_ui_canvas_gtk_callback)hero_ui_canvas_gtk_dragged, made);
    hero_ui_canvas_gtk_on(f, drag, "drag-end", (hero_ui_canvas_gtk_callback)hero_ui_canvas_gtk_drag_ended, made);
    f->widget_add_controller(widget, drag);
    motion = f->event_controller_motion_new();
    hero_ui_canvas_gtk_on(f, motion, "enter", (hero_ui_canvas_gtk_callback)hero_ui_canvas_gtk_entered, made);
    hero_ui_canvas_gtk_on(f, motion, "motion", (hero_ui_canvas_gtk_callback)hero_ui_canvas_gtk_moved, made);
    hero_ui_canvas_gtk_on(f, motion, "leave", (hero_ui_canvas_gtk_callback)hero_ui_canvas_gtk_left, made);
    f->widget_add_controller(widget, motion);
    keys = f->event_controller_key_new();
    hero_ui_canvas_gtk_on(f, keys, "key-pressed", (hero_ui_canvas_gtk_callback)hero_ui_canvas_gtk_key_pressed, made);
    hero_ui_canvas_gtk_on(f, keys, "key-released", (hero_ui_canvas_gtk_callback)hero_ui_canvas_gtk_key_released, made);
    f->widget_add_controller(widget, keys);
    *area = widget;
    *canvas = made;
    return HERO_UI_OK;
}

void hero_ui_canvas_gtk_forget(hero_ui_canvas_gtk *canvas) {
    const hero_ui_canvas_gtk_functions *f = hero_ui_canvas_gtk_loaded();
    void *area;

    if (canvas == NULL || f == NULL) {
        return;
    }
    area = canvas->area;
    canvas->id = 0;
    /* The record may be freed by this very call, the area finalised. */
    f->object_unref(area);
}

int64_t hero_ui_canvas_gtk_set_drawing(hero_ui_canvas_gtk *canvas, const uint8_t *list, int64_t count, char *words, size_t room) {
    const hero_ui_canvas_gtk_functions *f = hero_ui_canvas_gtk_loaded();
    hero_ui_canvas_gtk_drawing made;
    int64_t answer;

    if (f == NULL || canvas == NULL || count < 0 || (count > 0 && list == NULL)) {
        return HERO_UI_INVALID;
    }
    answer = hero_ui_canvas_gtk_decode(f, list, (size_t)count, &made, words, room);
    if (answer != HERO_UI_OK) {
        return answer;
    }
    hero_ui_canvas_gtk_free_drawing(f, &canvas->drawing);
    canvas->drawing = made;
    f->widget_queue_draw(canvas->area);
    return HERO_UI_OK;
}

int64_t hero_ui_canvas_gtk_snapshot(hero_ui_canvas_gtk *canvas, int64_t width, int64_t height, uint8_t *rgba, int64_t count, char *words, size_t room) {
    const hero_ui_canvas_gtk_functions *f = hero_ui_canvas_gtk_loaded();
    char said[160];
    void *surface;
    void *cr;
    unsigned char *data;
    int stride;

    if (f == NULL || canvas == NULL || width < 0 || height < 0) {
        return HERO_UI_INVALID;
    }
    if (width > HERO_UI_CANVAS_GTK_SIDE_MOST || height > HERO_UI_CANVAS_GTK_SIDE_MOST) {
        (void)snprintf(said, sizeof said, "the drawing area is %lld by %lld points; a snapshot's side is at most %d", (long long)width, (long long)height, HERO_UI_CANVAS_GTK_SIDE_MOST);
        return hero_ui_canvas_gtk_said(HERO_UI_OTHER, words, room, said);
    }
    if (width == 0 || height == 0 || rgba == NULL || count < width * height * 4) {
        return HERO_UI_OK;
    }
    surface = f->image_surface_create(HERO_CAIRO_FORMAT_ARGB32, (int)width, (int)height);
    if (surface == NULL || f->surface_status(surface) != HERO_CAIRO_STATUS_SUCCESS) {
        if (surface != NULL) {
            f->surface_destroy(surface);
        }
        (void)snprintf(said, sizeof said, "cairo could not make a bitmap of %lld by %lld pixels: memory is short", (long long)width, (long long)height);
        return hero_ui_canvas_gtk_said(HERO_UI_OTHER, words, room, said);
    }
    cr = f->create(surface);
    hero_ui_canvas_gtk_replay(f, &canvas->drawing, cr);
    f->destroy(cr);
    f->surface_flush(surface);
    data = f->image_surface_get_data(surface);
    stride = f->image_surface_get_stride(surface);
    if (data == NULL || stride < width * 4) {
        f->surface_destroy(surface);
        return hero_ui_canvas_gtk_said(HERO_UI_OTHER, words, room, "cairo gave no pixels for the drawing area's bitmap");
    }
    /* cairo holds premultiplied ARGB32 in the machine's order: each pixel
     * read whole, then its alpha taken back out of its colour. */
    for (int64_t row = 0; row < height; row += 1) {
        for (int64_t column = 0; column < width; column += 1) {
            uint32_t pixel;
            uint32_t a;
            uint8_t *out = rgba + (row * width + column) * 4;

            memcpy(&pixel, data + row * stride + column * 4, sizeof pixel);
            a = pixel >> 24;
            if (a == 0) {
                memset(out, 0, 4);
                continue;
            }
            for (int i = 0; i < 3; i += 1) {
                uint32_t c = (pixel >> (16 - 8 * i)) & 0xFFu;
                uint32_t straight = (c * 255u + a / 2u) / a;

                out[i] = (uint8_t)(straight > 255u ? 255u : straight);
            }
            out[3] = (uint8_t)a;
        }
    }
    f->surface_destroy(surface);
    return HERO_UI_OK;
}

int64_t hero_ui_canvas_gtk_pointer(hero_ui_canvas_gtk *canvas, void *window, int64_t x, int64_t y, int64_t phase, int64_t button, int64_t clicks, int64_t modifiers, char *words, size_t room) {
    const hero_ui_canvas_gtk_functions *f = hero_ui_canvas_gtk_loaded();
    double at_x = (double)x / 64.0;
    double at_y = (double)y / 64.0;

    if (f == NULL || canvas == NULL || window == NULL) {
        return HERO_UI_INVALID;
    }
    if (phase == HERO_UI_POINTER_MOVE || phase == HERO_UI_POINTER_ENTER || phase == HERO_UI_POINTER_EXIT) {
        hero_ui_canvas_gtk_tell(canvas, phase, at_x, at_y, 0, 0, modifiers);
        return HERO_UI_OK;
    }
    /* A press, and a drag or a release with no press held in the area, go
     * where GTK's hit test sends them; one with a press held goes to the
     * area the press started in, as GTK's implicit grab sends it. */
    if (phase == HERO_UI_POINTER_DOWN || canvas->held == 0) {
        float point[2] = {(float)at_x, (float)at_y};
        float in_window[2];

        if (!f->widget_compute_point(canvas->area, window, point, in_window)) {
            return hero_ui_canvas_gtk_said(HERO_UI_OTHER, words, room, "GTK places the drawing area in no window it has laid out");
        }
        if (f->widget_pick(window, in_window[0], in_window[1], HERO_GTK_PICK_DEFAULT) != canvas->area) {
            return hero_ui_canvas_gtk_said(HERO_UI_OTHER, words, room, "GTK's hit test gave the pointer to another widget than the drawing area: something stands over the point, or the point is outside the area");
        }
    }
    if (phase == HERO_UI_POINTER_DOWN) {
        (void)f->widget_grab_focus(canvas->area);
        canvas->held = button;
        canvas->clicks = clicks;
    } else if (phase == HERO_UI_POINTER_UP && canvas->held == button) {
        canvas->held = 0;
    }
    hero_ui_canvas_gtk_tell(canvas, phase, at_x, at_y, button, phase == HERO_UI_POINTER_DRAG ? 0 : clicks, modifiers);
    return HERO_UI_OK;
}

#endif
