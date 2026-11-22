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
 * by `hero_ui_appkit_dialogs.swift`'s sheets, and for the Win32 arm by THE
 * WAITING FORM below, until it defines its own; the GTK 4 arm defines its
 * own, GTK's dialogs answering later by their callbacks. */
int64_t hero_ui_arm_show_message(int64_t dialog, int64_t window, int64_t tone, int64_t buttons, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count);
int64_t hero_ui_arm_show_ask(int64_t dialog, int64_t window, int64_t tone, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count, const uint8_t *buttons, int64_t buttons_count, int64_t cancel);
int64_t hero_ui_arm_show_files(int64_t dialog, int64_t window, int64_t save, int64_t multiple, const uint8_t *title, int64_t title_count, const uint8_t *name, int64_t name_count, const uint8_t *filters, int64_t filters_count);
int64_t hero_ui_arm_show_folders(int64_t dialog, int64_t window, int64_t multiple, const uint8_t *title, int64_t title_count);
int64_t hero_ui_arm_answer_dialog(int64_t dialog, int64_t value, const uint8_t *paths, int64_t count);
void hero_ui_arm_dialogs_end(int64_t window);
int64_t hero_ui_arm_dialogs_words(char *words, int64_t count);

/* THE TRAY (`hero_ui.h`): a tray icon is a kind `hero_ui_arm_makes` and
 * `create` answer, its words and its image `set_text`'s and `set_image`'s;
 * `set_docked` keeps the app in the Dock or out of it. `hero_ui.c` defines
 * `set_docked` on macOS, and for the Win32 and GTK 4 arms as a refusal in
 * CALLS NO C ARM DRAWS YET until an arm defines its own. */
int64_t hero_ui_arm_set_docked(int64_t on);

/* A NOTIFICATION (`hero_ui.h`), and the words of the last refusal of a
 * call on the system around the program. `hero_ui.c` defines `notify` on
 * macOS by `hero_ui_appkit_system.swift`, and for the Win32 and GTK 4 arms
 * as a refusal in CALLS NO C ARM DRAWS YET until an arm defines its own;
 * `system_words` is the shared words on those two arms. */
int64_t hero_ui_arm_notify(const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count);
int64_t hero_ui_arm_system_words(char *words, int64_t count);

/* THE DARK OR LIGHT APPEARANCE (`hero_ui.h`): asked, and turned as the
 * system would turn it for `hero_ui_inject`; on macOS the arm watches it
 * from `open` to `close`, queuing `HERO_UI_EVENT_APPEARANCE`. Refusals for
 * the Win32 and GTK 4 arms in `hero_ui.c`'s CALLS NO C ARM DRAWS YET. */
int64_t hero_ui_arm_appearance(int64_t *appearance);
int64_t hero_ui_arm_appearance_set(int64_t appearance);

/* THE SESSION'S END ASKING THE PROGRAM (`hero_ui.h`): on macOS the arm
 * holds each ask from `open` to `close`, queuing `HERO_UI_EVENT_QUIT` and
 * answering it once the program does, or as ending at `close`;
 * `answer_quit` answers every ask held, `ask_quit` asks as the system would
 * for `hero_ui_inject`. Refusals for the Win32 and GTK 4 arms in
 * `hero_ui.c`'s CALLS NO C ARM DRAWS YET. */
int64_t hero_ui_arm_answer_quit(int64_t go);
int64_t hero_ui_arm_ask_quit(int64_t session_end);

/* FILES DROPPED ON A WINDOW (`hero_ui.h`): `set_drop` has window `id` take
 * them or not; `drop` drops `count` bytes of paths, each followed by one zero
 * byte, at `x`, `y` of its content as the person would, for
 * `hero_ui_inject`. On macOS the arm queues `HERO_UI_EVENT_DROP` with the
 * paths; refusals for the Win32 and GTK 4 arms in `hero_ui.c`'s CALLS NO C
 * ARM DRAWS YET. */
int64_t hero_ui_arm_set_drop(int64_t id, int64_t on);
int64_t hero_ui_arm_drop(int64_t id, int64_t x, int64_t y, const uint8_t *paths, int64_t count);

/* ACCESSIBLE NAMES (`hero_ui.h`): defined on macOS by `hero_ui.c` handing
 * them to `hero_ui_appkit.swift`, refusals for the Win32 and GTK 4 arms in
 * CALLS NO C ARM DRAWS YET. `accessible_name` writes the name where it fits
 * and its length through `length`, as `text` does. */
int64_t hero_ui_arm_set_accessible_name(int64_t id, const uint8_t *name, int64_t count);
int64_t hero_ui_arm_accessible_name(int64_t id, uint8_t *buffer, int64_t count, int64_t *length);
int64_t hero_ui_arm_set_label_for(int64_t id, int64_t label);

/* THE WAITING FORM of the dialogs, which the Win32 arm defines and
 * `hero_ui.c` wraps for it: each shows its dialog and returns once the
 * person has answered, writing the answer through its last parameter (the
 * button, `HERO_UI_ANSWER_*`, or the index; the bytes of the paths chosen,
 * each followed by one zero byte, 0 where the person cancelled, which
 * `dialog_paths` then writes). macOS defines none of them. */
int64_t hero_ui_arm_message(int64_t window, int64_t tone, int64_t buttons, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count, int64_t *answer);
int64_t hero_ui_arm_file_dialog(int64_t window, int64_t save, int64_t multiple, const uint8_t *title, int64_t title_count, const uint8_t *name, int64_t name_count, const uint8_t *filters, int64_t filters_count, int64_t *bytes);
int64_t hero_ui_arm_dialog_paths(uint8_t *buffer, int64_t count);
int64_t hero_ui_arm_folder_dialog(int64_t multiple, const uint8_t *title, int64_t title_count, int64_t *bytes);
int64_t hero_ui_arm_ask(int64_t tone, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count, const uint8_t *buttons, int64_t buttons_count, int64_t cancel, int64_t *pressed);

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
