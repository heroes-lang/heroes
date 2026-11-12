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
 */

#include "hero_net.h"

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
#include <pthread.h>
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

#define HERO_NET_SLOTS 65536u
/* The most one call sends or receives, which Windows' `int` counts can say. */
#define HERO_NET_CHUNK ((uint64_t)INT32_MAX)

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

/* A call done with the slot it took; the last call out of a slot closed
 * meanwhile closes its descriptor. */
static void hero_net_done(hero_net_slot *slot) {
    hero_net_fd last = HERO_NET_NO_FD;

    hero_net_enter();
    slot->busy -= 1;
    if (slot->closing && slot->busy == 0) {
        last = slot->fd;
        hero_net_release(slot);
    }
    hero_net_leave();

    if (last != HERO_NET_NO_FD) {
        (void)hero_net_close_fd(last);
    }
}

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

/* One address `getaddrinfo` gave, tried: a descriptor listening on it or
 * connected to it, or NO_FD with the platform's number in `code`. */
static hero_net_fd hero_net_try(const struct addrinfo *at, bool listening, int64_t *code) {
    hero_net_fd fd = socket(at->ai_family, at->ai_socktype, at->ai_protocol);

    if (fd == HERO_NET_NO_FD) {
        *code = hero_net_os_code();
        return HERO_NET_NO_FD;
    }
    if (!hero_net_ready(fd)) {
        *code = hero_net_os;
        (void)hero_net_close_fd(fd);
        return HERO_NET_NO_FD;
    }
    int failed = 0;

    if (listening) {
        int one = 1;
#if defined(_WIN32)
        /* Windows' SO_REUSEADDR lets another socket take the port from under
         * this one; its exclusive use is POSIX's plain bind. */
        failed = setsockopt(fd, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (const char *)&one, (hero_net_len)sizeof one);
#else
        /* A port a closed listener left in TIME_WAIT is taken again at once. */
        failed = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, (hero_net_len)sizeof one);
#endif
        if (failed == 0) {
            failed = bind(fd, at->ai_addr, (hero_net_len)at->ai_addrlen);
        }
        if (failed == 0) {
            failed = listen(fd, SOMAXCONN);
        }
    } else {
        /* Not asked again after a signal: the connection then goes on
         * without the call, and a second `connect` would say so. */
        failed = connect(fd, at->ai_addr, (hero_net_len)at->ai_addrlen);
    }
    if (failed != 0) {
        *code = hero_net_os_code();
        (void)hero_net_close_fd(fd);
        return HERO_NET_NO_FD;
    }
    return fd;
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

    for (const struct addrinfo *at = found; at != NULL && fd == HERO_NET_NO_FD; at = at->ai_next) {
        fd = hero_net_try(at, listening, &code);
    }
    freeaddrinfo(found);

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

    do {
        got = accept(fd, NULL, NULL);
        code = got == HERO_NET_NO_FD ? hero_net_os_code() : 0;
    } while (got == HERO_NET_NO_FD && hero_net_interrupted(code));
    hero_net_done(slot);

    if (got == HERO_NET_NO_FD) {
        hero_net_failed_with(code);
        return NULL;
    }
    if (!hero_net_ready(got)) {
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

    hero_net_done(slot);
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

/* One send or one receive of at most a chunk, asked again after a signal:
 * the count, or -1 with the platform's number in `code`. */
static int64_t hero_net_move(hero_net_fd fd, void *bytes, uint64_t count, bool sending, int64_t *code) {
    uint64_t n = count > HERO_NET_CHUNK ? HERO_NET_CHUNK : count;
    int64_t moved = -1;

    do {
#if defined(_WIN32)
        moved = sending ? (int64_t)send(fd, (const char *)bytes, (int)n, HERO_NET_SEND_FLAGS)
                        : (int64_t)recv(fd, (char *)bytes, (int)n, 0);
#else
        moved = sending ? (int64_t)send(fd, bytes, (size_t)n, HERO_NET_SEND_FLAGS)
                        : (int64_t)recv(fd, bytes, (size_t)n, 0);
#endif
        *code = moved < 0 ? hero_net_os_code() : 0;
    } while (moved < 0 && hero_net_interrupted(*code));
    return moved;
}

static int64_t hero_net_carry(hero_net_socket *s, void *bytes, uint64_t count, bool sending) {
    hero_net_fd fd = HERO_NET_NO_FD;
    hero_net_slot *slot = hero_net_take(s, &fd);

    if (slot == NULL) {
        return -1;
    }
    if (bytes == NULL && count > 0) {
        hero_net_done(slot);
        hero_net_fail(HERO_NET_INVALID, 0);
        return -1;
    }
    int64_t code = 0;
    int64_t moved = count == 0 ? 0 : hero_net_move(fd, bytes, count, sending, &code);

    hero_net_done(slot);
    if (moved < 0) {
        hero_net_failed_with(code);
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
