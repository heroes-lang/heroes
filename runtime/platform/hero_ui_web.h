/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_web.h: what a system's web view arm and `hero_ui.c` say
 * to each other about a widget of kind `HERO_UI_WEBVIEW` (`hero_ui.h`, THE
 * WEB VIEW). A header of the layer's own, as `hero_handle_tag.h` is, and no
 * group's: a program binds `hero_ui.h`.
 *
 * WHY THE ARM ASKS AND `hero_ui.c` ANSWERS. What a web view serves, where
 * its page may go and which message is heard are rules of the layer, not of
 * a system, so they are written once, in `hero_ui.c`, and every arm (the
 * Swift of `hero_ui_appkit.swift`, `hero_ui_web_win32.c`,
 * `hero_ui_web_gtk.c`) asks them through the functions of a
 * `hero_ui_web_host`, which `hero_ui.c` hands the arm. An arm calls them on
 * the UI thread alone, from inside a call `hero_ui.c` made into it or from
 * the system's loop that `hero_ui_wait` runs; no member links to
 * `hero_ui.c`'s names, so each is archived alone and taken only into a
 * program whose `hero_ui.c` calls it.
 *
 * NO CONDITIONAL BUT THE GUARD, as every header of the layer.
 */

#ifndef HERO_UI_WEB_H
#define HERO_UI_WEB_H

#include <stdint.h>

/* What `ask` is asked of a URL: whether the main frame may go to it,
 * whether a frame inside the page may, whether a message sent by the main
 * frame of that origin is heard, and whether it is the app's own origin,
 * which an arm asks when a page has loaded, to know whether the texts the
 * program posted may be handed to it. A message refused is counted lost by
 * `ask` itself; an arm asks `SENDER` with no URL for a message from a frame
 * inside the page, or one no string, so it is counted too. */
#define HERO_UI_WEB_ASK_FRAME 1
#define HERO_UI_WEB_ASK_SUBFRAME 2
#define HERO_UI_WEB_ASK_SENDER 3
#define HERO_UI_WEB_ASK_OURS 4

/* Where `serve` writes its answer: `numbers[HERO_UI_WEB_NUMBERS]` and
 * `texts[HERO_UI_WEB_TEXTS]`, each text a pointer to the layer's own bytes,
 * valid until the arm's call returns into the system and never written
 * through. `LENGTH` is the response's `Content-Length`; `BODY_COUNT` the
 * bytes to send, none for a `HEAD`. */
#define HERO_UI_WEB_STATUS 0
#define HERO_UI_WEB_LENGTH 1
#define HERO_UI_WEB_TYPE_COUNT 2
#define HERO_UI_WEB_BODY_COUNT 3
#define HERO_UI_WEB_POLICY_COUNT 4
#define HERO_UI_WEB_NUMBERS 5
#define HERO_UI_WEB_TYPE 0
#define HERO_UI_WEB_BODY 1
#define HERO_UI_WEB_POLICY 2
#define HERO_UI_WEB_TEXTS 3

/* The scheme the app's own origin is served on, and its host. */
#define HERO_UI_WEB_SCHEME "app"
#define HERO_UI_WEB_HOST "localhost"

/* Queues an event, `hero_ui_push`: a message is `HERO_UI_EVENT_MESSAGE`
 * naming the web view, its text the UTF-8 the page sent. */
typedef void (*hero_ui_web_push_fn)(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count);

/* Answers a request of the app's scheme to web view `id`: its method and its
 * whole URL, each as bytes and a count; writes the status, the headers' two
 * texts and the body through `numbers` and `texts`. Answers `HERO_UI_OK`. */
typedef int64_t (*hero_ui_web_serve_fn)(int64_t id, const uint8_t *method, int64_t method_count, const uint8_t *url, int64_t url_count, int64_t *numbers, const uint8_t **texts);

/* 1 where `url` is allowed for `what` (`HERO_UI_WEB_ASK_*`) in web view
 * `id`, 0 where it is not. */
typedef int64_t (*hero_ui_web_ask_fn)(int64_t what, int64_t id, const uint8_t *url, int64_t count);

/* Tells the layer the arm destroyed web view `id`, with its window or
 * alone, so its table is given back. */
typedef void (*hero_ui_web_gone_fn)(int64_t id);

/* The functions an arm asks, and the bridge's script, which the arm gives
 * every page's main frame before the page's own scripts run (`hero_ui.h`,
 * THE BRIDGE): `bridge_count` bytes of UTF-8 JavaScript, the layer's own,
 * living as long as the process. */
typedef struct hero_ui_web_host {
    hero_ui_web_push_fn push;
    hero_ui_web_serve_fn serve;
    hero_ui_web_ask_fn ask;
    hero_ui_web_gone_fn gone;
    const uint8_t *bridge;
    int64_t bridge_count;
} hero_ui_web_host;

#endif
