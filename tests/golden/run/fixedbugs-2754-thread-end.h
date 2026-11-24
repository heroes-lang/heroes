/* fixedbugs-2754-thread-end.h: a C function that ends the thread calling it,
 * the system's way, before the function that called it returns (panel 258's
 * R1, defect 2754); and a nap. */
#include <stdint.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <time.h>
#endif

static inline void te_nap(int64_t ms) {
#if defined(_WIN32)
    Sleep((DWORD)ms);
#else
    struct timespec left = { (time_t)(ms / 1000), (long)((ms % 1000) * 1000000) };

    while (nanosleep(&left, &left) != 0) {
    }
#endif
}

/* Naps `ms`, then ends this thread: `pthread_exit` on POSIX, `ExitThread` on
 * Windows. */
static inline void te_end_after(int64_t ms) {
    te_nap(ms);
#if defined(_WIN32)
    ExitThread(0);
#else
    pthread_exit(NULL);
#endif
}
