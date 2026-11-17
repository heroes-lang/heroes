/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_proc.c — the platform layer's child processes, one member of
 * the archive the compiler puts on every program's link line (M-rebel-library;
 * `selfhost/cli/layer.hero` builds, keys and links it). Its header,
 * `hero_proc.h`, says what each function answers; this file says how each
 * platform gets there.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so an arm that
 * defines another signature than the header's is `conflicting types` where
 * it is compiled.
 *
 * A PROCESS IS A SLOT OF THE TABLE BELOW, NEVER AN ADDRESS THIS FILE READS
 * THROUGH, AND NEVER A BARE PID. The handle a program holds is the slot's
 * index and the life the slot held when it was issued, so a handle released
 * already names a life that ended and is `HERO_PROC_NOT_OPEN`, as are NULL
 * and a value this layer never issued. The table is fixed, 4,096 processes
 * held or still watched, since the runtime's allocator is reached only
 * through `parts/alloc.c` (the net's `runtime` suite, rule 1) and a member
 * of the layer cannot reach that file's functions: so this member allocates
 * nothing. Its lists are built on the calling thread's stack, each with a
 * bound the header states, and a launch past one is `HERO_PROC_LIMIT`.
 *
 * NO SIGNAL EVER GOES TO A PID THE SYSTEM MAY HAVE GIVEN AWAY (POSIX). A pid
 * is the child's until the child is waited for, and from then the system
 * hands it to any new process. So the one call that takes a child's status,
 * `wait4`, is made UNDER THE TABLE'S LOCK and never blocks there (`WNOHANG`),
 * and `kill` is made under the same lock, only while the slot says nothing
 * has taken the status yet. The call that blocks, in `hero_proc_wait`, is
 * `waitid` with `WNOWAIT`, outside the lock: it returns when the child has
 * ended and leaves it waitable, its pid still its own.
 *
 * A CALL IN FLIGHT KEEPS ITS SLOT. Each call marks its slot busy; a release
 * meanwhile, from another thread, marks it closing, and the last call to
 * leave gives the slot back. On Windows that is when the process's `HANDLE`
 * closes, never under a `WaitForSingleObject` another thread is in.
 *
 * A PROCESS RELEASED WHILE IT RUNS LEAVES NO ZOMBIE (POSIX). Its slot stays
 * the layer's, and one detached thread of the layer's own, on a small stack
 * and with every signal but a fault's held off, blocks in `waitpid` for that
 * one child and gives the slot back when it ends: no handle names the slot
 * any more, so nothing can signal the pid it frees. Where the system gives no
 * thread, the slot is looked at (`WNOHANG`) on every later start and release.
 * Windows has no zombie: the process's object goes with its last handle.
 *
 * THE THREE STREAMS COME FROM `hero_stream.h`, AND THIS FILE OPENS NOTHING.
 * The parent's own is `hero_stream_std`, nothing is the null device opened
 * by `hero_stream_open`, a new pipe is `hero_stream_pipe`, whose other end
 * the program takes, and each is passed to the child through
 * `hero_stream_hold`, which keeps its descriptor open across the launch
 * whatever another thread closes. Everything that member opens is
 * close-on-exec, or not inheritable, so the child gets a stream only by this
 * file's act: `dup2` onto 0, 1 and 2 (a spawn's file action, or after the
 * fork), or `DuplicateHandle` asking an inheritable copy that is named in the
 * child's handle list and closed after the launch.
 *
 * A DESCRIPTOR BELOW 3 IS COPIED ABOVE THEM FIRST (POSIX), with
 * `F_DUPFD_CLOEXEC`: `dup2` of a descriptor onto itself changes nothing, its
 * close-on-exec mark included, and the three `dup2`s could overwrite a
 * source one of them still needs (the parent's output as the child's error
 * beside its error as the child's output).
 *
 * POSIX STARTS WITH `posix_spawn`, AND FORKS ONLY FOR A FOLDER IT CANNOT SAY.
 * A working folder is the one thing asked that is not portable there: Darwin
 * has `posix_spawn_file_actions_addchdir` from macOS 26 and deprecates the
 * `_np` name it had from 10.15, which this file calls before 26 (this Mac's
 * SDK, read 2026-11-15); glibc has the `_np` name alone, from 2.29, and
 * declares it only under `_GNU_SOURCE` (Debian's 2.41, read the same day),
 * which a member cannot define before its own header, so it is declared
 * below, as is 2.34's `addclosefrom_np`, which leaves a Linux child its three
 * descriptors alone. Elsewhere, and with a folder
 * alone, the child is made by `fork` and calls only what POSIX promises
 * after a fork in a process with threads (`dup2`, `close`, `chdir`,
 * `sigaction`, `sigprocmask`, `execve`, `write`, `_exit`), and tells its
 * parent why it could not start through a close-on-exec pipe: a successful
 * `execve` closes the pipe and the parent reads nothing.
 *
 * THE PROGRAM IS FOUND HERE, BEFORE ANY CHILD EXISTS (POSIX), in this
 * process's `PATH`, so `HERO_PROC_NOT_FOUND` is told by the start on every C
 * library, also one whose `posix_spawn` reports a failed `execve` only as a
 * child that exits 127. With a folder the path found is written whole, since
 * the child reads it after it has entered the folder.
 *
 * DARWIN HAS NO CALL THAT MAKES A PIPE CLOSE-ON-EXEC IN ONE STEP, so between
 * `pipe` and `fcntl` another thread's child could inherit an end and hold it
 * open. This file's own children cannot: there `POSIX_SPAWN_CLOEXEC_DEFAULT`
 * closes in the child every descriptor no file action names. Windows has the
 * same window between `DuplicateHandle` and the launch, for a child another
 * thread starts with every inheritable handle and no list; this file's
 * children take their list alone.
 *
 * WINDOWS: `CreateProcessW` with `STARTUPINFOEXW` and
 * `PROC_THREAD_ATTRIBUTE_HANDLE_LIST`; the line by the rule the C runtime
 * and `CommandLineToArgvW` undo (Microsoft's "Parsing C command-line
 * arguments"); a given environment as a UTF-16 block sorted by name without
 * case (Microsoft's "Changing Environment Variables"). UNRUN AND UNCOMPILED
 * on the day it was written, 2026-11-15: this Mac has no Windows headers.
 */

#include "hero_proc.h"

/* The tag that keeps this table's handles apart from every other member's
 * (2026-11-16): `HERO_HANDLE_TAG_PROC`. */
#include "hero_handle_tag.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#include <limits.h>
#include <stdlib.h>
#include <wchar.h>
#else
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <spawn.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
extern char **environ;
/* Whether a spawn's file actions can name the child's folder, and by which
 * name (the opening comment says who has which). `HERO_PROC_NO_SPAWN_FOLDER`
 * takes it away, so a test can run the fork on a machine that has it. */
#if !defined(HERO_PROC_NO_SPAWN_FOLDER)
#if defined(__APPLE__)
#include <Availability.h>
#define HERO_PROC_SPAWN_FOLDER_DARWIN 1
#elif defined(__GLIBC__) && defined(__GLIBC_PREREQ)
#if __GLIBC_PREREQ(2, 29)
#define HERO_PROC_SPAWN_FOLDER_GLIBC 1
int posix_spawn_file_actions_addchdir_np(posix_spawn_file_actions_t *restrict actions, const char *restrict path);
#endif
#endif
#endif
/* glibc 2.34 closes in a spawned child every descriptor from a number up,
 * so the child keeps its three alone, as Darwin's
 * `POSIX_SPAWN_CLOEXEC_DEFAULT` does; declared, as above, only under
 * `_GNU_SOURCE`. */
#if defined(__GLIBC__) && defined(__GLIBC_PREREQ)
#if __GLIBC_PREREQ(2, 34)
#define HERO_PROC_SPAWN_CLOSEFROM 1
int posix_spawn_file_actions_addclosefrom_np(posix_spawn_file_actions_t *actions, int from);
#endif
#endif
/* Linux makes a pipe close-on-exec in one step; glibc and musl declare the
 * call only under `_GNU_SOURCE`. */
#if defined(__linux__)
int pipe2(int ends[2], int flags);
#endif
#if !defined(NSIG)
#define NSIG 65
#endif
#endif

/* A handle carries this table's tag, a slot and a life in one pointer's 64
 * bits: `HERO_HANDLE_TAG(HERO_HANDLE_TAG_PROC) | (life <<
 * HERO_PROC_INDEX_BITS) | (index + 1)`, the tag in bits 58 to 61
 * (`hero_handle_tag.h`), so no handle is NULL and none is another member's
 * (2026-11-16: a socket and a stream at one slot and life were one value
 * until then, which the runtime's set of ended handles mistook). */
_Static_assert(sizeof(void *) == 8 && sizeof(uintptr_t) == 8, "a process handle carries its slot and its life in 64 bits");
#define HERO_PROC_INDEX_BITS 13
#define HERO_PROC_INDEX_MASK ((UINT64_C(1) << HERO_PROC_INDEX_BITS) - 1)
/* The last life a slot is given, 2^45 - 1 since the tag (2^49 - 1 before
 * it): the life stays below the tag, and a slot at it is never issued
 * again. */
#define HERO_PROC_LIFE_MOST HERO_HANDLE_LIFE_MOST(HERO_PROC_INDEX_BITS)
_Static_assert((HERO_PROC_LIFE_MOST << HERO_PROC_INDEX_BITS) < (UINT64_C(1) << HERO_HANDLE_TAG_SHIFT), "a life stays below the tag");
#define HERO_PROC_SLOTS 4096u
_Static_assert(HERO_PROC_SLOTS <= HERO_PROC_INDEX_MASK, "an index plus one fits below a handle's life");

/* The most arguments and variables one launch takes: the pointers to them
 * stand on the calling thread's stack. */
#define HERO_PROC_WORDS_MOST 4096u
#define HERO_PROC_VARIABLES_MOST 4096u

/* The kinds this member hands on from `hero_stream.h` are that header's
 * numbers, and the three streams are numbered as it numbers them. */
_Static_assert(HERO_PROC_INVALID == HERO_STREAM_INVALID && HERO_PROC_NOT_OPEN == HERO_STREAM_NOT_OPEN &&
                   HERO_PROC_NOT_FOUND == HERO_STREAM_NOT_FOUND && HERO_PROC_DENIED == HERO_STREAM_DENIED &&
                   HERO_PROC_LIMIT == HERO_STREAM_LIMIT && HERO_PROC_UNSUPPORTED == HERO_STREAM_UNSUPPORTED &&
                   HERO_PROC_OTHER == HERO_STREAM_OTHER,
               "a kind both headers have is one number");
_Static_assert(HERO_PROC_INPUT == HERO_STREAM_STD_IN && HERO_PROC_OUTPUT == HERO_STREAM_STD_OUT &&
                   HERO_PROC_ERROR == HERO_STREAM_STD_ERR,
               "the child's three streams are numbered as the process's own");

/* What a slot knows of its process. */
#define HERO_PROC_STATE_NONE 0
/* Nothing has taken its status: on POSIX its pid is still its own. */
#define HERO_PROC_STATE_RUNNING 1
/* It ended, and how is kept in the slot. */
#define HERO_PROC_STATE_ENDED 2
/* It is gone and the system would not say how: something else waited for it
 * first. */
#define HERO_PROC_STATE_LOST 3

#if defined(_WIN32)
#define HERO_PROC_NOWHERE "NUL"
/* `CreateProcessW`'s line is at most 32,767 units with its zero; a unit is at
 * most three bytes of UTF-8. */
#define HERO_PROC_LINE_UNITS 32767
#define HERO_PROC_LINE_BYTES (3 * HERO_PROC_LINE_UNITS)
#define HERO_PROC_FOLDER_UNITS 4096
/* The most a given environment holds, in UTF-16 units with every zero. */
#define HERO_PROC_BLOCK_UNITS 32768
/* Room for an attribute list of one attribute, which takes far less. */
#define HERO_PROC_ATTRIBUTE_BYTES 1024
/* The exit code of a process `hero_proc_kill` ended. */
#define HERO_PROC_KILLED 1
#else
#define HERO_PROC_NOWHERE "/dev/null"
#if defined(PATH_MAX)
#define HERO_PROC_PATH_ROOM PATH_MAX
#else
#define HERO_PROC_PATH_ROOM 4096
#endif
/* Where a child made by `fork` could not go on, told to its parent. */
#define HERO_PROC_STAGE_STREAMS 1
#define HERO_PROC_STAGE_FOLDER 2
#define HERO_PROC_STAGE_PROGRAM 3
/* The stack of the thread that waits for a released process: address space,
 * a multiple of every page size, and room for a sanitizer's frames. */
#define HERO_PROC_WATCH_STACK 262144
#endif

typedef struct {
#if defined(_WIN32)
    HANDLE process;
#else
    pid_t pid;
#endif
    /* The system's number for it, for `hero_proc_id`. */
    int64_t id;
    /* The life the slot holds or last held, which its handle names. */
    uint64_t life;
    /* Calls in flight on it. */
    uint32_t busy;
    /* The index plus one of the next slot given back, 0 for none. */
    uint32_t next;
    /* The program's ends of its pipes, each until the program takes it. */
    hero_stream *pipes[3];
    /* How it ended, once `state` is `HERO_PROC_STATE_ENDED`. */
    int64_t code;
    int64_t by_signal;
    /* The CPU time it used, or -1 where the system would not say. */
    int64_t user_ns;
    int64_t system_ns;
    /* The platform's number for why it is `HERO_PROC_STATE_LOST`. */
    int64_t lost;
    uint8_t state;
    /* A handle the program has not released names it. */
    bool held;
    /* The program released it while a call was in flight: the last one
     * gives it back. */
    bool closing;
    /* POSIX: released while it ran, and the layer's until it ends. */
    bool orphan;
    /* POSIX: a thread of the layer waits for it. */
    bool watched;
} hero_proc_slot;

/* The table and its free list, one object under one lock. */
typedef struct {
    hero_proc_slot slots[HERO_PROC_SLOTS];
    /* Every slot at or above it has never held a process. */
    uint32_t fresh;
    /* The index plus one of the last slot given back, 0 for none. */
    uint32_t given_back;
    /* POSIX: released processes still running that no thread waits for. */
    uint32_t unwatched;
} hero_proc_slots;

static hero_proc_slots hero_proc_table;

static _Thread_local int64_t hero_proc_kind;
static _Thread_local int64_t hero_proc_os;

#if defined(_WIN32)
static SRWLOCK hero_proc_lock = SRWLOCK_INIT;
#else
static pthread_mutex_t hero_proc_lock = PTHREAD_MUTEX_INITIALIZER;
#endif

static void hero_proc_enter(void) {
#if defined(_WIN32)
    AcquireSRWLockExclusive(&hero_proc_lock);
#else
    (void)pthread_mutex_lock(&hero_proc_lock);
#endif
}

static void hero_proc_leave(void) {
#if defined(_WIN32)
    ReleaseSRWLockExclusive(&hero_proc_lock);
#else
    (void)pthread_mutex_unlock(&hero_proc_lock);
#endif
}

/* -- the last error, per thread ------------------------------------------- */

static void hero_proc_fail(int64_t kind, int64_t os) {
    hero_proc_kind = kind;
    hero_proc_os = os;
}

static void hero_proc_clear(void) {
    hero_proc_fail(HERO_PROC_OK, 0);
}

/* The kind a platform's number is, the same kind on every platform. */
static int64_t hero_proc_kind_of(int64_t code) {
    switch (code) {
#if defined(_WIN32)
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
    case ERROR_INVALID_NAME:
    case ERROR_BAD_PATHNAME:
    case ERROR_INVALID_DRIVE:
        return HERO_PROC_NOT_FOUND;
    case ERROR_ACCESS_DENIED:
    case ERROR_ELEVATION_REQUIRED:
        return HERO_PROC_DENIED;
    case ERROR_BAD_EXE_FORMAT:
    case ERROR_EXE_MACHINE_TYPE_MISMATCH:
        return HERO_PROC_NOT_A_PROGRAM;
    case ERROR_DIRECTORY:
        return HERO_PROC_NOT_A_DIRECTORY;
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
    case ERROR_TOO_MANY_OPEN_FILES:
    case ERROR_FILENAME_EXCED_RANGE:
    case ERROR_INSUFFICIENT_BUFFER:
        return HERO_PROC_LIMIT;
    case ERROR_NO_UNICODE_TRANSLATION:
        return HERO_PROC_INVALID;
#else
    case ENOENT:
    case ENOTDIR:
        return HERO_PROC_NOT_FOUND;
    case EACCES:
    case EPERM:
        return HERO_PROC_DENIED;
    case ENOEXEC:
#if defined(EBADARCH)
    case EBADARCH:
#endif
#if defined(EBADEXEC)
    case EBADEXEC:
#endif
#if defined(EBADMACHO)
    case EBADMACHO:
#endif
        return HERO_PROC_NOT_A_PROGRAM;
    case EMFILE:
    case ENFILE:
    case ENOMEM:
    case EAGAIN:
    case E2BIG:
    case ENAMETOOLONG:
    case ERANGE:
        return HERO_PROC_LIMIT;
#endif
    default:
        return HERO_PROC_OTHER;
    }
}

static void hero_proc_failed_with(int64_t code) {
    hero_proc_fail(hero_proc_kind_of(code), code);
}

/* The failure `hero_stream.h`'s last call reported, as this member's: its
 * kind where both headers have it, else `HERO_PROC_OTHER`. */
static void hero_proc_fail_as_stream(void) {
    int64_t kind = hero_stream_last_error();
    int64_t os = hero_stream_last_os_error();

    switch (kind) {
    case HERO_STREAM_INVALID:
    case HERO_STREAM_NOT_OPEN:
    case HERO_STREAM_NOT_FOUND:
    case HERO_STREAM_DENIED:
    case HERO_STREAM_LIMIT:
    case HERO_STREAM_UNSUPPORTED:
        hero_proc_fail(kind, os);
        break;
    default:
        hero_proc_fail(HERO_PROC_OTHER, os);
        break;
    }
}

/* -- the table ------------------------------------------------------------ */

static hero_proc *hero_proc_handle(uint32_t index, uint64_t life) {
    return (hero_proc *)(uintptr_t)(HERO_HANDLE_TAG(HERO_HANDLE_TAG_PROC) | (life << HERO_PROC_INDEX_BITS) | ((uint64_t)index + 1));
}

/* The slot `p` names while it holds the life `p` was issued for and the
 * program has not released it; NULL for any other value. Under the lock. */
static hero_proc_slot *hero_proc_find(hero_proc *p) {
    uint64_t value = (uint64_t)(uintptr_t)p;
    uint64_t low = value & HERO_PROC_INDEX_MASK;

    if ((value & HERO_HANDLE_TAG_MASK) != HERO_HANDLE_TAG(HERO_HANDLE_TAG_PROC) || low == 0 || low > hero_proc_table.fresh) {
        return NULL;
    }
    hero_proc_slot *slot = &hero_proc_table.slots[low - 1];

    if (!slot->held || slot->closing || slot->life != (value & ~HERO_HANDLE_TAG_MASK) >> HERO_PROC_INDEX_BITS) {
        return NULL;
    }
    return slot;
}

/* The slot given back, nothing of its process kept. Under the lock, with no
 * call in flight on it. */
static void hero_proc_free(hero_proc_slot *slot) {
    slot->state = HERO_PROC_STATE_NONE;
    slot->held = false;
    slot->closing = false;
    slot->orphan = false;
    slot->watched = false;
    slot->busy = 0;

    if (slot->life < HERO_PROC_LIFE_MOST) {
        slot->next = hero_proc_table.given_back;
        hero_proc_table.given_back = (uint32_t)(slot - hero_proc_table.slots) + 1;
    }
}

#if defined(_WIN32)

/* A `FILETIME`'s count of 100 ns as nanoseconds, the largest where it does
 * not fit. */
static int64_t hero_proc_time_ns(FILETIME span) {
    uint64_t units = ((uint64_t)span.dwHighDateTime << 32) | (uint64_t)span.dwLowDateTime;

    if (units > (uint64_t)INT64_MAX / 100) {
        return INT64_MAX;
    }
    return (int64_t)(units * 100);
}

/* How a process that has ended did, kept in its slot. Under the lock. */
static void hero_proc_collect(hero_proc_slot *slot) {
    DWORD code = 0;
    FILETIME made;
    FILETIME ended;
    FILETIME in_system;
    FILETIME in_user;

    if (!GetExitCodeProcess(slot->process, &code)) {
        slot->lost = (int64_t)GetLastError();
        slot->state = HERO_PROC_STATE_LOST;
        return;
    }
    slot->code = (int64_t)code;
    slot->by_signal = 0;
    slot->user_ns = -1;
    slot->system_ns = -1;
    if (GetProcessTimes(slot->process, &made, &ended, &in_system, &in_user)) {
        slot->user_ns = hero_proc_time_ns(in_user);
        slot->system_ns = hero_proc_time_ns(in_system);
    }
    slot->state = HERO_PROC_STATE_ENDED;
}

/* Whether the process has ended, asked without waiting, and how where it
 * has. Under the lock. */
static void hero_proc_look(hero_proc_slot *slot) {
    if (WaitForSingleObject(slot->process, 0) == WAIT_OBJECT_0) {
        hero_proc_collect(slot);
    }
}

/* Waits for the process to end and keeps how. Outside the lock: the slot is
 * busy, so its `HANDLE` stays open under the wait. */
static void hero_proc_await(hero_proc_slot *slot) {
    for (;;) {
        hero_proc_enter();
        bool runs = slot->state == HERO_PROC_STATE_RUNNING;
        HANDLE process = slot->process;
        hero_proc_leave();

        if (!runs) {
            return;
        }
        DWORD waited = WaitForSingleObject(process, INFINITE);
        DWORD why = waited == WAIT_OBJECT_0 ? 0 : GetLastError();

        hero_proc_enter();
        if (slot->state == HERO_PROC_STATE_RUNNING) {
            if (waited == WAIT_OBJECT_0) {
                hero_proc_collect(slot);
            } else {
                slot->lost = (int64_t)why;
                slot->state = HERO_PROC_STATE_LOST;
            }
        }
        hero_proc_leave();
    }
}

/* Ends a running process at once, or refuses the request Windows has no
 * word for; a process that ended meanwhile is no failure. Under the lock. */
static void hero_proc_end(hero_proc_slot *slot, bool at_once, int64_t *kind, int64_t *os) {
    if (!at_once) {
        *kind = HERO_PROC_UNSUPPORTED;
        return;
    }
    if (!TerminateProcess(slot->process, HERO_PROC_KILLED)) {
        DWORD why = GetLastError();

        if (WaitForSingleObject(slot->process, 0) != WAIT_OBJECT_0) {
            *kind = hero_proc_kind_of((int64_t)why);
            *os = (int64_t)why;
        }
    }
}

/* Nothing on Windows: no released process is the layer's to wait for. */
static void hero_proc_sweep(void) {
}

/* A released slot's process let go: its `HANDLE` closed, and the process
 * runs on where it runs. Under the lock, no call in flight. */
static void hero_proc_part(hero_proc_slot *slot) {
    (void)CloseHandle(slot->process);
    slot->process = NULL;
    hero_proc_free(slot);
}

#else

/* A `timeval` as nanoseconds, the largest where it does not fit. */
static int64_t hero_proc_time_ns(struct timeval span) {
    int64_t seconds = 0;
    int64_t total = 0;

    if (span.tv_sec < 0 || span.tv_usec < 0 ||
        __builtin_mul_overflow((int64_t)span.tv_sec, INT64_C(1000000000), &seconds) ||
        __builtin_add_overflow(seconds, (int64_t)span.tv_usec * 1000, &total)) {
        return INT64_MAX;
    }
    return total;
}

/* Takes the child's status where it has ended, without waiting, and keeps
 * how it ended; nothing where it still runs. The one place a child's status
 * is taken while a handle names it, so from here its pid is no longer its
 * own, and the slot says so in the same step. Under the lock. */
static void hero_proc_look(hero_proc_slot *slot) {
    int status = 0;
    struct rusage used;
    pid_t got = 0;
    int why = 0;

    memset(&used, 0, sizeof used);
    do {
        got = wait4(slot->pid, &status, WNOHANG, &used);
        why = errno;
    } while (got < 0 && why == EINTR);

    if (got == 0) {
        return;
    }
    if (got < 0 || !(WIFEXITED(status) || WIFSIGNALED(status))) {
        slot->lost = got < 0 ? (int64_t)why : 0;
        slot->state = HERO_PROC_STATE_LOST;
        return;
    }
    slot->code = WIFEXITED(status) ? (int64_t)WEXITSTATUS(status) : -1;
    slot->by_signal = WIFSIGNALED(status) ? (int64_t)WTERMSIG(status) : 0;
    slot->user_ns = hero_proc_time_ns(used.ru_utime);
    slot->system_ns = hero_proc_time_ns(used.ru_stime);
    slot->state = HERO_PROC_STATE_ENDED;
}

/* Waits for the process to end and keeps how. The wait is outside the lock
 * and takes nothing (`WNOWAIT`); the status is taken under it. */
static void hero_proc_await(hero_proc_slot *slot) {
    for (;;) {
        siginfo_t told;

        hero_proc_enter();
        bool runs = slot->state == HERO_PROC_STATE_RUNNING;
        pid_t pid = slot->pid;
        hero_proc_leave();

        if (!runs) {
            return;
        }
        memset(&told, 0, sizeof told);
        int waited = waitid(P_PID, (id_t)pid, &told, WEXITED | WNOWAIT);
        int why = errno;

        if (waited != 0 && why == EINTR) {
            continue;
        }
        hero_proc_enter();
        if (slot->state == HERO_PROC_STATE_RUNNING) {
            if (waited == 0) {
                hero_proc_look(slot);
            } else {
                slot->lost = (int64_t)why;
                slot->state = HERO_PROC_STATE_LOST;
            }
        }
        runs = slot->state == HERO_PROC_STATE_RUNNING;
        hero_proc_leave();

        if (runs) {
            /* The system said it ended and would not hand its status yet:
             * asked again, after a moment and never in a tight loop. */
            struct timespec moment = {0, 1000000};

            (void)nanosleep(&moment, NULL);
        }
    }
}

/* Sends a running process the request to end, or the signal that ends it at
 * once. Under the lock, where the slot says its pid is still its own; a pid
 * the system no longer knows is a process that ended, and no failure. */
static void hero_proc_end(hero_proc_slot *slot, bool at_once, int64_t *kind, int64_t *os) {
    if (kill(slot->pid, at_once ? SIGKILL : SIGTERM) != 0 && errno != ESRCH) {
        *os = (int64_t)errno;
        *kind = hero_proc_kind_of(*os);
    }
}

/* The thread that waits for one process released while it ran, and gives
 * its slot back. No handle names the slot, so this is the one wait on it. */
static void *hero_proc_watching(void *at) {
    hero_proc_slot *slot = at;
    int status = 0;
    pid_t got = 0;

    hero_proc_enter();
    pid_t pid = slot->pid;
    hero_proc_leave();

    do {
        got = waitpid(pid, &status, 0);
    } while (got < 0 && errno == EINTR);

    hero_proc_enter();
    hero_proc_free(slot);
    hero_proc_leave();
    return NULL;
}

/* Starts that thread: detached, on a small stack, with every signal but a
 * fault's held off, so no handler of the program ever runs on it (a fault's
 * signal goes to the thread that faulted, and holding one turns a fault into
 * a hang on Darwin, as `parts/spawn.c` measured). False where the system
 * gives no thread. Under the lock, which the new thread then waits for. */
static bool hero_proc_watch(hero_proc_slot *slot) {
    pthread_attr_t how;
    pthread_t thread;
    sigset_t held;
    sigset_t before;
    bool made = false;

    if (pthread_attr_init(&how) != 0) {
        return false;
    }
    (void)pthread_attr_setdetachstate(&how, PTHREAD_CREATE_DETACHED);
    (void)pthread_attr_setstacksize(&how, HERO_PROC_WATCH_STACK);
    (void)sigfillset(&held);
    (void)sigdelset(&held, SIGSEGV);
    (void)sigdelset(&held, SIGBUS);
    (void)sigdelset(&held, SIGILL);
    (void)sigdelset(&held, SIGTRAP);
    (void)sigdelset(&held, SIGFPE);
    (void)sigdelset(&held, SIGSYS);
    if (pthread_sigmask(SIG_BLOCK, &held, &before) == 0) {
        made = pthread_create(&thread, &how, hero_proc_watching, slot) == 0;
        (void)pthread_sigmask(SIG_SETMASK, &before, NULL);
    }
    (void)pthread_attr_destroy(&how);
    return made;
}

/* Every released process no thread waits for is looked at, and its slot
 * given back where it has ended. Under the lock. */
static void hero_proc_sweep(void) {
    for (uint32_t index = 0; index < hero_proc_table.fresh && hero_proc_table.unwatched > 0; index += 1) {
        hero_proc_slot *slot = &hero_proc_table.slots[index];

        if (slot->orphan && !slot->watched) {
            hero_proc_look(slot);
            if (slot->state != HERO_PROC_STATE_RUNNING) {
                hero_proc_table.unwatched -= 1;
                hero_proc_free(slot);
            }
        }
    }
}

/* A released slot's process let go: its slot given back where it has ended,
 * else kept as the layer's until it does, a thread waiting for it. Under the
 * lock, no call in flight. */
static void hero_proc_part(hero_proc_slot *slot) {
    if (slot->state == HERO_PROC_STATE_RUNNING) {
        hero_proc_look(slot);
    }
    if (slot->state != HERO_PROC_STATE_RUNNING) {
        hero_proc_free(slot);
        return;
    }
    slot->held = false;
    slot->closing = false;
    slot->orphan = true;
    slot->watched = true;
    if (!hero_proc_watch(slot)) {
        slot->watched = false;
        hero_proc_table.unwatched += 1;
    }
}

#endif

/* The program released the slot and no call is in flight on it: the ends of
 * its pipes the program never took are handed to the caller in `left`, to
 * close outside the lock, and its process is let go. Under the lock. */
static void hero_proc_let_go(hero_proc_slot *slot, hero_stream *left[3]) {
    for (int which = 0; which < 3; which += 1) {
        left[which] = slot->pipes[which];
        slot->pipes[which] = NULL;
    }
    hero_proc_part(slot);
}

static void hero_proc_close_all(hero_stream *const left[3]) {
    for (int which = 0; which < 3; which += 1) {
        if (left[which] != NULL) {
            (void)hero_stream_close(left[which]);
        }
    }
}

/* A slot for a process about to be started, with a new life and no handle
 * yet, so nothing finds it; NULL where every slot holds one. */
static hero_proc_slot *hero_proc_reserve(void) {
    hero_proc_slot *slot = NULL;

    hero_proc_enter();
    hero_proc_sweep();
    if (hero_proc_table.given_back != 0) {
        slot = &hero_proc_table.slots[hero_proc_table.given_back - 1];
        hero_proc_table.given_back = slot->next;
    } else if (hero_proc_table.fresh < HERO_PROC_SLOTS) {
        slot = &hero_proc_table.slots[hero_proc_table.fresh];
        hero_proc_table.fresh += 1;
    }
    if (slot != NULL) {
        slot->life += 1;
        slot->id = -1;
        slot->busy = 0;
        slot->next = 0;
        slot->pipes[0] = NULL;
        slot->pipes[1] = NULL;
        slot->pipes[2] = NULL;
        slot->code = -1;
        slot->by_signal = 0;
        slot->user_ns = -1;
        slot->system_ns = -1;
        slot->lost = 0;
        slot->state = HERO_PROC_STATE_NONE;
        slot->held = false;
        slot->closing = false;
        slot->orphan = false;
        slot->watched = false;
    }
    hero_proc_leave();
    return slot;
}

/* The slot `p` names, marked busy; NULL with `HERO_PROC_NOT_OPEN` where it
 * names none. */
static hero_proc_slot *hero_proc_take(hero_proc *p) {
    hero_proc_enter();
    hero_proc_slot *slot = hero_proc_find(p);

    if (slot != NULL) {
        slot->busy += 1;
    }
    hero_proc_leave();

    if (slot == NULL) {
        hero_proc_fail(HERO_PROC_NOT_OPEN, 0);
    }
    return slot;
}

/* A call done with the slot it took, answering whether the program released
 * it meanwhile; the last call to leave a released slot gives it back. */
static bool hero_proc_done(hero_proc_slot *slot) {
    hero_stream *left[3] = {NULL, NULL, NULL};

    hero_proc_enter();
    bool closed = slot->closing;

    slot->busy -= 1;
    if (closed && slot->busy == 0) {
        hero_proc_let_go(slot, left);
    }
    hero_proc_leave();
    hero_proc_close_all(left);
    return closed;
}

/* -- what a launch is asked ----------------------------------------------- */

/* How many words `count` bytes hold, each followed by a zero byte: -1 where
 * the bytes do not end with one, -2 where they hold more than `most`. */
static int64_t hero_proc_counted(const unsigned char *bytes, uint64_t count, uint64_t most) {
    uint64_t words = 0;

    if (count == 0) {
        return 0;
    }
    if (bytes == NULL || bytes[count - 1] != 0) {
        return -1;
    }
    for (uint64_t at = 0; at < count; at += 1) {
        if (bytes[at] == 0) {
            words += 1;
            if (words > most) {
                return -2;
            }
        }
    }
    return (int64_t)words;
}

/* Whether a launch's arguments are ones the layer takes, the error set
 * where they are not; how many words and variables its lists hold. */
static bool hero_proc_asked(const char *program, const unsigned char *arguments, uint64_t arguments_count,
                            int64_t environment_kind, const unsigned char *environment, uint64_t environment_count,
                            const char *folder, const int64_t kinds[3], uint64_t *words, uint64_t *variables) {
    int64_t found = 0;

    *words = 0;
    *variables = 0;
    if (program == NULL || program[0] == '\0' || folder == NULL ||
        (environment_kind != HERO_PROC_ENV_PARENT && environment_kind != HERO_PROC_ENV_GIVEN)) {
        hero_proc_fail(HERO_PROC_INVALID, 0);
        return false;
    }
    for (int which = 0; which < 3; which += 1) {
        if (kinds[which] < HERO_PROC_STDIO_PARENT || kinds[which] > HERO_PROC_STDIO_STREAM) {
            hero_proc_fail(HERO_PROC_INVALID, 0);
            return false;
        }
    }
    found = hero_proc_counted(arguments, arguments_count, HERO_PROC_WORDS_MOST);
    if (found < 0) {
        hero_proc_fail(found == -2 ? HERO_PROC_LIMIT : HERO_PROC_INVALID, 0);
        return false;
    }
    *words = (uint64_t)found;
    if (environment_kind == HERO_PROC_ENV_PARENT) {
        return true;
    }
    found = hero_proc_counted(environment, environment_count, HERO_PROC_VARIABLES_MOST);
    if (found < 0) {
        hero_proc_fail(found == -2 ? HERO_PROC_LIMIT : HERO_PROC_INVALID, 0);
        return false;
    }
    *variables = (uint64_t)found;

    /* Each is `NAME=value`: a `=` after its first byte, which Windows' own
     * entries for a drive's folder (`=C:=C:\w`) have too. */
    const char *entry = (const char *)environment;

    for (uint64_t at = 0; at < *variables; at += 1) {
        if (entry[0] == '\0' || strchr(entry + 1, '=') == NULL) {
            hero_proc_fail(HERO_PROC_INVALID, 0);
            return false;
        }
        entry += strlen(entry) + 1;
    }
    return true;
}

/* -- the child's three streams -------------------------------------------- */

/* One of the child's three streams while the launch is made. */
typedef struct {
    /* What the child's stream is a copy of; NULL for none. */
    hero_stream *source;
    /* The program's end of a new pipe; NULL where it is no pipe. */
    hero_stream *kept;
    /* `source`'s descriptor, held open. */
    int64_t descriptor;
    /* `source` is the layer's own, closed once the launch is made: the null
     * device, or the child's end of a pipe. */
    bool owned;
    /* A hold stands on `source`. */
    bool held;
} hero_proc_stdio;

/* The launch is made, or failed: the hold ended, the layer's own source
 * closed, and the program's end of a pipe too where no child holds the
 * other. */
static void hero_proc_stdio_shut(hero_proc_stdio *io, bool started) {
    if (io->held) {
        (void)hero_stream_release(io->source);
    }
    if (io->owned && io->source != NULL) {
        (void)hero_stream_close(io->source);
    }
    if (!started && io->kept != NULL) {
        (void)hero_stream_close(io->kept);
    }
}

/* What the child's stream `which` is to be, readied and held; false with
 * the error set, nothing left open, where it cannot be. */
static bool hero_proc_stdio_open(hero_proc_stdio *io, int which, int64_t kind, hero_stream *given) {
    hero_stream *reader = NULL;
    hero_stream *writer = NULL;

    io->source = NULL;
    io->kept = NULL;
    io->descriptor = -1;
    io->owned = false;
    io->held = false;

    switch (kind) {
    case HERO_PROC_STDIO_PARENT:
        /* A process started without that stream hands on none. */
        io->source = hero_stream_std(which);
        if (io->source == NULL) {
            return true;
        }
        break;
    case HERO_PROC_STDIO_NOTHING:
        io->source = hero_stream_open(HERO_PROC_NOWHERE, HERO_STREAM_ACCESS_READ_WRITE, 0);
        if (io->source == NULL) {
            hero_proc_fail_as_stream();
            return false;
        }
        io->owned = true;
        break;
    case HERO_PROC_STDIO_PIPE:
        if (hero_stream_pipe(&reader, &writer) != 0) {
            hero_proc_fail_as_stream();
            return false;
        }
        io->source = which == HERO_PROC_INPUT ? reader : writer;
        io->kept = which == HERO_PROC_INPUT ? writer : reader;
        io->owned = true;
        break;
    default:
        io->source = given;
        break;
    }
    io->descriptor = hero_stream_hold(io->source);
    if (io->descriptor == -1) {
        hero_proc_fail_as_stream();
        hero_proc_stdio_shut(io, false);
        return false;
    }
    io->held = true;
    return true;
}

/* All three, in order; false with the error set and none left open. */
static bool hero_proc_stdio_open_all(hero_proc_stdio io[3], const int64_t kinds[3], hero_stream *const given[3]) {
    for (int which = 0; which < 3; which += 1) {
        if (!hero_proc_stdio_open(&io[which], which, kinds[which], given[which])) {
            for (int earlier = 0; earlier < which; earlier += 1) {
                hero_proc_stdio_shut(&io[earlier], false);
            }
            return false;
        }
    }
    return true;
}

static void hero_proc_stdio_shut_all(hero_proc_stdio io[3], bool started) {
    for (int which = 0; which < 3; which += 1) {
        hero_proc_stdio_shut(&io[which], started);
    }
}

#if defined(_WIN32)

/* -- the launch, Windows --------------------------------------------------- */

static bool hero_proc_put(char *out, size_t room, size_t *at, char byte) {
    if (*at + 1 >= room) {
        return false;
    }
    out[*at] = byte;
    *at += 1;
    return true;
}

static bool hero_proc_put_slashes(char *out, size_t room, size_t *at, size_t count) {
    for (size_t k = 0; k < count; k += 1) {
        if (!hero_proc_put(out, room, at, '\\')) {
            return false;
        }
    }
    return true;
}

/* One word written so that the C runtime and `CommandLineToArgvW` read it
 * back as it was: bare where it holds no blank and no quote; else inside
 * quotes, a quote written `\"`, and every backslash that stands before a
 * quote, the closing one included, written twice, since those two readers
 * take backslashes as themselves except before a quote, where each pair is
 * one. False where the line has no room. */
static bool hero_proc_quote(const char *word, char *out, size_t room, size_t *at) {
    bool bare = word[0] != '\0';
    size_t slashes = 0;

    for (const char *c = word; *c != '\0' && bare; c += 1) {
        bare = !(*c == ' ' || *c == '\t' || *c == '\n' || *c == '\v' || *c == '"');
    }
    if (bare) {
        for (const char *c = word; *c != '\0'; c += 1) {
            if (!hero_proc_put(out, room, at, *c)) {
                return false;
            }
        }
        return true;
    }
    if (!hero_proc_put(out, room, at, '"')) {
        return false;
    }
    for (const char *c = word; *c != '\0'; c += 1) {
        if (*c == '\\') {
            slashes += 1;
            continue;
        }
        if (*c == '"') {
            if (!hero_proc_put_slashes(out, room, at, slashes * 2 + 1)) {
                return false;
            }
        } else if (!hero_proc_put_slashes(out, room, at, slashes)) {
            return false;
        }
        if (!hero_proc_put(out, room, at, *c)) {
            return false;
        }
        slashes = 0;
    }
    return hero_proc_put_slashes(out, room, at, slashes * 2) && hero_proc_put(out, room, at, '"');
}

/* Whether `program` names a batch file: `.bat` or `.cmd`, in any case, past
 * the dots and blanks Windows drops from a name's end. `cmd.exe` runs one,
 * and reads its line by rules of its own, under which an argument can end
 * the line and begin another command; so the layer starts none. */
static bool hero_proc_is_batch(const char *program) {
    size_t n = strlen(program);

    while (n > 0 && (program[n - 1] == ' ' || program[n - 1] == '.')) {
        n -= 1;
    }
    if (n < 4 || program[n - 4] != '.') {
        return false;
    }
    char a = (char)(program[n - 3] | 0x20);
    char b = (char)(program[n - 2] | 0x20);
    char c = (char)(program[n - 1] | 0x20);

    return (a == 'b' && b == 'a' && c == 't') || (a == 'c' && b == 'm' && c == 'd');
}

/* Text the system would not take as UTF-16: no room for it, or not UTF-8. */
static void hero_proc_fail_text(void) {
    DWORD why = GetLastError();

    hero_proc_fail(why == ERROR_INSUFFICIENT_BUFFER ? HERO_PROC_LIMIT : HERO_PROC_INVALID, (int64_t)why);
}

/* How long a variable's name is: up to the first `=` after its first unit. */
static size_t hero_proc_name_units(const wchar_t *entry) {
    const wchar_t *equals = wcschr(entry + 1, L'=');

    return equals != NULL ? (size_t)(equals - entry) : wcslen(entry);
}

/* For `qsort`: by name as Windows compares names, ordinally and without
 * case, the order a block must be in; two of one name by their whole text. */
static int hero_proc_variable_order(const void *x, const void *y) {
    const wchar_t *a = *(const wchar_t *const *)x;
    const wchar_t *b = *(const wchar_t *const *)y;
    int by_name = CompareStringOrdinal(a, (int)hero_proc_name_units(a), b, (int)hero_proc_name_units(b), TRUE) - CSTR_EQUAL;

    return by_name != 0 ? by_name : wcscmp(a, b);
}

/* The given variables as the block `CreateProcessW` takes: UTF-16, sorted,
 * each ended by a zero and the block by one more. `scratch` and `entries`
 * are the caller's room. False with the error set. */
static bool hero_proc_block(const unsigned char *environment, uint64_t count, wchar_t *scratch, wchar_t *block,
                            const wchar_t **entries) {
    size_t held = 0;
    size_t put = 0;

    if (count == 0) {
        block[0] = 0;
        block[1] = 0;
        return true;
    }
    if (count > (uint64_t)INT_MAX) {
        hero_proc_fail(HERO_PROC_LIMIT, 0);
        return false;
    }
    /* The count is given, so every zero byte is converted too and the units
     * end with the last entry's zero. */
    int units = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (const char *)environment, (int)count, scratch,
                                    HERO_PROC_BLOCK_UNITS);

    if (units <= 0) {
        hero_proc_fail_text();
        return false;
    }
    for (size_t at = 0; at < (size_t)units && held < HERO_PROC_VARIABLES_MOST; at += wcslen(scratch + at) + 1) {
        entries[held] = scratch + at;
        held += 1;
    }
    qsort((void *)entries, held, sizeof entries[0], hero_proc_variable_order);
    for (size_t k = 0; k < held; k += 1) {
        size_t length = wcslen(entries[k]) + 1;

        memcpy(block + put, entries[k], length * sizeof block[0]);
        put += length;
    }
    block[put] = 0;
    return true;
}

/* Starts the program in `slot`'s name: true with the slot's process, id and
 * pipes set, or false with the error set and nothing started or left open. */
static bool hero_proc_launch(hero_proc_slot *slot, const char *program, const unsigned char *arguments,
                             uint64_t words, bool given_environment, const unsigned char *environment,
                             uint64_t environment_count, uint64_t variables, const char *folder,
                             const int64_t kinds[3], hero_stream *const given[3]) {
    char line_bytes[HERO_PROC_LINE_BYTES];
    wchar_t line[HERO_PROC_LINE_UNITS];
    wchar_t folder_units[HERO_PROC_FOLDER_UNITS];
    wchar_t scratch[HERO_PROC_BLOCK_UNITS];
    wchar_t block[HERO_PROC_BLOCK_UNITS + 1];
    const wchar_t *entries[HERO_PROC_VARIABLES_MOST];
    union {
        void *aligned;
        unsigned char bytes[HERO_PROC_ATTRIBUTE_BYTES];
    } room;
    hero_proc_stdio io[3];
    HANDLE give[3] = {NULL, NULL, NULL};
    HANDLE listed[3] = {NULL, NULL, NULL};
    DWORD count = 0;
    DWORD failed = 0;
    DWORD flags = CREATE_UNICODE_ENVIRONMENT;
    LPPROC_THREAD_ATTRIBUTE_LIST attributes = NULL;
    STARTUPINFOEXW startup;
    PROCESS_INFORMATION made;
    const wchar_t *where = NULL;
    const char *word = (const char *)arguments;
    size_t at = 0;

    (void)variables;
    if (hero_proc_is_batch(program)) {
        hero_proc_fail(HERO_PROC_NOT_A_PROGRAM, 0);
        return false;
    }

    /* The line: the program's name first, as every word. No application
     * name is handed beside it, so the system's own search runs and tries
     * `.exe`. */
    bool fits = hero_proc_quote(program, line_bytes, sizeof line_bytes, &at);

    for (uint64_t k = 0; k < words && fits; k += 1) {
        fits = hero_proc_put(line_bytes, sizeof line_bytes, &at, ' ') &&
               hero_proc_quote(word, line_bytes, sizeof line_bytes, &at);
        word += strlen(word) + 1;
    }
    if (!fits) {
        hero_proc_fail(HERO_PROC_LIMIT, 0);
        return false;
    }
    int units = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, line_bytes, (int)at, line, HERO_PROC_LINE_UNITS - 1);

    if (units <= 0) {
        hero_proc_fail_text();
        return false;
    }
    line[units] = 0;

    /* The folder, asked here whether it is one, so its refusal is never
     * read as a missing program's. */
    if (folder[0] != '\0') {
        if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, folder, -1, folder_units, HERO_PROC_FOLDER_UNITS) <= 0) {
            hero_proc_fail_text();
            return false;
        }
        DWORD is = GetFileAttributesW(folder_units);

        if (is == INVALID_FILE_ATTRIBUTES) {
            hero_proc_fail(HERO_PROC_NOT_A_DIRECTORY, (int64_t)GetLastError());
            return false;
        }
        if ((is & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            hero_proc_fail(HERO_PROC_NOT_A_DIRECTORY, (int64_t)ERROR_DIRECTORY);
            return false;
        }
        where = folder_units;
    }
    if (given_environment && !hero_proc_block(environment, environment_count, scratch, block, entries)) {
        return false;
    }
    if (!hero_proc_stdio_open_all(io, kinds, given)) {
        return false;
    }

    /* An inheritable copy of each stream's handle, which the child's list
     * names and this process closes after the launch. */
    for (int which = 0; which < 3 && failed == 0; which += 1) {
        if (!io[which].held) {
            continue;
        }
        if (!DuplicateHandle(GetCurrentProcess(), (HANDLE)(intptr_t)io[which].descriptor, GetCurrentProcess(),
                             &give[which], 0, TRUE, DUPLICATE_SAME_ACCESS)) {
            failed = GetLastError();
            give[which] = NULL;
        } else {
            listed[count] = give[which];
            count += 1;
        }
    }

    memset(&startup, 0, sizeof startup);
    memset(&made, 0, sizeof made);
    startup.StartupInfo.cb = sizeof startup.StartupInfo;
    if (failed == 0 && count > 0) {
        SIZE_T size = 0;

        /* Asked for its size first, which fails by design and sets it. */
        (void)InitializeProcThreadAttributeList(NULL, 1, 0, &size);
        if (size == 0 || size > sizeof room.bytes) {
            failed = ERROR_INSUFFICIENT_BUFFER;
        } else if (!InitializeProcThreadAttributeList((LPPROC_THREAD_ATTRIBUTE_LIST)room.bytes, 1, 0, &size)) {
            failed = GetLastError();
        } else {
            attributes = (LPPROC_THREAD_ATTRIBUTE_LIST)room.bytes;
            if (!UpdateProcThreadAttribute(attributes, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST, listed,
                                           count * sizeof listed[0], NULL, NULL)) {
                failed = GetLastError();
            }
        }
        startup.StartupInfo.cb = sizeof startup;
        startup.StartupInfo.dwFlags = STARTF_USESTDHANDLES;
        startup.StartupInfo.hStdInput = give[0];
        startup.StartupInfo.hStdOutput = give[1];
        startup.StartupInfo.hStdError = give[2];
        startup.lpAttributeList = attributes;
        flags |= EXTENDED_STARTUPINFO_PRESENT;
    }
    /* With no stream to hand on the child inherits no handle at all. Read
     * on the very next line: every call below may reset the last error. */
    if (failed == 0 && !CreateProcessW(NULL, line, NULL, NULL, count > 0 ? TRUE : FALSE, flags,
                                       given_environment ? block : NULL, where, &startup.StartupInfo, &made)) {
        failed = GetLastError();
    }
    if (attributes != NULL) {
        DeleteProcThreadAttributeList(attributes);
    }
    for (int which = 0; which < 3; which += 1) {
        if (give[which] != NULL) {
            (void)CloseHandle(give[which]);
        }
    }
    hero_proc_stdio_shut_all(io, failed == 0);
    if (failed != 0) {
        hero_proc_failed_with((int64_t)failed);
        return false;
    }
    (void)CloseHandle(made.hThread);
    slot->process = made.hProcess;
    slot->id = (int64_t)made.dwProcessId;
    for (int which = 0; which < 3; which += 1) {
        slot->pipes[which] = io[which].kept;
    }
    return true;
}

#else

/* -- the launch, POSIX ----------------------------------------------------- */

/* The start of each word of a list, in order, into `out`. */
static void hero_proc_point(char **out, const unsigned char *bytes, uint64_t words) {
    /* The system's calls take the words as `char *` and write none. */
    char *word = (char *)(uintptr_t)bytes;

    for (uint64_t at = 0; at < words; at += 1) {
        out[at] = word;
        word += strlen(word) + 1;
    }
}

/* Whether the folder is one the child can start in, asked before any child
 * exists, so its refusal is never read as a missing program's. */
static bool hero_proc_folder_ok(const char *folder) {
    struct stat is;

    if (stat(folder, &is) != 0) {
        hero_proc_fail(HERO_PROC_NOT_A_DIRECTORY, (int64_t)errno);
        return false;
    }
    if (!S_ISDIR(is.st_mode)) {
        hero_proc_fail(HERO_PROC_NOT_A_DIRECTORY, (int64_t)ENOTDIR);
        return false;
    }
    if (faccessat(AT_FDCWD, folder, X_OK, AT_EACCESS) != 0) {
        hero_proc_fail(HERO_PROC_NOT_A_DIRECTORY, (int64_t)errno);
        return false;
    }
    return true;
}

/* Whether this user may run the file at `path`; `errno` says why not. */
static bool hero_proc_runs(const char *path) {
    return faccessat(AT_FDCWD, path, X_OK, AT_EACCESS) == 0;
}

/* The path the child's `execve` is handed, in `out`: `program` itself where
 * it holds a `/`, else the first file of that name a folder of this
 * process's `PATH` holds that this user may run. Written whole where
 * `whole`, since the child then reads it from another folder. False with
 * the error set: `HERO_PROC_NOT_FOUND` where nothing of that name is there,
 * `HERO_PROC_DENIED` where one is and may not be run. */
static bool hero_proc_resolve(const char *program, bool whole, char *out, size_t room) {
    char found[HERO_PROC_PATH_ROOM];
    size_t name = strlen(program);

    if (strchr(program, '/') != NULL) {
        if (name >= sizeof found) {
            hero_proc_fail(HERO_PROC_LIMIT, (int64_t)ENAMETOOLONG);
            return false;
        }
        if (!hero_proc_runs(program)) {
            hero_proc_failed_with((int64_t)errno);
            return false;
        }
        memcpy(found, program, name + 1);
    } else {
        const char *list = getenv("PATH");
        bool have = false;
        bool refused = false;

        if (list == NULL) {
            list = "/usr/bin:/bin";
        }
        for (const char *entry = list; !have;) {
            const char *end = strchr(entry, ':');
            size_t length = end != NULL ? (size_t)(end - entry) : strlen(entry);
            const char *folder = length > 0 ? entry : ".";
            size_t folder_length = length > 0 ? length : 1;

            if (folder_length + 1 + name < sizeof found) {
                struct stat is;

                memcpy(found, folder, folder_length);
                found[folder_length] = '/';
                memcpy(found + folder_length + 1, program, name + 1);
                if (stat(found, &is) == 0 && S_ISREG(is.st_mode)) {
                    /* The marks too: to the superuser `X_OK` may be granted
                     * on a file nobody may run. */
                    if ((is.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0 && hero_proc_runs(found)) {
                        have = true;
                    } else {
                        refused = true;
                    }
                }
            }
            if (end == NULL) {
                break;
            }
            entry = end + 1;
        }
        if (!have) {
            hero_proc_fail(refused ? HERO_PROC_DENIED : HERO_PROC_NOT_FOUND, refused ? (int64_t)EACCES : (int64_t)ENOENT);
            return false;
        }
    }

    size_t length = strlen(found);

    if (whole && found[0] != '/') {
        if (getcwd(out, room) == NULL) {
            hero_proc_failed_with((int64_t)errno);
            return false;
        }
        size_t here = strlen(out);

        if (here + 1 + length >= room) {
            hero_proc_fail(HERO_PROC_LIMIT, (int64_t)ENAMETOOLONG);
            return false;
        }
        out[here] = '/';
        memcpy(out + here + 1, found, length + 1);
        return true;
    }
    if (length >= room) {
        hero_proc_fail(HERO_PROC_LIMIT, (int64_t)ENAMETOOLONG);
        return false;
    }
    memcpy(out, found, length + 1);
    return true;
}

/* Whether a spawn's file actions can name the child's folder here. */
static bool hero_proc_spawn_enters(void) {
#if defined(HERO_PROC_SPAWN_FOLDER_DARWIN)
    if (__builtin_available(macOS 10.15, *)) {
        return true;
    }
    return false;
#elif defined(HERO_PROC_SPAWN_FOLDER_GLIBC)
    return true;
#else
    return false;
#endif
}

/* The file action that enters `folder`; a number where there is none. */
static int hero_proc_spawn_folder(posix_spawn_file_actions_t *actions, const char *folder) {
#if defined(HERO_PROC_SPAWN_FOLDER_DARWIN)
#if defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED >= 260000
    if (__builtin_available(macOS 26.0, *)) {
        return posix_spawn_file_actions_addchdir(actions, folder);
    }
#endif
    /* The name it had from 10.15, which macOS 26 deprecates and keeps: the
     * one way a Darwin before 26 spawns in a folder, so Darwin never forks. */
    if (__builtin_available(macOS 10.15, *)) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        return posix_spawn_file_actions_addchdir_np(actions, folder);
#pragma clang diagnostic pop
    }
    return ENOTSUP;
#elif defined(HERO_PROC_SPAWN_FOLDER_GLIBC)
    return posix_spawn_file_actions_addchdir_np(actions, folder);
#else
    (void)actions;
    (void)folder;
    return ENOTSUP;
#endif
}

/* The file action for a stream the child has none of: closed, whatever
 * descriptor of that number this process holds. Darwin's spawn closes every
 * descriptor no action names (`POSIX_SPAWN_CLOEXEC_DEFAULT`), and refuses an
 * action that closes one not open. */
static int hero_proc_spawn_without(posix_spawn_file_actions_t *actions, int which) {
#if defined(__APPLE__)
    (void)actions;
    (void)which;
    return 0;
#else
    return posix_spawn_file_actions_addclose(actions, which);
#endif
}

/* The child by `posix_spawn`: 0 and its pid, or the system's number. */
static int hero_proc_spawn(pid_t *pid, const char *path, char *const words[], char *const variables[],
                           const char *folder, const int sources[3]) {
    posix_spawn_file_actions_t actions;
    posix_spawnattr_t how;
    sigset_t plain;
    short flags = POSIX_SPAWN_SETSIGDEF;
    int failed = posix_spawn_file_actions_init(&actions);

    if (failed != 0) {
        return failed;
    }
    failed = posix_spawnattr_init(&how);
    if (failed != 0) {
        (void)posix_spawn_file_actions_destroy(&actions);
        return failed;
    }
    for (int which = 0; which < 3 && failed == 0; which += 1) {
        if (sources[which] >= 0) {
            failed = posix_spawn_file_actions_adddup2(&actions, sources[which], which);
        } else {
            failed = hero_proc_spawn_without(&actions, which);
        }
    }
#if defined(HERO_PROC_SPAWN_CLOSEFROM)
    if (failed == 0) {
        failed = posix_spawn_file_actions_addclosefrom_np(&actions, 3);
    }
#endif
    if (failed == 0 && folder != NULL) {
        failed = hero_proc_spawn_folder(&actions, folder);
    }
    /* SIGPIPE at its default in the child, whatever this process does with
     * it: a layer that makes a write to a closed pipe a value may hold it
     * ignored, and a child that inherited that would write on for ever. */
    (void)sigemptyset(&plain);
    (void)sigaddset(&plain, SIGPIPE);
    if (failed == 0) {
        failed = posix_spawnattr_setsigdefault(&how, &plain);
    }
#if defined(__APPLE__)
    flags |= POSIX_SPAWN_CLOEXEC_DEFAULT;
#endif
    if (failed == 0) {
        failed = posix_spawnattr_setflags(&how, flags);
    }
    if (failed == 0) {
        failed = posix_spawn(pid, path, &actions, &how, words, variables);
    }
    (void)posix_spawnattr_destroy(&how);
    (void)posix_spawn_file_actions_destroy(&actions);
    return failed;
}

/* A descriptor at 3 or above, close-on-exec, in place of `fd`, which is
 * closed where it was below; -1 with `errno` set, `fd` closed. */
static int hero_proc_above(int fd) {
    if (fd > 2) {
        return fd;
    }
    int moved = fcntl(fd, F_DUPFD_CLOEXEC, 3);
    int why = errno;

    (void)close(fd);
    errno = why;
    return moved;
}

/* The pipe a forked child tells its parent through: both ends close-on-exec
 * and above the three the child overwrites. 0, or -1 with `errno` set. */
static int hero_proc_report_pipe(int ends[2]) {
#if defined(__linux__)
    if (pipe2(ends, O_CLOEXEC) != 0) {
        return -1;
    }
#else
    if (pipe(ends) != 0) {
        return -1;
    }
    if (fcntl(ends[0], F_SETFD, FD_CLOEXEC) != 0 || fcntl(ends[1], F_SETFD, FD_CLOEXEC) != 0) {
        int why = errno;

        (void)close(ends[0]);
        (void)close(ends[1]);
        errno = why;
        return -1;
    }
#endif
    ends[0] = hero_proc_above(ends[0]);
    if (ends[0] < 0) {
        int why = errno;

        (void)close(ends[1]);
        errno = why;
        return -1;
    }
    ends[1] = hero_proc_above(ends[1]);
    if (ends[1] < 0) {
        int why = errno;

        (void)close(ends[0]);
        errno = why;
        return -1;
    }
    return 0;
}

/* What the forked child does, and it returns only by becoming the program
 * or by ending: every call here is one POSIX promises after a fork in a
 * process with threads. Each signal this process handles goes back to its
 * default before the mask the fork held is given back, so no handler of the
 * parent's runs here; one it ignores stays ignored, but SIGPIPE. */
static void hero_proc_child(int report, const char *path, char *const words[], char *const variables[],
                            const char *folder, const int sources[3], const sigset_t *before) {
    int said[2] = {HERO_PROC_STAGE_STREAMS, 0};
    struct sigaction plain;
    bool ready = true;

    for (int which = 0; which < 3 && ready; which += 1) {
        if (sources[which] >= 0) {
            ready = dup2(sources[which], which) >= 0;
        } else {
            (void)close(which);
        }
    }
    if (ready && folder != NULL) {
        said[0] = HERO_PROC_STAGE_FOLDER;
        ready = chdir(folder) == 0;
    }
    if (ready) {
        said[0] = HERO_PROC_STAGE_PROGRAM;
        plain.sa_handler = SIG_DFL;
        plain.sa_flags = 0;
        (void)sigemptyset(&plain.sa_mask);
        for (int sig = 1; sig < NSIG; sig += 1) {
            struct sigaction now;

            if (sigaction(sig, NULL, &now) == 0 && now.sa_handler != SIG_DFL &&
                (now.sa_handler != SIG_IGN || sig == SIGPIPE)) {
                (void)sigaction(sig, &plain, NULL);
            }
        }
        (void)sigprocmask(SIG_SETMASK, before, NULL);
        (void)execve(path, words, variables);
    }
    said[1] = errno;

    const char *bytes = (const char *)said;
    size_t left = sizeof said;

    while (left > 0) {
        ssize_t wrote = write(report, bytes, left);

        if (wrote > 0) {
            bytes += wrote;
            left -= (size_t)wrote;
        } else if (wrote < 0 && errno != EINTR) {
            break;
        }
    }
    _exit(127);
}

/* The child by `fork` and `execve`, for a folder no spawn here can say: 0
 * and its pid, or the system's number with the stage it stopped at. */
static int hero_proc_fork(pid_t *pid, const char *path, char *const words[], char *const variables[],
                          const char *folder, const int sources[3], int *stage) {
    int report[2] = {-1, -1};
    int said[2] = {0, 0};
    sigset_t every;
    sigset_t before;
    size_t got = 0;

    if (hero_proc_report_pipe(report) != 0) {
        return errno;
    }
    /* Held off across the fork, so none of this process's handlers runs in
     * the child before it has put them back to their defaults. */
    (void)sigfillset(&every);
    (void)pthread_sigmask(SIG_SETMASK, &every, &before);
    pid_t child = fork();

    if (child == 0) {
        hero_proc_child(report[1], path, words, variables, folder, sources, &before);
    }
    int why = errno;

    (void)pthread_sigmask(SIG_SETMASK, &before, NULL);
    (void)close(report[1]);
    if (child < 0) {
        (void)close(report[0]);
        return why;
    }
    /* Nothing to read is a child that became the program: its `execve`
     * closed the pipe. */
    while (got < sizeof said) {
        ssize_t count = read(report[0], (char *)said + got, sizeof said - got);

        if (count > 0) {
            got += (size_t)count;
        } else if (count == 0 || errno != EINTR) {
            break;
        }
    }
    (void)close(report[0]);
    if (got == 0) {
        *pid = child;
        return 0;
    }
    /* It could not start, and ends at once: waited for here, so it leaves
     * no zombie and its pid goes back with nothing naming it. */
    pid_t ended = 0;

    do {
        ended = waitpid(child, NULL, 0);
    } while (ended < 0 && errno == EINTR);
    *stage = got == sizeof said ? said[0] : HERO_PROC_STAGE_PROGRAM;
    return got == sizeof said && said[1] != 0 ? said[1] : EIO;
}

/* Starts the program in `slot`'s name: true with the slot's pid, id and
 * pipes set, or false with the error set and nothing started or left open. */
static bool hero_proc_launch(hero_proc_slot *slot, const char *program, const unsigned char *arguments,
                             uint64_t words, bool given_environment, const unsigned char *environment,
                             uint64_t environment_count, uint64_t variables, const char *folder,
                             const int64_t kinds[3], hero_stream *const given[3]) {
    char *word_list[HERO_PROC_WORDS_MOST + 2];
    char *variable_list[HERO_PROC_VARIABLES_MOST + 1];
    char *none[1] = {NULL};
    char path[2 * HERO_PROC_PATH_ROOM];
    hero_proc_stdio io[3];
    int sources[3] = {-1, -1, -1};
    int copies[3] = {-1, -1, -1};
    const char *where = folder[0] != '\0' ? folder : NULL;
    char **child_variables = environ != NULL ? environ : none;
    int stage = HERO_PROC_STAGE_PROGRAM;
    int failed = 0;
    pid_t pid = -1;

    (void)environment_count;
    word_list[0] = (char *)(uintptr_t)program;
    hero_proc_point(word_list + 1, arguments, words);
    word_list[words + 1] = NULL;
    if (given_environment) {
        hero_proc_point(variable_list, environment, variables);
        variable_list[variables] = NULL;
        child_variables = variable_list;
    }
    if (where != NULL && !hero_proc_folder_ok(where)) {
        return false;
    }
    if (!hero_proc_resolve(program, where != NULL, path, sizeof path)) {
        return false;
    }
    if (!hero_proc_stdio_open_all(io, kinds, given)) {
        return false;
    }
#if defined(F_SETNOSIGPIPE)
    /* Darwin's mark that a write to a pipe nobody reads raises no signal is
     * the OPEN FILE's, and `hero_stream_pipe` sets it on every pipe's
     * writing end (`platform/hero_stream.c` measured both). The writing end
     * of a pipe made here for the child's output or error is the child's
     * alone, so the mark is taken off it: a child whose reader is gone ends
     * by SIGPIPE, as one a shell starts does, and never writes on against an
     * error it does not read. A stream the program itself holds keeps its
     * mark, being the program's to write to as well. */
    for (int which = HERO_PROC_OUTPUT; which <= HERO_PROC_ERROR; which += 1) {
        if (kinds[which] == HERO_PROC_STDIO_PIPE) {
            (void)fcntl((int)io[which].descriptor, F_SETNOSIGPIPE, 0);
        }
    }
#endif
    for (int which = 0; which < 3 && failed == 0; which += 1) {
        if (!io[which].held) {
            continue;
        }
        sources[which] = (int)io[which].descriptor;
        if (sources[which] <= 2) {
            copies[which] = fcntl(sources[which], F_DUPFD_CLOEXEC, 3);
            if (copies[which] < 0) {
                failed = errno;
            }
            sources[which] = copies[which];
        }
    }
    if (failed == 0) {
        failed = where == NULL || hero_proc_spawn_enters()
                     ? hero_proc_spawn(&pid, path, word_list, child_variables, where, sources)
                     : hero_proc_fork(&pid, path, word_list, child_variables, where, sources, &stage);
    }
    for (int which = 0; which < 3; which += 1) {
        if (copies[which] >= 0) {
            (void)close(copies[which]);
        }
    }
    hero_proc_stdio_shut_all(io, failed == 0);
    if (failed != 0) {
        if (stage == HERO_PROC_STAGE_FOLDER) {
            hero_proc_fail(HERO_PROC_NOT_A_DIRECTORY, (int64_t)failed);
        } else {
            hero_proc_failed_with((int64_t)failed);
        }
        return false;
    }
    slot->pid = pid;
    slot->id = (int64_t)pid;
    for (int which = 0; which < 3; which += 1) {
        slot->pipes[which] = io[which].kept;
    }
    return true;
}

#endif

/* -- the doors ------------------------------------------------------------ */

hero_proc *hero_proc_start(const char *program,
                           const void *arguments, uint64_t arguments_count,
                           int64_t environment_kind,
                           const void *environment, uint64_t environment_count,
                           const char *folder,
                           int64_t input_kind, hero_stream *input,
                           int64_t output_kind, hero_stream *output,
                           int64_t error_kind, hero_stream *error) {
    const int64_t kinds[3] = {input_kind, output_kind, error_kind};
    hero_stream *const given[3] = {input, output, error};
    uint64_t words = 0;
    uint64_t variables = 0;

    if (!hero_proc_asked(program, arguments, arguments_count, environment_kind, environment, environment_count, folder,
                         kinds, &words, &variables)) {
        return NULL;
    }
    hero_proc_slot *slot = hero_proc_reserve();

    if (slot == NULL) {
        hero_proc_fail(HERO_PROC_LIMIT, 0);
        return NULL;
    }
    bool started = hero_proc_launch(slot, program, arguments, words, environment_kind == HERO_PROC_ENV_GIVEN,
                                    environment, environment_count, variables, folder, kinds, given);

    hero_proc_enter();
    hero_proc *handle = NULL;

    if (started) {
        slot->state = HERO_PROC_STATE_RUNNING;
        slot->held = true;
        handle = hero_proc_handle((uint32_t)(slot - hero_proc_table.slots), slot->life);
    } else {
        hero_proc_free(slot);
    }
    hero_proc_leave();

    if (started) {
        hero_proc_clear();
    }
    return handle;
}

hero_stream *hero_proc_pipe(hero_proc *p, int64_t which) {
    hero_stream *end = NULL;

    hero_proc_enter();
    hero_proc_slot *slot = hero_proc_find(p);

    if (slot != NULL && which >= HERO_PROC_INPUT && which <= HERO_PROC_ERROR) {
        end = slot->pipes[which];
        slot->pipes[which] = NULL;
    }
    hero_proc_leave();

    if (slot == NULL) {
        hero_proc_fail(HERO_PROC_NOT_OPEN, 0);
        return NULL;
    }
    if (end == NULL) {
        hero_proc_fail(HERO_PROC_INVALID, 0);
        return NULL;
    }
    hero_proc_clear();
    return end;
}

int64_t hero_proc_wait(hero_proc *p, int64_t *code, int64_t *by_signal) {
    if (code != NULL) {
        *code = -1;
    }
    if (by_signal != NULL) {
        *by_signal = 0;
    }
    hero_proc_slot *slot = hero_proc_take(p);

    if (slot == NULL) {
        return -1;
    }
    hero_proc_await(slot);

    hero_proc_enter();
    uint8_t state = slot->state;
    int64_t ended_code = slot->code;
    int64_t ended_by = slot->by_signal;
    int64_t lost = slot->lost;
    hero_proc_leave();

    if (hero_proc_done(slot)) {
        hero_proc_fail(HERO_PROC_NOT_OPEN, 0);
        return -1;
    }
    if (state != HERO_PROC_STATE_ENDED) {
        hero_proc_fail(HERO_PROC_OTHER, lost);
        return -1;
    }
    if (code != NULL) {
        *code = ended_code;
    }
    if (by_signal != NULL) {
        *by_signal = ended_by;
    }
    hero_proc_clear();
    return 0;
}

int64_t hero_proc_ended(hero_proc *p) {
    hero_proc_slot *slot = hero_proc_take(p);

    if (slot == NULL) {
        return -1;
    }
    hero_proc_enter();
    if (slot->state == HERO_PROC_STATE_RUNNING) {
        hero_proc_look(slot);
    }
    uint8_t state = slot->state;
    hero_proc_leave();

    if (hero_proc_done(slot)) {
        hero_proc_fail(HERO_PROC_NOT_OPEN, 0);
        return -1;
    }
    hero_proc_clear();
    return state == HERO_PROC_STATE_RUNNING ? 0 : 1;
}

/* Both doors that stop a process: nothing is sent to one that has ended. */
static int64_t hero_proc_stop(hero_proc *p, bool at_once) {
    int64_t kind = HERO_PROC_OK;
    int64_t os = 0;
    hero_proc_slot *slot = hero_proc_take(p);

    if (slot == NULL) {
        return -1;
    }
    hero_proc_enter();
    if (slot->state == HERO_PROC_STATE_RUNNING) {
        hero_proc_look(slot);
    }
    if (slot->state == HERO_PROC_STATE_RUNNING) {
        hero_proc_end(slot, at_once, &kind, &os);
    }
    hero_proc_leave();

    if (hero_proc_done(slot)) {
        hero_proc_fail(HERO_PROC_NOT_OPEN, 0);
        return -1;
    }
    if (kind != HERO_PROC_OK) {
        hero_proc_fail(kind, os);
        return -1;
    }
    hero_proc_clear();
    return 0;
}

int64_t hero_proc_terminate(hero_proc *p) {
    return hero_proc_stop(p, false);
}

int64_t hero_proc_kill(hero_proc *p) {
    return hero_proc_stop(p, true);
}

int64_t hero_proc_id(hero_proc *p) {
    hero_proc_slot *slot = hero_proc_take(p);

    if (slot == NULL) {
        return -1;
    }
    hero_proc_enter();
    int64_t id = slot->id;
    hero_proc_leave();

    if (hero_proc_done(slot)) {
        hero_proc_fail(HERO_PROC_NOT_OPEN, 0);
        return -1;
    }
    hero_proc_clear();
    return id;
}

int64_t hero_proc_cpu(hero_proc *p, int64_t *user_ns, int64_t *system_ns) {
    if (user_ns != NULL) {
        *user_ns = -1;
    }
    if (system_ns != NULL) {
        *system_ns = -1;
    }
    hero_proc_slot *slot = hero_proc_take(p);

    if (slot == NULL) {
        return -1;
    }
    hero_proc_enter();
    if (slot->state == HERO_PROC_STATE_RUNNING) {
        hero_proc_look(slot);
    }
    uint8_t state = slot->state;
    int64_t in_user = slot->user_ns;
    int64_t in_system = slot->system_ns;
    int64_t lost = slot->lost;
    hero_proc_leave();

    if (hero_proc_done(slot)) {
        hero_proc_fail(HERO_PROC_NOT_OPEN, 0);
        return -1;
    }
    if (state == HERO_PROC_STATE_RUNNING) {
        hero_proc_fail(HERO_PROC_RUNNING, 0);
        return -1;
    }
    if (state != HERO_PROC_STATE_ENDED || in_user < 0 || in_system < 0) {
        hero_proc_fail(HERO_PROC_OTHER, lost);
        return -1;
    }
    if (user_ns != NULL) {
        *user_ns = in_user;
    }
    if (system_ns != NULL) {
        *system_ns = in_system;
    }
    hero_proc_clear();
    return 0;
}

int64_t hero_proc_release(hero_proc *p) {
    hero_stream *left[3] = {NULL, NULL, NULL};

    hero_proc_enter();
    hero_proc_sweep();
    hero_proc_slot *slot = hero_proc_find(p);

    if (slot != NULL) {
        slot->closing = true;
        if (slot->busy == 0) {
            hero_proc_let_go(slot, left);
        }
    }
    hero_proc_leave();
    hero_proc_close_all(left);

    if (slot == NULL) {
        hero_proc_fail(HERO_PROC_NOT_OPEN, 0);
        return -1;
    }
    hero_proc_clear();
    return 0;
}

int64_t hero_proc_last_error(void) {
    return hero_proc_kind;
}

int64_t hero_proc_last_os_error(void) {
    return hero_proc_os;
}
