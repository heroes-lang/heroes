/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui.c — the platform layer's native user interface,
 * `hero_ui.h`'s functions (M-rebel-library, 2026-11-22).
 *
 * WHAT THIS FILE HOLDS, AND WHAT EACH SYSTEM'S ARM HOLDS. This file is the
 * same on every system: the thread rule, the table of apps and their
 * handles, the queue of events, and the check of every argument before a
 * system sees it. What draws a widget is the ARM, a set of functions named
 * `hero_ui_arm_*` (THE ARM, below), which this file calls only on the UI
 * thread, for the one native app, with every argument already checked, and
 * which answer a kind, `HERO_UI_OK` where done. The macOS arm calls the
 * functions `platform/hero_ui_appkit.swift` exports to C (AppKit is
 * Swift's and Objective-C's, never C's) and queues what the person does by
 * the function this file hands it at `open`, `hero_ui_push`. The Windows arm
 * (Win32's own controls) is C in this file, below THE ARM, and calls
 * `hero_ui_push` itself; the arm of every other system answers
 * `HERO_UI_UNSUPPORTED`.
 *
 * THE THREAD. Every entry point but `on_ui_thread` and `native_here` asks
 * first whether it runs on the UI thread, before it reads the table, so a
 * call from another thread touches nothing and answers
 * `HERO_UI_WRONG_THREAD`. On macOS the UI thread is the main thread
 * (`pthread_main_np`), on Linux the thread whose id is the process's
 * (`gettid() == getpid()`), on Windows the thread that opened the first app
 * (Windows names no main thread). So the table, the queues and the last
 * failure are each the UI thread's own, `_Thread_local`, and only the count
 * of native apps, the numbers given and Windows' owner are shared, each
 * `_Atomic`.
 *
 * THE HANDLES. An app is a handle the layer issues and checks, as
 * `hero_net.c`'s socket is: this table's tag, a slot and the life the slot
 * held when it was issued, never an address, so a closed app, or one of
 * another table, is `HERO_UI_NOT_OPEN` at any slot and any life.
 *
 * MEMORY: a queue's ring is taken from `hero_layer_alloc` at its first
 * event and given back at `close`; an event's text likewise, given back
 * when the next event is taken. A system that has no memory for them drops
 * the event and counts it lost, never aborts.
 *
 * Written from the platforms' documentation: Apple's AppKit reference and
 * Microsoft's Win32 reference; no other library's source.
 */

#include "hero_ui.h"

#include "hero_handle_tag.h"
#include "hero_layer_alloc.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <cderr.h>
#include <wchar.h>
#elif defined(__APPLE__)
#include <pthread.h>
#include "hero_ui_appkit.h"
#else
#include <dlfcn.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>
#endif

/* -- THE LAST FAILURE, PER THREAD ------------------------------------------ */

/* The words kept of a failure, with their zero byte. */
#define HERO_UI_WORDS_MOST 512

static _Thread_local int64_t hero_ui_kind;
static _Thread_local char hero_ui_words[HERO_UI_WORDS_MOST];

/* `text` copied into `into`, `most` bytes with the zero, cut where a
 * character ends; answers the length copied. */
static size_t hero_ui_copy_text(char *into, size_t most, const char *text) {
    size_t length = 0;

    if (most == 0) {
        return 0;
    }
    while (text != NULL && text[length] != '\0' && length < most - 1) {
        length += 1;
    }
    if (text != NULL && text[length] != '\0') {
        /* `text[length]` is the first byte left out: while it continues a
         * character, the character it belongs to is left out whole. */
        while (length > 0 && ((unsigned char)text[length] & 0xC0u) == 0x80u) {
            length -= 1;
        }
    }
    if (length > 0) {
        memcpy(into, text, length);
    }
    into[length] = '\0';
    return length;
}

/* The failure this call answers, its kind and its words; answers -1, what
 * the call answers. */
static int64_t hero_ui_said(int64_t kind, const char *words) {
    hero_ui_kind = kind;
    (void)hero_ui_copy_text(hero_ui_words, sizeof hero_ui_words, words);
    return -1;
}

static void hero_ui_fine(void) {
    hero_ui_kind = HERO_UI_OK;
    hero_ui_words[0] = '\0';
}

int64_t hero_ui_last_error(void) {
    return hero_ui_kind;
}

int64_t hero_ui_last_message(void *buffer, uint64_t count) {
    size_t length = strlen(hero_ui_words);

    if (buffer == NULL || count == 0) {
        return hero_ui_said(HERO_UI_INVALID, "no room was given for the message");
    }
    if ((uint64_t)length > count) {
        length = (size_t)count;
        while (length > 0 && ((unsigned char)hero_ui_words[length] & 0xC0u) == 0x80u) {
            length -= 1;
        }
    }
    if (length > 0) {
        memcpy(buffer, hero_ui_words, length);
    }
    return (int64_t)length;
}

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
 * group's item. */

#define HERO_UI_FLAG_ENABLED 1
#define HERO_UI_FLAG_VISIBLE 2
#define HERO_UI_FLAG_RESIZABLE 3
#define HERO_UI_SIZE_CONTENT 1
#define HERO_UI_SIZE_LEAST 2

static void hero_ui_push(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count);
static bool hero_ui_is_utf8(const uint8_t *text, uint64_t length);

#if defined(__APPLE__)

static int64_t hero_ui_arm_here(void) {
    return hero_ui_appkit_here();
}

/* Why `here` answered 0, into `words` of `count` bytes. */
static void hero_ui_arm_absent(char *words, size_t count) {
    (void)hero_ui_copy_text(words, count, "no window server serves this session, as over a remote shell");
}

static int64_t hero_ui_arm_open(void) {
    return hero_ui_appkit_open(hero_ui_push);
}

static void hero_ui_arm_close(void) {
    hero_ui_appkit_close();
}

static int64_t hero_ui_arm_words(char *words, int64_t count) {
    return hero_ui_appkit_words(words, count);
}

static int64_t hero_ui_arm_create(int64_t id, int64_t kind, int64_t parent) {
    return hero_ui_appkit_create(id, kind, parent);
}

static int64_t hero_ui_arm_destroy(int64_t id) {
    return hero_ui_appkit_destroy(id);
}

static int64_t hero_ui_arm_set_text(int64_t id, const uint8_t *text, int64_t count) {
    return hero_ui_appkit_set_text(id, text, count);
}

static int64_t hero_ui_arm_set_items(int64_t id, const uint8_t *items, int64_t count) {
    return hero_ui_appkit_set_items(id, items, count);
}

static int64_t hero_ui_arm_set_value(int64_t id, int64_t value) {
    return hero_ui_appkit_set_value(id, value);
}

static int64_t hero_ui_arm_set_range(int64_t id, int64_t least, int64_t most) {
    return hero_ui_appkit_set_range(id, least, most);
}

static int64_t hero_ui_arm_set_selection(int64_t id, const int64_t *indices, int64_t count) {
    return hero_ui_appkit_set_selection(id, indices, count);
}

static int64_t hero_ui_arm_set_flag(int64_t id, int64_t which, int64_t on) {
    return hero_ui_appkit_set_flag(id, which, on);
}

static int64_t hero_ui_arm_set_shortcut(int64_t id, const uint8_t *key, int64_t count, int64_t modifiers) {
    return hero_ui_appkit_set_shortcut(id, key, count, modifiers);
}

static int64_t hero_ui_arm_set_image(int64_t id, const uint8_t *rgba, int64_t width, int64_t height) {
    return hero_ui_appkit_set_image(id, rgba, width, height);
}

static int64_t hero_ui_arm_set_size(int64_t id, int64_t which, int64_t width, int64_t height) {
    return hero_ui_appkit_set_size(id, which, width, height);
}

static int64_t hero_ui_arm_place(int64_t id, int64_t index, int64_t x, int64_t y, int64_t width, int64_t height) {
    return hero_ui_appkit_place(id, index, x, y, width, height);
}

static int64_t hero_ui_arm_preferred(int64_t id, int64_t *width, int64_t *height) {
    return hero_ui_appkit_preferred(id, width, height);
}

static int64_t hero_ui_arm_text(int64_t id, uint8_t *buffer, int64_t count, int64_t *length) {
    return hero_ui_appkit_text(id, buffer, count, length);
}

static int64_t hero_ui_arm_value(int64_t id, int64_t *value) {
    return hero_ui_appkit_value(id, value);
}

static int64_t hero_ui_arm_selection(int64_t id, int64_t *indices, int64_t count, int64_t *chosen) {
    return hero_ui_appkit_selection(id, indices, count, chosen);
}

static int64_t hero_ui_arm_pump(int64_t ms) {
    return hero_ui_appkit_pump(ms);
}

static int64_t hero_ui_arm_inject(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count) {
    return hero_ui_appkit_inject(kind, id, value, second, text, count);
}

static int64_t hero_ui_arm_message(int64_t window, int64_t tone, int64_t buttons, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count, int64_t *answer) {
    return hero_ui_appkit_message(window, tone, buttons, title, title_count, text, text_count, answer);
}

static int64_t hero_ui_arm_file_dialog(int64_t window, int64_t save, int64_t multiple, const uint8_t *title, int64_t title_count, const uint8_t *name, int64_t name_count, const uint8_t *filters, int64_t filters_count, int64_t *bytes) {
    return hero_ui_appkit_file_dialog(window, save, multiple, title, title_count, name, name_count, filters, filters_count, bytes);
}

static int64_t hero_ui_arm_dialog_paths(uint8_t *buffer, int64_t count) {
    return hero_ui_appkit_dialog_paths(buffer, count);
}

#elif defined(_WIN32)

/* -- WHAT THE WIN32 ARM KEEPS ------------------------------------------------
 *
 * The arm is C, in this file, so it calls `hero_ui_push` itself through
 * `hero_ui_tell`, and keeps its widgets in one table of its own,
 * a map from the program's numbers to the arm's record of each widget. Every
 * object below is the UI thread's own, `_Thread_local`, as the table of apps
 * is (THE THREAD). */

/* An arm's record of one widget: its number, its kind, its parent, and the
 * system's objects; each arm defines it. */
typedef struct hero_ui_native hero_ui_native;

/* The words of the arm's last refusal, which `hero_ui_arm_words` gives. */
static _Thread_local char hero_ui_native_words[HERO_UI_WORDS_MOST];

/* While above 0 the program is setting a widget, and what the system tells
 * of it queues nothing (`set_*` queues no event). */
static _Thread_local int64_t hero_ui_muted;

/* How many events the arm has queued, read by `pump` to stop at the first. */
static _Thread_local uint64_t hero_ui_pushed;

/* Answers `kind`, keeping `words` for `hero_ui_arm_words`: every refusal of
 * `HERO_UI_UNSUPPORTED` or `HERO_UI_OTHER` goes through here, so the words
 * read after one are its own. */
static int64_t hero_ui_refused(int64_t kind, const char *words) {
    (void)hero_ui_copy_text(hero_ui_native_words, sizeof hero_ui_native_words, words);
    return kind;
}

static int64_t hero_ui_arm_words(char *words, int64_t count) {
    if (words == NULL || count <= 0) {
        return 0;
    }
    return (int64_t)hero_ui_copy_text(words, (size_t)count, hero_ui_native_words);
}

/* Queues what the person did, unless the program is setting it. */
static void hero_ui_tell(int64_t kind, int64_t id, int64_t value, int64_t second) {
    if (hero_ui_muted > 0) {
        return;
    }
    hero_ui_pushed += 1;
    hero_ui_push(kind, id, value, second, NULL, 0);
}

/* THE MAP: open addressing over a power of two of places, probed in order
 * from the number's hash. A place given back is marked `gone`, so a probe
 * walks past it; the places are made anew, the gone ones dropped, when the
 * live and the gone fill half of them. */
typedef struct hero_ui_entry {
    int64_t id;
    hero_ui_native *widget;
    bool gone;
} hero_ui_entry;

static _Thread_local hero_ui_entry *hero_ui_entries;
static _Thread_local size_t hero_ui_room;
static _Thread_local size_t hero_ui_filled;

static size_t hero_ui_first_place(int64_t id, size_t room) {
    uint64_t mixed = (uint64_t)id * UINT64_C(0x9E3779B97F4A7C15);

    return (size_t)(mixed >> 32) & (room - 1);
}

/* The widget of number `id`, or NULL. */
static hero_ui_native *hero_ui_find(int64_t id) {
    size_t at;

    if (hero_ui_room == 0) {
        return NULL;
    }
    at = hero_ui_first_place(id, hero_ui_room);
    for (size_t probes = 0; probes < hero_ui_room; probes += 1) {
        hero_ui_entry *entry = &hero_ui_entries[at];

        if (entry->widget == NULL && !entry->gone) {
            return NULL;
        }
        if (entry->widget != NULL && entry->id == id) {
            return entry->widget;
        }
        at = (at + 1) & (hero_ui_room - 1);
    }
    return NULL;
}

/* Puts `widget` at `id` in `entries` of `room` places, which hold a free
 * one; answers whether the place was never used. */
static bool hero_ui_place_entry(hero_ui_entry *entries, size_t room, int64_t id, hero_ui_native *widget) {
    size_t at = hero_ui_first_place(id, room);

    while (entries[at].widget != NULL) {
        at = (at + 1) & (room - 1);
    }
    entries[at].id = id;
    entries[at].widget = widget;
    if (entries[at].gone) {
        entries[at].gone = false;
        return false;
    }
    return true;
}

/* Keeps `widget` under `id`, which the map does not hold; false where the
 * system had no memory for a bigger map. */
static bool hero_ui_keep(int64_t id, hero_ui_native *widget) {
    if ((hero_ui_filled + 1) * 2 > hero_ui_room) {
        size_t live = 0;
        size_t room = 64;
        hero_ui_entry *grown;

        for (size_t i = 0; i < hero_ui_room; i += 1) {
            live += hero_ui_entries[i].widget != NULL ? 1u : 0u;
        }
        while (room < (live + 1) * 4) {
            room *= 2;
        }
        grown = hero_layer_alloc_zero(room, sizeof *grown);
        if (grown == NULL) {
            return false;
        }
        for (size_t i = 0; i < hero_ui_room; i += 1) {
            if (hero_ui_entries[i].widget != NULL) {
                (void)hero_ui_place_entry(grown, room, hero_ui_entries[i].id, hero_ui_entries[i].widget);
            }
        }
        hero_layer_free(hero_ui_entries);
        hero_ui_entries = grown;
        hero_ui_room = room;
        hero_ui_filled = live;
    }
    if (hero_ui_place_entry(hero_ui_entries, hero_ui_room, id, widget)) {
        hero_ui_filled += 1;
    }
    return true;
}

/* Gives back the place of `id`. */
static void hero_ui_forget(int64_t id) {
    size_t at;

    if (hero_ui_room == 0) {
        return;
    }
    at = hero_ui_first_place(id, hero_ui_room);
    for (size_t probes = 0; probes < hero_ui_room; probes += 1) {
        hero_ui_entry *entry = &hero_ui_entries[at];

        if (entry->widget == NULL && !entry->gone) {
            return;
        }
        if (entry->widget != NULL && entry->id == id) {
            entry->widget = NULL;
            entry->gone = true;
            return;
        }
        at = (at + 1) & (hero_ui_room - 1);
    }
}

/* The widget at the map's place `at`, NULL where none stands: how the arms
 * walk every widget. Forgetting a widget while walking leaves every other
 * place where it was. */
static hero_ui_native *hero_ui_at(size_t at) {
    return at < hero_ui_room ? hero_ui_entries[at].widget : NULL;
}

/* Gives back the map, every widget forgotten first. */
static void hero_ui_free_map(void) {
    hero_layer_free(hero_ui_entries);
    hero_ui_entries = NULL;
    hero_ui_room = 0;
    hero_ui_filled = 0;
}

/* Numbers in an order: a window's menus, a menu's items. */
typedef struct hero_ui_ids {
    int64_t *at;
    int64_t count;
    int64_t room;
} hero_ui_ids;

/* Adds `id` at the end; false where the system had no memory for it. */
static bool hero_ui_ids_add(hero_ui_ids *ids, int64_t id) {
    if (ids->count == ids->room) {
        int64_t room = ids->room == 0 ? 8 : ids->room * 2;
        int64_t *grown = hero_layer_grow(ids->at, (size_t)room * sizeof *grown);

        if (grown == NULL) {
            return false;
        }
        ids->at = grown;
        ids->room = room;
    }
    ids->at[ids->count] = id;
    ids->count += 1;
    return true;
}

/* Takes `id` out, the others keeping their order. */
static void hero_ui_ids_drop(hero_ui_ids *ids, int64_t id) {
    for (int64_t i = 0; i < ids->count; i += 1) {
        if (ids->at[i] == id) {
            for (int64_t j = i + 1; j < ids->count; j += 1) {
                ids->at[j - 1] = ids->at[j];
            }
            ids->count -= 1;
            return;
        }
    }
}

static void hero_ui_ids_free(hero_ui_ids *ids) {
    hero_layer_free(ids->at);
    ids->at = NULL;
    ids->count = 0;
    ids->room = 0;
}

/* How many texts `count` bytes of a list hold, each followed by one zero
 * byte (`hero_ui.c` checked the last byte is one). */
static int64_t hero_ui_texts_in(const uint8_t *items, int64_t count) {
    int64_t texts = 0;

    for (int64_t at = 0; at < count; at += 1) {
        texts += items[at] == 0 ? 1 : 0;
    }
    return texts;
}

/* The `index`th text of a list as its first byte and its length. */
static const uint8_t *hero_ui_text_at(const uint8_t *items, int64_t count, int64_t index, int64_t *length) {
    int64_t start = 0;
    int64_t seen = 0;

    for (int64_t at = 0; at < count; at += 1) {
        if (items[at] == 0) {
            if (seen == index) {
                *length = at - start;
                return items + start;
            }
            seen += 1;
            start = at + 1;
        }
    }
    *length = 0;
    return items;
}

/* `value` held between `least` and `most`. */
static int64_t hero_ui_between(int64_t value, int64_t least, int64_t most) {
    return value < least ? least : (value > most ? most : value);
}

/* Where `value` stands between `least` and `most`, from 0 to 1; the range is
 * checked `least < most` where it was set. */
static double hero_ui_fraction(int64_t value, int64_t least, int64_t most) {
    double span = (double)most - (double)least;

    return span <= 0 ? 0 : ((double)value - (double)least) / span;
}

/* The value standing at `fraction` of the way from `least` to `most`,
 * rounded, and held inside them. */
static int64_t hero_ui_at_fraction(double fraction, int64_t least, int64_t most) {
    double exact = (double)least + fraction * ((double)most - (double)least);

    /* Each bound is a double before it is compared, so no conversion back to
     * an integer meets a value past the int64 range. */
    if (exact <= (double)least || exact <= -9.2233720368547758e18) {
        return least;
    }
    if (exact >= (double)most || exact >= 9.2233720368547758e18) {
        return most;
    }
    if (exact >= 0) {
        return hero_ui_between((int64_t)(exact + 0.5), least, most);
    }
    return hero_ui_between(-(int64_t)(-exact + 0.5), least, most);
}

/* ==========================================================================
 * THE WIN32 ARM: the system's own controls, `BUTTON`, `EDIT`, `STATIC`,
 * `LISTBOX`, `COMBOBOX` and comctl32's trackbar and progress bar, children of
 * top-level windows of this arm's own class, `HeroesUiWindow`, whose client
 * area is the window's content.
 *
 * EVERY CALL IS ON THE UI THREAD, the thread that opened the process's first
 * app (THE THREAD): Win32 delivers a window's messages to the thread that
 * made it, so every window here is that thread's and `pump` runs its queue.
 *
 * POINTS ARE MAPPED TO PIXELS PER MONITOR. The UI thread is made aware of
 * each monitor's scale while an app is open (`SetThreadDpiAwarenessContext`,
 * per-monitor version 2 where Windows has it), so each window is drawn at its
 * monitor's own scale: a window keeps its scale, `dpi`, and every place,
 * size and font of it is its points times `dpi / 96`, done again when the
 * window moves to a monitor of another scale (`WM_DPICHANGED`). The scaling
 * functions came with Windows 10 1607 and 1703, so each is found by
 * `GetProcAddress` and an older Windows draws at the system's one scale.
 *
 * THE SYSTEM'S CURRENT LOOK: a program's controls take version 6 of the
 * common controls, the look of the system's own programs, only through an
 * activation context asking for it, which the program's executable carries
 * none of. So `open` makes one from a manifest of a few lines written to a
 * file of the system's temporary folder and removed once read, and keeps it
 * active on the UI thread while the app is open; where no such file can be
 * made the controls are drawn in the classic look and work the same.
 *
 * TEXT crosses into UTF-16 at this boundary (`MultiByteToWideChar` of the
 * UTF-8 `hero_ui.c` checked) and back (`WideCharToMultiByte`, an unpaired
 * surrogate a person typed read as U+FFFD). A multi-line edit control and a
 * label hold lines ended by CR LF, so a line feed is written as CR LF and CR
 * LF read as a line feed; a button's, a checkbox's, a radio item's and a
 * menu's `&` is written `&&`, which Win32 draws as one `&` and not as the
 * underline of a key.
 *
 * WHAT THE PERSON DOES arrives as the notifications each control sends its
 * parent (`BN_CLICKED`, `EN_CHANGE`, `LBN_SELCHANGE`, `CBN_SELCHANGE`, a
 * trackbar's `WM_HSCROLL`), a menu's or a shortcut's `WM_COMMAND`, the close
 * button's `WM_CLOSE` and a new size's `WM_SIZE`; what the program sets is
 * muted where Win32 tells it too (an edit control's `EN_CHANGE` after
 * `SetWindowTextW`). A control's own number (`GWLP_ID`) is 0 and its
 * `GWLP_USERDATA` the widget's number, so a notification finds its widget
 * through the table and a stale one finds none: `WM_COMMAND` carries a
 * 16-bit number, which no 64-bit widget number fits.
 *
 * WHAT `inject` DOES is the control's own path: `BM_CLICK` presses a button,
 * a checkbox or a radio item, and where Win32 dropped it (its documentation
 * says a click of an inactive window's control may fail), the `BN_CLICKED`
 * the control sends is sent for it, so an injected act always makes its one
 * event; an edit is `EM_REPLACESEL` over the whole text, undoable as typing
 * is; a list's, a combo box's and a slider's choice is set and the
 * notification the control sends for a person's choice sent to its parent;
 * a menu item is the `WM_COMMAND` its menu sends; the close button is
 * `SC_CLOSE`.
 *
 * MENUS AND SHORTCUTS: a window's menu bar holds its menus' pop-ups, each
 * filled again, in the order made, when one of its items changes; Win32 has
 * no hidden menu item, so a hidden one is left out. A menu item is known to
 * Win32 by a 16-bit command number this arm gives, from 1000 up, past the
 * dialog manager's `IDOK` to `IDCONTINUE`; its shortcut is an entry of the
 * window's accelerator table, made again when a shortcut changes and handed
 * to `TranslateAcceleratorW` by `pump`, and shown after a tab in the item's
 * text by the keys' own names (`GetKeyNameTextW`). `pump` hands each message
 * to `IsDialogMessageW` too, so Tab and the arrows move between controls as
 * in a dialog.
 *
 * THE LOOP: `pump` takes the thread's messages until one queues an event or
 * its time passes, waiting in `MsgWaitForMultipleObjectsEx`. While the person
 * drags a window's border or holds a menu open, Windows runs a loop of its
 * own inside `DispatchMessageW`, and `pump` returns when it ends.
 *
 * DIALOGS: `MessageBoxW`, and `GetOpenFileNameW` and `GetSaveFileNameW`,
 * which show the system's current file dialog where no hook or template is
 * asked, need no COM apartment of the program's thread, and are told
 * `OFN_NOCHANGEDIR`, so the process's folder stays where it was.
 *
 * THE LIBRARIES ARE NAMED BY THE MEMBER, as `hero_net.c` names `ws2_32`:
 * `user32`, `gdi32`, `comctl32`, `comdlg32` and `msimg32` (`AlphaBlend`, a
 * picture's pixels drawn over the window's colour by their alpha), by
 * `#pragma comment(lib)`, so only a program that takes this member links them.
 *
 * Written from Microsoft's Win32 documentation; no other library's source.
 * ========================================================================== */

#if defined(_MSC_VER)
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "msimg32.lib")
#endif

_Static_assert(sizeof(LONG_PTR) == sizeof(int64_t), "a control's GWLP_USERDATA holds a widget's 64-bit number");

/* Win32's numbers this arm reads that an older SDK does not name. */
#define HERO_UI_WIN_DPICHANGED 0x02E0u
#define HERO_UI_WIN_AWARE_V2 ((HANDLE)(intptr_t)-4)
#define HERO_UI_WIN_AWARE ((HANDLE)(intptr_t)-3)
/* The first command number of a menu item, past `IDOK` to `IDCONTINUE`,
 * which the dialog manager sends as `WM_COMMAND` of its own. */
#define HERO_UI_WIN_FIRST_COMMAND 1000u
#define HERO_UI_WIN_LAST_COMMAND 0xEFFFu
/* The farthest a place or a size reaches, in points, so its pixels fit an
 * `int` at every scale Windows offers (500 %). */
#define HERO_UI_WIN_POINTS_MOST (INT64_C(1) << 24)
/* A slider's positions where its range is wider than a trackbar's 32 bits. */
#define HERO_UI_WIN_STEPS 1000000
/* The room a file dialog's answer has, in UTF-16 units. */
#define HERO_UI_WIN_PATHS_ROOM (1u << 20)

static const wchar_t hero_ui_win_window_class[] = L"HeroesUiWindow";
static const wchar_t hero_ui_win_panel_class[] = L"HeroesUiPanel";
static const wchar_t hero_ui_win_picture_class[] = L"HeroesUiPicture";

/* The manifest of the activation context (THE SYSTEM'S CURRENT LOOK). */
static const char hero_ui_win_manifest[] = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n"
                                           "<assembly xmlns=\"urn:schemas-microsoft-com:asm.v1\" manifestVersion=\"1.0\">\r\n"
                                           "<dependency><dependentAssembly><assemblyIdentity type=\"win32\" "
                                           "name=\"Microsoft.Windows.Common-Controls\" version=\"6.0.0.0\" processorArchitecture=\"*\" "
                                           "publicKeyToken=\"6595b64144ccf1df\" language=\"*\"/></dependentAssembly></dependency>\r\n"
                                           "</assembly>\r\n";

struct hero_ui_native {
    int64_t id;
    int64_t kind;
    int64_t parent;
    /* The window it stands in; a window's own number, a menu item's its
     * menu's window. */
    int64_t window;
    /* The window, the control, a radio group's panel, a picture's view. */
    HWND hwnd;
    /* Shown as the program asks; a placed widget shows once placed too. */
    bool visible;
    /* A menu's, an item's and a radio group's, kept to fill a menu again
     * or to make the group's items alike. */
    bool enabled;
    bool placed;
    /* Where the program placed it, in points. */
    int64_t x;
    int64_t y;
    int64_t width;
    int64_t height;
    /* A radio group's items and where each was placed, four numbers an item,
     * a width below 0 where it was not. */
    HWND *radios;
    int64_t *radio_places;
    int64_t radio_count;
    /* A list's or a combo box's items. */
    int64_t items;
    /* A slider's or a progress bar's range and value, exact. */
    int64_t least;
    int64_t most;
    int64_t value;
    /* A window's menu bar, a menu's pop-up. */
    HMENU menu;
    /* A window's menus, a menu's items and separators, in the order made. */
    hero_ui_ids order;
    /* A menu's or an item's text, kept to fill its menu again. */
    wchar_t *label;
    /* An item's or a separator's command number. */
    WORD command;
    /* An item's shortcut: its virtual key, 0 for none, the keys held
     * (`FCONTROL`, `FSHIFT`, `FALT`) and whether the key is an extended one. */
    WORD key;
    BYTE held;
    bool key_extended;
    /* A window's scale, font, content size and least size in points, and its
     * accelerator table, made again where `accelerators_stale`. */
    UINT dpi;
    HFONT font;
    int64_t content_width;
    int64_t content_height;
    int64_t least_width;
    int64_t least_height;
    HACCEL accelerators;
    bool accelerators_stale;
    /* A picture's pixels, premultiplied as `AlphaBlend` takes them. */
    HBITMAP bitmap;
    int64_t bitmap_width;
    int64_t bitmap_height;
};

/* The scaling functions of `user32` (POINTS ARE MAPPED), NULL where this
 * Windows has none. */
typedef UINT(WINAPI *hero_ui_win_dpi_of_window)(HWND);
typedef BOOL(WINAPI *hero_ui_win_adjust_for_dpi)(LPRECT, DWORD, BOOL, DWORD, UINT);
typedef BOOL(WINAPI *hero_ui_win_parameters_for_dpi)(UINT, UINT, PVOID, UINT, UINT);
typedef int(WINAPI *hero_ui_win_metrics_for_dpi)(int, UINT);
typedef HANDLE(WINAPI *hero_ui_win_thread_awareness)(HANDLE);

static _Thread_local hero_ui_win_dpi_of_window hero_ui_win_dpi_for_window;
static _Thread_local hero_ui_win_adjust_for_dpi hero_ui_win_adjust;
static _Thread_local hero_ui_win_parameters_for_dpi hero_ui_win_parameters;
static _Thread_local hero_ui_win_metrics_for_dpi hero_ui_win_metrics;
static _Thread_local hero_ui_win_thread_awareness hero_ui_win_awareness;

/* Whether the classes are registered and the functions found, which `open`
 * does once for the process's life. */
static _Thread_local bool hero_ui_win_ready;
/* The activation context, NULL before it was asked for and
 * `INVALID_HANDLE_VALUE` where none could be made; its cookie while active. */
static _Thread_local HANDLE hero_ui_win_context;
static _Thread_local ULONG_PTR hero_ui_win_cookie;
static _Thread_local bool hero_ui_win_activated;
/* The thread's awareness of scale before `open`, given back at `close`. */
static _Thread_local HANDLE hero_ui_win_awareness_before;
static _Thread_local bool hero_ui_win_aware;
/* The next command number a menu item may take. */
static _Thread_local UINT hero_ui_win_next_command;
/* Above 0 while `inject` acts, so a slider moved to where it stands is told. */
static _Thread_local int64_t hero_ui_win_injecting;
/* The paths the last file dialog answered, each followed by one zero byte. */
static _Thread_local uint8_t *hero_ui_win_paths;
static _Thread_local int64_t hero_ui_win_paths_count;

/* -- Words, numbers and text ------------------------------------------------ */

/* Answers `HERO_UI_OTHER` with `what` and the system's words for `code`. */
static int64_t hero_ui_win_failed(const char *what, DWORD code) {
    char words[HERO_UI_WORDS_MOST];
    wchar_t theirs[256];
    char narrow[HERO_UI_WORDS_MOST];
    DWORD length = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, code, 0, theirs, (DWORD)(sizeof theirs / sizeof theirs[0]), NULL);
    int made;

    while (length > 0 && (theirs[length - 1] == L'\r' || theirs[length - 1] == L'\n' || theirs[length - 1] == L' ' || theirs[length - 1] == L'.')) {
        length -= 1;
    }
    made = length == 0 ? 0 : WideCharToMultiByte(CP_UTF8, 0, theirs, (int)length, narrow, (int)sizeof narrow - 1, NULL, NULL);
    narrow[made > 0 ? made : 0] = '\0';
    (void)snprintf(words, sizeof words, "%s: Windows error %lu%s%s", what, (unsigned long)code, made > 0 ? ", " : "", narrow);
    return hero_ui_refused(HERO_UI_OTHER, words);
}

/* `points` as pixels at `dpi`, rounded to the nearest. */
static int hero_ui_win_px(int64_t points, UINT dpi) {
    int64_t bounded = hero_ui_between(points, -HERO_UI_WIN_POINTS_MOST, HERO_UI_WIN_POINTS_MOST);

    return MulDiv((int)bounded, (int)dpi, 96);
}

/* `pixels` as points at `dpi`, rounded to the nearest. */
static int64_t hero_ui_win_pt(int pixels, UINT dpi) {
    return dpi == 0 ? pixels : (int64_t)MulDiv(pixels, 96, (int)dpi);
}

/* `pixels` as points at `dpi`, rounded up, so a size measured is never cut. */
static int64_t hero_ui_win_pt_up(int pixels, UINT dpi) {
    int64_t scaled = (int64_t)(pixels < 0 ? 0 : pixels) * 96;

    return dpi == 0 ? pixels : (scaled + (int64_t)dpi - 1) / (int64_t)dpi;
}

/* What `hero_ui_win_wide` writes besides the UTF-16 of the text. */
#define HERO_UI_WIN_LINES 1u
#define HERO_UI_WIN_PREFIX 2u
#define HERO_UI_WIN_TABS 4u

/* `count` bytes of checked UTF-8 at `text` as UTF-16 with its zero, in a
 * block of `hero_layer_alloc`, or NULL where memory ran out: a line feed as
 * CR LF where `HERO_UI_WIN_LINES`, an `&` as `&&` where `HERO_UI_WIN_PREFIX`,
 * a tab as a space where `HERO_UI_WIN_TABS`. UTF-8 takes at least as many
 * bytes as UTF-16 takes units, so `count` units hold the text. */
static wchar_t *hero_ui_win_wide(const uint8_t *text, int64_t count, unsigned ways) {
    wchar_t *plain = hero_layer_alloc(((size_t)count + 1) * sizeof(wchar_t));
    wchar_t *written;
    int made = 0;
    size_t more = 0;
    size_t at = 0;

    if (plain == NULL) {
        return NULL;
    }
    if (count > 0) {
        made = MultiByteToWideChar(CP_UTF8, 0, (const char *)text, (int)count, plain, (int)count);
        if (made <= 0) {
            hero_layer_free(plain);
            return NULL;
        }
    }
    plain[made] = 0;
    for (int i = 0; i < made; i += 1) {
        more += ((ways & HERO_UI_WIN_LINES) != 0 && plain[i] == L'\n') || ((ways & HERO_UI_WIN_PREFIX) != 0 && plain[i] == L'&') ? 1u : 0u;
    }
    if (more == 0 && (ways & HERO_UI_WIN_TABS) == 0) {
        return plain;
    }
    written = hero_layer_alloc(((size_t)made + more + 1) * sizeof(wchar_t));
    if (written == NULL) {
        hero_layer_free(plain);
        return NULL;
    }
    for (int i = 0; i < made; i += 1) {
        if ((ways & HERO_UI_WIN_LINES) != 0 && plain[i] == L'\n') {
            written[at++] = L'\r';
        } else if ((ways & HERO_UI_WIN_PREFIX) != 0 && plain[i] == L'&') {
            written[at++] = L'&';
        }
        written[at++] = (ways & HERO_UI_WIN_TABS) != 0 && plain[i] == L'\t' ? L' ' : plain[i];
    }
    written[at] = 0;
    hero_layer_free(plain);
    return written;
}

/* The text of control `hwnd` as UTF-8 in a block of `hero_layer_alloc`, its
 * length through `*bytes`, CR LF read as a line feed where `lines`; NULL
 * where memory ran out. */
static uint8_t *hero_ui_win_text_of(HWND hwnd, bool lines, int64_t *bytes) {
    int units = GetWindowTextLengthW(hwnd);
    wchar_t *wide;
    uint8_t *narrow;
    int read;
    int made = 0;
    int64_t kept = 0;

    *bytes = 0;
    if (units < 0) {
        units = 0;
    }
    wide = hero_layer_alloc(((size_t)units + 1) * sizeof(wchar_t));
    if (wide == NULL) {
        return NULL;
    }
    read = GetWindowTextW(hwnd, wide, units + 1);
    if (read < 0) {
        read = 0;
    }
    if (read > 0) {
        made = WideCharToMultiByte(CP_UTF8, 0, wide, read, NULL, 0, NULL, NULL);
    }
    narrow = hero_layer_alloc(made > 0 ? (size_t)made : 1u);
    if (narrow == NULL) {
        hero_layer_free(wide);
        return NULL;
    }
    if (made > 0) {
        made = WideCharToMultiByte(CP_UTF8, 0, wide, read, (char *)narrow, made, NULL, NULL);
    }
    hero_layer_free(wide);
    for (int i = 0; i < made; i += 1) {
        if (lines && narrow[i] == '\r' && i + 1 < made && narrow[i + 1] == '\n') {
            continue;
        }
        narrow[kept] = narrow[i];
        kept += 1;
    }
    *bytes = kept;
    return narrow;
}

/* The text of control `hwnd` as it holds it, UTF-16 with its zero, in a
 * block of `hero_layer_alloc`, or NULL where memory ran out. */
static wchar_t *hero_ui_win_wide_of(HWND hwnd) {
    int units = GetWindowTextLengthW(hwnd);
    wchar_t *text = hero_layer_alloc(((size_t)(units > 0 ? units : 0) + 1) * sizeof(wchar_t));

    if (text != NULL) {
        text[0] = 0;
        (void)GetWindowTextW(hwnd, text, (units > 0 ? units : 0) + 1);
    }
    return text;
}

/* The widget whose window or control is `hwnd`, by the number it carries,
 * or NULL: a radio item answers its group. */
static hero_ui_native *hero_ui_win_of(HWND hwnd) {
    LONG_PTR id = hwnd == NULL ? 0 : GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    hero_ui_native *widget = id > 0 ? hero_ui_find((int64_t)id) : NULL;

    if (widget == NULL) {
        return NULL;
    }
    if (widget->hwnd == hwnd) {
        return widget;
    }
    for (int64_t i = 0; i < widget->radio_count; i += 1) {
        if (widget->radios[i] == hwnd) {
            return widget;
        }
    }
    return NULL;
}

/* The window of number `id`, or NULL. */
static hero_ui_native *hero_ui_win_window(int64_t id) {
    hero_ui_native *window = hero_ui_find(id);

    return window != NULL && window->kind == HERO_UI_WINDOW ? window : NULL;
}

/* The scale `hwnd` is drawn at, the system's where Windows names no window's
 * own. */
static UINT hero_ui_win_dpi(HWND hwnd) {
    HDC screen;
    int dpi = 0;

    if (hero_ui_win_dpi_for_window != NULL && hwnd != NULL) {
        UINT own = hero_ui_win_dpi_for_window(hwnd);

        if (own > 0) {
            return own;
        }
    }
    screen = GetDC(NULL);
    if (screen != NULL) {
        dpi = GetDeviceCaps(screen, LOGPIXELSX);
        (void)ReleaseDC(NULL, screen);
    }
    return dpi > 0 ? (UINT)dpi : 96u;
}

/* The system's metric `which` at `dpi`. */
static int hero_ui_win_metric(int which, UINT dpi) {
    if (hero_ui_win_metrics != NULL) {
        return hero_ui_win_metrics(which, dpi);
    }
    return MulDiv(GetSystemMetrics(which), (int)dpi, (int)hero_ui_win_dpi(NULL));
}

/* The font the system's own dialogs write with, at `dpi`. */
static HFONT hero_ui_win_font(UINT dpi) {
    NONCLIENTMETRICSW metrics;
    HFONT made = NULL;

    memset(&metrics, 0, sizeof metrics);
    metrics.cbSize = (UINT)sizeof metrics;
    if (hero_ui_win_parameters != NULL && hero_ui_win_parameters(SPI_GETNONCLIENTMETRICS, (UINT)sizeof metrics, &metrics, 0, dpi)) {
        made = CreateFontIndirectW(&metrics.lfMessageFont);
    } else if (SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, (UINT)sizeof metrics, &metrics, 0)) {
        metrics.lfMessageFont.lfHeight = MulDiv(metrics.lfMessageFont.lfHeight, (int)dpi, (int)hero_ui_win_dpi(NULL));
        made = CreateFontIndirectW(&metrics.lfMessageFont);
    }
    /* A stock object is never deleted, and `DeleteObject` of one is
     * harmless (its documentation), so this font is given back as a made
     * one is. */
    return made != NULL ? made : (HFONT)GetStockObject(DEFAULT_GUI_FONT);
}

/* The size `text` takes in `hwnd`'s font, in pixels: `DrawTextW`'s answer. */
static SIZE hero_ui_win_text_size(HWND hwnd, const wchar_t *text, UINT format) {
    SIZE size = {0, 0};
    RECT area = {0, 0, 0, 0};
    HDC dc = GetDC(hwnd);
    HGDIOBJ font;
    HGDIOBJ before = NULL;

    if (dc == NULL) {
        return size;
    }
    font = (HGDIOBJ)SendMessageW(hwnd, WM_GETFONT, 0, 0);
    if (font != NULL) {
        before = SelectObject(dc, font);
    }
    (void)DrawTextW(dc, text, -1, &area, format | DT_CALCRECT | DT_NOPREFIX);
    if (before != NULL) {
        (void)SelectObject(dc, before);
    }
    (void)ReleaseDC(hwnd, dc);
    size.cx = area.right - area.left;
    size.cy = area.bottom - area.top;
    return size;
}

/* The height of one line of `hwnd`'s font, in pixels. */
static int hero_ui_win_line(HWND hwnd) {
    TEXTMETRICW metrics;
    HDC dc = GetDC(hwnd);
    HGDIOBJ font;
    HGDIOBJ before = NULL;
    int height = 16;

    if (dc == NULL) {
        return height;
    }
    font = (HGDIOBJ)SendMessageW(hwnd, WM_GETFONT, 0, 0);
    if (font != NULL) {
        before = SelectObject(dc, font);
    }
    if (GetTextMetricsW(dc, &metrics)) {
        height = (int)metrics.tmHeight;
    }
    if (before != NULL) {
        (void)SelectObject(dc, before);
    }
    (void)ReleaseDC(hwnd, dc);
    return height;
}

/* -- Windows: their frame, their content, their scale ----------------------- */

/* `area`, a window's client rectangle, grown to the window's frame. */
static void hero_ui_win_frame_of(const hero_ui_native *window, RECT *area) {
    DWORD style = (DWORD)GetWindowLongPtrW(window->hwnd, GWL_STYLE);
    DWORD extended = (DWORD)GetWindowLongPtrW(window->hwnd, GWL_EXSTYLE);
    BOOL bar = GetMenu(window->hwnd) != NULL;

    if (hero_ui_win_adjust != NULL) {
        (void)hero_ui_win_adjust(area, style, bar, extended, window->dpi);
    } else {
        (void)AdjustWindowRectEx(area, style, bar, extended);
    }
}

/* Gives window `window` the content size it keeps, queuing nothing. */
static void hero_ui_win_size_content(hero_ui_native *window) {
    int width = hero_ui_win_px(window->content_width, window->dpi);
    int height = hero_ui_win_px(window->content_height, window->dpi);
    RECT frame = {0, 0, width, height};
    RECT client;

    hero_ui_win_frame_of(window, &frame);
    hero_ui_muted += 1;
    (void)SetWindowPos(window->hwnd, NULL, 0, 0, frame.right - frame.left, frame.bottom - frame.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    /* A menu bar wrapped onto more lines than the frame counts takes its
     * height out of the content: given back once. */
    if (GetClientRect(window->hwnd, &client) && client.bottom != height && client.right == width) {
        (void)SetWindowPos(window->hwnd, NULL, 0, 0, frame.right - frame.left, frame.bottom - frame.top + (height - client.bottom), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
    hero_ui_muted -= 1;
}

/* Places `widget` where the program placed it, at its window's scale, and
 * shows it where it is placed and asked to be. */
static void hero_ui_win_put(hero_ui_native *widget) {
    hero_ui_native *window = hero_ui_win_window(widget->window);
    UINT dpi = window != NULL ? window->dpi : 96u;
    int height = hero_ui_win_px(widget->height, dpi);
    UINT shown = widget->placed && widget->visible ? SWP_SHOWWINDOW : SWP_HIDEWINDOW;

    if (widget->hwnd == NULL) {
        return;
    }
    if (widget->kind == HERO_UI_COMBO) {
        /* A combo box's height is its field's and its list's together; the
         * field keeps the height of its font, and its list room for 12 items. */
        int item = (int)SendMessageW(widget->hwnd, CB_GETITEMHEIGHT, 0, 0);
        RECT field;

        if (GetWindowRect(widget->hwnd, &field) && field.bottom - field.top > height) {
            height = field.bottom - field.top;
        }
        height += (item > 0 ? item : 16) * 12 + 2;
    }
    (void)SetWindowPos(widget->hwnd, NULL, hero_ui_win_px(widget->x, dpi), hero_ui_win_px(widget->y, dpi), hero_ui_win_px(widget->width, dpi), height, SWP_NOZORDER | SWP_NOACTIVATE | shown);
    for (int64_t i = 0; i < widget->radio_count; i += 1) {
        const int64_t *place = &widget->radio_places[i * 4];

        if (place[2] < 0) {
            (void)SetWindowPos(widget->radios[i], NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
            continue;
        }
        (void)SetWindowPos(widget->radios[i], NULL, hero_ui_win_px(place[0] - widget->x, dpi), hero_ui_win_px(place[1] - widget->y, dpi), hero_ui_win_px(place[2], dpi), hero_ui_win_px(place[3], dpi), SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

/* Gives every control of window `window` its font, `SendMessageW` reaching
 * a radio group's items too. */
static void hero_ui_win_give_font(hero_ui_native *window) {
    for (size_t at = 0; at < hero_ui_room; at += 1) {
        hero_ui_native *widget = hero_ui_at(at);

        if (widget == NULL || widget->window != window->id || widget->kind == HERO_UI_WINDOW || widget->hwnd == NULL) {
            continue;
        }
        (void)SendMessageW(widget->hwnd, WM_SETFONT, (WPARAM)window->font, TRUE);
        for (int64_t i = 0; i < widget->radio_count; i += 1) {
            (void)SendMessageW(widget->radios[i], WM_SETFONT, (WPARAM)window->font, TRUE);
        }
    }
}

/* Window `hwnd` moved to a monitor of scale `dpi`: its font, its size and
 * its controls' places are made again at it, the content keeping its points. */
static void hero_ui_win_rescaled(HWND hwnd, UINT dpi, const RECT *suggested) {
    hero_ui_native *window = hero_ui_win_of(hwnd);
    HFONT before;

    if (window == NULL || window->kind != HERO_UI_WINDOW || dpi == 0) {
        return;
    }
    window->dpi = dpi;
    before = window->font;
    window->font = hero_ui_win_font(dpi);
    hero_ui_win_give_font(window);
    if (before != NULL) {
        (void)DeleteObject(before);
    }
    hero_ui_muted += 1;
    if (suggested != NULL) {
        (void)SetWindowPos(hwnd, NULL, suggested->left, suggested->top, suggested->right - suggested->left, suggested->bottom - suggested->top, SWP_NOZORDER | SWP_NOACTIVATE);
    }
    hero_ui_muted -= 1;
    hero_ui_win_size_content(window);
    for (size_t at = 0; at < hero_ui_room; at += 1) {
        hero_ui_native *widget = hero_ui_at(at);

        if (widget != NULL && widget->window == window->id && widget->kind != HERO_UI_WINDOW && widget->hwnd != NULL && widget->placed) {
            hero_ui_win_put(widget);
        }
    }
}

/* Window `hwnd`'s content took a new size: told where the person gave it,
 * kept where the program set it. */
static void hero_ui_win_sized(HWND hwnd) {
    hero_ui_native *window = hero_ui_win_of(hwnd);
    RECT client;

    if (window == NULL || window->kind != HERO_UI_WINDOW || !GetClientRect(hwnd, &client)) {
        return;
    }
    if (client.right == hero_ui_win_px(window->content_width, window->dpi) && client.bottom == hero_ui_win_px(window->content_height, window->dpi)) {
        return;
    }
    if (hero_ui_muted > 0) {
        return;
    }
    window->content_width = hero_ui_win_pt(client.right, window->dpi);
    window->content_height = hero_ui_win_pt(client.bottom, window->dpi);
    hero_ui_tell(HERO_UI_EVENT_RESIZE, window->id, window->content_width, window->content_height);
}

/* The least frame the person may give window `hwnd`. */
static bool hero_ui_win_least(HWND hwnd, MINMAXINFO *limits) {
    hero_ui_native *window = hero_ui_win_of(hwnd);
    RECT frame;

    if (window == NULL || window->kind != HERO_UI_WINDOW || limits == NULL) {
        return false;
    }
    frame.left = 0;
    frame.top = 0;
    frame.right = hero_ui_win_px(window->least_width, window->dpi);
    frame.bottom = hero_ui_win_px(window->least_height, window->dpi);
    hero_ui_win_frame_of(window, &frame);
    if (frame.right - frame.left > limits->ptMinTrackSize.x) {
        limits->ptMinTrackSize.x = frame.right - frame.left;
    }
    if (frame.bottom - frame.top > limits->ptMinTrackSize.y) {
        limits->ptMinTrackSize.y = frame.bottom - frame.top;
    }
    return true;
}

/* -- Menus and shortcuts ---------------------------------------------------- */

/* Whether menu item `item`, its menu and its window let the person choose it. */
static bool hero_ui_win_usable(const hero_ui_native *item) {
    const hero_ui_native *menu = hero_ui_find(item->parent);

    return item->kind == HERO_UI_MENU_ITEM && item->enabled && item->visible && menu != NULL && menu->enabled && menu->visible;
}

/* The menu item of command number `command`, or NULL. */
static hero_ui_native *hero_ui_win_item_of(UINT command) {
    for (size_t at = 0; at < hero_ui_room; at += 1) {
        hero_ui_native *item = hero_ui_at(at);

        if (item != NULL && (item->kind == HERO_UI_MENU_ITEM || item->kind == HERO_UI_MENU_SEPARATOR) && item->command == command) {
            return item;
        }
    }
    return NULL;
}

/* A command number no menu item holds, or 0 where all are taken. */
static WORD hero_ui_win_new_command(void) {
    UINT span = HERO_UI_WIN_LAST_COMMAND - HERO_UI_WIN_FIRST_COMMAND + 1;

    for (UINT tried = 0; tried < span; tried += 1) {
        UINT command = hero_ui_win_next_command;

        if (command < HERO_UI_WIN_FIRST_COMMAND || command > HERO_UI_WIN_LAST_COMMAND) {
            command = HERO_UI_WIN_FIRST_COMMAND;
        }
        hero_ui_win_next_command = command + 1;
        if (hero_ui_win_item_of(command) == NULL) {
            return (WORD)command;
        }
    }
    return 0;
}

/* The name Windows gives virtual key `key` on this keyboard, into `name`
 * of `room` units; `extended` for the keys of the arrows' block. */
static int hero_ui_win_key_name(UINT key, bool extended, wchar_t *name, int room) {
    UINT scan = MapVirtualKeyW(key, MAPVK_VK_TO_VSC);
    LONG word = (LONG)((scan & 0xFFu) << 16) | (extended ? (LONG)1 << 24 : 0);

    return scan == 0 ? 0 : GetKeyNameTextW(word, name, room);
}

/* Appends `text` to `into` of `room` units at `*used`, as much as fits with
 * the zero after it. */
static void hero_ui_win_append(wchar_t *into, size_t room, size_t *used, const wchar_t *text) {
    for (size_t i = 0; text[i] != 0 && *used + 1 < room; i += 1) {
        into[*used] = text[i];
        *used += 1;
    }
    into[*used] = 0;
}

/* `label` as a menu shows it, `&` doubled and a tab a space, then a tab and
 * `keys` where they are not empty; in a block of `hero_layer_alloc`, or
 * NULL. */
static wchar_t *hero_ui_win_menu_text(const wchar_t *label, const wchar_t *keys) {
    size_t length = label != NULL ? wcslen(label) : 0;
    wchar_t *text = hero_layer_alloc((length * 2 + wcslen(keys) + 2) * sizeof(wchar_t));
    size_t at = 0;

    if (text == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < length; i += 1) {
        if (label[i] == L'&') {
            text[at++] = L'&';
        }
        text[at++] = label[i] == L'\t' ? L' ' : label[i];
    }
    if (keys[0] != 0) {
        text[at++] = L'\t';
        for (size_t i = 0; keys[i] != 0; i += 1) {
            text[at++] = keys[i];
        }
    }
    text[at] = 0;
    return text;
}

/* Item `item`'s text as its menu shows it: its label, then its shortcut's
 * keys by the names this keyboard gives them, `Ctrl+Shift+O`. */
static wchar_t *hero_ui_win_item_text(const hero_ui_native *item) {
    static const struct {
        BYTE held;
        UINT key;
        const wchar_t *otherwise;
    } held_names[] = {{FCONTROL, VK_CONTROL, L"Ctrl"}, {FSHIFT, VK_SHIFT, L"Shift"}, {FALT, VK_MENU, L"Alt"}};
    wchar_t keys[160];
    size_t used = 0;

    keys[0] = 0;
    if (item->key != 0) {
        wchar_t name[48];

        for (size_t i = 0; i < sizeof held_names / sizeof held_names[0]; i += 1) {
            if ((item->held & held_names[i].held) == 0) {
                continue;
            }
            hero_ui_win_append(keys, 160, &used, hero_ui_win_key_name(held_names[i].key, false, name, 48) > 0 ? name : held_names[i].otherwise);
            hero_ui_win_append(keys, 160, &used, L"+");
        }
        hero_ui_win_append(keys, 160, &used, hero_ui_win_key_name(item->key, item->key_extended, name, 48) > 0 ? name : L"?");
    }
    return hero_ui_win_menu_text(item->label, keys);
}

/* Fills menu `menu`'s pop-up again with its items shown, in their order. */
static int64_t hero_ui_win_fill_menu(hero_ui_native *menu) {
    hero_ui_native *window = hero_ui_win_window(menu->parent);
    int64_t answer = HERO_UI_OK;

    while (GetMenuItemCount(menu->menu) > 0) {
        if (!RemoveMenu(menu->menu, 0, MF_BYPOSITION)) {
            break;
        }
    }
    for (int64_t i = 0; i < menu->order.count; i += 1) {
        hero_ui_native *item = hero_ui_find(menu->order.at[i]);
        wchar_t *text;
        BOOL added;

        if (item == NULL || !item->visible) {
            continue;
        }
        if (item->kind == HERO_UI_MENU_SEPARATOR) {
            added = AppendMenuW(menu->menu, MF_SEPARATOR, item->command, NULL);
        } else {
            text = hero_ui_win_item_text(item);
            if (text == NULL) {
                answer = HERO_UI_LIMIT;
                continue;
            }
            added = AppendMenuW(menu->menu, MF_STRING | (item->enabled ? MF_ENABLED : MF_GRAYED), item->command, text);
            hero_layer_free(text);
        }
        if (!added && answer == HERO_UI_OK) {
            answer = hero_ui_win_failed("adding an item to a menu", GetLastError());
        }
    }
    if (window != NULL) {
        window->accelerators_stale = true;
        if (GetMenu(window->hwnd) != NULL) {
            (void)DrawMenuBar(window->hwnd);
        }
    }
    return answer;
}

/* Fills window `window`'s menu bar again with its menus shown, in their
 * order; the bar is taken off the window while it holds none, and the
 * content keeps its size. */
static int64_t hero_ui_win_fill_bar(hero_ui_native *window) {
    int64_t answer = HERO_UI_OK;
    int shown = 0;
    bool had = GetMenu(window->hwnd) != NULL;

    if (window->menu == NULL) {
        window->menu = CreateMenu();
        if (window->menu == NULL) {
            return hero_ui_win_failed("making a menu bar", GetLastError());
        }
    }
    while (GetMenuItemCount(window->menu) > 0) {
        if (!RemoveMenu(window->menu, 0, MF_BYPOSITION)) {
            break;
        }
    }
    for (int64_t i = 0; i < window->order.count; i += 1) {
        hero_ui_native *menu = hero_ui_find(window->order.at[i]);
        wchar_t *text;

        if (menu == NULL || !menu->visible) {
            continue;
        }
        text = hero_ui_win_menu_text(menu->label, L"");
        if (text == NULL) {
            answer = HERO_UI_LIMIT;
            continue;
        }
        if (!AppendMenuW(window->menu, MF_POPUP | (menu->enabled ? MF_ENABLED : MF_GRAYED), (UINT_PTR)menu->menu, text)) {
            if (answer == HERO_UI_OK) {
                answer = hero_ui_win_failed("adding a menu to a menu bar", GetLastError());
            }
        } else {
            shown += 1;
        }
        hero_layer_free(text);
    }
    if ((shown > 0) != had) {
        (void)SetMenu(window->hwnd, shown > 0 ? window->menu : NULL);
        hero_ui_win_size_content(window);
    } else if (had) {
        (void)DrawMenuBar(window->hwnd);
    }
    window->accelerators_stale = true;
    return answer;
}

/* Makes window `window`'s accelerator table again from the shortcuts of its
 * menu items shown. */
static void hero_ui_win_make_accelerators(hero_ui_native *window) {
    ACCEL *table;
    int64_t most = 0;
    int count = 0;

    if (window->accelerators != NULL) {
        (void)DestroyAcceleratorTable(window->accelerators);
        window->accelerators = NULL;
    }
    window->accelerators_stale = false;
    for (int64_t m = 0; m < window->order.count; m += 1) {
        hero_ui_native *menu = hero_ui_find(window->order.at[m]);

        most += menu != NULL ? menu->order.count : 0;
    }
    if (most == 0 || most > INT32_MAX) {
        return;
    }
    table = hero_layer_alloc((size_t)most * sizeof *table);
    if (table == NULL) {
        /* No memory for the table: the shortcuts stay unmade and are asked
         * for again at the next message. */
        window->accelerators_stale = true;
        return;
    }
    for (int64_t m = 0; m < window->order.count; m += 1) {
        hero_ui_native *menu = hero_ui_find(window->order.at[m]);

        if (menu == NULL || !menu->visible) {
            continue;
        }
        for (int64_t i = 0; i < menu->order.count; i += 1) {
            hero_ui_native *item = hero_ui_find(menu->order.at[i]);

            if (item == NULL || item->kind != HERO_UI_MENU_ITEM || item->key == 0 || !item->visible) {
                continue;
            }
            table[count].fVirt = (BYTE)(FVIRTKEY | item->held);
            table[count].key = item->key;
            table[count].cmd = item->command;
            count += 1;
        }
    }
    if (count > 0) {
        window->accelerators = CreateAcceleratorTableW(table, count);
    }
    hero_layer_free(table);
}

/* -- What the person does ---------------------------------------------------- */

/* The position of `value` on slider `slider`'s trackbar: the value itself
 * where its range fits the trackbar's 32 bits, else its share of
 * `HERO_UI_WIN_STEPS`. */
static int hero_ui_win_position(const hero_ui_native *slider, int64_t value) {
    if (slider->least >= INT32_MIN && slider->most <= INT32_MAX) {
        return (int)hero_ui_between(value, slider->least, slider->most);
    }
    return (int)(hero_ui_fraction(hero_ui_between(value, slider->least, slider->most), slider->least, slider->most) * HERO_UI_WIN_STEPS + 0.5);
}

/* The value standing at trackbar position `position`. */
static int64_t hero_ui_win_value_at(const hero_ui_native *slider, int position) {
    if (slider->least >= INT32_MIN && slider->most <= INT32_MAX) {
        return hero_ui_between(position, slider->least, slider->most);
    }
    return hero_ui_at_fraction((double)position / HERO_UI_WIN_STEPS, slider->least, slider->most);
}

/* Gives a trackbar or a progress bar the range and the value `widget` keeps. */
static void hero_ui_win_show_range(hero_ui_native *widget) {
    bool direct = widget->least >= INT32_MIN && widget->most <= INT32_MAX;
    int low = direct ? (int)widget->least : 0;
    int high = direct ? (int)widget->most : HERO_UI_WIN_STEPS;
    int position = widget->kind == HERO_UI_SLIDER ? hero_ui_win_position(widget, widget->value) : direct ? (int)hero_ui_between(widget->value, widget->least, widget->most) : (int)(hero_ui_fraction(widget->value, widget->least, widget->most) * HERO_UI_WIN_STEPS + 0.5);

    if (widget->kind == HERO_UI_SLIDER) {
        (void)SendMessageW(widget->hwnd, TBM_SETRANGEMIN, FALSE, (LPARAM)low);
        (void)SendMessageW(widget->hwnd, TBM_SETRANGEMAX, FALSE, (LPARAM)high);
        (void)SendMessageW(widget->hwnd, TBM_SETPOS, TRUE, (LPARAM)position);
    } else {
        (void)SendMessageW(widget->hwnd, PBM_SETRANGE32, (WPARAM)low, (LPARAM)high);
        (void)SendMessageW(widget->hwnd, PBM_SETPOS, (WPARAM)position, 0);
    }
}

/* Checks the `index`th item of radio group `group` and no other; -1 none. */
static void hero_ui_win_check_radio(hero_ui_native *group, int64_t index) {
    for (int64_t i = 0; i < group->radio_count; i += 1) {
        (void)SendMessageW(group->radios[i], BM_SETCHECK, i == index ? BST_CHECKED : BST_UNCHECKED, 0);
    }
}

/* The item radio group `group` has checked, -1 for none. */
static int64_t hero_ui_win_checked_radio(const hero_ui_native *group) {
    for (int64_t i = 0; i < group->radio_count; i += 1) {
        if (SendMessageW(group->radios[i], BM_GETCHECK, 0, 0) == BST_CHECKED) {
            return i;
        }
    }
    return -1;
}

/* A control's notification `code` to its parent, or a menu's or a shortcut's
 * command where `control` is NULL. */
static void hero_ui_win_command(UINT code, UINT command, HWND control) {
    hero_ui_native *widget;

    if (control == NULL) {
        hero_ui_native *item = code <= 1 ? hero_ui_win_item_of(command) : NULL;

        if (item != NULL && hero_ui_win_usable(item)) {
            hero_ui_tell(HERO_UI_EVENT_MENU, item->id, 0, 0);
        }
        return;
    }
    widget = hero_ui_win_of(control);
    if (widget == NULL) {
        return;
    }
    switch (widget->kind) {
    case HERO_UI_BUTTON:
        if (code == BN_CLICKED) {
            hero_ui_tell(HERO_UI_EVENT_CLICK, widget->id, 0, 0);
        }
        break;
    case HERO_UI_CHECKBOX:
        if (code == BN_CLICKED) {
            hero_ui_tell(HERO_UI_EVENT_TOGGLE, widget->id, SendMessageW(control, BM_GETCHECK, 0, 0) == BST_CHECKED ? 1 : 0, 0);
        }
        break;
    case HERO_UI_RADIO_GROUP:
        if (code == BN_CLICKED) {
            for (int64_t i = 0; i < widget->radio_count; i += 1) {
                if (widget->radios[i] == control) {
                    hero_ui_win_check_radio(widget, i);
                    hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, i, 0);
                }
            }
        }
        break;
    case HERO_UI_TEXT_FIELD:
    case HERO_UI_TEXT_AREA:
        if (code == EN_CHANGE) {
            hero_ui_tell(HERO_UI_EVENT_TEXT, widget->id, 0, 0);
        }
        break;
    case HERO_UI_LIST:
    case HERO_UI_LIST_MULTIPLE:
        if (code == LBN_SELCHANGE) {
            hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, 0, 0);
        }
        break;
    case HERO_UI_COMBO:
        if (code == CBN_SELCHANGE) {
            LRESULT chosen = SendMessageW(control, CB_GETCURSEL, 0, 0);

            hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, chosen == CB_ERR ? -1 : (int64_t)chosen, 0);
        }
        break;
    default:
        break;
    }
}

/* Trackbar `control` moved. */
static void hero_ui_win_slid(HWND control) {
    hero_ui_native *slider = hero_ui_win_of(control);
    int64_t value;

    if (slider == NULL || slider->kind != HERO_UI_SLIDER) {
        return;
    }
    value = hero_ui_win_value_at(slider, (int)SendMessageW(control, TBM_GETPOS, 0, 0));
    if (value != slider->value || hero_ui_win_injecting > 0) {
        slider->value = value;
        hero_ui_tell(HERO_UI_EVENT_VALUE, slider->id, value, 0);
    }
}

/* Paints picture `hwnd`: the window's colour, and its pixels in the middle,
 * each pixel a point. */
static void hero_ui_win_paint(HWND hwnd) {
    PAINTSTRUCT paint;
    HDC dc = BeginPaint(hwnd, &paint);
    hero_ui_native *picture = hero_ui_win_of(hwnd);
    hero_ui_native *window = picture != NULL ? hero_ui_win_window(picture->window) : NULL;
    RECT area;

    if (dc == NULL) {
        return;
    }
    if (GetClientRect(hwnd, &area)) {
        (void)FillRect(dc, &area, GetSysColorBrush(COLOR_3DFACE));
    }
    if (picture != NULL && picture->bitmap != NULL && window != NULL) {
        int width = hero_ui_win_px(picture->bitmap_width, window->dpi);
        int height = hero_ui_win_px(picture->bitmap_height, window->dpi);
        HDC memory = CreateCompatibleDC(dc);

        if (memory != NULL) {
            HGDIOBJ before = SelectObject(memory, picture->bitmap);
            BLENDFUNCTION blend = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

            (void)AlphaBlend(dc, (area.right - width) / 2, (area.bottom - height) / 2, width, height, memory, 0, 0, (int)picture->bitmap_width, (int)picture->bitmap_height, blend);
            (void)SelectObject(memory, before);
            (void)DeleteDC(memory);
        }
    }
    (void)EndPaint(hwnd, &paint);
}

/* The system's quit (`WM_QUIT`): every window is asked to close, in the
 * order made, as AppKit's Quit asks. */
static void hero_ui_win_quit(void) {
    int64_t last = 0;

    for (;;) {
        int64_t next = 0;

        for (size_t at = 0; at < hero_ui_room; at += 1) {
            hero_ui_native *window = hero_ui_at(at);

            if (window != NULL && window->kind == HERO_UI_WINDOW && window->id > last && (next == 0 || window->id < next)) {
                next = window->id;
            }
        }
        if (next == 0) {
            return;
        }
        hero_ui_tell(HERO_UI_EVENT_CLOSE, next, 0, 0);
        last = next;
    }
}

static LRESULT CALLBACK hero_ui_win_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    hero_ui_native *widget;

    switch (message) {
    case WM_COMMAND:
        hero_ui_win_command(HIWORD(wparam), LOWORD(wparam), (HWND)lparam);
        return 0;
    case WM_HSCROLL:
        if (lparam != 0) {
            hero_ui_win_slid((HWND)lparam);
        }
        return 0;
    case WM_CLOSE:
        widget = hero_ui_win_of(hwnd);
        if (widget != NULL && widget->kind == HERO_UI_WINDOW) {
            hero_ui_tell(HERO_UI_EVENT_CLOSE, widget->id, 0, 0);
            return 0;
        }
        break;
    case WM_SIZE:
        if (wparam != SIZE_MINIMIZED) {
            hero_ui_win_sized(hwnd);
        }
        break;
    case WM_GETMINMAXINFO:
        if (hero_ui_win_least(hwnd, (MINMAXINFO *)lparam)) {
            return 0;
        }
        break;
    case HERO_UI_WIN_DPICHANGED:
        hero_ui_win_rescaled(hwnd, HIWORD(wparam), (const RECT *)lparam);
        return 0;
    case WM_ERASEBKGND:
        widget = hero_ui_win_of(hwnd);
        if (widget != NULL && widget->kind == HERO_UI_IMAGE) {
            return 1;
        }
        break;
    case WM_PAINT:
        widget = hero_ui_win_of(hwnd);
        if (widget != NULL && widget->kind == HERO_UI_IMAGE) {
            hero_ui_win_paint(hwnd);
            return 0;
        }
        break;
    default:
        break;
    }
    return DefWindowProcW(hwnd, message, wparam, lparam);
}

/* -- The arm ------------------------------------------------------------------ */

/* Whether the process's window station shows what is drawn and takes a
 * person's input: an interactive one does, and a service's or a remote
 * shell's session (`ssh`) does not. */
static int64_t hero_ui_arm_here(void) {
    USEROBJECTFLAGS flags;
    DWORD needed = 0;
    HWINSTA station = GetProcessWindowStation();

    memset(&flags, 0, sizeof flags);
    if (station == NULL || !GetUserObjectInformationW(station, UOI_FLAGS, &flags, (DWORD)sizeof flags, &needed)) {
        return 0;
    }
    return (flags.dwFlags & WSF_VISIBLE) != 0 ? 1 : 0;
}

static void hero_ui_arm_absent(char *words, size_t count) {
    (void)hero_ui_copy_text(words, count, "this process's window station shows nothing and takes no input, as a service's or a remote shell's does");
}

/* The function of `user32` named `name` into `slot`, left NULL where this
 * Windows has none; C11 converts no object pointer to a function pointer, so
 * the address is copied by its bytes. */
static void hero_ui_win_find_function(HMODULE user32, const char *name, void *slot, size_t size) {
    FARPROC found = user32 != NULL ? GetProcAddress(user32, name) : NULL;

    if (found != NULL && size == sizeof found) {
        memcpy(slot, &found, sizeof found);
    }
}

/* Registers class `name` of this arm's procedure, where it is not yet. */
static bool hero_ui_win_register(const wchar_t *name, HBRUSH background, HICON icon) {
    WNDCLASSEXW request;
    HINSTANCE module = GetModuleHandleW(NULL);

    memset(&request, 0, sizeof request);
    request.cbSize = (UINT)sizeof request;
    if (GetClassInfoExW(module, name, &request)) {
        return true;
    }
    memset(&request, 0, sizeof request);
    request.cbSize = (UINT)sizeof request;
    request.lpfnWndProc = hero_ui_win_proc;
    request.hInstance = module;
    request.hCursor = LoadCursorW(NULL, (LPCWSTR)IDC_ARROW);
    request.hbrBackground = background;
    request.hIcon = icon;
    request.lpszClassName = name;
    return RegisterClassExW(&request) != 0;
}

/* The activation context of the common controls' version 6, made once and
 * activated (THE SYSTEM'S CURRENT LOOK). */
static void hero_ui_win_take_styles(void) {
    if (hero_ui_win_context == NULL) {
        wchar_t folder[MAX_PATH + 1];
        wchar_t file[MAX_PATH + 1];
        DWORD length = GetTempPathW(MAX_PATH + 1, folder);

        hero_ui_win_context = INVALID_HANDLE_VALUE;
        if (length > 0 && length <= MAX_PATH && GetTempFileNameW(folder, L"hui", 0, file) != 0) {
            HANDLE out = CreateFileW(file, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
            DWORD written = 0;
            DWORD wanted = (DWORD)(sizeof hero_ui_win_manifest - 1);
            bool whole = false;

            if (out != INVALID_HANDLE_VALUE) {
                whole = WriteFile(out, hero_ui_win_manifest, wanted, &written, NULL) && written == wanted;
                (void)CloseHandle(out);
            }
            if (whole) {
                ACTCTXW request;

                memset(&request, 0, sizeof request);
                request.cbSize = (ULONG)sizeof request;
                request.lpSource = file;
                hero_ui_win_context = CreateActCtxW(&request);
            }
            (void)DeleteFileW(file);
        }
    }
    if (hero_ui_win_context != INVALID_HANDLE_VALUE && !hero_ui_win_activated) {
        hero_ui_win_activated = ActivateActCtx(hero_ui_win_context, &hero_ui_win_cookie) != FALSE;
    }
}

static int64_t hero_ui_arm_open(void) {
    INITCOMMONCONTROLSEX controls;

    if (!hero_ui_win_ready) {
        HMODULE user32 = GetModuleHandleW(L"user32.dll");

        hero_ui_win_find_function(user32, "GetDpiForWindow", &hero_ui_win_dpi_for_window, sizeof hero_ui_win_dpi_for_window);
        hero_ui_win_find_function(user32, "AdjustWindowRectExForDpi", &hero_ui_win_adjust, sizeof hero_ui_win_adjust);
        hero_ui_win_find_function(user32, "SystemParametersInfoForDpi", &hero_ui_win_parameters, sizeof hero_ui_win_parameters);
        hero_ui_win_find_function(user32, "GetSystemMetricsForDpi", &hero_ui_win_metrics, sizeof hero_ui_win_metrics);
        hero_ui_win_find_function(user32, "SetThreadDpiAwarenessContext", &hero_ui_win_awareness, sizeof hero_ui_win_awareness);
    }
    hero_ui_win_take_styles();
    if (hero_ui_win_awareness != NULL && !hero_ui_win_aware) {
        HANDLE before = hero_ui_win_awareness(HERO_UI_WIN_AWARE_V2);

        if (before == NULL) {
            before = hero_ui_win_awareness(HERO_UI_WIN_AWARE);
        }
        hero_ui_win_awareness_before = before;
        hero_ui_win_aware = before != NULL;
    }
    memset(&controls, 0, sizeof controls);
    controls.dwSize = (DWORD)sizeof controls;
    controls.dwICC = ICC_STANDARD_CLASSES | ICC_BAR_CLASSES | ICC_PROGRESS_CLASS;
    (void)InitCommonControlsEx(&controls);
    if (!hero_ui_win_ready) {
        HBRUSH face = (HBRUSH)(INT_PTR)(COLOR_3DFACE + 1);

        if (!hero_ui_win_register(hero_ui_win_window_class, face, LoadIconW(NULL, (LPCWSTR)IDI_APPLICATION)) || !hero_ui_win_register(hero_ui_win_panel_class, face, NULL) || !hero_ui_win_register(hero_ui_win_picture_class, NULL, NULL)) {
            return hero_ui_win_failed("registering the window classes", GetLastError());
        }
        hero_ui_win_ready = true;
    }
    hero_ui_muted = 0;
    hero_ui_win_injecting = 0;
    return HERO_UI_OK;
}

/* Destroys `widget`'s system objects and gives back its record; its children
 * are gone already. */
static void hero_ui_win_drop(hero_ui_native *widget) {
    hero_ui_native *owner = hero_ui_find(widget->parent);

    switch (widget->kind) {
    case HERO_UI_WINDOW:
        if (widget->hwnd != NULL) {
            (void)SetWindowLongPtrW(widget->hwnd, GWLP_USERDATA, 0);
            if (GetMenu(widget->hwnd) != NULL) {
                (void)SetMenu(widget->hwnd, NULL);
            }
            (void)DestroyWindow(widget->hwnd);
        }
        if (widget->menu != NULL) {
            (void)DestroyMenu(widget->menu);
        }
        if (widget->accelerators != NULL) {
            (void)DestroyAcceleratorTable(widget->accelerators);
        }
        if (widget->font != NULL) {
            (void)DeleteObject(widget->font);
        }
        break;
    case HERO_UI_MENU:
        if (owner != NULL) {
            hero_ui_ids_drop(&owner->order, widget->id);
            (void)hero_ui_win_fill_bar(owner);
        }
        if (widget->menu != NULL) {
            (void)DestroyMenu(widget->menu);
        }
        break;
    case HERO_UI_MENU_ITEM:
    case HERO_UI_MENU_SEPARATOR:
        if (owner != NULL) {
            hero_ui_ids_drop(&owner->order, widget->id);
            (void)hero_ui_win_fill_menu(owner);
        }
        break;
    default:
        for (int64_t i = 0; i < widget->radio_count; i += 1) {
            (void)SetWindowLongPtrW(widget->radios[i], GWLP_USERDATA, 0);
        }
        if (widget->hwnd != NULL) {
            (void)SetWindowLongPtrW(widget->hwnd, GWLP_USERDATA, 0);
            (void)DestroyWindow(widget->hwnd);
        }
        if (widget->bitmap != NULL) {
            (void)DeleteObject(widget->bitmap);
        }
        break;
    }
    hero_layer_free(widget->radios);
    hero_layer_free(widget->radio_places);
    hero_layer_free(widget->label);
    hero_ui_ids_free(&widget->order);
    hero_ui_forget(widget->id);
    hero_layer_free(widget);
}

/* Destroys widget `id` and every widget whose parent it is, children first. */
static void hero_ui_win_destroy_tree(int64_t id) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return;
    }
    for (;;) {
        int64_t child = 0;

        for (size_t at = 0; at < hero_ui_room && child == 0; at += 1) {
            hero_ui_native *each = hero_ui_at(at);

            if (each != NULL && each->parent == id && each->id != id) {
                child = each->id;
            }
        }
        if (child == 0) {
            break;
        }
        hero_ui_win_destroy_tree(child);
    }
    hero_ui_win_drop(widget);
}

static void hero_ui_arm_close(void) {
    for (;;) {
        int64_t first = 0;

        for (size_t at = 0; at < hero_ui_room && first == 0; at += 1) {
            hero_ui_native *widget = hero_ui_at(at);

            if (widget != NULL) {
                first = widget->kind == HERO_UI_WINDOW || hero_ui_find(widget->parent) == NULL ? widget->id : widget->parent;
            }
        }
        if (first == 0) {
            break;
        }
        hero_ui_win_destroy_tree(first);
    }
    hero_ui_free_map();
    hero_layer_free(hero_ui_win_paths);
    hero_ui_win_paths = NULL;
    hero_ui_win_paths_count = 0;
    if (hero_ui_win_activated) {
        (void)DeactivateActCtx(0, hero_ui_win_cookie);
        hero_ui_win_activated = false;
    }
    if (hero_ui_win_aware && hero_ui_win_awareness != NULL) {
        (void)hero_ui_win_awareness(hero_ui_win_awareness_before);
        hero_ui_win_aware = false;
    }
}

/* Makes control `kind` of class `class` in `parent`, at no place and hidden,
 * carrying widget number `id` and its window's font. */
static HWND hero_ui_win_control(const hero_ui_native *window, HWND parent, const wchar_t *class, DWORD style, DWORD extended, int64_t id) {
    HWND made = CreateWindowExW(extended, class, L"", WS_CHILD | WS_CLIPSIBLINGS | style, 0, 0, 0, 0, parent, NULL, GetModuleHandleW(NULL), NULL);

    if (made != NULL) {
        (void)SetWindowLongPtrW(made, GWLP_USERDATA, (LONG_PTR)id);
        (void)SendMessageW(made, WM_SETFONT, (WPARAM)window->font, FALSE);
    }
    return made;
}

static int64_t hero_ui_arm_create(int64_t id, int64_t kind, int64_t parent) {
    hero_ui_native *owner = NULL;
    hero_ui_native *window = NULL;
    hero_ui_native *made;
    int64_t answer = HERO_UI_OK;

    if (hero_ui_find(id) != NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (kind == HERO_UI_WEBVIEW) {
        /* THE WEB VIEW, kind 17, is an arm of its own; until it stands here
         * this arm makes none. */
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "the system's web view is not written for Windows yet");
    }
    if (kind != HERO_UI_WINDOW) {
        owner = hero_ui_find(parent);
        if (owner == NULL) {
            return HERO_UI_NOT_FOUND;
        }
        if ((kind == HERO_UI_MENU_ITEM || kind == HERO_UI_MENU_SEPARATOR) != (owner->kind == HERO_UI_MENU) || (owner->kind != HERO_UI_MENU && owner->kind != HERO_UI_WINDOW)) {
            return HERO_UI_INVALID;
        }
        window = hero_ui_win_window(owner->window);
        if (window == NULL) {
            return HERO_UI_NOT_FOUND;
        }
    }
    made = hero_layer_alloc_zero(1, sizeof *made);
    if (made == NULL) {
        return HERO_UI_LIMIT;
    }
    made->id = id;
    made->kind = kind;
    made->parent = parent;
    made->window = window != NULL ? window->id : id;
    made->visible = true;
    made->enabled = true;
    made->least = 0;
    made->most = 100;
    switch (kind) {
    case HERO_UI_WINDOW:
        made->hwnd = CreateWindowExW(WS_EX_CONTROLPARENT, hero_ui_win_window_class, L"", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, 100, 100, NULL, NULL, GetModuleHandleW(NULL), NULL);
        if (made->hwnd != NULL) {
            (void)SetWindowLongPtrW(made->hwnd, GWLP_USERDATA, (LONG_PTR)id);
            made->dpi = hero_ui_win_dpi(made->hwnd);
            made->font = hero_ui_win_font(made->dpi);
            made->content_width = 100;
            made->content_height = 100;
        }
        break;
    case HERO_UI_LABEL:
        made->hwnd = hero_ui_win_control(window, window->hwnd, L"STATIC", SS_LEFT | SS_NOPREFIX, 0, id);
        break;
    case HERO_UI_BUTTON:
        made->hwnd = hero_ui_win_control(window, window->hwnd, L"BUTTON", BS_PUSHBUTTON | WS_TABSTOP, 0, id);
        break;
    case HERO_UI_TEXT_FIELD:
        made->hwnd = hero_ui_win_control(window, window->hwnd, L"EDIT", ES_LEFT | ES_AUTOHSCROLL | WS_TABSTOP, WS_EX_CLIENTEDGE, id);
        break;
    case HERO_UI_TEXT_AREA:
        made->hwnd = hero_ui_win_control(window, window->hwnd, L"EDIT", ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | WS_VSCROLL | WS_TABSTOP, WS_EX_CLIENTEDGE, id);
        break;
    case HERO_UI_CHECKBOX:
        made->hwnd = hero_ui_win_control(window, window->hwnd, L"BUTTON", BS_AUTOCHECKBOX | WS_TABSTOP, 0, id);
        break;
    case HERO_UI_RADIO_GROUP:
        made->hwnd = hero_ui_win_control(window, window->hwnd, hero_ui_win_panel_class, 0, WS_EX_CONTROLPARENT, id);
        break;
    case HERO_UI_LIST:
    case HERO_UI_LIST_MULTIPLE:
        made->hwnd = hero_ui_win_control(window, window->hwnd, L"LISTBOX", LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_TABSTOP | (kind == HERO_UI_LIST_MULTIPLE ? LBS_EXTENDEDSEL : 0), WS_EX_CLIENTEDGE, id);
        break;
    case HERO_UI_COMBO:
        made->hwnd = hero_ui_win_control(window, window->hwnd, L"COMBOBOX", CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, 0, id);
        break;
    case HERO_UI_PROGRESS:
        made->hwnd = hero_ui_win_control(window, window->hwnd, PROGRESS_CLASSW, 0, 0, id);
        break;
    case HERO_UI_SLIDER:
        made->hwnd = hero_ui_win_control(window, window->hwnd, TRACKBAR_CLASSW, TBS_HORZ | TBS_NOTICKS | WS_TABSTOP, 0, id);
        break;
    case HERO_UI_IMAGE:
        made->hwnd = hero_ui_win_control(window, window->hwnd, hero_ui_win_picture_class, 0, 0, id);
        break;
    case HERO_UI_MENU:
        made->menu = CreatePopupMenu();
        if (made->menu == NULL) {
            answer = hero_ui_win_failed("making a menu", GetLastError());
        }
        break;
    case HERO_UI_MENU_ITEM:
    case HERO_UI_MENU_SEPARATOR:
        made->command = hero_ui_win_new_command();
        if (made->command == 0) {
            answer = hero_ui_refused(HERO_UI_OTHER, "every command number Win32 gives menu items is taken");
        }
        break;
    default:
        answer = HERO_UI_INVALID;
        break;
    }
    if (answer == HERO_UI_OK && made->hwnd == NULL && kind != HERO_UI_MENU && kind != HERO_UI_MENU_ITEM && kind != HERO_UI_MENU_SEPARATOR) {
        answer = hero_ui_win_failed("making a widget", GetLastError());
    }
    if (answer == HERO_UI_OK && !hero_ui_keep(id, made)) {
        answer = HERO_UI_LIMIT;
    }
    if (answer != HERO_UI_OK) {
        if (made->hwnd != NULL) {
            (void)DestroyWindow(made->hwnd);
        }
        if (made->menu != NULL) {
            (void)DestroyMenu(made->menu);
        }
        if (made->font != NULL) {
            (void)DeleteObject(made->font);
        }
        hero_layer_free(made);
        return answer;
    }
    switch (kind) {
    case HERO_UI_WINDOW:
        hero_ui_win_size_content(made);
        break;
    case HERO_UI_TEXT_FIELD:
    case HERO_UI_TEXT_AREA:
        (void)SendMessageW(made->hwnd, EM_SETLIMITTEXT, 0, 0);
        break;
    case HERO_UI_SLIDER:
    case HERO_UI_PROGRESS:
        hero_ui_win_show_range(made);
        break;
    case HERO_UI_MENU:
        if (!hero_ui_ids_add(&owner->order, id)) {
            hero_ui_win_drop(made);
            return HERO_UI_LIMIT;
        }
        return hero_ui_win_fill_bar(owner);
    case HERO_UI_MENU_ITEM:
    case HERO_UI_MENU_SEPARATOR:
        if (!hero_ui_ids_add(&owner->order, id)) {
            hero_ui_win_drop(made);
            return HERO_UI_LIMIT;
        }
        return hero_ui_win_fill_menu(owner);
    default:
        break;
    }
    return HERO_UI_OK;
}

static int64_t hero_ui_arm_destroy(int64_t id) {
    if (hero_ui_find(id) == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    hero_ui_win_destroy_tree(id);
    return HERO_UI_OK;
}

static int64_t hero_ui_arm_set_text(int64_t id, const uint8_t *text, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);
    unsigned ways = 0;
    wchar_t *wide;
    BOOL set;
    DWORD code;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    switch (widget->kind) {
    case HERO_UI_LABEL:
    case HERO_UI_TEXT_AREA:
        ways = HERO_UI_WIN_LINES;
        break;
    case HERO_UI_BUTTON:
    case HERO_UI_CHECKBOX:
        ways = HERO_UI_WIN_PREFIX;
        break;
    case HERO_UI_WINDOW:
    case HERO_UI_TEXT_FIELD:
    case HERO_UI_MENU:
    case HERO_UI_MENU_ITEM:
        break;
    default:
        return HERO_UI_INVALID;
    }
    wide = hero_ui_win_wide(text, count, ways);
    if (wide == NULL) {
        return HERO_UI_LIMIT;
    }
    if (widget->kind == HERO_UI_MENU || widget->kind == HERO_UI_MENU_ITEM) {
        hero_layer_free(widget->label);
        widget->label = wide;
        return widget->kind == HERO_UI_MENU ? hero_ui_win_fill_bar(hero_ui_find(widget->parent)) : hero_ui_win_fill_menu(hero_ui_find(widget->parent));
    }
    hero_ui_muted += 1;
    set = SetWindowTextW(widget->hwnd, wide);
    code = GetLastError();
    if (set && (widget->kind == HERO_UI_TEXT_FIELD || widget->kind == HERO_UI_TEXT_AREA)) {
        (void)SendMessageW(widget->hwnd, EM_SETSEL, wcslen(wide), (LPARAM)wcslen(wide));
        (void)SendMessageW(widget->hwnd, EM_SCROLLCARET, 0, 0);
    }
    hero_ui_muted -= 1;
    hero_layer_free(wide);
    return set ? HERO_UI_OK : hero_ui_win_failed("setting a text", code);
}

/* Makes radio group `group`'s items anew, the `count` bytes of `items`. */
static int64_t hero_ui_win_make_radios(hero_ui_native *group, const uint8_t *items, int64_t count) {
    hero_ui_native *window = hero_ui_win_window(group->window);
    int64_t texts = hero_ui_texts_in(items, count);
    HWND *radios = NULL;
    int64_t *places = NULL;

    if (window == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (texts > 0) {
        radios = hero_layer_alloc_zero((size_t)texts, sizeof *radios);
        places = hero_layer_alloc_zero((size_t)texts * 4, sizeof *places);
        if (radios == NULL || places == NULL) {
            hero_layer_free(radios);
            hero_layer_free(places);
            return HERO_UI_LIMIT;
        }
    }
    for (int64_t i = 0; i < group->radio_count; i += 1) {
        (void)SetWindowLongPtrW(group->radios[i], GWLP_USERDATA, 0);
        (void)DestroyWindow(group->radios[i]);
    }
    hero_layer_free(group->radios);
    hero_layer_free(group->radio_places);
    group->radios = radios;
    group->radio_places = places;
    group->radio_count = 0;
    for (int64_t i = 0; i < texts; i += 1) {
        int64_t length = 0;
        const uint8_t *text = hero_ui_text_at(items, count, i, &length);
        wchar_t *wide = hero_ui_win_wide(text, length, HERO_UI_WIN_PREFIX);
        HWND radio = hero_ui_win_control(window, group->hwnd, L"BUTTON", BS_AUTORADIOBUTTON | WS_VISIBLE | (i == 0 ? WS_GROUP | WS_TABSTOP : 0), 0, group->id);

        if (radio == NULL || wide == NULL) {
            DWORD code = GetLastError();

            hero_layer_free(wide);
            if (radio != NULL) {
                (void)DestroyWindow(radio);
            }
            return wide == NULL ? HERO_UI_LIMIT : hero_ui_win_failed("making a radio group's item", code);
        }
        (void)SetWindowTextW(radio, wide);
        hero_layer_free(wide);
        (void)EnableWindow(radio, group->enabled);
        radios[i] = radio;
        places[i * 4 + 2] = -1;
        group->radio_count = i + 1;
    }
    hero_ui_win_put(group);
    return HERO_UI_OK;
}

static int64_t hero_ui_arm_set_items(int64_t id, const uint8_t *items, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);
    int64_t texts = hero_ui_texts_in(items, count);
    UINT reset;
    UINT add;
    int64_t answer = HERO_UI_OK;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind == HERO_UI_RADIO_GROUP) {
        return hero_ui_win_make_radios(widget, items, count);
    }
    if (widget->kind == HERO_UI_LIST || widget->kind == HERO_UI_LIST_MULTIPLE) {
        reset = LB_RESETCONTENT;
        add = LB_ADDSTRING;
    } else if (widget->kind == HERO_UI_COMBO) {
        reset = CB_RESETCONTENT;
        add = CB_ADDSTRING;
    } else {
        return HERO_UI_INVALID;
    }
    hero_ui_muted += 1;
    (void)SendMessageW(widget->hwnd, reset, 0, 0);
    widget->items = 0;
    for (int64_t i = 0; i < texts && answer == HERO_UI_OK; i += 1) {
        int64_t length = 0;
        const uint8_t *text = hero_ui_text_at(items, count, i, &length);
        wchar_t *wide = hero_ui_win_wide(text, length, 0);
        LRESULT added;

        if (wide == NULL) {
            answer = HERO_UI_LIMIT;
            break;
        }
        added = SendMessageW(widget->hwnd, add, 0, (LPARAM)wide);
        hero_layer_free(wide);
        if (added < 0) {
            answer = HERO_UI_LIMIT;
            break;
        }
        widget->items += 1;
    }
    if (answer != HERO_UI_OK) {
        (void)SendMessageW(widget->hwnd, reset, 0, 0);
        widget->items = 0;
    }
    if (widget->kind == HERO_UI_COMBO) {
        (void)SendMessageW(widget->hwnd, CB_SETCURSEL, (WPARAM)-1, 0);
    }
    hero_ui_muted -= 1;
    return answer;
}

static int64_t hero_ui_arm_set_value(int64_t id, int64_t value) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    switch (widget->kind) {
    case HERO_UI_CHECKBOX:
        if (value != 0 && value != 1) {
            return HERO_UI_INVALID;
        }
        (void)SendMessageW(widget->hwnd, BM_SETCHECK, value == 1 ? BST_CHECKED : BST_UNCHECKED, 0);
        return HERO_UI_OK;
    case HERO_UI_SLIDER:
    case HERO_UI_PROGRESS:
        if (value < widget->least || value > widget->most) {
            return HERO_UI_INVALID;
        }
        widget->value = value;
        hero_ui_win_show_range(widget);
        return HERO_UI_OK;
    case HERO_UI_COMBO:
        if (value < -1 || value >= widget->items) {
            return HERO_UI_INVALID;
        }
        (void)SendMessageW(widget->hwnd, CB_SETCURSEL, (WPARAM)value, 0);
        return HERO_UI_OK;
    case HERO_UI_RADIO_GROUP:
        if (value < -1 || value >= widget->radio_count) {
            return HERO_UI_INVALID;
        }
        hero_ui_win_check_radio(widget, value);
        return HERO_UI_OK;
    default:
        return HERO_UI_INVALID;
    }
}

static int64_t hero_ui_arm_set_range(int64_t id, int64_t least, int64_t most) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind != HERO_UI_SLIDER && widget->kind != HERO_UI_PROGRESS) {
        return HERO_UI_INVALID;
    }
    widget->least = least;
    widget->most = most;
    widget->value = hero_ui_between(widget->value, least, most);
    hero_ui_win_show_range(widget);
    return HERO_UI_OK;
}

static int64_t hero_ui_arm_set_selection(int64_t id, const int64_t *indices, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if ((widget->kind != HERO_UI_LIST && widget->kind != HERO_UI_LIST_MULTIPLE) || (count > 1 && widget->kind != HERO_UI_LIST_MULTIPLE)) {
        return HERO_UI_INVALID;
    }
    for (int64_t i = 0; i < count; i += 1) {
        if (indices[i] >= widget->items) {
            return HERO_UI_INVALID;
        }
    }
    hero_ui_muted += 1;
    if (widget->kind == HERO_UI_LIST) {
        (void)SendMessageW(widget->hwnd, LB_SETCURSEL, count == 0 ? (WPARAM)-1 : (WPARAM)indices[0], 0);
    } else {
        (void)SendMessageW(widget->hwnd, LB_SETSEL, FALSE, -1);
        for (int64_t i = 0; i < count; i += 1) {
            (void)SendMessageW(widget->hwnd, LB_SETSEL, TRUE, (LPARAM)indices[i]);
        }
    }
    hero_ui_muted -= 1;
    return HERO_UI_OK;
}

static int64_t hero_ui_arm_set_flag(int64_t id, int64_t which, int64_t on) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (which == HERO_UI_FLAG_RESIZABLE) {
        LONG_PTR style;

        if (widget->kind != HERO_UI_WINDOW) {
            return HERO_UI_INVALID;
        }
        style = GetWindowLongPtrW(widget->hwnd, GWL_STYLE);
        style = on == 1 ? (style | WS_THICKFRAME | WS_MAXIMIZEBOX) : (style & ~(LONG_PTR)(WS_THICKFRAME | WS_MAXIMIZEBOX));
        (void)SetWindowLongPtrW(widget->hwnd, GWL_STYLE, style);
        hero_ui_muted += 1;
        (void)SetWindowPos(widget->hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
        hero_ui_muted -= 1;
        hero_ui_win_size_content(widget);
        return HERO_UI_OK;
    }
    if (which == HERO_UI_FLAG_VISIBLE) {
        widget->visible = on == 1;
        switch (widget->kind) {
        case HERO_UI_WINDOW:
            if (on == 1) {
                (void)ShowWindow(widget->hwnd, SW_SHOWNORMAL);
                (void)SetForegroundWindow(widget->hwnd);
            } else {
                (void)ShowWindow(widget->hwnd, SW_HIDE);
            }
            return HERO_UI_OK;
        case HERO_UI_MENU:
            return hero_ui_win_fill_bar(hero_ui_find(widget->parent));
        case HERO_UI_MENU_ITEM:
        case HERO_UI_MENU_SEPARATOR:
            return hero_ui_win_fill_menu(hero_ui_find(widget->parent));
        default:
            (void)ShowWindow(widget->hwnd, widget->visible && widget->placed ? SW_SHOWNA : SW_HIDE);
            return HERO_UI_OK;
        }
    }
    if (which == HERO_UI_FLAG_ENABLED) {
        if (widget->kind == HERO_UI_WINDOW) {
            return HERO_UI_INVALID;
        }
        widget->enabled = on == 1;
        switch (widget->kind) {
        case HERO_UI_MENU:
            return hero_ui_win_fill_bar(hero_ui_find(widget->parent));
        case HERO_UI_MENU_ITEM:
        case HERO_UI_MENU_SEPARATOR:
            return hero_ui_win_fill_menu(hero_ui_find(widget->parent));
        default:
            for (int64_t i = 0; i < widget->radio_count; i += 1) {
                (void)EnableWindow(widget->radios[i], on == 1);
            }
            (void)EnableWindow(widget->hwnd, on == 1);
            return HERO_UI_OK;
        }
    }
    return HERO_UI_INVALID;
}

/* The virtual key of the key `hero_ui.h` names, and whether it is an
 * extended one; false for a name it does not list. */
static bool hero_ui_win_key_of(const uint8_t *key, int64_t count, WORD *code, bool *extended) {
    static const struct {
        const char *name;
        WORD code;
        bool extended;
    } named[] = {
        {"Enter", VK_RETURN, false}, {"Escape", VK_ESCAPE, false}, {"Tab", VK_TAB, false},     {"Space", VK_SPACE, false}, {"Delete", VK_DELETE, true}, {"Backspace", VK_BACK, false}, {"Up", VK_UP, true},          {"Down", VK_DOWN, true},
        {"Left", VK_LEFT, true},     {"Right", VK_RIGHT, true},    {"Home", VK_HOME, true},     {"End", VK_END, true},      {"PageUp", VK_PRIOR, true},  {"PageDown", VK_NEXT, true},
    };

    *extended = false;
    if (count == 1 && key[0] >= 'a' && key[0] <= 'z') {
        *code = (WORD)(key[0] - 'a' + 'A');
        return true;
    }
    if (count == 1 && key[0] >= '0' && key[0] <= '9') {
        *code = (WORD)key[0];
        return true;
    }
    if (count >= 2 && count <= 3 && key[0] == 'F') {
        int64_t n = 0;

        for (int64_t i = 1; i < count; i += 1) {
            if (key[i] < '0' || key[i] > '9') {
                return false;
            }
            n = n * 10 + (key[i] - '0');
        }
        if (n >= 1 && n <= 12 && !(count == 3 && key[1] == '0')) {
            *code = (WORD)(VK_F1 + n - 1);
            return true;
        }
        return false;
    }
    for (size_t i = 0; i < sizeof named / sizeof named[0]; i += 1) {
        if (strlen(named[i].name) == (size_t)count && memcmp(named[i].name, key, (size_t)count) == 0) {
            *code = named[i].code;
            *extended = named[i].extended;
            return true;
        }
    }
    return false;
}

static int64_t hero_ui_arm_set_shortcut(int64_t id, const uint8_t *key, int64_t count, int64_t modifiers) {
    hero_ui_native *item = hero_ui_find(id);
    WORD code = 0;
    bool extended = false;

    if (item == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (item->kind != HERO_UI_MENU_ITEM || (count > 0 && !hero_ui_win_key_of(key, count, &code, &extended))) {
        return HERO_UI_INVALID;
    }
    item->key = code;
    item->key_extended = extended;
    item->held = (BYTE)(((modifiers & HERO_UI_PRIMARY) != 0 ? FCONTROL : 0) | ((modifiers & HERO_UI_SHIFT) != 0 ? FSHIFT : 0) | ((modifiers & HERO_UI_ALT) != 0 ? FALT : 0));
    return hero_ui_win_fill_menu(hero_ui_find(item->parent));
}

static int64_t hero_ui_arm_set_image(int64_t id, const uint8_t *rgba, int64_t width, int64_t height) {
    hero_ui_native *picture = hero_ui_find(id);
    BITMAPINFO format;
    void *bits = NULL;
    HBITMAP made;
    uint8_t *into;

    if (picture == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (picture->kind != HERO_UI_IMAGE) {
        return HERO_UI_INVALID;
    }
    if (picture->bitmap != NULL) {
        (void)DeleteObject(picture->bitmap);
        picture->bitmap = NULL;
    }
    picture->bitmap_width = 0;
    picture->bitmap_height = 0;
    if (width == 0 || height == 0) {
        (void)InvalidateRect(picture->hwnd, NULL, TRUE);
        return HERO_UI_OK;
    }
    if (width > INT32_MAX / 4 || height > INT32_MAX / 4) {
        return HERO_UI_LIMIT;
    }
    memset(&format, 0, sizeof format);
    format.bmiHeader.biSize = (DWORD)sizeof format.bmiHeader;
    format.bmiHeader.biWidth = (LONG)width;
    format.bmiHeader.biHeight = -(LONG)height;
    format.bmiHeader.biPlanes = 1;
    format.bmiHeader.biBitCount = 32;
    format.bmiHeader.biCompression = BI_RGB;
    made = CreateDIBSection(NULL, &format, DIB_RGB_COLORS, &bits, NULL, 0);
    if (made == NULL || bits == NULL) {
        if (made != NULL) {
            (void)DeleteObject(made);
        }
        return HERO_UI_LIMIT;
    }
    into = bits;
    for (int64_t i = 0; i < width * height; i += 1) {
        const uint8_t *from = rgba + i * 4;
        unsigned alpha = from[3];

        into[i * 4 + 0] = (uint8_t)(((unsigned)from[2] * alpha + 127u) / 255u);
        into[i * 4 + 1] = (uint8_t)(((unsigned)from[1] * alpha + 127u) / 255u);
        into[i * 4 + 2] = (uint8_t)(((unsigned)from[0] * alpha + 127u) / 255u);
        into[i * 4 + 3] = (uint8_t)alpha;
    }
    picture->bitmap = made;
    picture->bitmap_width = width;
    picture->bitmap_height = height;
    (void)InvalidateRect(picture->hwnd, NULL, TRUE);
    return HERO_UI_OK;
}

static int64_t hero_ui_arm_set_size(int64_t id, int64_t which, int64_t width, int64_t height) {
    hero_ui_native *window = hero_ui_find(id);
    RECT client;

    if (window == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (window->kind != HERO_UI_WINDOW) {
        return HERO_UI_INVALID;
    }
    if (which == HERO_UI_SIZE_LEAST) {
        window->least_width = hero_ui_between(width, 0, HERO_UI_WIN_POINTS_MOST);
        window->least_height = hero_ui_between(height, 0, HERO_UI_WIN_POINTS_MOST);
        return HERO_UI_OK;
    }
    width = hero_ui_between(width, 0, HERO_UI_WIN_POINTS_MOST);
    height = hero_ui_between(height, 0, HERO_UI_WIN_POINTS_MOST);
    if (width == window->content_width && height == window->content_height && GetClientRect(window->hwnd, &client) && client.right == hero_ui_win_px(width, window->dpi) && client.bottom == hero_ui_win_px(height, window->dpi)) {
        return HERO_UI_OK;
    }
    window->content_width = width;
    window->content_height = height;
    hero_ui_win_size_content(window);
    if (GetClientRect(window->hwnd, &client) && (client.right != hero_ui_win_px(width, window->dpi) || client.bottom != hero_ui_win_px(height, window->dpi))) {
        /* Windows kept the frame inside what the screen or the least size
         * allow: the size the content took is the one kept. */
        window->content_width = hero_ui_win_pt(client.right, window->dpi);
        window->content_height = hero_ui_win_pt(client.bottom, window->dpi);
    }
    return HERO_UI_OK;
}

static int64_t hero_ui_arm_place(int64_t id, int64_t index, int64_t x, int64_t y, int64_t width, int64_t height) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->hwnd == NULL || widget->kind == HERO_UI_WINDOW) {
        return HERO_UI_INVALID;
    }
    if (index >= 0) {
        int64_t *place;

        if (widget->kind != HERO_UI_RADIO_GROUP || index >= widget->radio_count) {
            return HERO_UI_INVALID;
        }
        place = &widget->radio_places[index * 4];
        place[0] = hero_ui_between(x, -HERO_UI_WIN_POINTS_MOST, HERO_UI_WIN_POINTS_MOST);
        place[1] = hero_ui_between(y, -HERO_UI_WIN_POINTS_MOST, HERO_UI_WIN_POINTS_MOST);
        place[2] = hero_ui_between(width, 0, HERO_UI_WIN_POINTS_MOST);
        place[3] = hero_ui_between(height, 0, HERO_UI_WIN_POINTS_MOST);
    } else {
        widget->x = x;
        widget->y = y;
        widget->width = width;
        widget->height = height;
        widget->placed = true;
    }
    hero_ui_win_put(widget);
    return HERO_UI_OK;
}

/* The size a button, a checkbox or a radio item asks, in pixels: the
 * system's ideal (version 6 of the common controls), else its text's with
 * the room around it. */
static SIZE hero_ui_win_button_size(HWND button, bool box, UINT dpi) {
    SIZE ideal = {0, 0};
    SIZE measured = {0, 0};
    wchar_t *text = hero_ui_win_wide_of(button);

    if (text != NULL) {
        measured = hero_ui_win_text_size(button, text, DT_SINGLELINE);
        hero_layer_free(text);
    }
    if (!SendMessageW(button, BCM_GETIDEALSIZE, 0, (LPARAM)&ideal)) {
        ideal.cx = 0;
        ideal.cy = 0;
    }
    if (box) {
        int mark = hero_ui_win_metric(SM_CXMENUCHECK, dpi);

        ideal.cx = ideal.cx > measured.cx + mark + MulDiv(8, (int)dpi, 96) ? ideal.cx : measured.cx + mark + MulDiv(8, (int)dpi, 96);
        ideal.cy = ideal.cy > MulDiv(17, (int)dpi, 96) ? ideal.cy : MulDiv(17, (int)dpi, 96);
        return ideal;
    }
    /* A push button is 75 by 23 pixels at least at 96 dots an inch, the
     * size Windows' own dialogs give their buttons. */
    ideal.cx = ideal.cx > measured.cx + MulDiv(20, (int)dpi, 96) ? ideal.cx : measured.cx + MulDiv(20, (int)dpi, 96);
    ideal.cx = ideal.cx > MulDiv(75, (int)dpi, 96) ? ideal.cx : MulDiv(75, (int)dpi, 96);
    ideal.cy = ideal.cy > MulDiv(23, (int)dpi, 96) ? ideal.cy : MulDiv(23, (int)dpi, 96);
    return ideal;
}

static int64_t hero_ui_arm_preferred(int64_t id, int64_t *width, int64_t *height) {
    hero_ui_native *widget = hero_ui_find(id);
    hero_ui_native *window;
    UINT dpi;
    SIZE size = {0, 0};

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    window = hero_ui_win_window(widget->window);
    dpi = window != NULL ? window->dpi : 96u;
    *width = 0;
    *height = 0;
    switch (widget->kind) {
    case HERO_UI_LABEL: {
        wchar_t *text = hero_ui_win_wide_of(widget->hwnd);

        if (text == NULL) {
            return HERO_UI_LIMIT;
        }
        /* An empty label still takes a line's height, as AppKit's does. */
        size = hero_ui_win_text_size(widget->hwnd, text[0] == 0 ? L" " : text, DT_LEFT);
        size.cx = text[0] == 0 ? 0 : size.cx;
        hero_layer_free(text);
        break;
    }
    case HERO_UI_BUTTON:
        size = hero_ui_win_button_size(widget->hwnd, false, dpi);
        break;
    case HERO_UI_CHECKBOX:
        size = hero_ui_win_button_size(widget->hwnd, true, dpi);
        break;
    case HERO_UI_RADIO_GROUP:
        for (int64_t i = 0; i < widget->radio_count; i += 1) {
            SIZE item = hero_ui_win_button_size(widget->radios[i], true, dpi);

            size.cx = item.cx > size.cx ? item.cx : size.cx;
            size.cy += item.cy;
        }
        break;
    case HERO_UI_TEXT_FIELD:
        /* The headless side's width, and a line of the font with the edit
         * control's borders: 23 pixels at 96 dots an inch in Windows' own
         * dialogs. */
        *width = 160;
        size.cy = hero_ui_win_line(widget->hwnd) + 2 * hero_ui_win_metric(SM_CYEDGE, dpi) + MulDiv(3, (int)dpi, 96);
        break;
    case HERO_UI_TEXT_AREA:
        *width = 240;
        *height = 96;
        return HERO_UI_OK;
    case HERO_UI_LIST:
    case HERO_UI_LIST_MULTIPLE:
        *width = 160;
        *height = 96;
        return HERO_UI_OK;
    case HERO_UI_COMBO: {
        RECT field;
        int widest = 0;

        for (int64_t i = 0; i < widget->items; i += 1) {
            LRESULT units = SendMessageW(widget->hwnd, CB_GETLBTEXTLEN, (WPARAM)i, 0);
            wchar_t *item = units >= 0 ? hero_layer_alloc(((size_t)units + 1) * sizeof(wchar_t)) : NULL;

            if (item == NULL) {
                continue;
            }
            item[0] = 0;
            if (SendMessageW(widget->hwnd, CB_GETLBTEXT, (WPARAM)i, (LPARAM)item) >= 0) {
                SIZE text = hero_ui_win_text_size(widget->hwnd, item, DT_SINGLELINE);

                widest = text.cx > widest ? text.cx : widest;
            }
            hero_layer_free(item);
        }
        size.cx = widest + hero_ui_win_metric(SM_CXVSCROLL, dpi) + 2 * hero_ui_win_metric(SM_CXEDGE, dpi) + MulDiv(12, (int)dpi, 96);
        size.cx = size.cx > MulDiv(80, (int)dpi, 96) ? size.cx : MulDiv(80, (int)dpi, 96);
        size.cy = GetWindowRect(widget->hwnd, &field) ? field.bottom - field.top : MulDiv(23, (int)dpi, 96);
        break;
    }
    case HERO_UI_PROGRESS:
        *width = 160;
        *height = 16;
        return HERO_UI_OK;
    case HERO_UI_SLIDER: {
        LRESULT thumb = SendMessageW(widget->hwnd, TBM_GETTHUMBLENGTH, 0, 0);

        *width = 160;
        size.cy = (int)(thumb > 0 ? thumb : MulDiv(20, (int)dpi, 96)) + MulDiv(10, (int)dpi, 96);
        break;
    }
    case HERO_UI_IMAGE:
        *width = widget->bitmap_width;
        *height = widget->bitmap_height;
        return HERO_UI_OK;
    default:
        return HERO_UI_OK;
    }
    if (*width == 0) {
        *width = hero_ui_win_pt_up(size.cx, dpi);
    }
    *height = hero_ui_win_pt_up(size.cy, dpi);
    return HERO_UI_OK;
}

static int64_t hero_ui_arm_text(int64_t id, uint8_t *buffer, int64_t count, int64_t *length) {
    hero_ui_native *widget = hero_ui_find(id);
    uint8_t *text;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind != HERO_UI_TEXT_FIELD && widget->kind != HERO_UI_TEXT_AREA) {
        return HERO_UI_INVALID;
    }
    text = hero_ui_win_text_of(widget->hwnd, widget->kind == HERO_UI_TEXT_AREA, length);
    if (text == NULL) {
        return HERO_UI_LIMIT;
    }
    if (*length > 0 && *length <= count) {
        memcpy(buffer, text, (size_t)*length);
    }
    hero_layer_free(text);
    return HERO_UI_OK;
}

static int64_t hero_ui_arm_value(int64_t id, int64_t *value) {
    hero_ui_native *widget = hero_ui_find(id);
    LRESULT chosen;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    switch (widget->kind) {
    case HERO_UI_CHECKBOX:
        *value = SendMessageW(widget->hwnd, BM_GETCHECK, 0, 0) == BST_CHECKED ? 1 : 0;
        return HERO_UI_OK;
    case HERO_UI_SLIDER:
    case HERO_UI_PROGRESS:
        *value = widget->value;
        return HERO_UI_OK;
    case HERO_UI_COMBO:
        chosen = SendMessageW(widget->hwnd, CB_GETCURSEL, 0, 0);
        *value = chosen == CB_ERR ? -1 : (int64_t)chosen;
        return HERO_UI_OK;
    case HERO_UI_RADIO_GROUP:
        *value = hero_ui_win_checked_radio(widget);
        return HERO_UI_OK;
    default:
        return HERO_UI_INVALID;
    }
}

static int64_t hero_ui_arm_selection(int64_t id, int64_t *indices, int64_t count, int64_t *chosen) {
    hero_ui_native *widget = hero_ui_find(id);
    int64_t one;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    switch (widget->kind) {
    case HERO_UI_LIST_MULTIPLE: {
        LRESULT selected = SendMessageW(widget->hwnd, LB_GETSELCOUNT, 0, 0);
        int *read;
        LRESULT got;

        if (selected <= 0) {
            *chosen = 0;
            return HERO_UI_OK;
        }
        read = hero_layer_alloc((size_t)selected * sizeof *read);
        if (read == NULL) {
            return HERO_UI_LIMIT;
        }
        got = SendMessageW(widget->hwnd, LB_GETSELITEMS, (WPARAM)selected, (LPARAM)read);
        got = got < 0 ? 0 : got;
        /* Win32 does not promise their order: ascending, as the header does. */
        for (LRESULT i = 1; i < got; i += 1) {
            int held = read[i];
            LRESULT j = i;

            while (j > 0 && read[j - 1] > held) {
                read[j] = read[j - 1];
                j -= 1;
            }
            read[j] = held;
        }
        *chosen = (int64_t)got;
        if (got <= count) {
            for (LRESULT i = 0; i < got; i += 1) {
                indices[i] = read[i];
            }
        }
        hero_layer_free(read);
        return HERO_UI_OK;
    }
    case HERO_UI_LIST:
        one = (int64_t)SendMessageW(widget->hwnd, LB_GETCURSEL, 0, 0);
        break;
    case HERO_UI_COMBO:
        one = (int64_t)SendMessageW(widget->hwnd, CB_GETCURSEL, 0, 0);
        break;
    case HERO_UI_RADIO_GROUP:
        one = hero_ui_win_checked_radio(widget);
        break;
    default:
        return HERO_UI_INVALID;
    }
    *chosen = one >= 0 ? 1 : 0;
    if (one >= 0 && count >= 1) {
        indices[0] = one;
    }
    return HERO_UI_OK;
}

/* Whether `message` was a window's shortcut or a key the dialog manager
 * moves between controls by, and so is handled. */
static bool hero_ui_win_translated(MSG *message) {
    HWND root = message->hwnd != NULL ? GetAncestor(message->hwnd, GA_ROOT) : NULL;
    hero_ui_native *window = root != NULL ? hero_ui_win_of(root) : NULL;

    if (window == NULL || window->kind != HERO_UI_WINDOW) {
        return false;
    }
    if (message->message >= WM_KEYFIRST && message->message <= WM_KEYLAST) {
        if (window->accelerators_stale) {
            hero_ui_win_make_accelerators(window);
        }
        if (window->accelerators != NULL && TranslateAcceleratorW(root, window->accelerators, message)) {
            return true;
        }
    }
    return IsDialogMessageW(root, message) != FALSE;
}

static int64_t hero_ui_arm_pump(int64_t ms) {
    uint64_t before = hero_ui_pushed;
    ULONGLONG deadline = GetTickCount64() + (ULONGLONG)hero_ui_between(ms, 0, INT32_MAX);
    MSG message;

    for (;;) {
        ULONGLONG now;

        while (hero_ui_pushed == before && PeekMessageW(&message, NULL, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT) {
                hero_ui_win_quit();
                continue;
            }
            if (hero_ui_win_translated(&message)) {
                continue;
            }
            (void)TranslateMessage(&message);
            (void)DispatchMessageW(&message);
        }
        now = GetTickCount64();
        if (hero_ui_pushed != before || now >= deadline) {
            return HERO_UI_OK;
        }
        (void)MsgWaitForMultipleObjectsEx(0, NULL, (DWORD)(deadline - now), QS_ALLINPUT, MWMO_INPUTAVAILABLE);
    }
}

/* Presses button `button`, a push button's, a checkbox's or a radio item's,
 * by `BM_CLICK`, and where Win32 dropped the press sends the `BN_CLICKED`
 * the control would have (WHAT `inject` DOES). */
static void hero_ui_win_press(HWND button, bool toggles) {
    uint64_t before = hero_ui_pushed;
    LRESULT state = SendMessageW(button, BM_GETCHECK, 0, 0);

    if (!IsWindowEnabled(button)) {
        return;
    }
    (void)SendMessageW(button, BM_CLICK, 0, 0);
    if (hero_ui_pushed != before) {
        return;
    }
    if (toggles && SendMessageW(button, BM_GETCHECK, 0, 0) == state) {
        (void)SendMessageW(button, BM_SETCHECK, state == BST_CHECKED ? BST_UNCHECKED : BST_CHECKED, 0);
    }
    (void)SendMessageW(GetParent(button), WM_COMMAND, MAKEWPARAM(0, BN_CLICKED), (LPARAM)button);
}

static int64_t hero_ui_arm_inject(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);
    int64_t answer = HERO_UI_OK;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    hero_ui_win_injecting += 1;
    switch (kind) {
    case HERO_UI_EVENT_CLICK:
    case HERO_UI_EVENT_TOGGLE:
        if (widget->kind == HERO_UI_BUTTON || widget->kind == HERO_UI_CHECKBOX) {
            hero_ui_win_press(widget->hwnd, widget->kind == HERO_UI_CHECKBOX);
        } else {
            answer = HERO_UI_INVALID;
        }
        break;
    case HERO_UI_EVENT_TEXT:
        if (widget->kind == HERO_UI_TEXT_FIELD || widget->kind == HERO_UI_TEXT_AREA) {
            wchar_t *wide = hero_ui_win_wide(text, count, widget->kind == HERO_UI_TEXT_AREA ? HERO_UI_WIN_LINES : 0);

            if (wide == NULL) {
                answer = HERO_UI_LIMIT;
            } else if (IsWindowEnabled(widget->hwnd)) {
                (void)SendMessageW(widget->hwnd, EM_SETSEL, 0, -1);
                (void)SendMessageW(widget->hwnd, EM_REPLACESEL, TRUE, (LPARAM)wide);
            }
            hero_layer_free(wide);
        } else {
            answer = HERO_UI_INVALID;
        }
        break;
    case HERO_UI_EVENT_SELECT:
        if (widget->kind == HERO_UI_LIST || widget->kind == HERO_UI_LIST_MULTIPLE) {
            if (value >= widget->items) {
                answer = HERO_UI_INVALID;
            } else if (IsWindowEnabled(widget->hwnd)) {
                if (widget->kind == HERO_UI_LIST) {
                    (void)SendMessageW(widget->hwnd, LB_SETCURSEL, value < 0 ? (WPARAM)-1 : (WPARAM)value, 0);
                } else {
                    if (value < 0 || second != 1) {
                        (void)SendMessageW(widget->hwnd, LB_SETSEL, FALSE, -1);
                    }
                    if (value >= 0) {
                        (void)SendMessageW(widget->hwnd, LB_SETSEL, TRUE, (LPARAM)value);
                    }
                }
                (void)SendMessageW(GetParent(widget->hwnd), WM_COMMAND, MAKEWPARAM(0, LBN_SELCHANGE), (LPARAM)widget->hwnd);
            }
        } else if (widget->kind == HERO_UI_COMBO) {
            if (value < 0 || value >= widget->items) {
                answer = HERO_UI_INVALID;
            } else if (IsWindowEnabled(widget->hwnd)) {
                (void)SendMessageW(widget->hwnd, CB_SETCURSEL, (WPARAM)value, 0);
                (void)SendMessageW(GetParent(widget->hwnd), WM_COMMAND, MAKEWPARAM(0, CBN_SELCHANGE), (LPARAM)widget->hwnd);
            }
        } else if (widget->kind == HERO_UI_RADIO_GROUP) {
            if (value < 0 || value >= widget->radio_count) {
                answer = HERO_UI_INVALID;
            } else {
                hero_ui_win_press(widget->radios[value], false);
            }
        } else {
            answer = HERO_UI_INVALID;
        }
        break;
    case HERO_UI_EVENT_VALUE:
        if (widget->kind != HERO_UI_SLIDER) {
            answer = HERO_UI_INVALID;
        } else if (IsWindowEnabled(widget->hwnd)) {
            int position = hero_ui_win_position(widget, value);

            (void)SendMessageW(widget->hwnd, TBM_SETPOS, TRUE, (LPARAM)position);
            (void)SendMessageW(GetParent(widget->hwnd), WM_HSCROLL, MAKEWPARAM(TB_THUMBPOSITION, (WORD)position), (LPARAM)widget->hwnd);
        }
        break;
    case HERO_UI_EVENT_MENU:
        if (widget->kind != HERO_UI_MENU_ITEM) {
            answer = HERO_UI_INVALID;
        } else if (hero_ui_win_usable(widget)) {
            hero_ui_native *window = hero_ui_win_window(widget->window);

            if (window != NULL) {
                (void)SendMessageW(window->hwnd, WM_COMMAND, MAKEWPARAM(widget->command, 0), 0);
            }
        }
        break;
    case HERO_UI_EVENT_CLOSE:
        if (widget->kind != HERO_UI_WINDOW) {
            answer = HERO_UI_INVALID;
        } else {
            (void)SendMessageW(widget->hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
        }
        break;
    case HERO_UI_EVENT_RESIZE:
        if (widget->kind != HERO_UI_WINDOW) {
            answer = HERO_UI_INVALID;
        } else if ((GetWindowLongPtrW(widget->hwnd, GWL_STYLE) & WS_THICKFRAME) != 0) {
            int width = hero_ui_win_px(hero_ui_between(value, widget->least_width, HERO_UI_WIN_POINTS_MOST), widget->dpi);
            int height = hero_ui_win_px(hero_ui_between(second, widget->least_height, HERO_UI_WIN_POINTS_MOST), widget->dpi);
            RECT frame = {0, 0, width, height};

            hero_ui_win_frame_of(widget, &frame);
            (void)SetWindowPos(widget->hwnd, NULL, 0, 0, frame.right - frame.left, frame.bottom - frame.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    default:
        answer = HERO_UI_INVALID;
        break;
    }
    hero_ui_win_injecting -= 1;
    return answer;
}

/* The top-level window of number `window` for a dialog to stand over, NULL
 * for 0; false where that number is no window. */
static bool hero_ui_win_owner(int64_t window, HWND *owner, int64_t *answer) {
    hero_ui_native *found;

    *owner = NULL;
    if (window == 0) {
        return true;
    }
    found = hero_ui_find(window);
    if (found == NULL || found->kind != HERO_UI_WINDOW) {
        *answer = found == NULL ? HERO_UI_NOT_FOUND : HERO_UI_INVALID;
        return false;
    }
    *owner = found->hwnd;
    return true;
}

static int64_t hero_ui_arm_message(int64_t window, int64_t tone, int64_t buttons, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count, int64_t *answer) {
    HWND owner;
    int64_t refused = HERO_UI_OK;
    wchar_t *heading;
    wchar_t *said;
    UINT style;
    int pressed;
    DWORD code;

    if (!hero_ui_win_owner(window, &owner, &refused)) {
        return refused;
    }
    heading = hero_ui_win_wide(title, title_count, 0);
    said = hero_ui_win_wide(text, text_count, HERO_UI_WIN_LINES);
    if (heading == NULL || said == NULL) {
        hero_layer_free(heading);
        hero_layer_free(said);
        return HERO_UI_LIMIT;
    }
    style = tone == HERO_UI_TONE_ERROR ? MB_ICONERROR : (tone == HERO_UI_TONE_WARNING ? MB_ICONWARNING : MB_ICONINFORMATION);
    style |= buttons == HERO_UI_BUTTONS_OK_CANCEL ? MB_OKCANCEL : buttons == HERO_UI_BUTTONS_YES_NO ? MB_YESNO : buttons == HERO_UI_BUTTONS_YES_NO_CANCEL ? MB_YESNOCANCEL : MB_OK;
    pressed = MessageBoxW(owner, said, heading, style | MB_SETFOREGROUND);
    code = GetLastError();
    hero_layer_free(heading);
    hero_layer_free(said);
    switch (pressed) {
    case IDOK:
        *answer = HERO_UI_ANSWER_OK;
        return HERO_UI_OK;
    case IDYES:
        *answer = HERO_UI_ANSWER_YES;
        return HERO_UI_OK;
    case IDNO:
        *answer = HERO_UI_ANSWER_NO;
        return HERO_UI_OK;
    case IDCANCEL:
        *answer = HERO_UI_ANSWER_CANCEL;
        return HERO_UI_OK;
    default:
        return hero_ui_win_failed("showing a message box", code);
    }
}

/* Keeps the UTF-16 path `wide` of `units` units, followed by one zero byte,
 * at the end of the paths answered; one that is no UTF-16 (an unpaired
 * surrogate) is passed over, since no UTF-8 names it. */
static bool hero_ui_win_keep_path(const wchar_t *wide, int units, uint8_t **paths, int64_t *count, int64_t *room) {
    int bytes = units > 0 ? WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wide, units, NULL, 0, NULL, NULL) : 0;

    if (bytes <= 0) {
        return true;
    }
    if (*count + bytes + 1 > *room) {
        int64_t grown_room = (*room + bytes + 1) * 2;
        uint8_t *grown = hero_layer_grow(*paths, (size_t)grown_room);

        if (grown == NULL) {
            return false;
        }
        *paths = grown;
        *room = grown_room;
    }
    (void)WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wide, units, (char *)*paths + *count, bytes, NULL, NULL);
    *count += bytes;
    (*paths)[*count] = 0;
    *count += 1;
    return true;
}

static int64_t hero_ui_arm_file_dialog(int64_t window, int64_t save, int64_t multiple, const uint8_t *title, int64_t title_count, const uint8_t *name, int64_t name_count, const uint8_t *filters, int64_t filters_count, int64_t *bytes) {
    OPENFILENAMEW request;
    HWND owner;
    int64_t refused = HERO_UI_OK;
    wchar_t *heading = hero_ui_win_wide(title, title_count, 0);
    wchar_t *proposed = hero_ui_win_wide(name, name_count, 0);
    wchar_t *answer = hero_layer_alloc_zero((size_t)HERO_UI_WIN_PATHS_ROOM, sizeof(wchar_t));
    wchar_t *spelled = NULL;
    wchar_t extension[64];
    int64_t texts = hero_ui_texts_in(filters, filters_count);
    uint8_t *paths = NULL;
    int64_t paths_count = 0;
    int64_t paths_room = 0;
    BOOL chosen;
    DWORD code;
    int64_t result = HERO_UI_OK;

    *bytes = 0;
    extension[0] = 0;
    if (!hero_ui_win_owner(window, &owner, &refused)) {
        hero_layer_free(heading);
        hero_layer_free(proposed);
        hero_layer_free(answer);
        return refused;
    }
    if (texts >= 2) {
        /* Each filter as Win32 takes it: its name, then each extension as
         * `*.ext`, `;` between, each followed by a zero, and one more after
         * the last. An extension written with its dot keeps one dot. */
        size_t room = (size_t)filters_count * 3 + 4;
        size_t at = 0;

        spelled = hero_layer_alloc(room * sizeof(wchar_t));
        for (int64_t f = 0; spelled != NULL && f + 1 < texts; f += 2) {
            int64_t name_length = 0;
            int64_t list_length = 0;
            const uint8_t *filter_name = hero_ui_text_at(filters, filters_count, f, &name_length);
            const uint8_t *list = hero_ui_text_at(filters, filters_count, f + 1, &list_length);
            wchar_t *wide_name = hero_ui_win_wide(filter_name, name_length, 0);
            wchar_t *wide_list = hero_ui_win_wide(list, list_length, 0);

            if (wide_name == NULL || wide_list == NULL) {
                hero_layer_free(wide_name);
                hero_layer_free(wide_list);
                hero_layer_free(spelled);
                spelled = NULL;
                break;
            }
            for (size_t c = 0; wide_name[c] != 0; c += 1) {
                spelled[at++] = wide_name[c];
            }
            spelled[at++] = 0;
            for (size_t c = 0, start = 0;; c += 1) {
                if (wide_list[c] == L';' || wide_list[c] == 0) {
                    size_t from = start < c && wide_list[start] == L'.' ? start + 1 : start;

                    if (from < c) {
                        if (spelled[at - 1] != 0) {
                            spelled[at++] = L';';
                        }
                        spelled[at++] = L'*';
                        spelled[at++] = L'.';
                        for (size_t e = from; e < c; e += 1) {
                            spelled[at++] = wide_list[e];
                        }
                        if (extension[0] == 0 && c - from < 63) {
                            memcpy(extension, wide_list + from, (c - from) * sizeof(wchar_t));
                            extension[c - from] = 0;
                        }
                    }
                    start = c + 1;
                }
                if (wide_list[c] == 0) {
                    break;
                }
            }
            if (spelled[at - 1] == 0) {
                spelled[at++] = L'*';
                spelled[at++] = L'.';
                spelled[at++] = L'*';
            }
            spelled[at++] = 0;
            hero_layer_free(wide_name);
            hero_layer_free(wide_list);
        }
        if (spelled != NULL) {
            spelled[at] = 0;
        }
    }
    if (heading == NULL || proposed == NULL || answer == NULL || (texts >= 2 && spelled == NULL)) {
        hero_layer_free(heading);
        hero_layer_free(proposed);
        hero_layer_free(answer);
        hero_layer_free(spelled);
        return HERO_UI_LIMIT;
    }
    answer[0] = 0;
    if (save == 1 && wcslen(proposed) < HERO_UI_WIN_PATHS_ROOM) {
        memcpy(answer, proposed, (wcslen(proposed) + 1) * sizeof(wchar_t));
    }
    memset(&request, 0, sizeof request);
    request.lStructSize = (DWORD)sizeof request;
    request.hwndOwner = owner;
    request.lpstrFilter = spelled;
    request.nFilterIndex = spelled != NULL ? 1 : 0;
    request.lpstrFile = answer;
    request.nMaxFile = HERO_UI_WIN_PATHS_ROOM;
    request.lpstrTitle = heading[0] != 0 ? heading : NULL;
    request.lpstrDefExt = save == 1 && extension[0] != 0 ? extension : NULL;
    request.Flags = OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | (save == 1 ? OFN_OVERWRITEPROMPT : OFN_FILEMUSTEXIST | (multiple == 1 ? OFN_ALLOWMULTISELECT : 0));
    chosen = save == 1 ? GetSaveFileNameW(&request) : GetOpenFileNameW(&request);
    code = CommDlgExtendedError();
    if (chosen) {
        /* Several files chosen are their folder, then each name, each
         * followed by a zero, and one more after the last; one file is its
         * whole path. */
        size_t first = wcslen(answer);
        bool several = multiple == 1 && save == 0 && answer[first + 1] != 0;
        bool kept = true;

        if (!several) {
            kept = hero_ui_win_keep_path(answer, (int)first, &paths, &paths_count, &paths_room);
        }
        for (size_t at = first + 1; several && kept && answer[at] != 0; at += wcslen(answer + at) + 1) {
            size_t length = wcslen(answer + at);
            wchar_t *joined = hero_layer_alloc((first + length + 2) * sizeof(wchar_t));

            if (joined == NULL) {
                kept = false;
                break;
            }
            memcpy(joined, answer, first * sizeof(wchar_t));
            joined[first] = first > 0 && answer[first - 1] == L'\\' ? 0 : L'\\';
            memcpy(joined + first + (joined[first] == 0 ? 0 : 1), answer + at, length * sizeof(wchar_t));
            kept = hero_ui_win_keep_path(joined, (int)(first + length + (joined[first] == 0 ? 0 : 1)), &paths, &paths_count, &paths_room);
            hero_layer_free(joined);
        }
        if (!kept) {
            hero_layer_free(paths);
            paths = NULL;
            paths_count = 0;
            result = HERO_UI_LIMIT;
        }
    } else if (code != 0) {
        result = code == FNERR_BUFFERTOOSMALL ? HERO_UI_LIMIT : hero_ui_win_failed("showing a file dialog: the common dialog's error", code);
    }
    hero_layer_free(heading);
    hero_layer_free(proposed);
    hero_layer_free(answer);
    hero_layer_free(spelled);
    if (result != HERO_UI_OK) {
        return result;
    }
    hero_layer_free(hero_ui_win_paths);
    hero_ui_win_paths = paths;
    hero_ui_win_paths_count = paths_count;
    *bytes = paths_count;
    return HERO_UI_OK;
}

static int64_t hero_ui_arm_dialog_paths(uint8_t *buffer, int64_t count) {
    if (hero_ui_win_paths_count > 0 && hero_ui_win_paths_count <= count) {
        memcpy(buffer, hero_ui_win_paths, (size_t)hero_ui_win_paths_count);
    }
    return hero_ui_win_paths_count;
}

#else
/* THE ARM OF A SYSTEM NOT WRITTEN YET: no native side, so `native_here`
 * answers 0, `open` refuses `HERO_UI_NATIVE` with `HERO_UI_UNSUPPORTED`, and
 * no other arm function is ever reached; each answers `UNSUPPORTED` all the
 * same, so a call that did reach one would be a value. */

static int64_t hero_ui_arm_here(void) {
    return 0;
}

static void hero_ui_arm_absent(char *words, size_t count) {
    (void)hero_ui_copy_text(words, count, "no arm of this layer draws this system's widgets");
}

static int64_t hero_ui_arm_open(void) {
    /* Named so it is not an unused function here; only an arm that draws
     * hands it to its system. */
    (void)hero_ui_push;
    return HERO_UI_UNSUPPORTED;
}

static void hero_ui_arm_close(void) {
}

static int64_t hero_ui_arm_words(char *words, int64_t count) {
    (void)words;
    (void)count;
    return 0;
}

static int64_t hero_ui_arm_create(int64_t id, int64_t kind, int64_t parent) {
    (void)id;
    (void)kind;
    (void)parent;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_destroy(int64_t id) {
    (void)id;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_set_text(int64_t id, const uint8_t *text, int64_t count) {
    (void)id;
    (void)text;
    (void)count;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_set_items(int64_t id, const uint8_t *items, int64_t count) {
    (void)id;
    (void)items;
    (void)count;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_set_value(int64_t id, int64_t value) {
    (void)id;
    (void)value;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_set_range(int64_t id, int64_t least, int64_t most) {
    (void)id;
    (void)least;
    (void)most;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_set_selection(int64_t id, const int64_t *indices, int64_t count) {
    (void)id;
    (void)indices;
    (void)count;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_set_flag(int64_t id, int64_t which, int64_t on) {
    (void)id;
    (void)which;
    (void)on;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_set_shortcut(int64_t id, const uint8_t *key, int64_t count, int64_t modifiers) {
    (void)id;
    (void)key;
    (void)count;
    (void)modifiers;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_set_image(int64_t id, const uint8_t *rgba, int64_t width, int64_t height) {
    (void)id;
    (void)rgba;
    (void)width;
    (void)height;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_set_size(int64_t id, int64_t which, int64_t width, int64_t height) {
    (void)id;
    (void)which;
    (void)width;
    (void)height;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_place(int64_t id, int64_t index, int64_t x, int64_t y, int64_t width, int64_t height) {
    (void)id;
    (void)index;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_preferred(int64_t id, int64_t *width, int64_t *height) {
    (void)id;
    (void)width;
    (void)height;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_text(int64_t id, uint8_t *buffer, int64_t count, int64_t *length) {
    (void)id;
    (void)buffer;
    (void)count;
    (void)length;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_value(int64_t id, int64_t *value) {
    (void)id;
    (void)value;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_selection(int64_t id, int64_t *indices, int64_t count, int64_t *chosen) {
    (void)id;
    (void)indices;
    (void)count;
    (void)chosen;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_pump(int64_t ms) {
    (void)ms;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_inject(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count) {
    (void)kind;
    (void)id;
    (void)value;
    (void)second;
    (void)text;
    (void)count;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_message(int64_t window, int64_t tone, int64_t buttons, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count, int64_t *answer) {
    (void)window;
    (void)tone;
    (void)buttons;
    (void)title;
    (void)title_count;
    (void)text;
    (void)text_count;
    (void)answer;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_file_dialog(int64_t window, int64_t save, int64_t multiple, const uint8_t *title, int64_t title_count, const uint8_t *name, int64_t name_count, const uint8_t *filters, int64_t filters_count, int64_t *bytes) {
    (void)window;
    (void)save;
    (void)multiple;
    (void)title;
    (void)title_count;
    (void)name;
    (void)name_count;
    (void)filters;
    (void)filters_count;
    (void)bytes;
    return HERO_UI_UNSUPPORTED;
}

static int64_t hero_ui_arm_dialog_paths(uint8_t *buffer, int64_t count) {
    (void)buffer;
    (void)count;
    return -1;
}

#endif

/* -- THE THREAD ------------------------------------------------------------ */

#if defined(_WIN32)
/* The thread that opened the process's first app, 0 before it. */
static _Atomic uint32_t hero_ui_owner;
#endif

/* Whether the calling thread is the UI thread, or, on Windows before the
 * first app, may become it. */
static bool hero_ui_thread_is_ours(void) {
#if defined(__APPLE__)
    return pthread_main_np() != 0;
#elif defined(_WIN32)
    uint32_t owner = atomic_load(&hero_ui_owner);
    return owner == 0 || owner == (uint32_t)GetCurrentThreadId();
#else
    return (long)syscall(SYS_gettid) == (long)getpid();
#endif
}

/* Makes the calling thread the UI thread where none is yet, on Windows; and
 * answers whether it is the UI thread. */
static bool hero_ui_thread_claimed(void) {
#if defined(_WIN32)
    uint32_t none = 0;
    uint32_t me = (uint32_t)GetCurrentThreadId();

    if (atomic_compare_exchange_strong(&hero_ui_owner, &none, me)) {
        return true;
    }
    return none == me;
#else
    return hero_ui_thread_is_ours();
#endif
}

int64_t hero_ui_on_ui_thread(void) {
    return hero_ui_thread_is_ours() ? 1 : 0;
}

int64_t hero_ui_native_here(void) {
    return hero_ui_arm_here() == 1 ? 1 : 0;
}

/* -- THE TABLE OF APPS ----------------------------------------------------- */

/* One event of a queue: its kind, its widget, its two values, and the text
 * a web view's message carries, the layer's own copy. */
typedef struct hero_ui_event {
    int64_t kind;
    int64_t id;
    int64_t value;
    int64_t second;
    uint8_t *text;
    int64_t text_count;
} hero_ui_event;

typedef struct hero_ui_slot {
    uint64_t life;
    /* The next slot given back, its index plus one, 0 for none. */
    uint32_t next;
    bool taken;
    int64_t backend;
    int64_t number;
    /* The ring of events, `HERO_UI_QUEUE_MOST` of them, NULL until the
     * first; `head` is the oldest, `held` how many. */
    hero_ui_event *ring;
    int64_t head;
    int64_t held;
    int64_t lost;
    /* The text of the event `next` took last, given back at the next. */
    uint8_t *last_text;
    int64_t last_text_count;
} hero_ui_slot;

#define HERO_UI_INDEX_BITS 6
#define HERO_UI_SLOTS 64u
#define HERO_UI_LIFE_MOST HERO_HANDLE_LIFE_MOST(HERO_UI_INDEX_BITS)

/* The UI thread's own table (THE THREAD, above). */
static _Thread_local hero_ui_slot hero_ui_slots[HERO_UI_SLOTS];
static _Thread_local uint32_t hero_ui_fresh;
static _Thread_local uint32_t hero_ui_given_back;
/* The slot of the native app, its index plus one, 0 for none. */
static _Thread_local uint32_t hero_ui_native_slot;
/* How many native apps are open in the process: 0 or 1. */
static _Atomic int64_t hero_ui_natives;
/* The last number an app was given. */
static _Atomic int64_t hero_ui_numbers;

/* A slot for a new app, or NULL. */
static hero_ui_slot *hero_ui_take_slot(uint32_t *index_out) {
    uint32_t index;
    hero_ui_slot *slot;

    if (hero_ui_given_back != 0) {
        index = hero_ui_given_back - 1;
        slot = &hero_ui_slots[index];
        hero_ui_given_back = slot->next;
    } else if (hero_ui_fresh < HERO_UI_SLOTS) {
        index = hero_ui_fresh;
        hero_ui_fresh += 1;
        slot = &hero_ui_slots[index];
    } else {
        return NULL;
    }
    *index_out = index;
    return slot;
}

/* The slot `app` names while it holds the life it was issued for, or NULL. */
static hero_ui_slot *hero_ui_slot_of(hero_ui_app *app) {
    uint64_t value = (uint64_t)(uintptr_t)app;
    uint64_t index = value & ((UINT64_C(1) << HERO_UI_INDEX_BITS) - 1);
    uint64_t life = (value & ~HERO_HANDLE_TAG_MASK) >> HERO_UI_INDEX_BITS;
    hero_ui_slot *slot;

    if ((value & HERO_HANDLE_TAG_MASK) != HERO_HANDLE_TAG(HERO_HANDLE_TAG_UI) || index == 0 || index > HERO_UI_SLOTS || life > HERO_UI_LIFE_MOST) {
        return NULL;
    }
    slot = &hero_ui_slots[index - 1];
    if (!slot->taken || slot->life != life) {
        return NULL;
    }
    return slot;
}

/* Gives back everything `slot` holds, and the slot. */
static void hero_ui_give_back(hero_ui_slot *slot) {
    if (slot->ring != NULL) {
        for (int64_t i = 0; i < slot->held; i += 1) {
            hero_layer_free(slot->ring[(slot->head + i) % (int64_t)HERO_UI_QUEUE_MOST].text);
        }
        hero_layer_free(slot->ring);
    }
    hero_layer_free(slot->last_text);
    slot->ring = NULL;
    slot->head = 0;
    slot->held = 0;
    slot->lost = 0;
    slot->last_text = NULL;
    slot->last_text_count = 0;
    slot->taken = false;
    if (slot->life < HERO_UI_LIFE_MOST) {
        slot->next = hero_ui_given_back;
        hero_ui_given_back = (uint32_t)(slot - hero_ui_slots) + 1;
    }
}

/* The slot `app` names, on the UI thread; NULL with the failure said. */
static hero_ui_slot *hero_ui_enter(hero_ui_app *app) {
    hero_ui_slot *slot;

    if (!hero_ui_thread_is_ours()) {
        (void)hero_ui_said(HERO_UI_WRONG_THREAD, "the user interface lives on the main thread, and this call came from another");
        return NULL;
    }
    slot = hero_ui_slot_of(app);
    if (slot == NULL) {
        (void)hero_ui_said(HERO_UI_NOT_OPEN, "the app is closed, or was never one");
        return NULL;
    }
    hero_ui_fine();
    return slot;
}

/* The slot of `app` where it is the native app; NULL with the failure
 * said. */
static hero_ui_slot *hero_ui_enter_native(hero_ui_app *app) {
    hero_ui_slot *slot = hero_ui_enter(app);

    if (slot != NULL && slot->backend != HERO_UI_NATIVE) {
        (void)hero_ui_said(HERO_UI_UNSUPPORTED, "a headless app holds no native widget");
        return NULL;
    }
    return slot;
}

/* -- THE QUEUE ------------------------------------------------------------- */

/* Whether two events of `kind` for one widget with nothing between are
 * queued once, the later's values kept. */
static bool hero_ui_coalesced(int64_t kind) {
    return kind == HERO_UI_EVENT_TEXT || kind == HERO_UI_EVENT_SELECT || kind == HERO_UI_EVENT_VALUE || kind == HERO_UI_EVENT_RESIZE;
}

/* Queues an event on the native app's queue: what an arm calls, on the UI
 * thread, while it runs the system's loop or acts for `inject`. */
static void hero_ui_push(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count) {
    hero_ui_slot *slot;
    hero_ui_event *last;
    hero_ui_event *into;
    uint8_t *copy = NULL;

    if (hero_ui_native_slot == 0 || !hero_ui_thread_is_ours()) {
        return;
    }
    slot = &hero_ui_slots[hero_ui_native_slot - 1];
    if (slot->ring == NULL) {
        slot->ring = hero_layer_alloc_zero(HERO_UI_QUEUE_MOST, sizeof *slot->ring);
        if (slot->ring == NULL) {
            slot->lost += 1;
            return;
        }
    }
    if (slot->held > 0 && hero_ui_coalesced(kind)) {
        last = &slot->ring[(slot->head + slot->held - 1) % (int64_t)HERO_UI_QUEUE_MOST];
        if (last->kind == kind && last->id == id) {
            last->value = value;
            last->second = second;
            return;
        }
    }
    if (slot->held == (int64_t)HERO_UI_QUEUE_MOST) {
        slot->lost += 1;
        return;
    }
    if (text != NULL && count > 0 && count <= HERO_UI_TEXT_MOST) {
        copy = hero_layer_alloc((size_t)count);
        if (copy == NULL) {
            slot->lost += 1;
            return;
        }
        memcpy(copy, text, (size_t)count);
    }
    into = &slot->ring[(slot->head + slot->held) % (int64_t)HERO_UI_QUEUE_MOST];
    into->kind = kind;
    into->id = id;
    into->value = value;
    into->second = second;
    into->text = copy;
    into->text_count = copy == NULL ? 0 : count;
    slot->held += 1;
}

/* -- WHAT EVERY ENTRY POINT CHECKS ------------------------------------------ */

/* Whether the `length` bytes at `text` are UTF-8: no byte that begins no
 * character, no character cut, no longer form than its shortest, no
 * surrogate, nothing past U+10FFFF. */
static bool hero_ui_is_utf8(const uint8_t *text, uint64_t length) {
    uint64_t at = 0;

    while (at < length) {
        uint8_t c = text[at];
        uint64_t more;
        uint32_t code;
        uint32_t least;

        if (c < 0x80u) {
            at += 1;
            continue;
        }
        if (c >= 0xC2u && c <= 0xDFu) {
            more = 1;
            code = c & 0x1Fu;
            least = 0x80u;
        } else if (c >= 0xE0u && c <= 0xEFu) {
            more = 2;
            code = c & 0x0Fu;
            least = 0x800u;
        } else if (c >= 0xF0u && c <= 0xF4u) {
            more = 3;
            code = c & 0x07u;
            least = 0x10000u;
        } else {
            return false;
        }
        if (length - at - 1 < more) {
            return false;
        }
        for (uint64_t i = 1; i <= more; i += 1) {
            if ((text[at + i] & 0xC0u) != 0x80u) {
                return false;
            }
            code = (code << 6) | (text[at + i] & 0x3Fu);
        }
        if (code < least || code > 0x10FFFFu || (code >= 0xD800u && code <= 0xDFFFu)) {
            return false;
        }
        at += more + 1;
    }
    return true;
}

/* Whether `count` bytes at `text` are a text a widget holds, else the
 * failure said, naming `what`; `list` lets a zero byte end each text, the
 * last byte then being one. */
static bool hero_ui_text_given(const void *text, uint64_t count, const char *what, bool list) {
    const uint8_t *bytes = text;
    char words[HERO_UI_WORDS_MOST];

    if (count > 0 && text == NULL) {
        (void)snprintf(words, sizeof words, "%s: %llu bytes were named and none given", what, (unsigned long long)count);
        (void)hero_ui_said(HERO_UI_INVALID, words);
        return false;
    }
    if (count > (uint64_t)HERO_UI_TEXT_MOST) {
        (void)snprintf(words, sizeof words, "%s takes %llu bytes, past the %d a widget holds", what, (unsigned long long)count, HERO_UI_TEXT_MOST);
        (void)hero_ui_said(HERO_UI_INVALID, words);
        return false;
    }
    if (list) {
        uint64_t start = 0;

        if (count > 0 && bytes[count - 1] != 0) {
            (void)snprintf(words, sizeof words, "%s: the last text is not followed by a zero byte", what);
            (void)hero_ui_said(HERO_UI_INVALID, words);
            return false;
        }
        for (uint64_t at = 0; at < count; at += 1) {
            if (bytes[at] == 0) {
                if (!hero_ui_is_utf8(bytes + start, at - start)) {
                    (void)snprintf(words, sizeof words, "%s: the text at byte %llu is no UTF-8", what, (unsigned long long)start);
                    (void)hero_ui_said(HERO_UI_INVALID, words);
                    return false;
                }
                start = at + 1;
            }
        }
        return true;
    }
    if (count > 0 && memchr(bytes, 0, (size_t)count) != NULL) {
        (void)snprintf(words, sizeof words, "%s holds a zero byte, which no widget shows", what);
        (void)hero_ui_said(HERO_UI_INVALID, words);
        return false;
    }
    if (count > 0 && !hero_ui_is_utf8(bytes, count)) {
        (void)snprintf(words, sizeof words, "%s is no UTF-8", what);
        (void)hero_ui_said(HERO_UI_INVALID, words);
        return false;
    }
    return true;
}

/* Whether `id` is a widget number, else the failure said. */
static bool hero_ui_id_given(int64_t id) {
    char words[HERO_UI_WORDS_MOST];

    if (id < 1) {
        (void)snprintf(words, sizeof words, "the widget number %lld is below 1", (long long)id);
        (void)hero_ui_said(HERO_UI_INVALID, words);
        return false;
    }
    return true;
}

/* Whether `width` and `height` are each 0 or more, else the failure said. */
static bool hero_ui_sizes_given(int64_t width, int64_t height) {
    char words[HERO_UI_WORDS_MOST];

    if (width < 0 || height < 0) {
        (void)snprintf(words, sizeof words, "a size of %lld by %lld; a size is 0 or more", (long long)width, (long long)height);
        (void)hero_ui_said(HERO_UI_INVALID, words);
        return false;
    }
    return true;
}

/* The arm's answer `kind` for the call on widget `id` that `what` names, as
 * the entry point answers it: 0 where done, else -1 with the failure said,
 * the system's own words after `what` for `HERO_UI_OTHER`. */
static int64_t hero_ui_answered(int64_t kind, const char *what, int64_t id) {
    char words[HERO_UI_WORDS_MOST];
    char theirs[HERO_UI_WORDS_MOST];

    if (kind == HERO_UI_OK) {
        hero_ui_fine();
        return 0;
    }
    if (kind == HERO_UI_NOT_FOUND) {
        (void)snprintf(words, sizeof words, "%s: the native side holds no widget %lld, or holds one already where it was to be made", what, (long long)id);
    } else if (kind == HERO_UI_INVALID) {
        (void)snprintf(words, sizeof words, "%s: widget %lld is not of a kind this call sets, or the value is outside its range", what, (long long)id);
    } else if (kind == HERO_UI_UNSUPPORTED) {
        /* The arm's own words where it gave some: which library or which
         * part of the system is missing. */
        theirs[0] = '\0';
        if (hero_ui_arm_words(theirs, (int64_t)sizeof theirs) <= 0) {
            (void)snprintf(theirs, sizeof theirs, "this system's native side offers no such thing");
        }
        (void)snprintf(words, sizeof words, "%s: %s", what, theirs);
    } else if (kind == HERO_UI_LIMIT) {
        (void)snprintf(words, sizeof words, "%s: the system has no memory left for it", what);
    } else {
        theirs[0] = '\0';
        if (hero_ui_arm_words(theirs, (int64_t)sizeof theirs) <= 0) {
            (void)snprintf(theirs, sizeof theirs, "the system refused, giving no words");
        }
        (void)snprintf(words, sizeof words, "%s: %s", what, theirs);
        kind = HERO_UI_OTHER;
    }
    return hero_ui_said(kind, words);
}

/* -- THE ENTRY POINTS ------------------------------------------------------ */

hero_ui_app *hero_ui_open(int64_t backend) {
    hero_ui_slot *slot;
    uint32_t index;
    char words[HERO_UI_WORDS_MOST];

    if (backend != HERO_UI_HEADLESS && backend != HERO_UI_NATIVE) {
        (void)snprintf(words, sizeof words, "the backend %lld is neither HERO_UI_HEADLESS nor HERO_UI_NATIVE", (long long)backend);
        (void)hero_ui_said(HERO_UI_INVALID, words);
        return NULL;
    }
    if (!hero_ui_thread_claimed()) {
        (void)hero_ui_said(HERO_UI_WRONG_THREAD, "an app opens only on the main thread, the one main runs on");
        return NULL;
    }
    if (backend == HERO_UI_NATIVE) {
        if (atomic_load(&hero_ui_natives) != 0) {
            (void)hero_ui_said(HERO_UI_BUSY, "a native app is open already in this process");
            return NULL;
        }
        if (hero_ui_arm_here() != 1) {
            char why[HERO_UI_WORDS_MOST];

            hero_ui_arm_absent(why, sizeof why);
            (void)snprintf(words, sizeof words, "this machine offers no native user interface to this session: %s", why);
            (void)hero_ui_said(HERO_UI_UNSUPPORTED, words);
            return NULL;
        }
    }
    slot = hero_ui_take_slot(&index);
    if (slot == NULL) {
        (void)snprintf(words, sizeof words, "the layer holds as many apps as it can, %u", HERO_UI_SLOTS);
        (void)hero_ui_said(HERO_UI_LIMIT, words);
        return NULL;
    }
    slot->life += 1;
    slot->next = 0;
    slot->taken = true;
    slot->backend = backend;
    slot->number = atomic_fetch_add(&hero_ui_numbers, 1) + 1;
    slot->ring = NULL;
    slot->head = 0;
    slot->held = 0;
    slot->lost = 0;
    slot->last_text = NULL;
    slot->last_text_count = 0;
    if (backend == HERO_UI_NATIVE) {
        int64_t kind;

        hero_ui_native_slot = index + 1;
        atomic_store(&hero_ui_natives, 1);
        kind = hero_ui_arm_open();
        if (kind != HERO_UI_OK) {
            hero_ui_native_slot = 0;
            atomic_store(&hero_ui_natives, 0);
            hero_ui_give_back(slot);
            (void)hero_ui_answered(kind, "opening the native side", 0);
            return NULL;
        }
    }
    hero_ui_fine();
    return (hero_ui_app *)(uintptr_t)(HERO_HANDLE_TAG(HERO_HANDLE_TAG_UI) | (slot->life << HERO_UI_INDEX_BITS) | ((uint64_t)index + 1));
}

int64_t hero_ui_close(hero_ui_app *app) {
    hero_ui_slot *slot = hero_ui_enter(app);

    if (slot == NULL) {
        return -1;
    }
    if (slot->backend == HERO_UI_NATIVE) {
        hero_ui_arm_close();
        hero_ui_native_slot = 0;
        atomic_store(&hero_ui_natives, 0);
    }
    hero_ui_give_back(slot);
    return 0;
}

int64_t hero_ui_number(hero_ui_app *app) {
    hero_ui_slot *slot = hero_ui_enter(app);

    return slot == NULL ? -1 : slot->number;
}

int64_t hero_ui_create(hero_ui_app *app, int64_t id, int64_t kind, int64_t parent) {
    char words[HERO_UI_WORDS_MOST];

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (kind < HERO_UI_WINDOW || kind > HERO_UI_WEBVIEW) {
        (void)snprintf(words, sizeof words, "the kind %lld is no kind the native side makes", (long long)kind);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    if ((kind == HERO_UI_WINDOW) != (parent == 0) || parent < 0) {
        (void)snprintf(words, sizeof words, "a widget of kind %lld in parent %lld: a window has none, every other kind one", (long long)kind, (long long)parent);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    return hero_ui_answered(hero_ui_arm_create(id, kind, parent), "making a widget", id);
}

int64_t hero_ui_destroy(hero_ui_app *app, int64_t id) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    return hero_ui_answered(hero_ui_arm_destroy(id), "destroying a widget", id);
}

int64_t hero_ui_set_text(hero_ui_app *app, int64_t id, const void *text, uint64_t count) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_text_given(text, count, "the text", false)) {
        return -1;
    }
    return hero_ui_answered(hero_ui_arm_set_text(id, text, (int64_t)count), "setting a text", id);
}

int64_t hero_ui_set_items(hero_ui_app *app, int64_t id, const void *items, uint64_t count) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_text_given(items, count, "the items", true)) {
        return -1;
    }
    return hero_ui_answered(hero_ui_arm_set_items(id, items, (int64_t)count), "setting the items", id);
}

int64_t hero_ui_set_value(hero_ui_app *app, int64_t id, int64_t value) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    return hero_ui_answered(hero_ui_arm_set_value(id, value), "setting a value", id);
}

int64_t hero_ui_set_range(hero_ui_app *app, int64_t id, int64_t least, int64_t most) {
    char words[HERO_UI_WORDS_MOST];

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (least >= most) {
        (void)snprintf(words, sizeof words, "a range from %lld to %lld; its lowest value is below its highest", (long long)least, (long long)most);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    return hero_ui_answered(hero_ui_arm_set_range(id, least, most), "setting a range", id);
}

int64_t hero_ui_set_selection(hero_ui_app *app, int64_t id, const int64_t *indices, uint64_t count) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if ((count > 0 && indices == NULL) || count > (uint64_t)HERO_UI_TEXT_MOST) {
        return hero_ui_said(HERO_UI_INVALID, "the indices chosen were named and not given, or are too many");
    }
    for (uint64_t i = 0; i < count; i += 1) {
        if (indices[i] < 0 || (i > 0 && indices[i] <= indices[i - 1])) {
            return hero_ui_said(HERO_UI_INVALID, "the indices chosen are not ascending, each once and 0 or more");
        }
    }
    return hero_ui_answered(hero_ui_arm_set_selection(id, indices, (int64_t)count), "choosing items", id);
}

/* One of the three flags of widget `id`, `on` 0 or 1. */
static int64_t hero_ui_flag(hero_ui_app *app, int64_t id, int64_t which, int64_t on, const char *what) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (on != 0 && on != 1) {
        return hero_ui_said(HERO_UI_INVALID, "a flag is 0 or 1");
    }
    return hero_ui_answered(hero_ui_arm_set_flag(id, which, on), what, id);
}

int64_t hero_ui_set_enabled(hero_ui_app *app, int64_t id, int64_t on) {
    return hero_ui_flag(app, id, HERO_UI_FLAG_ENABLED, on, "enabling a widget");
}

int64_t hero_ui_set_visible(hero_ui_app *app, int64_t id, int64_t on) {
    return hero_ui_flag(app, id, HERO_UI_FLAG_VISIBLE, on, "showing a widget");
}

int64_t hero_ui_set_resizable(hero_ui_app *app, int64_t id, int64_t on) {
    return hero_ui_flag(app, id, HERO_UI_FLAG_RESIZABLE, on, "letting a window resize");
}

int64_t hero_ui_set_shortcut(hero_ui_app *app, int64_t id, const void *key, uint64_t count, int64_t modifiers) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_text_given(key, count, "the key", false)) {
        return -1;
    }
    if (modifiers < 0 || modifiers > (HERO_UI_PRIMARY | HERO_UI_SHIFT | HERO_UI_ALT)) {
        return hero_ui_said(HERO_UI_INVALID, "the keys held are HERO_UI_PRIMARY, HERO_UI_SHIFT and HERO_UI_ALT added");
    }
    return hero_ui_answered(hero_ui_arm_set_shortcut(id, key, (int64_t)count, modifiers), "setting a shortcut", id);
}

int64_t hero_ui_set_image(hero_ui_app *app, int64_t id, const void *rgba, uint64_t count, int64_t width, int64_t height) {
    uint64_t pixels;
    char words[HERO_UI_WORDS_MOST];

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_sizes_given(width, height)) {
        return -1;
    }
    if (__builtin_mul_overflow((uint64_t)width, (uint64_t)height, &pixels) || __builtin_mul_overflow(pixels, (uint64_t)4, &pixels) || pixels != count || (count > 0 && rgba == NULL)) {
        (void)snprintf(words, sizeof words, "%llu bytes of pixels for an image of %lld by %lld, which takes 4 a pixel", (unsigned long long)count, (long long)width, (long long)height);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    return hero_ui_answered(hero_ui_arm_set_image(id, rgba, width, height), "setting an image", id);
}

int64_t hero_ui_set_size(hero_ui_app *app, int64_t id, int64_t width, int64_t height) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_sizes_given(width, height)) {
        return -1;
    }
    return hero_ui_answered(hero_ui_arm_set_size(id, HERO_UI_SIZE_CONTENT, width, height), "sizing a window", id);
}

int64_t hero_ui_set_min_size(hero_ui_app *app, int64_t id, int64_t width, int64_t height) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_sizes_given(width, height)) {
        return -1;
    }
    return hero_ui_answered(hero_ui_arm_set_size(id, HERO_UI_SIZE_LEAST, width, height), "setting a window's least size", id);
}

int64_t hero_ui_place(hero_ui_app *app, int64_t id, int64_t x, int64_t y, int64_t width, int64_t height) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_sizes_given(width, height)) {
        return -1;
    }
    return hero_ui_answered(hero_ui_arm_place(id, -1, x, y, width, height), "placing a widget", id);
}

int64_t hero_ui_place_item(hero_ui_app *app, int64_t id, int64_t index, int64_t x, int64_t y, int64_t width, int64_t height) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_sizes_given(width, height)) {
        return -1;
    }
    if (index < 0) {
        return hero_ui_said(HERO_UI_INVALID, "an item's index is 0 or more");
    }
    return hero_ui_answered(hero_ui_arm_place(id, index, x, y, width, height), "placing a radio group's item", id);
}

int64_t hero_ui_preferred(hero_ui_app *app, int64_t id, int64_t *width, int64_t *height) {
    int64_t w = 0;
    int64_t h = 0;
    int64_t answer;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (width == NULL || height == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "no place was given for the size");
    }
    answer = hero_ui_answered(hero_ui_arm_preferred(id, &w, &h), "measuring a widget", id);
    if (answer == 0) {
        *width = w < 0 ? 0 : w;
        *height = h < 0 ? 0 : h;
    }
    return answer;
}

int64_t hero_ui_text(hero_ui_app *app, int64_t id, void *buffer, uint64_t count) {
    int64_t length = 0;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (count > 0 && buffer == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "room was named for the text and none given");
    }
    if (count > (uint64_t)HERO_UI_TEXT_MOST) {
        count = (uint64_t)HERO_UI_TEXT_MOST;
    }
    if (hero_ui_answered(hero_ui_arm_text(id, buffer, (int64_t)count, &length), "reading a text", id) != 0) {
        return -1;
    }
    return length;
}

int64_t hero_ui_value(hero_ui_app *app, int64_t id, int64_t *value) {
    int64_t got = 0;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (value == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "no place was given for the value");
    }
    if (hero_ui_answered(hero_ui_arm_value(id, &got), "reading a value", id) != 0) {
        return -1;
    }
    *value = got;
    return 0;
}

int64_t hero_ui_selection(hero_ui_app *app, int64_t id, int64_t *indices, uint64_t count) {
    int64_t chosen = 0;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (count > 0 && indices == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "room was named for the indices and none given");
    }
    if (count > (uint64_t)HERO_UI_TEXT_MOST) {
        count = (uint64_t)HERO_UI_TEXT_MOST;
    }
    if (hero_ui_answered(hero_ui_arm_selection(id, indices, (int64_t)count, &chosen), "reading what is chosen", id) != 0) {
        return -1;
    }
    return chosen;
}

int64_t hero_ui_wait(hero_ui_app *app, int64_t ms) {
    hero_ui_slot *slot = hero_ui_enter(app);
    int64_t kind;

    if (slot == NULL) {
        return -1;
    }
    if (ms < 0) {
        return hero_ui_said(HERO_UI_INVALID, "a wait of fewer than 0 milliseconds");
    }
    if (slot->held > 0 || slot->backend != HERO_UI_NATIVE) {
        return slot->held > 0 ? 1 : 0;
    }
    kind = hero_ui_arm_pump(ms);
    if (kind != HERO_UI_OK) {
        return hero_ui_answered(kind, "running the system's loop", 0);
    }
    return slot->held > 0 ? 1 : 0;
}

int64_t hero_ui_next(hero_ui_app *app, int64_t *kind, int64_t *id, int64_t *value, int64_t *second) {
    hero_ui_slot *slot = hero_ui_enter(app);
    hero_ui_event *oldest;

    if (slot == NULL) {
        return -1;
    }
    if (kind == NULL || id == NULL || value == NULL || second == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "no place was given for the event");
    }
    hero_layer_free(slot->last_text);
    slot->last_text = NULL;
    slot->last_text_count = 0;
    if (slot->held == 0) {
        return 0;
    }
    oldest = &slot->ring[slot->head];
    *kind = oldest->kind;
    *id = oldest->id;
    *value = oldest->value;
    *second = oldest->second;
    slot->last_text = oldest->text;
    slot->last_text_count = oldest->text_count;
    oldest->text = NULL;
    oldest->text_count = 0;
    slot->head = (slot->head + 1) % (int64_t)HERO_UI_QUEUE_MOST;
    slot->held -= 1;
    return 1;
}

int64_t hero_ui_event_text(hero_ui_app *app, void *buffer, uint64_t count) {
    hero_ui_slot *slot = hero_ui_enter(app);

    if (slot == NULL) {
        return -1;
    }
    if (count > 0 && buffer == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "room was named for the text and none given");
    }
    if (slot->last_text_count > 0 && (uint64_t)slot->last_text_count <= count) {
        memcpy(buffer, slot->last_text, (size_t)slot->last_text_count);
    }
    return slot->last_text_count;
}

int64_t hero_ui_lost(hero_ui_app *app) {
    hero_ui_slot *slot = hero_ui_enter(app);

    return slot == NULL ? -1 : slot->lost;
}

int64_t hero_ui_inject(hero_ui_app *app, int64_t kind, int64_t id, int64_t value, int64_t second, const void *text, uint64_t count) {
    char words[HERO_UI_WORDS_MOST];

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_text_given(text, count, "the text typed", false)) {
        return -1;
    }
    if (kind < HERO_UI_EVENT_CLICK || kind > HERO_UI_EVENT_RESIZE) {
        (void)snprintf(words, sizeof words, "the act %lld is no act a person does", (long long)kind);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    if (kind == HERO_UI_EVENT_RESIZE && !hero_ui_sizes_given(value, second)) {
        return -1;
    }
    return hero_ui_answered(hero_ui_arm_inject(kind, id, value, second, text, (int64_t)count), "acting as the person", id);
}

int64_t hero_ui_message(hero_ui_app *app, int64_t window, int64_t tone, int64_t buttons, const void *title, uint64_t title_count, const void *text, uint64_t text_count) {
    int64_t answer = HERO_UI_ANSWER_CANCEL;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_text_given(title, title_count, "the title", false) || !hero_ui_text_given(text, text_count, "the text", false)) {
        return -1;
    }
    if (window < 0 || tone < HERO_UI_TONE_INFO || tone > HERO_UI_TONE_ERROR || buttons < HERO_UI_BUTTONS_OK || buttons > HERO_UI_BUTTONS_YES_NO_CANCEL) {
        return hero_ui_said(HERO_UI_INVALID, "a message box's window is 0 or more, its tone and its buttons hero_ui.h's");
    }
    if (hero_ui_answered(hero_ui_arm_message(window, tone, buttons, title, (int64_t)title_count, text, (int64_t)text_count, &answer), "showing a message box", window) != 0) {
        return -1;
    }
    return answer;
}

int64_t hero_ui_file_dialog(hero_ui_app *app, int64_t window, int64_t save, int64_t multiple, const void *title, uint64_t title_count, const void *name, uint64_t name_count, const void *filters, uint64_t filters_count) {
    int64_t bytes = 0;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_text_given(title, title_count, "the title", false) || !hero_ui_text_given(name, name_count, "the name", false) || !hero_ui_text_given(filters, filters_count, "the filters", true)) {
        return -1;
    }
    if (window < 0 || (save != 0 && save != 1) || (multiple != 0 && multiple != 1)) {
        return hero_ui_said(HERO_UI_INVALID, "a file dialog's window is 0 or more, `save` and `multiple` 0 or 1");
    }
    if (hero_ui_answered(hero_ui_arm_file_dialog(window, save, multiple, title, (int64_t)title_count, name, (int64_t)name_count, filters, (int64_t)filters_count, &bytes), "showing a file dialog", window) != 0) {
        return -1;
    }
    return bytes;
}

int64_t hero_ui_dialog_paths(hero_ui_app *app, void *buffer, uint64_t count) {
    int64_t bytes;

    if (hero_ui_enter_native(app) == NULL) {
        return -1;
    }
    if (count > 0 && buffer == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "room was named for the paths and none given");
    }
    if (count > (uint64_t)HERO_UI_TEXT_MOST) {
        count = (uint64_t)HERO_UI_TEXT_MOST;
    }
    bytes = hero_ui_arm_dialog_paths(buffer, (int64_t)count);
    if (bytes < 0) {
        return hero_ui_said(HERO_UI_OTHER, "no file dialog has answered paths");
    }
    return bytes;
}
