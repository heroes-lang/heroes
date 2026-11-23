/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_print_gtk.h — the GTK 4 arm of the print and page-setup
 * dialogs, which `hero_ui.c`'s GTK 4 arm calls (M-rebel-library,
 * 2026-11-23). A header of the members alone, as `hero_ui_arm.h` is: no
 * Heroes group names it.
 *
 * THE SETTINGS' TEXT is `hero_ui_print_win32.h`'s, the same on every arm.
 *
 * ANSWERED THROUGH THE QUEUE: both dialogs are GTK 4.14's `GtkPrintDialog`,
 * whose setup holds the page setup too, shown with `gtk_print_dialog_setup`
 * over the window's `GtkWindow`, which answers its call at once and the
 * person later; the answer is queued by `push`, `HERO_UI_EVENT_DIALOG`
 * naming `dialog`, its value 1 with the settings chosen in the text, 0 where
 * the person cancelled. Every call is on the UI thread, where GTK's main
 * loop runs the answer.
 */

#ifndef HERO_UI_PRINT_GTK_H
#define HERO_UI_PRINT_GTK_H

#include <stdint.h>

/* Shows the print dialog (`page` 0) or the page-setup dialog (`page` 1) over
 * the `GtkWindow` `window`, its fields first set from the `count` bytes of
 * `settings`, and answers `HERO_UI_OK` at once; or a kind of `hero_ui.h`,
 * nothing queued: `HERO_UI_INVALID` for settings that are not the text,
 * `HERO_UI_UNSUPPORTED` off GTK or where GTK is older than 4.14,
 * `HERO_UI_OTHER`, `words` saying why. */
int64_t hero_ui_print_gtk_show(void *window, int64_t dialog, int64_t page, const uint8_t *settings, int64_t count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t));

/* Prints the page the `WebKitWebView` `view` shows, over the `GtkWindow`
 * `window`: through GTK's print dialog where `ask` is 1 (the call returns
 * once the person has answered it), straight to the printer of the `count`
 * bytes of `settings` where it is 0; the answer queued by `push` as dialog
 * `dialog`'s, value 1 with the settings printed with once WebKit has handed
 * the page to the system, 0 where the person cancelled or WebKit failed.
 * `HERO_UI_UNSUPPORTED` where WebKitGTK 6.0 is not installed. */
int64_t hero_ui_print_gtk_web(void *view, void *window, int64_t dialog, int64_t ask, const uint8_t *settings, int64_t count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t));

/* Answers dialog `dialog` as its person would: chosen with the `count` bytes
 * of `settings` where `value` is 1, cancelled where it is 0, and answers
 * `HERO_UI_OK`; `HERO_UI_NOT_FOUND` where no such dialog stands,
 * `HERO_UI_INVALID` for settings that are not the text. */
int64_t hero_ui_print_gtk_answer(int64_t dialog, int64_t value, const uint8_t *settings, int64_t count);

/* Ends unanswered every dialog over the `GtkWindow` `window`, or every one
 * where `window` is NULL, before the window or the app goes. */
void hero_ui_print_gtk_end(void *window);

/* The words of the last refusal on this thread, written into `words` where
 * they fit with their zero byte in `count`, and their length. */
int64_t hero_ui_print_gtk_words(char *words, int64_t count);

#endif
