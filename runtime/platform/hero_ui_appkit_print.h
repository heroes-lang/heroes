/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_appkit_print.h: the functions
 * `hero_ui_appkit_print.swift` exports to C, which `hero_ui.c`'s macOS arm
 * calls to show the print and the page-setup dialogs as sheets of a window
 * and to answer one as the person would (`platform/hero_ui_print_win32.h`, THE
 * SETTINGS' TEXT). A header of the member's own, as
 * `hero_ui_appkit_dialogs.h` is, held to the header `swiftc` writes at every
 * compile of the layer (`selfhost/cli/layer_swift.hero`).
 *
 * Each is called on the main thread alone, the window's `NSWindow` handed
 * as `void *` by `hero_ui_appkit_object` within the same call and the
 * function the answer is queued by beside it; each answers a kind of
 * `hero_ui.h`, and `words` the bytes of the last refusal's words it wrote.
 */

#ifndef HERO_UI_APPKIT_PRINT_H
#define HERO_UI_APPKIT_PRINT_H

#include <stdint.h>

int64_t hero_ui_appkit_print_words(char *words, int64_t count);

int64_t hero_ui_appkit_print_show(void *window, int64_t dialog, int64_t page, const uint8_t *settings, int64_t count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t));

int64_t hero_ui_appkit_print_answer(int64_t dialog, int64_t value, const uint8_t *settings, int64_t count);

void hero_ui_appkit_print_end(void *window);

#endif
