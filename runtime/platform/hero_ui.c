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
 * `hero_ui_arm_*` (THE ARM, `hero_ui_arm.h`), which this file calls only on the UI
 * thread, for the one native app, with every argument already checked, and
 * which answer a kind, `HERO_UI_OK` where done. The macOS arm calls the
 * functions `platform/hero_ui_appkit.swift` exports to C (AppKit is
 * Swift's and Objective-C's, never C's) and queues what the person does by
 * the function this file hands it at `open`, `hero_ui_push`. The Windows arm
 * (Win32's own controls) and the arm of every other system (GTK 4, opened
 * at the first call with `dlopen`) are C, each a member of its own,
 * `hero_ui_win32.c` and `hero_ui_gtk.c`, compiled to nothing elsewhere, and
 * queue through `hero_ui_tell`, which this file keeps for both with the
 * table of their widgets (WHAT THE WIN32 AND GTK ARMS SHARE).
 *
 * THE THREAD. Every entry point but `on_ui_thread` and `native_here` asks
 * first whether it runs on the UI thread, before it reads the table, so a
 * call from another thread touches nothing and answers
 * `HERO_UI_WRONG_THREAD`. On macOS the UI thread is the main thread
 * (`pthread_main_np`), on Linux the thread whose id is the process's
 * (`gettid() == getpid()`), on Windows the thread a constructor of this
 * member noted before `main` (Windows names no main thread, and its C
 * library runs a program's constructors on the thread that then runs
 * `main`). So the table, the queues and the last
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
#include "hero_ui_arm.h"
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
#include <dispatch/dispatch.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "hero_ui_appkit.h"
#include "hero_ui_appkit_system.h"
#else
#include <dlfcn.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>
#include "hero_ui_web_gtk.h"
#endif

/* -- THE LAST FAILURE, PER THREAD ------------------------------------------ */

static _Thread_local int64_t hero_ui_kind;
static _Thread_local char hero_ui_words[HERO_UI_WORDS_MOST];

/* `text` copied into `into`, `most` bytes with the zero, cut where a
 * character ends; answers the length copied. */
size_t hero_ui_copy_text(char *into, size_t most, const char *text) {
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

static void hero_ui_push(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count);
#if defined(__APPLE__)
static void hero_ui_dialog_told(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count);
#endif
static int64_t hero_ui_web_serve(int64_t id, const uint8_t *method, int64_t method_count, const uint8_t *url, int64_t url_count, int64_t *numbers, const uint8_t **texts);
static int64_t hero_ui_web_ask(int64_t what, int64_t id, const uint8_t *url, int64_t count);
static void hero_ui_web_gone(int64_t id);
static const char *hero_ui_web_bridge(int64_t *count);

#if defined(__APPLE__)

int64_t hero_ui_arm_here(void) {
    return hero_ui_appkit_here();
}

/* Why `here` answered 0, into `words` of `count` bytes. */
void hero_ui_arm_absent(char *words, size_t count) {
    (void)hero_ui_copy_text(words, count, "no window server serves this session, as over a remote shell");
}

int64_t hero_ui_arm_open(void) {
    int64_t count = 0;
    const char *bridge = hero_ui_web_bridge(&count);

    int64_t kind;

    /* The application delegate stands before AppKit's launch, which hands
     * it the documents the system opened the app with. */
    hero_ui_appkit_session_watch(hero_ui_dialog_told);
    kind = hero_ui_appkit_open(hero_ui_push, hero_ui_web_serve, hero_ui_web_ask, hero_ui_web_gone, (const uint8_t *)bridge, count);
    if (kind == HERO_UI_OK) {
        hero_ui_appkit_quit_watch();
        hero_ui_appkit_appearance_watch(hero_ui_dialog_told);
    } else {
        hero_ui_appkit_session_watch(NULL);
    }
    return kind;
}

void hero_ui_arm_close(void) {
    hero_ui_appkit_session_watch(NULL);
    hero_ui_appkit_appearance_watch(NULL);
    hero_ui_appkit_close();
}

int64_t hero_ui_arm_words(char *words, int64_t count) {
    return hero_ui_appkit_words(words, count);
}

int64_t hero_ui_arm_create(int64_t id, int64_t kind, int64_t parent) {
    return hero_ui_appkit_create(id, kind, parent);
}

int64_t hero_ui_arm_destroy(int64_t id) {
    return hero_ui_appkit_destroy(id);
}

int64_t hero_ui_arm_set_text(int64_t id, const uint8_t *text, int64_t count) {
    return hero_ui_appkit_set_text(id, text, count);
}

int64_t hero_ui_arm_set_items(int64_t id, const uint8_t *items, int64_t count) {
    return hero_ui_appkit_set_items(id, items, count);
}

int64_t hero_ui_arm_set_value(int64_t id, int64_t value) {
    return hero_ui_appkit_set_value(id, value);
}

int64_t hero_ui_arm_set_range(int64_t id, int64_t least, int64_t most) {
    return hero_ui_appkit_set_range(id, least, most);
}

int64_t hero_ui_arm_set_selection(int64_t id, const int64_t *indices, int64_t count) {
    return hero_ui_appkit_set_selection(id, indices, count);
}

int64_t hero_ui_arm_set_flag(int64_t id, int64_t which, int64_t on) {
    return hero_ui_appkit_set_flag(id, which, on);
}

int64_t hero_ui_arm_set_shortcut(int64_t id, const uint8_t *key, int64_t count, int64_t modifiers) {
    return hero_ui_appkit_set_shortcut(id, key, count, modifiers);
}

int64_t hero_ui_arm_set_image(int64_t id, const uint8_t *rgba, int64_t width, int64_t height) {
    return hero_ui_appkit_set_image(id, rgba, width, height);
}

int64_t hero_ui_arm_set_size(int64_t id, int64_t which, int64_t width, int64_t height) {
    return hero_ui_appkit_set_size(id, which, width, height);
}

int64_t hero_ui_arm_place(int64_t id, int64_t index, int64_t x, int64_t y, int64_t width, int64_t height) {
    return hero_ui_appkit_place(id, index, x, y, width, height);
}

int64_t hero_ui_arm_preferred(int64_t id, int64_t *width, int64_t *height) {
    return hero_ui_appkit_preferred(id, width, height);
}

int64_t hero_ui_arm_text(int64_t id, uint8_t *buffer, int64_t count, int64_t *length) {
    return hero_ui_appkit_text(id, buffer, count, length);
}

int64_t hero_ui_arm_value(int64_t id, int64_t *value) {
    return hero_ui_appkit_value(id, value);
}

int64_t hero_ui_arm_selection(int64_t id, int64_t *indices, int64_t count, int64_t *chosen) {
    return hero_ui_appkit_selection(id, indices, count, chosen);
}

int64_t hero_ui_arm_pump(int64_t ms) {
    return hero_ui_appkit_pump(ms);
}

int64_t hero_ui_arm_inject(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count) {
    return hero_ui_appkit_inject(kind, id, value, second, text, count);
}

int64_t hero_ui_arm_web_load(int64_t id, const uint8_t *url, int64_t count) {
    return hero_ui_appkit_web_load(id, url, count);
}

int64_t hero_ui_arm_web_post(int64_t id, const uint8_t *text, int64_t count) {
    return hero_ui_appkit_web_post(id, text, count);
}

#else

/* -- WHAT THE WIN32 AND GTK ARMS SHARE ----------------------------------------
 *
 * Both arms are C, `hero_ui_win32.c` and `hero_ui_gtk.c`, so they queue
 * what the person does through `hero_ui_tell`, and keep their widgets in one
 * table, a map from the program's numbers to the arm's record of each
 * widget, which this file holds for whichever of the two is compiled and
 * `hero_ui_arm.h` declares. Every object below is the UI thread's own,
 * `_Thread_local`, as the table of apps is (THE THREAD). */

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
int64_t hero_ui_refused(int64_t kind, const char *words) {
    (void)hero_ui_copy_text(hero_ui_native_words, sizeof hero_ui_native_words, words);
    return kind;
}

int64_t hero_ui_arm_words(char *words, int64_t count) {
    if (words == NULL || count <= 0) {
        return 0;
    }
    return (int64_t)hero_ui_copy_text(words, (size_t)count, hero_ui_native_words);
}

/* Queues what the person did, unless the program is setting it. */
void hero_ui_tell(int64_t kind, int64_t id, int64_t value, int64_t second) {
    if (hero_ui_muted > 0) {
        return;
    }
    hero_ui_pushed += 1;
    hero_ui_push(kind, id, value, second, NULL, 0);
}

/* What the Win32 and GTK 4 arms queue an event that carries a text by (a
 * dialog's paths, a drop's): `hero_ui_push`, counted as the arm's own
 * events are, so `pump` returns at it; never muted, since no widget the
 * program sets carries one. */
void hero_ui_tell_text(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count) {
    hero_ui_pushed += 1;
    hero_ui_push(kind, id, value, second, text, count);
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

void hero_ui_make_host(void) {
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
hero_ui_native *hero_ui_find(int64_t id) {
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
bool hero_ui_keep(int64_t id, hero_ui_native *widget) {
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
void hero_ui_forget(int64_t id) {
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
hero_ui_native *hero_ui_at(size_t at) {
    return at < hero_ui_room ? hero_ui_entries[at].widget : NULL;
}

/* Gives back the map, every widget forgotten first. */
void hero_ui_free_map(void) {
    hero_layer_free(hero_ui_entries);
    hero_ui_entries = NULL;
    hero_ui_room = 0;
    hero_ui_filled = 0;
}

/* Adds `id` at the end; false where the system had no memory for it. */
bool hero_ui_ids_add(hero_ui_ids *ids, int64_t id) {
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
void hero_ui_ids_drop(hero_ui_ids *ids, int64_t id) {
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

void hero_ui_ids_free(hero_ui_ids *ids) {
    hero_layer_free(ids->at);
    ids->at = NULL;
    ids->count = 0;
    ids->room = 0;
}

/* How many texts `count` bytes of a list hold, each followed by one zero
 * byte (`hero_ui.c` checked the last byte is one). */
int64_t hero_ui_texts_in(const uint8_t *items, int64_t count) {
    int64_t texts = 0;

    for (int64_t at = 0; at < count; at += 1) {
        texts += items[at] == 0 ? 1 : 0;
    }
    return texts;
}

/* The `index`th text of a list as its first byte and its length. */
const uint8_t *hero_ui_text_at(const uint8_t *items, int64_t count, int64_t index, int64_t *length) {
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
int64_t hero_ui_between(int64_t value, int64_t least, int64_t most) {
    return value < least ? least : (value > most ? most : value);
}

/* Where `value` stands between `least` and `most`, from 0 to 1; the range is
 * checked `least < most` where it was set. */
double hero_ui_fraction(int64_t value, int64_t least, int64_t most) {
    double span = (double)most - (double)least;

    return span <= 0 ? 0 : ((double)value - (double)least) / span;
}

/* The value standing at `fraction` of the way from `least` to `most`,
 * rounded, and held inside them. */
int64_t hero_ui_at_fraction(double fraction, int64_t least, int64_t most) {
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

/* -- THE SHARED STATE, AS THE ARMS REACH IT ---------------------------------
 *
 * The state above is this file's own, `static`, so every object kept between
 * calls stands where the runtime suite's rule on shared state reads it; each
 * arm, a member of its own, reaches it through these. */

/* Moves the count of the program's settings in course by `by`: 1 as it
 * starts setting a widget, -1 as it ends. */
void hero_ui_mute(int64_t by) {
    hero_ui_muted += by;
}

/* No setting in course, as an app opens. */
void hero_ui_unmute(void) {
    hero_ui_muted = 0;
}

/* Whether the program is setting a widget, so what the system tells of it
 * queues nothing. */
bool hero_ui_is_muted(void) {
    return hero_ui_muted > 0;
}

/* How many events the arm has queued, which `pump` reads before and after. */
uint64_t hero_ui_pushed_count(void) {
    return hero_ui_pushed;
}

/* The places of the table of widgets, each read by `hero_ui_at`. */
size_t hero_ui_map_room(void) {
    return hero_ui_room;
}

/* The host `hero_ui_make_host` filled, which the arm hands each web view it
 * makes. */
const hero_ui_web_host *hero_ui_web_host_of(void) {
    return &hero_ui_web_hosted;
}

#endif

/* -- THE THREAD ------------------------------------------------------------ */

#if defined(_WIN32)
/* The thread `main` runs on, by the runtime's number for it
 * (`hero_layer_thread.h`), noted before `main` by the constructor below; 0
 * where no constructor of this member ran (the layer in a library loaded
 * after `main` began), and then the thread that opens the process's first
 * app. A thread's id is not used: Windows gives a thread started after the
 * owner ended the same id, which was then taken for the UI thread (defect
 * 2673's shape). */
static _Atomic uint64_t hero_ui_owner;

/* Notes the thread `main` will run on: Windows' C library runs a program's
 * constructors (`.CRT$XCU`) on its main thread, before `main`, so an app
 * opened first by another thread is refused there as on macOS and Linux,
 * where the system names the main thread. */
__attribute__((constructor)) static void hero_ui_note_main(void) {
    uint64_t none = 0;

    (void)atomic_compare_exchange_strong(&hero_ui_owner, &none, hero_layer_thread());
}
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
/* The native app's identity (`hero_ui_set_identity`), its length 0 until
 * given, and whether it has made a window, after which none is given. */
static _Thread_local char hero_ui_identity[HERO_UI_IDENTITY_MOST + 1];
static _Thread_local int64_t hero_ui_identity_count;
static _Thread_local bool hero_ui_windowed;
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
    return kind == HERO_UI_EVENT_TEXT || kind == HERO_UI_EVENT_SELECT || kind == HERO_UI_EVENT_VALUE || kind == HERO_UI_EVENT_RESIZE || kind == HERO_UI_EVENT_MOVE || kind == HERO_UI_EVENT_APPEARANCE;
}

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
bool hero_ui_is_utf8(const uint8_t *text, uint64_t length) {
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
        hero_ui_arm_dialogs_end(0);
        hero_ui_arm_instance_end();
        hero_ui_identity_count = 0;
        hero_ui_identity[0] = '\0';
        hero_ui_windowed = false;
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

static int64_t hero_ui_elsewhere(const char *what);
static int64_t hero_ui_answer_as_person(int64_t dialog, int64_t value, const void *paths, uint64_t count);
static int64_t hero_ui_system_answered(int64_t kind, const char *what, int64_t id);
static int64_t hero_ui_drop_as_person(int64_t id, int64_t x, int64_t y, const void *paths, uint64_t count);
static int64_t hero_ui_session_answered(int64_t kind, const char *what);
static int64_t hero_ui_open_as_system(int64_t value, int64_t what, const void *items, uint64_t count);
static const char *hero_ui_making(int64_t kind);
static const char *hero_ui_acting(int64_t act);

int64_t hero_ui_create(hero_ui_app *app, int64_t id, int64_t kind, int64_t parent) {
    char words[HERO_UI_WORDS_MOST];

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (kind < HERO_UI_WINDOW || (kind > HERO_UI_WEBVIEW && kind < HERO_UI_TABS) || kind > HERO_UI_SPIN) {
        (void)snprintf(words, sizeof words, "the kind %lld is no kind the native side makes", (long long)kind);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    if ((kind == HERO_UI_WINDOW || kind == HERO_UI_TRAY) != (parent == 0) || parent < 0) {
        (void)snprintf(words, sizeof words, "a widget of kind %lld in parent %lld: a window and a tray icon have none, every other kind one", (long long)kind, (long long)parent);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
#if defined(_WIN32)
    /* The kinds after the tray the Win32 arm makes not yet: refused by name
     * here, before its `makes` that knows them not. */
    if (kind > HERO_UI_TRAY) {
        return hero_ui_elsewhere(hero_ui_making(kind));
    }
#endif
    if (!hero_ui_arm_makes(kind, parent)) {
        return hero_ui_elsewhere(hero_ui_making(kind));
    }
    if (kind == HERO_UI_WEBVIEW) {
        return hero_ui_web_create(id, parent);
    }
    if (hero_ui_answered(hero_ui_arm_create(id, kind, parent), "making a widget", id) != 0) {
        return -1;
    }
    if (kind == HERO_UI_WINDOW) {
        hero_ui_windowed = true;
    }
    return 0;
}

int64_t hero_ui_destroy(hero_ui_app *app, int64_t id) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    /* A dialog over a window goes with it, unanswered (DIALOGS ANSWERED
     * THROUGH THE QUEUE). */
    hero_ui_arm_dialogs_end(id);
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
    int64_t answer = hero_ui_flag(app, id, HERO_UI_FLAG_VISIBLE, on, "showing a widget");

    /* A window is watched from the first time it is shown, so what the
     * person does to it among the screens is queued (WINDOWS ON THE SCREEN,
     * below); a widget's number names no window there, and nothing is. */
    if (answer == 0 && on == 1) {
        (void)hero_ui_arm_watch(id);
    }
    return answer;
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
    /* While a window is modal its session runs in place of the system's
     * loop, so the app's other windows take no input (WINDOWS ON THE
     * SCREEN, below). */
    kind = hero_ui_arm_modal_here() ? hero_ui_arm_modal_pump(ms) : hero_ui_arm_pump(ms);
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

    if (hero_ui_enter_native(app) == NULL) {
        return -1;
    }
    if (kind == HERO_UI_EVENT_DIALOG) {
        return hero_ui_answer_as_person(id, value, text, count);
    }
    if (kind == HERO_UI_EVENT_APPEARANCE) {
        if (id != 0 || value < 0 || value > HERO_UI_APPEARANCE_DARK || count != 0) {
            return hero_ui_said(HERO_UI_INVALID, "an appearance turned names no widget, 0, and is HERO_UI_APPEARANCE_* or 0 for the system's");
        }
        return hero_ui_system_answered(hero_ui_arm_appearance_set(value), "turning the appearance as the system would", 0);
    }
    if (kind == HERO_UI_EVENT_DROP) {
        return hero_ui_drop_as_person(id, value, second, text, count);
    }
    if (kind == HERO_UI_EVENT_OPEN) {
        return hero_ui_open_as_system(value, second, text, count);
    }
    if (kind == HERO_UI_EVENT_QUIT) {
        if (id != 0 || (value != 0 && value != 1) || second != 0 || count != 0) {
            return hero_ui_said(HERO_UI_INVALID, "an ask to end the app names no widget, 0, and is 1 for the session's end or 0 for another quit");
        }
        return hero_ui_session_answered(hero_ui_arm_ask_quit(value), "asking the app to end as the system would");
    }
    if (!hero_ui_id_given(id) || !hero_ui_text_given(text, count, "the text typed", false)) {
        return -1;
    }
    if ((kind < HERO_UI_EVENT_CLICK || kind > HERO_UI_EVENT_MESSAGE) && kind != HERO_UI_EVENT_ACTIVATE && kind != HERO_UI_EVENT_SUBMIT && kind != HERO_UI_EVENT_SORT && kind != HERO_UI_EVENT_UNFOLD) {
        (void)snprintf(words, sizeof words, "the act %lld is no act a person does", (long long)kind);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    if (kind == HERO_UI_EVENT_RESIZE && !hero_ui_sizes_given(value, second)) {
        return -1;
    }
    if (kind == HERO_UI_EVENT_ACTIVATE && value < 0) {
        (void)snprintf(words, sizeof words, "row %lld of a list; a row is 0 or more", (long long)value);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    if (kind == HERO_UI_EVENT_UNFOLD && (value < 1 || (second != 0 && second != 1))) {
        (void)snprintf(words, sizeof words, "unfolding item %lld, %lld; an item is 1 or more and the state 0 or 1", (long long)value, (long long)second);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    if (kind == HERO_UI_EVENT_SORT && (value < 0 || (second != 0 && second != 1))) {
        (void)snprintf(words, sizeof words, "a sort by column %lld, %lld; a column is 0 or more and the order 0 or 1", (long long)value, (long long)second);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    if (!hero_ui_arm_acts(kind)) {
        return hero_ui_elsewhere(hero_ui_acting(kind));
    }
    return hero_ui_answered(hero_ui_arm_inject(kind, id, value, second, text, (int64_t)count), "acting as the person", id);
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

/* -- THE CALLS ON A WINDOW AS THE SYSTEM SHOWS IT ----------------------------
 *
 * Their macOS arm is `platform/hero_ui_appkit_windows.swift`, a Swift member
 * of its own, which reads none of `hero_ui_appkit.swift`'s widgets: each
 * arm below asks that member for the AppKit object it acts on
 * (`hero_ui_appkit_object`) and hands it on within the same call, so a
 * widget the native side does not hold is `HERO_UI_NOT_FOUND` here, before
 * the member is reached. The Win32 and GTK arms answer
 * `HERO_UI_UNSUPPORTED` for each of them, in their own members (WHAT THIS ARM
 * DOES NOT DRAW), and the entry point says so in its own words
 * (`hero_ui_elsewhere`). */

#if defined(__APPLE__)

#include "hero_ui_appkit_windows.h"

int64_t hero_ui_arm_windows_words(char *words, int64_t count) {
    return hero_ui_appkit_windows_words(words, count);
}

int64_t hero_ui_arm_press_key(int64_t id, const uint8_t *key, int64_t count, int64_t modifiers) {
    void *window = hero_ui_appkit_object(id, HERO_UI_APPKIT_OBJECT_WINDOW);

    if (window == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    return hero_ui_appkit_press_key(window, hero_ui_appkit_object(id, HERO_UI_APPKIT_OBJECT_TYPED), key, count, modifiers);
}

bool hero_ui_arm_makes(int64_t kind, int64_t parent) {
    (void)kind;
    (void)parent;
    return true;
}

bool hero_ui_arm_acts(int64_t act) {
    (void)act;
    return true;
}

#endif

/* -1 with `HERO_UI_UNSUPPORTED`, the words naming the call `what` and where
 * the layer offers it: these calls are drawn on macOS alone, and the arm of
 * every other system answers none of them. */
static int64_t hero_ui_elsewhere(const char *what) {
    char words[HERO_UI_WORDS_MOST];

    (void)snprintf(words, sizeof words, "%s: the layer offers it on macOS alone, and this system's arm answers no such call", what);
    return hero_ui_said(HERO_UI_UNSUPPORTED, words);
}

/* What making a widget of `kind` is called where `hero_ui_arm_makes`
 * refuses it: one of this part's kinds, or a menu, which only a menu as its
 * parent brings here. */
static const char *hero_ui_making(int64_t kind) {
    switch (kind) {
    case HERO_UI_TABS:
        return "making tabs";
    case HERO_UI_PAGE:
        return "making a page of tabs";
    case HERO_UI_SCROLL:
        return "making a scrolled box";
    case HERO_UI_SPLIT_ROW:
        return "making two panes side by side";
    case HERO_UI_SPLIT_COLUMN:
        return "making two panes one above the other";
    case HERO_UI_PANE:
        return "making a pane";
    case HERO_UI_TABLE:
        return "making a table";
    case HERO_UI_TREE:
        return "making a tree";
    case HERO_UI_CONTEXT_MENU:
        return "making a context menu";
    case HERO_UI_TRAY:
        return "making a tray icon";
    case HERO_UI_SEPARATOR:
        return "making a separator";
    case HERO_UI_GROUP:
        return "making a group box";
    case HERO_UI_EDITABLE_COMBO:
        return "making an editable combo box";
    case HERO_UI_SPIN:
        return "making a spin";
    case HERO_UI_MENU:
        return "making a menu inside a menu";
    default:
        return "making a widget";
    }
}

/* What acting as the person with `act` is called where `hero_ui_arm_acts`
 * refuses it. */
static const char *hero_ui_acting(int64_t act) {
    switch (act) {
    case HERO_UI_EVENT_ACTIVATE:
        return "opening a row as the person";
    case HERO_UI_EVENT_SUBMIT:
        return "pressing Return in a text field as the person";
    case HERO_UI_EVENT_SORT:
        return "clicking a table's header as the person";
    case HERO_UI_EVENT_UNFOLD:
        return "unfolding an item of a tree as the person";
    default:
        return "acting as the person";
    }
}

/* The answer `kind` of an arm of this part for the call `what` names on
 * widget `id`, as the entry point answers it: `hero_ui_answered`'s, but for
 * `HERO_UI_OTHER`, whose words are this part's member's own, and
 * `HERO_UI_UNSUPPORTED`, which names the one system that offers the call
 * (`hero_ui_elsewhere`). */
static int64_t hero_ui_windows_answered(int64_t kind, const char *what, int64_t id) {
    char words[HERO_UI_WORDS_MOST];
    char theirs[HERO_UI_WORDS_MOST];

    if (kind == HERO_UI_OTHER) {
        theirs[0] = '\0';
        if (hero_ui_arm_windows_words(theirs, (int64_t)sizeof theirs) <= 0) {
            (void)snprintf(theirs, sizeof theirs, "the system refused, giving no words");
        }
        (void)snprintf(words, sizeof words, "%s: %s", what, theirs);
        return hero_ui_said(HERO_UI_OTHER, words);
    }
    /* An arm that draws the call says what this system lacks where it has
     * words; one that draws none answers in the layer's words. */
    if (kind == HERO_UI_UNSUPPORTED) {
        theirs[0] = '\0';
        if (hero_ui_arm_windows_words(theirs, (int64_t)sizeof theirs) <= 0) {
            return hero_ui_elsewhere(what);
        }
        (void)snprintf(words, sizeof words, "%s: %s", what, theirs);
        return hero_ui_said(HERO_UI_UNSUPPORTED, words);
    }
    return hero_ui_answered(kind, what, id);
}

int64_t hero_ui_press_key(hero_ui_app *app, int64_t id, const void *key, uint64_t count, int64_t modifiers) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_text_given(key, count, "the key", false)) {
        return -1;
    }
    if (count == 0) {
        return hero_ui_said(HERO_UI_INVALID, "pressing a key: no key was named");
    }
    if (modifiers < 0 || modifiers > (HERO_UI_PRIMARY | HERO_UI_SHIFT | HERO_UI_ALT)) {
        return hero_ui_said(HERO_UI_INVALID, "the keys held are HERO_UI_PRIMARY, HERO_UI_SHIFT and HERO_UI_ALT added");
    }
    return hero_ui_windows_answered(hero_ui_arm_press_key(id, key, (int64_t)count, modifiers), "pressing a key", id);
}

/* -- WINDOWS ON THE SCREEN ------------------------------------------------- */


#if defined(__APPLE__)

/* The `NSWindow` of window `id`, or NULL with `*kind` the answer: NOT_FOUND
 * where the native side holds no widget `id`, INVALID where it is no
 * window. */
static void *hero_ui_own_window(int64_t id, int64_t *kind) {
    void *window = hero_ui_appkit_object(id, HERO_UI_APPKIT_OBJECT_OWN_WINDOW);

    if (window == NULL) {
        *kind = hero_ui_appkit_object(id, HERO_UI_APPKIT_OBJECT_WINDOW) == NULL ? HERO_UI_NOT_FOUND : HERO_UI_INVALID;
    }
    return window;
}

int64_t hero_ui_arm_watch(int64_t id) {
    void *window = hero_ui_appkit_object(id, HERO_UI_APPKIT_OBJECT_OWN_WINDOW);

    return window == NULL ? HERO_UI_OK : hero_ui_appkit_windows_watch(window, id, hero_ui_push);
}

int64_t hero_ui_arm_screens(int64_t *values, int64_t count, int64_t *screens) {
    return hero_ui_appkit_screens(values, count, screens);
}

int64_t hero_ui_arm_frame_of(int64_t id, int64_t *screen, int64_t *x, int64_t *y, int64_t *width, int64_t *height) {
    int64_t kind = HERO_UI_OK;
    void *window = hero_ui_own_window(id, &kind);

    return window == NULL ? kind : hero_ui_appkit_frame_of(window, screen, x, y, width, height);
}

int64_t hero_ui_arm_set_position(int64_t id, int64_t screen, int64_t x, int64_t y, int64_t person) {
    int64_t kind = HERO_UI_OK;
    void *window = hero_ui_own_window(id, &kind);

    return window == NULL ? kind : hero_ui_appkit_set_position(window, screen, x, y, person);
}

int64_t hero_ui_arm_center(int64_t id, int64_t over) {
    int64_t kind = HERO_UI_OK;
    void *window = hero_ui_own_window(id, &kind);
    void *anchor = NULL;

    if (window == NULL) {
        return kind;
    }
    if (over != 0) {
        anchor = hero_ui_own_window(over, &kind);
        if (anchor == NULL) {
            return kind;
        }
    }
    return hero_ui_appkit_center(window, anchor);
}

int64_t hero_ui_arm_set_state(int64_t id, int64_t state, int64_t person) {
    int64_t kind = HERO_UI_OK;
    void *window = hero_ui_own_window(id, &kind);

    return window == NULL ? kind : hero_ui_appkit_set_state(window, state, person);
}

int64_t hero_ui_arm_state(int64_t id, int64_t *state) {
    int64_t kind = HERO_UI_OK;
    void *window = hero_ui_own_window(id, &kind);

    return window == NULL ? kind : hero_ui_appkit_state(window, state);
}

int64_t hero_ui_arm_window_set(int64_t id, int64_t which, int64_t value) {
    int64_t kind = HERO_UI_OK;
    void *window = hero_ui_own_window(id, &kind);

    if (window == NULL) {
        return kind;
    }
    switch (which) {
    case HERO_UI_WINDOW_SET_ON_TOP:
        return hero_ui_appkit_set_on_top(window, value);
    case HERO_UI_WINDOW_SET_FRAMED:
        return hero_ui_appkit_set_framed(window, value);
    case HERO_UI_WINDOW_SET_OPACITY:
        return hero_ui_appkit_set_opacity(window, value);
    case HERO_UI_WINDOW_SET_BACKDROP:
        return hero_ui_appkit_set_backdrop(window, value);
    default:
        return hero_ui_appkit_set_modal(window, value);
    }
}

int64_t hero_ui_arm_set_icon(const uint8_t *rgba, int64_t width, int64_t height) {
    return hero_ui_appkit_set_icon(rgba, width, height);
}

/* How many events the native app's queue holds: what the modal session's
 * loop asks to know when to hand back. */
static int64_t hero_ui_held_now(void) {
    return hero_ui_native_slot == 0 ? 0 : hero_ui_slots[hero_ui_native_slot - 1].held;
}

bool hero_ui_arm_modal_here(void) {
    return hero_ui_appkit_modal_here() == 1;
}

int64_t hero_ui_arm_modal_pump(int64_t ms) {
    return hero_ui_appkit_modal_pump(ms, hero_ui_held_now);
}

#endif

int64_t hero_ui_screens(hero_ui_app *app, int64_t *values, uint64_t count) {
    int64_t screens = 0;

    if (hero_ui_enter_native(app) == NULL) {
        return -1;
    }
    if (count > 0 && values == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "room was named for the screens and none given");
    }
    if (count > (uint64_t)HERO_UI_TEXT_MOST) {
        count = (uint64_t)HERO_UI_TEXT_MOST;
    }
    if (hero_ui_windows_answered(hero_ui_arm_screens(values, (int64_t)count, &screens), "reading the screens", 0) != 0) {
        return -1;
    }
    return screens;
}

int64_t hero_ui_frame_of(hero_ui_app *app, int64_t id, int64_t *screen, int64_t *x, int64_t *y, int64_t *width, int64_t *height) {
    int64_t on = 0;
    int64_t left = 0;
    int64_t top = 0;
    int64_t wide = 0;
    int64_t high = 0;
    int64_t answer;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (screen == NULL || x == NULL || y == NULL || width == NULL || height == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "no place was given for the frame");
    }
    answer = hero_ui_windows_answered(hero_ui_arm_frame_of(id, &on, &left, &top, &wide, &high), "reading a window's frame", id);
    if (answer == 0) {
        *screen = on;
        *x = left;
        *y = top;
        *width = wide;
        *height = high;
    }
    return answer;
}

/* Whether `x` and `y` are a position a window takes, else the failure
 * said. */
static bool hero_ui_position_given(int64_t screen, int64_t x, int64_t y) {
    char words[HERO_UI_WORDS_MOST];

    if (screen < 0 || x < -HERO_UI_POSITION_MOST || x > HERO_UI_POSITION_MOST || y < -HERO_UI_POSITION_MOST || y > HERO_UI_POSITION_MOST) {
        (void)snprintf(words, sizeof words, "the position %lld, %lld on screen %lld; a screen is 0 or more, and a position at most %d points from its corner either way", (long long)x, (long long)y, (long long)screen, HERO_UI_POSITION_MOST);
        (void)hero_ui_said(HERO_UI_INVALID, words);
        return false;
    }
    return true;
}

int64_t hero_ui_set_position(hero_ui_app *app, int64_t id, int64_t screen, int64_t x, int64_t y) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_position_given(screen, x, y)) {
        return -1;
    }
    return hero_ui_windows_answered(hero_ui_arm_set_position(id, screen, x, y, 0), "placing a window on a screen", id);
}

int64_t hero_ui_center(hero_ui_app *app, int64_t id, int64_t over) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (over < 0 || over == id) {
        return hero_ui_said(HERO_UI_INVALID, "a window is centred over another window, or over none, 0");
    }
    return hero_ui_windows_answered(hero_ui_arm_center(id, over), "centring a window", id);
}

/* Whether `state` is one of the four, else the failure said. */
static bool hero_ui_state_given(int64_t state) {
    char words[HERO_UI_WORDS_MOST];

    if (state < HERO_UI_STATE_NORMAL || state > HERO_UI_STATE_FULL_SCREEN) {
        (void)snprintf(words, sizeof words, "the state %lld is none of HERO_UI_STATE_NORMAL to HERO_UI_STATE_FULL_SCREEN", (long long)state);
        (void)hero_ui_said(HERO_UI_INVALID, words);
        return false;
    }
    return true;
}

int64_t hero_ui_set_state(hero_ui_app *app, int64_t id, int64_t state) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_state_given(state)) {
        return -1;
    }
    return hero_ui_windows_answered(hero_ui_arm_set_state(id, state, 0), "bringing a window to a state", id);
}

int64_t hero_ui_state(hero_ui_app *app, int64_t id, int64_t *state) {
    int64_t got = HERO_UI_STATE_NORMAL;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (state == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "no place was given for the state");
    }
    if (hero_ui_windows_answered(hero_ui_arm_state(id, &got), "reading a window's state", id) != 0) {
        return -1;
    }
    *state = got;
    return 0;
}

/* One of a window's settings, `value` within `least` and `most`. */
static int64_t hero_ui_window_set(hero_ui_app *app, int64_t id, int64_t which, int64_t value, int64_t least, int64_t most, const char *what) {
    char words[HERO_UI_WORDS_MOST];

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (value < least || value > most) {
        (void)snprintf(words, sizeof words, "%s: %lld is outside %lld to %lld", what, (long long)value, (long long)least, (long long)most);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    return hero_ui_windows_answered(hero_ui_arm_window_set(id, which, value), what, id);
}

int64_t hero_ui_set_on_top(hero_ui_app *app, int64_t id, int64_t on) {
    return hero_ui_window_set(app, id, HERO_UI_WINDOW_SET_ON_TOP, on, 0, 1, "keeping a window on top");
}

int64_t hero_ui_set_framed(hero_ui_app *app, int64_t id, int64_t on) {
    return hero_ui_window_set(app, id, HERO_UI_WINDOW_SET_FRAMED, on, 0, 1, "giving a window its frame");
}

int64_t hero_ui_set_opacity(hero_ui_app *app, int64_t id, int64_t alpha) {
    return hero_ui_window_set(app, id, HERO_UI_WINDOW_SET_OPACITY, alpha, 0, 255, "setting a window's opacity");
}

int64_t hero_ui_set_backdrop(hero_ui_app *app, int64_t id, int64_t backdrop) {
    return hero_ui_window_set(app, id, HERO_UI_WINDOW_SET_BACKDROP, backdrop, HERO_UI_BACKDROP_NONE, HERO_UI_BACKDROP_TRANSLUCENT, "setting a window's backdrop");
}

int64_t hero_ui_set_modal(hero_ui_app *app, int64_t id, int64_t on) {
    return hero_ui_window_set(app, id, HERO_UI_WINDOW_SET_MODAL, on, 0, 1, "making a window modal");
}

int64_t hero_ui_set_icon(hero_ui_app *app, const void *rgba, uint64_t count, int64_t width, int64_t height) {
    uint64_t pixels;
    char words[HERO_UI_WORDS_MOST];

    if (hero_ui_enter_native(app) == NULL || !hero_ui_sizes_given(width, height)) {
        return -1;
    }
    if (__builtin_mul_overflow((uint64_t)width, (uint64_t)height, &pixels) || __builtin_mul_overflow(pixels, (uint64_t)4, &pixels) || pixels != count || (count > 0 && rgba == NULL) || count > (uint64_t)HERO_UI_TEXT_MOST) {
        (void)snprintf(words, sizeof words, "%llu bytes of pixels for an icon of %lld by %lld, which takes 4 a pixel and at most %d bytes", (unsigned long long)count, (long long)width, (long long)height, HERO_UI_TEXT_MOST);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    return hero_ui_windows_answered(hero_ui_arm_set_icon(rgba, width, height), "setting the app's icon", 0);
}

int64_t hero_ui_inject_window(hero_ui_app *app, int64_t kind, int64_t id, int64_t value, int64_t second, int64_t third) {
    char words[HERO_UI_WORDS_MOST];

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (kind == HERO_UI_EVENT_MOVE) {
        if (!hero_ui_position_given(value, second, third)) {
            return -1;
        }
        return hero_ui_windows_answered(hero_ui_arm_set_position(id, value, second, third, 1), "moving a window as the person", id);
    }
    if (kind == HERO_UI_EVENT_STATE) {
        if (!hero_ui_state_given(value)) {
            return -1;
        }
        return hero_ui_windows_answered(hero_ui_arm_set_state(id, value, 1), "bringing a window to a state as the person", id);
    }
    (void)snprintf(words, sizeof words, "the act %lld is no act a person does to a window among the screens", (long long)kind);
    return hero_ui_said(HERO_UI_INVALID, words);
}

/* -- A MENU ITEM'S ROLE ---------------------------------------------------- */

#if defined(__APPLE__)

int64_t hero_ui_arm_set_role(int64_t id, int64_t role) {
    return hero_ui_appkit_set_role(id, role);
}

#endif

int64_t hero_ui_set_role(hero_ui_app *app, int64_t id, int64_t role) {
    char words[HERO_UI_WORDS_MOST];

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (role < HERO_UI_ROLE_NONE || role > HERO_UI_ROLE_QUIT) {
        (void)snprintf(words, sizeof words, "the role %lld is none of HERO_UI_ROLE_NONE to HERO_UI_ROLE_QUIT", (long long)role);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    return hero_ui_windows_answered(hero_ui_arm_set_role(id, role), "giving a menu item its role", id);
}

/* -- RETURN AND ESCAPE ----------------------------------------------------- */

#if defined(__APPLE__)

int64_t hero_ui_arm_set_button_key(int64_t id, int64_t key) {
    return hero_ui_appkit_set_button_key(id, key);
}

#endif

int64_t hero_ui_set_button_key(hero_ui_app *app, int64_t id, int64_t key) {
    char words[HERO_UI_WORDS_MOST];

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (key < HERO_UI_BUTTON_KEY_NONE || key > HERO_UI_BUTTON_KEY_ESCAPE) {
        (void)snprintf(words, sizeof words, "the key %lld is none of HERO_UI_BUTTON_KEY_NONE to HERO_UI_BUTTON_KEY_ESCAPE", (long long)key);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    return hero_ui_windows_answered(hero_ui_arm_set_button_key(id, key), "giving a button its key", id);
}

/* -- THE WIDGET WITH THE KEYS ---------------------------------------------- */

#if defined(__APPLE__)

int64_t hero_ui_arm_set_focus(int64_t id) {
    return hero_ui_appkit_set_focus(id);
}

int64_t hero_ui_arm_focus(int64_t id, int64_t *focus) {
    return hero_ui_appkit_focus(id, focus);
}

#endif

int64_t hero_ui_set_focus(hero_ui_app *app, int64_t id) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    return hero_ui_windows_answered(hero_ui_arm_set_focus(id), "giving a widget the keys", id);
}

int64_t hero_ui_focus(hero_ui_app *app, int64_t id, int64_t *focus) {
    int64_t found = 0;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (focus == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "no place was given for the widget with the keys");
    }
    if (hero_ui_windows_answered(hero_ui_arm_focus(id, &found), "asking which widget has the keys", id) != 0) {
        return -1;
    }
    *focus = found;
    return 0;
}

/* -- DIALOGS ANSWERED THROUGH THE QUEUE ------------------------------------ */

/* What a dialog's answer is queued by on macOS: the queue itself and the
 * wake of the loop `pump` runs, since AppKit hands a sheet's answer to its
 * completion handler outside any event that loop reads. The Win32 and GTK 4
 * arms queue their own by `hero_ui_tell_text`. */
#if defined(__APPLE__)
static void hero_ui_dialog_told(int64_t kind, int64_t id, int64_t value, int64_t second, const uint8_t *text, int64_t count) {
    hero_ui_push(kind, id, value, second, text, count);
    hero_ui_appkit_wake();
}
#endif

#if defined(__APPLE__)

#include "hero_ui_appkit_dialogs.h"

int64_t hero_ui_arm_show_message(int64_t dialog, int64_t window, int64_t tone, int64_t buttons, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count) {
    void *owner = hero_ui_appkit_object(window, HERO_UI_APPKIT_OBJECT_OWN_WINDOW);

    if (owner == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    return hero_ui_appkit_show_message(owner, window, dialog, tone, buttons, title, title_count, text, text_count, hero_ui_dialog_told);
}

int64_t hero_ui_arm_show_ask(int64_t dialog, int64_t window, int64_t tone, const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count, const uint8_t *buttons, int64_t buttons_count, int64_t cancel) {
    void *owner = hero_ui_appkit_object(window, HERO_UI_APPKIT_OBJECT_OWN_WINDOW);

    if (owner == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    return hero_ui_appkit_show_ask(owner, window, dialog, tone, title, title_count, text, text_count, buttons, buttons_count, cancel, hero_ui_dialog_told);
}

int64_t hero_ui_arm_show_files(int64_t dialog, int64_t window, int64_t save, int64_t multiple, const uint8_t *title, int64_t title_count, const uint8_t *name, int64_t name_count, const uint8_t *filters, int64_t filters_count) {
    void *owner = hero_ui_appkit_object(window, HERO_UI_APPKIT_OBJECT_OWN_WINDOW);

    if (owner == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    return hero_ui_appkit_show_files(owner, window, dialog, save, multiple, title, title_count, name, name_count, filters, filters_count, hero_ui_dialog_told);
}

int64_t hero_ui_arm_show_folders(int64_t dialog, int64_t window, int64_t multiple, const uint8_t *title, int64_t title_count) {
    void *owner = hero_ui_appkit_object(window, HERO_UI_APPKIT_OBJECT_OWN_WINDOW);

    if (owner == NULL) {
        return HERO_UI_NOT_FOUND;
    }
    return hero_ui_appkit_show_folders(owner, window, dialog, multiple, title, title_count, hero_ui_dialog_told);
}

int64_t hero_ui_arm_answer_dialog(int64_t dialog, int64_t value, const uint8_t *paths, int64_t count) {
    return hero_ui_appkit_answer_dialog(dialog, value, paths, count);
}

void hero_ui_arm_dialogs_end(int64_t window) {
    hero_ui_appkit_dialogs_end(window);
}

int64_t hero_ui_arm_dialogs_words(char *words, int64_t count) {
    return hero_ui_appkit_dialogs_words(words, count);
}

#endif

/* Whether `dialog` and the `window` it stands over are numbers a dialog
 * takes, else the failure said. */
static bool hero_ui_dialog_given(int64_t dialog, int64_t window) {
    char words[HERO_UI_WORDS_MOST];

    if (dialog >= 1 && window >= 1) {
        return true;
    }
    (void)snprintf(words, sizeof words, "dialog %lld over window %lld; a dialog's number is 1 or more, and so is the number of the window it stands over", (long long)dialog, (long long)window);
    (void)hero_ui_said(HERO_UI_INVALID, words);
    return false;
}

/* 0 where the arm answered `kind` `HERO_UI_OK`, else -1 with the failure
 * said as `what`, naming dialog `dialog` and, where 1 or more, the window
 * it was to stand over. */
static int64_t hero_ui_dialog_answered(int64_t kind, const char *what, int64_t dialog, int64_t window) {
    char words[HERO_UI_WORDS_MOST];
    char theirs[HERO_UI_WORDS_MOST];

    if (kind == HERO_UI_OK) {
        hero_ui_fine();
        return 0;
    }
    if (kind == HERO_UI_NOT_FOUND && window > 0) {
        (void)snprintf(words, sizeof words, "%s: the native side holds no window %lld, or shows a dialog %lld already", what, (long long)window, (long long)dialog);
        return hero_ui_said(HERO_UI_NOT_FOUND, words);
    }
    if (kind == HERO_UI_NOT_FOUND) {
        (void)snprintf(words, sizeof words, "%s: the native side shows no dialog %lld, or it was answered already", what, (long long)dialog);
        return hero_ui_said(HERO_UI_NOT_FOUND, words);
    }
    if (kind == HERO_UI_INVALID && window == 0) {
        (void)snprintf(words, sizeof words, "%s: dialog %lld has no button of that answer, or takes no such choice", what, (long long)dialog);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    if (kind == HERO_UI_OTHER || kind == HERO_UI_UNSUPPORTED) {
        theirs[0] = '\0';
        if (hero_ui_arm_dialogs_words(theirs, (int64_t)sizeof theirs) <= 0) {
            (void)snprintf(theirs, sizeof theirs, "the system refused, giving no words");
        }
        (void)snprintf(words, sizeof words, "%s: %s", what, theirs);
        return hero_ui_said(kind, words);
    }
    return hero_ui_answered(kind, what, window);
}

int64_t hero_ui_message(hero_ui_app *app, int64_t dialog, int64_t window, int64_t tone, int64_t buttons, const void *title, uint64_t title_count, const void *text, uint64_t text_count) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_dialog_given(dialog, window) || !hero_ui_text_given(title, title_count, "the title", false) || !hero_ui_text_given(text, text_count, "the text", false)) {
        return -1;
    }
    if (tone < HERO_UI_TONE_INFO || tone > HERO_UI_TONE_ERROR || buttons < HERO_UI_BUTTONS_OK || buttons > HERO_UI_BUTTONS_YES_NO_CANCEL) {
        return hero_ui_said(HERO_UI_INVALID, "a message box's tone and its buttons are hero_ui.h's");
    }
    return hero_ui_dialog_answered(hero_ui_arm_show_message(dialog, window, tone, buttons, title, (int64_t)title_count, text, (int64_t)text_count), "showing a message box", dialog, window);
}

int64_t hero_ui_file_dialog(hero_ui_app *app, int64_t dialog, int64_t window, int64_t save, int64_t multiple, const void *title, uint64_t title_count, const void *name, uint64_t name_count, const void *filters, uint64_t filters_count) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_dialog_given(dialog, window) || !hero_ui_text_given(title, title_count, "the title", false) || !hero_ui_text_given(name, name_count, "the name", false) || !hero_ui_text_given(filters, filters_count, "the filters", true)) {
        return -1;
    }
    if ((save != 0 && save != 1) || (multiple != 0 && multiple != 1) || (save == 1 && multiple == 1)) {
        return hero_ui_said(HERO_UI_INVALID, "a file dialog's `save` and `multiple` are 0 or 1, and a save chooses one file");
    }
    return hero_ui_dialog_answered(hero_ui_arm_show_files(dialog, window, save, multiple, title, (int64_t)title_count, name, (int64_t)name_count, filters, (int64_t)filters_count), "showing a file dialog", dialog, window);
}

int64_t hero_ui_folder_dialog(hero_ui_app *app, int64_t dialog, int64_t window, int64_t multiple, const void *title, uint64_t title_count) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_dialog_given(dialog, window) || !hero_ui_text_given(title, title_count, "the title", false)) {
        return -1;
    }
    if (multiple != 0 && multiple != 1) {
        return hero_ui_said(HERO_UI_INVALID, "a folder dialog's `multiple` is 0 or 1");
    }
    return hero_ui_dialog_answered(hero_ui_arm_show_folders(dialog, window, multiple, title, (int64_t)title_count), "showing a folder dialog", dialog, window);
}

int64_t hero_ui_ask(hero_ui_app *app, int64_t dialog, int64_t window, int64_t tone, const void *title, uint64_t title_count, const void *text, uint64_t text_count, const void *buttons, uint64_t buttons_count, int64_t cancel) {
    const uint8_t *labels = buttons;
    int64_t count = 0;
    char words[HERO_UI_WORDS_MOST];

    if (hero_ui_enter_native(app) == NULL || !hero_ui_dialog_given(dialog, window) || !hero_ui_text_given(title, title_count, "the title", false) || !hero_ui_text_given(text, text_count, "the text", false) || !hero_ui_text_given(buttons, buttons_count, "the buttons", true)) {
        return -1;
    }
    for (uint64_t at = 0; at < buttons_count; at += 1) {
        if (labels[at] == 0) {
            if (at == 0 || labels[at - 1] == 0) {
                return hero_ui_said(HERO_UI_INVALID, "a button's label is empty, and a person reads none");
            }
            count += 1;
        } else if (labels[at] == '\n') {
            return hero_ui_said(HERO_UI_INVALID, "a button's label holds a line feed, and shows one line");
        }
    }
    if (count < 1 || count > HERO_UI_ASK_BUTTONS_MOST) {
        (void)snprintf(words, sizeof words, "a message box of %lld buttons; it holds 1 to %d", (long long)count, HERO_UI_ASK_BUTTONS_MOST);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    if (tone < HERO_UI_TONE_INFO || tone > HERO_UI_TONE_ERROR || cancel < -1 || cancel >= count) {
        return hero_ui_said(HERO_UI_INVALID, "a message box's tone is hero_ui.h's, and its cancel button -1 or one of its buttons");
    }
    return hero_ui_dialog_answered(hero_ui_arm_show_ask(dialog, window, tone, title, (int64_t)title_count, text, (int64_t)text_count, labels, (int64_t)buttons_count, cancel), "showing a message box", dialog, window);
}

/* `hero_ui_inject`'s `HERO_UI_EVENT_DIALOG`: answers dialog `dialog` as its
 * person would, `value` the answer and `paths` the paths chosen. */
static int64_t hero_ui_answer_as_person(int64_t dialog, int64_t value, const void *paths, uint64_t count) {
    const uint8_t *bytes = paths;
    int64_t texts = 0;
    char words[HERO_UI_WORDS_MOST];

    if (dialog < 1 || value < 0) {
        (void)snprintf(words, sizeof words, "answering dialog %lld with %lld; a dialog's number is 1 or more and its answer 0 or more", (long long)dialog, (long long)value);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    if (!hero_ui_text_given(paths, count, "the paths chosen", true)) {
        return -1;
    }
    for (uint64_t at = 0; at < count; at += 1) {
        if (bytes[at] == 0) {
            texts += 1;
        }
    }
    if (count > 0 && texts != value) {
        (void)snprintf(words, sizeof words, "%lld paths given for a choice of %lld", (long long)texts, (long long)value);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    return hero_ui_dialog_answered(hero_ui_arm_answer_dialog(dialog, value, bytes, (int64_t)count), "answering a dialog as the person", dialog, 0);
}

/* -- CHECK AND RADIO MENU ITEMS -------------------------------------------- */

#if defined(__APPLE__)

int64_t hero_ui_arm_set_check(int64_t id, int64_t mode) {
    return hero_ui_appkit_set_check(id, mode);
}

#endif

int64_t hero_ui_set_check(hero_ui_app *app, int64_t id, int64_t mode) {
    char words[HERO_UI_WORDS_MOST];

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (mode < HERO_UI_CHECK_NONE || mode > HERO_UI_CHECK_RADIO) {
        (void)snprintf(words, sizeof words, "the mode %lld is none of HERO_UI_CHECK_NONE to HERO_UI_CHECK_RADIO", (long long)mode);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    return hero_ui_windows_answered(hero_ui_arm_set_check(id, mode), "making a menu item checkable", id);
}

/* -- A MASKED FIELD, AND A PROGRESS BAR WITH NO VALUE ----------------------- */

#if defined(__APPLE__)

int64_t hero_ui_arm_set_masked(int64_t id, int64_t on) {
    return hero_ui_appkit_set_masked(id, on);
}

int64_t hero_ui_arm_set_moving(int64_t id, int64_t on) {
    return hero_ui_appkit_set_moving(id, on);
}

#endif

int64_t hero_ui_set_masked(hero_ui_app *app, int64_t id, int64_t on) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (on != 0 && on != 1) {
        return hero_ui_said(HERO_UI_INVALID, "a flag is 0 or 1");
    }
    return hero_ui_windows_answered(hero_ui_arm_set_masked(id, on), "masking a text field", id);
}

int64_t hero_ui_set_moving(hero_ui_app *app, int64_t id, int64_t on) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (on != 0 && on != 1) {
        return hero_ui_said(HERO_UI_INVALID, "a flag is 0 or 1");
    }
    return hero_ui_windows_answered(hero_ui_arm_set_moving(id, on), "letting a progress bar move", id);
}

/* -- THE CLIPBOARD --------------------------------------------------------- */

#if defined(__APPLE__)

int64_t hero_ui_arm_clipboard_set(int64_t kind, const uint8_t *bytes, int64_t count, int64_t width, int64_t height) {
    return hero_ui_appkit_clipboard_set(kind, bytes, count, width, height);
}

int64_t hero_ui_arm_clipboard_get(int64_t kind, int64_t *bytes, int64_t *width, int64_t *height) {
    return hero_ui_appkit_clipboard_get(kind, bytes, width, height);
}

int64_t hero_ui_arm_clipboard_bytes(uint8_t *buffer, int64_t count) {
    return hero_ui_appkit_clipboard_bytes(buffer, count);
}

#endif

/* The name of clipboard kind `kind`, for the words of a refusal. */
static const char *hero_ui_clip_name(int64_t kind) {
    if (kind == HERO_UI_CLIP_NOTHING) {
        return "emptying";
    }
    if (kind == HERO_UI_CLIP_TEXT) {
        return "text";
    }
    if (kind == HERO_UI_CLIP_FILES) {
        return "paths";
    }
    return "an image";
}

/* Whether `count` bytes at `bytes` are absolute paths of UTF-8, each
 * followed by one zero byte, at least one; false with the words said
 * otherwise. Absolute as POSIX writes it, `/` first, or as Windows does, a
 * drive's letter and colon and slash or two slashes first: the clipboard is
 * read by other programs, whose working folder is not this one's. */
static bool hero_ui_paths_given(const uint8_t *bytes, uint64_t count) {
    char words[HERO_UI_WORDS_MOST];
    uint64_t start = 0;

    if (count == 0 || bytes[count - 1] != 0) {
        (void)hero_ui_said(HERO_UI_INVALID, count == 0 ? "the clipboard's paths: none was given" : "the clipboard's paths: the last is not followed by a zero byte");
        return false;
    }
    for (uint64_t at = 0; at < count; at += 1) {
        if (bytes[at] != 0) {
            continue;
        }
        uint64_t length = at - start;
        const uint8_t *path = bytes + start;
        bool posix = length >= 1 && path[0] == '/';
        bool drive = length >= 3 && ((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z')) && path[1] == ':' && (path[2] == '\\' || path[2] == '/');
        bool share = length >= 2 && path[0] == '\\' && path[1] == '\\';

        if (length == 0 || !hero_ui_is_utf8(path, length) || !(posix || drive || share)) {
            (void)snprintf(words, sizeof words, "the clipboard's paths: the path at byte %llu is %s", (unsigned long long)start, length == 0 ? "empty" : !hero_ui_is_utf8(path, length) ? "no UTF-8" : "not absolute, and the programs that read the clipboard work in folders of their own");
            (void)hero_ui_said(HERO_UI_INVALID, words);
            return false;
        }
        start = at + 1;
    }
    return true;
}

int64_t hero_ui_clipboard_write(hero_ui_app *app, int64_t kind, const void *bytes, uint64_t count, int64_t width, int64_t height) {
    char words[HERO_UI_WORDS_MOST];
    uint64_t pixels;

    if (hero_ui_enter_native(app) == NULL) {
        return -1;
    }
    if (kind < HERO_UI_CLIP_NOTHING || kind > HERO_UI_CLIP_IMAGE) {
        (void)snprintf(words, sizeof words, "the kind %lld is none of HERO_UI_CLIP_NOTHING to HERO_UI_CLIP_IMAGE", (long long)kind);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    if (count > 0 && bytes == NULL) {
        (void)snprintf(words, sizeof words, "the clipboard's %s: %llu bytes were named and none given", hero_ui_clip_name(kind), (unsigned long long)count);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    if (count > (uint64_t)HERO_UI_CLIP_MOST) {
        (void)snprintf(words, sizeof words, "the clipboard's %s takes %llu bytes, past the %d a write puts there", hero_ui_clip_name(kind), (unsigned long long)count, HERO_UI_CLIP_MOST);
        return hero_ui_said(HERO_UI_LIMIT, words);
    }
    if (kind == HERO_UI_CLIP_IMAGE) {
        if (width <= 0 || height <= 0 || __builtin_mul_overflow((uint64_t)width, (uint64_t)height, &pixels) || __builtin_mul_overflow(pixels, (uint64_t)4, &pixels) || pixels != count) {
            (void)snprintf(words, sizeof words, "%llu bytes of pixels for an image of %lld by %lld, which takes 4 a pixel and at least one pixel", (unsigned long long)count, (long long)width, (long long)height);
            return hero_ui_said(HERO_UI_INVALID, words);
        }
    } else if (width != 0 || height != 0) {
        return hero_ui_said(HERO_UI_INVALID, "the clipboard's emptying, text and paths take no size: width and height are 0");
    } else if (kind == HERO_UI_CLIP_NOTHING && count != 0) {
        return hero_ui_said(HERO_UI_INVALID, "emptying the clipboard takes no bytes");
    } else if (kind == HERO_UI_CLIP_TEXT && !hero_ui_is_utf8(bytes, count)) {
        return hero_ui_said(HERO_UI_INVALID, "the clipboard's text is no UTF-8");
    } else if (kind == HERO_UI_CLIP_FILES && !hero_ui_paths_given(bytes, count)) {
        return -1;
    }
    return hero_ui_windows_answered(hero_ui_arm_clipboard_set(kind, bytes, (int64_t)count, width, height), "writing the clipboard", 0);
}

int64_t hero_ui_clipboard_read(hero_ui_app *app, int64_t kind, int64_t *width, int64_t *height) {
    char words[HERO_UI_WORDS_MOST];
    char theirs[HERO_UI_WORDS_MOST];
    int64_t bytes = 0;
    int64_t answer;

    if (hero_ui_enter_native(app) == NULL) {
        return -1;
    }
    if (kind < HERO_UI_CLIP_TEXT || kind > HERO_UI_CLIP_IMAGE) {
        (void)snprintf(words, sizeof words, "the kind %lld is none of HERO_UI_CLIP_TEXT to HERO_UI_CLIP_IMAGE", (long long)kind);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    if (width == NULL || height == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "reading the clipboard: no room was given for the image's size");
    }
    *width = 0;
    *height = 0;
    answer = hero_ui_arm_clipboard_get(kind, &bytes, width, height);
    if (answer == HERO_UI_NOT_FOUND) {
        (void)snprintf(words, sizeof words, "reading the clipboard: it holds no %s", hero_ui_clip_name(kind));
        return hero_ui_said(HERO_UI_NOT_FOUND, words);
    }
    if (answer == HERO_UI_LIMIT) {
        theirs[0] = '\0';
        if (hero_ui_arm_windows_words(theirs, (int64_t)sizeof theirs) <= 0) {
            (void)snprintf(theirs, sizeof theirs, "what it holds is past the %d bytes a read takes", HERO_UI_CLIP_MOST);
        }
        (void)snprintf(words, sizeof words, "reading the clipboard: %s", theirs);
        return hero_ui_said(HERO_UI_LIMIT, words);
    }
    if (hero_ui_windows_answered(answer, "reading the clipboard", 0) < 0) {
        return -1;
    }
    return bytes;
}

int64_t hero_ui_clipboard_bytes(hero_ui_app *app, void *buffer, uint64_t count) {
    int64_t bytes;

    if (hero_ui_enter_native(app) == NULL) {
        return -1;
    }
    if (count > 0 && buffer == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "room was named for the clipboard's bytes and none given");
    }
    if (count > (uint64_t)HERO_UI_CLIP_MOST) {
        count = (uint64_t)HERO_UI_CLIP_MOST;
    }
    bytes = hero_ui_arm_clipboard_bytes(buffer, (int64_t)count);
    if (bytes < 0) {
        return hero_ui_said(HERO_UI_OTHER, "no read of the clipboard has found anything");
    }
    hero_ui_fine();
    return bytes;
}

/* -- BOXES THE SYSTEM DRAWS ------------------------------------------------ */

#if defined(__APPLE__)

int64_t hero_ui_arm_insets(int64_t id, int64_t *left, int64_t *top, int64_t *right, int64_t *bottom) {
    return hero_ui_appkit_insets(id, left, top, right, bottom);
}

#endif

int64_t hero_ui_insets(hero_ui_app *app, int64_t id, int64_t *left, int64_t *top, int64_t *right, int64_t *bottom) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (left == NULL || top == NULL || right == NULL || bottom == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "the insets: no room was given for one of the four");
    }
    *left = 0;
    *top = 0;
    *right = 0;
    *bottom = 0;
    return hero_ui_answered(hero_ui_arm_insets(id, left, top, right, bottom), "reading the room a widget draws around its content", id);
}

/* -- A TABLE --------------------------------------------------------------- */

#if defined(__APPLE__)

int64_t hero_ui_arm_set_columns(int64_t id, const uint8_t *titles, int64_t count) {
    return hero_ui_appkit_set_columns(id, titles, count);
}

int64_t hero_ui_arm_set_sort(int64_t id, int64_t column, int64_t ascending) {
    return hero_ui_appkit_set_sort(id, column, ascending);
}

#endif

int64_t hero_ui_set_columns(hero_ui_app *app, int64_t id, const void *titles, uint64_t count) {
    const uint8_t *bytes = titles;
    uint64_t columns = 0;
    char words[HERO_UI_WORDS_MOST];

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_text_given(titles, count, "the columns' titles", true)) {
        return -1;
    }
    for (uint64_t at = 0; at < count; at += 1) {
        if (bytes[at] == '\n') {
            return hero_ui_said(HERO_UI_INVALID, "a column's title holds a line feed, and a header shows one line");
        }
        if (bytes[at] == 0) {
            columns += 1;
        }
    }
    if (columns < 1 || columns > (uint64_t)HERO_UI_COLUMNS_MOST) {
        (void)snprintf(words, sizeof words, "a table of %llu columns; it holds 1 to %d", (unsigned long long)columns, HERO_UI_COLUMNS_MOST);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    return hero_ui_windows_answered(hero_ui_arm_set_columns(id, bytes, (int64_t)count), "giving a table its columns", id);
}

int64_t hero_ui_set_sort(hero_ui_app *app, int64_t id, int64_t column, int64_t ascending) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (column < -1 || column >= HERO_UI_COLUMNS_MOST || (ascending != 0 && ascending != 1)) {
        return hero_ui_said(HERO_UI_INVALID, "a sort is a column from -1, none, and a flag of 0 or 1");
    }
    return hero_ui_windows_answered(hero_ui_arm_set_sort(id, column, ascending), "showing a table's sort", id);
}

/* -- A TREE ---------------------------------------------------------------- */

#if defined(__APPLE__)

int64_t hero_ui_arm_tree_add(int64_t id, int64_t item, int64_t under, const uint8_t *text, int64_t count, int64_t foldable) {
    return hero_ui_appkit_tree_add(id, item, under, text, count, foldable);
}

int64_t hero_ui_arm_tree_remove(int64_t id, int64_t item) {
    return hero_ui_appkit_tree_remove(id, item);
}

int64_t hero_ui_arm_tree_unfold(int64_t id, int64_t item, int64_t on) {
    return hero_ui_appkit_tree_unfold(id, item, on);
}

#endif

int64_t hero_ui_tree_add(hero_ui_app *app, int64_t id, int64_t item, int64_t under, const void *text, uint64_t count, int64_t foldable) {
    const uint8_t *bytes = text;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_text_given(text, count, "an item's text", false)) {
        return -1;
    }
    if (item < 1 || under < 0 || (foldable != 0 && foldable != 1)) {
        return hero_ui_said(HERO_UI_INVALID, "a tree's item is 1 or more, the item it stands under 0 or more, and foldable 0 or 1");
    }
    for (uint64_t at = 0; at < count; at += 1) {
        if (bytes[at] == '\n' || bytes[at] == 0) {
            return hero_ui_said(HERO_UI_INVALID, "an item's text holds a line feed or a zero byte, and an item shows one line");
        }
    }
    return hero_ui_windows_answered(hero_ui_arm_tree_add(id, item, under, bytes, (int64_t)count, foldable), "adding a tree's item", id);
}

int64_t hero_ui_tree_remove(hero_ui_app *app, int64_t id, int64_t item) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (item < 1) {
        return hero_ui_said(HERO_UI_INVALID, "a tree's item is 1 or more");
    }
    return hero_ui_windows_answered(hero_ui_arm_tree_remove(id, item), "taking a tree's item out", id);
}

int64_t hero_ui_tree_unfold(hero_ui_app *app, int64_t id, int64_t item, int64_t on) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (item < 1 || (on != 0 && on != 1)) {
        return hero_ui_said(HERO_UI_INVALID, "a tree's item is 1 or more and the state 0 or 1");
    }
    return hero_ui_windows_answered(hero_ui_arm_tree_unfold(id, item, on), "unfolding a tree's item", id);
}

/* -- AN ICON IN THE TRAY, AND AN APP OUT OF THE DOCK ------------------------ */

#if defined(__APPLE__)

int64_t hero_ui_arm_set_docked(int64_t on) {
    return hero_ui_appkit_set_docked(on);
}

int64_t hero_ui_arm_notify(const uint8_t *title, int64_t title_count, const uint8_t *text, int64_t text_count) {
    return hero_ui_appkit_notify(title, title_count, text, text_count);
}

int64_t hero_ui_arm_system_words(char *words, int64_t count) {
    return hero_ui_appkit_system_words(words, count);
}

int64_t hero_ui_arm_appearance(int64_t *appearance) {
    return hero_ui_appkit_appearance(appearance);
}

int64_t hero_ui_arm_appearance_set(int64_t appearance) {
    return hero_ui_appkit_appearance_set(appearance);
}

int64_t hero_ui_arm_set_accessible_name(int64_t id, const uint8_t *name, int64_t count) {
    return hero_ui_appkit_set_accessible_name(id, name, count);
}

int64_t hero_ui_arm_accessible_name(int64_t id, uint8_t *buffer, int64_t count, int64_t *length) {
    return hero_ui_appkit_accessible_name(id, buffer, count, length);
}

int64_t hero_ui_arm_set_label_for(int64_t id, int64_t label) {
    return hero_ui_appkit_set_label_for(id, label);
}

int64_t hero_ui_arm_answer_quit(int64_t go) {
    return hero_ui_appkit_answer_quit(go);
}

int64_t hero_ui_arm_ask_quit(int64_t session_end) {
    return hero_ui_appkit_ask_quit(session_end);
}

int64_t hero_ui_arm_set_drop(int64_t id, int64_t on) {
    return hero_ui_appkit_set_drop(id, on);
}

int64_t hero_ui_arm_drop(int64_t id, int64_t x, int64_t y, const uint8_t *paths, int64_t count) {
    return hero_ui_appkit_drop(id, x, y, paths, count);
}

int64_t hero_ui_arm_undo(int64_t id, int64_t which, int64_t *done) {
    return hero_ui_appkit_undo(id, which, done);
}

int64_t hero_ui_arm_can_undo(int64_t id, int64_t which, int64_t *can) {
    return hero_ui_appkit_can_undo(id, which, can);
}

int64_t hero_ui_arm_set_text_selection(int64_t id, int64_t start, int64_t end) {
    return hero_ui_appkit_set_text_selection(id, start, end);
}

int64_t hero_ui_arm_text_selection(int64_t id, int64_t *start, int64_t *end) {
    return hero_ui_appkit_text_selection(id, start, end);
}

int64_t hero_ui_arm_set_identity(const uint8_t *identifier, int64_t identifier_count, const uint8_t *name, int64_t name_count) {
    return hero_ui_appkit_set_identity(identifier, identifier_count, name, name_count);
}

int64_t hero_ui_arm_set_tip(int64_t id, const uint8_t *text, int64_t count) {
    return hero_ui_appkit_set_tip(id, text, count);
}

int64_t hero_ui_arm_tip(int64_t id, uint8_t *buffer, int64_t count, int64_t *length) {
    return hero_ui_appkit_tip(id, buffer, count, length);
}

int64_t hero_ui_arm_open_items(int64_t what, const uint8_t *items, int64_t count) {
    return hero_ui_appkit_open_items(what, items, count);
}

/* ONE INSTANCE, on macOS: a lock file and a local socket in the user's own
 * temporary folder (`_CS_DARWIN_USER_TEMP_DIR`, which no other user reads),
 * both named by a hash of the identity, since a socket's path holds 104
 * bytes. The first process to lock the file holds the claim and listens; a
 * later one connects and hands its arguments over, the first answering one
 * byte once it has them. The listener is read on the main queue, which the
 * loop's pump runs; each hand-over is read on a queue of its own with a
 * deadline, so a peer that stalls never holds the user interface, and is
 * queued on the main queue as `HERO_UI_EVENT_OPEN`. A hand-over read after
 * the claim ended is dropped. */

/* The words of the claim's last refusal, which `hero_ui_claim_answered`
 * tells: the macOS arm's own, its other words being its members'. */
static _Thread_local char hero_ui_claim_words[HERO_UI_WORDS_MOST];

/* `kind`, its words `words` kept for the entry point. */
static int64_t hero_ui_claim_refused(int64_t kind, const char *words) {
    (void)hero_ui_copy_text(hero_ui_claim_words, sizeof hero_ui_claim_words, words);
    return kind;
}

/* The hand-over's first bytes. */
static const char hero_ui_hand_magic[8] = {'H', 'E', 'R', 'O', 'U', 'I', '1', '\0'};

/* The seconds a hand-over's peer has for each read and write. */
#define HERO_UI_HAND_SECONDS 5

static int hero_ui_instance_lock = -1;
static int hero_ui_instance_listener = -1;
static dispatch_source_t hero_ui_instance_source;
/* Counted up at each claim's end, so a hand-over read for an earlier claim
 * is dropped. */
static uint64_t hero_ui_instance_era;

/* One hand-over being read: the connection, the identity it must name, the
 * claim's era, and what was read. */
typedef struct hero_ui_hand {
    int socket;
    uint64_t era;
    char identity[HERO_UI_IDENTITY_MOST + 1];
    uint8_t *arguments;
    uint32_t count;
} hero_ui_hand;

/* The path of the claim's file ending `suffix` for `identity` into `path`
 * (`most` bytes); false where the folder is unknown or the path too long. */
static bool hero_ui_instance_path(const char *identity, const char *suffix, char *path, size_t most) {
    char folder[1024];
    uint64_t hash = 14695981039346656037ULL;
    size_t length = confstr(_CS_DARWIN_USER_TEMP_DIR, folder, sizeof folder);
    int written;

    if (length == 0 || length > sizeof folder) {
        return false;
    }
    for (const char *at = identity; *at != '\0'; at += 1) {
        hash = (hash ^ (uint8_t)*at) * 1099511628211ULL;
    }
    written = snprintf(path, most, "%s%shero-ui-%016llx%s", folder, folder[length - 2] == '/' ? "" : "/", (unsigned long long)hash, suffix);
    return written > 0 && (size_t)written < most;
}

/* Reads or writes all `count` bytes at `bytes` on `socket`, its deadline its
 * own; false at its end, a failure or the deadline. */
static bool hero_ui_whole(int socket, void *bytes, size_t count, bool writing) {
    uint8_t *at = bytes;

    while (count > 0) {
        ssize_t done = writing ? send(socket, at, count, 0) : recv(socket, at, count, 0);

        if (done < 0 && errno == EINTR) {
            continue;
        }
        if (done <= 0) {
            return false;
        }
        at += done;
        count -= (size_t)done;
    }
    return true;
}

/* Gives `socket` a deadline of `HERO_UI_HAND_SECONDS` for each read and
 * write, no `SIGPIPE`, and no life in a child process. */
static void hero_ui_hand_socket(int socket) {
    struct timeval limit = {HERO_UI_HAND_SECONDS, 0};
    int on = 1;

    (void)fcntl(socket, F_SETFD, FD_CLOEXEC);
    (void)setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &limit, sizeof limit);
    (void)setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, &limit, sizeof limit);
    (void)setsockopt(socket, SOL_SOCKET, SO_NOSIGPIPE, &on, sizeof on);
}

/* Connects to the claim's socket at `path` and hands `arguments` over,
 * named for `identity`: true once the first process answered it has them.
 * Tries for two seconds, the first process perhaps not listening yet. */
static bool hero_ui_hand_to(const char *path, const char *identity, const uint8_t *arguments, uint32_t count) {
    struct sockaddr_un address;
    uint8_t length[4] = {(uint8_t)count, (uint8_t)(count >> 8), (uint8_t)(count >> 16), (uint8_t)(count >> 24)};
    uint8_t answer = 0;

    memset(&address, 0, sizeof address);
    address.sun_family = AF_UNIX;
    (void)snprintf(address.sun_path, sizeof address.sun_path, "%s", path);
    for (int tries = 0; tries < 20; tries += 1) {
        int socket_id = socket(AF_UNIX, SOCK_STREAM, 0);
        bool handed;

        if (socket_id < 0) {
            return false;
        }
        hero_ui_hand_socket(socket_id);
        if (connect(socket_id, (struct sockaddr *)&address, sizeof address) != 0) {
            (void)close(socket_id);
            (void)usleep(100000);
            continue;
        }
        handed = hero_ui_whole(socket_id, (void *)hero_ui_hand_magic, sizeof hero_ui_hand_magic, true) && hero_ui_whole(socket_id, (void *)identity, strlen(identity) + 1, true) && hero_ui_whole(socket_id, length, sizeof length, true) && (count == 0 || hero_ui_whole(socket_id, (void *)arguments, count, true)) && hero_ui_whole(socket_id, &answer, 1, false) && answer == '1';
        (void)close(socket_id);
        return handed;
    }
    return false;
}

/* On the main queue: queues a hand-over read for the claim still held. */
static void hero_ui_hand_told(void *context) {
    hero_ui_hand *hand = context;
    int64_t items = 0;

    if (hand->era == hero_ui_instance_era && hero_ui_instance_listener >= 0) {
        for (uint32_t at = 0; at < hand->count; at += 1) {
            if (hand->arguments[at] == 0) {
                items += 1;
            }
        }
        hero_ui_dialog_told(HERO_UI_EVENT_OPEN, 0, items, HERO_UI_OPEN_ARGUMENTS, hand->arguments, (int64_t)hand->count);
    }
    free(hand->arguments);
    free(hand);
}

/* On a queue of its own: reads one hand-over, answers its peer, and hands
 * it to the main queue where it holds a list of UTF-8 arguments for this
 * identity. */
static void hero_ui_hand_read(void *context) {
    hero_ui_hand *hand = context;
    char magic[sizeof hero_ui_hand_magic];
    char named[HERO_UI_IDENTITY_MOST + 1];
    uint8_t length[4];
    size_t at = 0;
    bool good = hero_ui_whole(hand->socket, magic, sizeof magic, false) && memcmp(magic, hero_ui_hand_magic, sizeof magic) == 0;

    while (good && at < sizeof named) {
        good = hero_ui_whole(hand->socket, named + at, 1, false);
        if (good && named[at] == '\0') {
            break;
        }
        at += 1;
    }
    good = good && at < sizeof named && strcmp(named, hand->identity) == 0 && hero_ui_whole(hand->socket, length, sizeof length, false);
    if (good) {
        hand->count = (uint32_t)length[0] | ((uint32_t)length[1] << 8) | ((uint32_t)length[2] << 16) | ((uint32_t)length[3] << 24);
        good = hand->count <= HERO_UI_TEXT_MOST;
    }
    if (good && hand->count > 0) {
        hand->arguments = malloc(hand->count);
        good = hand->arguments != NULL && hero_ui_whole(hand->socket, hand->arguments, hand->count, false);
        if (good) {
            uint32_t start = 0;

            good = hand->arguments[hand->count - 1] == 0;
            for (uint32_t i = 0; good && i < hand->count; i += 1) {
                if (hand->arguments[i] == 0) {
                    good = hero_ui_is_utf8(hand->arguments + start, i - start);
                    start = i + 1;
                }
            }
        }
    }
    if (good) {
        uint8_t answer = '1';

        good = hero_ui_whole(hand->socket, &answer, 1, true);
    }
    (void)close(hand->socket);
    if (!good) {
        free(hand->arguments);
        free(hand);
        return;
    }
    dispatch_async_f(dispatch_get_main_queue(), hand, hero_ui_hand_told);
}

/* On the main queue, the listener readable: takes each connection waiting
 * from a process of this user, and reads it on a queue of its own. */
static void hero_ui_instance_accept(void *context) {
    (void)context;
    for (;;) {
        int socket_id = accept(hero_ui_instance_listener, NULL, NULL);
        uid_t user;
        gid_t group;
        hero_ui_hand *hand;

        if (socket_id < 0) {
            return;
        }
        hero_ui_hand_socket(socket_id);
        (void)fcntl(socket_id, F_SETFL, fcntl(socket_id, F_GETFL) & ~O_NONBLOCK);
        hand = calloc(1, sizeof *hand);
        if (hand == NULL || getpeereid(socket_id, &user, &group) != 0 || user != geteuid()) {
            free(hand);
            (void)close(socket_id);
            continue;
        }
        hand->socket = socket_id;
        hand->era = hero_ui_instance_era;
        (void)snprintf(hand->identity, sizeof hand->identity, "%s", hero_ui_identity);
        dispatch_async_f(dispatch_get_global_queue(QOS_CLASS_UTILITY, 0), hand, hero_ui_hand_read);
    }
}

static void hero_ui_instance_closed(void *context) {
    (void)close((int)(intptr_t)context);
}

int64_t hero_ui_arm_claim_instance(const uint8_t *identifier, int64_t identifier_count, const uint8_t *arguments, int64_t count, int64_t *first) {
    char identity[HERO_UI_IDENTITY_MOST + 1];
    char lock_path[1100];
    struct sockaddr_un address;
    int lock;
    int listener;

    if (hero_ui_instance_listener >= 0) {
        *first = 1;
        return HERO_UI_OK;
    }
    memcpy(identity, identifier, (size_t)identifier_count);
    identity[identifier_count] = '\0';
    memset(&address, 0, sizeof address);
    address.sun_family = AF_UNIX;
    if (!hero_ui_instance_path(identity, ".lock", lock_path, sizeof lock_path) || !hero_ui_instance_path(identity, ".sock", address.sun_path, sizeof address.sun_path)) {
        return hero_ui_claim_refused(HERO_UI_UNSUPPORTED, "the user's temporary folder is unknown, or its path leaves a local socket's no room");
    }
    lock = open(lock_path, O_RDWR | O_CREAT | O_CLOEXEC, 0600);
    if (lock < 0) {
        return hero_ui_claim_refused(HERO_UI_OTHER, "the claim's lock file could not be opened in the user's temporary folder");
    }
    if (flock(lock, LOCK_EX | LOCK_NB) != 0) {
        (void)close(lock);
        if (errno != EWOULDBLOCK) {
            return hero_ui_claim_refused(HERO_UI_OTHER, "the claim's lock file could not be locked");
        }
        if (!hero_ui_hand_to(address.sun_path, identity, arguments, (uint32_t)count)) {
            return hero_ui_claim_refused(HERO_UI_OTHER, "another process holds this identity's claim and took no hand-over within its deadline");
        }
        *first = 0;
        return HERO_UI_OK;
    }
    (void)unlink(address.sun_path);
    listener = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listener < 0 || fcntl(listener, F_SETFD, FD_CLOEXEC) != 0 || bind(listener, (struct sockaddr *)&address, sizeof address) != 0 || listen(listener, 16) != 0 || fcntl(listener, F_SETFL, O_NONBLOCK) != 0) {
        if (listener >= 0) {
            (void)close(listener);
        }
        (void)close(lock);
        return hero_ui_claim_refused(HERO_UI_OTHER, "the claim's local socket could not listen in the user's temporary folder");
    }
    hero_ui_instance_source = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ, (uintptr_t)listener, 0, dispatch_get_main_queue());
    if (hero_ui_instance_source == NULL) {
        (void)close(listener);
        (void)unlink(address.sun_path);
        (void)close(lock);
        return hero_ui_claim_refused(HERO_UI_LIMIT, "no memory for the claim's listener");
    }
    hero_ui_instance_lock = lock;
    hero_ui_instance_listener = listener;
    dispatch_set_context(hero_ui_instance_source, (void *)(intptr_t)listener);
    dispatch_source_set_event_handler_f(hero_ui_instance_source, hero_ui_instance_accept);
    dispatch_source_set_cancel_handler_f(hero_ui_instance_source, hero_ui_instance_closed);
    dispatch_resume(hero_ui_instance_source);
    *first = 1;
    return HERO_UI_OK;
}

/* On a queue of its own: hands this process's own claim the arguments a
 * later launch would, for `hero_ui_inject`. */
typedef struct hero_ui_own_hand {
    char path[sizeof(((struct sockaddr_un *)0)->sun_path)];
    char identity[HERO_UI_IDENTITY_MOST + 1];
    uint8_t *arguments;
    uint32_t count;
} hero_ui_own_hand;

static void hero_ui_hand_own(void *context) {
    hero_ui_own_hand *own = context;

    (void)hero_ui_hand_to(own->path, own->identity, own->arguments, own->count);
    free(own->arguments);
    free(own);
}

int64_t hero_ui_arm_hand_over(const uint8_t *arguments, int64_t count) {
    hero_ui_own_hand *own;

    if (hero_ui_instance_listener < 0) {
        return hero_ui_claim_refused(HERO_UI_INVALID, "this process holds no claim of its identity to hand arguments to");
    }
    own = calloc(1, sizeof *own);
    if (own == NULL || (count > 0 && (own->arguments = malloc((size_t)count)) == NULL)) {
        if (own != NULL) {
            free(own);
        }
        return HERO_UI_LIMIT;
    }
    if (!hero_ui_instance_path(hero_ui_identity, ".sock", own->path, sizeof own->path)) {
        free(own->arguments);
        free(own);
        return hero_ui_claim_refused(HERO_UI_OTHER, "the claim's local socket has no path");
    }
    (void)snprintf(own->identity, sizeof own->identity, "%s", hero_ui_identity);
    if (count > 0) {
        memcpy(own->arguments, arguments, (size_t)count);
    }
    own->count = (uint32_t)count;
    dispatch_async_f(dispatch_get_global_queue(QOS_CLASS_UTILITY, 0), own, hero_ui_hand_own);
    return HERO_UI_OK;
}

void hero_ui_arm_instance_end(void) {
    char path[sizeof(((struct sockaddr_un *)0)->sun_path)];

    if (hero_ui_instance_listener < 0) {
        return;
    }
    hero_ui_instance_era += 1;
    if (hero_ui_instance_path(hero_ui_identity, ".sock", path, sizeof path)) {
        (void)unlink(path);
    }
    dispatch_source_cancel(hero_ui_instance_source);
    dispatch_release(hero_ui_instance_source);
    hero_ui_instance_source = NULL;
    hero_ui_instance_listener = -1;
    (void)close(hero_ui_instance_lock);
    hero_ui_instance_lock = -1;
}

#elif defined(_WIN32)

/* The words of the last refusal of a call on the system around the
 * program, for the Win32 arm: the words its own member's refusals leave
 * (`hero_ui_refused`). The tray, a notification, the appearance and
 * accessible names are the Win32 member's own; the GTK 4 arm defines all of
 * them and these words in its member. */
int64_t hero_ui_arm_system_words(char *words, int64_t count) {
    return hero_ui_arm_words(words, count);
}

#endif

/* CALLS NO C ARM DRAWS YET: each refuses in words of its own, kept where the
 * shared words are (`hero_ui_refused`), so no word another call left is told
 * with it. An arm that draws one defines it in its own member, and its system
 * leaves this block's condition. */

#if defined(_WIN32)

/* Undo and the selection in text widgets, the app's identity, its one
 * instance, documents from the system and a tip over a widget: the Win32
 * arm's refusals; the GTK 4 arm draws them. */

int64_t hero_ui_arm_undo(int64_t id, int64_t which, int64_t *done) {
    (void)id;
    (void)which;
    (void)done;
    return hero_ui_refused(HERO_UI_UNSUPPORTED, "the layer undoes a text widget's edit on macOS alone, and this system's arm has no such call");
}

int64_t hero_ui_arm_can_undo(int64_t id, int64_t which, int64_t *can) {
    (void)id;
    (void)which;
    (void)can;
    return hero_ui_refused(HERO_UI_UNSUPPORTED, "the layer asks whether a text widget's edit can be undone on macOS alone, and this system's arm has no such call");
}

int64_t hero_ui_arm_set_text_selection(int64_t id, int64_t start, int64_t end) {
    (void)id;
    (void)start;
    (void)end;
    return hero_ui_refused(HERO_UI_UNSUPPORTED, "the layer selects a text widget's text on macOS alone, and this system's arm has no such call");
}

int64_t hero_ui_arm_text_selection(int64_t id, int64_t *start, int64_t *end) {
    (void)id;
    (void)start;
    (void)end;
    return hero_ui_refused(HERO_UI_UNSUPPORTED, "the layer reads a text widget's selection on macOS alone, and this system's arm has no such call");
}

int64_t hero_ui_arm_set_identity(const uint8_t *identifier, int64_t identifier_count, const uint8_t *name, int64_t name_count) {
    (void)identifier;
    (void)identifier_count;
    (void)name;
    (void)name_count;
    return hero_ui_refused(HERO_UI_UNSUPPORTED, "the layer gives the system the app's identity on macOS alone, and this system's arm has no such call");
}

int64_t hero_ui_arm_claim_instance(const uint8_t *identifier, int64_t identifier_count, const uint8_t *arguments, int64_t count, int64_t *first) {
    (void)identifier;
    (void)identifier_count;
    (void)arguments;
    (void)count;
    (void)first;
    return hero_ui_refused(HERO_UI_UNSUPPORTED, "the layer keeps one instance of an app on macOS alone, and this system's arm has no such call");
}

int64_t hero_ui_arm_hand_over(const uint8_t *arguments, int64_t count) {
    (void)arguments;
    (void)count;
    return hero_ui_refused(HERO_UI_UNSUPPORTED, "the layer hands a later launch's arguments to the first on macOS alone, and this system's arm has no such call");
}

int64_t hero_ui_arm_open_items(int64_t what, const uint8_t *items, int64_t count) {
    (void)what;
    (void)items;
    (void)count;
    return hero_ui_refused(HERO_UI_UNSUPPORTED, "the layer asks the app to open documents as the system would on macOS alone, and this system's arm has no such call");
}

void hero_ui_arm_instance_end(void) {
}

int64_t hero_ui_arm_set_tip(int64_t id, const uint8_t *text, int64_t count) {
    (void)id;
    (void)text;
    (void)count;
    return hero_ui_refused(HERO_UI_UNSUPPORTED, "the layer shows a tip over a widget on macOS alone, and this system's arm has no such call");
}

int64_t hero_ui_arm_tip(int64_t id, uint8_t *buffer, int64_t count, int64_t *length) {
    (void)id;
    (void)buffer;
    (void)count;
    (void)length;
    return hero_ui_refused(HERO_UI_UNSUPPORTED, "the layer reads a widget's tip on macOS alone, and this system's arm has no such call");
}

#endif

int64_t hero_ui_set_docked(hero_ui_app *app, int64_t on) {
    if (hero_ui_enter_native(app) == NULL) {
        return -1;
    }
    if (on != 0 && on != 1) {
        return hero_ui_said(HERO_UI_INVALID, "whether the app stands in the Dock is 0 or 1");
    }
    return hero_ui_answered(hero_ui_arm_set_docked(on), "keeping the app in the Dock or out of it", 0);
}

/* 0 where the arm answered `kind` `HERO_UI_OK`, else -1 with the failure
 * said as `what`, in the words of the member that answered it, naming
 * widget `id` where it is 1 or more. */
static int64_t hero_ui_system_answered(int64_t kind, const char *what, int64_t id) {
    char words[HERO_UI_WORDS_MOST];
    char theirs[HERO_UI_WORDS_MOST];

    if (kind == HERO_UI_UNSUPPORTED || kind == HERO_UI_OTHER) {
        theirs[0] = '\0';
        if (hero_ui_arm_system_words(theirs, (int64_t)sizeof theirs) <= 0) {
            (void)snprintf(theirs, sizeof theirs, "the system refused, giving no words");
        }
        (void)snprintf(words, sizeof words, "%s: %s", what, theirs);
        return hero_ui_said(kind, words);
    }
    return hero_ui_answered(kind, what, id);
}

int64_t hero_ui_notify(hero_ui_app *app, const void *title, uint64_t title_count, const void *text, uint64_t text_count) {
    const uint8_t *heading = title;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_text_given(title, title_count, "the title", false) || !hero_ui_text_given(text, text_count, "the text", false)) {
        return -1;
    }
    if (title_count == 0 || memchr(heading, '\n', (size_t)title_count) != NULL) {
        return hero_ui_said(HERO_UI_INVALID, "a notification's title is one line, not empty");
    }
    return hero_ui_system_answered(hero_ui_arm_notify(title, (int64_t)title_count, text, (int64_t)text_count), "showing a notification", 0);
}

int64_t hero_ui_appearance(hero_ui_app *app, int64_t *appearance) {
    if (hero_ui_enter_native(app) == NULL) {
        return -1;
    }
    if (appearance == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "no place was given for the appearance");
    }
    return hero_ui_system_answered(hero_ui_arm_appearance(appearance), "asking the appearance", 0);
}

/* -- ACCESSIBLE NAMES -------------------------------------------------------- */

int64_t hero_ui_set_accessible_name(hero_ui_app *app, int64_t id, const void *name, uint64_t count) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_text_given(name, count, "the accessible name", false)) {
        return -1;
    }
    if (count > 0 && memchr(name, '\n', (size_t)count) != NULL) {
        return hero_ui_said(HERO_UI_INVALID, "an accessible name is one line");
    }
    return hero_ui_system_answered(hero_ui_arm_set_accessible_name(id, name, (int64_t)count), "naming a widget for a screen reader", id);
}

int64_t hero_ui_accessible_name(hero_ui_app *app, int64_t id, void *buffer, uint64_t count) {
    int64_t length = 0;
    int64_t kind;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (count > 0 && buffer == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "room was named for the name and none given");
    }
    if (count > (uint64_t)HERO_UI_TEXT_MOST) {
        count = (uint64_t)HERO_UI_TEXT_MOST;
    }
    kind = hero_ui_arm_accessible_name(id, buffer, (int64_t)count, &length);
    if (hero_ui_system_answered(kind, "reading a widget's accessible name", id) != 0) {
        return -1;
    }
    return length;
}

int64_t hero_ui_set_label_for(hero_ui_app *app, int64_t id, int64_t label) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (label < 0 || label == id) {
        return hero_ui_said(HERO_UI_INVALID, "the label tied to a widget is another widget, 1 or more, or 0 for none");
    }
    return hero_ui_system_answered(hero_ui_arm_set_label_for(id, label), "tying a label to its widget", id);
}

/* -- THE SESSION'S END ASKING THE PROGRAM ----------------------------------- */

/* The answer `kind` of a call on the system around the program that no C
 * arm draws yet, as `what`: on macOS in the system member's words, which
 * answered it; elsewhere in the words its refusal left (`hero_ui_refused`),
 * whatever words an arm's own `system_words` keeps. */
static int64_t hero_ui_session_answered(int64_t kind, const char *what) {
#if defined(__APPLE__)
    return hero_ui_system_answered(kind, what, 0);
#else
    return hero_ui_answered(kind, what, 0);
#endif
}

int64_t hero_ui_answer_quit(hero_ui_app *app, int64_t go) {
    if (hero_ui_enter_native(app) == NULL) {
        return -1;
    }
    if (go != 0 && go != 1) {
        return hero_ui_said(HERO_UI_INVALID, "whether the app ends is 0 or 1");
    }
    return hero_ui_session_answered(hero_ui_arm_answer_quit(go), "answering the system's ask to end the app");
}

/* -- FILES DROPPED ON A WINDOW ---------------------------------------------- */

int64_t hero_ui_set_drop(hero_ui_app *app, int64_t id, int64_t on) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (on != 0 && on != 1) {
        return hero_ui_said(HERO_UI_INVALID, "whether a window takes dropped files is 0 or 1");
    }
    return hero_ui_answered(hero_ui_arm_set_drop(id, on), "having a window take dropped files", id);
}

/* `hero_ui_inject`'s `HERO_UI_EVENT_DROP`: drops the paths `paths` holds,
 * each followed by one zero byte, on window `id` at `x`, `y`. */
static int64_t hero_ui_drop_as_person(int64_t id, int64_t x, int64_t y, const void *paths, uint64_t count) {
    const uint8_t *bytes = paths;

    if (!hero_ui_id_given(id) || !hero_ui_text_given(paths, count, "the paths dropped", true)) {
        return -1;
    }
    if (count == 0) {
        return hero_ui_said(HERO_UI_INVALID, "a drop holds one path or more");
    }
    for (uint64_t at = 0; at < count; at += 1) {
        if (bytes[at] == 0 && (at == 0 || bytes[at - 1] == 0)) {
            return hero_ui_said(HERO_UI_INVALID, "a path dropped is empty");
        }
    }
    if (x < 0 || y < 0 || x > HERO_UI_POSITION_MOST || y > HERO_UI_POSITION_MOST) {
        return hero_ui_said(HERO_UI_INVALID, "a drop's point is 0 or more in the window's content");
    }
    return hero_ui_answered(hero_ui_arm_drop(id, x, y, bytes, (int64_t)count), "dropping files on a window as the person", id);
}

/* -- UNDO AND THE SELECTION IN TEXT WIDGETS --------------------------------- */

int64_t hero_ui_undo(hero_ui_app *app, int64_t id, int64_t which) {
    int64_t done = 0;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (which != HERO_UI_UNDO && which != HERO_UI_REDO) {
        return hero_ui_said(HERO_UI_INVALID, "an undo is HERO_UI_UNDO or HERO_UI_REDO");
    }
    if (hero_ui_answered(hero_ui_arm_undo(id, which, &done), which == HERO_UI_UNDO ? "undoing a text widget's edit" : "redoing a text widget's edit", id) != 0) {
        return -1;
    }
    return done == 1 ? 1 : 0;
}

int64_t hero_ui_can_undo(hero_ui_app *app, int64_t id, int64_t which) {
    int64_t can = 0;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (which != HERO_UI_UNDO && which != HERO_UI_REDO) {
        return hero_ui_said(HERO_UI_INVALID, "an undo is HERO_UI_UNDO or HERO_UI_REDO");
    }
    if (hero_ui_answered(hero_ui_arm_can_undo(id, which, &can), "asking whether a text widget's edit can be undone", id) != 0) {
        return -1;
    }
    return can == 1 ? 1 : 0;
}

int64_t hero_ui_set_text_selection(hero_ui_app *app, int64_t id, int64_t start, int64_t end) {
    char words[HERO_UI_WORDS_MOST];

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (start < 0 || end < start || end > HERO_UI_TEXT_MOST) {
        (void)snprintf(words, sizeof words, "a selection from byte %lld to byte %lld; it is 0 or more, its start at most its end", (long long)start, (long long)end);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    return hero_ui_answered(hero_ui_arm_set_text_selection(id, start, end), "selecting a text widget's text", id);
}

int64_t hero_ui_text_selection(hero_ui_app *app, int64_t id, int64_t *start, int64_t *end) {
    int64_t from = 0;
    int64_t to = 0;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (start == NULL || end == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "no place was given for the selection");
    }
    if (hero_ui_answered(hero_ui_arm_text_selection(id, &from, &to), "reading a text widget's selection", id) != 0) {
        return -1;
    }
    *start = from;
    *end = to;
    return 0;
}

/* -- THE APP'S IDENTITY, ONE INSTANCE, AND DOCUMENTS FROM THE SYSTEM -------- */

/* The answer `kind` of the one instance's call `what`: on macOS in the
 * words its refusal left, which are `hero_ui.c`'s own; elsewhere as every
 * call's. */
static int64_t hero_ui_claim_answered(int64_t kind, const char *what) {
#if defined(__APPLE__)
    char words[HERO_UI_WORDS_MOST];

    if (kind != HERO_UI_OK && kind != HERO_UI_LIMIT && hero_ui_claim_words[0] != '\0') {
        (void)snprintf(words, sizeof words, "%s: %s", what, hero_ui_claim_words);
        hero_ui_claim_words[0] = '\0';
        return hero_ui_said(kind, words);
    }
#endif
    return hero_ui_answered(kind, what, 0);
}

/* Whether `identifier` (`count` bytes) is a reverse domain name: two or more
 * parts joined by dots, each ASCII letters, digits and hyphens, the first a
 * letter, at most `HERO_UI_IDENTITY_MOST` bytes. */
static bool hero_ui_identity_ok(const uint8_t *identifier, uint64_t count) {
    int64_t parts = 1;
    bool fresh = true;

    if (count == 0 || count > HERO_UI_IDENTITY_MOST) {
        return false;
    }
    for (uint64_t at = 0; at < count; at += 1) {
        uint8_t c = identifier[at];
        bool letter = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');

        if (c == '.') {
            if (fresh) {
                return false;
            }
            parts += 1;
            fresh = true;
        } else if (fresh) {
            if (!letter) {
                return false;
            }
            fresh = false;
        } else if (!letter && !(c >= '0' && c <= '9') && c != '-') {
            return false;
        }
    }
    return !fresh && parts >= 2;
}

int64_t hero_ui_set_identity(hero_ui_app *app, const void *identifier, uint64_t identifier_count, const void *name, uint64_t name_count) {
    char words[HERO_UI_WORDS_MOST];
    char theirs[HERO_UI_WORDS_MOST];
    int64_t kind;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_text_given(identifier, identifier_count, "the identifier", false) || !hero_ui_text_given(name, name_count, "the name", false)) {
        return -1;
    }
    if (!hero_ui_identity_ok(identifier, identifier_count)) {
        return hero_ui_said(HERO_UI_INVALID, "an app's identifier is a reverse domain name: two or more parts joined by dots, each ASCII letters, digits and hyphens, the first a letter, at most 128 bytes");
    }
    if (name_count == 0 || memchr(name, '\n', (size_t)name_count) != NULL) {
        return hero_ui_said(HERO_UI_INVALID, "an app's name is one line, not empty");
    }
    if (hero_ui_identity_count > 0 || hero_ui_windowed) {
        return hero_ui_said(HERO_UI_INVALID, "an app is given its identity once, before its first window");
    }
    kind = hero_ui_arm_set_identity(identifier, (int64_t)identifier_count, name, (int64_t)name_count);
    /* An identity the system refuses as given (another bundle's) is told
     * in the arm's words. */
    if (kind == HERO_UI_INVALID) {
        theirs[0] = '\0';
        if (hero_ui_arm_words(theirs, (int64_t)sizeof theirs) <= 0) {
            (void)snprintf(theirs, sizeof theirs, "the system refused it, giving no words");
        }
        (void)snprintf(words, sizeof words, "giving the app its identity: %s", theirs);
        return hero_ui_said(HERO_UI_INVALID, words);
    }
    if (hero_ui_answered(kind, "giving the app its identity", 0) != 0) {
        return -1;
    }
    memcpy(hero_ui_identity, identifier, (size_t)identifier_count);
    hero_ui_identity[identifier_count] = '\0';
    hero_ui_identity_count = (int64_t)identifier_count;
    return 0;
}

int64_t hero_ui_claim_instance(hero_ui_app *app, const void *arguments, uint64_t count) {
    int64_t first = 0;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_text_given(arguments, count, "the arguments", true)) {
        return -1;
    }
    if (hero_ui_identity_count == 0) {
        return hero_ui_said(HERO_UI_INVALID, "an app claims its one instance once it has its identity (hero_ui_set_identity)");
    }
    if (hero_ui_claim_answered(hero_ui_arm_claim_instance((const uint8_t *)hero_ui_identity, hero_ui_identity_count, arguments, (int64_t)count, &first), "claiming the app's one instance") != 0) {
        return -1;
    }
    return first == 1 ? 1 : 0;
}

/* `hero_ui_inject`'s `HERO_UI_EVENT_OPEN`: hands `value` items of kind
 * `what`, each followed by one zero byte, as the system would. */
static int64_t hero_ui_open_as_system(int64_t value, int64_t what, const void *items, uint64_t count) {
    const uint8_t *bytes = items;
    int64_t texts = 0;

    if (!hero_ui_text_given(items, count, "the items opened", true)) {
        return -1;
    }
    for (uint64_t at = 0; at < count; at += 1) {
        if (bytes[at] == 0) {
            if (what != HERO_UI_OPEN_ARGUMENTS && (at == 0 || bytes[at - 1] == 0)) {
                return hero_ui_said(HERO_UI_INVALID, "a path or an address opened is empty");
            }
            texts += 1;
        }
    }
    if (what < HERO_UI_OPEN_FILES || what > HERO_UI_OPEN_ARGUMENTS || texts != value || (what != HERO_UI_OPEN_ARGUMENTS && value == 0)) {
        return hero_ui_said(HERO_UI_INVALID, "an opening is HERO_UI_OPEN_*, `value` the count of its items, each followed by one zero byte, one or more for files and addresses");
    }
    if (what == HERO_UI_OPEN_ARGUMENTS) {
        return hero_ui_claim_answered(hero_ui_arm_hand_over(bytes, (int64_t)count), "handing a later launch's arguments to the app");
    }
    return hero_ui_session_answered(hero_ui_arm_open_items(what, bytes, (int64_t)count), "asking the app to open documents as the system would");
}

/* -- A TIP OVER A WIDGET ---------------------------------------------------- */

int64_t hero_ui_set_tip(hero_ui_app *app, int64_t id, const void *text, uint64_t count) {
    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id) || !hero_ui_text_given(text, count, "the tip", false)) {
        return -1;
    }
    return hero_ui_answered(hero_ui_arm_set_tip(id, text, (int64_t)count), "giving a widget its tip", id);
}

int64_t hero_ui_tip(hero_ui_app *app, int64_t id, void *buffer, uint64_t count) {
    int64_t length = 0;

    if (hero_ui_enter_native(app) == NULL || !hero_ui_id_given(id)) {
        return -1;
    }
    if (count > 0 && buffer == NULL) {
        return hero_ui_said(HERO_UI_INVALID, "room was named for the tip and none given");
    }
    if (count > (uint64_t)HERO_UI_TEXT_MOST) {
        count = (uint64_t)HERO_UI_TEXT_MOST;
    }
    if (hero_ui_answered(hero_ui_arm_tip(id, buffer, (int64_t)count, &length), "reading a widget's tip", id) != 0) {
        return -1;
    }
    return length;
}
