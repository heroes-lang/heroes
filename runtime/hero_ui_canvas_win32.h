/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_ui_canvas_win32.h: the Win32 arm's drawing area (`hero_ui.h`'s A
 * DRAWING AREA, WITH ITS MOUSE AND ITS KEYS), which `hero_ui_win32.c` calls
 * for a widget of kind `HERO_UI_CANVAS`. A header of the members alone, as
 * `platform/hero_ui_arm.h` is: no Heroes group names it. `hero_ui_win32.c`
 * keeps the area's window, its place, its keys' window and its name; the
 * member keeps its drawing, draws it, reads it back, and tells what the
 * pointer and the keys do in it. Every function is called on the UI thread,
 * and is defined on Windows alone.
 *
 * ONE PROTOTYPE ON EVERY SYSTEM (panel 234's R6): a window is its `HWND` as
 * `void *`, a message's words `uint32_t` (`UINT`), `uintptr_t` (`WPARAM`) and
 * `intptr_t` (`LPARAM`, `LRESULT`), which are the same types on Windows'
 * 64-bit targets.
 */

#ifndef HERO_UI_CANVAS_WIN32_H
#define HERO_UI_CANVAS_WIN32_H

#include <stdbool.h>
#include <stdint.h>

typedef struct hero_ui_canvas_win32 hero_ui_canvas_win32;

/* A drawing area's record, holding no drawing; NULL where none can be made,
 * `*answer` then `HERO_UI_UNSUPPORTED` (Direct2D or DirectWrite is not on
 * this system) or `HERO_UI_LIMIT` (no memory), said through
 * `hero_ui_refused`. */
hero_ui_canvas_win32 *hero_ui_canvas_win32_new(int64_t *answer);

/* Frees `canvas`, a null one too. */
void hero_ui_canvas_win32_free(hero_ui_canvas_win32 *canvas);

/* Keeps the `count` bytes of `list` as the area's drawing in place of the
 * last and has the area's window `window` drawn again: `HERO_UI_OK`;
 * `HERO_UI_INVALID` with words for bytes that are no display list,
 * `HERO_UI_LIMIT` with none kept, the last drawing kept either way. */
int64_t hero_ui_canvas_win32_set(hero_ui_canvas_win32 *canvas, void *window, const uint8_t *list, int64_t count);

/* The drawing replayed at one pixel to a point into `rgba`, `width` by
 * `height` points, 8-bit RGBA with straight alpha, row 0 the top, where
 * `count` holds width * height * 4 bytes; nothing written where it does not.
 * `HERO_UI_OK`, or `HERO_UI_OTHER` with words (a side past 8192, a replay
 * Direct2D refused). */
int64_t hero_ui_canvas_win32_snapshot(hero_ui_canvas_win32 *canvas, int64_t width, int64_t height, uint8_t *rgba, int64_t count);

/* Message `message` of the area's window `window`, widget `id` of a window
 * at `dpi`: answers whether the member took it, `*result` then what the
 * window procedure answers; one it did not take goes to `DefWindowProcW`. */
bool hero_ui_canvas_win32_message(hero_ui_canvas_win32 *canvas, int64_t id, void *window, uint32_t dpi, uint32_t message, uintptr_t wparam, intptr_t lparam, intptr_t *result);

/* The pointer acting as the person's in the area (`hero_ui_arm_pointer`, the
 * arguments `hero_ui.c` checked): `HERO_UI_OK` once the one event is told;
 * `HERO_UI_INVALID` for an area whose window is not shown; `HERO_UI_OTHER`
 * with words where the window's hit test gives the point to another window,
 * as the person's press there would be. */
int64_t hero_ui_canvas_win32_pointer(hero_ui_canvas_win32 *canvas, int64_t id, void *window, uint32_t dpi, int64_t x, int64_t y, int64_t phase, int64_t button, int64_t clicks, int64_t modifiers);

/* Gives back what the areas of the UI thread share (the factories) at the
 * arm's `close`, once every area is freed. */
void hero_ui_canvas_win32_end(void);

#endif
