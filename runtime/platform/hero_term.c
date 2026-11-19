/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_term.c — the platform layer's terminal, one member of the
 * archive the compiler puts on every program's link line
 * (`selfhost/cli/layer.hero` builds, keys and links it). Its header,
 * `hero_term.h`, says what each function answers; this file says how each
 * platform gets there. Written from the systems' own pages (`termios(4)`,
 * `tcsetattr(3)`, `poll(2)`, `select(2)`, `sigaction(2)`, Microsoft's
 * console reference: `SetConsoleMode`, `ReadConsoleInputW`,
 * `WriteConsoleW`), never from another library's source.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite).
 *
 * ONE TERMINAL, THE PROCESS'S, AND NO HANDLE: the state below is the
 * process's, found at the first call and kept, so this member issues no
 * handle and takes no tag of `hero_handle_tag.h`. Two locks: `hero_term_lock`
 * over the mode and the descriptors, never held while a call waits; and
 * `hero_term_reading` over the keys read ahead, held by the one call that
 * reads, which may wait, so two threads reading the one terminal take its
 * keys in turn and never share a byte.
 *
 * WHAT A SIGNAL RUNS (POSIX): `hero_term_on_signal`, this file's own C,
 * which calls `tcsetattr` and `raise`, both on POSIX's list of functions safe
 * in a signal, and reads the saved state, written before the handler was
 * taken and never written while it stands. No Heroes function and no lock
 * there.
 *
 * KEYS ON WINDOWS: the console hands input records, not bytes. Each record of
 * a key going down that carries a character is that character in UTF-8, its
 * repeat count times, a surrogate pair joined across two records; a key with
 * no character is the escape sequence a POSIX terminal sends for it (an arrow
 * `ESC [ A`, Home `ESC [ H`, Delete `ESC [ 3 ~`, F1 `ESC O P`), so a program
 * reads the same bytes on the three systems. Where the console can hand the
 * sequences itself (`ENABLE_VIRTUAL_TERMINAL_INPUT`, Windows 10 and later),
 * the keys mode asks it to.
 */

#include "hero_term.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <sys/select.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#endif

/* How long the bytes after an Escape may take to come, in milliseconds,
 * before the Escape is a key of its own: a terminal sends a sequence in one
 * write, and a person cannot press a second key so soon. */
#define HERO_TERM_ESCAPE_MS 50
/* The keys read ahead and not yet handed out: room for many keys. */
#define HERO_TERM_AHEAD 256

static _Thread_local int64_t hero_term_kind;
static _Thread_local int64_t hero_term_os;

/* The bytes read from the terminal and not yet handed out, under
 * `hero_term_reading`. */
static unsigned char hero_term_ahead[HERO_TERM_AHEAD];
static size_t hero_term_ahead_count;

static void hero_term_fail(int64_t kind, int64_t os) {
    hero_term_kind = kind;
    hero_term_os = os;
}

static void hero_term_clear(void) {
    hero_term_fail(HERO_TERM_OK, 0);
}

/* -- the state, per platform -------------------------------------------- */

#if defined(_WIN32)
static SRWLOCK hero_term_lock = SRWLOCK_INIT;
static SRWLOCK hero_term_reading = SRWLOCK_INIT;
/* The terminal was looked for (under the lock), and what was found: the
 * console's input and output, `INVALID_HANDLE_VALUE` for none. */
static bool hero_term_looked;
static HANDLE hero_term_in = INVALID_HANDLE_VALUE;
static HANDLE hero_term_out = INVALID_HANDLE_VALUE;
/* The input's mode before the first enter, and whether the keys mode stands. */
static DWORD hero_term_saved;
static bool hero_term_keys;
/* A high surrogate whose low half is the next record's, 0 for none; under
 * `hero_term_reading`. */
static WCHAR hero_term_high;
/* The output consoles whose processing of sequences this member turned on,
 * with their mode before, put back when the program ends. */
#define HERO_TERM_TURNED 8
static HANDLE hero_term_turned[HERO_TERM_TURNED];
static DWORD hero_term_turned_mode[HERO_TERM_TURNED];
static int hero_term_turned_count;

static void hero_term_enter_lock(void) {
    AcquireSRWLockExclusive(&hero_term_lock);
}

static void hero_term_leave_lock(void) {
    ReleaseSRWLockExclusive(&hero_term_lock);
}

static void hero_term_enter_reading(void) {
    AcquireSRWLockExclusive(&hero_term_reading);
}

static void hero_term_leave_reading(void) {
    ReleaseSRWLockExclusive(&hero_term_reading);
}
#else
static pthread_mutex_t hero_term_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t hero_term_reading = PTHREAD_MUTEX_INITIALIZER;
static bool hero_term_looked;
/* The terminal's descriptors: one `/dev/tty` for both, or the process's
 * input and an output that is a terminal; -1 for none. */
static int hero_term_in = -1;
static int hero_term_out = -1;
/* The state before the first enter, written before `hero_term_saved_fd` is
 * published and never while it stands; and the descriptor it is put back on,
 * -1 while no mode stands, which the signal handler reads. */
static struct termios hero_term_saved;
static _Atomic int hero_term_saved_fd = -1;
/* The signals the keys mode takes where the program left them to their
 * default, and which of them this member took. */
static const int hero_term_signals[4] = { SIGINT, SIGTERM, SIGHUP, SIGQUIT };
static bool hero_term_took[4];

static void hero_term_enter_lock(void) {
    (void)pthread_mutex_lock(&hero_term_lock);
}

static void hero_term_leave_lock(void) {
    (void)pthread_mutex_unlock(&hero_term_lock);
}

static void hero_term_enter_reading(void) {
    (void)pthread_mutex_lock(&hero_term_reading);
}

static void hero_term_leave_reading(void) {
    (void)pthread_mutex_unlock(&hero_term_reading);
}
#endif

/* The `atexit` function is registered once, under the lock. */
static bool hero_term_exit_set;
static void hero_term_at_exit(void);

/* Registered where it is not yet. Under the lock. A refusal leaves the end
 * of the program unable to put the terminal back, which the header names
 * among what no member can undo; the mode is entered all the same. */
static void hero_term_exit_once(void) {
    if (!hero_term_exit_set) {
        hero_term_exit_set = atexit(hero_term_at_exit) == 0;
    }
}

/* -- finding the terminal ------------------------------------------------- */

#if defined(_WIN32)
/* The console's two handles, opened once. Under the lock. True where they
 * stand. */
static bool hero_term_found(void) {
    if (!hero_term_looked) {
        hero_term_looked = true;
        HANDLE in = CreateFileW(L"CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        HANDLE out = CreateFileW(L"CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        DWORD mode = 0;

        if (in != INVALID_HANDLE_VALUE && out != INVALID_HANDLE_VALUE && GetConsoleMode(in, &mode)) {
            (void)SetHandleInformation(in, HANDLE_FLAG_INHERIT, 0);
            (void)SetHandleInformation(out, HANDLE_FLAG_INHERIT, 0);
            hero_term_in = in;
            hero_term_out = out;
        } else {
            if (in != INVALID_HANDLE_VALUE) {
                (void)CloseHandle(in);
            }
            if (out != INVALID_HANDLE_VALUE) {
                (void)CloseHandle(out);
            }
        }
    }
    return hero_term_in != INVALID_HANDLE_VALUE;
}
#else
/* `/dev/tty`, or the process's input and an output that are terminals, found
 * once. Under the lock. True where an input stands. */
static bool hero_term_found(void) {
    if (!hero_term_looked) {
        hero_term_looked = true;
        int tty = open("/dev/tty", O_RDWR | O_NOCTTY | O_CLOEXEC);

        if (tty >= 0) {
            hero_term_in = tty;
            hero_term_out = tty;
        } else if (isatty(STDIN_FILENO)) {
            hero_term_in = STDIN_FILENO;
            hero_term_out = isatty(STDERR_FILENO) ? STDERR_FILENO : (isatty(STDOUT_FILENO) ? STDOUT_FILENO : -1);
        }
    }
    return hero_term_in >= 0;
}
#endif

/* -- the keys mode ----------------------------------------------------------- */

#if !defined(_WIN32)
/* THE HANDLER: the terminal put back, then the signal sent again, which ends
 * the program as it would have (`SA_RESETHAND` set its default back on
 * entry; held until this returns, or taken at once where the system defers
 * nothing). Only functions safe in a signal. */
static void hero_term_on_signal(int sig) {
    int saved = errno;
    int fd = atomic_load(&hero_term_saved_fd);

    if (fd >= 0) {
        (void)tcsetattr(fd, TCSANOW, &hero_term_saved);
    }
    (void)raise(sig);
    errno = saved;
}

/* Each of the four signals the program left to its default, taken. Under the
 * lock, the saved state published first. */
static void hero_term_take_signals(void) {
    for (int i = 0; i < 4; i += 1) {
        struct sigaction was;

        hero_term_took[i] = false;
        if (sigaction(hero_term_signals[i], NULL, &was) != 0 || (was.sa_flags & SA_SIGINFO) != 0 || was.sa_handler != SIG_DFL) {
            continue;
        }
        struct sigaction now;

        memset(&now, 0, sizeof now);
        now.sa_handler = hero_term_on_signal;
        (void)sigemptyset(&now.sa_mask);
        /* glibc's `SA_RESETHAND` is the unsigned bit 31 and `sa_flags` an
         * `int`: the cast says the bits are kept as they are. */
        now.sa_flags = (int)(SA_RESETHAND | SA_RESTART);
        hero_term_took[i] = sigaction(hero_term_signals[i], &now, NULL) == 0;
    }
}

/* Each signal this member took, given back to its default where it is still
 * this member's. Under the lock. */
static void hero_term_give_signals(void) {
    for (int i = 0; i < 4; i += 1) {
        struct sigaction was;

        if (!hero_term_took[i]) {
            continue;
        }
        hero_term_took[i] = false;
        if (sigaction(hero_term_signals[i], NULL, &was) == 0 && (was.sa_flags & SA_SIGINFO) == 0 && was.sa_handler == hero_term_on_signal) {
            struct sigaction plain;

            memset(&plain, 0, sizeof plain);
            plain.sa_handler = SIG_DFL;
            (void)sigemptyset(&plain.sa_mask);
            (void)sigaction(hero_term_signals[i], &plain, NULL);
        }
    }
}
#endif

int64_t hero_term_enter_keys(void) {
    int64_t kind = HERO_TERM_OK;
    int64_t os = 0;
    int64_t stood = 0;

    hero_term_enter_lock();
    if (!hero_term_found()) {
        kind = HERO_TERM_NOT_A_TERMINAL;
    } else {
#if defined(_WIN32)
        stood = hero_term_keys ? 1 : 0;
        if (!hero_term_keys) {
            DWORD was = 0;

            if (!GetConsoleMode(hero_term_in, &was)) {
                kind = HERO_TERM_OTHER;
                os = (int64_t)GetLastError();
            } else {
                DWORD off = ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT;
                DWORD now = was & ~off;

                hero_term_saved = was;
                hero_term_exit_once();
                /* The console's own sequences where it can, the member's
                 * otherwise (KEYS ON WINDOWS, above). */
                if (!SetConsoleMode(hero_term_in, now | ENABLE_VIRTUAL_TERMINAL_INPUT) && !SetConsoleMode(hero_term_in, now)) {
                    kind = HERO_TERM_OTHER;
                    os = (int64_t)GetLastError();
                } else {
                    hero_term_keys = true;
                }
            }
        }
#else
        stood = atomic_load(&hero_term_saved_fd) >= 0 ? 1 : 0;
        if (stood == 0) {
            struct termios was;

            if (tcgetattr(hero_term_in, &was) != 0) {
                kind = HERO_TERM_OTHER;
                os = (int64_t)errno;
            } else {
                struct termios now = was;

                now.c_lflag &= ~(tcflag_t)(ICANON | ECHO | ECHONL | ISIG | IEXTEN);
                now.c_iflag &= ~(tcflag_t)(IXON | ICRNL | INLCR | IGNCR | ISTRIP);
                now.c_cc[VMIN] = 1;
                now.c_cc[VTIME] = 0;
                /* Saved and published before the handlers are taken and
                 * before the mode, so the end of the program, by any route
                 * this member reaches, finds what to put back. */
                hero_term_saved = was;
                atomic_store(&hero_term_saved_fd, hero_term_in);
                hero_term_exit_once();
                hero_term_take_signals();
                if (tcsetattr(hero_term_in, TCSANOW, &now) != 0) {
                    os = (int64_t)errno;
                    kind = HERO_TERM_OTHER;
                    hero_term_give_signals();
                    atomic_store(&hero_term_saved_fd, -1);
                }
            }
        }
#endif
    }
    hero_term_leave_lock();

    if (kind != HERO_TERM_OK) {
        hero_term_fail(kind, os);
        return -1;
    }
    hero_term_clear();
    return stood;
}

int64_t hero_term_leave(void) {
    int64_t os = 0;
    bool refused = false;

    hero_term_enter_lock();
#if defined(_WIN32)
    if (hero_term_keys) {
        refused = !SetConsoleMode(hero_term_in, hero_term_saved);
        os = refused ? (int64_t)GetLastError() : 0;
        hero_term_keys = false;
    }
#else
    int fd = atomic_load(&hero_term_saved_fd);

    if (fd >= 0) {
        /* Put back first, then the signals given back, then the state
         * forgotten: a signal at any moment between finds the terminal put
         * back or puts it back itself. */
        refused = tcsetattr(fd, TCSANOW, &hero_term_saved) != 0;
        os = refused ? (int64_t)errno : 0;
        hero_term_give_signals();
        atomic_store(&hero_term_saved_fd, -1);
    }
#endif
    hero_term_leave_lock();

    if (refused) {
        hero_term_fail(HERO_TERM_OTHER, os);
        return -1;
    }
    hero_term_clear();
    return 0;
}

/* THE END OF THE PROGRAM: the keys mode left, and on Windows every output
 * console whose processing of sequences this member turned on put back. */
static void hero_term_at_exit(void) {
    (void)hero_term_leave();
#if defined(_WIN32)
    hero_term_enter_lock();
    for (int i = 0; i < hero_term_turned_count; i += 1) {
        (void)SetConsoleMode(hero_term_turned[i], hero_term_turned_mode[i]);
    }
    hero_term_turned_count = 0;
    hero_term_leave_lock();
#endif
}

/* -- reading keys ------------------------------------------------------------- */

/* Milliseconds on a clock that never goes back. */
static int64_t hero_term_now_ms(void) {
#if defined(_WIN32)
    return (int64_t)GetTickCount64();
#else
    struct timespec t;

    if (clock_gettime(CLOCK_MONOTONIC, &t) != 0) {
        return 0;
    }
    return (int64_t)t.tv_sec * 1000 + (int64_t)(t.tv_nsec / 1000000);
#endif
}

/* The time on `hero_term_now_ms`'s clock a wait of `ms` ends at, -1 for no
 * limit: a limit so far that the sum would not fit is none. */
static int64_t hero_term_ends(int64_t ms) {
    int64_t now = hero_term_now_ms();
    int64_t ends = 0;

    if (ms < 0 || __builtin_add_overflow(now, ms, &ends)) {
        return -1;
    }
    return ends;
}

#if defined(_WIN32)
/* `n` appended to what was read ahead, as UTF-8, `times` times, where it
 * fits; the rest of a repeat that does not fit is dropped, as a terminal's
 * own buffer drops it. Under `hero_term_reading`. */
static void hero_term_put(uint32_t n, int times) {
    unsigned char bytes[4];
    size_t len = 0;

    if (n < 0x80u) {
        bytes[0] = (unsigned char)n;
        len = 1;
    } else if (n < 0x800u) {
        bytes[0] = (unsigned char)(0xC0u | (n >> 6));
        bytes[1] = (unsigned char)(0x80u | (n & 0x3Fu));
        len = 2;
    } else if (n < 0x10000u) {
        bytes[0] = (unsigned char)(0xE0u | (n >> 12));
        bytes[1] = (unsigned char)(0x80u | ((n >> 6) & 0x3Fu));
        bytes[2] = (unsigned char)(0x80u | (n & 0x3Fu));
        len = 3;
    } else {
        bytes[0] = (unsigned char)(0xF0u | (n >> 18));
        bytes[1] = (unsigned char)(0x80u | ((n >> 12) & 0x3Fu));
        bytes[2] = (unsigned char)(0x80u | ((n >> 6) & 0x3Fu));
        bytes[3] = (unsigned char)(0x80u | (n & 0x3Fu));
        len = 4;
    }
    for (int i = 0; i < times && hero_term_ahead_count + len <= HERO_TERM_AHEAD; i += 1) {
        memcpy(&hero_term_ahead[hero_term_ahead_count], bytes, len);
        hero_term_ahead_count += len;
    }
}

/* The sequence a POSIX terminal sends for a key with no character, NULL for
 * a key it sends none for (a Shift alone). */
static const char *hero_term_sequence_of(WORD key) {
    switch (key) {
    case VK_UP:
        return "\x1b[A";
    case VK_DOWN:
        return "\x1b[B";
    case VK_RIGHT:
        return "\x1b[C";
    case VK_LEFT:
        return "\x1b[D";
    case VK_HOME:
        return "\x1b[H";
    case VK_END:
        return "\x1b[F";
    case VK_INSERT:
        return "\x1b[2~";
    case VK_DELETE:
        return "\x1b[3~";
    case VK_PRIOR:
        return "\x1b[5~";
    case VK_NEXT:
        return "\x1b[6~";
    case VK_F1:
        return "\x1bOP";
    case VK_F2:
        return "\x1bOQ";
    case VK_F3:
        return "\x1bOR";
    case VK_F4:
        return "\x1bOS";
    case VK_F5:
        return "\x1b[15~";
    case VK_F6:
        return "\x1b[17~";
    case VK_F7:
        return "\x1b[18~";
    case VK_F8:
        return "\x1b[19~";
    case VK_F9:
        return "\x1b[20~";
    case VK_F10:
        return "\x1b[21~";
    case VK_F11:
        return "\x1b[23~";
    case VK_F12:
        return "\x1b[24~";
    default:
        return NULL;
    }
}

/* What the console holds read ahead, waiting at most `ms` (-1 no limit) for
 * an input record: 1 where bytes came, 0 where the time passed with none, -1
 * with the failure set. Under `hero_term_reading`. */
static int hero_term_fill(HANDLE in, int64_t ms) {
    int64_t ends = hero_term_ends(ms);

    for (;;) {
        size_t before = hero_term_ahead_count;
        int64_t left = ends < 0 ? -1 : ends - hero_term_now_ms();

        if (ends >= 0 && left < 0) {
            left = 0;
        }
        DWORD waited = WaitForSingleObject(in, left < 0 ? INFINITE : (DWORD)(left > 0x7FFFFFFF ? 0x7FFFFFFF : left));

        if (waited == WAIT_TIMEOUT) {
            return 0;
        }
        if (waited != WAIT_OBJECT_0) {
            hero_term_fail(HERO_TERM_OTHER, (int64_t)GetLastError());
            return -1;
        }
        INPUT_RECORD records[16];
        DWORD got = 0;

        if (!ReadConsoleInputW(in, records, 16, &got)) {
            hero_term_fail(HERO_TERM_OTHER, (int64_t)GetLastError());
            return -1;
        }
        for (DWORD i = 0; i < got; i += 1) {
            if (records[i].EventType != KEY_EVENT || !records[i].Event.KeyEvent.bKeyDown) {
                continue;
            }
            const KEY_EVENT_RECORD *k = &records[i].Event.KeyEvent;
            int times = k->wRepeatCount > 0 ? (int)k->wRepeatCount : 1;
            WCHAR c = k->uChar.UnicodeChar;

            if (c == 0) {
                const char *sequence = hero_term_sequence_of(k->wVirtualKeyCode);

                for (int r = 0; sequence != NULL && r < times; r += 1) {
                    for (const char *at = sequence; *at != '\0'; at += 1) {
                        hero_term_put((uint32_t)(unsigned char)*at, 1);
                    }
                }
            } else if (c >= 0xD800 && c <= 0xDBFF) {
                hero_term_high = c;
            } else if (c >= 0xDC00 && c <= 0xDFFF) {
                if (hero_term_high != 0) {
                    hero_term_put(0x10000u + (((uint32_t)hero_term_high - 0xD800u) << 10) + ((uint32_t)c - 0xDC00u), times);
                }
                hero_term_high = 0;
            } else {
                hero_term_put((uint32_t)c, times);
            }
        }
        if (hero_term_ahead_count > before) {
            return 1;
        }
        if (ends >= 0 && hero_term_now_ms() >= ends) {
            return 0;
        }
    }
}
#else
/* `hero_term_wait` by `select(2)`, for a descriptor `poll(2)` cannot serve.
 * A set holds descriptors below FD_SETSIZE alone (the page: "The behavior of
 * these macros is undefined if a descriptor value is less than zero or
 * greater than or equal to FD_SETSIZE"), so a higher one is waited on
 * through a copy below it, which reads the same terminal, closed after;
 * where every descriptor below it is taken, -1 with EMFILE. */
static int hero_term_select(int fd, int ms) {
    int low = fd;

    if (fd >= FD_SETSIZE) {
        low = fcntl(fd, F_DUPFD_CLOEXEC, 0);

        if (low < 0) {
            return -1;
        }
        if (low >= FD_SETSIZE) {
            (void)close(low);
            errno = EMFILE;
            return -1;
        }
    }
    fd_set readable;
    struct timeval limit = { 0, 0 };

    FD_ZERO(&readable);
    FD_SET(low, &readable);
    if (ms > 0) {
        limit.tv_sec = (time_t)(ms / 1000);
        limit.tv_usec = (suseconds_t)((ms % 1000) * 1000);
    }
    int n = select(low + 1, &readable, NULL, NULL, ms < 0 ? NULL : &limit);

    if (low != fd) {
        int saved = errno;

        (void)close(low);
        errno = saved;
    }
    return n;
}

/* Whether a byte can be read from `fd`, waiting at most `ms` milliseconds
 * (-1 no limit): 1 where one can or the terminal hung up, which the read
 * then tells; 0 where the time passed; -1 with `errno` set.
 *
 * `poll(2)` first, and `select(2)` where it answers POLLNVAL. Darwin's
 * `poll(2)` says under BUGS: "The poll() system call currently does not
 * support devices." Measured on macOS on 2026-11-19 in a pseudo-terminal:
 * `poll` on the descriptor `/dev/tty` opens answered POLLNVAL at once, and
 * `select` waited its 200 ms, as `poll` did on the pseudo-terminal's own
 * device. Until then a POLLNVAL fell through to a `read(2)` that waited for
 * a key, so a limited read never timed out and an Escape alone was held
 * until the next key. Linux's `poll` serves a terminal, and that arm never
 * reaches `select`. */
static int hero_term_wait(int fd, int ms) {
    struct pollfd p = { fd, POLLIN, 0 };
    int n = poll(&p, 1, ms);

    if (n <= 0 || (p.revents & POLLNVAL) == 0) {
        return n;
    }
    return hero_term_select(fd, ms);
}

/* What the terminal holds read ahead, waiting at most `ms` (-1 no limit) for
 * a byte: 1 where bytes came, 0 where the time passed with none, 2 where the
 * terminal is gone, -1 with the failure set. Under `hero_term_reading`. */
static int hero_term_fill(int fd, int64_t ms) {
    int64_t ends = hero_term_ends(ms);

    for (;;) {
        int64_t left = ends < 0 ? -1 : ends - hero_term_now_ms();

        if (ends >= 0 && left < 0) {
            left = 0;
        }
        int n = hero_term_wait(fd, left < 0 ? -1 : (int)(left > 0x7FFFFFFF ? 0x7FFFFFFF : left));

        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            hero_term_fail(HERO_TERM_OTHER, (int64_t)errno);
            return -1;
        }
        if (n == 0) {
            return 0;
        }
        size_t room = HERO_TERM_AHEAD - hero_term_ahead_count;
        ssize_t got = read(fd, &hero_term_ahead[hero_term_ahead_count], room);

        if (got < 0) {
            if (errno == EINTR || errno == EAGAIN) {
                continue;
            }
            if (errno == EIO) {
                return 2;
            }
            hero_term_fail(HERO_TERM_OTHER, (int64_t)errno);
            return -1;
        }
        if (got == 0) {
            return 2;
        }
        hero_term_ahead_count += (size_t)got;
        return 1;
    }
}
#endif

/* How many of the bytes read ahead the first key takes, or 0 where they are
 * the first part of a key whose rest has not come. */
static size_t hero_term_key_length(const unsigned char *p, size_t n) {
    if (n == 0) {
        return 0;
    }
    unsigned char first = p[0];

    if (first == 0x1B) {
        if (n == 1) {
            return 0;
        }
        if (p[1] == '[') {
            for (size_t i = 2; i < n && i < HERO_TERM_KEY_MOST; i += 1) {
                if (p[i] >= 0x40 && p[i] <= 0x7E) {
                    return i + 1;
                }
                if (p[i] < 0x20 || p[i] > 0x3F) {
                    /* No sequence: the Escape is a key alone. */
                    return 1;
                }
            }
            return n >= HERO_TERM_KEY_MOST ? HERO_TERM_KEY_MOST : 0;
        }
        if (p[1] == 'O') {
            return n >= 3 ? 3 : 0;
        }
        return 1;
    }
    size_t want = first >= 0xF0 && first <= 0xF7 ? 4 : (first >= 0xE0 && first <= 0xEF ? 3 : (first >= 0xC0 && first <= 0xDF ? 2 : 1));

    for (size_t i = 1; i < want; i += 1) {
        if (i >= n) {
            return 0;
        }
        if ((p[i] & 0xC0) != 0x80) {
            /* No character: the lead byte is a key alone. */
            return 1;
        }
    }
    return want;
}

int64_t hero_term_read_key(void *buffer, uint64_t count, int64_t ms) {
    if (buffer == NULL || count < HERO_TERM_KEY_MOST || ms < -1) {
        hero_term_fail(HERO_TERM_INVALID, 0);
        return -1;
    }
    hero_term_enter_lock();
    bool found = hero_term_found();
#if defined(_WIN32)
    HANDLE in = hero_term_in;
#else
    int in = hero_term_in;
#endif
    hero_term_leave_lock();

    if (!found) {
        hero_term_fail(HERO_TERM_NOT_A_TERMINAL, 0);
        return -1;
    }
    int64_t answer = -1;

    hero_term_enter_reading();
    for (;;) {
        size_t length = hero_term_key_length(hero_term_ahead, hero_term_ahead_count);

        if (length > 0) {
            memcpy(buffer, hero_term_ahead, length);
            memmove(hero_term_ahead, &hero_term_ahead[length], hero_term_ahead_count - length);
            hero_term_ahead_count -= length;
            answer = (int64_t)length;
            break;
        }
        /* The first part of a key waits a short time for its rest, and is
         * handed as it is where none comes: Escape alone, or a cut byte. */
        bool part = hero_term_ahead_count > 0;
        int filled = hero_term_fill(in, part ? HERO_TERM_ESCAPE_MS : ms);

        if (filled < 0) {
            break;
        }
        if (filled == 0 && !part) {
            hero_term_fail(HERO_TERM_TIMED_OUT, 0);
            break;
        }
        if (filled != 1 && part) {
            length = hero_term_ahead_count < HERO_TERM_KEY_MOST ? hero_term_ahead_count : HERO_TERM_KEY_MOST;
            length = hero_term_ahead[0] == 0x1B ? 1 : length;
            memcpy(buffer, hero_term_ahead, length);
            memmove(hero_term_ahead, &hero_term_ahead[length], hero_term_ahead_count - length);
            hero_term_ahead_count -= length;
            answer = (int64_t)length;
            break;
        }
        if (filled == 2) {
            answer = 0;
            break;
        }
    }
    hero_term_leave_reading();

    if (answer >= 0) {
        hero_term_clear();
    }
    return answer;
}

/* -- writing to the terminal ----------------------------------------------- */

int64_t hero_term_write(const void *data, uint64_t count) {
    if (data == NULL && count > 0) {
        hero_term_fail(HERO_TERM_INVALID, 0);
        return -1;
    }
    hero_term_enter_lock();
    bool found = hero_term_found();
#if defined(_WIN32)
    HANDLE out = hero_term_out;
#else
    int out = hero_term_out;
#endif
    hero_term_leave_lock();

#if defined(_WIN32)
    if (!found || out == INVALID_HANDLE_VALUE) {
        hero_term_fail(HERO_TERM_NOT_A_TERMINAL, 0);
        return -1;
    }
    const unsigned char *bytes = (const unsigned char *)data;
    uint64_t at = 0;

    while (at < count) {
        /* A piece of at most 1,024 bytes, cut before a character's
         * continuation bytes, so no character is split between two. */
        uint64_t end = count - at > 1024 ? at + 1024 : count;

        while (end < count && end > at && (bytes[end] & 0xC0) == 0x80) {
            end -= 1;
        }
        if (end == at) {
            end = count - at > 1024 ? at + 1024 : count;
        }
        WCHAR wide[1024];
        int units = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (const char *)&bytes[at], (int)(end - at), wide, 1024);

        if (units <= 0) {
            hero_term_fail(HERO_TERM_INVALID, (int64_t)GetLastError());
            return -1;
        }
        int written = 0;

        while (written < units) {
            DWORD wrote = 0;

            if (!WriteConsoleW(out, &wide[written], (DWORD)(units - written), &wrote, NULL)) {
                hero_term_fail(HERO_TERM_OTHER, (int64_t)GetLastError());
                return -1;
            }
            written += (int)wrote;
        }
        at = end;
    }
#else
    if (!found || out < 0) {
        hero_term_fail(HERO_TERM_NOT_A_TERMINAL, 0);
        return -1;
    }
    const unsigned char *bytes = (const unsigned char *)data;
    uint64_t at = 0;

    while (at < count) {
        size_t piece = count - at > 65536 ? 65536 : (size_t)(count - at);
        ssize_t wrote = write(out, &bytes[at], piece);

        if (wrote < 0) {
            if (errno == EINTR) {
                continue;
            }
            hero_term_fail(HERO_TERM_OTHER, (int64_t)errno);
            return -1;
        }
        at += (uint64_t)wrote;
    }
#endif
    hero_term_clear();
    return (int64_t)count;
}

/* -- whether a stream takes sequences ------------------------------------------ */

int64_t hero_term_sequences(hero_stream *s) {
    int64_t held = hero_stream_hold(s);

    if (held < 0) {
        if (hero_stream_last_error() == HERO_STREAM_NOT_OPEN) {
            hero_term_fail(HERO_TERM_NOT_OPEN, 0);
            return -1;
        }
        /* A stream on no descriptor of its own, a connection's: no terminal. */
        hero_term_clear();
        return 0;
    }
    int64_t takes = 0;
#if defined(_WIN32)
    HANDLE h = (HANDLE)(intptr_t)held;
    DWORD mode = 0;
    CONSOLE_SCREEN_BUFFER_INFO info;

    /* An output console alone: an input console's bit of the same value is
     * its echo, and a file or a pipe has no mode. */
    if (GetConsoleScreenBufferInfo(h, &info) && GetConsoleMode(h, &mode)) {
        if ((mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0) {
            takes = 1;
        } else if (SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
            takes = 1;
            hero_term_enter_lock();
            hero_term_exit_once();
            if (hero_term_turned_count < HERO_TERM_TURNED) {
                hero_term_turned[hero_term_turned_count] = h;
                hero_term_turned_mode[hero_term_turned_count] = mode;
                hero_term_turned_count += 1;
            }
            hero_term_leave_lock();
        }
    }
#else
    takes = isatty((int)held) ? 1 : 0;
#endif
    (void)hero_stream_release(s);
    hero_term_clear();
    return takes;
}

int64_t hero_term_last_error(void) {
    return hero_term_kind;
}

int64_t hero_term_last_os_error(void) {
    return hero_term_os;
}
