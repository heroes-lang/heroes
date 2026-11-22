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
/* The tag that keeps this table's handles apart from every other member's
 * (2026-11-16): `HERO_HANDLE_TAG_NET`. */
#include "hero_handle_tag.h"
/* The layer's memory, for the one buffer an echo takes for its request and
 * its reply (round 4, 2026-11-17): up to 65,507 bytes of data each way, too
 * many for a thread's stack. */
#include "hero_layer_alloc.h"
/* The runtime's word for the stop, so a mailbox's receive answers it too. */
#include "hero_layer_stop.h"

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
/* `GetAdaptersAddresses`, for `hero_net_interfaces` alone (round 3), and its
 * library named the same way. */
#include <iphlpapi.h>
#if defined(_MSC_VER)
#pragma comment(lib, "iphlpapi.lib")
#endif
/* `IcmpSendEcho2` and `Icmp6SendEcho2`, for `hero_net_ping` alone (round 4),
 * which `iphlpapi` holds too; after `iphlpapi.h`, which defines what it
 * names. */
#include <icmpapi.h>
typedef SOCKET hero_net_fd;
typedef int hero_net_len;
#define HERO_NET_NO_FD INVALID_SOCKET
#define HERO_NET_SEND_FLAGS 0
#else
/* Darwin names RFC 3542's options, the hop limit an IPv6 reply arrived with
 * among them (`IPV6_RECVHOPLIMIT`, round 4), only where this is defined
 * before its `netinet/in.h` is first read, which is below: this member's own
 * header reads `stdint.h` alone. Other systems name them without it. */
#if defined(__APPLE__) && !defined(__APPLE_USE_RFC_3542)
#define __APPLE_USE_RFC_3542 1
#endif
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
/* For round 3's doors: `TCP_NODELAY`, an interface's flags and index, and
 * the list of interfaces. */
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/tcp.h>
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

/* A handle carries this table's tag, a slot and a life in one pointer's 64
 * bits: `HERO_HANDLE_TAG(HERO_HANDLE_TAG_NET) | (life << HERO_NET_INDEX_BITS)
 * | (index + 1)`, 17 bits for an index plus one up to 65,536, the life above
 * them below bit 58, and the tag in bits 58 to 61 (`hero_handle_tag.h`), so
 * no handle is NULL and none is another member's: until 2026-11-16 a socket
 * and a stream at the same slot and life were one value, which the runtime's
 * set of ended handles mistook for each other. */
_Static_assert(sizeof(void *) == 8 && sizeof(uintptr_t) == 8, "a socket handle carries its slot and its life in 64 bits");
#define HERO_NET_INDEX_BITS 17
#define HERO_NET_INDEX_MASK ((UINT64_C(1) << HERO_NET_INDEX_BITS) - 1)
/* The last life a slot is given, 2^41 - 1 since the tag (2026-11-16; 2^45
 * - 1 before it): a slot that takes a new life for every socket and every
 * number taken back reaches it after 254 days at a hundred thousand a
 * second, and a slot at it is never issued again, which costs a slot and
 * never a wrong handle. Until 2026-11-13 a life was 32 bits, which a socket
 * taken back as a number every time could spend in a day. */
#define HERO_NET_LIFE_MOST HERO_HANDLE_LIFE_MOST(HERO_NET_INDEX_BITS)
/* The bit a number carries and no handle does, so a number is positive and
 * never a handle's value: above the tag, which a number keeps. */
#define HERO_NET_NUMBERED (UINT64_C(1) << 62)
_Static_assert((HERO_NET_NUMBERED & HERO_HANDLE_TAG_MASK) == 0, "a number's bit is none of the tag's");
_Static_assert((HERO_NET_LIFE_MOST << HERO_NET_INDEX_BITS) < (UINT64_C(1) << HERO_HANDLE_TAG_SHIFT), "a life stays below the tag");
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
    /* What kind of socket it holds (M-rebel-library): 0 a connection or a
     * listener, else a datagram socket, 4 where its addresses are IPv4's and
     * 6 where they are IPv6's. Written before any handle names the slot, so
     * a call that holds the slot busy reads it as it was issued. */
    uint8_t datagram;
    /* The interface a datagram socket's datagrams to a group leave by, as
     * `HERO_NET_OPTION_MULTICAST_INTERFACE` last set it, 0 for the system's
     * choice (round 3): kept here, under the lock, since Linux answers an
     * address and no index where it is asked. */
    uint32_t multicast_interface;
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
    return (hero_net_socket *)(uintptr_t)(HERO_HANDLE_TAG(HERO_HANDLE_TAG_NET) | (life << HERO_NET_INDEX_BITS) | ((uint64_t)index + 1));
}

/* The slot a handle's value names while it holds the life the value was
 * issued for, and is neither closed nor given up for a number; NULL for any
 * other value, one of another table's tag before any slot is looked at. A
 * number's bit left on is no life a slot holds. Under the lock. */
static hero_net_slot *hero_net_slot_of(uint64_t value) {
    uint64_t low = value & HERO_NET_INDEX_MASK;

    if ((value & HERO_HANDLE_TAG_MASK) != HERO_HANDLE_TAG(HERO_HANDLE_TAG_NET) || low == 0 || low > hero_net_fresh) {
        return NULL;
    }
    hero_net_slot *slot = &hero_net_slots[low - 1];

    if (!slot->held || slot->closing || slot->life != (value & ~HERO_HANDLE_TAG_MASK) >> HERO_NET_INDEX_BITS) {
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
        slot->datagram = 0;
        slot->multicast_interface = 0;
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

/* As `hero_net_take`, for a call only one kind of socket has
 * (M-rebel-library): a datagram socket (`datagram` true), or a connection or
 * a listener. A socket of the other kind is `HERO_NET_INVALID`, nothing
 * taken: a `recv` on a datagram socket would cut a datagram in silence, and
 * a `send_to` on a connection would send to another peer than was named.
 * `family`, where it is asked, takes the 4 or the 6 of a datagram socket. */
static hero_net_slot *hero_net_take_kind(hero_net_socket *s, hero_net_fd *fd, bool datagram, int *family) {
    bool other = false;

    hero_net_enter();
    hero_net_slot *slot = hero_net_find(s);

    if (slot != NULL && (slot->datagram != 0) != datagram) {
        other = true;
        slot = NULL;
    }
    if (slot != NULL) {
        slot->busy += 1;
        *fd = slot->fd;
        if (family != NULL) {
            *family = (int)slot->datagram;
        }
    }
    hero_net_leave();

    if (slot == NULL) {
        hero_net_fail(other ? HERO_NET_INVALID : HERO_NET_NOT_OPEN, 0);
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

/* The other members' functions told of a stop (`hero_net_when_stopped`, at
 * the end of this file). Under the lock. */
static void hero_net_tell_hearers(void);

/* Every waiting call of the process asked to look again: the stop. Under the
 * lock. It is called in each of the three ways a stop is heard, and the other
 * members that wait are told there too (M-rebel-library, 2026-11-16). */
static void hero_net_nudge_all(void) {
    for (uint32_t i = 0; i < hero_net_fresh; i += 1) {
        hero_net_nudge(&hero_net_slots[i]);
    }
    hero_net_tell_hearers();
    /* The language's mailboxes (panel 254's R6): a receive that finds its
     * mailbox empty answers `stopped`. Their locks come after this one. */
    hero_mail_hear_stop();
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

/* The most sockets one connect to one address makes: each try after the
 * first is made because the system gave the socket before it the very port
 * it connects to (`hero_net_to_itself`). */
#define HERO_NET_SELF_TRIES 3

/* The platform's number for a refused connection, which a connection made to
 * itself is answered with (`hero_net_connect_once`). */
#if defined(_WIN32)
#define HERO_NET_REFUSED_CODE WSAECONNREFUSED
#else
#define HERO_NET_REFUSED_CODE ECONNREFUSED
#endif

/* Whether `fd`, connected or refused, stands on the port `at` names, on
 * `at`'s address or on no address in particular: a connection asked of
 * itself. The system chooses the port of a socket connected without a bind,
 * one no socket holds, so where nothing listens on the port asked it may
 * choose that very one, and on macOS the allocator that does (BSD sockets'
 * own, in order, `net.inet.tcp.randomize_ports` 0) walks the same range as
 * Network.framework's listeners' and crosses it (defect 2623). Darwin then
 * refuses the connect at once, EINVAL, the socket standing on 0.0.0.0 and
 * the port asked (40 connects steered there of 40, blocking and not, macOS
 * 27.2); Linux makes it, a socket connected to itself (one bound there by
 * hand, the Linux arm64 image). Read from the socket, never assumed. */
static bool hero_net_to_itself(hero_net_fd fd, const struct addrinfo *at) {
    struct sockaddr_storage where;
    hero_net_len size = (hero_net_len)sizeof where;

    memset(&where, 0, sizeof where);
    if (getsockname(fd, (struct sockaddr *)&where, &size) != 0 || where.ss_family != at->ai_family) {
        return false;
    }
    if (at->ai_family == AF_INET) {
        const struct sockaddr_in *mine = (const struct sockaddr_in *)&where;
        const struct sockaddr_in *asked = (const struct sockaddr_in *)at->ai_addr;

        return mine->sin_port == asked->sin_port && (mine->sin_addr.s_addr == asked->sin_addr.s_addr || mine->sin_addr.s_addr == htonl(INADDR_ANY));
    }
    if (at->ai_family == AF_INET6) {
        static const unsigned char none[16] = { 0 };
        const struct sockaddr_in6 *mine = (const struct sockaddr_in6 *)&where;
        const struct sockaddr_in6 *asked = (const struct sockaddr_in6 *)at->ai_addr;

        return mine->sin6_port == asked->sin6_port && (memcmp(&mine->sin6_addr, &asked->sin6_addr, sizeof none) == 0 || memcmp(&mine->sin6_addr, none, sizeof none) == 0);
    }
    return false;
}

/* One socket's try of `hero_net_try_connect`, `*itself` set where the system
 * gave the socket the port it connects to (`hero_net_to_itself`): that
 * failure's own kind and number then, and a connection made to itself given
 * back, never handed on, `HERO_NET_REFUSED`. A stop and a deadline end the
 * connect and are never tried again. The connection is asked for on a
 * non-blocking socket, which takes a slot of its own at once, a slot no
 * handle of the program names yet, so its wait is every call's wait and no
 * close can end it; it ends writable or failed, the system's own answer read
 * each time (`hero_net_pending_error`), and always after the look at its
 * writability, never before (below). On Windows the wait looks every
 * slice whatever stands, for the refusal its `WSAPoll` did not report before
 * version 2004. */
static hero_net_socket *hero_net_connect_once(const struct addrinfo *at, int64_t deadline, int64_t *kind, int64_t *code, bool *itself) {
    hero_net_fd fd = hero_net_made(at, code);

    if (fd == HERO_NET_NO_FD) {
        *kind = hero_net_kind_of(*code);
        return NULL;
    }
    bool at_once = connect(fd, at->ai_addr, (hero_net_len)at->ai_addrlen) == 0;

    if (!at_once) {
        *code = hero_net_os_code();
        if (!hero_net_connecting(*code)) {
            *itself = hero_net_to_itself(fd, at);
            (void)hero_net_close_fd(fd);
            *kind = hero_net_kind_of(*code);
            return NULL;
        }
    }
    hero_net_socket *handle = hero_net_keep(fd);

    if (handle != NULL && at_once && hero_net_to_itself(fd, at)) {
        *itself = true;
        (void)hero_net_close(handle);
        *kind = HERO_NET_REFUSED;
        *code = HERO_NET_REFUSED_CODE;
        return NULL;
    }
    if (handle == NULL || at_once) {
        *kind = hero_net_kind;
        *code = hero_net_os;
        return handle;
    }
    hero_net_fd same = HERO_NET_NO_FD;
    hero_net_slot *slot = hero_net_take(handle, &same);
    bool made = false;

    /* The time the connection must be made by (`hero_net_connect_within`,
     * round 3), 0 for none: the slot's deadline while it is being made,
     * which every wait below reads, taken away again once it is made. */
    if (deadline != 0) {
        hero_net_enter();
        slot->deadline = deadline;
        hero_net_leave();
    }
    *kind = HERO_NET_OK;
    for (;;) {
#if defined(_WIN32)
        hero_net_waited waited = hero_net_stopping() ? HERO_NET_WAIT_STOPPED : hero_net_wait_slice(fd, true, deadline, code);
#else
        hero_net_waited waited = hero_net_stopping() ? HERO_NET_WAIT_STOPPED : hero_net_wait(slot, fd, true, code);
#endif

        if (waited == HERO_NET_WAIT_STOPPED) {
            *kind = HERO_NET_STOPPED;
            *code = 0;
            break;
        }
        if (waited == HERO_NET_WAIT_TIMED_OUT) {
            /* A connection the system has made by the time is made, the
             * time passed or not, as a call that need not wait is spared a
             * deadline: what a time of 0 asks (`hero_net_connect_within`).
             * Writable first, then its pending error, so a connection that
             * failed between the two looks is never taken for one made. */
            if (hero_net_writable_now(fd) && hero_net_pending_error(fd) == 0) {
                made = true;
                break;
            }
            *kind = HERO_NET_TIMED_OUT;
            *code = 0;
            break;
        }
        if (waited == HERO_NET_WAIT_FAILED && !hero_net_interrupted(*code)) {
            *kind = hero_net_kind_of(*code);
            break;
        }
        /* Writable first, then its pending error, as at the deadline above. A
         * wake may come with nothing behind it (the poller's, for a slot given
         * back and issued again), and the error read then is 0 while the
         * connection is still being made; one that fails before a look at its
         * writability after that read is writable to `poll` too, POLLHUP on
         * Darwin, and was taken for one made, its first send told RESET
         * (measured on Darwin and Linux arm64, M-rebel-library step 87b).
         * Writable before the error is read means the connection ended, made
         * or failed, by then, so the error read after it is the one it ended
         * with. The error is read whatever the wait said, for the refusal
         * Windows' `WSAPoll` did not report before version 2004. */
        bool ended = waited == HERO_NET_WAIT_READY && hero_net_writable_now(fd);
        int64_t said = hero_net_pending_error(fd);

        if (said != 0) {
            *code = said;
            *kind = hero_net_kind_of(said);
            break;
        }
        if (ended) {
            made = true;
            break;
        }
    }
    if (*kind != HERO_NET_STOPPED && *kind != HERO_NET_TIMED_OUT && hero_net_to_itself(fd, at)) {
        *itself = true;
        if (made) {
            made = false;
            *kind = HERO_NET_REFUSED;
            *code = HERO_NET_REFUSED_CODE;
        }
    }
    if (made && deadline != 0) {
        hero_net_enter();
        slot->deadline = 0;
        hero_net_leave();
    }
    (void)hero_net_done(slot);
    if (!made) {
        (void)hero_net_close(handle);
        return NULL;
    }
    return handle;
}

/* A connection to the address, kept in a slot, or NULL with the kind in
 * `kind` and the platform's number in `code`, `HERO_NET_STOPPED` where the
 * stop was asked while it was being made. A socket the system gave the port
 * it connects to is no answer about that port: it is closed and another
 * socket asks again, given another port, up to `HERO_NET_SELF_TRIES`
 * sockets in all, the last one's answer kept. Until then, on Darwin, a
 * connect to a TLS listener's port just closed was `failed` ("the system
 * failed, its error 22") in 7 rounds of 80,000, eight programs at once on a
 * Mac, and one steered onto its own port in 500 of 500 (defect 2623). */
static hero_net_socket *hero_net_try_connect(const struct addrinfo *at, int64_t deadline, int64_t *kind, int64_t *code) {
    hero_net_socket *handle = NULL;
    bool itself = true;

    for (int tries = 0; itself && tries < HERO_NET_SELF_TRIES; tries += 1) {
        itself = false;
        handle = hero_net_connect_once(at, deadline, kind, code, &itself);
    }
    return handle;
}

/* Defined with the names below: the kind a resolver's answer is. */
static int64_t hero_net_name_kind(int said, int64_t *code);

/* A listener, or a connection made by `deadline` on the clock
 * `hero_net_now` reads, 0 for none. */
static hero_net_socket *hero_net_open(const char *address, int64_t port, bool listening, int64_t deadline) {
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
        /* A name the resolver does not know, or knows with no address, is
         * `HERO_NET_NOT_FOUND` to a connection, as to `hero_net_lookup`
         * (round 3); every other answer is what it was. */
        int64_t unused = 0;
        int64_t named = listening ? HERO_NET_ADDRESS : hero_net_name_kind(said, &unused);

        hero_net_fail(named == HERO_NET_NOT_FOUND ? HERO_NET_NOT_FOUND : said == EAI_MEMORY ? HERO_NET_LIMIT : HERO_NET_ADDRESS, (int64_t)said);
        return NULL;
    }
    hero_net_socket *handle = NULL;
    int64_t kind = HERO_NET_OTHER;
    int64_t code = 0;

    /* A connection's time is one for every address: past it, no other is
     * tried. */
    for (const struct addrinfo *at = found; at != NULL && handle == NULL && kind != HERO_NET_STOPPED && !(deadline != 0 && kind == HERO_NET_TIMED_OUT);
         at = at->ai_next) {
        if (listening) {
            hero_net_fd fd = hero_net_try_listen(at, &code);

            kind = fd == HERO_NET_NO_FD ? hero_net_kind_of(code) : HERO_NET_OK;
            if (fd != HERO_NET_NO_FD) {
                handle = hero_net_keep(fd);
                kind = hero_net_kind;
                code = hero_net_os;
            }
        } else {
            handle = hero_net_try_connect(at, deadline, &kind, &code);
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
    return hero_net_open(address, port, true, 0);
}

hero_net_socket *hero_net_connect(const char *address, int64_t port) {
    return hero_net_open(address, port, false, 0);
}

hero_net_socket *hero_net_accept(hero_net_socket *listener) {
    hero_net_fd fd = HERO_NET_NO_FD;
    /* A datagram socket holds no connection to take: `HERO_NET_INVALID`. */
    hero_net_slot *slot = hero_net_take_kind(listener, &fd, false, NULL);

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
#if defined(_WIN32)
        /* A receive after the program shut its own reading side
         * (`hero_net_shutdown`, round 3) is the end of the bytes, 0, as
         * POSIX answers it, where Windows fails it `WSAESHUTDOWN`. */
        if (!sending && *code == WSAESHUTDOWN) {
            return 0;
        }
#endif
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
    /* A datagram socket is sent to and received from by its own two calls,
     * which never cut a datagram in silence: here `HERO_NET_INVALID`. */
    hero_net_slot *slot = hero_net_take_kind(s, &fd, false, NULL);

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
        /* A number carrying another table's tag is one this table never
         * gave: not open, as every number it never gave. */
        hero_net_fail(HERO_NET_NOT_OPEN, 0);
        return NULL;
    }
    hero_net_clear();
    return handle;
}

/* -- the deadline (panel 248's R5) ------------------------------------- */

/* The time on `hero_net_now`'s clock `ms` milliseconds from now, into
 * `*at`, for a door that takes a time (`HERO_NET_FOREVER`'s comment): 0, a
 * slot's "none", where `ms` is `HERO_NET_FOREVER`; now itself where `ms` is
 * 0, a time already passed, and 1 where now reads 0, so it is never read as
 * none; `INT64_MAX` past what the clock counts, a time no wait reaches.
 * false, with `HERO_NET_INVALID` for a `ms` below -1 or `HERO_NET_OTHER` for
 * a clock that cannot be read, said. */
static bool hero_net_deadline_at(int64_t ms, int64_t *at) {
    int64_t now = 0;
    int64_t span = 0;

    *at = 0;
    if (ms == HERO_NET_FOREVER) {
        return true;
    }
    if (ms < 0) {
        hero_net_fail(HERO_NET_INVALID, 0);
        return false;
    }
    now = hero_net_now();
    if (now < 0) {
        hero_net_fail(HERO_NET_OTHER, 0);
        return false;
    }
    if (__builtin_mul_overflow(ms, INT64_C(1000000), &span) || __builtin_add_overflow(now, span, at)) {
        *at = INT64_MAX;
    }
    if (*at == 0) {
        *at = 1;
    }
    return true;
}

/* THE DEADLINE, Go's meaning: an absolute time kept in the socket's slot,
 * set from now by milliseconds, applying to every wait on the socket from
 * now, those already waiting included, which are woken to read it again; 0
 * a time already passed, `HERO_NET_FOREVER` taking it away. A call that need
 * not wait is not ended by it, as by the stop. A time past what the clock
 * can count is no deadline it reaches. */
int64_t hero_net_set_deadline(hero_net_socket *s, int64_t ms) {
    int64_t at = 0;

    if (!hero_net_deadline_at(ms, &at)) {
        return -1;
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

/* -- datagrams, names and a socket's two addresses (M-rebel-library) ---- */

/* The longest name the resolver is asked, and the longest it answers, in
 * bytes without the zero byte: `hero_net.h`'s 1,024. */
#define HERO_NET_NAME_MOST 1024

/* One address of the system's list: `GetAddrInfoW`'s on Windows, where a
 * name crosses as UTF-16, `getaddrinfo`'s elsewhere. The two records hold
 * the members this file reads under the same names. */
#if defined(_WIN32)
typedef ADDRINFOW hero_net_info;
#else
typedef struct addrinfo hero_net_info;
#endif

/* Whether `name` is one the resolver may be asked: there, and 1 to
 * `HERO_NET_NAME_MOST` bytes before its zero byte. */
static bool hero_net_nameable(const char *name) {
    if (name == NULL) {
        return false;
    }
    for (size_t i = 0; i <= HERO_NET_NAME_MOST; i += 1) {
        if (name[i] == '\0') {
            return i > 0;
        }
    }
    return false;
}

/* The system's list of addresses for `host` (NULL, with `AI_PASSIVE`, for
 * every address of the machine), of `family` and `socktype`, each at port
 * 0: the system's own answer, 0 where `*found` holds a list for
 * `hero_net_infos_free`. With `AI_NUMERICHOST` among `flags` the host is a
 * number and no resolver is asked. On Windows a name that is no UTF-8, or
 * longer than the buffer, names no host. */
static int hero_net_infos(const char *host, int family, int socktype, int flags, hero_net_info **found) {
#if defined(_WIN32)
    wchar_t wide[HERO_NET_NAME_MOST + 1];
    ADDRINFOW hints;

    if (host != NULL && MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, host, -1, wide, (int)(sizeof wide / sizeof wide[0])) == 0) {
        return WSAHOST_NOT_FOUND;
    }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = family;
    hints.ai_socktype = socktype;
    hints.ai_flags = flags | AI_NUMERICSERV;
    return GetAddrInfoW(host == NULL ? NULL : wide, L"0", &hints, found);
#else
    struct addrinfo hints;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = family;
    hints.ai_socktype = socktype;
    hints.ai_flags = flags | AI_NUMERICSERV;
    return getaddrinfo(host, "0", &hints, found);
#endif
}

static void hero_net_infos_free(hero_net_info *found) {
#if defined(_WIN32)
    FreeAddrInfoW(found);
#else
    freeaddrinfo(found);
#endif
}

/* The kind a resolver's answer is, and in `code` the platform's number for
 * it. Asked one by one and never by a `switch`: Windows' headers give two of
 * POSIX's names one number. */
static int64_t hero_net_name_kind(int said, int64_t *code) {
    *code = (int64_t)said;
#if defined(_WIN32)
    if (said == WSAHOST_NOT_FOUND || said == WSANO_DATA) {
        return HERO_NET_NOT_FOUND;
    }
    if (said == WSATRY_AGAIN) {
        return HERO_NET_UNREACHABLE;
    }
    return hero_net_kind_of((int64_t)said);
#else
    if (said == EAI_NONAME) {
        return HERO_NET_NOT_FOUND;
    }
#if defined(EAI_NODATA)
    if (said == EAI_NODATA) {
        return HERO_NET_NOT_FOUND;
    }
#endif
    if (said == EAI_AGAIN) {
        return HERO_NET_UNREACHABLE;
    }
    if (said == EAI_MEMORY) {
        return HERO_NET_LIMIT;
    }
    if (said == EAI_SYSTEM) {
        *code = hero_net_os_code();
        return hero_net_kind_of(*code);
    }
    return HERO_NET_OTHER;
#endif
}

/* The address `where` holds as text in `text`, `HERO_NET_ADDRESS_MOST`
 * bytes, with its zero byte, and its port in `*port`: the text's length, or
 * -1 for a family that is neither IPv4 nor IPv6, or a text the system would
 * not write in so many bytes. The text is a number, so no resolver is
 * asked; the system writes it, and with it the scope of an address that has
 * one. */
static int64_t hero_net_text_of(const struct sockaddr *where, hero_net_len size, char *text, int64_t *port) {
    if (where->sa_family == AF_INET && (size_t)size >= sizeof(struct sockaddr_in)) {
        struct sockaddr_in four;

        memcpy(&four, where, sizeof four);
        *port = (int64_t)ntohs(four.sin_port);
    } else if (where->sa_family == AF_INET6 && (size_t)size >= sizeof(struct sockaddr_in6)) {
        struct sockaddr_in6 six;

        memcpy(&six, where, sizeof six);
        *port = (int64_t)ntohs(six.sin6_port);
    } else {
        return -1;
    }
#if defined(_WIN32)
    wchar_t wide[HERO_NET_ADDRESS_MOST];

    if (GetNameInfoW(where, size, wide, (DWORD)HERO_NET_ADDRESS_MOST, NULL, 0, NI_NUMERICHOST) != 0) {
        return -1;
    }
    int wrote = WideCharToMultiByte(CP_UTF8, 0, wide, -1, text, (int)HERO_NET_ADDRESS_MOST, NULL, NULL);

    return wrote <= 0 ? -1 : (int64_t)wrote - 1;
#else
    if (getnameinfo(where, size, text, (socklen_t)HERO_NET_ADDRESS_MOST, NULL, 0, NI_NUMERICHOST) != 0) {
        return -1;
    }
    return (int64_t)strlen(text);
#endif
}

/* The address `at` holds, copied into `where` with `port` written into it,
 * and its size; false for a family that is neither IPv4 nor IPv6. */
static bool hero_net_placed(const hero_net_info *at, int64_t port, struct sockaddr_storage *where, hero_net_len *size) {
    memset(where, 0, sizeof *where);
    if (at->ai_addr == NULL) {
        return false;
    }
    if (at->ai_family == AF_INET && (size_t)at->ai_addrlen >= sizeof(struct sockaddr_in)) {
        struct sockaddr_in four;

        memcpy(&four, at->ai_addr, sizeof four);
        four.sin_port = htons((uint16_t)port);
        memcpy(where, &four, sizeof four);
        *size = (hero_net_len)sizeof four;
        return true;
    }
    if (at->ai_family == AF_INET6 && (size_t)at->ai_addrlen >= sizeof(struct sockaddr_in6)) {
        struct sockaddr_in6 six;

        memcpy(&six, at->ai_addr, sizeof six);
        six.sin6_port = htons((uint16_t)port);
        memcpy(where, &six, sizeof six);
        *size = (hero_net_len)sizeof six;
        return true;
    }
    return false;
}

/* `address`, a number, with `port`, in `where`: of the family a datagram
 * socket's 4 or 6 names, or of either where `family` is 0. False, the
 * system's answer in `code` where it gave one, for what is no number, is of
 * the other family, or is empty. */
static bool hero_net_literal(const char *address, int family, int64_t port, struct sockaddr_storage *where, hero_net_len *size, int64_t *code) {
    hero_net_info *found = NULL;

    *code = 0;
    if (!hero_net_nameable(address)) {
        return false;
    }
    int said = hero_net_infos(address, family == 4 ? AF_INET : family == 6 ? AF_INET6 : AF_UNSPEC, SOCK_DGRAM, AI_NUMERICHOST, &found);

    if (said != 0) {
        *code = (int64_t)said;
        return false;
    }
    bool placed = found != NULL && hero_net_placed(found, port, where, size);

    hero_net_infos_free(found);
    return placed;
}

/* A datagram socket of `family` bound to `where`, readied and non-blocking,
 * or NO_FD with the platform's number in `code`. No `SO_REUSEADDR`: a
 * datagram socket leaves no port in TIME_WAIT to take again, the one thing
 * a listener sets it for. On Windows the port is this socket's alone, as a
 * listener's is. */
static hero_net_fd hero_net_bound(int family, const struct sockaddr_storage *where, hero_net_len size, int64_t *code) {
    hero_net_fd fd = socket(family, SOCK_DGRAM, IPPROTO_UDP);

    if (fd == HERO_NET_NO_FD) {
        *code = hero_net_os_code();
        return HERO_NET_NO_FD;
    }
    if (!hero_net_ready(fd) || !hero_net_blocking(fd, false)) {
        *code = hero_net_os;
        (void)hero_net_close_fd(fd);
        return HERO_NET_NO_FD;
    }
#if defined(_WIN32)
    int one = 1;
    int failed = setsockopt(fd, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (const char *)&one, (hero_net_len)sizeof one);
#else
    int failed = 0;
#endif

    if (failed == 0) {
        failed = bind(fd, (const struct sockaddr *)where, size);
    }
    if (failed != 0) {
        *code = hero_net_os_code();
        (void)hero_net_close_fd(fd);
        return HERO_NET_NO_FD;
    }
    return fd;
}

/* A datagram socket the system made, kept in a slot marked with its family
 * before any handle of the program names it: no other thread knows the
 * handle until this returns it. */
static hero_net_socket *hero_net_keep_datagram(hero_net_fd fd, int family) {
    hero_net_socket *handle = hero_net_keep(fd);

    if (handle != NULL) {
        hero_net_enter();
        hero_net_slot *slot = hero_net_find(handle);

        if (slot != NULL) {
            slot->datagram = family == AF_INET6 ? 6 : 4;
        }
        hero_net_leave();
    }
    return handle;
}

hero_net_socket *hero_net_udp(const char *address, int64_t port) {
    hero_net_info *found = NULL;

    if (address == NULL || port < 0 || port > 65535 || (address[0] != '\0' && !hero_net_nameable(address))) {
        hero_net_fail(HERO_NET_INVALID, 0);
        return NULL;
    }
    if (!hero_net_started()) {
        return NULL;
    }
    int said = hero_net_infos(address[0] == '\0' ? NULL : address, AF_UNSPEC, SOCK_DGRAM, AI_NUMERICHOST | AI_PASSIVE, &found);

    if (said != 0) {
        hero_net_fail(said == EAI_MEMORY ? HERO_NET_LIMIT : HERO_NET_ADDRESS, (int64_t)said);
        return NULL;
    }
    hero_net_socket *handle = NULL;
    int64_t kind = HERO_NET_ADDRESS;
    int64_t code = 0;

    for (const hero_net_info *at = found; at != NULL && handle == NULL; at = at->ai_next) {
        struct sockaddr_storage where;
        hero_net_len size = 0;

        if (!hero_net_placed(at, port, &where, &size)) {
            continue;
        }
        hero_net_fd fd = hero_net_bound(at->ai_family, &where, size, &code);

        if (fd == HERO_NET_NO_FD) {
            kind = hero_net_kind_of(code);
            continue;
        }
        handle = hero_net_keep_datagram(fd, at->ai_family);
        kind = hero_net_kind;
        code = hero_net_os;
    }
    hero_net_infos_free(found);

    if (handle == NULL) {
        hero_net_fail(kind, code);
        return NULL;
    }
    hero_net_clear();
    return handle;
}

/* The kind a datagram call's failure is: the system's word for one too long
 * to send, or to receive whole, and every other as `hero_net_kind_of`
 * names it. */
static int64_t hero_net_datagram_kind(int64_t code) {
#if defined(_WIN32)
    if (code == WSAEMSGSIZE) {
        return HERO_NET_TOO_LONG;
    }
#else
    if (code == EMSGSIZE) {
        return HERO_NET_TOO_LONG;
    }
#endif
    return hero_net_kind_of(code);
}

/* What a datagram call does once the system said it would wait: the wait
 * every call of this file makes, on the layer's own poller. True where the
 * call tries again; false, the kind and the code set, where the stop, the
 * deadline or the system ended the wait. */
static bool hero_net_datagram_waited(hero_net_slot *slot, hero_net_fd fd, bool sending, int64_t *kind, int64_t *code) {
    hero_net_waited waited = hero_net_stopping() ? HERO_NET_WAIT_STOPPED : hero_net_wait(slot, fd, sending, code);

    if (waited == HERO_NET_WAIT_STOPPED) {
        *kind = HERO_NET_STOPPED;
        *code = 0;
        return false;
    }
    if (waited == HERO_NET_WAIT_TIMED_OUT) {
        *kind = HERO_NET_TIMED_OUT;
        *code = 0;
        return false;
    }
    if (waited == HERO_NET_WAIT_FAILED && !hero_net_interrupted(*code)) {
        *kind = hero_net_kind_of(*code);
        return false;
    }
    return true;
}

/* One datagram sent to `where`: tried first, so room already free is used
 * after a stop too, then waited for where the system has none, as
 * `hero_net_move` waits. The count, or -1 with the kind and the code. */
static int64_t hero_net_datagram_out(hero_net_slot *slot, hero_net_fd fd, const void *data, uint64_t count, const struct sockaddr_storage *where, hero_net_len size, int64_t *kind, int64_t *code) {
    /* A datagram of no bytes still hands the system a place to read from. */
    char none = 0;
    const void *bytes = count == 0 ? (const void *)&none : data;

    for (;;) {
        if (hero_net_closing(slot)) {
            *kind = HERO_NET_NOT_OPEN;
            *code = 0;
            return -1;
        }
#if defined(_WIN32)
        int64_t moved = (int64_t)sendto(fd, (const char *)bytes, (int)count, HERO_NET_SEND_FLAGS, (const struct sockaddr *)where, size);
#else
        int64_t moved = (int64_t)sendto(fd, bytes, (size_t)count, HERO_NET_SEND_FLAGS, (const struct sockaddr *)where, size);
#endif
        if (moved >= 0) {
            return moved;
        }
        *code = hero_net_os_code();
        if (hero_net_interrupted(*code)) {
            continue;
        }
        if (!hero_net_would_wait(*code)) {
            *kind = hero_net_datagram_kind(*code);
            return -1;
        }
        if (!hero_net_datagram_waited(slot, fd, true, kind, code)) {
            return -1;
        }
    }
}

/* One datagram received into `buffer`, its sender in `from` and
 * `from_size` (0 where the system named none): tried first, then waited
 * for. `cut` says the datagram was longer than `count`, which holds its
 * first bytes: POSIX's `recvmsg` says so in `MSG_TRUNC`, the one call there
 * that does, and Windows' `recvfrom` by failing `WSAEMSGSIZE` with the
 * buffer filled. The bytes the buffer holds, or -1 with the kind and the
 * code. */
static int64_t hero_net_datagram_in(hero_net_slot *slot, hero_net_fd fd, void *buffer, uint64_t count, struct sockaddr_storage *from, hero_net_len *from_size, bool *cut, int64_t *kind, int64_t *code) {
    char none = 0;
    void *bytes = count == 0 ? (void *)&none : buffer;
    uint64_t n = count > HERO_NET_CHUNK ? HERO_NET_CHUNK : count;

    for (;;) {
        if (hero_net_closing(slot)) {
            *kind = HERO_NET_NOT_OPEN;
            *code = 0;
            return -1;
        }
        memset(from, 0, sizeof *from);
#if defined(_WIN32)
        int size = (int)sizeof *from;
        int got = recvfrom(fd, (char *)bytes, (int)n, 0, (struct sockaddr *)from, &size);

        *code = got < 0 ? hero_net_os_code() : 0;
        if (got >= 0 || *code == WSAEMSGSIZE) {
            *cut = got < 0;
            *from_size = from->ss_family == 0 ? 0 : size;
            return got < 0 ? (int64_t)n : (int64_t)got;
        }
        /* No datagram, but the system's word on one this socket SENT
         * earlier, refused at its port or expired on its way (Microsoft's
         * page for `recvfrom`, of a datagram socket): nothing a receive
         * answers on POSIX, so the next datagram is looked for. */
        if (*code == WSAECONNRESET || *code == WSAENETRESET) {
            continue;
        }
#else
        struct iovec into = { bytes, (size_t)n };
        struct msghdr heard;

        memset(&heard, 0, sizeof heard);
        heard.msg_name = from;
        heard.msg_namelen = (socklen_t)sizeof *from;
        heard.msg_iov = &into;
        heard.msg_iovlen = 1;
        ssize_t got = recvmsg(fd, &heard, 0);

        if (got >= 0) {
            *cut = (heard.msg_flags & MSG_TRUNC) != 0;
            *from_size = heard.msg_namelen;
            return (int64_t)got;
        }
        *code = hero_net_os_code();
#endif
        if (hero_net_interrupted(*code)) {
            continue;
        }
        if (!hero_net_would_wait(*code)) {
            *kind = hero_net_datagram_kind(*code);
            return -1;
        }
        if (!hero_net_datagram_waited(slot, fd, false, kind, code)) {
            return -1;
        }
    }
}

int64_t hero_net_send_to(hero_net_socket *s, const void *data, uint64_t count, const char *address, int64_t port) {
    hero_net_fd fd = HERO_NET_NO_FD;
    int family = 0;
    hero_net_slot *slot = hero_net_take_kind(s, &fd, true, &family);

    if (slot == NULL) {
        return -1;
    }
    struct sockaddr_storage where;
    hero_net_len size = 0;
    int64_t kind = HERO_NET_OK;
    int64_t code = 0;
    int64_t sent = -1;

    if ((data == NULL && count > 0) || address == NULL || port < 1 || port > 65535) {
        kind = HERO_NET_INVALID;
    } else if (count > HERO_NET_CHUNK) {
        /* More than Windows' `int` count can say, and than any system sends
         * as one. */
        kind = HERO_NET_TOO_LONG;
    } else if (!hero_net_literal(address, family, port, &where, &size, &code)) {
        kind = HERO_NET_ADDRESS;
    } else {
        sent = hero_net_datagram_out(slot, fd, data, count, &where, size, &kind, &code);
    }
    /* Ended by a close meanwhile, the call says so, as every call does. */
    if (hero_net_done(slot) && sent < 0) {
        hero_net_fail(HERO_NET_NOT_OPEN, 0);
        return -1;
    }
    if (sent < 0) {
        hero_net_fail(kind, code);
        return -1;
    }
    hero_net_clear();
    return sent;
}

int64_t hero_net_recv_from(hero_net_socket *s, void *buffer, uint64_t count, void *address, uint64_t address_count, int64_t *port) {
    hero_net_fd fd = HERO_NET_NO_FD;
    hero_net_slot *slot = hero_net_take_kind(s, &fd, true, NULL);

    if (slot == NULL) {
        return -1;
    }
    struct sockaddr_storage from;
    hero_net_len from_size = 0;
    bool cut = false;
    int64_t kind = HERO_NET_OK;
    int64_t code = 0;
    int64_t got = -1;

    /* Refused before any datagram is taken: one taken whose sender could
     * not be written would be lost with half its answer. */
    if ((buffer == NULL && count > 0) || address == NULL || address_count < HERO_NET_ADDRESS_MOST || port == NULL) {
        kind = HERO_NET_INVALID;
    } else {
        got = hero_net_datagram_in(slot, fd, buffer, count, &from, &from_size, &cut, &kind, &code);
    }
    if (hero_net_done(slot) && got < 0) {
        hero_net_fail(HERO_NET_NOT_OPEN, 0);
        return -1;
    }
    if (got < 0) {
        hero_net_fail(kind, code);
        return -1;
    }
    char text[HERO_NET_ADDRESS_MOST];
    int64_t sender_port = 0;
    int64_t length = from_size == 0 ? -1 : hero_net_text_of((const struct sockaddr *)&from, from_size, text, &sender_port);

    if (length < 0) {
        text[0] = '\0';
        sender_port = 0;
        length = 0;
    }
    memcpy(address, text, (size_t)length + 1);
    *port = sender_port;

    if (cut) {
        hero_net_fail(HERO_NET_TOO_LONG, 0);
        return -1;
    }
    hero_net_clear();
    return got;
}

int64_t hero_net_lookup(const char *name, void *buffer, uint64_t count) {
    hero_net_info *found = NULL;

    if (!hero_net_nameable(name) || (buffer == NULL && count > 0)) {
        hero_net_fail(HERO_NET_INVALID, 0);
        return -1;
    }
    if (!hero_net_started()) {
        return -1;
    }
    /* The one moment the stop reaches a lookup: before the resolver is
     * asked. No call of the system ends one it has begun. */
    if (hero_net_stopping()) {
        hero_net_fail(HERO_NET_STOPPED, 0);
        return -1;
    }
    /* One socket type, so the system lists each address once. */
    int said = hero_net_infos(name, AF_UNSPEC, SOCK_STREAM, 0, &found);

    if (said != 0) {
        int64_t code = 0;
        int64_t kind = hero_net_name_kind(said, &code);

        hero_net_fail(kind, code);
        return -1;
    }
    char *out = (char *)buffer;
    uint64_t total = 0;
    uint64_t written = 0;
    bool fits = true;

    for (const hero_net_info *at = found; at != NULL; at = at->ai_next) {
        char text[HERO_NET_ADDRESS_MOST];
        int64_t unused = 0;
        int64_t length = at->ai_addr == NULL ? -1 : hero_net_text_of(at->ai_addr, (hero_net_len)at->ai_addrlen, text, &unused);

        if (length < 0) {
            continue;
        }
        /* The address, its newline, and room left for the last zero byte;
         * once one did not fit, none after it is written, so what stands
         * in the buffer is the list's beginning. */
        total += (uint64_t)length + 1;
        fits = fits && total < count;
        if (fits) {
            memcpy(out + written, text, (size_t)length);
            out[written + (uint64_t)length] = '\n';
            written = total;
        }
    }
    hero_net_infos_free(found);

    if (total == 0) {
        hero_net_fail(HERO_NET_NOT_FOUND, 0);
        return -1;
    }
    if (count > 0) {
        out[written] = '\0';
    }
    hero_net_clear();
    return (int64_t)total;
}

int64_t hero_net_name_of(const char *address, void *buffer, uint64_t count) {
    struct sockaddr_storage where;
    hero_net_len size = 0;
    int64_t code = 0;

    if (address == NULL || (buffer == NULL && count > 0)) {
        hero_net_fail(HERO_NET_INVALID, 0);
        return -1;
    }
    if (!hero_net_started()) {
        return -1;
    }
    if (!hero_net_literal(address, 0, 0, &where, &size, &code)) {
        hero_net_fail(HERO_NET_ADDRESS, code);
        return -1;
    }
    if (hero_net_stopping()) {
        hero_net_fail(HERO_NET_STOPPED, 0);
        return -1;
    }
#if defined(_WIN32)
    wchar_t wide[HERO_NET_NAME_MOST + 1];
    int said = GetNameInfoW((const struct sockaddr *)&where, size, wide, HERO_NET_NAME_MOST + 1, NULL, 0, NI_NAMEREQD);
    int needed = said != 0 ? 0 : WideCharToMultiByte(CP_UTF8, 0, wide, -1, NULL, 0, NULL, NULL);
#else
    char host[HERO_NET_NAME_MOST + 1];
    int said = getnameinfo((const struct sockaddr *)&where, size, host, (socklen_t)sizeof host, NULL, 0, NI_NAMEREQD);
    int needed = said != 0 ? 0 : (int)strlen(host) + 1;
#endif

    if (said != 0) {
        int64_t kind = hero_net_name_kind(said, &code);

        hero_net_fail(kind, code);
        return -1;
    }
    /* `needed` counts the name and its zero byte: none is a name the system
     * could not hand over as UTF-8. */
    if (needed <= 1) {
        hero_net_fail(HERO_NET_NOT_FOUND, 0);
        return -1;
    }
    if (count >= (uint64_t)needed) {
#if defined(_WIN32)
        if (WideCharToMultiByte(CP_UTF8, 0, wide, -1, (char *)buffer, needed, NULL, NULL) != needed) {
            hero_net_fail(HERO_NET_OTHER, (int64_t)GetLastError());
            return -1;
        }
#else
        memcpy(buffer, host, (size_t)needed);
#endif
    }
    hero_net_clear();
    return (int64_t)needed - 1;
}

/* The address a socket stands on, or its peer's (`peer` true), as the
 * system holds it now: `getsockname` or `getpeername`, neither of which
 * waits. */
static int64_t hero_net_end(hero_net_socket *s, void *address, uint64_t address_count, int64_t *port, bool peer) {
    hero_net_fd fd = HERO_NET_NO_FD;
    hero_net_slot *slot = hero_net_take(s, &fd);

    if (slot == NULL) {
        return -1;
    }
    if (address == NULL || address_count < HERO_NET_ADDRESS_MOST || port == NULL) {
        (void)hero_net_done(slot);
        hero_net_fail(HERO_NET_INVALID, 0);
        return -1;
    }
    struct sockaddr_storage where;
    hero_net_len size = (hero_net_len)sizeof where;

    memset(&where, 0, sizeof where);
    int failed = peer ? getpeername(fd, (struct sockaddr *)&where, &size) : getsockname(fd, (struct sockaddr *)&where, &size);
    int64_t code = failed != 0 ? hero_net_os_code() : 0;

    (void)hero_net_done(slot);
    if (failed != 0) {
        hero_net_failed_with(code);
        return -1;
    }
    char text[HERO_NET_ADDRESS_MOST];
    int64_t at = 0;
    int64_t length = hero_net_text_of((const struct sockaddr *)&where, size, text, &at);

    if (length < 0) {
        hero_net_fail(HERO_NET_OTHER, 0);
        return -1;
    }
    memcpy(address, text, (size_t)length + 1);
    *port = at;
    hero_net_clear();
    return length;
}

int64_t hero_net_local(hero_net_socket *s, void *address, uint64_t address_count, int64_t *port) {
    return hero_net_end(s, address, address_count, port, false);
}

int64_t hero_net_remote(hero_net_socket *s, void *address, uint64_t address_count, int64_t *port) {
    return hero_net_end(s, address, address_count, port, true);
}

/* -- round 3: a connect with a time limit, one direction closed, the options,
 * multicast, and the interfaces (M-rebel-library, 2026-11-16) ------------- */

hero_net_socket *hero_net_connect_within(const char *address, int64_t port, int64_t ms) {
    int64_t at = 0;

    /* The time as `hero_net_set_deadline` reads it: 0 for
     * `HERO_NET_FOREVER`, no limit; one already passed for 0. */
    if (!hero_net_deadline_at(ms, &at)) {
        return NULL;
    }
    return hero_net_open(address, port, false, at);
}

/* Whether the connection or listener a call holds listens, which has no
 * connection to shut. */
static bool hero_net_listens(hero_net_fd fd) {
    int said = 0;
    hero_net_len size = (hero_net_len)sizeof said;

    return getsockopt(fd, SOL_SOCKET, SO_ACCEPTCONN, (char *)&said, &size) == 0 && said != 0;
}

/* Whether a call failed as a socket with no connection. */
static bool hero_net_not_connected(int64_t code) {
#if defined(_WIN32)
    return code == WSAENOTCONN;
#else
    return code == ENOTCONN;
#endif
}

int64_t hero_net_shutdown(hero_net_socket *s, int64_t how) {
    hero_net_fd fd = HERO_NET_NO_FD;

    if (how != HERO_NET_SHUT_READ && how != HERO_NET_SHUT_WRITE && how != HERO_NET_SHUT_BOTH) {
        hero_net_fail(HERO_NET_INVALID, 0);
        return -1;
    }
    hero_net_slot *slot = hero_net_take_kind(s, &fd, false, NULL);

    if (slot == NULL) {
        return -1;
    }
    /* A listener is asked first: Linux would stop it listening, where
     * Darwin and Windows refuse it as no connection. */
    if (hero_net_listens(fd)) {
        (void)hero_net_done(slot);
        hero_net_fail(HERO_NET_RESET, 0);
        return -1;
    }
#if defined(_WIN32)
    int way = how == HERO_NET_SHUT_READ ? SD_RECEIVE : how == HERO_NET_SHUT_WRITE ? SD_SEND : SD_BOTH;
#else
    int way = how == HERO_NET_SHUT_READ ? SHUT_RD : how == HERO_NET_SHUT_WRITE ? SHUT_WR : SHUT_RDWR;
#endif
    int failed = shutdown(fd, way);
    int64_t code = failed != 0 ? hero_net_os_code() : 0;

    /* "Not connected" where the socket still has a peer is a direction at
     * its end already: Darwin refuses to shut a reading side the peer's
     * close has ended, and both POSIX systems a side shut before, where
     * Linux shuts the first; a reset connection has no peer and stays
     * refused (measured 2026-11-16 on Darwin and Linux arm64). */
    if (failed != 0 && hero_net_not_connected(code)) {
        struct sockaddr_storage peer;
        hero_net_len peer_size = (hero_net_len)sizeof peer;

        if (getpeername(fd, (struct sockaddr *)&peer, &peer_size) == 0) {
            failed = 0;
            code = 0;
        }
    }
    if (failed == 0) {
        /* Every call waiting on the socket tries again, and meets the
         * direction closed: a `recv` its 0, a `send` its failure. */
        hero_net_enter();
        hero_net_wake_ready(slot);
        hero_net_leave();
    }
    (void)hero_net_done(slot);
    if (failed != 0) {
        hero_net_failed_with(code);
        return -1;
    }
    hero_net_clear();
    return 0;
}

/* The level and the name of `option` on a socket of the kind `datagram`
 * says; false for one that kind has not. */
static bool hero_net_option_of(int64_t option, bool datagram, int *level, int *name) {
    if (option == HERO_NET_OPTION_NO_DELAY && !datagram) {
        *level = IPPROTO_TCP;
        *name = TCP_NODELAY;
        return true;
    }
    if (option == HERO_NET_OPTION_KEEP_ALIVE && !datagram) {
        *level = SOL_SOCKET;
        *name = SO_KEEPALIVE;
        return true;
    }
    if (option == HERO_NET_OPTION_BROADCAST && datagram) {
        *level = SOL_SOCKET;
        *name = SO_BROADCAST;
        return true;
    }
    return false;
}

/* Sets the interface the datagrams of the socket `fd`, of `family` 4 or 6,
 * leave by for a group, by its index, 0 for the system's choice: 0, or -1
 * with the system's number to read. Each platform takes the index its own
 * way: IPv6 as an unsigned number everywhere; IPv4 on Windows as an address
 * `0.0.0.index` in network order (its `IPPROTO_IP` page), on Darwin by
 * `IP_MULTICAST_IFINDEX`, on Linux in an `ip_mreqn`. */
static int hero_net_multicast_if(hero_net_fd fd, int family, uint32_t index) {
    if (family == 6) {
#if defined(_WIN32)
        DWORD v6 = (DWORD)index;
#else
        unsigned int v6 = (unsigned int)index;
#endif
        return setsockopt(fd, IPPROTO_IPV6, IPV6_MULTICAST_IF, (const char *)&v6, (hero_net_len)sizeof v6);
    }
#if defined(_WIN32)
    DWORD v4 = htonl((u_long)index);

    return setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, (const char *)&v4, (hero_net_len)sizeof v4);
#elif defined(IP_MULTICAST_IFINDEX)
    unsigned int v4 = (unsigned int)index;

    return setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IFINDEX, &v4, (hero_net_len)sizeof v4);
#elif defined(__linux__)
    struct ip_mreqn v4;

    memset(&v4, 0, sizeof v4);
    v4.imr_ifindex = (int)index;
    return setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &v4, (hero_net_len)sizeof v4);
#else
    (void)fd;
    (void)index;
    errno = ENOPROTOOPT;
    return -1;
#endif
}

/* The three multicast options of a datagram socket (round 3), set (`value`
 * 0 or more) or asked (`value` -1): the interface kept in the slot as well
 * as set, the loop and the hops asked of the system, as a 4-byte `int` both
 * ways, of which Darwin writes the first byte alone for IPv4 (its `u_char`)
 * and leaves the rest at 0. Called with the slot taken, which it lets go. */
static int64_t hero_net_multicast_opt(hero_net_slot *slot, hero_net_fd fd, int64_t option, int64_t value) {
    int family = (int)slot->datagram;
    int failed = 0;
    int said = 0;
    int64_t answer = 0;

    if (family == 0) {
        (void)hero_net_done(slot);
        hero_net_fail(HERO_NET_INVALID, 0);
        return -1;
    }
    if (option == HERO_NET_OPTION_MULTICAST_INTERFACE) {
        if (value < 0) {
            hero_net_enter();
            answer = (int64_t)slot->multicast_interface;
            hero_net_leave();
        } else {
            failed = hero_net_multicast_if(fd, family, (uint32_t)value);
            if (failed == 0) {
                hero_net_enter();
                slot->multicast_interface = (uint32_t)value;
                hero_net_leave();
            }
        }
    } else {
        int level = family == 6 ? IPPROTO_IPV6 : IPPROTO_IP;
        int name = option == HERO_NET_OPTION_MULTICAST_LOOP ? (family == 6 ? IPV6_MULTICAST_LOOP : IP_MULTICAST_LOOP)
                                                            : (family == 6 ? IPV6_MULTICAST_HOPS : IP_MULTICAST_TTL);
        hero_net_len size = (hero_net_len)sizeof said;

        if (value < 0) {
            failed = getsockopt(fd, level, name, (char *)&said, &size);
            answer = option == HERO_NET_OPTION_MULTICAST_LOOP ? (said != 0 ? 1 : 0) : (int64_t)said;
        } else {
            said = (int)value;
            failed = setsockopt(fd, level, name, (const char *)&said, size);
        }
    }
    int64_t code = failed != 0 ? hero_net_os_code() : 0;

    (void)hero_net_done(slot);
    if (failed != 0) {
        hero_net_failed_with(code);
        return -1;
    }
    hero_net_clear();
    return answer;
}

/* `hero_net_set_option` (`value` within the option's range) and
 * `hero_net_option` (`value` -1, asked): the slot taken, its kind read as
 * it was issued, the system asked once. */
static int64_t hero_net_opt(hero_net_socket *s, int64_t option, int64_t value) {
    hero_net_fd fd = HERO_NET_NO_FD;
    int level = 0;
    int name = 0;

    hero_net_slot *slot = hero_net_take(s, &fd);

    if (slot == NULL) {
        return -1;
    }
    if (option == HERO_NET_OPTION_MULTICAST_INTERFACE || option == HERO_NET_OPTION_MULTICAST_LOOP ||
        option == HERO_NET_OPTION_MULTICAST_HOPS) {
        return hero_net_multicast_opt(slot, fd, option, value);
    }
    if (!hero_net_option_of(option, slot->datagram != 0, &level, &name)) {
        (void)hero_net_done(slot);
        hero_net_fail(HERO_NET_INVALID, 0);
        return -1;
    }
    /* A 4-byte `int` both ways; Windows' `BOOL` is one, and an answer of
     * fewer bytes leaves the rest at 0. */
    int said = value == 1 ? 1 : 0;
    hero_net_len size = (hero_net_len)sizeof said;
    int failed = value < 0 ? getsockopt(fd, level, name, (char *)&said, &size) : setsockopt(fd, level, name, (const char *)&said, size);
    int64_t code = failed != 0 ? hero_net_os_code() : 0;

    (void)hero_net_done(slot);
    if (failed != 0) {
        hero_net_failed_with(code);
        return -1;
    }
    hero_net_clear();
    return value < 0 ? (said != 0 ? 1 : 0) : 0;
}

int64_t hero_net_set_option(hero_net_socket *s, int64_t option, int64_t value) {
    int64_t most = 1;

    if (option == HERO_NET_OPTION_MULTICAST_INTERFACE) {
        most = (int64_t)UINT32_MAX;
    } else if (option == HERO_NET_OPTION_MULTICAST_HOPS) {
        most = 255;
    }
    if (value < 0 || value > most) {
        hero_net_fail(HERO_NET_INVALID, 0);
        return -1;
    }
    return hero_net_opt(s, option, value);
}

int64_t hero_net_option(hero_net_socket *s, int64_t option) {
    return hero_net_opt(s, option, -1);
}

/* Whether `where` holds a multicast address: 224.0.0.0/4, or ff00::/8. */
static bool hero_net_multicast(const struct sockaddr_storage *where) {
    if (where->ss_family == AF_INET) {
        struct sockaddr_in four;

        memcpy(&four, where, sizeof four);
        return (ntohl(four.sin_addr.s_addr) >> 28) == 0xEu;
    }
    if (where->ss_family == AF_INET6) {
        struct sockaddr_in6 six;

        memcpy(&six, where, sizeof six);
        return six.sin6_addr.s6_addr[0] == 0xFFu;
    }
    return false;
}

/* The IPv6 calls that join and leave a group on an interface the system
 * chooses: POSIX's names, which Windows spells as the older ones too. */
#if defined(IPV6_JOIN_GROUP)
#define HERO_NET_IPV6_JOIN IPV6_JOIN_GROUP
#define HERO_NET_IPV6_LEAVE IPV6_LEAVE_GROUP
#else
#define HERO_NET_IPV6_JOIN IPV6_ADD_MEMBERSHIP
#define HERO_NET_IPV6_LEAVE IPV6_DROP_MEMBERSHIP
#endif

static int64_t hero_net_group(hero_net_socket *s, const char *group, int64_t interface_index, bool joining) {
    hero_net_fd fd = HERO_NET_NO_FD;
    int family = 0;
    hero_net_slot *slot = hero_net_take_kind(s, &fd, true, &family);

    if (slot == NULL) {
        return -1;
    }
    if (interface_index < 0 || interface_index > (int64_t)UINT32_MAX) {
        (void)hero_net_done(slot);
        hero_net_fail(HERO_NET_INVALID, 0);
        return -1;
    }
    struct sockaddr_storage where;
    hero_net_len size = 0;
    int64_t code = 0;

    if (!hero_net_literal(group, family, 0, &where, &size, &code) || !hero_net_multicast(&where)) {
        (void)hero_net_done(slot);
        hero_net_fail(HERO_NET_ADDRESS, code);
        return -1;
    }
    int level = family == 6 ? IPPROTO_IPV6 : IPPROTO_IP;
    int failed = 0;

    if (interface_index == 0) {
        /* The system's choice, by its routes: the classic calls, since
         * Darwin's `MCAST_JOIN_GROUP` refuses index 0 with `EADDRNOTAVAIL`
         * where Linux's takes it (measured 2026-11-16 on both). */
        if (family == 6) {
            struct sockaddr_in6 six;
            struct ipv6_mreq six_asked;

            memcpy(&six, &where, sizeof six);
            memset(&six_asked, 0, sizeof six_asked);
            six_asked.ipv6mr_multiaddr = six.sin6_addr;
            six_asked.ipv6mr_interface = 0;
            failed = setsockopt(fd, level, joining ? HERO_NET_IPV6_JOIN : HERO_NET_IPV6_LEAVE, (const char *)&six_asked,
                                (hero_net_len)sizeof six_asked);
        } else {
            struct sockaddr_in four;
            struct ip_mreq four_asked;

            memcpy(&four, &where, sizeof four);
            memset(&four_asked, 0, sizeof four_asked);
            four_asked.imr_multiaddr = four.sin_addr;
            four_asked.imr_interface.s_addr = htonl(INADDR_ANY);
            failed = setsockopt(fd, level, joining ? IP_ADD_MEMBERSHIP : IP_DROP_MEMBERSHIP, (const char *)&four_asked,
                                (hero_net_len)sizeof four_asked);
        }
    } else {
        /* An interface named by its index: RFC 3678's call, the same for
         * both families on the three platforms. */
        struct group_req asked;

        memset(&asked, 0, sizeof asked);
        asked.gr_interface = (uint32_t)interface_index;
        memcpy(&asked.gr_group, &where, (size_t)size);
        failed = setsockopt(fd, level, joining ? MCAST_JOIN_GROUP : MCAST_LEAVE_GROUP, (const char *)&asked, (hero_net_len)sizeof asked);
    }

    code = failed != 0 ? hero_net_os_code() : 0;
    (void)hero_net_done(slot);
    if (failed != 0) {
        hero_net_failed_with(code);
        return -1;
    }
    hero_net_clear();
    return 0;
}

int64_t hero_net_join_group(hero_net_socket *s, const char *group, int64_t interface_index) {
    return hero_net_group(s, group, interface_index, true);
}

int64_t hero_net_leave_group(hero_net_socket *s, const char *group, int64_t interface_index) {
    return hero_net_group(s, group, interface_index, false);
}

/* One field of an interface's entry, `length` bytes and a zero byte, into
 * `buffer` where each byte fits, and the bytes the list needs with it. */
static uint64_t hero_net_field(void *buffer, uint64_t count, uint64_t needed, const char *bytes, size_t length) {
    unsigned char *out = buffer;

    for (size_t i = 0; i < length; i += 1) {
        if (needed + i < count) {
            out[needed + i] = (unsigned char)bytes[i];
        }
    }
    if (needed + length < count) {
        out[needed + length] = 0;
    }
    return needed + (uint64_t)length + 1;
}

/* A number as a field, in decimal digits. */
static uint64_t hero_net_number_field(void *buffer, uint64_t count, uint64_t needed, int64_t number) {
    char digits[24];
    int wrote = snprintf(digits, sizeof digits, "%lld", (long long)number);

    return hero_net_field(buffer, count, needed, digits, wrote > 0 ? (size_t)wrote : 0);
}

/* One entry: a name, an index, flags, an address as text ("" for none) and
 * its prefix length. */
static uint64_t hero_net_entry(void *buffer, uint64_t count, uint64_t needed, const char *name, size_t name_length, int64_t index,
                               int64_t flags, const char *address, int64_t prefix) {
    needed = hero_net_field(buffer, count, needed, name, name_length);
    needed = hero_net_number_field(buffer, count, needed, index);
    needed = hero_net_number_field(buffer, count, needed, flags);
    needed = hero_net_field(buffer, count, needed, address, strlen(address));
    return hero_net_number_field(buffer, count, needed, prefix);
}

#if defined(_WIN32)

/* The 64 KiB `GetAdaptersAddresses` is handed, on the caller's stack (the
 * runtime allocates in `parts/alloc.c` alone): the record first, so the
 * bytes are aligned as the record is. Its own page suggests 15 KB. */
#define HERO_NET_ADAPTERS_BYTES 65536u

int64_t hero_net_interfaces(void *buffer, uint64_t count) {
    union {
        IP_ADAPTER_ADDRESSES first;
        unsigned char bytes[HERO_NET_ADAPTERS_BYTES];
    } store;
    ULONG size = (ULONG)sizeof store;
    uint64_t needed = 0;

    if (buffer == NULL && count > 0) {
        hero_net_fail(HERO_NET_INVALID, 0);
        return -1;
    }
    /* The addresses' text is written by `GetNameInfoW`, a call of the
     * sockets, which asks them started. */
    if (!hero_net_started()) {
        return -1;
    }
    ULONG said = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER, NULL, &store.first, &size);

    if (said == ERROR_BUFFER_OVERFLOW) {
        hero_net_fail(HERO_NET_LIMIT, (int64_t)said);
        return -1;
    }
    if (said == ERROR_NO_DATA) {
        hero_net_clear();
        return 0;
    }
    if (said != NO_ERROR) {
        hero_net_fail(HERO_NET_OTHER, (int64_t)said);
        return -1;
    }
    for (const IP_ADAPTER_ADDRESSES *a = &store.first; a != NULL; a = a->Next) {
        char name[1024];
        int wrote = a->FriendlyName == NULL ? 1 : WideCharToMultiByte(CP_UTF8, 0, a->FriendlyName, -1, name, (int)sizeof name, NULL, NULL);
        int64_t flags = 0;
        bool any = false;

        if (a->FriendlyName == NULL) {
            name[0] = '\0';
        }
        if (wrote <= 0) {
            hero_net_fail(HERO_NET_LIMIT, (int64_t)GetLastError());
            return -1;
        }
        if (a->OperStatus == IfOperStatusUp) {
            flags |= HERO_NET_INTERFACE_UP | HERO_NET_INTERFACE_RUNNING;
        }
        if (a->IfType == IF_TYPE_SOFTWARE_LOOPBACK) {
            flags |= HERO_NET_INTERFACE_LOOPBACK;
        }
        if (a->IfType == IF_TYPE_ETHERNET_CSMACD || a->IfType == IF_TYPE_IEEE80211) {
            flags |= HERO_NET_INTERFACE_BROADCAST;
        }
        if ((a->Flags & IP_ADAPTER_NO_MULTICAST) == 0) {
            flags |= HERO_NET_INTERFACE_MULTICAST;
        }
        if (a->IfType == IF_TYPE_PPP || a->IfType == IF_TYPE_TUNNEL) {
            flags |= HERO_NET_INTERFACE_POINT_TO_POINT;
        }
        int64_t index = a->IfIndex != 0 ? (int64_t)a->IfIndex : (int64_t)a->Ipv6IfIndex;

        for (const IP_ADAPTER_UNICAST_ADDRESS *u = a->FirstUnicastAddress; u != NULL; u = u->Next) {
            char text[HERO_NET_ADDRESS_MOST];
            int64_t port = 0;

            if (u->Address.lpSockaddr == NULL ||
                hero_net_text_of(u->Address.lpSockaddr, (hero_net_len)u->Address.iSockaddrLength, text, &port) < 0) {
                continue;
            }
            any = true;
            needed = hero_net_entry(buffer, count, needed, name, (size_t)wrote - 1, index, flags, text, (int64_t)u->OnLinkPrefixLength);
        }
        if (!any) {
            needed = hero_net_entry(buffer, count, needed, name, (size_t)wrote - 1, index, flags, "", 0);
        }
    }
    hero_net_clear();
    return (int64_t)needed;
}

#else

/* How many leading bits of `mask` are set, the prefix length of an address
 * of `family`; 0 where there is no mask. Darwin hands a mask cut to the
 * bytes it needs (`sa_len`), and its family may be 0, so the bytes are
 * copied up to that length into a record cleared first, and read as the
 * address's family says. */
static int64_t hero_net_prefix_of(const struct sockaddr *mask, int family) {
    struct sockaddr_storage whole;
    const unsigned char *bytes = NULL;
    size_t length = 0;
    size_t room = family == AF_INET6 ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in);
    int64_t bits = 0;

    if (mask == NULL) {
        return 0;
    }
#if defined(__APPLE__)
    if ((size_t)mask->sa_len < room) {
        room = (size_t)mask->sa_len;
    }
#endif
    memset(&whole, 0, sizeof whole);
    memcpy(&whole, mask, room);
    if (family == AF_INET6) {
        bytes = ((const struct sockaddr_in6 *)(const void *)&whole)->sin6_addr.s6_addr;
        length = 16;
    } else {
        bytes = (const unsigned char *)&((const struct sockaddr_in *)(const void *)&whole)->sin_addr.s_addr;
        length = 4;
    }
    for (size_t i = 0; i < length; i += 1) {
        unsigned char b = bytes[i];

        while ((b & 0x80u) != 0) {
            bits += 1;
            b = (unsigned char)(b << 1);
        }
        if (bytes[i] != 0xFFu) {
            break;
        }
    }
    return bits;
}

/* Whether the interface of `name` has an IPv4 or IPv6 address in the list
 * `all`. */
static bool hero_net_has_address(const struct ifaddrs *all, const char *name) {
    for (const struct ifaddrs *e = all; e != NULL; e = e->ifa_next) {
        if (e->ifa_addr != NULL && (e->ifa_addr->sa_family == AF_INET || e->ifa_addr->sa_family == AF_INET6) &&
            strcmp(e->ifa_name, name) == 0) {
            return true;
        }
    }
    return false;
}

/* Whether an entry of `name` with no address stands before `upto`. */
static bool hero_net_bare_before(const struct ifaddrs *all, const struct ifaddrs *upto, const char *name) {
    for (const struct ifaddrs *e = all; e != NULL && e != upto; e = e->ifa_next) {
        bool addressed = e->ifa_addr != NULL && (e->ifa_addr->sa_family == AF_INET || e->ifa_addr->sa_family == AF_INET6);

        if (!addressed && strcmp(e->ifa_name, name) == 0) {
            return true;
        }
    }
    return false;
}

int64_t hero_net_interfaces(void *buffer, uint64_t count) {
    struct ifaddrs *all = NULL;
    uint64_t needed = 0;

    if (buffer == NULL && count > 0) {
        hero_net_fail(HERO_NET_INVALID, 0);
        return -1;
    }
    if (getifaddrs(&all) != 0) {
        hero_net_failed_with(hero_net_os_code());
        return -1;
    }
    for (const struct ifaddrs *e = all; e != NULL; e = e->ifa_next) {
        if (e->ifa_name == NULL) {
            continue;
        }
        unsigned int raw = e->ifa_flags;
        int64_t flags = 0;

        flags |= (raw & IFF_UP) != 0 ? HERO_NET_INTERFACE_UP : 0;
        flags |= (raw & IFF_RUNNING) != 0 ? HERO_NET_INTERFACE_RUNNING : 0;
        flags |= (raw & IFF_LOOPBACK) != 0 ? HERO_NET_INTERFACE_LOOPBACK : 0;
        flags |= (raw & IFF_BROADCAST) != 0 ? HERO_NET_INTERFACE_BROADCAST : 0;
        flags |= (raw & IFF_MULTICAST) != 0 ? HERO_NET_INTERFACE_MULTICAST : 0;
        flags |= (raw & IFF_POINTOPOINT) != 0 ? HERO_NET_INTERFACE_POINT_TO_POINT : 0;

        int64_t index = (int64_t)if_nametoindex(e->ifa_name);
        int family = e->ifa_addr == NULL ? AF_UNSPEC : e->ifa_addr->sa_family;

        if (family == AF_INET || family == AF_INET6) {
            struct sockaddr_storage where;
            char text[HERO_NET_ADDRESS_MOST];
            int64_t port = 0;
            hero_net_len size = (hero_net_len)(family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6));

#if defined(__APPLE__)
            /* Never more than the address's own bytes; one shorter than its
             * record is then no address `hero_net_text_of` writes. */
            if ((size_t)e->ifa_addr->sa_len < (size_t)size) {
                size = (hero_net_len)e->ifa_addr->sa_len;
            }
#endif
            memset(&where, 0, sizeof where);
            memcpy(&where, e->ifa_addr, (size_t)size);
#if defined(__APPLE__)
            /* Darwin's kernel keeps a link-local address's scope inside its
             * third and fourth bytes, and `getifaddrs` hands it so with no
             * scope id: moved where an address's scope stands, so the text
             * is the address with its `%` and interface. */
            if (family == AF_INET6) {
                struct sockaddr_in6 six;

                memcpy(&six, &where, sizeof six);
                if (six.sin6_addr.s6_addr[0] == 0xFEu && (six.sin6_addr.s6_addr[1] & 0xC0u) == 0x80u && six.sin6_scope_id == 0) {
                    six.sin6_scope_id = (uint32_t)((six.sin6_addr.s6_addr[2] << 8) | six.sin6_addr.s6_addr[3]);
                    six.sin6_addr.s6_addr[2] = 0;
                    six.sin6_addr.s6_addr[3] = 0;
                    memcpy(&where, &six, sizeof six);
                }
            }
#endif
            if (hero_net_text_of((const struct sockaddr *)&where, size, text, &port) < 0) {
                continue;
            }
            needed = hero_net_entry(buffer, count, needed, e->ifa_name, strlen(e->ifa_name), index, flags, text,
                                    hero_net_prefix_of(e->ifa_netmask, family));
        } else if (!hero_net_has_address(all, e->ifa_name) && !hero_net_bare_before(all, e, e->ifa_name)) {
            needed = hero_net_entry(buffer, count, needed, e->ifa_name, strlen(e->ifa_name), index, flags, "", 0);
        }
    }
    freeifaddrs(all);
    hero_net_clear();
    return (int64_t)needed;
}

#endif

/* -- round 4: an echo asked by ICMP, without privileges (M-rebel-library,
 * 2026-11-17) ------------------------------------------------------------
 *
 * ONE DOOR AND NO HANDLE. Windows answers an echo with one call that waits
 * in the system (`IcmpSendEcho2`), on a handle of its own that is no socket
 * this table could hold, so a door that opened a socket, sent and waited in
 * three calls would have no Windows arm to match. And an echo is asked
 * once, answered once: on Darwin and Linux the socket is made, kept in a
 * slot of the table for as long as the call waits, so it waits through the
 * layer's own thread with the deadline and the stop every wait has, and
 * given back before the call returns. No state of its own outlives the
 * call.
 *
 * NEVER A RAW SOCKET. A datagram socket of the ICMP protocol is the one a
 * system lets a program without privileges make, where it lets it at all:
 * Darwin always (measured 2026-11-17, this Mac, as its own `ping` does
 * without root), Linux where the program's group is in
 * `net.ipv4.ping_group_range` (`EACCES` otherwise, which is `DENIED`). What
 * the two hand back differs, measured on this Mac: Darwin gives an IPv4
 * socket the reply WITH its IP header, and an IPv6 socket on the loopback
 * the request it sent before the reply; Linux gives the ICMP message alone,
 * and only the replies bearing the identifier it wrote over the request's.
 * So a datagram is read for what it is, a header first where one stands,
 * and every datagram that is not this echo's reply is passed over, each
 * receive moving one datagram, until the deadline. */

#if !defined(_WIN32)

/* The bytes past the echo's data a receive takes room for: an IPv4 header
 * at its longest, 60, before a reply on Darwin, and an ICMP error naming
 * the echo, which quotes the request's headers (8 + 60 + 8 for IPv4, 8 +
 * 40 + 8 for IPv6), with room beside. A datagram longer than the room is no
 * reply to this echo and is passed over. */
#define HERO_NET_ECHO_ROOM 136u

/* What one datagram the echo's socket received is to the echo. */
typedef enum {
    /* Another's, or one this echo did not ask. */
    HERO_NET_ECHO_OTHER,
    /* This echo's reply, its sequence and its data. */
    HERO_NET_ECHO_REPLY,
    /* An ICMP error quoting this echo's request: unreachable, or expired on
     * its way. */
    HERO_NET_ECHO_ERROR
} hero_net_echo_is;

/* The ones' complement of the ones' complement sum of `n` bytes taken two at
 * a time, the first the higher (RFC 792, RFC 1071): the checksum an IPv4
 * ICMP message carries, computed with its own field at 0. IPv6's covers a
 * header the system writes, and the system computes it (RFC 3542 3.1). */
static uint16_t hero_net_echo_sum(const unsigned char *bytes, size_t n) {
    uint64_t sum = 0;

    for (size_t i = 0; i + 1 < n; i += 2) {
        sum += (uint64_t)bytes[i] << 8 | (uint64_t)bytes[i + 1];
    }
    if (n % 2 == 1) {
        sum += (uint64_t)bytes[n - 1] << 8;
    }
    while (sum >> 16 != 0) {
        sum = (sum & UINT64_C(0xffff)) + (sum >> 16);
    }
    return (uint16_t)(~sum & UINT64_C(0xffff));
}

/* The 16-bit number at `at` in `bytes`, the first byte the higher. */
static uint32_t hero_net_echo_word(const unsigned char *bytes, size_t at) {
    return (uint32_t)bytes[at] << 8 | (uint32_t)bytes[at + 1];
}

/* Whether the `n` bytes at `icmp` begin this echo's request: its type, its
 * sequence, and where the system keeps the request's identifier, which
 * Linux writes over, the identifier. */
static bool hero_net_echo_asked(const unsigned char *icmp, size_t n, bool six, uint32_t id, uint32_t sequence) {
    if (n < 8 || icmp[0] != (six ? 128u : 8u)) {
        return false;
    }
#if defined(__linux__)
    (void)id;
#else
    if (hero_net_echo_word(icmp, 4) != id) {
        return false;
    }
#endif
    return hero_net_echo_word(icmp, 6) == sequence;
}

/* What the `n` bytes received at `got` are to the echo of `id`, `sequence`
 * and the `count` bytes of `data`; for an IPv4 datagram that arrived with
 * its header, the header's time to live in `*header_ttl`, else -1. */
static hero_net_echo_is hero_net_echo_read(const unsigned char *got, size_t n, bool six, uint32_t id, uint32_t sequence, const unsigned char *data,
                                           uint64_t count, int64_t *header_ttl) {
    *header_ttl = -1;
    /* An ICMP message begins with its type, and no type is 0x45 to 0x4f, so
     * a first half-byte of 4 is an IPv4 header Darwin left on. */
    if (!six && n >= 20 && got[0] >> 4 == 4) {
        size_t header = (size_t)(got[0] & 0x0f) * 4;

        if (header < 20 || header > n) {
            return HERO_NET_ECHO_OTHER;
        }
        *header_ttl = (int64_t)got[8];
        got += header;
        n -= header;
    }
    if (n < 8) {
        return HERO_NET_ECHO_OTHER;
    }
    unsigned type = got[0];

    if (type == (six ? 129u : 0u)) {
        if (got[1] != 0) {
            return HERO_NET_ECHO_OTHER;
        }
#if !defined(__linux__)
        if (hero_net_echo_word(got, 4) != id) {
            return HERO_NET_ECHO_OTHER;
        }
#endif
        if (hero_net_echo_word(got, 6) != sequence || (uint64_t)(n - 8) != count || (count > 0 && memcmp(got + 8, data, (size_t)count) != 0)) {
            return HERO_NET_ECHO_OTHER;
        }
        return HERO_NET_ECHO_REPLY;
    }
    /* Destination unreachable and time exceeded: IPv4's 3 and 11, IPv6's 1
     * and 3 (RFC 792, RFC 4443), each quoting the request after its own 8
     * bytes. */
    bool error = six ? (type == 1 || type == 3) : (type == 3 || type == 11);

    if (!error) {
        return HERO_NET_ECHO_OTHER;
    }
    const unsigned char *quoted = got + 8;
    size_t left = n - 8;

    if (six) {
        /* The request's IPv6 header, 40 bytes, its next header ICMPv6 (58):
         * this call sends no extension header between them. */
        if (left < 40 || quoted[6] != 58) {
            return HERO_NET_ECHO_OTHER;
        }
        quoted += 40;
        left -= 40;
    } else {
        if (left < 20 || quoted[0] >> 4 != 4 || quoted[9] != 1) {
            return HERO_NET_ECHO_OTHER;
        }
        size_t header = (size_t)(quoted[0] & 0x0f) * 4;

        if (header < 20 || header > left) {
            return HERO_NET_ECHO_OTHER;
        }
        quoted += header;
        left -= header;
    }
    return hero_net_echo_asked(quoted, left, six, id, sequence) ? HERO_NET_ECHO_ERROR : HERO_NET_ECHO_OTHER;
}

/* The time to live, or the hop limit, the system told beside a datagram it
 * handed `heard`, 0 to 255, or -1 where it told none: Linux's `IP_TTL` as
 * an `int`, Darwin's `IP_RECVTTL` as one byte, and `IPV6_HOPLIMIT` as an
 * `int` on both (RFC 3542 6.3). */
static int64_t hero_net_echo_told(struct msghdr *heard) {
    for (struct cmsghdr *c = CMSG_FIRSTHDR(heard); c != NULL; c = CMSG_NXTHDR(heard, c)) {
        bool four = c->cmsg_level == IPPROTO_IP && (c->cmsg_type == IP_TTL
#if defined(IP_RECVTTL)
                                                    || c->cmsg_type == IP_RECVTTL
#endif
                                                   );
#if defined(IPV6_HOPLIMIT)
        bool six = c->cmsg_level == IPPROTO_IPV6 && c->cmsg_type == IPV6_HOPLIMIT;
#else
        bool six = false;
#endif

        if (!four && !six) {
            continue;
        }
        if ((size_t)c->cmsg_len >= (size_t)CMSG_LEN(sizeof(int))) {
            int told = 0;

            memcpy(&told, CMSG_DATA(c), sizeof told);
            return told >= 0 && told <= 255 ? (int64_t)told : -1;
        }
        if ((size_t)c->cmsg_len >= (size_t)CMSG_LEN(1)) {
            unsigned char told = 0;

            memcpy(&told, CMSG_DATA(c), 1);
            return (int64_t)told;
        }
    }
    return -1;
}

/* One datagram the echo's socket received into `buffer`, its sender in
 * `from` and `from_size`, the time to live told beside it in `*told`:
 * tried first, then waited for on the layer's poller with the slot's
 * deadline and the stop, as `hero_net_datagram_in` waits. `*cut` where it
 * was longer than `count`. The bytes, or -1 with the kind and the code. */
static int64_t hero_net_echo_in(hero_net_slot *slot, hero_net_fd fd, unsigned char *buffer, size_t count, struct sockaddr_storage *from,
                                hero_net_len *from_size, bool *cut, int64_t *told, int64_t *kind, int64_t *code) {
    for (;;) {
        /* Aligned as a `cmsghdr`, which the system writes into it. */
        union {
            struct cmsghdr header;
            unsigned char bytes[256];
        } control;
        struct iovec into = { buffer, count };
        struct msghdr heard;

        memset(&control, 0, sizeof control);
        memset(&heard, 0, sizeof heard);
        memset(from, 0, sizeof *from);
        heard.msg_name = from;
        heard.msg_namelen = (socklen_t)sizeof *from;
        heard.msg_iov = &into;
        heard.msg_iovlen = 1;
        heard.msg_control = control.bytes;
        heard.msg_controllen = (socklen_t)sizeof control.bytes;
        ssize_t got = recvmsg(fd, &heard, 0);

        if (got >= 0) {
            *cut = (heard.msg_flags & MSG_TRUNC) != 0;
            *from_size = heard.msg_namelen;
            *told = hero_net_echo_told(&heard);
            return (int64_t)got;
        }
        *code = hero_net_os_code();
        if (hero_net_interrupted(*code)) {
            continue;
        }
        if (!hero_net_would_wait(*code)) {
            *kind = hero_net_kind_of(*code);
            return -1;
        }
        if (!hero_net_datagram_waited(slot, fd, false, kind, code)) {
            return -1;
        }
    }
}

/* The kind a refusal to make the echo's socket is: no such socket for the
 * family or the protocol, `UNSUPPORTED`; every other as `hero_net_kind_of`
 * names it, Linux's `EACCES` outside `ping_group_range` `DENIED`. */
static int64_t hero_net_echo_refused(int64_t code) {
    if (code == EPROTONOSUPPORT || code == EAFNOSUPPORT || code == EPROTOTYPE
#if defined(ESOCKTNOSUPPORT)
        || code == ESOCKTNOSUPPORT
#endif
#if defined(EPFNOSUPPORT)
        || code == EPFNOSUPPORT
#endif
    ) {
        return HERO_NET_UNSUPPORTED;
    }
    return hero_net_kind_of(code);
}

/* The echo of `hero_net_ping`, its arguments checked, to `where`. */
static int64_t hero_net_echo(const struct sockaddr_storage *where, hero_net_len size, uint32_t sequence, const void *data, uint64_t count, int64_t ms,
                             char *from, int64_t *ttl, int64_t *nanoseconds) {
    bool six = where->ss_family == AF_INET6;
    int family = six ? AF_INET6 : AF_INET;
    hero_net_fd fd = socket(family, SOCK_DGRAM, six ? IPPROTO_ICMPV6 : IPPROTO_ICMP);

    if (fd == HERO_NET_NO_FD) {
        int64_t code = hero_net_os_code();

        hero_net_fail(hero_net_echo_refused(code), code);
        return -1;
    }
    if (!hero_net_ready(fd) || !hero_net_blocking(fd, false)) {
        int64_t kind = hero_net_kind;
        int64_t code = hero_net_os;

        (void)hero_net_close_fd(fd);
        hero_net_fail(kind, code);
        return -1;
    }
    /* Asked, never required: where the system refuses, Darwin's IPv4 reply
     * still carries its header's, and elsewhere the answer is -1. */
    int on = 1;

    if (six) {
#if defined(IPV6_RECVHOPLIMIT)
        (void)setsockopt(fd, IPPROTO_IPV6, IPV6_RECVHOPLIMIT, &on, (hero_net_len)sizeof on);
#endif
    } else {
#if defined(IP_RECVTTL)
        (void)setsockopt(fd, IPPROTO_IP, IP_RECVTTL, &on, (hero_net_len)sizeof on);
#endif
    }
    (void)on;
    /* The process and the descriptor, which no two sockets of one process
     * share at once: two echoes of one process at once are told apart where
     * the system keeps the identifier. */
    uint32_t id = ((uint32_t)getpid() * 31u + (uint32_t)fd) & 0xffffu;
    hero_net_socket *handle = hero_net_keep_datagram(fd, family);

    if (handle == NULL) {
        return -1;
    }
    int64_t kind = HERO_NET_OK;
    int64_t code = 0;
    int64_t answer = -1;

    if (hero_net_set_deadline(handle, ms) != 0) {
        kind = hero_net_kind;
        code = hero_net_os;
        (void)hero_net_close(handle);
        hero_net_fail(kind, code);
        return -1;
    }
    hero_net_slot *slot = hero_net_take(handle, &fd);

    if (slot == NULL) {
        kind = hero_net_kind;
        (void)hero_net_close(handle);
        hero_net_fail(kind, 0);
        return -1;
    }
    size_t asked = (size_t)8 + (size_t)count;
    size_t room = (size_t)count + HERO_NET_ECHO_ROOM;
    unsigned char *block = hero_layer_alloc(asked + room);

    if (block == NULL) {
        kind = HERO_NET_LIMIT;
    } else {
        unsigned char *request = block;
        unsigned char *heard = block + asked;

        request[0] = (unsigned char)(six ? 128u : 8u);
        request[1] = 0;
        request[2] = 0;
        request[3] = 0;
        request[4] = (unsigned char)(id >> 8);
        request[5] = (unsigned char)(id & 0xffu);
        request[6] = (unsigned char)(sequence >> 8);
        request[7] = (unsigned char)(sequence & 0xffu);
        if (count > 0) {
            memcpy(request + 8, data, (size_t)count);
        }
        if (!six) {
            uint16_t sum = hero_net_echo_sum(request, asked);

            request[2] = (unsigned char)(sum >> 8);
            request[3] = (unsigned char)(sum & 0xffu);
        }
        int64_t began = hero_net_now();

        if (hero_net_datagram_out(slot, fd, request, (uint64_t)asked, where, size, &kind, &code) >= 0) {
            for (;;) {
                struct sockaddr_storage replier;
                hero_net_len replier_size = 0;
                bool cut = false;
                int64_t told = -1;
                int64_t got = hero_net_echo_in(slot, fd, heard, room, &replier, &replier_size, &cut, &told, &kind, &code);

                if (got < 0) {
                    break;
                }
                if (cut) {
                    continue;
                }
                int64_t header_ttl = -1;
                hero_net_echo_is is = hero_net_echo_read(heard, (size_t)got, six, id, sequence, request + 8, count, &header_ttl);

                if (is == HERO_NET_ECHO_OTHER) {
                    continue;
                }
                if (is == HERO_NET_ECHO_ERROR) {
                    kind = HERO_NET_UNREACHABLE;
                    code = 0;
                    break;
                }
                int64_t ended = hero_net_now();
                int64_t port = 0;

                *nanoseconds = began < 0 || ended < began ? 0 : ended - began;
                *ttl = told >= 0 ? told : header_ttl;
                if (replier_size == 0 || hero_net_text_of((const struct sockaddr *)&replier, replier_size, from, &port) < 0) {
                    from[0] = '\0';
                }
                answer = (int64_t)asked;
                break;
            }
        }
        hero_layer_free(block);
    }
    (void)hero_net_done(slot);
    (void)hero_net_close(handle);

    if (answer < 0) {
        from[0] = '\0';
        *ttl = -1;
        *nanoseconds = 0;
        hero_net_fail(kind, code);
        return -1;
    }
    hero_net_clear();
    return answer;
}

#else

/* The room past the reply and its data Microsoft's pages ask the reply
 * buffer to have: 8 bytes for an ICMP error and an `IO_STATUS_BLOCK`, 16
 * bytes on 64-bit Windows, with room beside. */
#define HERO_NET_ECHO_ROOM 64u

/* The kind an echo's status is, `IcmpSendEcho2`'s `GetLastError` or a
 * reply's `Status` (`ipexport.h`'s `IP_*` numbers, and the system's own
 * error numbers): the same kinds the POSIX arm answers. */
static int64_t hero_net_echo_kind(DWORD status) {
    if (status == IP_REQ_TIMED_OUT) {
        return HERO_NET_TIMED_OUT;
    }
    if (status == IP_DEST_NET_UNREACHABLE || status == IP_DEST_HOST_UNREACHABLE || status == IP_DEST_PROT_UNREACHABLE ||
        status == IP_DEST_PORT_UNREACHABLE || status == IP_BAD_ROUTE || status == IP_BAD_DESTINATION || status == IP_TTL_EXPIRED_TRANSIT ||
        status == IP_TTL_EXPIRED_REASSEM || status == ERROR_NETWORK_UNREACHABLE || status == ERROR_HOST_UNREACHABLE ||
        status == ERROR_PROTOCOL_UNREACHABLE || status == ERROR_PORT_UNREACHABLE
#if defined(IP_DEST_UNREACHABLE)
        || status == IP_DEST_UNREACHABLE
#endif
#if defined(IP_TIME_EXCEEDED)
        || status == IP_TIME_EXCEEDED
#endif
    ) {
        return HERO_NET_UNREACHABLE;
    }
    if (status == IP_PACKET_TOO_BIG) {
        return HERO_NET_TOO_LONG;
    }
    if (status == IP_NO_RESOURCES || status == ERROR_NOT_ENOUGH_MEMORY || status == ERROR_OUTOFMEMORY) {
        return HERO_NET_LIMIT;
    }
    if (status == ERROR_ACCESS_DENIED) {
        return HERO_NET_DENIED;
    }
    if (status == ERROR_NOT_SUPPORTED) {
        return HERO_NET_UNSUPPORTED;
    }
    if (status == ERROR_INVALID_PARAMETER) {
        return HERO_NET_INVALID;
    }
    return HERO_NET_OTHER;
}

/* The echo of `hero_net_ping`, its arguments checked, to `where`: one call
 * of the system's, which waits for the reply or `ms` itself. */
static int64_t hero_net_echo(const struct sockaddr_storage *where, hero_net_len size, uint32_t sequence, const void *data, uint64_t count, int64_t ms,
                             char *from, int64_t *ttl, int64_t *nanoseconds) {
    (void)size;
    /* Windows chooses the identifier and the sequence itself. */
    (void)sequence;
    bool six = where->ss_family == AF_INET6;
    HANDLE icmp = six ? Icmp6CreateFile() : IcmpCreateFile();

    if (icmp == INVALID_HANDLE_VALUE) {
        DWORD said = GetLastError();

        hero_net_fail(hero_net_echo_kind(said), (int64_t)said);
        return -1;
    }
    size_t reply_size = (six ? sizeof(ICMPV6_ECHO_REPLY) : sizeof(ICMP_ECHO_REPLY)) + (size_t)count + HERO_NET_ECHO_ROOM;
    unsigned char *reply = hero_layer_alloc(reply_size);

    if (reply == NULL) {
        (void)IcmpCloseHandle(icmp);
        hero_net_fail(HERO_NET_LIMIT, 0);
        return -1;
    }
    memset(reply, 0, reply_size);
    /* The system reads the request and never writes it; a request of no
     * bytes still hands it a place. */
    unsigned char none = 0;
    LPVOID request = count == 0 ? (LPVOID)&none : (LPVOID)(uintptr_t)data;
    int64_t began = hero_net_now();
    DWORD replies = 0;

    if (six) {
        struct sockaddr_in6 source;
        struct sockaddr_in6 target;

        memset(&source, 0, sizeof source);
        source.sin6_family = AF_INET6;
        memcpy(&target, where, sizeof target);
        replies = Icmp6SendEcho2(icmp, NULL, NULL, NULL, &source, &target, request, (WORD)count, NULL, reply, (DWORD)reply_size, (DWORD)ms);
        if (replies != 0) {
            replies = Icmp6ParseReplies(reply, (DWORD)reply_size);
        }
    } else {
        struct sockaddr_in target;

        memcpy(&target, where, sizeof target);
        replies = IcmpSendEcho2(icmp, NULL, NULL, NULL, target.sin_addr.S_un.S_addr, request, (WORD)count, NULL, reply, (DWORD)reply_size, (DWORD)ms);
    }
    DWORD said = replies == 0 ? GetLastError() : 0;
    int64_t ended = hero_net_now();

    (void)IcmpCloseHandle(icmp);

    int64_t kind = HERO_NET_OK;
    int64_t code = 0;
    int64_t answer = -1;
    int64_t port = 0;

    if (replies == 0) {
        kind = hero_net_echo_kind(said);
        code = (int64_t)said;
    } else if (six) {
        ICMPV6_ECHO_REPLY got;

        memcpy(&got, reply, sizeof got);
        if (got.Status != IP_SUCCESS) {
            kind = hero_net_echo_kind(got.Status);
            code = (int64_t)got.Status;
        } else {
            struct sockaddr_in6 replier;

            memset(&replier, 0, sizeof replier);
            replier.sin6_family = AF_INET6;
            memcpy(&replier.sin6_addr, got.Address.sin6_addr, sizeof replier.sin6_addr);
            replier.sin6_scope_id = got.Address.sin6_scope_id;
            if (hero_net_text_of((const struct sockaddr *)&replier, (hero_net_len)sizeof replier, from, &port) < 0) {
                from[0] = '\0';
            }
            *ttl = -1;
            answer = (int64_t)count + 8;
        }
    } else {
        ICMP_ECHO_REPLY got;

        memcpy(&got, reply, sizeof got);
        uintptr_t first = (uintptr_t)reply;
        uintptr_t at = (uintptr_t)got.Data;
        /* The reply's data, where the system put it inside the buffer, is
         * this echo's own: anything else is no answer to it. */
        bool inside = count == 0 || (at >= first && at - first <= reply_size && reply_size - (at - first) >= (size_t)count);

        if (got.Status != IP_SUCCESS) {
            kind = hero_net_echo_kind(got.Status);
            code = (int64_t)got.Status;
        } else if ((uint64_t)got.DataSize != count || !inside || (count > 0 && memcmp(got.Data, data, (size_t)count) != 0)) {
            kind = HERO_NET_OTHER;
        } else {
            struct sockaddr_in replier;

            memset(&replier, 0, sizeof replier);
            replier.sin_family = AF_INET;
            replier.sin_addr.S_un.S_addr = got.Address;
            if (hero_net_text_of((const struct sockaddr *)&replier, (hero_net_len)sizeof replier, from, &port) < 0) {
                from[0] = '\0';
            }
            *ttl = (int64_t)got.Options.Ttl;
            answer = (int64_t)count + 8;
        }
    }
    hero_layer_free(reply);

    if (answer < 0) {
        from[0] = '\0';
        *ttl = -1;
        *nanoseconds = 0;
        hero_net_fail(kind, code);
        return -1;
    }
    *nanoseconds = began < 0 || ended < began ? 0 : ended - began;
    hero_net_clear();
    return answer;
}

#endif

int64_t hero_net_ping(const char *address, int64_t sequence, const void *data, uint64_t count, int64_t ms, void *from, uint64_t from_count, int64_t *ttl,
                      int64_t *nanoseconds) {
    if (from != NULL && from_count > 0) {
        ((char *)from)[0] = '\0';
    }
    if (ttl != NULL) {
        *ttl = -1;
    }
    if (nanoseconds != NULL) {
        *nanoseconds = 0;
    }
    if (address == NULL || sequence < 0 || sequence > 65535 || (data == NULL && count > 0) || count > HERO_NET_PING_MOST || ms < 1 || ms > INT32_MAX ||
        from == NULL || from_count < HERO_NET_ADDRESS_MOST || ttl == NULL || nanoseconds == NULL) {
        hero_net_fail(HERO_NET_INVALID, 0);
        return -1;
    }
    if (hero_net_stopping()) {
        hero_net_fail(HERO_NET_STOPPED, 0);
        return -1;
    }
    if (!hero_net_started()) {
        return -1;
    }
    struct sockaddr_storage where;
    hero_net_len size = 0;
    int64_t code = 0;

    if (!hero_net_literal(address, 0, 0, &where, &size, &code)) {
        hero_net_fail(HERO_NET_ADDRESS, code);
        return -1;
    }
    return hero_net_echo(&where, size, (uint32_t)sequence, data, count, ms, (char *)from, ttl, nanoseconds);
}

/* -- for the other members of the layer (M-rebel-library, 2026-11-16) -----
 *
 * `hero_poll.c` waits on a socket's descriptor in its own sets, and hears the
 * stop through this member. A hold is a call in flight that lasts: the slot
 * stays busy, so a close meanwhile closes nothing under the other member, as
 * it closes nothing under a call. */

/* The functions `hero_net_when_stopped` registered, each place written once,
 * from NULL to a function, under the lock, and read under it by
 * `hero_net_nudge_all`. */
#define HERO_NET_HEARERS 4
static void (*hero_net_hearers[HERO_NET_HEARERS])(void);

static void hero_net_tell_hearers(void) {
    for (int i = 0; i < HERO_NET_HEARERS; i += 1) {
        if (hero_net_hearers[i] != NULL) {
            hero_net_hearers[i]();
        }
    }
}

int64_t hero_net_hold(hero_net_socket *s, int64_t *descriptor) {
    hero_net_fd fd = HERO_NET_NO_FD;
    hero_net_slot *slot = descriptor == NULL ? NULL : hero_net_take(s, &fd);

    if (slot == NULL) {
        hero_net_fail(HERO_NET_NOT_OPEN, 0);
        return -1;
    }
    *descriptor = (int64_t)fd;
    hero_net_clear();
    return (int64_t)(slot - hero_net_slots);
}

void hero_net_unhold(int64_t hold) {
    if (hold < 0 || hold >= (int64_t)HERO_NET_SLOTS) {
        return;
    }
    hero_net_slot *slot = &hero_net_slots[hold];

    hero_net_enter();
    bool held = (uint64_t)hold < hero_net_fresh && slot->busy > 0;
    hero_net_leave();

    if (held) {
        (void)hero_net_done(slot);
    }
}

int64_t hero_net_when_stopped(void (*wake)(void)) {
    if (wake == NULL) {
        hero_net_fail(HERO_NET_INVALID, 0);
        return -1;
    }
#if !defined(_WIN32)
    /* A signal is heard by the readiness thread alone, which watches the
     * stop's pipe from its making or from the stop's arming, whichever comes
     * last; on Windows the console's own thread hears it. */
    if (!hero_net_poller_up()) {
        hero_net_fail(HERO_NET_LIMIT, 0);
        return -1;
    }
#endif
    bool kept = false;

    hero_net_enter();
    for (int i = 0; i < HERO_NET_HEARERS && !kept; i += 1) {
        kept = hero_net_hearers[i] == wake;
    }
    for (int i = 0; i < HERO_NET_HEARERS && !kept; i += 1) {
        if (hero_net_hearers[i] == NULL) {
            hero_net_hearers[i] = wake;
            kept = true;
        }
    }
    hero_net_leave();

    if (!kept) {
        hero_net_fail(HERO_NET_LIMIT, 0);
        return -1;
    }
    hero_net_clear();
    return 0;
}
