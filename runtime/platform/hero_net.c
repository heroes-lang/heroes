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
 * it uses the descriptor; a close meanwhile, from another thread, shuts the
 * socket down so a waiting call returns, and the last call to leave closes
 * the descriptor. A descriptor is never closed while a call may still pass
 * its number to the system, which would then reach whatever socket the
 * system gave that number next.
 *
 * AND A CALL A CLOSE ENDED SAYS SO: `HERO_NET_NOT_OPEN`, never a peer's end
 * of stream nor a connection. Shutting a connection down ends a `recv`
 * waiting on it with 0 (11 to 13 ms after the close on this Mac and in
 * Linux arm64, measured 2026-11-12), which read as the peer's close; and
 * shutting a LISTENER down woke nothing on Darwin, where the `accept` went on
 * waiting and took a connection made after the close (sitting 246's
 * ffi-pragmatist, reproduced the same day), while on Linux it woke with
 * `EINVAL`, read as `HERO_NET_OTHER`. And on Windows a shutdown did not end
 * a `recv` another thread waited in at all (the box, clang 23.1.1, the same
 * day: still waiting 3 s after the close).
 *
 * SO NO CALL BLOCKS IN THE SYSTEM, ON ANY PLATFORM, AND NONE RESTS ON WHAT A
 * SHUTDOWN DOES THERE. Every socket the table holds is non-blocking; `send`,
 * `recv` and `accept` each try first, and where the system says it would
 * block, wait for the readiness they need in slices of `HERO_NET_SLICE_MS`
 * (50 ms), by `poll` (`WSAPoll` on Windows), asking between slices whether
 * the program closed the socket. So a close ends a call another thread waits
 * in within about one slice, and a call that ends on a slot closed meanwhile
 * answers `HERO_NET_NOT_OPEN`, a connection an `accept` took after the close
 * closed again. A wait has no other bound: a `recv` waits for bytes, an
 * `accept` for a connection and a `send` for room, as long as they take,
 * one wakeup a slice. `connect` asks for its connection on a socket already
 * non-blocking and waits for it the same way, writable or failed: no handle
 * names that socket yet, so no close can end the wait, the stop below can,
 * and a connection nobody answers ends at the system's own timeout.
 *
 * THE STOP IS THE SAME WAIT (panel 246's R4, the ffi-pragmatist's mechanism;
 * `hero_net.h` says what it promises). `hero_net_catch_stop` makes a pipe
 * and takes SIGINT and SIGTERM, or Windows' console events. The first of
 * them counts itself in `hero_net_stop_asks` and writes one byte down the
 * pipe, which nobody ever reads, so the pipe's read end stays readable from
 * then on and every `poll` watching it beside a socket wakes at once, on
 * whichever thread the system ran the handler. Each wait asks the count
 * between slices as well, and on Windows, where `WSAPoll` watches sockets
 * alone, that is how a wait learns of the stop, within a slice. No code of
 * the program runs in the handler: it is this file's C, and it touches
 * lock-free atomics, `getpid`, `write`, `sigaction` and `kill`, each safe in
 * a signal (C11 7.14.1.1p5; POSIX's list of async-signal-safe functions); a
 * Heroes function there that allocates kills its program (the sitting's
 * critic, item 12).
 *
 * A SEND MAY SEND FEWER BYTES THAN IT WAS HANDED, AND SAYS HOW MANY: on a
 * non-blocking socket the system takes what its buffer has room for and the
 * call answers that count at once, waiting in slices only while the buffer
 * has no room at all; the caller sends the rest (`hero_net.h`).
 */

#include "hero_net.h"

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
#endif

/* A handle carries a slot and a life in one pointer's 64 bits. */
_Static_assert(sizeof(void *) == 8 && sizeof(uintptr_t) == 8, "a socket handle carries its slot and its life in 64 bits");
/* The stop's handler reads and writes an `_Atomic int`, which C allows in a
 * signal only where it is lock-free (C11 7.14.1.1p5). */
_Static_assert(ATOMIC_INT_LOCK_FREE == 2, "the stop's handler touches lock-free atomics alone");

#define HERO_NET_SLOTS 65536u
/* The most one call sends or receives, which Windows' `int` counts can say. */
#define HERO_NET_CHUNK ((uint64_t)INT32_MAX)
/* How long a waiting call sleeps before it asks again whether its socket was
 * closed or the stop asked: either ends it within about this. */
#define HERO_NET_SLICE_MS 50
/* How long Windows' console handler holds the process after the console
 * closed, so `main` can return first: under the 5,000 ms after which the
 * system ends the process anyway (HandlerRoutine's page). */
#define HERO_NET_CLOSING_MS 4500

typedef struct {
    hero_net_fd fd;
    /* The life the slot holds or last held, which its handle names; a slot
     * whose life reached the largest is never issued again. */
    uint32_t life;
    /* Calls in flight on its descriptor. */
    uint32_t busy;
    /* The index plus one of the next slot given back, 0 for none. */
    uint32_t next;
    /* It holds a socket the program has not closed. */
    bool held;
    /* The program closed it while a call was in flight: the last one closes
     * the descriptor. */
    bool closing;
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

/* Wakes a call waiting on the socket, before the descriptor can close. */
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

static hero_net_socket *hero_net_handle(uint32_t index, uint32_t life) {
    return (hero_net_socket *)(uintptr_t)(((uint64_t)life << 32) | ((uint64_t)index + 1));
}

/* The slot `s` names while it holds the life `s` was issued for; NULL for
 * any other value. Under the lock. */
static hero_net_slot *hero_net_find(hero_net_socket *s) {
    uint64_t value = (uint64_t)(uintptr_t)s;
    uint64_t low = value & UINT64_C(0xffffffff);

    if (low == 0 || low > hero_net_fresh) {
        return NULL;
    }
    hero_net_slot *slot = &hero_net_slots[low - 1];

    if (!slot->held || slot->closing || slot->life != (uint32_t)(value >> 32)) {
        return NULL;
    }
    return slot;
}

/* The slot given back, its descriptor forgotten. Under the lock. */
static void hero_net_release(hero_net_slot *slot) {
    slot->held = false;
    slot->closing = false;
    slot->fd = HERO_NET_NO_FD;

    if (slot->life < UINT32_MAX) {
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
        slot->next = 0;
        slot->held = true;
        slot->closing = false;
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

/* A call done with the slot it took, answering whether the program closed it
 * meanwhile; the last call out of a slot closed meanwhile closes its
 * descriptor. */
static bool hero_net_done(hero_net_slot *slot) {
    hero_net_fd last = HERO_NET_NO_FD;
    bool closed = false;

    hero_net_enter();
    slot->busy -= 1;
    closed = slot->closing;
    if (slot->closing && slot->busy == 0) {
        last = slot->fd;
        hero_net_release(slot);
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

/* -- the stop (panel 246's R4) ------------------------------------------- */

/* Whether `hero_net_catch_stop` has taken the signals, 1 once it has: written
 * under `hero_net_lock`, so two first calls arm once. */
static _Atomic int hero_net_stop_armed;
/* The signals, or Windows' console events, that came since: 0 none, 1 the
 * program stops, more ends it. The one truth of the stop: the pipe below is
 * how a wait hears of it at once, and this is what every wait asks. */
static _Atomic int hero_net_stop_asks;

/* Whether the stop was asked. */
static bool hero_net_stopping(void) {
    return atomic_load(&hero_net_stop_asks) > 0;
}

#if defined(_WIN32)
/* Windows runs this on a thread of its own it makes for the event
 * (HandlerRoutine's page), so it touches the count alone. The first event
 * asks the stop and answers TRUE, handled; a second answers FALSE, and the
 * system's own handler after it ends the process as the event would have.
 * The console closing ends the process when this returns, or 5 s after the
 * close: it waits first, so `main` returns, and ends the process sooner. */
static BOOL WINAPI hero_net_on_console(DWORD event) {
    if (event != CTRL_C_EVENT && event != CTRL_BREAK_EVENT && event != CTRL_CLOSE_EVENT) {
        return FALSE;
    }
    if (atomic_fetch_add(&hero_net_stop_asks, 1) > 0) {
        return FALSE;
    }
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
/* The pipe's two ends, -1 until the stop is armed: the handler writes down
 * `hero_net_stop_out`, every wait watches `hero_net_stop_in`. */
static _Atomic int hero_net_stop_in = -1;
static _Atomic int hero_net_stop_out = -1;
/* The process that armed the stop. A child forked before it runs a program
 * shares the pipe and the handler until its `exec`, and a signal reaching it
 * there must end it as the signal would, never stop its parent. */
static _Atomic int hero_net_stop_owner = -1;

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

/* The pipe made, both ends close-on-exec and non-blocking, published, then
 * the two signals taken; -1 with the system's number where any of it is
 * refused, nothing left taken. Under the lock. */
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

/* How one slice of a wait ended. */
typedef enum {
    /* The system failed the wait, its number in `code`. */
    HERO_NET_WAIT_FAILED,
    /* The slice passed with nothing ready. */
    HERO_NET_WAIT_PASSED,
    /* The socket is ready for what the call needs. */
    HERO_NET_WAIT_READY,
    /* The stop was asked, whatever else is ready. */
    HERO_NET_WAIT_STOPPED
} hero_net_waited;

/* One slice of a wait for what a call needs, a connection or bytes to read
 * (`writing` false) or room to send or a connection made, beside the stop's
 * pipe where it is armed. */
static hero_net_waited hero_net_wait(hero_net_fd fd, bool writing, int64_t *code) {
#if defined(_WIN32)
    WSAPOLLFD one = { fd, (short)(writing ? POLLWRNORM : POLLRDNORM), 0 };
    int ready = WSAPoll(&one, 1, HERO_NET_SLICE_MS);
#else
    /* A negative descriptor, the stop unarmed, is one `poll` passes over. */
    struct pollfd two[2] = { { fd, (short)(writing ? POLLOUT : POLLIN), 0 }, { atomic_load(&hero_net_stop_in), POLLIN, 0 } };
    int ready = poll(two, 2, HERO_NET_SLICE_MS);
#endif

    if (ready < 0) {
        *code = hero_net_os_code();
        return HERO_NET_WAIT_FAILED;
    }
    if (hero_net_stopping()) {
        return HERO_NET_WAIT_STOPPED;
    }
    return ready > 0 ? HERO_NET_WAIT_READY : HERO_NET_WAIT_PASSED;
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
 * number in `code`. A listener never blocks: its `accept` waits in slices. */
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

/* A descriptor connected to the address, or NO_FD with the platform's number
 * in `code`, or with `stopped` set where the stop was asked while the
 * connection was being made. The connection is asked for on a non-blocking
 * socket and waited for in slices, writable or failed, the system's own
 * answer read after each (`hero_net_pending_error`). */
static hero_net_fd hero_net_try_connect(const struct addrinfo *at, int64_t *code, bool *stopped) {
    hero_net_fd fd = hero_net_made(at, code);

    if (fd == HERO_NET_NO_FD) {
        return HERO_NET_NO_FD;
    }
    if (connect(fd, at->ai_addr, (hero_net_len)at->ai_addrlen) == 0) {
        return fd;
    }
    *code = hero_net_os_code();
    if (!hero_net_connecting(*code)) {
        (void)hero_net_close_fd(fd);
        return HERO_NET_NO_FD;
    }
    for (;;) {
        hero_net_waited waited = hero_net_stopping() ? HERO_NET_WAIT_STOPPED : hero_net_wait(fd, true, code);

        if (waited == HERO_NET_WAIT_STOPPED) {
            *stopped = true;
            (void)hero_net_close_fd(fd);
            return HERO_NET_NO_FD;
        }
        if (waited == HERO_NET_WAIT_FAILED && !hero_net_interrupted(*code)) {
            (void)hero_net_close_fd(fd);
            return HERO_NET_NO_FD;
        }
        int64_t said = hero_net_pending_error(fd);

        if (said != 0) {
            *code = said;
            (void)hero_net_close_fd(fd);
            return HERO_NET_NO_FD;
        }
        if (waited == HERO_NET_WAIT_READY) {
            return fd;
        }
    }
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
    hero_net_fd fd = HERO_NET_NO_FD;
    int64_t code = 0;
    bool stopped = false;

    for (const struct addrinfo *at = found; at != NULL && fd == HERO_NET_NO_FD && !stopped; at = at->ai_next) {
        fd = listening ? hero_net_try_listen(at, &code) : hero_net_try_connect(at, &code, &stopped);
    }
    freeaddrinfo(found);

    if (stopped) {
        hero_net_fail(HERO_NET_STOPPED, 0);
        return NULL;
    }
    if (fd == HERO_NET_NO_FD) {
        hero_net_failed_with(code);
        return NULL;
    }
    return hero_net_keep(fd);
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

    /* In slices, asking between them whether the listener was closed or the
     * stop asked; the stop is asked before any connection waiting is taken,
     * so a loop of accepts under load still ends. */
    while (!hero_net_closing(slot)) {
        hero_net_waited waited = hero_net_stopping() ? HERO_NET_WAIT_STOPPED : hero_net_wait(fd, false, &code);

        if (waited == HERO_NET_WAIT_STOPPED) {
            stopped = true;
            break;
        }
        if (waited == HERO_NET_WAIT_FAILED && hero_net_interrupted(code)) {
            continue;
        }
        if (waited == HERO_NET_WAIT_FAILED) {
            break;
        }
        if (waited == HERO_NET_WAIT_PASSED) {
            continue;
        }
        got = accept(fd, NULL, NULL);
        code = got == HERO_NET_NO_FD ? hero_net_os_code() : 0;
        if (got != HERO_NET_NO_FD || !hero_net_passing(code)) {
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
 * would wait, waited for in slices, asking between them whether the program
 * closed the socket or the stop was asked. The count, or -1 with the kind in
 * `kind` and the platform's number in `code`, 0 where the layer ended it. */
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
        hero_net_waited waited = hero_net_stopping() ? HERO_NET_WAIT_STOPPED : hero_net_wait(fd, sending, code);

        if (waited == HERO_NET_WAIT_STOPPED) {
            *kind = HERO_NET_STOPPED;
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

    /* Ended by a close meanwhile, the shutdown's 0 or its failure, the call
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
             * last call out closes it, and only after this has returned. */
            slot->closing = true;
            hero_net_shut(slot->fd);
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

int64_t hero_net_last_error(void) {
    return hero_net_kind;
}

int64_t hero_net_last_os_error(void) {
    return hero_net_os;
}
