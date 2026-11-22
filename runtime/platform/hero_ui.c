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
 * (Win32's own controls) and the arm of every other system (GTK 4, opened
 * at the first call with `dlopen`) are C in this file, below THE ARM, and
 * call `hero_ui_push` themselves.
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
 * Written from the platforms' documentation: Apple's AppKit reference,
 * Microsoft's Win32 reference, GTK 4's, GLib's and GIO's reference and
 * headers; no other library's source.
 */

#include "hero_ui.h"

#include "hero_handle_tag.h"
#include "hero_layer_alloc.h"
/* The runtime's own number for a thread, the UI owner's on Windows. */
#include "hero_layer_thread.h"
#include "hero_ui_web.h"

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
#include "hero_ui_web_win32.h"
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

static void hero_ui_push(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count);
static bool hero_ui_is_utf8(const uint8_t *text, uint64_t length);
static int64_t hero_ui_web_serve(int64_t id, const uint8_t *method, int64_t method_count, const uint8_t *url, int64_t url_count, int64_t *numbers, const uint8_t **texts);
static int64_t hero_ui_web_ask(int64_t what, int64_t id, const uint8_t *url, int64_t count);
static void hero_ui_web_gone(int64_t id);
static const char *hero_ui_web_bridge(int64_t *count);

#if defined(__APPLE__)

static int64_t hero_ui_arm_here(void) {
    return hero_ui_appkit_here();
}

/* Why `here` answered 0, into `words` of `count` bytes. */
static void hero_ui_arm_absent(char *words, size_t count) {
    (void)hero_ui_copy_text(words, count, "no window server serves this session, as over a remote shell");
}

static int64_t hero_ui_arm_open(void) {
    int64_t count = 0;
    const char *bridge = hero_ui_web_bridge(&count);

    return hero_ui_appkit_open(hero_ui_push, hero_ui_web_serve, hero_ui_web_ask, hero_ui_web_gone, (const uint8_t *)bridge, count);
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

static int64_t hero_ui_arm_web_load(int64_t id, const uint8_t *url, int64_t count) {
    return hero_ui_appkit_web_load(id, url, count);
}

static int64_t hero_ui_arm_web_post(int64_t id, const uint8_t *text, int64_t count) {
    return hero_ui_appkit_web_post(id, text, count);
}

#else

/* -- WHAT THE WIN32 AND GTK ARMS SHARE ----------------------------------------
 *
 * Both arms are C, in this file, so they call `hero_ui_push` themselves
 * through `hero_ui_tell`, and keep their widgets in one table of their own,
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

/* What a web view's member queues a page's message by: `hero_ui_push`,
 * counted as the arm's own events are, so `pump` returns at it rather than
 * at its deadline. */
static void hero_ui_web_told(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count) {
    hero_ui_pushed += 1;
    hero_ui_push(kind, id, value, second, text, count);
}

/* What a web view's member asks (`platform/hero_ui_web.h`): made at each
 * `open`, the UI thread's own, so it outlives every web view it is handed
 * to. */
static _Thread_local hero_ui_web_host hero_ui_web_hosted;

static void hero_ui_make_host(void) {
    int64_t count = 0;
    const char *bridge = hero_ui_web_bridge(&count);

    hero_ui_web_hosted.push = hero_ui_web_told;
    hero_ui_web_hosted.serve = hero_ui_web_serve;
    hero_ui_web_hosted.ask = hero_ui_web_ask;
    hero_ui_web_hosted.gone = hero_ui_web_gone;
    hero_ui_web_hosted.bridge = (const uint8_t *)bridge;
    hero_ui_web_hosted.bridge_count = count;
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

#if defined(_WIN32)
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

/* The WebView2 member's answer `answer`, its words kept where it refused
 * `HERO_UI_OTHER` or `HERO_UI_UNSUPPORTED`, so the failure says which
 * part of WebView2 is missing. */
static int64_t hero_ui_win_web(int64_t answer) {
    char words[HERO_UI_WORDS_MOST];

    if (answer != HERO_UI_OTHER && answer != HERO_UI_UNSUPPORTED) {
        return answer;
    }
    if (hero_ui_web_win32_words(words, (int64_t)sizeof words) <= 0) {
        (void)snprintf(words, sizeof words, "WebView2 refused, giving no words");
    }
    return hero_ui_refused(answer, words);
}

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

    if (widget->kind == HERO_UI_WEBVIEW) {
        /* WebView2 draws into the window's client area where its member
         * puts it, in that window's pixels. */
        if (widget->placed) {
            (void)hero_ui_web_win32_place(widget->id, hero_ui_win_px(widget->x, dpi), hero_ui_win_px(widget->y, dpi), hero_ui_win_px(widget->width, dpi), height);
        }
        (void)hero_ui_web_win32_show(widget->id, widget->placed && widget->visible ? 1 : 0);
        return;
    }
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

        if (widget != NULL && widget->window == window->id && widget->kind != HERO_UI_WINDOW && (widget->hwnd != NULL || widget->kind == HERO_UI_WEBVIEW) && widget->placed) {
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
    hero_ui_make_host();
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
    case HERO_UI_WEBVIEW:
        /* Before its window goes: the member closes WebView2's controller
         * and calls the host's `gone`. */
        (void)hero_ui_web_win32_destroy(widget->id);
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

/* Makes web view `id` in window `window`: THE WEB VIEW, kind 17, is
 * `hero_ui_web_win32.c`'s, which draws WebView2 into the window; this arm
 * keeps its record, its place and whether it is shown. */
static int64_t hero_ui_win_make_web(int64_t id, hero_ui_native *window) {
    hero_ui_native *made = hero_layer_alloc_zero(1, sizeof *made);
    int64_t answer;

    if (made == NULL) {
        return HERO_UI_LIMIT;
    }
    made->id = id;
    made->kind = HERO_UI_WEBVIEW;
    made->parent = window->id;
    made->window = window->id;
    made->visible = true;
    made->enabled = true;
    if (!hero_ui_keep(id, made)) {
        hero_layer_free(made);
        return HERO_UI_LIMIT;
    }
    answer = hero_ui_win_web(hero_ui_web_win32_create(id, &hero_ui_web_hosted, window->hwnd));
    if (answer != HERO_UI_OK) {
        hero_ui_forget(id);
        hero_layer_free(made);
    }
    return answer;
}

static int64_t hero_ui_arm_create(int64_t id, int64_t kind, int64_t parent) {
    hero_ui_native *owner = NULL;
    hero_ui_native *window = NULL;
    hero_ui_native *made;
    int64_t answer = HERO_UI_OK;

    if (hero_ui_find(id) != NULL) {
        return HERO_UI_NOT_FOUND;
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
    if (kind == HERO_UI_WEBVIEW) {
        return hero_ui_win_make_web(id, window);
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
        case HERO_UI_WEBVIEW:
            hero_ui_win_put(widget);
            return HERO_UI_OK;
        default:
            (void)ShowWindow(widget->hwnd, widget->visible && widget->placed ? SW_SHOWNA : SW_HIDE);
            return HERO_UI_OK;
        }
    }
    if (which == HERO_UI_FLAG_ENABLED) {
        /* A window and a web view take no greying, as AppKit's take none. */
        if (widget->kind == HERO_UI_WINDOW || widget->kind == HERO_UI_WEBVIEW) {
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
    if ((widget->hwnd == NULL && widget->kind != HERO_UI_WEBVIEW) || widget->kind == HERO_UI_WINDOW) {
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
    case HERO_UI_WEBVIEW:
        /* The headless side's size of a web view, where nothing else decides
         * it (`ui/internal/sizes.hero`). */
        *width = 320;
        *height = 240;
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
    case HERO_UI_EVENT_MESSAGE:
        /* The page sends `text` by its own bridge, so the message comes as
         * the page's would. */
        answer = widget->kind == HERO_UI_WEBVIEW ? hero_ui_win_web(hero_ui_web_win32_send(id, text, count)) : HERO_UI_INVALID;
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

static int64_t hero_ui_arm_web_load(int64_t id, const uint8_t *url, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    return widget->kind == HERO_UI_WEBVIEW ? hero_ui_win_web(hero_ui_web_win32_load(id, url, count)) : HERO_UI_INVALID;
}

static int64_t hero_ui_arm_web_post(int64_t id, const uint8_t *text, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    return widget->kind == HERO_UI_WEBVIEW ? hero_ui_win_web(hero_ui_web_win32_post(id, text, count)) : HERO_UI_INVALID;
}

#else
/* ==========================================================================
 * THE GTK ARM: GTK 4's own widgets, `GtkButton`, `GtkEntry`, `GtkTextView`,
 * `GtkCheckButton`, `GtkListBox`, `GtkDropDown`, `GtkProgressBar`,
 * `GtkScale`, `GtkPicture`, `GtkLabel`, and GMenu's models in a
 * `GtkPopoverMenuBar`, on every system that is neither Apple's nor Windows.
 *
 * WHY `dlopen`, as `hero_keyring.c` says of libsecret: the layer's archive is
 * compiled for every program a machine builds, so a member that included
 * `<gtk/gtk.h>` would stop every build on a machine without GTK's headers,
 * and one linked `-lgtk-4` every program on one without the library. So the
 * member names no header of GTK, GLib or GIO: it declares the functions it
 * calls with their own signatures (GTK 4's ABI is kept under the one
 * `libgtk-4.so.1`, which needs GLib, GObject, GIO and GDK and so loads them)
 * and the two structures it reads, `GError` and `GtkRequisition`, as their
 * headers lay them out; the first call opens the library once, `RTLD_LOCAL`,
 * and finds every function by `dlsym` on its handle, which searches the
 * library and the libraries it needs. A function GTK 4.0 has missing is the
 * library missing: then `native_here` answers 0 and `open` refuses
 * `HERO_UI_UNSUPPORTED`, naming the library. GTK 4.10's dialogs,
 * `GtkAlertDialog` and `GtkFileDialog`, are used where the library has them
 * and GTK 4.0's `GtkMessageDialog` and `GtkFileChooserNative` where it does
 * not, so the least GTK this arm opens on is 4.0. The library is never
 * closed. Every number below was read from GTK 4.18.6's headers by a program
 * compiled against them, 2026-11-22.
 *
 * WHERE A DISPLAY IS NAMED: `native_here` answers 1 only where `DISPLAY` or
 * `WAYLAND_DISPLAY` names one and the library is there; `open` then asks
 * `gtk_init_check`, which opens it, and refuses `UNSUPPORTED` where it
 * cannot. GTK is told not to set the process's locale first
 * (`gtk_disable_setlocale`), since the runtime prints numbers in C's.
 *
 * EVERY CALL IS ON THE UI THREAD, the process's main thread (THE THREAD),
 * the one GTK's own documentation asks for; `native_here` alone may come from
 * another, and the library is found under a `pthread_once`.
 *
 * A WINDOW is a `GtkWindow` holding a box: the menu bar, shown while a menu
 * is, and the content, an overlay whose child is a drawing area that draws
 * nothing and whose overlay is a `GtkFixed` the widgets stand in at their
 * points. The overlay's own size is its drawing area's, so the widgets'
 * places never keep the person from making the window smaller, and the
 * drawing area's `resize` is the content's new size. GTK 4 sizes a shown
 * window by its default size, which counts the menu bar and a title bar GTK
 * draws itself; each is measured as the window's size less its content's,
 * and a content that took another size than asked while the window took the
 * one asked is given it again.
 *
 * WHAT THE PERSON DOES arrives as each widget's own signal (`clicked`,
 * `toggled`, `changed`, `selected-rows-changed`, `notify::selected`,
 * `value-changed`), a menu item's action's `activate`, a window's
 * `close-request` and its content's `resize`; what the program sets is
 * muted. A signal carries the widget's number, and the widget it came from is
 * asked of the table, so a stale one finds none.
 *
 * WHAT `inject` DOES is what the person's act does: `clicked` emitted on a
 * button, a checkbox's or a radio item's `active` set, an edit's text
 * replaced by the editable's own delete and insert, a list's row selected, a
 * drop-down's item chosen, a scale's value set, a menu item's action
 * activated, `gtk_window_close` (the window manager's close button's own
 * path), and a new default size.
 *
 * A COMBO BOX is a `GtkDropDown`, which always holds a choice while it holds
 * items (its selection chooses its first item itself, measured under Xvfb on
 * GTK 4.18.6). `hero_ui.h` says a combo box may hold none (after `set_items`,
 * and `set_value` of -1), so while it does, its list starts with an empty row,
 * chosen, which is taken away when an item is chosen.
 *
 * MENUS: a window's menus are a GMenu of submenus, each a GMenu of sections
 * between its separators, rebuilt when one of its items changes, with a
 * hidden item left out; each item is an action of the window's group,
 * `hero.item<number>`, disabled where the item, its menu or either's being
 * shown forbids it, and its shortcut a `GtkShortcut` of the window's
 * controller naming that action, shown in the menu by `gtk_accelerator_name`.
 * A label's `_` is written `__`, since a menu reads one `_` as a key's mark.
 *
 * THE LOOP: `pump` runs GLib's default main context until one of its
 * dispatches queues an event or its time passes, waking at its deadline by a
 * timeout source of its own.
 *
 * Written from GTK 4's, GLib's and GIO's documentation and headers; no other
 * library's source.
 * ========================================================================== */

/* GLib's `GError`: a domain, a code and the words. */
typedef struct hero_ui_gerror {
    uint32_t domain;
    int code;
    char *message;
} hero_ui_gerror;

/* GTK's `GtkRequisition`. */
typedef struct hero_ui_gtk_size {
    int width;
    int height;
} hero_ui_gtk_size;

/* GTK's `GtkTextIter`, 80 bytes on every 64-bit target GTK 4.18.6 builds for;
 * held in more room, aligned as its pointers are. */
typedef union hero_ui_gtk_iter {
    void *align;
    unsigned char bytes[256];
} hero_ui_gtk_iter;

/* GTK's numbers this arm hands it, each read from GTK 4.18.6's headers. */
#define HERO_GTK_ORIENTATION_HORIZONTAL 0
#define HERO_GTK_ORIENTATION_VERTICAL 1
#define HERO_GTK_SELECTION_SINGLE 1
#define HERO_GTK_SELECTION_MULTIPLE 3
#define HERO_GTK_POLICY_AUTOMATIC 1
#define HERO_GTK_POLICY_NEVER 2
#define HERO_GTK_WRAP_WORD_CHAR 3
#define HERO_GDK_SHIFT_MASK 1u
#define HERO_GDK_CONTROL_MASK 4u
#define HERO_GDK_ALT_MASK 8u
#define HERO_GTK_MESSAGE_INFO 0
#define HERO_GTK_MESSAGE_WARNING 1
#define HERO_GTK_MESSAGE_ERROR 3
#define HERO_GTK_BUTTONS_NONE 0
#define HERO_GTK_RESPONSE_ACCEPT (-3)
#define HERO_GTK_RESPONSE_OK (-5)
#define HERO_GTK_RESPONSE_CANCEL (-6)
#define HERO_GTK_RESPONSE_YES (-8)
#define HERO_GTK_RESPONSE_NO (-9)
#define HERO_GTK_FILE_CHOOSER_ACTION_OPEN 0
#define HERO_GTK_FILE_CHOOSER_ACTION_SAVE 1
#define HERO_GTK_DIALOG_MODAL 1
#define HERO_GTK_DIALOG_DESTROY_WITH_PARENT 2
#define HERO_GTK_DIALOG_ERROR_CANCELLED 1
#define HERO_GTK_DIALOG_ERROR_DISMISSED 2
#define HERO_GDK_MEMORY_R8G8B8A8 5
#define HERO_GTK_INVALID_LIST_POSITION 0xFFFFFFFFu
/* GDK's key values of the keys `hero_ui.h` names. */
#define HERO_GDK_KEY_F1 0xFFBEu

/* The most a slider or a progress bar is told apart by, where its range is
 * wider than a double holds every integer of: 2^53. */
#define HERO_UI_GTK_EXACT (INT64_C(1) << 53)

/* The functions, found once by `hero_ui_gtk_load` under `hero_ui_gtk_once`,
 * and read only after it. Each pointer is written with the C types GTK's
 * header gives its function: a pointer of GTK's or GLib's as `void *`,
 * `gboolean` and `gint` as `int`, `guint` as `unsigned`, `gsize` and `GType`
 * as `size_t`, `gdouble` as `double`, `gfloat` as `float`. */
typedef void (*hero_ui_gtk_callback)(void);
typedef int (*hero_ui_gtk_source)(void *data);
typedef void (*hero_ui_gtk_ready)(void *source, void *result, void *data);

typedef struct hero_ui_gtk_functions {
    int (*init_check)(void);
    void (*disable_setlocale)(void);
    void *(*window_new)(void);
    void (*window_set_title)(void *window, const char *title);
    void (*window_set_child)(void *window, void *child);
    void (*window_set_default_size)(void *window, int width, int height);
    void (*window_set_resizable)(void *window, int resizable);
    void (*window_present)(void *window);
    void (*window_close)(void *window);
    void (*window_destroy)(void *window);
    void (*widget_set_visible)(void *widget, int visible);
    int (*widget_get_visible)(void *widget);
    void (*widget_set_sensitive)(void *widget, int sensitive);
    int (*widget_is_sensitive)(void *widget);
    void (*widget_set_size_request)(void *widget, int width, int height);
    void (*widget_get_size_request)(void *widget, int *width, int *height);
    void (*widget_get_preferred_size)(void *widget, hero_ui_gtk_size *least, hero_ui_gtk_size *natural);
    void (*widget_set_hexpand)(void *widget, int expand);
    void (*widget_set_vexpand)(void *widget, int expand);
    int (*widget_get_width)(void *widget);
    int (*widget_get_height)(void *widget);
    void (*widget_realize)(void *widget);
    int (*widget_get_realized)(void *widget);
    void (*widget_add_controller)(void *widget, void *controller);
    void (*widget_insert_action_group)(void *widget, const char *prefix, void *group);
    void *(*box_new)(int orientation, int spacing);
    void (*box_append)(void *box, void *child);
    void *(*overlay_new)(void);
    void (*overlay_set_child)(void *overlay, void *child);
    void (*overlay_add_overlay)(void *overlay, void *child);
    void (*overlay_set_clip_overlay)(void *overlay, void *child, int clip);
    void *(*drawing_area_new)(void);
    void *(*fixed_new)(void);
    void (*fixed_put)(void *fixed, void *child, double x, double y);
    void (*fixed_move)(void *fixed, void *child, double x, double y);
    void (*fixed_remove)(void *fixed, void *child);
    void *(*label_new)(const char *text);
    void (*label_set_text)(void *label, const char *text);
    void (*label_set_xalign)(void *label, float align);
    void (*label_set_yalign)(void *label, float align);
    void *(*button_new_with_label)(const char *text);
    void (*button_set_label)(void *button, const char *text);
    void *(*entry_new)(void);
    void (*editable_set_text)(void *editable, const char *text);
    const char *(*editable_get_text)(void *editable);
    void (*editable_set_position)(void *editable, int position);
    void (*editable_delete_text)(void *editable, int start, int end);
    void (*editable_insert_text)(void *editable, const char *text, int length, int *position);
    int (*editable_get_editable)(void *editable);
    void *(*scrolled_window_new)(void);
    void (*scrolled_window_set_child)(void *scrolled, void *child);
    void (*scrolled_window_set_policy)(void *scrolled, int across, int down);
    void (*scrolled_window_set_has_frame)(void *scrolled, int frame);
    void *(*text_view_new)(void);
    void *(*text_view_get_buffer)(void *view);
    void (*text_view_set_wrap_mode)(void *view, int mode);
    int (*text_view_get_editable)(void *view);
    void (*text_buffer_set_text)(void *buffer, const char *text, int length);
    void (*text_buffer_get_start_iter)(void *buffer, hero_ui_gtk_iter *iter);
    void (*text_buffer_get_end_iter)(void *buffer, hero_ui_gtk_iter *iter);
    char *(*text_buffer_get_text)(void *buffer, const hero_ui_gtk_iter *start, const hero_ui_gtk_iter *end, int hidden);
    void (*text_buffer_place_cursor)(void *buffer, const hero_ui_gtk_iter *where);
    void (*text_buffer_begin_user_action)(void *buffer);
    void (*text_buffer_end_user_action)(void *buffer);
    int (*text_buffer_delete_interactive)(void *buffer, hero_ui_gtk_iter *start, hero_ui_gtk_iter *end, int editable);
    int (*text_buffer_insert_interactive_at_cursor)(void *buffer, const char *text, int length, int editable);
    void *(*check_button_new_with_label)(const char *text);
    void (*check_button_set_label)(void *button, const char *text);
    void (*check_button_set_active)(void *button, int active);
    int (*check_button_get_active)(void *button);
    void (*check_button_set_group)(void *button, void *group);
    void *(*list_box_new)(void);
    void (*list_box_set_selection_mode)(void *box, int mode);
    void (*list_box_append)(void *box, void *child);
    void (*list_box_remove)(void *box, void *child);
    void *(*list_box_get_row_at_index)(void *box, int index);
    void (*list_box_select_row)(void *box, void *row);
    void (*list_box_unselect_all)(void *box);
    int (*list_box_row_is_selected)(void *row);
    void *(*string_list_new)(const char *const *strings);
    void (*string_list_append)(void *list, const char *text);
    void (*string_list_remove)(void *list, unsigned position);
    void (*string_list_splice)(void *list, unsigned position, unsigned removals, const char *const *additions);
    void *(*drop_down_new)(void *model, void *expression);
    void (*drop_down_set_model)(void *drop, void *model);
    void (*drop_down_set_selected)(void *drop, unsigned position);
    unsigned (*drop_down_get_selected)(void *drop);
    void *(*progress_bar_new)(void);
    void (*progress_bar_set_fraction)(void *bar, double fraction);
    void *(*scale_new_with_range)(int orientation, double least, double most, double step);
    void (*scale_set_draw_value)(void *scale, int draw);
    void (*range_set_range)(void *range, double least, double most);
    void (*range_set_value)(void *range, double value);
    double (*range_get_value)(void *range);
    void (*range_set_increments)(void *range, double step, double page);
    void *(*picture_new)(void);
    void (*picture_set_paintable)(void *picture, void *paintable);
    void *(*popover_menu_bar_new_from_model)(void *model);
    void *(*shortcut_controller_new)(void);
    void (*shortcut_controller_add_shortcut)(void *controller, void *shortcut);
    void (*shortcut_controller_remove_shortcut)(void *controller, void *shortcut);
    void *(*shortcut_new)(void *trigger, void *action);
    void *(*keyval_trigger_new)(unsigned keyval, unsigned modifiers);
    void *(*named_action_new)(const char *name);
    char *(*accelerator_name)(unsigned keyval, unsigned modifiers);
    void *(*message_dialog_new)(void *parent, int flags, int type, int buttons, const char *format, ...);
    void (*message_dialog_format_secondary_text)(void *dialog, const char *format, ...);
    void *(*dialog_add_button)(void *dialog, const char *text, int response);
    void (*dialog_set_default_response)(void *dialog, int response);
    void *(*file_chooser_native_new)(const char *title, void *parent, int action, const char *accept, const char *cancel);
    void (*native_dialog_show)(void *dialog);
    void (*native_dialog_set_modal)(void *dialog, int modal);
    void (*file_chooser_set_select_multiple)(void *chooser, int multiple);
    void (*file_chooser_set_current_name)(void *chooser, const char *name);
    void (*file_chooser_add_filter)(void *chooser, void *filter);
    void *(*file_chooser_get_files)(void *chooser);
    void *(*file_filter_new)(void);
    void (*file_filter_set_name)(void *filter, const char *name);
    void (*file_filter_add_pattern)(void *filter, const char *pattern);
    size_t (*file_filter_get_type)(void);
    uint32_t (*dialog_error_quark)(void);
    void *(*memory_texture_new)(int width, int height, int format, void *bytes, size_t stride);
    int (*main_context_iteration)(void *context, int may_block);
    unsigned (*timeout_add)(unsigned interval, hero_ui_gtk_source function, void *data);
    unsigned (*idle_add)(hero_ui_gtk_source function, void *data);
    int (*source_remove)(unsigned tag);
    /* GLib's `g_free`, under the library's own name: a release of GLib's
     * memory, never C's `free` (the net's `runtime` suite reads a call to
     * `free` anywhere outside `parts/alloc.c` as a second allocation point). */
    void (*g_free)(void *memory);
    void *(*bytes_new)(const void *data, size_t size);
    void (*bytes_unref)(void *bytes);
    const char *(*dgettext)(const char *domain, const char *words);
    void (*error_free)(hero_ui_gerror *error);
    void *(*variant_new_string)(const char *text);
    unsigned long (*signal_connect_data)(void *instance, const char *signal, hero_ui_gtk_callback handler, void *data, void *destroy, int flags);
    void (*signal_emit_by_name)(void *instance, const char *signal, ...);
    void (*object_unref)(void *object);
    void *(*menu_new)(void);
    void (*menu_remove_all)(void *menu);
    void (*menu_append_section)(void *menu, const char *label, void *section);
    void (*menu_append_submenu)(void *menu, const char *label, void *submenu);
    void (*menu_append_item)(void *menu, void *item);
    void *(*menu_item_new)(const char *label, const char *action);
    void (*menu_item_set_attribute_value)(void *item, const char *attribute, void *value);
    void *(*simple_action_new)(const char *name, void *parameter_type);
    void (*simple_action_set_enabled)(void *action, int enabled);
    void *(*simple_action_group_new)(void);
    void (*action_map_add_action)(void *map, void *action);
    void (*action_map_remove_action)(void *map, const char *name);
    void (*action_activate)(void *action, void *parameter);
    unsigned (*list_model_get_n_items)(void *list);
    void *(*list_model_get_item)(void *list, unsigned position);
    char *(*file_get_path)(void *file);
    void *(*list_store_new)(size_t type);
    void (*list_store_append)(void *store, void *item);
    /* GTK 4.10's dialogs, NULL where the library is older. */
    void *(*alert_dialog_new)(const char *format, ...);
    void (*alert_dialog_set_detail)(void *dialog, const char *detail);
    void (*alert_dialog_set_buttons)(void *dialog, const char *const *labels);
    void (*alert_dialog_set_cancel_button)(void *dialog, int button);
    void (*alert_dialog_set_default_button)(void *dialog, int button);
    void (*alert_dialog_set_modal)(void *dialog, int modal);
    void (*alert_dialog_choose)(void *dialog, void *parent, void *cancellable, hero_ui_gtk_ready callback, void *data);
    int (*alert_dialog_choose_finish)(void *dialog, void *result, hero_ui_gerror **error);
    void *(*file_dialog_new)(void);
    void (*file_dialog_set_title)(void *dialog, const char *title);
    void (*file_dialog_set_modal)(void *dialog, int modal);
    void (*file_dialog_set_initial_name)(void *dialog, const char *name);
    void (*file_dialog_set_filters)(void *dialog, void *filters);
    void (*file_dialog_open)(void *dialog, void *parent, void *cancellable, hero_ui_gtk_ready callback, void *data);
    void *(*file_dialog_open_finish)(void *dialog, void *result, hero_ui_gerror **error);
    void (*file_dialog_open_multiple)(void *dialog, void *parent, void *cancellable, hero_ui_gtk_ready callback, void *data);
    void *(*file_dialog_open_multiple_finish)(void *dialog, void *result, hero_ui_gerror **error);
    void (*file_dialog_save)(void *dialog, void *parent, void *cancellable, hero_ui_gtk_ready callback, void *data);
    void *(*file_dialog_save_finish)(void *dialog, void *result, hero_ui_gerror **error);
    /* GTK 4.4's suffix filter, which matches its letters in either case. */
    void (*file_filter_add_suffix)(void *filter, const char *suffix);
} hero_ui_gtk_functions;

static hero_ui_gtk_functions hero_ui_gtk;
static bool hero_ui_gtk_found;
static pthread_once_t hero_ui_gtk_once = PTHREAD_ONCE_INIT;

#define HERO_UI_GTK_LIBRARY "libgtk-4.so.1"

/* One function by name into `*slot`; false where the library has none. C11
 * has no conversion between an object pointer and a function pointer, so the
 * address is copied by its bytes, which POSIX makes the same for both
 * (`dlsym`'s own page). */
static bool hero_ui_gtk_find(void *library, const char *name, void *slot, size_t slot_size) {
    void *found = dlsym(library, name);

    if (found == NULL || slot_size != sizeof found) {
        return false;
    }
    memcpy(slot, &found, sizeof found);
    return true;
}

#define HERO_UI_GTK_NEED(field, name) ok = hero_ui_gtk_find(library, name, &hero_ui_gtk.field, sizeof hero_ui_gtk.field) && ok
#define HERO_UI_GTK_MAY(field, name) (void)hero_ui_gtk_find(library, name, &hero_ui_gtk.field, sizeof hero_ui_gtk.field)

static void hero_ui_gtk_load(void) {
    bool ok = true;
    void *library = dlopen(HERO_UI_GTK_LIBRARY, RTLD_NOW | RTLD_LOCAL);

    if (library == NULL) {
        return;
    }
    HERO_UI_GTK_NEED(init_check, "gtk_init_check");
    HERO_UI_GTK_NEED(disable_setlocale, "gtk_disable_setlocale");
    HERO_UI_GTK_NEED(window_new, "gtk_window_new");
    HERO_UI_GTK_NEED(window_set_title, "gtk_window_set_title");
    HERO_UI_GTK_NEED(window_set_child, "gtk_window_set_child");
    HERO_UI_GTK_NEED(window_set_default_size, "gtk_window_set_default_size");
    HERO_UI_GTK_NEED(window_set_resizable, "gtk_window_set_resizable");
    HERO_UI_GTK_NEED(window_present, "gtk_window_present");
    HERO_UI_GTK_NEED(window_close, "gtk_window_close");
    HERO_UI_GTK_NEED(window_destroy, "gtk_window_destroy");
    HERO_UI_GTK_NEED(widget_set_visible, "gtk_widget_set_visible");
    HERO_UI_GTK_NEED(widget_get_visible, "gtk_widget_get_visible");
    HERO_UI_GTK_NEED(widget_set_sensitive, "gtk_widget_set_sensitive");
    HERO_UI_GTK_NEED(widget_is_sensitive, "gtk_widget_is_sensitive");
    HERO_UI_GTK_NEED(widget_set_size_request, "gtk_widget_set_size_request");
    HERO_UI_GTK_NEED(widget_get_size_request, "gtk_widget_get_size_request");
    HERO_UI_GTK_NEED(widget_get_preferred_size, "gtk_widget_get_preferred_size");
    HERO_UI_GTK_NEED(widget_set_hexpand, "gtk_widget_set_hexpand");
    HERO_UI_GTK_NEED(widget_set_vexpand, "gtk_widget_set_vexpand");
    HERO_UI_GTK_NEED(widget_get_width, "gtk_widget_get_width");
    HERO_UI_GTK_NEED(widget_get_height, "gtk_widget_get_height");
    HERO_UI_GTK_NEED(widget_realize, "gtk_widget_realize");
    HERO_UI_GTK_NEED(widget_get_realized, "gtk_widget_get_realized");
    HERO_UI_GTK_NEED(widget_add_controller, "gtk_widget_add_controller");
    HERO_UI_GTK_NEED(widget_insert_action_group, "gtk_widget_insert_action_group");
    HERO_UI_GTK_NEED(box_new, "gtk_box_new");
    HERO_UI_GTK_NEED(box_append, "gtk_box_append");
    HERO_UI_GTK_NEED(overlay_new, "gtk_overlay_new");
    HERO_UI_GTK_NEED(overlay_set_child, "gtk_overlay_set_child");
    HERO_UI_GTK_NEED(overlay_add_overlay, "gtk_overlay_add_overlay");
    HERO_UI_GTK_NEED(overlay_set_clip_overlay, "gtk_overlay_set_clip_overlay");
    HERO_UI_GTK_NEED(drawing_area_new, "gtk_drawing_area_new");
    HERO_UI_GTK_NEED(fixed_new, "gtk_fixed_new");
    HERO_UI_GTK_NEED(fixed_put, "gtk_fixed_put");
    HERO_UI_GTK_NEED(fixed_move, "gtk_fixed_move");
    HERO_UI_GTK_NEED(fixed_remove, "gtk_fixed_remove");
    HERO_UI_GTK_NEED(label_new, "gtk_label_new");
    HERO_UI_GTK_NEED(label_set_text, "gtk_label_set_text");
    HERO_UI_GTK_NEED(label_set_xalign, "gtk_label_set_xalign");
    HERO_UI_GTK_NEED(label_set_yalign, "gtk_label_set_yalign");
    HERO_UI_GTK_NEED(button_new_with_label, "gtk_button_new_with_label");
    HERO_UI_GTK_NEED(button_set_label, "gtk_button_set_label");
    HERO_UI_GTK_NEED(entry_new, "gtk_entry_new");
    HERO_UI_GTK_NEED(editable_set_text, "gtk_editable_set_text");
    HERO_UI_GTK_NEED(editable_get_text, "gtk_editable_get_text");
    HERO_UI_GTK_NEED(editable_set_position, "gtk_editable_set_position");
    HERO_UI_GTK_NEED(editable_delete_text, "gtk_editable_delete_text");
    HERO_UI_GTK_NEED(editable_insert_text, "gtk_editable_insert_text");
    HERO_UI_GTK_NEED(editable_get_editable, "gtk_editable_get_editable");
    HERO_UI_GTK_NEED(scrolled_window_new, "gtk_scrolled_window_new");
    HERO_UI_GTK_NEED(scrolled_window_set_child, "gtk_scrolled_window_set_child");
    HERO_UI_GTK_NEED(scrolled_window_set_policy, "gtk_scrolled_window_set_policy");
    HERO_UI_GTK_NEED(scrolled_window_set_has_frame, "gtk_scrolled_window_set_has_frame");
    HERO_UI_GTK_NEED(text_view_new, "gtk_text_view_new");
    HERO_UI_GTK_NEED(text_view_get_buffer, "gtk_text_view_get_buffer");
    HERO_UI_GTK_NEED(text_view_set_wrap_mode, "gtk_text_view_set_wrap_mode");
    HERO_UI_GTK_NEED(text_view_get_editable, "gtk_text_view_get_editable");
    HERO_UI_GTK_NEED(text_buffer_set_text, "gtk_text_buffer_set_text");
    HERO_UI_GTK_NEED(text_buffer_get_start_iter, "gtk_text_buffer_get_start_iter");
    HERO_UI_GTK_NEED(text_buffer_get_end_iter, "gtk_text_buffer_get_end_iter");
    HERO_UI_GTK_NEED(text_buffer_get_text, "gtk_text_buffer_get_text");
    HERO_UI_GTK_NEED(text_buffer_place_cursor, "gtk_text_buffer_place_cursor");
    HERO_UI_GTK_NEED(text_buffer_begin_user_action, "gtk_text_buffer_begin_user_action");
    HERO_UI_GTK_NEED(text_buffer_end_user_action, "gtk_text_buffer_end_user_action");
    HERO_UI_GTK_NEED(text_buffer_delete_interactive, "gtk_text_buffer_delete_interactive");
    HERO_UI_GTK_NEED(text_buffer_insert_interactive_at_cursor, "gtk_text_buffer_insert_interactive_at_cursor");
    HERO_UI_GTK_NEED(check_button_new_with_label, "gtk_check_button_new_with_label");
    HERO_UI_GTK_NEED(check_button_set_label, "gtk_check_button_set_label");
    HERO_UI_GTK_NEED(check_button_set_active, "gtk_check_button_set_active");
    HERO_UI_GTK_NEED(check_button_get_active, "gtk_check_button_get_active");
    HERO_UI_GTK_NEED(check_button_set_group, "gtk_check_button_set_group");
    HERO_UI_GTK_NEED(list_box_new, "gtk_list_box_new");
    HERO_UI_GTK_NEED(list_box_set_selection_mode, "gtk_list_box_set_selection_mode");
    HERO_UI_GTK_NEED(list_box_append, "gtk_list_box_append");
    HERO_UI_GTK_NEED(list_box_remove, "gtk_list_box_remove");
    HERO_UI_GTK_NEED(list_box_get_row_at_index, "gtk_list_box_get_row_at_index");
    HERO_UI_GTK_NEED(list_box_select_row, "gtk_list_box_select_row");
    HERO_UI_GTK_NEED(list_box_unselect_all, "gtk_list_box_unselect_all");
    HERO_UI_GTK_NEED(list_box_row_is_selected, "gtk_list_box_row_is_selected");
    HERO_UI_GTK_NEED(string_list_new, "gtk_string_list_new");
    HERO_UI_GTK_NEED(string_list_append, "gtk_string_list_append");
    HERO_UI_GTK_NEED(string_list_remove, "gtk_string_list_remove");
    HERO_UI_GTK_NEED(string_list_splice, "gtk_string_list_splice");
    HERO_UI_GTK_NEED(drop_down_new, "gtk_drop_down_new");
    HERO_UI_GTK_NEED(drop_down_set_model, "gtk_drop_down_set_model");
    HERO_UI_GTK_NEED(drop_down_set_selected, "gtk_drop_down_set_selected");
    HERO_UI_GTK_NEED(drop_down_get_selected, "gtk_drop_down_get_selected");
    HERO_UI_GTK_NEED(progress_bar_new, "gtk_progress_bar_new");
    HERO_UI_GTK_NEED(progress_bar_set_fraction, "gtk_progress_bar_set_fraction");
    HERO_UI_GTK_NEED(scale_new_with_range, "gtk_scale_new_with_range");
    HERO_UI_GTK_NEED(scale_set_draw_value, "gtk_scale_set_draw_value");
    HERO_UI_GTK_NEED(range_set_range, "gtk_range_set_range");
    HERO_UI_GTK_NEED(range_set_value, "gtk_range_set_value");
    HERO_UI_GTK_NEED(range_get_value, "gtk_range_get_value");
    HERO_UI_GTK_NEED(range_set_increments, "gtk_range_set_increments");
    HERO_UI_GTK_NEED(picture_new, "gtk_picture_new");
    HERO_UI_GTK_NEED(picture_set_paintable, "gtk_picture_set_paintable");
    HERO_UI_GTK_NEED(popover_menu_bar_new_from_model, "gtk_popover_menu_bar_new_from_model");
    HERO_UI_GTK_NEED(shortcut_controller_new, "gtk_shortcut_controller_new");
    HERO_UI_GTK_NEED(shortcut_controller_add_shortcut, "gtk_shortcut_controller_add_shortcut");
    HERO_UI_GTK_NEED(shortcut_controller_remove_shortcut, "gtk_shortcut_controller_remove_shortcut");
    HERO_UI_GTK_NEED(shortcut_new, "gtk_shortcut_new");
    HERO_UI_GTK_NEED(keyval_trigger_new, "gtk_keyval_trigger_new");
    HERO_UI_GTK_NEED(named_action_new, "gtk_named_action_new");
    HERO_UI_GTK_NEED(accelerator_name, "gtk_accelerator_name");
    HERO_UI_GTK_NEED(message_dialog_new, "gtk_message_dialog_new");
    HERO_UI_GTK_NEED(message_dialog_format_secondary_text, "gtk_message_dialog_format_secondary_text");
    HERO_UI_GTK_NEED(dialog_add_button, "gtk_dialog_add_button");
    HERO_UI_GTK_NEED(dialog_set_default_response, "gtk_dialog_set_default_response");
    HERO_UI_GTK_NEED(file_chooser_native_new, "gtk_file_chooser_native_new");
    HERO_UI_GTK_NEED(native_dialog_show, "gtk_native_dialog_show");
    HERO_UI_GTK_NEED(native_dialog_set_modal, "gtk_native_dialog_set_modal");
    HERO_UI_GTK_NEED(file_chooser_set_select_multiple, "gtk_file_chooser_set_select_multiple");
    HERO_UI_GTK_NEED(file_chooser_set_current_name, "gtk_file_chooser_set_current_name");
    HERO_UI_GTK_NEED(file_chooser_add_filter, "gtk_file_chooser_add_filter");
    HERO_UI_GTK_NEED(file_chooser_get_files, "gtk_file_chooser_get_files");
    HERO_UI_GTK_NEED(file_filter_new, "gtk_file_filter_new");
    HERO_UI_GTK_NEED(file_filter_set_name, "gtk_file_filter_set_name");
    HERO_UI_GTK_NEED(file_filter_add_pattern, "gtk_file_filter_add_pattern");
    HERO_UI_GTK_NEED(file_filter_get_type, "gtk_file_filter_get_type");
    HERO_UI_GTK_NEED(dialog_error_quark, "gtk_dialog_error_quark");
    HERO_UI_GTK_NEED(memory_texture_new, "gdk_memory_texture_new");
    HERO_UI_GTK_NEED(main_context_iteration, "g_main_context_iteration");
    HERO_UI_GTK_NEED(timeout_add, "g_timeout_add");
    HERO_UI_GTK_NEED(idle_add, "g_idle_add");
    HERO_UI_GTK_NEED(source_remove, "g_source_remove");
    HERO_UI_GTK_NEED(g_free, "g_free");
    HERO_UI_GTK_NEED(bytes_new, "g_bytes_new");
    HERO_UI_GTK_NEED(bytes_unref, "g_bytes_unref");
    HERO_UI_GTK_NEED(dgettext, "g_dgettext");
    HERO_UI_GTK_NEED(error_free, "g_error_free");
    HERO_UI_GTK_NEED(variant_new_string, "g_variant_new_string");
    HERO_UI_GTK_NEED(signal_connect_data, "g_signal_connect_data");
    HERO_UI_GTK_NEED(signal_emit_by_name, "g_signal_emit_by_name");
    HERO_UI_GTK_NEED(object_unref, "g_object_unref");
    HERO_UI_GTK_NEED(menu_new, "g_menu_new");
    HERO_UI_GTK_NEED(menu_remove_all, "g_menu_remove_all");
    HERO_UI_GTK_NEED(menu_append_section, "g_menu_append_section");
    HERO_UI_GTK_NEED(menu_append_submenu, "g_menu_append_submenu");
    HERO_UI_GTK_NEED(menu_append_item, "g_menu_append_item");
    HERO_UI_GTK_NEED(menu_item_new, "g_menu_item_new");
    HERO_UI_GTK_NEED(menu_item_set_attribute_value, "g_menu_item_set_attribute_value");
    HERO_UI_GTK_NEED(simple_action_new, "g_simple_action_new");
    HERO_UI_GTK_NEED(simple_action_set_enabled, "g_simple_action_set_enabled");
    HERO_UI_GTK_NEED(simple_action_group_new, "g_simple_action_group_new");
    HERO_UI_GTK_NEED(action_map_add_action, "g_action_map_add_action");
    HERO_UI_GTK_NEED(action_map_remove_action, "g_action_map_remove_action");
    HERO_UI_GTK_NEED(action_activate, "g_action_activate");
    HERO_UI_GTK_NEED(list_model_get_n_items, "g_list_model_get_n_items");
    HERO_UI_GTK_NEED(list_model_get_item, "g_list_model_get_item");
    HERO_UI_GTK_NEED(file_get_path, "g_file_get_path");
    HERO_UI_GTK_NEED(list_store_new, "g_list_store_new");
    HERO_UI_GTK_NEED(list_store_append, "g_list_store_append");
    HERO_UI_GTK_MAY(alert_dialog_new, "gtk_alert_dialog_new");
    HERO_UI_GTK_MAY(alert_dialog_set_detail, "gtk_alert_dialog_set_detail");
    HERO_UI_GTK_MAY(alert_dialog_set_buttons, "gtk_alert_dialog_set_buttons");
    HERO_UI_GTK_MAY(alert_dialog_set_cancel_button, "gtk_alert_dialog_set_cancel_button");
    HERO_UI_GTK_MAY(alert_dialog_set_default_button, "gtk_alert_dialog_set_default_button");
    HERO_UI_GTK_MAY(alert_dialog_set_modal, "gtk_alert_dialog_set_modal");
    HERO_UI_GTK_MAY(alert_dialog_choose, "gtk_alert_dialog_choose");
    HERO_UI_GTK_MAY(alert_dialog_choose_finish, "gtk_alert_dialog_choose_finish");
    HERO_UI_GTK_MAY(file_dialog_new, "gtk_file_dialog_new");
    HERO_UI_GTK_MAY(file_dialog_set_title, "gtk_file_dialog_set_title");
    HERO_UI_GTK_MAY(file_dialog_set_modal, "gtk_file_dialog_set_modal");
    HERO_UI_GTK_MAY(file_dialog_set_initial_name, "gtk_file_dialog_set_initial_name");
    HERO_UI_GTK_MAY(file_dialog_set_filters, "gtk_file_dialog_set_filters");
    HERO_UI_GTK_MAY(file_dialog_open, "gtk_file_dialog_open");
    HERO_UI_GTK_MAY(file_dialog_open_finish, "gtk_file_dialog_open_finish");
    HERO_UI_GTK_MAY(file_dialog_open_multiple, "gtk_file_dialog_open_multiple");
    HERO_UI_GTK_MAY(file_dialog_open_multiple_finish, "gtk_file_dialog_open_multiple_finish");
    HERO_UI_GTK_MAY(file_dialog_save, "gtk_file_dialog_save");
    HERO_UI_GTK_MAY(file_dialog_save_finish, "gtk_file_dialog_save_finish");
    HERO_UI_GTK_MAY(file_filter_add_suffix, "gtk_file_filter_add_suffix");
    hero_ui_gtk_found = ok;
}

/* Whether the library and every function of GTK 4.0 this arm calls are
 * there, found once. */
static bool hero_ui_gtk_loaded(void) {
    (void)pthread_once(&hero_ui_gtk_once, hero_ui_gtk_load);
    return hero_ui_gtk_found;
}

struct hero_ui_native {
    int64_t id;
    int64_t kind;
    int64_t parent;
    /* The window it stands in; a window's own number, a menu item's its
     * menu's window. */
    int64_t window;
    /* What stands in the window's content: the control, the scrolled window
     * around a list or a text area, a radio group's own `GtkFixed`; a
     * window's `GtkWindow`. */
    void *widget;
    /* The list box, the text view, the drop-down and its strings, a text
     * area's buffer. */
    void *inner;
    void *buffer;
    void *strings;
    /* Shown as the program asks; a placed widget shows once placed too. */
    bool visible;
    bool enabled;
    bool placed;
    int64_t x;
    int64_t y;
    int64_t width;
    int64_t height;
    /* A radio group's items and where each was placed, four numbers an item,
     * a width below 0 where it was not. */
    void **radios;
    int64_t *radio_places;
    int64_t radio_count;
    /* A list's or a combo box's items, and whether a combo box's list starts
     * with the empty row of no choice (A COMBO BOX). */
    int64_t items;
    bool placeholder;
    /* A slider's or a progress bar's range and value, exact. */
    int64_t least;
    int64_t most;
    int64_t value;
    /* A picture's size in pixels. */
    int64_t image_width;
    int64_t image_height;
    /* A window's content, its drawing area, its menu bar and its model, its
     * actions and its shortcuts. */
    void *fixed;
    void *area;
    void *bar;
    void *bar_model;
    void *actions;
    void *shortcuts;
    /* A window's content size and least size in points, the default size it
     * was given last, and whether a new size of the content is the person's
     * act `inject` gave. */
    int64_t content_width;
    int64_t content_height;
    int64_t least_width;
    int64_t least_height;
    int asked_width;
    int asked_height;
    bool resize_injected;
    /* A menu's model; a window's menus, a menu's items and separators, in
     * the order made. */
    void *menu_model;
    hero_ui_ids order;
    /* A menu's or an item's text as given. */
    char *label;
    /* An item's action and shortcut, its key value and the keys held. */
    void *action;
    void *shortcut;
    unsigned keyval;
    unsigned held;
};

/* Above 0 while `inject` acts, so a slider moved to where it stands is told. */
static _Thread_local int64_t hero_ui_gtk_injecting;
/* The paths the last file dialog answered, each followed by one zero byte. */
static _Thread_local uint8_t *hero_ui_gtk_paths;
static _Thread_local int64_t hero_ui_gtk_paths_count;

/* -- Words, numbers and text ------------------------------------------------ */

/* `count` bytes of checked UTF-8 at `text` with a zero after them, in a
 * block of `hero_layer_alloc`, or NULL; each `_` doubled where `marks`. */
static char *hero_ui_gtk_text(const uint8_t *text, int64_t count, bool marks) {
    int64_t more = 0;
    char *made;
    int64_t at = 0;

    for (int64_t i = 0; marks && i < count; i += 1) {
        more += text[i] == '_' ? 1 : 0;
    }
    made = hero_layer_alloc((size_t)(count + more) + 1);
    if (made == NULL) {
        return NULL;
    }
    for (int64_t i = 0; i < count; i += 1) {
        if (marks && text[i] == '_') {
            made[at++] = '_';
        }
        made[at++] = (char)text[i];
    }
    made[at] = '\0';
    return made;
}

/* The C text `text` with each `_` doubled, as a menu shows a label. */
static char *hero_ui_gtk_marked(const char *text) {
    const char *given = text != NULL ? text : "";

    return hero_ui_gtk_text((const uint8_t *)given, (int64_t)strlen(given), true);
}

/* GTK's own words for a button, in the person's language, `_` taken out. */
static void hero_ui_gtk_words_of(const char *msgid, char *into, size_t room) {
    const char *said = hero_ui_gtk.dgettext("gtk40", msgid);
    size_t at = 0;

    for (size_t i = 0; said != NULL && said[i] != '\0' && at + 1 < room; i += 1) {
        if (said[i] != '_') {
            into[at++] = said[i];
        }
    }
    into[at] = '\0';
}

/* Connects `handler` to `signal` of `instance`, carrying widget number `id`. */
static void hero_ui_gtk_connect(void *instance, const char *signal, hero_ui_gtk_callback handler, int64_t id) {
    (void)hero_ui_gtk.signal_connect_data(instance, signal, handler, (void *)(intptr_t)id, NULL, 0);
}

_Static_assert(sizeof(void *) == sizeof(int64_t), "a signal's data carries a widget's 64-bit number");

/* The widget whose signal carried `data`, where `instance` is still its own
 * (or one of a radio group's items), else NULL. */
static hero_ui_native *hero_ui_gtk_of(void *data, void *instance) {
    hero_ui_native *widget = hero_ui_find((int64_t)(intptr_t)data);

    if (widget == NULL) {
        return NULL;
    }
    if (widget->widget == instance || widget->inner == instance || widget->buffer == instance || widget->action == instance || widget->area == instance) {
        return widget;
    }
    for (int64_t i = 0; i < widget->radio_count; i += 1) {
        if (widget->radios[i] == instance) {
            return widget;
        }
    }
    return NULL;
}

/* The window of number `id`, or NULL. */
static hero_ui_native *hero_ui_gtk_window(int64_t id) {
    hero_ui_native *window = hero_ui_find(id);

    return window != NULL && window->kind == HERO_UI_WINDOW ? window : NULL;
}

/* `value` as GTK's `int` of a size or a place. */
static int hero_ui_gtk_int(int64_t value) {
    return (int)hero_ui_between(value, -(INT64_C(1) << 24), INT64_C(1) << 24);
}

/* A slider's or a progress bar's value as a double, and back, exact while
 * the range is within 2^53 of 0, its share of the range past that. */
static double hero_ui_gtk_double(const hero_ui_native *widget, int64_t value) {
    if (widget->least >= -HERO_UI_GTK_EXACT && widget->most <= HERO_UI_GTK_EXACT) {
        return (double)value;
    }
    return hero_ui_fraction(value, widget->least, widget->most) * (double)HERO_UI_GTK_EXACT;
}

static int64_t hero_ui_gtk_integer(const hero_ui_native *widget, double value) {
    if (widget->least >= -HERO_UI_GTK_EXACT && widget->most <= HERO_UI_GTK_EXACT) {
        /* Inside 2^53 of 0 a double holds the value, and its rounding fits. */
        double held = value < (double)widget->least ? (double)widget->least : (value > (double)widget->most ? (double)widget->most : value);

        return hero_ui_between(held >= 0 ? (int64_t)(held + 0.5) : -(int64_t)(-held + 0.5), widget->least, widget->most);
    }
    return hero_ui_at_fraction(value / (double)HERO_UI_GTK_EXACT, widget->least, widget->most);
}

/* -- Windows: their size, their menus ---------------------------------------- */

/* Gives window `window` the default size whose content is the content size
 * it keeps: the window's size less its content's, the menu bar and a title
 * bar GTK draws, measured where the window was laid out, else the menu bar's
 * own height. */
static void hero_ui_gtk_size_content(hero_ui_native *window) {
    int outer_width = hero_ui_gtk.widget_get_width(window->widget);
    int outer_height = hero_ui_gtk.widget_get_height(window->widget);
    int inner_width = hero_ui_gtk.widget_get_width(window->area);
    int inner_height = hero_ui_gtk.widget_get_height(window->area);
    int extra_width = 0;
    int extra_height = 0;

    if (outer_width > 0 && inner_width > 0 && outer_height > 0 && inner_height > 0) {
        extra_width = outer_width - inner_width;
        extra_height = outer_height - inner_height;
    } else if (window->bar != NULL && window->order.count > 0) {
        hero_ui_gtk_size least;
        hero_ui_gtk_size natural;

        hero_ui_gtk.widget_get_preferred_size(window->bar, &least, &natural);
        extra_height = natural.height;
    }
    window->asked_width = hero_ui_gtk_int(window->content_width) + extra_width;
    window->asked_height = hero_ui_gtk_int(window->content_height) + extra_height;
    hero_ui_gtk.window_set_default_size(window->widget, window->asked_width, window->asked_height);
}

/* Gives window `data` its content size again, from an idle source: what the
 * content's `resize` asks where the window's size was right and its
 * content's not, since a size given while GTK lays a window out is lost. */
static int hero_ui_gtk_resize_again(void *data) {
    hero_ui_native *window = hero_ui_gtk_window((int64_t)(intptr_t)data);

    if (window != NULL) {
        hero_ui_gtk_size_content(window);
    }
    return 0;
}

/* Window content `area` took the size `width` by `height`. */
static void hero_ui_gtk_resized(void *area, int width, int height, void *data) {
    hero_ui_native *window = hero_ui_gtk_of(data, area);

    if (window == NULL || window->kind != HERO_UI_WINDOW) {
        return;
    }
    if (width == hero_ui_gtk_int(window->content_width) && height == hero_ui_gtk_int(window->content_height)) {
        window->resize_injected = false;
        return;
    }
    if (!window->resize_injected && hero_ui_gtk.widget_get_width(window->widget) == window->asked_width && hero_ui_gtk.widget_get_height(window->widget) == window->asked_height) {
        /* The window took the size asked and its content another: the room
         * the menu bar and the title bar take was not yet known. */
        (void)hero_ui_gtk.idle_add(hero_ui_gtk_resize_again, data);
        return;
    }
    window->resize_injected = false;
    window->content_width = width;
    window->content_height = height;
    hero_ui_tell(HERO_UI_EVENT_RESIZE, window->id, width, height);
}

/* The person asked window `instance` to close: told, and nothing closed. */
static int hero_ui_gtk_close_asked(void *instance, void *data) {
    hero_ui_native *window = hero_ui_gtk_of(data, instance);

    if (window != NULL && window->kind == HERO_UI_WINDOW) {
        hero_ui_tell(HERO_UI_EVENT_CLOSE, window->id, 0, 0);
    }
    return 1;
}

/* Whether menu item `item`, its menu and the showing of both let the person
 * choose it. */
static bool hero_ui_gtk_usable(const hero_ui_native *item) {
    const hero_ui_native *menu = hero_ui_find(item->parent);

    return item->kind == HERO_UI_MENU_ITEM && item->enabled && item->visible && menu != NULL && menu->enabled && menu->visible;
}

/* The action's name of item `id`, `item<id>`, into `into`. */
static void hero_ui_gtk_action_name(int64_t id, bool prefixed, char *into, size_t room) {
    (void)snprintf(into, room, "%sitem%lld", prefixed ? "hero." : "", (long long)id);
}

/* Builds menu `menu`'s model again: its items shown, in their order, in
 * sections between its separators; and sets each item's action enabled as
 * `hero_ui_gtk_usable` says. */
static int64_t hero_ui_gtk_fill_menu(hero_ui_native *menu) {
    void *section = NULL;
    int64_t in_section = 0;
    int64_t answer = HERO_UI_OK;

    hero_ui_gtk.menu_remove_all(menu->menu_model);
    for (int64_t i = 0; i < menu->order.count; i += 1) {
        hero_ui_native *item = hero_ui_find(menu->order.at[i]);

        if (item == NULL) {
            continue;
        }
        if (item->action != NULL) {
            hero_ui_gtk.simple_action_set_enabled(item->action, hero_ui_gtk_usable(item) ? 1 : 0);
        }
        if (!item->visible) {
            continue;
        }
        if (item->kind == HERO_UI_MENU_SEPARATOR) {
            if (section != NULL && in_section > 0) {
                hero_ui_gtk.menu_append_section(menu->menu_model, NULL, section);
                hero_ui_gtk.object_unref(section);
                section = NULL;
                in_section = 0;
            }
            continue;
        }
        if (section == NULL) {
            section = hero_ui_gtk.menu_new();
        }
        {
            char action[64];
            char *label = hero_ui_gtk_marked(item->label);
            void *entry;

            if (label == NULL) {
                answer = HERO_UI_LIMIT;
                continue;
            }
            hero_ui_gtk_action_name(item->id, true, action, sizeof action);
            entry = hero_ui_gtk.menu_item_new(label, action);
            hero_layer_free(label);
            if (item->keyval != 0) {
                char *accel = hero_ui_gtk.accelerator_name(item->keyval, item->held);

                if (accel != NULL) {
                    hero_ui_gtk.menu_item_set_attribute_value(entry, "accel", hero_ui_gtk.variant_new_string(accel));
                    hero_ui_gtk.g_free(accel);
                }
            }
            hero_ui_gtk.menu_append_item(section, entry);
            hero_ui_gtk.object_unref(entry);
            in_section += 1;
        }
    }
    if (section != NULL) {
        if (in_section > 0) {
            hero_ui_gtk.menu_append_section(menu->menu_model, NULL, section);
        }
        hero_ui_gtk.object_unref(section);
    }
    return answer;
}

/* Builds window `window`'s menu bar's model again: its menus shown, in their
 * order; the bar is shown while it holds one, and the content keeps its
 * size. */
static int64_t hero_ui_gtk_fill_bar(hero_ui_native *window) {
    int64_t shown = 0;
    int64_t answer = HERO_UI_OK;

    hero_ui_gtk.menu_remove_all(window->bar_model);
    for (int64_t i = 0; i < window->order.count; i += 1) {
        hero_ui_native *menu = hero_ui_find(window->order.at[i]);
        char *label;

        if (menu == NULL) {
            continue;
        }
        (void)hero_ui_gtk_fill_menu(menu);
        if (!menu->visible) {
            continue;
        }
        label = hero_ui_gtk_marked(menu->label);
        if (label == NULL) {
            answer = HERO_UI_LIMIT;
            continue;
        }
        hero_ui_gtk.menu_append_submenu(window->bar_model, label, menu->menu_model);
        hero_layer_free(label);
        shown += 1;
    }
    hero_ui_gtk.widget_set_visible(window->bar, shown > 0 ? 1 : 0);
    hero_ui_gtk_size_content(window);
    return answer;
}

/* An item's action was activated: by its menu, its shortcut, or `inject`. */
static void hero_ui_gtk_chosen(void *action, void *parameter, void *data) {
    hero_ui_native *item = hero_ui_gtk_of(data, action);

    (void)parameter;
    if (item != NULL && hero_ui_gtk_usable(item)) {
        hero_ui_tell(HERO_UI_EVENT_MENU, item->id, 0, 0);
    }
}

/* -- What the person does ---------------------------------------------------- */

static void hero_ui_gtk_clicked(void *button, void *data) {
    hero_ui_native *widget = hero_ui_gtk_of(data, button);

    if (widget != NULL && widget->kind == HERO_UI_BUTTON) {
        hero_ui_tell(HERO_UI_EVENT_CLICK, widget->id, 0, 0);
    }
}

static void hero_ui_gtk_toggled(void *button, void *data) {
    hero_ui_native *widget = hero_ui_gtk_of(data, button);

    if (widget == NULL) {
        return;
    }
    if (widget->kind == HERO_UI_CHECKBOX) {
        hero_ui_tell(HERO_UI_EVENT_TOGGLE, widget->id, hero_ui_gtk.check_button_get_active(button) ? 1 : 0, 0);
        return;
    }
    for (int64_t i = 0; widget->kind == HERO_UI_RADIO_GROUP && i < widget->radio_count; i += 1) {
        if (widget->radios[i] == button && hero_ui_gtk.check_button_get_active(button)) {
            hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, i, 0);
        }
    }
}

static void hero_ui_gtk_edited(void *instance, void *data) {
    hero_ui_native *widget = hero_ui_gtk_of(data, instance);

    if (widget != NULL && (widget->kind == HERO_UI_TEXT_FIELD || widget->kind == HERO_UI_TEXT_AREA)) {
        hero_ui_tell(HERO_UI_EVENT_TEXT, widget->id, 0, 0);
    }
}

static void hero_ui_gtk_rows_chosen(void *box, void *data) {
    hero_ui_native *widget = hero_ui_gtk_of(data, box);

    if (widget != NULL && (widget->kind == HERO_UI_LIST || widget->kind == HERO_UI_LIST_MULTIPLE)) {
        hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, 0, 0);
    }
}

/* Takes the empty row of no choice out of combo box `data`'s list, from an
 * idle source, once an item was chosen: never inside the drop-down's own
 * notification. */
static int hero_ui_gtk_drop_placeholder(void *data) {
    hero_ui_native *combo = hero_ui_find((int64_t)(intptr_t)data);

    if (combo != NULL && combo->kind == HERO_UI_COMBO && combo->placeholder && hero_ui_gtk.drop_down_get_selected(combo->inner) != 0) {
        hero_ui_muted += 1;
        combo->placeholder = false;
        hero_ui_gtk.string_list_remove(combo->strings, 0);
        hero_ui_muted -= 1;
    }
    return 0;
}

/* The item combo box `combo` has chosen, -1 for none. */
static int64_t hero_ui_gtk_combo_choice(const hero_ui_native *combo) {
    unsigned chosen = hero_ui_gtk.drop_down_get_selected(combo->inner);
    int64_t item;

    if (chosen == HERO_GTK_INVALID_LIST_POSITION) {
        return -1;
    }
    item = (int64_t)chosen - (combo->placeholder ? 1 : 0);
    return item >= 0 && item < combo->items ? item : -1;
}

static void hero_ui_gtk_combo_chosen(void *drop, void *spec, void *data) {
    hero_ui_native *widget = hero_ui_gtk_of(data, drop);

    (void)spec;
    if (widget == NULL || widget->kind != HERO_UI_COMBO || hero_ui_muted > 0) {
        return;
    }
    if (widget->placeholder && hero_ui_gtk.drop_down_get_selected(drop) != 0) {
        (void)hero_ui_gtk.idle_add(hero_ui_gtk_drop_placeholder, data);
    }
    hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, hero_ui_gtk_combo_choice(widget), 0);
}

static void hero_ui_gtk_slid(void *range, void *data) {
    hero_ui_native *widget = hero_ui_gtk_of(data, range);
    int64_t value;

    if (widget == NULL || widget->kind != HERO_UI_SLIDER) {
        return;
    }
    value = hero_ui_gtk_integer(widget, hero_ui_gtk.range_get_value(range));
    if (value != widget->value || hero_ui_gtk_injecting > 0) {
        widget->value = value;
        hero_ui_tell(HERO_UI_EVENT_VALUE, widget->id, value, 0);
    }
}

/* -- The arm ------------------------------------------------------------------ */

/* Whether `name` names something in the environment. */
static bool hero_ui_gtk_named(const char *name) {
    const char *value = getenv(name);

    return value != NULL && value[0] != '\0';
}

static int64_t hero_ui_arm_here(void) {
    if (!hero_ui_gtk_named("DISPLAY") && !hero_ui_gtk_named("WAYLAND_DISPLAY")) {
        return 0;
    }
    return hero_ui_gtk_loaded() ? 1 : 0;
}

static void hero_ui_arm_absent(char *words, size_t count) {
    if (!hero_ui_gtk_named("DISPLAY") && !hero_ui_gtk_named("WAYLAND_DISPLAY")) {
        (void)hero_ui_copy_text(words, count, "no display is named, neither DISPLAY nor WAYLAND_DISPLAY");
        return;
    }
    (void)hero_ui_copy_text(words, count, "GTK 4 is not here: " HERO_UI_GTK_LIBRARY " is not installed, or lacks a function of GTK 4.0");
}

static int64_t hero_ui_arm_open(void) {
    if (!hero_ui_gtk_loaded()) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "GTK 4 is not here: " HERO_UI_GTK_LIBRARY " is not installed, or lacks a function of GTK 4.0");
    }
    hero_ui_gtk.disable_setlocale();
    if (!hero_ui_gtk.init_check()) {
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "GTK 4 could not open the display DISPLAY or WAYLAND_DISPLAY names");
    }
    hero_ui_muted = 0;
    hero_ui_gtk_injecting = 0;
    hero_ui_make_host();
    return HERO_UI_OK;
}

/* The container `widget` stands in: its window's content, or its radio
 * group's own fixed for an item. */
static void *hero_ui_gtk_container(const hero_ui_native *widget) {
    hero_ui_native *window = hero_ui_gtk_window(widget->window);

    return window != NULL ? window->fixed : NULL;
}

/* Destroys `widget`'s GTK objects and gives back its record; its children
 * are gone already. */
static void hero_ui_gtk_drop(hero_ui_native *widget) {
    hero_ui_native *owner = hero_ui_find(widget->parent);

    switch (widget->kind) {
    case HERO_UI_WINDOW:
        if (widget->widget != NULL) {
            hero_ui_gtk.window_destroy(widget->widget);
        }
        if (widget->bar_model != NULL) {
            hero_ui_gtk.object_unref(widget->bar_model);
        }
        if (widget->actions != NULL) {
            hero_ui_gtk.object_unref(widget->actions);
        }
        break;
    case HERO_UI_MENU:
        if (owner != NULL) {
            hero_ui_ids_drop(&owner->order, widget->id);
            (void)hero_ui_gtk_fill_bar(owner);
        }
        if (widget->menu_model != NULL) {
            hero_ui_gtk.object_unref(widget->menu_model);
        }
        break;
    case HERO_UI_MENU_ITEM:
    case HERO_UI_MENU_SEPARATOR: {
        hero_ui_native *window = hero_ui_gtk_window(widget->window);

        if (owner != NULL) {
            hero_ui_ids_drop(&owner->order, widget->id);
            (void)hero_ui_gtk_fill_menu(owner);
        }
        if (window != NULL && widget->shortcut != NULL) {
            hero_ui_gtk.shortcut_controller_remove_shortcut(window->shortcuts, widget->shortcut);
        }
        if (window != NULL && widget->action != NULL) {
            char name[64];

            hero_ui_gtk_action_name(widget->id, false, name, sizeof name);
            hero_ui_gtk.action_map_remove_action(window->actions, name);
        }
        if (widget->action != NULL) {
            hero_ui_gtk.object_unref(widget->action);
        }
        break;
    }
    default: {
        void *container = hero_ui_gtk_container(widget);

        if (container != NULL && widget->widget != NULL) {
            hero_ui_gtk.fixed_remove(container, widget->widget);
        }
        break;
    }
    }
    hero_layer_free(widget->radios);
    hero_layer_free(widget->radio_places);
    hero_layer_free(widget->label);
    hero_ui_ids_free(&widget->order);
    hero_ui_forget(widget->id);
    hero_layer_free(widget);
}

/* Destroys widget `id` and every widget whose parent it is, children first. */
static void hero_ui_gtk_destroy_tree(int64_t id) {
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
        hero_ui_gtk_destroy_tree(child);
    }
    hero_ui_gtk_drop(widget);
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
        hero_ui_gtk_destroy_tree(first);
    }
    hero_ui_free_map();
    hero_layer_free(hero_ui_gtk_paths);
    hero_ui_gtk_paths = NULL;
    hero_ui_gtk_paths_count = 0;
    /* GTK finishes a window's destruction in its own loop's turns: they are
     * given those turns before the app is gone, a bounded number, so a source
     * that is always ready cannot hold `close`. */
    for (int turns = 0; turns < 1000 && hero_ui_gtk.main_context_iteration(NULL, 0); turns += 1) {
    }
}

/* Makes window `made`: its box, its menu bar, its content, its actions and
 * its shortcuts. */
static bool hero_ui_gtk_make_window(hero_ui_native *made) {
    void *box;
    void *overlay;

    made->widget = hero_ui_gtk.window_new();
    made->bar_model = hero_ui_gtk.menu_new();
    made->actions = hero_ui_gtk.simple_action_group_new();
    made->shortcuts = hero_ui_gtk.shortcut_controller_new();
    if (made->widget == NULL || made->bar_model == NULL || made->actions == NULL || made->shortcuts == NULL) {
        return false;
    }
    box = hero_ui_gtk.box_new(HERO_GTK_ORIENTATION_VERTICAL, 0);
    made->bar = hero_ui_gtk.popover_menu_bar_new_from_model(made->bar_model);
    overlay = hero_ui_gtk.overlay_new();
    made->area = hero_ui_gtk.drawing_area_new();
    made->fixed = hero_ui_gtk.fixed_new();
    hero_ui_gtk.widget_set_hexpand(made->area, 1);
    hero_ui_gtk.widget_set_vexpand(made->area, 1);
    hero_ui_gtk.widget_set_vexpand(overlay, 1);
    hero_ui_gtk.overlay_set_child(overlay, made->area);
    hero_ui_gtk.overlay_add_overlay(overlay, made->fixed);
    hero_ui_gtk.overlay_set_clip_overlay(overlay, made->fixed, 1);
    hero_ui_gtk.widget_set_visible(made->bar, 0);
    hero_ui_gtk.box_append(box, made->bar);
    hero_ui_gtk.box_append(box, overlay);
    hero_ui_gtk.window_set_child(made->widget, box);
    hero_ui_gtk.widget_insert_action_group(made->widget, "hero", made->actions);
    hero_ui_gtk.widget_add_controller(made->widget, made->shortcuts);
    hero_ui_gtk_connect(made->widget, "close-request", (hero_ui_gtk_callback)hero_ui_gtk_close_asked, made->id);
    hero_ui_gtk_connect(made->area, "resize", (hero_ui_gtk_callback)hero_ui_gtk_resized, made->id);
    made->content_width = 100;
    made->content_height = 100;
    return true;
}

/* Puts `made`'s widget in its window's content, at no place and hidden. */
static void hero_ui_gtk_stand(hero_ui_native *made, hero_ui_native *window) {
    hero_ui_gtk.widget_set_visible(made->widget, 0);
    hero_ui_gtk.fixed_put(window->fixed, made->widget, 0, 0);
}

static int64_t hero_ui_arm_create(int64_t id, int64_t kind, int64_t parent) {
    hero_ui_native *owner = NULL;
    hero_ui_native *window = NULL;
    hero_ui_native *made;
    bool ok = true;

    if (hero_ui_find(id) != NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (kind == HERO_UI_WEBVIEW) {
        /* THE WEB VIEW, kind 17, is an arm of its own; until it stands here
         * this arm makes none. */
        return hero_ui_refused(HERO_UI_UNSUPPORTED, "the system's web view is not written for GTK yet");
    }
    if (kind != HERO_UI_WINDOW) {
        owner = hero_ui_find(parent);
        if (owner == NULL) {
            return HERO_UI_NOT_FOUND;
        }
        if ((kind == HERO_UI_MENU_ITEM || kind == HERO_UI_MENU_SEPARATOR) != (owner->kind == HERO_UI_MENU) || (owner->kind != HERO_UI_MENU && owner->kind != HERO_UI_WINDOW)) {
            return HERO_UI_INVALID;
        }
        window = hero_ui_gtk_window(owner->window);
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
        ok = hero_ui_gtk_make_window(made);
        break;
    case HERO_UI_LABEL:
        made->widget = hero_ui_gtk.label_new("");
        hero_ui_gtk.label_set_xalign(made->widget, 0.0f);
        hero_ui_gtk.label_set_yalign(made->widget, 0.0f);
        break;
    case HERO_UI_BUTTON:
        made->widget = hero_ui_gtk.button_new_with_label("");
        hero_ui_gtk_connect(made->widget, "clicked", (hero_ui_gtk_callback)hero_ui_gtk_clicked, id);
        break;
    case HERO_UI_TEXT_FIELD:
        made->widget = hero_ui_gtk.entry_new();
        hero_ui_gtk_connect(made->widget, "changed", (hero_ui_gtk_callback)hero_ui_gtk_edited, id);
        break;
    case HERO_UI_TEXT_AREA:
        made->widget = hero_ui_gtk.scrolled_window_new();
        made->inner = hero_ui_gtk.text_view_new();
        made->buffer = hero_ui_gtk.text_view_get_buffer(made->inner);
        hero_ui_gtk.text_view_set_wrap_mode(made->inner, HERO_GTK_WRAP_WORD_CHAR);
        hero_ui_gtk.scrolled_window_set_policy(made->widget, HERO_GTK_POLICY_NEVER, HERO_GTK_POLICY_AUTOMATIC);
        hero_ui_gtk.scrolled_window_set_has_frame(made->widget, 1);
        hero_ui_gtk.scrolled_window_set_child(made->widget, made->inner);
        hero_ui_gtk_connect(made->buffer, "changed", (hero_ui_gtk_callback)hero_ui_gtk_edited, id);
        break;
    case HERO_UI_CHECKBOX:
        made->widget = hero_ui_gtk.check_button_new_with_label("");
        hero_ui_gtk_connect(made->widget, "toggled", (hero_ui_gtk_callback)hero_ui_gtk_toggled, id);
        break;
    case HERO_UI_RADIO_GROUP:
        made->widget = hero_ui_gtk.fixed_new();
        break;
    case HERO_UI_LIST:
    case HERO_UI_LIST_MULTIPLE:
        made->widget = hero_ui_gtk.scrolled_window_new();
        made->inner = hero_ui_gtk.list_box_new();
        hero_ui_gtk.list_box_set_selection_mode(made->inner, kind == HERO_UI_LIST_MULTIPLE ? HERO_GTK_SELECTION_MULTIPLE : HERO_GTK_SELECTION_SINGLE);
        hero_ui_gtk.scrolled_window_set_policy(made->widget, HERO_GTK_POLICY_NEVER, HERO_GTK_POLICY_AUTOMATIC);
        hero_ui_gtk.scrolled_window_set_has_frame(made->widget, 1);
        hero_ui_gtk.scrolled_window_set_child(made->widget, made->inner);
        hero_ui_gtk_connect(made->inner, "selected-rows-changed", (hero_ui_gtk_callback)hero_ui_gtk_rows_chosen, id);
        break;
    case HERO_UI_COMBO:
        made->strings = hero_ui_gtk.string_list_new(NULL);
        made->widget = made->strings != NULL ? hero_ui_gtk.drop_down_new(made->strings, NULL) : NULL;
        made->inner = made->widget;
        if (made->widget != NULL) {
            hero_ui_gtk_connect(made->widget, "notify::selected", (hero_ui_gtk_callback)hero_ui_gtk_combo_chosen, id);
        }
        break;
    case HERO_UI_PROGRESS:
        made->widget = hero_ui_gtk.progress_bar_new();
        break;
    case HERO_UI_SLIDER:
        made->widget = hero_ui_gtk.scale_new_with_range(HERO_GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
        if (made->widget != NULL) {
            hero_ui_gtk.scale_set_draw_value(made->widget, 0);
            hero_ui_gtk.range_set_increments(made->widget, 1, 10);
            hero_ui_gtk_connect(made->widget, "value-changed", (hero_ui_gtk_callback)hero_ui_gtk_slid, id);
        }
        break;
    case HERO_UI_IMAGE:
        made->widget = hero_ui_gtk.picture_new();
        break;
    case HERO_UI_MENU:
        made->menu_model = hero_ui_gtk.menu_new();
        ok = made->menu_model != NULL;
        break;
    case HERO_UI_MENU_ITEM: {
        char name[64];

        hero_ui_gtk_action_name(id, false, name, sizeof name);
        made->action = hero_ui_gtk.simple_action_new(name, NULL);
        ok = made->action != NULL;
        if (ok) {
            hero_ui_gtk_connect(made->action, "activate", (hero_ui_gtk_callback)hero_ui_gtk_chosen, id);
        }
        break;
    }
    case HERO_UI_MENU_SEPARATOR:
        break;
    default:
        hero_layer_free(made);
        return HERO_UI_INVALID;
    }
    if (!ok || (made->widget == NULL && kind != HERO_UI_WINDOW && kind != HERO_UI_MENU && kind != HERO_UI_MENU_ITEM && kind != HERO_UI_MENU_SEPARATOR) || !hero_ui_keep(id, made)) {
        if (kind == HERO_UI_WINDOW && made->widget != NULL) {
            hero_ui_gtk.window_destroy(made->widget);
        }
        if (made->bar_model != NULL) {
            hero_ui_gtk.object_unref(made->bar_model);
        }
        if (made->actions != NULL) {
            hero_ui_gtk.object_unref(made->actions);
        }
        if (made->menu_model != NULL) {
            hero_ui_gtk.object_unref(made->menu_model);
        }
        if (made->action != NULL) {
            hero_ui_gtk.object_unref(made->action);
        }
        hero_layer_free(made);
        return ok ? HERO_UI_LIMIT : hero_ui_refused(HERO_UI_OTHER, "GTK made no widget of that kind");
    }
    switch (kind) {
    case HERO_UI_WINDOW:
        hero_ui_gtk_size_content(made);
        return HERO_UI_OK;
    case HERO_UI_MENU:
        if (!hero_ui_ids_add(&owner->order, id)) {
            hero_ui_gtk_drop(made);
            return HERO_UI_LIMIT;
        }
        return hero_ui_gtk_fill_bar(owner);
    case HERO_UI_MENU_ITEM:
    case HERO_UI_MENU_SEPARATOR:
        if (!hero_ui_ids_add(&owner->order, id)) {
            hero_ui_gtk_drop(made);
            return HERO_UI_LIMIT;
        }
        if (made->action != NULL) {
            hero_ui_gtk.action_map_add_action(window->actions, made->action);
        }
        return hero_ui_gtk_fill_menu(owner);
    default:
        hero_ui_gtk_stand(made, window);
        return HERO_UI_OK;
    }
}

static int64_t hero_ui_arm_destroy(int64_t id) {
    if (hero_ui_find(id) == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    hero_ui_gtk_destroy_tree(id);
    return HERO_UI_OK;
}

/* Sets text area `widget`'s whole text, the caret put at its end. */
static void hero_ui_gtk_area_text(hero_ui_native *widget, const char *text) {
    hero_ui_gtk_iter end;

    hero_ui_gtk.text_buffer_set_text(widget->buffer, text, -1);
    hero_ui_gtk.text_buffer_get_end_iter(widget->buffer, &end);
    hero_ui_gtk.text_buffer_place_cursor(widget->buffer, &end);
}

static int64_t hero_ui_arm_set_text(int64_t id, const uint8_t *text, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);
    char *made;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    switch (widget->kind) {
    case HERO_UI_WINDOW:
    case HERO_UI_LABEL:
    case HERO_UI_BUTTON:
    case HERO_UI_TEXT_FIELD:
    case HERO_UI_TEXT_AREA:
    case HERO_UI_CHECKBOX:
    case HERO_UI_MENU:
    case HERO_UI_MENU_ITEM:
        break;
    default:
        return HERO_UI_INVALID;
    }
    made = hero_ui_gtk_text(text, count, false);
    if (made == NULL) {
        return HERO_UI_LIMIT;
    }
    hero_ui_muted += 1;
    switch (widget->kind) {
    case HERO_UI_WINDOW:
        hero_ui_gtk.window_set_title(widget->widget, made);
        break;
    case HERO_UI_LABEL:
        hero_ui_gtk.label_set_text(widget->widget, made);
        break;
    case HERO_UI_BUTTON:
        hero_ui_gtk.button_set_label(widget->widget, made);
        break;
    case HERO_UI_CHECKBOX:
        hero_ui_gtk.check_button_set_label(widget->widget, made);
        break;
    case HERO_UI_TEXT_FIELD:
        hero_ui_gtk.editable_set_text(widget->widget, made);
        hero_ui_gtk.editable_set_position(widget->widget, -1);
        break;
    case HERO_UI_TEXT_AREA:
        hero_ui_gtk_area_text(widget, made);
        break;
    default:
        hero_layer_free(widget->label);
        widget->label = made;
        made = NULL;
        break;
    }
    hero_ui_muted -= 1;
    hero_layer_free(made);
    if (widget->kind == HERO_UI_MENU) {
        return hero_ui_gtk_fill_bar(hero_ui_find(widget->parent));
    }
    if (widget->kind == HERO_UI_MENU_ITEM) {
        return hero_ui_gtk_fill_menu(hero_ui_find(widget->parent));
    }
    return HERO_UI_OK;
}

/* Places radio group `group`'s items inside its own fixed, at their places
 * less the group's. */
static void hero_ui_gtk_put_radios(hero_ui_native *group) {
    for (int64_t i = 0; i < group->radio_count; i += 1) {
        const int64_t *place = &group->radio_places[i * 4];

        if (place[2] < 0) {
            hero_ui_gtk.widget_set_visible(group->radios[i], 0);
            continue;
        }
        hero_ui_gtk.fixed_move(group->widget, group->radios[i], (double)(place[0] - group->x), (double)(place[1] - group->y));
        hero_ui_gtk.widget_set_size_request(group->radios[i], hero_ui_gtk_int(place[2]), hero_ui_gtk_int(place[3]));
        hero_ui_gtk.widget_set_visible(group->radios[i], 1);
    }
}

static int64_t hero_ui_arm_set_items(int64_t id, const uint8_t *items, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);
    int64_t texts = hero_ui_texts_in(items, count);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind != HERO_UI_LIST && widget->kind != HERO_UI_LIST_MULTIPLE && widget->kind != HERO_UI_COMBO && widget->kind != HERO_UI_RADIO_GROUP) {
        return HERO_UI_INVALID;
    }
    hero_ui_muted += 1;
    if (widget->kind == HERO_UI_RADIO_GROUP) {
        void **radios = texts > 0 ? hero_layer_alloc_zero((size_t)texts, sizeof *radios) : NULL;
        int64_t *places = texts > 0 ? hero_layer_alloc_zero((size_t)texts * 4, sizeof *places) : NULL;

        if (texts > 0 && (radios == NULL || places == NULL)) {
            hero_layer_free(radios);
            hero_layer_free(places);
            hero_ui_muted -= 1;
            return HERO_UI_LIMIT;
        }
        for (int64_t i = 0; i < widget->radio_count; i += 1) {
            hero_ui_gtk.fixed_remove(widget->widget, widget->radios[i]);
        }
        hero_layer_free(widget->radios);
        hero_layer_free(widget->radio_places);
        widget->radios = radios;
        widget->radio_places = places;
        widget->radio_count = 0;
        for (int64_t i = 0; i < texts; i += 1) {
            int64_t length = 0;
            const uint8_t *text = hero_ui_text_at(items, count, i, &length);
            char *label = hero_ui_gtk_text(text, length, false);
            void *radio = label != NULL ? hero_ui_gtk.check_button_new_with_label(label) : NULL;

            hero_layer_free(label);
            if (radio == NULL) {
                hero_ui_muted -= 1;
                return HERO_UI_LIMIT;
            }
            if (i > 0) {
                hero_ui_gtk.check_button_set_group(radio, radios[0]);
            }
            hero_ui_gtk.widget_set_sensitive(radio, widget->enabled ? 1 : 0);
            hero_ui_gtk.widget_set_visible(radio, 0);
            hero_ui_gtk.fixed_put(widget->widget, radio, 0, 0);
            hero_ui_gtk_connect(radio, "toggled", (hero_ui_gtk_callback)hero_ui_gtk_toggled, widget->id);
            radios[i] = radio;
            places[i * 4 + 2] = -1;
            widget->radio_count = i + 1;
        }
    } else if (widget->kind == HERO_UI_COMBO) {
        void *strings = hero_ui_gtk.string_list_new(NULL);

        if (strings == NULL) {
            hero_ui_muted -= 1;
            return HERO_UI_LIMIT;
        }
        if (texts > 0) {
            hero_ui_gtk.string_list_append(strings, "");
        }
        for (int64_t i = 0; i < texts; i += 1) {
            int64_t length = 0;
            const uint8_t *text = hero_ui_text_at(items, count, i, &length);
            char *made = hero_ui_gtk_text(text, length, false);

            if (made == NULL) {
                hero_ui_gtk.object_unref(strings);
                hero_ui_muted -= 1;
                return HERO_UI_LIMIT;
            }
            hero_ui_gtk.string_list_append(strings, made);
            hero_layer_free(made);
        }
        hero_ui_gtk.drop_down_set_model(widget->inner, strings);
        hero_ui_gtk.object_unref(strings);
        widget->strings = strings;
        widget->items = texts;
        widget->placeholder = texts > 0;
        hero_ui_gtk.drop_down_set_selected(widget->inner, texts > 0 ? 0 : HERO_GTK_INVALID_LIST_POSITION);
    } else {
        void *row;

        while ((row = hero_ui_gtk.list_box_get_row_at_index(widget->inner, 0)) != NULL) {
            hero_ui_gtk.list_box_remove(widget->inner, row);
        }
        widget->items = 0;
        for (int64_t i = 0; i < texts; i += 1) {
            int64_t length = 0;
            const uint8_t *text = hero_ui_text_at(items, count, i, &length);
            char *made = hero_ui_gtk_text(text, length, false);
            void *label = made != NULL ? hero_ui_gtk.label_new(made) : NULL;

            hero_layer_free(made);
            if (label == NULL) {
                hero_ui_muted -= 1;
                return HERO_UI_LIMIT;
            }
            hero_ui_gtk.label_set_xalign(label, 0.0f);
            hero_ui_gtk.list_box_append(widget->inner, label);
            widget->items += 1;
        }
        hero_ui_gtk.list_box_unselect_all(widget->inner);
    }
    hero_ui_muted -= 1;
    if (widget->kind == HERO_UI_RADIO_GROUP) {
        hero_ui_gtk_put_radios(widget);
    }
    return HERO_UI_OK;
}

/* Chooses item `item` of combo box `combo`, -1 for none, the empty row of no
 * choice put in or taken out as the choice needs. */
static void hero_ui_gtk_choose(hero_ui_native *combo, int64_t item) {
    if (item < 0 && combo->items > 0 && !combo->placeholder) {
        const char *const empty[] = {"", NULL};

        hero_ui_gtk.string_list_splice(combo->strings, 0, 0, empty);
        combo->placeholder = true;
    }
    if (item >= 0 && combo->placeholder) {
        combo->placeholder = false;
        hero_ui_gtk.string_list_remove(combo->strings, 0);
    }
    hero_ui_gtk.drop_down_set_selected(combo->inner, item < 0 ? (combo->placeholder ? 0 : HERO_GTK_INVALID_LIST_POSITION) : (unsigned)item);
}

/* Shows a slider's or a progress bar's range and value. */
static void hero_ui_gtk_show_range(hero_ui_native *widget) {
    if (widget->kind == HERO_UI_SLIDER) {
        hero_ui_gtk.range_set_range(widget->widget, hero_ui_gtk_double(widget, widget->least), hero_ui_gtk_double(widget, widget->most));
        hero_ui_gtk.range_set_value(widget->widget, hero_ui_gtk_double(widget, widget->value));
    } else {
        hero_ui_gtk.progress_bar_set_fraction(widget->widget, hero_ui_fraction(widget->value, widget->least, widget->most));
    }
}

static int64_t hero_ui_arm_set_value(int64_t id, int64_t value) {
    hero_ui_native *widget = hero_ui_find(id);
    int64_t answer = HERO_UI_OK;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    hero_ui_muted += 1;
    switch (widget->kind) {
    case HERO_UI_CHECKBOX:
        if (value != 0 && value != 1) {
            answer = HERO_UI_INVALID;
            break;
        }
        hero_ui_gtk.check_button_set_active(widget->widget, value == 1 ? 1 : 0);
        break;
    case HERO_UI_SLIDER:
    case HERO_UI_PROGRESS:
        if (value < widget->least || value > widget->most) {
            answer = HERO_UI_INVALID;
            break;
        }
        widget->value = value;
        hero_ui_gtk_show_range(widget);
        break;
    case HERO_UI_COMBO:
        if (value < -1 || value >= widget->items) {
            answer = HERO_UI_INVALID;
            break;
        }
        hero_ui_gtk_choose(widget, value);
        break;
    case HERO_UI_RADIO_GROUP:
        if (value < -1 || value >= widget->radio_count) {
            answer = HERO_UI_INVALID;
            break;
        }
        for (int64_t i = 0; i < widget->radio_count; i += 1) {
            hero_ui_gtk.check_button_set_active(widget->radios[i], i == value ? 1 : 0);
        }
        break;
    default:
        answer = HERO_UI_INVALID;
        break;
    }
    hero_ui_muted -= 1;
    return answer;
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
    hero_ui_muted += 1;
    hero_ui_gtk_show_range(widget);
    hero_ui_muted -= 1;
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
    hero_ui_gtk.list_box_unselect_all(widget->inner);
    for (int64_t i = 0; i < count; i += 1) {
        hero_ui_gtk.list_box_select_row(widget->inner, hero_ui_gtk.list_box_get_row_at_index(widget->inner, (int)indices[i]));
    }
    hero_ui_muted -= 1;
    return HERO_UI_OK;
}

/* Shows `widget` where it is placed and asked to be. */
static void hero_ui_gtk_show_placed(hero_ui_native *widget) {
    hero_ui_gtk.widget_set_visible(widget->widget, widget->placed && widget->visible ? 1 : 0);
}

static int64_t hero_ui_arm_set_flag(int64_t id, int64_t which, int64_t on) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (which == HERO_UI_FLAG_RESIZABLE) {
        if (widget->kind != HERO_UI_WINDOW) {
            return HERO_UI_INVALID;
        }
        hero_ui_gtk.window_set_resizable(widget->widget, on == 1 ? 1 : 0);
        return HERO_UI_OK;
    }
    if (which == HERO_UI_FLAG_VISIBLE) {
        widget->visible = on == 1;
        switch (widget->kind) {
        case HERO_UI_WINDOW:
            if (on == 1) {
                hero_ui_gtk.window_present(widget->widget);
            } else {
                hero_ui_gtk.widget_set_visible(widget->widget, 0);
            }
            return HERO_UI_OK;
        case HERO_UI_MENU:
            return hero_ui_gtk_fill_bar(hero_ui_find(widget->parent));
        case HERO_UI_MENU_ITEM:
        case HERO_UI_MENU_SEPARATOR:
            return hero_ui_gtk_fill_menu(hero_ui_find(widget->parent));
        default:
            hero_ui_gtk_show_placed(widget);
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
            return hero_ui_gtk_fill_menu(widget);
        case HERO_UI_MENU_ITEM:
        case HERO_UI_MENU_SEPARATOR:
            return hero_ui_gtk_fill_menu(hero_ui_find(widget->parent));
        default:
            for (int64_t i = 0; i < widget->radio_count; i += 1) {
                hero_ui_gtk.widget_set_sensitive(widget->radios[i], on == 1 ? 1 : 0);
            }
            hero_ui_gtk.widget_set_sensitive(widget->widget, on == 1 ? 1 : 0);
            return HERO_UI_OK;
        }
    }
    return HERO_UI_INVALID;
}

/* GDK's key value of the key `hero_ui.h` names; false for a name it does not
 * list. */
static bool hero_ui_gtk_key_of(const uint8_t *key, int64_t count, unsigned *keyval) {
    static const struct {
        const char *name;
        unsigned keyval;
    } named[] = {
        {"Enter", 0xFF0Du}, {"Escape", 0xFF1Bu}, {"Tab", 0xFF09u},  {"Space", 0x20u},   {"Delete", 0xFFFFu}, {"Backspace", 0xFF08u}, {"Up", 0xFF52u},
        {"Down", 0xFF54u},  {"Left", 0xFF51u},   {"Right", 0xFF53u}, {"Home", 0xFF50u}, {"End", 0xFF57u},    {"PageUp", 0xFF55u},    {"PageDown", 0xFF56u},
    };

    if (count == 1 && ((key[0] >= 'a' && key[0] <= 'z') || (key[0] >= '0' && key[0] <= '9'))) {
        *keyval = key[0];
        return true;
    }
    if (count >= 2 && count <= 3 && key[0] == 'F') {
        unsigned n = 0;

        for (int64_t i = 1; i < count; i += 1) {
            if (key[i] < '0' || key[i] > '9') {
                return false;
            }
            n = n * 10u + (unsigned)(key[i] - '0');
        }
        if (n >= 1 && n <= 12 && !(count == 3 && key[1] == '0')) {
            *keyval = HERO_GDK_KEY_F1 + n - 1;
            return true;
        }
        return false;
    }
    for (size_t i = 0; i < sizeof named / sizeof named[0]; i += 1) {
        if (strlen(named[i].name) == (size_t)count && memcmp(named[i].name, key, (size_t)count) == 0) {
            *keyval = named[i].keyval;
            return true;
        }
    }
    return false;
}

static int64_t hero_ui_arm_set_shortcut(int64_t id, const uint8_t *key, int64_t count, int64_t modifiers) {
    hero_ui_native *item = hero_ui_find(id);
    hero_ui_native *window;
    unsigned keyval = 0;

    if (item == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (item->kind != HERO_UI_MENU_ITEM || (count > 0 && !hero_ui_gtk_key_of(key, count, &keyval))) {
        return HERO_UI_INVALID;
    }
    window = hero_ui_gtk_window(item->window);
    if (window == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (item->shortcut != NULL) {
        hero_ui_gtk.shortcut_controller_remove_shortcut(window->shortcuts, item->shortcut);
        item->shortcut = NULL;
    }
    item->keyval = keyval;
    item->held = ((modifiers & HERO_UI_PRIMARY) != 0 ? HERO_GDK_CONTROL_MASK : 0u) | ((modifiers & HERO_UI_SHIFT) != 0 ? HERO_GDK_SHIFT_MASK : 0u) | ((modifiers & HERO_UI_ALT) != 0 ? HERO_GDK_ALT_MASK : 0u);
    if (keyval != 0) {
        char action[64];
        void *trigger = hero_ui_gtk.keyval_trigger_new(keyval, item->held);
        void *named;

        hero_ui_gtk_action_name(item->id, true, action, sizeof action);
        named = hero_ui_gtk.named_action_new(action);
        item->shortcut = trigger != NULL && named != NULL ? hero_ui_gtk.shortcut_new(trigger, named) : NULL;
        if (item->shortcut == NULL) {
            return HERO_UI_LIMIT;
        }
        hero_ui_gtk.shortcut_controller_add_shortcut(window->shortcuts, item->shortcut);
    }
    return hero_ui_gtk_fill_menu(hero_ui_find(item->parent));
}

static int64_t hero_ui_arm_set_image(int64_t id, const uint8_t *rgba, int64_t width, int64_t height) {
    hero_ui_native *picture = hero_ui_find(id);
    void *bytes;
    void *texture;

    if (picture == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (picture->kind != HERO_UI_IMAGE) {
        return HERO_UI_INVALID;
    }
    if (width == 0 || height == 0) {
        hero_ui_gtk.picture_set_paintable(picture->widget, NULL);
        picture->image_width = 0;
        picture->image_height = 0;
        return HERO_UI_OK;
    }
    if (width > INT32_MAX / 4 || height > INT32_MAX / 4) {
        return HERO_UI_LIMIT;
    }
    bytes = hero_ui_gtk.bytes_new(rgba, (size_t)(width * height * 4));
    texture = bytes != NULL ? hero_ui_gtk.memory_texture_new((int)width, (int)height, HERO_GDK_MEMORY_R8G8B8A8, bytes, (size_t)width * 4) : NULL;
    if (bytes != NULL) {
        hero_ui_gtk.bytes_unref(bytes);
    }
    if (texture == NULL) {
        return HERO_UI_LIMIT;
    }
    hero_ui_gtk.picture_set_paintable(picture->widget, texture);
    hero_ui_gtk.object_unref(texture);
    picture->image_width = width;
    picture->image_height = height;
    return HERO_UI_OK;
}

static int64_t hero_ui_arm_set_size(int64_t id, int64_t which, int64_t width, int64_t height) {
    hero_ui_native *window = hero_ui_find(id);

    if (window == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (window->kind != HERO_UI_WINDOW) {
        return HERO_UI_INVALID;
    }
    if (which == HERO_UI_SIZE_LEAST) {
        window->least_width = width;
        window->least_height = height;
        hero_ui_gtk.widget_set_size_request(window->area, hero_ui_gtk_int(width), hero_ui_gtk_int(height));
        return HERO_UI_OK;
    }
    if (width == window->content_width && height == window->content_height && hero_ui_gtk.widget_get_width(window->area) == hero_ui_gtk_int(width) && hero_ui_gtk.widget_get_height(window->area) == hero_ui_gtk_int(height)) {
        return HERO_UI_OK;
    }
    window->content_width = width;
    window->content_height = height;
    hero_ui_gtk_size_content(window);
    return HERO_UI_OK;
}

static int64_t hero_ui_arm_place(int64_t id, int64_t index, int64_t x, int64_t y, int64_t width, int64_t height) {
    hero_ui_native *widget = hero_ui_find(id);
    void *container;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    container = hero_ui_gtk_container(widget);
    if (widget->widget == NULL || widget->kind == HERO_UI_WINDOW || container == NULL) {
        return HERO_UI_INVALID;
    }
    if (index >= 0) {
        int64_t *place;

        if (widget->kind != HERO_UI_RADIO_GROUP || index >= widget->radio_count) {
            return HERO_UI_INVALID;
        }
        place = &widget->radio_places[index * 4];
        place[0] = x;
        place[1] = y;
        place[2] = width;
        place[3] = height;
        hero_ui_gtk_put_radios(widget);
        return HERO_UI_OK;
    }
    widget->x = x;
    widget->y = y;
    widget->width = width;
    widget->height = height;
    widget->placed = true;
    if (widget->kind == HERO_UI_IMAGE && widget->image_width > 0 && widget->image_width <= width && widget->image_height <= height) {
        /* A picture stands at its own size in the middle of its place, as
         * AppKit draws one. */
        x += (width - widget->image_width) / 2;
        y += (height - widget->image_height) / 2;
        width = widget->image_width;
        height = widget->image_height;
    }
    hero_ui_gtk.fixed_move(container, widget->widget, (double)hero_ui_gtk_int(x), (double)hero_ui_gtk_int(y));
    hero_ui_gtk.widget_set_size_request(widget->widget, hero_ui_gtk_int(width), hero_ui_gtk_int(height));
    if (widget->kind == HERO_UI_RADIO_GROUP) {
        hero_ui_gtk_put_radios(widget);
    }
    hero_ui_gtk_show_placed(widget);
    return HERO_UI_OK;
}

/* The size GTK draws `widget` at as it stands with its text and items,
 * apart from the size it was placed at, and shown for the moment it is
 * measured: GTK 4 measures a widget it does not show as 0 by 0, and this arm
 * hides one until it is placed (measured under Xvfb, GTK 4.18.6). */
static hero_ui_gtk_size hero_ui_gtk_natural(void *widget) {
    int width = -1;
    int height = -1;
    int shown = hero_ui_gtk.widget_get_visible(widget);
    hero_ui_gtk_size least;
    hero_ui_gtk_size natural;

    hero_ui_gtk.widget_get_size_request(widget, &width, &height);
    hero_ui_gtk.widget_set_size_request(widget, -1, -1);
    if (!shown) {
        hero_ui_gtk.widget_set_visible(widget, 1);
    }
    hero_ui_gtk.widget_get_preferred_size(widget, &least, &natural);
    if (!shown) {
        hero_ui_gtk.widget_set_visible(widget, 0);
    }
    hero_ui_gtk.widget_set_size_request(widget, width, height);
    if (natural.width < least.width) {
        natural.width = least.width;
    }
    if (natural.height < least.height) {
        natural.height = least.height;
    }
    return natural;
}

static int64_t hero_ui_arm_preferred(int64_t id, int64_t *width, int64_t *height) {
    hero_ui_native *widget = hero_ui_find(id);
    hero_ui_gtk_size size = {0, 0};

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    *width = 0;
    *height = 0;
    switch (widget->kind) {
    case HERO_UI_LABEL:
    case HERO_UI_BUTTON:
    case HERO_UI_CHECKBOX:
    case HERO_UI_COMBO:
        size = hero_ui_gtk_natural(widget->widget);
        break;
    case HERO_UI_RADIO_GROUP:
        for (int64_t i = 0; i < widget->radio_count; i += 1) {
            hero_ui_gtk_size item = hero_ui_gtk_natural(widget->radios[i]);

            size.width = item.width > size.width ? item.width : size.width;
            size.height += item.height;
        }
        break;
    case HERO_UI_TEXT_FIELD:
    case HERO_UI_PROGRESS:
    case HERO_UI_SLIDER:
        /* The headless side's width, and GTK's height. */
        size = hero_ui_gtk_natural(widget->widget);
        size.width = 160;
        break;
    case HERO_UI_TEXT_AREA:
        size.width = 240;
        size.height = 96;
        break;
    case HERO_UI_LIST:
    case HERO_UI_LIST_MULTIPLE:
        size.width = 160;
        size.height = 96;
        break;
    case HERO_UI_IMAGE:
        *width = widget->image_width;
        *height = widget->image_height;
        return HERO_UI_OK;
    default:
        return HERO_UI_OK;
    }
    *width = size.width;
    *height = size.height;
    return HERO_UI_OK;
}

static int64_t hero_ui_arm_text(int64_t id, uint8_t *buffer, int64_t count, int64_t *length) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    if (widget->kind == HERO_UI_TEXT_FIELD) {
        const char *text = hero_ui_gtk.editable_get_text(widget->widget);
        size_t bytes = text != NULL ? strlen(text) : 0;

        *length = (int64_t)bytes;
        if (bytes > 0 && *length <= count) {
            memcpy(buffer, text, bytes);
        }
        return HERO_UI_OK;
    }
    if (widget->kind == HERO_UI_TEXT_AREA) {
        hero_ui_gtk_iter start;
        hero_ui_gtk_iter end;
        char *text;
        size_t bytes;

        hero_ui_gtk.text_buffer_get_start_iter(widget->buffer, &start);
        hero_ui_gtk.text_buffer_get_end_iter(widget->buffer, &end);
        text = hero_ui_gtk.text_buffer_get_text(widget->buffer, &start, &end, 1);
        if (text == NULL) {
            return HERO_UI_LIMIT;
        }
        bytes = strlen(text);
        *length = (int64_t)bytes;
        if (bytes > 0 && *length <= count) {
            memcpy(buffer, text, bytes);
        }
        hero_ui_gtk.g_free(text);
        return HERO_UI_OK;
    }
    return HERO_UI_INVALID;
}

/* The item radio group `group` has checked, -1 for none. */
static int64_t hero_ui_gtk_checked_radio(const hero_ui_native *group) {
    for (int64_t i = 0; i < group->radio_count; i += 1) {
        if (hero_ui_gtk.check_button_get_active(group->radios[i])) {
            return i;
        }
    }
    return -1;
}

static int64_t hero_ui_arm_value(int64_t id, int64_t *value) {
    hero_ui_native *widget = hero_ui_find(id);

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    switch (widget->kind) {
    case HERO_UI_CHECKBOX:
        *value = hero_ui_gtk.check_button_get_active(widget->widget) ? 1 : 0;
        return HERO_UI_OK;
    case HERO_UI_SLIDER:
    case HERO_UI_PROGRESS:
        *value = widget->value;
        return HERO_UI_OK;
    case HERO_UI_COMBO:
        *value = hero_ui_gtk_combo_choice(widget);
        return HERO_UI_OK;
    case HERO_UI_RADIO_GROUP:
        *value = hero_ui_gtk_checked_radio(widget);
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
    if (widget->kind == HERO_UI_LIST || widget->kind == HERO_UI_LIST_MULTIPLE) {
        int64_t found = 0;

        for (int64_t i = 0; i < widget->items; i += 1) {
            void *row = hero_ui_gtk.list_box_get_row_at_index(widget->inner, (int)i);

            if (row != NULL && hero_ui_gtk.list_box_row_is_selected(row)) {
                if (found < count) {
                    indices[found] = i;
                }
                found += 1;
            }
        }
        *chosen = found;
        return HERO_UI_OK;
    }
    if (widget->kind == HERO_UI_COMBO) {
        one = hero_ui_gtk_combo_choice(widget);
    } else if (widget->kind == HERO_UI_RADIO_GROUP) {
        one = hero_ui_gtk_checked_radio(widget);
    } else {
        return HERO_UI_INVALID;
    }
    *chosen = one >= 0 ? 1 : 0;
    if (one >= 0 && count >= 1) {
        indices[0] = one;
    }
    return HERO_UI_OK;
}

/* Wakes the loop `pump` blocks in: the deadline's timeout source. */
static int hero_ui_gtk_woke(void *data) {
    *(bool *)data = true;
    return 0;
}

/* The monotonic clock, in milliseconds. */
static int64_t hero_ui_gtk_now(void) {
    struct timespec now;

    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
        return 0;
    }
    return (int64_t)now.tv_sec * 1000 + (int64_t)now.tv_nsec / 1000000;
}

static int64_t hero_ui_arm_pump(int64_t ms) {
    uint64_t before = hero_ui_pushed;
    int64_t deadline = hero_ui_gtk_now() + hero_ui_between(ms, 0, INT32_MAX);

    for (;;) {
        int64_t now;
        bool woke = false;
        unsigned timer;

        while (hero_ui_pushed == before && hero_ui_gtk.main_context_iteration(NULL, 0)) {
        }
        now = hero_ui_gtk_now();
        if (hero_ui_pushed != before || now >= deadline) {
            return HERO_UI_OK;
        }
        timer = hero_ui_gtk.timeout_add((unsigned)(deadline - now), hero_ui_gtk_woke, &woke);
        (void)hero_ui_gtk.main_context_iteration(NULL, 1);
        if (!woke) {
            (void)hero_ui_gtk.source_remove(timer);
        }
    }
}

static int64_t hero_ui_arm_inject(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count) {
    hero_ui_native *widget = hero_ui_find(id);
    int64_t answer = HERO_UI_OK;
    bool usable;

    if (widget == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    usable = widget->widget != NULL && hero_ui_gtk.widget_is_sensitive(widget->widget);
    hero_ui_gtk_injecting += 1;
    switch (kind) {
    case HERO_UI_EVENT_CLICK:
    case HERO_UI_EVENT_TOGGLE:
        if (widget->kind == HERO_UI_BUTTON) {
            if (usable) {
                hero_ui_gtk.signal_emit_by_name(widget->widget, "clicked");
            }
        } else if (widget->kind == HERO_UI_CHECKBOX) {
            if (usable) {
                hero_ui_gtk.check_button_set_active(widget->widget, hero_ui_gtk.check_button_get_active(widget->widget) ? 0 : 1);
            }
        } else {
            answer = HERO_UI_INVALID;
        }
        break;
    case HERO_UI_EVENT_TEXT:
        if (widget->kind == HERO_UI_TEXT_FIELD) {
            char *typed = hero_ui_gtk_text(text, count, false);
            int position = 0;

            if (typed == NULL) {
                answer = HERO_UI_LIMIT;
            } else if (usable && hero_ui_gtk.editable_get_editable(widget->widget)) {
                hero_ui_gtk.editable_delete_text(widget->widget, 0, -1);
                hero_ui_gtk.editable_insert_text(widget->widget, typed, (int)count, &position);
            }
            hero_layer_free(typed);
        } else if (widget->kind == HERO_UI_TEXT_AREA) {
            char *typed = hero_ui_gtk_text(text, count, false);

            if (typed == NULL) {
                answer = HERO_UI_LIMIT;
            } else if (usable && hero_ui_gtk.text_view_get_editable(widget->inner)) {
                hero_ui_gtk_iter start;
                hero_ui_gtk_iter end;

                hero_ui_gtk.text_buffer_begin_user_action(widget->buffer);
                hero_ui_gtk.text_buffer_get_start_iter(widget->buffer, &start);
                hero_ui_gtk.text_buffer_get_end_iter(widget->buffer, &end);
                (void)hero_ui_gtk.text_buffer_delete_interactive(widget->buffer, &start, &end, 1);
                (void)hero_ui_gtk.text_buffer_insert_interactive_at_cursor(widget->buffer, typed, (int)count, 1);
                hero_ui_gtk.text_buffer_end_user_action(widget->buffer);
            }
            hero_layer_free(typed);
        } else {
            answer = HERO_UI_INVALID;
        }
        break;
    case HERO_UI_EVENT_SELECT:
        if (widget->kind == HERO_UI_LIST || widget->kind == HERO_UI_LIST_MULTIPLE) {
            if (value >= widget->items) {
                answer = HERO_UI_INVALID;
            } else if (usable) {
                if (value < 0 || second != 1 || widget->kind == HERO_UI_LIST) {
                    hero_ui_gtk.list_box_unselect_all(widget->inner);
                }
                if (value >= 0) {
                    hero_ui_gtk.list_box_select_row(widget->inner, hero_ui_gtk.list_box_get_row_at_index(widget->inner, (int)value));
                }
            }
        } else if (widget->kind == HERO_UI_COMBO) {
            if (value < 0 || value >= widget->items) {
                answer = HERO_UI_INVALID;
            } else if (usable) {
                /* The empty row of no choice is taken out first, muted, as
                 * the person's choice would take it out after; a choice of
                 * what stands is told, as AppKit's is. */
                if (widget->placeholder) {
                    hero_ui_muted += 1;
                    widget->placeholder = false;
                    hero_ui_gtk.string_list_remove(widget->strings, 0);
                    hero_ui_muted -= 1;
                }
                if (hero_ui_gtk_combo_choice(widget) == value) {
                    hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, value, 0);
                } else {
                    hero_ui_gtk.drop_down_set_selected(widget->inner, (unsigned)value);
                }
            }
        } else if (widget->kind == HERO_UI_RADIO_GROUP) {
            if (value < 0 || value >= widget->radio_count) {
                answer = HERO_UI_INVALID;
            } else if (usable && hero_ui_gtk.widget_is_sensitive(widget->radios[value])) {
                if (hero_ui_gtk.check_button_get_active(widget->radios[value])) {
                    hero_ui_tell(HERO_UI_EVENT_SELECT, widget->id, value, 0);
                } else {
                    hero_ui_gtk.check_button_set_active(widget->radios[value], 1);
                }
            }
        } else {
            answer = HERO_UI_INVALID;
        }
        break;
    case HERO_UI_EVENT_VALUE:
        if (widget->kind != HERO_UI_SLIDER) {
            answer = HERO_UI_INVALID;
        } else if (usable) {
            int64_t held = hero_ui_between(value, widget->least, widget->most);

            if (held == widget->value) {
                hero_ui_tell(HERO_UI_EVENT_VALUE, widget->id, held, 0);
            } else {
                hero_ui_gtk.range_set_value(widget->widget, hero_ui_gtk_double(widget, held));
            }
        }
        break;
    case HERO_UI_EVENT_MENU:
        if (widget->kind != HERO_UI_MENU_ITEM) {
            answer = HERO_UI_INVALID;
        } else if (hero_ui_gtk_usable(widget)) {
            hero_ui_gtk.action_activate(widget->action, NULL);
        }
        break;
    case HERO_UI_EVENT_CLOSE:
        if (widget->kind != HERO_UI_WINDOW) {
            answer = HERO_UI_INVALID;
        } else {
            if (!hero_ui_gtk.widget_get_realized(widget->widget)) {
                hero_ui_gtk.widget_realize(widget->widget);
            }
            hero_ui_gtk.window_close(widget->widget);
        }
        break;
    case HERO_UI_EVENT_RESIZE:
        if (widget->kind != HERO_UI_WINDOW) {
            answer = HERO_UI_INVALID;
        } else {
            int64_t content_width = widget->content_width;
            int64_t content_height = widget->content_height;

            widget->content_width = hero_ui_between(value, widget->least_width, INT64_C(1) << 24);
            widget->content_height = hero_ui_between(second, widget->least_height, INT64_C(1) << 24);
            hero_ui_gtk_size_content(widget);
            /* The content keeps its size until GTK gives it the new one,
             * which its `resize` then tells as the person's. */
            widget->resize_injected = true;
            widget->content_width = content_width;
            widget->content_height = content_height;
        }
        break;
    default:
        answer = HERO_UI_INVALID;
        break;
    }
    hero_ui_gtk_injecting -= 1;
    return answer;
}

/* The `GtkWindow` of number `window` for a dialog to stand over, NULL for 0;
 * false where that number is no window. */
static bool hero_ui_gtk_owner(int64_t window, void **owner, int64_t *answer) {
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
    *owner = found->widget;
    return true;
}

/* What a dialog run in a nested loop answers. */
typedef struct hero_ui_gtk_answer {
    bool done;
    int response;
    void *value;
    hero_ui_gerror *error;
    int multiple;
} hero_ui_gtk_answer;

/* Runs GLib's loop until `answer` is done: a dialog is the person's for as
 * long as they take, as every system's modal dialog is. */
static void hero_ui_gtk_await(hero_ui_gtk_answer *answer) {
    while (!answer->done) {
        (void)hero_ui_gtk.main_context_iteration(NULL, 1);
    }
}

static void hero_ui_gtk_alert_answered(void *dialog, void *result, void *data) {
    hero_ui_gtk_answer *answer = data;

    answer->response = hero_ui_gtk.alert_dialog_choose_finish(dialog, result, &answer->error);
    answer->done = true;
}

static void hero_ui_gtk_dialog_answered(void *dialog, int response, void *data) {
    hero_ui_gtk_answer *answer = data;

    (void)dialog;
    answer->response = response;
    answer->done = true;
}

static int64_t hero_ui_arm_message(int64_t window, int64_t tone, int64_t buttons, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count, int64_t *answer) {
    void *owner;
    int64_t refused = HERO_UI_OK;
    char *heading = hero_ui_gtk_text(title, title_count, false);
    char *said = hero_ui_gtk_text(text, text_count, false);
    hero_ui_gtk_answer got = {false, -1, NULL, NULL, 0};
    /* The buttons in the order shown, the answer each gives, GTK's own word
     * for each, and which one a dismissed dialog answers. */
    int64_t answers[3];
    const char *msgids[3];
    int shown = 0;
    int cancel = -1;

    if (heading == NULL || said == NULL) {
        hero_layer_free(heading);
        hero_layer_free(said);
        return HERO_UI_LIMIT;
    }
    if (!hero_ui_gtk_owner(window, &owner, &refused)) {
        hero_layer_free(heading);
        hero_layer_free(said);
        return refused;
    }
    if (buttons == HERO_UI_BUTTONS_YES_NO || buttons == HERO_UI_BUTTONS_YES_NO_CANCEL) {
        if (buttons == HERO_UI_BUTTONS_YES_NO_CANCEL) {
            cancel = shown;
            answers[shown] = HERO_UI_ANSWER_CANCEL;
            msgids[shown++] = "_Cancel";
        }
        answers[shown] = HERO_UI_ANSWER_NO;
        msgids[shown++] = "_No";
        answers[shown] = HERO_UI_ANSWER_YES;
        msgids[shown++] = "_Yes";
    } else {
        if (buttons == HERO_UI_BUTTONS_OK_CANCEL) {
            cancel = shown;
            answers[shown] = HERO_UI_ANSWER_CANCEL;
            msgids[shown++] = "_Cancel";
        }
        answers[shown] = HERO_UI_ANSWER_OK;
        msgids[shown++] = "_OK";
    }
    if (hero_ui_gtk.alert_dialog_new != NULL && hero_ui_gtk.alert_dialog_choose != NULL && hero_ui_gtk.alert_dialog_choose_finish != NULL && hero_ui_gtk.alert_dialog_set_buttons != NULL && hero_ui_gtk.alert_dialog_set_detail != NULL && hero_ui_gtk.alert_dialog_set_cancel_button != NULL && hero_ui_gtk.alert_dialog_set_default_button != NULL && hero_ui_gtk.alert_dialog_set_modal != NULL) {
        char words[3][64];
        const char *labels[4] = {NULL, NULL, NULL, NULL};
        void *dialog = hero_ui_gtk.alert_dialog_new("%s", heading);

        if (dialog == NULL) {
            hero_layer_free(heading);
            hero_layer_free(said);
            return HERO_UI_LIMIT;
        }
        for (int i = 0; i < shown; i += 1) {
            hero_ui_gtk_words_of(msgids[i], words[i], sizeof words[i]);
            labels[i] = words[i];
        }
        hero_ui_gtk.alert_dialog_set_detail(dialog, said);
        hero_ui_gtk.alert_dialog_set_buttons(dialog, labels);
        hero_ui_gtk.alert_dialog_set_cancel_button(dialog, cancel);
        hero_ui_gtk.alert_dialog_set_default_button(dialog, shown - 1);
        hero_ui_gtk.alert_dialog_set_modal(dialog, 1);
        hero_ui_gtk.alert_dialog_choose(dialog, owner, NULL, hero_ui_gtk_alert_answered, &got);
        hero_ui_gtk_await(&got);
        if (got.error != NULL) {
            hero_ui_gtk.error_free(got.error);
        }
        hero_ui_gtk.object_unref(dialog);
    } else {
        int type = tone == HERO_UI_TONE_ERROR ? HERO_GTK_MESSAGE_ERROR : (tone == HERO_UI_TONE_WARNING ? HERO_GTK_MESSAGE_WARNING : HERO_GTK_MESSAGE_INFO);
        void *dialog = hero_ui_gtk.message_dialog_new(owner, HERO_GTK_DIALOG_MODAL | HERO_GTK_DIALOG_DESTROY_WITH_PARENT, type, HERO_GTK_BUTTONS_NONE, "%s", heading);

        if (dialog == NULL) {
            hero_layer_free(heading);
            hero_layer_free(said);
            return HERO_UI_LIMIT;
        }
        hero_ui_gtk.message_dialog_format_secondary_text(dialog, "%s", said);
        for (int i = 0; i < shown; i += 1) {
            (void)hero_ui_gtk.dialog_add_button(dialog, hero_ui_gtk.dgettext("gtk40", msgids[i]), i);
        }
        hero_ui_gtk.dialog_set_default_response(dialog, shown - 1);
        /* The signal's data is the answer, not a widget's number. */
        (void)hero_ui_gtk.signal_connect_data(dialog, "response", (hero_ui_gtk_callback)hero_ui_gtk_dialog_answered, &got, NULL, 0);
        hero_ui_gtk.window_present(dialog);
        hero_ui_gtk_await(&got);
        hero_ui_gtk.window_destroy(dialog);
    }
    hero_layer_free(heading);
    hero_layer_free(said);
    /* A dialog dismissed with no button pressed answers Cancel, as an
     * answer no button gives is taken everywhere else. */
    *answer = got.response >= 0 && got.response < shown ? answers[got.response] : HERO_UI_ANSWER_CANCEL;
    return HERO_UI_OK;
}

static void hero_ui_gtk_file_answered(void *dialog, void *result, void *data) {
    hero_ui_gtk_answer *answer = data;

    if (answer->multiple == 1) {
        answer->value = hero_ui_gtk.file_dialog_open_multiple_finish(dialog, result, &answer->error);
    } else if (answer->multiple == 2) {
        answer->value = hero_ui_gtk.file_dialog_save_finish(dialog, result, &answer->error);
    } else {
        answer->value = hero_ui_gtk.file_dialog_open_finish(dialog, result, &answer->error);
    }
    answer->done = true;
}

/* Keeps the path of GIO file `file`, followed by one zero byte, at the end
 * of `paths`; a file with no local path, or one that is no UTF-8, is passed
 * over, since `hero_ui.h` answers UTF-8 paths. */
static bool hero_ui_gtk_keep_path(void *file, uint8_t **paths, int64_t *count, int64_t *room) {
    char *path = hero_ui_gtk.file_get_path(file);
    size_t bytes = path != NULL ? strlen(path) : 0;

    if (bytes == 0 || !hero_ui_is_utf8((const uint8_t *)path, bytes)) {
        hero_ui_gtk.g_free(path);
        return true;
    }
    if (*count + (int64_t)bytes + 1 > *room) {
        int64_t grown_room = (*room + (int64_t)bytes + 1) * 2;
        uint8_t *grown = hero_layer_grow(*paths, (size_t)grown_room);

        if (grown == NULL) {
            hero_ui_gtk.g_free(path);
            return false;
        }
        *paths = grown;
        *room = grown_room;
    }
    memcpy(*paths + *count, path, bytes);
    *count += (int64_t)bytes;
    (*paths)[*count] = 0;
    *count += 1;
    hero_ui_gtk.g_free(path);
    return true;
}

/* GTK's file filters of `filters`, each filter's name and then its
 * extensions joined by `;`; NULL where it names none. */
static void *hero_ui_gtk_filters(const uint8_t *filters, int64_t count, int64_t *made) {
    int64_t texts = hero_ui_texts_in(filters, count);
    void *store = texts >= 2 ? hero_ui_gtk.list_store_new(hero_ui_gtk.file_filter_get_type()) : NULL;

    *made = 0;
    for (int64_t f = 0; store != NULL && f + 1 < texts; f += 2) {
        int64_t name_length = 0;
        int64_t list_length = 0;
        const uint8_t *name = hero_ui_text_at(filters, count, f, &name_length);
        const uint8_t *list = hero_ui_text_at(filters, count, f + 1, &list_length);
        char *spelled_name = hero_ui_gtk_text(name, name_length, false);
        char *spelled_list = hero_ui_gtk_text(list, list_length, false);
        void *filter = hero_ui_gtk.file_filter_new();

        if (spelled_name == NULL || spelled_list == NULL || filter == NULL) {
            hero_layer_free(spelled_name);
            hero_layer_free(spelled_list);
            continue;
        }
        hero_ui_gtk.file_filter_set_name(filter, spelled_name);
        if (strspn(spelled_list, ";.") == strlen(spelled_list)) {
            /* A filter naming no extension takes every file, as Win32's
             * `*.*` does. */
            hero_ui_gtk.file_filter_add_pattern(filter, "*");
        }
        for (char *each = spelled_list, *end; *each != '\0' || each == spelled_list; each = end + 1) {
            char pattern[256];

            end = strchr(each, ';');
            if (end == NULL) {
                end = each + strlen(each);
            }
            if (*each == '.') {
                each += 1;
            }
            if (end > each && (size_t)(end - each) < sizeof pattern - 3) {
                char held = *end;

                *end = '\0';
                if (hero_ui_gtk.file_filter_add_suffix != NULL) {
                    hero_ui_gtk.file_filter_add_suffix(filter, each);
                } else {
                    (void)snprintf(pattern, sizeof pattern, "*.%s", each);
                    hero_ui_gtk.file_filter_add_pattern(filter, pattern);
                }
                *end = held;
            }
            if (*end == '\0') {
                break;
            }
        }
        hero_ui_gtk.list_store_append(store, filter);
        hero_ui_gtk.object_unref(filter);
        hero_layer_free(spelled_name);
        hero_layer_free(spelled_list);
        *made += 1;
    }
    return store;
}

static int64_t hero_ui_arm_file_dialog(int64_t window, int64_t save, int64_t multiple, const uint8_t *title, int64_t title_count, const uint8_t *name, int64_t name_count, const uint8_t *filters, int64_t filters_count, int64_t *bytes) {
    void *owner;
    int64_t refused = HERO_UI_OK;
    char *heading = hero_ui_gtk_text(title, title_count, false);
    char *proposed = hero_ui_gtk_text(name, name_count, false);
    int64_t filter_count = 0;
    void *store;
    hero_ui_gtk_answer got = {false, -1, NULL, NULL, 0};
    uint8_t *paths = NULL;
    int64_t paths_count = 0;
    int64_t paths_room = 0;
    bool kept = true;
    int64_t result = HERO_UI_OK;

    *bytes = 0;
    if (heading == NULL || proposed == NULL) {
        hero_layer_free(heading);
        hero_layer_free(proposed);
        return HERO_UI_LIMIT;
    }
    if (!hero_ui_gtk_owner(window, &owner, &refused)) {
        hero_layer_free(heading);
        hero_layer_free(proposed);
        return refused;
    }
    store = hero_ui_gtk_filters(filters, filters_count, &filter_count);
    if (hero_ui_gtk.file_dialog_new != NULL && hero_ui_gtk.file_dialog_set_title != NULL && hero_ui_gtk.file_dialog_set_modal != NULL && hero_ui_gtk.file_dialog_set_initial_name != NULL && hero_ui_gtk.file_dialog_set_filters != NULL && hero_ui_gtk.file_dialog_open != NULL && hero_ui_gtk.file_dialog_open_finish != NULL && hero_ui_gtk.file_dialog_open_multiple != NULL && hero_ui_gtk.file_dialog_open_multiple_finish != NULL && hero_ui_gtk.file_dialog_save != NULL && hero_ui_gtk.file_dialog_save_finish != NULL) {
        void *dialog = hero_ui_gtk.file_dialog_new();

        if (dialog == NULL) {
            result = HERO_UI_LIMIT;
        } else {
            if (heading[0] != '\0') {
                hero_ui_gtk.file_dialog_set_title(dialog, heading);
            }
            hero_ui_gtk.file_dialog_set_modal(dialog, 1);
            if (store != NULL && filter_count > 0) {
                hero_ui_gtk.file_dialog_set_filters(dialog, store);
            }
            if (save == 1) {
                got.multiple = 2;
                if (proposed[0] != '\0') {
                    hero_ui_gtk.file_dialog_set_initial_name(dialog, proposed);
                }
                hero_ui_gtk.file_dialog_save(dialog, owner, NULL, hero_ui_gtk_file_answered, &got);
            } else if (multiple == 1) {
                got.multiple = 1;
                hero_ui_gtk.file_dialog_open_multiple(dialog, owner, NULL, hero_ui_gtk_file_answered, &got);
            } else {
                hero_ui_gtk.file_dialog_open(dialog, owner, NULL, hero_ui_gtk_file_answered, &got);
            }
            hero_ui_gtk_await(&got);
            hero_ui_gtk.object_unref(dialog);
        }
    } else {
        char accept[64];
        char cancel[64];
        void *dialog;

        hero_ui_gtk_words_of(save == 1 ? "_Save" : "_Open", accept, sizeof accept);
        hero_ui_gtk_words_of("_Cancel", cancel, sizeof cancel);
        dialog = hero_ui_gtk.file_chooser_native_new(heading[0] != '\0' ? heading : NULL, owner, save == 1 ? HERO_GTK_FILE_CHOOSER_ACTION_SAVE : HERO_GTK_FILE_CHOOSER_ACTION_OPEN, accept, cancel);
        if (dialog == NULL) {
            result = HERO_UI_LIMIT;
        } else {
            unsigned filters_made = store != NULL ? hero_ui_gtk.list_model_get_n_items(store) : 0;

            hero_ui_gtk.native_dialog_set_modal(dialog, 1);
            hero_ui_gtk.file_chooser_set_select_multiple(dialog, save == 0 && multiple == 1 ? 1 : 0);
            if (save == 1 && proposed[0] != '\0') {
                hero_ui_gtk.file_chooser_set_current_name(dialog, proposed);
            }
            for (unsigned i = 0; i < filters_made; i += 1) {
                void *filter = hero_ui_gtk.list_model_get_item(store, i);

                hero_ui_gtk.file_chooser_add_filter(dialog, filter);
                hero_ui_gtk.object_unref(filter);
            }
            (void)hero_ui_gtk.signal_connect_data(dialog, "response", (hero_ui_gtk_callback)hero_ui_gtk_dialog_answered, &got, NULL, 0);
            hero_ui_gtk.native_dialog_show(dialog);
            hero_ui_gtk_await(&got);
            if (got.response == HERO_GTK_RESPONSE_ACCEPT) {
                got.value = hero_ui_gtk.file_chooser_get_files(dialog);
                got.multiple = 1;
            }
            hero_ui_gtk.object_unref(dialog);
        }
    }
    if (store != NULL) {
        hero_ui_gtk.object_unref(store);
    }
    hero_layer_free(heading);
    hero_layer_free(proposed);
    if (got.error != NULL) {
        if (got.error->domain != hero_ui_gtk.dialog_error_quark() || (got.error->code != HERO_GTK_DIALOG_ERROR_CANCELLED && got.error->code != HERO_GTK_DIALOG_ERROR_DISMISSED)) {
            char words[HERO_UI_WORDS_MOST];

            (void)snprintf(words, sizeof words, "GTK's file dialog failed: %s", got.error->message != NULL ? got.error->message : "no words");
            result = hero_ui_refused(HERO_UI_OTHER, words);
        }
        hero_ui_gtk.error_free(got.error);
    }
    if (got.value != NULL) {
        if (got.multiple == 1) {
            unsigned files = hero_ui_gtk.list_model_get_n_items(got.value);

            for (unsigned i = 0; i < files && kept; i += 1) {
                void *file = hero_ui_gtk.list_model_get_item(got.value, i);

                kept = hero_ui_gtk_keep_path(file, &paths, &paths_count, &paths_room);
                hero_ui_gtk.object_unref(file);
            }
        } else {
            kept = hero_ui_gtk_keep_path(got.value, &paths, &paths_count, &paths_room);
        }
        hero_ui_gtk.object_unref(got.value);
    }
    if (!kept) {
        hero_layer_free(paths);
        return HERO_UI_LIMIT;
    }
    if (result != HERO_UI_OK) {
        hero_layer_free(paths);
        return result;
    }
    hero_layer_free(hero_ui_gtk_paths);
    hero_ui_gtk_paths = paths;
    hero_ui_gtk_paths_count = paths_count;
    *bytes = paths_count;
    return HERO_UI_OK;
}

static int64_t hero_ui_arm_dialog_paths(uint8_t *buffer, int64_t count) {
    if (hero_ui_gtk_paths_count > 0 && hero_ui_gtk_paths_count <= count) {
        memcpy(buffer, hero_ui_gtk_paths, (size_t)hero_ui_gtk_paths_count);
    }
    return hero_ui_gtk_paths_count;
}

static int64_t hero_ui_arm_web_load(int64_t id, const uint8_t *url, int64_t count) {
    (void)id;
    (void)url;
    (void)count;
    return hero_ui_refused(HERO_UI_UNSUPPORTED, "the web view is not wired into this arm");
}

static int64_t hero_ui_arm_web_post(int64_t id, const uint8_t *text, int64_t count) {
    (void)id;
    (void)text;
    (void)count;
    return hero_ui_refused(HERO_UI_UNSUPPORTED, "the web view is not wired into this arm");
}

#endif
#endif

/* -- THE THREAD ------------------------------------------------------------ */

#if defined(_WIN32)
/* The thread that opened the process's first app, by the runtime's number
 * for it (`hero_layer_thread.h`); 0 before it. A thread's id is not used:
 * Windows gives a thread started after the owner ended the same id, which
 * was then taken for the UI thread (defect 2673's shape). */
static _Atomic uint64_t hero_ui_owner;
#endif

/* Whether the calling thread is the UI thread, or, on Windows before the
 * first app, may become it. */
static bool hero_ui_thread_is_ours(void) {
#if defined(__APPLE__)
    return pthread_main_np() != 0;
#elif defined(_WIN32)
    uint64_t owner = atomic_load(&hero_ui_owner);
    return owner == 0 || owner == hero_layer_thread();
#else
    return (long)syscall(SYS_gettid) == (long)getpid();
#endif
}

/* Makes the calling thread the UI thread where none is yet, on Windows; and
 * answers whether it is the UI thread. */
static bool hero_ui_thread_claimed(void) {
#if defined(_WIN32)
    uint64_t none = 0;
    uint64_t me = hero_layer_thread();

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

static bool hero_ui_is_utf8(const uint8_t *text, uint64_t length);

/* Queues an event on the native app's queue: what an arm calls, on the UI
 * thread, while it runs the system's loop or acts for `inject`. A web view's
 * message is held to the bridge's rule here, for every arm alike: at most
 * `HERO_UI_WEB_MESSAGE_MOST` bytes of UTF-8 holding no zero byte, else it is
 * dropped and counted lost, as the page could not have sent it. */
static void hero_ui_push(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count) {
    hero_ui_slot *slot;
    hero_ui_event *last;
    hero_ui_event *into;
    uint8_t *copy = NULL;

    if (hero_ui_native_slot == 0 || !hero_ui_thread_is_ours()) {
        return;
    }
    slot = &hero_ui_slots[hero_ui_native_slot - 1];
    if (kind == HERO_UI_EVENT_MESSAGE && (count < 0 || count > HERO_UI_WEB_MESSAGE_MOST || (count > 0 && (text == NULL || memchr(text, 0, (size_t)count) != NULL || !hero_ui_is_utf8(text, (uint64_t)count))))) {
        slot->lost += 1;
        return;
    }
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

/* -- THE WEB VIEW ---------------------------------------------------------- *
 *
 * What every system's web view shares, written once so each answers alike
 * (`hero_ui.h`, THE WEB VIEW; `platform/hero_ui_web.h`): the table of assets
 * a web view serves, its policy and the origins its page may go to, kept
 * here by the web view's number; the answer to every request of the app's
 * scheme; whether a navigation or a message's sender is allowed; and the
 * bridge's script. A web view's record lives from its `create` until its
 * arm says it is gone (`hero_ui_web_gone`, at a `destroy` of it or of its
 * window) or the native app closes, and every record is the UI thread's, as
 * the table of apps is.
 *
 * MEMORY: a record, each asset's three copies and the policy are taken from
 * `hero_layer_alloc` and given back with the record; a call that finds no
 * memory answers `LIMIT` and leaves the table as it was. */

/* One asset: its path, its content type and its body, the layer's copies. */
typedef struct hero_ui_asset {
    char *path;
    size_t path_count;
    char *type;
    size_t type_count;
    uint8_t *body;
    size_t body_count;
} hero_ui_asset;

/* One web view's table, policy and origins. */
typedef struct hero_ui_web {
    int64_t id;
    /* The assets, `held` of them in a block of `room`. */
    hero_ui_asset *assets;
    int64_t held;
    int64_t room;
    /* The bytes their bodies take in all. */
    uint64_t bytes;
    /* The program's policy, NULL for `HERO_UI_WEB_POLICY_DEFAULT`. */
    char *policy;
    size_t policy_count;
    /* The origins the page may go to, each as `normal_origin` writes it. */
    int64_t origin_count;
    char origins[HERO_UI_WEB_ORIGINS_MOST][HERO_UI_WEB_ORIGIN_MOST + 1];
} hero_ui_web;

/* The web views of the native app, `hero_ui_web_held` of them in a block of
 * `hero_ui_web_room`. */
static _Thread_local hero_ui_web **hero_ui_webs;
static _Thread_local int64_t hero_ui_web_held;
static _Thread_local int64_t hero_ui_web_room;

/* The bridge (`hero_ui.h`, THE BRIDGE): written for WebKit's
 * `window.webkit.messageHandlers` and WebView2's `window.chrome.webview`
 * alike, so one script serves every system; it does nothing in a frame
 * inside the page, nor where neither is there. Where an arm's system says
 * not which frame a message came from (WebKitGTK), the arm gives the main
 * frame, before this script, a seal of its own as `window.__heroesSeal`,
 * which the script takes away and puts before every text it sends, so a
 * frame inside the page, which never held it, is not heard. */
static const char hero_ui_web_bridge_text[] =
    "(function () {\n"
    "    \"use strict\";\n"
    "    if (window !== window.top || window.heroes !== undefined) {\n"
    "        return;\n"
    "    }\n"
    "    var MOST = 16777216;\n"
    "    var HELD_MOST = 4096;\n"
    "    var handlers = window.webkit && window.webkit.messageHandlers;\n"
    "    var webkit = handlers && handlers.heroes;\n"
    "    var chrome = window.chrome && window.chrome.webview;\n"
    "    if (!webkit && !chrome) {\n"
    "        return;\n"
    "    }\n"
    "    var seal = \"\";\n"
    "    if (typeof window.__heroesSeal === \"string\") {\n"
    "        seal = window.__heroesSeal;\n"
    "        delete window.__heroesSeal;\n"
    "    }\n"
    "    var post = webkit ? function (text) { webkit.postMessage(seal + text); } : chrome.postMessage.bind(chrome);\n"
    "    var encoder = new TextEncoder();\n"
    "    var receiver = null;\n"
    "    var held = [];\n"
    "    var calls = new Map();\n"
    "    var listeners = new Map();\n"
    "    var next = 1;\n"
    "    function send(text) {\n"
    "        if (typeof text !== \"string\") {\n"
    "            throw new TypeError(\"heroes.send takes a string\");\n"
    "        }\n"
    "        if (text.indexOf(\"\\u0000\") >= 0) {\n"
    "            throw new TypeError(\"heroes.send: the text holds U+0000\");\n"
    "        }\n"
    "        if (text.length * 3 > MOST && encoder.encode(text).length > MOST) {\n"
    "            throw new RangeError(\"heroes.send: the text is longer than \" + MOST + \" bytes of UTF-8\");\n"
    "        }\n"
    "        post(text);\n"
    "    }\n"
    "    function deliver(text) {\n"
    "        if (typeof text !== \"string\") {\n"
    "            return;\n"
    "        }\n"
    "        var framed = null;\n"
    "        if (text.charAt(0) === \"{\") {\n"
    "            try {\n"
    "                framed = JSON.parse(text);\n"
    "            } catch (e) {\n"
    "                framed = null;\n"
    "            }\n"
    "        }\n"
    "        if (framed !== null && typeof framed === \"object\" && framed.heroes === 1) {\n"
    "            if (typeof framed.id === \"number\" && calls.has(framed.id)) {\n"
    "                var call = calls.get(framed.id);\n"
    "                calls.delete(framed.id);\n"
    "                if (Object.prototype.hasOwnProperty.call(framed, \"error\")) {\n"
    "                    var told = framed.error !== null && typeof framed.error === \"object\" ? framed.error : {};\n"
    "                    var error = new Error(typeof told.msg === \"string\" ? told.msg : \"the program refused the call\");\n"
    "                    error.code = typeof told.code === \"string\" ? told.code : \"failed\";\n"
    "                    call.reject(error);\n"
    "                } else {\n"
    "                    call.resolve(framed.ok);\n"
    "                }\n"
    "                return;\n"
    "            }\n"
    "            if (typeof framed.event === \"string\") {\n"
    "                var hearers = listeners.get(framed.event);\n"
    "                if (hearers !== undefined) {\n"
    "                    hearers.slice().forEach(function (hear) {\n"
    "                        hear(framed.data);\n"
    "                    });\n"
    "                }\n"
    "                return;\n"
    "            }\n"
    "        }\n"
    "        if (receiver !== null) {\n"
    "            receiver(text);\n"
    "        } else if (held.length < HELD_MOST) {\n"
    "            held.push(text);\n"
    "        }\n"
    "    }\n"
    "    function receive(fn) {\n"
    "        if (typeof fn !== \"function\") {\n"
    "            throw new TypeError(\"heroes.receive takes a function\");\n"
    "        }\n"
    "        receiver = fn;\n"
    "        var waiting = held;\n"
    "        held = [];\n"
    "        waiting.forEach(function (text) {\n"
    "            fn(text);\n"
    "        });\n"
    "    }\n"
    "    function invoke(name, args) {\n"
    "        return new Promise(function (resolve, reject) {\n"
    "            var id = next;\n"
    "            next += 1;\n"
    "            var text = JSON.stringify({ heroes: 1, id: id, call: String(name), args: args === undefined ? null : args });\n"
    "            calls.set(id, { resolve: resolve, reject: reject });\n"
    "            try {\n"
    "                send(text);\n"
    "            } catch (e) {\n"
    "                calls.delete(id);\n"
    "                reject(e);\n"
    "            }\n"
    "        });\n"
    "    }\n"
    "    function on(name, fn) {\n"
    "        if (typeof name !== \"string\" || typeof fn !== \"function\") {\n"
    "            throw new TypeError(\"heroes.on takes a name and a function\");\n"
    "        }\n"
    "        var hearers = listeners.get(name);\n"
    "        if (hearers === undefined) {\n"
    "            hearers = [];\n"
    "            listeners.set(name, hearers);\n"
    "        }\n"
    "        hearers.push(fn);\n"
    "    }\n"
    "    if (!webkit) {\n"
    "        chrome.addEventListener(\"message\", function (event) {\n"
    "            deliver(event.data);\n"
    "        });\n"
    "    }\n"
    "    Object.defineProperty(window, \"heroes\", {\n"
    "        value: Object.freeze({ send: send, receive: receive, invoke: invoke, on: on, deliver: deliver }),\n"
    "        writable: false,\n"
    "        configurable: false,\n"
    "        enumerable: false\n"
    "    });\n"
    "})();\n"
    ;

static const char *hero_ui_web_bridge(int64_t *count) {
    *count = (int64_t)(sizeof hero_ui_web_bridge_text - 1);
    return hero_ui_web_bridge_text;
}

/* The record of web view `id`, or NULL. */
static hero_ui_web *hero_ui_web_of(int64_t id) {
    for (int64_t i = 0; i < hero_ui_web_held; i += 1) {
        if (hero_ui_webs[i]->id == id) {
            return hero_ui_webs[i];
        }
    }
    return NULL;
}

static void hero_ui_web_free_asset(hero_ui_asset *asset) {
    hero_layer_free(asset->path);
    hero_layer_free(asset->type);
    hero_layer_free(asset->body);
}

/* Gives back `web` and everything it holds. */
static void hero_ui_web_free(hero_ui_web *web) {
    for (int64_t i = 0; i < web->held; i += 1) {
        hero_ui_web_free_asset(&web->assets[i]);
    }
    hero_layer_free(web->assets);
    hero_layer_free(web->policy);
    hero_layer_free(web);
}

static void hero_ui_web_gone(int64_t id) {
    for (int64_t i = 0; i < hero_ui_web_held; i += 1) {
        if (hero_ui_webs[i]->id == id) {
            hero_ui_web_free(hero_ui_webs[i]);
            hero_ui_webs[i] = hero_ui_webs[hero_ui_web_held - 1];
            hero_ui_web_held -= 1;
            return;
        }
    }
}

/* Gives back every web view's record, at the native app's close, after its
 * arm destroyed every widget. */
static void hero_ui_web_give_back_all(void) {
    for (int64_t i = 0; i < hero_ui_web_held; i += 1) {
        hero_ui_web_free(hero_ui_webs[i]);
    }
    hero_layer_free(hero_ui_webs);
    hero_ui_webs = NULL;
    hero_ui_web_held = 0;
    hero_ui_web_room = 0;
}

/* Makes web view `id` in window `parent`: its record first, so the arm may
 * serve the page at once, then the system's widget; a refusal gives the
 * record back. */
static int64_t hero_ui_web_create(int64_t id, int64_t parent) {
    hero_ui_web *web;
    int64_t kind;

    if (hero_ui_web_of(id) != NULL) {
        return hero_ui_answered(HERO_UI_NOT_FOUND, "making a web view", id);
    }
    if (hero_ui_web_held == hero_ui_web_room) {
        int64_t room = hero_ui_web_room == 0 ? 4 : hero_ui_web_room * 2;
        hero_ui_web **grown = hero_layer_grow(hero_ui_webs, (size_t)room * sizeof *grown);

        if (grown == NULL) {
            return hero_ui_answered(HERO_UI_LIMIT, "making a web view", id);
        }
        hero_ui_webs = grown;
        hero_ui_web_room = room;
    }
    web = hero_layer_alloc_zero(1, sizeof *web);
    if (web == NULL) {
        return hero_ui_answered(HERO_UI_LIMIT, "making a web view", id);
    }
    web->id = id;
    hero_ui_webs[hero_ui_web_held] = web;
    hero_ui_web_held += 1;
    kind = hero_ui_arm_create(id, HERO_UI_WEBVIEW, parent);
    if (kind != HERO_UI_OK) {
        hero_ui_web_gone(id);
    }
    return hero_ui_answered(kind, "making a web view", id);
}

/* Whether `c` may stand in a segment of an asset's path. */
static bool hero_ui_web_path_byte(uint8_t c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-' || c == '.' || c == '_' || c == '~';
}

/* Whether the `count` bytes at `path` are an asset's path, or, where
 * `folder`, a folder's too, ending in `/` (`hero_ui.h`, THE WEB VIEW). */
static bool hero_ui_web_path_ok(const uint8_t *path, size_t count, bool folder) {
    size_t start = 1;

    if (count < 1 || count > HERO_UI_WEB_PATH_MOST || path[0] != '/') {
        return false;
    }
    if (count == 1) {
        return folder;
    }
    for (size_t at = 1; at <= count; at += 1) {
        if (at == count || path[at] == '/') {
            size_t length = at - start;

            if (length == 0) {
                /* An empty segment is a folder's end alone. */
                return folder && at == count;
            }
            if ((length == 1 && path[start] == '.') || (length == 2 && path[start] == '.' && path[start + 1] == '.')) {
                return false;
            }
            start = at + 1;
            continue;
        }
        if (!hero_ui_web_path_byte(path[at])) {
            return false;
        }
    }
    return true;
}

/* Whether `c` belongs to a token of a content type (RFC 9110 § 5.6.2). */
static bool hero_ui_web_token_byte(uint8_t c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c != 0 && strchr("!#$%&'*+-.^_`|~", c) != NULL);
}

/* Whether the `count` bytes at `type` are a content type: a token, `/`, a
 * token, then parameters of printable ASCII. */
static bool hero_ui_web_type_ok(const uint8_t *type, size_t count) {
    size_t at = 0;
    size_t slash;

    if (count < 3 || count > HERO_UI_WEB_TYPE_MOST) {
        return false;
    }
    while (at < count && hero_ui_web_token_byte(type[at])) {
        at += 1;
    }
    if (at == 0 || at == count || type[at] != '/') {
        return false;
    }
    slash = at;
    at += 1;
    while (at < count && hero_ui_web_token_byte(type[at])) {
        at += 1;
    }
    if (at == slash + 1) {
        return false;
    }
    for (; at < count; at += 1) {
        if (type[at] < 0x20 || type[at] > 0x7E) {
            return false;
        }
    }
    return true;
}

/* Whether the `count` bytes at `text` are printable ASCII, a header's value
 * that no line feed can end. */
static bool hero_ui_web_printable(const uint8_t *text, size_t count) {
    for (size_t at = 0; at < count; at += 1) {
        if (text[at] < 0x20 || text[at] > 0x7E) {
            return false;
        }
    }
    return true;
}

/* `c` in lower case, for the ASCII letters. */
static uint8_t hero_ui_web_lower(uint8_t c) {
    return (c >= 'A' && c <= 'Z') ? (uint8_t)(c - 'A' + 'a') : c;
}

/* Whether the `count` bytes at `text` begin with `head`, its letters in any
 * case. */
static bool hero_ui_web_begins(const uint8_t *text, size_t count, const char *head) {
    size_t length = strlen(head);

    if (count < length) {
        return false;
    }
    for (size_t at = 0; at < length; at += 1) {
        if (hero_ui_web_lower(text[at]) != (uint8_t)head[at]) {
            return false;
        }
    }
    return true;
}

/* The origin of the URL `url`, written into `into` as `scheme://host` and
 * `:port` where it is not the scheme's own, lower case, with its zero; true
 * where `url` is an `http`, an `https` or the app's own scheme's URL with a
 * host of lower-case letters, digits, `-` and `.` and a port from 1 to
 * 65535, followed by its end, `/`, `?` or `#`. `port_written` says whether
 * the scheme's own port was written out. */
static bool hero_ui_web_origin_of(const uint8_t *url, size_t count, char *into, size_t room, bool *port_written) {
    const char *scheme;
    size_t at;
    size_t host_start;
    size_t host_end;
    uint32_t port = 0;
    uint32_t own;
    int written;

    if (hero_ui_web_begins(url, count, "https://")) {
        scheme = "https";
        own = 443;
    } else if (hero_ui_web_begins(url, count, "http://")) {
        scheme = "http";
        own = 80;
    } else if (hero_ui_web_begins(url, count, HERO_UI_WEB_SCHEME "://")) {
        scheme = HERO_UI_WEB_SCHEME;
        own = 0;
    } else {
        return false;
    }
    at = strlen(scheme) + 3;
    host_start = at;
    while (at < count && url[at] != ':' && url[at] != '/' && url[at] != '?' && url[at] != '#') {
        uint8_t c = hero_ui_web_lower(url[at]);

        if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-' || c == '.')) {
            return false;
        }
        at += 1;
    }
    host_end = at;
    if (host_end == host_start || url[host_start] == '.' || url[host_end - 1] == '.' || host_end - host_start > 253) {
        return false;
    }
    *port_written = false;
    if (at < count && url[at] == ':') {
        size_t digits = 0;

        at += 1;
        while (at < count && url[at] >= '0' && url[at] <= '9') {
            if (digits == 5) {
                return false;
            }
            port = port * 10u + (uint32_t)(url[at] - '0');
            digits += 1;
            at += 1;
        }
        if (digits == 0 || port == 0 || port > 65535u || (digits > 1 && url[at - digits] == '0')) {
            return false;
        }
        if (port == own) {
            *port_written = true;
            port = 0;
        }
    }
    if (at < count && url[at] != '/' && url[at] != '?' && url[at] != '#') {
        return false;
    }
    written = snprintf(into, room, "%s://", scheme);
    if (written < 0 || (size_t)written + (host_end - host_start) + 7 > room) {
        return false;
    }
    for (size_t i = host_start; i < host_end; i += 1) {
        into[written] = (char)hero_ui_web_lower(url[i]);
        written += 1;
    }
    into[written] = '\0';
    if (port != 0) {
        (void)snprintf(into + written, room - (size_t)written, ":%u", (unsigned)port);
    }
    return true;
}

/* Whether the `count` bytes at `url` are a URL of the app's own origin. */
static bool hero_ui_web_is_ours(const uint8_t *url, size_t count) {
    char origin[HERO_UI_WEB_ORIGIN_MOST + 1];
    bool port_written = false;

    return hero_ui_web_origin_of(url, count, origin, sizeof origin, &port_written) && !port_written && strcmp(origin, HERO_UI_WEB_SCHEME "://" HERO_UI_WEB_HOST) == 0;
}

/* Whether the `count` bytes at `url` are `about:blank` or `about:srcdoc`,
 * perhaps with a query or a fragment: a frame's empty page, which inherits
 * the origin of the page that made it. */
static bool hero_ui_web_is_page(const uint8_t *url, size_t count, const char *page) {
    size_t length = strlen(page);

    return hero_ui_web_begins(url, count, page) && (count == length || url[length] == '?' || url[length] == '#');
}

static bool hero_ui_web_is_empty_page(const uint8_t *url, size_t count) {
    return hero_ui_web_is_page(url, count, "about:blank") || hero_ui_web_is_page(url, count, "about:srcdoc");
}

static int64_t hero_ui_web_ask(int64_t what, int64_t id, const uint8_t *url, int64_t count) {
    hero_ui_web *web = hero_ui_web_of(id);
    char origin[HERO_UI_WEB_ORIGIN_MOST + 1];
    bool port_written = false;

    if (web == NULL || count < 0 || (url == NULL && count > 0)) {
        return 0;
    }
    if (what == HERO_UI_WEB_ASK_SENDER) {
        if (hero_ui_web_is_ours(url, (size_t)count)) {
            return 1;
        }
        if (hero_ui_native_slot != 0) {
            hero_ui_slots[hero_ui_native_slot - 1].lost += 1;
        }
        return 0;
    }
    if (what == HERO_UI_WEB_ASK_OURS) {
        return hero_ui_web_is_ours(url, (size_t)count) ? 1 : 0;
    }
    if (what != HERO_UI_WEB_ASK_FRAME && what != HERO_UI_WEB_ASK_SUBFRAME) {
        return 0;
    }
    if (hero_ui_web_is_ours(url, (size_t)count) || hero_ui_web_is_empty_page(url, (size_t)count)) {
        return 1;
    }
    if (!hero_ui_web_origin_of(url, (size_t)count, origin, sizeof origin, &port_written)) {
        return 0;
    }
    for (int64_t i = 0; i < web->origin_count; i += 1) {
        if (strcmp(web->origins[i], origin) == 0) {
            return 1;
        }
    }
    return 0;
}

/* The value of the hexadecimal digit `c`, or -1. */
static int hero_ui_web_hex(uint8_t c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    c = hero_ui_web_lower(c);
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    return -1;
}

/* Whether the `count` bytes at `segment` are the WHATWG URL standard's
 * single-dot segment, `.` or `%2e` in any case, or, where `two`, its
 * double-dot one, `..`, `.%2e`, `%2e.` or `%2e%2e`. */
static bool hero_ui_web_dots(const uint8_t *segment, size_t count, bool two) {
    size_t dots = 0;
    size_t at = 0;

    while (at < count) {
        if (segment[at] == '.') {
            at += 1;
        } else if (count - at >= 3 && segment[at] == '%' && segment[at + 1] == '2' && hero_ui_web_lower(segment[at + 2]) == 'e') {
            at += 3;
        } else {
            return false;
        }
        dots += 1;
    }
    return dots == (two ? 2u : 1u);
}

/* The `count` bytes at `path`, a URL's path that begins with `/`, with its
 * dot segments resolved as the WHATWG URL standard's path state resolves
 * them, every web view's own parser having done so before it asks: a `.`
 * dropped, a `..` dropping the segment before it and none above the root,
 * and a path that ends in either ending in `/`; every other segment kept,
 * an empty one too. Written into `into` (`room` bytes, at least `count` +
 * 2) with its zero; answers its length. Read 2026-11-22 from the
 * standard's "path state"; measured the same day on WebKit, whose request
 * for `/%2e%2e/data.json` reached the scheme's handler as `/data.json`. */
static size_t hero_ui_web_dots_resolved(const uint8_t *path, size_t count, char *into, size_t room) {
    size_t starts[HERO_UI_WEB_PATH_MOST + 1];
    size_t kept = 0;
    size_t written = 0;
    size_t start = 1;
    bool ends_open = false;

    for (size_t at = 1; at <= count && kept <= HERO_UI_WEB_PATH_MOST; at += 1) {
        if (at < count && path[at] != '/') {
            continue;
        }
        if (hero_ui_web_dots(path + start, at - start, true)) {
            if (kept > 0) {
                kept -= 1;
                written = starts[kept];
            }
            ends_open = at == count;
        } else if (hero_ui_web_dots(path + start, at - start, false)) {
            ends_open = at == count;
        } else if (written + 1 + (at - start) + 2 <= room) {
            starts[kept] = written;
            kept += 1;
            into[written] = '/';
            memcpy(into + written + 1, path + start, at - start);
            written += 1 + (at - start);
            ends_open = false;
        }
        start = at + 1;
    }
    if (kept == 0) {
        into[0] = '/';
        written = 1;
    } else if (ends_open) {
        into[written] = '/';
        written += 1;
    }
    into[written] = '\0';
    return written;
}

/* The asset path a request of the app's scheme asks, written into `path`
 * (`room` bytes, at least `HERO_UI_WEB_PATH_MOST` + 11) with its zero, its
 * dot segments resolved, its escapes undone and a folder's `index.html`
 * added; false where the URL is not of the app's own origin or its path is
 * none an asset could have. */
static bool hero_ui_web_asked_path(const uint8_t *url, size_t count, char *path, size_t room, size_t *length) {
    size_t from = strlen(HERO_UI_WEB_SCHEME "://" HERO_UI_WEB_HOST);
    size_t end = from;
    uint8_t raw[HERO_UI_WEB_PATH_MOST + 1];
    char resolved[HERO_UI_WEB_PATH_MOST + 2];
    size_t raw_count = 0;
    size_t resolved_count;
    size_t written = 0;

    if (!hero_ui_web_is_ours(url, count)) {
        return false;
    }
    while (end < count && url[end] != '?' && url[end] != '#') {
        end += 1;
    }
    if (end == from || url[from] != '/') {
        raw[raw_count] = '/';
        raw_count += 1;
    }
    if (end - from > HERO_UI_WEB_PATH_MOST - raw_count) {
        return false;
    }
    memcpy(raw + raw_count, url + from, end - from);
    raw_count += end - from;
    resolved_count = hero_ui_web_dots_resolved(raw, raw_count, resolved, sizeof resolved);
    for (size_t at = 0; at < resolved_count; at += 1) {
        uint8_t c = (uint8_t)resolved[at];

        if (c == '%') {
            int high = at + 2 < resolved_count ? hero_ui_web_hex((uint8_t)resolved[at + 1]) : -1;
            int low = at + 2 < resolved_count ? hero_ui_web_hex((uint8_t)resolved[at + 2]) : -1;

            if (high < 0 || low < 0) {
                return false;
            }
            c = (uint8_t)(high * 16 + low);
            at += 2;
        }
        if (written == HERO_UI_WEB_PATH_MOST || c == 0) {
            return false;
        }
        path[written] = (char)c;
        written += 1;
    }
    if (written > 0 && path[written - 1] == '/') {
        if (written + 10 + 1 > room) {
            return false;
        }
        memcpy(path + written, "index.html", 10);
        written += 10;
    }
    path[written] = '\0';
    *length = written;
    return hero_ui_web_path_ok((const uint8_t *)path, written, false);
}

/* The index of the asset at `path` in `web`'s table, or -1. */
static int64_t hero_ui_web_find(const hero_ui_web *web, const char *path, size_t count) {
    for (int64_t i = 0; i < web->held; i += 1) {
        if (web->assets[i].path_count == count && memcmp(web->assets[i].path, path, count) == 0) {
            return i;
        }
    }
    return -1;
}

static int64_t hero_ui_web_serve(int64_t id, const uint8_t *method, int64_t method_count, const uint8_t *url, int64_t url_count, int64_t *numbers, const uint8_t **texts) {
    static const char plain[] = "text/plain; charset=utf-8";
    static const char missing[] = "not found\n";
    static const char refused[] = "method not allowed\n";
    hero_ui_web *web = hero_ui_web_of(id);
    char path[HERO_UI_WEB_PATH_MOST + 11];
    size_t length = 0;
    bool head = method_count == 4 && memcmp(method, "HEAD", 4) == 0;
    bool get = method_count == 3 && memcmp(method, "GET", 3) == 0;
    int64_t found = -1;

    if (numbers == NULL || texts == NULL) {
        return HERO_UI_INVALID;
    }
    if (web != NULL && web->policy != NULL) {
        texts[HERO_UI_WEB_POLICY] = (const uint8_t *)web->policy;
        numbers[HERO_UI_WEB_POLICY_COUNT] = (int64_t)web->policy_count;
    } else {
        texts[HERO_UI_WEB_POLICY] = (const uint8_t *)HERO_UI_WEB_POLICY_DEFAULT;
        numbers[HERO_UI_WEB_POLICY_COUNT] = (int64_t)strlen(HERO_UI_WEB_POLICY_DEFAULT);
    }
    texts[HERO_UI_WEB_TYPE] = (const uint8_t *)plain;
    numbers[HERO_UI_WEB_TYPE_COUNT] = (int64_t)(sizeof plain - 1);
    if (!head && !get) {
        numbers[HERO_UI_WEB_STATUS] = 405;
        texts[HERO_UI_WEB_BODY] = (const uint8_t *)refused;
        numbers[HERO_UI_WEB_LENGTH] = (int64_t)(sizeof refused - 1);
        numbers[HERO_UI_WEB_BODY_COUNT] = numbers[HERO_UI_WEB_LENGTH];
        return HERO_UI_OK;
    }
    if (web != NULL && url != NULL && url_count > 0 && hero_ui_web_asked_path(url, (size_t)url_count, path, sizeof path, &length)) {
        found = hero_ui_web_find(web, path, length);
    }
    if (found < 0) {
        numbers[HERO_UI_WEB_STATUS] = 404;
        texts[HERO_UI_WEB_BODY] = (const uint8_t *)missing;
        numbers[HERO_UI_WEB_LENGTH] = (int64_t)(sizeof missing - 1);
    } else {
        const hero_ui_asset *asset = &web->assets[found];

        numbers[HERO_UI_WEB_STATUS] = 200;
        texts[HERO_UI_WEB_TYPE] = (const uint8_t *)asset->type;
        numbers[HERO_UI_WEB_TYPE_COUNT] = (int64_t)asset->type_count;
        texts[HERO_UI_WEB_BODY] = asset->body;
        numbers[HERO_UI_WEB_LENGTH] = (int64_t)asset->body_count;
    }
    numbers[HERO_UI_WEB_BODY_COUNT] = head ? 0 : numbers[HERO_UI_WEB_LENGTH];
    return HERO_UI_OK;
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
        hero_ui_web_give_back_all();
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
    if (kind == HERO_UI_WEBVIEW) {
        return hero_ui_web_create(id, parent);
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
    if (kind < HERO_UI_EVENT_CLICK || kind > HERO_UI_EVENT_MESSAGE) {
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

/* -- THE WEB VIEW'S ENTRY POINTS ------------------------------------------- */

/* The record of web view `id` of `app`, on the UI thread; NULL with the
 * failure said. */
static hero_ui_web *hero_ui_web_enter(hero_ui_app *app, int64_t id) {
    hero_ui_web *web;
    char words[HERO_UI_WORDS_MOST];

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return NULL;
    }
    web = hero_ui_web_of(id);
    if (web == NULL) {
        (void)snprintf(words, sizeof words, "the native side holds no web view %lld", (long long)id);
        (void)hero_ui_said(HERO_UI_NOT_FOUND, words);
    }
    return web;
}

/* Whether `count` bytes at `text` are given, as `hero_ui_text_given` asks
 * of a pointer and its count alone. */
static bool hero_ui_web_bytes_given(const void *text, uint64_t count, const char *what) {
    char words[HERO_UI_WORDS_MOST];

    if (count > 0 && text == NULL) {
        (void)snprintf(words, sizeof words, "%s: %llu bytes were named and none given", what, (unsigned long long)count);
        (void)hero_ui_said(HERO_UI_INVALID, words);
        return false;
    }
    return true;
}

/* Whether `count` bytes at `path` are an asset's path, else the failure
 * said. */
static bool hero_ui_web_path_given(const void *path, uint64_t count) {
    char words[HERO_UI_WORDS_MOST];

    if (!hero_ui_web_bytes_given(path, count, "the path")) {
        return false;
    }
    if (count == 0 || !hero_ui_web_path_ok(path, (size_t)count, false)) {
        (void)snprintf(words, sizeof words, "the path of %llu bytes is no asset's: `/` and segments of A-Z, a-z, 0-9, `-`, `.`, `_` and `~` joined by `/`, never `.` or `..`, at most %d bytes", (unsigned long long)count, HERO_UI_WEB_PATH_MOST);
        (void)hero_ui_said(HERO_UI_INVALID, words);
        return false;
    }
    return true;
}

int64_t hero_ui_web_asset(hero_ui_app *app, int64_t id, const void *path, uint64_t path_count, const void *type, uint64_t type_count, const void *body, uint64_t body_count) {
    hero_ui_web *web = hero_ui_web_enter(app, id);
    hero_ui_asset made;
    int64_t at;
    uint64_t before = 0;
    char words[HERO_UI_WORDS_MOST];

    if (web == NULL || !hero_ui_web_path_given(path, path_count) || !hero_ui_web_bytes_given(type, type_count, "the content type") || !hero_ui_web_bytes_given(body, body_count, "the body")) {
        return -1;
    }
    if (!hero_ui_web_type_ok(type, (size_t)type_count)) {
        return hero_ui_said(HERO_UI_INVALID, "the content type is no `type/subtype`, then parameters, of printable ASCII, at most 255 bytes");
    }
    if (body_count > (uint64_t)HERO_UI_WEB_ASSET_MOST) {
        (void)snprintf(words, sizeof words, "a body of %llu bytes, past the %d an asset holds", (unsigned long long)body_count, HERO_UI_WEB_ASSET_MOST);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    at = hero_ui_web_find(web, path, (size_t)path_count);
    if (at >= 0) {
        before = web->assets[at].body_count;
    } else if (web->held == HERO_UI_WEB_ASSETS_MOST) {
        (void)snprintf(words, sizeof words, "web view %lld holds %d assets already, as many as its table takes", (long long)id, HERO_UI_WEB_ASSETS_MOST);
        return hero_ui_said(HERO_UI_LIMIT, words);
    }
    if (web->bytes - before + body_count > (uint64_t)HERO_UI_WEB_BYTES_MOST) {
        (void)snprintf(words, sizeof words, "web view %lld's assets would take %llu bytes, past the %d its table takes", (long long)id, (unsigned long long)(web->bytes - before + body_count), HERO_UI_WEB_BYTES_MOST);
        return hero_ui_said(HERO_UI_LIMIT, words);
    }
    if (at < 0 && web->held == web->room) {
        int64_t room = web->room == 0 ? 16 : web->room * 2;
        hero_ui_asset *grown = hero_layer_grow(web->assets, (size_t)room * sizeof *grown);

        if (grown == NULL) {
            return hero_ui_said(HERO_UI_LIMIT, "the system has no memory left for the asset table");
        }
        web->assets = grown;
        web->room = room;
    }
    made.path = hero_layer_alloc((size_t)path_count);
    made.type = hero_layer_alloc((size_t)type_count);
    made.body = hero_layer_alloc((size_t)body_count);
    if (made.path == NULL || made.type == NULL || made.body == NULL) {
        hero_ui_web_free_asset(&made);
        return hero_ui_said(HERO_UI_LIMIT, "the system has no memory left for the asset");
    }
    memcpy(made.path, path, (size_t)path_count);
    made.path_count = (size_t)path_count;
    memcpy(made.type, type, (size_t)type_count);
    made.type_count = (size_t)type_count;
    if (body_count > 0) {
        memcpy(made.body, body, (size_t)body_count);
    }
    made.body_count = (size_t)body_count;
    if (at >= 0) {
        hero_ui_web_free_asset(&web->assets[at]);
        web->assets[at] = made;
    } else {
        web->assets[web->held] = made;
        web->held += 1;
    }
    web->bytes = web->bytes - before + body_count;
    return 0;
}

int64_t hero_ui_web_remove(hero_ui_app *app, int64_t id, const void *path, uint64_t path_count) {
    hero_ui_web *web = hero_ui_web_enter(app, id);
    int64_t at;
    char words[HERO_UI_WORDS_MOST];

    if (web == NULL || !hero_ui_web_path_given(path, path_count)) {
        return -1;
    }
    at = hero_ui_web_find(web, path, (size_t)path_count);
    if (at < 0) {
        (void)snprintf(words, sizeof words, "web view %lld holds no asset at that path", (long long)id);
        return hero_ui_said(HERO_UI_NOT_FOUND, words);
    }
    web->bytes -= web->assets[at].body_count;
    hero_ui_web_free_asset(&web->assets[at]);
    web->assets[at] = web->assets[web->held - 1];
    web->held -= 1;
    return 0;
}

int64_t hero_ui_web_policy(hero_ui_app *app, int64_t id, const void *policy, uint64_t count) {
    hero_ui_web *web = hero_ui_web_enter(app, id);
    char *copy = NULL;

    if (web == NULL || !hero_ui_web_bytes_given(policy, count, "the policy")) {
        return -1;
    }
    if (count > (uint64_t)HERO_UI_WEB_POLICY_MOST || !hero_ui_web_printable(policy, (size_t)count)) {
        return hero_ui_said(HERO_UI_INVALID, "a policy is printable ASCII, at most 4096 bytes");
    }
    if (count > 0) {
        copy = hero_layer_alloc((size_t)count);
        if (copy == NULL) {
            return hero_ui_said(HERO_UI_LIMIT, "the system has no memory left for the policy");
        }
        memcpy(copy, policy, (size_t)count);
    }
    hero_layer_free(web->policy);
    web->policy = copy;
    web->policy_count = (size_t)count;
    return 0;
}

int64_t hero_ui_web_allow(hero_ui_app *app, int64_t id, const void *origin, uint64_t count) {
    hero_ui_web *web = hero_ui_web_enter(app, id);
    char normal[HERO_UI_WEB_ORIGIN_MOST + 1];
    bool port_written = false;
    bool read;
    char words[HERO_UI_WORDS_MOST];

    if (web == NULL || !hero_ui_web_bytes_given(origin, count, "the origin")) {
        return -1;
    }
    read = count > 0 && count <= (uint64_t)HERO_UI_WEB_ORIGIN_MOST && hero_ui_web_origin_of(origin, (size_t)count, normal, sizeof normal, &port_written);
    if (read && port_written) {
        return hero_ui_said(HERO_UI_INVALID, "the origin names its scheme's own port; it is written without it");
    }
    if (!read || strncmp(normal, "http", 4) != 0 || strlen(normal) != (size_t)count) {
        return hero_ui_said(HERO_UI_INVALID, "an origin is `https://` or `http://`, a host of a-z, 0-9, `-` and `.`, and a port where it is not the scheme's own, with no path, at most 255 bytes");
    }
    for (int64_t i = 0; i < web->origin_count; i += 1) {
        if (strcmp(web->origins[i], normal) == 0) {
            return 0;
        }
    }
    if (web->origin_count == HERO_UI_WEB_ORIGINS_MOST) {
        (void)snprintf(words, sizeof words, "web view %lld allows %d origins already, as many as it takes", (long long)id, HERO_UI_WEB_ORIGINS_MOST);
        return hero_ui_said(HERO_UI_LIMIT, words);
    }
    memcpy(web->origins[web->origin_count], normal, strlen(normal) + 1);
    web->origin_count += 1;
    return 0;
}

int64_t hero_ui_web_navigate(hero_ui_app *app, int64_t id, const void *path, uint64_t count) {
    static const char origin[] = HERO_UI_WEB_SCHEME "://" HERO_UI_WEB_HOST;
    hero_ui_web *web = hero_ui_web_enter(app, id);
    const uint8_t *bytes = path;
    uint8_t url[sizeof origin - 1 + HERO_UI_WEB_PATH_MOST];
    size_t end = 0;

    if (web == NULL || !hero_ui_web_bytes_given(path, count, "the path")) {
        return -1;
    }
    while (end < count && bytes[end] != '?' && bytes[end] != '#') {
        end += 1;
    }
    for (size_t at = end; at < count; at += 1) {
        uint8_t c = bytes[at];

        if (!hero_ui_web_path_byte(c) && (c == 0 || strchr("!$&'()*+,;=:@/?%#", c) == NULL)) {
            end = 0;
            break;
        }
    }
    if (count > (uint64_t)HERO_UI_WEB_PATH_MOST || end == 0 || !hero_ui_web_path_ok(bytes, end, true)) {
        return hero_ui_said(HERO_UI_INVALID, "a page's path is an asset's or a folder's ending in `/`, then perhaps a query and a fragment, at most 1024 bytes");
    }
    memcpy(url, origin, sizeof origin - 1);
    memcpy(url + sizeof origin - 1, bytes, (size_t)count);
    return hero_ui_answered(hero_ui_arm_web_load(id, url, (int64_t)(sizeof origin - 1 + count)), "showing a page", id);
}

int64_t hero_ui_web_post(hero_ui_app *app, int64_t id, const void *text, uint64_t count) {
    if (hero_ui_web_enter(app, id) == NULL || !hero_ui_text_given(text, count, "the text posted", false)) {
        return -1;
    }
    return hero_ui_answered(hero_ui_arm_web_post(id, text, (int64_t)count), "posting a text to the page", id);
}
