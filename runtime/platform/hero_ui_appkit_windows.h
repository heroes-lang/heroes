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

#endif
