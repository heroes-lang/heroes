/* Defect 2873 (lane sig-family, step 199n): panel 262's completeness critic's
 * `wdr.h`, the watchdog of the compiler-engineer's `wd.h` with its handler
 * installed by a raw system call (`fixedbugs-2522-a-raw-install.h`), so no
 * route of the runtime sees the handler begin. Here the alarm repeats, every
 * 200 microseconds, and each `siglongjmp`s out of whatever it interrupted, a
 * loop of C calling a Heroes function that only allocates, back to the loop's
 * head; sooner or later one lands while that function is inside this
 * runtime's allocator, the loop's next allocation finds the allocation never
 * finished, and the program MUST STOP, in words naming the jump as well as a
 * handler's own use: the program gives C no Heroes function as a handler. */
#if defined(__APPLE__)
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "fixedbugs-2522-a-raw-install.h"
typedef int64_t (*wdj_cb_t)(int64_t);
static sigjmp_buf wdj_env;
static void wdj_alarm(int sig) {
    (void)sig;
    siglongjmp(wdj_env, 1);
}
/* Calls cb until stopped; returns -1 where a million alarms jumped and none
 * landed inside the allocator. */
static inline int64_t wdj_run(wdj_cb_t cb) {
    if (raw_install(SIGALRM, wdj_alarm) != 0) return -2;
    static volatile int64_t jumps = 0;
    struct itimerval t;
    memset(&t, 0, sizeof t);
    t.it_value.tv_usec = 200;
    t.it_interval.tv_usec = 200;
    sigsetjmp(wdj_env, 1);
    jumps = jumps + 1;
    if (jumps == 1 && setitimer(ITIMER_REAL, &t, NULL) != 0) return -3;
    if (jumps > 1000000) {
        memset(&t, 0, sizeof t);
        setitimer(ITIMER_REAL, &t, NULL);
        return -1;
    }
    for (;;) cb(jumps);
}
