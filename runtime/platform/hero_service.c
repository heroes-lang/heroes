/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_service.c — the platform layer's service: the Windows Service
 * Control Manager's protocol for a program, behind `hero_service.h`
 * (M-rebel-library, 2026-11-23; spec § 13).
 *
 * THE PROTOCOL, from Microsoft's documentation of the Service Control
 * Manager (`StartServiceCtrlDispatcherW`, `RegisterServiceCtrlHandlerExW`,
 * `SetServiceStatus`, `HandlerEx`): a program the system starts as a service
 * has 30 seconds to call `StartServiceCtrlDispatcherW`, which blocks its
 * caller until every service of the process has stopped and calls the
 * service's `ServiceMain` on a thread the system makes. `ServiceMain`
 * registers the control handler, which the system then calls on the
 * DISPATCHER's thread for each control and which must return at once, and
 * reports the service's status; it returns once the service has stopped.
 *
 * THE SHAPE HERE. Three parties, one lock:
 *
 *   - the program's thread calls `hero_service_start`, which makes the
 *     DISPATCHER thread (this layer's own, never the program's: the program's
 *     `main` keeps its work) and waits for `ServiceMain` to say the handler is
 *     registered, then `hero_service_report`, `hero_service_next` and, last,
 *     `hero_service_end`;
 *   - the dispatcher thread sits in `StartServiceCtrlDispatcherW`, and the
 *     handler it calls puts each control in the queue and rings the bell;
 *   - `ServiceMain`, on the system's thread, registers, reports the first
 *     status, tells the program, and WAITS for `hero_service_end` before it
 *     returns, since a `ServiceMain` that returns is a service that stopped.
 *
 * NO HEROES CODE RUNS ON EITHER OF THE TWO THREADS THE SYSTEM CALLS INTO: the
 * runtime's threads hold their own numbers and handles (panel 254's R3), so
 * the handler and `ServiceMain` write C and nothing else, and the program
 * reaches what they queued through `hero_service_next` on its own thread.
 *
 * NO SYSTEM CALL UNDER THE LOCK THAT CAN WAIT ON THE SYSTEM: the status is
 * reported after the lock is let go, so a handler the system is waiting on
 * never waits behind a report the system is waiting on.
 *
 * EVERYTHING BUT WINDOWS answers `HERO_SERVICE_UNSUPPORTED`. The queue, the
 * bell and the waits are written once for every system, and a build that
 * defines `HERO_SERVICE_SEAM` on POSIX compiles them with two functions that
 * play the system's part (`hero_service_seam_open`, `hero_service_seam_control`),
 * so the sanitizers on a Mac read the code the Windows arm runs; no build of
 * the runtime defines it.
 */

#include "hero_service.h"

#include "hero_layer_alloc.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#if defined(_WIN32)
#define HERO_SERVICE_LIVE 1
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <winsvc.h>
#include <tlhelp32.h>
#if defined(_MSC_VER)
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "advapi32.lib")
#endif
typedef SOCKET hero_service_fd;
#elif defined(HERO_SERVICE_SEAM)
#define HERO_SERVICE_LIVE 1
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
typedef int hero_service_fd;
#endif

static _Thread_local int64_t hero_service_kind;
static _Thread_local int64_t hero_service_os;

static void hero_service_fail(int64_t kind, int64_t os) {
    hero_service_kind = kind;
    hero_service_os = os;
}

int64_t hero_service_last_error(void) {
    return hero_service_kind;
}

int64_t hero_service_last_os_error(void) {
    return hero_service_os;
}

#if !defined(HERO_SERVICE_LIVE)

/* -- a system with no services ---------------------------------------------- */

int64_t hero_service_supported(void) {
    return 0;
}

int64_t hero_service_is_service(void) {
    return 0;
}

int64_t hero_service_start(const char *name, int64_t limit_ms) {
    (void)name;
    (void)limit_ms;
    hero_service_fail(HERO_SERVICE_UNSUPPORTED, 0);
    return -1;
}

int64_t hero_service_arguments(void *buffer, uint64_t n) {
    (void)buffer;
    (void)n;
    hero_service_fail(HERO_SERVICE_UNSUPPORTED, 0);
    return -1;
}

int64_t hero_service_report(int64_t state, int64_t accepts, int64_t exit_code, int64_t wait_hint_ms) {
    (void)state;
    (void)accepts;
    (void)exit_code;
    (void)wait_hint_ms;
    hero_service_fail(HERO_SERVICE_UNSUPPORTED, 0);
    return -1;
}

int64_t hero_service_next(int64_t limit_ns, int64_t *numbers, uint64_t numbers_count) {
    (void)limit_ns;
    (void)numbers;
    (void)numbers_count;
    hero_service_fail(HERO_SERVICE_UNSUPPORTED, 0);
    return -1;
}

int64_t hero_service_count(void) {
    hero_service_fail(HERO_SERVICE_UNSUPPORTED, 0);
    return -1;
}

hero_net_socket *hero_service_bell_socket(void) {
    hero_service_fail(HERO_SERVICE_UNSUPPORTED, 0);
    return NULL;
}

int64_t hero_service_end(int64_t exit_code) {
    (void)exit_code;
    hero_service_fail(HERO_SERVICE_UNSUPPORTED, 0);
    return -1;
}

int64_t hero_service_last_message(void *buffer, uint64_t n) {
    (void)buffer;
    (void)n;
    return 0;
}

#else

/* -- the numbers ------------------------------------------------------------ */

/* A wait asks for the stop between slices of this length: the stop has no
 * hook into this lock's condition, so a wait is as deaf as one slice. */
#define HERO_SERVICE_SLICE_NS INT64_C(50000000)
/* One wait on the condition lasts at most an hour; the limit is then read
 * again, so no system's timeout is asked past what its type holds. */
#define HERO_SERVICE_CHUNK_NS (INT64_C(3600) * INT64_C(1000000000))
/* The wait hint of the first status, in milliseconds: the system's own start
 * limit. */
#define HERO_SERVICE_FIRST_HINT_MS 30000
#define HERO_SERVICE_HINT_MOST 3600000
/* How long `hero_service_end` waits for the dispatcher to return after the
 * service stopped. */
#define HERO_SERVICE_JOIN_MS 10000

/* -- the state -------------------------------------------------------------- */

typedef struct {
    int64_t kind;
    int64_t one;
    int64_t two;
} hero_service_control;

/* The one service of the process, every field under `hero_service_lock`
 * unless its comment says otherwise. */
typedef struct {
    /* `hero_service_start` ran and did not fail cleanly: never again. */
    bool started;
    /* The service stands: ServiceMain registered and `end` did not run. */
    bool open;
    /* Threads inside `hero_service_next`, which `end` waits out. */
    uint64_t waiting;
    hero_service_control queue[HERO_SERVICE_QUEUE];
    uint64_t head;
    uint64_t count;
    int64_t state;
    int64_t accepts;
    uint32_t checkpoint;
    /* The start parameters, UTF-8, each ended by a zero byte. */
    char *args;
    size_t args_len;
    /* The bell, made at the first ask. */
    bool bell;
    bool rung;
    hero_net_socket *bell_socket;
    int64_t bell_hold;
    hero_service_fd bell_fd;
    struct sockaddr_in bell_at;
#if defined(_WIN32)
    SERVICE_STATUS_HANDLE status;
    /* Made by `start` before the dispatcher thread, read by the two threads
     * the system calls into without the lock: a thread's start orders them. */
    HANDLE thread;
    HANDLE ready;
    HANDLE release;
    wchar_t *wide_name;
    /* ServiceMain failed after registering: the kind it could not give. */
    int64_t main_kind;
    bool dispatch_failed;
    DWORD dispatch_error;
    /* A start the system refused, told again to a second ask: the dispatcher
     * is not asked twice in a process, since its second answer is another
     * error than its first (measured on Windows 11: the box). */
    int64_t refused_kind;
    int64_t refused_os;
#endif
} hero_service_state;

#if defined(_WIN32)
static SRWLOCK hero_service_lock = SRWLOCK_INIT;
static CONDITION_VARIABLE hero_service_cond = CONDITION_VARIABLE_INIT;
#else
static pthread_mutex_t hero_service_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t hero_service_cond = PTHREAD_COND_INITIALIZER;
#endif
static hero_service_state hero_service_it;

static void hero_service_enter(void) {
#if defined(_WIN32)
    AcquireSRWLockExclusive(&hero_service_lock);
#else
    (void)pthread_mutex_lock(&hero_service_lock);
#endif
}

static void hero_service_leave(void) {
#if defined(_WIN32)
    ReleaseSRWLockExclusive(&hero_service_lock);
#else
    (void)pthread_mutex_unlock(&hero_service_lock);
#endif
}

static void hero_service_broadcast(void) {
#if defined(_WIN32)
    WakeAllConditionVariable(&hero_service_cond);
#else
    (void)pthread_cond_broadcast(&hero_service_cond);
#endif
}

/* Waits on the condition at most `ns` nanoseconds, under the lock, which it
 * lets go while it waits; a wakeup may come early and the caller asks again. */
static void hero_service_sleep(int64_t ns) {
    if (ns > HERO_SERVICE_CHUNK_NS) {
        ns = HERO_SERVICE_CHUNK_NS;
    }
#if defined(_WIN32)
    int64_t ms = ns / INT64_C(1000000) + (ns % INT64_C(1000000) != 0 ? 1 : 0);

    (void)SleepConditionVariableSRW(&hero_service_cond, &hero_service_lock, (DWORD)ms, 0);
#else
    struct timespec at;

    (void)clock_gettime(CLOCK_REALTIME, &at);
    int64_t nanos = (int64_t)at.tv_nsec + ns % INT64_C(1000000000);

    at.tv_sec += (time_t)(ns / INT64_C(1000000000) + nanos / INT64_C(1000000000));
    at.tv_nsec = (long)(nanos % INT64_C(1000000000));
    (void)pthread_cond_timedwait(&hero_service_cond, &hero_service_lock, &at);
#endif
}

/* Nanoseconds on a clock that never goes back, or -1. */
static int64_t hero_service_clock(void) {
#if defined(_WIN32)
    uint64_t ms = (uint64_t)GetTickCount64();

    if (ms > (uint64_t)(INT64_MAX / INT64_C(1000000))) {
        return -1;
    }
    return (int64_t)ms * INT64_C(1000000);
#else
    struct timespec t;

    if (clock_gettime(CLOCK_MONOTONIC, &t) != 0 || t.tv_sec < 0 || t.tv_sec > INT64_MAX / INT64_C(1000000000) - 1) {
        return -1;
    }
    return (int64_t)t.tv_sec * INT64_C(1000000000) + (int64_t)t.tv_nsec;
#endif
}

/* -- the queue and the bell -------------------------------------------------- */

static bool hero_service_notification(int64_t kind) {
    return kind == HERO_SERVICE_PARAM_CHANGE || kind == HERO_SERVICE_TIME_CHANGE || kind == HERO_SERVICE_POWER || kind == HERO_SERVICE_SESSION ||
           kind == HERO_SERVICE_USER;
}

/* The bell rung where it is made and not rung yet. Under the lock. */
static void hero_service_ring(hero_service_state *st) {
    if (!st->bell || st->rung) {
        return;
    }
    if (sendto(st->bell_fd, "\1", 1, 0, (const struct sockaddr *)&st->bell_at, (int)sizeof st->bell_at) == 1) {
        st->rung = true;
    }
}

/* The bell emptied where it was rung and the queue is empty. Under the lock.
 * Every datagram there goes, a stranger's among them: the socket is the
 * layer's, non-blocking. */
static void hero_service_hush(hero_service_state *st) {
    if (!st->bell || st->count != 0) {
        return;
    }
    char drop[64];

    while (recv(st->bell_fd, drop, (int)sizeof drop, 0) >= 0) {
    }
    st->rung = false;
}

/* The oldest entry of the queue put out. Under the lock. */
static void hero_service_drop_oldest(hero_service_state *st) {
    st->head = (st->head + 1) % HERO_SERVICE_QUEUE;
    st->count -= 1;
}

/* The oldest notification put out, wherever it stands; false where the queue
 * holds none. Under the lock. */
static bool hero_service_drop_notification(hero_service_state *st) {
    for (uint64_t i = 0; i < st->count; i++) {
        uint64_t at = (st->head + i) % HERO_SERVICE_QUEUE;

        if (!hero_service_notification(st->queue[at].kind)) {
            continue;
        }
        for (uint64_t j = i; j + 1 < st->count; j++) {
            st->queue[(st->head + j) % HERO_SERVICE_QUEUE] = st->queue[(st->head + j + 1) % HERO_SERVICE_QUEUE];
        }
        st->count -= 1;
        return true;
    }
    return false;
}

/* Puts one control at the queue's end where the program accepts it (`bit`,
 * 0 for a control that is always told) and answers whether it did accept.
 * A full queue loses a notification before a lifecycle control; the oldest
 * control goes where the queue holds no notification, which the system's
 * one-control-at-a-time protocol never lets happen. Called on the system's
 * thread: it takes the lock and nothing else, and waits for nothing. */
static bool hero_service_deliver(int64_t kind, int64_t one, int64_t two, int64_t bit) {
    hero_service_state *st = &hero_service_it;
    bool accepted = false;

    hero_service_enter();
    if (st->open && (bit == 0 || (st->accepts & bit) != 0)) {
        accepted = true;
        bool room = st->count < HERO_SERVICE_QUEUE;

        if (!room && !hero_service_notification(kind)) {
            if (!hero_service_drop_notification(st)) {
                hero_service_drop_oldest(st);
            }
            room = true;
        }
        if (room) {
            hero_service_control *c = &st->queue[(st->head + st->count) % HERO_SERVICE_QUEUE];

            c->kind = kind;
            c->one = one;
            c->two = two;
            st->count += 1;
            hero_service_ring(st);
            hero_service_broadcast();
        }
    }
    hero_service_leave();
    return accepted;
}

/* The state's name as a bit mask of the controls it lets the system send:
 * none while a state is pending or stopped. */
static int64_t hero_service_allowed(int64_t state, int64_t accepts) {
    if (state == HERO_SERVICE_STATE_RUNNING || state == HERO_SERVICE_STATE_PAUSED) {
        return accepts;
    }
    return 0;
}

#if defined(_WIN32)

/* -- the system's side ------------------------------------------------------- */

/* The handler the system calls for each control, on the dispatcher's thread.
 * A control not accepted is not implemented, as the system's documentation
 * asks; interrogate is answered here and told to nobody. */
static DWORD WINAPI hero_service_handler(DWORD control, DWORD event, LPVOID data, LPVOID context) {
    (void)context;
    switch (control) {
    case SERVICE_CONTROL_INTERROGATE:
        return NO_ERROR;
    case SERVICE_CONTROL_STOP:
        return hero_service_deliver(HERO_SERVICE_STOP, 0, 0, HERO_SERVICE_ACCEPT_STOP) ? NO_ERROR : ERROR_CALL_NOT_IMPLEMENTED;
    case SERVICE_CONTROL_PAUSE:
        return hero_service_deliver(HERO_SERVICE_PAUSE, 0, 0, HERO_SERVICE_ACCEPT_PAUSE_CONTINUE) ? NO_ERROR : ERROR_CALL_NOT_IMPLEMENTED;
    case SERVICE_CONTROL_CONTINUE:
        return hero_service_deliver(HERO_SERVICE_CONTINUE, 0, 0, HERO_SERVICE_ACCEPT_PAUSE_CONTINUE) ? NO_ERROR : ERROR_CALL_NOT_IMPLEMENTED;
    case SERVICE_CONTROL_SHUTDOWN:
        return hero_service_deliver(HERO_SERVICE_SHUTDOWN, 0, 0, HERO_SERVICE_ACCEPT_SHUTDOWN) ? NO_ERROR : ERROR_CALL_NOT_IMPLEMENTED;
    case SERVICE_CONTROL_PRESHUTDOWN:
        return hero_service_deliver(HERO_SERVICE_PRESHUTDOWN, 0, 0, HERO_SERVICE_ACCEPT_PRESHUTDOWN) ? NO_ERROR : ERROR_CALL_NOT_IMPLEMENTED;
    case SERVICE_CONTROL_PARAMCHANGE:
        return hero_service_deliver(HERO_SERVICE_PARAM_CHANGE, 0, 0, HERO_SERVICE_ACCEPT_PARAM_CHANGE) ? NO_ERROR : ERROR_CALL_NOT_IMPLEMENTED;
    case SERVICE_CONTROL_TIMECHANGE:
        return hero_service_deliver(HERO_SERVICE_TIME_CHANGE, 0, 0, HERO_SERVICE_ACCEPT_TIME_CHANGE) ? NO_ERROR : ERROR_CALL_NOT_IMPLEMENTED;
    case SERVICE_CONTROL_POWEREVENT:
        return hero_service_deliver(HERO_SERVICE_POWER, (int64_t)event, 0, HERO_SERVICE_ACCEPT_POWER) ? NO_ERROR : ERROR_CALL_NOT_IMPLEMENTED;
    case SERVICE_CONTROL_SESSIONCHANGE: {
        /* WTSSESSION_NOTIFICATION: a size, then the session's number. */
        const DWORD *note = (const DWORD *)data;
        int64_t session = note != NULL && note[0] >= 2 * sizeof(DWORD) ? (int64_t)note[1] : -1;

        return hero_service_deliver(HERO_SERVICE_SESSION, (int64_t)event, session, HERO_SERVICE_ACCEPT_SESSION) ? NO_ERROR : ERROR_CALL_NOT_IMPLEMENTED;
    }
    default:
        if (control >= 128 && control <= 255) {
            return hero_service_deliver(HERO_SERVICE_USER, (int64_t)control, 0, 0) ? NO_ERROR : ERROR_CALL_NOT_IMPLEMENTED;
        }
        return ERROR_CALL_NOT_IMPLEMENTED;
    }
}

/* The status the system is told, from what the program reports. */
static void hero_service_status(SERVICE_STATUS *s, int64_t state, int64_t accepts, int64_t exit_code, uint32_t checkpoint, int64_t hint_ms) {
    static const DWORD states[] = {0,
                                   SERVICE_STOPPED,
                                   SERVICE_START_PENDING,
                                   SERVICE_STOP_PENDING,
                                   SERVICE_RUNNING,
                                   SERVICE_CONTINUE_PENDING,
                                   SERVICE_PAUSE_PENDING,
                                   SERVICE_PAUSED};
    static const struct {
        int64_t bit;
        DWORD accept;
    } bits[] = {{HERO_SERVICE_ACCEPT_STOP, SERVICE_ACCEPT_STOP},
                {HERO_SERVICE_ACCEPT_PAUSE_CONTINUE, SERVICE_ACCEPT_PAUSE_CONTINUE},
                {HERO_SERVICE_ACCEPT_SHUTDOWN, SERVICE_ACCEPT_SHUTDOWN},
                {HERO_SERVICE_ACCEPT_PRESHUTDOWN, SERVICE_ACCEPT_PRESHUTDOWN},
                {HERO_SERVICE_ACCEPT_PARAM_CHANGE, SERVICE_ACCEPT_PARAMCHANGE},
                {HERO_SERVICE_ACCEPT_TIME_CHANGE, SERVICE_ACCEPT_TIMECHANGE},
                {HERO_SERVICE_ACCEPT_POWER, SERVICE_ACCEPT_POWEREVENT},
                {HERO_SERVICE_ACCEPT_SESSION, SERVICE_ACCEPT_SESSIONCHANGE}};

    memset(s, 0, sizeof *s);
    s->dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    s->dwCurrentState = states[state];
    for (size_t i = 0; i < sizeof bits / sizeof bits[0]; i++) {
        if ((accepts & bits[i].bit) != 0) {
            s->dwControlsAccepted |= bits[i].accept;
        }
    }
    bool pending = state == HERO_SERVICE_STATE_START_PENDING || state == HERO_SERVICE_STATE_STOP_PENDING || state == HERO_SERVICE_STATE_CONTINUE_PENDING ||
                   state == HERO_SERVICE_STATE_PAUSE_PENDING;

    s->dwCheckPoint = pending ? (DWORD)checkpoint : 0;
    s->dwWaitHint = pending ? (DWORD)hint_ms : 0;
    if (state == HERO_SERVICE_STATE_STOPPED && exit_code != 0) {
        s->dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
        s->dwServiceSpecificExitCode = (DWORD)exit_code;
    }
}

/* UTF-8 text as a zero-ended wide string the caller frees with
 * `hero_layer_free`; NULL where the text is not UTF-8 or no memory is left,
 * `*invalid` telling which. */
static wchar_t *hero_service_wide(const char *text, bool *invalid) {
    *invalid = false;
    int need = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, NULL, 0);

    if (need <= 0) {
        *invalid = true;
        return NULL;
    }
    wchar_t *wide = (wchar_t *)hero_layer_alloc((size_t)need * sizeof(wchar_t));

    if (wide == NULL) {
        return NULL;
    }
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, wide, need) != need) {
        hero_layer_free(wide);
        *invalid = true;
        return NULL;
    }
    return wide;
}

/* The start parameters as UTF-8 each ended by a zero byte, in a block the
 * caller frees; NULL where no memory is left. `*length` is its size. */
static char *hero_service_collect(DWORD argc, LPWSTR *argv, size_t *length) {
    size_t total = 0;

    for (DWORD i = 0; i < argc; i++) {
        int bytes = WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, NULL, 0, NULL, NULL);

        if (bytes <= 0) {
            return NULL;
        }
        total += (size_t)bytes;
    }
    char *all = (char *)hero_layer_alloc(total == 0 ? 1 : total);
    size_t at = 0;

    if (all == NULL) {
        return NULL;
    }
    for (DWORD i = 0; i < argc; i++) {
        int bytes = WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, all + at, (int)(total - at), NULL, NULL);

        if (bytes <= 0) {
            hero_layer_free(all);
            return NULL;
        }
        at += (size_t)bytes;
    }
    *length = total;
    return all;
}

/* ServiceMain, on the thread the system makes for it. */
static VOID WINAPI hero_service_main(DWORD argc, LPWSTR *argv) {
    hero_service_state *st = &hero_service_it;
    SERVICE_STATUS_HANDLE handle = RegisterServiceCtrlHandlerExW(st->wide_name, hero_service_handler, NULL);

    if (handle == NULL) {
        hero_service_enter();
        st->main_kind = HERO_SERVICE_OTHER;
        hero_service_leave();
        (void)SetEvent(st->ready);
        return;
    }
    size_t length = 0;
    char *args = hero_service_collect(argc, argv, &length);
    SERVICE_STATUS first;

    if (args == NULL) {
        /* Stopped at once, with the system's own words for no memory. */
        hero_service_status(&first, HERO_SERVICE_STATE_STOPPED, 0, 0, 0, 0);
        first.dwWin32ExitCode = ERROR_NOT_ENOUGH_MEMORY;
        (void)SetServiceStatus(handle, &first);
        hero_service_enter();
        st->main_kind = HERO_SERVICE_LIMIT;
        hero_service_leave();
        (void)SetEvent(st->ready);
        return;
    }
    hero_service_status(&first, HERO_SERVICE_STATE_START_PENDING, 0, 0, 1, HERO_SERVICE_FIRST_HINT_MS);
    (void)SetServiceStatus(handle, &first);
    hero_service_enter();
    st->status = handle;
    st->args = args;
    st->args_len = length;
    st->state = HERO_SERVICE_STATE_START_PENDING;
    st->accepts = 0;
    st->checkpoint = 1;
    st->open = true;
    hero_service_leave();
    (void)SetEvent(st->ready);
    /* A ServiceMain that returns is a service that stopped: `end` lets it. */
    (void)WaitForSingleObject(st->release, INFINITE);
}

/* The dispatcher's thread: blocked in the system's dispatcher until every
 * service of the process has stopped, or until it refuses. */
static DWORD WINAPI hero_service_dispatch(LPVOID unused) {
    (void)unused;
    hero_service_state *st = &hero_service_it;
    SERVICE_TABLE_ENTRYW table[2];

    table[0].lpServiceName = st->wide_name;
    table[0].lpServiceProc = hero_service_main;
    table[1].lpServiceName = NULL;
    table[1].lpServiceProc = NULL;
    BOOL ok = StartServiceCtrlDispatcherW(table);
    DWORD error = ok ? 0 : GetLastError();

    if (!ok) {
        hero_service_enter();
        st->dispatch_failed = true;
        st->dispatch_error = error;
        hero_service_leave();
        (void)SetEvent(st->ready);
    }
    return 0;
}

int64_t hero_service_supported(void) {
    return 1;
}

int64_t hero_service_is_service(void) {
    DWORD own = GetCurrentProcessId();
    DWORD session = 0;
    DWORD parent = 0;
    bool found = false;

    if (!ProcessIdToSessionId(own, &session) || session != 0) {
        return 0;
    }
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snap == INVALID_HANDLE_VALUE) {
        return 0;
    }
    PROCESSENTRY32W e;

    memset(&e, 0, sizeof e);
    e.dwSize = sizeof e;
    for (BOOL more = Process32FirstW(snap, &e); more; more = Process32NextW(snap, &e)) {
        if (e.th32ProcessID == own) {
            parent = e.th32ParentProcessID;
            found = true;
            break;
        }
    }
    bool services = false;

    if (found) {
        memset(&e, 0, sizeof e);
        e.dwSize = sizeof e;
        for (BOOL more = Process32FirstW(snap, &e); more; more = Process32NextW(snap, &e)) {
            if (e.th32ProcessID == parent) {
                services = _wcsicmp(e.szExeFile, L"services.exe") == 0;
                break;
            }
        }
    }
    (void)CloseHandle(snap);
    return services ? 1 : 0;
}

/* The state ended where `start` could not finish: handles and name given
 * back, so a program that was run from a console may ask again. */
static void hero_service_unmake(hero_service_state *st) {
    if (st->thread != NULL) {
        (void)CloseHandle(st->thread);
    }
    if (st->ready != NULL) {
        (void)CloseHandle(st->ready);
    }
    if (st->release != NULL) {
        (void)CloseHandle(st->release);
    }
    hero_layer_free(st->wide_name);
    st->thread = NULL;
    st->ready = NULL;
    st->release = NULL;
    st->wide_name = NULL;
}

int64_t hero_service_start(const char *name, int64_t limit_ms) {
    hero_service_state *st = &hero_service_it;

    if (name == NULL || limit_ms < -1) {
        hero_service_fail(HERO_SERVICE_INVALID, 0);
        return -1;
    }
    size_t length = strlen(name);

    if (length == 0 || length > HERO_SERVICE_NAME_MOST || strchr(name, '/') != NULL || strchr(name, '\\') != NULL) {
        hero_service_fail(HERO_SERVICE_INVALID, 0);
        return -1;
    }
    bool invalid = false;
    wchar_t *wide = hero_service_wide(name, &invalid);

    if (wide == NULL) {
        hero_service_fail(invalid ? HERO_SERVICE_INVALID : HERO_SERVICE_LIMIT, 0);
        return -1;
    }
    hero_service_enter();
    if (st->started) {
        int64_t kind = st->refused_kind != 0 ? st->refused_kind : HERO_SERVICE_EXISTS;
        int64_t os = st->refused_kind != 0 ? st->refused_os : 0;

        hero_service_leave();
        hero_layer_free(wide);
        hero_service_fail(kind, os);
        return -1;
    }
    st->started = true;
    st->wide_name = wide;
    st->main_kind = 0;
    st->dispatch_failed = false;
    st->dispatch_error = 0;
    hero_service_leave();
    st->ready = CreateEventW(NULL, TRUE, FALSE, NULL);
    st->release = CreateEventW(NULL, TRUE, FALSE, NULL);
    if (st->ready != NULL && st->release != NULL) {
        st->thread = CreateThread(NULL, 0, hero_service_dispatch, NULL, 0, NULL);
    }
    if (st->thread == NULL) {
        DWORD error = GetLastError();

        hero_service_unmake(st);
        hero_service_enter();
        st->started = false;
        hero_service_leave();
        hero_service_fail(HERO_SERVICE_LIMIT, (int64_t)error);
        return -1;
    }
    DWORD wait = limit_ms < 0 || limit_ms >= (int64_t)INFINITE ? INFINITE : (DWORD)limit_ms;
    DWORD got = WaitForSingleObject(st->ready, wait);

    if (got != WAIT_OBJECT_0) {
        /* The dispatcher cannot be called back: it stays, and the start is
         * spent, so the program does not ask a second dispatcher of it. */
        hero_service_fail(got == WAIT_TIMEOUT ? HERO_SERVICE_TIMEOUT : HERO_SERVICE_OTHER, got == WAIT_TIMEOUT ? 0 : (int64_t)GetLastError());
        return -1;
    }
    hero_service_enter();
    bool failed = st->dispatch_failed;
    DWORD error = st->dispatch_error;
    int64_t main_kind = st->main_kind;
    bool open = st->open;

    hero_service_leave();
    if (open && main_kind == 0 && !failed) {
        hero_service_fail(HERO_SERVICE_OK, 0);
        return 0;
    }
    /* A dispatcher that refused has returned, and a ServiceMain that gave up
     * ends it: where the thread did end, its handles and name are given back.
     * Where it did not, they are the system's still, and stay. Either way the
     * start is spent and a second ask is told this answer again. */
    if (WaitForSingleObject(st->thread, HERO_SERVICE_JOIN_MS) == WAIT_OBJECT_0) {
        hero_service_unmake(st);
    }
    int64_t kind = HERO_SERVICE_OTHER;
    int64_t os = 0;

    if (failed) {
        kind = error == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT ? HERO_SERVICE_NOT_SERVICE : HERO_SERVICE_OTHER;
        os = (int64_t)error;
    } else if (main_kind != 0) {
        kind = main_kind;
    }
    hero_service_enter();
    st->refused_kind = kind;
    st->refused_os = os;
    hero_service_leave();
    hero_service_fail(kind, os);
    return -1;
}

/* Reports `status` to the system after the lock was let go. */
static int64_t hero_service_send(SERVICE_STATUS_HANDLE handle, SERVICE_STATUS *status) {
    if (!SetServiceStatus(handle, status)) {
        hero_service_fail(HERO_SERVICE_DENIED, (int64_t)GetLastError());
        return -1;
    }
    hero_service_fail(HERO_SERVICE_OK, 0);
    return 0;
}

int64_t hero_service_report(int64_t state, int64_t accepts, int64_t exit_code, int64_t wait_hint_ms) {
    hero_service_state *st = &hero_service_it;

    if (state < HERO_SERVICE_STATE_STOPPED || state > HERO_SERVICE_STATE_PAUSED || accepts < 0 || accepts > HERO_SERVICE_ACCEPT_ALL || exit_code < 0 ||
        exit_code > (int64_t)UINT32_MAX || wait_hint_ms < 0 || wait_hint_ms > HERO_SERVICE_HINT_MOST) {
        hero_service_fail(HERO_SERVICE_INVALID, 0);
        return -1;
    }
    SERVICE_STATUS status;
    SERVICE_STATUS_HANDLE handle;

    hero_service_enter();
    if (!st->open) {
        hero_service_leave();
        hero_service_fail(HERO_SERVICE_NOT_OPEN, 0);
        return -1;
    }
    bool pending = state == HERO_SERVICE_STATE_START_PENDING || state == HERO_SERVICE_STATE_STOP_PENDING || state == HERO_SERVICE_STATE_CONTINUE_PENDING ||
                   state == HERO_SERVICE_STATE_PAUSE_PENDING;

    st->checkpoint = pending ? st->checkpoint + 1 : 0;
    st->state = state;
    st->accepts = hero_service_allowed(state, accepts);
    handle = st->status;
    hero_service_status(&status, state, st->accepts, exit_code, st->checkpoint, wait_hint_ms);
    hero_service_leave();
    return hero_service_send(handle, &status);
}

int64_t hero_service_end(int64_t exit_code) {
    hero_service_state *st = &hero_service_it;

    if (exit_code < 0 || exit_code > (int64_t)UINT32_MAX) {
        hero_service_fail(HERO_SERVICE_INVALID, 0);
        return -1;
    }
    SERVICE_STATUS status;
    SERVICE_STATUS_HANDLE handle;

    hero_service_enter();
    if (!st->open) {
        hero_service_leave();
        hero_service_fail(HERO_SERVICE_NOT_OPEN, 0);
        return -1;
    }
    bool stopped = st->state == HERO_SERVICE_STATE_STOPPED;

    st->open = false;
    st->accepts = 0;
    st->state = HERO_SERVICE_STATE_STOPPED;
    handle = st->status;
    hero_service_status(&status, HERO_SERVICE_STATE_STOPPED, 0, exit_code, 0, 0);
    hero_service_broadcast();
    while (st->waiting > 0) {
        hero_service_sleep(HERO_SERVICE_SLICE_NS);
    }
    hero_service_leave();
    /* A service that reported stopped already is not told twice: the system
     * may have let go of its handle. */
    int64_t answer = stopped ? 0 : hero_service_send(handle, &status);
    int64_t kind = hero_service_kind;
    int64_t os = hero_service_os;

    (void)SetEvent(st->release);
    bool joined = WaitForSingleObject(st->thread, HERO_SERVICE_JOIN_MS) == WAIT_OBJECT_0;

    hero_service_enter();
    if (st->bell) {
        hero_net_unhold(st->bell_hold);
        (void)hero_net_close(st->bell_socket);
        st->bell = false;
        st->rung = false;
        st->bell_socket = NULL;
    }
    hero_layer_free(st->args);
    st->args = NULL;
    st->args_len = 0;
    st->count = 0;
    st->head = 0;
    hero_service_leave();
    /* A dispatcher still in the system's hands keeps its handles and name. */
    if (joined) {
        hero_service_unmake(st);
    }
    if (answer < 0) {
        hero_service_fail(kind, os);
        return -1;
    }
    hero_service_fail(HERO_SERVICE_OK, 0);
    return 0;
}

int64_t hero_service_last_message(void *buffer, uint64_t n) {
    int64_t os = hero_service_os;
    wchar_t wide[512];

    if (os <= 0 || os > (int64_t)UINT32_MAX) {
        return 0;
    }
    DWORD got = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, (DWORD)os, 0, wide, (DWORD)(sizeof wide / sizeof wide[0]), NULL);

    while (got > 0 && (wide[got - 1] == L'\r' || wide[got - 1] == L'\n' || wide[got - 1] == L' ')) {
        got -= 1;
    }
    if (got == 0) {
        return 0;
    }
    int bytes = WideCharToMultiByte(CP_UTF8, 0, wide, (int)got, NULL, 0, NULL, NULL);

    if (bytes <= 0) {
        return 0;
    }
    if (buffer != NULL && n >= (uint64_t)bytes) {
        (void)WideCharToMultiByte(CP_UTF8, 0, wide, (int)got, (char *)buffer, (int)bytes, NULL, NULL);
    }
    return bytes;
}

#else /* HERO_SERVICE_SEAM on POSIX */

/* -- the seam: the system's part, played by a test ---------------------------- */

int64_t hero_service_supported(void) {
    return 1;
}

int64_t hero_service_is_service(void) {
    return 0;
}

int64_t hero_service_start(const char *name, int64_t limit_ms) {
    (void)name;
    (void)limit_ms;
    hero_service_fail(HERO_SERVICE_UNSUPPORTED, 0);
    return -1;
}

/* A service stands, running and accepting every control, as if the system
 * had called `ServiceMain` with the parameters `a` and `b`. */
int64_t hero_service_seam_open(const char *a, const char *b) {
    hero_service_state *st = &hero_service_it;
    size_t la = strlen(a) + 1;
    size_t lb = strlen(b) + 1;
    char *args = (char *)hero_layer_alloc(la + lb);

    memcpy(args, a, la);
    memcpy(args + la, b, lb);
    hero_service_enter();
    if (st->open) {
        hero_service_leave();
        hero_layer_free(args);
        hero_service_fail(HERO_SERVICE_EXISTS, 0);
        return -1;
    }
    st->started = true;
    st->open = true;
    st->args = args;
    st->args_len = la + lb;
    st->state = HERO_SERVICE_STATE_RUNNING;
    st->accepts = HERO_SERVICE_ACCEPT_ALL;
    hero_service_leave();
    return 0;
}

/* The system's handler called with one control. Answers whether it was
 * accepted. */
int64_t hero_service_seam_control(int64_t kind, int64_t one, int64_t two, int64_t bit) {
    return hero_service_deliver(kind, one, two, bit) ? 1 : 0;
}

int64_t hero_service_report(int64_t state, int64_t accepts, int64_t exit_code, int64_t wait_hint_ms) {
    hero_service_state *st = &hero_service_it;

    if (state < HERO_SERVICE_STATE_STOPPED || state > HERO_SERVICE_STATE_PAUSED || accepts < 0 || accepts > HERO_SERVICE_ACCEPT_ALL || exit_code < 0 ||
        exit_code > (int64_t)UINT32_MAX || wait_hint_ms < 0 || wait_hint_ms > HERO_SERVICE_HINT_MOST) {
        hero_service_fail(HERO_SERVICE_INVALID, 0);
        return -1;
    }
    hero_service_enter();
    if (!st->open) {
        hero_service_leave();
        hero_service_fail(HERO_SERVICE_NOT_OPEN, 0);
        return -1;
    }
    st->state = state;
    st->accepts = hero_service_allowed(state, accepts);
    hero_service_leave();
    hero_service_fail(HERO_SERVICE_OK, 0);
    return 0;
}

int64_t hero_service_end(int64_t exit_code) {
    hero_service_state *st = &hero_service_it;

    if (exit_code < 0 || exit_code > (int64_t)UINT32_MAX) {
        hero_service_fail(HERO_SERVICE_INVALID, 0);
        return -1;
    }
    hero_service_enter();
    if (!st->open) {
        hero_service_leave();
        hero_service_fail(HERO_SERVICE_NOT_OPEN, 0);
        return -1;
    }
    st->open = false;
    st->accepts = 0;
    hero_service_broadcast();
    while (st->waiting > 0) {
        hero_service_sleep(HERO_SERVICE_SLICE_NS);
    }
    if (st->bell) {
        hero_net_unhold(st->bell_hold);
        (void)hero_net_close(st->bell_socket);
        st->bell = false;
        st->rung = false;
        st->bell_socket = NULL;
    }
    hero_layer_free(st->args);
    st->args = NULL;
    st->args_len = 0;
    st->count = 0;
    st->head = 0;
    hero_service_leave();
    hero_service_fail(HERO_SERVICE_OK, 0);
    return 0;
}

/* The seam lets a test open again where the system would not: the state of
 * a service that ended, made fresh. */
void hero_service_seam_reset(void) {
    hero_service_enter();
    hero_service_it.started = false;
    hero_service_leave();
}

int64_t hero_service_last_message(void *buffer, uint64_t n) {
    (void)buffer;
    (void)n;
    return 0;
}

#endif /* _WIN32 */

/* -- the program's side, the same on both ------------------------------------- */

int64_t hero_service_arguments(void *buffer, uint64_t n) {
    hero_service_state *st = &hero_service_it;

    if (buffer == NULL && n > 0) {
        hero_service_fail(HERO_SERVICE_INVALID, 0);
        return -1;
    }
    hero_service_enter();
    if (!st->open) {
        hero_service_leave();
        hero_service_fail(HERO_SERVICE_NOT_OPEN, 0);
        return -1;
    }
    int64_t length = (int64_t)st->args_len;

    if (n >= (uint64_t)st->args_len && st->args_len > 0) {
        memcpy(buffer, st->args, st->args_len);
    }
    hero_service_leave();
    hero_service_fail(HERO_SERVICE_OK, 0);
    return length;
}

int64_t hero_service_next(int64_t limit_ns, int64_t *numbers, uint64_t numbers_count) {
    hero_service_state *st = &hero_service_it;

    if (limit_ns < -1 || numbers == NULL || numbers_count < HERO_SERVICE_NUMBERS) {
        hero_service_fail(HERO_SERVICE_INVALID, 0);
        return -1;
    }
    int64_t started = limit_ns > 0 ? hero_service_clock() : 0;

    if (limit_ns > 0 && started < 0) {
        hero_service_fail(HERO_SERVICE_OTHER, 0);
        return -1;
    }
    int64_t ends = limit_ns < 0 ? INT64_MAX : (started > INT64_MAX - limit_ns ? INT64_MAX : started + limit_ns);
    int64_t kind = HERO_SERVICE_OK;
    int64_t answer = 0;

    hero_service_enter();
    if (!st->open) {
        hero_service_leave();
        hero_service_fail(HERO_SERVICE_NOT_OPEN, 0);
        return -1;
    }
    st->waiting += 1;
    for (;;) {
        if (!st->open) {
            kind = HERO_SERVICE_NOT_OPEN;
            answer = -1;
            break;
        }
        if (st->count > 0) {
            hero_service_control *c = &st->queue[st->head];

            numbers[0] = c->kind;
            numbers[1] = c->one;
            numbers[2] = c->two;
            hero_service_drop_oldest(st);
            hero_service_hush(st);
            answer = 1;
            break;
        }
        /* Nothing waits: a stray datagram's ring goes with the answer. */
        hero_service_hush(st);
        int64_t at = limit_ns == 0 ? 0 : hero_service_clock();

        if (limit_ns == 0 || (ends != INT64_MAX && (at < 0 || at >= ends))) {
            answer = 0;
            break;
        }
        int64_t span = ends == INT64_MAX ? HERO_SERVICE_CHUNK_NS : ends - at;

        if (span > HERO_SERVICE_SLICE_NS) {
            span = HERO_SERVICE_SLICE_NS;
        }
        hero_service_sleep(span);
        /* The stop is asked between slices, never under the lock; what was
         * queued is told before it. */
        hero_service_leave();
        bool asked = hero_net_stop_asked() != 0;

        hero_service_enter();
        if (asked && st->open && st->count == 0) {
            kind = HERO_SERVICE_STOPPED;
            answer = -1;
            break;
        }
    }
    st->waiting -= 1;
    if (st->waiting == 0) {
        hero_service_broadcast();
    }
    hero_service_leave();
    if (answer < 0) {
        hero_service_fail(kind, 0);
        return -1;
    }
    hero_service_fail(HERO_SERVICE_OK, 0);
    return answer;
}

int64_t hero_service_count(void) {
    hero_service_state *st = &hero_service_it;

    hero_service_enter();
    int64_t count = st->open ? (int64_t)st->count : -1;

    hero_service_leave();
    if (count < 0) {
        hero_service_fail(HERO_SERVICE_NOT_OPEN, 0);
        return -1;
    }
    hero_service_fail(HERO_SERVICE_OK, 0);
    return count;
}

hero_net_socket *hero_service_bell_socket(void) {
    hero_service_state *st = &hero_service_it;

    hero_service_enter();
    bool open = st->open;
    hero_net_socket *made = open && st->bell ? st->bell_socket : NULL;

    hero_service_leave();
    if (!open) {
        hero_service_fail(HERO_SERVICE_NOT_OPEN, 0);
        return NULL;
    }
    if (made != NULL) {
        hero_service_fail(HERO_SERVICE_OK, 0);
        return made;
    }
    hero_net_socket *s = hero_net_udp("127.0.0.1", 0);

    if (s == NULL) {
        hero_service_fail(HERO_SERVICE_LIMIT, hero_net_last_os_error());
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
        hero_service_fail(HERO_SERVICE_LIMIT, 0);
        return NULL;
    }
    hero_service_enter();
    bool kept = st->open && !st->bell;

    if (kept) {
        st->bell_socket = s;
        st->bell_hold = hold;
        st->bell_fd = (hero_service_fd)(uint64_t)fd;
        memset(&st->bell_at, 0, sizeof st->bell_at);
        st->bell_at.sin_family = AF_INET;
        st->bell_at.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        st->bell_at.sin_port = htons((unsigned short)port);
        st->bell = true;
        if (st->count > 0) {
            hero_service_ring(st);
        }
    }
    made = st->open ? st->bell_socket : NULL;
    hero_service_leave();
    if (!kept) {
        hero_net_unhold(hold);
        (void)hero_net_close(s);
    }
    if (made == NULL) {
        hero_service_fail(HERO_SERVICE_NOT_OPEN, 0);
        return NULL;
    }
    hero_service_fail(HERO_SERVICE_OK, 0);
    return made;
}

#endif /* HERO_SERVICE_LIVE */
