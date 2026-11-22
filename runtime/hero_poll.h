/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_poll.h — the platform layer's readiness sets: one thread waits on many
 * streams, sockets, listeners and timers at once and is told which are ready
 * and for what (M-rebel-library, 2026-11-16; spec § 13). What an event loop
 * gives a program, without a coroutine (design.md Part 6 refuses them): the
 * loop is the program's own `while`, and this header is the one call it waits
 * in.
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h` is:
 * its functions are defined in `platform/hero_poll.c`, a member of the archive
 * on every program's link line, which the linker takes only into a program
 * that calls one; a group binds them by naming this header,
 * `extern "hero_poll.h"`, with no `link` word and no flag. The member calls
 * `hero_net.h`, `hero_stream.h` and `hero_stream_net.h`, so a program that
 * takes it takes those three; on Windows it links `ws2_32`.
 *
 * NO CONDITIONAL BUT THE GUARD, AND NO PLATFORM TYPE: a group sees `int64_t`,
 * `uint64_t`, the set below, and the two handles of the headers this one
 * includes for their names.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a function handing out a set
 * answers NULL, one answering a count or a key -1, and `hero_poll_last_error()`
 * then names the kind. A set is a handle the layer issues and checks, never an
 * address it reads through, so one closed already, NULL, or one this layer
 * never issued is `HERO_POLL_NOT_OPEN`; and a key is checked as a handle is,
 * so one removed already, or one of another set, is `HERO_POLL_NOT_FOUND`,
 * never another entry that took its place.
 *
 * WHAT IS WAITED ON, AND HOW, ON EACH SYSTEM. Darwin's `kqueue` and Linux's
 * `epoll`, one per set, each entry registered once and LEVEL-TRIGGERED: what
 * stays ready is told again at every wait until the program reads, writes or
 * removes it, so a loop that reads once per event loses nothing. On Windows,
 * `WSAPoll` over the set's sockets at each wait: the layer's sockets are
 * non-blocking and every call of `hero_net.h` tries before it waits, which is
 * the readiness model `WSAPoll` answers; an I/O completion port answers the
 * other model, an operation started and finished later into a buffer the
 * system holds meanwhile, which no call of this layer makes, and which no
 * pipe or console handle the layer opens could join. A regular file is ready
 * to read and to write at every wait, on every system, as POSIX's `poll`
 * defines it (`epoll` refuses one). Timers are this member's own, read on a
 * clock that never goes back, and need no descriptor.
 *
 * WHAT A SET HOLDS IS KEPT OPEN. A stream or a socket in a set is held: closed
 * by the program meanwhile, its handle ends at once, as ever, and its
 * descriptor stays open until it leaves the set, so no file the system opens
 * meanwhile can take its number under the set. A socket closed so is shut
 * down by `hero_net_close`, which a wait then tells as `HERO_POLL_HANGUP`.
 *
 * ONE CALL AT A TIME ON A SET: a wait, an add, a change or a remove asked of a
 * set another thread waits on is `HERO_POLL_INVALID`, nothing changed (a
 * program of the language cannot ask it: a thread is handed an `int64_t`, and
 * no number names a set). A close asked meanwhile ends the set's handle at
 * once and the set itself when that wait returns, which then answers
 * `HERO_POLL_NOT_OPEN`.
 *
 * THE STOP ENDS A WAIT (`hero_net.h`'s, panel 246's R4 and panel 248's R4):
 * once `hero_net_ask_stop` is called, or a signal is caught after
 * `hero_net_catch_stop`, a wait answers -1 with `HERO_POLL_STOPPED`, one
 * already waiting at once, and every later one before it waits, even with
 * entries ready, so a server's loop ends under load. The member hears it
 * through `hero_net.h`'s own readiness thread, never a thread of its own, and
 * spends one pipe for the process on POSIX and one loopback socket on Windows,
 * never one per set. Where it cannot (no descriptor, no thread, no loopback),
 * a wait waits in slices of about 50 ms and asks between them, every answer
 * the same and "at once" then "within about 50 ms".
 */

#ifndef HERO_POLL_H
#define HERO_POLL_H

#include <stdint.h>

#include "hero_net.h"
#include "hero_stream.h"

/* A set of entries this layer holds open: a group declares it as a handle,
 * `record Set tag hero_poll_set`, which `hero_poll_close` gives back. */
typedef struct hero_poll_set hero_poll_set;

/* The kinds of failure, the same numbers on every platform; the platform's
 * own number for the last failure is `hero_poll_last_os_error()`. */
#define HERO_POLL_OK 0
/* An argument the layer refuses: a size, an interest, a time or a buffer
 * outside what the function names, a change of a timer's interest, or a set
 * another thread waits on. */
#define HERO_POLL_INVALID 1
/* The set, the stream or the socket handed is none this layer holds open. */
#define HERO_POLL_NOT_OPEN 2
/* The key names no entry of this set: removed already, or another set's. */
#define HERO_POLL_NOT_FOUND 3
/* The set holds the most entries it was made for, or the process has no set,
 * entry, descriptor or memory left. */
#define HERO_POLL_LIMIT 4
/* This system cannot wait on a stream of this kind: a pipe or a console on
 * Windows, a device the system refuses (`/dev/null` on Darwin and Linux), a
 * stream that stands on neither a descriptor nor a socket of this layer. */
#define HERO_POLL_UNSUPPORTED 5
/* The stop was asked (THE STOP ENDS A WAIT, above). */
#define HERO_POLL_STOPPED 6
#define HERO_POLL_OTHER 7

/* What an entry waits for, and what a wait answers it is ready for: bits of
 * one number. READ: bytes to read, the peer's end of stream, or, for a
 * listener, a connection to accept. WRITE: room to write. The two are what an
 * entry is added with, one or both; the three below are only answered. */
#define HERO_POLL_READ 1
#define HERO_POLL_WRITE 2
/* The peer closed its side, or the connection is gone; told with READ where
 * READ was asked, so the read that follows answers the end. Where a system
 * tells it differs (`hero_poll.c` says how). */
#define HERO_POLL_HANGUP 4
/* The system holds a failure for the socket: the next call on it answers it. */
#define HERO_POLL_ERROR 8
/* A timer fell due. */
#define HERO_POLL_DUE 16

/* The most entries one set holds. */
#define HERO_POLL_MOST 65535
/* The numbers one event takes in a wait's buffer: its key, its bits, and for
 * a timer the times it fell due since it was last told (1, or more where the
 * program waited past later ones), 0 for every other entry. */
#define HERO_POLL_EVENT_NUMBERS 3

/* A new set that holds at most `most` entries, 1 to `HERO_POLL_MOST`; NULL with
 * `HERO_POLL_INVALID` outside it, `HERO_POLL_LIMIT` where the process has no
 * room for so many (65,536 entries among all its sets, 1,024 sets). */
hero_poll_set *hero_poll_make(int64_t most);

/* The socket `s` added to the set, waited on for `interest` (READ, WRITE, or
 * both): a connection, a listener, or a datagram socket. Answers the entry's
 * key, a positive number every event of it carries; or -1:
 * `HERO_POLL_INVALID` for another interest, `HERO_POLL_NOT_OPEN` for a set or
 * a socket that is not open. A socket may stand in a set twice: each entry is
 * its own, with its own key. */
int64_t hero_poll_add_socket(hero_poll_set *set, hero_net_socket *s, int64_t interest);

/* The stream `s` added to the set, as `hero_poll_add_socket` adds a socket: a
 * stream over a connection (`hero_stream_net.h`) is waited on as its socket;
 * a regular file is ready at every wait; a pipe, a terminal or the process's
 * own input on Darwin and Linux are waited on as their descriptor. Answers the
 * key, or -1 as `hero_poll_add_socket` does, and `HERO_POLL_UNSUPPORTED`
 * where the system cannot wait on what the stream stands on. */
int64_t hero_poll_add_stream(hero_poll_set *set, hero_stream *s, int64_t interest);

/* A timer added to the set: due `first_ns` nanoseconds from now, then every
 * `every_ns` after that, 0 for once. Answers its key, or -1 with
 * `HERO_POLL_INVALID` for a time below 0. A timer that fell due while the
 * program did not wait is told once, with how many times it fell due, and is
 * due next at the first of its times still to come, never in a burst. One
 * told once and not every is due no more until `hero_poll_set_timer`. */
int64_t hero_poll_add_timer(hero_poll_set *set, int64_t first_ns, int64_t every_ns);

/* THE SIGNALS THE PROGRAM COUNTS added to the set (feature 2690): one entry,
 * told `HERO_POLL_READ` while a delivery of any signal `hero_os.h`'s door
 * counts is not taken by `hero_signal_take`, whichever signal it is and
 * whether the program counted it before this call or after. Never told for a
 * byte with no count behind it. Each entry waits on its own copy of the
 * door's descriptor, so the signals stand in a set twice as a socket does.
 * Its interest is fixed: `hero_poll_change` answers `HERO_POLL_INVALID`.
 * Answers its key, or -1: `HERO_POLL_NOT_OPEN`, `HERO_POLL_LIMIT`,
 * `HERO_POLL_OTHER` where the system gave no pipe or no copy of it, and
 * `HERO_POLL_UNSUPPORTED` on Windows, whose wait takes sockets alone. */
int64_t hero_poll_add_signals(hero_poll_set *set);

/* The timer `key` set again, as `hero_poll_add_timer` sets one, from now.
 * Answers 0, or -1: `HERO_POLL_INVALID` for a time below 0 or a key that is no
 * timer, `HERO_POLL_NOT_FOUND`, `HERO_POLL_NOT_OPEN`. */
int64_t hero_poll_set_timer(hero_poll_set *set, int64_t key, int64_t first_ns, int64_t every_ns);

/* The entry `key` waited on for `interest` from now. Answers 0, or -1:
 * `HERO_POLL_INVALID` for another interest or a timer, `HERO_POLL_NOT_FOUND`,
 * `HERO_POLL_NOT_OPEN`. */
int64_t hero_poll_change(hero_poll_set *set, int64_t key, int64_t interest);

/* The entry `key` taken out of the set, its stream or socket let go. Answers
 * 0, or -1: `HERO_POLL_NOT_FOUND`, `HERO_POLL_NOT_OPEN`. */
int64_t hero_poll_remove(hero_poll_set *set, int64_t key);

/* How many entries the set holds, or -1 with `HERO_POLL_NOT_OPEN`. */
int64_t hero_poll_count(hero_poll_set *set);

/* WAITS until an entry of the set is ready, a timer falls due, `limit_ns`
 * nanoseconds pass (0 asks without waiting, -1 waits with no limit), or the
 * stop is asked; then writes one event per ready entry into `events`,
 * `HERO_POLL_EVENT_NUMBERS` numbers each, as many as `count` numbers hold, and
 * answers how many events it wrote: 0 where the limit passed with none ready.
 * The timers fallen due come first, then the files, then what the system
 * told, in its order; what did not fit is told at the next wait. -1:
 * `HERO_POLL_INVALID` for a limit below -1 or a `count` below 3,
 * `HERO_POLL_NOT_OPEN`, `HERO_POLL_STOPPED`, and `HERO_POLL_OTHER` with the
 * system's number where its wait failed. */
int64_t hero_poll_wait(hero_poll_set *set, int64_t limit_ns, int64_t *events, uint64_t count);

/* Closes the set and gives its handle back, every stream and socket in it let
 * go and none of them closed: they are the program's. Answers 0, or -1 with
 * `HERO_POLL_NOT_OPEN`. */
int64_t hero_poll_close(hero_poll_set *set);

/* The kind of the last call's failure on this thread, `HERO_POLL_OK` after a
 * call that did not fail, and the platform's own number for it (0 where the
 * layer itself refused). */
int64_t hero_poll_last_error(void);
int64_t hero_poll_last_os_error(void);

#endif
