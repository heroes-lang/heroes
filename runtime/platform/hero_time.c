/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_time.c — the platform layer's clock, one member of the archive
 * the compiler puts on every program's link line (panel 234's R6; feature
 * 2295; `selfhost/cli/layer.hero` builds, keys and links it).
 *
 * ITS OWN HEADER FIRST, before anything else (the ffi-pragmatist's condition
 * (2) on panel 234's P2, held by the net's `runtime` suite): the prototype in
 * `hero_time.h` is outside every arm, so an arm below that defines another
 * signature is `conflicting types` on the machine that compiles it. Without
 * the include, a narrower result in an arm built and read 4 s against an
 * uptime of 803 s on Linux, measured by the ffi-pragmatist (F.2's P2x).
 *
 * WHY THE CLOCKS DIFFER BY PLATFORM, AND IN MEANING RATHER THAN WIDTH (F.0):
 * `CLOCK_MONOTONIC` is 6 on Darwin and counts the time the machine sleeps,
 * and 1 on Linux and does not; Darwin's clock that does not is
 * `CLOCK_UPTIME_RAW`. Windows has no `clock_gettime` in its C library, and
 * its `timespec`'s `tv_nsec` is a 4-byte `long`: its arm reads the
 * performance counter.
 *
 * THE WALL CLOCK AND THE SLEEP (M-rebel-library) stand below the monotonic
 * clock, which they leave as it was. The wall clock is `CLOCK_REALTIME`, and
 * on Windows the system's file time, 100 ns ticks from 1601, moved to 1970.
 * THE SLEEP IS HELD TO THE MONOTONIC CLOCK ON EVERY PLATFORM: each arm waits
 * its own way (`nanosleep`, asked again for what a signal left; a waitable
 * timer, or `Sleep`, on Windows, whose `Sleep` may return before its time),
 * and one loop above both reads the clock after each wait and waits again
 * for what is still owed, so "at least `ns`" is what the clock says and not
 * what a wait promised. Their last error is per thread, the member's only
 * state.
 */

#include "hero_time.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <errno.h>
#include <time.h>
#endif

#define HERO_TIME_NS_PER_S INT64_C(1000000000)

/* `whole` seconds and `part` of a second counted in `per` units, as
 * nanoseconds; -1 where they do not fit 64 bits. `part` is below `per`, and
 * `per` at most a nanosecond's count, so `part * 1e9 / per` loses nothing a
 * nanosecond can hold. */
static int64_t hero_time_ns(int64_t whole, int64_t part, int64_t per) {
    int64_t seconds = 0;
    int64_t scaled = 0;
    int64_t total = 0;

    if (whole < 0 || part < 0 || per <= 0 || part >= per) {
        return -1;
    }
    if (__builtin_mul_overflow(whole, HERO_TIME_NS_PER_S, &seconds) ||
        __builtin_mul_overflow(part, HERO_TIME_NS_PER_S, &scaled) ||
        __builtin_add_overflow(seconds, scaled / per, &total)) {
        return -1;
    }
    return total;
}

#if defined(_WIN32)

int64_t hero_time_monotonic_ns(void) {
    LARGE_INTEGER count;
    LARGE_INTEGER frequency;

    if (!QueryPerformanceFrequency(&frequency) || !QueryPerformanceCounter(&count)) {
        return -1;
    }
    if (frequency.QuadPart <= 0 || count.QuadPart < 0) {
        return -1;
    }
    return hero_time_ns(count.QuadPart / frequency.QuadPart, count.QuadPart % frequency.QuadPart, frequency.QuadPart);
}

#else

#if defined(__APPLE__)
#define HERO_TIME_CLOCK CLOCK_UPTIME_RAW
#else
#define HERO_TIME_CLOCK CLOCK_MONOTONIC
#endif

int64_t hero_time_monotonic_ns(void) {
    struct timespec now;

    if (clock_gettime(HERO_TIME_CLOCK, &now) != 0) {
        return -1;
    }
    return hero_time_ns((int64_t)now.tv_sec, (int64_t)now.tv_nsec, HERO_TIME_NS_PER_S);
}

#endif

/* The wall clock and the sleep: their last failure, per thread. */
static _Thread_local int64_t hero_time_kind;
static _Thread_local int64_t hero_time_os;

static void hero_time_fine(void) {
    hero_time_kind = HERO_TIME_OK;
    hero_time_os = 0;
}

static int64_t hero_time_fail(int64_t kind, int64_t os) {
    hero_time_kind = kind;
    hero_time_os = os;
    return -1;
}

#if defined(_WIN32)

/* The 100 ns ticks from 1601-01-01, where Windows' file time starts, to
 * 1970-01-01: 134,774 days. */
#define HERO_TIME_TICKS_TO_1970 INT64_C(116444736000000000)
/* The most one `Sleep` is asked for, an hour: its count is 32 bits of
 * milliseconds, the largest of which means for ever. */
#define HERO_TIME_SLEEP_MOST_MS INT64_C(3600000)
/* A timer that keeps its time to well under a millisecond (Windows 10 1803
 * and later), named here for a set of headers older than it. */
#if !defined(CREATE_WAITABLE_TIMER_HIGH_RESOLUTION)
#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
#endif

int64_t hero_time_wall_ns(void) {
    FILETIME now;
    uint64_t ticks = 0;
    int64_t since = 0;
    int64_t ns = 0;

    GetSystemTimePreciseAsFileTime(&now);
    ticks = ((uint64_t)now.dwHighDateTime << 32) | (uint64_t)now.dwLowDateTime;
    if (ticks > (uint64_t)INT64_MAX) {
        return hero_time_fail(HERO_TIME_OTHER, 0);
    }
    since = (int64_t)ticks - HERO_TIME_TICKS_TO_1970;
    if (since < 0 || __builtin_mul_overflow(since, INT64_C(100), &ns)) {
        return hero_time_fail(HERO_TIME_OTHER, 0);
    }
    hero_time_fine();
    return ns;
}

/* Waits about `ns` nanoseconds, above 0: on a timer set for that span from
 * now, in 100 ns ticks rounded up, one that keeps its time finely where the
 * system has it; by `Sleep`, in milliseconds rounded up, where the system
 * gives no timer. Answers 0: the loop that calls it holds it to the clock. */
static int64_t hero_time_wait(int64_t ns) {
    HANDLE timer = CreateWaitableTimerExW(NULL, NULL, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
    LARGE_INTEGER due;
    int64_t ms = ns / 1000000 + (ns % 1000000 != 0 ? 1 : 0);

    if (timer == NULL) {
        timer = CreateWaitableTimerExW(NULL, NULL, 0, TIMER_ALL_ACCESS);
    }
    due.QuadPart = -(ns / 100 + (ns % 100 != 0 ? 1 : 0));
    if (timer != NULL) {
        int waited = SetWaitableTimer(timer, &due, 0, NULL, NULL, FALSE) && WaitForSingleObject(timer, INFINITE) == WAIT_OBJECT_0;

        (void)CloseHandle(timer);
        if (waited) {
            return 0;
        }
    }
    Sleep((DWORD)(ms > HERO_TIME_SLEEP_MOST_MS ? HERO_TIME_SLEEP_MOST_MS : ms));
    return 0;
}

#else

int64_t hero_time_wall_ns(void) {
    struct timespec now;
    int64_t ns = 0;

    if (clock_gettime(CLOCK_REALTIME, &now) != 0) {
        return hero_time_fail(HERO_TIME_OTHER, (int64_t)errno);
    }
    /* -1 for a clock before 1970 or past 64 bits, as for the monotonic one. */
    ns = hero_time_ns((int64_t)now.tv_sec, (int64_t)now.tv_nsec, HERO_TIME_NS_PER_S);
    if (ns < 0) {
        return hero_time_fail(HERO_TIME_OTHER, 0);
    }
    hero_time_fine();
    return ns;
}

/* Waits `ns` nanoseconds, above 0, by `nanosleep`, asked again for what was
 * left each time a signal ends it early. Answers 0, or the system's number
 * where it refused the wait. */
static int64_t hero_time_wait(int64_t ns) {
    struct timespec want;
    struct timespec rest;

    want.tv_sec = (time_t)(ns / HERO_TIME_NS_PER_S);
    want.tv_nsec = (long)(ns % HERO_TIME_NS_PER_S);
    rest = want;
    while (nanosleep(&want, &rest) != 0) {
        if (errno != EINTR) {
            return (int64_t)errno;
        }
        want = rest;
    }
    return 0;
}

#endif

int64_t hero_time_sleep_ns(int64_t ns) {
    int64_t start = 0;
    int64_t left = ns;

    if (ns < 0) {
        return hero_time_fail(HERO_TIME_INVALID, 0);
    }
    start = hero_time_monotonic_ns();
    while (left > 0) {
        int64_t refused = hero_time_wait(left);
        int64_t now = 0;

        if (refused != 0) {
            return hero_time_fail(HERO_TIME_OTHER, refused);
        }
        /* With no clock to hold it to, the wait's own word stands. */
        if (start < 0) {
            break;
        }
        now = hero_time_monotonic_ns();
        if (now < start) {
            break;
        }
        left = now - start >= ns ? 0 : ns - (now - start);
    }
    hero_time_fine();
    return 0;
}

int64_t hero_time_last_error(void) {
    return hero_time_kind;
}

int64_t hero_time_last_os_error(void) {
    return hero_time_os;
}
