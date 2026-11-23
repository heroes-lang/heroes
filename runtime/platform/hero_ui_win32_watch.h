/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_ui_win32_watch.h: THE EVENTS THE WIN32 ARM'S WAIT WATCHES (panel 256's
 * R9, provisional; `hero_ui.h`'s `hero_ui_add_set` and `hero_ui_add_mailbox`).
 * A poll set on Windows gives the loop a HANDLE, the event it owns
 * (`hero_poll_descriptor`), and the arm's wait waits on it in the one
 * `MsgWaitForMultipleObjectsEx` that waits for the thread's messages, so a
 * socket's notice, a mailbox's send or the stop ends the wait as a window's
 * message does, and `hero_ui_watch_told(id)` queues the event for the program.
 *
 * ONE SHOT, as `hero_ui_arm.h` says of every arm: an event told is not waited
 * on again until `hero_ui_arm_watch_fd` arms it again, which the common part
 * does at each wait. A handle that stays set, because the program has not
 * read the set or the event queue was full, is therefore told once to each
 * wait and the pump never spins on it.
 *
 * NO HANDLE IS THE ARM'S: it never sets, resets or closes one, it only waits
 * on it, and it is valid for as long as the set is open, which the common
 * part asks before it arms (`hero_ui_watch_open`) and the app's thread, the
 * only one a set of the app's is on, cannot end while the pump runs. A wait
 * that fails anyway (the system's `WAIT_FAILED`) disarms every handle rather
 * than fail again at once, and the common part drops the entry whose set is
 * gone at the next wait.
 *
 * THE FUNCTIONS ARE `static`, for the file that includes this header, and
 * need `HANDLE`, `DWORD`, `MsgWaitForMultipleObjectsEx`,
 * `WaitForSingleObject`, `MAXIMUM_WAIT_OBJECTS`, `hero_ui_refused`,
 * `hero_ui_watch_told` and the `HERO_UI_*` kinds already declared; a test of
 * its logic on another system declares them itself.
 */

#ifndef HERO_UI_WIN32_WATCH_H
#define HERO_UI_WIN32_WATCH_H

/* The most handles `MsgWaitForMultipleObjectsEx` takes: the system's 64 less
 * the place the thread's own input takes. */
#define HERO_UI_WIN_WATCH_MOST 63
_Static_assert(HERO_UI_WIN_WATCH_MOST == MAXIMUM_WAIT_OBJECTS - 1, "the wait takes one handle fewer than the system's most, its input taking one place");

/* One event a set gives, watched as number `id` (0 for a place that is free). */
typedef struct {
    int64_t id;
    HANDLE handle;
    bool armed;
} hero_ui_win_watch;

static _Thread_local hero_ui_win_watch hero_ui_win_watches[HERO_UI_WIN_WATCH_MOST];

/* The handle `fd` carries watched as `id` and armed, or armed again where `id`
 * is watched already. `HERO_UI_INVALID` for a handle or a number that is none
 * and for a number watched on another handle, `HERO_UI_LIMIT` where the places
 * are all taken, the words saying which. */
static int64_t hero_ui_win_watch_arm(int64_t fd, int64_t id) {
    hero_ui_win_watch *free_place = NULL;
    HANDLE handle = (HANDLE)(uintptr_t)fd;

    if (fd <= 0 || id < 1) {
        return hero_ui_refused(HERO_UI_INVALID, "an event watched is a handle of the system and a watch's number 1 or more");
    }
    for (size_t at = 0; at < HERO_UI_WIN_WATCH_MOST; at += 1) {
        hero_ui_win_watch *place = &hero_ui_win_watches[at];

        if (place->id == id) {
            if (place->handle != handle) {
                return hero_ui_refused(HERO_UI_INVALID, "a watch stands on one event, and the number was given another");
            }
            place->armed = true;
            return HERO_UI_OK;
        }
        if (place->id == 0 && free_place == NULL) {
            free_place = place;
        }
    }
    if (free_place == NULL) {
        return hero_ui_refused(HERO_UI_LIMIT, "Windows waits on 63 events at most beside a thread's messages, and the app's wait watches 63 sets already");
    }
    free_place->id = id;
    free_place->handle = handle;
    free_place->armed = true;
    return HERO_UI_OK;
}

/* Watch `id` taken away, every one where `id` is 0. */
static void hero_ui_win_watch_end(int64_t id) {
    for (size_t at = 0; at < HERO_UI_WIN_WATCH_MOST; at += 1) {
        if (hero_ui_win_watches[at].id != 0 && (id == 0 || hero_ui_win_watches[at].id == id)) {
            memset(&hero_ui_win_watches[at], 0, sizeof hero_ui_win_watches[at]);
        }
    }
}

/* Waits at most `ms` milliseconds for a message of the thread or an armed
 * event, and tells the first event that is set (`hero_ui_watch_told`), which it
 * disarms first: its message, or none, is what the caller's loop reads next.
 * An event told that queued nothing, because the set held nothing ready, has
 * been reset by the set's own question, and stays armed for the rest of the
 * wait; one still set is not waited on again, so a set that stays ready is told
 * once to each wait and the pump never spins. */
static void hero_ui_win_watch_wait(DWORD ms) {
    HANDLE handles[HERO_UI_WIN_WATCH_MOST];
    int64_t ids[HERO_UI_WIN_WATCH_MOST];
    DWORD count = 0;
    DWORD woke;

    for (size_t at = 0; at < HERO_UI_WIN_WATCH_MOST; at += 1) {
        if (hero_ui_win_watches[at].id != 0 && hero_ui_win_watches[at].armed) {
            handles[count] = hero_ui_win_watches[at].handle;
            ids[count] = hero_ui_win_watches[at].id;
            count += 1;
        }
    }
    woke = MsgWaitForMultipleObjectsEx(count, count == 0 ? NULL : handles, ms, QS_ALLINPUT, MWMO_INPUTAVAILABLE);
    if (woke < count) {
        int64_t told = ids[woke];

        for (size_t at = 0; at < HERO_UI_WIN_WATCH_MOST; at += 1) {
            if (hero_ui_win_watches[at].id == told) {
                hero_ui_win_watches[at].armed = false;
            }
        }
        if (!hero_ui_watch_told(told) && WaitForSingleObject(handles[woke], 0) != WAIT_OBJECT_0) {
            for (size_t at = 0; at < HERO_UI_WIN_WATCH_MOST; at += 1) {
                if (hero_ui_win_watches[at].id == told) {
                    hero_ui_win_watches[at].armed = true;
                }
            }
        }
    } else if (woke == WAIT_FAILED) {
        for (size_t at = 0; at < HERO_UI_WIN_WATCH_MOST; at += 1) {
            hero_ui_win_watches[at].armed = false;
        }
    }
}

#endif
