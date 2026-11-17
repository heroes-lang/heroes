/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ipc.c — communication between the processes of one machine,
 * one member of the archive the compiler puts on every program's link line
 * (M-rebel-library, 2026-11-17; `selfhost/cli/layer.hero` builds, keys and
 * links it). Its header, `hero_ipc.h`, says what each function answers; this
 * file says how each platform gets there. Written from the POSIX pages and
 * Microsoft's Win32 reference, never from another library's source.
 *
 * ITS OWN HEADER FIRST, so a definition with another signature than the
 * header's is `conflicting types` where it is compiled.
 *
 * A HANDLE IS A SLOT OF THE TABLE BELOW, NEVER AN ADDRESS THIS FILE READS
 * THROUGH, as `hero_net.c`'s and `hero_proc.c`'s are: the slot's index and
 * the life it held when it was issued, under the tag of its kind
 * (`HERO_HANDLE_TAG_IPC_*`, `hero_handle_tag.h`), so a listener's handle is
 * never memory's. The table is fixed, 4,096 slots, its static bytes touched
 * only as slots are issued, since the runtime allocates only in
 * `parts/alloc.c`; a program past it is told `HERO_IPC_LIMIT`.
 *
 * A CALL IN FLIGHT KEEPS WHAT ITS SLOT HOLDS. Each call marks its slot busy
 * while it uses the descriptor, the mapping or the system's object; a close
 * meanwhile marks the slot closing, and the last call to leave gives the
 * slot's things back. So no descriptor is closed and no mapping unmapped
 * while a copy or a wait may still reach it.
 *
 * EVERY WAIT IS IN SLICES OF AT MOST 50 MS, and between two slices it asks
 * three things: whether the program asked to stop (`hero_net_stop_asked`,
 * so this member takes `hero_net`'s into a program that takes it), whether
 * another thread closed the handle, and whether the bound passed. What a
 * slice waits on is the system's own: `poll` (`WSAPoll`) on a socket, a
 * `WaitForSingleObject` on a Windows object, `sem_clockwait` on Linux's
 * semaphores. Where the system has no wait with a bound, the call tries
 * again in steps that grow from 0.1 ms to 2 ms: a lock on POSIX (`flock`
 * takes none), and a semaphore on macOS (no `sem_timedwait` there).
 *
 * WHAT THE STEPS COST, measured on 2026-11-17 by a probe calling this file
 * from two processes, twenty hand-overs each, the clock read on both sides:
 * on this Mac (macOS 27.2, arm64), over four runs, a semaphore's post was
 * seen by a waiter 1.0 to 1.8 ms later on average and 3.0 ms at worst, and
 * a wait of 1 s that ended at its bound spent 1.3 to 9.0 ms of CPU, under
 * 1% of a core; a lock's release was seen 1.6 to 2.1 ms later on average
 * and 2.9 ms at worst. In the Linux arm64 container, where the semaphore
 * waits in `sem_clockwait`, the post was seen 0.1 to 0.3 ms later on
 * average, the wait of 1 s spent 1.2 to 1.6 ms of CPU, and the lock, which
 * steps there too, 1.5 to 1.7 ms on average and 3.5 ms at worst (4.8 ms
 * under the sanitizers). A close from another thread ended a waiting
 * accept within one slice (48 to 51 ms on this Mac, 2 to 5 ms there), and
 * `hero_net_ask_stop` a waiting semaphore within 2.6 ms.
 *
 * A CHANNEL IS A STREAM made by `hero_stream_make` over a block of this
 * file's own, `hero_layer_alloc`'s, holding the descriptor (or the pipe's
 * `HANDLE`) and the deadline; `hero_stream.c` calls its `close` once, after
 * the last read or write, so the block is never read after it is freed.
 *
 * NO POINTER INTO SHARED MEMORY LEAVES THIS FILE: a read and a write copy,
 * after the offset and the count are held against the size, and the atomic
 * words are reached by `__atomic` builtins on an address this file computes
 * from a checked offset.
 *
 * A LOCK ON POSIX IS A FILE, `/tmp/heroes-lock-<name>`, LOCKED WHOLE BY
 * `flock`: the system releases it when the process holding it ends, however
 * it ends, which a semaphore (the other route, `sem_open` with a count of
 * one) never does: a holder killed would leave it taken until someone
 * removed it. Its first byte tells whether a holder ended without releasing
 * it: the holder writes `H` there once it holds it and a zero before it
 * releases, so a process that finds `H` on taking it took it from one that
 * died holding it. On Windows a mutex, whose wait answers `WAIT_ABANDONED`
 * for the same thing. A lock is held by one THREAD: the table keeps which,
 * so on both platforms a release by another thread is refused and a thread
 * asking for a lock it holds is refused at once rather than waiting for
 * itself (`flock` would let a second call through the same descriptor
 * through, and a Windows mutex would count it as a second hold).
 *
 * WINDOWS: AF_UNIX sockets (`afunix.h`, Windows 10 1803 and later),
 * named pipes in overlapped mode, `CreateFileMappingW` and `MapViewOfFile`,
 * `CreateSemaphoreW`, `CreateMutexW`. UNRUN on 2026-11-17, the box being off:
 * compiled on this Mac against zig's MinGW-w64 headers, `-Werror`, and
 * owed its runs on the box (the floor's and the face's tests, plain and
 * sanitized, which take the Windows arm on every door above).
 */

#include "hero_ipc.h"

/* The stop every wait asks about. */
#include "hero_net.h"
/* The tags that keep this member's handles apart from every other's. */
#include "hero_handle_tag.h"
/* A channel's block, given back by its stream's close. */
#include "hero_layer_alloc.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#if defined(_WIN32)
#include <winsock2.h>
#include <afunix.h>
#include <windows.h>
/* The library the member needs, named by the member, as `hero_net.c` names
 * it: MSVC's word; a MinGW target links it by `-lws2_32`. */
#if defined(_MSC_VER)
#pragma comment(lib, "ws2_32.lib")
#endif
#else
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>
/* A write to a peer that has gone raises SIGPIPE, which ends the process:
 * Darwin stops it per socket, Linux per call, as `hero_net.c` does. */
#if defined(SO_NOSIGPIPE)
#define HERO_IPC_SEND_FLAGS 0
#elif defined(MSG_NOSIGNAL)
#define HERO_IPC_SEND_FLAGS MSG_NOSIGNAL
#else
#error "hero_ipc.c: this platform stops SIGPIPE neither per socket nor per call; a write to a closed peer would end the program"
#endif
#if defined(__linux__)
/* Linux makes a socket close-on-exec and non-blocking in the call that makes
 * it; glibc declares `accept4` only under `_GNU_SOURCE`, which a member that
 * includes its own header first cannot define. */
int accept4(int fd, struct sockaddr *address, socklen_t *length, int flags);
/* `SO_PEERCRED`'s answer, `struct ucred` in glibc under `_GNU_SOURCE` alone:
 * three numbers in this order (socket(7)). */
typedef struct {
    pid_t pid;
    uid_t uid;
    gid_t gid;
} hero_ipc_ucred;
#if defined(__GLIBC__) && defined(__GLIBC_PREREQ)
#if __GLIBC_PREREQ(2, 30)
/* A semaphore's wait on the monotonic clock (glibc 2.30), declared under
 * `_GNU_SOURCE` alone, as above; a wall clock moved back would make
 * `sem_timedwait`'s slice last as long as it moved. */
#define HERO_IPC_SEM_CLOCKWAIT 1
int sem_clockwait(sem_t *sem, clockid_t clock, const struct timespec *until);
#endif
#endif
#endif
#endif

/* A handle carries its kind's tag, a slot and a life in one pointer's 64
 * bits: `HERO_HANDLE_TAG(tag) | (life << HERO_IPC_INDEX_BITS) | (index + 1)`. */
_Static_assert(sizeof(void *) == 8 && sizeof(uintptr_t) == 8, "a handle carries its slot and its life in 64 bits");
#define HERO_IPC_INDEX_BITS 13
#define HERO_IPC_INDEX_MASK ((UINT64_C(1) << HERO_IPC_INDEX_BITS) - 1)
#define HERO_IPC_LIFE_MOST HERO_HANDLE_LIFE_MOST(HERO_IPC_INDEX_BITS)
_Static_assert((HERO_IPC_LIFE_MOST << HERO_IPC_INDEX_BITS) < (UINT64_C(1) << HERO_HANDLE_TAG_SHIFT), "a life stays below the tag");
#define HERO_IPC_SLOTS 4096u
_Static_assert(HERO_IPC_SLOTS <= HERO_IPC_INDEX_MASK, "an index plus one fits below a handle's life");

/* The kinds a channel's stream hands on are the stream's own numbers. */
_Static_assert(HERO_IPC_INVALID == HERO_STREAM_INVALID && HERO_IPC_NOT_OPEN == HERO_STREAM_NOT_OPEN &&
                   HERO_IPC_NOT_FOUND == HERO_STREAM_NOT_FOUND && HERO_IPC_EXISTS == HERO_STREAM_EXISTS &&
                   HERO_IPC_DENIED == HERO_STREAM_DENIED && HERO_IPC_LIMIT == HERO_STREAM_LIMIT &&
                   HERO_IPC_CLOSED_PEER == HERO_STREAM_CLOSED_PEER && HERO_IPC_UNSUPPORTED == HERO_STREAM_UNSUPPORTED &&
                   HERO_IPC_OTHER == HERO_STREAM_OTHER && HERO_IPC_TIMED_OUT == HERO_STREAM_TIMED_OUT &&
                   HERO_IPC_STOPPED == HERO_STREAM_STOPPED,
               "a kind both headers have is one number");

/* What a slot holds. */
#define HERO_IPC_TYPE_NONE 0
#define HERO_IPC_TYPE_LISTENER 1
#define HERO_IPC_TYPE_MEMORY 2
#define HERO_IPC_TYPE_SEMAPHORE 3
#define HERO_IPC_TYPE_LOCK 4

/* The longest name a slot keeps with its zero: a path a socket's address
 * holds (108 bytes on Linux and Windows), or an object's name. */
#define HERO_IPC_NAME_ROOM 112

/* The longest slice of a wait, between which the stop, a close and the
 * bound are asked again. */
#define HERO_IPC_SLICE_NS INT64_C(50000000)
/* The first and the longest step of a wait that tries again. */
#define HERO_IPC_STEP_FIRST_NS INT64_C(100000)
#define HERO_IPC_STEP_MOST_NS INT64_C(2000000)

/* A deadline no wait reaches: `HERO_IPC_FOREVER`'s. */
#define HERO_IPC_NEVER INT64_MAX

/* A lock's first byte while a holder holds it. */
#define HERO_IPC_HELD_MARK 'H'

/* Where a lock's file stands on POSIX, before its name. */
#define HERO_IPC_LOCK_PREFIX "/tmp/heroes-lock-"

typedef struct {
    /* The life the slot holds or last held, which its handle names. */
    uint64_t life;
    /* Calls in flight on it. */
    uint32_t busy;
    /* The index plus one of the next slot given back, 0 for none. */
    uint32_t next;
    /* `HERO_IPC_TYPE_*`, `HERO_IPC_TYPE_NONE` while free. */
    uint8_t type;
    /* The program closed it while a call was in flight: the last one gives
     * its things back. */
    bool closing;
    /* A lock: held by the thread `holder` names. */
    bool held;
    /* A pipe listener: an accept is in flight. */
    bool accepting;
    /* A listener's `HERO_IPC_SPACE_*`. */
    int64_t space;
    /* A listener's path or name, a lock's or a pipe's name. */
    char name[HERO_IPC_NAME_ROOM];
    /* Memory: where it is mapped, and how many bytes. */
    unsigned char *base;
    uint64_t length;
#if defined(_WIN32)
    /* A listener's socket. */
    SOCKET socket;
    /* A pipe listener's instance waiting for a client, a mapping, a
     * semaphore or a mutex. */
    HANDLE handle;
    DWORD holder;
#else
    /* A listener's socket, or a lock's file. */
    int fd;
    /* A path listener's socket file, as `stat` read it after the bind. */
    dev_t device;
    ino_t inode;
    sem_t *semaphore;
    pthread_t holder;
#endif
} hero_ipc_slot;

/* The table and its free list, one object under one lock. */
typedef struct {
    hero_ipc_slot slots[HERO_IPC_SLOTS];
    /* Every slot at or above it has never been issued. */
    uint32_t fresh;
    /* The index plus one of the last slot given back, 0 for none. */
    uint32_t given_back;
} hero_ipc_slots;

static hero_ipc_slots hero_ipc_table;

static _Thread_local int64_t hero_ipc_kind;
static _Thread_local int64_t hero_ipc_os;

#if defined(_WIN32)
static SRWLOCK hero_ipc_lock_of_table = SRWLOCK_INIT;
/* `WSAStartup` asked once for the process. */
static INIT_ONCE hero_ipc_winsock_once = INIT_ONCE_STATIC_INIT;
static int hero_ipc_winsock_said = -1;
#else
static pthread_mutex_t hero_ipc_lock_of_table = PTHREAD_MUTEX_INITIALIZER;
#endif

static void hero_ipc_enter(void) {
#if defined(_WIN32)
    AcquireSRWLockExclusive(&hero_ipc_lock_of_table);
#else
    (void)pthread_mutex_lock(&hero_ipc_lock_of_table);
#endif
}

static void hero_ipc_leave(void) {
#if defined(_WIN32)
    ReleaseSRWLockExclusive(&hero_ipc_lock_of_table);
#else
    (void)pthread_mutex_unlock(&hero_ipc_lock_of_table);
#endif
}

/* -- the last error, per thread ------------------------------------------- */

static void hero_ipc_fail(int64_t kind, int64_t os) {
    hero_ipc_kind = kind;
    hero_ipc_os = os;
}

static void hero_ipc_clear(void) {
    hero_ipc_fail(HERO_IPC_OK, 0);
}

int64_t hero_ipc_last_error(void) {
    return hero_ipc_kind;
}

int64_t hero_ipc_last_os_error(void) {
    return hero_ipc_os;
}

/* The kind a platform's number is, the same kind on every platform. */
static int64_t hero_ipc_kind_of(int64_t code) {
    switch (code) {
#if defined(_WIN32)
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
        return HERO_IPC_NOT_FOUND;
    case ERROR_ALREADY_EXISTS:
    case ERROR_FILE_EXISTS:
    case ERROR_PIPE_BUSY:
    case WSAEADDRINUSE:
        return HERO_IPC_EXISTS;
    case ERROR_ACCESS_DENIED:
    case WSAEACCES:
        return HERO_IPC_DENIED;
    case WSAECONNREFUSED:
        return HERO_IPC_REFUSED;
    case ERROR_BROKEN_PIPE:
    case ERROR_NO_DATA:
    case ERROR_PIPE_NOT_CONNECTED:
    case WSAECONNRESET:
    case WSAECONNABORTED:
    case WSAESHUTDOWN:
        return HERO_IPC_CLOSED_PEER;
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
    case ERROR_NO_SYSTEM_RESOURCES:
    case ERROR_COMMITMENT_LIMIT:
    case ERROR_TOO_MANY_OPEN_FILES:
    case ERROR_DISK_FULL:
    case WSAEMFILE:
    case WSAENOBUFS:
        return HERO_IPC_LIMIT;
    case ERROR_INVALID_NAME:
    case ERROR_BAD_PATHNAME:
    case ERROR_FILENAME_EXCED_RANGE:
    case ERROR_INVALID_PARAMETER:
    case WSAEINVAL:
    case WSAEFAULT:
        return HERO_IPC_INVALID;
    case ERROR_NOT_SUPPORTED:
    case WSAEAFNOSUPPORT:
    case WSAEPROTONOSUPPORT:
    case WSAESOCKTNOSUPPORT:
    case WSAEOPNOTSUPP:
        return HERO_IPC_UNSUPPORTED;
    case ERROR_TIMEOUT:
    case WSAETIMEDOUT:
        return HERO_IPC_TIMED_OUT;
#else
    case ENOENT:
    case ENOTDIR:
        return HERO_IPC_NOT_FOUND;
    case EEXIST:
    case EADDRINUSE:
        return HERO_IPC_EXISTS;
    case EACCES:
    case EPERM:
    case ELOOP:
        return HERO_IPC_DENIED;
    case ECONNREFUSED:
        return HERO_IPC_REFUSED;
    case EPIPE:
    case ECONNRESET:
    case ENOTCONN:
        return HERO_IPC_CLOSED_PEER;
    case EMFILE:
    case ENFILE:
    case ENOMEM:
    case ENOSPC:
    case ENOBUFS:
    case EFBIG:
#if defined(EOVERFLOW)
    case EOVERFLOW:
#endif
        return HERO_IPC_LIMIT;
    case EINVAL:
    case ENAMETOOLONG:
        return HERO_IPC_INVALID;
    case ENOSYS:
    case EAFNOSUPPORT:
    case EPROTONOSUPPORT:
#if defined(ENOTSUP)
    case ENOTSUP:
#endif
#if defined(EOPNOTSUPP) && (!defined(ENOTSUP) || EOPNOTSUPP != ENOTSUP)
    case EOPNOTSUPP:
#endif
        return HERO_IPC_UNSUPPORTED;
    case ETIMEDOUT:
        return HERO_IPC_TIMED_OUT;
#endif
    default:
        return HERO_IPC_OTHER;
    }
}

/* The failure of the platform's last call, by its own number. */
static void hero_ipc_failed_with(int64_t code) {
    hero_ipc_fail(hero_ipc_kind_of(code), code);
}

#if defined(_WIN32)
static void hero_ipc_failed_system(void) {
    hero_ipc_failed_with((int64_t)GetLastError());
}

static void hero_ipc_failed_socket(void) {
    hero_ipc_failed_with((int64_t)WSAGetLastError());
}
#else
static void hero_ipc_failed_system(void) {
    hero_ipc_failed_with((int64_t)errno);
}
#endif

/* The stream's kind for one of this member's, which a channel's operation
 * owes `hero_stream_set_error` before it answers -1: the same number where
 * both headers have it, `HERO_STREAM_OTHER` for the two this one alone has. */
static int64_t hero_ipc_stream_kind(int64_t kind) {
    switch (kind) {
    case HERO_IPC_REFUSED:
    case HERO_IPC_NOT_HELD:
    case HERO_IPC_OK:
        return HERO_STREAM_OTHER;
    default:
        return kind;
    }
}

/* This member's kind for the stream's last failure, where a call reached a
 * stream through `hero_stream.h`. */
static void hero_ipc_failed_as_stream(void) {
    int64_t kind = hero_stream_last_error();

    switch (kind) {
    case HERO_STREAM_NOT_A_DIRECTORY:
    case HERO_STREAM_IS_A_DIRECTORY:
    case HERO_STREAM_NOT_SEEKABLE:
    case HERO_STREAM_OK:
        hero_ipc_fail(HERO_IPC_OTHER, hero_stream_last_os_error());
        break;
    default:
        hero_ipc_fail(kind, hero_stream_last_os_error());
        break;
    }
}

/* -- the clock and the bound of a wait ------------------------------------ */

/* Nanoseconds on a clock that never goes back, or -1 where it cannot be read
 * or does not fit 64 bits. */
static int64_t hero_ipc_now(void) {
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

    if (clock_gettime(CLOCK_MONOTONIC, &t) != 0 || t.tv_sec < 0 || t.tv_nsec < 0) {
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

/* The deadline `within_ms` from now on `hero_ipc_now`'s clock into
 * `*until`, `HERO_IPC_NEVER` for `HERO_IPC_FOREVER` or a bound past the
 * clock's end; false with `HERO_IPC_INVALID` for another negative bound, or
 * the clock's failure. */
static bool hero_ipc_deadline(int64_t within_ms, int64_t *until) {
    int64_t span = 0;
    int64_t now = 0;

    if (within_ms == HERO_IPC_FOREVER) {
        *until = HERO_IPC_NEVER;
        return true;
    }
    if (within_ms < 0) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return false;
    }
    now = hero_ipc_now();
    if (now < 0) {
        hero_ipc_fail(HERO_IPC_OTHER, 0);
        return false;
    }
    if (__builtin_mul_overflow(within_ms, INT64_C(1000000), &span) || __builtin_add_overflow(now, span, until)) {
        *until = HERO_IPC_NEVER;
    }
    return true;
}

/* How long the next slice of a wait bounded by `until` may last, in
 * nanoseconds, at most `HERO_IPC_SLICE_NS`; 0 where the bound has passed
 * (or the clock cannot be read, which ends the wait rather than letting it
 * run without one). */
static int64_t hero_ipc_slice(int64_t until) {
    int64_t now = hero_ipc_now();

    if (now < 0 || now >= until) {
        return 0;
    }
    if (until - now < HERO_IPC_SLICE_NS) {
        return until - now;
    }
    return HERO_IPC_SLICE_NS;
}

/* A slice's nanoseconds as the milliseconds `poll` and the Windows waits
 * take, rounded up so a wait never ends before its slice. */
static int hero_ipc_slice_ms(int64_t ns) {
    return (int)((ns + INT64_C(999999)) / INT64_C(1000000));
}

/* Why a wait bounded by `until` must end now: `HERO_IPC_STOPPED` where the
 * program asked to stop, `HERO_IPC_TIMED_OUT` where the bound passed, else
 * `HERO_IPC_OK`. */
static int64_t hero_ipc_why_end(int64_t until) {
    if (hero_net_stop_asked() != 0) {
        return HERO_IPC_STOPPED;
    }
    if (hero_ipc_slice(until) == 0) {
        return HERO_IPC_TIMED_OUT;
    }
    return HERO_IPC_OK;
}

#if !defined(_WIN32)
/* Sleeps `*step` nanoseconds, at most what is left before `until` and at
 * most a slice, then doubles `*step` up to `HERO_IPC_STEP_MOST_NS`: the wait
 * of a call the system gives no bound. */
static void hero_ipc_step(int64_t until, int64_t *step) {
    int64_t left = hero_ipc_slice(until);
    int64_t span = *step < left ? *step : left;
    struct timespec t;

    if (span > 0) {
        t.tv_sec = (time_t)(span / INT64_C(1000000000));
        t.tv_nsec = (long)(span % INT64_C(1000000000));
        (void)nanosleep(&t, NULL);
    }
    if (*step < HERO_IPC_STEP_MOST_NS) {
        *step = *step * 2 > HERO_IPC_STEP_MOST_NS ? HERO_IPC_STEP_MOST_NS : *step * 2;
    }
}
#endif

/* -- names ------------------------------------------------------------------ */

/* Whether `name` is an object's name by `hero_ipc.h`'s rule, of at most
 * `most` bytes: ASCII letters, digits, `.`, `_` and `-`, the first no `.`. */
static bool hero_ipc_named(const char *name, size_t most) {
    size_t count = 0;

    if (name == NULL || name[0] == '\0' || name[0] == '.') {
        return false;
    }
    for (; name[count] != '\0'; count++) {
        char c = name[count];
        bool letter = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
        bool digit = c >= '0' && c <= '9';

        if (count >= most || !(letter || digit || c == '.' || c == '_' || c == '-')) {
            return false;
        }
    }
    return true;
}

/* `prefix` then `name` into `out` of `room` bytes, with its zero; false where
 * it does not fit. */
static bool hero_ipc_joined(char *out, size_t room, const char *prefix, const char *name) {
    size_t first = strlen(prefix);
    size_t second = strlen(name);

    if (first + second + 1 > room) {
        return false;
    }
    memcpy(out, prefix, first);
    memcpy(out + first, name, second + 1);
    return true;
}

#if defined(_WIN32)
/* `text`, UTF-8, as UTF-16 into `out` of `room` units with its zero; false
 * with `HERO_IPC_INVALID` where it is no UTF-8 or does not fit. */
static bool hero_ipc_wide(const char *text, wchar_t *out, int room) {
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, out, room) == 0) {
        hero_ipc_fail(HERO_IPC_INVALID, (int64_t)GetLastError());
        return false;
    }
    return true;
}

/* Starts the process's sockets, once; false with the failure named. */
static BOOL CALLBACK hero_ipc_winsock_start(PINIT_ONCE once, PVOID parameter, PVOID *context) {
    WSADATA data;

    (void)once;
    (void)parameter;
    (void)context;
    hero_ipc_winsock_said = WSAStartup(MAKEWORD(2, 2), &data);
    return TRUE;
}

static bool hero_ipc_winsock(void) {
    if (!InitOnceExecuteOnce(&hero_ipc_winsock_once, hero_ipc_winsock_start, NULL, NULL) || hero_ipc_winsock_said != 0) {
        hero_ipc_fail(HERO_IPC_OTHER, (int64_t)hero_ipc_winsock_said);
        return false;
    }
    return true;
}
#endif

/* -- the table ------------------------------------------------------------ */

static uint64_t hero_ipc_tag_of(uint8_t type) {
    switch (type) {
    case HERO_IPC_TYPE_LISTENER:
        return HERO_HANDLE_TAG(HERO_HANDLE_TAG_IPC_LISTENER);
    case HERO_IPC_TYPE_MEMORY:
        return HERO_HANDLE_TAG(HERO_HANDLE_TAG_IPC_MEMORY);
    case HERO_IPC_TYPE_SEMAPHORE:
        return HERO_HANDLE_TAG(HERO_HANDLE_TAG_IPC_SEMAPHORE);
    default:
        return HERO_HANDLE_TAG(HERO_HANDLE_TAG_IPC_LOCK);
    }
}

static void *hero_ipc_handle(hero_ipc_slot *slot) {
    uint64_t index = (uint64_t)(slot - hero_ipc_table.slots);

    return (void *)(uintptr_t)(hero_ipc_tag_of(slot->type) | (slot->life << HERO_IPC_INDEX_BITS) | (index + 1));
}

/* The slot `handle` names while it holds the life the handle was issued for,
 * is of `type` and is not closing; NULL for any other value. Under the lock. */
static hero_ipc_slot *hero_ipc_find(const void *handle, uint8_t type) {
    uint64_t value = (uint64_t)(uintptr_t)handle;
    uint64_t low = value & HERO_IPC_INDEX_MASK;

    if ((value & HERO_HANDLE_TAG_MASK) != hero_ipc_tag_of(type) || low == 0 || low > hero_ipc_table.fresh) {
        return NULL;
    }
    hero_ipc_slot *slot = &hero_ipc_table.slots[low - 1];

    if (slot->type != type || slot->closing || slot->life != (value & ~HERO_HANDLE_TAG_MASK) >> HERO_IPC_INDEX_BITS) {
        return NULL;
    }
    return slot;
}

/* A free slot, cleared and given `type` and a new life; NULL with
 * `HERO_IPC_LIMIT` where the table is full. Under the lock. */
static hero_ipc_slot *hero_ipc_issue(uint8_t type) {
    hero_ipc_slot *slot = NULL;

    if (hero_ipc_table.given_back != 0) {
        slot = &hero_ipc_table.slots[hero_ipc_table.given_back - 1];
        hero_ipc_table.given_back = slot->next;
    } else if (hero_ipc_table.fresh < HERO_IPC_SLOTS) {
        slot = &hero_ipc_table.slots[hero_ipc_table.fresh];
        hero_ipc_table.fresh++;
    } else {
        hero_ipc_fail(HERO_IPC_LIMIT, 0);
        return NULL;
    }
    uint64_t life = slot->life + 1;

    memset(slot, 0, sizeof *slot);
    slot->life = life;
    slot->type = type;
#if defined(_WIN32)
    slot->socket = INVALID_SOCKET;
    slot->handle = NULL;
#else
    slot->fd = -1;
#endif
    return slot;
}

/* The slot given back, nothing of what it held kept: its things were given
 * back first. Under the lock. */
static void hero_ipc_free(hero_ipc_slot *slot) {
    slot->type = HERO_IPC_TYPE_NONE;
    slot->closing = false;
    slot->busy = 0;
    if (slot->life < HERO_IPC_LIFE_MOST) {
        slot->next = hero_ipc_table.given_back;
        hero_ipc_table.given_back = (uint32_t)(slot - hero_ipc_table.slots) + 1;
    }
}

/* Gives back what the slot holds, by its type; 0, or the platform's number
 * of a failure while doing it. Under the lock, with no call in flight. */
static int64_t hero_ipc_dispose(hero_ipc_slot *slot);

/* The slot `handle` names, marked busy, or NULL with `HERO_IPC_NOT_OPEN`. */
static hero_ipc_slot *hero_ipc_take(const void *handle, uint8_t type) {
    hero_ipc_enter();
    hero_ipc_slot *slot = hero_ipc_find(handle, type);

    if (slot != NULL) {
        slot->busy++;
    }
    hero_ipc_leave();
    if (slot == NULL) {
        hero_ipc_fail(HERO_IPC_NOT_OPEN, 0);
    }
    return slot;
}

/* Ends a call's busy mark; the last call out of a slot the program closed
 * gives its things back and the slot. */
static void hero_ipc_drop(hero_ipc_slot *slot) {
    hero_ipc_enter();
    slot->busy--;
    if (slot->closing && slot->busy == 0) {
        (void)hero_ipc_dispose(slot);
        hero_ipc_free(slot);
    }
    hero_ipc_leave();
}

/* Whether another thread closed the slot a call is busy on. */
static bool hero_ipc_closed_meanwhile(hero_ipc_slot *slot) {
    hero_ipc_enter();
    bool closing = slot->closing;
    hero_ipc_leave();
    return closing;
}

/* Why a wait on a busy slot must end now: as `hero_ipc_why_end`, and
 * `HERO_IPC_NOT_OPEN` first where another thread closed the slot. */
static int64_t hero_ipc_why_end_on(hero_ipc_slot *slot, int64_t until) {
    if (hero_ipc_closed_meanwhile(slot)) {
        return HERO_IPC_NOT_OPEN;
    }
    return hero_ipc_why_end(until);
}

/* Closes the handle: its things given back now, or by the last call in
 * flight. 0, or -1 with `HERO_IPC_NOT_OPEN` or the platform's failure. */
static int64_t hero_ipc_close_slot(const void *handle, uint8_t type) {
    int64_t said = 0;

    hero_ipc_enter();
    hero_ipc_slot *slot = hero_ipc_find(handle, type);

    if (slot == NULL) {
        hero_ipc_leave();
        hero_ipc_fail(HERO_IPC_NOT_OPEN, 0);
        return -1;
    }
    if (slot->busy > 0) {
        slot->closing = true;
    } else {
        said = hero_ipc_dispose(slot);
        hero_ipc_free(slot);
    }
    hero_ipc_leave();
    if (said != 0) {
        hero_ipc_failed_with(said);
        return -1;
    }
    hero_ipc_clear();
    return 0;
}

/* -- channels: the stream's table ------------------------------------------- */

/* What a channel's stream is made over. */
typedef struct {
#if defined(_WIN32)
    /* An AF_UNIX socket, or `INVALID_SOCKET` for a pipe. */
    SOCKET socket;
    /* A named pipe's end in overlapped mode, or NULL for a socket. */
    HANDLE pipe;
    /* The pipe's server end, which asks its client's process. */
    bool server;
#else
    int fd;
#endif
    /* When every read and write ends, on `hero_ipc_now`'s clock; 0 for no
     * deadline. Moved by `hero_ipc_set_deadline` while a read waits. */
    _Atomic int64_t deadline;
    /* Who the peer is, read once when the connection is made and never
     * written after, since Darwin answers no peer once it has closed its
     * side (`ENOTCONN`, measured 2026-11-17): the process's id and its
     * user's, -1 where the platform has none; or the kind and the number of
     * the failure that asking met. */
    int64_t peer_pid;
    int64_t peer_uid;
    int64_t peer_kind;
    int64_t peer_os;
} hero_ipc_channel;

/* The bound a channel's wait has now: its deadline, or none. */
static int64_t hero_ipc_channel_until(hero_ipc_channel *c) {
    int64_t deadline = atomic_load(&c->deadline);

    return deadline == 0 ? HERO_IPC_NEVER : deadline;
}

#if defined(_WIN32)

/* Waits for the socket to be ready for `events`, a slice at a time: 0 once
 * it is (or has an error a call will tell), or the kind that ends the wait. */
static int64_t hero_ipc_socket_wait(SOCKET s, short events, hero_ipc_channel *c, hero_ipc_slot *slot, int64_t until) {
    for (;;) {
        int64_t bound = c != NULL ? hero_ipc_channel_until(c) : until;
        int64_t why = slot != NULL ? hero_ipc_why_end_on(slot, bound) : hero_ipc_why_end(bound);

        if (why != HERO_IPC_OK) {
            return why;
        }
        WSAPOLLFD one;

        one.fd = s;
        one.events = events;
        one.revents = 0;
        int ready = WSAPoll(&one, 1, hero_ipc_slice_ms(hero_ipc_slice(bound)));

        if (ready > 0) {
            return HERO_IPC_OK;
        }
        if (ready < 0) {
            hero_ipc_failed_socket();
            return hero_ipc_kind;
        }
    }
}

/* Waits for an overlapped operation on `handle` to end, a slice at a time,
 * and answers its count into `*moved`: `HERO_IPC_OK`, the kind that ended
 * the wait (the operation cancelled first, and its count kept where it
 * ended anyway), or the operation's own failure. */
static int64_t hero_ipc_overlapped(HANDLE handle, OVERLAPPED *done, hero_ipc_channel *c, hero_ipc_slot *slot, int64_t until, DWORD *moved) {
    for (;;) {
        int64_t bound = c != NULL ? hero_ipc_channel_until(c) : until;
        int64_t why = slot != NULL ? hero_ipc_why_end_on(slot, bound) : hero_ipc_why_end(bound);

        if (why != HERO_IPC_OK) {
            (void)CancelIoEx(handle, done);
            if (GetOverlappedResult(handle, done, moved, TRUE)) {
                return HERO_IPC_OK;
            }
            return why;
        }
        DWORD waited = WaitForSingleObject(done->hEvent, (DWORD)hero_ipc_slice_ms(hero_ipc_slice(bound)));

        if (waited == WAIT_OBJECT_0) {
            if (GetOverlappedResult(handle, done, moved, FALSE)) {
                return HERO_IPC_OK;
            }
            hero_ipc_failed_system();
            return hero_ipc_kind;
        }
        if (waited != WAIT_TIMEOUT) {
            hero_ipc_failed_system();
            (void)CancelIoEx(handle, done);
            (void)GetOverlappedResult(handle, done, moved, TRUE);
            return hero_ipc_kind;
        }
    }
}

static int64_t hero_ipc_channel_read(void *payload, void *buffer, uint64_t count) {
    hero_ipc_channel *c = (hero_ipc_channel *)payload;

    if (c->pipe != NULL) {
        OVERLAPPED done;
        DWORD moved = 0;

        memset(&done, 0, sizeof done);
        done.hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
        if (done.hEvent == NULL) {
            hero_stream_set_error(HERO_STREAM_LIMIT, (int64_t)GetLastError());
            return -1;
        }
        int64_t kind = HERO_IPC_OK;

        if (!ReadFile(c->pipe, buffer, (DWORD)count, &moved, &done)) {
            DWORD why = GetLastError();

            if (why == ERROR_IO_PENDING) {
                kind = hero_ipc_overlapped(c->pipe, &done, c, NULL, 0, &moved);
            } else {
                hero_ipc_failed_with((int64_t)why);
                kind = hero_ipc_kind;
            }
        }
        (void)CloseHandle(done.hEvent);
        if (kind == HERO_IPC_CLOSED_PEER) {
            /* The other end closed its handle: the pipe's end, as a socket's. */
            return 0;
        }
        if (kind != HERO_IPC_OK) {
            hero_stream_set_error(hero_ipc_stream_kind(kind), hero_ipc_os);
            return -1;
        }
        return (int64_t)moved;
    }
    for (;;) {
        int got = recv(c->socket, (char *)buffer, (int)count, 0);

        if (got >= 0) {
            return (int64_t)got;
        }
        int why = WSAGetLastError();

        if (why != WSAEWOULDBLOCK) {
            hero_stream_set_error(hero_ipc_stream_kind(hero_ipc_kind_of((int64_t)why)), (int64_t)why);
            return -1;
        }
        int64_t kind = hero_ipc_socket_wait(c->socket, POLLRDNORM, c, NULL, 0);

        if (kind != HERO_IPC_OK) {
            hero_stream_set_error(hero_ipc_stream_kind(kind), 0);
            return -1;
        }
    }
}

static int64_t hero_ipc_channel_write(void *payload, const void *data, uint64_t count) {
    hero_ipc_channel *c = (hero_ipc_channel *)payload;

    if (c->pipe != NULL) {
        OVERLAPPED done;
        DWORD moved = 0;

        memset(&done, 0, sizeof done);
        done.hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
        if (done.hEvent == NULL) {
            hero_stream_set_error(HERO_STREAM_LIMIT, (int64_t)GetLastError());
            return -1;
        }
        int64_t kind = HERO_IPC_OK;

        if (!WriteFile(c->pipe, data, (DWORD)count, &moved, &done)) {
            DWORD why = GetLastError();

            if (why == ERROR_IO_PENDING) {
                kind = hero_ipc_overlapped(c->pipe, &done, c, NULL, 0, &moved);
            } else {
                hero_ipc_failed_with((int64_t)why);
                kind = hero_ipc_kind;
            }
        }
        (void)CloseHandle(done.hEvent);
        if (kind != HERO_IPC_OK) {
            hero_stream_set_error(hero_ipc_stream_kind(kind), hero_ipc_os);
            return -1;
        }
        return (int64_t)moved;
    }
    for (;;) {
        int sent = send(c->socket, (const char *)data, (int)count, 0);

        if (sent >= 0) {
            return (int64_t)sent;
        }
        int why = WSAGetLastError();

        if (why != WSAEWOULDBLOCK) {
            hero_stream_set_error(hero_ipc_stream_kind(hero_ipc_kind_of((int64_t)why)), (int64_t)why);
            return -1;
        }
        int64_t kind = hero_ipc_socket_wait(c->socket, POLLWRNORM, c, NULL, 0);

        if (kind != HERO_IPC_OK) {
            hero_stream_set_error(hero_ipc_stream_kind(kind), 0);
            return -1;
        }
    }
}

static int64_t hero_ipc_channel_close(void *payload) {
    hero_ipc_channel *c = (hero_ipc_channel *)payload;
    bool closed = c->pipe != NULL ? CloseHandle(c->pipe) != 0 : closesocket(c->socket) == 0;
    int64_t why = c->pipe != NULL ? (int64_t)GetLastError() : (int64_t)WSAGetLastError();

    hero_layer_free(c);
    if (!closed) {
        hero_stream_set_error(hero_ipc_stream_kind(hero_ipc_kind_of(why)), why);
        return -1;
    }
    return 0;
}

#else

/* Waits for the descriptor to be ready for `events`, a slice at a time: 0
 * once it is (or has an error a call will tell), or the kind that ends the
 * wait. A channel's bound is read again at each slice, so a deadline moved
 * meanwhile is the one kept. */
static int64_t hero_ipc_fd_wait(int fd, short events, hero_ipc_channel *c, hero_ipc_slot *slot, int64_t until) {
    for (;;) {
        int64_t bound = c != NULL ? hero_ipc_channel_until(c) : until;
        int64_t why = slot != NULL ? hero_ipc_why_end_on(slot, bound) : hero_ipc_why_end(bound);

        if (why != HERO_IPC_OK) {
            return why;
        }
        struct pollfd one;

        one.fd = fd;
        one.events = events;
        one.revents = 0;
        int ready = poll(&one, 1, hero_ipc_slice_ms(hero_ipc_slice(bound)));

        if (ready > 0) {
            return HERO_IPC_OK;
        }
        if (ready < 0 && errno != EINTR) {
            hero_ipc_failed_system();
            return hero_ipc_kind;
        }
    }
}

static int64_t hero_ipc_channel_read(void *payload, void *buffer, uint64_t count) {
    hero_ipc_channel *c = (hero_ipc_channel *)payload;

    for (;;) {
        ssize_t got = recv(c->fd, buffer, (size_t)count, 0);

        if (got >= 0) {
            return (int64_t)got;
        }
        int why = errno;

        if (why == EINTR) {
            continue;
        }
        if (why != EAGAIN && why != EWOULDBLOCK) {
            hero_stream_set_error(hero_ipc_stream_kind(hero_ipc_kind_of((int64_t)why)), (int64_t)why);
            return -1;
        }
        int64_t kind = hero_ipc_fd_wait(c->fd, POLLIN, c, NULL, 0);

        if (kind != HERO_IPC_OK) {
            hero_stream_set_error(hero_ipc_stream_kind(kind), kind == hero_ipc_kind ? hero_ipc_os : 0);
            return -1;
        }
    }
}

static int64_t hero_ipc_channel_write(void *payload, const void *data, uint64_t count) {
    hero_ipc_channel *c = (hero_ipc_channel *)payload;

    for (;;) {
        ssize_t sent = send(c->fd, data, (size_t)count, HERO_IPC_SEND_FLAGS);

        if (sent >= 0) {
            return (int64_t)sent;
        }
        int why = errno;

        if (why == EINTR) {
            continue;
        }
        if (why != EAGAIN && why != EWOULDBLOCK) {
            hero_stream_set_error(hero_ipc_stream_kind(hero_ipc_kind_of((int64_t)why)), (int64_t)why);
            return -1;
        }
        int64_t kind = hero_ipc_fd_wait(c->fd, POLLOUT, c, NULL, 0);

        if (kind != HERO_IPC_OK) {
            hero_stream_set_error(hero_ipc_stream_kind(kind), kind == hero_ipc_kind ? hero_ipc_os : 0);
            return -1;
        }
    }
}

static int64_t hero_ipc_channel_close(void *payload) {
    hero_ipc_channel *c = (hero_ipc_channel *)payload;
    int closed = close(c->fd);
    int why = errno;

    hero_layer_free(c);
    /* POSIX leaves the descriptor's state unspecified after EINTR, and
     * Darwin and Linux both release it: never closed twice. */
    if (closed != 0 && why != EINTR) {
        hero_stream_set_error(hero_ipc_stream_kind(hero_ipc_kind_of((int64_t)why)), (int64_t)why);
        return -1;
    }
    return 0;
}

#endif


/* The table every channel's stream is made with: it stays where it is for
 * as long as any of them is open, and nothing writes it. */
static const hero_stream_ops hero_ipc_channel_ops = { hero_ipc_channel_read, hero_ipc_channel_write, hero_ipc_channel_close };

/* The channel under `s`, held until `hero_stream_release(s)`; NULL with the
 * failure named where `s` is no open stream this member made. */
static hero_ipc_channel *hero_ipc_channel_held(hero_stream *s) {
    void *payload = NULL;

    if (hero_stream_hold_payload(s, &hero_ipc_channel_ops, &payload) != 0) {
        hero_ipc_failed_as_stream();
        return NULL;
    }
    return (hero_ipc_channel *)payload;
}

/* A stream over the channel `c`, of `kind`; NULL with the failure named,
 * and then `c` given back and what it stands on closed. */
/* Asks the system who is at the other end of the new channel `c` and keeps
 * the answer, or the failure, in it. */
static void hero_ipc_learn_peer(hero_ipc_channel *c);

static hero_stream *hero_ipc_stream_of(hero_ipc_channel *c, int64_t kind) {
    hero_ipc_learn_peer(c);
    hero_stream *s = hero_stream_make(c, &hero_ipc_channel_ops, kind);

    if (s == NULL) {
        hero_ipc_failed_as_stream();
        int64_t why = hero_ipc_kind;
        int64_t os = hero_ipc_os;

        (void)hero_ipc_channel_close(c);
        hero_ipc_fail(why, os);
        return NULL;
    }
    hero_ipc_clear();
    return s;
}

/* A new channel's block, its deadline none; NULL with `HERO_IPC_LIMIT`. */
static hero_ipc_channel *hero_ipc_channel_new(void) {
    hero_ipc_channel *c = (hero_ipc_channel *)hero_layer_alloc(sizeof(hero_ipc_channel));

    if (c == NULL) {
        hero_ipc_fail(HERO_IPC_LIMIT, 0);
        return NULL;
    }
    memset(c, 0, sizeof *c);
    atomic_init(&c->deadline, 0);
    return c;
}

/* Whether `space` is one of the three, and `perm` 0 to 0777; false with
 * `HERO_IPC_INVALID`. */
static bool hero_ipc_space_valid(int64_t space) {
    if (space != HERO_IPC_SPACE_PATH && space != HERO_IPC_SPACE_ABSTRACT && space != HERO_IPC_SPACE_PIPE) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return false;
    }
    return true;
}

#if defined(_WIN32)

/* -- channels on Windows ----------------------------------------------------- */

/* A socket's address for `name`, a path; false with the failure named. */
static bool hero_ipc_address(const char *name, SOCKADDR_UN *address) {
    size_t length = name == NULL ? 0 : strlen(name);

    memset(address, 0, sizeof *address);
    address->sun_family = AF_UNIX;
    if (length == 0 || length >= sizeof address->sun_path) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return false;
    }
    memcpy(address->sun_path, name, length + 1);
    return true;
}

/* A new AF_UNIX socket, not inherited and not blocking; `INVALID_SOCKET`
 * with the failure named. */
static SOCKET hero_ipc_socket(void) {
    if (!hero_ipc_winsock()) {
        return INVALID_SOCKET;
    }
    SOCKET s = WSASocketW(AF_UNIX, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_NO_HANDLE_INHERIT);
    u_long on = 1;

    if (s == INVALID_SOCKET) {
        hero_ipc_failed_socket();
        return INVALID_SOCKET;
    }
    /* `FIONBIO` is an unsigned bit pattern the call takes as its `long`. */
    if (ioctlsocket(s, (long)FIONBIO, &on) != 0) {
        hero_ipc_failed_socket();
        (void)closesocket(s);
        return INVALID_SOCKET;
    }
    return s;
}

/* `\\.\pipe\` and the name, as UTF-16 into `out`; false with the failure
 * named. */
static bool hero_ipc_pipe_name(const char *name, wchar_t *out, int room) {
    char whole[HERO_IPC_NAME_ROOM];

    if (!hero_ipc_named(name, HERO_IPC_NAME_MOST) || !hero_ipc_joined(whole, sizeof whole, "\\\\.\\pipe\\", name)) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return false;
    }
    return hero_ipc_wide(whole, out, room);
}

/* One instance of the pipe `name` waiting for a client, overlapped, local
 * clients alone, the first of its name where `first` (so a second listener
 * on a name taken fails); `INVALID_HANDLE_VALUE` with the failure named. */
static HANDLE hero_ipc_pipe_instance(const char *name, bool first) {
    wchar_t wide[HERO_IPC_NAME_ROOM];

    if (!hero_ipc_pipe_name(name, wide, HERO_IPC_NAME_ROOM)) {
        return INVALID_HANDLE_VALUE;
    }
    DWORD open = PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED | (first ? FILE_FLAG_FIRST_PIPE_INSTANCE : 0);
    HANDLE pipe = CreateNamedPipeW(wide, open, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS,
                                   PIPE_UNLIMITED_INSTANCES, 65536, 65536, 0, NULL);

    if (pipe == INVALID_HANDLE_VALUE) {
        DWORD why = GetLastError();

        /* The first instance of a name another holds is refused as access
         * denied: the name is taken. */
        hero_ipc_fail(first && why == ERROR_ACCESS_DENIED ? HERO_IPC_EXISTS : hero_ipc_kind_of((int64_t)why), (int64_t)why);
    }
    return pipe;
}

hero_ipc_listener *hero_ipc_listen(const char *name, int64_t space, int64_t perm) {
    SOCKADDR_UN address;
    SOCKET s = INVALID_SOCKET;
    HANDLE pipe = NULL;

    if (!hero_ipc_space_valid(space)) {
        return NULL;
    }
    if (perm < 0 || perm > 0777) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return NULL;
    }
    if (space == HERO_IPC_SPACE_ABSTRACT) {
        hero_ipc_fail(HERO_IPC_UNSUPPORTED, 0);
        return NULL;
    }
    if (space == HERO_IPC_SPACE_PIPE) {
        pipe = hero_ipc_pipe_instance(name, true);
        if (pipe == INVALID_HANDLE_VALUE) {
            return NULL;
        }
    } else {
        if (!hero_ipc_address(name, &address)) {
            return NULL;
        }
        s = hero_ipc_socket();
        if (s == INVALID_SOCKET) {
            return NULL;
        }
        if (bind(s, (const struct sockaddr *)&address, (int)sizeof address) != 0 || listen(s, SOMAXCONN) != 0) {
            hero_ipc_failed_socket();
            (void)closesocket(s);
            return NULL;
        }
    }
    hero_ipc_enter();
    hero_ipc_slot *slot = hero_ipc_issue(HERO_IPC_TYPE_LISTENER);

    if (slot == NULL) {
        hero_ipc_leave();
        if (pipe != NULL) {
            (void)CloseHandle(pipe);
        } else {
            (void)closesocket(s);
        }
        hero_ipc_fail(HERO_IPC_LIMIT, 0);
        return NULL;
    }
    slot->space = space;
    slot->socket = s;
    slot->handle = pipe;
    memcpy(slot->name, name, strlen(name) + 1);
    hero_ipc_listener *handle = (hero_ipc_listener *)hero_ipc_handle(slot);
    hero_ipc_leave();
    hero_ipc_clear();
    return handle;
}

/* A stream over the pipe's end `pipe`, the server's where `server`; NULL
 * with the failure named, the end closed. */
static hero_stream *hero_ipc_stream_of_pipe(HANDLE pipe, bool server) {
    hero_ipc_channel *c = hero_ipc_channel_new();

    if (c == NULL) {
        (void)CloseHandle(pipe);
        return NULL;
    }
    c->socket = INVALID_SOCKET;
    c->pipe = pipe;
    c->server = server;
    return hero_ipc_stream_of(c, HERO_STREAM_KIND_PIPE);
}

/* A stream over the connected socket `s`; NULL with the failure named, the
 * socket closed. */
static hero_stream *hero_ipc_stream_of_socket(SOCKET s) {
    hero_ipc_channel *c = hero_ipc_channel_new();

    if (c == NULL) {
        (void)closesocket(s);
        return NULL;
    }
    c->socket = s;
    c->pipe = NULL;
    return hero_ipc_stream_of(c, HERO_STREAM_KIND_SOCKET);
}

/* The next client of the pipe listener `slot`, busy, by `until`. One accept
 * at a time: an instance waits for one client. */
static hero_stream *hero_ipc_pipe_accept(hero_ipc_slot *slot, int64_t until) {
    hero_ipc_enter();
    if (slot->accepting) {
        hero_ipc_leave();
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return NULL;
    }
    slot->accepting = true;
    HANDLE instance = slot->handle;
    hero_ipc_leave();

    if (instance == NULL) {
        /* The instance after the last client could not be made then. */
        instance = hero_ipc_pipe_instance(slot->name, false);
        hero_ipc_enter();
        slot->handle = instance == INVALID_HANDLE_VALUE ? NULL : instance;
        slot->accepting = instance != INVALID_HANDLE_VALUE;
        hero_ipc_leave();
        if (instance == INVALID_HANDLE_VALUE) {
            return NULL;
        }
    }
    OVERLAPPED done;
    DWORD moved = 0;
    int64_t kind = HERO_IPC_OK;
    bool connected = false;

    memset(&done, 0, sizeof done);
    done.hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
    if (done.hEvent == NULL) {
        hero_ipc_failed_system();
        kind = hero_ipc_kind;
    }
    for (int64_t tries = 0; done.hEvent != NULL; tries++) {
        /* After a client that came and went, the bound and the stop are
         * asked again, so such clients cannot hold the call. */
        if (tries > 0) {
            kind = hero_ipc_why_end_on(slot, until);
            if (kind != HERO_IPC_OK) {
                break;
            }
        }
        if (ConnectNamedPipe(instance, &done)) {
            connected = true;
            break;
        }
        DWORD why = GetLastError();

        if (why == ERROR_PIPE_CONNECTED) {
            connected = true;
            break;
        }
        if (why == ERROR_IO_PENDING) {
            kind = hero_ipc_overlapped(instance, &done, NULL, slot, until, &moved);
            connected = kind == HERO_IPC_OK;
            break;
        }
        if (why != ERROR_NO_DATA) {
            hero_ipc_failed_with((int64_t)why);
            kind = hero_ipc_kind;
            break;
        }
        /* A client came and went before this call: the instance is freed for
         * the next. */
        (void)DisconnectNamedPipe(instance);
        (void)ResetEvent(done.hEvent);
    }
    int64_t os = hero_ipc_os;

    if (done.hEvent != NULL) {
        (void)CloseHandle(done.hEvent);
    }
    /* The next instance before this one leaves the listener, so the name
     * never stands without one. */
    HANDLE next = hero_ipc_pipe_instance(slot->name, false);

    hero_ipc_enter();
    if (connected) {
        slot->handle = next == INVALID_HANDLE_VALUE ? NULL : next;
    } else if (next != INVALID_HANDLE_VALUE) {
        (void)CloseHandle(instance);
        slot->handle = next;
    }
    slot->accepting = false;
    hero_ipc_leave();
    if (!connected) {
        hero_ipc_fail(kind, os);
        return NULL;
    }
    return hero_ipc_stream_of_pipe(instance, true);
}

hero_stream *hero_ipc_accept(hero_ipc_listener *l, int64_t within_ms) {
    int64_t until = 0;

    if (!hero_ipc_deadline(within_ms, &until)) {
        return NULL;
    }
    hero_ipc_slot *slot = hero_ipc_take(l, HERO_IPC_TYPE_LISTENER);

    if (slot == NULL) {
        return NULL;
    }
    if (slot->space == HERO_IPC_SPACE_PIPE) {
        hero_stream *s = hero_ipc_pipe_accept(slot, until);
        int64_t kind = hero_ipc_kind;
        int64_t os = hero_ipc_os;

        hero_ipc_drop(slot);
        hero_ipc_fail(kind, os);
        return s;
    }
    for (;;) {
        SOCKET s = accept(slot->socket, NULL, NULL);

        if (s != INVALID_SOCKET) {
            u_long on = 1;

            hero_ipc_drop(slot);
            if (!SetHandleInformation((HANDLE)s, HANDLE_FLAG_INHERIT, 0) || ioctlsocket(s, (long)FIONBIO, &on) != 0) {
                hero_ipc_failed_socket();
                (void)closesocket(s);
                return NULL;
            }
            return hero_ipc_stream_of_socket(s);
        }
        int why = WSAGetLastError();

        if (why == WSAECONNRESET) {
            continue;
        }
        if (why != WSAEWOULDBLOCK) {
            hero_ipc_drop(slot);
            hero_ipc_failed_with((int64_t)why);
            return NULL;
        }
        int64_t kind = hero_ipc_socket_wait(slot->socket, POLLRDNORM, NULL, slot, until);

        if (kind != HERO_IPC_OK) {
            int64_t os = kind == hero_ipc_kind ? hero_ipc_os : 0;

            hero_ipc_drop(slot);
            hero_ipc_fail(kind, os);
            return NULL;
        }
    }
}

hero_stream *hero_ipc_connect(const char *name, int64_t space, int64_t within_ms) {
    int64_t until = 0;

    if (!hero_ipc_space_valid(space) || !hero_ipc_deadline(within_ms, &until)) {
        return NULL;
    }
    if (space == HERO_IPC_SPACE_ABSTRACT) {
        hero_ipc_fail(HERO_IPC_UNSUPPORTED, 0);
        return NULL;
    }
    if (space == HERO_IPC_SPACE_PIPE) {
        wchar_t wide[HERO_IPC_NAME_ROOM];

        if (!hero_ipc_pipe_name(name, wide, HERO_IPC_NAME_ROOM)) {
            return NULL;
        }
        for (;;) {
            /* SQOS: the server may learn who this client is and never act
             * as it. */
            HANDLE pipe = CreateFileW(wide, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
                                      FILE_FLAG_OVERLAPPED | SECURITY_SQOS_PRESENT | SECURITY_IDENTIFICATION, NULL);

            if (pipe != INVALID_HANDLE_VALUE) {
                return hero_ipc_stream_of_pipe(pipe, false);
            }
            DWORD why = GetLastError();

            if (why != ERROR_PIPE_BUSY) {
                hero_ipc_failed_with((int64_t)why);
                return NULL;
            }
            int64_t kind = hero_ipc_why_end(until);

            if (kind != HERO_IPC_OK) {
                hero_ipc_fail(kind, 0);
                return NULL;
            }
            /* Every instance serves a client: wait a slice for one to free. */
            (void)WaitNamedPipeW(wide, (DWORD)hero_ipc_slice_ms(hero_ipc_slice(until)));
        }
    }
    SOCKADDR_UN address;

    if (!hero_ipc_address(name, &address)) {
        return NULL;
    }
    SOCKET s = hero_ipc_socket();

    if (s == INVALID_SOCKET) {
        return NULL;
    }
    if (connect(s, (const struct sockaddr *)&address, (int)sizeof address) != 0) {
        int why = WSAGetLastError();

        if (why != WSAEWOULDBLOCK) {
            (void)closesocket(s);
            hero_ipc_failed_with((int64_t)why);
            return NULL;
        }
        int64_t kind = hero_ipc_socket_wait(s, POLLWRNORM, NULL, NULL, until);
        int said = 0;
        int length = (int)sizeof said;

        if (kind == HERO_IPC_OK && getsockopt(s, SOL_SOCKET, SO_ERROR, (char *)&said, &length) != 0) {
            hero_ipc_failed_socket();
            kind = hero_ipc_kind;
        }
        if (kind == HERO_IPC_OK && said != 0) {
            hero_ipc_failed_with((int64_t)said);
            kind = hero_ipc_kind;
        }
        if (kind != HERO_IPC_OK) {
            int64_t os = hero_ipc_os;

            (void)closesocket(s);
            hero_ipc_fail(kind, os);
            return NULL;
        }
    }
    return hero_ipc_stream_of_socket(s);
}

int64_t hero_ipc_shutdown_write(hero_stream *s) {
    hero_ipc_channel *c = hero_ipc_channel_held(s);

    if (c == NULL) {
        return -1;
    }
    int64_t answer = 0;

    if (c->pipe != NULL) {
        hero_ipc_fail(HERO_IPC_UNSUPPORTED, 0);
        answer = -1;
    } else if (shutdown(c->socket, SD_SEND) != 0) {
        hero_ipc_failed_socket();
        answer = -1;
    }
    int64_t kind = hero_ipc_kind;
    int64_t os = hero_ipc_os;

    (void)hero_stream_release(s);
    if (answer == 0) {
        hero_ipc_clear();
    } else {
        hero_ipc_fail(kind, os);
    }
    return answer;
}

static void hero_ipc_learn_peer(hero_ipc_channel *c) {
    ULONG id = 0;
    BOOL told = FALSE;
    int64_t why = 0;

    if (c->pipe != NULL) {
        told = c->server ? GetNamedPipeClientProcessId(c->pipe, &id) : GetNamedPipeServerProcessId(c->pipe, &id);
        why = told ? 0 : (int64_t)GetLastError();
    } else {
        DWORD returned = 0;

        told = WSAIoctl(c->socket, SIO_AF_UNIX_GETPEERPID, NULL, 0, &id, (DWORD)sizeof id, &returned, NULL, NULL) == 0;
        why = told ? 0 : (int64_t)WSAGetLastError();
    }
    c->peer_pid = told ? (int64_t)id : -1;
    /* Windows numbers no user so. */
    c->peer_uid = -1;
    c->peer_kind = told ? HERO_IPC_OK : hero_ipc_kind_of(why);
    c->peer_os = why;
}

#else

/* -- channels on POSIX ------------------------------------------------------- */

#if !defined(__linux__)
/* The descriptor made close-on-exec, not blocking, and silent on a write to
 * a peer gone where the platform marks that per socket; false with the
 * failure named. Linux sets the first two in the call that makes it, and
 * stops SIGPIPE per call. */
static bool hero_ipc_prepared(int fd) {
    int flags = fcntl(fd, F_GETFD);

    if (flags < 0 || fcntl(fd, F_SETFD, flags | FD_CLOEXEC) != 0) {
        hero_ipc_failed_system();
        return false;
    }
    flags = fcntl(fd, F_GETFL);
    if (flags < 0 || fcntl(fd, F_SETFL, flags | O_NONBLOCK) != 0) {
        hero_ipc_failed_system();
        return false;
    }
#if defined(SO_NOSIGPIPE)
    int one = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &one, (socklen_t)sizeof one) != 0) {
        hero_ipc_failed_system();
        return false;
    }
#endif
    return true;
}
#endif

/* A new AF_UNIX stream socket, prepared; -1 with the failure named. */
static int hero_ipc_socket(void) {
#if defined(__linux__)
    int fd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);

    if (fd < 0) {
        hero_ipc_failed_system();
    }
    return fd;
#else
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (fd < 0) {
        hero_ipc_failed_system();
        return -1;
    }
    if (!hero_ipc_prepared(fd)) {
        int64_t kind = hero_ipc_kind;
        int64_t os = hero_ipc_os;

        (void)close(fd);
        hero_ipc_fail(kind, os);
        return -1;
    }
    return fd;
#endif
}

/* A socket's address for `name` in `space` and its length; false with the
 * failure named: a pipe here, or an abstract name off Linux, is
 * `HERO_IPC_UNSUPPORTED`. */
static bool hero_ipc_address(const char *name, int64_t space, struct sockaddr_un *address, socklen_t *length) {
    size_t count = name == NULL ? 0 : strlen(name);

    memset(address, 0, sizeof *address);
    address->sun_family = AF_UNIX;
    if (space == HERO_IPC_SPACE_PIPE) {
        hero_ipc_fail(HERO_IPC_UNSUPPORTED, 0);
        return false;
    }
    if (space == HERO_IPC_SPACE_ABSTRACT) {
#if defined(__linux__)
        if (!hero_ipc_named(name, HERO_IPC_NAME_MOST)) {
            hero_ipc_fail(HERO_IPC_INVALID, 0);
            return false;
        }
        /* A zero, then the name's bytes, its length the address's own. */
        memcpy(address->sun_path + 1, name, count);
        *length = (socklen_t)(offsetof(struct sockaddr_un, sun_path) + 1 + count);
        return true;
#else
        hero_ipc_fail(HERO_IPC_UNSUPPORTED, 0);
        return false;
#endif
    }
    if (count == 0 || count >= sizeof address->sun_path) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return false;
    }
    memcpy(address->sun_path, name, count + 1);
    *length = (socklen_t)(offsetof(struct sockaddr_un, sun_path) + count + 1);
#if defined(__APPLE__)
    address->sun_len = (unsigned char)*length;
#endif
    return true;
}

hero_ipc_listener *hero_ipc_listen(const char *name, int64_t space, int64_t perm) {
    struct sockaddr_un address;
    socklen_t length = 0;
    struct stat made;

    if (!hero_ipc_space_valid(space)) {
        return NULL;
    }
    if (perm < 0 || perm > 0777) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return NULL;
    }
    if (!hero_ipc_address(name, space, &address, &length)) {
        return NULL;
    }
    int fd = hero_ipc_socket();

    if (fd < 0) {
        return NULL;
    }
    if (bind(fd, (const struct sockaddr *)&address, length) != 0) {
        hero_ipc_failed_system();
        int64_t kind = hero_ipc_kind;
        int64_t os = hero_ipc_os;

        (void)close(fd);
        hero_ipc_fail(kind, os);
        return NULL;
    }
    /* The permission before `listen`: until then a connect is refused, so
     * no client reaches the file at the mask's permission. */
    memset(&made, 0, sizeof made);
    if ((space == HERO_IPC_SPACE_PATH && (chmod(name, (mode_t)perm) != 0 || stat(name, &made) != 0)) || listen(fd, SOMAXCONN) != 0) {
        hero_ipc_failed_system();
        int64_t kind = hero_ipc_kind;
        int64_t os = hero_ipc_os;

        if (space == HERO_IPC_SPACE_PATH) {
            (void)unlink(name);
        }
        (void)close(fd);
        hero_ipc_fail(kind, os);
        return NULL;
    }
    hero_ipc_enter();
    hero_ipc_slot *slot = hero_ipc_issue(HERO_IPC_TYPE_LISTENER);

    if (slot == NULL) {
        hero_ipc_leave();
        if (space == HERO_IPC_SPACE_PATH) {
            (void)unlink(name);
        }
        (void)close(fd);
        hero_ipc_fail(HERO_IPC_LIMIT, 0);
        return NULL;
    }
    slot->space = space;
    slot->fd = fd;
    slot->device = made.st_dev;
    slot->inode = made.st_ino;
    memcpy(slot->name, name, strlen(name) + 1);
    hero_ipc_listener *handle = (hero_ipc_listener *)hero_ipc_handle(slot);
    hero_ipc_leave();
    hero_ipc_clear();
    return handle;
}

/* A stream over the connected socket `fd`; NULL with the failure named, the
 * socket closed. */
static hero_stream *hero_ipc_stream_of_fd(int fd) {
    hero_ipc_channel *c = hero_ipc_channel_new();

    if (c == NULL) {
        (void)close(fd);
        return NULL;
    }
    c->fd = fd;
    return hero_ipc_stream_of(c, HERO_STREAM_KIND_SOCKET);
}

hero_stream *hero_ipc_accept(hero_ipc_listener *l, int64_t within_ms) {
    int64_t until = 0;

    if (!hero_ipc_deadline(within_ms, &until)) {
        return NULL;
    }
    hero_ipc_slot *slot = hero_ipc_take(l, HERO_IPC_TYPE_LISTENER);

    if (slot == NULL) {
        return NULL;
    }
    for (;;) {
#if defined(__linux__)
        int fd = accept4(slot->fd, NULL, NULL, SOCK_CLOEXEC | SOCK_NONBLOCK);
#else
        int fd = accept(slot->fd, NULL, NULL);
#endif

        if (fd >= 0) {
            hero_ipc_drop(slot);
#if !defined(__linux__)
            if (!hero_ipc_prepared(fd)) {
                int64_t kind = hero_ipc_kind;
                int64_t os = hero_ipc_os;

                (void)close(fd);
                hero_ipc_fail(kind, os);
                return NULL;
            }
#endif
            return hero_ipc_stream_of_fd(fd);
        }
        int why = errno;

        if (why == EINTR || why == ECONNABORTED) {
            continue;
        }
        if (why != EAGAIN && why != EWOULDBLOCK) {
            hero_ipc_drop(slot);
            hero_ipc_failed_with((int64_t)why);
            return NULL;
        }
        int64_t kind = hero_ipc_fd_wait(slot->fd, POLLIN, NULL, slot, until);

        if (kind != HERO_IPC_OK) {
            int64_t os = kind == hero_ipc_kind ? hero_ipc_os : 0;

            hero_ipc_drop(slot);
            hero_ipc_fail(kind, os);
            return NULL;
        }
    }
}

hero_stream *hero_ipc_connect(const char *name, int64_t space, int64_t within_ms) {
    struct sockaddr_un address;
    socklen_t length = 0;
    int64_t until = 0;
    int64_t step = HERO_IPC_STEP_FIRST_NS;

    if (!hero_ipc_space_valid(space) || !hero_ipc_deadline(within_ms, &until) || !hero_ipc_address(name, space, &address, &length)) {
        return NULL;
    }
    int fd = hero_ipc_socket();

    if (fd < 0) {
        return NULL;
    }
    for (;;) {
        if (connect(fd, (const struct sockaddr *)&address, length) == 0) {
            return hero_ipc_stream_of_fd(fd);
        }
        int why = errno;
        int64_t kind = HERO_IPC_OK;

        if (why == EAGAIN) {
            /* Linux: the listener's queue is full, and the connection is not
             * made; it is asked again after a step. */
            kind = hero_ipc_why_end(until);
            if (kind == HERO_IPC_OK) {
                hero_ipc_step(until, &step);
                continue;
            }
        } else if (why == EINPROGRESS || why == EINTR) {
            int said = 0;
            socklen_t room = (socklen_t)sizeof said;

            kind = hero_ipc_fd_wait(fd, POLLOUT, NULL, NULL, until);
            if (kind == HERO_IPC_OK && getsockopt(fd, SOL_SOCKET, SO_ERROR, &said, &room) != 0) {
                hero_ipc_failed_system();
                kind = hero_ipc_kind;
            } else if (kind == HERO_IPC_OK && said != 0) {
                hero_ipc_failed_with((int64_t)said);
                kind = hero_ipc_kind;
            }
            if (kind == HERO_IPC_OK) {
                return hero_ipc_stream_of_fd(fd);
            }
        } else {
            hero_ipc_failed_with((int64_t)why);
            kind = hero_ipc_kind;
        }
        int64_t os = kind == hero_ipc_kind ? hero_ipc_os : 0;

        (void)close(fd);
        hero_ipc_fail(kind, os);
        return NULL;
    }
}

int64_t hero_ipc_shutdown_write(hero_stream *s) {
    hero_ipc_channel *c = hero_ipc_channel_held(s);

    if (c == NULL) {
        return -1;
    }
    int answer = shutdown(c->fd, SHUT_WR);
    int why = errno;

    (void)hero_stream_release(s);
    if (answer != 0) {
        hero_ipc_failed_with((int64_t)why);
        return -1;
    }
    hero_ipc_clear();
    return 0;
}

static void hero_ipc_learn_peer(hero_ipc_channel *c) {
    int why = 0;

    c->peer_pid = -1;
    c->peer_uid = -1;
#if defined(__linux__)
    hero_ipc_ucred cred;
    socklen_t room = (socklen_t)sizeof cred;

    if (getsockopt(c->fd, SOL_SOCKET, SO_PEERCRED, &cred, &room) != 0) {
        why = errno;
    } else {
        c->peer_pid = (int64_t)cred.pid;
        c->peer_uid = (int64_t)cred.uid;
    }
#elif defined(__APPLE__)
    pid_t process = 0;
    socklen_t room = (socklen_t)sizeof process;
    uid_t owner = 0;
    gid_t group = 0;

    if (getsockopt(c->fd, SOL_LOCAL, LOCAL_PEERPID, &process, &room) != 0 || getpeereid(c->fd, &owner, &group) != 0) {
        why = errno;
    } else {
        c->peer_pid = (int64_t)process;
        c->peer_uid = (int64_t)owner;
    }
#else
    why = ENOSYS;
#endif
    c->peer_kind = why == 0 ? HERO_IPC_OK : hero_ipc_kind_of((int64_t)why);
    c->peer_os = (int64_t)why;
}

#endif

int64_t hero_ipc_peer(hero_stream *s, int64_t *pid, int64_t *uid) {
    if (pid == NULL || uid == NULL) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return -1;
    }
    hero_ipc_channel *c = hero_ipc_channel_held(s);

    if (c == NULL) {
        return -1;
    }
    int64_t found_pid = c->peer_pid;
    int64_t found_uid = c->peer_uid;
    int64_t kind = c->peer_kind;
    int64_t os = c->peer_os;

    (void)hero_stream_release(s);
    if (kind != HERO_IPC_OK) {
        hero_ipc_fail(kind, os);
        return -1;
    }
    *pid = found_pid;
    *uid = found_uid;
    hero_ipc_clear();
    return 0;
}

int64_t hero_ipc_listener_close(hero_ipc_listener *l) {
    return hero_ipc_close_slot(l, HERO_IPC_TYPE_LISTENER);
}

int64_t hero_ipc_set_deadline(hero_stream *s, int64_t ms) {
    int64_t until = 0;

    if (ms < 0) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return -1;
    }
    if (ms > 0 && !hero_ipc_deadline(ms, &until)) {
        return -1;
    }
    hero_ipc_channel *c = hero_ipc_channel_held(s);

    if (c == NULL) {
        return -1;
    }
    atomic_store(&c->deadline, until);
    (void)hero_stream_release(s);
    hero_ipc_clear();
    return 0;
}

/* -- shared memory ----------------------------------------------------------- */

/* A new slot of `type`, the table's lock held on return for the caller to
 * fill it and leave; NULL with `HERO_IPC_LIMIT` and the lock left where the
 * table is full. */
static hero_ipc_slot *hero_ipc_issue_locked(uint8_t type) {
    hero_ipc_enter();
    hero_ipc_slot *slot = hero_ipc_issue(type);

    if (slot == NULL) {
        hero_ipc_leave();
    }
    return slot;
}

/* The memory `base` of `length` bytes as a new handle; NULL with
 * `HERO_IPC_LIMIT`, the mapping given back and, where `made`, the name taken
 * away again. */
#if defined(_WIN32)
static hero_ipc_memory *hero_ipc_memory_issue(HANDLE mapping, unsigned char *base, uint64_t length) {
    hero_ipc_slot *slot = hero_ipc_issue_locked(HERO_IPC_TYPE_MEMORY);

    if (slot == NULL) {
        (void)UnmapViewOfFile(base);
        (void)CloseHandle(mapping);
        hero_ipc_fail(HERO_IPC_LIMIT, 0);
        return NULL;
    }
    slot->handle = mapping;
#else
static hero_ipc_memory *hero_ipc_memory_issue(const char *object, bool made, unsigned char *base, uint64_t length) {
    hero_ipc_slot *slot = hero_ipc_issue_locked(HERO_IPC_TYPE_MEMORY);

    if (slot == NULL) {
        (void)munmap(base, (size_t)length);
        if (made) {
            (void)shm_unlink(object);
        }
        hero_ipc_fail(HERO_IPC_LIMIT, 0);
        return NULL;
    }
#endif
    slot->base = base;
    slot->length = length;
    hero_ipc_memory *handle = (hero_ipc_memory *)hero_ipc_handle(slot);
    hero_ipc_leave();
    hero_ipc_clear();
    return handle;
}

#if defined(_WIN32)

/* The object's name as UTF-16; false with `HERO_IPC_INVALID`. */
static bool hero_ipc_object(const char *name, wchar_t *out) {
    if (!hero_ipc_named(name, HERO_IPC_NAME_MOST)) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return false;
    }
    return hero_ipc_wide(name, out, HERO_IPC_NAME_ROOM);
}

/* The view of `mapping`, its whole size, as a handle; NULL with the failure
 * named, the mapping closed. */
static hero_ipc_memory *hero_ipc_memory_viewed(HANDLE mapping) {
    MEMORY_BASIC_INFORMATION about;
    void *view = MapViewOfFile(mapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);

    if (view == NULL) {
        hero_ipc_failed_system();
        int64_t kind = hero_ipc_kind;
        int64_t os = hero_ipc_os;

        (void)CloseHandle(mapping);
        hero_ipc_fail(kind, os);
        return NULL;
    }
    if (VirtualQuery(view, &about, sizeof about) == 0 || about.RegionSize == 0) {
        hero_ipc_failed_system();
        int64_t kind = hero_ipc_kind;
        int64_t os = hero_ipc_os;

        (void)UnmapViewOfFile(view);
        (void)CloseHandle(mapping);
        hero_ipc_fail(kind, os);
        return NULL;
    }
    return hero_ipc_memory_issue(mapping, (unsigned char *)view, (uint64_t)about.RegionSize);
}

hero_ipc_memory *hero_ipc_memory_create(const char *name, int64_t size, int64_t perm) {
    wchar_t wide[HERO_IPC_NAME_ROOM];

    if (size <= 0 || perm < 0 || perm > 0777) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return NULL;
    }
    if (!hero_ipc_object(name, wide)) {
        return NULL;
    }
    uint64_t whole = (uint64_t)size;
    HANDLE mapping = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, (DWORD)(whole >> 32), (DWORD)(whole & 0xffffffffu), wide);

    if (mapping == NULL) {
        DWORD why = GetLastError();

        /* A name another kind of object holds. */
        hero_ipc_fail(why == ERROR_INVALID_HANDLE ? HERO_IPC_EXISTS : hero_ipc_kind_of((int64_t)why), (int64_t)why);
        return NULL;
    }
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        (void)CloseHandle(mapping);
        hero_ipc_fail(HERO_IPC_EXISTS, (int64_t)ERROR_ALREADY_EXISTS);
        return NULL;
    }
    return hero_ipc_memory_viewed(mapping);
}

hero_ipc_memory *hero_ipc_memory_open(const char *name) {
    wchar_t wide[HERO_IPC_NAME_ROOM];

    if (!hero_ipc_object(name, wide)) {
        return NULL;
    }
    HANDLE mapping = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, wide);

    if (mapping == NULL) {
        hero_ipc_failed_system();
        return NULL;
    }
    return hero_ipc_memory_viewed(mapping);
}

/* 0 where an object of the name is open, opened by `open_one` and closed at
 * once; -1 with `HERO_IPC_NOT_FOUND` where none is. Windows takes a name
 * away with its object's last handle, so nothing else is to do. */
static int64_t hero_ipc_remove_object(const char *name, uint8_t type) {
    wchar_t wide[HERO_IPC_NAME_ROOM];
    HANDLE found = NULL;

    if (!hero_ipc_object(name, wide)) {
        return -1;
    }
    switch (type) {
    case HERO_IPC_TYPE_MEMORY:
        found = OpenFileMappingW(FILE_MAP_READ, FALSE, wide);
        break;
    case HERO_IPC_TYPE_SEMAPHORE:
        found = OpenSemaphoreW(SYNCHRONIZE, FALSE, wide);
        break;
    default:
        found = OpenMutexW(SYNCHRONIZE, FALSE, wide);
        break;
    }
    if (found == NULL) {
        hero_ipc_failed_system();
        return -1;
    }
    (void)CloseHandle(found);
    hero_ipc_clear();
    return 0;
}

int64_t hero_ipc_memory_remove(const char *name) {
    return hero_ipc_remove_object(name, HERO_IPC_TYPE_MEMORY);
}

#else

/* `/<name>`, POSIX's name for the object, into `out`; false with
 * `HERO_IPC_INVALID`. */
static bool hero_ipc_object(const char *name, char *out, size_t room) {
    if (!hero_ipc_named(name, HERO_IPC_NAME_MOST) || !hero_ipc_joined(out, room, "/", name)) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return false;
    }
    return true;
}

/* The object `fd` names, mapped whole, its size the system's; NULL with the
 * failure named. `fd` is closed either way, and where `made` the name is
 * taken away again on a failure. */
static hero_ipc_memory *hero_ipc_memory_mapped(int fd, const char *object, bool made) {
    struct stat about;
    int64_t kind = HERO_IPC_OK;
    int64_t os = 0;

    if (fstat(fd, &about) != 0) {
        hero_ipc_failed_system();
        kind = hero_ipc_kind;
        os = hero_ipc_os;
    } else if (about.st_size <= 0) {
        /* Its maker has not sized it yet. */
        kind = HERO_IPC_NOT_FOUND;
    }
    void *base = MAP_FAILED;

    if (kind == HERO_IPC_OK) {
        base = mmap(NULL, (size_t)about.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (base == MAP_FAILED) {
            hero_ipc_failed_system();
            kind = hero_ipc_kind;
            os = hero_ipc_os;
        }
    }
    (void)close(fd);
    if (kind != HERO_IPC_OK) {
        if (made) {
            (void)shm_unlink(object);
        }
        hero_ipc_fail(kind, os);
        return NULL;
    }
    return hero_ipc_memory_issue(object, made, (unsigned char *)base, (uint64_t)about.st_size);
}

hero_ipc_memory *hero_ipc_memory_create(const char *name, int64_t size, int64_t perm) {
    char object[HERO_IPC_NAME_MOST + 2];

    if (size <= 0 || perm < 0 || perm > 0777) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return NULL;
    }
    if (!hero_ipc_object(name, object, sizeof object)) {
        return NULL;
    }
    int fd = shm_open(object, O_RDWR | O_CREAT | O_EXCL, (mode_t)perm);

    if (fd < 0) {
        hero_ipc_failed_system();
        return NULL;
    }
    int said = ftruncate(fd, (off_t)size) == 0 ? 0 : errno;
#if defined(__linux__)
    /* The pages reserved now, so a machine with no room fails here and never
     * at a write, which would end the program with SIGBUS; a file system
     * that cannot reserve is left as it is. */
    if (said == 0) {
        int reserved = posix_fallocate(fd, 0, (off_t)size);

        if (reserved == ENOSPC || reserved == EFBIG || reserved == ENOMEM) {
            said = reserved;
        }
    }
#endif
    if (said != 0) {
        (void)close(fd);
        (void)shm_unlink(object);
        hero_ipc_failed_with((int64_t)said);
        return NULL;
    }
    return hero_ipc_memory_mapped(fd, object, true);
}

hero_ipc_memory *hero_ipc_memory_open(const char *name) {
    char object[HERO_IPC_NAME_MOST + 2];

    if (!hero_ipc_object(name, object, sizeof object)) {
        return NULL;
    }
    int fd = shm_open(object, O_RDWR, 0);

    if (fd < 0) {
        hero_ipc_failed_system();
        return NULL;
    }
    return hero_ipc_memory_mapped(fd, object, false);
}

int64_t hero_ipc_memory_remove(const char *name) {
    char object[HERO_IPC_NAME_MOST + 2];

    if (!hero_ipc_object(name, object, sizeof object)) {
        return -1;
    }
    if (shm_unlink(object) != 0) {
        hero_ipc_failed_system();
        return -1;
    }
    hero_ipc_clear();
    return 0;
}

#endif

int64_t hero_ipc_memory_size(hero_ipc_memory *m) {
    hero_ipc_enter();
    hero_ipc_slot *slot = hero_ipc_find(m, HERO_IPC_TYPE_MEMORY);
    uint64_t length = slot == NULL ? 0 : slot->length;
    hero_ipc_leave();

    if (slot == NULL) {
        hero_ipc_fail(HERO_IPC_NOT_OPEN, 0);
        return -1;
    }
    hero_ipc_clear();
    return (int64_t)length;
}

/* The memory `m` names, busy, where `count` bytes from `offset` stand inside
 * it and, for a word, `offset` is a multiple of `align`; NULL with the
 * failure named, nothing busy. */
static hero_ipc_slot *hero_ipc_memory_span(hero_ipc_memory *m, int64_t offset, uint64_t count, int64_t align) {
    hero_ipc_slot *slot = hero_ipc_take(m, HERO_IPC_TYPE_MEMORY);

    if (slot == NULL) {
        return NULL;
    }
    if (offset < 0 || (uint64_t)offset > slot->length || count > slot->length - (uint64_t)offset || offset % align != 0) {
        hero_ipc_drop(slot);
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return NULL;
    }
    return slot;
}

int64_t hero_ipc_memory_read(hero_ipc_memory *m, int64_t offset, void *buffer, uint64_t count) {
    if (buffer == NULL && count > 0) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return -1;
    }
    hero_ipc_slot *slot = hero_ipc_memory_span(m, offset, count, 1);

    if (slot == NULL) {
        return -1;
    }
    if (count > 0) {
        memcpy(buffer, slot->base + offset, (size_t)count);
    }
    hero_ipc_drop(slot);
    hero_ipc_clear();
    return (int64_t)count;
}

int64_t hero_ipc_memory_write(hero_ipc_memory *m, int64_t offset, const void *data, uint64_t count) {
    if (data == NULL && count > 0) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return -1;
    }
    hero_ipc_slot *slot = hero_ipc_memory_span(m, offset, count, 1);

    if (slot == NULL) {
        return -1;
    }
    if (count > 0) {
        memcpy(slot->base + offset, data, (size_t)count);
    }
    hero_ipc_drop(slot);
    hero_ipc_clear();
    return (int64_t)count;
}

/* The word at `offset` of a busy slot, an address this file computed from an
 * offset `hero_ipc_memory_span` held inside the mapping and aligned. */
static int64_t *hero_ipc_word(hero_ipc_slot *slot, int64_t offset) {
    return (int64_t *)(void *)(slot->base + offset);
}

int64_t hero_ipc_memory_load(hero_ipc_memory *m, int64_t offset, int64_t *value) {
    if (value == NULL) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return -1;
    }
    hero_ipc_slot *slot = hero_ipc_memory_span(m, offset, 8, 8);

    if (slot == NULL) {
        return -1;
    }
    *value = __atomic_load_n(hero_ipc_word(slot, offset), __ATOMIC_SEQ_CST);
    hero_ipc_drop(slot);
    hero_ipc_clear();
    return 0;
}

int64_t hero_ipc_memory_store(hero_ipc_memory *m, int64_t offset, int64_t value) {
    hero_ipc_slot *slot = hero_ipc_memory_span(m, offset, 8, 8);

    if (slot == NULL) {
        return -1;
    }
    __atomic_store_n(hero_ipc_word(slot, offset), value, __ATOMIC_SEQ_CST);
    hero_ipc_drop(slot);
    hero_ipc_clear();
    return 0;
}

int64_t hero_ipc_memory_add(hero_ipc_memory *m, int64_t offset, int64_t delta, int64_t *value) {
    if (value == NULL) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return -1;
    }
    hero_ipc_slot *slot = hero_ipc_memory_span(m, offset, 8, 8);

    if (slot == NULL) {
        return -1;
    }
    /* On the word's bits as unsigned, where the sum wraps by C's own rule
     * and no sanitizer calls it an overflow; the bits are the same as a
     * signed sum in two's complement. */
    uint64_t *word = (uint64_t *)(void *)hero_ipc_word(slot, offset);
    uint64_t sum = __atomic_add_fetch(word, (uint64_t)delta, __ATOMIC_SEQ_CST);

    hero_ipc_drop(slot);
    memcpy(value, &sum, sizeof sum);
    hero_ipc_clear();
    return 0;
}

int64_t hero_ipc_memory_swap(hero_ipc_memory *m, int64_t offset, int64_t expected, int64_t desired, int64_t *found) {
    if (found == NULL) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return -1;
    }
    hero_ipc_slot *slot = hero_ipc_memory_span(m, offset, 8, 8);

    if (slot == NULL) {
        return -1;
    }
    int64_t seen = expected;

    (void)__atomic_compare_exchange_n(hero_ipc_word(slot, offset), &seen, desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    hero_ipc_drop(slot);
    *found = seen;
    hero_ipc_clear();
    return 0;
}

int64_t hero_ipc_memory_close(hero_ipc_memory *m) {
    return hero_ipc_close_slot(m, HERO_IPC_TYPE_MEMORY);
}

/* -- semaphores -------------------------------------------------------------- */

/* The most a semaphore counts at its making on every platform: macOS's
 * `SEM_VALUE_MAX`, measured 2026-11-17 (32,768 refused `EINVAL`). */
#define HERO_IPC_COUNT_MOST 32767

#if defined(_WIN32)

static hero_ipc_semaphore *hero_ipc_semaphore_issue(HANDLE semaphore) {
    hero_ipc_slot *slot = hero_ipc_issue_locked(HERO_IPC_TYPE_SEMAPHORE);

    if (slot == NULL) {
        (void)CloseHandle(semaphore);
        hero_ipc_fail(HERO_IPC_LIMIT, 0);
        return NULL;
    }
    slot->handle = semaphore;
    hero_ipc_semaphore *handle = (hero_ipc_semaphore *)hero_ipc_handle(slot);
    hero_ipc_leave();
    hero_ipc_clear();
    return handle;
}

hero_ipc_semaphore *hero_ipc_semaphore_create(const char *name, int64_t count, int64_t perm) {
    wchar_t wide[HERO_IPC_NAME_ROOM];

    if (count < 0 || count > HERO_IPC_COUNT_MOST || perm < 0 || perm > 0777) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return NULL;
    }
    if (!hero_ipc_object(name, wide)) {
        return NULL;
    }
    /* The most Windows counts is LONG's, as Linux's is int's. */
    HANDLE semaphore = CreateSemaphoreW(NULL, (LONG)count, 0x7fffffff, wide);

    if (semaphore == NULL) {
        DWORD why = GetLastError();

        hero_ipc_fail(why == ERROR_INVALID_HANDLE ? HERO_IPC_EXISTS : hero_ipc_kind_of((int64_t)why), (int64_t)why);
        return NULL;
    }
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        (void)CloseHandle(semaphore);
        hero_ipc_fail(HERO_IPC_EXISTS, (int64_t)ERROR_ALREADY_EXISTS);
        return NULL;
    }
    return hero_ipc_semaphore_issue(semaphore);
}

hero_ipc_semaphore *hero_ipc_semaphore_open(const char *name) {
    wchar_t wide[HERO_IPC_NAME_ROOM];

    if (!hero_ipc_object(name, wide)) {
        return NULL;
    }
    HANDLE semaphore = OpenSemaphoreW(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, FALSE, wide);

    if (semaphore == NULL) {
        hero_ipc_failed_system();
        return NULL;
    }
    return hero_ipc_semaphore_issue(semaphore);
}

/* Waits for the object a busy slot holds, a slice at a time: `WAIT_OBJECT_0`
 * or `WAIT_ABANDONED` once it is taken, or `WAIT_FAILED` with the failure
 * named (the kind that ended the wait among them). */
static DWORD hero_ipc_object_wait(hero_ipc_slot *slot, HANDLE object, int64_t until) {
    for (;;) {
        DWORD waited = WaitForSingleObject(object, (DWORD)hero_ipc_slice_ms(hero_ipc_slice(until)));

        if (waited == WAIT_OBJECT_0 || waited == WAIT_ABANDONED) {
            return waited;
        }
        if (waited != WAIT_TIMEOUT) {
            hero_ipc_failed_system();
            return WAIT_FAILED;
        }
        int64_t why = hero_ipc_why_end_on(slot, until);

        if (why != HERO_IPC_OK) {
            hero_ipc_fail(why, 0);
            return WAIT_FAILED;
        }
    }
}

int64_t hero_ipc_semaphore_wait(hero_ipc_semaphore *s, int64_t within_ms) {
    int64_t until = 0;

    if (!hero_ipc_deadline(within_ms, &until)) {
        return -1;
    }
    hero_ipc_slot *slot = hero_ipc_take(s, HERO_IPC_TYPE_SEMAPHORE);

    if (slot == NULL) {
        return -1;
    }
    DWORD waited = hero_ipc_object_wait(slot, slot->handle, until);
    int64_t kind = hero_ipc_kind;
    int64_t os = hero_ipc_os;

    hero_ipc_drop(slot);
    if (waited == WAIT_FAILED) {
        hero_ipc_fail(kind, os);
        return -1;
    }
    hero_ipc_clear();
    return 0;
}

int64_t hero_ipc_semaphore_post(hero_ipc_semaphore *s) {
    hero_ipc_slot *slot = hero_ipc_take(s, HERO_IPC_TYPE_SEMAPHORE);

    if (slot == NULL) {
        return -1;
    }
    BOOL posted = ReleaseSemaphore(slot->handle, 1, NULL);
    DWORD why = posted ? 0 : GetLastError();

    hero_ipc_drop(slot);
    if (!posted) {
        hero_ipc_fail(why == ERROR_TOO_MANY_POSTS ? HERO_IPC_LIMIT : hero_ipc_kind_of((int64_t)why), (int64_t)why);
        return -1;
    }
    hero_ipc_clear();
    return 0;
}

int64_t hero_ipc_semaphore_remove(const char *name) {
    return hero_ipc_remove_object(name, HERO_IPC_TYPE_SEMAPHORE);
}

#else

static hero_ipc_semaphore *hero_ipc_semaphore_issue(sem_t *semaphore) {
    hero_ipc_slot *slot = hero_ipc_issue_locked(HERO_IPC_TYPE_SEMAPHORE);

    if (slot == NULL) {
        (void)sem_close(semaphore);
        hero_ipc_fail(HERO_IPC_LIMIT, 0);
        return NULL;
    }
    slot->semaphore = semaphore;
    hero_ipc_semaphore *handle = (hero_ipc_semaphore *)hero_ipc_handle(slot);
    hero_ipc_leave();
    hero_ipc_clear();
    return handle;
}

hero_ipc_semaphore *hero_ipc_semaphore_create(const char *name, int64_t count, int64_t perm) {
    char object[HERO_IPC_NAME_MOST + 2];

    if (count < 0 || count > HERO_IPC_COUNT_MOST || perm < 0 || perm > 0777) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return NULL;
    }
    if (!hero_ipc_object(name, object, sizeof object)) {
        return NULL;
    }
    sem_t *semaphore = sem_open(object, O_CREAT | O_EXCL, (mode_t)perm, (unsigned int)count);

    if (semaphore == SEM_FAILED) {
        hero_ipc_failed_system();
        return NULL;
    }
    return hero_ipc_semaphore_issue(semaphore);
}

hero_ipc_semaphore *hero_ipc_semaphore_open(const char *name) {
    char object[HERO_IPC_NAME_MOST + 2];

    if (!hero_ipc_object(name, object, sizeof object)) {
        return NULL;
    }
    sem_t *semaphore = sem_open(object, 0);

    if (semaphore == SEM_FAILED) {
        hero_ipc_failed_system();
        return NULL;
    }
    return hero_ipc_semaphore_issue(semaphore);
}

int64_t hero_ipc_semaphore_wait(hero_ipc_semaphore *s, int64_t within_ms) {
    int64_t until = 0;
    int64_t step = HERO_IPC_STEP_FIRST_NS;

    if (!hero_ipc_deadline(within_ms, &until)) {
        return -1;
    }
    hero_ipc_slot *slot = hero_ipc_take(s, HERO_IPC_TYPE_SEMAPHORE);

    if (slot == NULL) {
        return -1;
    }
    sem_t *semaphore = slot->semaphore;

    for (;;) {
        if (sem_trywait(semaphore) == 0) {
            break;
        }
        int why = errno;

        if (why == EINTR) {
            continue;
        }
        if (why != EAGAIN) {
            hero_ipc_drop(slot);
            hero_ipc_failed_with((int64_t)why);
            return -1;
        }
        int64_t kind = hero_ipc_why_end_on(slot, until);

        if (kind != HERO_IPC_OK) {
            hero_ipc_drop(slot);
            hero_ipc_fail(kind, 0);
            return -1;
        }
#if defined(HERO_IPC_SEM_CLOCKWAIT)
        int64_t at = hero_ipc_now() + hero_ipc_slice(until);
        struct timespec t;

        t.tv_sec = (time_t)(at / INT64_C(1000000000));
        t.tv_nsec = (long)(at % INT64_C(1000000000));
        if (sem_clockwait(semaphore, CLOCK_MONOTONIC, &t) == 0) {
            break;
        }
        why = errno;
        if (why != ETIMEDOUT && why != EINTR) {
            hero_ipc_drop(slot);
            hero_ipc_failed_with((int64_t)why);
            return -1;
        }
#else
        hero_ipc_step(until, &step);
#endif
    }
    (void)step;
    hero_ipc_drop(slot);
    hero_ipc_clear();
    return 0;
}

int64_t hero_ipc_semaphore_post(hero_ipc_semaphore *s) {
    hero_ipc_slot *slot = hero_ipc_take(s, HERO_IPC_TYPE_SEMAPHORE);

    if (slot == NULL) {
        return -1;
    }
    int posted = sem_post(slot->semaphore);
    int why = errno;

    hero_ipc_drop(slot);
    if (posted != 0) {
        hero_ipc_failed_with((int64_t)why);
        return -1;
    }
    hero_ipc_clear();
    return 0;
}

int64_t hero_ipc_semaphore_remove(const char *name) {
    char object[HERO_IPC_NAME_MOST + 2];

    if (!hero_ipc_object(name, object, sizeof object)) {
        return -1;
    }
    if (sem_unlink(object) != 0) {
        hero_ipc_failed_system();
        return -1;
    }
    hero_ipc_clear();
    return 0;
}

#endif

int64_t hero_ipc_semaphore_close(hero_ipc_semaphore *s) {
    return hero_ipc_close_slot(s, HERO_IPC_TYPE_SEMAPHORE);
}

/* -- locks ------------------------------------------------------------------- */

/* Whether the calling thread is the one `slot` records. Under the lock. */
static bool hero_ipc_mine(hero_ipc_slot *slot) {
#if defined(_WIN32)
    return slot->holder == GetCurrentThreadId();
#else
    return pthread_equal(slot->holder, pthread_self()) != 0;
#endif
}

/* Whether the calling thread holds the lock `name` through any handle.
 * Under the lock. */
static bool hero_ipc_held_here(const char *name) {
    for (uint32_t i = 0; i < hero_ipc_table.fresh; i++) {
        hero_ipc_slot *slot = &hero_ipc_table.slots[i];

        if (slot->type == HERO_IPC_TYPE_LOCK && slot->held && hero_ipc_mine(slot) && strcmp(slot->name, name) == 0) {
            return true;
        }
    }
    return false;
}

/* Records the calling thread as the holder. Under the lock. */
static void hero_ipc_hold(hero_ipc_slot *slot) {
    slot->held = true;
#if defined(_WIN32)
    slot->holder = GetCurrentThreadId();
#else
    slot->holder = pthread_self();
#endif
}

/* Releases a lock the calling thread holds: the mark cleared, then the
 * system's lock; 0, or the platform's number of a failure (the lock is
 * released whatever the mark's write answered). Under the lock. */
static int64_t hero_ipc_unhold(hero_ipc_slot *slot) {
    int64_t said = 0;

    slot->held = false;
#if defined(_WIN32)
    if (!ReleaseMutex(slot->handle)) {
        said = (int64_t)GetLastError();
    }
#else
    unsigned char free_mark = 0;

    if (pwrite(slot->fd, &free_mark, 1, 0) != 1) {
        said = (int64_t)errno;
    }
    if (flock(slot->fd, LOCK_UN) != 0 && said == 0) {
        said = (int64_t)errno;
    }
#endif
    return said;
}

#if defined(_WIN32)

hero_ipc_lock *hero_ipc_lock_open(const char *name, int64_t perm) {
    wchar_t wide[HERO_IPC_NAME_ROOM];

    if (perm < 0 || perm > 0777) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return NULL;
    }
    if (!hero_ipc_object(name, wide)) {
        return NULL;
    }
    HANDLE mutex = CreateMutexW(NULL, FALSE, wide);

    if (mutex == NULL) {
        DWORD why = GetLastError();

        hero_ipc_fail(why == ERROR_INVALID_HANDLE ? HERO_IPC_EXISTS : hero_ipc_kind_of((int64_t)why), (int64_t)why);
        return NULL;
    }
    hero_ipc_slot *slot = hero_ipc_issue_locked(HERO_IPC_TYPE_LOCK);

    if (slot == NULL) {
        (void)CloseHandle(mutex);
        hero_ipc_fail(HERO_IPC_LIMIT, 0);
        return NULL;
    }
    slot->handle = mutex;
    memcpy(slot->name, name, strlen(name) + 1);
    hero_ipc_lock *handle = (hero_ipc_lock *)hero_ipc_handle(slot);
    hero_ipc_leave();
    hero_ipc_clear();
    return handle;
}

int64_t hero_ipc_lock_acquire(hero_ipc_lock *l, int64_t within_ms) {
    int64_t until = 0;

    if (!hero_ipc_deadline(within_ms, &until)) {
        return -1;
    }
    hero_ipc_slot *slot = hero_ipc_take(l, HERO_IPC_TYPE_LOCK);

    if (slot == NULL) {
        return -1;
    }
    hero_ipc_enter();
    bool own = hero_ipc_held_here(slot->name);
    hero_ipc_leave();

    if (own) {
        hero_ipc_drop(slot);
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return -1;
    }
    DWORD waited = hero_ipc_object_wait(slot, slot->handle, until);

    if (waited == WAIT_FAILED) {
        int64_t kind = hero_ipc_kind;
        int64_t os = hero_ipc_os;

        hero_ipc_drop(slot);
        hero_ipc_fail(kind, os);
        return -1;
    }
    hero_ipc_enter();
    bool closing = slot->closing;

    if (closing) {
        /* Closed while this thread waited: what it took goes back. */
        (void)ReleaseMutex(slot->handle);
    } else {
        hero_ipc_hold(slot);
    }
    hero_ipc_leave();
    hero_ipc_drop(slot);
    if (closing) {
        hero_ipc_fail(HERO_IPC_NOT_OPEN, 0);
        return -1;
    }
    hero_ipc_clear();
    return waited == WAIT_ABANDONED ? 1 : 0;
}

int64_t hero_ipc_lock_remove(const char *name) {
    return hero_ipc_remove_object(name, HERO_IPC_TYPE_LOCK);
}

#else

/* The lock's file for `name` into `out`; false with `HERO_IPC_INVALID`. */
static bool hero_ipc_lock_path(const char *name, char *out, size_t room) {
    if (!hero_ipc_named(name, HERO_IPC_NAME_MOST) || !hero_ipc_joined(out, room, HERO_IPC_LOCK_PREFIX, name)) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return false;
    }
    return true;
}

hero_ipc_lock *hero_ipc_lock_open(const char *name, int64_t perm) {
    char path[HERO_IPC_NAME_ROOM];
    struct stat about;

    if (perm < 0 || perm > 0777) {
        hero_ipc_fail(HERO_IPC_INVALID, 0);
        return NULL;
    }
    if (!hero_ipc_lock_path(name, path, sizeof path)) {
        return NULL;
    }
    /* No link followed at the last name, and none written through: a file
     * someone else linked here would be written by the mark. */
    int fd = open(path, O_RDWR | O_CREAT | O_NOFOLLOW | O_CLOEXEC, (mode_t)perm);

    if (fd < 0) {
        hero_ipc_failed_system();
        return NULL;
    }
    if (fstat(fd, &about) != 0) {
        int why = errno;

        (void)close(fd);
        hero_ipc_failed_with((int64_t)why);
        return NULL;
    }
    if (!S_ISREG(about.st_mode) || about.st_nlink != 1) {
        (void)close(fd);
        hero_ipc_fail(HERO_IPC_DENIED, 0);
        return NULL;
    }
    hero_ipc_slot *slot = hero_ipc_issue_locked(HERO_IPC_TYPE_LOCK);

    if (slot == NULL) {
        (void)close(fd);
        hero_ipc_fail(HERO_IPC_LIMIT, 0);
        return NULL;
    }
    slot->fd = fd;
    memcpy(slot->name, name, strlen(name) + 1);
    hero_ipc_lock *handle = (hero_ipc_lock *)hero_ipc_handle(slot);
    hero_ipc_leave();
    hero_ipc_clear();
    return handle;
}

int64_t hero_ipc_lock_acquire(hero_ipc_lock *l, int64_t within_ms) {
    int64_t until = 0;
    int64_t step = HERO_IPC_STEP_FIRST_NS;

    if (!hero_ipc_deadline(within_ms, &until)) {
        return -1;
    }
    hero_ipc_slot *slot = hero_ipc_take(l, HERO_IPC_TYPE_LOCK);

    if (slot == NULL) {
        return -1;
    }
    for (;;) {
        int64_t answer = -1;
        int64_t kind = HERO_IPC_OK;
        int64_t os = 0;

        /* One try, whole, under the table's lock: no other thread of this
         * process sees the file locked and the slot not yet held. */
        hero_ipc_enter();
        if (slot->closing) {
            kind = HERO_IPC_NOT_OPEN;
        } else if (hero_ipc_held_here(slot->name)) {
            kind = HERO_IPC_INVALID;
        } else if (!slot->held) {
            if (flock(slot->fd, LOCK_EX | LOCK_NB) == 0) {
                unsigned char mark = 0;
                ssize_t got = pread(slot->fd, &mark, 1, 0);
                unsigned char held_mark = HERO_IPC_HELD_MARK;

                if (got < 0 || pwrite(slot->fd, &held_mark, 1, 0) != 1) {
                    /* No mark, no hold: the next holder could not be told. */
                    os = (int64_t)errno;
                    kind = hero_ipc_kind_of(os);
                    (void)flock(slot->fd, LOCK_UN);
                } else {
                    hero_ipc_hold(slot);
                    answer = got == 1 && mark == HERO_IPC_HELD_MARK ? 1 : 0;
                }
            } else if (errno != EWOULDBLOCK && errno != EINTR) {
                os = (int64_t)errno;
                kind = hero_ipc_kind_of(os);
            }
        }
        hero_ipc_leave();
        if (answer >= 0 || kind != HERO_IPC_OK) {
            hero_ipc_drop(slot);
            if (kind != HERO_IPC_OK) {
                hero_ipc_fail(kind, os);
                return -1;
            }
            hero_ipc_clear();
            return answer;
        }
        kind = hero_ipc_why_end(until);
        if (kind != HERO_IPC_OK) {
            hero_ipc_drop(slot);
            hero_ipc_fail(kind, 0);
            return -1;
        }
        hero_ipc_step(until, &step);
    }
}

int64_t hero_ipc_lock_remove(const char *name) {
    char path[HERO_IPC_NAME_ROOM];

    if (!hero_ipc_lock_path(name, path, sizeof path)) {
        return -1;
    }
    if (unlink(path) != 0) {
        hero_ipc_failed_system();
        return -1;
    }
    hero_ipc_clear();
    return 0;
}

#endif

int64_t hero_ipc_lock_release(hero_ipc_lock *l) {
    hero_ipc_enter();
    hero_ipc_slot *slot = hero_ipc_find(l, HERO_IPC_TYPE_LOCK);
    int64_t kind = HERO_IPC_OK;
    int64_t said = 0;

    if (slot == NULL) {
        kind = HERO_IPC_NOT_OPEN;
    } else if (!slot->held || !hero_ipc_mine(slot)) {
        kind = HERO_IPC_NOT_HELD;
    } else {
        said = hero_ipc_unhold(slot);
    }
    hero_ipc_leave();
    if (kind != HERO_IPC_OK) {
        hero_ipc_fail(kind, 0);
        return -1;
    }
    if (said != 0) {
        hero_ipc_failed_with(said);
        return -1;
    }
    hero_ipc_clear();
    return 0;
}

int64_t hero_ipc_lock_close(hero_ipc_lock *l) {
    int64_t said = 0;

    hero_ipc_enter();
    hero_ipc_slot *slot = hero_ipc_find(l, HERO_IPC_TYPE_LOCK);

    if (slot != NULL && slot->held && hero_ipc_mine(slot)) {
        said = hero_ipc_unhold(slot);
    }
    hero_ipc_leave();
    int64_t closed = hero_ipc_close_slot(l, HERO_IPC_TYPE_LOCK);

    if (closed == 0 && said != 0) {
        hero_ipc_failed_with(said);
        return -1;
    }
    return closed;
}

/* -- giving a slot's things back --------------------------------------------- */

static int64_t hero_ipc_dispose(hero_ipc_slot *slot) {
    int64_t said = 0;

    switch (slot->type) {
    case HERO_IPC_TYPE_LISTENER:
#if defined(_WIN32)
        if (slot->space == HERO_IPC_SPACE_PIPE) {
            if (slot->handle != NULL && !CloseHandle(slot->handle)) {
                said = (int64_t)GetLastError();
            }
        } else {
            wchar_t wide[HERO_IPC_NAME_ROOM];

            if (closesocket(slot->socket) != 0) {
                said = (int64_t)WSAGetLastError();
            }
            /* The socket file the bind made, as Go's listener removes it. */
            if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, slot->name, -1, wide, HERO_IPC_NAME_ROOM) != 0) {
                (void)DeleteFileW(wide);
            }
        }
#else
        if (slot->space == HERO_IPC_SPACE_PATH) {
            struct stat about;

            /* Only the file the listen made: one put there since by
             * another stays. */
            if (stat(slot->name, &about) == 0 && about.st_dev == slot->device && about.st_ino == slot->inode) {
                (void)unlink(slot->name);
            }
        }
        if (close(slot->fd) != 0 && errno != EINTR) {
            said = (int64_t)errno;
        }
#endif
        break;
    case HERO_IPC_TYPE_MEMORY:
#if defined(_WIN32)
        if (!UnmapViewOfFile(slot->base)) {
            said = (int64_t)GetLastError();
        }
        (void)CloseHandle(slot->handle);
#else
        if (munmap(slot->base, (size_t)slot->length) != 0) {
            said = (int64_t)errno;
        }
#endif
        slot->base = NULL;
        slot->length = 0;
        break;
    case HERO_IPC_TYPE_SEMAPHORE:
#if defined(_WIN32)
        if (!CloseHandle(slot->handle)) {
            said = (int64_t)GetLastError();
        }
#else
        if (sem_close(slot->semaphore) != 0) {
            said = (int64_t)errno;
        }
        slot->semaphore = NULL;
#endif
        break;
    default:
        /* A lock: closing what holds it releases it on POSIX, its mark left
         * for the next holder to read as abandoned where another thread
         * held it; on Windows the mutex stays its holder thread's. */
#if defined(_WIN32)
        if (!CloseHandle(slot->handle)) {
            said = (int64_t)GetLastError();
        }
#else
        if (close(slot->fd) != 0 && errno != EINTR) {
            said = (int64_t)errno;
        }
#endif
        slot->held = false;
        break;
    }
    return said;
}
