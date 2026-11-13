/* Defect 2492: the program's own C for its case. It starts a thread of its
 * own, with nothing of the Heroes runtime, and calls the function it is handed
 * on that thread, as a library calling back from its own thread does; the
 * platform's own call, because Windows has no pthreads. */
#include <stdint.h>
#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#endif

typedef int64_t (*foreign_body)(int64_t);

static foreign_body foreign_wanted = 0;

#if defined(_WIN32)
static DWORD WINAPI foreign_start(LPVOID unused) {
    (void)unused;
    foreign_wanted(7);
    return 0;
}
#else
static void *foreign_start(void *unused) {
    (void)unused;
    foreign_wanted(7);
    return 0;
}
#endif

/* `body` called once on a thread C starts, waited for: 0 once it returned,
 * -1 where the thread could not be started. */
static inline int64_t foreign_call(foreign_body body) {
    foreign_wanted = body;
#if defined(_WIN32)
    HANDLE thread = CreateThread(NULL, 0, foreign_start, NULL, 0, NULL);
    if (thread == NULL) return -1;
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
#else
    pthread_t thread;
    if (pthread_create(&thread, NULL, foreign_start, NULL) != 0) return -1;
    pthread_join(thread, NULL);
#endif
    return 0;
}
