/* Defect 2494: the program's own C for its case. Its handler is C, asks for
 * the thread's alternate signal stack (`SA_ONSTACK`) and makes none of its
 * own, so it runs on the one the runtime's stack guard gave the thread, and
 * calls the Heroes function it was handed for a result. */
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

static int64_t (*given_answer)(int64_t) = NULL;
static volatile sig_atomic_t given_got = 0;

static void given_handler(int sig) { given_got = (sig_atomic_t)given_answer((int64_t)sig); }

static inline int32_t given_install(int32_t sig, int64_t (*answer)(int64_t)) {
    struct sigaction sa;
    given_answer = answer;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = given_handler;
    sa.sa_flags = SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL);
}

static inline int32_t given_raise(int32_t sig) { return raise(sig); }

static inline int64_t given_seen(void) { return (int64_t)given_got; }
