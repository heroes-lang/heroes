/* alarm.h: a watchdog whose SIGALRM handler siglongjmps out of the program's function at
 * whatever instruction the timer finds it, as a real timeout does (not raise()). */
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
typedef int64_t (*wa_cb_t)(int64_t);
static sigjmp_buf wa_env;
static volatile sig_atomic_t wa_armed;
static void wa_handler(int sig) { (void)sig; if (wa_armed) { wa_armed = 0; siglongjmp(wa_env, 1); } }
static inline int64_t wa_run(wa_cb_t work, int64_t usec) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = wa_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, NULL) != 0) return -2;
    if (sigsetjmp(wa_env, 1) == 0) {
        struct itimerval t;
        memset(&t, 0, sizeof t);
        t.it_value.tv_usec = (int)usec;
        wa_armed = 1;
        setitimer(ITIMER_REAL, &t, NULL);
        (void)work(0);
        wa_armed = 0;
        return -1;
    }
    return 1;
}
