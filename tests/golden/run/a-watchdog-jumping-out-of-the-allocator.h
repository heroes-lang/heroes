/* Panel 262's R5 (lane land262, step 215a): a `run` golden of panel 258's
 * stop, the abort spec § 13 promises at the jump where C jumps out of a
 * signal handler whose signal interrupted this runtime's allocator and this
 * runtime saw the handler begin and the jump. Defect 2873's watchdog
 * (`fixedbugs-2873-a-watchdog-installed-by-a-raw-system-call.h`) with its
 * handler installed by `sigaction`, so the runtime sees it begin: the alarm
 * repeats every 200 microseconds and each one `siglongjmp`s out of whatever
 * it interrupted, a loop of C calling a Heroes function that only allocates,
 * back to the loop's head. A jump that leaves code outside the allocator ends
 * the handler and the loop goes on; the first that leaves the allocator
 * itself MUST STOP the program at the jump, in panel 258's words. The timer
 * cannot be aimed at the allocator's window, and the runtime offers no raise
 * inside it, so the alarm repeats until one lands there. */
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
typedef int64_t (*wds_cb_t)(int64_t);
static sigjmp_buf wds_env;
static void wds_alarm(int sig) {
    (void)sig;
    siglongjmp(wds_env, 1);
}
/* Calls cb until stopped; returns -1 where a million alarms jumped and none
 * landed inside the allocator. */
static inline int64_t wds_run(wds_cb_t cb) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = wds_alarm;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, NULL) != 0) return -2;
    static volatile int64_t jumps = 0;
    struct itimerval t;
    memset(&t, 0, sizeof t);
    t.it_value.tv_usec = 200;
    t.it_interval.tv_usec = 200;
    sigsetjmp(wds_env, 1);
    jumps = jumps + 1;
    if (jumps == 1 && setitimer(ITIMER_REAL, &t, NULL) != 0) return -3;
    if (jumps > 1000000) {
        memset(&t, 0, sizeof t);
        setitimer(ITIMER_REAL, &t, NULL);
        return -1;
    }
    for (;;) cb(jumps);
}
