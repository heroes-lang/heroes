/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_stream_net.c — a socket as a stream, one member of the
 * archive the compiler puts on every program's link line (M-rebel-library;
 * `selfhost/cli/layer.hero` builds, keys and links it). Its header,
 * `hero_stream_net.h`, says what the function answers; this file says how.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so a definition
 * with another signature than the header's is `conflicting types` where it
 * is compiled.
 *
 * ONE ARM FOR EVERY PLATFORM. Nothing here names a system: the member calls
 * `hero_net.h`'s functions and `hero_stream.h`'s, each of which has its own
 * arms beneath, so there is no `_WIN32` in this file to keep in step.
 *
 * THE PAYLOAD IS THE SOCKET'S HANDLE ITSELF, AND THIS MEMBER HOLDS NO MEMORY
 * AND NO STATE. A handle of `hero_net` is a value that layer issues and
 * checks, a slot and a life in one pointer's bits, never an address anyone
 * reads through. So the stream keeps the handle where another member would
 * keep a block of its own: nothing is allocated (the runtime allocates in
 * `parts/alloc.c` alone), nothing survives between calls, and a socket
 * closed under the stream by some other route is `HERO_NET_NOT_OPEN` to the
 * stream's next call, never a read of freed memory.
 *
 * THE SOCKET IS UNTOUCHED UNTIL THE STREAM EXISTS. The one thing asked of it
 * first is whether it has a peer, which changes nothing; `hero_stream_make`
 * either hands back the stream or leaves the payload its caller's. So a
 * NULL answer always leaves the socket open and the program's, which is
 * what the `transfers` mark of the header's binding promises a program.
 */

#include "hero_stream_net.h"

#include <stddef.h>

/* The stream's word for a socket's kind of failure; a kind with no word of
 * the stream's is `HERO_STREAM_OTHER`, and `hero_net_last_error()` still
 * names it. */
static int64_t hero_stream_net_kind(int64_t kind) {
    switch (kind) {
    case HERO_NET_INVALID:
        return HERO_STREAM_INVALID;
    case HERO_NET_NOT_OPEN:
        return HERO_STREAM_NOT_OPEN;
    case HERO_NET_RESET:
        return HERO_STREAM_CLOSED_PEER;
    case HERO_NET_LIMIT:
        return HERO_STREAM_LIMIT;
    case HERO_NET_DENIED:
        return HERO_STREAM_DENIED;
    case HERO_NET_TIMED_OUT:
        return HERO_STREAM_TIMED_OUT;
    case HERO_NET_STOPPED:
        return HERO_STREAM_STOPPED;
    default:
        return HERO_STREAM_OTHER;
    }
}

/* The socket's last failure told as the stream's, which a function of the
 * table owes before it answers -1. Never `HERO_STREAM_OK`: a kind with no
 * word of the stream's, the socket's `HERO_NET_OK` among them, is
 * `HERO_STREAM_OTHER`. */
static void hero_stream_net_failed(void) {
    hero_stream_set_error(hero_stream_net_kind(hero_net_last_error()), hero_net_last_os_error());
}

static int64_t hero_stream_net_read(void *payload, void *buffer, uint64_t count) {
    int64_t got = hero_net_recv((hero_net_socket *)payload, buffer, count);

    if (got < 0) {
        hero_stream_net_failed();
    }
    return got;
}

static int64_t hero_stream_net_write(void *payload, const void *data, uint64_t count) {
    int64_t sent = hero_net_send((hero_net_socket *)payload, data, count);

    if (sent < 0) {
        hero_stream_net_failed();
    }
    return sent;
}

/* The stream's close is the socket's: called once, by `hero_stream.h`'s
 * word, on the thread that leaves the stream last. */
static int64_t hero_stream_net_close(void *payload) {
    int64_t closed = hero_net_close((hero_net_socket *)payload);

    if (closed < 0) {
        hero_stream_net_failed();
    }
    return closed;
}

/* The table every stream over a socket is made with: it stays where it is
 * for as long as any of them is open, and nothing writes it. */
/* No `waitable`: a wait reaches a socket's stream through the socket's own
 * hold (`hero_stream_net_socket`, then `hero_net_hold`), which keeps its
 * descriptor open for the wait whatever closes the stream. */
static const hero_stream_ops hero_stream_net_ops = { hero_stream_net_read, hero_stream_net_write, hero_stream_net_close, NULL };

hero_stream *hero_stream_net_of(hero_net_socket *s) {
    char peer[HERO_NET_ADDRESS_MOST];
    int64_t port = 0;

    /* Whether it is a connection, asked of the socket's own layer by the
     * one call that tells one from a listener and from a datagram socket:
     * only a connection has a peer. It reads the socket and changes
     * nothing. */
    if (hero_net_remote(s, peer, sizeof peer, &port) < 0) {
        hero_stream_set_error(hero_net_last_error() == HERO_NET_NOT_OPEN ? HERO_STREAM_NOT_OPEN : HERO_STREAM_INVALID, hero_net_last_os_error());
        return NULL;
    }
    /* `hero_stream_make` names its own failure, and then the payload is
     * still its caller's: the socket, untouched. */
    return hero_stream_make((void *)s, &hero_stream_net_ops, HERO_STREAM_KIND_SOCKET);
}

/* -- the socket under the stream (round 3, 2026-11-16) -------------------- */

/* The socket under `s`, held as `hero_stream_hold_payload` holds it until
 * `hero_stream_release`; NULL with the stream's failure named, nothing held,
 * where `s` is no stream this member made. */
static hero_net_socket *hero_stream_net_held(hero_stream *s) {
    void *payload = NULL;

    if (hero_stream_hold_payload(s, &hero_stream_net_ops, &payload) != 0) {
        return NULL;
    }
    return (hero_net_socket *)payload;
}

/* What a call on the held socket answered, the stream's failure named where
 * it failed; the hold ended either way, after the socket's number was read,
 * since the release may close the socket on this thread. */
static int64_t hero_stream_net_answer(hero_stream *s, int64_t answer) {
    int64_t kind = hero_net_last_error();
    int64_t os = hero_net_last_os_error();

    (void)hero_stream_release(s);
    if (answer < 0) {
        hero_stream_set_error(hero_stream_net_kind(kind), os);
        return -1;
    }
    hero_stream_set_error(HERO_STREAM_OK, 0);
    return answer;
}

int64_t hero_stream_net_set_deadline(hero_stream *s, int64_t ms) {
    hero_net_socket *socket = hero_stream_net_held(s);

    if (socket == NULL) {
        return -1;
    }
    return hero_stream_net_answer(s, hero_net_set_deadline(socket, ms));
}

int64_t hero_stream_net_shutdown(hero_stream *s, int64_t how) {
    hero_net_socket *socket = hero_stream_net_held(s);

    if (socket == NULL) {
        return -1;
    }
    return hero_stream_net_answer(s, hero_net_shutdown(socket, how));
}

int64_t hero_stream_net_set_option(hero_stream *s, int64_t option, int64_t value) {
    hero_net_socket *socket = hero_stream_net_held(s);

    if (socket == NULL) {
        return -1;
    }
    return hero_stream_net_answer(s, hero_net_set_option(socket, option, value));
}

int64_t hero_stream_net_option(hero_stream *s, int64_t option) {
    hero_net_socket *socket = hero_stream_net_held(s);

    if (socket == NULL) {
        return -1;
    }
    return hero_stream_net_answer(s, hero_net_option(socket, option));
}

/* -- for the other members of the layer (2026-11-16, for `hero_poll.c`) ---- */

/* The held socket itself, handed to the member that asked: its hold is that
 * member's to end with `hero_stream_release`. */
hero_net_socket *hero_stream_net_socket(hero_stream *s) {
    return hero_stream_net_held(s);
}
