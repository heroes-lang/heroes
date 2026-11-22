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
 * Swift's and Objective-C's, never C's); the arm of every other system is
 * the one that answers `HERO_UI_UNSUPPORTED`, for the lanes of Win32 and
 * GTK to replace. An arm queues what the person does by the function this
 * file hands it at `open`, `hero_ui_push`.
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
 * Written from the platforms' documentation; no other library's source.
 */

#include "hero_ui.h"

#include "hero_handle_tag.h"
#include "hero_layer_alloc.h"
/* The runtime's own number for a thread, the UI owner's on Windows. */
#include "hero_layer_thread.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__APPLE__)
#include <pthread.h>
#include "hero_ui_appkit.h"
#else
#include <sys/syscall.h>
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

#if defined(__APPLE__)

static int64_t hero_ui_arm_here(void) {
    return hero_ui_appkit_here();
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

#else

/* THE ARM OF A SYSTEM NOT WRITTEN YET: no native side, so `native_here`
 * answers 0, `open` refuses `HERO_UI_NATIVE` with `HERO_UI_UNSUPPORTED`, and
 * no other arm function is ever reached; each answers `UNSUPPORTED` all the
 * same, so a call that did reach one would be a value. */

static int64_t hero_ui_arm_here(void) {
    return 0;
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
        (void)snprintf(words, sizeof words, "%s: this system's native side offers no such thing", what);
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
            (void)hero_ui_said(HERO_UI_UNSUPPORTED, "this machine offers no native user interface to this session");
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
