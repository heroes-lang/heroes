/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_ui_canvas_gtk.h: the drawing area's arm on GTK 4 (`hero_ui.h`'s A
 * DRAWING AREA, WITH ITS MOUSE AND ITS KEYS, kind `HERO_UI_CANVAS`), which
 * the GTK arm, `platform/hero_ui_gtk.c`, calls for a widget of that kind. A
 * header of the platform layer's own and no group's: a program binds
 * `hero_ui.h`, whose functions check every argument, the drawing whole among
 * them (`hero_ui_drawing_checked`), and the thread before any of these runs.
 *
 * WHAT THE GTK ARM HOLDS, AND WHAT THIS MEMBER HOLDS. The GTK arm keeps the
 * windows, places the area as it places a picture and gives it the keys;
 * this member makes the area, a `GtkDrawingArea` of role image, keeps its
 * drawing decoded once into operations of its own, replays them with cairo
 * and Pango whenever GTK draws the area and into a bitmap of its own for a
 * snapshot (the SAME replay, so a case reads what the person sees), and
 * tells the pointer and the keys GTK gives the area. It finds cairo's,
 * Pango's and the GTK calls it makes by `dlsym` on GTK 4's library, as the
 * GTK arm finds its own, so a build needs no header of either.
 *
 * WHO OUTLIVES WHOM. `make` answers the area with a reference of its own,
 * which `forget` gives back as the arm drops the widget; the record this
 * member keeps for the area is freed by GTK when the area itself is
 * finalised, never before, so a draw or a signal GTK runs late reads a
 * record that stands, and one that runs after `forget` tells nothing.
 *
 * EVERY CALL ON THE UI THREAD, the GTK arm's. Each answers a kind of
 * `hero_ui.h`, `HERO_UI_OK` or a refusal whose words it writes into `words`
 * (`room` bytes, zero byte included) for the arm to keep.
 */

#ifndef HERO_UI_CANVAS_GTK_H
#define HERO_UI_CANVAS_GTK_H

#include <stddef.h>
#include <stdint.h>

/* What this member keeps for one drawing area. */
typedef struct hero_ui_canvas_gtk hero_ui_canvas_gtk;

/* The name `hero_ui_set_shortcut` gives the key of GDK key value `keyval`
 * (a letter in lower case, a digit, `Up`, `Enter` and the rest), written
 * into `name` with its zero byte where it fits in `room`; its length, 0 for
 * a key it names none. The GTK arm's, whose table names the keys. */
typedef size_t (*hero_ui_canvas_gtk_namer)(unsigned keyval, char *name, size_t room);

/* Makes drawing area `id`, drawing nothing, its pointer and keys told:
 * `*area` its `GtkWidget *`, with a reference this member holds, and
 * `*canvas` its record. `HERO_UI_UNSUPPORTED` where GTK's library lacks a
 * call it makes, `HERO_UI_LIMIT` where memory is short, `HERO_UI_OTHER`
 * where GTK made none; both NULL then. */
int64_t hero_ui_canvas_gtk_make(int64_t id, hero_ui_canvas_gtk_namer namer, void **area, hero_ui_canvas_gtk **canvas, char *words, size_t room);

/* The arm drops the area: nothing is told for it after, and the reference
 * `make` took is given back. */
void hero_ui_canvas_gtk_forget(hero_ui_canvas_gtk *canvas);

/* Replaces the area's drawing by the `count` bytes of `list`, `hero_ui.h`'s
 * display list checked whole by `hero_ui.c` (a null list of count 0 clears
 * it), and has the area drawn again. `HERO_UI_INVALID` for a list this
 * member reads as malformed all the same, `HERO_UI_UNSUPPORTED` for a text
 * taller than Pango sizes a font, `HERO_UI_LIMIT` where memory is short; the
 * last drawing stays on each. */
int64_t hero_ui_canvas_gtk_set_drawing(hero_ui_canvas_gtk *canvas, const uint8_t *list, int64_t count, char *words, size_t room);

/* The drawing replayed at one pixel to a point into `rgba`, `width` by
 * `height` pixels, 8-bit RGBA with straight alpha, row 0 the top, where
 * `count` holds width * height * 4 (nothing written where it does not).
 * `HERO_UI_OTHER` for a side past 8192 or a bitmap cairo could not make. */
int64_t hero_ui_canvas_gtk_snapshot(hero_ui_canvas_gtk *canvas, int64_t width, int64_t height, uint8_t *rgba, int64_t count, char *words, size_t room);

/* Acts as the person's pointer does at `x`, `y`, in 64ths of a point from
 * the area's top-left corner, the phase, button, clicks and keys held
 * `hero_ui.h`'s, the area standing in the `GtkWindow` `window`, shown.
 * `HERO_UI_OTHER` where GTK's hit test gives a press to another widget. */
int64_t hero_ui_canvas_gtk_pointer(hero_ui_canvas_gtk *canvas, void *window, int64_t x, int64_t y, int64_t phase, int64_t button, int64_t clicks, int64_t modifiers, char *words, size_t room);

#endif
