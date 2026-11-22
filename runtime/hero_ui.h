/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_ui.h — the platform layer's native user interface: windows and the
 * system's own widgets in them, made, set, placed, read back and destroyed,
 * and the queue of what the person did to them (M-rebel-library, 2026-11-22;
 * spec § 13). Rebel's `ui` stands on it.
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h` is:
 * its functions are defined in `platform/hero_ui.c`, a member of the archive
 * on every program's link line, which the linker takes only into a program
 * that calls one; a group binds them by naming this header,
 * `extern "hero_ui.h"`, with no `link` word and no flag.
 *
 * THE SHAPE, ONE FOR EVERY SYSTEM. A widget is the system's own: an
 * `NSButton` on macOS, a Win32 `BUTTON` on Windows, a `GtkButton` on Linux,
 * never one this layer draws. The layer knows widgets by the numbers the
 * program gives them (`id`, from 1, never given twice in one app), and it
 * keeps no tree and computes no geometry: the program says which window a
 * widget stands in, where it stands in that window's content (`place`, in
 * points from the content's top-left corner, the y axis down), and what it
 * shows (`set_*`); it asks back what the person changed (`text`, `value`,
 * `selection`) and how big the system draws a widget (`preferred`). Rows,
 * columns, grids and timers are the program's and never reach the layer.
 * What the person does is queued as events, each naming the widget's
 * number, and taken one at a time by `next` after `wait` has run the
 * system's own loop.
 *
 * THE SYSTEMS. On macOS the native side is AppKit, written in Swift
 * (`platform/hero_ui_appkit.swift`, which the layer compiles on an Apple
 * target alone). On Windows it is Win32's own controls, made per-monitor
 * aware of the scale so a point is a ninety-sixth of an inch at every
 * monitor's scale, drawn in the common controls' current look, and linking
 * `user32`, `gdi32`, `comctl32`, `comdlg32` and `msimg32` itself. On every
 * other system it is GTK 4, opened at the first call with `dlopen` as
 * `hero_keyring.c` opens libsecret, so no build needs GTK and a machine
 * without `libgtk-4.so.1` answers `HERO_UI_UNSUPPORTED`, naming it; the
 * least GTK it opens on is 4.0, and GTK 4.10's dialogs are used where the
 * library has them. `hero_ui_native_here` asks whether the native side can
 * open on this machine, where a person would see it: macOS answers 0 where
 * no window server serves the session (a login over ssh), Windows where
 * the process's window station shows nothing (a service's, or ssh's), and
 * every other system where no display is named (`DISPLAY`,
 * `WAYLAND_DISPLAY`) or GTK 4 is not installed. A program there runs on the
 * headless side.
 *
 * THE HEADLESS SIDE is an app opened with `HERO_UI_HEADLESS`: a handle with
 * the same life and the same thread rule, on which every widget call answers
 * `HERO_UI_UNSUPPORTED`, since the program keeps a headless app's widgets
 * itself and draws them as text (Rebel's `ui`).
 *
 * ONE THREAD. AppKit, Win32 and GTK each hold their windows on one thread,
 * and so does this layer: an app opens only on the process's main thread
 * (the thread `main` runs on, which on Windows, naming no main thread, the
 * layer notes before `main` begins), and every call naming it
 * from another thread answers -1 with `HERO_UI_WRONG_THREAD` before it
 * touches anything. A call is never a crash for its thread.
 *
 * ONE NATIVE APP AT A TIME: the system's application object is the
 * process's own, so a second native open while one is open answers
 * `HERO_UI_BUSY`. Headless apps are as many as the program opens.
 *
 * TEXT crosses as UTF-8 bytes and a count, never a zero byte inside (no
 * system's widget holds one), at most `HERO_UI_TEXT_MOST` bytes; a list of
 * texts (the items of a list, a combo box or a radio group; a file dialog's
 * filters; the paths it answers) is each text followed by one zero byte.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a call answering -1, or NULL for
 * `open`, and `hero_ui_last_error()` then names the kind and
 * `hero_ui_last_message` says which argument or which system refused. An app
 * is a handle the layer issues and checks: one closed already, `NULL`, or
 * one it never issued is `HERO_UI_NOT_OPEN`, never a read of freed memory; a
 * widget number the native side does not hold is `HERO_UI_NOT_FOUND`.
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): every
 * platform's arm of the member is compiled against these prototypes.
 */

#ifndef HERO_UI_H
#define HERO_UI_H

#include <stdint.h>

/* An app open on the layer: the system's application on the native side,
 * nothing but its life and its thread on the headless one. */
typedef struct hero_ui_app hero_ui_app;

/* The kinds of failure, the same numbers on every platform. */
#define HERO_UI_OK 0
/* An argument the layer refuses: a kind or a backend it does not know, a
 * widget number below 1, a count with no bytes behind it, a text holding a
 * zero byte, no UTF-8 or longer than `HERO_UI_TEXT_MOST`, a size below zero,
 * pixels whose count is not the width times the height times 4, a widget of
 * a kind the call does not set, or a parent of a kind that holds none. */
#define HERO_UI_INVALID 1
/* The app is closed, or was never one this layer issued. */
#define HERO_UI_NOT_OPEN 2
/* The call came from a thread other than the one the user interface lives
 * on (ONE THREAD, above). */
#define HERO_UI_WRONG_THREAD 3
/* This machine has no native user interface this layer reaches, or the
 * call asked the native side of a headless app. */
#define HERO_UI_UNSUPPORTED 4
/* A native app is open already in this process. */
#define HERO_UI_BUSY 5
/* The native side holds no widget of that number, or holds one already
 * where `create` was asked. */
#define HERO_UI_NOT_FOUND 6
/* The system has no memory, or no room, for the call. */
#define HERO_UI_LIMIT 7
/* The system refused for a reason none of the above names; the message
 * carries its words. */
#define HERO_UI_OTHER 8

/* The two sides an app opens on. */
#define HERO_UI_HEADLESS 1
#define HERO_UI_NATIVE 2

/* The kinds of widget, the same numbers in Rebel's `ui` and in every arm. A
 * window is a top-level window, its `parent` 0; a widget's parent is the
 * window it stands in; a menu's parent is the window whose menu bar holds
 * it (on macOS the application's menu bar shows the menus of the window in
 * front); a menu item's and a separator's parent is its menu. */
#define HERO_UI_WINDOW 1
#define HERO_UI_LABEL 2
#define HERO_UI_BUTTON 3
#define HERO_UI_TEXT_FIELD 4
#define HERO_UI_TEXT_AREA 5
#define HERO_UI_CHECKBOX 6
#define HERO_UI_RADIO_GROUP 7
#define HERO_UI_LIST 8
#define HERO_UI_LIST_MULTIPLE 9
#define HERO_UI_COMBO 10
#define HERO_UI_PROGRESS 11
#define HERO_UI_SLIDER 12
#define HERO_UI_IMAGE 13
#define HERO_UI_MENU 14
#define HERO_UI_MENU_ITEM 15
#define HERO_UI_MENU_SEPARATOR 16
/* The system's own web view, its parent a window (THE WEB VIEW, below). */
#define HERO_UI_WEBVIEW 17
/* The program's own kinds, numbered here so that one list names every kind;
 * `create` refuses them, `HERO_UI_INVALID`. */
#define HERO_UI_ROW 18
#define HERO_UI_COLUMN 19
#define HERO_UI_GRID 20
#define HERO_UI_TIMER 21

/* The kinds of event the queue holds. Each names the widget's number; what
 * changed is asked of the widget itself (`text`, `selection`), so an event
 * carries no copy of it, but for the fields below. */
/* A button was pressed. */
#define HERO_UI_EVENT_CLICK 1
/* The person changed the text of a text field or a text area. */
#define HERO_UI_EVENT_TEXT 2
/* The person changed what a list, a combo box or a radio group has chosen. */
#define HERO_UI_EVENT_SELECT 3
/* The person checked a checkbox, `value` 1, or unchecked it, `value` 0. */
#define HERO_UI_EVENT_TOGGLE 4
/* The person moved a slider to `value`. */
#define HERO_UI_EVENT_VALUE 5
/* The person chose a menu item, by the mouse or by its shortcut. */
#define HERO_UI_EVENT_MENU 6
/* The person asked a window to close (its close button, the system's quit);
 * nothing is closed. */
#define HERO_UI_EVENT_CLOSE 7
/* A window's content took a new size, `value` wide and `second` high. */
#define HERO_UI_EVENT_RESIZE 8
/* A web view's page sent a text by its bridge, read by `hero_ui_event_text`
 * (THE WEB VIEW, below). */
#define HERO_UI_EVENT_MESSAGE 9

/* The keys held with a menu item's shortcut, added together. `PRIMARY` is
 * Command on macOS and Control elsewhere, the key every system's own
 * shortcuts are made with; `ALT` is Option on macOS. */
#define HERO_UI_PRIMARY 1
#define HERO_UI_SHIFT 2
#define HERO_UI_ALT 4

/* A message box's tone and buttons, and which button the person pressed. */
#define HERO_UI_TONE_INFO 1
#define HERO_UI_TONE_WARNING 2
#define HERO_UI_TONE_ERROR 3
#define HERO_UI_BUTTONS_OK 1
#define HERO_UI_BUTTONS_OK_CANCEL 2
#define HERO_UI_BUTTONS_YES_NO 3
#define HERO_UI_BUTTONS_YES_NO_CANCEL 4
#define HERO_UI_ANSWER_OK 1
#define HERO_UI_ANSWER_CANCEL 2
#define HERO_UI_ANSWER_YES 3
#define HERO_UI_ANSWER_NO 4

/* The most bytes one text holds, 16 MiB. */
#define HERO_UI_TEXT_MOST 16777216
/* The most events the queue holds. Past it a new event is dropped and the
 * next `next` reports none lost: the person's later acts are what a program
 * that took 4096 events late most needs, so the queue keeps its oldest and
 * `hero_ui_lost` counts the dropped. A text, a selection, a value or a size
 * changed twice for one widget with nothing between is queued once. */
#define HERO_UI_QUEUE_MOST 4096

/* 1 where the calling thread is the one an app may open on (ONE THREAD,
 * above), 0 where it is not. */
int64_t hero_ui_on_ui_thread(void);

/* 1 where the native side can open on this machine, 0 where it cannot
 * (THE SYSTEMS, above). Asks nothing of the system's application object, so
 * it is safe to call from any thread. */
int64_t hero_ui_native_here(void);

/* Opens an app on `HERO_UI_HEADLESS` or `HERO_UI_NATIVE`, or NULL with
 * `INVALID`, `WRONG_THREAD`, `UNSUPPORTED`, `BUSY`, `LIMIT` or `OTHER`. The
 * native side makes the system's application object the first time and
 * keeps it, as every system wants, for the process's life. */
hero_ui_app *hero_ui_open(int64_t backend);

/* Closes `app`: every native widget it still holds is destroyed, its queue
 * emptied, and the handle given back. 0, or -1 with `NOT_OPEN` or
 * `WRONG_THREAD`, the handle then left as it was. */
int64_t hero_ui_close(hero_ui_app *app);

/* The app's number, from 1, never given twice in one process, so a widget
 * number of one app is told from another's; -1 with `NOT_OPEN` or
 * `WRONG_THREAD`. */
int64_t hero_ui_number(hero_ui_app *app);

/* Makes the native widget `id` of `kind` in `parent` (the kinds above say
 * which parent each takes), hidden, empty and at no place until set and
 * placed; a window is made hidden and shown by `set_visible`. 0, or -1:
 * `INVALID`, `NOT_FOUND` (no parent of that number, or `id` taken),
 * `UNSUPPORTED`, `LIMIT`, `OTHER`. */
int64_t hero_ui_create(hero_ui_app *app, int64_t id, int64_t kind, int64_t parent);

/* Destroys the native widget `id`, and with a window or a menu every widget
 * whose parent it is. 0, or -1 `NOT_FOUND`. */
int64_t hero_ui_destroy(hero_ui_app *app, int64_t id);

/* Sets what `id` shows as its text: a window's title, a label's, a
 * button's, a checkbox's, a menu's or a menu item's text, or the whole text
 * of a text field or a text area, the caret put at its end. No event is
 * queued for it. 0, or -1. */
int64_t hero_ui_set_text(hero_ui_app *app, int64_t id, const void *text, uint64_t count);

/* Sets the items of a list, a combo box or a radio group, each followed by
 * one zero byte, `count` bytes in all; nothing is chosen after it. 0, or
 * -1. */
int64_t hero_ui_set_items(hero_ui_app *app, int64_t id, const void *items, uint64_t count);

/* Sets a checkbox's state (0 or 1), a slider's or a progress bar's value,
 * or the one item a combo box or a radio group has chosen (-1 for none). No
 * event is queued. 0, or -1 `INVALID` where the value is outside the range. */
int64_t hero_ui_set_value(hero_ui_app *app, int64_t id, int64_t value);

/* Sets the values a slider or a progress bar runs between, `least` below
 * `most`. 0, or -1. */
int64_t hero_ui_set_range(hero_ui_app *app, int64_t id, int64_t least, int64_t most);

/* Sets the items a list has chosen, `count` indices in ascending order, at
 * most one for a list of one choice. No event is queued. 0, or -1. */
int64_t hero_ui_set_selection(hero_ui_app *app, int64_t id, const int64_t *indices, uint64_t count);

/* Lets the person use `id`, 1, or greys it out, 0. 0, or -1. */
int64_t hero_ui_set_enabled(hero_ui_app *app, int64_t id, int64_t on);

/* Shows `id`, 1, or hides it, 0; a window shown is brought to the front. 0,
 * or -1. */
int64_t hero_ui_set_visible(hero_ui_app *app, int64_t id, int64_t on);

/* Gives a menu item the shortcut `key` with the keys `modifiers` adds up:
 * one letter `a` to `z`, one digit, or one of `F1` to `F12`, `Enter`,
 * `Escape`, `Tab`, `Space`, `Delete`, `Backspace`, `Up`, `Down`, `Left`,
 * `Right`, `Home`, `End`, `PageUp`, `PageDown`; an empty key takes the
 * shortcut away. 0, or -1. */
int64_t hero_ui_set_shortcut(hero_ui_app *app, int64_t id, const void *key, uint64_t count, int64_t modifiers);

/* Sets an image widget's pixels: `width` by `height`, 8-bit RGBA, alpha
 * straight, row after row from the top, `count` bytes, which is
 * `width * height * 4`. 0, or -1. */
int64_t hero_ui_set_image(hero_ui_app *app, int64_t id, const void *rgba, uint64_t count, int64_t width, int64_t height);

/* Lets the person resize window `id`, 1, or keeps its size, 0. 0, or -1. */
int64_t hero_ui_set_resizable(hero_ui_app *app, int64_t id, int64_t on);

/* Sets the size of window `id`'s content, in points; the frame around it is
 * the system's. No event is queued. 0, or -1. */
int64_t hero_ui_set_size(hero_ui_app *app, int64_t id, int64_t width, int64_t height);

/* Sets the least size the person may give window `id`'s content. 0, or -1. */
int64_t hero_ui_set_min_size(hero_ui_app *app, int64_t id, int64_t width, int64_t height);

/* Places widget `id` at `x`, `y` in its window's content, `width` by
 * `height` points. 0, or -1. */
int64_t hero_ui_place(hero_ui_app *app, int64_t id, int64_t x, int64_t y, int64_t width, int64_t height);

/* Places the item `index` of radio group `id` at `x`, `y` in its window's
 * content, `width` by `height` points. 0, or -1 `INVALID` past the items. */
int64_t hero_ui_place_item(hero_ui_app *app, int64_t id, int64_t index, int64_t x, int64_t y, int64_t width, int64_t height);

/* Writes the size the system draws widget `id` at, in points, through
 * `width` and `height`, as it stands with its text and items: the least
 * size its content needs, where the system's widget gives one, else the
 * system's usual size for its kind. A radio group's is one item's width,
 * the widest, and the sum of the items' heights. 0, or -1. */
int64_t hero_ui_preferred(hero_ui_app *app, int64_t id, int64_t *width, int64_t *height);

/* Writes the text a text field or a text area holds now into `buffer` as
 * UTF-8 and answers its length; where it is longer than `count`, nothing is
 * written and the length is answered all the same. -1 on failure. */
int64_t hero_ui_text(hero_ui_app *app, int64_t id, void *buffer, uint64_t count);

/* Writes a checkbox's state, a slider's value, or the item a combo box or a
 * radio group has chosen (-1 for none) through `value`. 0, or -1. */
int64_t hero_ui_value(hero_ui_app *app, int64_t id, int64_t *value);

/* Writes the indices a list or a combo box or a radio group has chosen now,
 * ascending, into `indices` and answers how many; where they are more than
 * `count`, nothing is written and the number is answered all the same. -1
 * on failure. */
int64_t hero_ui_selection(hero_ui_app *app, int64_t id, int64_t *indices, uint64_t count);

/* Runs the system's loop until an event is queued or `ms` milliseconds have
 * passed, 0 running it only over what is ready. Answers 1 where the queue
 * holds an event, 0 where none came, -1 on failure. A headless app holds
 * no native side, so it only answers whether its queue holds one. */
int64_t hero_ui_wait(hero_ui_app *app, int64_t ms);

/* Takes the queue's oldest event: writes its kind, its widget's number and
 * its two values through the four places and answers 1, or answers 0 where
 * the queue is empty. -1 on failure. */
int64_t hero_ui_next(hero_ui_app *app, int64_t *kind, int64_t *id, int64_t *value, int64_t *second);

/* Writes the text the event `next` took last carries into `buffer` and
 * answers its length, 0 for an event that carries none; where it is longer
 * than `count`, nothing is written and the length is answered all the same.
 * -1 on failure. */
int64_t hero_ui_event_text(hero_ui_app *app, void *buffer, uint64_t count);

/* How many events the queue dropped since the app opened (QUEUE_MOST,
 * above); -1 on failure. */
int64_t hero_ui_lost(hero_ui_app *app);

/* Does what a person does, through the system's own path, so that the
 * event comes as the person's would: `HERO_UI_EVENT_CLICK` presses button
 * `id`; `TEXT` replaces a text field's or a text area's text by `text` as
 * an edit; `SELECT` chooses the item `value` of a list, a combo box or a
 * radio group (-1 for none); `TOGGLE` clicks a checkbox; `VALUE` moves a
 * slider to `value`; `MENU` chooses menu item `id`; `CLOSE` presses window
 * `id`'s close button; `RESIZE` gives window `id`'s content the size
 * `value` by `second`; `MESSAGE` has the page web view `id` shows send
 * `text` by its bridge, as the page's own script would, at once or once a
 * page of the app's own origin has loaded, so the message is checked and
 * queued as the page's is (THE WEB VIEW, below; `LIMIT` where
 * `HERO_UI_QUEUE_MOST` wait for the page already). Its event is
 * queued as the person's would be. 0, or -1, `UNSUPPORTED` where the system
 * offers no such path. */
int64_t hero_ui_inject(hero_ui_app *app, int64_t kind, int64_t id, int64_t value, int64_t second, const void *text, uint64_t count);

/* Shows a message box over window `window` (0 for none), with `title` and
 * `text`, a tone and buttons, and waits for the person: answers the button
 * pressed, `HERO_UI_ANSWER_*`, or -1. */
int64_t hero_ui_message(hero_ui_app *app, int64_t window, int64_t tone, int64_t buttons, const void *title, uint64_t title_count, const void *text, uint64_t text_count);

/* Shows the system's dialog to open files (`save` 0; `multiple` 1 lets the
 * person choose several) or to save one (`save` 1, `name` the name it
 * proposes), over window `window` (0 for none), and waits for the person.
 * `filters` is each filter's name and then its extensions joined by `;`,
 * each followed by one zero byte, `filters_count` bytes in all, none for
 * every file. Answers the bytes the chosen paths take, each followed by one
 * zero byte, which `hero_ui_dialog_paths` then writes; 0 where the person
 * cancelled; -1 on failure. */
int64_t hero_ui_file_dialog(hero_ui_app *app, int64_t window, int64_t save, int64_t multiple, const void *title, uint64_t title_count, const void *name, uint64_t name_count, const void *filters, uint64_t filters_count);

/* Writes the paths the last file dialog answered into `buffer`, as it
 * counted them, and answers their bytes; where they are more than `count`,
 * nothing is written and the bytes are answered all the same. -1 on
 * failure. */
int64_t hero_ui_dialog_paths(hero_ui_app *app, void *buffer, uint64_t count);

/* -- THE WEB VIEW -------------------------------------------------------------
 *
 * A widget of kind `HERO_UI_WEBVIEW` is the system's own web engine shown in
 * a window: WebKit's `WKWebView` on macOS, Microsoft Edge's WebView2 on
 * Windows (its runtime found at run time through `WebView2Loader.dll`),
 * WebKitGTK on Linux (opened with `dlopen`), never an engine the program
 * carries. Where the system has none, `create` answers `UNSUPPORTED` and the
 * message names what is missing. Its page is the interface and the program
 * is its logic, the two speaking through a bridge of texts and through
 * nothing else.
 *
 * EVERY PAGE COMES FROM THE PROGRAM, NEVER FROM A PORT. A web view serves
 * its page, and every file the page loads, from its ASSET TABLE, which the
 * program fills with `hero_ui_web_asset`, at the app's own origin
 * `app://localhost`, through the system's own hook for a scheme (WebKit's
 * `WKURLSchemeHandler`, WebView2's resource requests on a custom scheme it
 * registers, WebKitGTK's URI scheme): no socket is opened, no loopback
 * server stands anywhere, and a path the table does not hold is never looked
 * for elsewhere. A request is answered:
 *   - `GET` or `HEAD` of a path the table holds: 200, with the asset's
 *     `Content-Type`, its `Content-Length`, the web view's
 *     `Content-Security-Policy` (`hero_ui_web_policy`, else
 *     `HERO_UI_WEB_POLICY_DEFAULT`), `X-Content-Type-Options: nosniff`,
 *     `Cache-Control: no-store` and `Referrer-Policy: no-referrer`; a path
 *     ending in `/` reads that folder's `index.html`; a query is ignored;
 *   - a host other than `localhost`, a path the table does not hold, or one
 *     no asset could have: 404;
 *   - any other method: 405, with `Allow: GET, HEAD`.
 * An asset's PATH is `/` and segments joined by `/`, each one or more of
 * `A`-`Z`, `a`-`z`, `0`-`9`, `-`, `.`, `_`, `~`, never `.` or `..`, at most
 * `HERO_UI_WEB_PATH_MOST` bytes; a request's path is read after its `%XX`
 * escapes are undone. Its CONTENT TYPE is `type/subtype`, then any
 * parameters, printable ASCII with no control character, at most
 * `HERO_UI_WEB_TYPE_MOST` bytes. Its BODY is any bytes, at most
 * `HERO_UI_WEB_ASSET_MOST`; one web view's table holds at most
 * `HERO_UI_WEB_ASSETS_MOST` assets and `HERO_UI_WEB_BYTES_MOST` bytes of
 * bodies. The layer keeps its own copy of each.
 *
 * WHERE A PAGE MAY GO. Every frame may show the app's own origin,
 * `about:blank` and `about:srcdoc`, and an origin the program allowed
 * (`hero_ui_web_allow`); every other navigation is cancelled before it
 * starts: `file:`, `data:`, another scheme, another host. A new window (the
 * page's `window.open`, a link with a target) is refused, a download is
 * cancelled, a request for the camera, the microphone or the location is
 * denied, a file chooser opens nothing, and the page's cookies and storage
 * are kept in memory alone and forgotten with the web view, so no page
 * reaches the person's files, devices or another program's data. The
 * program's own state is the program's to keep.
 *
 * THE BRIDGE. Before any script of a page runs, the layer gives its main
 * frame `window.heroes`, an object that cannot be replaced or changed:
 *   - `heroes.send(text)` sends `text` to the program. It must be a string
 *     holding no U+0000 whose UTF-8 is at most `HERO_UI_WEB_MESSAGE_MOST`
 *     bytes, else it throws a `TypeError` or a `RangeError` and nothing
 *     crosses. The text crosses as UTF-8 (a lone surrogate becomes U+FFFD,
 *     the system's own conversion) and is queued as `HERO_UI_EVENT_MESSAGE`
 *     naming the web view, its text read by `hero_ui_event_text`. Only the
 *     main frame showing the app's own origin is heard: a message from
 *     another frame or another origin, or one no string, is dropped and
 *     counted by `hero_ui_lost`.
 *   - `heroes.receive(fn)` names the one function each text the program
 *     posts (`hero_ui_web_post`) is handed to, in the order posted; texts
 *     posted before it names one wait in the page, at most
 *     `HERO_UI_QUEUE_MOST`, the later dropped.
 *   - `heroes.invoke(name, args)` answers a promise, and `heroes.on(name,
 *     fn)` hears the program's events, over the same two directions in
 *     JSON: a call `{"heroes":1,"id":N,"call":NAME,"args":ARGS}` is sent,
 *     and a text the program posts whose JSON is `{"heroes":1,"id":N,"ok":V}`
 *     resolves call N's promise with V, `{"heroes":1,"id":N,"error":{"code":
 *     C,"msg":M}}` rejects it with an `Error` of message M and `code` C, and
 *     `{"heroes":1,"event":NAME,"data":V}` calls every function `on` gave
 *     for NAME with V; any other text goes to `receive`'s function.
 * A posted text reaches only a page of the app's own origin, after it has
 * loaded: one posted while a page loads waits for it (at most
 * `HERO_UI_QUEUE_MOST`, else `LIMIT`), and one posted while the web view
 * shows another origin is dropped. No text crosses as code: the layer
 * offers no way to run a script the program writes, so no value of the
 * program's can be read by the page as a script.
 */

/* The app's own origin, which every web view serves its assets at. */
#define HERO_UI_WEB_ORIGIN "app://localhost"
/* The most bytes an asset's path, or a path `navigate` shows, takes. */
#define HERO_UI_WEB_PATH_MOST 1024
/* The most bytes a content type takes. */
#define HERO_UI_WEB_TYPE_MOST 255
/* The most bytes one asset's body holds, 64 MiB. */
#define HERO_UI_WEB_ASSET_MOST 67108864
/* The most assets one web view's table holds. */
#define HERO_UI_WEB_ASSETS_MOST 4096
/* The most bytes the bodies of one web view's table hold in all, 256 MiB. */
#define HERO_UI_WEB_BYTES_MOST 268435456
/* The most bytes a content security policy takes. */
#define HERO_UI_WEB_POLICY_MOST 4096
/* The most origins one web view lets its page go to, and the most bytes one
 * takes. */
#define HERO_UI_WEB_ORIGINS_MOST 64
#define HERO_UI_WEB_ORIGIN_MOST 255
/* The most bytes one message takes, either way: `HERO_UI_TEXT_MOST`. */
#define HERO_UI_WEB_MESSAGE_MOST 16777216
/* The policy every response carries where the program set none: everything
 * from the app's own origin and nothing else, an image also from a `data:`
 * URL, no plug-in, no `<base>`, no form sent anywhere, and the page framed
 * by nobody. */
#define HERO_UI_WEB_POLICY_DEFAULT "default-src 'self'; img-src 'self' data:; object-src 'none'; base-uri 'none'; form-action 'none'; frame-ancestors 'none'"

/* Puts into web view `id`'s table the asset `path` of `path_count` bytes,
 * its content type `type` of `type_count` bytes and its body `body` of
 * `body_count` bytes, replacing the one at that path; the page sees it at
 * its next request. 0, or -1: `INVALID` (a path, a type or a size the table
 * refuses, or a widget no web view), `NOT_FOUND`, `LIMIT` (past the table's
 * most, or no memory). */
int64_t hero_ui_web_asset(hero_ui_app *app, int64_t id, const void *path, uint64_t path_count, const void *type, uint64_t type_count, const void *body, uint64_t body_count);

/* Takes the asset `path` out of web view `id`'s table. 0, or -1:
 * `INVALID`, `NOT_FOUND` (no such web view, or no such asset). */
int64_t hero_ui_web_remove(hero_ui_app *app, int64_t id, const void *path, uint64_t path_count);

/* Sets the content security policy every response of web view `id` carries
 * from its next request, `count` bytes of printable ASCII; none puts
 * `HERO_UI_WEB_POLICY_DEFAULT` back. 0, or -1: `INVALID`, `NOT_FOUND`. */
int64_t hero_ui_web_policy(hero_ui_app *app, int64_t id, const void *policy, uint64_t count);

/* Lets web view `id`'s page go to the origin `origin`, `count` bytes:
 * `https://` or `http://`, a host of lower-case letters, digits, `-` and
 * `.`, and a port from 1 to 65535 where it is not the scheme's own; no path.
 * A page of that origin is shown and never heard: the bridge serves the
 * app's own origin alone. 0, or -1: `INVALID`, `NOT_FOUND`, `LIMIT` (past
 * `HERO_UI_WEB_ORIGINS_MOST`). */
int64_t hero_ui_web_allow(hero_ui_app *app, int64_t id, const void *origin, uint64_t count);

/* Has web view `id` show `path` of the app's own origin, `count` bytes: an
 * asset's path, or a folder's ending in `/`, then perhaps a query `?...`
 * and a fragment `#...` of `A`-`Z`, `a`-`z`, `0`-`9` and
 * `-._~!$&'()*+,;=:@/?%`. The page loads at the next `wait`; texts posted
 * from now wait for it. 0, or -1: `INVALID`, `NOT_FOUND`. */
int64_t hero_ui_web_navigate(hero_ui_app *app, int64_t id, const void *path, uint64_t count);

/* Posts the text `text`, `count` bytes of UTF-8 holding no zero byte, to
 * web view `id`'s page (THE BRIDGE, above). 0, or -1: `INVALID`,
 * `NOT_FOUND`, `LIMIT` (`HERO_UI_QUEUE_MOST` texts wait already). */
int64_t hero_ui_web_post(hero_ui_app *app, int64_t id, const void *text, uint64_t count);

/* The kind of the last call's failure on this thread, `HERO_UI_OK` after a
 * call that did not fail. */
int64_t hero_ui_last_error(void);

/* The words of the last call's failure on this thread, written into
 * `buffer` as UTF-8 with no zero byte, cut where a character ends to fit
 * `count`; answers the bytes written, 0 after a call that did not fail. -1
 * with `INVALID` where `buffer` is NULL or `count` is 0. */
int64_t hero_ui_last_message(void *buffer, uint64_t count);

#endif
