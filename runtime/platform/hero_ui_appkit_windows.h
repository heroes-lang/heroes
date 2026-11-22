/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_appkit_windows.h: the functions
 * `hero_ui_appkit_windows.swift` exports to C, which `hero_ui.c`'s macOS arm
 * calls to act on a window as the system shows it. A header of the member's
 * own, as `hero_ui_appkit.h` is, held to the header `swiftc` writes at every
 * compile of the layer (`selfhost/cli/layer_swift.hero`).
 *
 * Each is called on the main thread alone, with the arguments `hero_ui.c`
 * has checked, and the AppKit objects it acts on handed as `void *` by
 * `hero_ui_appkit_object` within the same call; each answers a kind of
 * `hero_ui.h`, and `words` the bytes of the last `HERO_UI_OTHER`'s words it
 * wrote.
 */

#ifndef HERO_UI_APPKIT_WINDOWS_H
#define HERO_UI_APPKIT_WINDOWS_H

#include <stdint.h>

int64_t hero_ui_appkit_windows_words(char *words, int64_t count);

int64_t hero_ui_appkit_press_key(void *window, void *typed, const uint8_t *key, int64_t count, int64_t modifiers);

int64_t hero_ui_appkit_windows_watch(void *window, int64_t id, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t));

int64_t hero_ui_appkit_screens(int64_t *values, int64_t count, int64_t *screens);

int64_t hero_ui_appkit_frame_of(void *window, int64_t *screen, int64_t *x, int64_t *y, int64_t *width, int64_t *height);

int64_t hero_ui_appkit_set_position(void *window, int64_t screen, int64_t x, int64_t y, int64_t person);

int64_t hero_ui_appkit_center(void *window, void *over);

int64_t hero_ui_appkit_set_state(void *window, int64_t state, int64_t person);

int64_t hero_ui_appkit_state(void *window, int64_t *state);

int64_t hero_ui_appkit_set_on_top(void *window, int64_t on);

int64_t hero_ui_appkit_set_framed(void *window, int64_t on);

int64_t hero_ui_appkit_set_opacity(void *window, int64_t alpha);

int64_t hero_ui_appkit_set_backdrop(void *window, int64_t backdrop);

int64_t hero_ui_appkit_set_icon(const uint8_t *rgba, int64_t width, int64_t height);

int64_t hero_ui_appkit_set_modal(void *window, int64_t on);

int64_t hero_ui_appkit_modal_here(void);

int64_t hero_ui_appkit_modal_pump(int64_t ms, int64_t (*held)(void));

#endif
