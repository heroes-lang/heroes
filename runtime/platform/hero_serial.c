/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_serial.c — serial ports, one member of the archive the
 * compiler puts on every program's link line (M-rebel-library, 2026-11-18;
 * `selfhost/cli/layer.hero` builds, keys and links it). Its header,
 * `hero_serial.h`, says what each function answers; this file says how each
 * platform gets there. Written from the POSIX pages, Apple's and Linux's
 * manual pages for `termios` and `tty_ioctl`, and Microsoft's Win32 reference
 * for communications resources, never from another library's source.
 *
 * ITS OWN HEADER FIRST, so a definition with another signature than the
 * header's is `conflicting types` where it is compiled.
 *
 * A PORT IS A STREAM made by `hero_stream_make` over a block of this file's
 * own, `hero_layer_alloc`'s, holding the descriptor (the `HANDLE` on Windows),
 * the timeouts and a lock. The stream's table issues and checks the handle,
 * so this member keeps no table and takes no tag of `hero_handle_tag.h`: a
 * call on a port reaches its block by `hero_stream_hold_payload`, which
 * refuses a stream another member made and keeps the block alive until
 * `hero_stream_release`, and `hero_stream.c` calls the table's `close` once,
 * after the last read or write, so the block is never read after it is freed.
 *
 * NO STATE BETWEEN CALLS BUT THE LAST ERROR, which is per thread: the lock is
 * the port's own, in its block, and serialises what reads the system's
 * settings and writes them back (a `configure`, a line, the timeouts on
 * Windows); a read and a write take no lock and may run on two threads.
 *
 * EVERY WAIT IS IN SLICES OF AT MOST 50 MS, and between two it asks whether
 * the program asked to stop (`hero_net_stop_asked`) and whether the time
 * passed: `poll` on POSIX, on a descriptor opened non-blocking, and on
 * Windows an overlapped `ReadFile` or `WriteFile` waited on by
 * `WaitForSingleObject` and cancelled by `CancelIoEx` when the time is up.
 *
 * WINDOWS: `CreateFileW`, the `DCB` (`GetCommState`, `SetCommState`),
 * `COMMTIMEOUTS`, `GetCommModemStatus`, `EscapeCommFunction`,
 * `SetCommBreak`, `PurgeComm`, `ClearCommError`, and the registry's
 * `SERIALCOMM` key for the list. UNRUN on 2026-11-18, the box being off:
 * compiled on this Mac against zig's MinGW-w64 headers, `-Werror`, and owed
 * its runs on the box (`rebel-scratch/windows/serial.md`).
 */

#include "hero_serial.h"

/* The stop every wait asks about. */
#include "hero_net.h"
/* A port's block, given back by its stream's close. */
#include "hero_layer_alloc.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#if defined(__APPLE__)
/* `IOSSIOSPEED`, the speed a driver takes that `termios` names by no number:
 * a macro of a header of IOKit's, which needs no framework at the link. */
#include <IOKit/serial/ioss.h>
#endif
#endif

/* The longest a wait lasts before it asks again whether to stop. */
#define HERO_SERIAL_SLICE_NS INT64_C(50000000)
/* A deadline no clock reaches. */
#define HERO_SERIAL_NEVER INT64_MAX
/* The bytes a pseudo-terminal's path takes, its zero included: Darwin's
 * `TIOCPTYGNAME` writes at most 128. */
#define HERO_SERIAL_PSEUDO_PATH_MOST 128

/* -- the last error, per thread ------------------------------------------- */

static _Thread_local int64_t hero_serial_kind;
static _Thread_local int64_t hero_serial_os;

static void hero_serial_fail(int64_t kind, int64_t os) {
    hero_serial_kind = kind;
    hero_serial_os = os;
}

static void hero_serial_clear(void) {
    hero_serial_fail(HERO_SERIAL_OK, 0);
}

int64_t hero_serial_last_error(void) {
    return hero_serial_kind;
}

int64_t hero_serial_last_os_error(void) {
    return hero_serial_os;
}

/* -- a port's block ------------------------------------------------------- */

typedef struct hero_serial_port {
#if defined(_WIN32)
    HANDLE handle;
    SRWLOCK lock;
#else
    int fd;
    pthread_mutex_t lock;
#endif
    /* The timeouts, `HERO_SERIAL_FOREVER` or milliseconds; read by a read or
     * a write without the lock. */
    _Atomic int64_t read_ms;
    _Atomic int64_t write_ms;
    _Atomic int64_t gap_ms;
    /* Windows: DTR and RTS as this member set them last, in
     * `HERO_SERIAL_LINE_*` bits, since no call reads them back there. */
    _Atomic int64_t raised;
    /* Darwin: a speed set through `IOSSIOSPEED`, which `termios` may not
     * show; 0 where `termios` holds the speed. Under the lock. */
    int64_t custom_baud;
    /* The controlling side of a pseudo-terminal, which is no port. */
    bool controller;
    /* A controlling side's port side, its zero after it. */
    char path[HERO_SERIAL_PSEUDO_PATH_MOST];
} hero_serial_port;

static void hero_serial_lock(hero_serial_port *p) {
#if defined(_WIN32)
    AcquireSRWLockExclusive(&p->lock);
#else
    (void)pthread_mutex_lock(&p->lock);
#endif
}

static void hero_serial_unlock(hero_serial_port *p) {
#if defined(_WIN32)
    ReleaseSRWLockExclusive(&p->lock);
#else
    (void)pthread_mutex_unlock(&p->lock);
#endif
}

/* -- the clock and the bound of a wait ------------------------------------ */

/* Nanoseconds on a clock that never goes back, or -1 where it cannot be read
 * or does not fit 64 bits. */
static int64_t hero_serial_now(void) {
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

/* The deadline `ms` from now into `*until`, `HERO_SERIAL_NEVER` for
 * `HERO_SERIAL_FOREVER` or a bound past the clock's end; false where the
 * clock cannot be read. */
static bool hero_serial_deadline(int64_t ms, int64_t *until) {
    int64_t span = 0;
    int64_t now = 0;

    if (ms < 0) {
        *until = HERO_SERIAL_NEVER;
        return true;
    }
    now = hero_serial_now();
    if (now < 0) {
        return false;
    }
    if (__builtin_mul_overflow(ms, INT64_C(1000000), &span) || __builtin_add_overflow(now, span, until)) {
        *until = HERO_SERIAL_NEVER;
    }
    return true;
}

/* How long the next slice of a wait bounded by `until` may last, in
 * nanoseconds, at most `HERO_SERIAL_SLICE_NS`; 0 where the bound has passed
 * (or the clock cannot be read, which ends the wait rather than letting it
 * run without one). */
static int64_t hero_serial_slice(int64_t until) {
    int64_t now = 0;

    if (until == HERO_SERIAL_NEVER) {
        return HERO_SERIAL_SLICE_NS;
    }
    now = hero_serial_now();
    if (now < 0 || now >= until) {
        return 0;
    }
    if (until - now < HERO_SERIAL_SLICE_NS) {
        return until - now;
    }
    return HERO_SERIAL_SLICE_NS;
}

/* A slice's nanoseconds as the milliseconds `poll` and the Windows waits
 * take, rounded up so a wait never ends before its slice. */
static int hero_serial_slice_ms(int64_t ns) {
    return (int)((ns + INT64_C(999999)) / INT64_C(1000000));
}

/* Sleeps at most `ns` nanoseconds and at most a slice. */
static void hero_serial_sleep(int64_t ns) {
    if (ns <= 0) {
        return;
    }
    if (ns > HERO_SERIAL_SLICE_NS) {
        ns = HERO_SERIAL_SLICE_NS;
    }
#if defined(_WIN32)
    Sleep((DWORD)hero_serial_slice_ms(ns));
#else
    struct timespec t;

    t.tv_sec = 0;
    t.tv_nsec = (long)ns;
    (void)nanosleep(&t, NULL);
#endif
}

/* -- the stream's table, and a port's block reached through a stream ------ */

static int64_t hero_serial_read(void *payload, void *buffer, uint64_t count);
static int64_t hero_serial_write(void *payload, const void *data, uint64_t count);
static int64_t hero_serial_close(void *payload);

/* The table every port's stream is made with: it stays where it is for as
 * long as any of them is open, and nothing writes it. */
static const hero_stream_ops hero_serial_ops = { hero_serial_read, hero_serial_write, hero_serial_close };

/* The block under `s`, held until `hero_stream_release(s)`; NULL with this
 * member's failure named and nothing held where `s` is no open stream
 * (`HERO_SERIAL_NOT_OPEN`) or no stream this member made, or, where
 * `port_only`, the controlling side of a pseudo-terminal
 * (`HERO_SERIAL_NOT_A_PORT`). */
static hero_serial_port *hero_serial_held(hero_stream *s, bool port_only) {
    void *payload = NULL;
    hero_serial_port *p = NULL;

    if (s == NULL) {
        hero_serial_fail(HERO_SERIAL_NOT_OPEN, 0);
        return NULL;
    }
    if (hero_stream_hold_payload(s, &hero_serial_ops, &payload) != 0) {
        hero_serial_fail(hero_stream_last_error() == HERO_STREAM_NOT_OPEN ? HERO_SERIAL_NOT_OPEN : HERO_SERIAL_NOT_A_PORT, 0);
        return NULL;
    }
    p = (hero_serial_port *)payload;
    if (port_only && p->controller) {
        (void)hero_stream_release(s);
        hero_serial_fail(HERO_SERIAL_NOT_A_PORT, 0);
        return NULL;
    }
    return p;
}

/* What a call on a held block answered, its failure named where it failed
 * and cleared where it did not; the hold ended either way, after which the
 * block may be gone, so nothing of it is read here. */
static int64_t hero_serial_answer(hero_stream *s, int64_t answer) {
    int64_t kind = hero_serial_kind;
    int64_t os = hero_serial_os;

    (void)hero_stream_release(s);
    if (answer < 0) {
        hero_serial_fail(kind, os);
        return -1;
    }
    hero_serial_clear();
    return answer;
}

/* A new block, its timeouts `HERO_SERIAL_FOREVER` and no gap; NULL where
 * there is no memory. */
static hero_serial_port *hero_serial_port_new(bool controller) {
    hero_serial_port *p = (hero_serial_port *)hero_layer_alloc_zero(1, sizeof(hero_serial_port));

    if (p == NULL) {
        return NULL;
    }
#if defined(_WIN32)
    p->handle = INVALID_HANDLE_VALUE;
    InitializeSRWLock(&p->lock);
#else
    p->fd = -1;
    if (pthread_mutex_init(&p->lock, NULL) != 0) {
        hero_layer_free(p);
        return NULL;
    }
#endif
    atomic_init(&p->read_ms, HERO_SERIAL_FOREVER);
    atomic_init(&p->write_ms, HERO_SERIAL_FOREVER);
    atomic_init(&p->gap_ms, 0);
    atomic_init(&p->raised, HERO_SERIAL_LINE_DTR | HERO_SERIAL_LINE_RTS);
    p->custom_baud = 0;
    p->controller = controller;
    return p;
}

/* Closes what the block stands on and gives it back; 0, or -1 where the
 * system reported a failure while closing. */
static int64_t hero_serial_port_free(hero_serial_port *p) {
    int64_t answer = 0;

#if defined(_WIN32)
    if (p->handle != INVALID_HANDLE_VALUE && !CloseHandle(p->handle)) {
        answer = -1;
    }
#else
    if (p->fd >= 0 && close(p->fd) != 0 && errno != EINTR) {
        answer = -1;
    }
    (void)pthread_mutex_destroy(&p->lock);
#endif
    hero_layer_free(p);
    return answer;
}

/* A stream over the block `p`, or NULL with the failure named and the block
 * given back with what it stands on. */
static hero_stream *hero_serial_stream_of(hero_serial_port *p) {
    hero_stream *s = hero_stream_make(p, &hero_serial_ops, HERO_STREAM_KIND_OTHER);

    if (s == NULL) {
        hero_serial_fail(hero_stream_last_error() == HERO_STREAM_LIMIT ? HERO_SERIAL_LIMIT : HERO_SERIAL_OTHER, hero_stream_last_os_error());
        (void)hero_serial_port_free(p);
        return NULL;
    }
    hero_serial_clear();
    return s;
}

static int64_t hero_serial_close(void *payload) {
    if (hero_serial_port_free((hero_serial_port *)payload) != 0) {
        hero_stream_set_error(HERO_STREAM_OTHER, 0);
        return -1;
    }
    return 0;
}

/* -- arguments every arm checks alike ------------------------------------- */

static bool hero_serial_settings_valid(int64_t baud, int64_t data_bits, int64_t parity, int64_t stop_bits, int64_t flow) {
    return baud >= 1 && baud <= HERO_SERIAL_BAUD_MOST && data_bits >= 5 && data_bits <= 8 && parity >= HERO_SERIAL_PARITY_NONE &&
           parity <= HERO_SERIAL_PARITY_SPACE && stop_bits >= HERO_SERIAL_STOP_ONE && stop_bits <= HERO_SERIAL_STOP_ONE_AND_HALF && flow >= 0 &&
           flow <= (HERO_SERIAL_FLOW_RTS_CTS | HERO_SERIAL_FLOW_XON_XOFF | HERO_SERIAL_FLOW_DSR_DTR);
}

/* Why a wait must end now: `HERO_STREAM_STOPPED` where the program asked to
 * stop, `HERO_STREAM_TIMED_OUT` where `until` passed, else `HERO_STREAM_OK`. */
static int64_t hero_serial_why_end(int64_t until) {
    if (hero_net_stop_asked() != 0) {
        return HERO_STREAM_STOPPED;
    }
    if (hero_serial_slice(until) == 0) {
        return HERO_STREAM_TIMED_OUT;
    }
    return HERO_STREAM_OK;
}

/* A stream's kind of an end as this member's, for a drain and a break. */
static int64_t hero_serial_of_stream_kind(int64_t kind) {
    return kind == HERO_STREAM_STOPPED ? HERO_SERIAL_STOPPED : HERO_SERIAL_TIMED_OUT;
}

int64_t hero_serial_timeouts(hero_stream *s, int64_t *read_ms, int64_t *write_ms, int64_t *gap_ms) {
    hero_serial_port *p = hero_serial_held(s, false);

    if (p == NULL) {
        return -1;
    }
    if (read_ms == NULL || write_ms == NULL || gap_ms == NULL) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    *read_ms = atomic_load_explicit(&p->read_ms, memory_order_relaxed);
    *write_ms = atomic_load_explicit(&p->write_ms, memory_order_relaxed);
    *gap_ms = atomic_load_explicit(&p->gap_ms, memory_order_relaxed);
    return hero_serial_answer(s, 0);
}

#if defined(_WIN32)

/* ========================================================================
 * WINDOWS: a port is a `HANDLE` opened for overlapped reads and writes, its
 * settings the `DCB`, its waits `COMMTIMEOUTS` and an event per call.
 * ======================================================================== */

/* The bytes every port's two queues are asked to hold at an open, and the
 * room left when XON/XOFF stops and starts the other side. */
#define HERO_SERIAL_QUEUE_BYTES 4096
#define HERO_SERIAL_XON_LIMIT 2048
#define HERO_SERIAL_XOFF_LIMIT 512

static int64_t hero_serial_kind_of(DWORD code) {
    switch (code) {
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
    case ERROR_INVALID_NAME:
    case ERROR_BAD_NETPATH:
        return HERO_SERIAL_NOT_FOUND;
    case ERROR_ACCESS_DENIED:
        return HERO_SERIAL_DENIED;
    case ERROR_SHARING_VIOLATION:
        return HERO_SERIAL_BUSY;
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
    case ERROR_TOO_MANY_OPEN_FILES:
    case ERROR_NO_SYSTEM_RESOURCES:
        return HERO_SERIAL_LIMIT;
    case ERROR_DEVICE_NOT_CONNECTED:
    case ERROR_BAD_COMMAND:
    case ERROR_GEN_FAILURE:
        return HERO_SERIAL_GONE;
    case ERROR_INVALID_PARAMETER:
    case ERROR_NOT_SUPPORTED:
    case ERROR_INVALID_FUNCTION:
        return HERO_SERIAL_UNSUPPORTED;
    default:
        return HERO_SERIAL_OTHER;
    }
}

static void hero_serial_failed_system(void) {
    DWORD code = GetLastError();

    hero_serial_fail(hero_serial_kind_of(code), (int64_t)code);
}

/* Whether the failure `code` of a read or a write says the device is gone. */
static bool hero_serial_gone(DWORD code) {
    return code == ERROR_DEVICE_NOT_CONNECTED || code == ERROR_BAD_COMMAND || code == ERROR_GEN_FAILURE || code == ERROR_ACCESS_DENIED;
}

/* A stream's kind for the failure `code` of a read or a write. */
static int64_t hero_serial_stream_kind_of(DWORD code) {
    switch (code) {
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
    case ERROR_NO_SYSTEM_RESOURCES:
        return HERO_STREAM_LIMIT;
    case ERROR_INVALID_HANDLE:
        return HERO_STREAM_NOT_OPEN;
    default:
        return HERO_STREAM_OTHER;
    }
}

/* The timeouts the port's driver is given: with no gap, a read ends at its
 * first bytes (`ReadIntervalTimeout` and the multiplier at `MAXDWORD`, the
 * constant past any wait this member lets run); with a gap, a read waits for
 * its first byte and ends `gap_ms` after the last. A write has no timeout of
 * the driver's. Each wait is bounded by this member's slices. */
static BOOL hero_serial_apply_timeouts(HANDLE handle, int64_t gap_ms) {
    COMMTIMEOUTS t;

    memset(&t, 0, sizeof t);
    if (gap_ms > 0) {
        t.ReadIntervalTimeout = gap_ms >= (int64_t)(MAXDWORD - 1) ? MAXDWORD - 1 : (DWORD)gap_ms;
        t.ReadTotalTimeoutMultiplier = 0;
        t.ReadTotalTimeoutConstant = 0;
    } else {
        t.ReadIntervalTimeout = MAXDWORD;
        t.ReadTotalTimeoutMultiplier = MAXDWORD;
        t.ReadTotalTimeoutConstant = MAXDWORD - 1;
    }
    t.WriteTotalTimeoutMultiplier = 0;
    t.WriteTotalTimeoutConstant = 0;
    return SetCommTimeouts(handle, &t);
}

/* One overlapped read or write of the port, waited for in slices until
 * `until`: the bytes it moved into `*moved` and `HERO_STREAM_OK`, or the
 * stream's kind of why it ended with nothing moved, `*code` the system's
 * number where it failed. */
static int64_t hero_serial_overlapped(hero_serial_port *p, bool reading, void *buffer, const void *data, DWORD count, int64_t until, DWORD *moved, DWORD *code) {
    OVERLAPPED o;
    HANDLE event = CreateEventW(NULL, TRUE, FALSE, NULL);
    BOOL done = FALSE;
    int64_t why = HERO_STREAM_OK;

    *moved = 0;
    *code = 0;
    if (event == NULL) {
        *code = GetLastError();
        return HERO_STREAM_LIMIT;
    }
    memset(&o, 0, sizeof o);
    o.hEvent = event;
    done = reading ? ReadFile(p->handle, buffer, count, NULL, &o) : WriteFile(p->handle, data, count, NULL, &o);
    if (!done && GetLastError() != ERROR_IO_PENDING) {
        *code = GetLastError();
        (void)CloseHandle(event);
        return HERO_STREAM_OTHER;
    }
    for (;;) {
        int64_t slice = hero_serial_slice(until);
        DWORD waited = WaitForSingleObject(event, (DWORD)hero_serial_slice_ms(slice));

        if (waited == WAIT_OBJECT_0) {
            if (!GetOverlappedResult(p->handle, &o, moved, FALSE)) {
                *code = GetLastError();
                why = HERO_STREAM_OTHER;
            }
            break;
        }
        if (waited != WAIT_TIMEOUT) {
            *code = GetLastError();
            (void)CancelIoEx(p->handle, &o);
            (void)GetOverlappedResult(p->handle, &o, moved, TRUE);
            why = HERO_STREAM_OTHER;
            break;
        }
        why = hero_serial_why_end(until);
        if (why != HERO_STREAM_OK) {
            /* The time is up or the stop was asked: the call is cancelled and
             * waited for to its end, which may have moved bytes meanwhile;
             * those are answered and the end is not. */
            (void)CancelIoEx(p->handle, &o);
            (void)GetOverlappedResult(p->handle, &o, moved, TRUE);
            if (*moved > 0) {
                why = HERO_STREAM_OK;
            }
            break;
        }
    }
    (void)CloseHandle(event);
    return why;
}

static int64_t hero_serial_read(void *payload, void *buffer, uint64_t count) {
    hero_serial_port *p = (hero_serial_port *)payload;
    int64_t until = 0;

    if (!hero_serial_deadline(atomic_load_explicit(&p->read_ms, memory_order_relaxed), &until)) {
        hero_stream_set_error(HERO_STREAM_OTHER, 0);
        return -1;
    }
    for (;;) {
        DWORD moved = 0;
        DWORD code = 0;
        int64_t why = hero_serial_overlapped(p, true, buffer, NULL, (DWORD)count, until, &moved, &code);

        if (why == HERO_STREAM_OK && moved > 0) {
            return (int64_t)moved;
        }
        if (why == HERO_STREAM_OTHER && hero_serial_gone(code)) {
            /* The device is gone: its end. */
            return 0;
        }
        if (why != HERO_STREAM_OK) {
            hero_stream_set_error(why == HERO_STREAM_OTHER ? hero_serial_stream_kind_of(code) : why, (int64_t)code);
            return -1;
        }
        /* The driver ended the read with nothing (its own constant passed):
         * asked again, the bound still this call's. */
        why = hero_serial_why_end(until);
        if (why != HERO_STREAM_OK) {
            hero_stream_set_error(why, 0);
            return -1;
        }
    }
}

static int64_t hero_serial_write(void *payload, const void *data, uint64_t count) {
    hero_serial_port *p = (hero_serial_port *)payload;
    int64_t until = 0;

    if (!hero_serial_deadline(atomic_load_explicit(&p->write_ms, memory_order_relaxed), &until)) {
        hero_stream_set_error(HERO_STREAM_OTHER, 0);
        return -1;
    }
    if (count == 0) {
        return 0;
    }
    for (;;) {
        DWORD moved = 0;
        DWORD code = 0;
        int64_t why = hero_serial_overlapped(p, false, NULL, data, (DWORD)count, until, &moved, &code);

        if (why == HERO_STREAM_OK && moved > 0) {
            return (int64_t)moved;
        }
        if (why == HERO_STREAM_OTHER && hero_serial_gone(code)) {
            hero_stream_set_error(HERO_STREAM_CLOSED_PEER, (int64_t)code);
            return -1;
        }
        if (why != HERO_STREAM_OK) {
            hero_stream_set_error(why == HERO_STREAM_OTHER ? hero_serial_stream_kind_of(code) : why, (int64_t)code);
            return -1;
        }
        why = hero_serial_why_end(until);
        if (why != HERO_STREAM_OK) {
            hero_stream_set_error(why, 0);
            return -1;
        }
    }
}

/* `path` as the wide `\\.\` path `CreateFileW` opens a port by, in a block
 * the caller gives back; NULL with the failure named. */
static wchar_t *hero_serial_wide_path(const char *path) {
    static const wchar_t prefix[] = L"\\\\.\\";
    size_t prefix_count = (sizeof prefix / sizeof prefix[0]) - 1;
    bool whole = strncmp(path, "\\\\.\\", 4) == 0 || strncmp(path, "\\\\?\\", 4) == 0;
    int count = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path, -1, NULL, 0);
    wchar_t *wide = NULL;
    size_t at = whole ? 0 : prefix_count;

    if (count <= 0) {
        hero_serial_fail(HERO_SERIAL_INVALID, (int64_t)GetLastError());
        return NULL;
    }
    wide = (wchar_t *)hero_layer_alloc_zero((size_t)count + prefix_count, sizeof(wchar_t));
    if (wide == NULL) {
        hero_serial_fail(HERO_SERIAL_LIMIT, 0);
        return NULL;
    }
    if (!whole) {
        memcpy(wide, prefix, prefix_count * sizeof(wchar_t));
    }
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path, -1, wide + at, count) != count) {
        hero_serial_fail(HERO_SERIAL_INVALID, (int64_t)GetLastError());
        hero_layer_free(wide);
        return NULL;
    }
    return wide;
}

hero_stream *hero_serial_open(const char *path) {
    wchar_t *wide = NULL;
    HANDLE handle = INVALID_HANDLE_VALUE;
    DCB dcb;
    hero_serial_port *p = NULL;

    if (path == NULL || path[0] == '\0') {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return NULL;
    }
    wide = hero_serial_wide_path(path);
    if (wide == NULL) {
        return NULL;
    }
    handle = CreateFileW(wide, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    hero_layer_free(wide);
    if (handle == INVALID_HANDLE_VALUE) {
        hero_serial_failed_system();
        return NULL;
    }
    memset(&dcb, 0, sizeof dcb);
    dcb.DCBlength = sizeof dcb;
    if (GetFileType(handle) != FILE_TYPE_CHAR || !GetCommState(handle, &dcb)) {
        hero_serial_fail(HERO_SERIAL_NOT_A_PORT, (int64_t)GetLastError());
        (void)CloseHandle(handle);
        return NULL;
    }
    (void)SetupComm(handle, HERO_SERIAL_QUEUE_BYTES, HERO_SERIAL_QUEUE_BYTES);
    /* Bytes as they are: no character replaced, none dropped, no read ended
     * by an error; DTR and RTS raised unless the flow drives them. */
    dcb.fBinary = TRUE;
    dcb.fNull = FALSE;
    dcb.fErrorChar = FALSE;
    dcb.fAbortOnError = FALSE;
    dcb.fDsrSensitivity = FALSE;
    dcb.fTXContinueOnXoff = TRUE;
    dcb.XonChar = 17;
    dcb.XoffChar = 19;
    dcb.XonLim = HERO_SERIAL_XON_LIMIT;
    dcb.XoffLim = HERO_SERIAL_XOFF_LIMIT;
    if (dcb.fDtrControl != DTR_CONTROL_HANDSHAKE) {
        dcb.fDtrControl = DTR_CONTROL_ENABLE;
    }
    if (dcb.fRtsControl != RTS_CONTROL_HANDSHAKE) {
        dcb.fRtsControl = RTS_CONTROL_ENABLE;
    }
    if (!SetCommState(handle, &dcb) || !hero_serial_apply_timeouts(handle, 0)) {
        hero_serial_failed_system();
        (void)CloseHandle(handle);
        return NULL;
    }
    p = hero_serial_port_new(false);
    if (p == NULL) {
        hero_serial_fail(HERO_SERIAL_LIMIT, 0);
        (void)CloseHandle(handle);
        return NULL;
    }
    p->handle = handle;
    return hero_serial_stream_of(p);
}

int64_t hero_serial_configure(hero_stream *s, int64_t baud, int64_t data_bits, int64_t parity, int64_t stop_bits, int64_t flow) {
    hero_serial_port *p = hero_serial_held(s, true);
    DCB was;
    DCB dcb;
    DCB now;
    int64_t answer = 0;

    if (p == NULL) {
        return -1;
    }
    if (!hero_serial_settings_valid(baud, data_bits, parity, stop_bits, flow)) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    hero_serial_lock(p);
    memset(&was, 0, sizeof was);
    was.DCBlength = sizeof was;
    if (!GetCommState(p->handle, &was)) {
        hero_serial_failed_system();
        answer = -1;
    } else {
        dcb = was;
        dcb.BaudRate = (DWORD)baud;
        dcb.ByteSize = (BYTE)data_bits;
        dcb.fParity = parity != HERO_SERIAL_PARITY_NONE;
        dcb.Parity = parity == HERO_SERIAL_PARITY_ODD     ? ODDPARITY
                     : parity == HERO_SERIAL_PARITY_EVEN  ? EVENPARITY
                     : parity == HERO_SERIAL_PARITY_MARK  ? MARKPARITY
                     : parity == HERO_SERIAL_PARITY_SPACE ? SPACEPARITY
                                                          : NOPARITY;
        dcb.StopBits = stop_bits == HERO_SERIAL_STOP_TWO ? TWOSTOPBITS : stop_bits == HERO_SERIAL_STOP_ONE_AND_HALF ? ONE5STOPBITS : ONESTOPBIT;
        dcb.fOutxCtsFlow = (flow & HERO_SERIAL_FLOW_RTS_CTS) != 0;
        dcb.fRtsControl = (flow & HERO_SERIAL_FLOW_RTS_CTS) != 0 ? RTS_CONTROL_HANDSHAKE : RTS_CONTROL_ENABLE;
        dcb.fOutxDsrFlow = (flow & HERO_SERIAL_FLOW_DSR_DTR) != 0;
        dcb.fDtrControl = (flow & HERO_SERIAL_FLOW_DSR_DTR) != 0 ? DTR_CONTROL_HANDSHAKE : DTR_CONTROL_ENABLE;
        dcb.fOutX = (flow & HERO_SERIAL_FLOW_XON_XOFF) != 0;
        dcb.fInX = (flow & HERO_SERIAL_FLOW_XON_XOFF) != 0;
        memset(&now, 0, sizeof now);
        now.DCBlength = sizeof now;
        if (!SetCommState(p->handle, &dcb)) {
            hero_serial_failed_system();
            answer = -1;
        } else if (!GetCommState(p->handle, &now) || now.BaudRate != dcb.BaudRate || now.ByteSize != dcb.ByteSize || now.Parity != dcb.Parity ||
                   now.StopBits != dcb.StopBits) {
            /* The driver took the call and kept another value: the port is
             * put back as it was. */
            (void)SetCommState(p->handle, &was);
            hero_serial_fail(HERO_SERIAL_UNSUPPORTED, 0);
            answer = -1;
        } else {
            atomic_store_explicit(&p->raised, HERO_SERIAL_LINE_DTR | HERO_SERIAL_LINE_RTS, memory_order_relaxed);
        }
    }
    hero_serial_unlock(p);
    return hero_serial_answer(s, answer);
}

int64_t hero_serial_setting(hero_stream *s, int64_t which) {
    hero_serial_port *p = hero_serial_held(s, true);
    DCB dcb;
    int64_t answer = -1;

    if (p == NULL) {
        return -1;
    }
    memset(&dcb, 0, sizeof dcb);
    dcb.DCBlength = sizeof dcb;
    if (!GetCommState(p->handle, &dcb)) {
        hero_serial_failed_system();
        return hero_serial_answer(s, -1);
    }
    switch (which) {
    case HERO_SERIAL_SETTING_BAUD:
        answer = (int64_t)dcb.BaudRate;
        break;
    case HERO_SERIAL_SETTING_DATA_BITS:
        answer = (int64_t)dcb.ByteSize;
        break;
    case HERO_SERIAL_SETTING_PARITY:
        answer = !dcb.fParity && dcb.Parity == NOPARITY ? HERO_SERIAL_PARITY_NONE
                 : dcb.Parity == ODDPARITY              ? HERO_SERIAL_PARITY_ODD
                 : dcb.Parity == EVENPARITY             ? HERO_SERIAL_PARITY_EVEN
                 : dcb.Parity == MARKPARITY             ? HERO_SERIAL_PARITY_MARK
                 : dcb.Parity == SPACEPARITY            ? HERO_SERIAL_PARITY_SPACE
                                                        : HERO_SERIAL_PARITY_NONE;
        break;
    case HERO_SERIAL_SETTING_STOP_BITS:
        answer = dcb.StopBits == TWOSTOPBITS ? HERO_SERIAL_STOP_TWO : dcb.StopBits == ONE5STOPBITS ? HERO_SERIAL_STOP_ONE_AND_HALF : HERO_SERIAL_STOP_ONE;
        break;
    case HERO_SERIAL_SETTING_FLOW:
        answer = 0;
        if (dcb.fOutxCtsFlow && dcb.fRtsControl == RTS_CONTROL_HANDSHAKE) {
            answer |= HERO_SERIAL_FLOW_RTS_CTS;
        }
        if (dcb.fOutX || dcb.fInX) {
            answer |= HERO_SERIAL_FLOW_XON_XOFF;
        }
        if (dcb.fOutxDsrFlow && dcb.fDtrControl == DTR_CONTROL_HANDSHAKE) {
            answer |= HERO_SERIAL_FLOW_DSR_DTR;
        }
        break;
    default:
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        break;
    }
    return hero_serial_answer(s, answer);
}

int64_t hero_serial_set_timeouts(hero_stream *s, int64_t read_ms, int64_t write_ms, int64_t gap_ms) {
    hero_serial_port *p = hero_serial_held(s, false);
    int64_t answer = 0;

    if (p == NULL) {
        return -1;
    }
    if (read_ms < HERO_SERIAL_FOREVER || write_ms < HERO_SERIAL_FOREVER || gap_ms < 0) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    hero_serial_lock(p);
    if (!hero_serial_apply_timeouts(p->handle, gap_ms)) {
        hero_serial_failed_system();
        answer = -1;
    } else {
        atomic_store_explicit(&p->read_ms, read_ms, memory_order_relaxed);
        atomic_store_explicit(&p->write_ms, write_ms, memory_order_relaxed);
        atomic_store_explicit(&p->gap_ms, gap_ms, memory_order_relaxed);
    }
    hero_serial_unlock(p);
    return hero_serial_answer(s, answer);
}

int64_t hero_serial_lines(hero_stream *s) {
    hero_serial_port *p = hero_serial_held(s, true);
    DWORD status = 0;
    int64_t answer = 0;

    if (p == NULL) {
        return -1;
    }
    if (!GetCommModemStatus(p->handle, &status)) {
        hero_serial_failed_system();
        return hero_serial_answer(s, -1);
    }
    answer = atomic_load_explicit(&p->raised, memory_order_relaxed);
    if ((status & MS_CTS_ON) != 0) {
        answer |= HERO_SERIAL_LINE_CTS;
    }
    if ((status & MS_DSR_ON) != 0) {
        answer |= HERO_SERIAL_LINE_DSR;
    }
    if ((status & MS_RLSD_ON) != 0) {
        answer |= HERO_SERIAL_LINE_CD;
    }
    if ((status & MS_RING_ON) != 0) {
        answer |= HERO_SERIAL_LINE_RI;
    }
    return hero_serial_answer(s, answer);
}

int64_t hero_serial_set_line(hero_stream *s, int64_t line, int64_t on) {
    hero_serial_port *p = hero_serial_held(s, true);
    DWORD function = 0;
    int64_t answer = 0;

    if (p == NULL) {
        return -1;
    }
    if ((line != HERO_SERIAL_LINE_DTR && line != HERO_SERIAL_LINE_RTS) || (on != 0 && on != 1)) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    function = line == HERO_SERIAL_LINE_DTR ? (on != 0 ? SETDTR : CLRDTR) : (on != 0 ? SETRTS : CLRRTS);
    hero_serial_lock(p);
    if (!EscapeCommFunction(p->handle, function)) {
        hero_serial_failed_system();
        answer = -1;
    } else {
        int64_t raised = atomic_load_explicit(&p->raised, memory_order_relaxed);

        atomic_store_explicit(&p->raised, on != 0 ? (raised | line) : (raised & ~line), memory_order_relaxed);
    }
    hero_serial_unlock(p);
    return hero_serial_answer(s, answer);
}

int64_t hero_serial_set_break(hero_stream *s, int64_t on) {
    hero_serial_port *p = hero_serial_held(s, true);
    BOOL done = FALSE;

    if (p == NULL) {
        return -1;
    }
    if (on != 0 && on != 1) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    done = on != 0 ? SetCommBreak(p->handle) : ClearCommBreak(p->handle);
    if (!done) {
        hero_serial_failed_system();
        return hero_serial_answer(s, -1);
    }
    return hero_serial_answer(s, 0);
}

int64_t hero_serial_send_break(hero_stream *s, int64_t ms) {
    hero_serial_port *p = hero_serial_held(s, true);
    int64_t until = 0;
    int64_t why = HERO_STREAM_OK;

    if (p == NULL) {
        return -1;
    }
    if (ms < 0) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    if (!hero_serial_deadline(ms, &until)) {
        hero_serial_fail(HERO_SERIAL_OTHER, 0);
        return hero_serial_answer(s, -1);
    }
    if (!SetCommBreak(p->handle)) {
        hero_serial_failed_system();
        return hero_serial_answer(s, -1);
    }
    while ((why = hero_serial_why_end(until)) == HERO_STREAM_OK) {
        hero_serial_sleep(hero_serial_slice(until));
    }
    if (!ClearCommBreak(p->handle)) {
        hero_serial_failed_system();
        return hero_serial_answer(s, -1);
    }
    if (why == HERO_STREAM_STOPPED) {
        hero_serial_fail(HERO_SERIAL_STOPPED, 0);
        return hero_serial_answer(s, -1);
    }
    return hero_serial_answer(s, 0);
}

int64_t hero_serial_discard(hero_stream *s, int64_t which) {
    hero_serial_port *p = hero_serial_held(s, false);
    DWORD flags = 0;

    if (p == NULL) {
        return -1;
    }
    if (which < HERO_SERIAL_INPUT || which > (HERO_SERIAL_INPUT | HERO_SERIAL_OUTPUT)) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    if ((which & HERO_SERIAL_INPUT) != 0) {
        flags |= PURGE_RXCLEAR;
    }
    if ((which & HERO_SERIAL_OUTPUT) != 0) {
        flags |= PURGE_TXCLEAR;
    }
    if (!PurgeComm(p->handle, flags)) {
        hero_serial_failed_system();
        return hero_serial_answer(s, -1);
    }
    return hero_serial_answer(s, 0);
}

/* The bytes the port's driver holds of `which`, or -1 with the failure
 * named. */
static int64_t hero_serial_queued(hero_serial_port *p, int64_t which) {
    DWORD errors = 0;
    COMSTAT status;

    memset(&status, 0, sizeof status);
    if (!ClearCommError(p->handle, &errors, &status)) {
        hero_serial_failed_system();
        return -1;
    }
    return which == HERO_SERIAL_INPUT ? (int64_t)status.cbInQue : (int64_t)status.cbOutQue;
}

int64_t hero_serial_waiting(hero_stream *s, int64_t which) {
    hero_serial_port *p = hero_serial_held(s, false);

    if (p == NULL) {
        return -1;
    }
    if (which != HERO_SERIAL_INPUT && which != HERO_SERIAL_OUTPUT) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    return hero_serial_answer(s, hero_serial_queued(p, which));
}

int64_t hero_serial_drain(hero_stream *s) {
    hero_serial_port *p = hero_serial_held(s, false);
    int64_t until = 0;

    if (p == NULL) {
        return -1;
    }
    if (!hero_serial_deadline(atomic_load_explicit(&p->write_ms, memory_order_relaxed), &until)) {
        hero_serial_fail(HERO_SERIAL_OTHER, 0);
        return hero_serial_answer(s, -1);
    }
    for (;;) {
        int64_t left = hero_serial_queued(p, HERO_SERIAL_OUTPUT);
        int64_t why = HERO_STREAM_OK;

        if (left < 0) {
            return hero_serial_answer(s, -1);
        }
        if (left == 0) {
            /* The driver's queue is empty; what the hardware still shifts out
             * is waited for by the system's own flush. */
            if (!FlushFileBuffers(p->handle)) {
                hero_serial_failed_system();
                return hero_serial_answer(s, -1);
            }
            return hero_serial_answer(s, 0);
        }
        why = hero_serial_why_end(until);
        if (why != HERO_STREAM_OK) {
            hero_serial_fail(hero_serial_of_stream_kind(why), 0);
            return hero_serial_answer(s, -1);
        }
        hero_serial_sleep(INT64_C(2000000));
    }
}

/* Adds `name`, UTF-16, and a zero byte after it to the list in `buffer`, as
 * far as `count` reaches, and the bytes it takes to `*total`; false with the
 * failure named. */
static bool hero_serial_listed(const wchar_t *name, unsigned char *buffer, uint64_t count, int64_t *total) {
    int bytes = WideCharToMultiByte(CP_UTF8, 0, name, -1, NULL, 0, NULL, NULL);
    char *text = NULL;
    int64_t at = *total;

    if (bytes <= 0) {
        hero_serial_failed_system();
        return false;
    }
    text = (char *)hero_layer_alloc((size_t)bytes);
    if (text == NULL) {
        hero_serial_fail(HERO_SERIAL_LIMIT, 0);
        return false;
    }
    if (WideCharToMultiByte(CP_UTF8, 0, name, -1, text, bytes, NULL, NULL) != bytes) {
        hero_serial_failed_system();
        hero_layer_free(text);
        return false;
    }
    /* `bytes` counts the zero the conversion wrote, which ends the name in the
     * list too. */
    for (int i = 0; i < bytes; i++) {
        if (buffer != NULL && (uint64_t)(at + i) < count) {
            buffer[at + i] = (unsigned char)text[i];
        }
    }
    hero_layer_free(text);
    if (__builtin_add_overflow(*total, (int64_t)bytes, total)) {
        hero_serial_fail(HERO_SERIAL_LIMIT, 0);
        return false;
    }
    return true;
}

int64_t hero_serial_ports(void *buffer, uint64_t count) {
    HKEY key = NULL;
    LONG opened = 0;
    DWORD values = 0;
    DWORD name_most = 0;
    DWORD data_most = 0;
    wchar_t *name = NULL;
    BYTE *data = NULL;
    int64_t total = 0;
    bool fine = true;

    if (buffer == NULL && count > 0) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return -1;
    }
    opened = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_QUERY_VALUE, &key);
    if (opened == ERROR_FILE_NOT_FOUND) {
        /* No key is no port: Windows makes it with the first. */
        hero_serial_clear();
        return 0;
    }
    if (opened != ERROR_SUCCESS) {
        hero_serial_fail(HERO_SERIAL_OTHER, (int64_t)opened);
        return -1;
    }
    opened = RegQueryInfoKeyW(key, NULL, NULL, NULL, NULL, NULL, NULL, &values, &name_most, &data_most, NULL, NULL);
    if (opened != ERROR_SUCCESS) {
        hero_serial_fail(HERO_SERIAL_OTHER, (int64_t)opened);
        (void)RegCloseKey(key);
        return -1;
    }
    name = (wchar_t *)hero_layer_alloc_zero((size_t)name_most + 1, sizeof(wchar_t));
    data = (BYTE *)hero_layer_alloc_zero((size_t)data_most + sizeof(wchar_t), 1);
    if (name == NULL || data == NULL) {
        hero_serial_fail(HERO_SERIAL_LIMIT, 0);
        fine = false;
    }
    for (DWORD i = 0; fine && i < values; i++) {
        DWORD name_count = name_most + 1;
        DWORD data_bytes = data_most;
        DWORD type = 0;
        LONG read = RegEnumValueW(key, i, name, &name_count, NULL, &type, data, &data_bytes);

        if (read == ERROR_NO_MORE_ITEMS) {
            break;
        }
        if (read != ERROR_SUCCESS || type != REG_SZ) {
            /* A value changed under the walk, or one no port's name: passed
             * over, as the list is the system's of this moment. */
            continue;
        }
        /* The data may lack its zero: one is put after its last whole
         * character, which the block's extra two bytes hold. */
        ((wchar_t *)data)[data_bytes / sizeof(wchar_t)] = L'\0';
        fine = hero_serial_listed((const wchar_t *)data, (unsigned char *)buffer, count, &total);
    }
    hero_layer_free(name);
    hero_layer_free(data);
    (void)RegCloseKey(key);
    if (!fine) {
        return -1;
    }
    hero_serial_clear();
    return total;
}

hero_stream *hero_serial_pseudo(void) {
    hero_serial_fail(HERO_SERIAL_UNSUPPORTED, 0);
    return NULL;
}

int64_t hero_serial_pseudo_path(hero_stream *s, void *buffer, uint64_t count) {
    hero_serial_port *p = hero_serial_held(s, false);

    if (p == NULL) {
        return -1;
    }
    /* No stream of this arm is a pseudo-terminal's controlling side. */
    (void)buffer;
    (void)count;
    hero_serial_fail(HERO_SERIAL_INVALID, 0);
    return hero_serial_answer(s, -1);
}

#else

/* ========================================================================
 * POSIX: a port is a terminal device opened non-blocking, its settings
 * `termios`, its lines and its break `ioctl`s of `tty_ioctl(4)`.
 * ======================================================================== */

static int64_t hero_serial_kind_of(int code) {
    switch (code) {
    case ENOENT:
    case ENOTDIR:
    case ENXIO:
    case ENODEV:
    case ELOOP:
        return HERO_SERIAL_NOT_FOUND;
    case EACCES:
    case EPERM:
    case EROFS:
        return HERO_SERIAL_DENIED;
    case EBUSY:
        return HERO_SERIAL_BUSY;
    case EISDIR:
    case ENOTTY:
        return HERO_SERIAL_NOT_A_PORT;
    case EMFILE:
    case ENFILE:
    case ENOMEM:
    case ENOSPC:
        return HERO_SERIAL_LIMIT;
    case ENAMETOOLONG:
        return HERO_SERIAL_INVALID;
    case EIO:
        return HERO_SERIAL_GONE;
    default:
        return HERO_SERIAL_OTHER;
    }
}

static void hero_serial_failed_system(void) {
    int code = errno;

    hero_serial_fail(hero_serial_kind_of(code), (int64_t)code);
}

/* The failure of an `ioctl` a port with no such thing refuses (a
 * pseudo-terminal's lines): `HERO_SERIAL_UNSUPPORTED` for `ENOTTY` and
 * `EINVAL`, the system's kind for the rest. */
static void hero_serial_failed_ioctl(void) {
    int code = errno;

    hero_serial_fail(code == ENOTTY || code == EINVAL || code == ENOSYS ? HERO_SERIAL_UNSUPPORTED : hero_serial_kind_of(code), (int64_t)code);
}

/* Whether the failure `code` of a read or a write says the device is gone,
 * or, for a pseudo-terminal's controlling side, its port side closed. */
static bool hero_serial_gone(int code) {
    return code == EIO || code == ENXIO || code == ENODEV;
}

static int64_t hero_serial_stream_kind_of(int code) {
    switch (code) {
    case EBADF:
        return HERO_STREAM_NOT_OPEN;
    case ENOMEM:
    case ENOBUFS:
        return HERO_STREAM_LIMIT;
    case EACCES:
    case EPERM:
        return HERO_STREAM_DENIED;
    default:
        return HERO_STREAM_OTHER;
    }
}

/* Waits one slice of a wait bounded by `until` for `events` on `fd`: the
 * stream's `HERO_STREAM_OK` to try again, `*hung` set where the system said
 * the other side hung up, or why the wait ends (the stop, the time, a failure
 * of `poll` with `*code` its number). */
static int64_t hero_serial_wait(int fd, short events, int64_t until, int *code, bool *hung) {
    int64_t why = hero_serial_why_end(until);
    struct pollfd one;
    int ready = 0;

    *code = 0;
    if (why != HERO_STREAM_OK) {
        return why;
    }
    one.fd = fd;
    one.events = events;
    one.revents = 0;
    ready = poll(&one, 1, hero_serial_slice_ms(hero_serial_slice(until)));
    if (ready < 0 && errno != EINTR) {
        *code = errno;
        return HERO_STREAM_OTHER;
    }
    if (ready > 0 && (one.revents & POLLNVAL) != 0) {
        *code = EBADF;
        return HERO_STREAM_NOT_OPEN;
    }
    if (ready > 0 && (one.revents & POLLHUP) != 0) {
        *hung = true;
    }
    /* Ready, a hang-up or nothing within the slice: the caller's next read or
     * write says which, and a hang-up its read or write still cannot move is
     * the end, so no loop turns on one without moving. */
    return HERO_STREAM_OK;
}

static int64_t hero_serial_read(void *payload, void *buffer, uint64_t count) {
    hero_serial_port *p = (hero_serial_port *)payload;
    unsigned char *into = (unsigned char *)buffer;
    int64_t until = 0;
    int64_t gap = atomic_load_explicit(&p->gap_ms, memory_order_relaxed);
    uint64_t got = 0;
    bool hung = false;

    if (!hero_serial_deadline(atomic_load_explicit(&p->read_ms, memory_order_relaxed), &until)) {
        hero_stream_set_error(HERO_STREAM_OTHER, 0);
        return -1;
    }
    /* The first bytes, waited for at most the read timeout. */
    for (;;) {
        ssize_t n = read(p->fd, into, (size_t)count);
        int code = 0;
        int64_t why = HERO_STREAM_OK;

        if (n > 0) {
            got = (uint64_t)n;
            break;
        }
        if (n == 0) {
            /* A hang-up: the end. */
            return 0;
        }
        code = errno;
        if (code == EINTR) {
            continue;
        }
        if (hero_serial_gone(code) || hung) {
            return 0;
        }
        if (code != EAGAIN && code != EWOULDBLOCK) {
            hero_stream_set_error(hero_serial_stream_kind_of(code), (int64_t)code);
            return -1;
        }
        why = hero_serial_wait(p->fd, POLLIN, until, &code, &hung);
        if (why != HERO_STREAM_OK) {
            hero_stream_set_error(why, (int64_t)code);
            return -1;
        }
    }
    /* With a gap, the bytes that follow within it of each other; what fails
     * here ends the read with the bytes it has, and the next read says it. */
    if (gap > 0) {
        int64_t quiet = 0;

        if (!hero_serial_deadline(gap, &quiet)) {
            return (int64_t)got;
        }
        while (got < count) {
            ssize_t n = read(p->fd, into + got, (size_t)(count - got));
            int code = 0;

            if (n > 0) {
                got += (uint64_t)n;
                if (!hero_serial_deadline(gap, &quiet)) {
                    break;
                }
                continue;
            }
            if (n == 0) {
                break;
            }
            code = errno;
            if (code == EINTR) {
                continue;
            }
            if (code != EAGAIN && code != EWOULDBLOCK) {
                break;
            }
            if (hung || hero_serial_wait(p->fd, POLLIN, quiet, &code, &hung) != HERO_STREAM_OK) {
                break;
            }
        }
    }
    return (int64_t)got;
}

static int64_t hero_serial_write(void *payload, const void *data, uint64_t count) {
    hero_serial_port *p = (hero_serial_port *)payload;
    int64_t until = 0;
    bool hung = false;

    if (count == 0) {
        return 0;
    }
    if (!hero_serial_deadline(atomic_load_explicit(&p->write_ms, memory_order_relaxed), &until)) {
        hero_stream_set_error(HERO_STREAM_OTHER, 0);
        return -1;
    }
    for (;;) {
        ssize_t n = write(p->fd, data, (size_t)count);
        int code = 0;
        int64_t why = HERO_STREAM_OK;

        if (n > 0) {
            return (int64_t)n;
        }
        code = n == 0 ? EAGAIN : errno;
        if (code == EINTR) {
            continue;
        }
        if (hero_serial_gone(code) || hung) {
            hero_stream_set_error(HERO_STREAM_CLOSED_PEER, (int64_t)code);
            return -1;
        }
        if (code != EAGAIN && code != EWOULDBLOCK) {
            hero_stream_set_error(hero_serial_stream_kind_of(code), (int64_t)code);
            return -1;
        }
        why = hero_serial_wait(p->fd, POLLOUT, until, &code, &hung);
        if (why != HERO_STREAM_OK) {
            hero_stream_set_error(why, (int64_t)code);
            return -1;
        }
    }
}

/* The flags that make a terminal pass bytes as they are, none echoed,
 * translated or taken as a signal; the settings `hero_serial_configure`
 * owns (speed, size, parity, stops, flow, parity checking) are left. */
static void hero_serial_raw(struct termios *t) {
    t->c_iflag &= ~(tcflag_t)(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXANY | IMAXBEL);
    t->c_oflag &= ~(tcflag_t)OPOST;
    t->c_lflag &= ~(tcflag_t)(ECHO | ECHOE | ECHOK | ECHONL | ICANON | ISIG | IEXTEN);
    t->c_cflag |= (tcflag_t)(CREAD | CLOCAL);
    t->c_cc[VMIN] = 1;
    t->c_cc[VTIME] = 0;
}

/* Makes the terminal at `fd` pass bytes as they are; false with the failure
 * named. */
static bool hero_serial_make_raw(int fd) {
    struct termios t;

    if (tcgetattr(fd, &t) != 0) {
        hero_serial_failed_system();
        return false;
    }
    hero_serial_raw(&t);
    if (tcsetattr(fd, TCSANOW, &t) != 0) {
        hero_serial_failed_system();
        return false;
    }
    return true;
}

hero_stream *hero_serial_open(const char *path) {
    int fd = -1;
    int lines = TIOCM_DTR | TIOCM_RTS;
    hero_serial_port *p = NULL;

    if (path == NULL || path[0] == '\0') {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return NULL;
    }
    do {
        fd = open(path, O_RDWR | O_NOCTTY | O_NONBLOCK | O_CLOEXEC);
    } while (fd < 0 && errno == EINTR);
    if (fd < 0) {
        hero_serial_failed_system();
        return NULL;
    }
    if (!isatty(fd)) {
        hero_serial_fail(HERO_SERIAL_NOT_A_PORT, 0);
        (void)close(fd);
        return NULL;
    }
    if (ioctl(fd, TIOCEXCL) != 0) {
        hero_serial_failed_system();
        (void)close(fd);
        return NULL;
    }
    if (!hero_serial_make_raw(fd)) {
        (void)close(fd);
        return NULL;
    }
    /* DTR and RTS raised, as an open of a port does on most drivers; a port
     * with no lines (a pseudo-terminal) refuses it, which changes nothing. */
    (void)ioctl(fd, TIOCMBIS, &lines);
    p = hero_serial_port_new(false);
    if (p == NULL) {
        hero_serial_fail(HERO_SERIAL_LIMIT, 0);
        (void)close(fd);
        return NULL;
    }
    p->fd = fd;
    return hero_serial_stream_of(p);
}

#if !defined(__APPLE__)
/* The speeds `termios` names on this platform, by their numbers. */
static const struct {
    int64_t baud;
    speed_t speed;
} hero_serial_speeds[] = {
    { 50, B50 },         { 75, B75 },         { 110, B110 },       { 134, B134 },       { 150, B150 },     { 200, B200 },
    { 300, B300 },       { 600, B600 },       { 1200, B1200 },     { 1800, B1800 },     { 2400, B2400 },   { 4800, B4800 },
    { 9600, B9600 },     { 19200, B19200 },   { 38400, B38400 },
#if defined(B57600)
    { 57600, B57600 },
#endif
#if defined(B115200)
    { 115200, B115200 },
#endif
#if defined(B230400)
    { 230400, B230400 },
#endif
#if defined(B460800)
    { 460800, B460800 },
#endif
#if defined(B500000)
    { 500000, B500000 },
#endif
#if defined(B576000)
    { 576000, B576000 },
#endif
#if defined(B921600)
    { 921600, B921600 },
#endif
#if defined(B1000000)
    { 1000000, B1000000 },
#endif
#if defined(B1152000)
    { 1152000, B1152000 },
#endif
#if defined(B1500000)
    { 1500000, B1500000 },
#endif
#if defined(B2000000)
    { 2000000, B2000000 },
#endif
#if defined(B2500000)
    { 2500000, B2500000 },
#endif
#if defined(B3000000)
    { 3000000, B3000000 },
#endif
#if defined(B3500000)
    { 3500000, B3500000 },
#endif
#if defined(B4000000)
    { 4000000, B4000000 },
#endif
};

static bool hero_serial_speed_of(int64_t baud, speed_t *speed) {
    for (size_t i = 0; i < sizeof hero_serial_speeds / sizeof hero_serial_speeds[0]; i++) {
        if (hero_serial_speeds[i].baud == baud) {
            *speed = hero_serial_speeds[i].speed;
            return true;
        }
    }
    return false;
}

static int64_t hero_serial_baud_of(speed_t speed) {
    for (size_t i = 0; i < sizeof hero_serial_speeds / sizeof hero_serial_speeds[0]; i++) {
        if (hero_serial_speeds[i].speed == speed) {
            return hero_serial_speeds[i].baud;
        }
    }
    return -1;
}
#endif

/* The flags of `cflag` and `iflag` the settings own, which a configure
 * compares after it set them. */
#if defined(CMSPAR)
#define HERO_SERIAL_PARITY_FLAGS (PARENB | PARODD | CMSPAR)
#else
#define HERO_SERIAL_PARITY_FLAGS (PARENB | PARODD)
#endif
#if defined(CRTSCTS)
#define HERO_SERIAL_CFLAGS (CSIZE | HERO_SERIAL_PARITY_FLAGS | CSTOPB | CRTSCTS)
#else
#define HERO_SERIAL_CFLAGS (CSIZE | HERO_SERIAL_PARITY_FLAGS | CSTOPB)
#endif
#define HERO_SERIAL_IFLAGS (IXON | IXOFF | INPCK)

/* The settings written into `t`; false with the failure named where the
 * platform has no word for one. */
static bool hero_serial_settings_into(struct termios *t, int64_t data_bits, int64_t parity, int64_t stop_bits, int64_t flow) {
    static const tcflag_t sizes[] = { CS5, CS6, CS7, CS8 };

    if (stop_bits == HERO_SERIAL_STOP_ONE_AND_HALF || (flow & HERO_SERIAL_FLOW_DSR_DTR) != 0) {
        hero_serial_fail(HERO_SERIAL_UNSUPPORTED, 0);
        return false;
    }
#if !defined(CMSPAR)
    if (parity == HERO_SERIAL_PARITY_MARK || parity == HERO_SERIAL_PARITY_SPACE) {
        hero_serial_fail(HERO_SERIAL_UNSUPPORTED, 0);
        return false;
    }
#endif
#if !defined(CRTSCTS)
    if ((flow & HERO_SERIAL_FLOW_RTS_CTS) != 0) {
        hero_serial_fail(HERO_SERIAL_UNSUPPORTED, 0);
        return false;
    }
#endif
    t->c_cflag &= ~(tcflag_t)HERO_SERIAL_CFLAGS;
    t->c_iflag &= ~(tcflag_t)HERO_SERIAL_IFLAGS;
    t->c_cflag |= sizes[data_bits - 5];
    switch (parity) {
    case HERO_SERIAL_PARITY_ODD:
        t->c_cflag |= (tcflag_t)(PARENB | PARODD);
        break;
    case HERO_SERIAL_PARITY_EVEN:
        t->c_cflag |= (tcflag_t)PARENB;
        break;
#if defined(CMSPAR)
    case HERO_SERIAL_PARITY_MARK:
        t->c_cflag |= (tcflag_t)(PARENB | PARODD | CMSPAR);
        break;
    case HERO_SERIAL_PARITY_SPACE:
        t->c_cflag |= (tcflag_t)(PARENB | CMSPAR);
        break;
#endif
    default:
        break;
    }
    if (parity != HERO_SERIAL_PARITY_NONE) {
        t->c_iflag |= (tcflag_t)INPCK;
    }
    if (stop_bits == HERO_SERIAL_STOP_TWO) {
        t->c_cflag |= (tcflag_t)CSTOPB;
    }
#if defined(CRTSCTS)
    if ((flow & HERO_SERIAL_FLOW_RTS_CTS) != 0) {
        t->c_cflag |= (tcflag_t)CRTSCTS;
    }
#endif
    if ((flow & HERO_SERIAL_FLOW_XON_XOFF) != 0) {
        t->c_iflag |= (tcflag_t)(IXON | IXOFF);
    }
    return true;
}

/* Whether the port kept what `want` asked: the flags the settings own and,
 * where `speed_too`, the speed. */
static bool hero_serial_kept(const struct termios *want, const struct termios *now, bool speed_too) {
    if ((want->c_cflag & (tcflag_t)HERO_SERIAL_CFLAGS) != (now->c_cflag & (tcflag_t)HERO_SERIAL_CFLAGS) ||
        (want->c_iflag & (tcflag_t)HERO_SERIAL_IFLAGS) != (now->c_iflag & (tcflag_t)HERO_SERIAL_IFLAGS)) {
        return false;
    }
    return !speed_too || (cfgetospeed(want) == cfgetospeed(now) && cfgetispeed(want) == cfgetispeed(now));
}

/* Sets the port under `p`'s lock; 0, or -1 with the failure named and the
 * port put back as it was. */
static int64_t hero_serial_configure_locked(hero_serial_port *p, int64_t baud, int64_t data_bits, int64_t parity, int64_t stop_bits, int64_t flow) {
    struct termios was;
    struct termios want;
    struct termios now;
    speed_t speed = 0;
    bool through_ioctl = false;

    if (tcgetattr(p->fd, &was) != 0) {
        hero_serial_failed_system();
        return -1;
    }
    want = was;
    if (!hero_serial_settings_into(&want, data_bits, parity, stop_bits, flow)) {
        return -1;
    }
#if defined(__APPLE__)
    /* Darwin's `speed_t` is the number itself, and a driver may take any. */
    speed = (speed_t)baud;
#else
    if (!hero_serial_speed_of(baud, &speed)) {
        hero_serial_fail(HERO_SERIAL_UNSUPPORTED, 0);
        return -1;
    }
#endif
    if (cfsetispeed(&want, speed) != 0 || cfsetospeed(&want, speed) != 0) {
        hero_serial_fail(HERO_SERIAL_UNSUPPORTED, (int64_t)errno);
        return -1;
    }
    if (tcsetattr(p->fd, TCSANOW, &want) != 0) {
        int code = errno;

#if defined(__APPLE__)
        /* A speed `termios` refuses is asked of the driver by `IOSSIOSPEED`,
         * the rest set with the speed the port had. */
        if (code == EINVAL) {
            speed_t asked = (speed_t)baud;

            (void)cfsetispeed(&want, cfgetispeed(&was));
            (void)cfsetospeed(&want, cfgetospeed(&was));
            if (tcsetattr(p->fd, TCSANOW, &want) == 0 && ioctl(p->fd, IOSSIOSPEED, &asked) == 0) {
                through_ioctl = true;
            } else {
                code = errno;
                (void)tcsetattr(p->fd, TCSANOW, &was);
                hero_serial_fail(code == ENOTTY || code == EINVAL ? HERO_SERIAL_UNSUPPORTED : hero_serial_kind_of(code), (int64_t)code);
                return -1;
            }
        }
#endif
        if (!through_ioctl) {
            hero_serial_fail(code == EINVAL ? HERO_SERIAL_UNSUPPORTED : hero_serial_kind_of(code), (int64_t)code);
            return -1;
        }
    }
    /* `tcsetattr` succeeds where any of the changes was made: what the port
     * kept is read back, and a port that kept another value is put back. */
    if (tcgetattr(p->fd, &now) != 0 || !hero_serial_kept(&want, &now, !through_ioctl)) {
        (void)tcsetattr(p->fd, TCSANOW, &was);
        hero_serial_fail(HERO_SERIAL_UNSUPPORTED, 0);
        return -1;
    }
    p->custom_baud = through_ioctl ? baud : 0;
    return 0;
}

int64_t hero_serial_configure(hero_stream *s, int64_t baud, int64_t data_bits, int64_t parity, int64_t stop_bits, int64_t flow) {
    hero_serial_port *p = hero_serial_held(s, true);
    int64_t answer = 0;

    if (p == NULL) {
        return -1;
    }
    if (!hero_serial_settings_valid(baud, data_bits, parity, stop_bits, flow)) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    hero_serial_lock(p);
    answer = hero_serial_configure_locked(p, baud, data_bits, parity, stop_bits, flow);
    hero_serial_unlock(p);
    return hero_serial_answer(s, answer);
}

int64_t hero_serial_setting(hero_stream *s, int64_t which) {
    hero_serial_port *p = hero_serial_held(s, true);
    struct termios t;
    int64_t answer = -1;
    tcflag_t size = 0;

    if (p == NULL) {
        return -1;
    }
    if (tcgetattr(p->fd, &t) != 0) {
        hero_serial_failed_system();
        return hero_serial_answer(s, -1);
    }
    switch (which) {
    case HERO_SERIAL_SETTING_BAUD:
#if defined(__APPLE__)
        hero_serial_lock(p);
        answer = p->custom_baud != 0 ? p->custom_baud : (int64_t)cfgetospeed(&t);
        hero_serial_unlock(p);
#else
        answer = hero_serial_baud_of(cfgetospeed(&t));
        if (answer < 0) {
            hero_serial_fail(HERO_SERIAL_UNSUPPORTED, 0);
        }
#endif
        break;
    case HERO_SERIAL_SETTING_DATA_BITS:
        size = t.c_cflag & (tcflag_t)CSIZE;
        answer = size == (tcflag_t)CS5 ? 5 : size == (tcflag_t)CS6 ? 6 : size == (tcflag_t)CS7 ? 7 : 8;
        break;
    case HERO_SERIAL_SETTING_PARITY:
        if ((t.c_cflag & (tcflag_t)PARENB) == 0) {
            answer = HERO_SERIAL_PARITY_NONE;
        }
#if defined(CMSPAR)
        else if ((t.c_cflag & (tcflag_t)CMSPAR) != 0) {
            answer = (t.c_cflag & (tcflag_t)PARODD) != 0 ? HERO_SERIAL_PARITY_MARK : HERO_SERIAL_PARITY_SPACE;
        }
#endif
        else {
            answer = (t.c_cflag & (tcflag_t)PARODD) != 0 ? HERO_SERIAL_PARITY_ODD : HERO_SERIAL_PARITY_EVEN;
        }
        break;
    case HERO_SERIAL_SETTING_STOP_BITS:
        answer = (t.c_cflag & (tcflag_t)CSTOPB) != 0 ? HERO_SERIAL_STOP_TWO : HERO_SERIAL_STOP_ONE;
        break;
    case HERO_SERIAL_SETTING_FLOW:
        answer = 0;
#if defined(CRTSCTS)
        if ((t.c_cflag & (tcflag_t)CRTSCTS) == (tcflag_t)CRTSCTS) {
            answer |= HERO_SERIAL_FLOW_RTS_CTS;
        }
#endif
        if ((t.c_iflag & (tcflag_t)(IXON | IXOFF)) != 0) {
            answer |= HERO_SERIAL_FLOW_XON_XOFF;
        }
        break;
    default:
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        break;
    }
    return hero_serial_answer(s, answer);
}

int64_t hero_serial_set_timeouts(hero_stream *s, int64_t read_ms, int64_t write_ms, int64_t gap_ms) {
    hero_serial_port *p = hero_serial_held(s, false);

    if (p == NULL) {
        return -1;
    }
    if (read_ms < HERO_SERIAL_FOREVER || write_ms < HERO_SERIAL_FOREVER || gap_ms < 0) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    atomic_store_explicit(&p->read_ms, read_ms, memory_order_relaxed);
    atomic_store_explicit(&p->write_ms, write_ms, memory_order_relaxed);
    atomic_store_explicit(&p->gap_ms, gap_ms, memory_order_relaxed);
    return hero_serial_answer(s, 0);
}

int64_t hero_serial_lines(hero_stream *s) {
    hero_serial_port *p = hero_serial_held(s, true);
    int bits = 0;
    int64_t answer = 0;

    if (p == NULL) {
        return -1;
    }
    if (ioctl(p->fd, TIOCMGET, &bits) != 0) {
        hero_serial_failed_ioctl();
        return hero_serial_answer(s, -1);
    }
    if ((bits & TIOCM_DTR) != 0) {
        answer |= HERO_SERIAL_LINE_DTR;
    }
    if ((bits & TIOCM_RTS) != 0) {
        answer |= HERO_SERIAL_LINE_RTS;
    }
    if ((bits & TIOCM_CTS) != 0) {
        answer |= HERO_SERIAL_LINE_CTS;
    }
    if ((bits & TIOCM_DSR) != 0) {
        answer |= HERO_SERIAL_LINE_DSR;
    }
    if ((bits & TIOCM_CAR) != 0) {
        answer |= HERO_SERIAL_LINE_CD;
    }
    if ((bits & TIOCM_RNG) != 0) {
        answer |= HERO_SERIAL_LINE_RI;
    }
    return hero_serial_answer(s, answer);
}

int64_t hero_serial_set_line(hero_stream *s, int64_t line, int64_t on) {
    hero_serial_port *p = hero_serial_held(s, true);
    int bits = 0;
    int64_t answer = 0;

    if (p == NULL) {
        return -1;
    }
    if ((line != HERO_SERIAL_LINE_DTR && line != HERO_SERIAL_LINE_RTS) || (on != 0 && on != 1)) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    bits = line == HERO_SERIAL_LINE_DTR ? TIOCM_DTR : TIOCM_RTS;
    hero_serial_lock(p);
    if (ioctl(p->fd, on != 0 ? TIOCMBIS : TIOCMBIC, &bits) != 0) {
        hero_serial_failed_ioctl();
        answer = -1;
    }
    hero_serial_unlock(p);
    return hero_serial_answer(s, answer);
}

int64_t hero_serial_set_break(hero_stream *s, int64_t on) {
    hero_serial_port *p = hero_serial_held(s, true);

    if (p == NULL) {
        return -1;
    }
    if (on != 0 && on != 1) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    if (ioctl(p->fd, on != 0 ? TIOCSBRK : TIOCCBRK) != 0) {
        hero_serial_failed_ioctl();
        return hero_serial_answer(s, -1);
    }
    return hero_serial_answer(s, 0);
}

int64_t hero_serial_send_break(hero_stream *s, int64_t ms) {
    hero_serial_port *p = hero_serial_held(s, true);
    int64_t until = 0;
    int64_t why = HERO_STREAM_OK;

    if (p == NULL) {
        return -1;
    }
    if (ms < 0) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    if (!hero_serial_deadline(ms, &until)) {
        hero_serial_fail(HERO_SERIAL_OTHER, 0);
        return hero_serial_answer(s, -1);
    }
    if (ioctl(p->fd, TIOCSBRK) != 0) {
        hero_serial_failed_ioctl();
        return hero_serial_answer(s, -1);
    }
    while ((why = hero_serial_why_end(until)) == HERO_STREAM_OK) {
        hero_serial_sleep(hero_serial_slice(until));
    }
    if (ioctl(p->fd, TIOCCBRK) != 0) {
        hero_serial_failed_ioctl();
        return hero_serial_answer(s, -1);
    }
    if (why == HERO_STREAM_STOPPED) {
        hero_serial_fail(HERO_SERIAL_STOPPED, 0);
        return hero_serial_answer(s, -1);
    }
    return hero_serial_answer(s, 0);
}

int64_t hero_serial_discard(hero_stream *s, int64_t which) {
    hero_serial_port *p = hero_serial_held(s, false);
    int queue = 0;

    if (p == NULL) {
        return -1;
    }
    if (which < HERO_SERIAL_INPUT || which > (HERO_SERIAL_INPUT | HERO_SERIAL_OUTPUT)) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    queue = which == HERO_SERIAL_INPUT ? TCIFLUSH : which == HERO_SERIAL_OUTPUT ? TCOFLUSH : TCIOFLUSH;
    if (tcflush(p->fd, queue) != 0) {
        hero_serial_failed_system();
        return hero_serial_answer(s, -1);
    }
    return hero_serial_answer(s, 0);
}

/* The bytes the port holds of `which`, or -1 with the failure named. */
static int64_t hero_serial_queued(hero_serial_port *p, int64_t which) {
    int bytes = 0;

    if (ioctl(p->fd, which == HERO_SERIAL_INPUT ? FIONREAD : TIOCOUTQ, &bytes) != 0) {
        hero_serial_failed_ioctl();
        return -1;
    }
    return bytes < 0 ? 0 : (int64_t)bytes;
}

int64_t hero_serial_waiting(hero_stream *s, int64_t which) {
    hero_serial_port *p = hero_serial_held(s, false);

    if (p == NULL) {
        return -1;
    }
    if (which != HERO_SERIAL_INPUT && which != HERO_SERIAL_OUTPUT) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    return hero_serial_answer(s, hero_serial_queued(p, which));
}

int64_t hero_serial_drain(hero_stream *s) {
    hero_serial_port *p = hero_serial_held(s, false);
    int64_t until = 0;
    int64_t step = INT64_C(100000);

    if (p == NULL) {
        return -1;
    }
    if (!hero_serial_deadline(atomic_load_explicit(&p->write_ms, memory_order_relaxed), &until)) {
        hero_serial_fail(HERO_SERIAL_OTHER, 0);
        return hero_serial_answer(s, -1);
    }
    /* The queue is asked in steps that grow from 0.1 ms to 10 ms, so the
     * wait has the write timeout and the stop; `tcdrain`, which has neither,
     * runs only once the queue is empty, for what the hardware still shifts
     * out. */
    for (;;) {
        int64_t left = hero_serial_queued(p, HERO_SERIAL_OUTPUT);
        int64_t why = HERO_STREAM_OK;

        if (left < 0) {
            return hero_serial_answer(s, -1);
        }
        if (left == 0) {
            if (tcdrain(p->fd) != 0 && errno != ENOTTY && errno != EINVAL) {
                hero_serial_failed_system();
                return hero_serial_answer(s, -1);
            }
            return hero_serial_answer(s, 0);
        }
        why = hero_serial_why_end(until);
        if (why != HERO_STREAM_OK) {
            hero_serial_fail(hero_serial_of_stream_kind(why), 0);
            return hero_serial_answer(s, -1);
        }
        hero_serial_sleep(step < hero_serial_slice(until) ? step : hero_serial_slice(until));
        if (step < INT64_C(10000000)) {
            step *= 2;
        }
    }
}

/* Whether `name`, an entry of `/dev`, is a port's by this platform's rule. */
static bool hero_serial_port_name(const char *name) {
#if defined(__APPLE__)
    return strncmp(name, "tty.", 4) == 0 || strncmp(name, "cu.", 3) == 0;
#elif defined(__linux__)
    static const char prefixes[][8] = { "ttyS", "ttyUSB", "ttyACM" };

    for (size_t i = 0; i < sizeof prefixes / sizeof prefixes[0]; i++) {
        size_t length = strlen(prefixes[i]);
        size_t at = length;

        if (strncmp(name, prefixes[i], length) != 0 || name[length] == '\0') {
            continue;
        }
        while (name[at] >= '0' && name[at] <= '9') {
            at++;
        }
        if (name[at] != '\0') {
            continue;
        }
        if (i == 0) {
            /* The kernel makes `ttyS0` to `ttyS31` whatever the machine has;
             * a port stands where the device behind it does. */
            char device[300];
            static const char head[] = "/sys/class/tty/";
            static const char tail[] = "/device";
            size_t name_length = strlen(name);

            if (sizeof head - 1 + name_length + sizeof tail > sizeof device) {
                return false;
            }
            memcpy(device, head, sizeof head - 1);
            memcpy(device + sizeof head - 1, name, name_length);
            memcpy(device + sizeof head - 1 + name_length, tail, sizeof tail);
            return access(device, F_OK) == 0;
        }
        return true;
    }
    return false;
#else
    (void)name;
    return false;
#endif
}

int64_t hero_serial_ports(void *buffer, uint64_t count) {
    static const char folder[] = "/dev/";
    unsigned char *into = (unsigned char *)buffer;
    DIR *dir = NULL;
    int64_t total = 0;

    if (buffer == NULL && count > 0) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return -1;
    }
    dir = opendir("/dev");
    if (dir == NULL) {
        hero_serial_fail(HERO_SERIAL_OTHER, (int64_t)errno);
        return -1;
    }
    for (;;) {
        struct dirent *entry = NULL;
        size_t length = 0;
        int64_t at = total;

        errno = 0;
        entry = readdir(dir);
        if (entry == NULL) {
            break;
        }
        if (!hero_serial_port_name(entry->d_name)) {
            continue;
        }
        length = strlen(entry->d_name);
        /* `/dev/`, the name, and the zero that ends it in the list. */
        if (__builtin_add_overflow(total, (int64_t)(sizeof folder - 1 + length + 1), &total)) {
            (void)closedir(dir);
            hero_serial_fail(HERO_SERIAL_LIMIT, 0);
            return -1;
        }
        for (size_t i = 0; i < sizeof folder - 1 + length + 1; i++) {
            if ((uint64_t)at + i < count) {
                into[(uint64_t)at + i] = i < sizeof folder - 1 ? (unsigned char)folder[i]
                                         : i < sizeof folder - 1 + length ? (unsigned char)entry->d_name[i - (sizeof folder - 1)]
                                                                         : 0;
            }
        }
    }
    if (errno != 0) {
        int code = errno;

        (void)closedir(dir);
        hero_serial_fail(HERO_SERIAL_OTHER, (int64_t)code);
        return -1;
    }
    (void)closedir(dir);
    hero_serial_clear();
    return total;
}

#if defined(__linux__)
/* glibc declares these under `_XOPEN_SOURCE` alone, which a member that
 * includes its own header first cannot define (as `hero_ipc.c` declares
 * `accept4`). */
int posix_openpt(int flags);
int grantpt(int fd);
int unlockpt(int fd);
#endif

hero_stream *hero_serial_pseudo(void) {
#if defined(__APPLE__) || defined(__linux__)
    int controller = -1;
    int port = -1;
    char path[HERO_SERIAL_PSEUDO_PATH_MOST];
    hero_serial_port *p = NULL;

    memset(path, 0, sizeof path);
    controller = posix_openpt(O_RDWR | O_NOCTTY);
    if (controller < 0) {
        hero_serial_failed_system();
        if (hero_serial_kind == HERO_SERIAL_NOT_FOUND) {
            hero_serial_kind = HERO_SERIAL_LIMIT;
        }
        return NULL;
    }
    if (fcntl(controller, F_SETFD, FD_CLOEXEC) != 0 || grantpt(controller) != 0 || unlockpt(controller) != 0) {
        hero_serial_failed_system();
        (void)close(controller);
        return NULL;
    }
#if defined(__APPLE__)
    /* The port side's name, written by the system into 128 bytes. */
    if (ioctl(controller, TIOCPTYGNAME, path) != 0) {
        hero_serial_failed_system();
        (void)close(controller);
        return NULL;
    }
    path[sizeof path - 1] = '\0';
#else
    {
        /* The port side's number, `/dev/pts/<n>`, as `pts(4)` names it. */
        unsigned int number = 0;
        char digits[12];
        size_t count = 0;
        static const char head[] = "/dev/pts/";

        if (ioctl(controller, TIOCGPTN, &number) != 0) {
            hero_serial_failed_system();
            (void)close(controller);
            return NULL;
        }
        do {
            digits[count++] = (char)('0' + number % 10);
            number /= 10;
        } while (number != 0 && count < sizeof digits);
        memcpy(path, head, sizeof head - 1);
        for (size_t i = 0; i < count; i++) {
            path[sizeof head - 1 + i] = digits[count - 1 - i];
        }
    }
#endif
    /* The pair passes bytes as they are from the start: the port side is
     * opened once to make it so, so a byte the controlling side writes
     * before a program opens the port is not echoed back. */
    port = open(path, O_RDWR | O_NOCTTY | O_NONBLOCK | O_CLOEXEC);
    if (port < 0 || !hero_serial_make_raw(port)) {
        if (port < 0) {
            hero_serial_failed_system();
        } else {
            (void)close(port);
        }
        (void)close(controller);
        return NULL;
    }
    (void)close(port);
    if (fcntl(controller, F_SETFL, fcntl(controller, F_GETFL) | O_NONBLOCK) != 0) {
        hero_serial_failed_system();
        (void)close(controller);
        return NULL;
    }
    p = hero_serial_port_new(true);
    if (p == NULL) {
        hero_serial_fail(HERO_SERIAL_LIMIT, 0);
        (void)close(controller);
        return NULL;
    }
    p->fd = controller;
    memcpy(p->path, path, sizeof path);
    return hero_serial_stream_of(p);
#else
    hero_serial_fail(HERO_SERIAL_UNSUPPORTED, 0);
    return NULL;
#endif
}

int64_t hero_serial_pseudo_path(hero_stream *s, void *buffer, uint64_t count) {
    hero_serial_port *p = hero_serial_held(s, false);
    size_t length = 0;

    if (p == NULL) {
        return -1;
    }
    if (!p->controller || (buffer == NULL && count > 0)) {
        hero_serial_fail(HERO_SERIAL_INVALID, 0);
        return hero_serial_answer(s, -1);
    }
    length = strlen(p->path);
    if (buffer != NULL) {
        memcpy(buffer, p->path, length < count ? length : (size_t)count);
    }
    return hero_serial_answer(s, (int64_t)length);
}

#endif
