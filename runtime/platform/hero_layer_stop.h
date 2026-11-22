/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_layer_stop.h — the platform layer's word to the runtime that
 * the program's stop was asked (feature 2647).
 *
 * A HEADER OF THE MEMBERS ALONE, `hero_layer_alloc.h`'s shape: no Heroes group
 * names it. `hero_net.c` calls the function each time it hears a stop, by a
 * signal it caught or by the program's own `hero_net_ask_stop`, never inside a
 * signal handler; `runtime.c` names this header before `parts/mail.c`,
 * which defines it, so the runtime's key covers it and clang checks the
 * definition against it. What it does: every receive waiting on a mailbox looks
 * again, and one that finds its mailbox empty answers `stopped`, as that
 * mailbox does from then on (`parts/mail.c`, THE STOP DRAINS, THEN HOLDS;
 * panel 254's R4 and R6). Called by one line at the end of
 * `hero_net_nudge_all`, under the layer's lock: no queue operation takes that
 * lock, so the order is the layer's, then the queues' list, then a queue's. */
#ifndef HERO_LAYER_STOP_H
#define HERO_LAYER_STOP_H

void hero_mail_hear_stop(void);

#endif
