/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_net.h — the platform layer's TCP sockets (panel 234's R6; feature
 * 2295; spec § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_time.h`
 * is: its functions are defined in `platform/hero_net.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_net.h"`, with no `link` word and no flag, and no unit reads
 * it unless one of its groups names it. On Windows the member, and only a
 * program that takes it, links `ws2_32`.
 *
 * WHY C OF THE PROJECT'S OWN, AND NOT A BINDING OF THE PLATFORM'S (F.0): no
 * record of `sockaddr_in` builds on both POSIX machines (`sin_len` is Darwin's
 * alone, `sin_family` a byte there and two on Linux), `addrinfo`'s fields come
 * in another order, the signal a write to a closed peer raises is stopped
 * per socket on Darwin and per call on Linux, and Windows' `SOCKET` is an
 * unsigned 64-bit handle with its own error numbers. So no platform type
 * reaches this header: a group sees `int64_t`, `uint64_t`, `void *`,
 * `const char *` and the handle below.
 *
 * NO CONDITIONAL BUT THE GUARD (the ffi-pragmatist's condition (1), held by
 * the net's `runtime` suite): every platform's arm of the member is compiled
 * against these prototypes.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a function handing out a socket
 * answers NULL, one answering a count -1, and `hero_net_last_error()` then
 * names the kind. A socket is a handle the layer issues and checks, never an
 * address it reads through, so one closed already, NULL, or one this layer
 * never issued is `HERO_NET_NOT_OPEN`, never a read of freed memory and
 * never another socket that took its number.
 */

#ifndef HERO_NET_H
#define HERO_NET_H

#include <stdint.h>

/* A socket this layer holds open: a group declares it as a handle,
 * `record Socket tag hero_net_socket`, which `hero_net_close` gives back. */
typedef struct hero_net_socket hero_net_socket;

/* The kinds of failure, the same numbers on every platform; the platform's
 * own number for the last failure is `hero_net_last_os_error()`. */
#define HERO_NET_OK 0
/* An argument the layer refuses: no address, a port outside 0 to 65535, or
 * a count with no bytes behind it. */
#define HERO_NET_INVALID 1
/* The socket is none this layer holds open: closed already, or never one. */
#define HERO_NET_NOT_OPEN 2
/* The address names nothing this machine can reach or listen on. */
#define HERO_NET_ADDRESS 3
#define HERO_NET_IN_USE 4
#define HERO_NET_REFUSED 5
/* The peer closed or reset the connection, or it was never made. */
#define HERO_NET_RESET 6
#define HERO_NET_TIMED_OUT 7
#define HERO_NET_UNREACHABLE 8
/* The process or the machine has no socket, descriptor or memory left. */
#define HERO_NET_LIMIT 9
#define HERO_NET_DENIED 10
#define HERO_NET_OTHER 11
/* The program was asked to stop (`hero_net_catch_stop`) before or while the
 * call would have waited. */
#define HERO_NET_STOPPED 12

/* A socket listening on `address` (`127.0.0.1`, `::1`, or "" for every
 * address of the machine) at `port`, 0 asking the system for a free one,
 * which `hero_net_port` reads back. */
hero_net_socket *hero_net_listen(const char *address, int64_t port);

/* A connection to `address` (a number or a name) at `port`. */
hero_net_socket *hero_net_connect(const char *address, int64_t port);

/* The next connection a listening socket holds, waiting for one. */
hero_net_socket *hero_net_accept(hero_net_socket *listener);

/* The port the socket stands on, or -1. */
int64_t hero_net_port(hero_net_socket *s);

/* Sends up to `count` bytes of `data` and answers how many were sent, which
 * may be fewer, as many as the system had room for: the caller sends the
 * rest. It waits only while there is no room at all. */
int64_t hero_net_send(hero_net_socket *s, const void *data, uint64_t count);

/* Receives up to `count` bytes into `buffer`, waiting for one, and answers
 * how many; 0 when the peer has closed its side. */
int64_t hero_net_recv(hero_net_socket *s, void *buffer, uint64_t count);

/* Closes the socket and gives its handle back, whatever is answered: 0, or
 * -1 where the socket was not open or the system reported a failure while
 * closing it. A call another thread waits in on it, an `accept`, a `recv`
 * or a `send`, ends within about 50 ms, failing `HERO_NET_NOT_OPEN`, since
 * every call waits in slices of that; no connection made after the close is
 * accepted, and no later call reaches the socket. */
int64_t hero_net_close(hero_net_socket *s);

/* THE STOP (panel 246's R4): how a server ends on Ctrl-C or a request to
 * end, `kill`'s, `timeout`'s or a service manager's, and returns from `main`,
 * so every handle is given back and the leak check at its end runs.
 *
 * From this call on, SIGINT and SIGTERM (on Windows Ctrl-C, Ctrl-Break and
 * the console closing) no longer end the program at once. The first of them
 * asks it to stop: from then, on every thread, a call of this layer that
 * would wait answers -1 or NULL with `HERO_NET_STOPPED` instead, one
 * already waiting within about 50 ms. An `accept` answers so even with a
 * connection waiting, so a loop of accepts ends under load, and so does a
 * `connect` whose connection the system cannot make at once; a `recv` with
 * bytes already there and a `send` the system has room for still move them,
 * so a stopping program can finish what it holds. A second one while the
 * program stops ends it at once, as the signal would have.
 *
 * What runs in the signal is the layer's own C, which writes one byte down
 * a pipe the waiting calls watch beside their socket: no code of the
 * program, no allocation. A signal the program inherited ignored stays
 * ignored (a job a shell starts with `&` has SIGINT so). Any other handler
 * of the two is replaced, among them the runtime's own that passes the
 * signal on to a child the program waits on: once the stop is caught, such
 * a child is not sent the signal and runs to its end. A name being looked
 * up for `hero_net_connect` is not ended by a stop; the call answers
 * `HERO_NET_STOPPED` once the lookup returns. A signal sent to `heroes run`
 * is passed on to the program, which then has about two seconds before
 * `heroes` kills it. On Windows the console closing ends the process within
 * five seconds whatever it does (Microsoft's HandlerRoutine page; unrun).
 *
 * Answers 0, also where the stop was already caught, or -1 where the system
 * gave no pipe or refused the handler. */
int64_t hero_net_catch_stop(void);

/* 1 once a stop has been asked, else 0: a loop that works between the
 * layer's calls asks it between its steps, and a thread that joins a worker
 * asks it after. A reader, as the two below: it changes nothing they
 * answer. */
int64_t hero_net_stop_asked(void);

/* The kind of the last call's failure on this thread, `HERO_NET_OK` after a
 * call that did not fail, and the platform's own number for it (0 where the
 * layer itself refused). */
int64_t hero_net_last_error(void);
int64_t hero_net_last_os_error(void);

#endif
