/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_ui_web_win32.h: the web view's arm on Windows, Microsoft Edge's
 * WebView2, which the Win32 arm, `platform/hero_ui_win32.c`, calls for a
 * widget of kind `HERO_UI_WEBVIEW` (`hero_ui.h`, THE WEB VIEW). A header of
 * the platform layer's own, as `hero_ui_appkit.h` is, and no group's: a
 * program binds `hero_ui.h`, whose functions check every argument and the
 * thread before any of these runs.
 *
 * WHAT THE WIN32 ARM HOLDS, AND WHAT THIS MEMBER HOLDS. The Win32 arm keeps
 * the windows; this member loads `WebView2Loader.dll` at the first web view
 * and asks it for the WebView2 runtime the machine has, so a build needs no
 * SDK and a machine without the loader or the runtime answers
 * `HERO_UI_UNSUPPORTED` with words naming which. A web view is no window of
 * the arm's: WebView2 draws it into the parent window it is given, so the
 * arm places it and shows it through `place` and `show`, in pixels of that
 * window's client area, and calls `destroy` before it destroys the parent.
 *
 * EVERY CALL ON THE UI THREAD, the Win32 arm's, with the arguments
 * `hero_ui.c` checked. `create` runs the thread's messages while WebView2
 * makes its environment and its controller, at most `HERO_UI_WEB_WIN32_WAIT`
 * milliseconds, and the arm's window procedures may be called meanwhile, as
 * in any modal loop. Each answers a kind of `hero_ui.h`, `HERO_UI_OK` where
 * done; `words` gives the words of the last failure on this thread. A URL is
 * the whole URL of the app's own origin `hero_ui.c` built; a text is UTF-8
 * holding no zero byte, at most `HERO_UI_WEB_MESSAGE_MOST` bytes.
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): the
 * member's Windows arm and the arm every other system compiles, which
 * answers `HERO_UI_UNSUPPORTED`, are both compiled against these prototypes.
 */

#ifndef HERO_UI_WEB_WIN32_H
#define HERO_UI_WEB_WIN32_H

#include <stdint.h>

#include "platform/hero_ui_web.h"

/* The longest `create` waits for WebView2, in milliseconds. */
#define HERO_UI_WEB_WIN32_WAIT 30000

/* Makes web view `id` in the window `parent` (an `HWND`), hidden, asking
 * `host` (whose functions and bridge outlive it) what it serves and where
 * its page may go. `UNSUPPORTED` where the loader or the runtime is
 * missing, `LIMIT`, `OTHER`. */
int64_t hero_ui_web_win32_create(int64_t id, const hero_ui_web_host *host, void *parent);

/* Places web view `id` at `left`, `top` in its window's client area,
 * `width` by `height` pixels. */
int64_t hero_ui_web_win32_place(int64_t id, int64_t left, int64_t top, int64_t width, int64_t height);

/* Shows web view `id`, `on` 1, or hides it, 0. */
int64_t hero_ui_web_win32_show(int64_t id, int64_t on);

/* Closes web view `id`, lets go of everything this member holds for it, and
 * calls the host's `gone`. `NOT_FOUND` where it holds no web view `id`. */
int64_t hero_ui_web_win32_destroy(int64_t id);

/* Has web view `id` show `url`, `count` bytes. */
int64_t hero_ui_web_win32_load(int64_t id, const uint8_t *url, int64_t count);

/* Hands `text`, `count` bytes, to the function the page gave
 * `heroes.receive`, or keeps it until a page of the app's own origin has
 * loaded; `LIMIT` where `HERO_UI_QUEUE_MOST` wait already. */
int64_t hero_ui_web_win32_post(int64_t id, const uint8_t *text, int64_t count);

/* Has web view `id`'s page send `text`, `count` bytes, by its own bridge,
 * now or once a page of the app's own origin has loaded; `LIMIT` as
 * `post`. */
int64_t hero_ui_web_win32_send(int64_t id, const uint8_t *text, int64_t count);

/* Writes the words of this thread's last failure into `words`, `count`
 * bytes with their zero, cut where a character ends; answers the bytes
 * written, 0 where none failed. */
int64_t hero_ui_web_win32_words(char *words, int64_t count);

#endif
