/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_time.h — the platform layer's clocks and its sleep (panel 234's R6;
 * feature 2295; spec § 13; the wall clock and the sleep, M-rebel-library).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME. Its functions are
 * defined in `platform/hero_time.c`, a member of the archive the compiler
 * puts on every program's link line (`selfhost/cli/layer.hero`), so the
 * linker takes the member only into a program that calls one: `print(1)`
 * carries no clock. A group binds it only by naming this header,
 * `extern "hero_time.h"`, with no `link` word and no flag: the header stands
 * on the runtime's include path, beside `hero_os.h`, and is not a seed, so no
 * unit reads it unless one of its groups names it.
 *
 * NO CONDITIONAL BUT THE GUARD (the ffi-pragmatist's condition (1) on panel
 * 234's P2, held by the net's `runtime` suite): the prototypes below are the
 * ones every platform's arm of the member is compiled against, so an arm that
 * disagrees with one is refused at the layer's own compile, on the machine
 * that compiles that arm, and never reaches a program (F.2's P2 against P2x).
 *
 * UTC ONLY: the wall clock counts from 1970-01-01 UTC and no function here
 * knows a time zone.
 */

#ifndef HERO_TIME_H
#define HERO_TIME_H

#include <stdint.h>

/* Nanoseconds on a clock that never goes back, counted from a start this
 * header does not name, so only the difference of two readings means
 * anything. It does not advance while the machine sleeps on Darwin and on
 * Linux (`CLOCK_UPTIME_RAW` and `CLOCK_MONOTONIC`); on Windows it is
 * `QueryPerformanceCounter`, of which that is unread. -1 where the clock
 * cannot be read or its reading does not fit 64 bits, never a wrong value. */
int64_t hero_time_monotonic_ns(void);

/* The kinds of failure of the two functions below, the same numbers on every
 * platform, and the numbers `hero_stream.h` and `hero_sys.h` give the kinds
 * of the same names; the platform's own number for the last failure is
 * `hero_time_last_os_error()`. `hero_time_monotonic_ns` above answers -1 and
 * names no kind, as it did before these stood here. */
#define HERO_TIME_OK 0
/* A count of nanoseconds below 0. */
#define HERO_TIME_INVALID 1
#define HERO_TIME_OTHER 12

/* Nanoseconds since 1970-01-01 00:00:00 UTC on the machine's wall clock, the
 * time of day as the machine keeps it: `clock_gettime(CLOCK_REALTIME)`, and
 * on Windows `GetSystemTimePreciseAsFileTime`, whose count from 1601 is moved
 * to 1970. Leap seconds are not counted, on any platform. The machine's
 * owner and the network's time set this clock, so it may jump either way
 * between two readings: a span is measured on the clock above, never on this
 * one. -1 with `HERO_TIME_OTHER` where the clock cannot be read, or stands
 * before 1970 or past what 64 bits of nanoseconds hold (the year 2262),
 * never a wrong value. */
int64_t hero_time_wall_ns(void);

/* Suspends the calling thread until at least `ns` nanoseconds have passed on
 * the clock `hero_time_monotonic_ns` reads, and answers 0. A signal the
 * program takes meanwhile does not cut it short: `nanosleep` is asked again
 * for what was left; Windows has no such signal, and waits on a timer, or by
 * `Sleep` where the system gives none. It may take longer, as long as the
 * system takes to run the thread again, and since that clock stands still
 * while the machine sleeps on Darwin and on Linux, so does the count. 0
 * returns at once. -1 with `HERO_TIME_INVALID` where `ns` is below 0. No
 * stop ends it: it knows nothing of `hero_net.h`'s. */
int64_t hero_time_sleep_ns(int64_t ns);

/* The kind of the last failure of `hero_time_wall_ns` or `hero_time_sleep_ns`
 * on this thread, `HERO_TIME_OK` after a call of either that did not fail,
 * and the platform's own number for it (0 where the layer itself refused). */
int64_t hero_time_last_error(void);
int64_t hero_time_last_os_error(void);

#endif
