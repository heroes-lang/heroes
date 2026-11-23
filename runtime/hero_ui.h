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
 * front), or a menu, inside which it stands as a submenu after the items
 * made before it; a menu item's and a separator's parent is its menu. */
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
 * which parent each takes, a container standing for a window where a
 * window is named: BOXES THE SYSTEM DRAWS, below), hidden, empty and at
 * no place until set and placed; a window is made hidden and shown by
 * `set_visible`. 0, or -1:
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

/* Places widget `id` at `x`, `y` in its window's content, or its
 * container's, `width` by `height` points. 0, or -1. */
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
 * radio group (-1 for none), the row of a table, or the page of tabs;
 * `TOGGLE` clicks a checkbox; `VALUE` moves a slider to `value`, or a
 * split's divider; `MENU` chooses menu item `id`; `CLOSE` presses window
 * `id`'s close button; `RESIZE` gives window `id`'s content the size
 * `value` by `second`; `MESSAGE` has the page web view `id` shows send
 * `text` by its bridge, as the page's own script would, at once or once a
 * page of the app's own origin has loaded, so the message is checked and
 * queued as the page's is (THE WEB VIEW, below; `LIMIT` where
 * `HERO_UI_QUEUE_MOST` wait for the page already); `ACTIVATE` opens row
 * `value` of list or table `id`, choosing it first where it is not chosen,
 * as a double click does; `SUBMIT` presses Return in text field `id`;
 * `SORT` clicks the header of column `value` of table `id` until the table
 * stands sorted by it in the order `second` names; `UNFOLD` unfolds item
 * `value` of tree `id`, `second` 1, or folds it, 0; `SELECT` and
 * `ACTIVATE` take a tree's item by its number; `DIALOG` answers dialog
 * `id`, shown and not yet answered, as its person would: a message box's
 * button `value` (`HERO_UI_ANSWER_*`, or a box of the program's words'
 * index), or a file or folder dialog's choice of `value` paths, `text`
 * holding them each followed by one zero byte, 0 cancelling it. Its event
 * is queued as the person's would be. 0, or -1, `UNSUPPORTED` where the
 * system offers no such path. */
int64_t hero_ui_inject(hero_ui_app *app, int64_t kind, int64_t id, int64_t value, int64_t second, const void *text, uint64_t count);

/* Shows a message box over window `window`, with `title` and `text`, a tone
 * and buttons, and answers at once, 0, or -1: the person's button comes
 * later as `HERO_UI_EVENT_DIALOG` naming `dialog`, its `value` the button,
 * `HERO_UI_ANSWER_*` (DIALOGS ANSWERED THROUGH THE QUEUE, below). */
int64_t hero_ui_message(hero_ui_app *app, int64_t dialog, int64_t window, int64_t tone, int64_t buttons, const void *title, uint64_t title_count, const void *text, uint64_t text_count);

/* Shows the system's dialog to open files (`save` 0; `multiple` 1 lets the
 * person choose several) or to save one (`save` 1, `name` the name it
 * proposes), over window `window`, and answers at once, 0, or -1.
 * `filters` is each filter's name and then its extensions joined by `;`,
 * each followed by one zero byte, `filters_count` bytes in all, none for
 * every file. The person's choice comes later as `HERO_UI_EVENT_DIALOG`
 * naming `dialog`, its `value` how many paths they chose, 0 where they
 * cancelled, its text the paths, each followed by one zero byte. */
int64_t hero_ui_file_dialog(hero_ui_app *app, int64_t dialog, int64_t window, int64_t save, int64_t multiple, const void *title, uint64_t title_count, const void *name, uint64_t name_count, const void *filters, uint64_t filters_count);

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

/* -- KEYS PRESSED BY THE PROGRAM ----------------------------------------------
 *
 * A key pressed as the person's keyboard presses it, through the system's
 * own path, as wxWidgets' `wxUIActionSimulator` presses one: a menu item's
 * shortcut, the Edit menu's commands and the widget with the keys take it as
 * they take the person's. What a test of a native app drives the keys with;
 * a headless app holds no system to press them in. */

/* Presses the key `key`, `count` bytes naming a key as `hero_ui_set_shortcut`
 * names one (not empty), with the keys `modifiers` adds up held, in window
 * `id`, or with widget `id` holding the keys, given them first as the
 * person's click would. The system hands a key to the key window of the
 * active application alone, so the window is first made that (on macOS the
 * app is activated and the window brought to the front); the key goes down
 * and up. Its events are queued as the person's would be. 0, or -1:
 * `INVALID` (a key no shortcut names, held keys past the three, or a widget
 * that takes no keys), `NOT_FOUND`, `OTHER` where the window is hidden or
 * the system keeps another application in front, `UNSUPPORTED`. */
int64_t hero_ui_press_key(hero_ui_app *app, int64_t id, const void *key, uint64_t count, int64_t modifiers);

/* -- WINDOWS ON THE SCREEN ----------------------------------------------------
 *
 * Where a window stands among the screens, the state it stands in, how the
 * system draws it, the app's icon, and the window the person must answer
 * before any other, as wxWidgets' `wxTopLevelWindow`, `wxDisplay` and
 * `wxDialog::ShowModal` offer them.
 *
 * THE SCREENS are numbered from 0, the system's main one first (on macOS
 * the screen holding the menu bar, on Windows the primary monitor), then in
 * the order the system lists them. Each is given by `HERO_UI_SCREEN_FIELDS`
 * numbers: its whole area's left, top, width and height, then its work
 * area's (the whole less the menu bar, the Dock or the task bar), in the
 * desktop's points, whose origin is screen 0's top-left corner and whose y
 * axis goes down; and its scale in hundredths, the pixels a point takes on
 * each axis times 100 (100 on an ordinary screen, 200 on a Retina one).
 *
 * A WINDOW'S POSITION is its frame's top-left corner, its title bar and its
 * border included, in points from the top-left corner of the screen it is
 * given on, or that it stands on most when it is read; the system may keep
 * part of a frame on a screen (macOS keeps a title bar below the menu bar).
 *
 * WHAT THE PERSON DOES to a window among the screens is queued as the events
 * below, each naming the window; what the program sets is not, but for the
 * new size a change of state gives the content, queued as
 * `HERO_UI_EVENT_RESIZE` as any resize is. Each of these calls answers
 * `UNSUPPORTED` on a headless app, and on a system whose arm does not offer
 * it. */

/* How many numbers give one screen, and the place of each. */
#define HERO_UI_SCREEN_FIELDS 9
#define HERO_UI_SCREEN_LEFT 0
#define HERO_UI_SCREEN_TOP 1
#define HERO_UI_SCREEN_WIDTH 2
#define HERO_UI_SCREEN_HEIGHT 3
#define HERO_UI_SCREEN_WORK_LEFT 4
#define HERO_UI_SCREEN_WORK_TOP 5
#define HERO_UI_SCREEN_WORK_WIDTH 6
#define HERO_UI_SCREEN_WORK_HEIGHT 7
#define HERO_UI_SCREEN_SCALE 8

/* The states a window stands in: shown at the size the program or the
 * person gave it, minimized into the Dock or the task bar, maximized over
 * its screen's work area (zoomed, on macOS), or filling its screen with no
 * frame (on macOS in a space of its own). */
#define HERO_UI_STATE_NORMAL 1
#define HERO_UI_STATE_MINIMIZED 2
#define HERO_UI_STATE_MAXIMIZED 3
#define HERO_UI_STATE_FULL_SCREEN 4

/* What stands behind a window's widgets: the system's plain background, or
 * its translucent material, which blurs what stands behind the window
 * (`NSVisualEffectView` on macOS, Mica on Windows 11). */
#define HERO_UI_BACKDROP_NONE 0
#define HERO_UI_BACKDROP_TRANSLUCENT 1

/* The person moved window `id`: `value` and `second` its new position on
 * the screen it stands on, which `hero_ui_frame_of` names. */
#define HERO_UI_EVENT_MOVE 10
/* Window `id` stands in a new state, `value`, by the person's act (its
 * buttons, the Dock, a key of the system's). */
#define HERO_UI_EVENT_STATE 11
/* Window `id` stands on another screen, `value`, moved there or left there
 * by a screen that changed. */
#define HERO_UI_EVENT_SCREEN 12

/* Writes the screens through `values`, `HERO_UI_SCREEN_FIELDS` numbers
 * each, and answers how many there are; where they take more than `count`
 * numbers, nothing is written and the number of screens is answered all the
 * same. -1 on failure. */
int64_t hero_ui_screens(hero_ui_app *app, int64_t *values, uint64_t count);

/* Writes window `id`'s frame through the five places: the screen it stands
 * on most, its position on that screen, and its frame's width and height in
 * points, its title bar and border included. 0, or -1: `INVALID` for a
 * widget no window, `NOT_FOUND`. */
int64_t hero_ui_frame_of(hero_ui_app *app, int64_t id, int64_t *screen, int64_t *x, int64_t *y, int64_t *width, int64_t *height);

/* Moves window `id` to position `x`, `y` on screen `screen`. No event is
 * queued. 0, or -1: `INVALID` (no such screen, a position past
 * `HERO_UI_POSITION_MOST` either way, a widget no window), `NOT_FOUND`. */
int64_t hero_ui_set_position(hero_ui_app *app, int64_t id, int64_t screen, int64_t x, int64_t y);

/* The farthest a position goes from its screen's corner either way, in
 * points: a million, past every desktop. */
#define HERO_UI_POSITION_MOST 1000000

/* Centres window `id`'s frame in the work area of the screen it stands on,
 * `over` 0, or over window `over`'s frame, keeping it within that screen's
 * work area as far as its size allows. No event is queued. 0, or -1. */
int64_t hero_ui_center(hero_ui_app *app, int64_t id, int64_t over);

/* Brings window `id` to `state`, leaving the one it stands in first, and
 * waits for the system to finish it (each step is animated, and a full
 * screen moves the window to a space of its own on macOS): 0, or -1:
 * `INVALID` (a state these four do not name, a widget no window), `NOT_FOUND`,
 * `OTHER` where the system did not bring it there within five seconds, or
 * would not (a window with no frame is never minimized on macOS). No state
 * event is queued; the size its content takes is, as `HERO_UI_EVENT_RESIZE`. */
int64_t hero_ui_set_state(hero_ui_app *app, int64_t id, int64_t state);

/* Writes the state window `id` stands in through `state`. 0, or -1. */
int64_t hero_ui_state(hero_ui_app *app, int64_t id, int64_t *state);

/* Keeps window `id` above every other application's windows, 1, or at the
 * level of an ordinary window, 0. 0, or -1. */
int64_t hero_ui_set_on_top(hero_ui_app *app, int64_t id, int64_t on);

/* Gives window `id` its frame, a title bar with its buttons and a border,
 * 1, as a window is made, or takes them away, 0, its content keeping its
 * size; a window with no frame is moved by a drag on its background. 0, or
 * -1: `OTHER` for a window in full screen. */
int64_t hero_ui_set_framed(hero_ui_app *app, int64_t id, int64_t on);

/* Sets window `id`'s opacity, `alpha` from 0, wholly see-through, to 255,
 * wholly opaque, as a window is made. 0, or -1. */
int64_t hero_ui_set_opacity(hero_ui_app *app, int64_t id, int64_t alpha);

/* Puts `backdrop`, a `HERO_UI_BACKDROP_*`, behind window `id`'s widgets.
 * 0, or -1: `UNSUPPORTED` where the system offers no translucent material. */
int64_t hero_ui_set_backdrop(hero_ui_app *app, int64_t id, int64_t backdrop);

/* Gives the app the icon of `width` by `height` pixels of 8-bit RGBA,
 * alpha straight, row after row from the top, `count` bytes: the Dock's on
 * macOS, every window's and the task bar's on Windows; a size of 0 by 0
 * gives the system's own back. 0, or -1 `INVALID` where `count` is not
 * `width * height * 4`. */
int64_t hero_ui_set_icon(hero_ui_app *app, const void *rgba, uint64_t count, int64_t width, int64_t height);

/* Makes window `id` modal, 1: until the program ends it, 0, or destroys the
 * window, it takes the person's every act and the app's other windows take
 * none, as `wxDialog::ShowModal` holds them; the program goes on asking
 * `wait` as before, and the window is brought to the front. One window at a
 * time. 0, or -1: `OTHER` where another window is modal already. */
int64_t hero_ui_set_modal(hero_ui_app *app, int64_t id, int64_t on);

/* Does what the person does to window `id` among the screens, through the
 * system's own path, so its event comes as theirs would:
 * `HERO_UI_EVENT_MOVE` drags it to position `second`, `third` on screen
 * `value`; `HERO_UI_EVENT_STATE` brings it to state `value` as its buttons
 * would. 0, or -1, as `set_position` and `set_state` answer. */
int64_t hero_ui_inject_window(hero_ui_app *app, int64_t kind, int64_t id, int64_t value, int64_t second, int64_t third);

/* -- A LIST'S ROW OPENED -------------------------------------------------- */

/* The person opened a row of list `id`, by a double click on it or by the
 * Return key while the list had the keys: `value` the row, for the Return
 * key the first row chosen. A first click's choice comes before it as
 * `HERO_UI_EVENT_SELECT`, as the person's click gave it; `inject` gives it
 * with `HERO_UI_EVENT_ACTIVATE`. A file manager enters the folder, a mail
 * client opens the message. */
#define HERO_UI_EVENT_ACTIVATE 13

/* -- A MENU ITEM'S ROLE ----------------------------------------------------
 *
 * An item of the program's own that stands for something every program of
 * the system has, as wxWidgets' `wxID_ABOUT`, `wxID_PREFERENCES` and
 * `wxID_EXIT` and Qt's `QAction::MenuRole` mark it. On macOS such an item
 * stands in the application's menu, in the place Apple's guidelines give
 * it, and is hidden in the menu the program put it in; its choosing is
 * still `HERO_UI_EVENT_MENU` naming it. Elsewhere it stays where the
 * program put it, the system having no such menu, and the call is done. */
#define HERO_UI_ROLE_NONE 0
#define HERO_UI_ROLE_ABOUT 1
/* The program's settings; macOS gives it Command+, where the item has no
 * shortcut of its own. */
#define HERO_UI_ROLE_SETTINGS 2
/* Quit: on macOS the application menu's Quit, which without such an item
 * asks every window to close (`HERO_UI_EVENT_CLOSE` for each). */
#define HERO_UI_ROLE_QUIT 3

/* Gives menu item `id` the role `role`, `HERO_UI_ROLE_NONE` taking it
 * away. 0, or -1: `INVALID` (a role these do not name, a widget no menu
 * item), `NOT_FOUND`. */
int64_t hero_ui_set_role(hero_ui_app *app, int64_t id, int64_t role);

/* -- RETURN AND ESCAPE -----------------------------------------------------
 *
 * Return pressed in a text field, and the buttons Return and Escape press
 * wherever their window has the keys, as wxWidgets' `wxEVT_TEXT_ENTER`,
 * `wxButton::SetDefault` and a dialog's `wxID_CANCEL` give them: a search
 * field searches, a dialog's OK and Cancel answer the keys every dialog
 * answers. A window's button for Return takes Return before any field of
 * it, as the systems' dialogs do. */

/* The person pressed Return in text field `id`; its text is asked of the
 * field. */
#define HERO_UI_EVENT_SUBMIT 14

/* The key that presses a button: none, as a button is made; Return, the
 * button then drawn as its window's default; Escape. */
#define HERO_UI_BUTTON_KEY_NONE 0
#define HERO_UI_BUTTON_KEY_ENTER 1
#define HERO_UI_BUTTON_KEY_ESCAPE 2

/* Lets `key` press button `id` wherever its window has the keys, the one
 * of its window that had that key losing it, or takes the key away,
 * `HERO_UI_BUTTON_KEY_NONE`. Pressed so, the button's
 * `HERO_UI_EVENT_CLICK` is queued as a click's. 0, or -1: `INVALID` (a key
 * these do not name, a widget no button), `NOT_FOUND`. */
int64_t hero_ui_set_button_key(hero_ui_app *app, int64_t id, int64_t key);

/* -- THE WIDGET WITH THE KEYS ---------------------------------------------
 *
 * Which widget of a window takes what the person types, set and asked, as
 * wxWidgets' `wxWindow::SetFocus` and `FindFocus`; Tab and Shift+Tab move
 * it through the widgets in the order they stand, left to right and top to
 * bottom, the order the layout reads them in. The widgets that take the
 * keys on every system are text fields, text areas and lists. */

/* Gives widget `id`, a text field, a text area or a list, the keys in its
 * window; the person types into it once the window is in front. 0, or -1:
 * `INVALID` (a widget of another kind, or one the system will not give the
 * keys, as a hidden one), `NOT_FOUND`. */
int64_t hero_ui_set_focus(hero_ui_app *app, int64_t id);

/* Writes through `focus` the widget of window `id` that has the keys, `id`
 * itself where none of its widgets has them. 0, or -1: `INVALID` (a widget
 * no window), `NOT_FOUND`. */
int64_t hero_ui_focus(hero_ui_app *app, int64_t id, int64_t *focus);

/* -- A FOLDER CHOSEN, AND A MESSAGE BOX OF THE PROGRAM'S WORDS ---------------
 *
 * wxWidgets' `wxDirDialog`, and `wxMessageDialog` with labels of the
 * program's (`SetYesNoCancelLabels`) or Electron's `showMessageBox` with
 * its `buttons`: "Save", "Don't Save", "Cancel" before a document closes.
 * Each answers through the queue, as `hero_ui_message` and the file dialogs
 * do (DIALOGS ANSWERED THROUGH THE QUEUE, below). */

/* The most buttons a message box of the program's words holds. */
#define HERO_UI_ASK_BUTTONS_MOST 16

/* Shows the system's dialog to choose a folder, or several where `multiple`
 * is 1, over window `window`, titled `title`, and answers at once, 0, or
 * -1. The person's choice comes later as `HERO_UI_EVENT_DIALOG` naming
 * `dialog`, as a file dialog's does. */
int64_t hero_ui_folder_dialog(hero_ui_app *app, int64_t dialog, int64_t window, int64_t multiple, const void *title, uint64_t title_count);

/* Shows a message box over window `window` of `tone`, with `title` and
 * `text`, whose buttons are `buttons`, each label one line of UTF-8
 * followed by one zero byte, `buttons_count` bytes in all, 1 to
 * `HERO_UI_ASK_BUTTONS_MOST` of them, in the order the person reads them:
 * the first is pressed by Return, and the one at index `cancel` by Escape
 * (-1 for none). Answers at once, 0, or -1; the button pressed comes later
 * as `HERO_UI_EVENT_DIALOG` naming `dialog`, its `value` the button's
 * index. */
int64_t hero_ui_ask(hero_ui_app *app, int64_t dialog, int64_t window, int64_t tone, const void *title, uint64_t title_count, const void *text, uint64_t text_count, const void *buttons, uint64_t buttons_count, int64_t cancel);

/* -- CHECK AND RADIO MENU ITEMS ----------------------------------------------
 *
 * A menu item checked and unchecked (View > Show Sidebar), or one of a
 * group of which one is checked (View > as Icons, as List), as wxWidgets'
 * `wxITEM_CHECK` and `wxITEM_RADIO` items: the person's choosing turns a
 * check item over, and checks a radio item and unchecks the other radio
 * items of its group, the radio items of its menu no other item or
 * separator stands between, before the item's `HERO_UI_EVENT_MENU`, whose
 * `value` is its state after. `hero_ui_set_value` and `hero_ui_value` set
 * and read the state, 1 checked, 0 not, no event queued for a set. */
#define HERO_UI_CHECK_NONE 0
#define HERO_UI_CHECK_BOX 1
#define HERO_UI_CHECK_RADIO 2

/* Makes menu item `id` a check item, a radio item, or neither,
 * `HERO_UI_CHECK_NONE`; unchecked. 0, or -1: `INVALID` (a mode these do not
 * name, a widget no menu item), `NOT_FOUND`. */
int64_t hero_ui_set_check(hero_ui_app *app, int64_t id, int64_t mode);

/* -- A MASKED FIELD, AND A PROGRESS BAR WITH NO VALUE -------------------------
 *
 * A text field whose text the system masks as it is typed, a password's,
 * as wxWidgets' `wxTE_PASSWORD`; and a progress bar that moves with no
 * value while the work's end is unknown, as `wxGauge::Pulse`. */

/* Masks text field `id`'s text, 1, or shows it, 0, as a field is made; its
 * text, its place and its events are kept. 0, or -1: `INVALID` (a widget
 * no text field), `NOT_FOUND`. */
int64_t hero_ui_set_masked(hero_ui_app *app, int64_t id, int64_t on);

/* Lets progress bar `id` move with no value, 1, or shows its value, 0, as
 * a bar is made. 0, or -1: `INVALID` (a widget no progress bar),
 * `NOT_FOUND`. */
int64_t hero_ui_set_moving(hero_ui_app *app, int64_t id, int64_t on);


/* -- THE CLIPBOARD -----------------------------------------------------------
 *
 * The system's clipboard, the one every program copies to and pastes from,
 * as wxWidgets' `wxClipboard` with `wxTextDataObject`, `wxFileDataObject`
 * and `wxBitmapDataObject`, or Electron's `clipboard`: the program's own
 * copying and pasting, a list's path copied, a file manager's files, a
 * picture. A text field's Cut, Copy and Paste reach it through the Edit
 * menu without these. A write puts one kind in place of everything the
 * clipboard held, or empties it; a read asks for one kind and finds it where
 * the program that copied offered it, a program offering several at once.
 * What a write puts is found as its own kind alone. */
#define HERO_UI_CLIP_NOTHING 0
#define HERO_UI_CLIP_TEXT 1
#define HERO_UI_CLIP_FILES 2
#define HERO_UI_CLIP_IMAGE 3

/* The most bytes a write puts on the clipboard or a read takes from it: an
 * image of 8192 by 8192 pixels. */
#define HERO_UI_CLIP_MOST 268435456

/* Puts `count` bytes at `bytes` on the system's clipboard as `kind`, in
 * place of what it held: nothing, which empties it, with no bytes; UTF-8
 * text; absolute paths of UTF-8, each followed by one zero byte, at least
 * one; or `width` by `height` pixels of 8-bit RGBA, alpha straight, 4 bytes
 * a pixel, rows from the top. `width` and `height` are 0 for all but an
 * image. 0, or -1: `INVALID` (a kind these do not name, bytes not as the
 * kind takes them), `LIMIT` (past `HERO_UI_CLIP_MOST`), `OTHER` (the
 * system's refusal). */
int64_t hero_ui_clipboard_write(hero_ui_app *app, int64_t kind, const void *bytes, uint64_t count, int64_t width, int64_t height);

/* Reads the system's clipboard for `kind` and answers the bytes what it
 * holds of that kind takes, in the form `hero_ui_clipboard_write` takes,
 * an image's size written to `width` and `height` (0 for text and paths);
 * `hero_ui_clipboard_bytes` then writes them. -1: `NOT_FOUND` (the
 * clipboard holds nothing of that kind), `INVALID`, `LIMIT` (what it holds
 * takes past `HERO_UI_CLIP_MOST`). `HERO_UI_CLIP_NOTHING` is no kind a read
 * asks for. */
int64_t hero_ui_clipboard_read(hero_ui_app *app, int64_t kind, int64_t *width, int64_t *height);

/* Writes what the last `hero_ui_clipboard_read` found into `buffer` where
 * its `count` bytes hold it, and answers the bytes it takes all the same;
 * -1 where no read has found anything. */
int64_t hero_ui_clipboard_bytes(hero_ui_app *app, void *buffer, uint64_t count);

/* -- BOXES THE SYSTEM DRAWS: PAGES, A SCROLLED BOX, TWO PANES ---------------
 *
 * Pages chosen by tabs (wxWidgets' `wxNotebook`), a box whose content
 * scrolls (`wxScrolledWindow`) and two panes whose border the person drags
 * (`wxSplitterWindow`). A page, a scrolled box and a pane are CONTAINERS:
 * a widget whose parent is one stands in the container's own content, and
 * `hero_ui_place` gives its place from that content's top-left corner, as
 * a window's widgets are placed from its content's; every kind a window
 * holds, these included, may have a container for its parent instead. The
 * system sizes a page and a pane itself, and `hero_ui_place` refuses them. */

/* Tabs above pages, one page shown at a time; its parent a window or a
 * container. Holds pages alone. `hero_ui_set_selection` and
 * `hero_ui_selection` set and read the page shown, by its index in the
 * order the pages were made; the person choosing a tab queues
 * `HERO_UI_EVENT_SELECT`. */
#define HERO_UI_TABS 22
/* A page of tabs, a container; `hero_ui_set_text` sets its tab's title. */
#define HERO_UI_PAGE 23
/* A box whose content scrolls up and down within it, a container;
 * `hero_ui_set_size` sets its content's size. */
#define HERO_UI_SCROLL 24
/* Two panes side by side, or one over the other, and the divider between
 * them that the person drags; its parent a window or a container. Holds two
 * panes at most. `hero_ui_set_value` and `hero_ui_value` set and read the
 * divider's place, the first pane's width or height in points;
 * `hero_ui_set_range` bounds where the person may drag it; the person
 * dragging it queues `HERO_UI_EVENT_VALUE` with its place. Its
 * `hero_ui_preferred` is the divider's thickness on both axes. */
#define HERO_UI_SPLIT_ROW 25
#define HERO_UI_SPLIT_COLUMN 26
/* A pane of a split, a container, the first made the left or the top one. */
#define HERO_UI_PANE 27

/* Writes the room tabs' or a scrolled box's own drawing takes inside its
 * place around its content, in points, through `left`, `top`, `right` and
 * `bottom`: the tabs and the border around a page, the scroll bar where
 * the system draws it beside the content rather than over it. 0, or -1:
 * `INVALID` (a widget of another kind), `NOT_FOUND`. */
int64_t hero_ui_insets(hero_ui_app *app, int64_t id, int64_t *left, int64_t *top, int64_t *right, int64_t *bottom);

/* -- A TABLE: ROWS OF CELLS IN COLUMNS --------------------------------------
 *
 * Rows of cells under column headers whose widths the person drags, several
 * rows chosen, as wxWidgets' `wxDataViewListCtrl` or a report-mode
 * `wxListCtrl`: a file manager's details, a mail client's messages. A
 * table's parent is a window or a container. `hero_ui_set_items` gives its
 * cells, row after row, a whole number of rows of `hero_ui_set_columns`'
 * count; `hero_ui_set_selection` and `hero_ui_selection` set and read the
 * rows chosen, any number of them, and the person choosing queues
 * `HERO_UI_EVENT_SELECT`; a row opened queues `HERO_UI_EVENT_ACTIVATE`, as
 * a list's does. The person clicking a column's header asks for the rows
 * sorted by it, `HERO_UI_EVENT_SORT`; the program sorts its rows, gives
 * them again, and shows the column it sorted by with
 * `hero_ui_set_sort`. */
#define HERO_UI_TABLE 28

/* The person clicked the header of column `value` of table `id`, asking for
 * the rows sorted by it, ascending where `second` is 1 and descending where
 * it is 0: the column already sorted by turns over, another starts
 * ascending. The arrow is shown at once. */
#define HERO_UI_EVENT_SORT 15

/* The most columns a table holds. */
#define HERO_UI_COLUMNS_MOST 64

/* Gives table `id` its columns, each header's title one line of UTF-8
 * followed by one zero byte, `count` bytes in all, 1 to
 * `HERO_UI_COLUMNS_MOST` of them, in place of the columns and the cells it
 * held. 0, or -1: `INVALID` (a widget no table, titles not so),
 * `NOT_FOUND`. */
int64_t hero_ui_set_columns(hero_ui_app *app, int64_t id, const void *titles, uint64_t count);

/* Shows table `id` sorted by column `column`, ascending where `ascending`
 * is 1, descending where it is 0, its header's arrow showing it; by none
 * where `column` is -1. No event is queued. 0, or -1: `INVALID` (a widget
 * no table, a column past its columns), `NOT_FOUND`. */
int64_t hero_ui_set_sort(hero_ui_app *app, int64_t id, int64_t column, int64_t ascending);

/* -- A TREE: ITEMS FOLDED AND UNFOLDED ---------------------------------------
 *
 * Items under items, each folded or unfolded by the person, one chosen, as
 * wxWidgets' `wxTreeCtrl`: a disk's folders, a mail client's mailboxes. A
 * tree's parent is a window or a container. Its items are numbered by the
 * program, 1 or more, 0 naming the tree's top. An item may be FOLDABLE
 * before it holds any item, so a program loads what is under it only when
 * the person unfolds it, as a disk's folders cannot all load at once.
 * `hero_ui_set_selection` and `hero_ui_selection` set and read the item
 * chosen, by its number, one at most; the person choosing queues
 * `HERO_UI_EVENT_SELECT`, opening an item by a double click or Return
 * `HERO_UI_EVENT_ACTIVATE` with its number, and unfolding or folding one
 * `HERO_UI_EVENT_UNFOLD`. */
#define HERO_UI_TREE 29

/* The person unfolded item `value` of tree `id`, `second` 1, or folded it,
 * 0. An item unfolded the first time while it holds none is the program's
 * moment to add what is under it. */
#define HERO_UI_EVENT_UNFOLD 16

/* Adds item `item` to tree `id`, at the end of the items under item
 * `under` (0 for the top), showing `text`, one line of UTF-8, `count`
 * bytes; foldable where `foldable` is 1 though it holds no item yet. 0, or
 * -1: `INVALID` (a widget no tree, an item below 1, a text not so),
 * `NOT_FOUND` (no item `under`, or `item` taken). */
int64_t hero_ui_tree_add(hero_ui_app *app, int64_t id, int64_t item, int64_t under, const void *text, uint64_t count, int64_t foldable);

/* Takes item `item` out of tree `id`, with every item under it; nothing is
 * queued, the choice of one of them gone with it. 0, or -1: `INVALID`,
 * `NOT_FOUND`. */
int64_t hero_ui_tree_remove(hero_ui_app *app, int64_t id, int64_t item);

/* Unfolds item `item` of tree `id`, 1, or folds it, 0, as the person
 * would, but with no event queued. 0, or -1: `INVALID` (an item that is not
 * foldable), `NOT_FOUND`. */
int64_t hero_ui_tree_unfold(hero_ui_app *app, int64_t id, int64_t item, int64_t on);

/* -- A CONTEXT MENU ----------------------------------------------------------
 *
 * A menu shown where the person right-clicks a widget, or Control-clicks
 * it on macOS, as wxWidgets' `wxWindow::PopupMenu` on `wxEVT_CONTEXT_MENU`:
 * a file's Open, Rename and Delete. Its parent is the widget, or a window
 * for its content; one a widget. Its items are made as a menu's are,
 * `HERO_UI_MENU_ITEM`, `HERO_UI_MENU_SEPARATOR` and `HERO_UI_MENU` with it
 * for parent, and choosing one queues `HERO_UI_EVENT_MENU` as any item's
 * does; a shortcut of its items is shown and takes no key. Destroying it
 * takes it from the widget. */
#define HERO_UI_CONTEXT_MENU 30

/* -- DIALOGS ANSWERED THROUGH THE QUEUE --------------------------------------
 *
 * A message box, a box of the program's words, a file dialog and a folder
 * dialog answer their call at once and the person later, through the queue,
 * as GTK 4's `GtkAlertDialog` and `GtkFileDialog` and a macOS sheet do:
 * while the person thinks, the program's timers fire and its other windows
 * answer. A dialog is known by the number the program gives it, `dialog`,
 * 1 or more and not one of a dialog still shown (`HERO_UI_NOT_FOUND`), as a
 * widget is. It stands over window `window` of the app: on macOS it is a
 * sheet of that window, and the dialogs shown over one window come one
 * after the other, as the system shows them. Its answer is the event below,
 * queued once; destroying the window it stands over, or closing the app,
 * closes it unanswered, and no event comes for it. */

/* A dialog was answered: `id` is its number; `value` the button pressed,
 * `HERO_UI_ANSWER_*` for `hero_ui_message`, the index for `hero_ui_ask`, or,
 * for a file or folder dialog, how many paths the person chose, 0 where
 * they cancelled, the text holding them each followed by one zero byte,
 * read by `hero_ui_event_text`. */
#define HERO_UI_EVENT_DIALOG 17

/* -- AN ICON IN THE TRAY, AND AN APP OUT OF THE DOCK -------------------------
 *
 * wxWidgets' `wxTaskBarIcon`, Tauri's tray: an icon the system shows in its
 * tray, at the right of the menu bar on macOS, with a menu the person opens
 * by clicking it, and a utility that lives there alone. A widget of kind
 * `HERO_UI_TRAY` stands in no window, its parent 0, as a window's; one app
 * may show several. `hero_ui_set_image` gives its icon, pixels as an
 * image's, drawn at the menu bar's height on macOS; `hero_ui_set_text` its
 * words, shown where it has no icon and as its tip. Its menu is a context
 * menu whose parent is the tray (A CONTEXT MENU), each item's choosing
 * queuing `HERO_UI_EVENT_MENU`. Destroying it takes it from the tray. */
#define HERO_UI_TRAY 31

/* Shows the app in the Dock and with a menu bar of its own, `on` 1, as it
 * opens, or keeps it out of both, 0, its tray icons and its windows staying:
 * a utility of the tray alone (macOS's accessory application). 0, or -1. */
int64_t hero_ui_set_docked(hero_ui_app *app, int64_t on);

/* -- A NOTIFICATION ----------------------------------------------------------
 *
 * wxWidgets' `wxNotificationMessage`, Electron's `Notification`: a title and
 * its words the system shows in its own place, beside every application's,
 * and keeps in its list. */

/* Shows a notification of `title`, one line, not empty, and `text`. 0, or
 * -1: `UNSUPPORTED` where the system shows none for this program, on macOS a
 * program outside an application bundle. The system may hold it back where
 * the person has not allowed this program's notifications, and tells the
 * program nothing then. */
int64_t hero_ui_notify(hero_ui_app *app, const void *title, uint64_t title_count, const void *text, uint64_t text_count);

/* -- THE DARK OR LIGHT APPEARANCE --------------------------------------------
 *
 * wxWidgets' `wxSystemAppearance::IsDark` and its change: the appearance the
 * system draws the app's windows in, which the person turns in the system's
 * settings, and which a drawing, a web view's page or a tray icon follows. */
#define HERO_UI_APPEARANCE_LIGHT 1
#define HERO_UI_APPEARANCE_DARK 2

/* The app's appearance turned: `value` the new one, `HERO_UI_APPEARANCE_*`;
 * `id` 0. Queued once each time it turns, a second turn before the first is
 * taken replacing it. `hero_ui_inject` with this kind draws the app in
 * appearance `value`, as the system's turn would, or in the system's own
 * again where `value` is 0, `id` 0. */
#define HERO_UI_EVENT_APPEARANCE 18

/* Writes the appearance the app's windows are drawn in now through
 * `appearance`. 0, or -1. */
int64_t hero_ui_appearance(hero_ui_app *app, int64_t *appearance);

/* -- ACCESSIBLE NAMES --------------------------------------------------------
 *
 * Qt's `QWidget::setAccessibleName` and `QLabel::setBuddy`, ARIA's
 * `aria-label` and `aria-labelledby`: the name a screen reader says for a
 * widget that shows no words of its own (a picture, a slider, a button of
 * an image) or whose words do not say what it is, and a label tied to the
 * field it names, so the field is read by the label's words. */

/* Gives widget `id` the accessible name `name`, `count` bytes of one line;
 * none, `count` 0, takes it back, the widget then named by its own words.
 * 0, or -1. */
int64_t hero_ui_set_accessible_name(hero_ui_app *app, int64_t id, const void *name, uint64_t count);

/* Writes the accessible name widget `id` holds, as the system holds it,
 * into `buffer` and answers its length, 0 for none; where it is longer than
 * `count`, nothing is written and the length is answered all the same. -1
 * on failure. */
int64_t hero_ui_accessible_name(hero_ui_app *app, int64_t id, void *buffer, uint64_t count);

/* Ties label `label` to widget `id`, which a screen reader then names by
 * the label's words; `label` 0 unties it. 0, or -1, `INVALID` where `label`
 * is no label. */
int64_t hero_ui_set_label_for(hero_ui_app *app, int64_t id, int64_t label);

/* -- THE SESSION'S END ASKING THE PROGRAM -----------------------------------
 *
 * wxWidgets' `wxEVT_QUERY_END_SESSION`, Qt's `commitDataRequest`: the
 * system asks the app to end, at the person's logout, restart or shutdown or
 * at another program's request, and an editor with unsaved work is asked
 * before it is ended. */

/* The system asks the app to end: `value` 1 for the session's end (the
 * person logging out, restarting or shutting down), 0 for another quit
 * asked of the app (on macOS the Dock's Quit, another program's quit
 * request); `id` 0. Nothing ends, and the system waits: the program saves
 * what the person would lose, then answers by `hero_ui_answer_quit`, or by
 * `hero_ui_close`, which answers that the app ends. Each ask is queued, none
 * replacing another. `hero_ui_inject` with this kind asks the app to end as
 * the system would, `value` 1 as the session's end, `id` 0. The app menu's
 * Quit on macOS is not such an ask: it asks every window to close
 * (`HERO_UI_ROLE_QUIT`). */
#define HERO_UI_EVENT_QUIT 19

/* Answers every ask to end the app not yet answered: `go` 1, the app ends
 * and the session's end goes on once it has; 0, the app stays, and a
 * session's end waiting on it is cancelled, as the person's own Cancel in
 * an editor's "save changes?" would. With no ask waiting it answers
 * nothing. 0, or -1. */
int64_t hero_ui_answer_quit(hero_ui_app *app, int64_t go);

/* -- FILES DROPPED ON A WINDOW ----------------------------------------------
 *
 * wxWidgets' `wxWindow::DragAcceptFiles` and `wxDropFilesEvent`: the person
 * drags files from the system's file manager and drops them on a window, an
 * editor opening each. */

/* The person dropped files on window `id`, which takes them
 * (`hero_ui_set_drop`): `value` and `second` the point of the drop, in
 * points from the top-left corner of the window's content; the text the
 * paths, each followed by one zero byte, read by `hero_ui_event_text`. A
 * drop on a text area of the window is the window's; one on a web view is
 * the page's own, and one on a text field the person is typing in types
 * the path, as the system's fields do. `hero_ui_inject` with this kind
 * drops the paths `text` holds, each followed by one zero byte, on window
 * `id` at `value`, `second`, as the person would. */
#define HERO_UI_EVENT_DROP 20

/* Has window `id` take files dropped on it, `on` 1, or take none, 0, as a
 * window does until asked. 0, or -1, `INVALID` where `id` is no window. */
int64_t hero_ui_set_drop(hero_ui_app *app, int64_t id, int64_t on);

/* -- UNDO AND THE SELECTION IN TEXT WIDGETS ---------------------------------
 *
 * wxWidgets' `wxTextEntry::Undo`, `Redo`, `CanUndo`, `CanRedo`,
 * `SetSelection` and `GetSelection`: an editor's own Undo and Redo, and its
 * find, replace and go to line, which select what they found and show it.
 * A text area keeps the person's edits from the last text the program set,
 * which none undoes past; a text field keeps those the person makes while
 * it has the keys, as the system's fields do. A text the program sets puts
 * the caret at its end. */
#define HERO_UI_UNDO 1
#define HERO_UI_REDO 2

/* Undoes the last edit of text field or text area `id`, `which`
 * `HERO_UI_UNDO`, or redoes the last undone, `HERO_UI_REDO`, as the Edit
 * menu's Undo and Redo do. Nothing is queued, as for every change the
 * program makes: the text it leaves is read by `hero_ui_text`. Answers 1
 * where an edit was undone or redone, 0 where none was there, -1 on
 * failure. */
int64_t hero_ui_undo(hero_ui_app *app, int64_t id, int64_t which);

/* 1 where `hero_ui_undo` with `which` would undo or redo an edit of text
 * field or text area `id` now, 0 where it would not, -1 on failure. */
int64_t hero_ui_can_undo(hero_ui_app *app, int64_t id, int64_t which);

/* Selects bytes `start` to `end` of the text of text field or text area
 * `id`, as `hero_ui_text` gives it: each at the first byte of a character
 * or at the text's end, `start` at most `end`; equal, they put the caret
 * there with nothing selected. A text area shows the selection, scrolled
 * to it; a text field takes the keys, where the system shows a field's
 * selection. 0, or -1, `INVALID` where either is outside the text or
 * inside a character, or the text field is hidden or greyed out. */
int64_t hero_ui_set_text_selection(hero_ui_app *app, int64_t id, int64_t start, int64_t end);

/* Writes the selection of text field or text area `id` through `start` and
 * `end`, in bytes of its text as `hero_ui_text` gives it; with nothing
 * selected both are the caret. A text field without the keys answers the
 * selection it had when it last held them, or its text's end where it
 * never held them since the program set its text. 0, or -1. */
int64_t hero_ui_text_selection(hero_ui_app *app, int64_t id, int64_t *start, int64_t *end);

/* -- THE APP'S IDENTITY, ONE INSTANCE, AND DOCUMENTS FROM THE SYSTEM --------
 *
 * wxWidgets' `wxApp::SetAppName`, `wxSingleInstanceChecker` and
 * `wxApp::MacOpenFiles`, Electron's `app.requestSingleInstanceLock` and
 * `open-file`: the name the system knows the app by, one process of it at a
 * time, a second launch handing its arguments to the first, and the
 * documents the system asks the app to open. */

/* The most bytes of an app's identifier. */
#define HERO_UI_IDENTITY_MOST 128

/* Gives the app its identity: `identifier`, a reverse domain name (two or
 * more parts joined by dots, each ASCII letters, digits and hyphens, the
 * first a letter; at most `HERO_UI_IDENTITY_MOST` bytes), which keys its one
 * instance, and `name`, one line, the name its menus show the person.
 * Before the app's first window and once: `INVALID` after either. Inside a
 * macOS application bundle the identifier is the bundle's own
 * (`CFBundleIdentifier`), and another is `INVALID`, naming it. 0, or -1. */
int64_t hero_ui_set_identity(hero_ui_app *app, const void *identifier, uint64_t identifier_count, const void *name, uint64_t name_count);

/* Makes this process its identity's one instance: answers 1 where no other
 * process of it runs for this user, each later launch's arguments then
 * coming as `HERO_UI_EVENT_OPEN` with `HERO_UI_OPEN_ARGUMENTS`; 0 where one
 * runs, `arguments` (`count` bytes, each argument followed by one zero
 * byte, the program's own name left out) having been handed to it, so this
 * process closes the app and ends; -1, `INVALID` before the identity. The
 * claim holds until the app closes; a second claim answers 1 again. */
int64_t hero_ui_claim_instance(hero_ui_app *app, const void *arguments, uint64_t count);

/* Documents, addresses or a later launch's arguments handed to the app:
 * `value` how many, `second` what they are, the text holding them, each
 * followed by one zero byte; `id` 0. `hero_ui_inject` with this kind hands
 * them as the system would: files and addresses by the system's own open
 * request (on macOS its Apple events), arguments as a later launch of the
 * claimed instance would, through its hand-over. */
#define HERO_UI_EVENT_OPEN 21
/* Paths of files the system asks the app to open: a double click, Open
 * With, a drop on its Dock icon (on macOS for an app in a bundle that
 * names their types). */
#define HERO_UI_OPEN_FILES 1
/* Addresses of a scheme the app claims (on macOS a bundle's
 * `CFBundleURLTypes`). */
#define HERO_UI_OPEN_URLS 2
/* A later launch's arguments (`hero_ui_claim_instance`). */
#define HERO_UI_OPEN_ARGUMENTS 3

#endif
