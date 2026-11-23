/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_print_win32.h — the Win32 arm of the print and
 * page-setup dialogs, which `hero_ui.c`'s Win32 arm calls (M-rebel-library,
 * 2026-11-23). A header of the members alone, as `hero_ui_arm.h` is: no
 * Heroes group names it.
 *
 * THE SETTINGS' TEXT, the same on every arm (`hero_ui_appkit_print.swift`,
 * `hero_ui_print_gtk.c`) and in `printing.format_settings`: UTF-8 lines
 * `<key>=<value>`, each ended by a line feed, the keys and values IPP's own
 * job-template attributes (RFC 8011, 5.2): `printer` (a printer's name as
 * `hero_printing.h` lists it, empty for the default), `copies`, `media` (a
 * PWG name, empty for the printer's), `orientation-requested` (3 to 6),
 * `sides` (`one-sided`, `two-sided-long-edge`, `two-sided-short-edge`),
 * `print-color-mode` (`auto`, `color`, `monochrome`),
 * `multiple-document-handling` (`separate-documents-collated-copies`,
 * `separate-documents-uncollated-copies`) and `page-ranges`
 * (`<first>-<last>` joined by commas, empty for every page). A key not
 * given keeps its default: one copy, upright, one side, the printer's
 * colour, collated, every page.
 *
 * THE WAITING FORM (`hero_ui_arm.h`): `PrintDlgExW` and `PageSetupDlgW` are
 * modal, so `show` waits for the person and queues the answer by `push`
 * before it returns: `HERO_UI_EVENT_DIALOG` naming `dialog`, its value 1
 * with the settings chosen in the text, 0 where the person cancelled. No
 * dialog of this arm stands after `show` returns, so `answer` finds none and
 * `end` has none to end.
 */

#ifndef HERO_UI_PRINT_WIN32_H
#define HERO_UI_PRINT_WIN32_H

#include <stdint.h>

/* Shows the print dialog (`page` 0) or the page-setup dialog (`page` 1) over
 * the window `window` (its `HWND`), its fields first set from the `count`
 * bytes of `settings`, waits for the person, queues the answer by `push`,
 * and answers `HERO_UI_OK`; or a kind of `hero_ui.h`, nothing queued:
 * `HERO_UI_INVALID` for settings that are not the text above,
 * `HERO_UI_NOT_FOUND` for a printer no spooler names, `HERO_UI_UNSUPPORTED`
 * off Windows, `HERO_UI_OTHER` where the system refused, `words` saying
 * why. */
int64_t hero_ui_print_win32_show(void *window, int64_t dialog, int64_t page, const uint8_t *settings, int64_t count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t));

/* Prints the page the WebView2 `view` shows (its `ICoreWebView2`), over the
 * window `window`: through the print dialog first where `ask` is 1, whose
 * Cancel is queued at once, then with WebView2's own `Print` to the printer
 * of the settings, the answer queued by `push` as dialog `dialog`'s once
 * WebView2 has handed the page to the system, value 1 with the settings
 * printed with, 0 where it could not. `HERO_UI_UNSUPPORTED` off Windows or
 * where the WebView2 runtime predates `ICoreWebView2_16`. */
int64_t hero_ui_print_win32_web(void *view, void *window, int64_t dialog, int64_t ask, const uint8_t *settings, int64_t count, void (*push)(int64_t, int64_t, int64_t, int64_t, const uint8_t *, int64_t));

/* `HERO_UI_NOT_FOUND`: no dialog of this arm stands to be answered (THE
 * WAITING FORM). */
int64_t hero_ui_print_win32_answer(int64_t dialog, int64_t value, const uint8_t *settings, int64_t count);

/* Marks ended every web view's print over the `HWND` `window`, or every
 * one where it is NULL: its completion queues nothing. No dialog of this
 * arm stands to be ended. */
void hero_ui_print_win32_end(void *window);

/* The words of the last refusal on this thread, written into `words` where
 * they fit with their zero byte in `count`, and their length. */
int64_t hero_ui_print_win32_words(char *words, int64_t count);

#endif
