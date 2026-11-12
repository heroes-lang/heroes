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
 */

#include "hero_time.h"

#if defined(_WIN32)
#include <windows.h>
#else
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
