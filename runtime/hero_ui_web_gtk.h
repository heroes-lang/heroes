/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_ui_web_gtk.h: the web view's arm on Linux, WebKitGTK 6.0 (the GTK 4
 * build, `libwebkitgtk-6.0.so.4`, with libsoup 3), which the GTK arm,
 * `platform/hero_ui_gtk.c`, calls for a widget of kind `HERO_UI_WEBVIEW`
 * (`hero_ui.h`, THE WEB VIEW). A header of the platform layer's own, as
 * `hero_ui_appkit.h` is, and no group's: a program binds `hero_ui.h`, whose
 * functions check every argument and the thread before any of these runs.
 *
 * WHAT THE GTK ARM HOLDS, AND WHAT THIS MEMBER HOLDS. The GTK arm opens GTK 4
 * and keeps the windows; this member opens WebKitGTK 6.0 with `dlopen` at the
 * first web view, as `hero_keyring.c` opens libsecret, so a build needs
 * neither library's headers and a machine without it answers
 * `HERO_UI_UNSUPPORTED` with words naming the library. `create` answers the
 * web view as a `GtkWidget *`, which the arm places in its window as any of
 * its widgets; before the arm destroys that widget it calls `destroy`, which
 * lets go of everything this member holds for it and tells the layer through
 * the host's `gone`.
 *
 * EVERY CALL ON THE UI THREAD, the GTK arm's, with the arguments
 * `hero_ui.c` checked; each answers a kind of `hero_ui.h`, `HERO_UI_OK`
 * where done, and `words` gives the words of the last failure on this
 * thread. A URL is the whole URL of the app's own origin `hero_ui.c` built;
 * a text is UTF-8 holding no zero byte, at most `HERO_UI_WEB_MESSAGE_MOST`
 * bytes.
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): the
 * member's Linux arm and the arm every other system compiles, which answers
 * `HERO_UI_UNSUPPORTED`, are both compiled against these prototypes.
 */

#ifndef HERO_UI_WEB_GTK_H
#define HERO_UI_WEB_GTK_H

#include <stdint.h>

#include "platform/hero_ui_web.h"

/* Makes web view `id`, asking `host` (whose functions and bridge outlive
 * it) what it serves and where its page may go, and writes the
 * `GtkWidget *` through `widget`, not yet placed. `UNSUPPORTED` where
 * WebKitGTK 6.0 is not on this machine, `LIMIT`, `OTHER`. */
int64_t hero_ui_web_gtk_create(int64_t id, const hero_ui_web_host *host, void **widget);

/* Lets go of web view `id`, its page stopped and its handlers taken away,
 * and calls the host's `gone`; the arm then destroys its widget.
 * `NOT_FOUND` where this member holds no web view `id`. */
int64_t hero_ui_web_gtk_destroy(int64_t id);

/* Has web view `id` show `url`, `count` bytes. */
int64_t hero_ui_web_gtk_load(int64_t id, const uint8_t *url, int64_t count);

/* Hands `text`, `count` bytes, to the function the page gave
 * `heroes.receive`, or keeps it until a page of the app's own origin has
 * loaded; `LIMIT` where `HERO_UI_QUEUE_MOST` wait already. */
int64_t hero_ui_web_gtk_post(int64_t id, const uint8_t *text, int64_t count);

/* Has web view `id`'s page send `text`, `count` bytes, by its own bridge,
 * now or once a page of the app's own origin has loaded (`hero_ui_inject`'s
 * `HERO_UI_EVENT_MESSAGE`); `LIMIT` as `post`. */
int64_t hero_ui_web_gtk_send(int64_t id, const uint8_t *text, int64_t count);

/* Writes the words of this thread's last failure into `words`, `count`
 * bytes with their zero, cut where a character ends; answers the bytes
 * written, 0 where none failed. */
int64_t hero_ui_web_gtk_words(char *words, int64_t count);

#endif
