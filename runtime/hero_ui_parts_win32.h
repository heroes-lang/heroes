/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_ui_parts_win32.h: the Win32 arm's parts of a drawing area for a
 * screen reader (`hero_ui.h`'s A DRAWING AREA'S PARTS, READ BY A SCREEN
 * READER), which `hero_ui_win32.c` calls for a widget of kind
 * `HERO_UI_CANVAS`. A header of the members alone, as `platform/hero_ui_arm.h`
 * is: no Heroes group names it. `hero_ui_win32.c` keeps the area's window,
 * its scale and its name; the member keeps the parts as UI Automation
 * objects, answers the window's `WM_GETOBJECT` with them, tells what a
 * reader does to them, and reads them back from them. Every function is
 * called on the UI thread, and is defined on Windows alone.
 *
 * ONE PROTOTYPE ON EVERY SYSTEM (panel 234's R6), as
 * `hero_ui_canvas_win32.h`: a window is its `HWND` as `void *`, a message's
 * words `uint32_t`, `uintptr_t` and `intptr_t`, a name UTF-16 as `uint16_t`.
 */

#ifndef HERO_UI_PARTS_WIN32_H
#define HERO_UI_PARTS_WIN32_H

#include <stdbool.h>
#include <stdint.h>

typedef struct hero_ui_parts_win32 hero_ui_parts_win32;

/* The scale of drawing area `id`'s window as the arm keeps it, 96 one pixel
 * to a point: asked on the UI thread at each placing of the area, each list
 * and each read-back, and kept for the objects UI Automation calls on its
 * own threads, so a box follows the window to another monitor. */
typedef uint32_t (*hero_ui_parts_win32_scale)(int64_t id);

/* Whether UI Automation is on this system (`UIAutomationCore.dll` and the
 * functions the member calls): `HERO_UI_OK`, or `HERO_UI_UNSUPPORTED` with
 * words naming what is missing, said through `hero_ui_refused`. */
int64_t hero_ui_parts_win32_offered(void);

/* Drawing area `id`'s parts, none yet, for its window `window`; NULL where
 * none can be made, `*answer` then `HERO_UI_UNSUPPORTED` (as `offered`) or
 * `HERO_UI_LIMIT` (no memory). */
hero_ui_parts_win32 *hero_ui_parts_win32_new(int64_t id, void *window, hero_ui_parts_win32_scale scale, int64_t *answer);

/* Gives the parts back at the area's end: UI Automation told the window
 * offers none, every object disconnected from its clients; a null one too. */
void hero_ui_parts_win32_free(hero_ui_parts_win32 *parts);

/* The area's name for a reader, `name` UTF-16 ended by a 0, NULL for none:
 * the program's, or its tied label's (`hero_ui_win32.c`'s ACCESSIBLE
 * NAMES). Kept as a copy; `HERO_UI_OK` or `HERO_UI_LIMIT`. */
int64_t hero_ui_parts_win32_name(hero_ui_parts_win32 *parts, const uint16_t *name);

/* Replaces the parts by the `count` bytes of `list`, records `hero_ui.c`
 * checked: `HERO_UI_OK`; `HERO_UI_LIMIT` with the last parts kept;
 * `HERO_UI_INVALID` for an area whose window is gone. */
int64_t hero_ui_parts_win32_set(hero_ui_parts_win32 *parts, const uint8_t *list, int64_t count);

/* The parts read back from their UI Automation objects into `buffer` as
 * `hero_ui.h`'s records, where `count` holds them, their length through
 * `length` either way: `HERO_UI_OK`, or `HERO_UI_OTHER` with words where an
 * object answered no value. */
int64_t hero_ui_parts_win32_read(hero_ui_parts_win32 *parts, uint8_t *buffer, int64_t count, int64_t *length);

/* A screen reader's act `act` (`HERO_UI_PART_PRESS` to `_DECREASE`) on part
 * `part` (1 the first), through the pattern the reader's call reaches:
 * `HERO_UI_OK` once told; `HERO_UI_INVALID` with words for a part that is
 * none or an act its role or state takes none of. */
int64_t hero_ui_parts_win32_act(hero_ui_parts_win32 *parts, int64_t part, int64_t act);

/* Message `message` of the area's window: answers whether the member took
 * it, `*result` then what the window procedure answers (`WM_GETOBJECT` for
 * UI Automation's root while the area has parts); the member watches
 * `WM_SETFOCUS` and `WM_DESTROY` and leaves them to the window. */
bool hero_ui_parts_win32_message(hero_ui_parts_win32 *parts, uint32_t message, uintptr_t wparam, intptr_t lparam, intptr_t *result);

#endif
