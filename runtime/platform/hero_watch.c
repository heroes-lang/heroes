/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_watch.c — the platform layer's watches, one member of the
 * archive the compiler puts on every program's link line
 * (`selfhost/cli/layer.hero` builds, keys and links it). Its header,
 * `hero_watch.h`, says what each function answers; this file says how each
 * platform gets there. Written from the systems' own pages (`inotify(7)`,
 * Apple's File System Events reference, Microsoft's `ReadDirectoryChangesW`
 * and `ReadDirectoryChangesExW`), never from another library's source.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so an arm that
 * defines another signature than the header's is `conflicting types`.
 *
 * ONE SHAPE ON EVERY SYSTEM. A watch is a slot of `hero_watch_slots`, its
 * handle the slot's index and the life it held when issued, as `hero_poll.c`'s
 * sets are, with the table's tag (`hero_handle_tag.h`). Behind the slot a
 * state the layer allocates (`hero_layer_alloc.h`) holds the watched path, a
 * bounded queue of events and the bell; a SOURCE reads the system's changes
 * into the queue: a thread of the member's own on Linux and Windows, FSEvents'
 * dispatch queue on macOS. The program's thread takes events from the queue
 * in `hero_watch_next`, waiting on one condition the member broadcasts at
 * every event, close and stop.
 *
 * ONE LOCK, AND NO OTHER MEMBER CALLED UNDER IT. The slots and every queue
 * stand under `hero_watch_lock`; while it is held nothing of `hero_net.h` or
 * `hero_stream.h` is called (the bell is rung and emptied by the system's own
 * `write`, `read`, `sendto` and `recv` on descriptors held for the watch's
 * life), so the hook `hero_net.c` calls at a stop, under its own lock, may
 * take this one: the order is that file's lock, then this one, never the
 * other way.
 *
 * MACOS: FSEVENTS, MEASURED AGAINST KQUEUE (2026-11-18, the probes in
 * `rebel-scratch/fs-watch/probe/`, on APFS). `kqueue`'s `EVFILT_VNODE` told a
 * write of one file at once and exactly (`WRITE EXTEND`, `ATTRIB`), but it
 * follows the file it opened and not its name: a file replaced by a rename
 * over it, as an editor saves, was told `DELETE` and its replacement's write
 * was told nothing; and a folder's `WRITE` names no entry, so every change
 * would cost a reading of the folder. FSEvents with file-level events named
 * every path and followed a name through its replacement and its
 * re-creation, 9 to 13 ms after each change. Its price, measured: the flags
 * of one path ACCUMULATE for about a second (a file made and then written
 * 0.3 s later was told `Created Modified`, and a third write `Created
 * Modified` again), a rename is two events of one inode in a row (the old
 * path, then the new), and a change made just before the stream began was
 * told. So this arm keeps, per path, the inode and the flags it last saw, and
 * tells what is NEW in an event's flags, reading the disk to tell whether the
 * path now stands (`hero_watch_fsevents_one`). The system watches a whole
 * tree below the folder whatever is asked, so a watch of a folder's own
 * entries, or of one file, keeps its own from the tree's events.
 *
 * LINUX: INOTIFY, one watch per folder (`inotify(7)`): a rename is
 * `IN_MOVED_FROM` and `IN_MOVED_TO` of one cookie, which the thread pairs, a
 * `MOVED_FROM` with no partner within 20 ms told as the entry moved out
 * (removed); `IN_Q_OVERFLOW` is an overflow; the watched folder's own
 * `IN_DELETE_SELF`, `IN_MOVE_SELF` or `IN_UNMOUNT` is GONE.
 *
 * WINDOWS: `ReadDirectoryChangesExW` with the extended information, which
 * says whether an entry is a folder even once it is gone (Windows 10 1709);
 * where the system has not the function or the volume refuses the form,
 * plain `ReadDirectoryChangesW`, an entry then asked of the disk, and a
 * removed one told not a folder. A buffer the system could not fill (0 bytes,
 * or `ERROR_NOTIFY_ENUM_DIR`) is an overflow; a read refused, or a watched
 * folder no longer standing at its path, is GONE.
 */

#include "hero_watch.h"

/* The layer's one list of handle tags, and its memory, after the member's
 * own header. */
#include "hero_handle_tag.h"
#include "hero_layer_alloc.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#if defined(_MSC_VER)
#pragma comment(lib, "ws2_32.lib")
#endif
#else
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#if defined(__APPLE__)
#include <CoreServices/CoreServices.h>
#include <dispatch/dispatch.h>
/* The framework the arm calls, named in the object, so that the linker links
 * it into a program that takes this member and into no other. */
__asm__(".linker_option \"-framework\", \"CoreServices\"");
__asm__(".linker_option \"-framework\", \"CoreFoundation\"");
#define HERO_WATCH_FSEVENTS 1
#elif defined(__linux__)
#include <poll.h>
#include <sys/inotify.h>
#define HERO_WATCH_INOTIFY 1
#endif
#endif

/* -- the numbers -------------------------------------------------------------- */

_Static_assert(sizeof(void *) == 8 && sizeof(uintptr_t) == 8, "a watch's handle carries its slot and its life in 64 bits");
#define HERO_WATCH_INDEX_BITS 17
#define HERO_WATCH_INDEX_MASK ((UINT64_C(1) << HERO_WATCH_INDEX_BITS) - 1)
#define HERO_WATCH_LIFE_MOST HERO_HANDLE_LIFE_MOST(HERO_WATCH_INDEX_BITS)
#define HERO_WATCH_SLOTS 1024u
/* A wait where the stop cannot be heard asks for it this often. */
#define HERO_WATCH_SLICE_NS INT64_C(50000000)
/* One wait on the condition lasts at most an hour; the limit is then read
 * again, so no system's timeout is asked past what its type holds. */
#define HERO_WATCH_CHUNK_NS (INT64_C(3600) * INT64_C(1000000000))
/* The bytes the source reads the system's changes into. */
#define HERO_WATCH_BUFFER 65536u
/* How long a Linux `IN_MOVED_FROM` waits for its `IN_MOVED_TO`. */
#define HERO_WATCH_PAIR_MS 20

/* -- the state ---------------------------------------------------------------- */

/* One event in a watch's queue: a path below the watched folder (`rel`) and,
 * for a rename, the one before (`old`), both in the block after the event's
 * own bytes; `root` for an event about the watched path itself. */
typedef struct hero_watch_event {
    struct hero_watch_event *next;
    int64_t kind;
    int64_t dir;
    size_t rel_len;
    size_t old_len;
    bool root;
    char *text;
} hero_watch_event;

#if defined(HERO_WATCH_FSEVENTS)
/* What FSEvents last told of a path: its inode and its accumulated flags. */
typedef struct {
    char *key;
    size_t len;
    uint64_t hash;
    int64_t inode;
    uint32_t flags;
} hero_watch_seen;
#endif

typedef struct hero_watch_state {
    /* The watched path as the program gave it, and what a path below it is
     * joined to: the path and a separator for a folder, the path's folder
     * part with its separator for a file. */
    char *given;
    size_t given_len;
    char *base;
    size_t base_len;
    /* A watched file's name in its folder; NULL for a folder. */
    char *name;
    size_t name_len;
    /* The folder the system watches, absolute: the file's folder for a file. */
    char *abs;
    size_t abs_len;
    bool tree;
    uint64_t most;

    /* The queue, under the lock. */
    hero_watch_event *head;
    hero_watch_event *tail;
    uint64_t count;
    /* GONE is queued: nothing more is. */
    bool gone_told;
    /* GONE was taken: a wait answers `HERO_WATCH_ENDED` once the queue is
     * empty. */
    bool ended;
    hero_watch_event spare_overflow;
    hero_watch_event spare_gone;
    bool spare_overflow_used;
    bool spare_gone_used;

    /* The bell, made at the first ask, under the lock once made. */
    bool bell;
    bool rung;
#if defined(_WIN32)
    hero_net_socket *bell_socket;
    int64_t bell_hold;
    SOCKET bell_fd;
    struct sockaddr_in bell_at;
#else
    hero_stream *bell_in;
    hero_stream *bell_out;
    int bell_in_fd;
    int bell_out_fd;
#endif

    /* The source. */
#if defined(HERO_WATCH_INOTIFY)
    int inotify;
    int wake[2];
    pthread_t thread;
    bool thread_made;
    /* The folders watched: `wd_keys` sorted, each beside its path below the
     * watched folder. */
    int *wd_keys;
    char **wd_rels;
    size_t wd_count;
    size_t wd_cap;
    int root_wd;
    /* A `MOVED_FROM` waiting for its `MOVED_TO`. */
    bool moving;
    uint32_t moving_cookie;
    bool moving_dir;
    char *moving_rel;
    char *buffer;
#elif defined(HERO_WATCH_FSEVENTS)
    FSEventStreamRef stream;
    dispatch_queue_t queue;
    /* The folder's path as the kernel names it, a `/` after it. */
    char *real;
    size_t real_len;
    dev_t root_dev;
    ino_t root_ino;
    hero_watch_seen *seen;
    size_t seen_cap;
    size_t seen_used;
#elif defined(_WIN32)
    HANDLE dir;
    HANDLE stop;
    HANDLE ready;
    HANDLE thread;
    OVERLAPPED overlapped;
    bool extended;
    DWORD start_error;
    wchar_t *abs_w;
    /* The folder's identity, to tell it still stands at its path. */
    DWORD volume;
    DWORD index_high;
    DWORD index_low;
    void *buffer;
    /* A `RENAMED_OLD_NAME` waiting for its `RENAMED_NEW_NAME`. */
    char *moving_rel;
    bool moving_dir;
#endif
} hero_watch_state;

typedef struct {
    uint64_t life;
    bool held;
    bool closing;
    uint32_t busy;
    hero_watch_state *state;
    uint32_t next;
} hero_watch_slot;

static hero_watch_slot hero_watch_slots[HERO_WATCH_SLOTS];
static uint32_t hero_watch_fresh;
static uint32_t hero_watch_given_back;
/* The stop was heard by the hook; read and written under the lock. */
static bool hero_watch_stop_heard;
/* 0: the hook not asked yet; 1: registered, so a wait hears the stop at
 * once; 2: refused, so a wait asks in slices. */
static _Atomic int hero_watch_hook;

static _Thread_local int64_t hero_watch_kind;
static _Thread_local int64_t hero_watch_os;

#if defined(_WIN32)
static SRWLOCK hero_watch_lock = SRWLOCK_INIT;
static CONDITION_VARIABLE hero_watch_cond = CONDITION_VARIABLE_INIT;
#elif defined(__APPLE__)
static pthread_mutex_t hero_watch_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t hero_watch_cond = PTHREAD_COND_INITIALIZER;
#else
static pthread_mutex_t hero_watch_lock = PTHREAD_MUTEX_INITIALIZER;
/* INITIALISED IN PLACE, ONCE, never copied (defect 2601). POSIX lets only the
 * condition object itself be used to wait and to wake, and leaves a copy's
 * behaviour undefined (`pthread_cond_init`'s page, *"Only cond itself may be
 * used for performing synchronization"*), and initialising one already
 * initialised is undefined too. Until then this arm initialised a local
 * condition with the monotonic clock and assigned it over the static one,
 * which `PTHREAD_COND_INITIALIZER` had already initialised. So the static one
 * has no initialiser, and `hero_watch_cond_made` initialises it where it
 * stands, once, before any wait or wake can reach it: every way in passes
 * `hero_watch_up`'s `pthread_once` first (`hero_watch_open`, whose handle is
 * the only way to the others). Which clock its waits are read on is kept
 * beside it, since a system that refuses the monotonic one leaves it on
 * C's realtime clock, and a deadline read on the other clock would end every
 * wait at once; neither initialisation standing, a watch is refused
 * `HERO_WATCH_LIMIT` and nothing waits on it. Written once inside the
 * `pthread_once`, and read after it. */
static pthread_cond_t hero_watch_cond;
static pthread_once_t hero_watch_cond_once = PTHREAD_ONCE_INIT;
static _Atomic bool hero_watch_cond_ready;
static _Atomic clockid_t hero_watch_cond_clock = CLOCK_REALTIME;
#endif

static void hero_watch_enter(void) {
#if defined(_WIN32)
    AcquireSRWLockExclusive(&hero_watch_lock);
#else
    (void)pthread_mutex_lock(&hero_watch_lock);
#endif
}

static void hero_watch_leave(void) {
#if defined(_WIN32)
    ReleaseSRWLockExclusive(&hero_watch_lock);
#else
    (void)pthread_mutex_unlock(&hero_watch_lock);
#endif
}

static void hero_watch_broadcast(void) {
#if defined(_WIN32)
    WakeAllConditionVariable(&hero_watch_cond);
#else
    (void)pthread_cond_broadcast(&hero_watch_cond);
#endif
}

#if !defined(_WIN32) && !defined(__APPLE__)
/* The condition initialised where it stands, read on a clock that never goes
 * back where the system lets a condition choose its clock, else on C's
 * realtime clock; `hero_watch_cond_ready` says whether either stands. Run
 * once, by `pthread_once`. */
static void hero_watch_cond_made(void) {
    pthread_condattr_t attr;
    bool made = false;

    if (pthread_condattr_init(&attr) == 0) {
        if (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC) == 0 && pthread_cond_init(&hero_watch_cond, &attr) == 0) {
            atomic_store(&hero_watch_cond_clock, CLOCK_MONOTONIC);
            made = true;
        }
        (void)pthread_condattr_destroy(&attr);
    }
    if (!made && pthread_cond_init(&hero_watch_cond, NULL) == 0) {
        atomic_store(&hero_watch_cond_clock, CLOCK_REALTIME);
        made = true;
    }
    atomic_store(&hero_watch_cond_ready, made);
}
#endif

/* Waits on the condition at most `ns` nanoseconds, under the lock, which it
 * lets go while it waits; a wakeup may come early or for another watch, and
 * the caller asks again. */
static void hero_watch_sleep(int64_t ns) {
    if (ns > HERO_WATCH_CHUNK_NS) {
        ns = HERO_WATCH_CHUNK_NS;
    }
#if defined(_WIN32)
    int64_t ms = ns / INT64_C(1000000) + (ns % INT64_C(1000000) != 0 ? 1 : 0);

    (void)SleepConditionVariableSRW(&hero_watch_cond, &hero_watch_lock, (DWORD)ms, 0);
#elif defined(__APPLE__)
    struct timespec span;

    span.tv_sec = (time_t)(ns / INT64_C(1000000000));
    span.tv_nsec = (long)(ns % INT64_C(1000000000));
    (void)pthread_cond_timedwait_relative_np(&hero_watch_cond, &hero_watch_lock, &span);
#else
    struct timespec at;

    /* The deadline on the clock the condition was made with (defect 2601). */
    if (clock_gettime(atomic_load(&hero_watch_cond_clock), &at) != 0) {
        at.tv_sec = 0;
        at.tv_nsec = 0;
    }
    int64_t nanos = (int64_t)at.tv_nsec + ns % INT64_C(1000000000);

    at.tv_sec += (time_t)(ns / INT64_C(1000000000) + nanos / INT64_C(1000000000));
    at.tv_nsec = (long)(nanos % INT64_C(1000000000));
    (void)pthread_cond_timedwait(&hero_watch_cond, &hero_watch_lock, &at);
#endif
}

static void hero_watch_fail(int64_t kind, int64_t os) {
    hero_watch_kind = kind;
    hero_watch_os = os;
}

static void hero_watch_clear(void) {
    hero_watch_fail(HERO_WATCH_OK, 0);
}

/* Nanoseconds on a clock that never goes back, or -1. */
static int64_t hero_watch_now(void) {
#if defined(_WIN32)
    LARGE_INTEGER count;
    LARGE_INTEGER frequency;

    if (!QueryPerformanceFrequency(&frequency) || !QueryPerformanceCounter(&count) || frequency.QuadPart <= 0 || count.QuadPart < 0) {
        return -1;
    }
    int64_t whole = count.QuadPart / frequency.QuadPart;
    int64_t part = count.QuadPart % frequency.QuadPart;
    int64_t per = frequency.QuadPart;
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
    int64_t whole = (int64_t)t.tv_sec;
    int64_t part = (int64_t)t.tv_nsec;
    int64_t per = INT64_C(1000000000);
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

/* The kind a system's number is, for a path asked of it. */
static int64_t hero_watch_kind_of(int64_t code) {
    switch (code) {
#if defined(_WIN32)
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
    case ERROR_BAD_NETPATH:
    case ERROR_BAD_NET_NAME:
    case ERROR_INVALID_DRIVE:
        return HERO_WATCH_NOT_FOUND;
    case ERROR_ACCESS_DENIED:
    case ERROR_SHARING_VIOLATION:
        return HERO_WATCH_DENIED;
    case ERROR_DIRECTORY:
        return HERO_WATCH_NOT_A_DIRECTORY;
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
    case ERROR_TOO_MANY_OPEN_FILES:
    case ERROR_FILENAME_EXCED_RANGE:
    case ERROR_NO_SYSTEM_RESOURCES:
        return HERO_WATCH_LIMIT;
    case ERROR_INVALID_FUNCTION:
    case ERROR_NOT_SUPPORTED:
        return HERO_WATCH_UNSUPPORTED;
    case ERROR_INVALID_NAME:
        return HERO_WATCH_INVALID;
#else
    case ENOENT:
        return HERO_WATCH_NOT_FOUND;
    case EACCES:
    case EPERM:
        return HERO_WATCH_DENIED;
    case ENOTDIR:
        return HERO_WATCH_NOT_A_DIRECTORY;
    case EMFILE:
    case ENFILE:
    case ENOMEM:
    case ENOSPC:
    case EAGAIN:
    case ENAMETOOLONG:
        return HERO_WATCH_LIMIT;
    case ENOTSUP:
#if defined(EOPNOTSUPP) && EOPNOTSUPP != ENOTSUP
    case EOPNOTSUPP:
#endif
        return HERO_WATCH_UNSUPPORTED;
#endif
    default:
        return HERO_WATCH_OTHER;
    }
}

/* -- text ------------------------------------------------------------------- */

/* A copy of `n` bytes with a zero after them, or NULL. */
static char *hero_watch_copy(const char *s, size_t n) {
    char *out = hero_layer_alloc(n + 1);

    if (out != NULL) {
        if (n > 0) {
            memcpy(out, s, n);
        }
        out[n] = 0;
    }
    return out;
}

/* `a`, then `sep` where both are not empty, then `b`, as a new text, or NULL. */
static char *hero_watch_join(const char *a, size_t a_len, char sep, const char *b, size_t b_len) {
    bool both = a_len > 0 && b_len > 0;
    size_t n = a_len + (both ? 1u : 0u) + b_len;
    char *out = hero_layer_alloc(n + 1);

    if (out == NULL) {
        return NULL;
    }
    if (a_len > 0) {
        memcpy(out, a, a_len);
    }
    if (both) {
        out[a_len] = sep;
    }
    if (b_len > 0) {
        memcpy(out + a_len + (both ? 1u : 0u), b, b_len);
    }
    out[n] = 0;
    return out;
}

static bool hero_watch_is_sep(char c) {
#if defined(_WIN32)
    return c == '\\' || c == '/';
#else
    return c == '/';
#endif
}

#if defined(_WIN32)
#define HERO_WATCH_SEP '\\'
#else
#define HERO_WATCH_SEP '/'
#endif

/* Whether a name read from the system is the watched file's. */
static bool hero_watch_same_name(const hero_watch_state *st, const char *rel, size_t len) {
    if (len != st->name_len) {
        return false;
    }
#if defined(_WIN32)
    for (size_t i = 0; i < len; i += 1) {
        char a = rel[i];
        char b = st->name[i];

        if (a >= 'A' && a <= 'Z') {
            a = (char)(a - 'A' + 'a');
        }
        if (b >= 'A' && b <= 'Z') {
            b = (char)(b - 'A' + 'a');
        }
        if (a != b) {
            return false;
        }
    }
    return true;
#else
    return memcmp(rel, st->name, len) == 0;
#endif
}

/* Whether an entry's path below the watched folder is one the watch tells:
 * the file's name for a file, an entry of the folder's own for a folder, any
 * entry for a tree. */
static bool hero_watch_in_scope(const hero_watch_state *st, const char *rel, size_t len) {
    if (len == 0) {
        return false;
    }
    if (st->name != NULL) {
        return hero_watch_same_name(st, rel, len);
    }
    if (st->tree) {
        return true;
    }
    for (size_t i = 0; i < len; i += 1) {
        if (hero_watch_is_sep(rel[i])) {
            return false;
        }
    }
    return true;
}

/* -- the queue -------------------------------------------------------------- */

static void hero_watch_event_free(hero_watch_state *st, hero_watch_event *e) {
    if (e == NULL) {
        return;
    }
    if (e == &st->spare_overflow) {
        st->spare_overflow_used = false;
    } else if (e == &st->spare_gone) {
        st->spare_gone_used = false;
    } else {
        hero_layer_free(e);
    }
}

/* The bell rung where it is made and not rung yet. Under the lock. */
static void hero_watch_ring(hero_watch_state *st) {
    if (!st->bell || st->rung) {
        return;
    }
#if defined(_WIN32)
    if (sendto(st->bell_fd, "\1", 1, 0, (const struct sockaddr *)&st->bell_at, (int)sizeof st->bell_at) == 1) {
        st->rung = true;
    }
#else
    char one = 1;

    if (write(st->bell_out_fd, &one, 1) == 1) {
        st->rung = true;
    }
#endif
}

/* The bell emptied where it was rung and the queue is empty. Under the lock. */
static void hero_watch_hush(hero_watch_state *st) {
    if (!st->bell || st->head != NULL) {
        return;
    }
#if defined(_WIN32)
    /* Every datagram there, a stranger's among them: the socket is the
     * layer's, non-blocking. */
    char drop[64];

    while (recv(st->bell_fd, drop, (int)sizeof drop, 0) >= 0) {
    }
    st->rung = false;
#else
    if (st->rung) {
        char drop[8];

        while (read(st->bell_in_fd, drop, sizeof drop) > 0) {
        }
        st->rung = false;
    }
#endif
}

/* Puts one event at the queue's end. The event's block is allocated before
 * the lock is taken; where it could not be, or the queue holds `most`, the
 * event is lost and one overflow stands where it would have, never two in a
 * row. GONE goes in whatever the queue holds, once, and after it nothing. */
static void hero_watch_push(hero_watch_state *st, int64_t kind, bool dir, const char *rel, size_t rel_len, const char *old, size_t old_len, bool root) {
    hero_watch_event *e = hero_layer_alloc(sizeof *e + rel_len + old_len);

    if (e != NULL) {
        e->next = NULL;
        e->kind = kind;
        e->dir = dir ? 1 : 0;
        e->rel_len = rel_len;
        e->old_len = old_len;
        e->root = root;
        e->text = (char *)(e + 1);
        if (rel_len > 0) {
            memcpy(e->text, rel, rel_len);
        }
        if (old_len > 0) {
            memcpy(e->text + rel_len, old, old_len);
        }
    }
    hero_watch_enter();
    if (st->gone_told) {
        hero_watch_leave();
        hero_layer_free(e);
        return;
    }
    if (kind == HERO_WATCH_GONE) {
        if (e == NULL) {
            e = &st->spare_gone;
            st->spare_gone_used = true;
            e->next = NULL;
            e->kind = HERO_WATCH_GONE;
            e->dir = 0;
            e->rel_len = 0;
            e->old_len = 0;
            e->root = true;
            e->text = NULL;
        }
        st->gone_told = true;
    } else {
        /* A write told again before the program took the first is one. */
        if (e != NULL && kind == HERO_WATCH_CHANGED && st->tail != NULL && st->tail->kind == HERO_WATCH_CHANGED && st->tail->rel_len == rel_len &&
            (rel_len == 0 || memcmp(st->tail->text, rel, rel_len) == 0)) {
            hero_watch_leave();
            hero_layer_free(e);
            return;
        }
        if (e == NULL || st->count >= st->most || kind == HERO_WATCH_OVERFLOW) {
            if (st->tail != NULL && st->tail->kind == HERO_WATCH_OVERFLOW) {
                hero_watch_leave();
                hero_layer_free(e);
                return;
            }
            if (e == NULL) {
                if (st->spare_overflow_used) {
                    hero_watch_leave();
                    return;
                }
                e = &st->spare_overflow;
                st->spare_overflow_used = true;
                e->text = NULL;
            }
            e->next = NULL;
            e->kind = HERO_WATCH_OVERFLOW;
            e->dir = 0;
            e->rel_len = 0;
            e->old_len = 0;
            e->root = true;
        }
    }
    if (st->tail == NULL) {
        st->head = e;
    } else {
        st->tail->next = e;
    }
    st->tail = e;
    st->count += 1;
    hero_watch_ring(st);
    hero_watch_broadcast();
    hero_watch_leave();
}

/* What a source tells, kept to what the watch watches: a rename with one end
 * outside it is told as the entry leaving or coming. */
static void hero_watch_tell(hero_watch_state *st, int64_t kind, bool dir, const char *rel, size_t rel_len, const char *old, size_t old_len) {
    if (kind == HERO_WATCH_OVERFLOW || kind == HERO_WATCH_GONE) {
        hero_watch_push(st, kind, false, NULL, 0, NULL, 0, true);
        return;
    }
    if (kind == HERO_WATCH_RENAMED) {
        bool was = hero_watch_in_scope(st, old, old_len);
        bool is = hero_watch_in_scope(st, rel, rel_len);

        if (was && is) {
            hero_watch_push(st, HERO_WATCH_RENAMED, dir, rel, rel_len, old, old_len, false);
        } else if (was) {
            hero_watch_push(st, HERO_WATCH_REMOVED, dir, old, old_len, NULL, 0, false);
        } else if (is) {
            hero_watch_push(st, HERO_WATCH_CREATED, dir, rel, rel_len, NULL, 0, false);
        }
        return;
    }
    if (hero_watch_in_scope(st, rel, rel_len)) {
        hero_watch_push(st, kind, dir, rel, rel_len, NULL, 0, false);
    }
}

/* -- a folder that arrived whole, walked -------------------------------------- */

#if !defined(_WIN32)
/* Every entry below the folder `rel` told created, with its own list of what
 * is left to walk, so no tree is too deep for it; on Linux each folder found
 * is watched before it is read (`added`, NULL elsewhere). Links are told as
 * entries and never followed. */
static void hero_watch_walk(hero_watch_state *st, const char *rel, size_t rel_len, int (*added)(hero_watch_state *, const char *, size_t)) {
    size_t cap = 16;
    size_t used = 0;
    char **left = hero_layer_alloc(cap * sizeof *left);

    if (left == NULL) {
        hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
        return;
    }
    left[used] = hero_watch_copy(rel, rel_len);
    if (left[used] == NULL) {
        hero_layer_free(left);
        hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
        return;
    }
    used += 1;
    while (used > 0) {
        used -= 1;
        char *here = left[used];
        size_t here_len = strlen(here);
        char *full = hero_watch_join(st->abs, st->abs_len, '/', here, here_len);
        DIR *d = full == NULL ? NULL : opendir(full);

        if (d != NULL) {
            struct dirent *one;

            while ((one = readdir(d)) != NULL) {
                const char *n = one->d_name;
                size_t n_len = strlen(n);

                if ((n_len == 1 && n[0] == '.') || (n_len == 2 && n[0] == '.' && n[1] == '.')) {
                    continue;
                }
                bool is_dir = one->d_type == DT_DIR;

                if (one->d_type == DT_UNKNOWN) {
                    struct stat facts;

                    is_dir = fstatat(dirfd(d), n, &facts, AT_SYMLINK_NOFOLLOW) == 0 && S_ISDIR(facts.st_mode);
                }
                char *child = hero_watch_join(here, here_len, '/', n, n_len);

                if (child == NULL) {
                    hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
                    continue;
                }
                size_t child_len = strlen(child);

                if (is_dir && added != NULL) {
                    (void)added(st, child, child_len);
                }
                hero_watch_tell(st, HERO_WATCH_CREATED, is_dir, child, child_len, NULL, 0);
                if (!is_dir) {
                    hero_layer_free(child);
                    continue;
                }
                if (used == cap) {
                    char **grown = hero_layer_grow(left, cap * 2 * sizeof *left);

                    if (grown == NULL) {
                        hero_layer_free(child);
                        hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
                        continue;
                    }
                    left = grown;
                    cap *= 2;
                }
                left[used] = child;
                used += 1;
            }
            (void)closedir(d);
        }
        hero_layer_free(full);
        hero_layer_free(here);
    }
    hero_layer_free(left);
}
#endif

/* ============================================================================
 * LINUX: inotify
 * ========================================================================= */

#if defined(HERO_WATCH_INOTIFY)
#define HERO_WATCH_MASK (IN_CREATE | IN_DELETE | IN_MODIFY | IN_ATTRIB | IN_MOVED_FROM | IN_MOVED_TO | IN_DELETE_SELF | IN_MOVE_SELF | IN_ONLYDIR | IN_EXCL_UNLINK)

/* Where `wd` stands in the sorted table, or where it would. */
static size_t hero_watch_wd_at(const hero_watch_state *st, int wd) {
    size_t low = 0;
    size_t high = st->wd_count;

    while (low < high) {
        size_t mid = low + (high - low) / 2;

        if (st->wd_keys[mid] < wd) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

static const char *hero_watch_wd_rel(const hero_watch_state *st, int wd) {
    size_t at = hero_watch_wd_at(st, wd);

    return at < st->wd_count && st->wd_keys[at] == wd ? st->wd_rels[at] : NULL;
}

static void hero_watch_wd_forget(hero_watch_state *st, int wd) {
    size_t at = hero_watch_wd_at(st, wd);

    if (at < st->wd_count && st->wd_keys[at] == wd) {
        hero_layer_free(st->wd_rels[at]);
        memmove(&st->wd_keys[at], &st->wd_keys[at + 1], (st->wd_count - at - 1) * sizeof *st->wd_keys);
        memmove(&st->wd_rels[at], &st->wd_rels[at + 1], (st->wd_count - at - 1) * sizeof *st->wd_rels);
        st->wd_count -= 1;
    }
}

/* The folder `rel` watched and kept in the table. Answers 0, or the system's
 * number. Only the source's thread, or `open` before it, calls it. */
static int hero_watch_wd_add(hero_watch_state *st, const char *rel, size_t rel_len) {
    char *full = hero_watch_join(st->abs, st->abs_len, '/', rel, rel_len);

    if (full == NULL) {
        return ENOMEM;
    }
    uint32_t mask = HERO_WATCH_MASK | (rel_len > 0 ? IN_DONT_FOLLOW : 0u);
    int wd = inotify_add_watch(st->inotify, full, mask);
    int code = errno;

    hero_layer_free(full);
    if (wd < 0) {
        return code;
    }
    char *kept = hero_watch_copy(rel, rel_len);

    if (kept == NULL) {
        (void)inotify_rm_watch(st->inotify, wd);
        return ENOMEM;
    }
    size_t at = hero_watch_wd_at(st, wd);

    if (at < st->wd_count && st->wd_keys[at] == wd) {
        hero_layer_free(st->wd_rels[at]);
        st->wd_rels[at] = kept;
        return 0;
    }
    if (st->wd_count == st->wd_cap) {
        size_t cap = st->wd_cap == 0 ? 16 : st->wd_cap * 2;
        int *keys = hero_layer_grow(st->wd_keys, cap * sizeof *keys);

        if (keys == NULL) {
            hero_layer_free(kept);
            (void)inotify_rm_watch(st->inotify, wd);
            return ENOMEM;
        }
        st->wd_keys = keys;
        char **rels = hero_layer_grow(st->wd_rels, cap * sizeof *rels);

        if (rels == NULL) {
            hero_layer_free(kept);
            (void)inotify_rm_watch(st->inotify, wd);
            return ENOMEM;
        }
        st->wd_rels = rels;
        st->wd_cap = cap;
    }
    memmove(&st->wd_keys[at + 1], &st->wd_keys[at], (st->wd_count - at) * sizeof *st->wd_keys);
    memmove(&st->wd_rels[at + 1], &st->wd_rels[at], (st->wd_count - at) * sizeof *st->wd_rels);
    st->wd_keys[at] = wd;
    st->wd_rels[at] = kept;
    st->wd_count += 1;
    if (rel_len == 0) {
        st->root_wd = wd;
    }
    return 0;
}

/* The `added` of a walk: a folder found is watched, and one the system will
 * not watch is an overflow, its changes lost. */
static int hero_watch_wd_added(hero_watch_state *st, const char *rel, size_t rel_len) {
    int code = hero_watch_wd_add(st, rel, rel_len);

    if (code != 0 && code != ENOENT && code != ENOTDIR) {
        hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
    }
    return code;
}

/* Whether `rel` is `top` or below it. */
static bool hero_watch_below(const char *rel, const char *top, size_t top_len) {
    size_t len = strlen(rel);

    return len >= top_len && memcmp(rel, top, top_len) == 0 && (len == top_len || rel[top_len] == '/');
}

/* The folders at and below `top` no longer watched: moved out of the tree. */
static void hero_watch_wd_drop(hero_watch_state *st, const char *top, size_t top_len) {
    size_t i = 0;

    while (i < st->wd_count) {
        if (st->wd_keys[i] != st->root_wd && hero_watch_below(st->wd_rels[i], top, top_len)) {
            int wd = st->wd_keys[i];

            (void)inotify_rm_watch(st->inotify, wd);
            hero_watch_wd_forget(st, wd);
        } else {
            i += 1;
        }
    }
}

/* The folders at and below `from` renamed to below `to`. */
static void hero_watch_wd_move(hero_watch_state *st, const char *from, size_t from_len, const char *to, size_t to_len) {
    for (size_t i = 0; i < st->wd_count; i += 1) {
        const char *rel = st->wd_rels[i];

        if (st->wd_keys[i] != st->root_wd && hero_watch_below(rel, from, from_len)) {
            size_t rest = strlen(rel) - from_len;
            char *moved = hero_layer_alloc(to_len + rest + 1);

            if (moved == NULL) {
                hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
                continue;
            }
            memcpy(moved, to, to_len);
            memcpy(moved + to_len, rel + from_len, rest);
            moved[to_len + rest] = 0;
            hero_layer_free(st->wd_rels[i]);
            st->wd_rels[i] = moved;
        }
    }
}

/* The `MOVED_FROM` waiting for its partner told as the entry moving out. */
static void hero_watch_moved_out(hero_watch_state *st) {
    if (!st->moving) {
        return;
    }
    size_t len = strlen(st->moving_rel);

    hero_watch_tell(st, HERO_WATCH_REMOVED, st->moving_dir, st->moving_rel, len, NULL, 0);
    if (st->moving_dir && st->tree) {
        hero_watch_wd_drop(st, st->moving_rel, len);
    }
    hero_layer_free(st->moving_rel);
    st->moving_rel = NULL;
    st->moving = false;
}

/* A folder that arrived in a tree, made or moved in: watched, then walked. */
static void hero_watch_arrived(hero_watch_state *st, const char *rel, size_t rel_len) {
    if (hero_watch_wd_added(st, rel, rel_len) == 0) {
        hero_watch_walk(st, rel, rel_len, hero_watch_wd_added);
    }
}

/* One event of the kernel's: `name` is `name_len` bytes, none where the event
 * is the folder's own. */
static void hero_watch_inotify_one(hero_watch_state *st, int wd, uint32_t mask, uint32_t cookie, const char *name, size_t name_len) {
    if ((mask & IN_Q_OVERFLOW) != 0) {
        hero_watch_moved_out(st);
        hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
        return;
    }
    if ((mask & IN_IGNORED) != 0) {
        if (wd == st->root_wd) {
            hero_watch_moved_out(st);
            hero_watch_tell(st, HERO_WATCH_GONE, false, NULL, 0, NULL, 0);
        }
        hero_watch_wd_forget(st, wd);
        return;
    }
    const char *dir_rel = hero_watch_wd_rel(st, wd);

    if (dir_rel == NULL) {
        return;
    }
    if ((mask & (IN_DELETE_SELF | IN_MOVE_SELF | IN_UNMOUNT)) != 0) {
        if (wd == st->root_wd) {
            hero_watch_moved_out(st);
            hero_watch_tell(st, HERO_WATCH_GONE, false, NULL, 0, NULL, 0);
        }
        return;
    }
    if (name_len == 0) {
        return;
    }
    char *rel = hero_watch_join(dir_rel, strlen(dir_rel), '/', name, name_len);

    if (rel == NULL) {
        hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
        return;
    }
    size_t rel_len = strlen(rel);
    bool dir = (mask & IN_ISDIR) != 0;

    if (st->moving && !((mask & IN_MOVED_TO) != 0 && cookie == st->moving_cookie)) {
        hero_watch_moved_out(st);
    }
    if ((mask & IN_MOVED_FROM) != 0) {
        st->moving = true;
        st->moving_cookie = cookie;
        st->moving_dir = dir;
        st->moving_rel = rel;
        return;
    }
    if ((mask & IN_MOVED_TO) != 0) {
        if (st->moving) {
            size_t old_len = strlen(st->moving_rel);

            hero_watch_tell(st, HERO_WATCH_RENAMED, dir, rel, rel_len, st->moving_rel, old_len);
            if (dir && st->tree) {
                hero_watch_wd_move(st, st->moving_rel, old_len, rel, rel_len);
            }
            hero_layer_free(st->moving_rel);
            st->moving_rel = NULL;
            st->moving = false;
        } else {
            hero_watch_tell(st, HERO_WATCH_CREATED, dir, rel, rel_len, NULL, 0);
            if (dir && st->tree) {
                hero_watch_arrived(st, rel, rel_len);
            }
        }
    } else if ((mask & IN_CREATE) != 0) {
        hero_watch_tell(st, HERO_WATCH_CREATED, dir, rel, rel_len, NULL, 0);
        if (dir && st->tree) {
            hero_watch_arrived(st, rel, rel_len);
        }
    } else if ((mask & IN_DELETE) != 0) {
        hero_watch_tell(st, HERO_WATCH_REMOVED, dir, rel, rel_len, NULL, 0);
    } else if ((mask & IN_MODIFY) != 0) {
        hero_watch_tell(st, HERO_WATCH_CHANGED, dir, rel, rel_len, NULL, 0);
    } else if ((mask & IN_ATTRIB) != 0) {
        hero_watch_tell(st, HERO_WATCH_ATTRIBUTES, dir, rel, rel_len, NULL, 0);
    }
    hero_layer_free(rel);
}

/* THE SOURCE'S THREAD: reads the kernel's events until the wake pipe is
 * written by the close. */
static void *hero_watch_inotify_loop(void *arg) {
    hero_watch_state *st = arg;

    for (;;) {
        struct pollfd ask[2];

        ask[0].fd = st->inotify;
        ask[0].events = POLLIN;
        ask[0].revents = 0;
        ask[1].fd = st->wake[0];
        ask[1].events = POLLIN;
        ask[1].revents = 0;
        int ready = poll(ask, 2, st->moving ? HERO_WATCH_PAIR_MS : -1);

        if (ready < 0) {
            if (errno == EINTR) {
                continue;
            }
            hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
            hero_watch_tell(st, HERO_WATCH_GONE, false, NULL, 0, NULL, 0);
            break;
        }
        if (ask[1].revents != 0) {
            break;
        }
        if (ready == 0) {
            hero_watch_moved_out(st);
            continue;
        }
        if ((ask[0].revents & POLLIN) == 0) {
            continue;
        }
        ssize_t got = read(st->inotify, st->buffer, HERO_WATCH_BUFFER);

        if (got <= 0) {
            continue;
        }
        size_t at = 0;
        size_t total = (size_t)got;

        while (at + sizeof(struct inotify_event) <= total) {
            struct inotify_event head;

            memcpy(&head, st->buffer + at, sizeof head);
            size_t room = total - at - sizeof head;

            if (head.len > room) {
                break;
            }
            const char *name = st->buffer + at + sizeof head;
            size_t name_len = strnlen(name, head.len);

            hero_watch_inotify_one(st, head.wd, head.mask, head.cookie, name, name_len);
            at += sizeof head + head.len;
        }
    }
    hero_watch_moved_out(st);
    return NULL;
}

/* Every folder below the watched one watched, at `open`, with a list of what
 * is left and never recursion. A folder gone meanwhile or refused to this
 * program is passed over; the system's limit of watches, or no memory, is the
 * answer. Answers 0 or the system's number. */
static int hero_watch_wd_tree(hero_watch_state *st) {
    size_t cap = 16;
    size_t used = 0;
    int code = 0;
    char **left = hero_layer_alloc(cap * sizeof *left);

    if (left == NULL || (left[0] = hero_watch_copy("", 0)) == NULL) {
        hero_layer_free(left);
        return ENOMEM;
    }
    used = 1;
    while (used > 0 && code == 0) {
        char *here = left[--used];
        size_t here_len = strlen(here);
        char *full = hero_watch_join(st->abs, st->abs_len, '/', here, here_len);
        DIR *d = full == NULL ? NULL : opendir(full);

        while (d != NULL && code == 0) {
            struct dirent *one = readdir(d);

            if (one == NULL) {
                break;
            }
            const char *n = one->d_name;
            size_t n_len = strlen(n);

            if ((n_len == 1 && n[0] == '.') || (n_len == 2 && n[0] == '.' && n[1] == '.')) {
                continue;
            }
            bool is_dir = one->d_type == DT_DIR;

            if (one->d_type == DT_UNKNOWN) {
                struct stat facts;

                is_dir = fstatat(dirfd(d), n, &facts, AT_SYMLINK_NOFOLLOW) == 0 && S_ISDIR(facts.st_mode);
            }
            if (!is_dir) {
                continue;
            }
            char *child = hero_watch_join(here, here_len, '/', n, n_len);

            if (child == NULL) {
                code = ENOMEM;
                break;
            }
            int added = hero_watch_wd_add(st, child, strlen(child));

            if (added == ENOSPC || added == ENOMEM) {
                code = added;
                hero_layer_free(child);
                break;
            }
            if (added != 0) {
                hero_layer_free(child);
                continue;
            }
            if (used == cap) {
                char **grown = hero_layer_grow(left, cap * 2 * sizeof *left);

                if (grown == NULL) {
                    hero_layer_free(child);
                    code = ENOMEM;
                    break;
                }
                left = grown;
                cap *= 2;
            }
            left[used++] = child;
        }
        if (d != NULL) {
            (void)closedir(d);
        }
        hero_layer_free(full);
        hero_layer_free(here);
    }
    while (used > 0) {
        hero_layer_free(left[--used]);
    }
    hero_layer_free(left);
    return code;
}

/* The source begun: the kernel's instance, the folders watched, the thread
 * made. Answers 0, or the system's number with nothing left begun. */
static int hero_watch_start(hero_watch_state *st) {
    st->inotify = -1;
    st->wake[0] = -1;
    st->wake[1] = -1;
    st->root_wd = -1;
    st->buffer = hero_layer_alloc(HERO_WATCH_BUFFER);
    if (st->buffer == NULL) {
        return ENOMEM;
    }
    st->inotify = inotify_init1(IN_CLOEXEC | IN_NONBLOCK);
    if (st->inotify < 0) {
        return errno;
    }
    if (pipe(st->wake) != 0) {
        return errno;
    }
    (void)fcntl(st->wake[0], F_SETFD, FD_CLOEXEC);
    (void)fcntl(st->wake[1], F_SETFD, FD_CLOEXEC);
    int code = hero_watch_wd_add(st, NULL, 0);

    if (code != 0) {
        return code;
    }
    if (st->tree) {
        code = hero_watch_wd_tree(st);
        if (code != 0) {
            return code;
        }
    }
    sigset_t all;
    sigset_t was;

    (void)sigfillset(&all);
    (void)pthread_sigmask(SIG_SETMASK, &all, &was);
    code = pthread_create(&st->thread, NULL, hero_watch_inotify_loop, st);
    (void)pthread_sigmask(SIG_SETMASK, &was, NULL);
    if (code != 0) {
        return code;
    }
    st->thread_made = true;
    return 0;
}

/* The source ended: the thread woken and joined, everything it held let go. */
static void hero_watch_stop_source(hero_watch_state *st) {
    if (st->thread_made) {
        char one = 1;
        ssize_t wrote = write(st->wake[1], &one, 1);

        (void)wrote;
        (void)pthread_join(st->thread, NULL);
        st->thread_made = false;
    }
    if (st->inotify >= 0) {
        (void)close(st->inotify);
    }
    if (st->wake[0] >= 0) {
        (void)close(st->wake[0]);
    }
    if (st->wake[1] >= 0) {
        (void)close(st->wake[1]);
    }
    for (size_t i = 0; i < st->wd_count; i += 1) {
        hero_layer_free(st->wd_rels[i]);
    }
    hero_layer_free(st->wd_keys);
    hero_layer_free(st->wd_rels);
    hero_layer_free(st->moving_rel);
    hero_layer_free(st->buffer);
}
#endif

/* ============================================================================
 * MACOS: FSEvents
 * ========================================================================= */

#if defined(HERO_WATCH_FSEVENTS)
/* The flags that say an entry's facts changed and not its content. */
#define HERO_WATCH_FACTS                                                                                                                              \
    ((uint32_t)(kFSEventStreamEventFlagItemInodeMetaMod | kFSEventStreamEventFlagItemFinderInfoMod | kFSEventStreamEventFlagItemChangeOwner |      \
                kFSEventStreamEventFlagItemXattrMod))
#define HERO_WATCH_SEEN_MOST ((size_t)1 << 20)
/* A slot of the seen table emptied by a removal: the walk goes past it. */
static char hero_watch_seen_tomb;

static uint64_t hero_watch_hash(const char *s, size_t n) {
    uint64_t h = UINT64_C(14695981039346656037);

    for (size_t i = 0; i < n; i += 1) {
        h ^= (uint64_t)(unsigned char)s[i];
        h *= UINT64_C(1099511628211);
    }
    return h;
}

/* The slot holding `key`, or NULL. Only the source's queue calls these. */
static hero_watch_seen *hero_watch_seen_get(hero_watch_state *st, const char *key, size_t len) {
    if (st->seen_cap == 0) {
        return NULL;
    }
    uint64_t h = hero_watch_hash(key, len);
    size_t mask = st->seen_cap - 1;

    for (size_t i = (size_t)h & mask, n = 0; n < st->seen_cap; i = (i + 1) & mask, n += 1) {
        hero_watch_seen *s = &st->seen[i];

        if (s->key == NULL) {
            return NULL;
        }
        if (s->key != &hero_watch_seen_tomb && s->hash == h && s->len == len && memcmp(s->key, key, len) == 0) {
            return s;
        }
    }
    return NULL;
}

static void hero_watch_seen_forget_all(hero_watch_state *st) {
    for (size_t i = 0; i < st->seen_cap; i += 1) {
        if (st->seen[i].key != NULL && st->seen[i].key != &hero_watch_seen_tomb) {
            hero_layer_free(st->seen[i].key);
        }
    }
    hero_layer_free(st->seen);
    st->seen = NULL;
    st->seen_cap = 0;
    st->seen_used = 0;
}

static void hero_watch_seen_del(hero_watch_state *st, const char *key, size_t len) {
    hero_watch_seen *s = hero_watch_seen_get(st, key, len);

    if (s != NULL) {
        hero_layer_free(s->key);
        s->key = &hero_watch_seen_tomb;
    }
}

/* `key` set to `inode` and `flags`; where no memory is left the table is
 * forgotten, which costs a later event's precision and never its truth. */
static void hero_watch_seen_put(hero_watch_state *st, const char *key, size_t len, int64_t inode, uint32_t flags) {
    hero_watch_seen *s = hero_watch_seen_get(st, key, len);

    if (s != NULL) {
        s->inode = inode;
        s->flags = flags;
        return;
    }
    if (st->seen_used + 1 > st->seen_cap / 2) {
        size_t cap = st->seen_cap == 0 ? 64 : st->seen_cap * 2;

        if (cap > HERO_WATCH_SEEN_MOST * 2) {
            hero_watch_seen_forget_all(st);
            cap = 64;
        }
        hero_watch_seen *fresh = hero_layer_alloc_zero(cap, sizeof *fresh);

        if (fresh == NULL) {
            hero_watch_seen_forget_all(st);
            return;
        }
        size_t used = 0;

        for (size_t i = 0; i < st->seen_cap; i += 1) {
            hero_watch_seen *old = &st->seen[i];

            if (old->key == NULL || old->key == &hero_watch_seen_tomb) {
                continue;
            }
            size_t j = (size_t)old->hash & (cap - 1);

            while (fresh[j].key != NULL) {
                j = (j + 1) & (cap - 1);
            }
            fresh[j] = *old;
            used += 1;
        }
        hero_layer_free(st->seen);
        st->seen = fresh;
        st->seen_cap = cap;
        st->seen_used = used;
    }
    char *kept = hero_watch_copy(key, len);

    if (kept == NULL) {
        return;
    }
    uint64_t h = hero_watch_hash(key, len);
    size_t j = (size_t)h & (st->seen_cap - 1);

    while (st->seen[j].key != NULL && st->seen[j].key != &hero_watch_seen_tomb) {
        j = (j + 1) & (st->seen_cap - 1);
    }
    if (st->seen[j].key == NULL) {
        st->seen_used += 1;
    }
    st->seen[j].key = kept;
    st->seen[j].len = len;
    st->seen[j].hash = h;
    st->seen[j].inode = inode;
    st->seen[j].flags = flags;
}

/* One FSEvents event read out of its dictionary. */
typedef struct {
    char *rel;
    size_t rel_len;
    int64_t inode;
    uint32_t flags;
    bool whole_root;
} hero_watch_raw;

/* The path of `rel` now: 1 where it stands with `inode` (or with any inode
 * where `inode` is -1), 2 where another inode stands there, 0 where nothing
 * does. `dir` is set where what stands is a folder. */
static int hero_watch_stands(const hero_watch_state *st, const char *rel, size_t rel_len, int64_t inode, bool *dir) {
    char *full = hero_watch_join(st->abs, st->abs_len, '/', rel, rel_len);
    struct stat facts;
    int answer = 0;

    if (full != NULL && lstat(full, &facts) == 0) {
        *dir = S_ISDIR(facts.st_mode);
        answer = inode < 0 || (int64_t)facts.st_ino == inode ? 1 : 2;
    }
    hero_layer_free(full);
    return answer;
}

/* Whether the watched folder still stands at its path, the same folder. */
static bool hero_watch_root_stands(const hero_watch_state *st) {
    struct stat facts;

    return stat(st->abs, &facts) == 0 && facts.st_dev == st->root_dev && facts.st_ino == st->root_ino;
}

/* What is new in one event, told: `renamed_to` is the path the event before
 * it in the batch was told renamed into, so a replaced file's own event is
 * not told again as removed. */
static void hero_watch_fsevents_one(hero_watch_state *st, const hero_watch_raw *e, const char *renamed_to, size_t renamed_to_len) {
    hero_watch_seen *was = hero_watch_seen_get(st, e->rel, e->rel_len);
    bool known = was != NULL && (e->inode < 0 || was->inode == e->inode);
    uint32_t before = known ? was->flags : 0u;
    uint32_t fresh = e->flags & ~before;
    bool dir = (e->flags & kFSEventStreamEventFlagItemIsDir) != 0;
    bool now_dir = dir;
    int stands = hero_watch_stands(st, e->rel, e->rel_len, e->inode, &now_dir);
    const uint32_t created = kFSEventStreamEventFlagItemCreated;
    const uint32_t removed = kFSEventStreamEventFlagItemRemoved;
    const uint32_t renamed = kFSEventStreamEventFlagItemRenamed;
    const uint32_t modified = kFSEventStreamEventFlagItemModified;

    if (stands == 2) {
        /* Another inode stands at the path: this event's file was replaced
         * or removed. A replacement by rename was told with the rename. */
        if (!(renamed_to != NULL && renamed_to_len == e->rel_len && memcmp(renamed_to, e->rel, e->rel_len) == 0) && known) {
            hero_watch_tell(st, HERO_WATCH_REMOVED, dir, e->rel, e->rel_len, NULL, 0);
        }
        return;
    }
    if (stands == 0) {
        if (!known && (fresh & created) != 0) {
            hero_watch_tell(st, HERO_WATCH_CREATED, dir, e->rel, e->rel_len, NULL, 0);
            if ((fresh & modified) != 0) {
                hero_watch_tell(st, HERO_WATCH_CHANGED, dir, e->rel, e->rel_len, NULL, 0);
            }
        }
        if ((e->flags & (removed | renamed)) != 0 && (known || (fresh & created) != 0 || (fresh & (removed | renamed)) != 0)) {
            hero_watch_tell(st, HERO_WATCH_REMOVED, dir, e->rel, e->rel_len, NULL, 0);
        }
        hero_watch_seen_del(st, e->rel, e->rel_len);
        return;
    }
    hero_watch_seen_put(st, e->rel, e->rel_len, e->inode, e->flags);
    if (!known && (fresh & (created | renamed)) != 0) {
        hero_watch_tell(st, HERO_WATCH_CREATED, now_dir, e->rel, e->rel_len, NULL, 0);
        if ((fresh & modified) != 0 && !now_dir) {
            hero_watch_tell(st, HERO_WATCH_CHANGED, now_dir, e->rel, e->rel_len, NULL, 0);
        }
        if (now_dir && st->tree && (fresh & created) == 0) {
            /* A folder moved in whole: its entries told too. */
            hero_watch_walk(st, e->rel, e->rel_len, NULL);
        }
        return;
    }
    if (now_dir) {
        if ((fresh & HERO_WATCH_FACTS) != 0) {
            hero_watch_tell(st, HERO_WATCH_ATTRIBUTES, true, e->rel, e->rel_len, NULL, 0);
        }
        return;
    }
    if ((fresh & modified) != 0 || (fresh == 0 && (e->flags & modified) != 0)) {
        hero_watch_tell(st, HERO_WATCH_CHANGED, false, e->rel, e->rel_len, NULL, 0);
    } else if ((fresh & HERO_WATCH_FACTS) != 0 || (fresh == 0 && (e->flags & HERO_WATCH_FACTS) != 0)) {
        hero_watch_tell(st, HERO_WATCH_ATTRIBUTES, false, e->rel, e->rel_len, NULL, 0);
    } else if (fresh == 0 && (e->flags & (created | renamed)) != 0) {
        /* Told again with nothing new: a change the flags cannot name. */
        hero_watch_tell(st, HERO_WATCH_CHANGED, false, e->rel, e->rel_len, NULL, 0);
    }
}

/* A text of the system's, as UTF-8 into a new block, or NULL. */
static char *hero_watch_cf_text(CFStringRef s, size_t *len) {
    if (s == NULL || CFGetTypeID(s) != CFStringGetTypeID()) {
        return NULL;
    }
    CFIndex units = CFStringGetLength(s);
    CFIndex most = CFStringGetMaximumSizeForEncoding(units, kCFStringEncodingUTF8);

    if (most < 0 || most > INT32_MAX - 1) {
        return NULL;
    }
    char *out = hero_layer_alloc((size_t)most + 1);

    if (out == NULL) {
        return NULL;
    }
    if (!CFStringGetCString(s, out, most + 1, kCFStringEncodingUTF8)) {
        hero_layer_free(out);
        return NULL;
    }
    *len = strlen(out);
    return out;
}

/* THE CALLBACK, on the watch's dispatch queue: a batch of events read, each
 * pair of a rename joined, and what is new told. */
static void hero_watch_fsevents_batch(ConstFSEventStreamRef ref, void *info, size_t count, void *paths, const FSEventStreamEventFlags flags[],
                                      const FSEventStreamEventId ids[]) {
    (void)ref;
    (void)ids;
    hero_watch_state *st = info;
    CFArrayRef list = paths;

    if (list == NULL || CFGetTypeID(list) != CFArrayGetTypeID() || count == 0) {
        return;
    }
    hero_watch_raw *raw = hero_layer_alloc_zero(count, sizeof *raw);

    if (raw == NULL) {
        hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
        return;
    }
    size_t kept = 0;
    CFIndex listed = CFArrayGetCount(list);

    for (size_t i = 0; i < count && (CFIndex)i < listed; i += 1) {
        uint32_t f = (uint32_t)flags[i];

        if ((f & (kFSEventStreamEventFlagMustScanSubDirs | kFSEventStreamEventFlagUserDropped | kFSEventStreamEventFlagKernelDropped)) != 0) {
            hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
            continue;
        }
        if ((f & (kFSEventStreamEventFlagRootChanged | kFSEventStreamEventFlagUnmount)) != 0) {
            if (!hero_watch_root_stands(st)) {
                hero_watch_tell(st, HERO_WATCH_GONE, false, NULL, 0, NULL, 0);
            }
            continue;
        }
        CFTypeRef item = CFArrayGetValueAtIndex(list, (CFIndex)i);
        CFStringRef text = NULL;
        int64_t inode = -1;

        if (item != NULL && CFGetTypeID(item) == CFDictionaryGetTypeID()) {
            text = CFDictionaryGetValue(item, kFSEventStreamEventExtendedDataPathKey);
            CFNumberRef number = CFDictionaryGetValue(item, kFSEventStreamEventExtendedFileIDKey);
            long long value = -1;

            if (number != NULL && CFGetTypeID(number) == CFNumberGetTypeID() && CFNumberGetValue(number, kCFNumberLongLongType, &value)) {
                inode = (int64_t)value;
            }
        } else if (item != NULL) {
            text = item;
        }
        size_t len = 0;
        char *path = hero_watch_cf_text(text, &len);

        if (path == NULL) {
            hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
            continue;
        }
        /* Below the folder, or the folder itself. */
        if (len + 1 == st->real_len && memcmp(path, st->real, len) == 0) {
            hero_layer_free(path);
            if (!hero_watch_root_stands(st)) {
                hero_watch_tell(st, HERO_WATCH_GONE, false, NULL, 0, NULL, 0);
            }
            continue;
        }
        if (len <= st->real_len || memcmp(path, st->real, st->real_len) != 0) {
            hero_layer_free(path);
            continue;
        }
        raw[kept].rel_len = len - st->real_len;
        memmove(path, path + st->real_len, raw[kept].rel_len + 1);
        raw[kept].rel = path;
        raw[kept].inode = inode;
        raw[kept].flags = f;
        kept += 1;
    }
    const uint32_t renamed = kFSEventStreamEventFlagItemRenamed;
    const char *renamed_to = NULL;
    size_t renamed_to_len = 0;

    for (size_t i = 0; i < kept; i += 1) {
        hero_watch_raw *e = &raw[i];
        hero_watch_raw *next = i + 1 < kept ? &raw[i + 1] : NULL;

        if (next != NULL && (e->flags & renamed) != 0 && (next->flags & renamed) != 0 && e->inode >= 0 && e->inode == next->inode &&
            !(e->rel_len == next->rel_len && memcmp(e->rel, next->rel, e->rel_len) == 0)) {
            /* A rename: the old path first, the new one after. */
            bool dir = false;
            int old_stands = hero_watch_stands(st, e->rel, e->rel_len, e->inode, &dir);
            int new_stands = hero_watch_stands(st, next->rel, next->rel_len, next->inode, &dir);
            const hero_watch_raw *from = e;
            const hero_watch_raw *to = next;

            if (old_stands == 1 && new_stands != 1) {
                from = next;
                to = e;
            }
            hero_watch_seen *was = hero_watch_seen_get(st, from->rel, from->rel_len);
            bool known = was != NULL && was->inode == from->inode;
            uint32_t fresh = from->flags & ~(known ? was->flags : 0u);
            bool is_dir = (from->flags & kFSEventStreamEventFlagItemIsDir) != 0;

            if (!known && (fresh & kFSEventStreamEventFlagItemCreated) != 0) {
                hero_watch_tell(st, HERO_WATCH_CREATED, is_dir, from->rel, from->rel_len, NULL, 0);
                if ((fresh & kFSEventStreamEventFlagItemModified) != 0 && !is_dir) {
                    hero_watch_tell(st, HERO_WATCH_CHANGED, false, from->rel, from->rel_len, NULL, 0);
                }
            }
            hero_watch_tell(st, HERO_WATCH_RENAMED, is_dir, to->rel, to->rel_len, from->rel, from->rel_len);
            hero_watch_seen_del(st, from->rel, from->rel_len);
            hero_watch_seen_put(st, to->rel, to->rel_len, to->inode, to->flags);
            if (hero_watch_stands(st, to->rel, to->rel_len, to->inode, &dir) == 0) {
                hero_watch_tell(st, HERO_WATCH_REMOVED, is_dir, to->rel, to->rel_len, NULL, 0);
                hero_watch_seen_del(st, to->rel, to->rel_len);
            }
            renamed_to = to->rel;
            renamed_to_len = to->rel_len;
            i += 1;
            continue;
        }
        hero_watch_fsevents_one(st, e, renamed_to, renamed_to_len);
        renamed_to = NULL;
        renamed_to_len = 0;
    }
    for (size_t i = 0; i < kept; i += 1) {
        hero_layer_free(raw[i].rel);
    }
    hero_layer_free(raw);
}

static void hero_watch_noop(void *arg) {
    (void)arg;
}

static int hero_watch_start(hero_watch_state *st) {
    /* The folder as the kernel names it, which is how FSEvents names its
     * paths (`/private/var` where the program said `/var`). */
    int fd = open(st->abs, O_EVTONLY | O_CLOEXEC);

    if (fd < 0) {
        return errno;
    }
    char real[PATH_MAX];
    struct stat facts;

    if (fcntl(fd, F_GETPATH, real) != 0 || fstat(fd, &facts) != 0) {
        int code = errno;

        (void)close(fd);
        return code;
    }
    (void)close(fd);
    if (!S_ISDIR(facts.st_mode)) {
        return ENOTDIR;
    }
    st->root_dev = facts.st_dev;
    st->root_ino = facts.st_ino;
    size_t real_len = strlen(real);
    bool slash = real_len > 0 && real[real_len - 1] == '/';

    st->real = hero_layer_alloc(real_len + 2);
    if (st->real == NULL) {
        return ENOMEM;
    }
    memcpy(st->real, real, real_len);
    st->real_len = slash ? real_len : real_len + 1;
    st->real[real_len] = '/';
    st->real[st->real_len] = 0;
    CFStringRef path = CFStringCreateWithBytes(NULL, (const UInt8 *)real, (CFIndex)real_len, kCFStringEncodingUTF8, false);

    if (path == NULL) {
        return ENOMEM;
    }
    CFArrayRef paths = CFArrayCreate(NULL, (const void **)&path, 1, &kCFTypeArrayCallBacks);

    CFRelease(path);
    if (paths == NULL) {
        return ENOMEM;
    }
    FSEventStreamContext context;

    memset(&context, 0, sizeof context);
    context.info = st;
    st->stream = FSEventStreamCreate(NULL, hero_watch_fsevents_batch, &context, paths, kFSEventStreamEventIdSinceNow, 0.0,
                                     kFSEventStreamCreateFlagFileEvents | kFSEventStreamCreateFlagNoDefer | kFSEventStreamCreateFlagWatchRoot |
                                         kFSEventStreamCreateFlagUseCFTypes | kFSEventStreamCreateFlagUseExtendedData);
    CFRelease(paths);
    if (st->stream == NULL) {
        return ENOMEM;
    }
    st->queue = dispatch_queue_create("hero_watch", DISPATCH_QUEUE_SERIAL);
    if (st->queue == NULL) {
        FSEventStreamRelease(st->stream);
        st->stream = NULL;
        return ENOMEM;
    }
    FSEventStreamSetDispatchQueue(st->stream, st->queue);
    if (!FSEventStreamStart(st->stream)) {
        FSEventStreamInvalidate(st->stream);
        FSEventStreamRelease(st->stream);
        st->stream = NULL;
        dispatch_release(st->queue);
        st->queue = NULL;
        return ENOTSUP;
    }
    return 0;
}

static void hero_watch_stop_source(hero_watch_state *st) {
    if (st->stream != NULL) {
        FSEventStreamStop(st->stream);
        FSEventStreamInvalidate(st->stream);
        FSEventStreamRelease(st->stream);
        st->stream = NULL;
    }
    if (st->queue != NULL) {
        /* A batch in flight on the queue finishes before the state goes. */
        dispatch_sync_f(st->queue, NULL, hero_watch_noop);
        dispatch_release(st->queue);
        st->queue = NULL;
    }
    hero_watch_seen_forget_all(st);
    hero_layer_free(st->real);
}
#endif

/* ============================================================================
 * WINDOWS: ReadDirectoryChangesExW
 * ========================================================================= */

#if defined(_WIN32)
#define HERO_WATCH_FILTER                                                                                                                             \
    (FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE |                         \
     FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SECURITY | FILE_NOTIFY_CHANGE_CREATION)

typedef BOOL(WINAPI *hero_watch_read_ex)(HANDLE, LPVOID, DWORD, BOOL, DWORD, LPDWORD, LPOVERLAPPED, LPOVERLAPPED_COMPLETION_ROUTINE,
                                        READ_DIRECTORY_NOTIFY_INFORMATION_CLASS);

/* UTF-8 as UTF-16 into a new block, or NULL. */
static wchar_t *hero_watch_wide(const char *s, size_t len) {
    if (len > (size_t)INT32_MAX) {
        return NULL;
    }
    int n = len == 0 ? 0 : MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, s, (int)len, NULL, 0);

    if (len > 0 && n <= 0) {
        return NULL;
    }
    wchar_t *out = hero_layer_alloc(((size_t)n + 1) * sizeof *out);

    if (out == NULL) {
        return NULL;
    }
    if (n > 0 && MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, s, (int)len, out, n) != n) {
        hero_layer_free(out);
        return NULL;
    }
    out[n] = 0;
    return out;
}

/* UTF-16 of `units` as UTF-8 into a new block, or NULL where it holds what
 * UTF-8 cannot (a lone surrogate). */
static char *hero_watch_narrow(const wchar_t *s, size_t units, size_t *len) {
    if (units > (size_t)INT32_MAX) {
        return NULL;
    }
    int n = units == 0 ? 0 : WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, s, (int)units, NULL, 0, NULL, NULL);

    if (units > 0 && n <= 0) {
        return NULL;
    }
    char *out = hero_layer_alloc((size_t)n + 1);

    if (out == NULL) {
        return NULL;
    }
    if (n > 0 && WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, s, (int)units, out, n, NULL, NULL) != n) {
        hero_layer_free(out);
        return NULL;
    }
    out[n] = 0;
    *len = (size_t)n;
    return out;
}

/* Whether `rel` now stands as a folder, asked of the disk. */
static bool hero_watch_now_dir(const hero_watch_state *st, const char *rel, size_t rel_len) {
    char *full = hero_watch_join(st->abs, st->abs_len, '\\', rel, rel_len);
    wchar_t *wide = full == NULL ? NULL : hero_watch_wide(full, strlen(full));
    DWORD attributes = wide == NULL ? INVALID_FILE_ATTRIBUTES : GetFileAttributesW(wide);

    hero_layer_free(wide);
    hero_layer_free(full);
    return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

/* Whether the watched folder still stands at its path, the same folder. */
static bool hero_watch_root_stands(const hero_watch_state *st) {
    HANDLE h = CreateFileW(st->abs_w, FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING,
                           FILE_FLAG_BACKUP_SEMANTICS, NULL);
    BY_HANDLE_FILE_INFORMATION facts;
    bool same = false;

    if (h == INVALID_HANDLE_VALUE) {
        return false;
    }
    if (GetFileInformationByHandle(h, &facts)) {
        same = facts.dwVolumeSerialNumber == st->volume && facts.nFileIndexHigh == st->index_high && facts.nFileIndexLow == st->index_low;
    }
    (void)CloseHandle(h);
    return same;
}

/* Every entry below the folder `rel` told created, with its own list of what
 * is left, never recursion; links are told and not followed. */
static void hero_watch_walk_w(hero_watch_state *st, const char *rel, size_t rel_len) {
    size_t cap = 16;
    size_t used = 0;
    char **left = hero_layer_alloc(cap * sizeof *left);

    if (left == NULL || (left[0] = hero_watch_copy(rel, rel_len)) == NULL) {
        hero_layer_free(left);
        hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
        return;
    }
    used = 1;
    while (used > 0) {
        char *here = left[--used];
        size_t here_len = strlen(here);
        char *full = hero_watch_join(st->abs, st->abs_len, '\\', here, here_len);
        char *pattern = full == NULL ? NULL : hero_watch_join(full, strlen(full), '\\', "*", 1);
        wchar_t *wide = pattern == NULL ? NULL : hero_watch_wide(pattern, strlen(pattern));
        WIN32_FIND_DATAW found;
        HANDLE h = wide == NULL ? INVALID_HANDLE_VALUE : FindFirstFileExW(wide, FindExInfoBasic, &found, FindExSearchNameMatch, NULL, 0);

        if (h != INVALID_HANDLE_VALUE) {
            do {
                const wchar_t *n = found.cFileName;
                size_t units = wcslen(n);

                if ((units == 1 && n[0] == L'.') || (units == 2 && n[0] == L'.' && n[1] == L'.')) {
                    continue;
                }
                size_t n_len = 0;
                char *name = hero_watch_narrow(n, units, &n_len);
                char *child = name == NULL ? NULL : hero_watch_join(here, here_len, '\\', name, n_len);

                hero_layer_free(name);
                if (child == NULL) {
                    hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
                    continue;
                }
                bool is_dir = (found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 && (found.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) == 0;

                hero_watch_tell(st, HERO_WATCH_CREATED, is_dir, child, strlen(child), NULL, 0);
                if (!is_dir) {
                    hero_layer_free(child);
                    continue;
                }
                if (used == cap) {
                    char **grown = hero_layer_grow(left, cap * 2 * sizeof *left);

                    if (grown == NULL) {
                        hero_layer_free(child);
                        hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
                        continue;
                    }
                    left = grown;
                    cap *= 2;
                }
                left[used++] = child;
            } while (FindNextFileW(h, &found));
            (void)FindClose(h);
        }
        hero_layer_free(wide);
        hero_layer_free(pattern);
        hero_layer_free(full);
        hero_layer_free(here);
    }
    hero_layer_free(left);
}

/* The `RENAMED_OLD_NAME` waiting for its partner told as the entry leaving. */
static void hero_watch_moved_out(hero_watch_state *st) {
    if (st->moving_rel != NULL) {
        hero_watch_tell(st, HERO_WATCH_REMOVED, st->moving_dir, st->moving_rel, strlen(st->moving_rel), NULL, 0);
        hero_layer_free(st->moving_rel);
        st->moving_rel = NULL;
    }
}

/* One record of the buffer: `action`, the name of `units` UTF-16 units, and
 * whether the system says the entry is a folder (`known` false where it did
 * not say). */
static void hero_watch_windows_one(hero_watch_state *st, DWORD action, const wchar_t *name, size_t units, bool known, bool dir) {
    size_t len = 0;
    char *rel = hero_watch_narrow(name, units, &len);

    if (rel == NULL) {
        hero_watch_moved_out(st);
        hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
        return;
    }
    if (!known && action != FILE_ACTION_REMOVED && action != FILE_ACTION_RENAMED_OLD_NAME) {
        dir = hero_watch_now_dir(st, rel, len);
    }
    if (action != FILE_ACTION_RENAMED_NEW_NAME) {
        hero_watch_moved_out(st);
    }
    switch (action) {
    case FILE_ACTION_ADDED:
        hero_watch_tell(st, HERO_WATCH_CREATED, dir, rel, len, NULL, 0);
        if (dir && st->tree) {
            hero_watch_walk_w(st, rel, len);
        }
        break;
    case FILE_ACTION_REMOVED:
        hero_watch_tell(st, HERO_WATCH_REMOVED, dir, rel, len, NULL, 0);
        break;
    case FILE_ACTION_MODIFIED:
        /* A folder's own `MODIFIED` says its entries changed, which are told
         * on their own. */
        if (!dir) {
            hero_watch_tell(st, HERO_WATCH_CHANGED, false, rel, len, NULL, 0);
        }
        break;
    case FILE_ACTION_RENAMED_OLD_NAME:
        st->moving_rel = rel;
        st->moving_dir = dir;
        return;
    case FILE_ACTION_RENAMED_NEW_NAME:
        if (st->moving_rel != NULL) {
            hero_watch_tell(st, HERO_WATCH_RENAMED, dir, rel, len, st->moving_rel, strlen(st->moving_rel));
            hero_layer_free(st->moving_rel);
            st->moving_rel = NULL;
        } else {
            hero_watch_tell(st, HERO_WATCH_CREATED, dir, rel, len, NULL, 0);
            if (dir && st->tree) {
                hero_watch_walk_w(st, rel, len);
            }
        }
        break;
    default:
        break;
    }
    hero_layer_free(rel);
}

/* Asks the system for the next changes into the buffer. */
static BOOL hero_watch_ask(hero_watch_state *st) {
    (void)ResetEvent(st->overlapped.hEvent);
    if (st->extended) {
        hero_watch_read_ex read_ex = (hero_watch_read_ex)(void *)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "ReadDirectoryChangesExW");

        if (read_ex != NULL && read_ex(st->dir, st->buffer, HERO_WATCH_BUFFER, st->tree ? TRUE : FALSE, HERO_WATCH_FILTER, NULL, &st->overlapped, NULL,
                                       ReadDirectoryNotifyExtendedInformation)) {
            return TRUE;
        }
        DWORD code = read_ex == NULL ? ERROR_INVALID_FUNCTION : GetLastError();

        if (code != ERROR_INVALID_FUNCTION && code != ERROR_INVALID_PARAMETER && code != ERROR_NOT_SUPPORTED) {
            SetLastError(code);
            return FALSE;
        }
        st->extended = false;
    }
    return ReadDirectoryChangesW(st->dir, st->buffer, HERO_WATCH_BUFFER, st->tree ? TRUE : FALSE, HERO_WATCH_FILTER, NULL, &st->overlapped, NULL);
}

/* The records of a buffer the system filled with `got` bytes. */
static void hero_watch_windows_read(hero_watch_state *st, DWORD got) {
    const unsigned char *at = st->buffer;
    size_t total = got;
    size_t offset = 0;

    for (;;) {
        DWORD next = 0;
        DWORD action = 0;
        DWORD name_bytes = 0;
        size_t head = 0;
        bool known = false;
        bool dir = false;

        if (st->extended) {
            FILE_NOTIFY_EXTENDED_INFORMATION record;

            head = offsetof(FILE_NOTIFY_EXTENDED_INFORMATION, FileName);
            if (offset + head > total) {
                break;
            }
            memcpy(&record, at + offset, head);
            next = record.NextEntryOffset;
            action = record.Action;
            name_bytes = record.FileNameLength;
            known = true;
            dir = (record.FileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 && (record.FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) == 0;
        } else {
            FILE_NOTIFY_INFORMATION record;

            head = offsetof(FILE_NOTIFY_INFORMATION, FileName);
            if (offset + head > total) {
                break;
            }
            memcpy(&record, at + offset, head);
            next = record.NextEntryOffset;
            action = record.Action;
            name_bytes = record.FileNameLength;
        }
        if (name_bytes > total - offset - head || name_bytes % sizeof(wchar_t) != 0) {
            break;
        }
        size_t units = name_bytes / sizeof(wchar_t);
        wchar_t *name = hero_layer_alloc((units + 1) * sizeof *name);

        if (name == NULL) {
            hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
        } else {
            memcpy(name, at + offset + head, name_bytes);
            name[units] = 0;
            hero_watch_windows_one(st, action, name, units, known, dir);
            hero_layer_free(name);
        }
        if (next == 0 || next > total - offset) {
            break;
        }
        offset += next;
    }
}

/* THE SOURCE'S THREAD: asks, waits for the answer or the close, reads. The
 * first ask is made before `ready` is set, so `open` returns only once the
 * system holds one. */
static DWORD WINAPI hero_watch_windows_loop(LPVOID arg) {
    hero_watch_state *st = arg;

    if (!hero_watch_ask(st)) {
        st->start_error = GetLastError();
        (void)SetEvent(st->ready);
        return 0;
    }
    st->start_error = 0;
    (void)SetEvent(st->ready);
    for (;;) {
        HANDLE both[2] = { st->overlapped.hEvent, st->stop };
        DWORD woke = WaitForMultipleObjects(2, both, FALSE, INFINITE);
        DWORD got = 0;

        if (woke != WAIT_OBJECT_0) {
            (void)CancelIoEx(st->dir, &st->overlapped);
            (void)GetOverlappedResult(st->dir, &st->overlapped, &got, TRUE);
            break;
        }
        if (!GetOverlappedResult(st->dir, &st->overlapped, &got, FALSE)) {
            DWORD code = GetLastError();

            if (code == ERROR_NOTIFY_ENUM_DIR) {
                hero_watch_moved_out(st);
                hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
            } else {
                hero_watch_moved_out(st);
                hero_watch_tell(st, HERO_WATCH_GONE, false, NULL, 0, NULL, 0);
                break;
            }
        } else if (got == 0) {
            hero_watch_moved_out(st);
            hero_watch_tell(st, HERO_WATCH_OVERFLOW, false, NULL, 0, NULL, 0);
        } else {
            hero_watch_windows_read(st, got);
        }
        if (!hero_watch_root_stands(st)) {
            hero_watch_moved_out(st);
            hero_watch_tell(st, HERO_WATCH_GONE, false, NULL, 0, NULL, 0);
            break;
        }
        if (!hero_watch_ask(st)) {
            hero_watch_moved_out(st);
            hero_watch_tell(st, HERO_WATCH_GONE, false, NULL, 0, NULL, 0);
            break;
        }
    }
    hero_watch_moved_out(st);
    return 0;
}

static int hero_watch_start(hero_watch_state *st) {
    st->dir = INVALID_HANDLE_VALUE;
    st->buffer = hero_layer_alloc(HERO_WATCH_BUFFER);
    st->abs_w = hero_watch_wide(st->abs, st->abs_len);
    if (st->buffer == NULL || st->abs_w == NULL) {
        return ERROR_NOT_ENOUGH_MEMORY;
    }
    st->dir = CreateFileW(st->abs_w, FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING,
                          FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
    if (st->dir == INVALID_HANDLE_VALUE) {
        return (int)GetLastError();
    }
    BY_HANDLE_FILE_INFORMATION facts;

    if (!GetFileInformationByHandle(st->dir, &facts)) {
        return (int)GetLastError();
    }
    if ((facts.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
        return ERROR_DIRECTORY;
    }
    st->volume = facts.dwVolumeSerialNumber;
    st->index_high = facts.nFileIndexHigh;
    st->index_low = facts.nFileIndexLow;
    st->extended = true;
    st->overlapped.hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
    st->stop = CreateEventW(NULL, TRUE, FALSE, NULL);
    st->ready = CreateEventW(NULL, TRUE, FALSE, NULL);
    if (st->overlapped.hEvent == NULL || st->stop == NULL || st->ready == NULL) {
        return (int)GetLastError();
    }
    st->thread = CreateThread(NULL, 0, hero_watch_windows_loop, st, 0, NULL);
    if (st->thread == NULL) {
        return (int)GetLastError();
    }
    (void)WaitForSingleObject(st->ready, INFINITE);
    if (st->start_error != 0) {
        (void)WaitForSingleObject(st->thread, INFINITE);
        (void)CloseHandle(st->thread);
        st->thread = NULL;
        return (int)st->start_error;
    }
    return 0;
}

static void hero_watch_stop_source(hero_watch_state *st) {
    if (st->thread != NULL) {
        (void)SetEvent(st->stop);
        (void)WaitForSingleObject(st->thread, INFINITE);
        (void)CloseHandle(st->thread);
        st->thread = NULL;
    }
    if (st->dir != INVALID_HANDLE_VALUE && st->dir != NULL) {
        (void)CloseHandle(st->dir);
    }
    if (st->overlapped.hEvent != NULL) {
        (void)CloseHandle(st->overlapped.hEvent);
    }
    if (st->stop != NULL) {
        (void)CloseHandle(st->stop);
    }
    if (st->ready != NULL) {
        (void)CloseHandle(st->ready);
    }
    hero_layer_free(st->moving_rel);
    hero_layer_free(st->abs_w);
    hero_layer_free(st->buffer);
}
#endif

#if !defined(_WIN32) && !defined(HERO_WATCH_INOTIFY) && !defined(HERO_WATCH_FSEVENTS)
/* A POSIX system with neither: no watch, said as a value. */
static int hero_watch_start(hero_watch_state *st) {
    (void)st;
    return ENOTSUP;
}

static void hero_watch_stop_source(hero_watch_state *st) {
    (void)st;
}
#endif

/* -- the stop ----------------------------------------------------------------- */

/* THE HOOK `hero_net.c` CALLS AT A STOP, under its own lock: this file never
 * calls that file under its own, so taking it here cannot cross them. */
static void hero_watch_hear_stop(void) {
    hero_watch_enter();
    hero_watch_stop_heard = true;
    hero_watch_broadcast();
    hero_watch_leave();
}

/* Before the first watch: the condition made, and the stop's hook asked once,
 * outside the lock, a wait asking in slices where it is refused. False where
 * the condition could not be made. The `pthread_once` is asked by every
 * caller, never by the first alone (defect 2601): a second thread opening a
 * watch while the first still makes the condition waits for it, where it
 * passed the hook's exchange and could reach the condition half made. */
static bool hero_watch_up(void) {
#if !defined(_WIN32) && !defined(__APPLE__)
    (void)pthread_once(&hero_watch_cond_once, hero_watch_cond_made);
    if (!atomic_load(&hero_watch_cond_ready)) {
        return false;
    }
#endif
    int was = 0;

    if (atomic_compare_exchange_strong(&hero_watch_hook, &was, 3)) {
        atomic_store(&hero_watch_hook, hero_net_when_stopped(hero_watch_hear_stop) == 0 ? 1 : 2);
    }
    return true;
}

/* -- the table -------------------------------------------------------------- */

static hero_watch *hero_watch_handle(uint32_t index, uint64_t life) {
    return (hero_watch *)(uintptr_t)(HERO_HANDLE_TAG(HERO_HANDLE_TAG_WATCH) | (life << HERO_WATCH_INDEX_BITS) | ((uint64_t)index + 1));
}

/* The slot a handle names while it holds the life it was issued for and is
 * not closed; NULL for any other value. Under the lock. */
static hero_watch_slot *hero_watch_find(hero_watch *w) {
    uint64_t value = (uint64_t)(uintptr_t)w;
    uint64_t low = value & HERO_WATCH_INDEX_MASK;

    if (low == 0 || low > hero_watch_fresh) {
        return NULL;
    }
    hero_watch_slot *slot = &hero_watch_slots[low - 1];

    if (!slot->held || slot->closing || value != (uint64_t)(uintptr_t)hero_watch_handle((uint32_t)(low - 1), slot->life)) {
        return NULL;
    }
    return slot;
}

/* A slot given back, its state detached for the caller to end. Under the
 * lock. */
static hero_watch_state *hero_watch_slot_free(hero_watch_slot *slot) {
    hero_watch_state *st = slot->state;

    slot->state = NULL;
    slot->held = false;
    slot->closing = false;
    slot->busy = 0;
    if (slot->life < HERO_WATCH_LIFE_MOST) {
        slot->next = hero_watch_given_back;
        hero_watch_given_back = (uint32_t)(slot - hero_watch_slots) + 1;
    }
    return st;
}

/* A state ended, outside the lock: its source stopped, its queue and bell
 * given back. */
static void hero_watch_end(hero_watch_state *st) {
    if (st == NULL) {
        return;
    }
    hero_watch_stop_source(st);
    hero_watch_event *e = st->head;

    while (e != NULL) {
        hero_watch_event *next = e->next;

        hero_watch_event_free(st, e);
        e = next;
    }
#if defined(_WIN32)
    if (st->bell_socket != NULL) {
        hero_net_unhold(st->bell_hold);
        (void)hero_net_close(st->bell_socket);
    }
#else
    if (st->bell_in != NULL) {
        (void)hero_stream_release(st->bell_in);
        (void)hero_stream_close(st->bell_in);
    }
    if (st->bell_out != NULL) {
        (void)hero_stream_release(st->bell_out);
        (void)hero_stream_close(st->bell_out);
    }
#endif
    hero_layer_free(st->given);
    hero_layer_free(st->base);
    hero_layer_free(st->name);
    hero_layer_free(st->abs);
    hero_layer_free(st);
}

/* -- the doors -------------------------------------------------------------- */

/* The absolute form of `path`, as the system resolves it, or NULL with the
 * system's number in `code`. */
static char *hero_watch_absolute(const char *path, int *code) {
#if defined(_WIN32)
    wchar_t *wide = hero_watch_wide(path, strlen(path));

    if (wide == NULL) {
        *code = ERROR_INVALID_NAME;
        return NULL;
    }
    DWORD need = GetFullPathNameW(wide, 0, NULL, NULL);
    wchar_t *full = need == 0 ? NULL : hero_layer_alloc((size_t)need * sizeof *full);
    DWORD got = full == NULL ? 0 : GetFullPathNameW(wide, need, full, NULL);

    hero_layer_free(wide);
    if (got == 0 || got >= need) {
        *code = full == NULL && need != 0 ? ERROR_NOT_ENOUGH_MEMORY : (int)GetLastError();
        hero_layer_free(full);
        return NULL;
    }
    size_t len = 0;
    char *out = hero_watch_narrow(full, got, &len);

    hero_layer_free(full);
    if (out == NULL) {
        *code = ERROR_INVALID_NAME;
    }
    return out;
#else
    char real[PATH_MAX];

    if (realpath(path, real) == NULL) {
        *code = errno;
        return NULL;
    }
    char *out = hero_watch_copy(real, strlen(real));

    if (out == NULL) {
        *code = ENOMEM;
    }
    return out;
#endif
}

hero_watch *hero_watch_open(const char *path, int64_t flags, int64_t most) {
    if (path == NULL || path[0] == 0 || (flags & ~(int64_t)HERO_WATCH_TREE) != 0 || most < 1 || most > HERO_WATCH_MOST) {
        hero_watch_fail(HERO_WATCH_INVALID, 0);
        return NULL;
    }
    if (!hero_watch_up()) {
        hero_watch_fail(HERO_WATCH_LIMIT, 0);
        return NULL;
    }
    hero_watch_state *st = hero_layer_alloc_zero(1, sizeof *st);

    if (st == NULL) {
        hero_watch_fail(HERO_WATCH_LIMIT, 0);
        return NULL;
    }
#if defined(HERO_WATCH_INOTIFY)
    st->inotify = -1;
    st->wake[0] = -1;
    st->wake[1] = -1;
#elif defined(_WIN32)
    st->dir = INVALID_HANDLE_VALUE;
    st->bell_fd = INVALID_SOCKET;
#endif
#if !defined(_WIN32)
    st->bell_in_fd = -1;
    st->bell_out_fd = -1;
#endif
    st->tree = (flags & HERO_WATCH_TREE) != 0;
    st->most = (uint64_t)most;
    size_t given_len = strlen(path);
    int code = 0;
    char *abs = hero_watch_absolute(path, &code);

    st->given = hero_watch_copy(path, given_len);
    st->given_len = given_len;
    if (abs == NULL || st->given == NULL) {
        hero_layer_free(abs);
        hero_watch_end(st);
        hero_watch_fail(abs == NULL ? hero_watch_kind_of(code) : HERO_WATCH_LIMIT, abs == NULL ? code : 0);
        return NULL;
    }
    /* A folder or a file: a file is watched as its folder's entry. */
    bool is_dir = false;
#if defined(_WIN32)
    wchar_t *wide = hero_watch_wide(abs, strlen(abs));
    DWORD attributes = wide == NULL ? INVALID_FILE_ATTRIBUTES : GetFileAttributesW(wide);

    if (attributes == INVALID_FILE_ATTRIBUTES) {
        code = wide == NULL ? ERROR_INVALID_NAME : (int)GetLastError();
        hero_layer_free(wide);
        hero_layer_free(abs);
        hero_watch_end(st);
        hero_watch_fail(hero_watch_kind_of(code), code);
        return NULL;
    }
    is_dir = (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
    if (!is_dir) {
        /* The name as the volume keeps it, which the system's events carry. */
        WIN32_FIND_DATAW found;
        HANDLE h = FindFirstFileExW(wide, FindExInfoBasic, &found, FindExSearchNameMatch, NULL, 0);

        if (h != INVALID_HANDLE_VALUE) {
            size_t len = 0;

            st->name = hero_watch_narrow(found.cFileName, wcslen(found.cFileName), &len);
            st->name_len = len;
            (void)FindClose(h);
        }
    }
    hero_layer_free(wide);
#else
    struct stat facts;

    if (stat(abs, &facts) != 0) {
        code = errno;
        hero_layer_free(abs);
        hero_watch_end(st);
        hero_watch_fail(hero_watch_kind_of(code), code);
        return NULL;
    }
    is_dir = S_ISDIR(facts.st_mode);
#endif
    if (st->tree && !is_dir) {
        hero_layer_free(abs);
        hero_watch_end(st);
        hero_watch_fail(HERO_WATCH_NOT_A_DIRECTORY, 0);
        return NULL;
    }
    size_t abs_len = strlen(abs);

    if (is_dir) {
        bool ends = hero_watch_is_sep(path[given_len - 1]);

        st->abs = abs;
        st->abs_len = abs_len;
        st->base = hero_layer_alloc(given_len + 2);
        if (st->base != NULL) {
            memcpy(st->base, path, given_len);
            st->base_len = ends ? given_len : given_len + 1;
            st->base[given_len] = HERO_WATCH_SEP;
            st->base[st->base_len] = 0;
        }
    } else {
        /* The file's folder, absolute, and the file's own name. */
        size_t cut = abs_len;

        while (cut > 0 && !hero_watch_is_sep(abs[cut - 1])) {
            cut -= 1;
        }
        size_t folder_len = cut > 1 ? cut - 1 : cut;
#if defined(_WIN32)
        /* `C:\` keeps its separator: `C:` alone names the drive's own
         * working folder. */
        if (folder_len == 2 && abs[1] == ':') {
            folder_len = 3;
        }
#endif
        if (st->name == NULL) {
            st->name = hero_watch_copy(abs + cut, abs_len - cut);
            st->name_len = abs_len - cut;
        }
        st->abs = hero_watch_copy(abs, folder_len);
        st->abs_len = folder_len;
        size_t given_cut = given_len;

        while (given_cut > 0 && !hero_watch_is_sep(path[given_cut - 1])) {
            given_cut -= 1;
        }
        st->base = hero_watch_copy(path, given_cut);
        st->base_len = given_cut;
        hero_layer_free(abs);
#if defined(HERO_WATCH_FSEVENTS)
        /* The name as the volume keeps it, which FSEvents' paths carry. */
        if (st->name != NULL) {
            int fd = open(path, O_EVTONLY | O_CLOEXEC);
            char real[PATH_MAX];

            if (fd >= 0) {
                if (fcntl(fd, F_GETPATH, real) == 0) {
                    size_t real_len = strlen(real);
                    size_t at = real_len;

                    while (at > 0 && real[at - 1] != '/') {
                        at -= 1;
                    }
                    char *kept = hero_watch_copy(real + at, real_len - at);

                    if (kept != NULL) {
                        hero_layer_free(st->name);
                        st->name = kept;
                        st->name_len = real_len - at;
                    }
                }
                (void)close(fd);
            }
        }
#endif
    }
    if (st->abs == NULL || st->base == NULL || (!is_dir && st->name == NULL)) {
        hero_watch_end(st);
        hero_watch_fail(HERO_WATCH_LIMIT, 0);
        return NULL;
    }
    code = hero_watch_start(st);
    if (code != 0) {
        hero_watch_end(st);
        hero_watch_fail(hero_watch_kind_of(code), code);
        return NULL;
    }
    hero_watch_enter();
    uint32_t index = 0;
    bool found = false;

    if (hero_watch_given_back != 0) {
        index = hero_watch_given_back - 1;
        hero_watch_given_back = hero_watch_slots[index].next;
        found = true;
    } else if (hero_watch_fresh < HERO_WATCH_SLOTS) {
        index = hero_watch_fresh;
        hero_watch_fresh += 1;
        found = true;
    }
    if (!found) {
        hero_watch_leave();
        hero_watch_end(st);
        hero_watch_fail(HERO_WATCH_LIMIT, 0);
        return NULL;
    }
    hero_watch_slot *slot = &hero_watch_slots[index];

    slot->life += 1;
    slot->held = true;
    slot->closing = false;
    slot->busy = 0;
    slot->state = st;
    slot->next = 0;
    hero_watch *handle = hero_watch_handle(index, slot->life);

    hero_watch_leave();
    hero_watch_clear();
    return handle;
}

/* The bytes an event's path takes as the program reads it. */
static size_t hero_watch_told_len(const hero_watch_state *st, const hero_watch_event *e, bool old) {
    size_t rel_len = old ? e->old_len : e->rel_len;

    if (old && e->kind != HERO_WATCH_RENAMED) {
        return 0;
    }
    if (e->root || rel_len == 0) {
        return st->given_len;
    }
    if (st->name != NULL && hero_watch_same_name(st, old ? e->text + e->rel_len : e->text, rel_len)) {
        return st->given_len;
    }
    return st->base_len + rel_len;
}

/* An event's path written at `out` as the program reads it. */
static void hero_watch_told(const hero_watch_state *st, const hero_watch_event *e, bool old, char *out) {
    size_t rel_len = old ? e->old_len : e->rel_len;
    const char *rel = old ? e->text + e->rel_len : e->text;

    if (e->root || rel_len == 0 || (st->name != NULL && hero_watch_same_name(st, rel, rel_len))) {
        memcpy(out, st->given, st->given_len);
        return;
    }
    if (st->base_len > 0) {
        memcpy(out, st->base, st->base_len);
    }
    memcpy(out + st->base_len, rel, rel_len);
}

int64_t hero_watch_next(hero_watch *w, int64_t limit_ns, int64_t *numbers, uint64_t numbers_count, void *text, uint64_t text_count) {
    if (limit_ns < -1 || numbers == NULL || numbers_count < HERO_WATCH_EVENT_NUMBERS || (text == NULL && text_count > 0)) {
        hero_watch_fail(HERO_WATCH_INVALID, 0);
        return -1;
    }
    if (hero_net_stop_asked() != 0) {
        hero_watch_fail(HERO_WATCH_STOPPED, 0);
        return -1;
    }
    /* When the wait ends on the clock that never goes back, INT64_MAX for
     * never, a limit past the clock's end among them; a clock that cannot be
     * read (`blind`) waits once, as long as it is asked to in one chunk at
     * most, then answers. */
    int64_t now = hero_watch_now();
    int64_t ends = INT64_MAX;
    bool blind = false;
    bool slept = false;

    if (limit_ns >= 0) {
        if (now < 0) {
            blind = true;
        } else if (__builtin_add_overflow(now, limit_ns, &ends)) {
            ends = INT64_MAX;
        }
    }
    bool sliced = atomic_load(&hero_watch_hook) != 1;
    int64_t answer = -1;
    int64_t kind = HERO_WATCH_OK;

    hero_watch_enter();
    hero_watch_slot *slot = hero_watch_find(w);

    if (slot == NULL) {
        hero_watch_leave();
        hero_watch_fail(HERO_WATCH_NOT_OPEN, 0);
        return -1;
    }
    slot->busy += 1;
    hero_watch_state *st = slot->state;

    for (;;) {
        if (slot->closing) {
            kind = HERO_WATCH_NOT_OPEN;
            break;
        }
        if (hero_watch_stop_heard) {
            kind = HERO_WATCH_STOPPED;
            break;
        }
        hero_watch_event *e = st->head;

        if (e != NULL) {
            size_t path_len = hero_watch_told_len(st, e, false);
            size_t old_len = hero_watch_told_len(st, e, true);

            numbers[0] = e->kind;
            numbers[1] = e->dir;
            numbers[2] = (int64_t)path_len;
            numbers[3] = (int64_t)old_len;
            if ((uint64_t)path_len + (uint64_t)old_len > text_count) {
                kind = HERO_WATCH_SHORT;
                break;
            }
            hero_watch_told(st, e, false, text);
            if (old_len > 0) {
                hero_watch_told(st, e, true, (char *)text + path_len);
            }
            st->head = e->next;
            if (st->head == NULL) {
                st->tail = NULL;
            }
            st->count -= 1;
            if (e->kind == HERO_WATCH_GONE) {
                st->ended = true;
            }
            hero_watch_event_free(st, e);
            hero_watch_hush(st);
            answer = 1;
            break;
        }
        if (st->ended) {
            kind = HERO_WATCH_ENDED;
            break;
        }
        int64_t at = hero_watch_now();

        if (limit_ns == 0 || (blind && slept) || (!blind && ends != INT64_MAX && (at < 0 || at >= ends))) {
            answer = 0;
            break;
        }
        int64_t span = blind ? limit_ns : ends == INT64_MAX ? HERO_WATCH_CHUNK_NS : ends - at;

        slept = true;

        if (sliced && span > HERO_WATCH_SLICE_NS) {
            span = HERO_WATCH_SLICE_NS;
        }
        hero_watch_sleep(span);
        if (sliced) {
            /* No hook: the stop is asked between slices, never under the lock. */
            hero_watch_leave();
            bool asked = hero_net_stop_asked() != 0;

            hero_watch_enter();
            if (asked) {
                kind = HERO_WATCH_STOPPED;
                break;
            }
        }
    }
    slot->busy -= 1;
    hero_watch_state *ended = NULL;

    if (slot->closing && slot->busy == 0) {
        ended = hero_watch_slot_free(slot);
    }
    hero_watch_leave();
    hero_watch_end(ended);
    if (answer < 0) {
        hero_watch_fail(kind, 0);
        return -1;
    }
    hero_watch_clear();
    return answer;
}

int64_t hero_watch_count(hero_watch *w) {
    hero_watch_enter();
    hero_watch_slot *slot = hero_watch_find(w);
    int64_t count = slot == NULL ? -1 : (int64_t)slot->state->count;

    hero_watch_leave();
    if (count < 0) {
        hero_watch_fail(HERO_WATCH_NOT_OPEN, 0);
        return -1;
    }
    hero_watch_clear();
    return count;
}

hero_stream *hero_watch_bell_stream(hero_watch *w) {
#if defined(_WIN32)
    hero_watch_enter();
    bool open = hero_watch_find(w) != NULL;

    hero_watch_leave();
    hero_watch_fail(open ? HERO_WATCH_UNSUPPORTED : HERO_WATCH_NOT_OPEN, 0);
    return NULL;
#else
    hero_watch_enter();
    hero_watch_slot *slot = hero_watch_find(w);
    hero_stream *made = slot != NULL && slot->state->bell ? slot->state->bell_in : NULL;

    hero_watch_leave();
    if (slot == NULL) {
        hero_watch_fail(HERO_WATCH_NOT_OPEN, 0);
        return NULL;
    }
    if (made != NULL) {
        hero_watch_clear();
        return made;
    }
    /* Made outside the lock: no other member is called under it. */
    hero_stream *in = NULL;
    hero_stream *out = NULL;

    if (hero_stream_pipe(&in, &out) != 0) {
        hero_watch_fail(HERO_WATCH_LIMIT, hero_stream_last_os_error());
        return NULL;
    }
    int64_t in_fd = hero_stream_hold(in);
    int64_t out_fd = in_fd < 0 ? -1 : hero_stream_hold(out);

    if (in_fd < 0 || out_fd < 0 || in_fd > INT32_MAX || out_fd > INT32_MAX) {
        if (in_fd >= 0) {
            (void)hero_stream_release(in);
        }
        if (out_fd >= 0) {
            (void)hero_stream_release(out);
        }
        (void)hero_stream_close(in);
        (void)hero_stream_close(out);
        hero_watch_fail(HERO_WATCH_LIMIT, 0);
        return NULL;
    }
    /* The pipe is the watch's alone: neither end ever waits. */
    for (int i = 0; i < 2; i += 1) {
        int fd = (int)(i == 0 ? in_fd : out_fd);
        int flags = fcntl(fd, F_GETFL);

        if (flags >= 0) {
            (void)fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
    hero_watch_enter();
    slot = hero_watch_find(w);
    bool kept = slot != NULL && !slot->state->bell;

    if (kept) {
        hero_watch_state *st = slot->state;

        st->bell_in = in;
        st->bell_out = out;
        st->bell_in_fd = (int)in_fd;
        st->bell_out_fd = (int)out_fd;
        st->bell = true;
        if (st->head != NULL) {
            hero_watch_ring(st);
        }
    }
    made = slot == NULL ? NULL : slot->state->bell_in;
    hero_watch_leave();
    if (!kept) {
        (void)hero_stream_release(in);
        (void)hero_stream_release(out);
        (void)hero_stream_close(in);
        (void)hero_stream_close(out);
    }
    if (made == NULL) {
        hero_watch_fail(HERO_WATCH_NOT_OPEN, 0);
        return NULL;
    }
    hero_watch_clear();
    return made;
#endif
}

hero_net_socket *hero_watch_bell_socket(hero_watch *w) {
#if !defined(_WIN32)
    hero_watch_enter();
    bool open = hero_watch_find(w) != NULL;

    hero_watch_leave();
    hero_watch_fail(open ? HERO_WATCH_UNSUPPORTED : HERO_WATCH_NOT_OPEN, 0);
    return NULL;
#else
    hero_watch_enter();
    hero_watch_slot *slot = hero_watch_find(w);
    hero_net_socket *made = slot != NULL && slot->state->bell ? slot->state->bell_socket : NULL;

    hero_watch_leave();
    if (slot == NULL) {
        hero_watch_fail(HERO_WATCH_NOT_OPEN, 0);
        return NULL;
    }
    if (made != NULL) {
        hero_watch_clear();
        return made;
    }
    hero_net_socket *s = hero_net_udp("127.0.0.1", 0);

    if (s == NULL) {
        hero_watch_fail(HERO_WATCH_LIMIT, hero_net_last_os_error());
        return NULL;
    }
    int64_t port = hero_net_port(s);
    int64_t fd = -1;
    int64_t hold = port <= 0 ? -1 : hero_net_hold(s, &fd);

    if (hold < 0 || port > 65535) {
        if (hold >= 0) {
            hero_net_unhold(hold);
        }
        (void)hero_net_close(s);
        hero_watch_fail(HERO_WATCH_LIMIT, 0);
        return NULL;
    }
    hero_watch_enter();
    slot = hero_watch_find(w);
    bool kept = slot != NULL && !slot->state->bell;

    if (kept) {
        hero_watch_state *st = slot->state;

        st->bell_socket = s;
        st->bell_hold = hold;
        st->bell_fd = (SOCKET)(uint64_t)fd;
        memset(&st->bell_at, 0, sizeof st->bell_at);
        st->bell_at.sin_family = AF_INET;
        st->bell_at.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        st->bell_at.sin_port = htons((u_short)port);
        st->bell = true;
        if (st->head != NULL) {
            hero_watch_ring(st);
        }
    }
    made = slot == NULL ? NULL : slot->state->bell_socket;
    hero_watch_leave();
    if (!kept) {
        hero_net_unhold(hold);
        (void)hero_net_close(s);
    }
    if (made == NULL) {
        hero_watch_fail(HERO_WATCH_NOT_OPEN, 0);
        return NULL;
    }
    hero_watch_clear();
    return made;
#endif
}

int64_t hero_watch_close(hero_watch *w) {
    hero_watch_state *ended = NULL;

    hero_watch_enter();
    hero_watch_slot *slot = hero_watch_find(w);

    if (slot != NULL) {
        slot->closing = true;
        if (slot->busy == 0) {
            ended = hero_watch_slot_free(slot);
        }
        hero_watch_broadcast();
    }
    hero_watch_leave();
    if (slot == NULL) {
        hero_watch_fail(HERO_WATCH_NOT_OPEN, 0);
        return -1;
    }
    hero_watch_end(ended);
    hero_watch_clear();
    return 0;
}

int64_t hero_watch_last_error(void) {
    return hero_watch_kind;
}

int64_t hero_watch_last_os_error(void) {
    return hero_watch_os;
}
