/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_appkit_system.h: the functions
 * `hero_ui_appkit_system.swift` exports to C, which `hero_ui.c`'s macOS arm
 * calls on the system around the program: a notification, the appearance.
 * A header of the member's own, as `hero_ui_appkit.h` is, held to the
 * header `swiftc` writes at every compile of the layer
 * (`selfhost/cli/layer_swift.hero`).
 *
 * Each is called on the main thread alone, with the arguments `hero_ui.c`
 * has checked; each answers a kind of `hero_ui.h`, and `words` the bytes of
 * the last `HERO_UI_OTHER`'s or `HERO_UI_UNSUPPORTED`'s words it wrote.
 */

#ifndef HERO_UI_APPKIT_SYSTEM_H
#define HERO_UI_APPKIT_SYSTEM_H

#include <stdint.h>

int64_t hero_ui_appkit_system_words(char *words, int64_t count);

int64_t hero_ui_appkit_notify(const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count);

int64_t hero_ui_appkit_appearance(int64_t *appearance);

void hero_ui_appkit_appearance_watch(void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t));

int64_t hero_ui_appkit_appearance_set(int64_t appearance);

#endif
