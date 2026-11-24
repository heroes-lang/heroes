/* The C the case of feature 2690 calls: the runtime's signal door, readable
 * while a counted signal is not taken, waited on by a set of `hero_poll.h`
 * and by `hero_proc.h`'s wait for a child. The interrupt is counted by the
 * platform layer (`hero_net_count_signal`) and sent by the stop cases' own C,
 * `the-platform-layer-stop.h`: SIGINT to the process on POSIX, the console's
 * Ctrl-C on Windows, in a console of the program's own. Each function answers
 * 1 where every step held, or where the platform waits on none (Windows,
 * whose readiness sets take sockets alone, answers `HERO_POLL_UNSUPPORTED`),
 * else the number of the step that did not. A wait lasts at most 5 s, so the
 * program ends either way. */
#ifndef THE_SIGNAL_DOOR_H
#define THE_SIGNAL_DOOR_H

#include "hero_os.h"
#include "hero_poll.h"
#include "hero_proc.h"
#include "the-platform-layer-stop.h"

#include <stdint.h>

#if defined(_WIN32)
#include <windows.h>
#endif

#define DOOR_PATIENCE_NS INT64_C(5000000000)
#define DOOR_EVENTS (4 * HERO_POLL_EVENT_NUMBERS)

/* The interrupt at its default, then counted by the platform layer: 1, or
 * the step that failed. */
static inline int64_t door_begin(void) {
    if (stop_begin(0) != 1) return 2;
    return hero_net_count_signal(hero_signal_number(HERO_SIGNAL_INTERRUPT)) == 0 ? 1 : 3;
}

/* A set holding the counted signals: quiet before any came, told READ with
 * the entry's key once the interrupt came, told again while it is untaken,
 * quiet once taken; its interest fixed, and a second entry told beside the
 * first. */
static inline int64_t door_set(void) {
    int32_t interrupt = hero_signal_number(HERO_SIGNAL_INTERRUPT);
    int64_t events[DOOR_EVENTS];
    hero_poll_set *set = hero_poll_make(4);

    if (set == NULL) return 2;
    int64_t key = hero_poll_add_signals(set);
#if defined(_WIN32)
    if (key != -1 || hero_poll_last_error() != HERO_POLL_UNSUPPORTED) return 3;
    return hero_poll_close(set) == 0 ? 1 : 4;
#else
    if (key <= 0) return 3;
    if (hero_poll_wait(set, 0, events, DOOR_EVENTS) != 0) return 4;
    if (stop_send_now(STOP_INTERRUPT) != 1) return 5;
    if (hero_poll_wait(set, DOOR_PATIENCE_NS, events, DOOR_EVENTS) != 1) return 6;
    if (events[0] != key || events[1] != HERO_POLL_READ) return 7;
    if (hero_poll_wait(set, 0, events, DOOR_EVENTS) != 1) return 8;
    if (hero_signal_take(interrupt) != 1) return 9;
    if (hero_poll_wait(set, 0, events, DOOR_EVENTS) != 0) return 10;
    if (hero_poll_change(set, key, HERO_POLL_WRITE) != -1 || hero_poll_last_error() != HERO_POLL_INVALID) return 11;
    int64_t second = hero_poll_add_signals(set);

    if (second <= 0 || second == key) return 12;
    if (stop_send_now(STOP_INTERRUPT) != 1) return 13;
    if (hero_poll_wait(set, DOOR_PATIENCE_NS, events, DOOR_EVENTS) != 2) return 14;
    if (hero_signal_take(interrupt) != 1) return 15;
    if (hero_poll_remove(set, second) != 0) return 16;
    if (hero_poll_wait(set, 0, events, DOOR_EVENTS) != 0) return 17;
    return hero_poll_close(set) == 0 ? 1 : 18;
#endif
}

/* A wait for a child that runs on: it looks once and answers the limit
 * passed, ends when the interrupt comes, answers at once while it is
 * untaken, waits out a limit once it is taken, and answers the end once the
 * child is ended. The interrupt must not end the child: on POSIX the child
 * stands in a group of its own, which the SIGINT sent to this process's
 * group does not reach. A console program on Windows has no such group
 * (`hero_proc_start` answers every kind but the parent's
 * `HERO_PROC_UNSUPPORTED`), so the child shares this console and would end
 * at its Ctrl-C; it is started with Ctrl-C ignored, a state a child takes
 * from its parent when it is made (`SetConsoleCtrlHandler`), and this
 * process heeds Ctrl-C again as soon as the start returns. */
static inline int64_t door_child(void) {
    int32_t interrupt = hero_signal_number(HERO_SIGNAL_INTERRUPT);
#if defined(_WIN32)
    static const char program[] = "ping";
    static const char words[] = "-n\0" "31\0" "127.0.0.1";
    const int64_t group = HERO_PROC_GROUP_PARENT;
#else
    static const char program[] = "sleep";
    static const char words[] = "30";
    const int64_t group = HERO_PROC_GROUP_NEW;
#endif
    int64_t stop = -1;
    int64_t code = 0;
    int64_t by_signal = 0;
#if defined(_WIN32)
    if (!SetConsoleCtrlHandler(NULL, TRUE)) return 14;
#endif
    hero_proc *child = hero_proc_start(program, words, sizeof words, HERO_PROC_ENV_PARENT, NULL, 0, "",
                                       HERO_PROC_STDIO_NOTHING, NULL, HERO_PROC_STDIO_NOTHING, NULL,
                                       HERO_PROC_STDIO_NOTHING, NULL, group, NULL);
#if defined(_WIN32)
    if (!SetConsoleCtrlHandler(NULL, FALSE)) return 15;
#endif

    if (child == NULL) return 2;
    if (hero_proc_heed(child, 0, &stop) != 0 || stop != 0) return 3;
    if (stop_send_after(STOP_INTERRUPT, 200) != 1) return 4;
    int64_t heard = hero_proc_heed(child, DOOR_PATIENCE_NS, &stop);

    stop_note_done();
    if (heard != 3 || stop != 0) return 5;
    if (hero_proc_heed(child, DOOR_PATIENCE_NS, &stop) != 3) return 6;
    if (hero_signal_take(interrupt) != 1) return 7;
    if (hero_proc_heed(child, 100000000, &stop) != 0) return 8;
    if (hero_proc_kill(child) != 0) return 9;
    if (hero_proc_heed(child, DOOR_PATIENCE_NS, &stop) != 1) return 10;
    if (hero_proc_wait(child, &code, &by_signal) != 0) return 11;
    if (hero_proc_heed(child, -1, &stop) != 1) return 12;
    return hero_proc_release(child) == 0 ? 1 : 13;
}

#endif
