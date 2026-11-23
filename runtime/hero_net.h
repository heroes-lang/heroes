/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_net.h — the platform layer's TCP sockets (panel 234's R6; feature
 * 2295; spec § 13), and, since M-rebel-library, datagrams, names and a
 * socket's two addresses, which stand at its end.
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
 * never another socket that took its number. A handle of another member of
 * the layer, a stream's or a process's, is one this layer never issued,
 * `HERO_NET_NOT_OPEN` too, and never one of its slots: each table's handles
 * carry a tag of their own since 2026-11-16 (`platform/hero_handle_tag.h`).
 *
 * A CALL THAT WAITS COSTS NOTHING WHILE IT WAITS (panel 248's R2; defect
 * 2502). At the first call that waits the layer makes one thread of its own
 * for the process, which tells every waiting call when its socket is ready,
 * and spends one descriptor on it (two sockets on Windows, a loopback pair),
 * never one per waiting call: the descriptors of the process are the ones
 * its C libraries spend too. Where the layer cannot make it (no descriptor
 * or thread left, or on Windows no loopback), its calls wait in slices of
 * about 50 ms instead, every answer below the same and each "at once" then
 * "within about 50 ms".
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
/* The system gave the connection up, or a wait passed the socket's deadline
 * (`hero_net_set_deadline`). */
#define HERO_NET_TIMED_OUT 7
#define HERO_NET_UNREACHABLE 8
/* The process or the machine has no socket, descriptor or memory left. */
#define HERO_NET_LIMIT 9
#define HERO_NET_DENIED 10
#define HERO_NET_OTHER 11
/* The program was asked to stop (`hero_net_catch_stop`) before or while the
 * call would have waited. */
#define HERO_NET_STOPPED 12
/* A datagram did not fit: one received was longer than the buffer handed for
 * it, or one to send longer than the system sends as one. */
#define HERO_NET_TOO_LONG 13
/* A name the system's resolver knows no address for, or an address it knows
 * no name for. */
#define HERO_NET_NOT_FOUND 14

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
 * or a `send`, ends at once, failing `HERO_NET_NOT_OPEN`: the close wakes
 * it itself, on every platform; no connection made after the close is
 * accepted, and no later call reaches the socket. */
int64_t hero_net_close(hero_net_socket *s);

/* ONE CONNECTION PER THREAD (panel 246's R6): a thread of the program is
 * handed one `int64_t` and nothing else (`hero_os.h`'s `hero_thread_spawn`),
 * so a socket crosses into one as a number. `hero_net_number` gives the
 * socket up for its number: from that call no handle names it, the one
 * given up answering `HERO_NET_NOT_OPEN` (a call already waiting on it goes
 * on), and the number names it until `hero_net_of` takes it back, once, with
 * a handle of its own and never the one given up. A group binds the two with
 * the marks that keep the socket counted while it crosses:
 *
 *     function hero_net_number(s: Socket consumes) -> i64
 *     function hero_net_of(n: i64) -> Socket acquires hero_net_close | hero_net_number
 *
 * and names `hero_net_number` beside `hero_net_close` in the `acquires` of
 * every call whose socket it may give up. A number is counted as a handle
 * is: one never taken back is told by the program's leak check when `main`
 * returns, as a handle never given back is, since its socket stays open.
 *
 * The number is positive, or -1 with `HERO_NET_NOT_OPEN` where `s` names no
 * open socket. */
int64_t hero_net_number(hero_net_socket *s);

/* The socket a number names, taken back with a new handle, which
 * `hero_net_close` gives back, or `hero_net_number` again; NULL with
 * `HERO_NET_NOT_OPEN` where the number names none: taken back already, its
 * socket closed, or a number this layer never gave. */
hero_net_socket *hero_net_of(int64_t number);

/* THE STOP (panel 246's R4): how a server ends on Ctrl-C or a request to
 * end, `kill`'s, `timeout`'s or a service manager's, and returns from `main`,
 * so every handle is given back and the leak check at its end runs.
 *
 * From this call on, SIGINT and SIGTERM (on Windows Ctrl-C, Ctrl-Break and
 * the console closing) no longer end the program at once. The first of them
 * asks it to stop: from then, on every thread, a call of this layer that
 * would wait answers -1 or NULL with `HERO_NET_STOPPED` instead, one
 * already waiting at once. An `accept` answers so even with a
 * connection waiting, so a loop of accepts ends under load, and so does a
 * `connect` whose connection the system cannot make at once; a `recv` with
 * bytes already there and a `send` the system has room for still move them,
 * so a stopping program can finish what it holds. A second one while the
 * program stops ends it at once, as the signal would have.
 *
 * What runs in the signal is the layer's own C, which writes one byte down
 * a pipe the layer's own thread watches, and that thread wakes every
 * waiting call: no code of the program, no allocation. A signal the program inherited ignored stays
 * ignored (a job a shell starts with `&` has SIGINT so). Any other handler
 * of the two is replaced, among them the runtime's own that passes the
 * signal on to a child the program waits on: once the stop is caught, such
 * a child is not sent the signal and runs to its end, whichever was first,
 * the stop caught or the child's launch. A name being looked up for
 * `hero_net_connect` is not ended by a stop: the lookup runs to its end, and
 * the connection after it meets the stop as any later call does. A signal
 * sent to `heroes run` is passed on to the program, which then has about
 * two seconds before `heroes` kills it. On Windows the console closing ends
 * the process within five seconds whatever it does (Microsoft's
 * HandlerRoutine page; unrun).
 *
 * Answers 0, also where the stop was already caught, or -1 where the system
 * gave no pipe or refused the handler, and with `HERO_NET_INVALID` where
 * `hero_net_count_signal` counts SIGINT or SIGTERM: the two are the stop's or
 * a count's for the process's life, never both. */
int64_t hero_net_catch_stop(void);

/* 1 once a stop has been asked, by a signal after `hero_net_catch_stop` or
 * by the program's own `hero_net_ask_stop`, else 0: a loop that works between
 * the layer's calls asks it between its steps, and a thread that joins a
 * worker asks it after. A reader, as the two below: it changes nothing they
 * answer. */
int64_t hero_net_stop_asked(void);

/* THE STOP THE PROGRAM ASKS ITSELF (panel 248's R4): from this call, on every
 * thread, a call of this layer that would wait answers -1 or NULL with
 * `HERO_NET_STOPPED`, one already waiting at once, and `hero_net_stop_asked`
 * answers 1, as after the first signal `hero_net_catch_stop` catches; a call
 * that need not wait still moves what it can, as there. It cannot fail, and
 * a second call changes nothing. It is asked from any thread, a connection's
 * among them, but never from a signal handler: it takes a lock.
 *
 * KEPT APART FROM THE SIGNALS: it takes no signal and counts as none. Before
 * `hero_net_catch_stop`, or with none, Ctrl-C and SIGTERM end the program at
 * once, as ever; after it, the first of them after an asked stop asks the
 * stop again, which is asked already, and the program goes on, and the
 * second ends it at once, so one Ctrl-C never ends a program finishing the
 * stop it asked. On Windows it is the one way a program asks its own stop:
 * no signal it can raise reaches the console's handler. */
void hero_net_ask_stop(void);

/* CTRL-C OR SIGTERM COUNTED, AND NEVER THE STOP (feature 2669): what a shell
 * or a REPL does with Ctrl-C, which reaches a child it runs in front and must
 * leave the program running, the second time as the first. From this call
 * `sig`, SIGINT or SIGTERM, neither ends the program nor asks the stop: each
 * one that comes is counted by the runtime's signal door, `hero_os.h`'s
 * `hero_signal_take` giving the count since its last call and
 * `hero_signal_fd` readable once one came. What runs in the signal is the
 * runtime's own C (panel 246's R4), and `hero_signal_watch` still refuses the
 * two (panel 247's R7): they are this layer's to hand over, and this is
 * where it hands them.
 *
 * THE STOP AND THE COUNT ARE ONE OR THE OTHER for the process's life, so no
 * Ctrl-C is a stop to one reader and a count to another: -1 with
 * `HERO_NET_INVALID` once `hero_net_catch_stop` has caught the two, and
 * `hero_net_catch_stop` answers so once either is counted.
 * `hero_net_ask_stop` stays the program's own, as ever. A signal the program
 * inherited ignored stays ignored, as the stop leaves it: a job a shell
 * started with `&` hears no Ctrl-C. A child the program starts begins with
 * the signal at its default, as with any handler.
 *
 * On Windows SIGINT is Ctrl-C and Ctrl-Break, counted as SIGINT by a console
 * handler on the thread the system makes for each event, and the console
 * closing still ends the process; SIGTERM is `HERO_NET_UNSUPPORTED`, since
 * nothing sends a console program one. Compiled for both of Windows' targets
 * by zig, unrun on the box.
 *
 * Answers 0, also where `sig` is counted already; or -1: `HERO_NET_INVALID`
 * for a `sig` that is neither of the two, or where the stop is caught; and
 * the system's number where it gave no pipe or refused the handler. */
int64_t hero_net_count_signal(int32_t sig);

/* What a door taking a time in milliseconds takes for NO LIMIT:
 * `hero_net_set_deadline`, `hero_net_connect_within`, and
 * `hero_stream_net.h`'s `hero_stream_net_set_deadline`. 0 is then a time
 * already passed, so a call that would wait answers at once, and a time
 * below -1 is `HERO_NET_INVALID`: the shape of `hero_serial.h`'s timeouts
 * (`HERO_SERIAL_FOREVER`) and of `hero_ipc.h`'s `within_ms`
 * (`HERO_IPC_FOREVER`). Since 2026-11-20 (M-rebel-library, step 83a): until
 * then 0 was no limit and -1 was refused, so no time said "do not wait". */
#define HERO_NET_FOREVER (-1)

/* THE DEADLINE (panel 248's R5, Go's `SetDeadline` set from now): from this
 * call, every call on `s` that waits, an `accept`, a `recv` or a `send`,
 * waits until `ms` milliseconds from now at the latest, then answers -1 or
 * NULL with `HERO_NET_TIMED_OUT`. It is a time, not a span: it applies to
 * calls already waiting, which take it at once, and to every later one, so
 * a peer that sends one byte before each limit still meets it. A call that
 * need not wait is not ended by it: bytes already there are received and
 * room already free is used, a deadline passed or not. 0 IS A DEADLINE
 * ALREADY PASSED: from this call every call on `s` that would wait answers
 * `HERO_NET_TIMED_OUT` at once, those already waiting among them.
 * `HERO_NET_FOREVER` takes the deadline away; a `ms` below -1 is
 * `HERO_NET_INVALID`. It is kept with the socket, so it stays when the
 * socket crosses to another thread as its number. The time is read on the
 * clock `hero_time.h` names, which never goes back. Answers 0, or -1 with
 * `HERO_NET_NOT_OPEN` where `s` names no open socket. */
int64_t hero_net_set_deadline(hero_net_socket *s, int64_t ms);

/* The kind of the last call's failure on this thread, `HERO_NET_OK` after a
 * call that did not fail, and the platform's own number for it (0 where the
 * layer itself refused). */
int64_t hero_net_last_error(void);
int64_t hero_net_last_os_error(void);

/* -- DATAGRAMS, NAMES, AND A SOCKET'S TWO ADDRESSES (M-rebel-library) -------
 *
 * Added 2026-11-15 for Rebel, the language's library. Everything above
 * stands as it was, its names, its signatures and what it answers.
 *
 * AN ADDRESS AS TEXT, in every call below, is a number: `127.0.0.1`, `::1`,
 * with the scope the system writes after a `%` where the address has one
 * (`fe80::1%lo0`). Never a name: `hero_net_lookup` gives a name's numbers.
 *
 * TEXT THIS HEADER WRITES ENDS WITH A ZERO BYTE THE ANSWER DOES NOT COUNT,
 * and is written whole or not at all, never cut. An address is at most
 * `HERO_NET_ADDRESS_MOST` bytes with that byte, so a call that writes one
 * takes a buffer of at least so many, and refuses a shorter one
 * `HERO_NET_INVALID` before it does anything else: no datagram is taken
 * whose sender could not be told. A count, as the counts below are: a group
 * declares it `u64`. */
#define HERO_NET_ADDRESS_MOST UINT64_C(64)

/* A datagram socket (UDP) bound to `address` (a number, or "" for every
 * address of the machine, the first the system lists) at `port`, 0 asking
 * the system for a free one, which `hero_net_port` reads back. A name is
 * `HERO_NET_ADDRESS`. It is a socket as every other: `hero_net_close` gives
 * it back, it crosses to a thread as its number, and it takes a deadline.
 * `hero_net_send`, `hero_net_recv` and `hero_net_accept` are a connection's
 * and a listener's and refuse it `HERO_NET_INVALID`, a `recv` being a call
 * that would cut a datagram in silence; the two calls below refuse a
 * connection or a listener the same way. */
hero_net_socket *hero_net_udp(const char *address, int64_t port);

/* A datagram socket as `hero_net_udp` makes it, standing on a port other
 * sockets may stand on too: it asks the system for that (`SO_REUSEADDR`, and
 * `SO_REUSEPORT` where the system has it), and a socket that asks the same
 * is let in whatever order the two bind. Every one of them hears a datagram
 * sent to a multicast group it joined, which is what mDNS's port 5353
 * needs beside the system's own responder; a datagram sent to one address
 * goes to one of them, the system's choice. A socket made by `hero_net_udp`
 * on that port, before or after, is refused `HERO_NET_IN_USE`, as an
 * exclusive bind always is. One bound to IPv6 hears IPv6 alone, never the IPv4
 * datagrams to its port. The same failures as `hero_net_udp`. */
hero_net_socket *hero_net_udp_shared(const char *address, int64_t port);

/* Sends the `count` bytes of `data` as one datagram to `address`, a number
 * of the socket's own family (IPv4 or IPv6), at `port`, and answers `count`:
 * a datagram goes whole or not at all, and one of no bytes is one. One
 * longer than the system sends as one fails `HERO_NET_TOO_LONG` (65,507
 * bytes is the most over IPv4); an address that is no number, or of the
 * other family, `HERO_NET_ADDRESS`; a port outside 1 to 65535
 * `HERO_NET_INVALID`. It waits only while the system has no room, as every
 * call waits: until there is, the deadline, the stop, or a close. Sent is
 * not received: the system says nothing of a datagram nobody took. */
int64_t hero_net_send_to(hero_net_socket *s, const void *data, uint64_t count, const char *address, int64_t port);

/* Receives one datagram into `buffer`, waiting for one, and answers how many
 * bytes it holds, 0 for a datagram of none; `address` and `*port` then say
 * who sent it (AN ADDRESS AS TEXT, above; an empty text and port 0 where
 * the system did not say).
 *
 * A DATAGRAM LONGER THAN `count` IS REPORTED, NEVER CUT IN SILENCE: the call
 * answers -1 with `HERO_NET_TOO_LONG`, `buffer` holds its first `count`
 * bytes and `address` and `*port` its sender all the same, and the rest of
 * it is gone, since no system keeps it. 65,535 bytes hold any datagram. */
int64_t hero_net_recv_from(hero_net_socket *s, void *buffer, uint64_t count, void *address, uint64_t address_count, int64_t *port);

/* THE NAMES. `hero_net_lookup` and `hero_net_name_of` are THE TWO CALLS OF
 * THIS HEADER THAT MAY BLOCK IN THE SYSTEM'S RESOLVER, for as long as it
 * takes to answer: seconds, where a name server does not. The layer has
 * nothing of its own to wait on there, the resolver being the system's call
 * and not a socket of the table, so no deadline reaches one and no stop
 * ends one in flight. A stop asked BEFORE the call answers
 * `HERO_NET_STOPPED` and the resolver is not asked, as a call that would
 * wait answers; one asked while it runs lets it run to its end, as the
 * lookup inside `hero_net_connect` does. A program that cannot wait so long
 * looks names up on a thread of its own.
 *
 * The addresses `name` has, IPv4 and IPv6, in the order the system gives
 * them, each AS TEXT and each ended by a newline. Answers the bytes the
 * whole list takes; a buffer of one byte more holds it all with its zero
 * byte. Into a shorter one the addresses are written in order, each whole,
 * as many as fit before that byte, so an answer of `count` or more says the
 * list was cut after the last address written. A name the resolver does not
 * know, or knows with no address, is `HERO_NET_NOT_FOUND`; a resolver that
 * could not answer now, a failure that may pass, `HERO_NET_UNREACHABLE`. A
 * number is a name too, and answers itself. The name is UTF-8, 1 to 1,024
 * bytes, else `HERO_NET_INVALID`. */
int64_t hero_net_lookup(const char *name, void *buffer, uint64_t count);

/* The reverse: the name the resolver has for `address`, a number. Answers
 * the bytes the name takes, and writes it where `count` is more than that,
 * so 1,025 bytes hold any; into a shorter buffer nothing is written. An
 * address the resolver has no name for is `HERO_NET_NOT_FOUND`, and what is
 * no number `HERO_NET_ADDRESS`. */
int64_t hero_net_name_of(const char *address, void *buffer, uint64_t count);

/* THE TWO ADDRESSES OF A SOCKET: the one it stands on itself,
 * `hero_net_local`, and the one of the peer it is connected to,
 * `hero_net_remote`. The text goes into `address` (AN ADDRESS AS TEXT,
 * above), the port into `*port`, and the answer is the text's bytes; or -1,
 * nothing written. A socket bound to every address says `0.0.0.0` or `::`
 * until it is connected. A socket with no peer, a listener, a datagram
 * socket or a connection never made, fails `hero_net_remote` with
 * `HERO_NET_RESET`. Neither waits, so neither is ended by the stop. */
int64_t hero_net_local(hero_net_socket *s, void *address, uint64_t address_count, int64_t *port);
int64_t hero_net_remote(hero_net_socket *s, void *address, uint64_t address_count, int64_t *port);

/* -- ROUND 3: A CONNECT WITH A TIME LIMIT, ONE DIRECTION CLOSED, THE OPTIONS,
 * MULTICAST, AND THE MACHINE'S INTERFACES (M-rebel-library, 2026-11-16) ------
 *
 * Added for Rebel's `net`. Everything above stands as it was, its names and
 * its signatures, and one answer is corrected: `hero_net_connect` handed a
 * name the resolver does not know answers `HERO_NET_NOT_FOUND`, as
 * `hero_net_lookup` does, where it answered `HERO_NET_ADDRESS` until this
 * day; a name it knows with no address is `HERO_NET_NOT_FOUND` too, and every
 * other failure of the lookup inside it is what it was. */

/* A connection to `address` (a number or a name) at `port`, as
 * `hero_net_connect` makes it, made within `ms` milliseconds or given up:
 * NULL with `HERO_NET_TIMED_OUT` once that time passes with no connection
 * made, every address the name has tried in turn within the one time.
 * `HERO_NET_FOREVER` is no limit, `hero_net_connect` itself. 0 waits not at
 * all: the connection the system made by the time it is asked is answered,
 * and else `HERO_NET_TIMED_OUT` at once, no other address tried; whether a
 * system makes one so soon, even on its own loopback, is its own. A `ms`
 * below -1 is `HERO_NET_INVALID` (since 2026-11-20, `HERO_NET_FOREVER`'s
 * comment). The time bounds the connection being made, waited through the layer's own
 * thread as every wait; the name's lookup before it is the system resolver's
 * call, which no deadline ends (THE NAMES, above), so a program that must
 * bound that too hands a number. The connection made keeps no deadline: its
 * waits are as long as `hero_net_set_deadline` then says. */
hero_net_socket *hero_net_connect_within(const char *address, int64_t port, int64_t ms);

/* Closes one direction of the connection `s`, or both, keeping the socket:
 * READ, after which a `recv` answers 0, as at the peer's close, on every
 * platform; WRITE, after which the peer's `recv` answers 0 once it has read
 * what was sent, and a `send` here fails `HERO_NET_RESET`; BOTH, the two.
 * A direction at its end already, the peer's close read or shut before, is
 * shut again with no failure while the connection stands, on every platform
 * (Darwin refuses that shutdown `ENOTCONN`: the layer then asks whether the
 * socket still has a peer). A call another thread waits in on the socket
 * looks again at once. Answers
 * 0, or -1: `HERO_NET_INVALID` for a datagram socket or a `how` that is none
 * of the three, `HERO_NET_RESET` for a socket with no connection (a
 * listener, one the peer reset), `HERO_NET_NOT_OPEN`. `hero_net_close` is
 * still what gives the socket back. */
#define HERO_NET_SHUT_READ 1
#define HERO_NET_SHUT_WRITE 2
#define HERO_NET_SHUT_BOTH 3
int64_t hero_net_shutdown(hero_net_socket *s, int64_t how);

/* THE OPTIONS a socket is set with, each on (1) or off (0):
 *
 * NO_DELAY, a connection's or a listener's: bytes are sent as soon as they
 * are handed, never held to join later ones (`TCP_NODELAY`; off when a
 * socket is made, Nagle's way). KEEP_ALIVE, a connection's or a listener's:
 * the system asks an idle peer whether it is there, and a connection whose
 * peer is gone then fails (`SO_KEEPALIVE`, at the system's own intervals;
 * off when made). A listener's are given to the connections it accepts on
 * Darwin and Windows and not on Linux (unrun on the three), so a program
 * sets them on each connection. BROADCAST, a datagram socket's: datagrams
 * may be sent to a broadcast address (`SO_BROADCAST`; off when made, a
 * send to one then failing `HERO_NET_DENIED`).
 *
 * THE MULTICAST OPTIONS, a datagram socket's, IPv4's or IPv6's as the
 * socket is (added 2026-11-16, after the joins below met a machine with two
 * routes for multicast): MULTICAST_INTERFACE, the interface a datagram sent
 * to a group leaves by, by its index as `hero_net_join_group` takes it, 0
 * for the system's choice by its routes, as when the socket is made
 * (`IP_MULTICAST_IF`, `IPV6_MULTICAST_IF`; Darwin's `IP_MULTICAST_IFINDEX`;
 * read back as this layer last set it, since Linux answers an address);
 * MULTICAST_LOOP, 1 where a datagram sent to a group the sending machine
 * has joined comes back to it, as when made, or 0; MULTICAST_HOPS, 0 to
 * 255, how many routers a datagram to a group may cross, IPv4's time to
 * live, 1 when made.
 *
 * `hero_net_set_option` answers 0, `hero_net_option` the value; or -1,
 * `HERO_NET_INVALID` for an option the socket's kind has not or a value out
 * of the option's range (0 or 1, else as said above), `HERO_NET_NOT_OPEN`.
 * Neither waits. */
#define HERO_NET_OPTION_NO_DELAY 1
#define HERO_NET_OPTION_KEEP_ALIVE 2
#define HERO_NET_OPTION_BROADCAST 3
#define HERO_NET_OPTION_MULTICAST_INTERFACE 4
#define HERO_NET_OPTION_MULTICAST_LOOP 5
#define HERO_NET_OPTION_MULTICAST_HOPS 6
int64_t hero_net_set_option(hero_net_socket *s, int64_t option, int64_t value);
int64_t hero_net_option(hero_net_socket *s, int64_t option);

/* MULTICAST: the datagram socket `s` joins the group `group`, a multicast
 * address AS TEXT of the socket's own family, on the interface whose index
 * is `interface_index` (`hero_net_interfaces` gives them), 0 letting the system
 * choose; from then it receives the datagrams sent to the group at the port
 * it is bound to. `hero_net_leave_group` leaves it. An interface named by
 * its index by RFC 3678's protocol-independent calls (`MCAST_JOIN_GROUP`,
 * `MCAST_LEAVE_GROUP`), the same on IPv4 and IPv6 and on the three
 * platforms; index 0 by the classic ones (`IP_ADD_MEMBERSHIP` with any
 * interface, `IPV6_JOIN_GROUP` with 0), the system choosing by its routes,
 * since Darwin's `MCAST_JOIN_GROUP` refuses index 0. Answers 0, or -1:
 * `HERO_NET_ADDRESS` where `group` is no number of the socket's family or no
 * multicast address, `HERO_NET_INVALID` for a connection or a listener or an
 * index outside 0 to 2^32 - 1, the system's own kind where it refuses
 * (joining a group twice, leaving one not joined, an interface with no
 * multicast), `HERO_NET_NOT_OPEN`. A datagram sent to a group the sender has
 * joined comes back to it, the system's default. */
int64_t hero_net_join_group(hero_net_socket *s, const char *group, int64_t interface_index);
int64_t hero_net_leave_group(hero_net_socket *s, const char *group, int64_t interface_index);

/* THE MACHINE'S NETWORK INTERFACES and their addresses, as the system lists
 * them now (`getifaddrs`; `GetAdaptersAddresses` on Windows), into `buffer`.
 * Each entry is five fields, each ended by a zero byte, so a field may hold
 * any byte but zero: the interface's name (UTF-8; Windows' friendly name,
 * `Ethernet`), its index as decimal digits, its flags (the bits below) as
 * decimal digits, one address AS TEXT, and that address's prefix length in
 * bits as decimal digits. An interface with several addresses has one entry
 * for each, in the system's order; one with none has one entry whose
 * address is empty and whose prefix is 0. Answers how many bytes the whole
 * list is: an answer greater than `count` says the buffer was short, nothing
 * in it then to be read, and the caller asks again with at least that many,
 * as interfaces may come between two calls. -1 where the system cannot list
 * them, `HERO_NET_LIMIT` where Windows' list is longer than the 64 KiB this
 * layer carries of it. Never waits, so the stop does not end it. */
#define HERO_NET_INTERFACE_UP 1
#define HERO_NET_INTERFACE_RUNNING 2
#define HERO_NET_INTERFACE_LOOPBACK 4
#define HERO_NET_INTERFACE_BROADCAST 8
#define HERO_NET_INTERFACE_MULTICAST 16
#define HERO_NET_INTERFACE_POINT_TO_POINT 32
int64_t hero_net_interfaces(void *buffer, uint64_t count);

/* -- ROUND 4: AN ECHO ASKED BY ICMP, WITHOUT PRIVILEGES (M-rebel-library,
 * 2026-11-17) ----------------------------------------------------------------
 *
 * Added for Rebel's `net.ping`. Everything above stands as it was, its names,
 * its signatures and what it answers; one kind is added, which no call above
 * answers. */

/* The system has no such thing for a program: no ICMP socket a program
 * without privileges may make, of that family, on this machine. */
#define HERO_NET_UNSUPPORTED 15

/* The most data bytes one echo carries: an IPv4 datagram's 65,535 less its
 * 20-byte header and the echo's 8, the same bound for IPv6. A system may
 * refuse fewer, `HERO_NET_TOO_LONG` (Darwin refuses 9,000 to a program
 * without privileges, measured 2026-11-17 with its own `ping -s 9000`). */
#define HERO_NET_PING_MOST UINT64_C(65507)

/* ONE ECHO: an ICMP echo request (IPv4's type 8, IPv6's 128) carrying the
 * `count` bytes of `data` and the sequence number `sequence`, 0 to 65535,
 * sent to `address`, a number (AN ADDRESS AS TEXT, above), and its reply
 * waited for, `ms` milliseconds at most, 1 to 2,147,483,647: a ping always
 * has a limit, since a host that never answers would hold the call for
 * ever. Answers the bytes of the reply, its 8-byte ICMP header and its
 * data, as the system's `ping` counts them; `from` then holds the address
 * that replied AS TEXT, `*ttl` the reply's time to live (IPv4) or hop
 * limit (IPv6), 0 to 255, or -1 where the system did not say, and
 * `*nanoseconds` the round trip on the clock a deadline is read on, which
 * never goes back, from just before the request left to the reply read.
 * Or -1, nothing to be read in `from`, `*ttl` -1 and `*nanoseconds` 0:
 * `HERO_NET_INVALID` for a `sequence`, `count`, `ms` or `from_count` out of
 * its range (`from_count` at least `HERO_NET_ADDRESS_MOST`), `data` NULL
 * with bytes behind it, or `ttl` or `nanoseconds` NULL; `HERO_NET_ADDRESS`
 * for what is no number; `HERO_NET_TIMED_OUT` where no reply came in time;
 * `HERO_NET_UNREACHABLE` where the system, or a router by an ICMP error
 * naming this echo, said the host cannot be reached or the echo expired on
 * its way; `HERO_NET_TOO_LONG` where the system refuses an echo so long;
 * `HERO_NET_DENIED` where the system refuses the program such a socket
 * (Linux, where the program's group is outside `net.ipv4.ping_group_range`,
 * which no group is by default on several distributions);
 * `HERO_NET_UNSUPPORTED` where the system has no such socket for the
 * family; `HERO_NET_STOPPED`, `HERO_NET_LIMIT`.
 *
 * NEVER A RAW SOCKET AND NEVER A PRIVILEGE: on Darwin and Linux a datagram
 * socket of the ICMP protocol (`SOCK_DGRAM` with `IPPROTO_ICMP`, or
 * `IPPROTO_ICMPV6`), which the system lets a program make where it allows
 * it at all; on Windows the system's echo calls (`IcmpSendEcho2`,
 * `Icmp6SendEcho2`), which ask none either. The reply is the one carrying
 * this echo's sequence and data, and on Darwin its identifier, which the
 * layer chooses from the process and the socket so two echoes of one
 * process at once are told apart; Linux writes its own identifier over the
 * request's and gives the socket only the replies bearing it. Windows
 * chooses the identifier and the sequence itself, `sequence` going nowhere
 * there, and gives no hop limit for IPv6.
 *
 * IT WAITS AS EVERY CALL OF THE LAYER WAITS on Darwin and Linux, through the
 * layer's own thread, so the stop ends it at once, `HERO_NET_STOPPED`. On
 * Windows the system's call waits, as the resolver does in THE NAMES above:
 * a stop asked before the call answers `HERO_NET_STOPPED` and sends
 * nothing, and one asked while it waits lets it run to its reply or its
 * time. No handle is made and none kept: the socket lives inside the call. */
int64_t hero_net_ping(const char *address, int64_t sequence, const void *data, uint64_t count, int64_t ms, void *from, uint64_t from_count, int64_t *ttl,
                      int64_t *nanoseconds);

/* -- FOR THE OTHER MEMBERS OF THE LAYER, AND BOUND BY NO HEROES GROUP
 * (M-rebel-library, 2026-11-16, for `hero_poll.h`) ---------------------------
 *
 * Called from C of the layer alone, as `hero_stream.h`'s section of the same
 * name is: a descriptor and a function of C are nothing a Heroes program can
 * be trusted to hand over. Everything above stands as it was. */

/* The descriptor of `s` as a number into `*descriptor` (POSIX's `int`, the
 * bits of Windows' `SOCKET`), HELD: the socket is marked in use as a call in
 * flight marks it, so a close by the program meanwhile ends its handle at
 * once and shuts it down, and the descriptor stays open until the hold is let
 * go, the last of the two closing it. A member that hands the descriptor to
 * the system for longer than one call holds it so, and no socket the system
 * makes meanwhile can take its number under that member. Answers the hold, a
 * number at or above 0 that `hero_net_unhold` takes once; or -1 with
 * `HERO_NET_NOT_OPEN`, nothing held. */
int64_t hero_net_hold(hero_net_socket *s, int64_t *descriptor);

/* One hold let go; a number no hold answered is passed over. */
void hero_net_unhold(int64_t hold);

/* `wake` called at every stop this layer hears, as soon as it hears it: on
 * the thread that asks it (`hero_net_ask_stop`), on the layer's readiness
 * thread for a signal caught after `hero_net_catch_stop` on POSIX, on the
 * console's own thread on Windows; never in a signal handler. It runs with
 * this layer's lock held, so it takes no lock a caller of this layer may hold,
 * allocates nothing and does not wait: it rings something a waiting thread
 * watches. At most four functions; one registered already is passed over.
 * Answers 0; or -1, nothing registered, with `HERO_NET_INVALID` for NULL, or
 * `HERO_NET_LIMIT` where four are, or on POSIX where the readiness thread that
 * hears a signal cannot be made. */
int64_t hero_net_when_stopped(void (*wake)(void));

/* ADOPTING A DESCRIPTOR A MANAGER HANDED THE PROGRAM (M-rebel-library, 2026-11-23,
 * for `hero_activation.h`): called from C of the layer alone, as the section
 * above is. A descriptor is no number a Heroes program can be trusted to
 * hand over, so no group binds it.
 *
 * The kinds `hero_net_adopt` writes. */
#define HERO_NET_ADOPTED_LISTENER 1
#define HERO_NET_ADOPTED_CONNECTION 2
#define HERO_NET_ADOPTED_DATAGRAM 3

/* The socket `descriptor` is (POSIX's `int`, the bits of Windows' `SOCKET`), a
 * handle of the table that `hero_net_close` gives back, its kind in `*kind`:
 * a stream socket that listens, a stream socket connected, or a datagram
 * socket, each of IPv4 or IPv6. The descriptor is ASKED what it is, never
 * assumed. On success the table owns the descriptor and closes it with the
 * handle, set close-on-exec and non-blocking as every socket the table holds
 * is; on failure NULL and the descriptor is untouched and still the caller's:
 * `HERO_NET_INVALID` for a number no descriptor can be or one that names
 * no socket (a file, say), `HERO_NET_NOT_OPEN` for a number that names
 * nothing, `HERO_NET_UNSUPPORTED` for a socket of another family (a local
 * one) or type, `HERO_NET_IN_USE` for one the table holds already, and
 * `HERO_NET_LIMIT` when the table is full, which closes the descriptor. A
 * stream socket neither listening nor connected is `HERO_NET_INVALID`.
 * A datagram socket crosses to a thread and takes a deadline as `hero_net_udp`'s
 * does. */
hero_net_socket *hero_net_adopt(int64_t descriptor, int64_t *kind);

#endif
