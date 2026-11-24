/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_arm.h: what `hero_ui.c` and each system's arm of the
 * native user interface share, private to the layer's three members that
 * draw it, `hero_ui.c` (the common part and the macOS arm, which hands
 * AppKit's work to `hero_ui_appkit.swift` and `hero_ui_appkit_windows.swift`),
 * `hero_ui_win32.c` (Win32's own controls) and `hero_ui_gtk.c` (GTK 4, on
 * every system neither Apple's nor Windows). No program includes it.
 *
 * TWO DIRECTIONS. `hero_ui.c` calls THE ARM, `hero_ui_arm_*`, which the
 * member of the system it is compiled on defines: on macOS `hero_ui.c`
 * itself, on Windows `hero_ui_win32.c`, elsewhere `hero_ui_gtk.c`, the other
 * two compiling to nothing there. And the two C arms call what `hero_ui.c`
 * keeps for both (WHAT THE WIN32 AND GTK ARMS SHARE): the table of their
 * widgets, the queue they tell, the words of their refusals.
 *
 * NO CONDITIONAL BUT ITS GUARD, as a header of the layer (panel 234's R6),
 * so each arm is compiled against the one prototype of every function. */

#ifndef HERO_UI_ARM_H
#define HERO_UI_ARM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "hero_ui.h"
#include "hero_ui_web.h"

/* The words kept of a failure, with their zero byte. */
#define HERO_UI_WORDS_MOST 512

/* -- THE ARM ------------------------------------------------------------------
 *
 * What each system's side implements. Each is called on the UI thread, for
 * the one native app, its arguments checked by the entry point that calls
 * it: a widget number 1 or more, a kind the arm draws, a pointer with its
 * count behind it, a text of UTF-8 with no zero byte, a size 0 or more. Each
 * answers a kind, `HERO_UI_OK` where done; `hero_ui_arm_words` then gives
 * the system's own words for a `HERO_UI_OTHER`. The calls' meanings are
 * `hero_ui.h`'s functions of the same name; `which` chooses between the
 * flags (`HERO_UI_FLAG_*`) and the two sizes (`HERO_UI_SIZE_*`), and
 * `place` with an `index` of -1 places the widget, with 0 or more a radio
 * group's item.
 *
 * A web view's arm takes two more: `web_load` shows a whole URL of the
 * app's own origin, which this file built from a path it checked, and
 * `web_post` hands the page a text, or keeps it until the page has loaded.
 * What a web view serves and where it may go are this file's (THE WEB VIEW,
 * below), so every system answers alike: an arm asks `hero_ui_web_serve`
 * what to answer a request of the app's scheme, `hero_ui_web_ask` whether a
 * navigation or a message's sender is allowed, and tells
 * `hero_ui_web_gone` when it destroyed a web view; `open` hands it the
 * three, with `hero_ui_push` and the bridge's script. */

#define HERO_UI_FLAG_ENABLED 1
#define HERO_UI_FLAG_VISIBLE 2
#define HERO_UI_FLAG_RESIZABLE 3
#define HERO_UI_SIZE_CONTENT 1
#define HERO_UI_SIZE_LEAST 2
/* Which of a window's settings `window_set` makes, `value` checked by the
 * entry point. */
#define HERO_UI_WINDOW_SET_ON_TOP 1
#define HERO_UI_WINDOW_SET_FRAMED 2
#define HERO_UI_WINDOW_SET_OPACITY 3
#define HERO_UI_WINDOW_SET_BACKDROP 4
#define HERO_UI_WINDOW_SET_MODAL 5

/* The arm's functions, in the order `hero_ui.c` first defines them on macOS. */
int64_t hero_ui_arm_here(void);
void hero_ui_arm_absent(char *words, size_t count);
int64_t hero_ui_arm_open(void);
void hero_ui_arm_close(void);
int64_t hero_ui_arm_words(char *words, int64_t count);
int64_t hero_ui_arm_create(int64_t id, int64_t kind, int64_t parent);
int64_t hero_ui_arm_destroy(int64_t id);
int64_t hero_ui_arm_set_text(int64_t id, const uint8_t *text, int64_t count);
int64_t hero_ui_arm_set_items(int64_t id, const uint8_t *items, int64_t count);
int64_t hero_ui_arm_set_value(int64_t id, int64_t value);
int64_t hero_ui_arm_set_range(int64_t id, int64_t least, int64_t most);
int64_t hero_ui_arm_set_selection(int64_t id, const int64_t *indices, int64_t count);
int64_t hero_ui_arm_set_flag(int64_t id, int64_t which, int64_t on);
int64_t hero_ui_arm_set_shortcut(int64_t id, const uint8_t *key, int64_t count, int64_t modifiers);
int64_t hero_ui_arm_set_image(int64_t id, const uint8_t *rgba, int64_t width, int64_t height);
int64_t hero_ui_arm_set_size(int64_t id, int64_t which, int64_t width, int64_t height);
int64_t hero_ui_arm_place(int64_t id, int64_t index, int64_t x, int64_t y, int64_t width, int64_t height);
int64_t hero_ui_arm_preferred(int64_t id, int64_t *width, int64_t *height);
int64_t hero_ui_arm_text(int64_t id, uint8_t *buffer, int64_t count, int64_t *length);
int64_t hero_ui_arm_value(int64_t id, int64_t *value);
int64_t hero_ui_arm_selection(int64_t id, int64_t *indices, int64_t count, int64_t *chosen);
int64_t hero_ui_arm_pump(int64_t ms);
int64_t hero_ui_arm_inject(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count);
int64_t hero_ui_arm_web_load(int64_t id, const uint8_t *url, int64_t count);
int64_t hero_ui_arm_web_post(int64_t id, const uint8_t *text, int64_t count);
int64_t hero_ui_arm_windows_words(char *words, int64_t count);
int64_t hero_ui_arm_press_key(int64_t id, const uint8_t *key, int64_t count, int64_t modifiers);
bool hero_ui_arm_makes(int64_t kind, int64_t parent);
bool hero_ui_arm_acts(int64_t act);
int64_t hero_ui_arm_watch(int64_t id);
int64_t hero_ui_arm_screens(int64_t *values, int64_t count, int64_t *screens);
int64_t hero_ui_arm_frame_of(int64_t id, int64_t *screen, int64_t *x, int64_t *y, int64_t *width, int64_t *height);
int64_t hero_ui_arm_set_position(int64_t id, int64_t screen, int64_t x, int64_t y, int64_t person);
int64_t hero_ui_arm_center(int64_t id, int64_t over);
int64_t hero_ui_arm_set_state(int64_t id, int64_t state, int64_t person);
int64_t hero_ui_arm_state(int64_t id, int64_t *state);
int64_t hero_ui_arm_window_set(int64_t id, int64_t which, int64_t value);
int64_t hero_ui_arm_set_icon(const uint8_t *rgba, int64_t width, int64_t height);
bool hero_ui_arm_modal_here(void);
int64_t hero_ui_arm_modal_pump(int64_t ms);
int64_t hero_ui_arm_set_role(int64_t id, int64_t role);
int64_t hero_ui_arm_set_button_key(int64_t id, int64_t key);
int64_t hero_ui_arm_set_focus(int64_t id);
int64_t hero_ui_arm_focus(int64_t id, int64_t *focus);
int64_t hero_ui_arm_set_check(int64_t id, int64_t mode);
int64_t hero_ui_arm_set_masked(int64_t id, int64_t on);
int64_t hero_ui_arm_set_moving(int64_t id, int64_t on);
int64_t hero_ui_arm_clipboard_set(int64_t kind, const uint8_t *bytes, int64_t count, int64_t width, int64_t height);
int64_t hero_ui_arm_clipboard_get(int64_t kind, int64_t *bytes, int64_t *width, int64_t *height);
int64_t hero_ui_arm_clipboard_bytes(uint8_t *buffer, int64_t count);
int64_t hero_ui_arm_insets(int64_t id, int64_t *left, int64_t *top, int64_t *right, int64_t *bottom);
int64_t hero_ui_arm_set_columns(int64_t id, const uint8_t *titles, int64_t count);
int64_t hero_ui_arm_set_sort(int64_t id, int64_t column, int64_t ascending);
int64_t hero_ui_arm_tree_add(int64_t id, int64_t item, int64_t under, const uint8_t *text, int64_t count, int64_t foldable);
int64_t hero_ui_arm_tree_remove(int64_t id, int64_t item);
int64_t hero_ui_arm_tree_unfold(int64_t id, int64_t item, int64_t on);

/* DIALOGS ANSWERED THROUGH THE QUEUE (`hero_ui.h`). Each `show_*` shows
 * dialog `dialog` over window `window` and answers at once; the person's
 * answer is queued later by `hero_ui_push`, `HERO_UI_EVENT_DIALOG` naming
 * `dialog`, with the value and the paths `hero_ui.h` says. `answer_dialog`
 * does to a dialog shown what its person does, its answer then queued as
 * theirs; `dialogs_end` ends unanswered every dialog over window `window`,
 * or every one where `window` is 0, before the window or the app goes; and
 * `dialogs_words` gives the words of the last `HERO_UI_OTHER` or
 * `HERO_UI_UNSUPPORTED` they answered. `hero_ui.c` defines them on macOS,
 * by `hero_ui_appkit_dialogs.swift`'s sheets; the Win32 and GTK 4 arms
 * define their own, their dialogs answering later by `hero_ui_tell_text`. */
int64_t hero_ui_arm_show_message(int64_t dialog, int64_t window, int64_t tone, int64_t buttons, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count);
int64_t hero_ui_arm_show_ask(int64_t dialog, int64_t window, int64_t tone, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count, const uint8_t *buttons, int64_t buttons_count, int64_t cancel);
int64_t hero_ui_arm_show_files(int64_t dialog, int64_t window, int64_t save, int64_t multiple, const uint8_t *title, int64_t title_count, const uint8_t *name, int64_t name_count, const uint8_t *filters, int64_t filters_count);
int64_t hero_ui_arm_show_folders(int64_t dialog, int64_t window, int64_t multiple, const uint8_t *title, int64_t title_count);
int64_t hero_ui_arm_answer_dialog(int64_t dialog, int64_t value, const uint8_t *paths, int64_t count);
void hero_ui_arm_dialogs_end(int64_t window);
int64_t hero_ui_arm_dialogs_words(char *words, int64_t count);

/* THE PRINT AND PAGE-SETUP DIALOGS (`hero_ui.h`'s `hero_ui_print_dialog` and
 * `hero_ui_web_print`), answered through the queue as the dialogs above are.
 * `settings` is THE SETTINGS' TEXT (`platform/hero_ui_print_win32.h`).
 * `print_show` shows the print dialog (`page` 0) or the page-setup dialog
 * (`page` 1) as dialog `dialog` over window `window`; `print_web` prints the
 * page of web view `view`, through the print dialog first where `ask` is 1.
 * Each answers at once, a kind of `hero_ui.h`, and the person's answer is
 * queued later by `hero_ui_push`: `HERO_UI_EVENT_DIALOG` naming `dialog`,
 * value 1 with the settings chosen (or printed with) as its text, 0 where
 * they cancelled. `print_answer` does what the person does to a print dialog
 * or a web view's print shown, `value` 1 with the settings in `settings` or 0
 * for Cancel, and answers `HERO_UI_NOT_FOUND`, before reading anything else,
 * where no print of that number stands (the common part then tries the other
 * dialogs); `print_end` ends unanswered every print over window `window`, or
 * every one where it is 0, before the window or the app goes; `print_words`
 * gives the words of the last refusal each of them answered. `hero_ui.c`
 * defines them on macOS by `hero_ui_appkit_print.swift`'s sheets; the Win32
 * and GTK 4 arms define their own over `hero_ui_print_win32.c` and
 * `hero_ui_print_gtk.c`, which `hero_ui.c` answers for until they do. */
int64_t hero_ui_arm_print_show(int64_t dialog, int64_t window, int64_t page, const uint8_t *settings, int64_t count);
int64_t hero_ui_arm_print_web(int64_t dialog, int64_t view, int64_t ask, const uint8_t *settings, int64_t count);
int64_t hero_ui_arm_print_answer(int64_t dialog, int64_t value, const uint8_t *settings, int64_t count);
void hero_ui_arm_print_end(int64_t window);
int64_t hero_ui_arm_print_words(char *words, int64_t count);

/* THE DESCRIPTORS A WAIT WATCHES (`hero_ui.h`'s `hero_ui_add_set`).
 * `watch_fd` has the arm's loop wake, and `hero_ui_watch_told(id)` be called
 * on the main thread, once descriptor `fd` is readable, `id` being the
 * watch's number (a descriptor the program never closes through the arm, only
 * waits on for readability); once told it is not watched again until
 * `watch_fd` is called for it again, which `hero_ui.c` does at each wait, so a
 * descriptor that stays readable is told once per wait and never in a spin;
 * called for a number watched already it arms that watch again. `unwatch_fd`
 * takes watch `id` away, or every watch where it is 0. `hero_ui.c` defines
 * them on macOS (a `CFFileDescriptor` source of the main run loop); the Win32
 * and GTK 4 arms define their own (`g_unix_fd_add` on GTK 4, whose callback
 * calls `hero_ui_watch_told` and returns `G_SOURCE_REMOVE`), `hero_ui.c`
 * answering `HERO_UI_UNSUPPORTED` for them until they do. `hero_ui_watch_told`
 * answers whether it queued an event. On Windows `fd` is the HANDLE of the
 * event the set owns (`hero_poll_descriptor`), which the arm waits on in the
 * same `MsgWaitForMultipleObjectsEx` as the window messages, one shot as
 * above; `HERO_UI_LIMIT` for more than 63 watched at once. */
int64_t hero_ui_arm_watch_fd(int64_t fd, int64_t id);
void hero_ui_arm_unwatch_fd(int64_t id);
bool hero_ui_watch_told(int64_t id);

/* THE TRAY (`hero_ui.h`): a tray icon is a kind `hero_ui_arm_makes` and
 * `create` answer, its words and its image `set_text`'s and `set_image`'s;
 * `set_docked` keeps the app in the Dock or out of it. `hero_ui.c` defines
 * `set_docked` on macOS; the Win32 and GTK 4 arms define their own. */
int64_t hero_ui_arm_set_docked(int64_t on);

/* A NOTIFICATION (`hero_ui.h`), and the words of the last refusal of a
 * call on the system around the program. `hero_ui.c` defines `notify` on
 * macOS by `hero_ui_appkit_system.swift`; the Win32 and GTK 4 arms define
 * their own, and `system_words` is `hero_ui.c`'s on Windows (the words its
 * member's refusals leave) and the GTK 4 member's own. */
int64_t hero_ui_arm_notify(const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count);
int64_t hero_ui_arm_system_words(char *words, int64_t count);

/* THE DARK OR LIGHT APPEARANCE (`hero_ui.h`): asked, and turned as the
 * system would turn it for `hero_ui_inject`; each arm watches it from
 * `open` to `close`, queuing `HERO_UI_EVENT_APPEARANCE`. */
int64_t hero_ui_arm_appearance(int64_t *appearance);
int64_t hero_ui_arm_appearance_set(int64_t appearance);

/* THE SESSION'S END ASKING THE PROGRAM (`hero_ui.h`): on macOS the arm
 * holds each ask from `open` to `close`, queuing `HERO_UI_EVENT_QUIT` and
 * answering it once the program does, or as ending at `close`;
 * `answer_quit` answers every ask held, `ask_quit` asks as the system would
 * for `hero_ui_inject`. Each arm defines its own. */
int64_t hero_ui_arm_answer_quit(int64_t go);
int64_t hero_ui_arm_ask_quit(int64_t session_end);

/* FILES DROPPED ON A WINDOW (`hero_ui.h`): `set_drop` has window `id` take
 * them or not; `drop` drops `count` bytes of paths, each followed by one zero
 * byte, at `x`, `y` of its content as the person would, for
 * `hero_ui_inject`. Each arm queues `HERO_UI_EVENT_DROP` with the paths. */
int64_t hero_ui_arm_set_drop(int64_t id, int64_t on);
int64_t hero_ui_arm_drop(int64_t id, int64_t x, int64_t y, const uint8_t *paths, int64_t count);

/* UNDO AND THE SELECTION IN TEXT WIDGETS (`hero_ui.h`): `undo` writes
 * through `done` 1 where an edit was undone or redone, 0 where none was,
 * and `can_undo` through `can` likewise; the selection is in bytes of the
 * text. On macOS by `hero_ui_appkit.swift`; refusals for the Win32 and GTK
 * 4 arms in `hero_ui.c`'s CALLS NO C ARM DRAWS YET. */
int64_t hero_ui_arm_undo(int64_t id, int64_t which, int64_t *done);
int64_t hero_ui_arm_can_undo(int64_t id, int64_t which, int64_t *can);
int64_t hero_ui_arm_set_text_selection(int64_t id, int64_t start, int64_t end);
int64_t hero_ui_arm_text_selection(int64_t id, int64_t *start, int64_t *end);

/* THE APP'S IDENTITY, ONE INSTANCE, AND DOCUMENTS FROM THE SYSTEM
 * (`hero_ui.h`): `hero_ui.c` checks the identity and keeps it; `set_identity`
 * gives the system its name, `claim_instance` writes through `first` 1 for
 * the one instance and 0 where `arguments` were handed to another,
 * `hand_over` hands `arguments` to this process's own claim as a later
 * launch would, `open_items` asks the app to open `count` bytes of paths
 * (`what` `HERO_UI_OPEN_FILES`) or addresses (`HERO_UI_OPEN_URLS`) as the
 * system would, and `instance_end` gives the claim up at `close`. On macOS
 * the claim is `hero_ui.c`'s, a lock file and a local socket in the user's
 * own temporary folder; refusals for the Win32 and GTK 4 arms in the block
 * of calls neither draws. */
int64_t hero_ui_arm_set_identity(const uint8_t *identifier, int64_t identifier_count, const uint8_t *name, int64_t name_count);
int64_t hero_ui_arm_claim_instance(const uint8_t *identifier, int64_t identifier_count, const uint8_t *arguments, int64_t count, int64_t *first);
int64_t hero_ui_arm_hand_over(const uint8_t *arguments, int64_t count);
int64_t hero_ui_arm_open_items(int64_t what, const uint8_t *items, int64_t count);
void hero_ui_arm_instance_end(void);

/* A TIP OVER A WIDGET (`hero_ui.h`): `tip` writes the tip where it fits
 * and its length through `length`, as `text` does. On macOS by
 * `hero_ui_appkit.swift`; refusals for the Win32 and GTK 4 arms in the
 * block of calls neither draws. */
int64_t hero_ui_arm_set_tip(int64_t id, const uint8_t *text, int64_t count);
int64_t hero_ui_arm_tip(int64_t id, uint8_t *buffer, int64_t count, int64_t *length);

/* A DRAWING AREA (`hero_ui.h`'s A DRAWING AREA, WITH ITS MOUSE AND ITS KEYS):
 * `hero_ui.c` has checked a drawing whole (`hero_ui_drawing_checked`) before
 * `set_drawing` hands its `count` bytes to the arm, which keeps them and has
 * the area drawn again; `snapshot` writes through `width` and `height` the
 * area's size and, where `count` holds width * height * 4, the drawing
 * replayed at a pixel to a point into `rgba`; `pointer` acts as the person's
 * pointer does at `x`, `y` in 64ths of a point (`hero_ui.h`'s phases,
 * buttons and held keys). On macOS by `hero_ui_appkit.swift`; refusals for
 * the Win32 and GTK 4 arms in the block of calls neither draws, which a
 * system leaves where its member defines them. */
int64_t hero_ui_arm_set_drawing(int64_t id, const uint8_t *list, int64_t count);
int64_t hero_ui_arm_snapshot(int64_t id, uint8_t *rgba, int64_t count, int64_t *width, int64_t *height);
int64_t hero_ui_arm_pointer(int64_t id, int64_t x, int64_t y, int64_t phase, int64_t button, int64_t clicks, int64_t modifiers);

/* ACCESSIBLE NAMES (`hero_ui.h`): defined on macOS by `hero_ui.c` handing
 * them to `hero_ui_appkit.swift`, and by the Win32 and GTK 4 members.
 * `accessible_name` writes the name where it fits and its length through
 * `length`, as `text` does. */
int64_t hero_ui_arm_set_accessible_name(int64_t id, const uint8_t *name, int64_t count);
int64_t hero_ui_arm_accessible_name(int64_t id, uint8_t *buffer, int64_t count, int64_t *length);
int64_t hero_ui_arm_set_label_for(int64_t id, int64_t label);

/* A DRAWING AREA'S PARTS (`hero_ui.h`'s A DRAWING AREA'S PARTS, READ BY A
 * SCREEN READER): `hero_ui.c` has checked the records whole
 * (`hero_ui_parts_checked`) before `set_parts` hands their `count` bytes to
 * the arm, which builds the system's accessibility objects of them;
 * `parts` writes the records read back from those objects where `count`
 * holds them and their length through `length`; `part_act` acts on part
 * `part` (1 the first) as a screen reader does, `act` one of
 * `HERO_UI_PART_PRESS` to `_DECREASE`, checked for the part's role and
 * states by the arm, which holds them. On macOS by `hero_ui_appkit.swift`;
 * refusals for the Win32 and GTK 4 arms in `hero_ui.c`'s block of the parts'
 * calls, which a system leaves where its member defines them. */
int64_t hero_ui_arm_set_parts(int64_t id, const uint8_t *list, int64_t count);
int64_t hero_ui_arm_parts(int64_t id, uint8_t *buffer, int64_t count, int64_t *length);
int64_t hero_ui_arm_part_act(int64_t id, int64_t part, int64_t act);

/* -- WHAT THE WIN32 AND GTK ARMS SHARE ----------------------------------------
 *
 * Kept by `hero_ui.c` for whichever of the two is compiled, each object the
 * UI thread's own (THE THREAD), and each function documented where
 * `hero_ui.c` defines it. */

/* An arm's record of one widget: its number, its kind, its parent, and the
 * system's objects; each arm defines it. */
typedef struct hero_ui_native hero_ui_native;

/* Numbers in an order: a window's menus, a menu's items. */
typedef struct hero_ui_ids {
    int64_t *at;
    int64_t count;
    int64_t room;
} hero_ui_ids;

size_t hero_ui_copy_text(char *into, size_t most, const char *text);
bool hero_ui_is_utf8(const uint8_t *text, uint64_t length);
int64_t hero_ui_refused(int64_t kind, const char *words);
void hero_ui_tell(int64_t kind, int64_t id, int64_t value, int64_t second);
void hero_ui_tell_text(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count);
void hero_ui_make_host(void);
hero_ui_native *hero_ui_find(int64_t id);
bool hero_ui_keep(int64_t id, hero_ui_native *widget);
void hero_ui_forget(int64_t id);
hero_ui_native *hero_ui_at(size_t at);
void hero_ui_free_map(void);
bool hero_ui_ids_add(hero_ui_ids *ids, int64_t id);
void hero_ui_ids_drop(hero_ui_ids *ids, int64_t id);
void hero_ui_ids_free(hero_ui_ids *ids);
int64_t hero_ui_texts_in(const uint8_t *items, int64_t count);
const uint8_t *hero_ui_text_at(const uint8_t *items, int64_t count, int64_t index, int64_t *length);
int64_t hero_ui_between(int64_t value, int64_t least, int64_t most);
double hero_ui_fraction(int64_t value, int64_t least, int64_t most);
int64_t hero_ui_at_fraction(double fraction, int64_t least, int64_t most);
void hero_ui_mute(int64_t by);
void hero_ui_unmute(void);
bool hero_ui_is_muted(void);
uint64_t hero_ui_pushed_count(void);
size_t hero_ui_map_room(void);
const hero_ui_web_host *hero_ui_web_host_of(void);

#endif
