/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_appkit_dialogs.h: the functions
 * `hero_ui_appkit_dialogs.swift` exports to C, which `hero_ui.c`'s macOS arm
 * calls to show a dialog as a sheet of a window and to answer one as the
 * person would (`hero_ui.h`'s DIALOGS ANSWERED THROUGH THE QUEUE). A header
 * of the member's own, as `hero_ui_appkit.h` is, held to the header
 * `swiftc` writes at every compile of the layer
 * (`selfhost/cli/layer_swift.hero`).
 *
 * Each is called on the main thread alone, with the arguments `hero_ui.c`
 * has checked, the window's `NSWindow` handed as `void *` by
 * `hero_ui_appkit_object` within the same call and the function the answer
 * is queued by beside it; each answers a kind of `hero_ui.h`, and `words`
 * the bytes of the last `HERO_UI_OTHER`'s or `HERO_UI_UNSUPPORTED`'s words
 * it wrote.
 */

#ifndef HERO_UI_APPKIT_DIALOGS_H
#define HERO_UI_APPKIT_DIALOGS_H

#include <stdint.h>

int64_t hero_ui_appkit_dialogs_words(char *words, int64_t count);

int64_t hero_ui_appkit_show_message(void *window, int64_t id, int64_t dialog, int64_t tone, int64_t buttons, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t));

int64_t hero_ui_appkit_show_ask(void *window, int64_t id, int64_t dialog, int64_t tone, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count, const uint8_t *buttons, int64_t buttons_count, int64_t cancel, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t));

int64_t hero_ui_appkit_show_files(void *window, int64_t id, int64_t dialog, int64_t save, int64_t multiple, const uint8_t *title, int64_t title_count, const uint8_t *name, int64_t name_count, const uint8_t *filters, int64_t filters_count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t));

int64_t hero_ui_appkit_show_folders(void *window, int64_t id, int64_t dialog, int64_t multiple, const uint8_t *title, int64_t title_count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t));

int64_t hero_ui_appkit_answer_dialog(int64_t dialog, int64_t value, const uint8_t *paths, int64_t paths_count);

void hero_ui_appkit_dialogs_end(int64_t window);

#endif
