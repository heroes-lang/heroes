/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_layer_signal.h — the runtime's signal door as the platform
 * layer hands it SIGINT and SIGTERM (feature 2669).
 *
 * A HEADER OF THE MEMBERS ALONE, `hero_layer_stop.h`'s shape: no Heroes group
 * names it, and no header a group names includes it. `runtime.c` names it
 * before `parts/sigdoor.c`, which defines its two functions, so the runtime's
 * key covers it and clang checks the definitions against it.
 *
 * WHY THE LAYER AND NOT THE PROGRAM: `hero_signal_watch` refuses the two
 * (panel 247's R7), which are the platform layer's stop; `hero_net.c` alone
 * calls these, from `hero_net_count_signal`, under its own lock and once it
 * has refused the stop for the process, so the stop's handler and the
 * count's are never both installed for one signal. What a delivery then does
 * is `parts/sigdoor.c`'s, as for every other signal the door counts: one
 * counter per signal for the whole process, the pipe, `hero_signal_take`.
 *
 * NO CONDITIONAL BUT THE GUARD: both arms of `parts/sigdoor.c` define both. */

#ifndef HERO_LAYER_SIGNAL_H
#define HERO_LAYER_SIGNAL_H

#include <stdint.h>

/* POSIX: the runtime's own counting handler installed for `sig`, SIGINT or
 * SIGTERM, its pipe made first: 0, or -1 for any other signal, or where the
 * system gave no pipe or refused the handler, `errno` then the system's.
 * Windows: -1, since a console's events reach no signal handler there. */
int64_t hero_signal_take_over(int32_t sig);

/* One delivery of `sig` counted, from outside any signal handler: the
 * console's handler on Windows, on the thread the system makes for each
 * event. Lock-free, so it takes no lock a caller may hold. */
void hero_signal_count_one(int32_t sig);

#endif
