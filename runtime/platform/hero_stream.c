/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_stream.c — the platform layer's streams, one member of the
 * archive the compiler puts on every program's link line (M-rebel-library;
 * `selfhost/cli/layer.hero` builds, keys and links it). Its header,
 * `hero_stream.h`, says what each function answers; this file says how each
 * platform gets there.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so an arm that
 * defines another signature than the header's is `conflicting types` where
 * it is compiled.
 *
 * A STREAM IS A SLOT OF THE TABLE BELOW, NEVER AN ADDRESS THIS FILE READS
 * THROUGH, as a socket is in `hero_net.c`, whose shape this follows. The
 * handle a program holds is the slot's index and the life the slot held when
 * it was issued, so a handle closed already names a life that ended and is
 * `HERO_STREAM_NOT_OPEN`, as are NULL and a value this layer never issued.
 * The table is fixed, 65,536 streams, since the runtime's allocator is
 * reached only through `parts/alloc.c` (the net's `runtime` suite, rule 1),
 * and its static bytes are touched only as streams are made; a program past
 * it is told `HERO_STREAM_LIMIT`, as past the system's own.
 *
 * THE FIRST THREE SLOTS ARE THE PROCESS'S OWN STREAMS, borrowed: issued at
 * the first `hero_stream_std` with a life that never ends, so the handle is
 * the same at every call, and never closed. On POSIX they stand on
 * descriptors 0, 1 and 2; on Windows on what `GetStdHandle` answers at each
 * call, which a program may have changed. What they are is asked of the
 * system at each call that needs it, never remembered: a C library of the
 * program may point them elsewhere.
 *
 * A CALL IN FLIGHT KEEPS ITS DESCRIPTOR. Each call marks its slot busy while
 * it uses the descriptor; a close meanwhile, from another thread, marks the
 * slot closing, so no later call finds it, and the last call to leave closes
 * the descriptor. A descriptor is never closed while a call may still pass
 * its number to the system, which would then reach whatever file the system
 * gave that number next. Unlike a socket's, a call blocked in the system on
 * a pipe or a terminal is not ended by the close: it runs to its end and
 * answers what the system answered, bytes it read among them, which a
 * `NOT_OPEN` would lose. A hold (`hero_stream_hold`) is one more such user,
 * until its release.
 *
 * A WRITE TO A PIPE NOBODY READS IS A VALUE, AND THE TWO POSIX SYSTEMS GET
 * THERE DIFFERENTLY. Such a write raises SIGPIPE, which ends the process,
 * and `write` has no word to stop it. Measured for this file on 2026-11-15:
 *
 *   - Darwin 27.2.0 sends the signal to the PROCESS: with the writing thread
 *     blocking it, the handler ran on another thread. So blocking it around
 *     the write protects nothing there. What Darwin has is a mark,
 *     `F_SETNOSIGPIPE` (fcntl(2)), and the mark is the OPEN FILE's, not the
 *     descriptor's: set on a pipe's end, its `dup` read it set and wrote
 *     `EPIPE` with no signal. A pipe this file makes, and anything it opens
 *     that is no regular file, is marked once, being this process's alone.
 *     A standard stream is shared with whoever started the program and with
 *     whatever else it started, so there the mark is set around each write
 *     and taken away when no write to a standard stream is in flight: left
 *     set, a sibling in the same pipeline that counts on the signal to end
 *     would run on.
 *   - Linux 7.0.14 sends it to the writing THREAD: blocked there, it stayed
 *     pending on the writer, no other thread's handler ran, and
 *     `sigtimedwait` took it back. So each write to what may be a pipe
 *     blocks the signal on its own thread, and after an `EPIPE` takes back
 *     the one signal the write raised, unless one was pending before, which
 *     is somebody else's and stays.
 *
 * Any other POSIX system is refused at compile time, below: neither route
 * has been measured on one. On Windows no signal exists, and the write fails
 * with `ERROR_NO_DATA` or `ERROR_BROKEN_PIPE` (Microsoft's pages; unrun).
 *
 * EVERY CALL THE SYSTEM INTERRUPTS IS ASKED AGAIN (`EINTR`), but `close`:
 * Linux's close(2) says the descriptor is closed either way and asking again
 * may close one another thread was just given; POSIX leaves it unspecified.
 * So a close a signal interrupted counts as closed.
 *
 * NOTHING IS HANDED TO A CHILD BY DEFAULT: `O_CLOEXEC` at `open`, `pipe2`
 * with it on Linux, a `HANDLE` made with no security attributes on Windows.
 * Darwin has no `pipe2` (its SDK's `unistd.h` names none), so there a pipe's
 * two ends are marked right after `pipe`, and a child another thread starts
 * between the two calls would carry them: the platform's window, which a
 * member that starts processes closes on its side.
 *
 * SYNC REACHES THE STORAGE ON EVERY PLATFORM: Darwin's `fsync` leaves the
 * drive's own buffer unwritten and its `F_FULLFSYNC` asks the drive too
 * (fsync(2), fcntl(2)), so that is asked first, and `fsync` where the file
 * system has none.
 *
 * WINDOWS, ALL OF IT UNRUN ON THE MACHINE THIS WAS WRITTEN ON: `CreateFileW`
 * with every sharing right, so another program may open, rename and remove
 * an open file as on POSIX; the path from UTF-8 by `MultiByteToWideChar`,
 * which refuses bytes that are no UTF-8, into a buffer on the stack, 32,768
 * units, the longest path the system takes; `HERO_STREAM_ACCESS_WRITE` as
 * `OPEN_ALWAYS` then `SetEndOfFile`, so a file already there keeps its
 * attributes as it keeps its permissions on POSIX, where `CREATE_ALWAYS`
 * would replace them and refuse a hidden file; `HERO_STREAM_ACCESS_APPEND`
 * as the right to append without the right to write data, which makes every
 * write go to the end (Microsoft's File Access Rights page); `ReadFile` and
 * `WriteFile` raw, a console's bytes included.
 */

#include "hero_stream.h"

/* The tag that keeps this table's handles apart from every other member's
 * (2026-11-16): `HERO_HANDLE_TAG_STREAM`. */
#include "hero_handle_tag.h"

#include <stdbool.h>
#include <stddef.h>

#if defined(_WIN32)
#include <windows.h>
typedef HANDLE hero_stream_fd;
#define HERO_STREAM_NO_FD INVALID_HANDLE_VALUE
/* The longest path the wide calls take, in UTF-16 units, its end included. */
#define HERO_STREAM_PATH_MOST 32768
#else
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
typedef int hero_stream_fd;
#define HERO_STREAM_NO_FD (-1)
/* A position and a size are 64 bits, as the header's are. */
_Static_assert(sizeof(off_t) == 8, "a stream's position is 64 bits");
/* How a write to a pipe nobody reads is kept from raising SIGPIPE (the
 * opening comment): Darwin's mark on the open file, or Linux's signal
 * blocked on the writing thread and taken back. */
#if defined(F_SETNOSIGPIPE)
#define HERO_STREAM_MARKS 1
#elif defined(__linux__)
#define HERO_STREAM_BLOCKS 1
#else
#error "hero_stream.c: no measured way to stop SIGPIPE on this platform; a write to a closed pipe would end the program"
#endif
#if defined(__linux__)
/* glibc declares it only under `_GNU_SOURCE`, which a member cannot define:
 * its own header comes first, and that reads the C library's settings. */
int pipe2(int ends[2], int flags);
#endif
#endif

/* A handle carries this table's tag, a slot and a life in one pointer's 64
 * bits: `HERO_HANDLE_TAG(HERO_HANDLE_TAG_STREAM) | (life <<
 * HERO_STREAM_INDEX_BITS) | (index + 1)`, 17 bits for an index plus one up
 * to 65,536, the life above them below bit 58, and the tag in bits 58 to 61
 * (`hero_handle_tag.h`), so no handle is NULL and none is another member's:
 * until 2026-11-16 a stream and a socket at the same slot and life were one
 * value, which the runtime's set of ended handles mistook for each other. */
_Static_assert(sizeof(void *) == 8 && sizeof(uintptr_t) == 8, "a stream handle carries its slot and its life in 64 bits");
#define HERO_STREAM_INDEX_BITS 17
#define HERO_STREAM_INDEX_MASK ((UINT64_C(1) << HERO_STREAM_INDEX_BITS) - 1)
/* The last life a slot is given, 2^41 - 1 since the tag (2^45 - 1 before
 * it); a slot at it is never issued again (`hero_handle_tag.h` counts how
 * long that takes). */
#define HERO_STREAM_LIFE_MOST HERO_HANDLE_LIFE_MOST(HERO_STREAM_INDEX_BITS)
_Static_assert((HERO_STREAM_LIFE_MOST << HERO_STREAM_INDEX_BITS) < (UINT64_C(1) << HERO_HANDLE_TAG_SHIFT), "a life stays below the tag");

#define HERO_STREAM_SLOTS 65536u
_Static_assert(HERO_STREAM_SLOTS <= HERO_STREAM_INDEX_MASK, "an index plus one fits below a handle's life");
/* The slots of the process's own three streams, the table's first. */
#define HERO_STREAM_STANDARD 3u
/* The most one call reads or writes: under what Darwin's `read` takes
 * (`INT_MAX`), what Linux moves in one call, and Windows' 32-bit counts. */
#define HERO_STREAM_CHUNK (UINT64_C(1) << 30)

typedef struct {
    /* What the stream stands on; none for one made over a table. */
    hero_stream_fd fd;
    /* The table of a stream another member made, and what it is called
     * with; NULL for a stream on a descriptor. */
    const hero_stream_ops *ops;
    void *payload;
    /* The life the slot holds or last held, which its handle names. */
    uint64_t life;
    /* Calls in flight on it, the holds among them. */
    uint32_t busy;
    /* `hero_stream_hold`s not yet released, each one of `busy`. */
    uint32_t holds;
    /* The index plus one of the next slot given back, 0 for none. */
    uint32_t next;
    /* One of `HERO_STREAM_KIND_*`, as it was when the stream was made; not
     * kept for a standard stream, which is asked each time. */
    uint8_t kind;
    /* What it was opened to do; a standard stream is taken to do both, and
     * the system says where it does not. */
    bool reads;
    bool writes;
    bool appends;
    /* It holds a stream the program has not closed. */
    bool held;
    /* The program closed it while a call was in flight: the last one
     * releases what it stands on. */
    bool closing;
    /* One of the process's own three: borrowed, never closed. */
    bool standard;
} hero_stream_slot;

/* What a call holds of its slot while it runs, read under the lock: a slot
 * given back and issued again meanwhile cannot change what the call uses. */
typedef struct {
    hero_stream_slot *slot;
    hero_stream_fd fd;
    const hero_stream_ops *ops;
    void *payload;
    int64_t kind;
    bool reads;
    bool writes;
    bool appends;
    bool standard;
} hero_stream_use;

static hero_stream_slot hero_stream_slots[HERO_STREAM_SLOTS];
/* Every slot at or above it has never held a stream. */
static uint32_t hero_stream_fresh = HERO_STREAM_STANDARD;
/* The index plus one of the last slot given back, 0 for none. */
static uint32_t hero_stream_given_back;

static _Thread_local int64_t hero_stream_failed_kind;
static _Thread_local int64_t hero_stream_failed_os;

#if defined(_WIN32)
static SRWLOCK hero_stream_lock = SRWLOCK_INIT;
#else
static pthread_mutex_t hero_stream_lock = PTHREAD_MUTEX_INITIALIZER;
#endif

static void hero_stream_enter(void) {
#if defined(_WIN32)
    AcquireSRWLockExclusive(&hero_stream_lock);
#else
    (void)pthread_mutex_lock(&hero_stream_lock);
#endif
}

static void hero_stream_leave(void) {
#if defined(_WIN32)
    ReleaseSRWLockExclusive(&hero_stream_lock);
#else
    (void)pthread_mutex_unlock(&hero_stream_lock);
#endif
}

/* The platform's number for the failure the last system call reported. */
static int64_t hero_stream_os_code(void) {
#if defined(_WIN32)
    return (int64_t)GetLastError();
#else
    return (int64_t)errno;
#endif
}

/* The kind a platform's number is, the same kind on every platform. */
static int64_t hero_stream_kind_of_code(int64_t code) {
    switch (code) {
#if defined(_WIN32)
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
    case ERROR_INVALID_DRIVE:
    case ERROR_BAD_NETPATH:
    case ERROR_BAD_NET_NAME:
        return HERO_STREAM_NOT_FOUND;
    case ERROR_FILE_EXISTS:
    case ERROR_ALREADY_EXISTS:
        return HERO_STREAM_EXISTS;
    case ERROR_ACCESS_DENIED:
    case ERROR_SHARING_VIOLATION:
    case ERROR_LOCK_VIOLATION:
    case ERROR_WRITE_PROTECT:
        return HERO_STREAM_DENIED;
    case ERROR_DIRECTORY:
        return HERO_STREAM_NOT_A_DIRECTORY;
    case ERROR_TOO_MANY_OPEN_FILES:
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
    case ERROR_DISK_FULL:
    case ERROR_HANDLE_DISK_FULL:
    case ERROR_FILENAME_EXCED_RANGE:
        return HERO_STREAM_LIMIT;
    case ERROR_BROKEN_PIPE:
    case ERROR_NO_DATA:
        return HERO_STREAM_CLOSED_PEER;
    case ERROR_INVALID_PARAMETER:
    case ERROR_NEGATIVE_SEEK:
    case ERROR_INVALID_NAME:
    case ERROR_NO_UNICODE_TRANSLATION:
        return HERO_STREAM_INVALID;
    case ERROR_NOT_SUPPORTED:
        return HERO_STREAM_UNSUPPORTED;
#else
    case ENOENT:
        return HERO_STREAM_NOT_FOUND;
    case EEXIST:
        return HERO_STREAM_EXISTS;
    /* `EBADF` on a descriptor this file holds open is the wrong direction:
     * a read of one opened to write, a write of one opened to read. */
    case EACCES:
    case EPERM:
    case EROFS:
    case EBADF:
        return HERO_STREAM_DENIED;
    case ENOTDIR:
        return HERO_STREAM_NOT_A_DIRECTORY;
    case EISDIR:
        return HERO_STREAM_IS_A_DIRECTORY;
    case EMFILE:
    case ENFILE:
    case ENOMEM:
    case ENOSPC:
    case EDQUOT:
    case EFBIG:
    case ENAMETOOLONG:
        return HERO_STREAM_LIMIT;
    case ESPIPE:
        return HERO_STREAM_NOT_SEEKABLE;
    case EPIPE:
        return HERO_STREAM_CLOSED_PEER;
    case EINVAL:
        return HERO_STREAM_INVALID;
#endif
    default:
        return HERO_STREAM_OTHER;
    }
}

static void hero_stream_fail(int64_t kind, int64_t os) {
    hero_stream_failed_kind = kind;
    hero_stream_failed_os = os;
}

static void hero_stream_failed_with(int64_t code) {
    hero_stream_fail(hero_stream_kind_of_code(code), code);
}

static void hero_stream_clear(void) {
    hero_stream_fail(HERO_STREAM_OK, 0);
}

/* A descriptor closed, the last error left as it stands: for the paths that
 * give up one they just made. */
static void hero_stream_shut(hero_stream_fd fd) {
#if defined(_WIN32)
    (void)CloseHandle(fd);
#else
    (void)close(fd);
#endif
}

static hero_stream *hero_stream_handle(uint32_t index, uint64_t life) {
    return (hero_stream *)(uintptr_t)(HERO_HANDLE_TAG(HERO_HANDLE_TAG_STREAM) | (life << HERO_STREAM_INDEX_BITS) | ((uint64_t)index + 1));
}

/* The slot a handle's value names while it holds the life the value was
 * issued for; NULL for any other value. One the program closed while a call
 * was in flight is found only where `closed_too`, which a hold's release
 * asks. Under the lock. */
static hero_stream_slot *hero_stream_find(uint64_t value, bool closed_too) {
    uint64_t low = value & HERO_STREAM_INDEX_MASK;

    if ((value & HERO_HANDLE_TAG_MASK) != HERO_HANDLE_TAG(HERO_HANDLE_TAG_STREAM) || low == 0 || low > hero_stream_fresh) {
        return NULL;
    }
    hero_stream_slot *slot = &hero_stream_slots[low - 1];

    if (!slot->held || (slot->closing && !closed_too) || slot->life != (value & ~HERO_HANDLE_TAG_MASK) >> HERO_STREAM_INDEX_BITS) {
        return NULL;
    }
    return slot;
}

/* The slot given back, what it stood on forgotten. Under the lock, with no
 * call in flight on it. */
static void hero_stream_release_slot(hero_stream_slot *slot) {
    slot->held = false;
    slot->closing = false;
    slot->holds = 0;
    slot->fd = HERO_STREAM_NO_FD;
    slot->ops = NULL;
    slot->payload = NULL;

    if (slot->life < HERO_STREAM_LIFE_MOST) {
        slot->next = hero_stream_given_back;
        hero_stream_given_back = (uint32_t)(slot - hero_stream_slots) + 1;
    }
}

/* A stream given a slot and a new life; NULL with `HERO_STREAM_LIMIT` where
 * every slot holds one, and what it would have stood on is still the
 * caller's. */
static hero_stream *hero_stream_keep(hero_stream_fd fd, const hero_stream_ops *ops, void *payload, int64_t kind, bool reads, bool writes,
                                     bool appends) {
    hero_stream *handle = NULL;
    uint32_t index = 0;
    bool room = true;

    hero_stream_enter();
    if (hero_stream_given_back != 0) {
        index = hero_stream_given_back - 1;
        hero_stream_given_back = hero_stream_slots[index].next;
    } else if (hero_stream_fresh < HERO_STREAM_SLOTS) {
        index = hero_stream_fresh;
        hero_stream_fresh += 1;
    } else {
        room = false;
    }
    if (room) {
        hero_stream_slot *slot = &hero_stream_slots[index];

        slot->life += 1;
        slot->fd = fd;
        slot->ops = ops;
        slot->payload = payload;
        slot->busy = 0;
        slot->holds = 0;
        slot->next = 0;
        slot->kind = (uint8_t)kind;
        slot->reads = reads;
        slot->writes = writes;
        slot->appends = appends;
        slot->held = true;
        slot->closing = false;
        slot->standard = false;
        handle = hero_stream_handle(index, slot->life);
    }
    hero_stream_leave();

    if (!room) {
        hero_stream_fail(HERO_STREAM_LIMIT, 0);
        return NULL;
    }
    hero_stream_clear();
    return handle;
}

/* What a slot holds, copied for a call. Under the lock. */
static void hero_stream_read_slot(hero_stream_slot *slot, hero_stream_use *use) {
    use->slot = slot;
    use->fd = slot->fd;
    use->ops = slot->ops;
    use->payload = slot->payload;
    use->kind = (int64_t)slot->kind;
    use->reads = slot->reads;
    use->writes = slot->writes;
    use->appends = slot->appends;
    use->standard = slot->standard;
}

/* What a stream stood on, released: the table's `close`, or the system's.
 * 0, or -1 with the failure named. */
static int64_t hero_stream_dispose(const hero_stream_use *use) {
    if (use->ops != NULL) {
        hero_stream_clear();
        if (use->ops->close != NULL && use->ops->close(use->payload) < 0) {
            if (hero_stream_failed_kind == HERO_STREAM_OK) {
                hero_stream_fail(HERO_STREAM_OTHER, 0);
            }
            return -1;
        }
        hero_stream_clear();
        return 0;
    }
#if defined(_WIN32)
    if (!CloseHandle(use->fd)) {
        hero_stream_failed_with(hero_stream_os_code());
        return -1;
    }
#else
    if (close(use->fd) != 0 && errno != EINTR) {
        hero_stream_failed_with(hero_stream_os_code());
        return -1;
    }
#endif
    hero_stream_clear();
    return 0;
}

/* One user of a slot gone, a call or a hold. Where the program closed the
 * slot meanwhile and this was the last user, what it stood on is released
 * here, the calling thread's last error left as the call set it. */
static void hero_stream_done(const hero_stream_use *use) {
    hero_stream_slot *slot = use->slot;
    bool last = false;

    hero_stream_enter();
    slot->busy -= 1;
    if (slot->closing && slot->busy == 0) {
        hero_stream_release_slot(slot);
        last = true;
    }
    hero_stream_leave();

    if (last) {
        int64_t kind = hero_stream_failed_kind;
        int64_t os = hero_stream_failed_os;

        (void)hero_stream_dispose(use);
        hero_stream_fail(kind, os);
    }
}

/* What a standard stream stands on now: its number on POSIX, and on Windows
 * what `GetStdHandle` answers, false with `HERO_STREAM_NOT_OPEN` where the
 * process has none. */
static bool hero_stream_standard_now(uint32_t which, hero_stream_fd *fd) {
#if defined(_WIN32)
    HANDLE h = GetStdHandle(which == 0 ? STD_INPUT_HANDLE : which == 1 ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);

    if (h == NULL || h == INVALID_HANDLE_VALUE) {
        hero_stream_fail(HERO_STREAM_NOT_OPEN, h == NULL ? 0 : hero_stream_os_code());
        return false;
    }
    *fd = h;
#else
    *fd = (int)which;
#endif
    return true;
}

/* The slot `s` names, marked busy and copied into `use`; false with
 * `HERO_STREAM_NOT_OPEN` where it names none. */
static bool hero_stream_take(hero_stream *s, hero_stream_use *use) {
    hero_stream_enter();
    hero_stream_slot *slot = hero_stream_find((uint64_t)(uintptr_t)s, false);

    if (slot != NULL) {
        slot->busy += 1;
        hero_stream_read_slot(slot, use);
    }
    hero_stream_leave();

    if (slot == NULL) {
        hero_stream_fail(HERO_STREAM_NOT_OPEN, 0);
        return false;
    }
    if (use->standard && !hero_stream_standard_now((uint32_t)(slot - hero_stream_slots), &use->fd)) {
        hero_stream_done(use);
        return false;
    }
    return true;
}

/* What a descriptor is, one of `HERO_STREAM_KIND_*`, asked of the system;
 * -1 with the failure named. */
#if defined(_WIN32)
static int64_t hero_stream_kind_of_fd(hero_stream_fd fd) {
    DWORD mode = 0;

    SetLastError(NO_ERROR);
    switch (GetFileType(fd)) {
    case FILE_TYPE_DISK:
        return HERO_STREAM_KIND_FILE;
    /* A socket is told a pipe here too (Microsoft's page for `GetFileType`);
     * one a member hands out says what it is itself. */
    case FILE_TYPE_PIPE:
        return HERO_STREAM_KIND_PIPE;
    case FILE_TYPE_CHAR:
        return GetConsoleMode(fd, &mode) ? HERO_STREAM_KIND_TERMINAL : HERO_STREAM_KIND_OTHER;
    default:
        if (GetLastError() != NO_ERROR) {
            hero_stream_failed_with(hero_stream_os_code());
            return -1;
        }
        return HERO_STREAM_KIND_OTHER;
    }
}
#else
static int64_t hero_stream_kind_of_stat(hero_stream_fd fd, const struct stat *st) {
    if (S_ISREG(st->st_mode)) {
        return HERO_STREAM_KIND_FILE;
    }
    if (S_ISFIFO(st->st_mode)) {
        return HERO_STREAM_KIND_PIPE;
    }
    if (S_ISSOCK(st->st_mode)) {
        return HERO_STREAM_KIND_SOCKET;
    }
    if (S_ISCHR(st->st_mode) && isatty(fd) == 1) {
        return HERO_STREAM_KIND_TERMINAL;
    }
    return HERO_STREAM_KIND_OTHER;
}

static int64_t hero_stream_kind_of_fd(hero_stream_fd fd) {
    struct stat st;

    if (fstat(fd, &st) != 0) {
        hero_stream_failed_with(hero_stream_os_code());
        return -1;
    }
    return hero_stream_kind_of_stat(fd, &st);
}
#endif

/* What the stream a call holds is now: as it was made, or, for a standard
 * stream, as the system says at this call. -1 with the failure named. */
static int64_t hero_stream_kind_now(const hero_stream_use *use) {
    if (use->ops == NULL && use->standard) {
        return hero_stream_kind_of_fd(use->fd);
    }
    return use->kind;
}

/* Whether the stream a call holds is a file, with positions and a size;
 * where it is not, false with `otherwise`, or with the system's failure
 * where it could not be asked. */
static bool hero_stream_is_file(const hero_stream_use *use, int64_t otherwise) {
    int64_t kind = hero_stream_kind_now(use);

    if (kind < 0) {
        return false;
    }
    if (kind != HERO_STREAM_KIND_FILE) {
        hero_stream_fail(otherwise, 0);
        return false;
    }
    return true;
}

/* -- opening ------------------------------------------------------------- */

/* The file at `path` opened for `access` into `out`, what it is in `kind`;
 * false with the failure named, nothing left open. */
#if defined(_WIN32)
static bool hero_stream_open_fd(const char *path, int64_t access, int64_t perm, hero_stream_fd *out, int64_t *kind) {
    wchar_t wide[HERO_STREAM_PATH_MOST];
    DWORD rights = 0;
    DWORD creation = 0;

    /* Windows has no permission bits: a file created takes its folder's. */
    (void)perm;
    int units = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path, -1, NULL, 0);

    if (units <= 0) {
        hero_stream_failed_with(hero_stream_os_code());
        return false;
    }
    if (units > HERO_STREAM_PATH_MOST) {
        hero_stream_fail(HERO_STREAM_LIMIT, ERROR_FILENAME_EXCED_RANGE);
        return false;
    }
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path, -1, wide, units) != units) {
        hero_stream_failed_with(hero_stream_os_code());
        return false;
    }
    switch (access) {
    case HERO_STREAM_ACCESS_READ:
        rights = GENERIC_READ;
        creation = OPEN_EXISTING;
        break;
    case HERO_STREAM_ACCESS_WRITE:
        rights = GENERIC_WRITE;
        creation = OPEN_ALWAYS;
        break;
    case HERO_STREAM_ACCESS_CREATE:
        rights = GENERIC_WRITE;
        creation = CREATE_NEW;
        break;
    case HERO_STREAM_ACCESS_APPEND:
        rights = FILE_GENERIC_WRITE & ~(DWORD)FILE_WRITE_DATA;
        creation = OPEN_ALWAYS;
        break;
    default:
        rights = GENERIC_READ | GENERIC_WRITE;
        creation = OPEN_EXISTING;
        break;
    }
    HANDLE h = CreateFileW(wide, rights, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD code = GetLastError();

    if (h == INVALID_HANDLE_VALUE) {
        /* A folder is refused as access denied; the path is asked what it
         * is, so the program is told the folder. To create, a folder is
         * something already there, `HERO_STREAM_EXISTS`, as POSIX's
         * `O_CREAT | O_EXCL` answers `EEXIST` before it asks what is there. */
        DWORD attributes = code == ERROR_ACCESS_DENIED ? GetFileAttributesW(wide) : INVALID_FILE_ATTRIBUTES;

        if (attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
            hero_stream_fail(creation == CREATE_NEW ? HERO_STREAM_EXISTS : HERO_STREAM_IS_A_DIRECTORY, (int64_t)code);
        } else {
            hero_stream_failed_with((int64_t)code);
        }
        return false;
    }
    *kind = hero_stream_kind_of_fd(h);
    if (*kind < 0) {
        (void)CloseHandle(h);
        return false;
    }
    /* `OPEN_ALWAYS` says a file was there already by this last error; it is
     * emptied here, the position standing at its start. */
    if (access == HERO_STREAM_ACCESS_WRITE && code == ERROR_ALREADY_EXISTS && *kind == HERO_STREAM_KIND_FILE && !SetEndOfFile(h)) {
        code = GetLastError();
        (void)CloseHandle(h);
        hero_stream_failed_with((int64_t)code);
        return false;
    }
    *out = h;
    return true;
}
#else
static bool hero_stream_open_fd(const char *path, int64_t access, int64_t perm, hero_stream_fd *out, int64_t *kind) {
    /* Never the terminal that controls the process by the accident of
     * opening one, and never a child's. */
    int flags = O_CLOEXEC | O_NOCTTY;
    struct stat st;
    int fd = -1;

    switch (access) {
    case HERO_STREAM_ACCESS_READ:
        flags |= O_RDONLY;
        break;
    case HERO_STREAM_ACCESS_WRITE:
        flags |= O_WRONLY | O_CREAT | O_TRUNC;
        break;
    case HERO_STREAM_ACCESS_CREATE:
        flags |= O_WRONLY | O_CREAT | O_EXCL;
        break;
    case HERO_STREAM_ACCESS_APPEND:
        flags |= O_WRONLY | O_CREAT | O_APPEND;
        break;
    default:
        flags |= O_RDWR;
        break;
    }
    do {
        fd = open(path, flags, (mode_t)perm);
    } while (fd < 0 && errno == EINTR);
    if (fd < 0) {
        hero_stream_failed_with(hero_stream_os_code());
        return false;
    }
    if (fstat(fd, &st) != 0) {
        int64_t code = hero_stream_os_code();

        (void)close(fd);
        hero_stream_failed_with(code);
        return false;
    }
    /* A folder opens to read on POSIX and fails only at the read. */
    if (S_ISDIR(st.st_mode)) {
        (void)close(fd);
        hero_stream_fail(HERO_STREAM_IS_A_DIRECTORY, 0);
        return false;
    }
    *kind = hero_stream_kind_of_stat(fd, &st);
#if defined(HERO_STREAM_MARKS)
    /* A named pipe, or a device, opened here is this process's own open
     * file, so the mark is set once; one that will not take it and is a
     * pipe could end the program at a write, and is refused. */
    if (*kind != HERO_STREAM_KIND_FILE && fcntl(fd, F_SETNOSIGPIPE, 1) != 0 && *kind == HERO_STREAM_KIND_PIPE) {
        int64_t code = hero_stream_os_code();

        (void)close(fd);
        hero_stream_failed_with(code);
        return false;
    }
#endif
    *out = fd;
    return true;
}
#endif

hero_stream *hero_stream_open(const char *path, int64_t access, int64_t perm) {
    hero_stream_fd fd = HERO_STREAM_NO_FD;
    int64_t kind = 0;

    if (path == NULL || access < HERO_STREAM_ACCESS_READ || access > HERO_STREAM_ACCESS_READ_WRITE || perm < 0 || perm > 0777) {
        hero_stream_fail(HERO_STREAM_INVALID, 0);
        return NULL;
    }
    if (!hero_stream_open_fd(path, access, perm, &fd, &kind)) {
        return NULL;
    }
    hero_stream *s = hero_stream_keep(fd, NULL, NULL, kind, access == HERO_STREAM_ACCESS_READ || access == HERO_STREAM_ACCESS_READ_WRITE,
                                      access != HERO_STREAM_ACCESS_READ, access == HERO_STREAM_ACCESS_APPEND);

    if (s == NULL) {
        hero_stream_shut(fd);
    }
    return s;
}

hero_stream *hero_stream_std(int64_t which) {
    hero_stream_fd fd = HERO_STREAM_NO_FD;

    if (which < HERO_STREAM_STD_IN || which > HERO_STREAM_STD_ERR) {
        hero_stream_fail(HERO_STREAM_INVALID, 0);
        return NULL;
    }
    if (!hero_stream_standard_now((uint32_t)which, &fd)) {
        return NULL;
    }
#if !defined(_WIN32)
    /* A process started with the descriptor closed has no such stream. */
    if (fcntl(fd, F_GETFD) < 0) {
        hero_stream_fail(HERO_STREAM_NOT_OPEN, hero_stream_os_code());
        return NULL;
    }
#endif
    hero_stream_enter();
    hero_stream_slot *slot = &hero_stream_slots[which];

    if (!slot->held) {
        slot->fd = fd;
        slot->ops = NULL;
        slot->payload = NULL;
        slot->life = 1;
        slot->busy = 0;
        slot->holds = 0;
        slot->next = 0;
        slot->kind = 0;
        slot->reads = true;
        slot->writes = true;
        slot->appends = false;
        slot->closing = false;
        slot->standard = true;
        slot->held = true;
    }
    hero_stream_leave();

    hero_stream_clear();
    return hero_stream_handle((uint32_t)which, 1);
}

/* A pipe's two ends in `ends`, the reading one first, neither handed to a
 * child, and on Darwin the writing one marked; false with the failure named,
 * nothing left open. */
static bool hero_stream_pipe_fds(hero_stream_fd ends[2]) {
#if defined(_WIN32)
    if (!CreatePipe(&ends[0], &ends[1], NULL, 0)) {
        hero_stream_failed_with(hero_stream_os_code());
        return false;
    }
#elif defined(__linux__)
    if (pipe2(ends, O_CLOEXEC) != 0) {
        hero_stream_failed_with(hero_stream_os_code());
        return false;
    }
#else
    if (pipe(ends) != 0) {
        hero_stream_failed_with(hero_stream_os_code());
        return false;
    }
    if (fcntl(ends[0], F_SETFD, FD_CLOEXEC) != 0 || fcntl(ends[1], F_SETFD, FD_CLOEXEC) != 0 || fcntl(ends[1], F_SETNOSIGPIPE, 1) != 0) {
        int64_t code = hero_stream_os_code();

        (void)close(ends[0]);
        (void)close(ends[1]);
        hero_stream_failed_with(code);
        return false;
    }
#endif
    return true;
}

int64_t hero_stream_pipe(hero_stream **reader, hero_stream **writer) {
    hero_stream_fd ends[2] = { HERO_STREAM_NO_FD, HERO_STREAM_NO_FD };

    if (reader == NULL || writer == NULL) {
        hero_stream_fail(HERO_STREAM_INVALID, 0);
        return -1;
    }
    *reader = NULL;
    *writer = NULL;
    if (!hero_stream_pipe_fds(ends)) {
        return -1;
    }
    hero_stream *r = hero_stream_keep(ends[0], NULL, NULL, HERO_STREAM_KIND_PIPE, true, false, false);
    hero_stream *w = r != NULL ? hero_stream_keep(ends[1], NULL, NULL, HERO_STREAM_KIND_PIPE, false, true, false) : NULL;

    if (w == NULL) {
        if (r != NULL) {
            (void)hero_stream_close(r);
        } else {
            hero_stream_shut(ends[0]);
        }
        hero_stream_shut(ends[1]);
        hero_stream_fail(HERO_STREAM_LIMIT, 0);
        return -1;
    }
    *reader = r;
    *writer = w;
    hero_stream_clear();
    return 0;
}

/* -- reading and writing ------------------------------------------------- */

/* What a function of a member's table answered, held to the header's
 * words: a count within what was asked, or -1 with a failure named. */
static int64_t hero_stream_answered(int64_t result, uint64_t count) {
    if (result >= 0 && (uint64_t)result <= count) {
        hero_stream_clear();
        return result;
    }
    if (result >= 0 || hero_stream_failed_kind == HERO_STREAM_OK) {
        hero_stream_fail(HERO_STREAM_OTHER, 0);
    }
    return -1;
}

int64_t hero_stream_read(hero_stream *s, void *buffer, uint64_t count) {
    hero_stream_use use;
    int64_t got = -1;

    if (buffer == NULL || count == 0) {
        hero_stream_fail(HERO_STREAM_INVALID, 0);
        return -1;
    }
    if (!hero_stream_take(s, &use)) {
        return -1;
    }
    if (count > HERO_STREAM_CHUNK) {
        count = HERO_STREAM_CHUNK;
    }
    if (use.ops != NULL) {
        if (use.ops->read == NULL) {
            hero_stream_fail(HERO_STREAM_UNSUPPORTED, 0);
        } else {
            hero_stream_clear();
            got = hero_stream_answered(use.ops->read(use.payload, buffer, count), count);
        }
    } else if (!use.reads) {
        hero_stream_fail(HERO_STREAM_DENIED, 0);
    } else {
#if defined(_WIN32)
        DWORD moved = 0;

        if (ReadFile(use.fd, buffer, (DWORD)count, &moved, NULL)) {
            hero_stream_clear();
            got = (int64_t)moved;
        } else {
            int64_t code = hero_stream_os_code();

            /* A pipe whose writers are all closed is at its end. */
            if (code == ERROR_BROKEN_PIPE || code == ERROR_HANDLE_EOF) {
                hero_stream_clear();
                got = 0;
            } else {
                hero_stream_failed_with(code);
            }
        }
#else
        ssize_t moved = 0;

        do {
            moved = read(use.fd, buffer, (size_t)count);
        } while (moved < 0 && errno == EINTR);
        if (moved < 0) {
            hero_stream_failed_with(hero_stream_os_code());
        } else {
            hero_stream_clear();
            got = (int64_t)moved;
        }
#endif
    }
    hero_stream_done(&use);
    return got;
}

#if defined(HERO_STREAM_MARKS)
/* Writes to a standard stream in flight, and the standard descriptors whose
 * open file this file marked for them, a bit each: under the lock. */
static uint32_t hero_stream_quiet_writers;
static uint32_t hero_stream_quiet_marked;

typedef struct {
    bool on;
} hero_stream_quiet;

/* Before a write to a standard stream: its open file marked, where it was
 * not, so the write raises no signal (the opening comment). */
static void hero_stream_quiet_begin(const hero_stream_use *use, hero_stream_quiet *q) {
    q->on = use->standard;
    if (!q->on) {
        return;
    }
    hero_stream_enter();
    hero_stream_quiet_writers += 1;
    if (fcntl(use->fd, F_GETNOSIGPIPE) == 0 && fcntl(use->fd, F_SETNOSIGPIPE, 1) == 0) {
        hero_stream_quiet_marked |= UINT32_C(1) << (unsigned)use->fd;
    }
    hero_stream_leave();
}

/* After it: with no write to a standard stream left in flight, every mark
 * this file set is taken away, each open file left as it was found. */
static void hero_stream_quiet_end(hero_stream_quiet *q, bool raised) {
    (void)raised;
    if (!q->on) {
        return;
    }
    hero_stream_enter();
    hero_stream_quiet_writers -= 1;
    if (hero_stream_quiet_writers == 0) {
        for (int fd = 0; fd < (int)HERO_STREAM_STANDARD; fd += 1) {
            if ((hero_stream_quiet_marked & (UINT32_C(1) << (unsigned)fd)) != 0) {
                (void)fcntl(fd, F_SETNOSIGPIPE, 0);
            }
        }
        hero_stream_quiet_marked = 0;
    }
    hero_stream_leave();
}
#elif defined(HERO_STREAM_BLOCKS)
typedef struct {
    sigset_t was;
    /* The signal is blocked on this thread for the write. */
    bool on;
    /* One was pending before the write: somebody else's, left pending. */
    bool pending;
} hero_stream_quiet;

/* Before a write to what may be a pipe: SIGPIPE blocked on this thread. A
 * file and a terminal this file opened raise none and pay nothing. */
static void hero_stream_quiet_begin(const hero_stream_use *use, hero_stream_quiet *q) {
    sigset_t pipe_only;
    sigset_t waiting;

    q->on = false;
    q->pending = false;
    if (!use->standard && (use->kind == HERO_STREAM_KIND_FILE || use->kind == HERO_STREAM_KIND_TERMINAL)) {
        return;
    }
    (void)sigemptyset(&pipe_only);
    (void)sigaddset(&pipe_only, SIGPIPE);
    q->on = pthread_sigmask(SIG_BLOCK, &pipe_only, &q->was) == 0;
    /* One can be pending only where the thread blocked it already. */
    q->pending = q->on && sigismember(&q->was, SIGPIPE) == 1 && sigpending(&waiting) == 0 && sigismember(&waiting, SIGPIPE) == 1;
}

/* After it: the signal the write raised taken back, and the thread's mask
 * as it was. */
static void hero_stream_quiet_end(hero_stream_quiet *q, bool raised) {
    if (!q->on) {
        return;
    }
    if (raised && !q->pending) {
        struct timespec none = { 0, 0 };
        sigset_t pipe_only;

        (void)sigemptyset(&pipe_only);
        (void)sigaddset(&pipe_only, SIGPIPE);
        while (sigtimedwait(&pipe_only, NULL, &none) < 0 && errno == EINTR) {
        }
    }
    (void)pthread_sigmask(SIG_SETMASK, &q->was, NULL);
}
#endif

int64_t hero_stream_write(hero_stream *s, const void *data, uint64_t count) {
    hero_stream_use use;
    int64_t put = -1;

    if (data == NULL && count != 0) {
        hero_stream_fail(HERO_STREAM_INVALID, 0);
        return -1;
    }
    if (!hero_stream_take(s, &use)) {
        return -1;
    }
    if (count > HERO_STREAM_CHUNK) {
        count = HERO_STREAM_CHUNK;
    }
    if (use.ops != NULL) {
        if (use.ops->write == NULL) {
            hero_stream_fail(HERO_STREAM_UNSUPPORTED, 0);
        } else if (count == 0) {
            hero_stream_clear();
            put = 0;
        } else {
            hero_stream_clear();
            put = hero_stream_answered(use.ops->write(use.payload, data, count), count);
        }
    } else if (!use.writes) {
        hero_stream_fail(HERO_STREAM_DENIED, 0);
    } else if (count == 0) {
        hero_stream_clear();
        put = 0;
    } else {
#if defined(_WIN32)
        DWORD moved = 0;

        if (WriteFile(use.fd, data, (DWORD)count, &moved, NULL)) {
            hero_stream_clear();
            put = (int64_t)moved;
        } else {
            hero_stream_failed_with(hero_stream_os_code());
        }
#else
        hero_stream_quiet quiet;
        ssize_t moved = 0;

        hero_stream_quiet_begin(&use, &quiet);
        do {
            moved = write(use.fd, data, (size_t)count);
        } while (moved < 0 && errno == EINTR);
        int64_t code = moved < 0 ? hero_stream_os_code() : 0;

        hero_stream_quiet_end(&quiet, moved < 0 && code == EPIPE);
        if (moved < 0) {
            hero_stream_failed_with(code);
        } else {
            hero_stream_clear();
            put = (int64_t)moved;
        }
#endif
    }
    hero_stream_done(&use);
    return put;
}

/* -- positions, size, storage -------------------------------------------- */

int64_t hero_stream_seek(hero_stream *s, int64_t offset, int64_t from) {
    hero_stream_use use;
    int64_t at = -1;

    if (from < HERO_STREAM_FROM_START || from > HERO_STREAM_FROM_END) {
        hero_stream_fail(HERO_STREAM_INVALID, 0);
        return -1;
    }
    if (!hero_stream_take(s, &use)) {
        return -1;
    }
    int64_t kind = hero_stream_kind_now(&use);

    if (kind < 0) {
        /* The failure is named already. */
    } else if (use.ops != NULL || kind == HERO_STREAM_KIND_PIPE || kind == HERO_STREAM_KIND_TERMINAL || kind == HERO_STREAM_KIND_SOCKET) {
        hero_stream_fail(HERO_STREAM_NOT_SEEKABLE, 0);
    } else {
#if defined(_WIN32)
        LARGE_INTEGER move;
        LARGE_INTEGER reached;

        move.QuadPart = offset;
        reached.QuadPart = 0;
        /* Microsoft's page: no pointer is set on a device that does not
         * seek, and `GetFileType` is how to know one. */
        if (kind != HERO_STREAM_KIND_FILE) {
            hero_stream_fail(HERO_STREAM_NOT_SEEKABLE, 0);
        } else if (!SetFilePointerEx(use.fd, move, &reached,
                                     from == HERO_STREAM_FROM_START ? FILE_BEGIN : from == HERO_STREAM_FROM_CURRENT ? FILE_CURRENT : FILE_END)) {
            hero_stream_failed_with(hero_stream_os_code());
        } else {
            hero_stream_clear();
            at = (int64_t)reached.QuadPart;
        }
#else
        off_t reached = lseek(use.fd, (off_t)offset, from == HERO_STREAM_FROM_START ? SEEK_SET : from == HERO_STREAM_FROM_CURRENT ? SEEK_CUR : SEEK_END);

        if (reached < 0) {
            hero_stream_failed_with(hero_stream_os_code());
        } else {
            hero_stream_clear();
            at = (int64_t)reached;
        }
#endif
    }
    hero_stream_done(&use);
    return at;
}

int64_t hero_stream_size(hero_stream *s) {
    hero_stream_use use;
    int64_t size = -1;

    if (!hero_stream_take(s, &use)) {
        return -1;
    }
    if (use.ops != NULL) {
        hero_stream_fail(HERO_STREAM_NOT_SEEKABLE, 0);
    } else if (hero_stream_is_file(&use, HERO_STREAM_NOT_SEEKABLE)) {
#if defined(_WIN32)
        LARGE_INTEGER bytes;

        bytes.QuadPart = 0;
        if (!GetFileSizeEx(use.fd, &bytes)) {
            hero_stream_failed_with(hero_stream_os_code());
        } else {
            hero_stream_clear();
            size = (int64_t)bytes.QuadPart;
        }
#else
        struct stat st;

        if (fstat(use.fd, &st) != 0) {
            hero_stream_failed_with(hero_stream_os_code());
        } else {
            hero_stream_clear();
            size = (int64_t)st.st_size;
        }
#endif
    }
    hero_stream_done(&use);
    return size;
}

int64_t hero_stream_sync(hero_stream *s) {
    hero_stream_use use;
    int64_t answer = -1;

    if (!hero_stream_take(s, &use)) {
        return -1;
    }
    if (use.ops != NULL) {
        hero_stream_fail(HERO_STREAM_UNSUPPORTED, 0);
    } else if (!hero_stream_is_file(&use, HERO_STREAM_UNSUPPORTED)) {
        /* The failure is named already. */
    } else if (!use.writes) {
        hero_stream_fail(HERO_STREAM_DENIED, 0);
    } else {
#if defined(_WIN32)
        if (!FlushFileBuffers(use.fd)) {
            hero_stream_failed_with(hero_stream_os_code());
        } else {
            hero_stream_clear();
            answer = 0;
        }
#else
        int done = -1;

#if defined(F_FULLFSYNC)
        do {
            done = fcntl(use.fd, F_FULLFSYNC);
        } while (done < 0 && errno == EINTR);
#endif
        if (done < 0) {
            do {
                done = fsync(use.fd);
            } while (done < 0 && errno == EINTR);
        }
        if (done < 0) {
            hero_stream_failed_with(hero_stream_os_code());
        } else {
            hero_stream_clear();
            answer = 0;
        }
#endif
    }
    hero_stream_done(&use);
    return answer;
}

int64_t hero_stream_truncate(hero_stream *s, int64_t size) {
    hero_stream_use use;
    int64_t answer = -1;

    if (size < 0) {
        hero_stream_fail(HERO_STREAM_INVALID, 0);
        return -1;
    }
    if (!hero_stream_take(s, &use)) {
        return -1;
    }
    if (use.ops != NULL) {
        hero_stream_fail(HERO_STREAM_NOT_SEEKABLE, 0);
    } else if (!hero_stream_is_file(&use, HERO_STREAM_NOT_SEEKABLE)) {
        /* The failure is named already. */
    } else if (!use.writes || use.appends) {
        hero_stream_fail(HERO_STREAM_DENIED, 0);
    } else {
#if defined(_WIN32)
        FILE_END_OF_FILE_INFO end;

        /* The end set by itself, the position where it was: `SetEndOfFile`
         * would need the position moved there and back, two more calls
         * another thread's read could fall between. */
        end.EndOfFile.QuadPart = size;
        if (!SetFileInformationByHandle(use.fd, FileEndOfFileInfo, &end, (DWORD)sizeof end)) {
            hero_stream_failed_with(hero_stream_os_code());
        } else {
            hero_stream_clear();
            answer = 0;
        }
#else
        int done = -1;

        do {
            done = ftruncate(use.fd, (off_t)size);
        } while (done < 0 && errno == EINTR);
        if (done < 0) {
            int64_t code = hero_stream_os_code();

            /* What is left of these two, the size and the kind judged
             * above, is a standard stream not open to write. */
            if (code == EINVAL || code == EBADF) {
                hero_stream_fail(HERO_STREAM_DENIED, code);
            } else {
                hero_stream_failed_with(code);
            }
        } else {
            hero_stream_clear();
            answer = 0;
        }
#endif
    }
    hero_stream_done(&use);
    return answer;
}

/* -- what a stream is ---------------------------------------------------- */

int64_t hero_stream_kind(hero_stream *s) {
    hero_stream_use use;

    if (!hero_stream_take(s, &use)) {
        return -1;
    }
    int64_t kind = hero_stream_kind_now(&use);

    if (kind >= 0) {
        hero_stream_clear();
    }
    hero_stream_done(&use);
    return kind;
}

int64_t hero_stream_is_terminal(hero_stream *s) {
    hero_stream_use use;
    int64_t answer = 0;

    if (!hero_stream_take(s, &use)) {
        return -1;
    }
    if (use.ops != NULL) {
        answer = use.kind == HERO_STREAM_KIND_TERMINAL ? 1 : 0;
    } else {
#if defined(_WIN32)
        DWORD mode = 0;

        answer = GetConsoleMode(use.fd, &mode) ? 1 : 0;
#else
        answer = isatty(use.fd) == 1 ? 1 : 0;
#endif
    }
    hero_stream_clear();
    hero_stream_done(&use);
    return answer;
}

/* -- closing ------------------------------------------------------------- */

int64_t hero_stream_close(hero_stream *s) {
    hero_stream_use use;
    /* 0 it names no open stream, 1 a standard one, 2 it is released here,
     * 3 by the last call still in flight on it. */
    int found = 0;

    hero_stream_enter();
    hero_stream_slot *slot = hero_stream_find((uint64_t)(uintptr_t)s, false);

    if (slot != NULL && slot->standard) {
        found = 1;
    } else if (slot != NULL) {
        hero_stream_read_slot(slot, &use);
        slot->closing = true;
        found = 3;
        if (slot->busy == 0) {
            hero_stream_release_slot(slot);
            found = 2;
        }
    }
    hero_stream_leave();

    if (found == 0) {
        hero_stream_fail(HERO_STREAM_NOT_OPEN, 0);
        return -1;
    }
    if (found == 1) {
        hero_stream_fail(HERO_STREAM_INVALID, 0);
        return -1;
    }
    if (found == 2) {
        return hero_stream_dispose(&use);
    }
    hero_stream_clear();
    return 0;
}

int64_t hero_stream_last_error(void) {
    return hero_stream_failed_kind;
}

int64_t hero_stream_last_os_error(void) {
    return hero_stream_failed_os;
}

/* -- for the other members of the layer ---------------------------------- */

hero_stream *hero_stream_make(void *payload, const hero_stream_ops *ops, int64_t kind) {
    if (ops == NULL || kind < HERO_STREAM_KIND_FILE || kind > HERO_STREAM_KIND_OTHER) {
        hero_stream_fail(HERO_STREAM_INVALID, 0);
        return NULL;
    }
    return hero_stream_keep(HERO_STREAM_NO_FD, ops, payload, kind, true, true, false);
}

void hero_stream_set_error(int64_t kind, int64_t os_error) {
    hero_stream_fail(kind, os_error);
}

/* The descriptor a call holds, as the header's number; -1 with
 * `HERO_STREAM_UNSUPPORTED` for a stream that stands on none. */
static int64_t hero_stream_number_of(const hero_stream_use *use) {
    if (use->ops != NULL) {
        hero_stream_fail(HERO_STREAM_UNSUPPORTED, 0);
        return -1;
    }
    hero_stream_clear();
#if defined(_WIN32)
    return (int64_t)(intptr_t)use->fd;
#else
    return (int64_t)use->fd;
#endif
}

int64_t hero_stream_descriptor(hero_stream *s) {
    hero_stream_use use;

    if (!hero_stream_take(s, &use)) {
        return -1;
    }
    int64_t number = hero_stream_number_of(&use);

    hero_stream_done(&use);
    return number;
}

int64_t hero_stream_hold(hero_stream *s) {
    hero_stream_use use;

    if (!hero_stream_take(s, &use)) {
        return -1;
    }
    int64_t number = hero_stream_number_of(&use);

    if (number < 0) {
        hero_stream_done(&use);
        return -1;
    }
    /* The call's own use of the slot stays, as the hold, until the
     * release. */
    hero_stream_enter();
    use.slot->holds += 1;
    hero_stream_leave();
    return number;
}

int64_t hero_stream_hold_payload(hero_stream *s, const hero_stream_ops *ops, void **payload) {
    hero_stream_use use;

    if (ops == NULL || payload == NULL) {
        hero_stream_fail(HERO_STREAM_INVALID, 0);
        return -1;
    }
    if (!hero_stream_take(s, &use)) {
        return -1;
    }
    if (use.ops != ops) {
        hero_stream_done(&use);
        hero_stream_fail(HERO_STREAM_INVALID, 0);
        return -1;
    }
    /* The call's own use of the slot stays, as the hold, until the
     * release, as in `hero_stream_hold`. */
    hero_stream_enter();
    use.slot->holds += 1;
    hero_stream_leave();
    *payload = use.payload;
    hero_stream_clear();
    return 0;
}

int64_t hero_stream_release(hero_stream *s) {
    hero_stream_use use;
    bool held = false;

    hero_stream_enter();
    hero_stream_slot *slot = hero_stream_find((uint64_t)(uintptr_t)s, true);

    if (slot != NULL && slot->holds > 0) {
        slot->holds -= 1;
        hero_stream_read_slot(slot, &use);
        held = true;
    }
    hero_stream_leave();

    if (!held) {
        hero_stream_fail(HERO_STREAM_NOT_OPEN, 0);
        return -1;
    }
    hero_stream_done(&use);
    hero_stream_clear();
    return 0;
}
