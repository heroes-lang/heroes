/* The C the case of a set's descriptor calls (panel 256's R9, provisional):
 * a set of `hero_poll.h` holding the program's mailbox by its number, its
 * descriptor asked once, and each step asked of it as a loop that waits on
 * readability alone would: `poll` with no wait, or on Windows, where the
 * descriptor is the HANDLE of an event the set owns, a wait of no time on the
 * handle. Each function answers 1 where the step held, else 0, or a negative
 * step. */
#ifndef POLL_A_SETS_DESCRIPTOR_SHOWN_H
#define POLL_A_SETS_DESCRIPTOR_SHOWN_H

#include "hero_poll.h"

#include <stddef.h>
#include <stdint.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <poll.h>
#endif

static hero_poll_set *shown_set;
static int64_t shown_fd = -1;
static int64_t shown_key = -1;

/* The set made with the mailbox `number` in it, and its descriptor asked. */
static int64_t shown_made(int64_t number) {
    shown_set = hero_poll_make(2);
    shown_key = shown_set == NULL ? -1 : hero_poll_add_mailbox(shown_set, number);
    if (shown_key <= 0) {
        return -1;
    }
    shown_fd = hero_poll_descriptor(shown_set);
    return shown_fd >= 0 ? 1 : -2;
}

/* Whether the descriptor is readable now is `want`. */
static int64_t shown_readable(int64_t want) {
#if defined(_WIN32)
    int64_t now = WaitForSingleObject((HANDLE)(uintptr_t)shown_fd, 0) == WAIT_OBJECT_0 ? 1 : 0;
#else
    struct pollfd p = { (int)shown_fd, POLLIN, 0 };
    int64_t now = poll(&p, 1, 0) == 1 && (p.revents & POLLIN) != 0 ? 1 : 0;
#endif

    return now == want ? 1 : 0;
}

/* Whether a wait of no time tells the mailbox, is `want`. */
static int64_t shown_told(int64_t want) {
    int64_t events[6];
    int64_t count = hero_poll_wait(shown_set, 0, events, 6);
    int64_t told = count == 1 && events[0] == shown_key ? 1 : 0;

    return count >= 0 && told == want ? 1 : 0;
}

/* The set closed, and its descriptor asked again: refused. */
static int64_t shown_closed(void) {
    if (hero_poll_close(shown_set) != 0) {
        return -1;
    }
    return hero_poll_descriptor(shown_set) == -1 && hero_poll_last_error() == HERO_POLL_NOT_OPEN ? 1 : 0;
}

#endif
