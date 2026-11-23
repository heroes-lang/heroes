/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_poll.c — the platform layer's readiness sets, one member of
 * the archive the compiler puts on every program's link line
 * (`selfhost/cli/layer.hero` builds, keys and links it). Its header,
 * `hero_poll.h`, says what each function answers; this file says how each
 * platform gets there. Written from the systems' own pages (`kqueue(2)`,
 * `epoll(7)`, `poll(2)`, Microsoft's `WSAPoll`), never from another
 * library's source.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so an arm that
 * defines another signature than the header's is `conflicting types`.
 *
 * TWO TABLES, BOTH STATIC, NEVER AN ADDRESS READ THROUGH. A set is a slot of
 * `hero_poll_sets` and an entry a slot of `hero_poll_entries`; a handle and a
 * key are each a slot's index and the life it held when issued, as
 * `hero_net.c`'s sockets are, so a closed set and a removed key name a life
 * that ended. No table grows: the runtime allocates in `parts/alloc.c` alone
 * (the net's `runtime` suite, rule 1), so a set is made with the most entries
 * it will hold and RESERVES THAT MANY CONTIGUOUS ENTRIES of the 65,536 the
 * process has, its own range for its life. The range is what lets Windows'
 * `WSAPoll`, which reads one array, wait on a set with no allocation: the
 * array is a static one beside the entries, the set's part of it filled at
 * each wait by the one call that may wait on the set.
 *
 * NO LOCK OF THIS FILE IS HELD WHILE ANOTHER MEMBER IS CALLED, BUT ONE. A
 * stream or a socket is held (`hero_net_hold`, `hero_stream_hold`) and let go
 * outside `hero_poll_lock`; the one call made under it is
 * `hero_net_when_stopped`, once per process, so the order between the two
 * locks is this file's then `hero_net.c`'s, and the function `hero_net.c`
 * calls back at the stop, under its own lock, takes none of this file's.
 *
 * THE STOP: `hero_poll_hear_stop`, called by `hero_net.c` at each stop it
 * hears (its readiness thread for a signal on POSIX, the console's thread on
 * Windows, or the thread that asked), writes one byte down a pipe on POSIX
 * and sends one datagram to a loopback socket of its own on Windows, bound
 * with its address held exclusively and connected to itself, so it hears no
 * other socket (defect 2696). Nobody reads either, so it stays ready from
 * then on, and every set watches it beside its entries: every wait of the
 * process wakes at once, and every later one finds it ready, as the stop is
 * never taken back. A wait told the bell asks whether the stop was asked and
 * never takes the bell's word alone (`hero_poll_bell_heard`).
 *
 * THE DOORBELL, AND A MAILBOX IN A SET (panel 256's R7 and R8, provisional).
 * A mailbox entry stands on no descriptor: it is the number `m.number()`
 * gave, and the runtime answers whether a receive on it would wait
 * (`hero_layer_mail.h`). Every wait asks each mailbox of its set before it
 * asks the system, so the entry is level as every other is. To end a wait
 * already in the system, the set registers a watch on each mailbox, which the
 * runtime rings, under the queue's lock, when the queue goes from empty to one
 * value, at its close, at the stop and at its end; the ring rings the set's
 * doorbell and takes no lock: an `EVFILT_USER` event of the set's own `kqueue`
 * on Darwin and the BSDs, an `eventfd` in its `epoll` on Linux, a pipe on the
 * array's POSIX systems, and on Windows a loopback datagram socket that hears
 * itself alone. A wait told the doorbell empties it and asks the mailboxes
 * again before it blocks, so a ring meanwhile is never lost. The doorbell is
 * made at the set's first mailbox (Darwin's at its making, as it costs no
 * descriptor) and closed at its release, after every watch is taken away.
 * A WAIT THAT MUST NOT BLOCK RINGS ITS OWN DOORBELL on `kqueue`, in the one
 * call: Darwin's `kevent` with a zero timeout that finds nothing ready costs
 * tens of microseconds, one that finds an event a fraction of one.
 * A WINDOWS SET'S EVENT (panel 256's R9, provisional). `WSAPoll` waits on
 * sockets and takes no handle, so a loop that waits in another call, a user
 * interface's `MsgWaitForMultipleObjectsEx`, is given an event the set owns
 * (`hero_poll_descriptor`, made at that call, a manual-reset one). It is a
 * hint and never the answer: it is set by `WSAEventSelect` on the set's
 * sockets (`hero_poll_select`), by a mailbox's ring and by the stop, and by a
 * wait that told an entry, so what stays ready is told again, and it is
 * reset at the start of every wait, which asks `WSAPoll` and the mailboxes
 * itself; a notice that comes after the reset sets it again, so none is lost.
 * A socket holds ONE registration, the last `WSAEventSelect` winning, so a
 * set registers each socket once, for every interest its entries on it have,
 * and a socket another set that was handed out holds is refused, `UNSUPPORTED`
 * with `WSAEALREADY`, where the system would let the earlier loop go deaf in
 * silence. An event is kept for the life of the process once made, one for
 * each set slot at most: the stop's hook and the runtime's ring then never
 * reach a handle the system gave to another object.
 * HANGUP, BY SYSTEM: Darwin tells it as `EV_EOF` on the read or the write
 * filter, once the peer has closed its side; Linux as `EPOLLRDHUP` (the peer
 * closed its side) or `EPOLLHUP` (both are closed); Windows as `POLLHUP`. A
 * wait answers READ beside it wherever READ was asked, so a loop that reads on
 * READ meets the end of stream on every system.
 */

#include "hero_poll.h"

/* The layer's one list of handle tags, after the member's own header. */
#include "hero_handle_tag.h"
#include "hero_layer_mail.h"
#include "hero_layer_signal.h"
#include "hero_stream_net.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
/* The library the member needs, named by the member, as `hero_net.c` names
 * it: the linker reads it only where it takes this member. */
#if defined(_MSC_VER)
#pragma comment(lib, "ws2_32.lib")
#endif
#define HERO_POLL_ARRAY 1
typedef WSAPOLLFD hero_poll_fd;
#define HERO_POLL_IN POLLRDNORM
#define HERO_POLL_OUT POLLWRNORM
#else
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
/* The system's wait: Linux's `epoll`, the BSDs' and Darwin's `kqueue`; on any
 * other POSIX system `poll` over the set's array, as Windows waits. */
#if defined(__linux__)
#include <sys/epoll.h>
#include <sys/eventfd.h>
#define HERO_POLL_EPOLL 1
#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#include <sys/event.h>
#define HERO_POLL_KQUEUE 1
#else
#define HERO_POLL_ARRAY 1
typedef struct pollfd hero_poll_fd;
#define HERO_POLL_IN POLLIN
#define HERO_POLL_OUT POLLOUT
#endif
#endif

/* A handle and a key each carry a slot and a life in 64 bits, as
 * `hero_net.c`'s do: `(life << 17) | (index + 1)`, never 0.
 *
 * AND A SET'S HANDLE CARRIES THE TABLE'S TAG, `HERO_HANDLE_TAG_POLL`
 * (`hero_handle_tag.h`): the program's handle set knows a handle by its value
 * alone, so a set and the socket, stream or child of the same slot and life
 * were one handle to it, and closing the set was told as closing the socket
 * (measured 2026-11-16 by this member's first tests, a set and a listener
 * both `0x20001`). A key is no handle, the runtime never holds one, and it
 * carries no tag. */
_Static_assert(sizeof(void *) == 8 && sizeof(uintptr_t) == 8, "a set's handle carries its slot and its life in 64 bits");
#define HERO_POLL_INDEX_BITS 17
#define HERO_POLL_INDEX_MASK ((UINT64_C(1) << HERO_POLL_INDEX_BITS) - 1)
/* A slot whose life reached it is never issued again: below the tag. */
#define HERO_POLL_LIFE_MOST HERO_HANDLE_LIFE_MOST(HERO_POLL_INDEX_BITS)
#define HERO_POLL_ENTRIES 65536u
#define HERO_POLL_SETS 1024u
/* A wait where the stop cannot be heard asks for it this often. */
#define HERO_POLL_SLICE_MS 50
/* One wait in the system lasts at most an hour, then the limit is read
 * again: no system's timeout is asked past what its type holds. */
#define HERO_POLL_CHUNK_MS (60 * 60 * 1000)

/* What an entry stands on. */
#define HERO_POLL_ON_NOTHING 0
/* A socket held by `hero_net_hold`, its descriptor waited on. */
#define HERO_POLL_ON_SOCKET 1
/* A stream's descriptor held by `hero_stream_hold`. */
#define HERO_POLL_ON_STREAM 2
/* A regular file, ready at every wait. */
#define HERO_POLL_ON_FILE 3
#define HERO_POLL_ON_TIMER 4
/* The signals the program counts: a copy of the signal door's descriptor,
 * the entry's own, closed when it leaves the set (POSIX). */
#define HERO_POLL_ON_SIGNALS 5
/* A mailbox the runtime knows by number, watched (`hero_layer_mail.h`). */
#define HERO_POLL_ON_MAILBOX 6

/* What the system tells of the set's doorbell, which no key is: a key is
 * `(life << 17) | (index + 1)` with a life below the tag, never all ones. */
#define HERO_POLL_DOOR_KEY UINT64_MAX
/* The doorbell's place in an array's answer, which no entry's index plus one
 * reaches (65,536 entries). */
#define HERO_POLL_DOOR_PLACE UINT32_MAX
#if defined(HERO_POLL_KQUEUE)
/* The doorbell's identifier among the set's `EVFILT_USER` events. */
#define HERO_POLL_DOOR_IDENT 1
#endif

typedef struct {
    /* The life the entry holds or last held, which its key names. */
    uint64_t life;
    /* One of `HERO_POLL_ON_*`, `HERO_POLL_ON_NOTHING` while free. */
    uint8_t on;
    /* The `HERO_POLL_READ` and `HERO_POLL_WRITE` bits it waits for. */
    uint8_t interest;
    /* The descriptor waited on, the bits of a `SOCKET` on Windows. */
    int64_t fd;
    /* What `hero_net_hold` answered, -1 for none. */
    int64_t net_hold;
    /* The stream whose descriptor `hero_stream_hold` holds, NULL for none. */
    hero_stream *stream;
    /* A timer's next due time on `hero_poll_now`'s clock, -1 for none; and
     * its period in nanoseconds, 0 for once. */
    int64_t due;
    int64_t every;
    /* A mailbox's number, -1 for none or once its watch is taken away. */
    int64_t number;
    /* The index plus one of the next entry given back in the set, 0 none. */
    uint32_t next;
    /* The index plus one of the next and the previous entry of the set's list
     * of timers or of files, which a wait walks, 0 none. */
    uint32_t after;
    uint32_t before;
    /* The wait round that last told it, and where in its answer: two events
     * of one descriptor (`kqueue` tells reading and writing apart) are one. */
    uint64_t round;
    uint64_t told_at;
} hero_poll_entry;

typedef struct {
    uint64_t life;
    /* It holds a set the program has not closed. */
    bool held;
    /* The program closed it while a call was in it: the last one releases it. */
    bool closing;
    /* A wait is in it. */
    bool waiting;
    /* Calls in it. */
    uint32_t busy;
    /* Its entries are `base` to `base + span - 1`, and the array index
     * `base + span - 1` is the stop's on Windows; `fresh` of them have ever
     * held an entry; `given_back` is the index plus one of the last one given
     * back, 0 for none. */
    uint32_t base;
    uint32_t span;
    uint32_t fresh;
    uint32_t given_back;
    uint32_t count;
    /* The index plus one of the first timer, file and mailbox, 0 none. */
    uint32_t timers;
    uint32_t files;
    uint32_t mailboxes;
    /* The doorbell a mailbox's watch rings (THE DOORBELL, above), made at the
     * set's first mailbox and kept to its end: an `eventfd` on Linux (both
     * ends the one descriptor), a pipe on the array's POSIX systems, a
     * loopback socket connected to itself on Windows; -1 for none, and always
     * on Darwin and the BSDs, whose doorbell is an `EVFILT_USER` event of the
     * set's own queue. */
    int64_t door_in;
    int64_t door_out;
    /* Its descriptor was handed out (`hero_poll_descriptor`): a wait that
     * leaves an entry ready rings the doorbell again, so the descriptor stays
     * readable while one is. */
    bool shown;
    uint64_t rounds;
    /* The `kqueue` or the `epoll` of the set, -1 on the array's systems. */
    int queue;
    /* The index plus one of the next set slot given back, 0 for none. */
    uint32_t next;
} hero_poll_slot;

static hero_poll_slot hero_poll_sets[HERO_POLL_SETS];
static hero_poll_entry hero_poll_entries[HERO_POLL_ENTRIES];
/* Every set slot at or above it has never held a set. */
static uint32_t hero_poll_fresh;
/* The index plus one of the last set slot given back, 0 for none. */
static uint32_t hero_poll_given_back;
/* The stop was heard by `hero_net.c`'s hook, or will be: false where a wait
 * must ask in slices. Written once, under the lock. */
static _Atomic int hero_poll_heard;
/* The member's once: the stop's bell made and the hook asked. Under the lock. */
static bool hero_poll_up_done;

#if defined(HERO_POLL_ARRAY)
/* What a wait hands the system: each set's part is its range, filled by the
 * one wait the set allows, its last place the stop's bell. */
static hero_poll_fd hero_poll_array[HERO_POLL_ENTRIES];
/* The entry each place of the array stands for, index plus one, 0 for the
 * stop's bell and `HERO_POLL_DOOR_PLACE` for the set's doorbell. */
static uint32_t hero_poll_array_entry[HERO_POLL_ENTRIES];
#endif

static _Thread_local int64_t hero_poll_kind;
static _Thread_local int64_t hero_poll_os;

#if defined(_WIN32)
static SRWLOCK hero_poll_lock = SRWLOCK_INIT;
/* The stop's bell: a datagram socket on the loopback that hears itself alone
 * (`hero_poll_loopback`), which the hook sends one datagram to and nobody
 * reads (defect 2696: bound and never connected, it took any process's
 * datagram, and every later wait failed `stopped`). */
static _Atomic SOCKET hero_poll_bell = INVALID_SOCKET;
/* The event of each set slot (`hero_poll_descriptor`), 0 until the slot's
 * first set is handed its descriptor and the same handle for every set the
 * slot holds after: never closed, so the stop's hook and a ring, which take no
 * lock, never signal a handle the system has given to another object. */
static _Atomic uintptr_t hero_poll_events[HERO_POLL_SETS];
#else
static pthread_mutex_t hero_poll_lock = PTHREAD_MUTEX_INITIALIZER;
/* The stop's pipe: the hook writes `hero_poll_bell_out`, every set watches
 * `hero_poll_bell_in`, and nobody reads it. -1 until made. */
static _Atomic int hero_poll_bell_in = -1;
static _Atomic int hero_poll_bell_out = -1;
#endif

static void hero_poll_enter(void) {
#if defined(_WIN32)
    AcquireSRWLockExclusive(&hero_poll_lock);
#else
    (void)pthread_mutex_lock(&hero_poll_lock);
#endif
}

static void hero_poll_leave(void) {
#if defined(_WIN32)
    ReleaseSRWLockExclusive(&hero_poll_lock);
#else
    (void)pthread_mutex_unlock(&hero_poll_lock);
#endif
}

static void hero_poll_fail(int64_t kind, int64_t os) {
    hero_poll_kind = kind;
    hero_poll_os = os;
}

static void hero_poll_clear(void) {
    hero_poll_fail(HERO_POLL_OK, 0);
}

/* The platform's number for the failure the last system call reported. */
static int64_t hero_poll_os_code(void) {
#if defined(_WIN32)
    return (int64_t)WSAGetLastError();
#else
    return (int64_t)errno;
#endif
}

/* The kind a refusal to watch a descriptor is: a descriptor the system will
 * not wait on (`epoll`'s `EPERM` for a regular file, `kqueue`'s `EINVAL` for
 * `/dev/null`), a system with no room, or another. */
static int64_t hero_poll_kind_of(int64_t code) {
    switch (code) {
#if defined(_WIN32)
    case WSAENOTSOCK:
    case WSAEALREADY:
        return HERO_POLL_UNSUPPORTED;
    case WSAENOBUFS:
    case WSAEMFILE:
        return HERO_POLL_LIMIT;
#else
    case EPERM:
    case EINVAL:
    case ENODEV:
    case ENOTSUP:
        return HERO_POLL_UNSUPPORTED;
    case ENOMEM:
    case ENOSPC:
    case EMFILE:
    case ENFILE:
        return HERO_POLL_LIMIT;
#endif
    default:
        return HERO_POLL_OTHER;
    }
}

/* Nanoseconds on a clock that never goes back, the one `hero_net.c` reads:
 * Darwin's `CLOCK_UPTIME_RAW`, Linux's `CLOCK_MONOTONIC`, Windows'
 * performance counter. -1 where it cannot be read or does not fit. */
static int64_t hero_poll_now(void) {
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

/* `at` plus `ns`, or the largest time where the sum does not fit: a time so
 * far is never reached. */
static int64_t hero_poll_later(int64_t at, int64_t ns) {
    int64_t sum = 0;

    return __builtin_add_overflow(at, ns, &sum) ? INT64_MAX : sum;
}

/* -- the stop's bell ------------------------------------------------------- */

/* THE HOOK `hero_net.c` CALLS AT A STOP, under its own lock: it takes none of
 * this file's, allocates nothing, and calls one system function that does not
 * wait (the bell is non-blocking). */
static void hero_poll_hear_stop(void) {
#if defined(_WIN32)
    SOCKET bell = atomic_load(&hero_poll_bell);

    if (bell != INVALID_SOCKET) {
        /* `send`, the socket being connected to itself. */
        (void)send(bell, "\1", 1, 0);
    }
    /* Every event a set owns, as the pipe is in every `kqueue` and `epoll`: a
     * loop that waits on a set's descriptor hears the stop, and goes on hearing
     * it, since no wait resets an event once the stop is asked. */
    for (uint32_t at = 0; at < HERO_POLL_SETS; at += 1) {
        uintptr_t event = atomic_load(&hero_poll_events[at]);

        if (event != 0) {
            (void)SetEvent((HANDLE)event);
        }
    }
#else
    int out = atomic_load(&hero_poll_bell_out);

    if (out >= 0) {
        char one = 1;
        ssize_t wrote = write(out, &one, 1);

        (void)wrote;
    }
#endif
}

#if defined(_WIN32)
static SOCKET hero_poll_loopback(void);

/* The bell made: a datagram socket on the loopback that hears itself alone,
 * non-blocking and never inherited. False where any of it is refused, the
 * loopback down among the causes, nothing left made. */
static bool hero_poll_bell_made(void) {
    WSADATA data;

    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        return false;
    }
    SOCKET bell = hero_poll_loopback();

    if (bell == INVALID_SOCKET) {
        return false;
    }
    atomic_store(&hero_poll_bell, bell);
    return true;
}
#else
/* The bell made: a pipe, both ends close-on-exec and non-blocking. False
 * where any of it is refused, nothing left made. */
static bool hero_poll_bell_made(void) {
    int ends[2];

    if (pipe(ends) != 0) {
        return false;
    }
    for (int i = 0; i < 2; i += 1) {
        int flags = fcntl(ends[i], F_GETFL);

        if (fcntl(ends[i], F_SETFD, FD_CLOEXEC) != 0 || flags < 0 || fcntl(ends[i], F_SETFL, flags | O_NONBLOCK) != 0) {
            (void)close(ends[0]);
            (void)close(ends[1]);
            return false;
        }
    }
    atomic_store(&hero_poll_bell_in, ends[0]);
    atomic_store(&hero_poll_bell_out, ends[1]);
    return true;
}
#endif

/* The bell told ready by the system: the stop where the program's stop was
 * asked, and nothing else (defect 2696). The flag is set before any bell is
 * rung (`hero_net.c`), so a bell ready with no stop asked holds bytes no stop
 * sent: they are drained, and the flag asked again, the bell rung again where
 * a stop came meanwhile, so no wait sharing it misses that stop. */
static bool hero_poll_bell_heard(void) {
    if (hero_net_stop_asked() != 0) {
        return true;
    }
#if defined(_WIN32)
    SOCKET bell = atomic_load(&hero_poll_bell);
    char got[16];

    while (bell != INVALID_SOCKET && recv(bell, got, (int)sizeof got, 0) > 0) {
    }
#else
    int in = atomic_load(&hero_poll_bell_in);
    char got[64];

    while (in >= 0 && read(in, got, sizeof got) > 0) {
    }
#endif
    if (hero_net_stop_asked() != 0) {
        hero_poll_hear_stop();
        return true;
    }
    return false;
}

/* The member's once, under the lock: the bell made and `hero_net.c` asked to
 * call the hook at every stop. Where either is refused the waits ask in
 * slices; a set is made all the same. */
static void hero_poll_up(void) {
    if (hero_poll_up_done) {
        return;
    }
    hero_poll_up_done = true;
    if (hero_poll_bell_made() && hero_net_when_stopped(hero_poll_hear_stop) == 0) {
        atomic_store(&hero_poll_heard, 1);
    }
}

/* -- the doorbell a mailbox rings (panel 256's R7 and R8, provisional) ----- */

/* THE RING, called by the runtime under one queue's lock or the queues' list
 * lock (`hero_layer_mail.h`): it takes no lock, does not wait, allocates
 * nothing, and reads only what the set held before its first watch was
 * registered. What it writes to stays open until every watch of the set is
 * taken away (`hero_poll_release`), so a ring never reaches a descriptor the
 * system has given to another file. A ring that finds the doorbell full has
 * nothing to add: it is ready already. */
static void hero_poll_ring(void *ctx) {
    const hero_poll_slot *slot = (const hero_poll_slot *)ctx;
#if defined(HERO_POLL_KQUEUE)
    struct kevent trigger;

    EV_SET(&trigger, HERO_POLL_DOOR_IDENT, EVFILT_USER, 0, NOTE_TRIGGER, 0, (void *)(uintptr_t)HERO_POLL_DOOR_KEY);
    (void)kevent(slot->queue, &trigger, 1, NULL, 0, NULL);
#elif defined(_WIN32)
    uintptr_t event = atomic_load(&hero_poll_events[slot - hero_poll_sets]);

    if (slot->door_out >= 0) {
        (void)send((SOCKET)(uint64_t)slot->door_out, "\1", 1, 0);
    }
    /* The event of a loop that waits in the system's own call, set here and
     * not by a datagram that reaches it later, so a value sent is told at
     * once to a program that asks the handle in the next call. */
    if (event != 0) {
        (void)SetEvent((HANDLE)event);
    }
#elif defined(HERO_POLL_EPOLL)
    uint64_t one = 1;
    ssize_t wrote = write((int)slot->door_out, &one, sizeof one);

    (void)wrote;
#else
    char one = 1;
    ssize_t wrote = write((int)slot->door_out, &one, 1);

    (void)wrote;
#endif
}

#if defined(_WIN32)
/* A datagram socket on the loopback that hears itself alone: bound at a port
 * the system chooses, the address held exclusively so no other socket binds
 * it again, and CONNECTED TO ITSELF, so Winsock drops a datagram from any
 * other address, another process's among them; non-blocking and never
 * inherited. `INVALID_SOCKET` where any of it is refused, nothing left made. */
static SOCKET hero_poll_loopback(void) {
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (s == INVALID_SOCKET) {
        return INVALID_SOCKET;
    }
    struct sockaddr_in at;
    int size = (int)sizeof at;
    u_long on = 1;
    BOOL alone = TRUE;

    memset(&at, 0, sizeof at);
    at.sin_family = AF_INET;
    at.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    at.sin_port = 0;
    /* `FIONBIO` is an unsigned bit pattern the call takes as its `long`. */
    if (setsockopt(s, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (const char *)&alone, (int)sizeof alone) != 0 ||
        bind(s, (const struct sockaddr *)&at, (int)sizeof at) != 0 || getsockname(s, (struct sockaddr *)&at, &size) != 0 ||
        connect(s, (const struct sockaddr *)&at, (int)sizeof at) != 0 || ioctlsocket(s, (long)FIONBIO, &on) != 0 ||
        !SetHandleInformation((HANDLE)s, HANDLE_FLAG_INHERIT, 0)) {
        (void)closesocket(s);
        return INVALID_SOCKET;
    }
    return s;
}
#endif

#if !defined(HERO_POLL_KQUEUE)
/* A doorbell made for a set, outside the lock: its reading and its writing
 * end. False with the system's number where it is refused. */
static bool hero_poll_door_made(int64_t *in, int64_t *out, int64_t *code) {
#if defined(_WIN32)
    SOCKET s = hero_poll_loopback();

    if (s == INVALID_SOCKET) {
        *code = hero_poll_os_code();
        return false;
    }
    *in = (int64_t)(uint64_t)s;
    *out = *in;
    return true;
#elif defined(HERO_POLL_EPOLL)
    int fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);

    if (fd < 0) {
        *code = hero_poll_os_code();
        return false;
    }
    *in = fd;
    *out = fd;
    return true;
#else
    int ends[2];

    if (pipe(ends) != 0) {
        *code = hero_poll_os_code();
        return false;
    }
    for (int i = 0; i < 2; i += 1) {
        int flags = fcntl(ends[i], F_GETFL);

        if (fcntl(ends[i], F_SETFD, FD_CLOEXEC) != 0 || flags < 0 || fcntl(ends[i], F_SETFL, flags | O_NONBLOCK) != 0) {
            *code = hero_poll_os_code();
            (void)close(ends[0]);
            (void)close(ends[1]);
            return false;
        }
    }
    *in = ends[0];
    *out = ends[1];
    return true;
#endif
}

/* The doorbell's ends closed, once no watch can ring it. */
static void hero_poll_door_close(int64_t in, int64_t out) {
#if defined(_WIN32)
    (void)out;
    if (in >= 0) {
        (void)closesocket((SOCKET)(uint64_t)in);
    }
#else
    if (in >= 0) {
        (void)close((int)in);
    }
    if (out >= 0 && out != in) {
        (void)close((int)out);
    }
#endif
}

/* The doorbell emptied once the system told it, so it is told again for a
 * later ring alone. A ring meanwhile is never lost: the wait asks every
 * mailbox after this, before it blocks again. */
static void hero_poll_door_drain(int64_t in) {
#if defined(_WIN32)
    char got[16];

    while (recv((SOCKET)(uint64_t)in, got, (int)sizeof got, 0) > 0) {
    }
#elif defined(HERO_POLL_EPOLL)
    uint64_t rings = 0;
    ssize_t got = read((int)in, &rings, sizeof rings);

    (void)got;
#else
    char got[64];

    while (read((int)in, got, sizeof got) > 0) {
    }
#endif
}
#endif

/* -- the tables ------------------------------------------------------------ */

static hero_poll_set *hero_poll_handle(uint32_t index, uint64_t life) {
    return (hero_poll_set *)(uintptr_t)(HERO_HANDLE_TAG(HERO_HANDLE_TAG_POLL) | (life << HERO_POLL_INDEX_BITS) | ((uint64_t)index + 1));
}

/* The set a handle names while it holds the life the handle was issued for
 * and is not closed; NULL for any other value: the handle must be bit for bit
 * the one that slot and life issue, so another table's handle, whose tag is
 * not this one, and a value with any other bit set, name nothing here. Under
 * the lock. */
static hero_poll_slot *hero_poll_find(hero_poll_set *set) {
    uint64_t value = (uint64_t)(uintptr_t)set;
    uint64_t low = value & HERO_POLL_INDEX_MASK;

    if (low == 0 || low > hero_poll_fresh) {
        return NULL;
    }
    hero_poll_slot *slot = &hero_poll_sets[low - 1];

    if (!slot->held || slot->closing || value != (uint64_t)(uintptr_t)hero_poll_handle((uint32_t)(low - 1), slot->life)) {
        return NULL;
    }
    return slot;
}

/* The entry `key` names in the set, NULL for none. Under the lock. */
static hero_poll_entry *hero_poll_entry_of(const hero_poll_slot *slot, int64_t key) {
    if (key <= 0) {
        return NULL;
    }
    uint64_t value = (uint64_t)key;
    uint64_t low = value & HERO_POLL_INDEX_MASK;

    if (low <= slot->base || low > (uint64_t)slot->base + slot->fresh) {
        return NULL;
    }
    hero_poll_entry *e = &hero_poll_entries[low - 1];

    if (e->on == HERO_POLL_ON_NOTHING || e->life != value >> HERO_POLL_INDEX_BITS) {
        return NULL;
    }
    return e;
}

static int64_t hero_poll_key(const hero_poll_entry *e) {
    return (int64_t)((e->life << HERO_POLL_INDEX_BITS) | ((uint64_t)(e - hero_poll_entries) + 1));
}

/* A set slot given back. Under the lock. */
static void hero_poll_slot_free(hero_poll_slot *slot) {
    slot->held = false;
    slot->closing = false;
    slot->waiting = false;
    slot->span = 0;
    slot->queue = -1;
    slot->door_in = -1;
    slot->door_out = -1;
    if (slot->life < HERO_POLL_LIFE_MOST) {
        slot->next = hero_poll_given_back;
        hero_poll_given_back = (uint32_t)(slot - hero_poll_sets) + 1;
    }
}

/* Where `need` contiguous entries are free, the lowest place first; false
 * where no place holds so many. A set holds its range from its making to its
 * release, closing or not. Under the lock. */
static bool hero_poll_range(uint32_t need, uint32_t *base) {
    uint32_t at = 0;
    bool moved = true;

    while (moved) {
        moved = false;
        if ((uint64_t)at + need > HERO_POLL_ENTRIES) {
            return false;
        }
        for (uint32_t i = 0; i < hero_poll_fresh; i += 1) {
            const hero_poll_slot *other = &hero_poll_sets[i];

            if (other->span != 0 && at < other->base + other->span && other->base < at + need) {
                at = other->base + other->span;
                moved = true;
            }
        }
    }
    *base = at;
    return true;
}

/* A list of timers or files, as the set heads it: `e` put at its front, or
 * taken out of it. Under the lock. */
static void hero_poll_link(uint32_t *head, hero_poll_entry *e) {
    uint32_t me = (uint32_t)(e - hero_poll_entries) + 1;

    e->before = 0;
    e->after = *head;
    if (*head != 0) {
        hero_poll_entries[*head - 1].before = me;
    }
    *head = me;
}

static void hero_poll_unlink(uint32_t *head, hero_poll_entry *e) {
    if (e->before != 0) {
        hero_poll_entries[e->before - 1].after = e->after;
    } else {
        *head = e->after;
    }
    if (e->after != 0) {
        hero_poll_entries[e->after - 1].before = e->before;
    }
    e->before = 0;
    e->after = 0;
}

/* -- the system's wait, per arm --------------------------------------------- */

#if defined(HERO_POLL_KQUEUE)
/* The descriptor's filters moved from `was` to `now`: each added or deleted
 * alone, and a failure of the second undoing the first. 0, or the system's
 * number. */
static int64_t hero_poll_watch(int q, int64_t fd, unsigned now, unsigned was, uint64_t key) {
    static const unsigned ways[2] = { HERO_POLL_READ, HERO_POLL_WRITE };
    static const short filters[2] = { EVFILT_READ, EVFILT_WRITE };
    int done = -1;

    for (int i = 0; i < 2; i += 1) {
        bool want = (now & ways[i]) != 0;
        bool had = (was & ways[i]) != 0;

        if (want == had) {
            continue;
        }
        struct kevent change;

        EV_SET(&change, (uintptr_t)fd, filters[i], want ? EV_ADD : EV_DELETE, 0, 0, (void *)(uintptr_t)key);
        if (kevent(q, &change, 1, NULL, 0, NULL) != 0) {
            int64_t code = hero_poll_os_code();

            if (done >= 0) {
                bool undo = (now & ways[done]) != 0;

                EV_SET(&change, (uintptr_t)fd, filters[done], undo ? EV_DELETE : EV_ADD, 0, 0, (void *)(uintptr_t)key);
                (void)kevent(q, &change, 1, NULL, 0, NULL);
            }
            return code;
        }
        done = i;
    }
    return 0;
}

static int hero_poll_queue_made(void) {
    return kqueue();
}
#elif defined(HERO_POLL_EPOLL)
static int64_t hero_poll_watch(int q, int64_t fd, unsigned now, unsigned was, uint64_t key) {
    struct epoll_event ev;

    memset(&ev, 0, sizeof ev);
    ev.events = ((now & HERO_POLL_READ) != 0 ? (uint32_t)(EPOLLIN | EPOLLRDHUP) : 0u) | ((now & HERO_POLL_WRITE) != 0 ? (uint32_t)EPOLLOUT : 0u);
    ev.data.u64 = key;
    int op = was == 0 ? EPOLL_CTL_ADD : (now == 0 ? EPOLL_CTL_DEL : EPOLL_CTL_MOD);

    return epoll_ctl(q, op, (int)fd, &ev) != 0 ? hero_poll_os_code() : 0;
}

static int hero_poll_queue_made(void) {
    return epoll_create1(EPOLL_CLOEXEC);
}
#else
/* The array's systems keep nothing in the system between waits. */
static int64_t hero_poll_watch(int q, int64_t fd, unsigned now, unsigned was, uint64_t key) {
    (void)q;
    (void)fd;
    (void)now;
    (void)was;
    (void)key;
    return 0;
}
#endif

#if defined(_WIN32)
/* The notices `WSAEventSelect` is asked for a socket that an entry waits on
 * for `interest`: what it reads, what it writes, and always the connection
 * made, refused or closed, which `WSAPoll` tells as writable, hangup or error. */
static long hero_poll_notices(unsigned interest) {
    return (long)(FD_CLOSE | FD_CONNECT | ((interest & HERO_POLL_READ) != 0 ? FD_READ | FD_ACCEPT : 0) | ((interest & HERO_POLL_WRITE) != 0 ? FD_WRITE : 0));
}

/* Socket `fd` registered with `event` for what every entry of `slot` on it
 * waits for, or its registration cancelled where none does (`except`, an entry
 * on its way out, is not counted). A socket holds one registration, so one
 * that an entry of another set handed out also holds is refused: 0, or the
 * system's number, `WSAEALREADY` for that refusal. Under the lock; the cost is
 * the entries of this set and of the sets handed out, and only a set handed
 * out pays it. */
static int64_t hero_poll_select(const hero_poll_slot *slot, uintptr_t event, int64_t fd, const hero_poll_entry *except) {
    long notices = 0;

    for (uint32_t i = 0; i < slot->fresh; i += 1) {
        const hero_poll_entry *e = &hero_poll_entries[slot->base + i];

        if (e != except && e->on == HERO_POLL_ON_SOCKET && e->fd == fd) {
            notices |= hero_poll_notices(e->interest);
        }
    }
    if (notices == 0) {
        return WSAEventSelect((SOCKET)(uint64_t)fd, NULL, 0) != 0 ? hero_poll_os_code() : 0;
    }
    for (uint32_t s = 0; s < hero_poll_fresh; s += 1) {
        const hero_poll_slot *other = &hero_poll_sets[s];

        if (other == slot || other->span == 0 || !other->shown) {
            continue;
        }
        for (uint32_t i = 0; i < other->fresh; i += 1) {
            const hero_poll_entry *e = &hero_poll_entries[other->base + i];

            if (e->on == HERO_POLL_ON_SOCKET && e->fd == fd) {
                return WSAEALREADY;
            }
        }
    }
    return WSAEventSelect((SOCKET)(uint64_t)fd, (WSAEVENT)event, notices) != 0 ? hero_poll_os_code() : 0;
}

/* Every socket of `slot` registered with `event`, as the set is handed out:
 * 0, or the number of the first refusal with every registration made before it
 * cancelled: the refused socket's own is another set's, left as it is. Under
 * the lock. */
static int64_t hero_poll_select_all(const hero_poll_slot *slot, uintptr_t event) {
    for (uint32_t i = 0; i < slot->fresh; i += 1) {
        const hero_poll_entry *e = &hero_poll_entries[slot->base + i];
        int64_t os = 0;

        if (e->on != HERO_POLL_ON_SOCKET) {
            continue;
        }
        os = hero_poll_select(slot, event, e->fd, NULL);
        if (os != 0) {
            for (uint32_t j = 0; j < i; j += 1) {
                const hero_poll_entry *done = &hero_poll_entries[slot->base + j];

                if (done->on == HERO_POLL_ON_SOCKET) {
                    (void)WSAEventSelect((SOCKET)(uint64_t)done->fd, NULL, 0);
                }
            }
            return os;
        }
    }
    return 0;
}

/* The event of a set handed out, 0 for none. */
static uintptr_t hero_poll_event_of(const hero_poll_slot *slot) {
    return slot->shown ? atomic_load(&hero_poll_events[slot - hero_poll_sets]) : 0;
}
#endif

#if !defined(HERO_POLL_ARRAY)
/* A set's queue, close-on-exec, the stop's pipe watched in it under the key 0
 * where the pipe stands: -1 with the system's number where refused. */
static int hero_poll_queue(int64_t *code) {
    int q = hero_poll_queue_made();

    if (q < 0) {
        *code = hero_poll_os_code();
        return -1;
    }
    (void)fcntl(q, F_SETFD, FD_CLOEXEC);
    int bell = atomic_load(&hero_poll_bell_in);

    if (bell >= 0) {
        int64_t refused = hero_poll_watch(q, bell, HERO_POLL_READ, 0, 0);

        if (refused != 0) {
            (void)close(q);
            *code = refused;
            return -1;
        }
    }
#if defined(HERO_POLL_KQUEUE)
    /* The doorbell, an event of the queue's own that a ring triggers and a
     * wait's taking of it clears: no descriptor, so every set has one. */
    struct kevent door;

    EV_SET(&door, HERO_POLL_DOOR_IDENT, EVFILT_USER, EV_ADD | EV_CLEAR, 0, 0, (void *)(uintptr_t)HERO_POLL_DOOR_KEY);
    if (kevent(q, &door, 1, NULL, 0, NULL) != 0) {
        *code = hero_poll_os_code();
        (void)close(q);
        return -1;
    }
#endif
    return q;
}
#endif

/* -- making, adding, changing, removing -------------------------------------- */

hero_poll_set *hero_poll_make(int64_t most) {
    if (most < 1 || most > HERO_POLL_MOST) {
        hero_poll_fail(HERO_POLL_INVALID, 0);
        return NULL;
    }
    hero_poll_enter();
    hero_poll_up();
    hero_poll_leave();

    int queue = -1;
#if !defined(HERO_POLL_ARRAY)
    int64_t code = 0;

    queue = hero_poll_queue(&code);
    if (queue < 0) {
        hero_poll_fail(hero_poll_kind_of(code) == HERO_POLL_LIMIT ? HERO_POLL_LIMIT : HERO_POLL_OTHER, code);
        return NULL;
    }
#endif
    uint32_t need = (uint32_t)most + 1;
    uint32_t base = 0;
    uint32_t index = 0;
    bool room = true;
    hero_poll_set *handle = NULL;

    hero_poll_enter();
    if (!hero_poll_range(need, &base)) {
        room = false;
    } else if (hero_poll_given_back != 0) {
        index = hero_poll_given_back - 1;
        hero_poll_given_back = hero_poll_sets[index].next;
    } else if (hero_poll_fresh < HERO_POLL_SETS) {
        index = hero_poll_fresh;
        hero_poll_fresh += 1;
    } else {
        room = false;
    }
    if (room) {
        hero_poll_slot *slot = &hero_poll_sets[index];

        slot->life += 1;
        slot->held = true;
        slot->closing = false;
        slot->waiting = false;
        slot->busy = 0;
        slot->base = base;
        slot->span = need;
        slot->fresh = 0;
        slot->given_back = 0;
        slot->count = 0;
        slot->timers = 0;
        slot->files = 0;
        slot->mailboxes = 0;
        slot->door_in = -1;
        slot->door_out = -1;
        slot->shown = false;
        slot->rounds = 0;
        slot->queue = queue;
        slot->next = 0;
        handle = hero_poll_handle(index, slot->life);
    }
    hero_poll_leave();

    if (!room) {
#if !defined(HERO_POLL_ARRAY)
        (void)close(queue);
#endif
        hero_poll_fail(HERO_POLL_LIMIT, 0);
        return NULL;
    }
    hero_poll_clear();
    return handle;
}

/* A new entry of the set: watched where it stands on a descriptor, linked
 * where it is a timer or a file. Its key, or -1 with the kind set; nothing of
 * the caller's hold is let go here, which the caller does on -1. */
static int64_t hero_poll_enter_entry(hero_poll_set *set, uint8_t on, unsigned interest, int64_t fd, int64_t net_hold, hero_stream *stream,
                                     int64_t due, int64_t every, int64_t number) {
    int64_t kind = HERO_POLL_OK;
    int64_t os = 0;
    int64_t key = -1;

    hero_poll_enter();
    hero_poll_slot *slot = hero_poll_find(set);

    if (slot == NULL) {
        kind = HERO_POLL_NOT_OPEN;
    } else if (slot->waiting) {
        kind = HERO_POLL_INVALID;
    } else if (slot->count >= slot->span - 1 || (slot->given_back == 0 && slot->fresh >= slot->span - 1)) {
        kind = HERO_POLL_LIMIT;
    } else {
        uint32_t index = 0;

        if (slot->given_back != 0) {
            index = slot->given_back - 1;
            slot->given_back = hero_poll_entries[index].next;
        } else {
            index = slot->base + slot->fresh;
            slot->fresh += 1;
        }
        hero_poll_entry *e = &hero_poll_entries[index];

        e->life += 1;
        e->on = on;
        e->interest = (uint8_t)interest;
        e->fd = fd;
        e->net_hold = net_hold;
        e->stream = stream;
        e->due = due;
        e->every = every;
        e->number = number;
        e->next = 0;
        e->after = 0;
        e->before = 0;
        e->round = 0;
        e->told_at = 0;
        key = hero_poll_key(e);
        if (on == HERO_POLL_ON_SOCKET || on == HERO_POLL_ON_STREAM || on == HERO_POLL_ON_SIGNALS) {
            os = hero_poll_watch(slot->queue, fd, interest, 0, (uint64_t)key);
        }
#if defined(_WIN32)
        /* A set handed out has its event told of the socket as it stands. */
        if (os == 0 && on == HERO_POLL_ON_SOCKET && slot->shown) {
            os = hero_poll_select(slot, hero_poll_event_of(slot), fd, NULL);
        }
#endif
        if (os != 0) {
            kind = hero_poll_kind_of(os);
            e->on = HERO_POLL_ON_NOTHING;
            if (e->life < HERO_POLL_LIFE_MOST) {
                e->next = slot->given_back;
                slot->given_back = index + 1;
            }
            key = -1;
        } else {
            slot->count += 1;
            if (on == HERO_POLL_ON_TIMER) {
                hero_poll_link(&slot->timers, e);
            } else if (on == HERO_POLL_ON_FILE) {
                hero_poll_link(&slot->files, e);
            } else if (on == HERO_POLL_ON_MAILBOX) {
                hero_poll_link(&slot->mailboxes, e);
            }
        }
    }
    hero_poll_leave();

    if (key < 0) {
        hero_poll_fail(kind, os);
        return -1;
    }
    hero_poll_clear();
    return key;
}

static bool hero_poll_interest_ok(int64_t interest) {
    return interest == HERO_POLL_READ || interest == HERO_POLL_WRITE || interest == (HERO_POLL_READ | HERO_POLL_WRITE);
}

int64_t hero_poll_add_socket(hero_poll_set *set, hero_net_socket *s, int64_t interest) {
    if (!hero_poll_interest_ok(interest)) {
        hero_poll_fail(HERO_POLL_INVALID, 0);
        return -1;
    }
    int64_t fd = -1;
    int64_t hold = hero_net_hold(s, &fd);

    if (hold < 0) {
        hero_poll_fail(HERO_POLL_NOT_OPEN, 0);
        return -1;
    }
    int64_t key = hero_poll_enter_entry(set, HERO_POLL_ON_SOCKET, (unsigned)interest, fd, hold, NULL, -1, 0, -1);

    if (key < 0) {
        int64_t kind = hero_poll_kind;
        int64_t os = hero_poll_os;

        hero_net_unhold(hold);
        hero_poll_fail(kind, os);
    }
    return key;
}

int64_t hero_poll_add_stream(hero_poll_set *set, hero_stream *s, int64_t interest) {
    if (!hero_poll_interest_ok(interest)) {
        hero_poll_fail(HERO_POLL_INVALID, 0);
        return -1;
    }
    int64_t kind = hero_stream_kind(s);

    if (kind < 0) {
        hero_poll_fail(HERO_POLL_NOT_OPEN, 0);
        return -1;
    }
    if (kind == HERO_STREAM_KIND_FILE) {
        return hero_poll_enter_entry(set, HERO_POLL_ON_FILE, (unsigned)interest, -1, -1, NULL, -1, 0, -1);
    }
    /* A socket's stream `hero_stream_net.h` made; one of another member's
     * that says it is a socket (an ipc channel, a TLS session) is
     * `hero_stream_net_socket`'s `HERO_STREAM_INVALID` and is waited on as
     * every other stream is, below. */
    hero_net_socket *socket = kind == HERO_STREAM_KIND_SOCKET ? hero_stream_net_socket(s) : NULL;

    if (kind == HERO_STREAM_KIND_SOCKET && socket == NULL && hero_stream_last_error() != HERO_STREAM_INVALID) {
        hero_poll_fail(hero_stream_last_error() == HERO_STREAM_NOT_OPEN ? HERO_POLL_NOT_OPEN : HERO_POLL_UNSUPPORTED, 0);
        return -1;
    }
    if (socket != NULL) {
        /* The socket under the stream, held only while its own hold is
         * taken: from then the descriptor stays open for the entry whatever
         * closes the stream, and the stream's close shuts the socket down. */
        int64_t fd = -1;
        int64_t hold = hero_net_hold(socket, &fd);

        (void)hero_stream_release(s);
        if (hold < 0) {
            hero_poll_fail(HERO_POLL_NOT_OPEN, 0);
            return -1;
        }
        int64_t key = hero_poll_enter_entry(set, HERO_POLL_ON_SOCKET, (unsigned)interest, fd, hold, NULL, -1, 0, -1);

        if (key < 0) {
            int64_t failed = hero_poll_kind;
            int64_t os = hero_poll_os;

            hero_net_unhold(hold);
            hero_poll_fail(failed, os);
        }
        return key;
    }
#if defined(_WIN32)
    /* `WSAPoll` waits on sockets alone: a pipe's or a console's handle is
     * refused (`WSAENOTSOCK`, measured by `hero_net.c`'s sitting on the box). */
    hero_poll_fail(HERO_POLL_UNSUPPORTED, 0);
    return -1;
#else
    /* The stream's own descriptor, or the one its table says its readiness
     * is (a serial port's, an ipc channel's); a table with none, a TLS
     * session's, is `HERO_STREAM_UNSUPPORTED` (`hero_stream.h`). */
    int64_t fd = hero_stream_hold_waitable(s);

    if (fd < 0) {
        hero_poll_fail(hero_stream_last_error() == HERO_STREAM_NOT_OPEN ? HERO_POLL_NOT_OPEN : HERO_POLL_UNSUPPORTED, 0);
        return -1;
    }
    int64_t key = hero_poll_enter_entry(set, HERO_POLL_ON_STREAM, (unsigned)interest, fd, -1, s, -1, 0, -1);

    if (key < 0) {
        int64_t failed = hero_poll_kind;
        int64_t os = hero_poll_os;

        (void)hero_stream_release(s);
        hero_poll_fail(failed, os);
    }
    return key;
#endif
}

int64_t hero_poll_add_timer(hero_poll_set *set, int64_t first_ns, int64_t every_ns) {
    if (first_ns < 0 || every_ns < 0) {
        hero_poll_fail(HERO_POLL_INVALID, 0);
        return -1;
    }
    int64_t now = hero_poll_now();

    if (now < 0) {
        hero_poll_fail(HERO_POLL_OTHER, 0);
        return -1;
    }
    return hero_poll_enter_entry(set, HERO_POLL_ON_TIMER, 0, -1, -1, NULL, hero_poll_later(now, first_ns), every_ns, -1);
}

int64_t hero_poll_add_signals(hero_poll_set *set) {
#if defined(_WIN32)
    (void)set;
    hero_poll_fail(HERO_POLL_UNSUPPORTED, 0);
    return -1;
#else
    int door = hero_signal_door();

    if (door < 0) {
        hero_poll_fail(HERO_POLL_OTHER, hero_poll_os_code());
        return -1;
    }
    /* Its own copy, close-on-exec: two entries of one descriptor would be one
     * to `kqueue` and refused by `epoll`. */
    int copy = fcntl(door, F_DUPFD_CLOEXEC, 3);

    if (copy < 0) {
        int64_t code = hero_poll_os_code();

        hero_poll_fail(hero_poll_kind_of(code), code);
        return -1;
    }
    int64_t key = hero_poll_enter_entry(set, HERO_POLL_ON_SIGNALS, HERO_POLL_READ, copy, -1, NULL, -1, 0, -1);

    if (key < 0) {
        (void)close(copy);
    }
    return key;
#endif
}

int64_t hero_poll_add_mailbox(hero_poll_set *set, int64_t number) {
    if (hero_mail_ready(number) < 0) {
        hero_poll_fail(HERO_POLL_NOT_OPEN, 0);
        return -1;
    }
    int64_t kind = HERO_POLL_OK;
    hero_poll_slot *slot = NULL;
    bool door = false;

    hero_poll_enter();
    slot = hero_poll_find(set);
    if (slot == NULL) {
        kind = HERO_POLL_NOT_OPEN;
    } else if (slot->waiting) {
        kind = HERO_POLL_INVALID;
    } else {
        door = slot->door_in < 0;
    }
    hero_poll_leave();

    if (kind != HERO_POLL_OK) {
        hero_poll_fail(kind, 0);
        return -1;
    }
#if defined(HERO_POLL_KQUEUE)
    (void)door;
#else
    if (door) {
        int64_t in = -1;
        int64_t out = -1;
        int64_t code = 0;

        if (!hero_poll_door_made(&in, &out, &code)) {
            hero_poll_fail(hero_poll_kind_of(code) == HERO_POLL_LIMIT ? HERO_POLL_LIMIT : HERO_POLL_OTHER, code);
            return -1;
        }
        bool kept = false;

        hero_poll_enter();
        if (hero_poll_find(set) == slot && slot->door_in < 0 && !slot->waiting) {
#if defined(HERO_POLL_EPOLL)
            code = hero_poll_watch(slot->queue, in, HERO_POLL_READ, 0, HERO_POLL_DOOR_KEY);
#endif
            if (code == 0) {
                slot->door_in = in;
                slot->door_out = out;
                kept = true;
            }
        }
        hero_poll_leave();

        if (!kept) {
            hero_poll_door_close(in, out);
            if (code != 0) {
                hero_poll_fail(hero_poll_kind_of(code), code);
                return -1;
            }
        }
    }
#endif
    int64_t key = hero_poll_enter_entry(set, HERO_POLL_ON_MAILBOX, HERO_POLL_READ, -1, -1, NULL, -1, 0, number);

    if (key < 0) {
        return -1;
    }
    /* Watched once it stands: a value sent before is found by the wait's own
     * question, which comes before any block. */
    if (hero_mail_watch(number, hero_poll_ring, slot) != 0) {
        (void)hero_poll_remove(set, key);
        hero_poll_fail(HERO_POLL_NOT_OPEN, 0);
        return -1;
    }
    return key;
}

int64_t hero_poll_set_timer(hero_poll_set *set, int64_t key, int64_t first_ns, int64_t every_ns) {
    int64_t now = hero_poll_now();
    int64_t kind = HERO_POLL_OK;

    if (first_ns < 0 || every_ns < 0) {
        kind = HERO_POLL_INVALID;
    } else if (now < 0) {
        kind = HERO_POLL_OTHER;
    } else {
        hero_poll_enter();
        hero_poll_slot *slot = hero_poll_find(set);
        hero_poll_entry *e = slot == NULL ? NULL : hero_poll_entry_of(slot, key);

        if (slot == NULL) {
            kind = HERO_POLL_NOT_OPEN;
        } else if (slot->waiting) {
            kind = HERO_POLL_INVALID;
        } else if (e == NULL) {
            kind = HERO_POLL_NOT_FOUND;
        } else if (e->on != HERO_POLL_ON_TIMER) {
            kind = HERO_POLL_INVALID;
        } else {
            e->due = hero_poll_later(now, first_ns);
            e->every = every_ns;
        }
        hero_poll_leave();
    }
    if (kind != HERO_POLL_OK) {
        hero_poll_fail(kind, 0);
        return -1;
    }
    hero_poll_clear();
    return 0;
}

int64_t hero_poll_change(hero_poll_set *set, int64_t key, int64_t interest) {
    int64_t kind = HERO_POLL_OK;
    int64_t os = 0;

    hero_poll_enter();
    hero_poll_slot *slot = hero_poll_find(set);
    hero_poll_entry *e = slot == NULL ? NULL : hero_poll_entry_of(slot, key);

    if (slot == NULL) {
        kind = HERO_POLL_NOT_OPEN;
    } else if (slot->waiting) {
        kind = HERO_POLL_INVALID;
    } else if (e == NULL) {
        kind = HERO_POLL_NOT_FOUND;
    } else if (e->on == HERO_POLL_ON_TIMER || e->on == HERO_POLL_ON_SIGNALS || e->on == HERO_POLL_ON_MAILBOX || !hero_poll_interest_ok(interest)) {
        kind = HERO_POLL_INVALID;
    } else {
        if (e->on == HERO_POLL_ON_SOCKET || e->on == HERO_POLL_ON_STREAM) {
            os = hero_poll_watch(slot->queue, e->fd, (unsigned)interest, e->interest, (uint64_t)key);
        }
#if defined(_WIN32)
        if (os == 0 && e->on == HERO_POLL_ON_SOCKET && slot->shown) {
            uint8_t was = e->interest;

            e->interest = (uint8_t)interest;
            os = hero_poll_select(slot, hero_poll_event_of(slot), e->fd, NULL);
            e->interest = was;
        }
#endif
        if (os != 0) {
            kind = hero_poll_kind_of(os);
        } else {
            e->interest = (uint8_t)interest;
        }
    }
    hero_poll_leave();

    if (kind != HERO_POLL_OK) {
        hero_poll_fail(kind, os);
        return -1;
    }
    hero_poll_clear();
    return 0;
}

/* What an entry held, let go outside the lock. */
typedef struct {
    int64_t net_hold;
    hero_stream *stream;
    /* The signals entry's own copy of the door's descriptor, -1 for none. */
    int64_t copy;
    /* A mailbox entry's number and the set its watch rings, -1 for none. */
    int64_t number;
    hero_poll_slot *watched;
} hero_poll_held;

static void hero_poll_let_go(hero_poll_held held) {
    if (held.net_hold >= 0) {
        hero_net_unhold(held.net_hold);
    }
    if (held.stream != NULL) {
        (void)hero_stream_release(held.stream);
    }
#if !defined(_WIN32)
    if (held.copy >= 0) {
        (void)close((int)held.copy);
    }
#endif
    if (held.number >= 0) {
        /* -1 where the mailbox has gone, its watches with it. */
        (void)hero_mail_unwatch(held.number, hero_poll_ring, held.watched);
    }
}

/* The entry taken out of its set's lists and given back, what it held
 * answered. Under the lock; the system forgets the descriptor separately. */
static hero_poll_held hero_poll_entry_free(hero_poll_slot *slot, hero_poll_entry *e) {
    hero_poll_held held = { e->net_hold, e->stream, e->on == HERO_POLL_ON_SIGNALS ? e->fd : -1, e->on == HERO_POLL_ON_MAILBOX ? e->number : -1, slot };

    if (e->on == HERO_POLL_ON_TIMER) {
        hero_poll_unlink(&slot->timers, e);
    } else if (e->on == HERO_POLL_ON_FILE) {
        hero_poll_unlink(&slot->files, e);
    } else if (e->on == HERO_POLL_ON_MAILBOX) {
        hero_poll_unlink(&slot->mailboxes, e);
    }
    e->on = HERO_POLL_ON_NOTHING;
    e->net_hold = -1;
    e->stream = NULL;
    e->fd = -1;
    e->number = -1;
    slot->count -= 1;
    if (e->life < HERO_POLL_LIFE_MOST) {
        e->next = slot->given_back;
        slot->given_back = (uint32_t)(e - hero_poll_entries) + 1;
    }
    return held;
}

int64_t hero_poll_remove(hero_poll_set *set, int64_t key) {
    int64_t kind = HERO_POLL_OK;
    hero_poll_held held = { -1, NULL, -1, -1, NULL };

    hero_poll_enter();
    hero_poll_slot *slot = hero_poll_find(set);
    hero_poll_entry *e = slot == NULL ? NULL : hero_poll_entry_of(slot, key);

    if (slot == NULL) {
        kind = HERO_POLL_NOT_OPEN;
    } else if (slot->waiting) {
        kind = HERO_POLL_INVALID;
    } else if (e == NULL) {
        kind = HERO_POLL_NOT_FOUND;
    } else {
        if (e->on == HERO_POLL_ON_SOCKET || e->on == HERO_POLL_ON_STREAM || e->on == HERO_POLL_ON_SIGNALS) {
            /* Forgotten by the system before its hold is let go: the
             * descriptor is still open, so the call reaches this entry's. */
            (void)hero_poll_watch(slot->queue, e->fd, 0, e->interest, (uint64_t)key);
        }
#if defined(_WIN32)
        if (e->on == HERO_POLL_ON_SOCKET && slot->shown) {
            /* What the system is told of the socket afterwards is what the
             * entries left on it want, none cancelling it; a refusal here
             * leaves a notice or two more than they want, which is a hint. */
            int64_t fd = e->fd;

            held = hero_poll_entry_free(slot, e);
            (void)hero_poll_select(slot, hero_poll_event_of(slot), fd, NULL);
        } else {
            held = hero_poll_entry_free(slot, e);
        }
#else
        held = hero_poll_entry_free(slot, e);
#endif
    }
    hero_poll_leave();

    if (kind != HERO_POLL_OK) {
        hero_poll_fail(kind, 0);
        return -1;
    }
    hero_poll_let_go(held);
    hero_poll_clear();
    return 0;
}

int64_t hero_poll_count(hero_poll_set *set) {
    hero_poll_enter();
    hero_poll_slot *slot = hero_poll_find(set);
    int64_t count = slot == NULL ? -1 : (int64_t)slot->count;
    hero_poll_leave();

    if (count < 0) {
        hero_poll_fail(HERO_POLL_NOT_OPEN, 0);
        return -1;
    }
    hero_poll_clear();
    return count;
}

int64_t hero_poll_descriptor(hero_poll_set *set) {
#if defined(_WIN32)
    int64_t kind = HERO_POLL_OK;
    int64_t os = 0;
    int64_t answer = -1;
    hero_poll_slot *slot = NULL;
    uintptr_t event = 0;
    bool shown = false;

    hero_poll_enter();
    slot = hero_poll_find(set);
    if (slot == NULL) {
        kind = HERO_POLL_NOT_OPEN;
    } else if (slot->waiting) {
        kind = HERO_POLL_INVALID;
    } else {
        shown = slot->shown;
        event = atomic_load(&hero_poll_events[slot - hero_poll_sets]);
    }
    hero_poll_leave();

    if (kind != HERO_POLL_OK) {
        hero_poll_fail(kind, 0);
        return -1;
    }
    if (!shown) {
        if (event == 0) {
            /* Manual-reset, not set, inherited by no child: the set's own. */
            HANDLE made = CreateEventW(NULL, TRUE, FALSE, NULL);
            uintptr_t none = 0;

            if (made == NULL) {
                int64_t code = (int64_t)GetLastError();

                hero_poll_fail(hero_poll_kind_of(code) == HERO_POLL_LIMIT ? HERO_POLL_LIMIT : HERO_POLL_OTHER, code);
                return -1;
            }
            if (atomic_compare_exchange_strong(&hero_poll_events[slot - hero_poll_sets], &none, (uintptr_t)made)) {
                event = (uintptr_t)made;
            } else {
                (void)CloseHandle(made);
                event = none;
            }
        }
        hero_poll_enter();
        if (hero_poll_find(set) == slot && !slot->shown && !slot->waiting) {
            /* Reset first: the event of a set that held the slot before may
             * still be set. Every socket then told to it, or none is. */
            (void)ResetEvent((HANDLE)event);
            os = hero_poll_select_all(slot, event);
            if (os == 0) {
                slot->shown = true;
            }
        }
        hero_poll_leave();

        if (os != 0) {
            hero_poll_fail(hero_poll_kind_of(os), os);
            return -1;
        }
    }
    hero_poll_enter();
    if (hero_poll_find(set) == slot && slot->shown) {
        answer = (int64_t)atomic_load(&hero_poll_events[slot - hero_poll_sets]);
    }
    hero_poll_leave();

    if (answer < 0) {
        hero_poll_fail(HERO_POLL_NOT_OPEN, 0);
        return -1;
    }
    /* Set once, so whatever is ready now, a mailbox's value, a file or a
     * socket, is told by the handle too; a wait that finds nothing resets it,
     * and a wait asked after the stop never gets as far as resetting. */
    hero_poll_ring(slot);
    hero_poll_clear();
    return answer;
#elif defined(HERO_POLL_ARRAY)
    hero_poll_enter();
    bool open = hero_poll_find(set) != NULL;
    hero_poll_leave();

    hero_poll_fail(open ? HERO_POLL_UNSUPPORTED : HERO_POLL_NOT_OPEN, 0);
    return -1;
#else
    int64_t kind = HERO_POLL_OK;
    int64_t answer = -1;
    hero_poll_slot *slot = NULL;
    bool door = false;

    hero_poll_enter();
    slot = hero_poll_find(set);
    if (slot == NULL) {
        kind = HERO_POLL_NOT_OPEN;
    } else if (slot->waiting) {
        kind = HERO_POLL_INVALID;
    } else {
        door = slot->door_in < 0;
    }
    hero_poll_leave();

    if (kind != HERO_POLL_OK) {
        hero_poll_fail(kind, 0);
        return -1;
    }
#if defined(HERO_POLL_EPOLL)
    if (door) {
        /* The doorbell made now, so a file or a mailbox told by a wait can
         * keep the descriptor readable. */
        int64_t in = -1;
        int64_t out = -1;
        int64_t code = 0;

        if (!hero_poll_door_made(&in, &out, &code)) {
            hero_poll_fail(hero_poll_kind_of(code) == HERO_POLL_LIMIT ? HERO_POLL_LIMIT : HERO_POLL_OTHER, code);
            return -1;
        }
        bool kept = false;

        hero_poll_enter();
        if (hero_poll_find(set) == slot && slot->door_in < 0 && !slot->waiting) {
            code = hero_poll_watch(slot->queue, in, HERO_POLL_READ, 0, HERO_POLL_DOOR_KEY);
            if (code == 0) {
                slot->door_in = in;
                slot->door_out = out;
                kept = true;
            }
        }
        hero_poll_leave();

        if (!kept) {
            hero_poll_door_close(in, out);
            if (code != 0) {
                hero_poll_fail(hero_poll_kind_of(code), code);
                return -1;
            }
        }
    }
#else
    (void)door;
#endif
    hero_poll_enter();
    if (hero_poll_find(set) == slot) {
        slot->shown = true;
        answer = slot->queue;
    }
    hero_poll_leave();

    if (answer < 0) {
        hero_poll_fail(HERO_POLL_NOT_OPEN, 0);
        return -1;
    }
    /* Rung once, so whatever is ready now, a mailbox's value or a file, is
     * told by the descriptor too; a wait that finds nothing blocks again. */
    hero_poll_ring(slot);
    hero_poll_clear();
    return answer;
#endif
}

int64_t hero_poll_next_due(hero_poll_set *set) {
    int64_t now = hero_poll_now();
    int64_t next = -1;

    hero_poll_enter();
    hero_poll_slot *slot = hero_poll_find(set);

    if (slot != NULL) {
        for (uint32_t at = slot->timers; at != 0; at = hero_poll_entries[at - 1].after) {
            const hero_poll_entry *e = &hero_poll_entries[at - 1];

            if (e->due >= 0 && (next < 0 || e->due < next)) {
                next = e->due;
            }
        }
    }
    hero_poll_leave();

    if (slot == NULL) {
        hero_poll_fail(HERO_POLL_NOT_OPEN, 0);
        return -1;
    }
    hero_poll_clear();
    if (next < 0) {
        return -1;
    }
    return now < 0 || next <= now ? 0 : next - now;
}

/* The set released by the one thread that owns its release: its queue closed
 * first, so the system forgets every descriptor before any hold is let go,
 * then each entry given back and let go, then the slot. The set is closing,
 * so no other call finds it meanwhile. */
static void hero_poll_release(hero_poll_slot *slot) {
    /* Every watch taken away first: once the last is, no ring reaches the
     * queue or the doorbell, which may then close. */
    for (uint32_t at = slot->mailboxes; at != 0; at = hero_poll_entries[at - 1].after) {
        hero_poll_entry *e = &hero_poll_entries[at - 1];

        (void)hero_mail_unwatch(e->number, hero_poll_ring, slot);
        e->number = -1;
    }
#if !defined(HERO_POLL_ARRAY)
    if (slot->queue >= 0) {
        (void)close(slot->queue);
    }
#endif
#if !defined(HERO_POLL_KQUEUE)
    hero_poll_door_close(slot->door_in, slot->door_out);
#endif
    for (uint32_t i = 0; i < slot->fresh; i += 1) {
        hero_poll_held held = { -1, NULL, -1, -1, NULL };

        hero_poll_enter();
        hero_poll_entry *e = &hero_poll_entries[slot->base + i];

        if (e->on != HERO_POLL_ON_NOTHING) {
#if defined(_WIN32)
            /* Forgotten by the system before the socket's hold is let go. */
            if (e->on == HERO_POLL_ON_SOCKET && slot->shown) {
                (void)WSAEventSelect((SOCKET)(uint64_t)e->fd, NULL, 0);
            }
#endif
            held = hero_poll_entry_free(slot, e);
        }
        hero_poll_leave();
        hero_poll_let_go(held);
    }
#if defined(_WIN32)
    /* A loop that waits on the event learns the set is gone: it is set, kept
     * as it is for the slot's next set to reset (`hero_poll_events`). */
    uintptr_t event = hero_poll_event_of(slot);

    if (event != 0) {
        (void)SetEvent((HANDLE)event);
    }
#endif
    hero_poll_enter();
    hero_poll_slot_free(slot);
    hero_poll_leave();
}

int64_t hero_poll_close(hero_poll_set *set) {
    bool now = false;

    hero_poll_enter();
    hero_poll_slot *slot = hero_poll_find(set);

    if (slot != NULL) {
        slot->closing = true;
        now = slot->busy == 0;
    }
    hero_poll_leave();

    if (slot == NULL) {
        hero_poll_fail(HERO_POLL_NOT_OPEN, 0);
        return -1;
    }
    if (now) {
        hero_poll_release(slot);
    }
    hero_poll_clear();
    return 0;
}

/* -- the wait ---------------------------------------------------------------- */

/* One event written at `at` in the caller's numbers. */
static void hero_poll_tell(int64_t *events, uint64_t at, int64_t key, int64_t bits, int64_t times) {
    events[at * HERO_POLL_EVENT_NUMBERS] = key;
    events[at * HERO_POLL_EVENT_NUMBERS + 1] = bits;
    events[at * HERO_POLL_EVENT_NUMBERS + 2] = times;
}

/* An event of the system's for `key`, the bits it says, added to the answer:
 * merged into the event the entry already has this round, or written after
 * the others where there is room. Where the entry is gone, or the bits hold
 * nothing it waits for, nothing is written. Under the lock. */
static void hero_poll_told(hero_poll_slot *slot, uint64_t key, int64_t bits, int64_t *events, uint64_t room, uint64_t *written) {
    hero_poll_entry *e = hero_poll_entry_of(slot, (int64_t)key);

    if (e == NULL || (e->on != HERO_POLL_ON_SOCKET && e->on != HERO_POLL_ON_STREAM && e->on != HERO_POLL_ON_SIGNALS)) {
        return;
    }
    if (e->on == HERO_POLL_ON_SIGNALS) {
        /* Whatever the system says of the pipe is told as READ only where a
         * count stands behind it; a byte with none is drained here and the
         * wait goes on. */
        if (hero_signal_any() == 0) {
            return;
        }
        bits = HERO_POLL_READ;
    }
    if ((bits & HERO_POLL_HANGUP) != 0 && (e->interest & HERO_POLL_READ) != 0) {
        bits |= HERO_POLL_READ;
    }
    bits &= (int64_t)(e->interest | HERO_POLL_HANGUP | HERO_POLL_ERROR);
    if (bits == 0) {
        return;
    }
    if (e->round == slot->rounds) {
        events[e->told_at * HERO_POLL_EVENT_NUMBERS + 1] |= bits;
        return;
    }
    if (*written >= room) {
        return;
    }
    e->round = slot->rounds;
    e->told_at = *written;
    hero_poll_tell(events, *written, (int64_t)key, bits, 0);
    *written += 1;
}

/* The timers fallen due by `now` and the files, written first; the earliest
 * time a timer still to come is due answered in `next`, -1 for none. Under
 * the lock. */
static uint64_t hero_poll_due(hero_poll_slot *slot, int64_t now, int64_t *events, uint64_t room, int64_t *next) {
    uint64_t written = 0;

    *next = -1;
    for (uint32_t at = slot->timers; at != 0; at = hero_poll_entries[at - 1].after) {
        hero_poll_entry *e = &hero_poll_entries[at - 1];

        if (e->due < 0) {
            continue;
        }
        if (e->due > now || written >= room) {
            if (*next < 0 || e->due < *next) {
                *next = e->due;
            }
            continue;
        }
        int64_t times = 1;

        if (e->every == 0) {
            e->due = -1;
        } else {
            /* Every time it fell due by now is counted, and the next is the
             * first still to come: never a burst of the ones waited past. */
            times += (now - e->due) / e->every;
            int64_t step = 0;

            e->due = __builtin_mul_overflow(times, e->every, &step) ? INT64_MAX : hero_poll_later(e->due, step);
            if (*next < 0 || e->due < *next) {
                *next = e->due;
            }
        }
        hero_poll_tell(events, written, hero_poll_key(e), HERO_POLL_DUE, times);
        written += 1;
    }
    for (uint32_t at = slot->files; at != 0 && written < room; at = hero_poll_entries[at - 1].after) {
        hero_poll_entry *e = &hero_poll_entries[at - 1];

        hero_poll_tell(events, written, hero_poll_key(e), (int64_t)e->interest, 0);
        written += 1;
    }
    return written;
}

/* Each mailbox of the set asked, after the timers and the files and outside
 * the lock: told READ while a receive on it would not wait, and HANGUP beside
 * it once its last copy is gone; one event each, as many as there is room
 * for, the rest told at the next wait, since each is asked again there. The
 * set's entries stay as they are while it waits (ONE CALL AT A TIME). */
static uint64_t hero_poll_mailboxes_told(uint32_t first, int64_t *events, uint64_t room, uint64_t written, bool *ready_left) {
    for (uint32_t at = first; at != 0; at = hero_poll_entries[at - 1].after) {
        const hero_poll_entry *e = &hero_poll_entries[at - 1];

        if (written >= room) {
            /* Not asked: it may be ready, and is told at the next wait. */
            *ready_left = true;
            break;
        }
        int64_t ready = hero_mail_ready(e->number);

        if (ready == 0) {
            continue;
        }
        *ready_left = true;
        hero_poll_tell(events, written, hero_poll_key(e), ready > 0 ? HERO_POLL_READ : HERO_POLL_READ | HERO_POLL_HANGUP, 0);
        written += 1;
    }
    return written;
}

/* How long the system may wait, in milliseconds, rounded up so no wait ends
 * before its time: none past `ends` or the next timer, at most a chunk, and a
 * slice where the stop cannot be heard; -1 for no limit. */
static int hero_poll_ms(int64_t now, int64_t ends, int64_t next, bool sliced) {
    int64_t until = ends;

    if (next >= 0 && (until < 0 || next < until)) {
        until = next;
    }
    int64_t ms = -1;

    if (until >= 0) {
        int64_t left = until > now ? until - now : 0;

        ms = left / 1000000 + (left % 1000000 != 0 ? 1 : 0);
    }
    if (ms < 0 || ms > HERO_POLL_CHUNK_MS) {
        ms = HERO_POLL_CHUNK_MS;
    }
    if (sliced && ms > HERO_POLL_SLICE_MS) {
        ms = HERO_POLL_SLICE_MS;
    }
    return (int)ms;
}

#if defined(HERO_POLL_ARRAY)
/* The set's array filled for the system: every entry on a descriptor, then
 * the bell where it stands. How many places. Under the lock. */
static uint32_t hero_poll_fill(const hero_poll_slot *slot) {
    uint32_t n = 0;

    for (uint32_t i = 0; i < slot->fresh; i += 1) {
        const hero_poll_entry *e = &hero_poll_entries[slot->base + i];

        if (e->on != HERO_POLL_ON_SOCKET && e->on != HERO_POLL_ON_STREAM && e->on != HERO_POLL_ON_SIGNALS) {
            continue;
        }
        hero_poll_fd *p = &hero_poll_array[slot->base + n];

        memset(p, 0, sizeof *p);
#if defined(_WIN32)
        p->fd = (SOCKET)(uint64_t)e->fd;
#else
        p->fd = (int)e->fd;
#endif
        p->events = (short)(((e->interest & HERO_POLL_READ) != 0 ? HERO_POLL_IN : 0) | ((e->interest & HERO_POLL_WRITE) != 0 ? HERO_POLL_OUT : 0));
        hero_poll_array_entry[slot->base + n] = slot->base + i + 1;
        n += 1;
    }
    /* The doorbell while a mailbox stands: that entry takes no place, so the
     * set's range, its most entries and the bell, holds the doorbell too. */
    if (slot->mailboxes != 0 && slot->door_in >= 0) {
        hero_poll_fd *p = &hero_poll_array[slot->base + n];

        memset(p, 0, sizeof *p);
#if defined(_WIN32)
        p->fd = (SOCKET)(uint64_t)slot->door_in;
#else
        p->fd = (int)slot->door_in;
#endif
        p->events = HERO_POLL_IN;
        hero_poll_array_entry[slot->base + n] = HERO_POLL_DOOR_PLACE;
        n += 1;
    }
#if defined(_WIN32)
    SOCKET bell = atomic_load(&hero_poll_bell);

    if (bell != INVALID_SOCKET) {
        hero_poll_fd *p = &hero_poll_array[slot->base + n];

        memset(p, 0, sizeof *p);
        p->fd = bell;
        p->events = HERO_POLL_IN;
        hero_poll_array_entry[slot->base + n] = 0;
        n += 1;
    }
#else
    int bell = atomic_load(&hero_poll_bell_in);

    if (bell >= 0) {
        hero_poll_fd *p = &hero_poll_array[slot->base + n];

        memset(p, 0, sizeof *p);
        p->fd = bell;
        p->events = HERO_POLL_IN;
        hero_poll_array_entry[slot->base + n] = 0;
        n += 1;
    }
#endif
    return n;
}
#endif

int64_t hero_poll_wait(hero_poll_set *set, int64_t limit_ns, int64_t *events, uint64_t count) {
    if (limit_ns < -1 || events == NULL || count < HERO_POLL_EVENT_NUMBERS) {
        hero_poll_fail(HERO_POLL_INVALID, 0);
        return -1;
    }
    if (hero_net_stop_asked() != 0) {
        hero_poll_fail(HERO_POLL_STOPPED, 0);
        return -1;
    }
    int64_t started = hero_poll_now();

    if (started < 0) {
        hero_poll_fail(HERO_POLL_OTHER, 0);
        return -1;
    }
    int64_t ends = limit_ns < 0 ? -1 : hero_poll_later(started, limit_ns);
    uint64_t room = count / HERO_POLL_EVENT_NUMBERS;
    bool sliced = atomic_load(&hero_poll_heard) == 0;

    for (;;) {
        int64_t kind = HERO_POLL_OK;
        int64_t next = -1;
        uint64_t written = 0;
        int64_t now = hero_poll_now();
        int queue = -1;
        uint32_t mailboxes = 0;
        int64_t door = -1;
        bool shown = false;
        bool ready_left = false;
#if defined(_WIN32)
        uintptr_t signal = 0;
#endif
#if defined(HERO_POLL_ARRAY)
        uint32_t places = 0;
        uint32_t base = 0;
#endif

        hero_poll_enter();
        hero_poll_slot *slot = hero_poll_find(set);

        if (slot == NULL) {
            kind = HERO_POLL_NOT_OPEN;
        } else if (slot->waiting) {
            kind = HERO_POLL_INVALID;
        } else {
            slot->waiting = true;
            slot->busy += 1;
            written = hero_poll_due(slot, now, events, room, &next);
            queue = slot->queue;
            mailboxes = slot->mailboxes;
            door = slot->door_in;
            shown = slot->shown;
#if defined(HERO_POLL_ARRAY)
            places = hero_poll_fill(slot);
            base = slot->base;
#endif
        }
        hero_poll_leave();

        if (kind != HERO_POLL_OK) {
            hero_poll_fail(kind, 0);
            return -1;
        }
#if defined(_WIN32)
        /* The event of a loop waiting in the system's own call reset first,
         * and what is ready asked after: a notice that comes between sets it
         * again, and nothing the wait finds is lost to the reset. */
        if (shown) {
            signal = atomic_load(&hero_poll_events[slot - hero_poll_sets]);
            (void)ResetEvent((HANDLE)signal);
        }
#endif
        /* Asked before the system is: a value sent after this rings the
         * doorbell, which ends the system's wait at once. */
        written = hero_poll_mailboxes_told(mailboxes, events, room, written, &ready_left);
        (void)door;
        int ms = written > 0 || (ends >= 0 && now >= ends) ? 0 : hero_poll_ms(now, ends, next, sliced);
        bool stopped = false;
        int64_t failed = 0;

        if (written < room) {
#if defined(HERO_POLL_KQUEUE)
            struct kevent got[64];
            uint64_t want = room - written < 64 ? room - written : 64;
            struct timespec wait = { (time_t)(ms / 1000), (long)(ms % 1000) * 1000000L };
            /* A wait that must not block rings the set's own doorbell in the
             * same call: Darwin's `kevent` that finds nothing ready with a
             * zero timeout takes tens of microseconds, and one that finds an
             * event a fraction of one (panel 256, the ffi-pragmatist's
             * `kev0.c`), so an answer found before the system is asked never
             * pays the empty call. */
            struct kevent trigger;
            int changes = 0;

            if (ms == 0) {
                EV_SET(&trigger, HERO_POLL_DOOR_IDENT, EVFILT_USER, 0, NOTE_TRIGGER, 0, (void *)(uintptr_t)HERO_POLL_DOOR_KEY);
                changes = 1;
            }
            int n = kevent(queue, changes > 0 ? &trigger : NULL, changes, got, (int)want, &wait);

            if (n < 0 && errno != EINTR) {
                failed = hero_poll_os_code();
            }
            hero_poll_enter();
            slot->rounds += 1;
            for (int i = 0; i < n; i += 1) {
                uint64_t key = (uint64_t)(uintptr_t)got[i].udata;
                int64_t bits = got[i].filter == EVFILT_WRITE ? HERO_POLL_WRITE : HERO_POLL_READ;

                if (key == 0) {
                    stopped = stopped || hero_poll_bell_heard();
                    continue;
                }
                if (key == HERO_POLL_DOOR_KEY || got[i].filter == EVFILT_USER) {
                    /* Rung: the mailboxes are asked again before any block. */
                    continue;
                }
                if ((got[i].flags & EV_EOF) != 0) {
                    bits |= HERO_POLL_HANGUP;
                    if (got[i].fflags != 0) {
                        bits |= HERO_POLL_ERROR;
                    }
                }
                if ((got[i].flags & EV_ERROR) != 0) {
                    bits = HERO_POLL_ERROR;
                }
                hero_poll_told(slot, key, bits, events, room, &written);
            }
            hero_poll_leave();
#elif defined(HERO_POLL_EPOLL)
            struct epoll_event got[64];
            uint64_t want = room - written < 64 ? room - written : 64;
            int n = epoll_wait(queue, got, (int)want, ms);

            if (n < 0 && errno != EINTR) {
                failed = hero_poll_os_code();
            }
            hero_poll_enter();
            slot->rounds += 1;
            for (int i = 0; i < n; i += 1) {
                uint64_t key = got[i].data.u64;
                uint32_t said = got[i].events;
                int64_t bits = 0;

                if (key == 0) {
                    stopped = stopped || hero_poll_bell_heard();
                    continue;
                }
                if (key == HERO_POLL_DOOR_KEY) {
                    hero_poll_door_drain(door);
                    continue;
                }
                bits |= (said & EPOLLIN) != 0 ? HERO_POLL_READ : 0;
                bits |= (said & EPOLLOUT) != 0 ? HERO_POLL_WRITE : 0;
                bits |= (said & (EPOLLRDHUP | EPOLLHUP)) != 0 ? HERO_POLL_HANGUP : 0;
                bits |= (said & EPOLLERR) != 0 ? HERO_POLL_ERROR : 0;
                hero_poll_told(slot, key, bits, events, room, &written);
            }
            hero_poll_leave();
#else
            int n = 0;

            if (places == 0) {
                /* Nothing to hand the system: the time passes all the same. */
#if defined(_WIN32)
                Sleep((DWORD)ms);
#else
                struct timespec pause = { (time_t)(ms / 1000), (long)(ms % 1000) * 1000000L };

                (void)nanosleep(&pause, NULL);
#endif
            } else {
#if defined(_WIN32)
                n = WSAPoll(&hero_poll_array[base], (ULONG)places, ms);
                if (n == SOCKET_ERROR) {
                    failed = hero_poll_os_code();
                    n = 0;
                }
#else
                n = poll(&hero_poll_array[base], (nfds_t)places, ms);
                if (n < 0) {
                    if (errno != EINTR) {
                        failed = hero_poll_os_code();
                    }
                    n = 0;
                }
#endif
            }
            hero_poll_enter();
            slot->rounds += 1;
            for (uint32_t i = 0; n > 0 && i < places; i += 1) {
                const hero_poll_fd *p = &hero_poll_array[base + i];
                uint32_t entry = hero_poll_array_entry[base + i];
                int64_t bits = 0;

                if (p->revents == 0) {
                    continue;
                }
                if (entry == 0) {
                    stopped = stopped || hero_poll_bell_heard();
                    continue;
                }
                if (entry == HERO_POLL_DOOR_PLACE) {
                    hero_poll_door_drain(door);
                    continue;
                }
                bits |= (p->revents & HERO_POLL_IN) != 0 ? HERO_POLL_READ : 0;
                bits |= (p->revents & HERO_POLL_OUT) != 0 ? HERO_POLL_WRITE : 0;
                bits |= (p->revents & POLLHUP) != 0 ? HERO_POLL_HANGUP : 0;
                bits |= (p->revents & (POLLERR | POLLNVAL)) != 0 ? HERO_POLL_ERROR : 0;
                hero_poll_told(slot, (uint64_t)hero_poll_key(&hero_poll_entries[entry - 1]), bits, events, room, &written);
            }
            hero_poll_leave();
#endif
        }
        hero_poll_enter();
        slot->waiting = false;
        slot->busy -= 1;
        bool closed = slot->closing;
        bool release = closed && slot->busy == 0;
        hero_poll_leave();

        if (release) {
            hero_poll_release(slot);
        }
        if (closed) {
            hero_poll_fail(HERO_POLL_NOT_OPEN, 0);
            return -1;
        }
        if (stopped || hero_net_stop_asked() != 0) {
#if defined(_WIN32)
            /* The stop's own signal may have come before this wait's reset. */
            if (signal != 0) {
                (void)SetEvent((HANDLE)signal);
            }
#endif
            hero_poll_fail(HERO_POLL_STOPPED, 0);
            return -1;
        }
        if (failed != 0) {
            hero_poll_fail(HERO_POLL_OTHER, failed);
            return -1;
        }
        if (written > 0) {
#if defined(_WIN32)
            /* The event stays set while an entry told may still be ready: a
             * socket is told again until the program reads it, and a
             * registration notices a read's end and no more. */
            if (signal != 0) {
                (void)SetEvent((HANDLE)signal);
            }
#else
            if (shown && ready_left) {
                /* The descriptor stays readable while a mailbox told may
                 * still hold a value: the program may take one of many. */
                hero_poll_ring(slot);
            }
#endif
            hero_poll_clear();
            return (int64_t)written;
        }
        now = hero_poll_now();
        if (ends >= 0 && (now < 0 || now >= ends)) {
            hero_poll_clear();
            return 0;
        }
        (void)queue;
    }
}

#if defined(_WIN32)
int64_t hero_poll_ready(hero_poll_set *set) {
    int64_t kind = HERO_POLL_OK;
    hero_poll_slot *slot = NULL;
    uint32_t places = 0;
    uint32_t base = 0;
    uint32_t mailboxes = 0;
    uint32_t files = 0;
    uintptr_t event = 0;

    hero_poll_enter();
    slot = hero_poll_find(set);
    if (slot == NULL) {
        kind = HERO_POLL_NOT_OPEN;
    } else if (slot->waiting) {
        kind = HERO_POLL_INVALID;
    } else {
        slot->waiting = true;
        slot->busy += 1;
        places = hero_poll_fill(slot);
        base = slot->base;
        mailboxes = slot->mailboxes;
        files = slot->files;
        event = hero_poll_event_of(slot);
    }
    hero_poll_leave();

    if (kind != HERO_POLL_OK) {
        hero_poll_fail(kind, 0);
        return -1;
    }
    /* Reset first and asked after, so a notice that comes between sets it again
     * and the next wait of a loop on the handle returns for it; once the stop
     * is asked it is left set, as no wait resets it either. */
    bool stop = hero_net_stop_asked() != 0;

    if (event != 0 && !stop) {
        (void)ResetEvent((HANDLE)event);
    }
    int64_t ready = stop || files != 0 ? 1 : 0;
    int64_t failed = 0;

    if (ready == 0) {
        int64_t one[HERO_POLL_EVENT_NUMBERS];
        bool left = false;

        ready = hero_poll_mailboxes_told(mailboxes, one, 1, 0, &left) > 0 ? 1 : 0;
    }
    if (ready == 0 && places > 0) {
        int n = WSAPoll(&hero_poll_array[base], (ULONG)places, 0);

        if (n == SOCKET_ERROR) {
            failed = hero_poll_os_code();
        }
        /* The stop's bell and the doorbell are not entries: the stop was asked
         * above, and a ring is asked of the mailboxes themselves. */
        for (uint32_t i = 0; n > 0 && i < places && ready == 0; i += 1) {
            uint32_t entry = hero_poll_array_entry[base + i];

            if (entry != 0 && entry != HERO_POLL_DOOR_PLACE && hero_poll_array[base + i].revents != 0) {
                ready = 1;
            }
        }
    }
    hero_poll_enter();
    slot->waiting = false;
    slot->busy -= 1;
    bool closed = slot->closing;
    bool release = closed && slot->busy == 0;
    hero_poll_leave();

    if (release) {
        hero_poll_release(slot);
    }
    if (closed) {
        hero_poll_fail(HERO_POLL_NOT_OPEN, 0);
        return -1;
    }
    if (failed != 0) {
        hero_poll_fail(HERO_POLL_OTHER, failed);
        return -1;
    }
    hero_poll_clear();
    return ready;
}
#else
int64_t hero_poll_ready(hero_poll_set *set) {
    hero_poll_enter();
    bool open = hero_poll_find(set) != NULL;
    hero_poll_leave();

    hero_poll_fail(open ? HERO_POLL_UNSUPPORTED : HERO_POLL_NOT_OPEN, 0);
    return -1;
}
#endif

int64_t hero_poll_last_error(void) {
    return hero_poll_kind;
}

int64_t hero_poll_last_os_error(void) {
    return hero_poll_os;
}
