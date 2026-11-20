/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_stream_net.h — a socket as a stream: the one function that hands
 * back a `hero_stream` over a connected `hero_net_socket` (M-rebel-library;
 * spec § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h`
 * and `hero_stream.h` are: its function is defined in
 * `platform/hero_stream_net.c`, a member of the archive on every program's
 * link line, which the linker takes only into a program that calls it; a
 * group binds it by naming this header, `extern "hero_stream_net.h"`, with
 * no `link` word and no flag, and no unit reads it unless one of its groups
 * names it.
 *
 * A MEMBER OF ITS OWN, BETWEEN TWO THAT DO NOT KNOW EACH OTHER. `hero_net`
 * names no stream and `hero_stream` no socket, so a program that only reads
 * files links no socket, and one that uses sockets as sockets links no
 * stream. Only a program that calls the function below takes this member,
 * and with it the other two.
 *
 * NO CONDITIONAL BUT THE GUARD, AND NO PLATFORM TYPE: the two handles are
 * the two headers' own, which this one includes for their names. Its member
 * has one arm for every platform, since it calls the functions of those two
 * headers and nothing of any system.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT, and it is the stream's:
 * `hero_stream_last_error()` names its kind and
 * `hero_stream_last_os_error()` the platform's number, as after any call on
 * a stream.
 */

#ifndef HERO_STREAM_NET_H
#define HERO_STREAM_NET_H

#include "hero_net.h"
#include "hero_stream.h"

/* A stream over the connection `s`, of kind `HERO_STREAM_KIND_SOCKET`: its
 * read is `hero_net_recv`, its write `hero_net_send` and its close
 * `hero_net_close`.
 *
 * FROM THIS CALL THE SOCKET'S LIFE IS THE STREAM'S: `hero_stream_close`
 * closes the socket, once, and the program gives the socket back by no call
 * of its own. A group says so with the mark that hands a life into the
 * handle a call's result begins:
 *
 *     function hero_stream_net_of(s: Socket transfers hero_net_close) -> Stream acquires hero_stream_close
 *
 * NULL, AND THE SOCKET STAYS THE CALLER'S, open and untouched, where no
 * stream was made: `HERO_STREAM_NOT_OPEN` where `s` names no open socket,
 * `HERO_STREAM_INVALID` where it has no peer (a listener, a datagram
 * socket, a connection never made or reset already), `HERO_STREAM_LIMIT`
 * where the layer holds as many streams as it can.
 *
 * WHAT THE STREAM ANSWERS is what the socket would: a read waits for a byte
 * and answers 0 once the peer has closed its side, and a write may take
 * fewer bytes than it was handed. A failure's kind is the stream's nearest
 * word: `HERO_STREAM_NOT_OPEN`, `HERO_STREAM_INVALID`, `HERO_STREAM_LIMIT`
 * and `HERO_STREAM_DENIED` for the socket's kinds of those names, and
 * `HERO_STREAM_CLOSED_PEER` for a peer that closed or reset the connection,
 * in a read as in a write; `HERO_STREAM_TIMED_OUT` for a deadline passed and
 * `HERO_STREAM_STOPPED` for the stop asked. Every other kind is
 * `HERO_STREAM_OTHER`, and `hero_net_last_error()`, asked on the same thread
 * before any other call of that header, still names the socket's own.
 *
 * THE DEADLINE IS THE SOCKET'S, set by `hero_net_set_deadline` BEFORE this
 * call: a time and not a span, which once the socket is a stream only
 * `hero_stream_net_set_deadline` moves (below, since round 3; until
 * 2026-11-20 this sentence said nothing moved it). The stop reaches a read or a write waiting on the stream as it
 * reaches one waiting on the socket. */
hero_stream *hero_stream_net_of(hero_net_socket *s);

/* THE SOCKET UNDER THE STREAM (M-rebel-library, round 3, 2026-11-16): what a
 * program asks of a connection after it became a stream, when it holds no
 * socket any more. Each call takes a stream `hero_stream_net_of` made and
 * does to its socket what the `hero_net.h` call of the same name does,
 * answering what that call answers: `hero_stream_net_set_deadline` is
 * `hero_net_set_deadline`, so A DEADLINE MOVES AFTER THE SOCKET BECAME A
 * STREAM, a read or a write waiting on the stream taking the new one at
 * once, 0 a deadline already passed and `HERO_NET_FOREVER` none, a `ms`
 * below -1 refused (since 2026-11-20, `hero_net.h`'s `HERO_NET_FOREVER`);
 * `hero_stream_net_shutdown` is `hero_net_shutdown`;
 * `hero_stream_net_set_option` and `hero_stream_net_option` are
 * `hero_net_set_option` and `hero_net_option`.
 *
 * A FAILURE is -1 with the stream's kind and number, as every call on a
 * stream, the socket's own kind being `hero_net_last_error()` on the same
 * thread, as for a read: `HERO_STREAM_NOT_OPEN` where `s` is no open stream
 * or its socket is closed, `HERO_STREAM_INVALID` where it is a stream of
 * another kind (a file, a pipe, one another member made) or the socket's call
 * refuses an argument, and the rest as `hero_stream_net_of`'s comment maps
 * them. The socket is held while the call runs, so a close of the stream on
 * another thread meanwhile closes the socket when the call ends, never
 * under it. */
int64_t hero_stream_net_set_deadline(hero_stream *s, int64_t ms);
int64_t hero_stream_net_shutdown(hero_stream *s, int64_t how);
int64_t hero_stream_net_set_option(hero_stream *s, int64_t option, int64_t value);
int64_t hero_stream_net_option(hero_stream *s, int64_t option);

/* -- FOR THE OTHER MEMBERS OF THE LAYER, AND BOUND BY NO HEROES GROUP
 * (M-rebel-library, 2026-11-16, for `hero_poll.h`) ---------------------------
 *
 * Called from C of the layer alone, as `hero_stream.h`'s section of the same
 * name is. Everything above stands as it was. */

/* The socket under `s`, a stream `hero_stream_net_of` made, HELD as
 * `hero_stream_hold_payload` holds a payload: whatever closes the stream
 * meanwhile, its socket is not closed until `hero_stream_release(s)` is
 * called, once, on the same handle. What a member that waits on the socket
 * itself needs (`hero_poll.h`, which holds the socket's descriptor by
 * `hero_net_hold` and then lets this hold go). NULL with nothing held and the
 * stream's failure named: `HERO_STREAM_NOT_OPEN` where `s` is no open stream,
 * `HERO_STREAM_INVALID` where this member did not make it. */
hero_net_socket *hero_stream_net_socket(hero_stream *s);

#endif
