/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_time.h — the platform layer's clock (panel 234's R6; feature 2295;
 * spec § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME. Its one function is
 * defined in `platform/hero_time.c`, a member of the archive the compiler
 * puts on every program's link line (`selfhost/cli/layer.hero`), so the
 * linker takes the member only into a program that calls it: `print(1)`
 * carries no clock. A group binds it only by naming this header,
 * `extern "hero_time.h"`, with no `link` word and no flag: the header stands
 * on the runtime's include path, beside `hero_os.h`, and is not a seed, so no
 * unit reads it unless one of its groups names it.
 *
 * NO CONDITIONAL BUT THE GUARD (the ffi-pragmatist's condition (1) on panel
 * 234's P2, held by the net's `runtime` suite): the prototype below is the
 * one every platform's arm of the member is compiled against, so an arm that
 * disagrees with it is refused at the layer's own compile, on the machine
 * that compiles that arm, and never reaches a program (F.2's P2 against P2x).
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

#endif
