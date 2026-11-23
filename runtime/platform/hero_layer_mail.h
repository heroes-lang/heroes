/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_layer_mail.h — what the platform layer asks of a mailbox it
 * knows by number: whether a receive would find something, and a watch the
 * mailbox rings when that changes (panel 256's R6 to R8, provisional).
 *
 * A HEADER OF THE MEMBERS ALONE, `hero_layer_stop.h`'s shape: no Heroes group
 * names it. `runtime.c` names it before `parts/mail.c`, which defines its
 * functions, so the runtime's key covers it and clang checks each definition
 * against it. A program reaches a mailbox's number by `m.number()`
 * (`hero_mail_number`, `heroes_runtime.h`); a number reaches no value, holds
 * no copy, and once the mailbox's last copy is dropped names nothing, never
 * another mailbox.
 *
 * THE RUNTIME NAMES NO SYMBOL OF THE LAYER. A ring is a function of the
 * layer's, handed by pointer at run time, so a program that links no member
 * links none of this. A RING TAKES NO LOCK AT ALL and does not wait: it is
 * called under the queue's own lock (a send, a close) or the queues' list
 * lock (the stop, the queue's end), so a ring that waited on any lock could
 * close a cycle with a thread holding it. An `EVFILT_USER` trigger, an
 * `eventfd` write, a byte down a non-blocking pipe and a `send` on a
 * non-blocking socket each qualify. Because every ring runs under the queue's
 * lock, a watch taken away by `hero_mail_unwatch` is never rung again once
 * that call returns: the layer may then close what `ctx` names. */
#ifndef HERO_LAYER_MAIL_H
#define HERO_LAYER_MAIL_H

#include <stdint.h>

/* A watch's ring: `ctx` is the layer's, handed back as it was given. */
typedef void (*HeroMailRing)(void *ctx);

/* 1 while a receive on the mailbox `number` names would not wait: it holds a
 * value, it is closed, or it answers `stopped`. 0 while a receive would wait.
 * -1 once the number names nothing: its mailbox's last copy is gone, or it
 * was never a mailbox's number. */
int64_t hero_mail_ready(int64_t number);

/* A watch on the mailbox `number` names: `ring(ctx)` is called each time the
 * mailbox goes from empty to holding a value, at its close, at the program's
 * stop, and once when its last copy is dropped. Readiness is level: a ring
 * says only that `hero_mail_ready` may now answer 1, so the layer asks it.
 * One mailbox holds any number of watches, the same pair twice among them.
 * Answers 0, or -1 where `ring` is NULL or the number names nothing. */
int64_t hero_mail_watch(int64_t number, HeroMailRing ring, void *ctx);

/* One watch of the pair `ring` and `ctx` taken away from the mailbox `number`
 * names; once this returns, no ring of it runs or will run. Answers 0, or -1
 * where the number names nothing (its watches went with its last copy) or
 * holds no such watch. */
int64_t hero_mail_unwatch(int64_t number, HeroMailRing ring, void *ctx);

#endif
