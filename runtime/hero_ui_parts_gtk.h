/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_ui_parts_gtk.h: a drawing area's parts read by a screen reader on
 * GTK 4 (`hero_ui.h`'s A DRAWING AREA'S PARTS, READ BY A SCREEN READER), the
 * member the drawing area's (`platform/hero_ui_canvas_gtk.c`) makes its area
 * through and the GTK arm (`platform/hero_ui_gtk.c`) hands the three calls
 * to. A header of the platform layer's own and no group's: `hero_ui.c` checks
 * the list whole (`hero_ui_parts_checked`), the act's numbers and the thread
 * before any of these runs.
 *
 * WHAT GTK 4 OFFERS, read in GTK 4.18.6's source and measured in the Linux
 * image. Since GTK 4.10 an object of a program's own type may implement
 * `GtkAccessible`, and GTK's AT-SPI backend presents it to a screen reader as
 * it presents a widget: its role, name, description, states and bounds, and
 * through `GtkAccessibleRange` the Value interface, whose setter is how a
 * reader steps a slider. Two things it gives such an object none of: the
 * Action interface, a reader's press, which `gtk/a11y/gtkatspiaction.c` gives
 * to widgets alone, and a reader's request for the focus, which
 * `gtk/a11y/gtkatspicomponent.c` answers NOT_SUPPORTED for every object,
 * widgets included. So on GTK a part is read, walked and stepped, and a press
 * or a focus asked of it is refused `unsupported` with those words.
 *
 * A WIDGET LISTS ONLY WIDGET CHILDREN to the reader (gtkwidget.c's
 * `get_first_accessible_child`), so the area is made of a type of this
 * member's, a `GtkDrawingArea` that lists its parts instead. GTK fixes a
 * widget's role as it is made, so the area is a group from the start: with
 * parts it holds them, and with none it holds one image of the area's own
 * size, named by the area's accessible name.
 *
 * EVERY CALL ON THE UI THREAD, the GTK arm's. Each answers a kind of
 * `hero_ui.h`, `HERO_UI_OK` or a refusal whose words it writes into `words`
 * (`room` bytes, zero byte included).
 */

#ifndef HERO_UI_PARTS_GTK_H
#define HERO_UI_PARTS_GTK_H

#include <stddef.h>
#include <stdint.h>

/* The `GType` of the drawing area this member's parts stand under, 0 where
 * GTK's library lacks a call the parts make (GTK 4.10's among them): the
 * area is then a plain `GtkDrawingArea` of role image, and the three calls
 * below refuse `unsupported`. */
size_t hero_ui_parts_gtk_area_type(void);

/* The accessible role an area of that type is made with: a group. */
int hero_ui_parts_gtk_area_role(void);

/* Gives `area`, made of the type above, drawing area `id`'s record: no
 * parts, and the image standing for it. `HERO_UI_LIMIT` where memory is
 * short. An area of another type is left as it is. */
int64_t hero_ui_parts_gtk_attach(void *area, int64_t id, char *words, size_t room);

/* The arm drops the area: its parts are taken back, and nothing is told for
 * it after. */
void hero_ui_parts_gtk_forget(void *area);

/* The area's accessible name, `name` NUL-terminated or NULL for none, which
 * the image standing for it carries while it has no parts. */
void hero_ui_parts_gtk_named(void *area, const char *name);

/* Replaces the area's parts by the `count` bytes of `list`, `hero_ui.h`'s
 * records (a null list of count 0 takes them back). */
int64_t hero_ui_parts_gtk_set(void *area, const uint8_t *list, int64_t count, char *words, size_t room);

/* Writes the area's parts as GTK offers them to a reader into `buffer`
 * where `count` holds them, and their length through `length` always. */
int64_t hero_ui_parts_gtk_read(void *area, uint8_t *buffer, int64_t count, int64_t *length, char *words, size_t room);

/* Acts on part `part` (1 the first) as a screen reader does, `act` one of
 * `HERO_UI_PART_PRESS` to `_DECREASE`. */
int64_t hero_ui_parts_gtk_act(void *area, int64_t part, int64_t act, char *words, size_t room);

#endif
