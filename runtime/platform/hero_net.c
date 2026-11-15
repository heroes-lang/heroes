/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_net.c — the platform layer's TCP sockets, one member of the
 * archive the compiler puts on every program's link line (panel 234's R6;
 * feature 2295; `selfhost/cli/layer.hero` builds, keys and links it). Its
 * header, `hero_net.h`, says what each function answers; this file says how
 * each platform gets there.
 *
 * ITS OWN HEADER FIRST (the ffi-pragmatist's condition (2), held by the net's
 * `runtime` suite), so an arm that defines another signature than the
 * header's is `conflicting types` where it is compiled (F.2).
 *
 * A SOCKET IS A SLOT OF THE TABLE BELOW, NEVER AN ADDRESS THIS FILE READS
 * THROUGH. The handle a program holds is the slot's index and the life the
 * slot held when it was issued, so a handle closed already names a life that
 * ended and is `HERO_NET_NOT_OPEN`, as are NULL and a value this layer never
 * issued: a pointer that `close` freed could only be read after its free, and
 * a bare descriptor closed twice could close another socket that took its
 * number in between. The table is fixed, 65,536 sockets, since the runtime's
 * allocator is reached only through `parts/alloc.c` (the net's `runtime`
 * suite, rule 1), and its static bytes are touched only as sockets are made;
 * a program past it is told `HERO_NET_LIMIT`, as past the system's own.
 *
 * A CALL IN FLIGHT KEEPS ITS DESCRIPTOR. Each call marks its slot busy while
 * it uses the descriptor; a close meanwhile, from another thread, marks the
 * slot closing and wakes every call waiting on it, and the last call to
 * leave closes the descriptor. A descriptor is never closed while a call may
 * still pass its number to the system, which would then reach whatever
 * socket the system gave that number next.
 *
 * AND A CALL A CLOSE ENDED SAYS SO: `HERO_NET_NOT_OPEN`, never a peer's end
 * of stream nor a connection. What a shutdown does to a call blocked in the
 * system is the platform's and differs: it ends a waiting `recv` with 0 on
 * Darwin and Linux, read as the peer's close; it woke nothing waiting on a
 * LISTENER on Darwin, where the `accept` took a connection made after the
 * close (sitting 246's ffi-pragmatist), and woke it with `EINVAL` on Linux;
 * and on Windows it did not end a `recv` another thread waited in at all
 * (the box, clang 23.1.1, 2026-11-12).
 *
 * SO NO CALL BLOCKS IN THE SYSTEM, ON ANY PLATFORM, AND NONE RESTS ON WHAT A
 * SHUTDOWN DOES THERE. Every socket the table holds is non-blocking; `send`,
 * `recv` and `accept` each try first, and where the system says it would
 * block, the call waits on a condition variable of its own, under the
 * table's lock, until something wakes it: its socket ready, a close of it,
 * or the stop. A call that ends on a slot closed meanwhile answers
 * `HERO_NET_NOT_OPEN`, a connection an `accept` took after the close closed
 * again. A wait has no other bound: a `recv` waits for bytes, an `accept`
 * for a connection and a `send` for room, as long as they take.
 *
 * ONE POLLER FOR THE PROCESS TELLS A WAITING CALL ITS SOCKET IS READY (panel
 * 248's R2, provisional; defect 2502, which measured the slices this
 * replaces: 100 threads waiting 5 s cost 273 million instructions and ten
 * thousand involuntary switches, woken twenty times a second each). It is one
 * thread of this layer's own, made at the first wait, that every signal is
 * blocked on, and it waits in the system on every socket a call waits on:
 * Darwin's `kqueue` and Linux's `epoll`, each socket armed once per wait,
 * one-shot, so a socket that stays ready wakes it once; and on Windows,
 * whose `WSAPoll` watches sockets alone (an event's handle is refused,
 * `WSAENOTSOCK`, the box), a `WSAPoll` over every waiting socket and the
 * reading end of a loopback socket pair, which a call that begins to wait
 * writes one byte down so the poller reads the table again. A wake is a
 * hint, never an answer: a call woken tries its system call again, and one
 * that would still block waits again. A close and the stop never go
 * through the system: the closer, or the stop, wakes the waiting calls
 * itself, so a close ends a waiting call at once on every platform, and a
 * listener on Darwin no differently from a connection. The poller costs one
 * descriptor on POSIX and two sockets on Windows, never one per waiting call:
 * every descriptor of the process is one the program's C libraries spend
 * too (panel 248's ffi-pragmatist: under a soft limit of 256 a pipe per
 * waiting call left a server 83 connections, the poller 249, the slices
 * 250).
 *
 * WHERE THE POLLER CANNOT BE MADE, THE CALLS WAIT IN SLICES, AS BEFORE: no
 * descriptor or thread for it (asked again at the next wait), a socket the
 * system will not arm, or on Windows a loopback pair it cannot make, which
 * happens where the machine's loopback is down (libevent's caveat for its
 * own pair). A call then waits in slices of `HERO_NET_SLICE_MS` (50 ms) by
 * `poll` (`WSAPoll`), asking between them whether the program closed the
 * socket or the stop was asked: within about one slice, every answer the
 * same. And on Windows a `connect` checks its socket between slices
 * whatever stands: `WSAPoll` did not report a refused connection on the
 * Windows of before version 2004, and the system's answer, read after each
 * slice, is the one way to hear of it there.
 *
 * THE STOP (panel 246's R4, the ffi-pragmatist's mechanism; `hero_net.h`
 * says what it promises), AND THE STOP THE PROGRAM ASKS ITSELF (panel 248's
 * R4): `hero_net_ask_stop` sets a flag of its own, every wait asks it beside
 * the signals' count, and the count never sees it. `hero_net_catch_stop` makes a pipe and takes
 * SIGINT and SIGTERM, or Windows' console events. The first of them counts
 * itself in `hero_net_stop_asks` and writes one byte down the pipe, which
 * nobody ever reads, so its read end stays readable from then on: the poller
 * watches it, one-shot, and wakes every waiting call, and a call waiting in
 * slices watches it beside its socket. On Windows the console's handler
 * runs on a thread the system makes for it, where a lock may be taken, and
 * wakes the waiting calls itself. No code of the program runs in a signal:
 * the handler is this file's C, and it touches lock-free atomics, `getpid`,
 * `write`, `sigaction` and `kill`, each safe in a signal (C11 7.14.1.1p5;
 * POSIX's list of async-signal-safe functions); a Heroes function there that
 * allocates kills its program (the sitting's critic, item 12).
 *
 * A SEND MAY SEND FEWER BYTES THAN IT WAS HANDED, AND SAYS HOW MANY: on a
 * non-blocking socket the system takes what its buffer has room for and the
 * call answers that count at once, waiting only while the buffer has no room
 * at all; the caller sends the rest (`hero_net.h`).
 *
 * A SOCKET CROSSES INTO A THREAD AS A NUMBER, AND THE NUMBER IS COUNTED
 * (panel 246's R6, the compiler-engineer's two doors with the condition the
 * sitting put on them). `hero_net_number` marks the slot given up, so no
 * handle finds it, and answers the handle's value with a bit no handle
 * carries; `hero_net_of` finds the slot by the number, clears the mark and
 * gives the slot a new life, so the handle given up, and any copy of it,
 * names nothing after. The number begins a life of its own in the
 * program's handle set (`hero_handle_acquired_at`, the runtime's), owed to
 * `hero_net_of`, which ends it: with doors that did not, the sitting's
 * critic measured a number dropped and its socket left open passing the
 * leak check at exit 0, plain and under `--sanitize`. Both doors do it
 * under the table's lock, so a number is in the set before any `hero_net_of`
 * can find its slot marked.
 */

#include "hero_net.h"

/* The handle set the program's leak check reads, which a number joins. */
#include "heroes_runtime.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
/* The library the member needs, named by the member: the linker reads it only
 * where it takes this member, so a program that calls no socket function
 * links no `ws2_32`. MSVC's word; a MinGW target links it by `-lws2_32`. */
#if defined(_MSC_VER)
#pragma comment(lib, "ws2_32.lib")
#endif
typedef SOCKET hero_net_fd;
typedef int hero_net_len;
#define HERO_NET_NO_FD INVALID_SOCKET
#define HERO_NET_SEND_FLAGS 0
#else
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
typedef int hero_net_fd;
typedef socklen_t hero_net_len;
#define HERO_NET_NO_FD (-1)
/* A write to a peer that has gone raises SIGPIPE, which ends the process, and
 * the runtime does not ignore it. Darwin stops it per socket, Linux per call
 * (F.0; this Mac's SDK defines `MSG_NOSIGNAL` too, and `man 2 send` here names
 * only the socket's option, which is what Darwin's arm uses). */
#if defined(SO_NOSIGPIPE)
#define HERO_NET_SEND_FLAGS 0
#elif defined(MSG_NOSIGNAL)
#define HERO_NET_SEND_FLAGS MSG_NOSIGNAL
#else
#error "hero_net.c: this platform stops SIGPIPE neither per socket nor per call; a write to a closed peer would end the program"
#endif
/* The poller's system call: Linux's `epoll`, the BSDs' and Darwin's `kqueue`;
 * on any other POSIX system no poller, and every wait in slices. */
#if defined(__linux__)
#include <sys/epoll.h>
#define HERO_NET_EPOLL 1
#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#include <sys/event.h>
#define HERO_NET_KQUEUE 1
#endif
#endif

/* A handle carries a slot and a life in one pointer's 64 bits:
 * `(life << HERO_NET_INDEX_BITS) | (index + 1)`, 17 bits for an index plus
 * one up to 65,536 and the life above them, so no handle is NULL. */
_Static_assert(sizeof(void *) == 8 && sizeof(uintptr_t) == 8, "a socket handle carries its slot and its life in 64 bits");
#define HERO_NET_INDEX_BITS 17
#define HERO_NET_INDEX_MASK ((UINT64_C(1) << HERO_NET_INDEX_BITS) - 1)
/* The last life a slot is given, 2^45 - 1: a handle stays below 2^62, and a
 * slot that takes a new life for every socket and every number taken back
 * reaches it after eleven years at a hundred thousand a second; a slot at
 * it is never issued again. Until 2026-11-13 a life was 32 bits, which a
 * socket taken back as a number every time could spend in a day. */
#define HERO_NET_LIFE_MOST ((UINT64_C(1) << 45) - 1)
/* The bit a number carries and no handle does, so a number is positive and
 * never a handle's value. */
#define HERO_NET_NUMBERED (UINT64_C(1) << 62)
/* The stop's handler reads and writes an `_Atomic int`, which C allows in a
 * signal only where it is lock-free (C11 7.14.1.1p5). */
_Static_assert(ATOMIC_INT_LOCK_FREE == 2, "the stop's handler touches lock-free atomics alone");

#define HERO_NET_SLOTS 65536u
_Static_assert(HERO_NET_SLOTS <= HERO_NET_INDEX_MASK, "an index plus one fits below a handle's life");
/* The most one call sends or receives, which Windows' `int` counts can say. */
#define HERO_NET_CHUNK ((uint64_t)INT32_MAX)
/* How long a call waiting in slices sleeps before it asks again whether its
 * socket was closed or the stop asked, where the poller cannot be made: either
 * ends it within about this. */
#define HERO_NET_SLICE_MS 50
/* How long Windows' console handler holds the process after the console
 * closed, so `main` can return first: under the 5,000 ms after which the
 * system ends the process anyway (HandlerRoutine's page). */
#define HERO_NET_CLOSING_MS 4500
/* What a waiting call waits for, as a bit: bytes to read or a connection to
 * accept, and room to send or a connection made. */
#define HERO_NET_READING 1u
#define HERO_NET_WRITING 2u

/* A condition variable, which a waiting call sleeps on under the table's
 * lock: POSIX's, or Windows' own, which sleeps on an SRW lock. */
#if defined(_WIN32)
typedef CONDITION_VARIABLE hero_net_cond;
#else
typedef pthread_cond_t hero_net_cond;
#endif

/* A call waiting for its socket, on the waiting thread's own stack, in its
 * slot's list for what it waits for while it waits, and out of it before it
 * returns: so whoever wakes it, under the table's lock, never reaches a
 * frame that has returned. */
typedef struct hero_net_waiter {
    struct hero_net_waiter *next;
    struct hero_net_waiter *prev;
    hero_net_cond wake;
    /* The poller said the socket is ready. */
    bool ready;
} hero_net_waiter;

typedef struct {
    hero_net_fd fd;
    /* The life the slot holds or last held, which its handle names; a slot
     * whose life reached `HERO_NET_LIFE_MOST` is never issued again. */
    uint64_t life;
    /* Calls in flight on its descriptor; on Windows the poller counts as one
     * while its `WSAPoll` holds the socket. */
    uint32_t busy;
    /* Windows' alone: the poller's round holds it, one of `busy` (defect
     * 2554). */
    bool polled;
    /* The index plus one of the next slot given back, 0 for none. */
    uint32_t next;
    /* The calls waiting on it: [0] to read or accept, [1] to send or connect. */
    hero_net_waiter *waiting[2];
    /* What the poller's system call is armed for on POSIX, the
     * `HERO_NET_READING` and `HERO_NET_WRITING` bits: set by a waiting call
     * that arms it, cleared when the one-shot fires or the last call waiting
     * for it leaves. */
    uint8_t armed;
    /* The deadline (panel 248's R5): the time on `hero_net_now`'s clock at
     * which a wait on the socket ends `HERO_NET_TIMED_OUT`, 0 for none. Kept
     * here, so it crosses with the socket's number to a connection's thread
     * and needs no second value. */
    int64_t deadline;
    /* It holds a socket the program has not closed. */
    bool held;
    /* The program closed it while a call was in flight: the last one closes
     * the descriptor. */
    bool closing;
    /* Its socket was given up for a number no `hero_net_of` has taken back:
     * no handle finds it. */
    bool numbered;
} hero_net_slot;

static hero_net_slot hero_net_slots[HERO_NET_SLOTS];
/* Every slot at or above it has never held a socket. */
static uint32_t hero_net_fresh;
/* The index plus one of the last slot given back, 0 for none. */
static uint32_t hero_net_given_back;

static _Thread_local int64_t hero_net_kind;
static _Thread_local int64_t hero_net_os;

#if defined(_WIN32)
static SRWLOCK hero_net_lock = SRWLOCK_INIT;
/* `WSAStartup` has answered, under the lock, once for the process. */
static bool hero_net_up;
#else
static pthread_mutex_t hero_net_lock = PTHREAD_MUTEX_INITIALIZER;
#endif

static void hero_net_enter(void) {
#if defined(_WIN32)
    AcquireSRWLockExclusive(&hero_net_lock);
#else
    (void)pthread_mutex_lock(&hero_net_lock);
#endif
}

static void hero_net_leave(void) {
#if defined(_WIN32)
    ReleaseSRWLockExclusive(&hero_net_lock);
#else
    (void)pthread_mutex_unlock(&hero_net_lock);
#endif
}

/* The platform's number for the failure the last system call reported. */
static int64_t hero_net_os_code(void) {
#if defined(_WIN32)
    return (int64_t)WSAGetLastError();
#else
    return (int64_t)errno;
#endif
}

/* Whether that failure was a signal, after which the call is asked again. */
static bool hero_net_interrupted(int64_t code) {
#if defined(_WIN32)
    return code == WSAEINTR;
#else
    return code == EINTR;
#endif
}

/* The kind a platform's number is, the same kind on every platform. */
static int64_t hero_net_kind_of(int64_t code) {
    switch (code) {
#if defined(_WIN32)
    case WSAEADDRINUSE:
        return HERO_NET_IN_USE;
    case WSAEADDRNOTAVAIL:
    case WSAEAFNOSUPPORT:
        return HERO_NET_ADDRESS;
    case WSAECONNREFUSED:
        return HERO_NET_REFUSED;
    case WSAECONNRESET:
    case WSAECONNABORTED:
    case WSAESHUTDOWN:
    case WSAENOTCONN:
        return HERO_NET_RESET;
    case WSAETIMEDOUT:
        return HERO_NET_TIMED_OUT;
    case WSAENETUNREACH:
    case WSAEHOSTUNREACH:
    case WSAENETDOWN:
    case WSAEHOSTDOWN:
        return HERO_NET_UNREACHABLE;
    case WSAEMFILE:
    case WSAENOBUFS:
    case WSA_NOT_ENOUGH_MEMORY:
        return HERO_NET_LIMIT;
    case WSAEACCES:
        return HERO_NET_DENIED;
#else
    case EADDRINUSE:
        return HERO_NET_IN_USE;
    case EADDRNOTAVAIL:
    case EAFNOSUPPORT:
        return HERO_NET_ADDRESS;
    case ECONNREFUSED:
        return HERO_NET_REFUSED;
    case ECONNRESET:
    case ECONNABORTED:
    case ESHUTDOWN:
    case ENOTCONN:
    case EPIPE:
        return HERO_NET_RESET;
    case ETIMEDOUT:
        return HERO_NET_TIMED_OUT;
    case ENETUNREACH:
    case EHOSTUNREACH:
    case ENETDOWN:
    case EHOSTDOWN:
        return HERO_NET_UNREACHABLE;
    case EMFILE:
    case ENFILE:
    case ENOBUFS:
    case ENOMEM:
        return HERO_NET_LIMIT;
    case EACCES:
    case EPERM:
        return HERO_NET_DENIED;
#endif
    default:
        return HERO_NET_OTHER;
    }
}

static void hero_net_fail(int64_t kind, int64_t os) {
    hero_net_kind = kind;
    hero_net_os = os;
}

static void hero_net_failed_with(int64_t code) {
    hero_net_fail(hero_net_kind_of(code), code);
}

static void hero_net_clear(void) {
    hero_net_fail(HERO_NET_OK, 0);
}

static int hero_net_close_fd(hero_net_fd fd) {
#if defined(_WIN32)
    return closesocket(fd);
#else
    return close(fd);
#endif
}

/* Ends a call blocked in the system on the socket, before the descriptor can
 * close: none is, but one waiting in slices where the poller could not be
 * made, which the shutdown then ends at once on Darwin and Linux. */
static void hero_net_shut(hero_net_fd fd) {
#if defined(_WIN32)
    (void)shutdown(fd, SD_BOTH);
#else
    (void)shutdown(fd, SHUT_RDWR);
#endif
}

/* A descriptor the system just made, readied: never inherited by a program
 * the runtime spawns, and on Darwin never raising SIGPIPE. False, the error
 * set, where it could not be. */
static bool hero_net_ready(hero_net_fd fd) {
#if defined(_WIN32)
    if (!SetHandleInformation((HANDLE)fd, HANDLE_FLAG_INHERIT, 0)) {
        hero_net_fail(HERO_NET_OTHER, (int64_t)GetLastError());
        return false;
    }
#else
    if (fcntl(fd, F_SETFD, FD_CLOEXEC) != 0) {
        hero_net_failed_with(hero_net_os_code());
        return false;
    }
#if defined(SO_NOSIGPIPE)
    int one = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &one, (hero_net_len)sizeof one) != 0) {
        hero_net_failed_with(hero_net_os_code());
        return false;
    }
#endif
#endif
    return true;
}

static hero_net_socket *hero_net_handle(uint32_t index, uint64_t life) {
    return (hero_net_socket *)(uintptr_t)((life << HERO_NET_INDEX_BITS) | ((uint64_t)index + 1));
}

/* The slot a handle's value names while it holds the life the value was
 * issued for, and is neither closed nor given up for a number; NULL for any
 * other value. Under the lock. */
static hero_net_slot *hero_net_slot_of(uint64_t value) {
    uint64_t low = value & HERO_NET_INDEX_MASK;

    if (low == 0 || low > hero_net_fresh) {
        return NULL;
    }
    hero_net_slot *slot = &hero_net_slots[low - 1];

    if (!slot->held || slot->closing || slot->life != value >> HERO_NET_INDEX_BITS) {
        return NULL;
    }
    return slot;
}

/* The slot `s` names, as `hero_net_slot_of` finds it, and not given up for a
 * number. Under the lock. */
static hero_net_slot *hero_net_find(hero_net_socket *s) {
    hero_net_slot *slot = hero_net_slot_of((uint64_t)(uintptr_t)s);

    return slot != NULL && !slot->numbered ? slot : NULL;
}

/* The slot given back, its descriptor forgotten. Under the lock, with no call
 * in flight on it, so no call waits on it either. The system forgets what its
 * poller watched for the descriptor when the descriptor closes. */
static void hero_net_release(hero_net_slot *slot) {
    slot->held = false;
    slot->closing = false;
    slot->numbered = false;
    slot->armed = 0;
    slot->deadline = 0;
    slot->fd = HERO_NET_NO_FD;

    if (slot->life < HERO_NET_LIFE_MOST) {
        slot->next = hero_net_given_back;
        hero_net_given_back = (uint32_t)(slot - hero_net_slots) + 1;
    }
}

/* A socket the system made, given a slot and a new life; closed, and NULL
 * with `HERO_NET_LIMIT`, where every slot holds one. */
static hero_net_socket *hero_net_keep(hero_net_fd fd) {
    hero_net_socket *handle = NULL;
    uint32_t index = 0;
    bool room = true;

    hero_net_enter();
    if (hero_net_given_back != 0) {
        index = hero_net_given_back - 1;
        hero_net_given_back = hero_net_slots[index].next;
    } else if (hero_net_fresh < HERO_NET_SLOTS) {
        index = hero_net_fresh;
        hero_net_fresh += 1;
    } else {
        room = false;
    }
    if (room) {
        hero_net_slot *slot = &hero_net_slots[index];

        slot->life += 1;
        slot->fd = fd;
        slot->busy = 0;
        slot->polled = false;
        slot->next = 0;
        slot->waiting[0] = NULL;
        slot->waiting[1] = NULL;
        slot->armed = 0;
        slot->deadline = 0;
        slot->held = true;
        slot->closing = false;
        slot->numbered = false;
        handle = hero_net_handle(index, slot->life);
    }
    hero_net_leave();

    if (!room) {
        (void)hero_net_close_fd(fd);
        hero_net_fail(HERO_NET_LIMIT, 0);
        return NULL;
    }
    hero_net_clear();
    return handle;
}

/* The slot `s` names, marked busy, its descriptor in `fd`; NULL with
 * `HERO_NET_NOT_OPEN` where it names none. */
static hero_net_slot *hero_net_take(hero_net_socket *s, hero_net_fd *fd) {
    hero_net_enter();
    hero_net_slot *slot = hero_net_find(s);

    if (slot != NULL) {
        slot->busy += 1;
        *fd = slot->fd;
    }
    hero_net_leave();

    if (slot == NULL) {
        hero_net_fail(HERO_NET_NOT_OPEN, 0);
    }
    return slot;
}

/* One user of a slot gone, a call or the Windows poller: the descriptor to
 * close where the program closed the slot meanwhile and this was the last
 * user, else none. Under the lock. */
static hero_net_fd hero_net_let_go(hero_net_slot *slot) {
    hero_net_fd last = HERO_NET_NO_FD;

    slot->busy -= 1;
    if (slot->closing && slot->busy == 0) {
        last = slot->fd;
        hero_net_release(slot);
    }
    return last;
}

/* Where the Windows poller's round holds a slot the program closed, that
 * round ended and the slot let go; nothing on POSIX. Under the lock, which
 * the wait gives up while it sleeps; the slot is not read after it, since
 * whoever let it go last may have given it a new life. */
static void hero_net_unpolled(hero_net_slot *slot);

/* A call done with the slot it took, answering whether the program closed it
 * meanwhile; the last user of a slot closed meanwhile closes its
 * descriptor. A call that leaves a closed slot only the Windows poller still
 * holds waits for the poller's round to let it go, which closes it, so the
 * call returns with the socket closed, as it does on POSIX, where the poller
 * holds no slot and the call is the last user (defect 2554). */
static bool hero_net_done(hero_net_slot *slot) {
    hero_net_enter();
    bool closed = slot->closing;
    hero_net_fd last = hero_net_let_go(slot);

    if (closed && last == HERO_NET_NO_FD && slot->busy == 1) {
        hero_net_unpolled(slot);
    }
    hero_net_leave();

    if (last != HERO_NET_NO_FD) {
        (void)hero_net_close_fd(last);
    }
    return closed;
}

/* Whether the program closed the slot a call holds busy. */
static bool hero_net_closing(hero_net_slot *slot) {
    hero_net_enter();
    bool closed = slot->closing;
    hero_net_leave();
    return closed;
}

/* -- the stop's state (panel 246's R4) ----------------------------------- */

/* Whether `hero_net_catch_stop` has taken the signals, 1 once it has: written
 * under `hero_net_lock`, so two first calls arm once. */
static _Atomic int hero_net_stop_armed;
/* The signals, or Windows' console events, that came since: 0 none, 1 the
 * program stops, more ends it. The pipe below is how a wait hears of it at
 * once; this and the program's own ask, below, are what every wait asks. */
static _Atomic int hero_net_stop_asks;
/* 1 once the program asked its own stop (`hero_net_ask_stop`, panel 248's
 * R4). KEPT APART FROM THE SIGNALS' COUNT: the handler counts signals alone,
 * so the first Ctrl-C after an asked stop is still the first and the process
 * goes on, and only a second ends it; with the ask counted as the first
 * request, a user's one Ctrl-C would end a server draining its own stop
 * (measured on three systems by the sitting's ffi-pragmatist). */
static _Atomic int hero_net_stop_own;

#if !defined(_WIN32)
/* The pipe's two ends, -1 until the stop is armed: the handler writes down
 * `hero_net_stop_out`, the poller and every wait in slices watch
 * `hero_net_stop_in`. */
static _Atomic int hero_net_stop_in = -1;
static _Atomic int hero_net_stop_out = -1;
/* The process that armed the stop. A child forked before it runs a program
 * shares the pipe and the handler until its `exec`, and a signal reaching it
 * there must end it as the signal would, never stop its parent. */
static _Atomic int hero_net_stop_owner = -1;
#endif

/* Whether the stop was asked, by a signal or by the program. */
static bool hero_net_stopping(void) {
    return atomic_load(&hero_net_stop_asks) > 0 || atomic_load(&hero_net_stop_own) != 0;
}

/* A descriptor made to wait never in the system's own calls, or made to
 * again (below, with the waits). */
static bool hero_net_blocking(hero_net_fd fd, bool blocking);

/* -- the clock a deadline is read on (panel 248's R5) -------------------- */

/* Nanoseconds on a clock that never goes back, as `hero_time.h`'s, which
 * this member reads itself so a program of sockets alone takes no clock
 * member: Darwin's `CLOCK_UPTIME_RAW`, the clock its timed waits count on,
 * Linux's `CLOCK_MONOTONIC`, neither advancing while the machine sleeps, and
 * Windows' performance counter, whose resolution is under a microsecond
 * where `GetTickCount64` steps by 10 to 16 ms (Microsoft's two pages, read
 * by the sitting's ffi-pragmatist, who measured that step as a deadline's
 * worst lateness). -1 where it cannot be read or does not fit 64 bits. */
static int64_t hero_net_now(void) {
    int64_t whole = 0;
    int64_t part = 0;
    int64_t per = 0;
#if defined(_WIN32)
    LARGE_INTEGER count;
    LARGE_INTEGER frequency;

    if (!QueryPerformanceFrequency(&frequency) || !QueryPerformanceCounter(&count) || frequency.QuadPart <= 0 || count.QuadPart < 0) {
        return -1;
    }
    whole = count.QuadPart / frequency.QuadPart;
    part = count.QuadPart % frequency.QuadPart;
    per = frequency.QuadPart;
#else
    struct timespec t;
#if defined(__APPLE__)
    int read = clock_gettime(CLOCK_UPTIME_RAW, &t);
#else
    int read = clock_gettime(CLOCK_MONOTONIC, &t);
#endif

    if (read != 0 || t.tv_sec < 0 || t.tv_nsec < 0) {
        return -1;
    }
    whole = (int64_t)t.tv_sec;
    part = (int64_t)t.tv_nsec;
    per = INT64_C(1000000000);
#endif
    int64_t seconds = 0;
    int64_t scaled = 0;
    int64_t total = 0;

    if (__builtin_mul_overflow(whole, INT64_C(1000000000), &seconds) || __builtin_mul_overflow(part, INT64_C(1000000000), &scaled) ||
        __builtin_add_overflow(seconds, scaled / per, &total)) {
        return -1;
    }
    return total;
}

/* -- waking a waiting call (panel 248's R2) ------------------------------ */

/* A condition variable made for one wait; false where the system refused,
 * and the call then waits in slices. Linux's counts a timed wait on the
 * deadline's own clock. */
static bool hero_net_cond_made(hero_net_cond *c) {
#if defined(_WIN32)
    InitializeConditionVariable(c);
    return true;
#elif defined(__APPLE__)
    return pthread_cond_init(c, NULL) == 0;
#else
    pthread_condattr_t clocked;

    if (pthread_condattr_init(&clocked) != 0) {
        return false;
    }
    bool made = pthread_condattr_setclock(&clocked, CLOCK_MONOTONIC) == 0 && pthread_cond_init(c, &clocked) == 0;

    (void)pthread_condattr_destroy(&clocked);
    return made;
#endif
}

static void hero_net_cond_ended(hero_net_cond *c) {
#if defined(_WIN32)
    (void)c;
#else
    (void)pthread_cond_destroy(c);
#endif
}

/* The lock let go and the calling thread asleep until `c` is signalled, the
 * lock taken again before it returns. It may return with nothing signalled,
 * so every caller asks again what it waits for. */
static void hero_net_cond_sleep(hero_net_cond *c) {
#if defined(_WIN32)
    (void)SleepConditionVariableSRW(c, &hero_net_lock, INFINITE, 0);
#else
    (void)pthread_cond_wait(c, &hero_net_lock);
#endif
}

/* As `hero_net_cond_sleep`, and at the latest until `deadline` on
 * `hero_net_now`'s clock, `left` nanoseconds from now, both positive. */
static void hero_net_cond_sleep_until(hero_net_cond *c, int64_t deadline, int64_t left) {
#if defined(_WIN32)
    (void)deadline;
    int64_t ms = left / 1000000 + 1;

    (void)SleepConditionVariableSRW(c, &hero_net_lock, ms >= (int64_t)INFINITE ? INFINITE - 1 : (DWORD)ms, 0);
#elif defined(__APPLE__)
    (void)deadline;
    struct timespec span = { (time_t)(left / 1000000000), (long)(left % 1000000000) };

    (void)pthread_cond_timedwait_relative_np(c, &hero_net_lock, &span);
#else
    (void)left;
    struct timespec at = { (time_t)(deadline / 1000000000), (long)(deadline % 1000000000) };

    (void)pthread_cond_timedwait(c, &hero_net_lock, &at);
#endif
}

static void hero_net_cond_signal(hero_net_cond *c) {
#if defined(_WIN32)
    WakeConditionVariable(c);
#else
    (void)pthread_cond_signal(c);
#endif
}

/* Every call waiting on the slot asked to look again at what it waits for: a
 * close, or the stop. Under the lock. */
static void hero_net_nudge(hero_net_slot *slot) {
    for (int way = 0; way < 2; way += 1) {
        for (hero_net_waiter *w = slot->waiting[way]; w != NULL; w = w->next) {
            hero_net_cond_signal(&w->wake);
        }
    }
}

/* Every call waiting on the slot told its socket may be ready, which the
 * call learns by trying again. Under the lock. */
static void hero_net_wake_ready(hero_net_slot *slot) {
    for (int way = 0; way < 2; way += 1) {
        for (hero_net_waiter *w = slot->waiting[way]; w != NULL; w = w->next) {
            w->ready = true;
            hero_net_cond_signal(&w->wake);
        }
    }
}

/* Every waiting call of the process asked to look again: the stop. Under the
 * lock. */
static void hero_net_nudge_all(void) {
    for (uint32_t i = 0; i < hero_net_fresh; i += 1) {
        hero_net_nudge(&hero_net_slots[i]);
    }
}

/* -- the poller (panel 248's R2) ---------------------------------------- */

#if defined(_WIN32)
/* The poller's state, written under the lock: 0 not made, 1 made. Once made it
 * runs until the process ends. */
static _Atomic int hero_net_poller_made;
/* The loopback pair the poller is woken through: it watches `hero_net_wake_in`
 * beside every waiting socket, and a call that begins to wait, or a close,
 * sends one byte down `hero_net_wake_out`. */
static _Atomic SOCKET hero_net_wake_in = INVALID_SOCKET;
static _Atomic SOCKET hero_net_wake_out = INVALID_SOCKET;
/* A byte is down the pair that the poller has not read: a second need not be
 * sent. Under the lock. */
static _Atomic int hero_net_wake_sent;
/* What the poller's `WSAPoll` watches, the pair's reading end first, then each
 * waiting socket, and the slot each entry stands for: read and written by the
 * poller's thread alone. */
static WSAPOLLFD hero_net_polled[HERO_NET_SLOTS + 1];
static uint32_t hero_net_polled_slot[HERO_NET_SLOTS + 1];
/* The rounds the poller has ended, each counted once it has let go of every
 * slot it held and closed every socket it was the last user of; and the
 * condition a close waits on for the round holding its socket to end. Under
 * the lock. */
static uint64_t hero_net_rounds;
static CONDITION_VARIABLE hero_net_round_ended = CONDITION_VARIABLE_INIT;

/* The poller woken to read the table again, once until it has. Under the
 * lock. */
static void hero_net_poke(void) {
    if (atomic_load(&hero_net_poller_made) == 0 || atomic_load(&hero_net_wake_sent) != 0) {
        return;
    }
    atomic_store(&hero_net_wake_sent, 1);
    (void)send(atomic_load(&hero_net_wake_out), "\1", 1, 0);
}

/* THE ROUND THAT HOLDS A CLOSED SOCKET, WAITED OUT (defect 2554). The poller
 * holds every socket it watches, one of the slot's users, since Winsock's
 * `closesocket` "must never" be issued "concurrently with another Winsock
 * function call" on the socket and its number "may immediately be reused"
 * (Microsoft's page for `closesocket`): so the last user out closes the
 * socket, and the poller can be that user. A close nudged a waiting `accept`,
 * which left first and answered `HERO_NET_NOT_OPEN`, and the listener went
 * on listening until the poller's round ended, a moment the scheduler picks,
 * a `shutdown` doing nothing to a listener ("does not close the socket",
 * `WSAENOTCONN` on one not connected, Microsoft's page for `shutdown`): a
 * connect made after the `accept` ended reached the port 16 times in 80 on
 * the box, at `-O0` and `-O2` alike. On POSIX the poller holds nothing, so
 * the call that leaves is the last user and the socket is closed before it
 * answers. So here the close, and a call leaving a closed slot only the
 * poller still holds, wake the poller and wait for its round to end; it
 * takes no closed slot into a later round. A byte that never reached it is
 * sent again each slice of the wait. */
static void hero_net_unpolled(hero_net_slot *slot) {
    if (!slot->polled) {
        return;
    }
    uint64_t round = hero_net_rounds;

    hero_net_poke();
    while (hero_net_rounds == round) {
        if (!SleepConditionVariableSRW(&hero_net_round_ended, &hero_net_lock, HERO_NET_SLICE_MS, 0)) {
            (void)send(atomic_load(&hero_net_wake_out), "\1", 1, 0);
        }
    }
}

/* A loopback pair of sockets in `pair`, both non-blocking and never inherited,
 * the accepted end checked to be the one this made: a listener on a port the
 * system chose, which another process could also reach, so the peer of the
 * accepted socket must be the connecting socket's own address (libevent's
 * `evutil_ersatz_socketpair` asks the same). False, nothing left open, where
 * any step fails. */
static bool hero_net_pair(SOCKET pair[2]) {
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKET out = INVALID_SOCKET;
    SOCKET in = INVALID_SOCKET;
    struct sockaddr_in at;
    struct sockaddr_in mine;
    struct sockaddr_in theirs;
    int size = (int)sizeof at;
    bool made = false;

    memset(&at, 0, sizeof at);
    at.sin_family = AF_INET;
    at.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (listener != INVALID_SOCKET && bind(listener, (struct sockaddr *)&at, (int)sizeof at) == 0 && listen(listener, 1) == 0 &&
        getsockname(listener, (struct sockaddr *)&at, &size) == 0) {
        out = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (out != INVALID_SOCKET && connect(out, (struct sockaddr *)&at, (int)sizeof at) == 0) {
            in = accept(listener, NULL, NULL);
        }
    }
    int mine_size = (int)sizeof mine;
    int theirs_size = (int)sizeof theirs;

    if (in != INVALID_SOCKET && getsockname(out, (struct sockaddr *)&mine, &mine_size) == 0 &&
        getpeername(in, (struct sockaddr *)&theirs, &theirs_size) == 0 && mine.sin_port == theirs.sin_port &&
        mine.sin_addr.s_addr == theirs.sin_addr.s_addr) {
        made = hero_net_ready(in) && hero_net_ready(out) && hero_net_blocking(in, false) && hero_net_blocking(out, false);
    }
    if (listener != INVALID_SOCKET) {
        (void)closesocket(listener);
    }
    if (!made) {
        if (in != INVALID_SOCKET) {
            (void)closesocket(in);
        }
        if (out != INVALID_SOCKET) {
            (void)closesocket(out);
        }
        return false;
    }
    pair[0] = in;
    pair[1] = out;
    return true;
}

/* THE POLLER'S THREAD, on Windows. Each round it reads the table under the
 * lock: every slot the program has not closed with a call waiting that the
 * poller has not woken goes into the `WSAPoll`, the poller holding it busy,
 * so no close can close the socket under the system's call; then it waits,
 * with no bound, until a socket is ready or a byte comes down the pair; then,
 * under the lock, it wakes the calls of each ready socket, lets each socket
 * go, and closes those the program closed meanwhile whose last user it was;
 * and then it counts the round ended, which a close waits for
 * (`hero_net_unpolled`). */
static DWORD WINAPI hero_net_poll_loop(LPVOID unused) {
    (void)unused;
    SOCKET wake = atomic_load(&hero_net_wake_in);

    for (;;) {
        ULONG count = 1;

        hero_net_enter();
        atomic_store(&hero_net_wake_sent, 0);
        hero_net_polled[0].fd = wake;
        hero_net_polled[0].events = POLLRDNORM;
        hero_net_polled[0].revents = 0;
        for (uint32_t i = 0; i < hero_net_fresh; i += 1) {
            hero_net_slot *slot = &hero_net_slots[i];
            short events = 0;

            for (int way = 0; way < 2; way += 1) {
                for (hero_net_waiter *w = slot->waiting[way]; w != NULL; w = w->next) {
                    if (!w->ready) {
                        events |= way == 0 ? POLLRDNORM : POLLWRNORM;
                    }
                }
            }
            if (events != 0 && slot->held && !slot->closing) {
                slot->busy += 1;
                slot->polled = true;
                hero_net_polled[count].fd = slot->fd;
                hero_net_polled[count].events = events;
                hero_net_polled[count].revents = 0;
                hero_net_polled_slot[count] = i;
                count += 1;
            }
        }
        hero_net_leave();

        int ready = WSAPoll(hero_net_polled, count, -1);

        if (hero_net_polled[0].revents != 0) {
            char drained[64];

            while (recv(wake, drained, (int)sizeof drained, 0) > 0) {
            }
        }
        hero_net_enter();
        for (ULONG k = 1; k < count; k += 1) {
            hero_net_slot *slot = &hero_net_slots[hero_net_polled_slot[k]];

            /* A failed round wakes every call it held, which each then tries
             * again: never a call left asleep on a socket nobody watches. */
            if (ready < 0 || hero_net_polled[k].revents != 0) {
                hero_net_wake_ready(slot);
            }
            slot->polled = false;
            hero_net_polled[k].fd = hero_net_let_go(slot);
        }
        hero_net_leave();
        for (ULONG k = 1; k < count; k += 1) {
            if (hero_net_polled[k].fd != INVALID_SOCKET) {
                (void)closesocket(hero_net_polled[k].fd);
            }
        }
        hero_net_enter();
        hero_net_rounds += 1;
        WakeAllConditionVariable(&hero_net_round_ended);
        hero_net_leave();
        if (ready < 0) {
            Sleep(HERO_NET_SLICE_MS);
        }
    }
}

/* The poller, made where it is not yet: true where it runs. Called without
 * the lock, since the pair's `connect` and `accept` go through the system's
 * loopback; a pair made by a thread that lost the race to another is
 * closed. */
static bool hero_net_poller_up(void) {
    if (atomic_load(&hero_net_poller_made) != 0) {
        return true;
    }
    SOCKET pair[2];

    if (!hero_net_pair(pair)) {
        return false;
    }
    hero_net_enter();
    bool up = atomic_load(&hero_net_poller_made) != 0;
    bool mine = false;

    if (!up) {
        atomic_store(&hero_net_wake_in, pair[0]);
        atomic_store(&hero_net_wake_out, pair[1]);
        HANDLE thread = CreateThread(NULL, 0, hero_net_poll_loop, NULL, 0, NULL);

        if (thread != NULL) {
            (void)CloseHandle(thread);
            atomic_store(&hero_net_poller_made, 1);
            mine = true;
        } else {
            atomic_store(&hero_net_wake_in, INVALID_SOCKET);
            atomic_store(&hero_net_wake_out, INVALID_SOCKET);
        }
    }
    hero_net_leave();

    if (!mine) {
        (void)closesocket(pair[0]);
        (void)closesocket(pair[1]);
    }
    return up || mine;
}

/* A slot's socket watched for `way`: on Windows the poller reads the table
 * again, which a byte down the pair asks of it. Under the lock. */
static bool hero_net_arm(hero_net_slot *slot, unsigned way) {
    (void)slot;
    (void)way;
    hero_net_poke();
    return true;
}

static void hero_net_disarm(hero_net_slot *slot, unsigned way) {
    (void)slot;
    (void)way;
}
#elif defined(HERO_NET_EPOLL) || defined(HERO_NET_KQUEUE)
/* The poller's `kqueue` or `epoll`, -1 until made; written under the lock, and
 * once made it runs until the process ends. */
static _Atomic int hero_net_poller = -1;

/* The key an armed descriptor carries back to the poller: its slot's index
 * plus one, and 0 for the stop's pipe. */
#define HERO_NET_STOP_KEY 0

/* `fd` armed in `q` for the `way` bits under `key`, one-shot: it fires once,
 * then waits to be armed again. Where it was armed for one way already,
 * `armed` names that way, which it stays armed for (`epoll` holds one entry a
 * descriptor, `kqueue` one a filter). False, `errno` set, where the system
 * refused. */
static bool hero_net_watch(int q, int fd, unsigned way, unsigned armed, uint64_t key) {
#if defined(HERO_NET_EPOLL)
    struct epoll_event e;
    unsigned all = way | armed;

    memset(&e, 0, sizeof e);
    e.events = EPOLLONESHOT | ((all & HERO_NET_READING) ? (EPOLLIN | EPOLLRDHUP) : 0) | ((all & HERO_NET_WRITING) ? EPOLLOUT : 0);
    e.data.u64 = key;
    /* An entry fired already stays, disarmed: armed again in place, or added
     * where the descriptor has none. */
    if (epoll_ctl(q, EPOLL_CTL_MOD, fd, &e) == 0) {
        return true;
    }
    return errno == ENOENT && epoll_ctl(q, EPOLL_CTL_ADD, fd, &e) == 0;
#else
    struct kevent e;

    (void)armed;
    EV_SET(&e, (uintptr_t)fd, way == HERO_NET_READING ? EVFILT_READ : EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, (void *)(uintptr_t)key);
    return kevent(q, &e, 1, NULL, 0, NULL) == 0;
#endif
}

/* `fd` no longer armed in `q` for `way`, still for `kept`. A failure leaves an
 * entry that wakes the poller once for nobody, which wakes no call. */
static void hero_net_unwatch(int q, int fd, unsigned way, unsigned kept, uint64_t key) {
#if defined(HERO_NET_EPOLL)
    (void)way;
    if (kept != 0) {
        (void)hero_net_watch(q, fd, kept, 0, key);
        return;
    }
    struct epoll_event e;

    memset(&e, 0, sizeof e);
    (void)epoll_ctl(q, EPOLL_CTL_DEL, fd, &e);
#else
    struct kevent e;

    (void)kept;
    EV_SET(&e, (uintptr_t)fd, way == HERO_NET_READING ? EVFILT_READ : EVFILT_WRITE, EV_DELETE, 0, 0, (void *)(uintptr_t)key);
    (void)kevent(q, &e, 1, NULL, 0, NULL);
#endif
}

/* THE POLLER'S THREAD, on POSIX: it waits in the system, with no bound, for
 * any armed descriptor to fire, then, under the lock, wakes every call
 * waiting on that socket and forgets what the one-shot disarmed; the stop's
 * pipe firing asks every waiting call to look again. A wait the system fails
 * (none is expected: a signal is blocked here) is tried again after a slice,
 * so a failure never spins. */
static void *hero_net_poll_loop(void *given) {
    int q = (int)(intptr_t)given;

    for (;;) {
        uint64_t keys[64];
        unsigned fired[64];
#if defined(HERO_NET_EPOLL)
        struct epoll_event events[64];
        int n = epoll_wait(q, events, 64, -1);

        for (int i = 0; i < n; i += 1) {
            keys[i] = events[i].data.u64;
            fired[i] = HERO_NET_READING | HERO_NET_WRITING;
        }
#else
        struct kevent events[64];
        int n = kevent(q, NULL, 0, events, 64, NULL);

        for (int i = 0; i < n; i += 1) {
            keys[i] = (uint64_t)(uintptr_t)events[i].udata;
            fired[i] = events[i].filter == EVFILT_WRITE ? HERO_NET_WRITING : HERO_NET_READING;
        }
#endif
        if (n < 0) {
            struct timespec pause = { 0, HERO_NET_SLICE_MS * 1000000L };

            (void)nanosleep(&pause, NULL);
            continue;
        }
        hero_net_enter();
        for (int i = 0; i < n; i += 1) {
            if (keys[i] == HERO_NET_STOP_KEY) {
                hero_net_nudge_all();
            } else if (keys[i] <= hero_net_fresh) {
                /* A slot given back and issued again since the system fired
                 * wakes its new calls for nothing, which try again. */
                hero_net_slot *slot = &hero_net_slots[keys[i] - 1];

                slot->armed &= (uint8_t)~fired[i];
                hero_net_wake_ready(slot);
            }
        }
        hero_net_leave();
    }
    return NULL;
}

/* The poller's thread started over `q`, the stop's pipe watched first where
 * the stop is armed already: false where either is refused. Under the lock,
 * so a stop armed meanwhile is either seen here or arms after `q` is
 * published and watches its pipe there. The thread starts with every signal
 * blocked, so no handler of the program or its libraries ever runs on it. */
static bool hero_net_poller_start(int q) {
    int stop = atomic_load(&hero_net_stop_in);

    if (stop >= 0 && !hero_net_watch(q, stop, HERO_NET_READING, 0, HERO_NET_STOP_KEY)) {
        return false;
    }
    sigset_t all;
    sigset_t was;
    pthread_attr_t attr;
    pthread_t thread;

    (void)sigfillset(&all);
    if (pthread_attr_init(&attr) != 0) {
        return false;
    }
    (void)pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    (void)pthread_sigmask(SIG_SETMASK, &all, &was);
    int made = pthread_create(&thread, &attr, hero_net_poll_loop, (void *)(intptr_t)q);
    (void)pthread_sigmask(SIG_SETMASK, &was, NULL);
    (void)pthread_attr_destroy(&attr);

    if (made != 0) {
        return false;
    }
    atomic_store(&hero_net_poller, q);
    return true;
}

/* The poller, made where it is not yet: true where it runs. Called without
 * the lock; its descriptor is made first, outside it, and one made by a
 * thread that lost the race to another is closed. */
static bool hero_net_poller_up(void) {
    if (atomic_load(&hero_net_poller) >= 0) {
        return true;
    }
#if defined(HERO_NET_EPOLL)
    int q = epoll_create1(EPOLL_CLOEXEC);
#else
    int q = kqueue();
#endif
    if (q < 0) {
        return false;
    }
    (void)fcntl(q, F_SETFD, FD_CLOEXEC);
    hero_net_enter();
    bool up = atomic_load(&hero_net_poller) >= 0;
    bool mine = !up && hero_net_poller_start(q);
    hero_net_leave();

    if (!mine) {
        (void)close(q);
    }
    return up || mine;
}

/* A slot's socket armed for `way` where it is not yet. Under the lock. */
static bool hero_net_arm(hero_net_slot *slot, unsigned way) {
    if ((slot->armed & way) != 0) {
        return true;
    }
    uint64_t key = (uint64_t)(slot - hero_net_slots) + 1;

    if (!hero_net_watch(atomic_load(&hero_net_poller), slot->fd, way, slot->armed, key)) {
        return false;
    }
    slot->armed |= (uint8_t)way;
    return true;
}

/* A slot's socket no longer armed for `way`, the last call waiting for it
 * gone. Under the lock. */
static void hero_net_disarm(hero_net_slot *slot, unsigned way) {
    if ((slot->armed & way) == 0) {
        return;
    }
    slot->armed &= (uint8_t)~way;
    hero_net_unwatch(atomic_load(&hero_net_poller), slot->fd, way, slot->armed, (uint64_t)(slot - hero_net_slots) + 1);
}

/* The stop's pipe watched by the poller where it runs. Under the lock. */
static bool hero_net_poller_hears_stop(int stop) {
    int q = atomic_load(&hero_net_poller);

    return q < 0 || hero_net_watch(q, stop, HERO_NET_READING, 0, HERO_NET_STOP_KEY);
}
#else
/* No poller on this system: every wait in slices. */
static bool hero_net_poller_up(void) {
    return false;
}

static bool hero_net_arm(hero_net_slot *slot, unsigned way) {
    (void)slot;
    (void)way;
    return false;
}

static void hero_net_disarm(hero_net_slot *slot, unsigned way) {
    (void)slot;
    (void)way;
}

static bool hero_net_poller_hears_stop(int stop) {
    (void)stop;
    return true;
}
#endif

#if !defined(_WIN32)
/* No poller on POSIX holds a slot: a `kqueue` or an `epoll` forgets a
 * descriptor when it closes, and its events carry the slot's life, so the
 * call that leaves a closed slot is its last user and closes it. */
static void hero_net_unpolled(hero_net_slot *slot) {
    (void)slot;
}
#endif

/* -- the stop (panel 246's R4) ------------------------------------------- */

#if defined(_WIN32)
/* Windows runs this on a thread of its own it makes for the event
 * (HandlerRoutine's page), so it may take the lock: the first event asks the
 * stop, wakes every waiting call, and answers TRUE, handled; a second answers
 * FALSE, and the system's own handler after it ends the process as the event
 * would have. The console closing ends the process when this returns, or 5 s
 * after the close: it waits first, so `main` returns, and ends the process
 * sooner. */
static BOOL WINAPI hero_net_on_console(DWORD event) {
    if (event != CTRL_C_EVENT && event != CTRL_BREAK_EVENT && event != CTRL_CLOSE_EVENT) {
        return FALSE;
    }
    if (atomic_fetch_add(&hero_net_stop_asks, 1) > 0) {
        return FALSE;
    }
    hero_net_enter();
    hero_net_nudge_all();
    hero_net_leave();
    if (event == CTRL_CLOSE_EVENT) {
        for (int waited = 0; waited < HERO_NET_CLOSING_MS; waited += HERO_NET_SLICE_MS) {
            Sleep(HERO_NET_SLICE_MS);
        }
    }
    return TRUE;
}

/* The handler taken, once; -1 with the system's number where it is refused.
 * Under the lock. A program that inherited Ctrl-C ignored (a process group
 * made for it) is not sent the event, which this leaves as it found. */
static int64_t hero_net_stop_arm(void) {
    if (!SetConsoleCtrlHandler(hero_net_on_console, TRUE)) {
        hero_net_fail(HERO_NET_OTHER, (int64_t)GetLastError());
        return -1;
    }
    atomic_store(&hero_net_stop_armed, 1);
    return 0;
}
#else
/* THE HANDLER, the stop's one piece of code a signal runs. The first signal
 * of the process that armed it counts itself and writes one byte; any other,
 * a second one or one reaching a forked child, restores the signal's own
 * end and sends the signal again, so the process ends as it would have. */
static void hero_net_on_stop(int sig) {
    int saved = errno;

    if ((int)getpid() == atomic_load(&hero_net_stop_owner) && atomic_fetch_add(&hero_net_stop_asks, 1) == 0) {
        char one = 1;
        ssize_t wrote = write(atomic_load(&hero_net_stop_out), &one, 1);

        (void)wrote;
    } else {
        struct sigaction plain = { 0 };

        plain.sa_handler = SIG_DFL;
        sigemptyset(&plain.sa_mask);
        (void)sigaction(sig, &plain, NULL);
        (void)kill(getpid(), sig);
    }
    errno = saved;
}

/* `sig` taken for the stop, what stood before kept in `was`; one the process
 * inherited ignored is left ignored, the shell's word. False, with `errno`,
 * where the system refused. Both signals are held while the handler runs,
 * so a second one waits for the first to have counted itself. */
static bool hero_net_stop_take(int sig, struct sigaction *was) {
    if (sigaction(sig, NULL, was) != 0) {
        return false;
    }
    if (!(was->sa_flags & SA_SIGINFO) && was->sa_handler == SIG_IGN) {
        return true;
    }
    struct sigaction now = { 0 };

    now.sa_handler = hero_net_on_stop;
    sigemptyset(&now.sa_mask);
    sigaddset(&now.sa_mask, SIGINT);
    sigaddset(&now.sa_mask, SIGTERM);
    /* Restarted: the pipe, never `EINTR`, is how a wait hears of the stop, so
     * every other call of the program and its libraries keeps its meaning. */
    now.sa_flags = SA_RESTART;
    return sigaction(sig, &now, NULL) == 0;
}

/* The pipe's ends given back and forgotten, where arming failed. */
static void hero_net_stop_unmade(int in, int out) {
    atomic_store(&hero_net_stop_in, -1);
    atomic_store(&hero_net_stop_out, -1);
    (void)close(in);
    (void)close(out);
}

/* The pipe made, both ends close-on-exec and non-blocking, published and
 * watched by the poller where it runs, then the two signals taken: a signal
 * that comes once they are taken finds the pipe watched. -1 with the
 * system's number where any of it is refused, nothing left taken. Under the
 * lock. */
static int64_t hero_net_stop_arm(void) {
    int ends[2];

    if (pipe(ends) != 0) {
        hero_net_failed_with(hero_net_os_code());
        return -1;
    }
    for (int i = 0; i < 2; i += 1) {
        int flags = fcntl(ends[i], F_GETFL);

        if (fcntl(ends[i], F_SETFD, FD_CLOEXEC) != 0 || flags < 0 || fcntl(ends[i], F_SETFL, flags | O_NONBLOCK) != 0) {
            int64_t code = hero_net_os_code();

            (void)close(ends[0]);
            (void)close(ends[1]);
            hero_net_failed_with(code);
            return -1;
        }
    }
    /* Published before the handler is, which reads them. */
    atomic_store(&hero_net_stop_in, ends[0]);
    atomic_store(&hero_net_stop_out, ends[1]);
    atomic_store(&hero_net_stop_owner, (int)getpid());

    if (!hero_net_poller_hears_stop(ends[0])) {
        int64_t code = hero_net_os_code();

        hero_net_stop_unmade(ends[0], ends[1]);
        hero_net_failed_with(code);
        return -1;
    }
    struct sigaction was_int;
    struct sigaction was_term;

    if (!hero_net_stop_take(SIGINT, &was_int)) {
        int64_t code = hero_net_os_code();

        hero_net_stop_unmade(ends[0], ends[1]);
        hero_net_failed_with(code);
        return -1;
    }
    if (!hero_net_stop_take(SIGTERM, &was_term)) {
        int64_t code = hero_net_os_code();

        (void)sigaction(SIGINT, &was_int, NULL);
        hero_net_stop_unmade(ends[0], ends[1]);
        hero_net_failed_with(code);
        return -1;
    }
    atomic_store(&hero_net_stop_armed, 1);
    return 0;
}
#endif

int64_t hero_net_catch_stop(void) {
    hero_net_enter();
    int64_t answer = atomic_load(&hero_net_stop_armed) != 0 ? 0 : hero_net_stop_arm();
    hero_net_leave();

    if (answer == 0) {
        hero_net_clear();
    }
    return answer;
}

int64_t hero_net_stop_asked(void) {
    return hero_net_stopping() ? 1 : 0;
}

/* THE STOP THE PROGRAM ASKS ITSELF (panel 248's R4): what the first signal
 * does after `hero_net_catch_stop`, asked from any thread with no signal and
 * no catch. The first ask wakes every waiting call under the lock, which
 * then answers `HERO_NET_STOPPED`, and writes one byte down the stop's pipe
 * where it stands, so a call waiting in slices beside it wakes at once too;
 * a later ask finds the flag set and does nothing. It takes the table's lock,
 * so it is no function for a signal handler: the handler's own stop is the
 * signal's. */
void hero_net_ask_stop(void) {
    if (atomic_exchange(&hero_net_stop_own, 1) == 0) {
        hero_net_enter();
        hero_net_nudge_all();
        hero_net_leave();
#if !defined(_WIN32)
        int out = atomic_load(&hero_net_stop_out);

        if (out >= 0) {
            char one = 1;
            ssize_t wrote = write(out, &one, 1);

            (void)wrote;
        }
#endif
    }
    hero_net_clear();
}

/* -- waiting ------------------------------------------------------------ */

/* A descriptor made to wait never in the system's own calls (`blocking`
 * false), or made to again; false, the error set, where it could not be. */
static bool hero_net_blocking(hero_net_fd fd, bool blocking) {
#if defined(_WIN32)
    u_long on = blocking ? 0 : 1;

    /* `FIONBIO` is an unsigned bit pattern the call takes as its `long`. */
    if (ioctlsocket(fd, (long)FIONBIO, &on) != 0) {
        hero_net_failed_with(hero_net_os_code());
        return false;
    }
#else
    int flags = fcntl(fd, F_GETFL);

    if (flags < 0 || fcntl(fd, F_SETFL, blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK)) != 0) {
        hero_net_failed_with(hero_net_os_code());
        return false;
    }
#endif
    return true;
}

/* How a wait ended. */
typedef enum {
    /* The system failed the wait, its number in `code`. */
    HERO_NET_WAIT_FAILED,
    /* A slice passed with nothing ready, where the call waits in slices. */
    HERO_NET_WAIT_PASSED,
    /* The socket may be ready for what the call needs, or the program closed
     * it: the call looks. */
    HERO_NET_WAIT_READY,
    /* The stop was asked, whatever else is ready. */
    HERO_NET_WAIT_STOPPED,
    /* The socket's deadline passed (panel 248's R5). */
    HERO_NET_WAIT_TIMED_OUT
} hero_net_waited;

/* Nanoseconds left to `deadline`, 0 where it has passed or the clock cannot
 * be read, which ends a wait rather than let it outlive its deadline. */
static int64_t hero_net_left(int64_t deadline) {
    int64_t now = hero_net_now();

    return now < 0 || now >= deadline ? 0 : deadline - now;
}

/* The deadline of a slot, read under the lock. */
static int64_t hero_net_deadline_of(hero_net_slot *slot) {
    hero_net_enter();
    int64_t deadline = slot->deadline;
    hero_net_leave();
    return deadline;
}

/* One slice of a wait for what a call needs, a connection or bytes to read
 * (`writing` false) or room to send or a connection made, beside the stop's
 * pipe where it is armed: the wait where the poller cannot be made. A slice
 * is cut short at the socket's deadline, `deadline` (0 for none). */
static hero_net_waited hero_net_wait_slice(hero_net_fd fd, bool writing, int64_t deadline, int64_t *code) {
    int timeout = HERO_NET_SLICE_MS;

    if (deadline != 0) {
        int64_t left = hero_net_left(deadline);

        if (left == 0) {
            return HERO_NET_WAIT_TIMED_OUT;
        }
        if (left / 1000000 + 1 < HERO_NET_SLICE_MS) {
            timeout = (int)(left / 1000000 + 1);
        }
    }
#if defined(_WIN32)
    WSAPOLLFD one = { fd, (short)(writing ? POLLWRNORM : POLLRDNORM), 0 };
    int ready = WSAPoll(&one, 1, timeout);
#else
    /* A negative descriptor, the stop unarmed, is one `poll` passes over. */
    struct pollfd two[2] = { { fd, (short)(writing ? POLLOUT : POLLIN), 0 }, { atomic_load(&hero_net_stop_in), POLLIN, 0 } };
    int ready = poll(two, 2, timeout);
#endif

    if (ready < 0) {
        *code = hero_net_os_code();
        return HERO_NET_WAIT_FAILED;
    }
    if (hero_net_stopping()) {
        return HERO_NET_WAIT_STOPPED;
    }
    if (ready == 0 && deadline != 0 && hero_net_left(deadline) == 0) {
        return HERO_NET_WAIT_TIMED_OUT;
    }
    return ready > 0 ? HERO_NET_WAIT_READY : HERO_NET_WAIT_PASSED;
}

/* A wait for what a call on `slot` needs: a connection or bytes to read
 * (`writing` false), or room to send or a connection made (`writing` true).
 *
 * THE CALL ARMS THE POLLER FOR ITS SOCKET AND SLEEPS ON ITS OWN CONDITION
 * VARIABLE, IN ITS SLOT'S LIST, UNDER THE LOCK. Whatever can end the wait is
 * a state set under the same lock before its wake is sent: the poller's
 * `ready`, a close's `closing`, the stop's count, the slot's deadline, which
 * bounds the sleep itself and is read again whenever the call wakes. The call joins the list
 * and asks each before it sleeps, and again whenever it wakes, so a wake sent
 * before it slept is never lost, and one with nothing behind it costs a
 * look. It leaves the list, disarming the socket where it was the last call
 * waiting for that, before it returns, so nobody reaches its frame after.
 * Where the poller cannot be made or the socket armed, the call waits one
 * slice instead. */
static hero_net_waited hero_net_wait(hero_net_slot *slot, hero_net_fd fd, bool writing, int64_t *code) {
    unsigned way = writing ? HERO_NET_WRITING : HERO_NET_READING;
    int list = writing ? 1 : 0;
    hero_net_waiter me;

    memset(&me, 0, sizeof me);
    if (!hero_net_poller_up() || !hero_net_cond_made(&me.wake)) {
        return hero_net_wait_slice(fd, writing, hero_net_deadline_of(slot), code);
    }
    hero_net_enter();
    if (!hero_net_arm(slot, way)) {
        int64_t deadline = slot->deadline;

        hero_net_leave();
        hero_net_cond_ended(&me.wake);
        return hero_net_wait_slice(fd, writing, deadline, code);
    }
    me.next = slot->waiting[list];
    if (me.next != NULL) {
        me.next->prev = &me;
    }
    slot->waiting[list] = &me;

    hero_net_waited result = HERO_NET_WAIT_READY;

    while (!me.ready && !slot->closing) {
        if (hero_net_stopping()) {
            result = HERO_NET_WAIT_STOPPED;
            break;
        }
        /* Read again at each wake: a deadline set while the call waits is
         * the one it keeps (Go's meaning, panel 248's R5). */
        int64_t deadline = slot->deadline;

        if (deadline == 0) {
            hero_net_cond_sleep(&me.wake);
            continue;
        }
        int64_t left = hero_net_left(deadline);

        if (left == 0) {
            result = HERO_NET_WAIT_TIMED_OUT;
            break;
        }
        hero_net_cond_sleep_until(&me.wake, deadline, left);
    }
    if (me.prev != NULL) {
        me.prev->next = me.next;
    } else {
        slot->waiting[list] = me.next;
    }
    if (me.next != NULL) {
        me.next->prev = me.prev;
    }
    if (slot->waiting[list] == NULL) {
        hero_net_disarm(slot, way);
    }
    hero_net_leave();
    hero_net_cond_ended(&me.wake);
    return result;
}

/* Whether a call on a non-blocking socket failed only because it would have
 * waited. */
static bool hero_net_would_wait(int64_t code) {
#if defined(_WIN32)
    return code == WSAEWOULDBLOCK;
#else
    return code == EAGAIN || code == EWOULDBLOCK;
#endif
}

/* Whether a `connect` on a non-blocking socket failed only because the
 * connection is still being made: POSIX says a signal does not abort one
 * either, and it goes on without the call. */
static bool hero_net_connecting(int64_t code) {
#if defined(_WIN32)
    return code == WSAEWOULDBLOCK || code == WSAEINPROGRESS;
#else
    return code == EINPROGRESS || code == EINTR;
#endif
}

/* Whether a listener's `accept` failed for a reason that passes: no
 * connection after all, a signal, or one reset before it was taken. */
static bool hero_net_passing(int64_t code) {
#if defined(_WIN32)
    return hero_net_would_wait(code) || code == WSAEINTR || code == WSAECONNRESET;
#else
    return hero_net_would_wait(code) || code == EINTR || code == ECONNABORTED;
#endif
}

/* The failure a connection being made has met, 0 for none yet: the
 * system's answer once the socket is writable, and on Windows the one way to
 * hear of a refusal `WSAPoll` did not report on the Windows of before
 * version 2004. */
static int64_t hero_net_pending_error(hero_net_fd fd) {
    int said = 0;
    hero_net_len size = (hero_net_len)sizeof said;

    if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (char *)&said, &size) != 0) {
        return hero_net_os_code();
    }
    return (int64_t)said;
}

/* Whether a socket being connected is writable now, its connection made or
 * failed: asked with no wait, since a wake is a hint. */
static bool hero_net_writable_now(hero_net_fd fd) {
#if defined(_WIN32)
    WSAPOLLFD one = { fd, POLLWRNORM, 0 };

    return WSAPoll(&one, 1, 0) > 0;
#else
    struct pollfd one = { fd, POLLOUT, 0 };

    return poll(&one, 1, 0) > 0;
#endif
}

/* -- making sockets ----------------------------------------------------- */

/* Windows asks a process to start its sockets once; elsewhere nothing to do. */
static bool hero_net_started(void) {
#if defined(_WIN32)
    WSADATA data;
    int said = 0;
    bool up = false;

    hero_net_enter();
    if (!hero_net_up) {
        said = WSAStartup(MAKEWORD(2, 2), &data);
        hero_net_up = said == 0;
    }
    up = hero_net_up;
    hero_net_leave();

    if (!up) {
        hero_net_fail(HERO_NET_OTHER, (int64_t)said);
    }
    return up;
#else
    return true;
#endif
}

/* A descriptor for one address `getaddrinfo` gave, readied and non-blocking,
 * or NO_FD with the platform's number in `code`. */
static hero_net_fd hero_net_made(const struct addrinfo *at, int64_t *code) {
    hero_net_fd fd = socket(at->ai_family, at->ai_socktype, at->ai_protocol);

    if (fd == HERO_NET_NO_FD) {
        *code = hero_net_os_code();
        return HERO_NET_NO_FD;
    }
    if (!hero_net_ready(fd) || !hero_net_blocking(fd, false)) {
        *code = hero_net_os;
        (void)hero_net_close_fd(fd);
        return HERO_NET_NO_FD;
    }
    return fd;
}

/* A descriptor listening on the address, or NO_FD with the platform's
 * number in `code`. A listener never blocks: its `accept` waits as every
 * call does. */
static hero_net_fd hero_net_try_listen(const struct addrinfo *at, int64_t *code) {
    hero_net_fd fd = hero_net_made(at, code);

    if (fd == HERO_NET_NO_FD) {
        return HERO_NET_NO_FD;
    }
    int one = 1;
#if defined(_WIN32)
    /* Windows' SO_REUSEADDR lets another socket take the port from under
     * this one; its exclusive use is POSIX's plain bind. */
    int failed = setsockopt(fd, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (const char *)&one, (hero_net_len)sizeof one);
#else
    /* A port a closed listener left in TIME_WAIT is taken again at once. */
    int failed = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, (hero_net_len)sizeof one);
#endif

    if (failed == 0) {
        failed = bind(fd, at->ai_addr, (hero_net_len)at->ai_addrlen);
    }
    if (failed == 0) {
        failed = listen(fd, SOMAXCONN);
    }
    if (failed != 0) {
        *code = hero_net_os_code();
        (void)hero_net_close_fd(fd);
        return HERO_NET_NO_FD;
    }
    return fd;
}

/* A connection to the address, kept in a slot, or NULL with the kind in
 * `kind` and the platform's number in `code`, `HERO_NET_STOPPED` where the
 * stop was asked while it was being made. The connection is asked for on a
 * non-blocking socket, which takes a slot of its own at once, a slot no
 * handle of the program names yet, so its wait is every call's wait and no
 * close can end it; it ends writable or failed, the system's own answer read
 * each time (`hero_net_pending_error`). On Windows the wait looks every
 * slice whatever stands, for the refusal its `WSAPoll` did not report before
 * version 2004. */
static hero_net_socket *hero_net_try_connect(const struct addrinfo *at, int64_t *kind, int64_t *code) {
    hero_net_fd fd = hero_net_made(at, code);

    if (fd == HERO_NET_NO_FD) {
        *kind = hero_net_kind_of(*code);
        return NULL;
    }
    bool at_once = connect(fd, at->ai_addr, (hero_net_len)at->ai_addrlen) == 0;

    if (!at_once) {
        *code = hero_net_os_code();
        if (!hero_net_connecting(*code)) {
            (void)hero_net_close_fd(fd);
            *kind = hero_net_kind_of(*code);
            return NULL;
        }
    }
    hero_net_socket *handle = hero_net_keep(fd);

    if (handle == NULL || at_once) {
        *kind = hero_net_kind;
        *code = hero_net_os;
        return handle;
    }
    hero_net_fd same = HERO_NET_NO_FD;
    hero_net_slot *slot = hero_net_take(handle, &same);
    bool made = false;

    *kind = HERO_NET_OK;
    for (;;) {
#if defined(_WIN32)
        hero_net_waited waited = hero_net_stopping() ? HERO_NET_WAIT_STOPPED : hero_net_wait_slice(fd, true, 0, code);
#else
        hero_net_waited waited = hero_net_stopping() ? HERO_NET_WAIT_STOPPED : hero_net_wait(slot, fd, true, code);
#endif

        if (waited == HERO_NET_WAIT_STOPPED) {
            *kind = HERO_NET_STOPPED;
            *code = 0;
            break;
        }
        if (waited == HERO_NET_WAIT_TIMED_OUT) {
            *kind = HERO_NET_TIMED_OUT;
            *code = 0;
            break;
        }
        if (waited == HERO_NET_WAIT_FAILED && !hero_net_interrupted(*code)) {
            *kind = hero_net_kind_of(*code);
            break;
        }
        int64_t said = hero_net_pending_error(fd);

        if (said != 0) {
            *code = said;
            *kind = hero_net_kind_of(said);
            break;
        }
        if (waited == HERO_NET_WAIT_READY && hero_net_writable_now(fd)) {
            made = true;
            break;
        }
    }
    (void)hero_net_done(slot);
    if (!made) {
        (void)hero_net_close(handle);
        return NULL;
    }
    return handle;
}

static hero_net_socket *hero_net_open(const char *address, int64_t port, bool listening) {
    char service[8];
    struct addrinfo hints;
    struct addrinfo *found = NULL;

    if (address == NULL || port < 0 || port > 65535 || (!listening && address[0] == '\0')) {
        hero_net_fail(HERO_NET_INVALID, 0);
        return NULL;
    }
    if (!hero_net_started()) {
        return NULL;
    }
    (void)snprintf(service, sizeof service, "%d", (int)port);
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV | (listening ? AI_PASSIVE : 0);

    int said = getaddrinfo(address[0] == '\0' ? NULL : address, service, &hints, &found);

    if (said != 0) {
        hero_net_fail(said == EAI_MEMORY ? HERO_NET_LIMIT : HERO_NET_ADDRESS, (int64_t)said);
        return NULL;
    }
    hero_net_socket *handle = NULL;
    int64_t kind = HERO_NET_OTHER;
    int64_t code = 0;

    for (const struct addrinfo *at = found; at != NULL && handle == NULL && kind != HERO_NET_STOPPED; at = at->ai_next) {
        if (listening) {
            hero_net_fd fd = hero_net_try_listen(at, &code);

            kind = fd == HERO_NET_NO_FD ? hero_net_kind_of(code) : HERO_NET_OK;
            if (fd != HERO_NET_NO_FD) {
                handle = hero_net_keep(fd);
                kind = hero_net_kind;
                code = hero_net_os;
            }
        } else {
            handle = hero_net_try_connect(at, &kind, &code);
        }
    }
    freeaddrinfo(found);

    if (handle == NULL) {
        hero_net_fail(kind, code);
        return NULL;
    }
    hero_net_clear();
    return handle;
}

hero_net_socket *hero_net_listen(const char *address, int64_t port) {
    return hero_net_open(address, port, true);
}

hero_net_socket *hero_net_connect(const char *address, int64_t port) {
    return hero_net_open(address, port, false);
}

hero_net_socket *hero_net_accept(hero_net_socket *listener) {
    hero_net_fd fd = HERO_NET_NO_FD;
    hero_net_slot *slot = hero_net_take(listener, &fd);

    if (slot == NULL) {
        return NULL;
    }
    hero_net_fd got = HERO_NET_NO_FD;
    int64_t code = 0;
    bool stopped = false;
    bool late = false;

    /* Tried first, then waited for where none is there; the stop is asked
     * before any connection waiting is taken, so a loop of accepts under load
     * still ends, and the close before each try. */
    while (!hero_net_closing(slot)) {
        if (hero_net_stopping()) {
            stopped = true;
            break;
        }
        got = accept(fd, NULL, NULL);
        code = got == HERO_NET_NO_FD ? hero_net_os_code() : 0;
        if (got != HERO_NET_NO_FD || !hero_net_passing(code)) {
            break;
        }
        if (!hero_net_would_wait(code)) {
            continue;
        }
        hero_net_waited waited = hero_net_wait(slot, fd, false, &code);

        if (waited == HERO_NET_WAIT_STOPPED) {
            stopped = true;
            break;
        }
        if (waited == HERO_NET_WAIT_TIMED_OUT) {
            late = true;
            break;
        }
        if (waited == HERO_NET_WAIT_FAILED && !hero_net_interrupted(code)) {
            break;
        }
    }
    /* A connection taken after the close is no connection of a listener
     * the program holds: closed again, the call answering as a closed one. */
    if (hero_net_done(slot)) {
        if (got != HERO_NET_NO_FD) {
            (void)hero_net_close_fd(got);
        }
        hero_net_fail(HERO_NET_NOT_OPEN, 0);
        return NULL;
    }
    if (stopped) {
        hero_net_fail(HERO_NET_STOPPED, 0);
        return NULL;
    }
    if (late) {
        hero_net_fail(HERO_NET_TIMED_OUT, 0);
        return NULL;
    }
    if (got == HERO_NET_NO_FD) {
        hero_net_failed_with(code);
        return NULL;
    }
    /* Non-blocking, as every socket the table holds: BSD's accept gives it
     * the listener's flags and Linux's does not, so it is asked here. */
    if (!hero_net_blocking(got, false) || !hero_net_ready(got)) {
        (void)hero_net_close_fd(got);
        return NULL;
    }
    return hero_net_keep(got);
}

int64_t hero_net_port(hero_net_socket *s) {
    hero_net_fd fd = HERO_NET_NO_FD;
    hero_net_slot *slot = hero_net_take(s, &fd);

    if (slot == NULL) {
        return -1;
    }
    struct sockaddr_storage where;
    hero_net_len size = (hero_net_len)sizeof where;

    memset(&where, 0, sizeof where);
    int failed = getsockname(fd, (struct sockaddr *)&where, &size);
    int64_t code = failed != 0 ? hero_net_os_code() : 0;

    (void)hero_net_done(slot);
    if (failed != 0) {
        hero_net_failed_with(code);
        return -1;
    }
    if (where.ss_family == AF_INET) {
        hero_net_clear();
        return (int64_t)ntohs(((const struct sockaddr_in *)&where)->sin_port);
    }
    if (where.ss_family == AF_INET6) {
        hero_net_clear();
        return (int64_t)ntohs(((const struct sockaddr_in6 *)&where)->sin6_port);
    }
    hero_net_fail(HERO_NET_OTHER, 0);
    return -1;
}

/* -- moving bytes ------------------------------------------------------- */

/* One send or one receive of at most a chunk: tried first, so bytes already
 * there and room already free move after a stop too, then where the system
 * would wait, waited for, asking before each try whether the program closed
 * the socket and before each wait whether the stop was asked. The count, or
 * -1 with the kind in `kind` and the platform's number in `code`, 0 where the
 * layer ended it. */
static int64_t hero_net_move(hero_net_slot *slot, hero_net_fd fd, void *bytes, uint64_t count, bool sending, int64_t *kind, int64_t *code) {
    uint64_t n = count > HERO_NET_CHUNK ? HERO_NET_CHUNK : count;

    for (;;) {
        if (hero_net_closing(slot)) {
            *kind = HERO_NET_NOT_OPEN;
            *code = 0;
            return -1;
        }
#if defined(_WIN32)
        int64_t moved = sending ? (int64_t)send(fd, (const char *)bytes, (int)n, HERO_NET_SEND_FLAGS)
                                : (int64_t)recv(fd, (char *)bytes, (int)n, 0);
#else
        int64_t moved = sending ? (int64_t)send(fd, bytes, (size_t)n, HERO_NET_SEND_FLAGS)
                                : (int64_t)recv(fd, bytes, (size_t)n, 0);
#endif
        if (moved >= 0) {
            return moved;
        }
        *code = hero_net_os_code();
        if (hero_net_interrupted(*code)) {
            continue;
        }
        if (!hero_net_would_wait(*code)) {
            *kind = hero_net_kind_of(*code);
            return -1;
        }
        hero_net_waited waited = hero_net_stopping() ? HERO_NET_WAIT_STOPPED : hero_net_wait(slot, fd, sending, code);

        if (waited == HERO_NET_WAIT_STOPPED) {
            *kind = HERO_NET_STOPPED;
            *code = 0;
            return -1;
        }
        if (waited == HERO_NET_WAIT_TIMED_OUT) {
            *kind = HERO_NET_TIMED_OUT;
            *code = 0;
            return -1;
        }
        if (waited == HERO_NET_WAIT_FAILED && !hero_net_interrupted(*code)) {
            *kind = hero_net_kind_of(*code);
            return -1;
        }
    }
}

static int64_t hero_net_carry(hero_net_socket *s, void *bytes, uint64_t count, bool sending) {
    hero_net_fd fd = HERO_NET_NO_FD;
    hero_net_slot *slot = hero_net_take(s, &fd);

    if (slot == NULL) {
        return -1;
    }
    if (bytes == NULL && count > 0) {
        (void)hero_net_done(slot);
        hero_net_fail(HERO_NET_INVALID, 0);
        return -1;
    }
    int64_t kind = HERO_NET_OK;
    int64_t code = 0;
    int64_t moved = count == 0 ? 0 : hero_net_move(slot, fd, bytes, count, sending, &kind, &code);

    /* Ended by a close meanwhile, a shutdown's 0 or its failure, the call
     * says so; bytes it moved before the close are the program's. */
    if (hero_net_done(slot) && moved <= 0 && count > 0) {
        hero_net_fail(HERO_NET_NOT_OPEN, 0);
        return -1;
    }
    if (moved < 0) {
        hero_net_fail(kind, code);
        return -1;
    }
    hero_net_clear();
    return moved;
}

int64_t hero_net_send(hero_net_socket *s, const void *data, uint64_t count) {
    /* `send` reads these bytes and writes none: the one cast to the common
     * routine's pointer, never written through. */
    return hero_net_carry(s, (void *)(uintptr_t)data, count, true);
}

int64_t hero_net_recv(hero_net_socket *s, void *buffer, uint64_t count) {
    return hero_net_carry(s, buffer, count, false);
}

int64_t hero_net_close(hero_net_socket *s) {
    hero_net_fd fd = HERO_NET_NO_FD;
    bool found = false;
    bool now = false;

    hero_net_enter();
    hero_net_slot *slot = hero_net_find(s);

    if (slot != NULL) {
        found = true;
        if (slot->busy == 0) {
            fd = slot->fd;
            now = true;
            hero_net_release(slot);
        } else {
            /* Under the lock, so the descriptor is still this socket's: the
             * last user out closes it, and only after this has returned.
             * Every call waiting on it is woken here, never through the
             * system; the shutdown ends one waiting in slices. */
            slot->closing = true;
            hero_net_nudge(slot);
            hero_net_shut(slot->fd);
            /* On Windows the poller may hold it in its `WSAPoll`: the close
             * waits for that round to let it go, so the poller's hold never
             * outlives the close, and with no call left in flight the socket
             * is closed when this returns, as on POSIX (defect 2554). */
            hero_net_unpolled(slot);
        }
    }
    hero_net_leave();

    if (!found) {
        hero_net_fail(HERO_NET_NOT_OPEN, 0);
        return -1;
    }
    if (now && hero_net_close_fd(fd) != 0) {
        hero_net_failed_with(hero_net_os_code());
        return -1;
    }
    hero_net_clear();
    return 0;
}

/* -- a socket as a number (panel 246's R6) ------------------------------ */

/* Where a number's life began, as the leak check's report names it: the
 * layer's door, no line of the program, and what the program owes. Its line
 * 0 is how the report's first sentence tells a door's life from a mark's
 * (defect 2504, `parts/panic.c`'s `hero_place_is_a_door`). */
static const HeroPlace hero_net_numbered_at = { "hero_net.h", "hero_net_number, a socket given up as a number no hero_net_of took back", 0, 0 };

int64_t hero_net_number(hero_net_socket *s) {
    int64_t number = -1;

    hero_net_enter();
    hero_net_slot *slot = hero_net_find(s);

    if (slot != NULL) {
        slot->numbered = true;
        number = (int64_t)((uint64_t)(uintptr_t)s | HERO_NET_NUMBERED);
        /* In the handle set before the lock lets any `hero_net_of` find the
         * slot marked, owed to `hero_net_of` alone. */
        hero_handle_acquired_at((const void *)(uintptr_t)(uint64_t)number, "hero_net_of", &hero_net_numbered_at);
    }
    hero_net_leave();

    if (number < 0) {
        hero_net_fail(HERO_NET_NOT_OPEN, 0);
        return -1;
    }
    hero_net_clear();
    return number;
}

hero_net_socket *hero_net_of(int64_t number) {
    uint64_t value = (uint64_t)number;
    hero_net_socket *handle = NULL;

    hero_net_enter();
    hero_net_slot *slot = number > 0 && (value & HERO_NET_NUMBERED) != 0 ? hero_net_slot_of(value & ~HERO_NET_NUMBERED) : NULL;

    if (slot != NULL && slot->numbered) {
        slot->numbered = false;
        /* A life of its own, so the handle given up, and any copy of it,
         * names nothing from now on; a slot at its last life keeps it. */
        if (slot->life < HERO_NET_LIFE_MOST) {
            slot->life += 1;
        }
        handle = hero_net_handle((uint32_t)(slot - hero_net_slots), slot->life);
        /* The number's own life ends here, announced and then made, as a
         * call marked `consumes` ends a handle's. */
        const void *owed = (const void *)(uintptr_t)value;
        uint64_t ending = hero_handle_ending_at(owed, "hero_net_of", NULL, NULL);

        (void)hero_handle_ended(owed, ending);
    }
    hero_net_leave();

    if (handle == NULL) {
        hero_net_fail(HERO_NET_NOT_OPEN, 0);
        return NULL;
    }
    hero_net_clear();
    return handle;
}

/* -- the deadline (panel 248's R5) ------------------------------------- */

/* THE DEADLINE, Go's meaning: an absolute time kept in the socket's slot,
 * set from now by milliseconds, applying to every wait on the socket from
 * now, those already waiting included, which are woken to read it again; 0
 * takes it away. A call that need not wait is not ended by it, as by the
 * stop. A time past what the clock can count is no deadline it reaches. */
int64_t hero_net_set_deadline(hero_net_socket *s, int64_t ms) {
    int64_t at = 0;

    if (ms < 0) {
        hero_net_fail(HERO_NET_INVALID, 0);
        return -1;
    }
    if (ms > 0) {
        int64_t now = hero_net_now();
        int64_t span = 0;

        if (now < 0) {
            hero_net_fail(HERO_NET_OTHER, 0);
            return -1;
        }
        if (__builtin_mul_overflow(ms, INT64_C(1000000), &span) || __builtin_add_overflow(now, span, &at)) {
            at = INT64_MAX;
        }
    }
    hero_net_enter();
    hero_net_slot *slot = hero_net_find(s);

    if (slot != NULL) {
        slot->deadline = at;
        hero_net_nudge(slot);
    }
    hero_net_leave();

    if (slot == NULL) {
        hero_net_fail(HERO_NET_NOT_OPEN, 0);
        return -1;
    }
    hero_net_clear();
    return 0;
}

int64_t hero_net_last_error(void) {
    return hero_net_kind;
}

int64_t hero_net_last_os_error(void) {
    return hero_net_os;
}
