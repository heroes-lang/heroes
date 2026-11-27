/* Panel 262's R5 (lane land262, step 215b): `n4`'s shape as a library makes
 * it, the ffi-pragmatist's `pt` row `timer` and its kin. The program's C never
 * starts a thread: it hands the function to the platform's own facility, whose
 * worker thread, one the library made, calls it. Darwin: libdispatch's
 * `dispatch_async_f` on a global queue. Windows: the system pool's timer,
 * `CreateThreadpoolTimer`, due in 10 ms (the box's C has no `timer_create`).
 * Elsewhere: POSIX's `timer_create` with `SIGEV_THREAD`, which glibc serves
 * from a helper thread of its own. Spec § 13 says such a callback aborts,
 * saying it ran on a thread this program did not start, so the function MUST
 * NOT RUN. Waits at most five seconds for the call. */
#include <stdint.h>
#if defined(__APPLE__)
#include <dispatch/dispatch.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#else
#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#endif

typedef int64_t (*lib_body)(int64_t);

static lib_body lib_wanted = 0;
static volatile int64_t lib_out = -1;
static volatile int lib_done = 0;

#if defined(__APPLE__)
static void lib_fired(void *unused) {
    (void)unused;
    lib_out = lib_wanted(4);
    lib_done = 1;
}
#elif defined(_WIN32)
static void CALLBACK lib_fired(PTP_CALLBACK_INSTANCE instance, PVOID unused, PTP_TIMER timer) {
    (void)instance;
    (void)unused;
    (void)timer;
    lib_out = lib_wanted(4);
    lib_done = 1;
}
#else
static void lib_fired(union sigval unused) {
    (void)unused;
    lib_out = lib_wanted(4);
    lib_done = 1;
}
#endif

/* `body` handed to the library, which calls it on a worker thread of its
 * own: what it returned, -1 where it was not called within five seconds, -2
 * where the library refused the request. */
static inline int64_t lib_hand(lib_body body) {
    lib_wanted = body;
#if defined(__APPLE__)
    dispatch_async_f(dispatch_get_global_queue(QOS_CLASS_DEFAULT, 0), 0, lib_fired);
    for (int i = 0; i < 5000 && !lib_done; i++) usleep(1000);
#elif defined(_WIN32)
    PTP_TIMER timer = CreateThreadpoolTimer(lib_fired, 0, 0);
    if (timer == 0) return -2;
    ULARGE_INTEGER due;
    due.QuadPart = (ULONGLONG)(-(LONGLONG)(10 * 1000 * 10));
    FILETIME when;
    when.dwLowDateTime = due.LowPart;
    when.dwHighDateTime = due.HighPart;
    SetThreadpoolTimer(timer, &when, 0, 0);
    for (int i = 0; i < 5000 && !lib_done; i++) Sleep(1);
#else
    struct sigevent ev;
    memset(&ev, 0, sizeof ev);
    ev.sigev_notify = SIGEV_THREAD;
    ev.sigev_notify_function = lib_fired;
    timer_t timer;
    if (timer_create(CLOCK_MONOTONIC, &ev, &timer) != 0) return -2;
    struct itimerspec its;
    memset(&its, 0, sizeof its);
    its.it_value.tv_nsec = 10 * 1000 * 1000;
    if (timer_settime(timer, 0, &its, 0) != 0) return -2;
    for (int i = 0; i < 5000 && !lib_done; i++) usleep(1000);
#endif
    return lib_done ? lib_out : -1;
}
