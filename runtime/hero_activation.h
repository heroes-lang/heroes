/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_activation.h — the platform layer's two protocols a service manager
 * speaks to the program it starts on POSIX systems (M-rebel-library,
 * 2026-11-23; spec § 13): systemd's READINESS notice, a datagram to the
 * socket `$NOTIFY_SOCKET` names, and the SOCKETS A MANAGER HANDS OVER, which
 * systemd passes from descriptor 3 on (`$LISTEN_PID`, `$LISTEN_FDS`) and
 * launchd gives through `launch_activate_socket`. What go-systemd's `daemon`
 * and `activation`, Rust's `sd-notify` and `listenfd` and Python's
 * `systemd.daemon` give a program.
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_service.h`
 * is: its functions are defined in `platform/hero_activation.c`, a member of
 * the archive on every program's link line, which the linker takes only into
 * a program that calls one; a group binds them by naming this header,
 * `extern "hero_activation.h"`, with no `link` word and no flag. The member
 * calls `hero_net.h` (every socket handed over becomes a socket of its
 * table), so a program that takes it takes that one.
 *
 * NO CONDITIONAL BUT THE GUARD, AND NO PLATFORM TYPE: a group sees `int64_t`,
 * `uint64_t`, `const char *` and the record below.
 *
 * ONLY POSIX HAS THESE PROTOCOLS, and launchd's only Darwin. Every other
 * call answers `HERO_ACTIVATION_UNSUPPORTED`, so a program learns what its
 * system has from `hero_activation_has`.
 *
 * NO FUNCTION OF THIS MEMBER READS A DESCRIPTOR A CALLER NAMES: the
 * environment says which descriptors were handed over, and the layer reads
 * it itself, so a program cannot make the layer adopt (and close) a
 * descriptor that is not a manager's. Each of those is adopted once in the
 * life of the process, and the sockets go to the program as the numbers of
 * `hero_net.h`'s `hero_net_number`, taken back with `hero_net_of`.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a call answers -1 and
 * `hero_activation_last_error()` names the kind, `hero_activation_last_os_error()`
 * the system's number, `hero_activation_last_message` words of the layer's
 * own. All three are the calling thread's. */

#ifndef HERO_ACTIVATION_H
#define HERO_ACTIVATION_H

#include <stdint.h>

/* The kinds of failure, the same numbers on every platform. */
#define HERO_ACTIVATION_OK 0
/* An argument the layer refuses, or an environment it cannot read: a
 * variable that is no number, a name too long for a socket's address, a
 * descriptor the variables promise that is not open. */
#define HERO_ACTIVATION_INVALID 1
/* This system has no such protocol, or the address names a kind of socket
 * this system has none of (an abstract name off Linux, `vsock:`). */
#define HERO_ACTIVATION_UNSUPPORTED 2
/* Nothing is there: no socket answers at the name, or launchd has no socket
 * of that name, or the program is no launchd job. */
#define HERO_ACTIVATION_NOT_FOUND 3
/* Taken already: the descriptors were adopted once, or launchd handed the
 * name's sockets over, or a socket stands at the path. */
#define HERO_ACTIVATION_EXISTS 4
/* The system has no more of what the call needs: descriptors, memory, room
 * in the receiver's queue, or a message longer than a datagram carries. */
#define HERO_ACTIVATION_LIMIT 5
#define HERO_ACTIVATION_DENIED 6
/* A wait passed its bound. */
#define HERO_ACTIVATION_TIMEOUT 7
#define HERO_ACTIVATION_OTHER 8

/* What `hero_activation_has` is asked. It answers 1 or 0. */
#define HERO_ACTIVATION_HAS_NOTIFY 0
#define HERO_ACTIVATION_HAS_INHERIT 1
#define HERO_ACTIVATION_HAS_LAUNCHD 2

/* The kinds of an entry. */
#define HERO_ACTIVATION_ENTRY_LISTENER 1
#define HERO_ACTIVATION_ENTRY_CONNECTION 2
#define HERO_ACTIVATION_ENTRY_DATAGRAM 3
/* A descriptor that is no socket this layer holds: a FIFO, a file, a local
 * socket, a netlink one. It is left open and untouched, apart from being
 * marked close-on-exec, and `hero_activation_describe` says what it is. */
#define HERO_ACTIVATION_ENTRY_OTHER 4

/* The most descriptors `$LISTEN_FDS` may say, and the most sockets one name
 * of launchd's may hold. */
#define HERO_ACTIVATION_MOST 4096
#define HERO_ACTIVATION_LAUNCHD_MOST 64

/* The most bytes of one notice. */
#define HERO_ACTIVATION_NOTICE_MOST 4096

/* One descriptor a manager handed over. `number` is a socket's number of
 * `hero_net.h` for a kind that is one, taken back once with `hero_net_of`
 * (or `hero_net_close` through it), and -1 for `HERO_ACTIVATION_ENTRY_OTHER`;
 * `descriptor` is the descriptor's own number, which only
 * `hero_activation_describe` reads. */
typedef struct hero_activation_entry {
    int64_t kind;
    int64_t number;
    int64_t descriptor;
} hero_activation_entry;

/* 1 where this system has the protocol `what` names, 0 where it has not or
 * `what` names none. Never fails. */
int64_t hero_activation_has(int64_t what);

/* Sends the `count` bytes of `data`, 1 to `HERO_ACTIVATION_NOTICE_MOST`, as
 * one datagram to the local socket `name` names, which is what
 * `$NOTIFY_SOCKET` holds: a path, or `@` and a name on Linux, where the
 * socket has no file. Answers 0, or -1 with `HERO_ACTIVATION_NOT_FOUND`
 * where nothing answers there, `HERO_ACTIVATION_LIMIT` where the receiver's
 * queue is full or the datagram is longer than the system carries,
 * `HERO_ACTIVATION_DENIED` where the system refuses the sender, and
 * `HERO_ACTIVATION_INVALID` for a name longer than a socket's address
 * holds. The call never waits: a full queue is told, never waited for. */
int64_t hero_activation_notify(const char *name, const void *data, uint64_t count);

/* THE RECEIVING HALF, one datagram at a time: makes a datagram socket at the
 * path `path`, waits for one datagram up to `within_ms` (0 or more, 0
 * not waiting at all), writes its bytes into `buffer`, `n` long, and
 * answers how many, removing the socket either way. A datagram longer than
 * `n` is `HERO_ACTIVATION_LIMIT`, its first bytes in `buffer`; a path where
 * something stands is `HERO_ACTIVATION_EXISTS`; a wait that passes
 * `HERO_ACTIVATION_TIMEOUT`. It is how a program plays the supervisor of one
 * notice, and the test of `hero_activation_notify` against a real datagram. */
int64_t hero_activation_receive(const char *path, int64_t within_ms, void *buffer, uint64_t n);

/* How many descriptors the manager handed this process by systemd's
 * protocol: `$LISTEN_PID` names this process and `$LISTEN_FDS` is how many,
 * from descriptor 3 on. 0 where either variable is absent or the pid is
 * another process's, so a program started by hand, or the child of one that
 * was handed sockets, is handed none. -1 with `HERO_ACTIVATION_INVALID` for
 * a variable that is no number, `HERO_ACTIVATION_LIMIT` above
 * `HERO_ACTIVATION_MOST`. */
int64_t hero_activation_count(void);

/* What `hero_activation_count` answers for the two texts a manager wrote
 * (`$LISTEN_PID`, `$LISTEN_FDS`) and the process `pid`, the environment left
 * out: the whole of its reading, so a test needs no environment. A NULL text is
 * an absent variable. */
int64_t hero_activation_count_for(const char *pid_text, const char *count_text, int64_t pid);

/* The machine's `CLOCK_MONOTONIC` in whole microseconds, which a notice of
 * `RELOADING=1` is sent with; -1 where this system has none. */
int64_t hero_activation_monotonic_usec(void);

/* Adopts the `hero_activation_count()` descriptors, once in the life of the
 * process, into `entries`, `n` long (at least the count): each a socket of
 * `hero_net.h`'s table where it is a listening or connected stream socket or
 * a datagram socket of IPv4 or IPv6, an `OTHER` entry where it is anything
 * else. Answers the count, or -1: `HERO_ACTIVATION_EXISTS` the second time,
 * `HERO_ACTIVATION_INVALID` where `n` is below the count or a descriptor the
 * variables promise is not open, `HERO_ACTIVATION_LIMIT` where the socket
 * table is full. On a failure the sockets already adopted are closed and
 * the call is spent. */
int64_t hero_activation_inherit(hero_activation_entry *entries, uint64_t n);

/* launchd's `launch_activate_socket(name)`: the sockets the job's property
 * list names `name` under `Sockets`, adopted as `hero_activation_inherit`
 * adopts, into `entries`, `n` long, at least `HERO_ACTIVATION_LAUNCHD_MOST`.
 * Answers how many, or -1: `HERO_ACTIVATION_NOT_FOUND` where the program is
 * no launchd job or the job has no socket of that name,
 * `HERO_ACTIVATION_EXISTS` where the name's sockets were handed over before
 * (launchd hands them over once), `HERO_ACTIVATION_LIMIT` for more sockets
 * than that, which are closed, `HERO_ACTIVATION_UNSUPPORTED` off Darwin. */
int64_t hero_activation_launchd(const char *name, hero_activation_entry *entries, uint64_t n);

/* What the descriptor `descriptor` is, in words: written into `buffer`, `n`
 * long, where they fit, and their length; 0 where the descriptor is not open
 * or the layer has no words. */
int64_t hero_activation_describe(int64_t descriptor, void *buffer, uint64_t n);

int64_t hero_activation_last_error(void);
int64_t hero_activation_last_os_error(void);

/* The layer's own words for the last failure, UTF-8, written into `buffer`,
 * `n` long, where they fit; their length, 0 where there are none. */
int64_t hero_activation_last_message(void *buffer, uint64_t n);

#endif
